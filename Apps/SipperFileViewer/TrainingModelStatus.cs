using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using PicesInterface;

namespace SipperFileViewer
{
  public partial class TrainingModelStatus : Form
  {
    private  SipperFileViewer  viewer = null;
    private  PicesRunLog       runLog = null;
    private  int               lastLineCount = 0;
    private  bool              closeForm = false;
    private  string            newModelName = null;




    public TrainingModelStatus(PicesRunLog       _runLog,
                               SipperFileViewer  _viewer
                              )
    {
      runLog   = _runLog;
      viewer   = _viewer;
      InitializeComponent();
    }



    public  void  SetModelName (string  _newModelName)
    {
      newModelName = _newModelName;
    }



    public  void  CloseForm ()
    {
      closeForm = true;
    }


    private void CancelButton_Click(object sender, EventArgs e)
    {
      DialogResult  dr =  MessageBox.Show ("Cancel Training Model Load", "Training Model Load", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        viewer.CancelLoadingModels ();
        CancelLoadButton.Enabled = false;
      }
    }

    private void timer1_Tick (object sender, EventArgs e)
    {
      if  (closeForm)
      {
        timer1.Enabled = false;
        Close ();
        runLog = null;
        return;
      }

      if  (newModelName != null)
      {
        modelName.Text = newModelName;
        newModelName = null;
      }


      lock  (runLog)
      {
        if  (runLog.LineCount () != lastLineCount)
        {
          lastLineCount = runLog.LineCount ();
          String  lastLine = runLog.LastLine ();
          TrainingModelOutput.AppendText (lastLine + "\n");
        }
      }
    }
  }
}