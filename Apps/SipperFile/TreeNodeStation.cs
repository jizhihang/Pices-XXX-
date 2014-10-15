using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;



using  PicesInterface;


namespace SipperFile
{
  public  class  TreeNodeStation: TreeNode
  {
    private  PicesSipperStation  station = null;
    private  PicesDataBase       dbConn  = null;
    private  PicesRunLog         runLog  = null;
    
    private  String              nextDeploymentNum = "A";


    public  TreeNodeStation (PicesDataBase       _dbConn,
                             PicesRunLog         _runLog,
                             PicesSipperStation  _station
                            )
    {
      dbConn  = _dbConn;
      runLog  = _runLog;
      station = _station;
      LoadDeploymentDetails ();

      Text = ToString ();

      ContextMenuStrip cms = new ContextMenuStrip ();
      cms.Items.Add ("Edit Station Parameters", null, EditStationParameters);

      if  (PicesSipperVariables.AllowUpdates ())
      {
        if  (Nodes.Count < 1)
          cms.Items.Add ("Delete Station",      null, DeleteStation);
        cms.Items.Add ("Add a new Deployment",  null, AddANewDeployment);
      }
      this.ContextMenuStrip = cms;
    }


    private  void  LoadDeploymentDetails ()
    {
      nextDeploymentNum = "A";
      this.Nodes.Clear ();
      PicesSipperDeploymentList  deplomyents = dbConn.SipperDeploymentLoad (station.CruiseName, station.StationName);
      if  (deplomyents == null)  return;
      String  lastDeplymentNum = "";
      foreach (PicesSipperDeployment  deployment in deplomyents)
      {
        this.Nodes.Add (new TreeNodeDeployment (dbConn, deployment));
        if  (deployment.DeploymentNum.Length > 0)
          lastDeplymentNum = deployment.DeploymentNum;
      }

      if  (lastDeplymentNum.Length > 0)
      {
        if  ((lastDeplymentNum[0] >= 'A')  &&  (lastDeplymentNum[0] < 'Z'))
        {
          Char  x = lastDeplymentNum[0];
          x++;
          nextDeploymentNum = x.ToString ();
        }
      }
    }  /* LoadDeploymentDetails */



    public  override  String  ToString ()
    {
      String  name = station.CruiseName + "-" + station.StationName;

      return  name.PadRight (12) +  "  " + 
              station.DateTimeStart.ToString ("yyyy-MMM-dd").PadRight (11) + "  " + 
              station.GPSString + "  " + 
              station.Description;
    }

    
    private  void  EditStationParameters (Object sender, EventArgs e)
    {
      if  (station == null)  return;
      StationDialog  sd = new StationDialog (station, dbConn, runLog, false, false);
      sd.ShowDialog ();
      this.Text = this.ToString ();
    }


    private  void  DeleteStation (Object sender, EventArgs e)
    {
     if  (!PicesSipperVariables.AllowUpdates ())
       return;

      if  (station == null) return;
      StationDialog  sd = new StationDialog (station, dbConn, runLog, false, true);
      sd.ShowDialog ();
      this.Text = this.ToString ();
      if  (sd.StationDeleted)
      {
        if  (this.Parent != null)
        {
          this.Parent.Nodes.Remove (this);
          return;
        }

        this.Text = "Station[" + station.StationName + "]   ***Deleted***";
        this.ContextMenuStrip = null;
        this.station = null;
      }
    }  /* DeleteStation */



   private  void  AddANewDeployment (Object sender, EventArgs e)
   {
     if  (!PicesSipperVariables.AllowUpdates ())
       return;

     if  (station == null)  return;
     PicesSipperDeployment  deployment = new PicesSipperDeployment (station.CruiseName, station.StationName);
     deployment.Latitude  = station.Latitude;
     deployment.Longitude = station.Longitude;
     deployment.DeploymentNum = nextDeploymentNum;

     DeploymentDialog  dd = new DeploymentDialog (deployment, true, false);
     dd.ShowDialog ();
     LoadDeploymentDetails ();
     this.Text = this.ToString ();
   }

  };  /* TreeNodeStation */
}

