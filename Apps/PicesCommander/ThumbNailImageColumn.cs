using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;


namespace PicesCommander
{
  public class ThumbNailImageColumn: DataGridViewColumn
  {
    private  IWin32Window  parent = null;

    public  IWin32Window  Parent  {get {return  parent;}}

    public ThumbNailImageColumn (IWin32Window  _parent)
    {
      parent = _parent;
      this.CellTemplate = new ThumbNailImageCell ();
    }
  }
}
