using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using  PicesInterface;

namespace SipperFile
{
  public partial class CruiseMaintenance : Form
  {
    private  int  lastHeight = 0;
    private  int  lastWidth  = 0;

    private  PicesDataBase  dbConn = null;
    private  PicesRunLog    runLog = null;
    private  bool           weOwnDbConn = false;


    private  PicesDataBase  DbConn ()
    {
      if  (dbConn == null)
      {
        weOwnDbConn = true;
        dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      }
      return  dbConn;
    }


    public CruiseMaintenance (PicesDataBase  _dbConn,
                              PicesRunLog    _runLog
                             )
    {
      dbConn = _dbConn;
      runLog = _runLog;
      if  (runLog == null)
        runLog = new PicesRunLog ();
      InitializeComponent();
    }


    private void CruiseMaintenance_Load(object sender, EventArgs e)
    {
      lastHeight = Height;
      lastWidth  = Width;
      LoadCruises ();
    }


    private void  LoadCruises ()
    {
      PicesSipperCruiseList  cruises = DbConn ().SipperCruiseLoadAllCruises ();
      if  (cruises != null)
      {
        foreach  (PicesSipperCruise  cruise in cruises)
        {
          CruiseTreeList.Nodes.Add (new TreeNodeCruise (DbConn (), runLog, cruise));
        }
      }
    }  /* LoadCruises */



    private void CruiseMaintenance_SizeChanged(object sender, EventArgs e)
    {
      if  (lastHeight == 0)
        return;

      int  deltaHeight = Height - lastHeight;
      int  deltaWidth  = Width  - lastWidth;
      lastHeight = Height;
      lastWidth  = Width;

      int  halfWidth = (int)(0.5f + deltaWidth / 2.0);

      TitleLabel.Left = TitleLabel.Left + halfWidth;
      AddNewCruiseButton.Top = AddNewCruiseButton.Top + deltaHeight;
      AddNewCruiseButton.Left = AddNewCruiseButton.Left + halfWidth;
      CruiseTreeList.Height = CruiseTreeList.Height + deltaHeight;
      CruiseTreeList.Width  = CruiseTreeList.Width  + deltaWidth;
    }

    private void AddNewCruiseButton_Click(object sender, EventArgs e)
    {
      if  (!PicesSipperVariables.AllowUpdates ())
      {
        MessageBox.Show (this, "Updates are not allowed.", "Cruise Maintenance", MessageBoxButtons.OK);
        return;
      }

      PicesSipperCruise  newCruise = new PicesSipperCruise ();
      CruiseDialog  cd = new CruiseDialog (newCruise, DbConn (), runLog, true, false);
      cd.ShowDialog (this);
      if  (cd.CruiseAdded)
        CruiseTreeList.Nodes.Add (new TreeNodeCruise (DbConn (), runLog, newCruise));
    }



    private void CruiseMaintenance_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (weOwnDbConn)
      {
        dbConn.Close ();
        dbConn = null;
        GC.Collect ();
      }
    }
  }
}
