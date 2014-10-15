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
using SipperFile;


namespace PicesCommander
{
  public partial class ReconcileImagesToDataBase : Form
  {
    private  PicesDataBase   dbConn = null;
    private  PicesRunLog     runLog = null;

    private  int             runStatsImagesInDataBase    = 0;
    private  int             runStatsImagesNotInDataBase = 0;
    private  int             runStatsImagesFixed         = 0;
    private  int             runStatsTotalImages         = 0;
    private  String          runStatsCurDirectory        = "";

    private  bool            validationErrorsFound       = false;



    // Importing Thread Related Variables.
    private  bool            cancelReconciling    = false;
    private  bool            reconcilingRunning   = false;
    private  Thread          reconcilingThread    = null;
    private  PicesRunLog     reconcilingRunLog    = null;

    private  bool            formIsClosing = false;

    private  bool            removeImagesThatAreNotInDatabase = false;
    private  String          removalDestRootDir = "";




    public ReconcileImagesToDataBase ()
    {
      InitializeComponent ();
    }


    
    private void ReconcileImagesToDataBase_Load (object sender, EventArgs e)
    {
      runLog  = new PicesRunLog ();
      dbConn  = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
    }



    private  void  ValidateSourceDirectory ()
    {
      errorProvider1.SetError (SourceDirectory, null);
      DirectoryInfo di = null;
      try  {di = new DirectoryInfo (SourceDirectory.Text);}  catch (Exception)  {di = null;}
      if  ((di == null)  ||  (!di.Exists))
      {
        errorProvider1.SetError (SourceDirectory, "No such directory");
        validationErrorsFound = true;
      }
    }  /* ValidateSourceDirectory */



    private  void  ValidateRemoveImagesThatAreNotIndatabase ()
    {
      errorProvider1.SetError (RemoveImagesThatAreNotInDatabase, null);
      removeImagesThatAreNotInDatabase = RemoveImagesThatAreNotInDatabase.Checked;
    }



    private  void  ValidateAll ()
    {
      validationErrorsFound = false;
      ValidateSourceDirectory ();
      ValidateRemoveImagesThatAreNotIndatabase ();
    }



    private  void  CancelRecondiliationThread ()
    {
      cancelReconciling = true;

      int  loopCount = 0;
      while  (reconcilingRunning  &&  (loopCount < 300))
      {
        Thread.Sleep (100);
      }

      if  (reconcilingThread.IsAlive)
        reconcilingThread.Abort ();
    }



    private  void  StartReconciliationThread ()
    {
      String  rootName = OSservices.GetRootNameOfDirectory (SourceDirectory.Text);
      DateTime d = DateTime.Now;
      String  logFileName =  OSservices.AddSlash (PicesSipperVariables.PicesTempDirectory ()) + rootName + "_ReconcileImageNames_" +
                             d.ToString ("yyyyMMdd-HHmmss") + ".txt";

      // Any images thnatare not in the Database thataretobe removed will be moved to the subdirectory structure below.
      removalDestRootDir = "c:\\Temp\\PicesDatabaseImageReconciliationRemovedImages\\" + rootName;
      OSservices.CreateDirectoryPath (removalDestRootDir);

      reconcilingRunLog = new PicesRunLog (logFileName);


      Start.Enabled = false;
      SourceDirectory.ReadOnly = true;
      SourceDirectoryBrowse.Enabled = false;
      reconcilingThread = new Thread (new ThreadStart (ReconciliationThread));
      reconcilingThread.Start ();
      Cancel.Enabled = true;
      RunStatsTimer.Enabled = true;
    }  /* StartReconciliationThread */


    
    private  void  ReconciliationThread ()
    {
      reconcilingRunLog.SetLoggingLevel (10);
      reconcilingRunLog.Writeln ("Source Directory [" + SourceDirectory.Text                          + "]" + "\n");
      reconcilingRunLog.Writeln ("Start Date/Time  [" + DateTime.Now.ToString("yyyy-MMM-dd HH:mm:ss") + "]" + "\n");
      reconcilingRunLog.Writeln ("Active DataBase  [" + dbConn.Server.Description                     + "]" + "\n");

      reconcilingRunning = true;
      String rootDir = SourceDirectory.Text;
      DirectoryInfo di = null;
      try {di = new DirectoryInfo (rootDir);}  catch (Exception)  {di = null;}
      if  (di == null)
        return;
      
      CountDirectory (di);
      if  (!cancelReconciling)
      {
        reconcilingRunLog.Writeln ("Total Images To Process[" + runStatsTotalImages.ToString ("###,###,##0") + "]" + "\n");
        ProcessDirectory (di);
      }


      reconcilingRunLog.Writeln ("Total Images" + "\t" + runStatsTotalImages.ToString ("###,###,##0"));
      reconcilingRunLog.Writeln ("In Database"  + "\t" + runStatsImagesInDataBase.ToString ("###,###,##0"));
      reconcilingRunLog.Writeln ("Not Found"    + "\t" + runStatsImagesNotInDataBase.ToString ("###,###,##0"));
      reconcilingRunLog.Writeln ("Fixed"        + "\t" + runStatsImagesFixed.ToString ("###,###,##0"));

      reconcilingRunning = false;
    }  /* ProcessAllDirectories */



