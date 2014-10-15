using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;

using PicesInterface;

namespace SipperFile
{
  public partial class SaveRawCTDData : Form
  {
    private  PicesMsgQueue  msgQueue = null;
    private  PicesRunLog    runLog   = null;

    private  bool  threadIsRunning = false;
    private  bool  threadIsDone    = false;

    private  String  sipperFileName = "";
    private  String  ctdFileName = "";

    private  bool  successful = false;



    public SaveRawCTDData (String  _sipperFileName)
    {
      sipperFileName = _sipperFileName;
      InitializeComponent ();
      msgQueue = new PicesMsgQueue ("SaveRawCtdData");
      runLog   = new PicesRunLog (msgQueue);

      SipperFileName.Text = sipperFileName;
      ctdFileName = OSservices.RemoveExtension (sipperFileName) + "_ctdData.txt";
      CtdFileName.Text = ctdFileName;
    
    }



    Sipper3File  OpenSipperFile (String  sipperFileName)
    {
      bool   validSipper3      = false;

      Sipper3File sipperFile = null;
      
      try
      {
        sipperFile = new Sipper3File (sipperFileName, null);
        validSipper3 = sipperFile.ValidateFormat ();
      }
      catch  (Exception e)
      {
        msgQueue.AddMsg ("Exception openning SipperFile[" + sipperFileName + "]" + "\n" + e.ToString ());
        sipperFile = null;
        validSipper3 = false;
      }
      return   sipperFile;
    }



    private  void  SaveCtdFileDataThread ()
    {
      threadIsRunning = true;
      threadIsDone    = false;

      Sipper3File  sf = OpenSipperFile (sipperFileName);
      if  (sf == null)
      {
        successful = false;
        return;
      }
      
      StreamWriter sw = null;
      try  {sw = new StreamWriter (ctdFileName);}
      catch  (Exception e2)
      {
        msgQueue.AddMsg ("Exception Opening Output File[" + ctdFileName + "]" + "\n" + e2.ToString ());
        sw = null;
      }

      if  (sw == null)
      {
        successful = false;
        sf = null;
        return;
      }

      sf.SaveRawCtdData (sw, msgQueue, ref successful);

      sw.Close ();
      sw = null;

      threadIsDone = true;
      threadIsRunning = false;
    }  /* SaveCtdFileDataThread */


    
    private void timer1_Tick(object sender, EventArgs e)
    {
      if  (msgQueue != null)
      {
        String  nextMsg = msgQueue.GetNextMsg ();
        while  (nextMsg != null)
        {
          RunTimeMsgs.AppendText (nextMsg);
          RunTimeMsgs.AppendText ("\n");
          nextMsg = msgQueue.GetNextMsg ();
        }
      }

      if  (threadIsDone)
      {
        threadIsDone = false;
        if  (successful)
        {
          MessageBox.Show (this, "Save completed successfully", "Save Raw CTD Data", MessageBoxButtons.OK);
        }
        else
        {
          MessageBox.Show (this, "Error occured trying to save CTD Data; refer to log on screen.", "Save Raw CTD Data", MessageBoxButtons.OK);
        }

        Close ();
      }
    }

    private void SaveRawCTDData_Load (object sender, EventArgs e)
    {
      SaveFileDialog  sfd = new SaveFileDialog ();
      sfd.FileName = ctdFileName;

      DialogResult dr = sfd.ShowDialog (this);
      if  (dr != DialogResult.OK)
      {
        this.Close ();
        return;
      }

      ctdFileName = sfd.FileName;
      CtdFileName.Text = ctdFileName;

      Thread  savingThread = new Thread (new ThreadStart (SaveCtdFileDataThread));
      savingThread.Name = "SavingCtdData";
      savingThread.Start ();
    }


    private void SaveRawCTDData_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (threadIsRunning)
      {
        MessageBox.Show (this, "Data is still being saved; wait to completed before closing form.", "Save Raw CTD Data", MessageBoxButtons.OK);
        e.Cancel = true;
        return;
      }
    }
  }
}
