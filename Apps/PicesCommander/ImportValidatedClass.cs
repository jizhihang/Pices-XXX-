using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.IO;
using System.Drawing;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using  PicesInterface;

namespace PicesCommander
{
  public partial class ImportValidatdClass : Form
  {
    private  PicesDataBase           mainWinConn = null;
    private  PicesRunLog             runLog = null;

    private  String                  sourceDirectory = null;

    // Importing Thread Related Variables.
    private  bool                    cancelImporting   = false;
    private  bool                    importingStarted  = false;  // Will be set to 'true' after the start button is pressed
                                                                 // and allvalidations are passed.
    private  bool                    importingRunning   = false;
    private  bool                    importingCompleted = false;
    private  Thread                  importingThread    = null;

    // 
    private bool                     validationErrorFound = false;

    private PicesGoalKeeper          runLogGoalie   = null;
    private Queue<String>            runLogMsgQueue = null;  // List of messages that can be added to 'RunLog'.

    private int                      totalImagesUpdated = 0;

    public  bool                     CancelImporting     {get {return cancelImporting;}   set  {cancelImporting = value;}}
    public  bool                     ImportingRunning    {get {return importingRunning;}}


    public ImportValidatdClass()
    {
      runLog  = new PicesRunLog ();
      mainWinConn  = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      runLogGoalie = new PicesGoalKeeper ("ImportValidatingClass");
      runLogMsgQueue = new Queue<String> ();

      InitializeComponent();

      SourceDirectory.Text = PicesSipperVariables.PicesHomeDir ();

    }



    private  void  RunLogAddMsg (String  msg)
    {
      runLogGoalie.StartBlock ();

      if  (runLogMsgQueue == null)
        runLogMsgQueue = new Queue<String> ();

      runLogMsgQueue.Enqueue (msg);

      runLogGoalie.EndBlock ();
    }  /* RunLogAddMsg */



    private  void  RunLogMsgQueueFlush ()
    {
      runLogGoalie.StartBlock ();
      if  (runLogMsgQueue != null)
      {
        while  (runLogMsgQueue.Count > 0)
        {
          String  msg = runLogMsgQueue.Dequeue ();
          RunLog.AppendText (msg);
        }
      }

      runLogGoalie.EndBlock ();
    }  /* RunLogMsgQueueFlush */




    private  void  ValidateSourceDirectory ()
    {
      errorProvider1.SetError (SourceDirectory, "");

      String  parentDir = OSservices.GetParentDirectoryOfDirPath (SourceDirectory.Text);
      DirectoryInfo di = new DirectoryInfo (SourceDirectory.Text);
      if  (!di.Exists)
      {
        validationErrorFound = true;
        errorProvider1.SetError (SourceDirectory, "No such directory[" + SourceDirectory.Text + "].");
      }
      else
      {
        sourceDirectory = SourceDirectory.Text;
      }
    }  /* ValidateSourceDirectory */

    

    private void ValidateAll ()
    {
      validationErrorFound = false;
      ValidateSourceDirectory ();
    }  /* ValidateAll */



    private  void  StartTheImportingProcedure ()
    {
      importingStarted = true;
      cancelImporting = false;
      importingThread = new Thread (new ThreadStart (ImportValidatedClassAssignments));
      importingThread.Start ();

      ImportTimer.Enabled      = true;
      SourceDirectory.Enabled  = false;
    }  /* StartTheHarvestingProcedure */



    private  void  ImportValidatedClassAssignments ()
    {
      importingRunning = true;
      importingCompleted = false;

      PicesDataBase.ThreadInit ();

      PicesRunLog  runLog = new PicesRunLog ();
      PicesDataBase  threadConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      totalImagesUpdated = 0;

      ImportValidatedClassAssignmentsDir (threadConn, sourceDirectory);
      if  (cancelImporting)
      {
        RunLogAddMsg ("\n" + "Importing Assignemnst has benen Canceled." + "\n\n");
      }
      else
      {
        RunLogAddMsg ("\n\n" + "Total Images[" + totalImagesUpdated.ToString ("###,##0") + "\n\n");
      }

      threadConn.Close ();
      threadConn = null;
      GC.Collect ();

      PicesDataBase.ThreadEnd ();

      importingRunning = false;
      importingCompleted = true;
    }  /* ImportValidatedClassAssignments */



    private  PicesClass  GetClassFromName (PicesDataBase  threadConn,
                                           String         _name
                                          )
    {
      PicesClass pc = threadConn.MLClassLoad (_name);
      if  (pc == null)
      {
        pc = new PicesClass (_name, "");
        threadConn.MLClassInsert (pc);
      }
      return  pc;
    }


