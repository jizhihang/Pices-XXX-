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
  public partial class CreateFeatureSelectionDataSets : Form
  {
    private bool             errorsFound = false;
    private String[]         modelNames = null;
    int                      numOfFolds    = 10;
    private float            trainingDataPercentage  = 33.33f;
    private int              trainingDataMaxImagesPerClass = 0;
    private float            testDataPercentage   = 33.33f;
    private float            validationDataPercentage = 33.34f;
    private String           modelName       = "";
    private PicesGoalKeeper  dialogMsgGoalie = new PicesGoalKeeper ("DialogMsgGoalie");
    private List<String>     dialogMsgQueue  = new List<string> (); 
    private PicesRunLog      runLog = null;
    private Thread           tuningThread          = null;
    private bool             tuningThreadRunning   = false;
    private bool             tuningThreadFailed    = false;
    private bool             tuningThreadCancelReq = false;
    private bool             tuningThreadDone      = false;

    private bool          formIsClosing          = false;

    private String        trainDataFileName      = "";
    private String        testDataFileName       = "";
    private String        validationDataFileName = "";

    private String        initialModelName = "";



    private PicesTrainingConfiguration  config = null;  


    public CreateFeatureSelectionDataSets (String  _initialModelName)
    {
      initialModelName = _initialModelName;
      InitializeComponent();
    }



    private  void  AddMsgToDialogMsgQueue (String  s)
    {
      dialogMsgGoalie.StartBlock ();
      dialogMsgQueue.Add (s);
      dialogMsgGoalie.EndBlock   ();
    }  /* AddMsgToDialogMsgQueue */



    private void  RunLogMsgQueueFlush()
    {
      dialogMsgGoalie.StartBlock ();
      foreach (String s in dialogMsgQueue)
      {
        Dialog.AppendText (s + "\n");
      }
      dialogMsgQueue.Clear ();
      dialogMsgGoalie.EndBlock   ();
    }




    private  void  UpdatePercentageFields ()
    {
      Training2DataPercentage.Text  = trainingDataPercentage.ToString   ("##0.00");
      Test2DataPercentage.Text      = testDataPercentage.ToString       ("##0.00");
      ValidaionDataPercentage.Text  = validationDataPercentage.ToString ("##0.00");
    }



    private void CreateFeatureSelectionDataSets_Load (object sender, EventArgs e)
    {
      modelNames = TrainingModel2.GetListOfTrainingModels();
      if (modelNames != null)
      {
        foreach  (String s in modelNames)
          ModelName.Items.Add (s);
      }

      ModelName.Text = initialModelName;

      UpdatePercentageFields ();
      DestinationDirectory.Text = OSservices.AddSlash (PicesSipperVariables.PicesDataFilesDirectory ()) + "FeatureSelection";
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
        runLog = new PicesRunLog ();
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




    private  void  ValidateTrainingDataPercentage ()
    {
      errorProvider1.SetError (Training2DataPercentage, null);
      float zed = trainingDataPercentage = PicesKKStr.StrToFloat (Training2DataPercentage.Text);
      if  ((zed < 0.0f)  ||  (zed > 100.0f))
      {
        errorProvider1.SetError (Training2DataPercentage, "Training Data Percentage must be between 1.0 and 100.0");
        errorsFound = true;
      }
      else
      {
        trainingDataPercentage = zed;
        validationDataPercentage = 100.0f - (trainingDataPercentage + testDataPercentage);
      }
      UpdatePercentageFields ();
    }  /* ValidateTrainingDataPercentage */



    private  void  ValidateTrainingMaxImagesPerClass ()
    {
      errorProvider1.SetError (TrainingMaxImagesPerClass, null);
      trainingDataMaxImagesPerClass = PicesKKStr.StrToInt (TrainingMaxImagesPerClass.Text);
      if   (trainingDataMaxImagesPerClass < 0)
      {
        errorsFound = true;
        errorProvider1.SetError (TrainingMaxImagesPerClass, "Max Images Per Directory can not be negative.");
      }
    }



    private  void  ValidateTestingDataPercentage ()
    {
      errorProvider1.SetError (Test2DataPercentage, null);
      float zed = testDataPercentage = PicesKKStr.StrToFloat (Test2DataPercentage.Text);
      if  ((zed < 0.0f)  ||  (zed > 100.0f))
      {
        errorProvider1.SetError (Test2DataPercentage, "Testing Data Percentage must be between 1.0 and 100.0");
        errorsFound = true;
      }
      else
      {
        testDataPercentage = zed;
        validationDataPercentage = 100.0f - (trainingDataPercentage + testDataPercentage);
      }
      UpdatePercentageFields ();
    }




    private  void  ValidateValidationDataPercentage ()
    {
      errorProvider1.SetError (ValidaionDataPercentage, null);
      float zed = validationDataPercentage = PicesKKStr.StrToFloat (ValidaionDataPercentage.Text);
      if  ((zed < 0.0f)  ||  (zed > 100.0f))
      {
        errorProvider1.SetError (ValidaionDataPercentage, "Validation Data Percentage must be between 1.0 and 100.0");
        errorsFound = true;
      }
      else
      {
        validationDataPercentage = zed;
        testDataPercentage = 100.0f - (trainingDataPercentage + validationDataPercentage);
      }
      UpdatePercentageFields ();
    }



    private  void  ValidateDestinationDirectory ()
    {
      errorProvider1.SetError (DestinationDirectory, null);
      System.IO.DirectoryInfo  di = null;
      
      try  {di = new DirectoryInfo (DestinationDirectory.Text);}  catch (Exception)  {di = null;}
      if  (!di.Exists)
      {
        errorProvider1.SetError (DestinationDirectory, "Destinaion Directory does not exist.");
        errorsFound = true;
      }
    }  /* ValidateDestinationDirectory*/




    private  void  ValidateWholeScreen ()
    {
      errorsFound = false;
      
      ValidateModelName ();
      ValidateTrainingDataPercentage ();
      ValidateTrainingMaxImagesPerClass ();
      ValidateTestingDataPercentage ();
      ValidateValidationDataPercentage ();
      ValidateDestinationDirectory ();
    }

    

    private void DirectoryBrowseButton_Click(object sender, EventArgs e)
    {
      FolderBrowserDialog  fd = new FolderBrowserDialog ();
      fd.Description = "Directory where we want to create folder with Feature Data.";
      fd.SelectedPath  = DestinationDirectory.Text;
      DialogResult dr = fd.ShowDialog ();
      if  (dr == DialogResult.OK)
      {
        DestinationDirectory.Text = fd.SelectedPath;
      }
    }


    private void CreateFeatureFiles_Click(object sender, EventArgs e)
    {
      ValidateWholeScreen ();
      if  (errorsFound)
        return;

      DialogResult  dr = MessageBox.Show (this, "Do you want to create the Feature Data files?", "Create Feature Data Files", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
        return;

      if  (runLog == null)
        runLog = new PicesRunLog ();

      CreateFeatureFiles.Enabled = false;
      DialogTimer.Enabled = true;

      tuningThread = new Thread (new ThreadStart (CreateTuningFeatureFiles));
      tuningThread.Start ();
      CancelCreateFeatureSelectionButton.Enabled = true;
    }



    int  lastRunLogLoneCount = -1;
    private void DialogTimer_Tick(object sender, EventArgs e)
    {
      RunLogMsgQueueFlush ();

      if  (runLog != null)
      {
        if  (runLog.LineCount () != lastRunLogLoneCount)
        {
          String s = runLog.LastLine ();
          if  (!String.IsNullOrEmpty (s))
            AddMsgToDialogMsgQueue (s);
          lastRunLogLoneCount = runLog.LineCount ();
        }
      }

      if  (tuningThreadDone)
      {
        DialogTimer.Enabled = false;
        if  (tuningThreadFailed)
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




    private  void  CreateTuningFeatureFiles ()
    {
      tuningThreadRunning = true;

      PicesDataBase.ThreadInit ();


      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Started");

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Creating Destination Directory");
      String  destinationDirectory = OSservices.AddSlash (DestinationDirectory.Text) + OSservices.GetRootName (modelName);
      OSservices.CreateDirectoryPath (destinationDirectory);

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Loading feature data for Model[" + modelName + "]");
      config = new PicesTrainingConfiguration (modelName, runLog);
      if  (!config.Valid ())
      {
        AddMsgToDialogMsgQueue ("\n\n");
        AddMsgToDialogMsgQueue ("Model[" +  modelName + "]  is invalid.");
        tuningThreadFailed = true;
        tuningThreadDone   = true;
        tuningThreadRunning = false;
        return;
      }


      bool  changesMade = false;
      PicesFeatureVectorList  data = config.LoadFeatureDataFromTrainingLibraries (changesMade);
      if  (data == null)
      {
        AddMsgToDialogMsgQueue ("\n\n");
        AddMsgToDialogMsgQueue ("Model[" +  modelName + "]  Cound not load Feature Data.");
        tuningThreadFailed = true;
        tuningThreadDone   = true;
        tuningThreadRunning = false;
        return;
      }

      if  (tuningThreadCancelReq)
      {
        AddMsgToDialogMsgQueue ("\n\n");
        AddMsgToDialogMsgQueue ("Process Canceled.");
        tuningThreadDone   = true;
        tuningThreadRunning = false;
        return;
      }

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Feature Data Loaded.");

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Checking that Depth Data Exists.");
      MakeSureDepthFieldIsIncluded (data);

      AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Spliting into seperate files.");

      PicesFeatureVectorList  trainData      = new PicesFeatureVectorList ();
      PicesFeatureVectorList  testData       = new PicesFeatureVectorList ();
      PicesFeatureVectorList  validationData = new PicesFeatureVectorList ();

      PicesClassList  classes = data.ExtractListOfClasses ();
      if  (classes == null)
      {
        AddMsgToDialogMsgQueue ("\n\n");
        AddMsgToDialogMsgQueue ("Could not extract list of classses from data.");
        tuningThreadFailed  = true;
        tuningThreadDone    = true;
        tuningThreadRunning = false;
        return;
      }


      foreach (PicesClass  c in classes)
      {
        int  trainCount      = 0;
        int  testCount       = 0;
        int  validationCount = 0;

        PicesFeatureVectorList  examplesForClass = data.ExtractImagesForAGivenClass (c);
        int n = examplesForClass.Count;
        
        AddMsgToDialogMsgQueue (DateTime.Now.ToShortTimeString () + " Processing Class[" + c.Name + "]  Count[" + n + "]");
        
        trainCount = (int)((float)n * trainingDataPercentage / 100.0f + 0.5f);
        if  (trainingDataMaxImagesPerClass > 0)
        {
          if  (trainCount > trainingDataMaxImagesPerClass)
            trainCount = trainingDataMaxImagesPerClass;
        }

        if  (validationDataPercentage == 0.0f)
          testCount = n - trainCount;
        else
          testCount  = (int)((float)n * testDataPercentage / 100.0f + 0.5f);

        validationCount = n - (trainCount + testCount);

        examplesForClass.RandomizeOrder ();

        int  validationStart = trainCount + testCount;
        int  testStart       = trainCount;

        for  (int idx = 0;  idx < n;  idx++)
        {
          PicesFeatureVector  fv = examplesForClass[idx];
          if  (idx >= validationStart)
            validationData.Add (fv);

          else if  (idx >= testStart)
            testData.Add (fv);

          else
            trainData.Add (fv);
        }

        if  (tuningThreadCancelReq)
        {
          AddMsgToDialogMsgQueue ("\n\n");
          AddMsgToDialogMsgQueue ("Process Canceled.");
          break;
        }
      }

      if  (!tuningThreadCancelReq)
      {
        AddMsgToDialogMsgQueue ("\n\n" + DateTime.Now.ToShortTimeString () + " Stratifying Data");

        PicesFeatureVectorList trainDataStratified      = trainData.StratifyAmoungstClasses      (numOfFolds);
        PicesFeatureVectorList testDataStratified       = testData.StratifyAmoungstClasses       (numOfFolds);
        PicesFeatureVectorList validationDataStratified = validationData.StratifyAmoungstClasses (numOfFolds);

        String rootName = OSservices.AddSlash (destinationDirectory) + OSservices.GetRootName (modelName);


        if  ((trainDataStratified.Count > 0) && (!tuningThreadCancelReq))
        {
          trainDataFileName = rootName + "_Train.data";
          AddMsgToDialogMsgQueue ("Saving Training Data[" + trainDataFileName + "]");
          trainDataStratified.Save (trainDataFileName, "", runLog);
        }

        if  ((testDataStratified.Count > 0) && (!tuningThreadCancelReq))
        {
          testDataFileName = rootName + "_Test.data";
          AddMsgToDialogMsgQueue ("Saving Test Data[" + testDataFileName + "]");
          testDataStratified.Save (testDataFileName, "", runLog);
        }

        if  ((validationDataStratified.Count > 0) && (!tuningThreadCancelReq))
        {
          validationDataFileName = rootName + "_Validation.data";
          AddMsgToDialogMsgQueue ("Saving Validation Data[" + validationDataFileName + "]");
          validationDataStratified.Save (validationDataFileName, "", runLog);
        }
      }

      AddMsgToDialogMsgQueue ("\n\n");
      if  (tuningThreadCancelReq)
        AddMsgToDialogMsgQueue ("Process    *** CANCELED ***.");
      else
        AddMsgToDialogMsgQueue ("Process Completed.");

      PicesDataBase.ThreadEnd ();

      tuningThreadDone   = true;
      tuningThreadRunning = false;
    }  /* CreateTuningFeatureFiles */



    void  MakeSureDepthFieldIsIncluded (PicesFeatureVectorList  data)
    {
      PicesDataBase  dbConn = null;
      PicesRunLog  runLog = new PicesRunLog ();

      data.SortByImageFileName ();

      String  curSipperFileName = "";
      float  curSipperFileDepth = 0.0f;


      foreach  (PicesFeatureVector fv in data)
      {
        if  (fv.Depth == 0.0f)
        {

          String  nextSipperFileName = PicesMethods.SipperFileNameFromImageFileName (fv.ImageFileName);

          if  (nextSipperFileName != curSipperFileName)
          {
            curSipperFileDepth = 0.0f;
            curSipperFileName = nextSipperFileName;

            if  (dbConn == null)
              dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

            PicesSipperFile  sf = dbConn.SipperFileRecLoad (curSipperFileName);
            if  (sf != null)
              curSipperFileDepth = sf.Depth;
          }

          if  (fv.Depth == 0.0f)
            fv.Depth = curSipperFileDepth;
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
      tuningThreadCancelReq = true;
      if  (config != null)
        config.CancelLoad ();

      int   loopCount = 0;
      while (tuningThreadRunning  &&  (loopCount < 300)  &&  tuningThread.IsAlive)
      {
        Thread.Sleep(100);
        RunLogMsgQueueFlush();
        loopCount++;
      }
    }  


    private void CancelButton_Click(object sender, EventArgs e)
    {
      if  (!tuningThreadRunning)
        return;

      DialogResult dr = MessageBox.Show (this, "Cancel Tuning File Creation Yes/No ?", "Create Feature Selection data Sets", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
        return;

      CancelCreateFeatureSelectionButton.Enabled = false;

      CancelTuningThread ();

      // We do not need to close the window.  The 'DialogTimer_Tick' method will do it,
    }




    private void CreateFeatureSelectionDataSets_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (tuningThreadRunning)
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



    private void TrainingMaxImagesPerClass_Validating (object sender, CancelEventArgs e)
    {
      ValidateTrainingMaxImagesPerClass ();
    }

    private void Training2DataPercentage_Validating (object sender, CancelEventArgs e)
    {
      ValidateTrainingDataPercentage ();
    }

    private void ModelName_Validating (object sender, CancelEventArgs e)
    {
      ValidateModelName ();
    }

    private void Test2DataPercentage_Validating (object sender, CancelEventArgs e)
    {
      ValidateTestingDataPercentage ();
    }

    private void ValidaionDataPercentage_Validating (object sender, CancelEventArgs e)
    {
      ValidateValidationDataPercentage ();
    }

    private void DestinationDirectory_Validating (object sender, CancelEventArgs e)
    {
      ValidateDestinationDirectory ();
    }  
  }
}