    private  void  CountDirectory (DirectoryInfo di)
    {
      if  (cancelReconciling)
        return;

      runStatsCurDirectory = di.FullName;

      FileInfo[] files = null;
      try  {files = di.GetFiles ("*.bmp");}  catch (Exception)  {files = null;}
      if  (files != null)
        runStatsTotalImages += files.Length;

      if  (!cancelReconciling)
      {
        DirectoryInfo[]  subDirectories = null;

        try  {subDirectories = di.GetDirectories ();} catch (Exception)  {subDirectories = null;}
        if  (subDirectories != null)
        {
          foreach  (DirectoryInfo  subDirectory in subDirectories)
          {
            if  (cancelReconciling)
              break;
            CountDirectory (subDirectory);
          }
        }
      }
    }  /* CountDirectory */




    
    private  void  ProcessDirectory ( DirectoryInfo di)
    {
      if  (cancelReconciling)
        return;

      runStatsCurDirectory = di.FullName;

      FileInfo[] files = null;
      try  {files = di.GetFiles ("*.bmp");}  catch (Exception)  {files = null;}
      if  (files != null)
      {
        foreach  (FileInfo fi in files)
        {
          if  (cancelReconciling)
            break;
          String  fullFileName = fi.FullName;
          ProcessOneImage (fullFileName);
        }
      }

      if  (!cancelReconciling)
      {

        DirectoryInfo[]  subDirectories = null;

        try  {subDirectories = di.GetDirectories ();} catch (Exception)  {subDirectories = null;}
        if  (subDirectories != null)
        {
          foreach  (DirectoryInfo  subDirectory in subDirectories)
          {
            if  (cancelReconciling)
              break;
            ProcessDirectory (subDirectory);
          }
        }

      }
    }  /* ProcessDirectory */



    private  void  ProcessOneImage (String  fullImageFileName)
    {
      String  rootName = OSservices.GetRootName (fullImageFileName);
      PicesDataBaseImage  i = dbConn.ImageLoad (rootName);
      if  (i != null)
      {
        runStatsImagesInDataBase++;
        return;
      }


      String  sipperFileName = null;
      uint  scanLine = 0;
      uint  scanCol  = 0;
      PicesFeatureVector.ParseImageFileName (fullImageFileName, ref sipperFileName, ref scanLine, ref scanCol);

      if  (String.IsNullOrEmpty (sipperFileName)  ||  (scanLine < 1))
      {
        // We are not going to beable to locate this image in the Database.
        runStatsImagesNotInDataBase++;
        reconcilingRunLog.Writeln (fullImageFileName + "\t" + "Name format will not let me derive needed info.");
        return;
      }

      String  sqlStr = "call  ImagesLocateClosestImage(" + "\"" + sipperFileName + "\"" + ", " + scanLine.ToString () + ", " + scanCol.ToString () + ")";

      String[][]  results = dbConn.QueryStatement (sqlStr, null);
      if  ((results == null)  ||  (results.Length < 1))
      {
        runStatsImagesNotInDataBase++;
        reconcilingRunLog.Writeln (fullImageFileName + "\t" + "Could not find an appropriate image in the Database");

        RemoveImage (fullImageFileName);
        return;
      }

      String  correctImageFileName = results[0][1];
      String  newFullName = OSservices.AddSlash (OSservices.GetPathPartOfFile (fullImageFileName)) +
                            correctImageFileName + "." +
                            OSservices.GetFileExtension (fullImageFileName);

      OSservices.RenameFile (fullImageFileName, newFullName);
      runStatsImagesFixed++;
    }  /* ProcessOneImage */


