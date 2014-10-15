using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;


using SipperFile;




using PicesInterface;

namespace SipperFile
{
  public partial class TrainingModelStatus : Form
  {
    public  delegate  void  AlertCompletion (TrainingModelStatus  sender);


    private  String          trainingModelName    = null;
    private  TrainingModel2  trainingModel        = null; 
    private  Thread          trainingModelThread  = null;
    private  bool            trainingModelRunning = false;
    private  bool            forceRebuild         = false;
    private  bool            forceCancelation     = false;  
    private  bool            cancelFlag           = false;
    private  DateTime        cancelFlagStartTime;
    private  bool            validModel           = false;
    private  PicesRunLog     runLog               = new PicesRunLog ();
    private  String          trainingModelLogFileName  = null;

    private  Queue<String>   logMessages          = new  Queue<String> ();
    private  PicesGoalKeeper logGoalie            = null;


    private  AlertCompletion  completionEvent;


    public TrainingModelStatus (String           _trainingModelName,
                                bool             _forceRebuild,
                                AlertCompletion  _completionEvent
                               )
    {
      InitializeComponent();
      trainingModelName = _trainingModelName;
      forceRebuild      = _forceRebuild;
      completionEvent   = _completionEvent;

      logGoalie = new PicesGoalKeeper ("TrainingModelStatus-" + OSservices.GetRootName (_trainingModelName));
    }

   

    public  TrainingModel2  TrainiedModel         ()  {return  trainingModel;}
    public  bool            TrainingModelRunning  ()  {return  trainingModelRunning;}
    public  bool            ValidModel            ()  {return  validModel;}





    public  void  CleanUpMemory ()
    {
      timer1.Enabled = false;
      if  (trainingModelRunning)
      {
        if  (!cancelFlag)
          CancelLoad ();
        int  x = 0;
        while  ((x < 100)   &&  (trainingModelRunning))
        {
          Thread.Sleep (100);
        }

        ForceCancelation ();
      }
    }   /* CleanUpMemory */





    private void TrainingModelStatus_Load (object sender, EventArgs e)
    {
      ModelName.Text = trainingModelName;
      StartLoadingModel ();
    }



    public void  CancelLoad ()
    {
      cancelFlag = true;
      cancelFlagStartTime = DateTime.Now;
      if  (trainingModel != null)
        trainingModel.CancelLoad ();
    }  /* CancelLoad*/




    public  void  ForceCancelation ()
    {
      // This is going to be a very ungracefull way to cancel. I am
      // I am going to attempt to kill the thread.

      trainingModelThread.Abort ();
      forceCancelation = true;

      Thread.Sleep (200);
    }  /* ForceCancelation */



    private  void  LogMessagesAdd (String  msg)
    {
      logGoalie.StartBlock ();
      if  (logMessages == null)
        logMessages = new Queue<string> ();
      logMessages.Enqueue (msg);
      logGoalie.EndBlock ();
    }


    private  String  LogMessageGetNext ()
    {
      String  msg = null;
      logGoalie.StartBlock ();
      if  ((logMessages != null)  &&  (logMessages.Count > 0))
        msg = logMessages.Dequeue ();
      logGoalie.EndBlock ();
      return msg;
    }



    private void  LogMessageFlush ()
    {
      String msg = LogMessageGetNext();
      while (msg != null)
      {
        TrainingModelOutput.AppendText(msg);
        TrainingModelOutput.AppendText("\n");
        msg = LogMessageGetNext();
      }
    }

    
    private  void  StartLoadingModel ()
    {
      trainingModelThread = new Thread (new ThreadStart (LoadTrainingModel));
      trainingModelThread.Start ();
      timer1.Enabled = true;
    }