    private  void  ImportValidatedClassAssignmentsDir (PicesDataBase  threadConn,
                                                       String         dirName
                                                      )
    {
      RunLogAddMsg ("Dir[" + dirName + "]" + "\n");

      String[]  fileNames = null;
      try  {fileNames = Directory.GetFiles (dirName);}  
      catch(Exception e) 
      {
        RunLogAddMsg ("\n" + "Error retrieving file info for Directory[" + dirName + "]" + "\n");
        RunLogAddMsg ("Exception[" + e.ToString () + "]" + "\n\n");
        fileNames = null;
      }

      if  (fileNames != null)
      {
        String  className = PicesClass.GetClassNameFromDirName (dirName);
        PicesClass  mlClass = null;

        int numThisDir       = 0;
        int numFailedThisDir = 0;
        foreach (String fn in fileNames)
        {
          String  ext = OSservices.GetFileExtension (fn).ToLower ();
          if  (ext == "bmp")
          {
            String  rn = OSservices.GetRootName (fn);
            if  (mlClass == null)
              mlClass =  GetClassFromName (threadConn, className);

            threadConn.ImagesUpdateValidatedAndPredictClass (rn, mlClass, 1.0f);
            if  (threadConn.Successful ())
            {
              numThisDir++;
              if  ((numThisDir  % 100) == 0)
                RunLogAddMsg ("Dir[" + dirName + "]  Files Updated[" + numThisDir.ToString ("###,##0") + "]" + "\n");
            }
            else
            {
              numFailedThisDir++;
              RunLogAddMsg ("Dir[" + dirName + "]  RootName["  + rn + "]  Failed" + "\n");
              RunLogAddMsg (threadConn.LastErrorDesc () + "\n");
            }
          }
        }

        RunLogAddMsg ("Dir[" + dirName + "]  Files Updated[" + numThisDir.ToString ("###,##0") + "]" + "\n");
        totalImagesUpdated += numThisDir;
      }

      if  (!cancelImporting)
      {
        String[]  directories = null;
        try {directories = Directory.GetDirectories (dirName);}
        catch(Exception) {directories = null;}

        if  (directories != null)
        {
          foreach  (String  subDir in directories)
          {
            if  ((subDir == ".")  ||  (subDir == ".."))
              continue;
            ImportValidatedClassAssignmentsDir (threadConn, subDir);
            if  (cancelImporting)
              break;
          }
        }
      }
    }  /* ImportValidatedClassAssignmentsDir */



    private  void  CancelImportingProcess ()
    {
      cancelImporting = true;
      int  loopCount = 0;

      while  (importingRunning  && (loopCount < 200)  &&   importingThread.IsAlive)
      {
        Thread.Sleep (100);
        RunLogMsgQueueFlush ();
        loopCount++;
      }

      if  ((importingThread != null)  &&  (importingThread.IsAlive))
        importingThread.Abort ();
    }  /* CancelImportingProcess */




    private void BrowseSourceDir_Click(object sender, EventArgs e)
    {
      FolderBrowserDialog  fbd = new FolderBrowserDialog ();

      if  (String.IsNullOrEmpty (SourceDirectory.Text))
        SourceDirectory.Text = PicesSipperVariables.PicesHomeDir ();

      if  (Directory.Exists (SourceDirectory.Text))
        SourceDirectory.Text = PicesSipperVariables.PicesHomeDir ();

      fbd.SelectedPath = SourceDirectory.Text;

      DialogResult dr = fbd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        SourceDirectory.Text = fbd.SelectedPath;
        ValidateSourceDirectory ();
      }
    }



    private void SourceDirectory_Leave(object sender, EventArgs e)
    {
      ValidateSourceDirectory ();
    }


   
    private void Start_Click(object sender, EventArgs e)
    {
      if  (importingStarted)
      {
        // This should not have been able to have happened.
        Start.Enabled = false;
        return;
      }

      ValidateAll ();
      if  (validationErrorFound)
        return;

      DialogResult  dr = MessageBox.Show (this, "Start Importing Images Assignments?", "Import Group Assignments", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
        return;

      Start.Enabled = false;

      StartTheImportingProcedure ();
    }



    private void Cancel_Click (object sender, EventArgs e)
    {
      Cancel.Enabled = false;

      if  (!importingRunning)
      {
        this.Close ();
      }

      DialogResult dr = MessageBox.Show (this, "Cancel Importing ?", "Importing", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        CancelImportingProcess ();
        this.Close ();
      }

      Cancel.Enabled = true;
    }


    private void ImportTimer_Tick(object sender, EventArgs e)
    {
      RunLogMsgQueueFlush ();

      if  (!importingRunning)
      {
        ImportTimer.Enabled = false;

        if ((importingCompleted)  &&  (!cancelImporting))
          MessageBox.Show (this, "Importing of Validated Class Assignments completed.");

        this.Close ();
      }
    } 
  }
}
