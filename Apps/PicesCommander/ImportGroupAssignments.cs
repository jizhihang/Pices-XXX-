using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;



using  PicesInterface;

namespace PicesCommander
{
  public partial class ImportGroupAssignments : Form
  {

    private  PicesDataBase           mainWinConn = null;
    private  PicesRunLog             runLog      = null;

    private  String                  groupName       = "";
    private  String                  groupDesc       = "";
    private  String                  sourceDirectory = null;

    // Importing Thread Related Variables.
    private  bool                    cancelBackGround   = false;
    private  bool                    backGroundStarted  = false;  // Will be set to 'true' after the start button is pressed
                                                                 // and allvalidations are passed.
    private  bool                    backGroundRunning   = false;
    private  bool                    backGroundCompleted = false;
    private  Thread                  backGroundThread    = null;
    private  PicesRunLog             backGroundLog       = null;

    private  List<String>            importedFileNames = null;
    private  PicesDataBaseImageGroup group = null;


    // 
    private bool                     validationErrorFound = false;

    private PicesMsgQueue            runLogMsgQueue = null;  // List of messages that can be added to 'RunLog'.

    public  bool                     CancelBackGround     {get {return cancelBackGround;}   set  {cancelBackGround = value;}}
    public  bool                     BackGroundRunning    {get {return backGroundRunning;}}
    public  PicesDataBaseImageGroup  Group                {get {return group;}}

    private List<String>             imageFileErrorList = null;  // Will contain alistof Image File Names that could not be insertd.



    public ImportGroupAssignments()
    {
      runLogMsgQueue = new PicesMsgQueue ("ImportGroupAssignemnts");
      runLog  = new PicesRunLog (runLogMsgQueue);
      mainWinConn  = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      InitializeComponent();

      SourceDirectory.Text = PicesSipperVariables.PicesHomeDir ();
    }



    private  void  RunLogAddMsg (String  msg)
    {
      runLogMsgQueue.AddMsg (msg);
    }  /* RunLogAddMsg */



