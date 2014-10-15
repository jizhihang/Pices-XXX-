using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using  PicesInterface;

namespace PicesCommander
{
  public partial class SipperInstrumentDataExtraction : Form
  {
    PicesDataBase            dbConn         = null;

    String                   cruise         = "";
    String                   station        = "";
    String                   deployment     = "";
    String                   sipperFileName = "";


    PicesRunLog  runLog = null;

    bool    errorsFound = false;

    // 
    Thread  processingThread           = null;
    bool    processingThreadRunning    = false;
    bool    processingThreadCompleted  = false;
    bool    cancelRequested            = false;

    private PicesGoalKeeper         runLogGoalie = null;
    private Queue<String>           runLogMsgQueue = null;  // List of messages that can be added to 'RunLog'.

    PicesInstrumentDataFileManager  instrumentDataFileManager = null;


    
    public  SipperInstrumentDataExtraction 
                             (
                              PicesDataBase  _dbConn,
                              String         _cruise,
                              String         _station,
                              String         _deployment,
                              String         _sipperFileName
                             )
    {
      dbConn         = _dbConn;
      cruise         = (_cruise         == null) ? ""   : _cruise;
      station        = (_station        == null) ? ""   : _station;
      deployment     = (_deployment     == null) ? ""   : _deployment;
      sipperFileName = (_sipperFileName == null) ? ""   : _sipperFileName;

      runLog = new PicesRunLog ();

      runLogGoalie = new PicesGoalKeeper ("Harvesting Run Log");
      runLogMsgQueue = new Queue<String> ();

      InitializeComponent ();
    }



    private void SipperInstrumentDataExtraction_Load(object sender, EventArgs e)
    {
      CruiseName.Text    = cruise;
      StationName.Text   = station;
      DeploymentNum.Text = deployment;
      SipperFile.Text    = sipperFileName;
    }  /* SaveImagesToDisk2_Load */ 




    private  void  RunLogMsgQueueFlush ()
    {
      runLogGoalie.StartBlock ();
      if  (runLogMsgQueue != null)
      {
        while  (runLogMsgQueue.Count > 0)
        {
          String  msg = runLogMsgQueue.Dequeue ();
          RunLogText.AppendText (msg);
        }
      }

      runLogGoalie.EndBlock ();
    }  /* RunLogMsgQueueFlush */




    private  void  RunLogAddMsg (String  msg)
    {
      runLogGoalie.StartBlock ();

      if  (runLogMsgQueue == null)
        runLogMsgQueue = new Queue<String> ();

      runLogMsgQueue.Enqueue (msg);

      runLogGoalie.EndBlock ();
    }  /* RunLogAddMsg */




    private  void  ValidateAll ()
    {
      errorsFound = false;
    }



    private  void  StartProcessingThread ()
    {
      processingThread = new Thread (new ThreadStart (Processing));
      processingThread.Start ();
      Thread.Sleep (10);
      runLogTimer.Enabled = true;
    }  /* StartProcessingThread */



    private  void  Processing ()
    {
      processingThreadRunning   = true;
      PicesDataBase.ThreadInit ();

      RunLogAddMsg ("Retrieving list of SIPPER Files\n");

      dbConn.CancelFlag = false;

      String[]  sipperFileNames = null;
      if  (String.IsNullOrEmpty (sipperFileName))
      {
        sipperFileNames = dbConn.SipperFileGetList (cruise, station, deployment);
      }
      else
      {
        sipperFileNames = new String[1];
        sipperFileNames[0] = sipperFileName;
      }

      if  (sipperFileNames == null)
      {
        RunLogAddMsg ("\n\n  ***ERROR***   Not SIPPETR files names were returned.\n\n");
      }
      else
      {
        ProcessSipperFiles (sipperFileNames);
      }

      PicesDataBase.ThreadEnd ();
      processingThreadCompleted = true;
      processingThreadRunning   = false;
    }  /* Processing */


    void  CancelRequested ()
    {
      cancelRequested = true;
      if  (instrumentDataFileManager != null)
        instrumentDataFileManager.Cancel = true;
      dbConn.CancelFlag = true;
    }


