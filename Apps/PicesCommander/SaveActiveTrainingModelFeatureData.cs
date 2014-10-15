using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using PicesInterface;
//**********************************************************************************
//* Developer:  Kurt Kramer                                                        *
//* Date        2009-05-23                                                         *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//**********************************************************************************

namespace PicesCommander
{
  public partial class SaveActiveTrainingModelFeatureData : Form
  {
    private bool             errorsFound = false;
    private String[]         modelNames = null;
    private String           modelName       = "";
    private PicesMsgQueue    dialogMsgQueue  = new PicesMsgQueue ("SaveActiveTrainingModel");
    private PicesRunLog      runLog = null;
    private Thread           savingThread          = null;
    private bool             savingThreadRunning   = false;
    private bool             savingThreadFailed    = false;
    private bool             savingThreadCancelReq = false;
    private bool             savingThreadDone      = false;

    private bool             formIsClosing         = false;

    private String           initialModelName = "";
    private String           destinationFileName = "";
    private String           fileFormat = "Pices";



    private PicesTrainingConfiguration  config = null;  


    public SaveActiveTrainingModelFeatureData (String  _initialModelName)
    {
      initialModelName = _initialModelName;
      InitializeComponent();
    }



    private  void  AddMsgToDialogMsgQueue (String  s)
    {
      dialogMsgQueue.AddMsg (s);
    }  /* AddMsgToDialogMsgQueue */



    private void  RunLogMsgQueueFlush()
    {
      String msg = dialogMsgQueue.GetNextMsg ();
      while  (msg != null)
      {
        Dialog.AppendText (msg + "\n");
        msg = dialogMsgQueue.GetNextMsg ();
      }
    }


    private void SaveActiveTrainingModelFeatureData_Load (object sender, EventArgs e)
    {
      modelNames = TrainingModel2.GetListOfTrainingModels();
      if (modelNames != null)
      {
        foreach  (String s in modelNames)
          ModelName.Items.Add (s);
      }
      ModelName.Text = OSservices.GetRootName (initialModelName);

      String[]  fileFormats = PicesFeatureFileIO.RegisteredDriverNames (false, true);
      if  (fileFormats != null)
      {
        foreach  (String  ff in fileFormats)
          FileFormat.Items.Add (ff);
      }
      FileFormat.Text = "Pices";

      DestinationFileName.Text = OSservices.AddSlash (PicesSipperVariables.PicesTempDirectory ()) + ModelName.Text + ".data";
    }



    private  void  ValidateModelName ()
    {
      errorProvider1.SetError (ModelName, null);
      if  (String.IsNullOrEmpty (ModelName.Text))
      {
        MessageBox.Show (this, "Model Name is blank");
        errorsFound = true;
        ModelName.Text = modelName;
        return;
      }

      if  (ModelName.Text == modelName)
        return;

      if  (runLog == null)
        runLog = new PicesRunLog (dialogMsgQueue);
      PicesTrainingConfiguration  tc = new PicesTrainingConfiguration (ModelName.Text, runLog);

      if  (!tc.Valid ())
      {
        errorsFound = true;
        errorProvider1.SetError (ModelName, "Model[" + ModelName.Text + "] is not a valid model");
        ModelName.Text = modelName;
      }
      else
      {
        modelName = ModelName.Text;
      }
    }  /* ValidateModelName*/


    private  void  ValidateFileFormat ()
    {
      errorProvider1.SetError (FileFormat, null);
      fileFormat = FileFormat.Text;
      if  (!PicesFeatureFileIO.ValidDriverName (fileFormat))
      {
        errorsFound = true;
        errorProvider1.SetError (FileFormat, "Format[" + fileFormat + "] is not a valid.");
      }
    }



    private  void  ValidateDestinationFileName ()
    {
      errorProvider1.SetError (DestinationFileName, null);
      System.IO.FileInfo  fi = null;
      
      try  {fi = new FileInfo (DestinationFileName.Text);}  catch (Exception)  {fi = null;}
      if  (fi.Exists)
      {
        errorProvider1.SetError (DestinationFileName, "Destinaion File already exists.");
        errorsFound = true;
      }
      else
      {
        destinationFileName = DestinationFileName.Text;
      }
    }  /* ValidateDestinationFileName*/


