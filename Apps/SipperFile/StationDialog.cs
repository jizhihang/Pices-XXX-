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
  public partial class StationDialog : Form
  {
    private  bool                addingNewStation  = false;
    private  bool                deletingStation   = false;

    private  PicesDataBase       dbConn = null;
    private  bool                weOwnDbConn = false;
    private  PicesRunLog         runLog = null;

    private  PicesSipperStation  station = null;
    private  bool                validationErrorsFound = true;

    private  bool                stationDeleted = false;

    private  bool                allowUpdates = false;



    public   bool    StationDeleted  {get {return stationDeleted;}}


    private  PicesDataBase  DbConn ()
    {
      if  (dbConn == null)
      {
        weOwnDbConn = true;
        dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      }
      return  dbConn;
    }


    public StationDialog (PicesSipperStation  _station,
                          PicesDataBase       _dbConn,
                          PicesRunLog         _runLog,
                          bool                _addingNewStation,
                          bool                _deletingStation
                         )
    {
      station          = _station;
      dbConn           = _dbConn;
      runLog           = _runLog;
      addingNewStation = _addingNewStation;
      deletingStation  = _deletingStation;

      if  (runLog == null)
        runLog = new PicesRunLog ();

      InitializeComponent();

      allowUpdates = PicesSipperVariables.AllowUpdates ();
    }


    private void PopulateScreen()
    {
      CruiseName.Text = station.CruiseName;
      StationName.Text = station.StationName;
      if (addingNewStation)
      {
        StationName.ReadOnly = false;
        StationName.TabStop  = true;
        UpdateButton.Text    = "Add";
        AddDeploymentButton.Enabled = false;
      }
      else
      {
        StationName.ReadOnly = true;
        StationName.TabStop  = false;
        UpdateButton.Text    = "Update"; 
        AddDeploymentButton.Enabled = true;
        LoadDeploymentDetails ();
      }


      Description.Text = station.Description;
      Latitude.Text  = PicesKKStr.LatitudeToStr  (station.Latitude);
      Longitude.Text = PicesKKStr.LongitudeToStr (station.Longitude);
      DateTimeStart.Value = station.DateTimeStart;

      if  (deletingStation)
      {
        StationName.ReadOnly  = true;
        Description.ReadOnly  = true;
        Latitude.ReadOnly     = true;
        Longitude.ReadOnly    = true;
        DateTimeStart.Enabled = false;
        AddDeploymentButton.Enabled = false;
        UpdateButton.Text = "Delete";
      }

      if  (!allowUpdates)
      {
        UpdateButton.Enabled = false;
        AddDeploymentButton.Enabled = false;
      }
    }  /* PopulateScreen */


 
    private void  PopulateStationFromScreen()
    {
      station.StationName   = StationName.Text;
      station.Description   = Description.Text;
      station.Latitude      = PicesKKStr.StrToLatitude  (Latitude.Text);
      station.Longitude     = PicesKKStr.StrToLongitude (Longitude.Text);
      station.DateTimeStart = DateTimeStart.Value;
    }  /* PopulateStationFromScreen*/


    private  void  LoadDeploymentDetails ()
    {
      if  (addingNewStation)
        return;
      Deployments.Nodes.Clear ();
      PicesSipperDeploymentList  deplomyents = DbConn ().SipperDeploymentLoad (station.CruiseName, station.StationName);
      if  (deplomyents == null)  return;
      foreach (PicesSipperDeployment  deployment in deplomyents)
        Deployments.Nodes.Add (new TreeNodeDeployment (DbConn (), deployment));
    }  /* LoadDeploymentDetails */



    private void  ValidateStationName()
    {
      errorProvider1.SetError (StationName, null);
      String s = StationName.Text.Trim ();
      if  (String.IsNullOrEmpty (s))
      {
        errorProvider1.SetError (StationName, "Can not leave  'Station Name'  blank.");
        validationErrorsFound = true;
        return;
      }

      String invalidCharacters = "";
      for  (int x = 0;  x < s.Length;  x++)
      {
        char  ch = char.ToUpper (s[x]);
        bool validChar = ((ch >= '0')  &&  (ch <= '9'))  ||  ((ch >= 'A')  &&  (ch <= 'Z'));
        if  (!validChar)
        {
          if  (invalidCharacters.Length > 0)
            invalidCharacters += ", ";
          invalidCharacters += s[x].ToString ();
        }
      }

      if  (invalidCharacters.Length > 0)
      {
        errorProvider1.SetError (StationName, "Invalid characters[" + invalidCharacters + "]");
        validationErrorsFound = true;
        return;
      }

      if  ((addingNewStation)  ||  (s != station.StationName))
      {
        PicesSipperStation  existingStation = DbConn ().SipperStationLoad (CruiseName.Text, s);
        if  (existingStation != null)
        {
          errorProvider1.SetError (StationName, "StationName[" + s + "] is already been used for Cruise[" + CruiseName.Text + "].");
          validationErrorsFound = true;
          return;
        }
      }

      StationName.Text = s;
    }  /* ValidateStationName */



    private  void  ValidateLatitude ()
    {
      errorProvider1.SetError (Latitude, null);
      double latitude = PicesKKStr.StrToLatitude (Latitude.Text);
      Latitude.Text = PicesKKStr.LatitudeToStr (latitude);
    }


    private  void  ValidateLongitude ()
    {
      errorProvider1.SetError (Longitude, null);
      double longitude = PicesKKStr.StrToLatitude (Longitude.Text);
      Longitude.Text = PicesKKStr.LongitudeToStr (longitude);
    }


    private  void  ValidateAllFields ()
    {
      validationErrorsFound = false;
      ValidateStationName ();
      ValidateLatitude ();
      ValidateLongitude ();
    }


    private  bool  ChangesMade ()
    {
      bool  changesMade = (StationName.Text                          != station.StationName)    ||
                          (Description.Text                          != station.Description)    ||
                          (PicesKKStr.StrToLatitude (Latitude.Text)  != station.Latitude)       ||
                          (PicesKKStr.StrToLongitude(Longitude.Text) != station.Longitude)      ||
                          (DateTimeStart.Value                       != station.DateTimeStart);
      return  changesMade;
    }


    private  void  PerformUpdate ()
    {
      PopulateStationFromScreen ();
      if  (addingNewStation)
        DbConn ().SipperStationInsert (station);

      else if  (deletingStation)
      {
        DbConn ().SipperStationDelete (station.CruiseName, station.StationName);
        if  (DbConn ().Valid ())
          stationDeleted = true;
      }

      else
        DbConn ().SipperStationUpdate (station);

      if  (!DbConn ().LastOpSuccessful)
      {
        MessageBox.Show (this, "Error occured udating Database.\n\n" + DbConn ().LastErrorDesc (), "Station Table Update Error", MessageBoxButtons.OK);
      }
    }  /* PerformUpdate*/



    private void StationDialog_Load(object sender, EventArgs e)
    {
      allowUpdates = PicesSipperVariables.AllowUpdates ();
      PopulateScreen ();
    }


    private void UpdateButton_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
        return;

      if  ((!ChangesMade ())  &&  (!deletingStation))
      {
        MessageBox.Show ("No changes have been made", "Update");
        return;
      }

      ValidateAllFields ();
      if  (!validationErrorsFound)
      {
        PerformUpdate ();
        Close ();
      }
    }


    private void CancelReqButton_Click(object sender, EventArgs e)
    {
      if  (ChangesMade ())
      {
        DialogResult dr = MessageBox.Show ("Changes have been made.  Do you still want to exit.", 
                                           "Cancel", 
                                           MessageBoxButtons.YesNo
                                          );
        if  (dr == DialogResult.No)
          return;
      }
      Close ();
    }


    private void StationName_Validating(object sender, CancelEventArgs e)
    {
      ValidateStationName ();
    }

    private void Latitude_Validating(object sender, CancelEventArgs e)
    {
      ValidateLatitude ();
    }

    private void Longitude_Validating(object sender, CancelEventArgs e)
    {
      ValidateLongitude ();
    }

    private void AddDeploymentButton_Click (object sender, EventArgs e)
    {
      if  (!allowUpdates)
        return;

      if (this.addingNewStation)
      {
        MessageBox.Show (this, "Can only add deployments after the Station entry has been inserted into database.", "Add a New Station.", MessageBoxButtons.OK);
        return;
      }
      PicesSipperDeployment  deployment = new PicesSipperDeployment (station.CruiseName, station.StationName);
      deployment.Latitude = station.Latitude;
      deployment.Longitude = station.Longitude;
      if  (Deployments.Nodes.Count < 1)
        deployment.DeploymentNum = "A";

      DeploymentDialog  dd = new DeploymentDialog (deployment, true, false);
      dd.ShowDialog (this);
      LoadDeploymentDetails ();
    }


    private void StationDialog_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (weOwnDbConn)
      {
        dbConn.Close ();
        dbConn = null;
        GC.Collect();
        weOwnDbConn = false;
      }
    }
  }
}