    void  ProcessSipperFiles (String[]  sipperFileNames)
    {
      int  sfIDX = 0;
      while  ((sfIDX < sipperFileNames.Length)  &&  (!cancelRequested))
      {
        String  sfn = sipperFileNames[sfIDX];
        RunLogAddMsg ("\n\n" + "Processing " + (sfIDX + 1) + " of " + sipperFileNames.Length + "  SipperFile[" + sfn + "]\n");
        sfIDX++;

        ProcessSipperFile (sfn);
      }
    }  /* ProcessSipperFiles*/



    void  ProcessSipperFile (String  sfn)
    {
      PicesSipperFile  sipperFile = dbConn.SipperFileRecLoad (sfn);
      if  (sipperFile == null)
      {
        RunLogAddMsg ("\n\n  ***ERROR***      Sipper File[" + sfn + "]  not defined in database.\n\n");
        return;
      }

      if  (instrumentDataFileManager == null)
        instrumentDataFileManager = new PicesInstrumentDataFileManager ();
      PicesInstrumentDataList  data = instrumentDataFileManager.ReExtractInstrumentDataForSipperFile (sfn, sipperFile, runLog);

      if  (cancelRequested)
        return;

      if  (data == null)
      {
        RunLogAddMsg ("\n\n ***ERROR***  Sipper File[" + sfn + "] retuned no Instrument Data.\n\n");
        return;
      }

      RunLogAddMsg ("Saving Instrument Data;  SipperFileName[" + sfn + "].\n");
      dbConn.InstrumentDataSaveListForOneSipperFile (sfn, data);

      if  (cancelRequested)
        return;

      uint  lastScanLine = 0;
      {
        RunLogAddMsg ("Updating Images Table;  SipperFileName[" + sfn + "].\n");
        int  c1 = 0;
        
        foreach (PicesInstrumentData id in data)
        {
          if  (cancelRequested)
            break;

          if  (id.ScanLine > 0)
            dbConn.ImageUpdateInstrumentDataFields (id, sfn, lastScanLine, id.ScanLine);
          lastScanLine = id.ScanLine;

          c1++;
          if  ((c1 % 25) == 0)
            RunLogAddMsg ("Updating Images Table;  SipperFileName[" + sfn + "]   ScanLine[" + lastScanLine.ToString ("###,###,##0") + "]\n");
        }
      }
    }  /* ProcessSipperFile*/





    private void StartButton_Click(object sender, EventArgs e)
    {
      if  (processingThreadRunning)
        return;

      ValidateAll ();
      if  (errorsFound)
        return;
      DialogResult  dr = MessageBox.Show (this, "Start Extracting Instrument Data?", "Extract Instrument Data", MessageBoxButtons.YesNo);

      if  (dr == DialogResult.Yes)
      {
        StartButton.Enabled = false;
        StartProcessingThread ();
      }
    }


    int  lastLineCount = 0;

    private void runLogTimer_Tick(object sender, EventArgs e)
    {
      if  (runLog.LineCount () != lastLineCount)
      {
        RunLogAddMsg (runLog.LastLine () + "\n");
        lastLineCount = runLog.LineCount ();
      }

      RunLogMsgQueueFlush ();
 
      if  (processingThreadCompleted)
      {
        runLogTimer.Enabled = false;
        if  (cancelRequested)
        {
          MessageBox.Show (this, "Instrument Data Extraction Canceled.");
        }
        else
        {
          MessageBox.Show (this, "Instrument Data Extraction Completed.");
        }
        
        Close ();
      }
    }


    private void CancelProcess_Click(object sender, EventArgs e)
    {
      if  (!processingThreadRunning)
        this.Close ();

      DialogResult  dr = MessageBox.Show (this, "Cancel Insrument Data Extraction ?", "Cancel", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        CancelRequested ();
        CancelProcessButton.Enabled = false;
      }
    }




    private void SipperInstrumentDataExtraction_FormClosing(object sender, FormClosingEventArgs e)
    {
      if  (!processingThreadRunning)
        return;

      if  (e.CloseReason == CloseReason.UserClosing)
      {
        DialogResult dr = MessageBox.Show (this, 
                                           "Instrument Data Extraction has not completed!   Are you sure that you want to exit?", 
                                           "Instrument Data Extraction not Completed!",
                                           MessageBoxButtons.YesNo
                                          );
        if  (dr == DialogResult.No)
        {
          e.Cancel = true;
          return;
        }
      }

      CancelRequested ();
      e.Cancel = true;
    }
  }
}