    private  void  ValidateWholeScreen ()
    {
      errorsFound = false;
      ValidateModelName ();
      ValidateDestinationFileName ();
      ValidateFileFormat ();
    }

    

    private void DestinationBrowseButton_Click(object sender, EventArgs e)
    {
      FeatureFileNamePrompter  fnfp = new FeatureFileNamePrompter (true);
      fnfp.Directory = OSservices.GetPathPartOfFile (DestinationFileName.Text);
      fnfp.FileName  = OSservices.GetRootNameWithExtension (DestinationFileName.Text);
      fnfp.Format    = FileFormat.Text;
      fnfp.ShowDialog ();
      DialogResult  dr = fnfp.Result;
      if  (dr == DialogResult.OK)
      {
        DestinationFileName.Text = OSservices.AddSlash (fnfp.Directory) + fnfp.FileName;
        FileFormat.Text = fnfp.Format;
      }
    }


    private void CreateFeatureFiles_Click(object sender, EventArgs e)
    {
      ValidateWholeScreen ();
      if  (errorsFound)
        return;

      DialogResult  dr = MessageBox.Show (this, "Do you want to create the Feature Data files", "Save Active Training Model Deature Data", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
        return;

      if  (runLog == null)
        runLog = new PicesRunLog (dialogMsgQueue);

      CreateFeatureFiles.Enabled = false;
      DialogTimer.Enabled = true;

      savingThread = new Thread (new ThreadStart (CreateFeatureDataFileThread));
      savingThread.Start ();
      CancelCreateFeatureSelectionButton.Enabled = true;
    }



    private void DialogTimer_Tick(object sender, EventArgs e)
    {
      RunLogMsgQueueFlush ();

      if  (savingThreadDone)
      {
        DialogTimer.Enabled = false;
        if  (savingThreadFailed)
        {
          MessageBox.Show (this, "Feature File Extraction Failed.");
        }
        else
        {
          MessageBox.Show (this, "Feature File Extraction Completed.");
        }

        if  (!formIsClosing)
          Close ();
      }
    }  /* DialogTimer_Tick */




    private  void  CreateFeatureDataFileThread ()
    {
      savingThreadRunning = true;

      PicesDataBase.ThreadInit ();

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Started");

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Loading feature data for Model[" + modelName + "]");
      config = new PicesTrainingConfiguration (modelName, runLog);
      if  (!config.Valid ())
      {
        AddMsgToDialogMsgQueue ("\n\n");
        AddMsgToDialogMsgQueue ("Model[" +  modelName + "]  is invalid.");
        savingThreadFailed = true;
        savingThreadDone   = true;
        savingThreadRunning = false;
        return;
      }


      bool  changesMade = false;
      PicesFeatureVectorList  data = config.LoadFeatureDataFromTrainingLibraries (changesMade);
      if  (data == null)
      {
        AddMsgToDialogMsgQueue ("\n\n");
        AddMsgToDialogMsgQueue ("Model[" +  modelName + "]  Cound not load Feature Data.");
        savingThreadFailed = true;
        savingThreadDone   = true;
        savingThreadRunning = false;
        return;
      }

      if  (savingThreadCancelReq)
      {
        AddMsgToDialogMsgQueue ("\n\n");
        AddMsgToDialogMsgQueue ("Process Canceled.");
        savingThreadDone   = true;
        savingThreadRunning = false;
        return;
      }

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Feature Data Loaded.");

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Checking that Depth Data Exists.");
      MakeSureDepthFieldIsIncluded (data);

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Spliting into seperate files.");

      if  (!savingThreadCancelReq)
      {
        data.Save (destinationFileName, FileFormat.Text, runLog);
      }

      AddMsgToDialogMsgQueue ("\n\n");
      if  (savingThreadCancelReq)
        AddMsgToDialogMsgQueue ("Process    *** CANCELED ***.");
      else
        AddMsgToDialogMsgQueue ("Process Completed.");

      PicesDataBase.ThreadEnd ();

      savingThreadDone   = true;
      savingThreadRunning = false;
    }  /* CreateFeatureDataFileThread */



    void  MakeSureDepthFieldIsIncluded (PicesFeatureVectorList  data)
    {
      PicesDataBase  dbConn = null;
      PicesRunLog  runLog = new PicesRunLog (dialogMsgQueue);

      data.SortByImageFileName ();

      String  curSipperFileName = "";
      float  curSipperFileDepth = 0.0f;


      foreach  (PicesFeatureVector fv in data)
      {
        if  (fv.Depth == 0.0f)
        {
          if  (dbConn == null)
            dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
          uint  sacnLineNum = 0;
          uint  scanCol = 0;
          String  nextSipperFileName = "";

          PicesFeatureVector.ParseImageFileName (fv.ImageFileName, ref nextSipperFileName, ref sacnLineNum, ref scanCol);

          if  (nextSipperFileName != curSipperFileName)
          {
            curSipperFileDepth = 0.0f;
            curSipperFileName = nextSipperFileName;

            PicesSipperFile  sf = dbConn.SipperFileRecLoad (curSipperFileName);
            if  (sf != null)
              curSipperFileDepth = sf.Depth;
          }

          PicesInstrumentData  id = dbConn.InstrumentDataGetByScanLine (curSipperFileName, sacnLineNum);
          if  ((id == null)  ||  (id.Depth == 0.0))
          {
            fv.Depth = curSipperFileDepth;
          }
          else
          {
            fv.Depth = id.Depth;
          }
        }
      }

      if  (dbConn != null)
      {
        dbConn.Close ();
        dbConn = null;
        GC.Collect ();
      }

    }  /* MakeSureDepthFieldIsIncluded */



    private  void   CancelTuningThread ()
    {
      savingThreadCancelReq = true;
      if  (config != null)
        config.CancelLoad ();

      int   loopCount = 0;
      while (savingThreadRunning  &&  (loopCount < 300)  &&  savingThread.IsAlive)
      {
        Thread.Sleep(100);
        RunLogMsgQueueFlush();
        loopCount++;
      }
    }  


    private void CancelButton_Click(object sender, EventArgs e)
    {
      if  (!savingThreadRunning)
        return;

      DialogResult dr = MessageBox.Show (this, "Cancel Tuning File Creation Yes/No ?", "Create Feature Selection data Sets", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
        return;

      CancelCreateFeatureSelectionButton.Enabled = false;

      CancelTuningThread ();

      // We do not need to close the window.  The 'DialogTimer_Tick' method will do it,
    }



    private void SaveActiveTrainingModelFeatureData_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (savingThreadRunning)
      {
        DialogResult dr = MessageBox.Show (this, "Process still running;   Do you want to Cancel Yes/No ?", "Cancel Feature Select Datasets Creation", MessageBoxButtons.YesNo);
        if  (dr == DialogResult.No)
        {
          e.Cancel = true;
          return;
        }

        formIsClosing = true;  // Need to set this to true before calling 'CancelTuningThread'  otherwise the 'DialogTimer_Tick' will try to close also.
        CancelTuningThread ();
      }
      formIsClosing = true;
    }


    private void ModelName_Validating (object sender, CancelEventArgs e)
    {
      ValidateModelName ();
    }


    private void DestinationFileName_Validating (object sender, CancelEventArgs e)
    {
      ValidateDestinationFileName ();
    }

    private void FileFormat_Validating (object sender, CancelEventArgs e)
    {
      ValidateFileFormat ();
    }

    private void ModelName_SelectedIndexChanged (object sender, EventArgs e)
    {
      ValidateModelName ();
    }

    private void FileFormat_SelectedIndexChanged (object sender, EventArgs e)
    {
      ValidateFileFormat ();
    }
  }
}
