using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using PicesInterface;

namespace SipperFile
{
  public partial class GroupMaintenance : Form
  {
    PicesDataBase  dbConn = null;
    PicesRunLog    runLog = null;
    bool           allowUpdates = false;


    public GroupMaintenance ()
    {
      allowUpdates = PicesSipperVariables.AllowUpdates ();

      runLog = new PicesRunLog ();
      InitializeComponent ();

      DataGridViewCellStyle  style = ImageGroupGrid.Columns[0].DefaultCellStyle;
      if  (style == null)
      {
        style = new DataGridViewCellStyle ();
        style.Alignment = DataGridViewContentAlignment.MiddleCenter;
      }
      style.Alignment = DataGridViewContentAlignment.MiddleCenter;
      ImageGroupGrid.Columns[0].DefaultCellStyle = style;
      ImageGroupGrid.Columns[3].DefaultCellStyle = style;
    }



    private void  LoadGroups ()
    {
      ImageGroupGrid.Rows.Clear ();

      PicesDataBaseImageGroupList  groups = dbConn.ImageGroupLoad ();

      foreach  (PicesDataBaseImageGroup  group in groups)
      {
        DataGridViewRow  dgvr = new DataGridViewRow ();

        Object[]  data = new Object[5];
        data[0] = group.ImageGroupId;
        data[1] = group.Name;
        data[2] = group.Description;
        data[3] = group.Count;
        data[4] = "Delete";
        ImageGroupGrid.Rows.Add (data);
      }
    }  /* LoadGroups */




    private void GroupMaintenance_Load (object sender, EventArgs e)
    {
      dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      LoadGroups ();
    }



    private void ImageGroupGrid_CellClick (object sender, DataGridViewCellEventArgs e)
    {
      DataGridView  dgv = (DataGridView)sender;
      if  (dgv.Columns[e.ColumnIndex].Name != "Delete")
        return;

      if  (!allowUpdates)
      {
        PicesSipperVariables.UpdatesNotAllowed ("Group Maintenance");
        return;
      }

      if  (e.RowIndex < 0)
        return;
      
      PicesDataBaseImageGroup ig = dbConn.ImageGroupLoad ((String)dgv.Rows[e.RowIndex].Cells[1].Value);
      if  (ig == null)
        return;
      DialogResult  dr = MessageBox.Show (this, "Are you sure you want to delete this group ?", "Delete " + ig.Name, MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        dbConn.ImageGroupDelete (ig.ImageGroupId);
        LoadGroups ();
      }
    }



    private void GroupMaintenance_Resize (object sender, EventArgs e)
    {
      int  oldWidth = ImageGroupGrid.Width;
      int  newWidth = Width  - (12 + 2 * ImageGroupGrid.Left);
      int  deltaWidth = newWidth - oldWidth;
      
      ImageGroupGrid.Height = Height - (20 + 2 * ImageGroupGrid.Top);
      ImageGroupGrid.Width  = newWidth;
      
      DataGridViewColumn  col = null;
      try  {col = ImageGroupGrid.Columns["GroupDescription"];}
      catch (Exception) {col = null;}
      if  (col != null)
      {
        int  oldDescWidth = col.Width;
        int  newDescWidth = oldDescWidth + deltaWidth;
        if  (newDescWidth > 100)
          col.Width = newDescWidth;
      }

    }
  }  /* GroupMaintenance */
}
