using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;



using  PicesInterface;


namespace SipperFile
{
  public  class  TreeNodeCruise: TreeNode
  {
    private  PicesSipperCruise  cruise  = null;
    private  PicesDataBase      dbConn  = null;
    private  PicesRunLog        runLog  = null;


    public  TreeNodeCruise (PicesDataBase      _dbConn,
                            PicesRunLog        _runLog,
                            PicesSipperCruise  _cruise
                           )
    {
      dbConn = _dbConn;
      runLog = _runLog;
      cruise = _cruise;
      LoadStationDetails ();

      Text = ToString ();

      if  (PicesSipperVariables.AllowUpdates ())
      {
        ContextMenuStrip cms = new ContextMenuStrip ();
        cms.Items.Add ("Edit Cruise Parameters", null, EditCruiseParameters);
        
        if  (Nodes.Count < 1)
          cms.Items.Add ("Delete Cruise", null, DeleteCruise);

        cms.Items.Add ("Add a new Station", null, AddANewStation);
        this.ContextMenuStrip = cms;
      }
    }


    private  void  LoadStationDetails ()
    {
      this.Nodes.Clear ();
      PicesSipperStationList  stations = dbConn.SipperStationLoad (cruise.CruiseName);
      if  (stations == null)  return;
      foreach (PicesSipperStation  station in stations)
        this.Nodes.Add (new TreeNodeStation (dbConn, runLog, station));
    }  /* LoadStationDetails */



    public  override  String  ToString ()
    {
      String  name = cruise.CruiseName;

      name += " " + cruise.DateStart.ToString ("yyyy MMM dd");

      char[] trimChars  = {'\n', '\r', 't'};
      char[] splitChars = {'\n', '\r'};
      String  fullDesc = cruise.Description.Trim (trimChars);
      String[]  descLines = fullDesc.Split (splitChars);
      String  descLine = "";
      if  (descLines.Length >= 1)
        descLine = descLines[0];

      if  (String.IsNullOrEmpty (descLine))
        name += " " + cruise.ShipName;
      else
        name += " " + descLine;

      return  name;
    }

    
    private  void  EditCruiseParameters (Object sender, EventArgs e)
    {
      if  (cruise == null)  return;
      CruiseDialog  sd = new CruiseDialog (cruise, dbConn, runLog, false, false);
      sd.ShowDialog ();
      this.Text = this.ToString ();
      LoadStationDetails ();
    }


    private  void  DeleteCruise (Object sender, EventArgs e)
    {
      if  (!PicesSipperVariables.AllowUpdates ())
        return;

      if  (cruise == null) return;
      CruiseDialog  sd = new CruiseDialog (cruise, dbConn, runLog, false, true);
      sd.ShowDialog ();
      this.Text = this.ToString ();
      if  (sd.CruiseDeleted)
      {
        if  (this.Parent != null)
        {
          this.Parent.Nodes.Remove (this);
          return;
        }

        this.Text = "Cruise[" + cruise.CruiseName + "]   ***Deleted***";
        this.ContextMenuStrip = null;
        this.cruise = null;
      }
    }  /* DeleteCruise */



   private  void  AddANewStation (Object sender, EventArgs e)
   {
      if  (!PicesSipperVariables.AllowUpdates ())
        return;

     if  (cruise == null)  return;
     PicesSipperStation  station = new PicesSipperStation (cruise);
     StationDialog  dd = new StationDialog (station, dbConn, runLog, true, false);
     dd.ShowDialog ();
     LoadStationDetails ();
     this.Text = this.ToString ();
   }

  };  /* TreeNodeCruise */
}