    private  void  RemoveImage (String  fullImageFileName)
    {
      String  dir = OSservices.GetPathPartOfFile (fullImageFileName);
      String  rootNameWithExt = OSservices.GetRootNameWithExtension (fullImageFileName);
      String  subDir = "";
      if  (SourceDirectory.Text.Length < dir.Length)
      {
        subDir = dir.Substring (SourceDirectory.Text.Length);
        if  ((subDir.Length > 0)  &&  (subDir[0] == '\\'))
          subDir = subDir.Substring (1);
      }

      String  destDir = removalDestRootDir;
      if  (subDir.Length > 0)
      {
        destDir = OSservices.AddSlash (destDir) + subDir;
        OSservices.CreateDirectoryPath (destDir);
      }

      FileInfo fi = null;
      
      try  {fi = new FileInfo (fullImageFileName);}  catch (Exception) {fi = null;}
      if  (fi != null)
      {
        String  destFileName =  OSservices.AddSlash (destDir) + rootNameWithExt;
        try  
        {
          fi.MoveTo (destFileName);
          reconcilingRunLog.Writeln (rootNameWithExt + "\t" + "Removed");
        }
        catch (Exception e)  
        {
          reconcilingRunLog.Writeln (rootNameWithExt + "\t" + "Error Removing[" + e.ToString () + "]");
          MessageBox.Show ("RemoveImage[" + fullImageFileName + "]" + "\n\n" + e.ToString ());
        }
      }
    }  /* RemoveImage*/



    private void SourceDirectoryBrowse_Click (object sender, EventArgs e)
    {
      FolderBrowserDialog  fbd = new FolderBrowserDialog ();
      fbd.SelectedPath = SourceDirectory.Text;

      DialogResult dr = fbd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        SourceDirectory.Text = fbd.SelectedPath;
        ValidateSourceDirectory ();
      }

    }

    private void RunStatsTimer_Tick (object sender, EventArgs e)
    {
      RunStatsCurDirectory_.Text         = runStatsCurDirectory;
      RunStatusTotalImages_.Text         = runStatsTotalImages.ToString         ("###,###,##0");
      RunStatsImagesInDataBase_.Text     = runStatsImagesInDataBase.ToString    ("###,###,##0");
      RunStatsImagesNotInDataBase_.Text  = runStatsImagesNotInDataBase.ToString ("###,###,##0");
      RunStatsImagesFixed_.Text          = runStatsImagesFixed.ToString         ("###,###,##0");

      int  totalImagesProcessed = runStatsImagesInDataBase + runStatsImagesNotInDataBase + runStatsImagesFixed;

      if  (totalImagesProcessed > runStatsTotalImages)
        runStatsTotalImages = totalImagesProcessed;

      if  (runStatsTotalImages > 10)
      {
        ReconciliationProgress.Maximum = runStatsTotalImages;
        ReconciliationProgress.Value   = totalImagesProcessed;
      }

      if  (!reconcilingRunning)
      {
        RunStatsTimer.Enabled = false;
        if  (cancelReconciling)
          MessageBox.Show (this, "Reconciliation   *** CANCELED ***", "Image Filename Reconciliation");
        else
        {
          DialogResult  dr = MessageBox.Show (this, "Reconciliation  Completed!   Want to see RunLog ?  Yes/No ?", "Image Filename Reconciliation", MessageBoxButtons.YesNo);
          if  (dr == DialogResult.Yes)
            reconcilingRunLog.DisplayWithNoteBook ();
        }

        if  (!formIsClosing)
          Close ();
      }
    }


    private void  Start_Click (object sender, EventArgs e)
    {
      ValidateAll ();
      if  (validationErrorsFound)
        return;
      StartReconciliationThread ();
    }
    

    private void  Cancel_Click (object sender, EventArgs e)
    {
      if  (!reconcilingRunning)
      {
        Cancel.Enabled = false;
        return;
      }

      DialogResult dr = MessageBox.Show (this, "So you want to cancel Image File Name Reconciliation ?    Yes/No ?", "Cancel Reconciliation", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
        return;

      Cancel.Enabled = false;

      CancelRecondiliationThread ();
    }



    private void SourceDirectory_Validating (object sender, CancelEventArgs e)
    {
      ValidateSourceDirectory ();
    }



    private void ReconcileImagesToDataBase_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (reconcilingRunning)
      {
        if  (e.CloseReason == CloseReason.UserClosing)
        {
          DialogResult dr = MessageBox.Show (this, "Reconciliation Process is still runnning!   Cancel Process Yes/No ?", "Cancel Reconciliation", MessageBoxButtons.YesNo);
          if  (dr == DialogResult.No)
          {
            e.Cancel = true;
            return;
          }
        }

        formIsClosing = true;
        CancelRecondiliationThread ();

        if  (dbConn != null)
        {
          dbConn.Close ();
          dbConn = null;
          GC.Collect ();
        }
      }
    }

    private void RemoveImagesThatAreNotInDatabase_Validating(object sender, CancelEventArgs e)
    {
      ValidateRemoveImagesThatAreNotIndatabase ();
    }  
  }
}
