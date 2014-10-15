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
  public partial class DeploymentDialog : Form
  {
    private  bool                   addingNewDeployment  = false;
    private  bool                   deletingDeployment   = false;

    private  PicesDataBase          dbConn  = null;
    private  bool                   weOwnDbConn = false;
    private  PicesRunLog            runLog = null;

    private  PicesSipperDeployment  deployment = null;
    private  bool                   validationErrorsFound = true;

    private  bool                   deploymentDeleted = false;

    private  String                 lastSipperFileDir = "";

    private  bool                   allowUpdates = false;



    public   bool    DeploymentDeleted  {get {return deploymentDeleted;}}



    private  PicesDataBase  DbConn ()
    {
      if  (dbConn == null)
      {
        weOwnDbConn = true;
        dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      }
      return  dbConn;
    }



    public DeploymentDialog (PicesSipperDeployment _deployment,
                             bool                  _addingNewDeployment,
                             bool                  _deletingDeployment
                            )
    {
      allowUpdates = PicesSipperVariables.AllowUpdates ();
      deployment          = _deployment;
      addingNewDeployment = _addingNewDeployment;
      deletingDeployment  = _deletingDeployment;

      runLog = new PicesRunLog ();

      InitializeComponent();
    }


    private void PopulateScreen()
    {
      CruiseName.Text    = deployment.CruiseName;
      StationName.Text   = deployment.StationName;
      DeploymentNum.Text = deployment.DeploymentNum;
      if (addingNewDeployment)
      {
        DeploymentNum.ReadOnly = false;
        DeploymentNum.TabStop  = true;
      }
      else
      {
        DeploymentNum.ReadOnly = true;
        DeploymentNum.TabStop  = false;
      }
      Description.Text     = deployment.Description;
      Latitude.Text        = PicesKKStr.LatitudeToStr  (deployment.Latitude);
      Longitude.Text       = PicesKKStr.LongitudeToStr (deployment.Longitude);
      DateTimeStart.Value  = deployment.DateTimeStart;
      DateTimeEnd.Value    = deployment.DateTimeEnd;

      SyncTimeStampActual.Value = deployment.SyncTimeStampActual;
      SyncTimeStampCTD.Value    = deployment.SyncTimeStampCTD;
      SyncTimeStampGPS.Value    = deployment.SyncTimeStampGPS;

      if  (deletingDeployment)
      {
        StationName.ReadOnly        = false;
        Description.ReadOnly        = false;
        Latitude.ReadOnly           = false;
        Longitude.ReadOnly          = false;
        DateTimeStart.Enabled       = false;
        DateTimeEnd.Enabled         = false;
        SyncTimeStampActual.Enabled = false;
        SyncTimeStampCTD.Enabled    = false;
        SyncTimeStampGPS.Enabled    = false;
        UpdateButton.Text = "Delete";
      }

      PopulateSipperFiles ();

      if  (!allowUpdates)
      {
        UpdateButton.Enabled = false;
        AddSipperFileButton.Enabled = false;
      }
    }  /* PopulateScreen */



    private  void  PopulateSipperFiles ()
    {
      SipperFiles.Nodes.Clear ();
      PicesSipperFileList  sipperFiles = DbConn ().SipperFileLoad (deployment.CruiseName, deployment.StationName, deployment.DeploymentNum);
      if  (sipperFiles != null)
      {
        foreach  (PicesSipperFile  sf  in  sipperFiles)
          SipperFiles.Nodes.Add (new TreeNodeSipperFile (DbConn (), sf));
      }
    }

 
    private void PopulateDeploymentFromScreen()
    {
      deployment.DeploymentNum       = DeploymentNum.Text;
      deployment.Description         = Description.Text;
      deployment.Latitude            = PicesKKStr.StrToLatitude  (Latitude.Text);
      deployment.Longitude           = PicesKKStr.StrToLongitude (Longitude.Text);
      deployment.DateTimeStart       = DateTimeStart.Value;
      deployment.DateTimeEnd         = DateTimeEnd.Value;
      deployment.SyncTimeStampActual = SyncTimeStampActual.Value;
      deployment.SyncTimeStampCTD    = SyncTimeStampCTD.Value;
      deployment.SyncTimeStampGPS    = SyncTimeStampGPS.Value;
    }  /* PopulateDeploymentFromScreen*/



    private void ValidateDeploymentNum()
    {
      errorProvider1.SetError (DeploymentNum, null);
      if  (!addingNewDeployment)
        return;

      String s = DeploymentNum.Text.Trim ();
      if  (String.IsNullOrEmpty (s))
      {
        errorProvider1.SetError (DeploymentNum, "Can not leave  'DeploymentNum'  blank.");
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
        errorProvider1.SetError (DeploymentNum, "Invalid characters[" + invalidCharacters + "]");
        validationErrorsFound = true;
        return;
      }

      if  (s.Length > 4)
      {
        errorProvider1.SetError (DeploymentNum, "DeploymentNum[" + s + "] is to long;  maximum of 4 characters allowed.");
        validationErrorsFound = true;
        return;
      }

      if  ((addingNewDeployment)  ||  (s != deployment.DeploymentNum))
      {
        PicesSipperDeployment  existingDeployment = DbConn ().SipperDeploymentLoad (CruiseName.Text, StationName.Text, s);
        if  (existingDeployment != null)
        {
          errorProvider1.SetError (DeploymentNum, "DeploymentNum[" + s + "] is already been used for Cruise[" + CruiseName.Text + "], Station[" + StationName.Text + "].");
          validationErrorsFound = true;
          return;
        }
      }

      DeploymentNum.Text = s;
    }  /* ValidateDeploymentNum */



    private  void  ValidateLatitude ()
    {
      errorProvider1.SetError (Latitude, null);
      double latitude = PicesKKStr.StrToLatitude (Latitude.Text);
      Latitude.Text = PicesKKStr.LatitudeToStr (latitude);
    }


    private  void  ValidateLongitude ()
    {
      errorProvider1.SetError (Longitude, null);
      double longitude = PicesKKStr.StrToLongitude (Longitude.Text);
      Longitude.Text = PicesKKStr.LongitudeToStr (longitude);
    }


    private  void  ValidateAllFields ()
    {
      validationErrorsFound = false;
      ValidateDeploymentNum ();
      ValidateLatitude ();
      ValidateLongitude ();
    }


    private  bool  ChangesMade ()
    {
      bool  changesMade = (DeploymentNum.Text                        != deployment.DeploymentNum)       ||
                          (Description.Text                          != deployment.Description)         ||
                          (PicesKKStr.StrToLatitude (Latitude.Text)  != deployment.Latitude)            ||
                          (PicesKKStr.StrToLongitude(Longitude.Text) != deployment.Longitude)           ||
                          (SyncTimeStampActual.Value                 != deployment.SyncTimeStampActual) ||
                          (SyncTimeStampCTD.Value                    != deployment.SyncTimeStampCTD)    ||
                          (SyncTimeStampGPS.Value                    != deployment.SyncTimeStampGPS)    ||
                          (DateTimeStart.Value                       != deployment.DateTimeStart)       ||
                          (DateTimeEnd.Value                         != deployment.DateTimeEnd);
      return  changesMade;
    }


    private  void  PerformUpdate ()
    {
      if  (!allowUpdates)
      {
        PicesSipperVariables.UpdatesNotAllowed ("Deployment Dialog");
        return;
      }

      PopulateDeploymentFromScreen ();
      if  (addingNewDeployment)
        DbConn ().SipperDeploymentInsert (deployment);

      else if  (deletingDeployment)
      {
        DbConn ().SipperDeploymentDelete (deployment.CruiseName, deployment.StationName, deployment.DeploymentNum);
        if  (DbConn ().Valid ())
          deploymentDeleted = true;
      }

      else
        DbConn ().SipperDeploymentUpdate (deployment);

      if  (!DbConn ().LastOpSuccessful)
      {
        MessageBox.Show (this, "Update operation failed.\n\n" + DbConn ().LastErrorDesc (), "Update MySQL Server", MessageBoxButtons.OK);
      }
    }


    private void DeploymentDialog_Load(object sender, EventArgs e)
    {
      lastSipperFileDir = PicesSipperVariables.SipperFilesDirectory ();
      PopulateScreen ();
    }


    private void UpdateButton_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
      {
        PicesSipperVariables.UpdatesNotAllowed ("Deployment Dialog");
        return;
      }

      if  (deletingDeployment)
      {
        if  (SipperFiles.Nodes.Count > 0)
        {
          MessageBox.Show (this, "Can not delete deployment while there are Sipper files attached.", "Delete a Deployment", MessageBoxButtons.OK);
          return;
        }
        else
        {
          PerformUpdate ();
          Close ();
        }
      }
      else
      {
        if  (!ChangesMade ())
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


    private void textBox1_Validating(object sender, CancelEventArgs e)
    {
      ValidateDeploymentNum ();
    }


    private void Latitude_Validating(object sender, CancelEventArgs e)
    {
      ValidateLatitude ();
    }



    private void Longitude_Validating(object sender, CancelEventArgs e)
    {
      ValidateLongitude ();
    }

    
    private  void  AddSipperFiles (String[]  sipperFileNames)
    {
      if  (sipperFileNames == null)
        return;

      String  errorMsgs = "";

      foreach  (String sfn in sipperFileNames)
      {
        String  rootName = OSservices.GetRootName (sfn);
        PicesSipperFile sf = DbConn ().SipperFileRecLoad (rootName);
        if  (sf != null)
        {
          if  (errorMsgs.Length > 0)
            errorMsgs += "\n";
          errorMsgs += "File[" + rootName + "] already in DataBase;  Cruise[" + sf.CruiseName + "]  Station[" + sf.StationName + "]  Deployment[" + sf.DeploymentNum + "]";
        }
      }
      if  (errorMsgs.Length > 0)
      {
        MessageBox.Show ("The fllowing errors occured" + "\n" + "\n" + errorMsgs + "\n", "Add Sipper Files Failed.");
      }
      else
      {
        foreach  (String sfn in sipperFileNames)
        {
          String  rootName = OSservices.GetRootName (sfn);
          PicesSipperFile  newSipperFile = new PicesSipperFile (rootName);
          newSipperFile.CruiseName    = deployment.CruiseName;
          newSipperFile.StationName   = deployment.StationName;
          newSipperFile.DeploymentNum = deployment.DeploymentNum;

          SipperConfigRec config = new SipperConfigRec ();
          newSipperFile.Sp0 = config.HeaderRec ().SerialPortShortName (0);
          newSipperFile.Sp1 = config.HeaderRec ().SerialPortShortName (1);
          newSipperFile.Sp2 = config.HeaderRec ().SerialPortShortName (2);
          newSipperFile.Sp3 = config.HeaderRec ().SerialPortShortName (3);
          if  (String.IsNullOrEmpty (newSipperFile.Sp0))
          {
            newSipperFile.Sp0 = "CTD";
            newSipperFile.Sp1 = "P-R";
            newSipperFile.Sp2 = "BAT";
            newSipperFile.Sp3 = "";
          }

          newSipperFile.CtdExt0 = config.HeaderRec ().CtdExt0Code ();
          newSipperFile.CtdExt1 = config.HeaderRec ().CtdExt1Code ();
          newSipperFile.CtdExt2 = config.HeaderRec ().CtdExt2Code ();
          newSipperFile.CtdExt3 = config.HeaderRec ().CtdExt3Code ();

          if  (String.IsNullOrEmpty (newSipperFile.CtdExt0))
          {
            newSipperFile.CtdExt1 = "TRN";
            newSipperFile.CtdExt1 = "OXG";
            newSipperFile.CtdExt2 = "FLO";
            newSipperFile.CtdExt3 = "TUR";
          }
          DbConn ().SipperFileInsert (newSipperFile);
          if  (!DbConn ().Valid ())
          {
            MessageBox.Show ("Error occured adding SipperFile[" + rootName + "]" + "\n\n" + DbConn ().LastErrorDesc ());
            break;
          }
        }
        PopulateScreen ();
      }
    }  /* AddSipperFiles */



    private void AddSipperFileButton_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
      {
        PicesSipperVariables.UpdatesNotAllowed ("Deployment Dialog");
        return;
      }
      OpenFileDialog ofd = new OpenFileDialog ();
      ofd.DefaultExt = "*.spr";
      ofd.Filter = "SIPPER Files (*.spr) | *.spr";
      ofd.FilterIndex = 1;
      ofd.InitialDirectory = lastSipperFileDir;
      ofd.Multiselect = true;
      ofd.Title = "Select SIPPER Files to Add";
      DialogResult  dr = ofd.ShowDialog (this);
      if  (dr == DialogResult.OK)
        AddSipperFiles (ofd.FileNames);
    }


    private void DeploymentDialog_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (weOwnDbConn)
      {
        dbConn.Close ();
        dbConn = null;
        GC.Collect ();
        weOwnDbConn = false;
      }
    }
  }
}