    private  void  LoadTrainingModel ()
    {
      trainingModelRunning = true;
      PicesDataBase.ThreadInit ();

      if  (trainingModel != null)
      {
        trainingModel = null;
        GC.Collect ();
      }

      trainingModel = new TrainingModel2 (runLog, trainingModelName);
      trainingModelLogFileName = trainingModel.RunLogFileName;

      if  (forceRebuild)
        trainingModel.LoadTrainigLibrary (true);  // true = Force Rebuild of Model
      else
        trainingModel.LoadExistingModelOtherwiseBuild (null);

      if  (trainingModel.Valid)
      {
        validModel = true;
      }
      else
      {
        List<String>  errMsgs = trainingModel.ErrorMsgs;
        if  (errMsgs != null)
          foreach (String s in errMsgs)
            LogMessagesAdd (s);

        validModel = false;
        trainingModel = null;
      }

      PicesDataBase.ThreadEnd ();
      trainingModelRunning = false;
    }  /* LoadTrainingModel */




    private void timer1_Tick (object sender, EventArgs e)
    {
      if  (forceCancelation)
      {
        // We want to just kill the TrainingModel2 thread.  By this time
        // the abort function should have been called in 'ForceCancelation'.
        // So we are just going to exit the program now.
        Thread.Sleep (200);
        trainingModelThread.Priority = ThreadPriority.Lowest;
        trainingModelThread = null;
        if  (trainingModel != null)
          trainingModel.CancelLoad ();
        Close ();
        timer1.Enabled = false;
        return;
      }


      if  (trainingModel != null)
      {
        LogMessageFlush();
        String  msg = trainingModel.LastRunLogTextLine ();
        if  (msg != null)
        {
          TrainingModelOutput.AppendText (msg);
          TrainingModelOutput.AppendText ("\n");
        }
      }

      if  (!trainingModelRunning) 
      {
        timer1.Enabled = false;

        // The Training Model thread is done; 
        if  (cancelFlag)
        {
          LogMessageFlush ();
          MessageBox.Show ("Training Model[" + trainingModelName + "] load has been canceled.");
        }

        else if  (!validModel)
        {
          LogMessageFlush();
          DialogResult dr = MessageBox.Show("Training Model[" + trainingModelName + "] failed to load; Press 'Yes' to see Log Messages.", 
                                             "Training Model Status", 
                                             MessageBoxButtons.YesNo
                                            );
          if  (dr == DialogResult.Yes)
          {
            Process proc = new Process();
     
            proc.StartInfo.WorkingDirectory = @"";
            proc.StartInfo.FileName = "notepad.exe";
            proc.StartInfo.Arguments = trainingModelLogFileName;
            proc.StartInfo.UseShellExecute = false;
            proc.StartInfo.RedirectStandardOutput = false;
            proc.StartInfo.RedirectStandardError = true;
            proc.Start();
            proc.WaitForExit();
            proc.Close();
          }
        }

        Close ();
        completionEvent (this);
      }
    }  /* timer1_Tick*/



    private void CancelLoadButton_Click(object sender, EventArgs e)
    {
      CancelLoadButton.Enabled = false;

      DialogResult dr;

      if  (cancelFlag)
      {
         dr = MessageBox.Show ("Cancel has alreadty been requested.   Do you wish to force cancelaion Yes/No ?",
                               "Training Model Status", 
                               MessageBoxButtons.YesNo
                              );
        if  (dr == DialogResult.Yes)
          ForceCancelation ();
        else
          CancelLoadButton.Enabled = true;
        return;
      }
   
      dr = MessageBox.Show ("Cancel Training Model[" + trainingModelName + "] Load Yes/No ?", 
                            "Training Model Status", 
                            MessageBoxButtons.YesNo
                           );
      if  (dr == DialogResult.Yes)
        CancelLoad ();

      CancelLoadButton.Enabled = true;
    }


    private void TrainingModelStatus_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (trainingModelRunning)
      {
        e.Cancel = true;
        CancelLoadButton_Click (sender, e);
        return;
      }
    }  /* TrainingModelStatus_FormClosing */
  }
}