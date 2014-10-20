using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using  PicesInterface;


namespace SipperFile
{
  public  class  TreeNodeDeployment:  TreeNode
  {
    private  PicesSipperDeployment  deployment = null;
    private  PicesDataBase          dbConn = null;

    private  bool                   allowUpdates = false;

    public  TreeNodeDeployment (PicesDataBase          _dbConn,
                                PicesSipperDeployment  _deployment
                               )
    {
      allowUpdates = PicesSipperVariables.AllowUpdates ();
      dbConn     = _dbConn;
      deployment = _deployment;
      Text = ToString ();
      ContextMenuStrip cms = new ContextMenuStrip ();
      cms.Items.Add ("Edit Deployment Parameters", null, EditDeploymentParameters);
      if  ((this.Nodes.Count < 1)  &&  allowUpdates)
        cms.Items.Add ("Delete Deployment", null, DeleteDeployment);
      this.ContextMenuStrip = cms;
      LoadSipperFileDetails ();
    }



    private  void  LoadSipperFileDetails ()
    {
      PicesSipperFileList  sipperFiles = dbConn.SipperFileLoad (deployment.CruiseName, deployment.StationName, deployment.DeploymentNum);
      if  (sipperFiles != null)
      {
        foreach  (PicesSipperFile sf in sipperFiles)
          this.Nodes.Add (new TreeNodeSipperFile (dbConn, sf));
      }
    }



    public  override  String  ToString ()
    {
      if  (deployment == null)
        return  "***deleted***";

      String  name = deployment.CruiseName + "-" + deployment.StationName + "-" + deployment.DeploymentNum;

      return name.PadRight (14) + "  " + 
             deployment.DateTimeStart.ToString ("yyyy-MMM-dd HH:mm") + " thru " + deployment.DateTimeEnd.ToString ("HH:mm") + "  " + 
             deployment.GPSString + "  " + deployment.Description;
    }



    private  void  EditDeploymentParameters (Object sender, EventArgs e)
    {
      DeploymentDialog sdd = new DeploymentDialog (deployment, false, false);
      sdd.ShowDialog ();
      Text = ToString ();
    }


    private  void  DeleteDeployment (Object sender, EventArgs e)
    {
      if  (!allowUpdates)
        return;

      DeploymentDialog sdd = new DeploymentDialog (deployment, false, true);
      sdd.ShowDialog ();
      if  (sdd.DeploymentDeleted)
      {
        if  (this.Parent != null)
        {
          Parent.Nodes.Remove (this);
          return;
        }
        this.ContextMenuStrip = null;
        Text = "Deployment[" + deployment.DeploymentNum + "]   ***Deleted***";
        deployment = null;
      }
    }  /* DeleteDeployment */


  };  /* DeploymentNode */
}
