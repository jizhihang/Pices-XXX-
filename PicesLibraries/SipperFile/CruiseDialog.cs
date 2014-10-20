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
  public partial class CruiseDialog : Form
  {
    private  bool               addingNewCruise  = false;
    private  bool               deletingCruise   = false;
    private  bool               cruiseAdded      = false;
    private  bool               cruiseDeleted    = false;

    private  PicesSipperCruise  cruise  = null;
    private  PicesDataBase      dbConn  = null;
    private  bool               weOwnDbConn = false;
    private  PicesRunLog        runLog  = null;
    private  bool               validationErrorsFound = false;

    private  bool               allowUpdates = false;


    public  bool  CruiseDeleted  {get {return  cruiseDeleted;}}
    public  bool  CruiseAdded    {get {return  cruiseAdded;}}


    private  PicesDataBase  DbConn ()
    {
      if  (dbConn == null)
      {
        weOwnDbConn = true;
        dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      }
      return  dbConn;
    }


    public  CruiseDialog (String        _cruiseName,
                          PicesRunLog   _runLog
                         )
    {
      runLog = _runLog;
      weOwnDbConn = true;

      cruise = DbConn ().SipperCruiseLoad (_cruiseName);
      if  (cruise == null)
      {
        cruise = new PicesSipperCruise ();
        cruise.CruiseName = _cruiseName;
        addingNewCruise = true;
      }

      InitializeComponent();

      allowUpdates = PicesSipperVariables.AllowUpdates ();
    }



    public CruiseDialog(PicesSipperCruise  _cruise,
                        PicesDataBase      _dbConn,
                        PicesRunLog        _runLog,
                        bool               _addingNewCruise,
                        bool               _deletingCruise
                       )
    {
      cruise = _cruise;
      dbConn = _dbConn;
      runLog = _runLog;
      addingNewCruise = _addingNewCruise;
      deletingCruise  = _deletingCruise;
      InitializeComponent();
    }



    private void CruiseDialog_Load(object sender, EventArgs e)
    {
      if  (runLog == null)
        runLog = new PicesRunLog ();
      
      allowUpdates = PicesSipperVariables.AllowUpdates ();

      PopulateScreen ();
      LoadCruiseDetails ();
      EnableFields ();
    }

    
    private  void  PopulateScreen ()
    {
      TitleLabel.Text = "Cruise " + cruise.CruiseName;
      CruiseName.Text     = cruise.CruiseName;
      ShipName.Text       = cruise.ShipName;
      Description.Text    = cruise.Description;
      DateStart.Value     = cruise.DateStart;
      DateEnd.Value       = cruise.DateEnd;
      CruiseLocation.Text = cruise.Location;
      Objective.Text      = cruise.Objective;
      Principal.Text      = cruise.Principal;
      ResearchOrg.Text    = cruise.ResearchOrg;
    }  /* PopulateScreen */



    private  void  EnableFields ()
    {
      if  (allowUpdates)
        UpdateButon.Enabled   = true;
      CancelReqButton.Enabled = true;
      CruiseName.Enabled      = true;
      ShipName.Enabled        = true;
      Description.Enabled     = true;
      DateStart.Enabled       = true;
      DateEnd.Enabled         = true;
      CruiseLocation.Enabled  = true;
      Objective.Enabled       = true;
      Principal.Enabled       = true;
      ResearchOrg.Enabled     = true;

      if  (addingNewCruise)
      {
        CruiseName.Enabled  = true;
        CruiseName.ReadOnly = false;
        CruiseName.TabStop  = true;
        UpdateButon.Text = "Add";
        AddStationButton.Enabled = false;
      }
      else if  (deletingCruise)
      {
        UpdateButon.Text = "Delete";
        CruiseName.ReadOnly      = true;
        ShipName.ReadOnly        = true;
        Description.ReadOnly     = true;
        DateStart.Enabled        = false;
        DateEnd.Enabled          = false;
        CruiseLocation.ReadOnly  = true;
        Objective.ReadOnly       = true;
        Principal.ReadOnly       = true;
        ResearchOrg.ReadOnly     = true;
        AddStationButton.Enabled = false;
      }
      else
      {
        CruiseName.ReadOnly = true;
        CruiseName.TabStop = false;
        if  (allowUpdates)
        {
          AddStationButton.Enabled = true;
          UpdateButon.Text = "Update";
        }
        else
        {
          UpdateButon.Text = "";
        }
      }
    }  /* EnableFields*/

 

    private  void  LoadCruiseDetails ()
    {
      CruiseDetials.Nodes.Clear ();
      GC.Collect ();
      if  (!addingNewCruise)
      {
        PicesSipperStationList stations = DbConn ().SipperStationLoad (cruise.CruiseName);
        foreach  (PicesSipperStation  station in stations)
          CruiseDetials.Nodes.Add (new TreeNodeStation (dbConn, runLog, station));
      }
    }  /* LoadCruiseDetails */


    
    private  bool  ChangesMade ()
    {
      bool  changesMade = (CruiseName.Text.CompareTo      (cruise.CruiseName)   != 0)  ||
                          (ShipName.Text.CompareTo        (cruise.ShipName)     != 0)  ||
                          (Description.Text.CompareTo     (cruise.Description)  != 0)  ||
                          (DateStart.Value.Date.CompareTo (cruise.DateStart)    != 0)  ||
                          (DateEnd.Value.Date.CompareTo   (cruise.DateEnd)      != 0)  ||
                          (CruiseLocation.Text.CompareTo  (cruise.Location)     != 0)  ||
                          (Objective.Text.CompareTo       (cruise.Objective)    != 0)  ||
                          (Principal.Text.CompareTo       (cruise.Principal)    != 0)  ||
                          (ResearchOrg.Text.CompareTo     (cruise.ResearchOrg)  != 0);
      return  changesMade;
    }
    


    private  void  ValidateCruiseName ()
    {
      errorProvider1.SetError (CruiseName, null);
      if  (CruiseName.Text == null)  CruiseName.Text = "";
      CruiseName.Text = CruiseName.Text.Trim ();
      String  cruiseNameErrorDesc = PicesSipperCruise.ValidateCruiseName (CruiseName.Text);
      if  (cruiseNameErrorDesc != null)
      {
        errorProvider1.SetError (CruiseName, cruiseNameErrorDesc);
        validationErrorsFound = true;
        return;
      }

      if  (CruiseName.Text.CompareTo (cruise.CruiseName) != 0)
      {
        PicesSipperCruise  existingCruise = DbConn ().SipperCruiseLoad (CruiseName.Text);
        if  (existingCruise != null)
        {
          errorProvider1.SetError (CruiseName, "Cruise Name[" + CruiseName.Text + "] is already in use.");
          validationErrorsFound = true;
          return;
        }
      }

      return;
    }  /* ValidateCruiseName */



    private  void  ValidateShipName ()
    {
      errorProvider1.SetError (ShipName, null);
      if  (String.IsNullOrEmpty (ShipName.Text))
      {
        errorProvider1.SetError (ShipName, "Ship Name can not be left blank!");
        validationErrorsFound = true;
      }
    }


    private  void  ValidateDateEnd ()
    {
      errorProvider1.SetError (DateEnd, null);
      if  (DateStart.Value > DateEnd.Value)
      {
        errorProvider1.SetError (DateEnd, "Cruise can not end before it Start's!");
        validationErrorsFound = true;
      }
    }


    private  void  ValidateAllFields ()
    {
      validationErrorsFound = false;
      ValidateCruiseName ();
      ValidateShipName ();
      ValidateDateEnd ();
    }


    private void CruiseName_Validating (object sender, CancelEventArgs e)
    {
      ValidateCruiseName ();
    }


    private void ShipName_Validating (object sender, CancelEventArgs e)
    {
      ValidateShipName ();
    }


    private void DateEnd_Validating(object sender, CancelEventArgs e)
    {
      ValidateDateEnd ();
    }



    private  void  PerformUpdate ()
    {
      if  (!allowUpdates)
      {
        PicesSipperVariables.UpdatesNotAllowed ("Cruise Dialog");
        return;
      }
      cruise.CruiseName  = CruiseName.Text;
      cruise.ShipName    = ShipName.Text;
      cruise.Description = Description.Text;
      cruise.DateStart   = DateStart.Value;
      cruise.DateEnd     = DateEnd.Value;
      cruise.Location    = CruiseLocation.Text;
      cruise.Objective   = Objective.Text;
      cruise.Principal   = Principal.Text;
      cruise.ResearchOrg = ResearchOrg.Text;

      if  (addingNewCruise)
        DbConn ().SipperCruiseInsert (cruise);

      else if  (deletingCruise)
        DbConn ().SipperCruiseDelete (CruiseName.Text);

      else
        DbConn ().SipperCruiseUpdate (cruise);

      if  (DbConn ().Valid ())
      {
        if  (deletingCruise)
          cruiseDeleted = true;
         if  (addingNewCruise)
           cruiseAdded = true;
        Close ();
      }
      else
      {
        MessageBox.Show (DbConn ().LastErrorDesc (),  "Update Failed");
      }
    }  /* PerformUpdate */




    private void UpdateButon_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
        return;

      ValidateAllFields ();
      if  (validationErrorsFound)
        return;

      if  ((!ChangesMade ())  &&  (!deletingCruise))
      {
        MessageBox.Show ("No Changes were Made");
        return;
      }

      PerformUpdate ();
    }



    private void CancelButton_Click(object sender, EventArgs e)
    {
      try {Close ();}
      catch  (Exception closeEcp)  
      {
        MessageBox.Show (this, closeEcp.ToString (), "Excepting Closing CRuise Dialog");
      }
    }



    private void CruiseDialog_FormClosing(object sender, FormClosingEventArgs e)
    {
      if  (ChangesMade ()  &&  (!deletingCruise))
      {
        DialogResult dr = MessageBox.Show (this, "You have made change;  are you sure you want to exit ?", "You have made changes", MessageBoxButtons.YesNo);
        if  (dr == DialogResult.No)
          e.Cancel = true;
      }

      if  (weOwnDbConn)
      {
        dbConn.Close ();
        dbConn = null;
        GC.Collect ();
        weOwnDbConn = false;
      }
    }


    private void AddStationButton_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
        return;

      if (this.addingNewCruise)
      {
        MessageBox.Show (this, "Can only add stations after you have updated database with new cruise.", "Add a New Station.", MessageBoxButtons.OK);
        return;
      }
      PicesSipperStation  station = new PicesSipperStation (cruise);
      StationDialog  sd = new StationDialog (station, dbConn, runLog, true, false);
      sd.ShowDialog (this);
      LoadCruiseDetails ();
    }
  }
}
