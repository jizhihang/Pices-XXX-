using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SipperFile
{
  public partial class DataRowListLoadStatus : Form
  {
    public DataRowListLoadStatus (string  _fileName)
    {
      InitializeComponent ();
      fileName.Text = _fileName;
    }
    
    public  void  Update (DateTime  _dateTime,
                          int       _recordCount
                         )
    {
      dateTime.Text    = _dateTime.ToString ("g");
      recordCount.Text = _recordCount.ToString ();
      this.Refresh ();
    }  /* Update */
  }
}