    private  void  RunLogMsgQueueFlush ()
    {
      String  msg = runLogMsgQueue.GetNextMsg ();
      while  (msg != null)
      {
        RunLog.AppendText (msg + "\n");
        backGroundLog.Writeln (msg);
        msg = runLogMsgQueue.GetNextMsg ();
      }
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

    

    private  void  ValidateGroupName ()
    {
      errorProvider1.SetError (GroupName, "");
      String   errorDesc = "";
      bool     valid = false;

      groupName = GroupName.Text;
      PicesDataBaseImageGroup.ValidName (ref groupName, ref errorDesc, ref valid);
      if  (!valid)
      {
        errorProvider1.SetError (GroupName, errorDesc);
        validationErrorFound = true;
      }
      GroupName.Text = groupName;
    }  /* ValidateGroupName */



    private  void  ValidateGroupNameDoesntExist ()
    {
      group = mainWinConn.ImageGroupLoad (groupName);
      if  (group != null)
      {
        DialogResult dr = MessageBox.Show 
                        (this, 
                         "Group[" + groupName + "] already exists" + "\n\n" +
                         "Do you want to repleace it ?",
                         "Group Already Exists", 
                         MessageBoxButtons.YesNo
                        );

        if  (dr == DialogResult.No)
        {
          validationErrorFound = true;
          errorProvider1.SetError (GroupName, "Group already exists");
        }
      }
    }   /* ValidateGroupNameDoesntExist */


    
    private  void  ValidateGroupDescription ()
    {
      groupDesc = GroupDescription.Text;
    }



    private void ValidateAll ()
    {
      validationErrorFound = false;
      ValidateSourceDirectory ();
      ValidateGroupName ();
      ValidateGroupDescription ();
      if  (!validationErrorFound)
        ValidateGroupNameDoesntExist ();
    }  /* ValidateAll */



    private  void  StartTheBackGroundProcedure ()
    {
      DateTime n = DateTime.Now;
      String logFileName = OSservices.AddSlash(PicesSipperVariables.PicesTempDirectory()) + "ImportGroupAssignments_Log_" +
                             n.Year.ToString("0000") + "-" + n.Month.ToString("00") + "-" + n.Day.ToString("00") + "_" +
                             n.Hour.ToString("00") + "-" + n.Minute.ToString("00") + "-" + n.Hour.ToString("00") +
                             ".txt";

      backGroundLog = new PicesRunLog (logFileName);

      cancelBackGround = false;
      backGroundThread = new Thread (new ThreadStart (ImportAssignments));
      backGroundThread.Start ();

      BackGroundTimer.Enabled      = true;
      SourceDirectory.Enabled  = false;
      BrowseSourceDir.Enabled  = false;
      GroupName.Enabled        = false;
      GroupDescription.Enabled = false;
    }  /* StartTheHarvestingProcedure */



    private  void  ImportAssignments ()
    {
      PicesDataBase.ThreadInit ();
      backGroundRunning = true;
      backGroundCompleted = false;

      PicesDataBase  threadConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      RunLogAddMsg ("Source Directory [" + sourceDirectory                               + "]");
      RunLogAddMsg ("Group Name       [" + groupName                                     + "]");
      RunLogAddMsg ("Start Date/Time  [" + DateTime.Now.ToString("yyyy-MMM-dd HH:mm:ss") + "]");
      RunLogAddMsg ("Active DataBase  [" + threadConn.Server.Description                 + "]");
      
      importedFileNames = new List<String> ();

      group = threadConn.ImageGroupLoad (groupName);
      if  (group != null)
      {
        // Group already exists;  will delete it.
        RunLogAddMsg ("\n" + "Group[" + groupName + "] already exists.   Will remove and replace with new one." + "\n");

        threadConn.ImageGroupDelete (group.ImageGroupId);
      }

      group = new PicesDataBaseImageGroup (-1, groupName, groupDesc, 0);
      threadConn.ImageGroupInsert (group);

      ImportAssignmentsImportDir (sourceDirectory);
      if  (cancelBackGround)
      {
        RunLogAddMsg ("\n" + "Importing Assignemnst has benen Canceled." + "\n");
      }
      else
      {
        RunLogAddMsg ("\n\n" + "Total Images[" + importedFileNames.Count.ToString ("###,##0") + "\n");

        RunLogAddMsg ("Wrinting to DataBase Now" + "\n");

        imageFileErrorList = new List<string>();

        int  numInserted = 0;

        for  (int x = 0; (x < importedFileNames.Count) && (!cancelBackGround); x++)
        {
          PicesDataBaseImageGroupEntry ige = new PicesDataBaseImageGroupEntry ();
          ige.ImageGroupId = group.ImageGroupId;
          ige.ImageFileName = importedFileNames[x];
          threadConn.ImageGroupEntriesInsert (ige);
          if  (!threadConn.Valid ())
          {
            imageFileErrorList.Add (importedFileNames[x]);
            RunLogAddMsg ("Image[" + importedFileNames[x] + "]  ***ERROR***");
          }
          else
          {
            numInserted++;
          }

          if  ((x % 100) == 0)
          {
            RunLogAddMsg ("Images Assigned[" + numInserted + "]  Failures[" + imageFileErrorList.Count + "]");
          }
        }
      }

      threadConn.Close ();
      threadConn = null;
      GC.Collect ();
      PicesDataBase.ThreadEnd ();

      backGroundRunning = false;
      backGroundCompleted = true;
    }  /* ImportAssignments */



    private  void  ImportAssignmentsImportDir (String  dirName)
    {
      String[]  fileNames = null;
      try  {fileNames = Directory.GetFiles (dirName);}  
      catch(Exception e) 
      {
        RunLogAddMsg ("\n" + "Error retrieving file info for Directory[" + dirName + "]");
        RunLogAddMsg ("Exception[" + e.ToString () + "]" + "\n");
        fileNames = null;
      }

      if  (fileNames != null)
      {
        int numThisDir = 0;
        foreach (String fn in fileNames)
        {
          String  ext = OSservices.GetFileExtension (fn).ToLower ();
          if  (ext == "bmp")
          {
            importedFileNames.Add (OSservices.GetRootName (fn));
            numThisDir++;
          }
        }

        RunLogAddMsg ("Dir[" + dirName + "]  FilesFound[" + numThisDir.ToString ("###,##0") + "]");
      }

      if  (!cancelBackGround)
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
            ImportAssignmentsImportDir (subDir);
            if  (cancelBackGround)
              break;
          }
        }
      }
    }  /* ImportAssignmentsImportDir */



    private  void  CancelBackGroundProcess ()
    {
      cancelBackGround = true;
      int  loopCount = 0;

      while  (backGroundRunning  && (loopCount < 200)  &&   backGroundThread.IsAlive)
      {
        Thread.Sleep (100);
        RunLogMsgQueueFlush ();
        loopCount++;
      }

      if  ((backGroundThread != null)  &&  (backGroundThread.IsAlive))
        backGroundThread.Abort ();
    }  /* CancelBackGroundProcess */




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



    private void GroupName_Leave(object sender, EventArgs e)
    {
      ValidateGroupName ();
    }



    private void GroupDescription_Leave(object sender, EventArgs e)
    {
      ValidateGroupDescription ();
    }

    
    private void Start_Click(object sender, EventArgs e)
    {
      if  (backGroundStarted)
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

      StartTheBackGroundProcedure ();
    }



    private void Cancel_Click (object sender, EventArgs e)
    {
      Cancel.Enabled = false;

      if  (!backGroundRunning)
      {
        CloseMySQL ();
        Close ();
      }

      DialogResult dr = MessageBox.Show (this, "Cancel Importing ?", "Importing", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        CancelBackGroundProcess ();
        CloseMySQL ();
        Close ();
      }

      Cancel.Enabled = true;
    }



    private void BackgroundTimer_Tick(object sender, EventArgs e)
    {
      RunLogMsgQueueFlush ();

      if  (!backGroundRunning)
      {
        BackGroundTimer.Enabled = false;

        if ((backGroundCompleted)  &&  (!cancelBackGround))
        {
          if  ((imageFileErrorList.Count > 0)  &&  (backGroundLog != null))
          {
            DialogResult  dr = MessageBox.Show (this, "Error Inserting [" + imageFileErrorList.Count + "] images." + "\n\n" + "Do you want to see log file Yes/No ?", "Error Inserting into DataBase", MessageBoxButtons.YesNo);
            if (dr == DialogResult.Yes)
              backGroundLog.DisplayWithNoteBook ();
          }
          else
          {
            MessageBox.Show (this, "Importing of Assignments completed.");
          }
        }

        CloseMySQL ();
        this.Close ();
      }
    }




    private  void  ImportGroupAssignments_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (backGroundRunning)
      {
        if  (e.CloseReason == CloseReason.UserClosing)
        {
          DialogResult  dr = MessageBox.Show (this, "The Importing Process is Still Running!  Cancel Process Yes/No ?", "Import Group Assignments  Cancel Request", MessageBoxButtons.YesNo);
          if  (dr == DialogResult.No)
          {
            e.Cancel = true;
            return;
          }
        }

        CancelBackGroundProcess ();
      }
      CloseMySQL ();
    }


    private  void  CloseMySQL ()
    {
      if  (mainWinConn != null)
      {
        mainWinConn.Close ();
        mainWinConn = null;
        GC.Collect ();
      }
    }
  }
}
