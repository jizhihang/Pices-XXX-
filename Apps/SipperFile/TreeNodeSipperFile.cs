using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using  PicesInterface;

namespace SipperFile
{
  public  class  TreeNodeSipperFile:  TreeNode
  {
    private  PicesSipperFile  sipperFile = null;


    public  TreeNodeSipperFile (PicesDataBase    _dbConn,
                                PicesSipperFile  _sipperFile
                               )
    {
      sipperFile = _sipperFile;
      Text = ToString ();

      ContextMenuStrip cms = new ContextMenuStrip ();
      cms.Items.Add ("View Sipper File", null, ViewSipperFile);
      cms.Items.Add ("Edit Sipper File Parameters", null, EditSipperFileParameters);
      this.ContextMenuStrip = cms;
    }


    public  override  String  ToString ()
    {
      return  sipperFile.SipperFileName.PadRight (14)        + "  " +
              sipperFile.DateTimeStart.ToString ("HH:mm:ss") + "  " +
              sipperFile.NumScanLines.ToString ("#,###,###,##0").PadLeft (12);
     }


    private  void  ViewSipperFile (Object sender, EventArgs e)
    {
    }

    private  void  EditSipperFileParameters (Object sender, EventArgs e)
    {
      SipperFileDialog sfd = new SipperFileDialog (sipperFile, null);
      sfd.ShowDialog ();
    }
  };  /* TreeNodeSipperFile */
}
