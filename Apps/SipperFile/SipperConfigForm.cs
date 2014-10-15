using System;
using System.IO;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Net;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Security;
using SipperFile;

using  PicesInterface;

namespace SipperFile
{
	/// <summary>
	/// Summary description for SipperConfigForm.
	/// </summary>
  public class SipperConfigForm : System.Windows.Forms.Form
  {
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Label label2;

    //private ArrayList config = new ArrayList ();
    private System.Windows.Forms.Button bt_Save;
    private System.Windows.Forms.Label objectiveLabel;
    private System.Windows.Forms.Label cruiseLocationLabel;
    private System.Windows.Forms.Label principlaInvestigatorLabel;
    private System.Windows.Forms.Label researchOrgLabel;
    private System.Windows.Forms.Label cruiseDatesLabel;
    private System.Windows.Forms.TextBox objective;
    private System.Windows.Forms.TextBox principal;
    private System.Windows.Forms.TextBox researchOrg;
    private System.Windows.Forms.DateTimePicker cruiseDateStart;
    private System.Windows.Forms.Label deploymentVehicleLabel;
    private System.Windows.Forms.DateTimePicker cruiseDateEnd;
    private System.Windows.Forms.TextBox deploymentVehicle;
    private System.Windows.Forms.Label seaConditionsLabel;
    private System.Windows.Forms.TextBox seaConditions;
    private System.Windows.Forms.Label waveHeightLabel;
    private System.Windows.Forms.TextBox waveHeight;
    private System.Windows.Forms.Label weatherCondLabel;
    private System.Windows.Forms.TextBox weatherConditions;
    private System.Windows.Forms.Label airTempLabel;
    private System.Windows.Forms.TextBox airTemp;
    private System.Windows.Forms.Label scanRateLabel;
    private System.Windows.Forms.TextBox scanRate;
    private System.Windows.Forms.Label gpsLabel;
    private System.Windows.Forms.Label serialPot0Label;
    private System.Windows.Forms.Label serialPort1Label;
    private System.Windows.Forms.Label serialPort2Label;
    private System.Windows.Forms.Label serialPort3Label;
    private System.Windows.Forms.ComboBox serialPort0;
    private System.Windows.Forms.TextBox serialPort0Text;
    private System.Windows.Forms.ComboBox serialPort1;
    private System.Windows.Forms.TextBox serialPort1Text;
    private System.Windows.Forms.TextBox serialPort3Text;
    private System.Windows.Forms.ComboBox serialPort3;
    private System.Windows.Forms.TextBox serialPort2Text;
    private System.Windows.Forms.ComboBox serialPort2;
    private System.Windows.Forms.Label sipperFileNameLabel;
    private System.Windows.Forms.TextBox sipperFileName;
    private IContainer components;
    private System.Windows.Forms.TextBox cruiseLocation;
    private System.Windows.Forms.TextBox ipAddress;
    private System.Windows.Forms.TextBox ipPort;


    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.Label headerSpaceUsedLabel;
    private System.Windows.Forms.TextBox headerSpaceUsed;
    private System.Windows.Forms.Label headerSpaceLeftLabel;
    private System.Windows.Forms.TextBox headerSpaceLeft;
    private MaskedTextBox latitude;
    private MaskedTextBox longitude;
    private Label label5;
    private Label LatitudeLabel;
    private Label CtdExt2CodeLabel;
    private Label CtdExt1CodeLabel;
    private Label CtdExt0CodeLabel;
    private Label CtdExt3CodeLabel;
    private ComboBox ctdExt0Code;
    private ComboBox ctdExt3Code;
    private ComboBox ctdExt2Code;
    private ComboBox ctdExt1Code;
    private Panel panel2;
    private Panel panel1;
    private Label label3;
    private Label label4;
    protected CheckBox UseDataBase;

    private  bool             userSavedDataToFile = false;

    private  SipperConfigRec  sipperConfigRec     = null;

    public  bool  UserSavedDataToFile ()  {return userSavedDataToFile;}


    public  SipperConfigForm (SipperConfigRec  _sipperConfigRec)
    {
      //
      // Required for Windows Form Designer support
      //
      InitializeComponent();

      sipperConfigRec = _sipperConfigRec;
    }



    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    protected override void Dispose( bool disposing )
    {
      if( disposing )
      {
        if(components != null)
        {
          components.Dispose();
        }
      }
      base.Dispose( disposing );
    }



    #region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container ();
      this.ipAddress = new System.Windows.Forms.TextBox ();
      this.label1 = new System.Windows.Forms.Label ();
      this.label2 = new System.Windows.Forms.Label ();
      this.ipPort = new System.Windows.Forms.TextBox ();
      this.bt_Save = new System.Windows.Forms.Button ();
      this.objectiveLabel = new System.Windows.Forms.Label ();
      this.cruiseLocationLabel = new System.Windows.Forms.Label ();
      this.cruiseLocation = new System.Windows.Forms.TextBox ();
      this.objective = new System.Windows.Forms.TextBox ();
      this.principlaInvestigatorLabel = new System.Windows.Forms.Label ();
      this.principal = new System.Windows.Forms.TextBox ();
      this.researchOrgLabel = new System.Windows.Forms.Label ();
      this.researchOrg = new System.Windows.Forms.TextBox ();
      this.cruiseDatesLabel = new System.Windows.Forms.Label ();
      this.cruiseDateStart = new System.Windows.Forms.DateTimePicker ();
      this.cruiseDateEnd = new System.Windows.Forms.DateTimePicker ();
      this.deploymentVehicleLabel = new System.Windows.Forms.Label ();
      this.deploymentVehicle = new System.Windows.Forms.TextBox ();
      this.seaConditionsLabel = new System.Windows.Forms.Label ();
      this.seaConditions = new System.Windows.Forms.TextBox ();
      this.waveHeightLabel = new System.Windows.Forms.Label ();
      this.waveHeight = new System.Windows.Forms.TextBox ();
      this.weatherCondLabel = new System.Windows.Forms.Label ();
      this.weatherConditions = new System.Windows.Forms.TextBox ();
      this.airTempLabel = new System.Windows.Forms.Label ();
      this.airTemp = new System.Windows.Forms.TextBox ();
      this.gpsLabel = new System.Windows.Forms.Label ();
      this.scanRateLabel = new System.Windows.Forms.Label ();
      this.scanRate = new System.Windows.Forms.TextBox ();
      this.serialPot0Label = new System.Windows.Forms.Label ();
      this.serialPort3Label = new System.Windows.Forms.Label ();
      this.serialPort2Label = new System.Windows.Forms.Label ();
      this.serialPort1Label = new System.Windows.Forms.Label ();
      this.serialPort0 = new System.Windows.Forms.ComboBox ();
      this.serialPort0Text = new System.Windows.Forms.TextBox ();
      this.serialPort1 = new System.Windows.Forms.ComboBox ();
      this.serialPort1Text = new System.Windows.Forms.TextBox ();
      this.serialPort3Text = new System.Windows.Forms.TextBox ();
      this.serialPort3 = new System.Windows.Forms.ComboBox ();
      this.serialPort2Text = new System.Windows.Forms.TextBox ();
      this.serialPort2 = new System.Windows.Forms.ComboBox ();
      this.sipperFileNameLabel = new System.Windows.Forms.Label ();
      this.sipperFileName = new System.Windows.Forms.TextBox ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.headerSpaceUsedLabel = new System.Windows.Forms.Label ();
      this.headerSpaceUsed = new System.Windows.Forms.TextBox ();
      this.headerSpaceLeftLabel = new System.Windows.Forms.Label ();
      this.headerSpaceLeft = new System.Windows.Forms.TextBox ();
      this.latitude = new System.Windows.Forms.MaskedTextBox ();
      this.longitude = new System.Windows.Forms.MaskedTextBox ();
      this.LatitudeLabel = new System.Windows.Forms.Label ();
      this.label5 = new System.Windows.Forms.Label ();
      this.CtdExt0CodeLabel = new System.Windows.Forms.Label ();
      this.CtdExt1CodeLabel = new System.Windows.Forms.Label ();
      this.CtdExt2CodeLabel = new System.Windows.Forms.Label ();
      this.CtdExt3CodeLabel = new System.Windows.Forms.Label ();
      this.ctdExt0Code = new System.Windows.Forms.ComboBox ();
      this.ctdExt1Code = new System.Windows.Forms.ComboBox ();
      this.ctdExt2Code = new System.Windows.Forms.ComboBox ();
      this.ctdExt3Code = new System.Windows.Forms.ComboBox ();
      this.panel1 = new System.Windows.Forms.Panel ();
      this.label3 = new System.Windows.Forms.Label ();
      this.panel2 = new System.Windows.Forms.Panel ();
      this.label4 = new System.Windows.Forms.Label ();
      this.UseDataBase = new System.Windows.Forms.CheckBox ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.panel1.SuspendLayout ();
      this.panel2.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // ipAddress
      // 
      this.ipAddress.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ipAddress.Location = new System.Drawing.Point (15, 32);
      this.ipAddress.Name = "ipAddress";
      this.ipAddress.Size = new System.Drawing.Size (232, 22);
      this.ipAddress.TabIndex = 0;
      this.ipAddress.Validated += new System.EventHandler (this.ipAddress_Validated);
      this.ipAddress.Validating += new System.ComponentModel.CancelEventHandler (this.ipAddress_Validating);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point (12, 19);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size (58, 13);
      this.label1.TabIndex = 1;
      this.label1.Text = "IP Address";
      this.label1.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point (262, 19);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size (67, 13);
      this.label2.TabIndex = 3;
      this.label2.Text = "Port Address";
      this.label2.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // ipPort
      // 
      this.ipPort.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ipPort.Location = new System.Drawing.Point (265, 32);
      this.ipPort.Name = "ipPort";
      this.ipPort.Size = new System.Drawing.Size (67, 22);
      this.ipPort.TabIndex = 1;
      this.ipPort.Validated += new System.EventHandler (this.ipPort_Validated);
      this.ipPort.Validating += new System.ComponentModel.CancelEventHandler (this.ipPort_Validating);
      // 
      // bt_Save
      // 
      this.bt_Save.Location = new System.Drawing.Point (362, 604);
      this.bt_Save.Name = "bt_Save";
      this.bt_Save.Size = new System.Drawing.Size (84, 29);
      this.bt_Save.TabIndex = 60;
      this.bt_Save.Text = "Save Now";
      this.bt_Save.Click += new System.EventHandler (this.bt_Save_Click);
      // 
      // objectiveLabel
      // 
      this.objectiveLabel.AutoSize = true;
      this.objectiveLabel.Location = new System.Drawing.Point (3, 76);
      this.objectiveLabel.Name = "objectiveLabel";
      this.objectiveLabel.Size = new System.Drawing.Size (52, 13);
      this.objectiveLabel.TabIndex = 12;
      this.objectiveLabel.Text = "Objective";
      this.objectiveLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // cruiseLocationLabel
      // 
      this.cruiseLocationLabel.AutoSize = true;
      this.cruiseLocationLabel.Location = new System.Drawing.Point (3, 42);
      this.cruiseLocationLabel.Name = "cruiseLocationLabel";
      this.cruiseLocationLabel.Size = new System.Drawing.Size (80, 13);
      this.cruiseLocationLabel.TabIndex = 11;
      this.cruiseLocationLabel.Text = "Cruise Location";
      this.cruiseLocationLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // cruiseLocation
      // 
      this.cruiseLocation.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.cruiseLocation.Location = new System.Drawing.Point (89, 37);
      this.cruiseLocation.Name = "cruiseLocation";
      this.cruiseLocation.Size = new System.Drawing.Size (300, 22);
      this.cruiseLocation.TabIndex = 13;
      this.cruiseLocation.Text = "Cruise Location";
      this.cruiseLocation.Validating += new System.ComponentModel.CancelEventHandler (this.cruiseLocation_Validating);
      // 
      // objective
      // 
      this.objective.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.objective.Location = new System.Drawing.Point (89, 72);
      this.objective.Name = "objective";
      this.objective.Size = new System.Drawing.Size (300, 22);
      this.objective.TabIndex = 14;
      this.objective.Text = "Objective";
      this.objective.Validating += new System.ComponentModel.CancelEventHandler (this.objective_Validating);
      // 
      // principlaInvestigatorLabel
      // 
      this.principlaInvestigatorLabel.AutoSize = true;
      this.principlaInvestigatorLabel.Location = new System.Drawing.Point (3, 112);
      this.principlaInvestigatorLabel.Name = "principlaInvestigatorLabel";
      this.principlaInvestigatorLabel.Size = new System.Drawing.Size (47, 13);
      this.principlaInvestigatorLabel.TabIndex = 15;
      this.principlaInvestigatorLabel.Text = "Principal";
      this.principlaInvestigatorLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // principal
      // 
      this.principal.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.principal.Location = new System.Drawing.Point (89, 107);
      this.principal.Name = "principal";
      this.principal.Size = new System.Drawing.Size (300, 22);
      this.principal.TabIndex = 16;
      this.principal.Text = "Principal";
      this.principal.Validating += new System.ComponentModel.CancelEventHandler (this.principal_Validating);
      // 
      // researchOrgLabel
      // 
      this.researchOrgLabel.AutoSize = true;
      this.researchOrgLabel.Location = new System.Drawing.Point (3, 147);
      this.researchOrgLabel.Name = "researchOrgLabel";
      this.researchOrgLabel.Size = new System.Drawing.Size (73, 13);
      this.researchOrgLabel.TabIndex = 17;
      this.researchOrgLabel.Text = "Research Org";
      this.researchOrgLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // researchOrg
      // 
      this.researchOrg.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.researchOrg.Location = new System.Drawing.Point (89, 142);
      this.researchOrg.Name = "researchOrg";
      this.researchOrg.Size = new System.Drawing.Size (300, 22);
      this.researchOrg.TabIndex = 18;
      this.researchOrg.Text = "Research Organization";
      this.researchOrg.Validating += new System.ComponentModel.CancelEventHandler (this.researchOrg_Validating);
      // 
      // cruiseDatesLabel
      // 
      this.cruiseDatesLabel.AutoSize = true;
      this.cruiseDatesLabel.Location = new System.Drawing.Point (3, 184);
      this.cruiseDatesLabel.Name = "cruiseDatesLabel";
      this.cruiseDatesLabel.Size = new System.Drawing.Size (67, 13);
      this.cruiseDatesLabel.TabIndex = 19;
      this.cruiseDatesLabel.Text = "Cruise Dates";
      this.cruiseDatesLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // cruiseDateStart
      // 
      this.cruiseDateStart.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.cruiseDateStart.Format = System.Windows.Forms.DateTimePickerFormat.Short;
      this.cruiseDateStart.Location = new System.Drawing.Point (89, 177);
      this.cruiseDateStart.Name = "cruiseDateStart";
      this.cruiseDateStart.Size = new System.Drawing.Size (120, 22);
      this.cruiseDateStart.TabIndex = 20;
      // 
      // cruiseDateEnd
      // 
      this.cruiseDateEnd.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.cruiseDateEnd.Format = System.Windows.Forms.DateTimePickerFormat.Short;
      this.cruiseDateEnd.Location = new System.Drawing.Point (233, 177);
      this.cruiseDateEnd.Name = "cruiseDateEnd";
      this.cruiseDateEnd.Size = new System.Drawing.Size (120, 22);
      this.cruiseDateEnd.TabIndex = 21;
      // 
      // deploymentVehicleLabel
      // 
      this.deploymentVehicleLabel.AutoSize = true;
      this.deploymentVehicleLabel.Location = new System.Drawing.Point (3, 217);
      this.deploymentVehicleLabel.Name = "deploymentVehicleLabel";
      this.deploymentVehicleLabel.Size = new System.Drawing.Size (42, 13);
      this.deploymentVehicleLabel.TabIndex = 22;
      this.deploymentVehicleLabel.Text = "Vehicle";
      this.deploymentVehicleLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // deploymentVehicle
      // 
      this.deploymentVehicle.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.deploymentVehicle.Location = new System.Drawing.Point (89, 212);
      this.deploymentVehicle.Name = "deploymentVehicle";
      this.deploymentVehicle.Size = new System.Drawing.Size (300, 22);
      this.deploymentVehicle.TabIndex = 23;
      this.deploymentVehicle.Text = "Deployment Vehicle";
      this.deploymentVehicle.Validating += new System.ComponentModel.CancelEventHandler (this.deploymentVehicle_Validating);
      // 
      // seaConditionsLabel
      // 
      this.seaConditionsLabel.AutoSize = true;
      this.seaConditionsLabel.Location = new System.Drawing.Point (3, 252);
      this.seaConditionsLabel.Name = "seaConditionsLabel";
      this.seaConditionsLabel.Size = new System.Drawing.Size (78, 13);
      this.seaConditionsLabel.TabIndex = 24;
      this.seaConditionsLabel.Text = "Sea Conditions";
      this.seaConditionsLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // seaConditions
      // 
      this.seaConditions.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.seaConditions.Location = new System.Drawing.Point (89, 247);
      this.seaConditions.Name = "seaConditions";
      this.seaConditions.Size = new System.Drawing.Size (300, 22);
      this.seaConditions.TabIndex = 25;
      this.seaConditions.Text = "Sea Conditions";
      this.seaConditions.Validating += new System.ComponentModel.CancelEventHandler (this.seaConditions_Validating);
      // 
      // waveHeightLabel
      // 
      this.waveHeightLabel.AutoSize = true;
      this.waveHeightLabel.Location = new System.Drawing.Point (3, 290);
      this.waveHeightLabel.Name = "waveHeightLabel";
      this.waveHeightLabel.Size = new System.Drawing.Size (70, 13);
      this.waveHeightLabel.TabIndex = 26;
      this.waveHeightLabel.Text = "Wave Height";
      this.waveHeightLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // waveHeight
      // 
      this.waveHeight.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.waveHeight.Location = new System.Drawing.Point (89, 282);
      this.waveHeight.Name = "waveHeight";
      this.waveHeight.Size = new System.Drawing.Size (216, 22);
      this.waveHeight.TabIndex = 27;
      this.waveHeight.Text = "0.0";
      this.waveHeight.Validating += new System.ComponentModel.CancelEventHandler (this.waveHeight_Validating);
      // 
      // weatherCondLabel
      // 
      this.weatherCondLabel.AutoSize = true;
      this.weatherCondLabel.Location = new System.Drawing.Point (3, 323);
      this.weatherCondLabel.Name = "weatherCondLabel";
      this.weatherCondLabel.Size = new System.Drawing.Size (76, 13);
      this.weatherCondLabel.TabIndex = 28;
      this.weatherCondLabel.Text = "Weather Cond";
      this.weatherCondLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // weatherConditions
      // 
      this.weatherConditions.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.weatherConditions.Location = new System.Drawing.Point (89, 317);
      this.weatherConditions.Name = "weatherConditions";
      this.weatherConditions.Size = new System.Drawing.Size (300, 22);
      this.weatherConditions.TabIndex = 29;
      this.weatherConditions.Text = "Weather Conditions";
      this.weatherConditions.Validating += new System.ComponentModel.CancelEventHandler (this.weatherConditions_Validating);
      // 
      // airTempLabel
      // 
      this.airTempLabel.AutoSize = true;
      this.airTempLabel.Location = new System.Drawing.Point (3, 355);
      this.airTempLabel.Name = "airTempLabel";
      this.airTempLabel.Size = new System.Drawing.Size (49, 13);
      this.airTempLabel.TabIndex = 30;
      this.airTempLabel.Text = "Air Temp";
      this.airTempLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // airTemp
      // 
      this.airTemp.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.airTemp.Location = new System.Drawing.Point (89, 352);
      this.airTemp.Name = "airTemp";
      this.airTemp.Size = new System.Drawing.Size (216, 22);
      this.airTemp.TabIndex = 31;
      this.airTemp.Text = "Air Temperature";
      this.airTemp.Validating += new System.ComponentModel.CancelEventHandler (this.airTemp_Validating);
      // 
      // gpsLabel
      // 
      this.gpsLabel.Location = new System.Drawing.Point (3, 391);
      this.gpsLabel.Name = "gpsLabel";
      this.gpsLabel.Size = new System.Drawing.Size (75, 23);
      this.gpsLabel.TabIndex = 32;
      this.gpsLabel.Text = "GPS Location";
      this.gpsLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // scanRateLabel
      // 
      this.scanRateLabel.AutoSize = true;
      this.scanRateLabel.Location = new System.Drawing.Point (21, 42);
      this.scanRateLabel.Name = "scanRateLabel";
      this.scanRateLabel.Size = new System.Drawing.Size (58, 13);
      this.scanRateLabel.TabIndex = 34;
      this.scanRateLabel.Text = "Scan Rate";
      this.scanRateLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // scanRate
      // 
      this.scanRate.Enabled = false;
      this.scanRate.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.scanRate.Location = new System.Drawing.Point (103, 37);
      this.scanRate.Name = "scanRate";
      this.scanRate.Size = new System.Drawing.Size (150, 22);
      this.scanRate.TabIndex = 35;
      this.scanRate.Text = "25000";
      // 
      // serialPot0Label
      // 
      this.serialPot0Label.AutoSize = true;
      this.serialPot0Label.Location = new System.Drawing.Point (21, 76);
      this.serialPot0Label.Name = "serialPot0Label";
      this.serialPot0Label.Size = new System.Drawing.Size (64, 13);
      this.serialPot0Label.TabIndex = 36;
      this.serialPot0Label.Text = "Serial Port 0";
      this.serialPot0Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // serialPort3Label
      // 
      this.serialPort3Label.AutoSize = true;
      this.serialPort3Label.Location = new System.Drawing.Point (21, 184);
      this.serialPort3Label.Name = "serialPort3Label";
      this.serialPort3Label.Size = new System.Drawing.Size (64, 13);
      this.serialPort3Label.TabIndex = 38;
      this.serialPort3Label.Text = "Serial Port 3";
      this.serialPort3Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // serialPort2Label
      // 
      this.serialPort2Label.AutoSize = true;
      this.serialPort2Label.Location = new System.Drawing.Point (21, 147);
      this.serialPort2Label.Name = "serialPort2Label";
      this.serialPort2Label.Size = new System.Drawing.Size (64, 13);
      this.serialPort2Label.TabIndex = 39;
      this.serialPort2Label.Text = "Serial Port 2";
      this.serialPort2Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // serialPort1Label
      // 
      this.serialPort1Label.AutoSize = true;
      this.serialPort1Label.Location = new System.Drawing.Point (21, 112);
      this.serialPort1Label.Name = "serialPort1Label";
      this.serialPort1Label.Size = new System.Drawing.Size (64, 13);
      this.serialPort1Label.TabIndex = 40;
      this.serialPort1Label.Text = "Serial Port 1";
      this.serialPort1Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // serialPort0
      // 
      this.serialPort0.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.serialPort0.Items.AddRange (new object[] {
            "CTD",
            "GPS",
            "Inclinometer",
            "Other"});
      this.serialPort0.Location = new System.Drawing.Point (103, 72);
      this.serialPort0.Name = "serialPort0";
      this.serialPort0.Size = new System.Drawing.Size (64, 24);
      this.serialPort0.TabIndex = 41;
      this.serialPort0.Text = "CTD";
      this.serialPort0.SelectedIndexChanged += new System.EventHandler (this.serialPort0_SelectedIndexChanged);
      // 
      // serialPort0Text
      // 
      this.serialPort0Text.Enabled = false;
      this.serialPort0Text.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.serialPort0Text.Location = new System.Drawing.Point (173, 74);
      this.serialPort0Text.Name = "serialPort0Text";
      this.serialPort0Text.Size = new System.Drawing.Size (144, 22);
      this.serialPort0Text.TabIndex = 42;
      this.serialPort0Text.Validated += new System.EventHandler (this.serialPort0Text_Validated);
      this.serialPort0Text.Validating += new System.ComponentModel.CancelEventHandler (this.serialPort0Text_Validating);
      // 
      // serialPort1
      // 
      this.serialPort1.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.serialPort1.Items.AddRange (new object[] {
            "CTD",
            "GPS",
            "Inclinometer",
            "Other"});
      this.serialPort1.Location = new System.Drawing.Point (103, 107);
      this.serialPort1.Name = "serialPort1";
      this.serialPort1.Size = new System.Drawing.Size (64, 24);
      this.serialPort1.TabIndex = 43;
      this.serialPort1.Text = "CTD";
      this.serialPort1.SelectedIndexChanged += new System.EventHandler (this.serialPort1_SelectedIndexChanged);
      // 
      // serialPort1Text
      // 
      this.serialPort1Text.Enabled = false;
      this.serialPort1Text.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.serialPort1Text.Location = new System.Drawing.Point (175, 107);
      this.serialPort1Text.Name = "serialPort1Text";
      this.serialPort1Text.Size = new System.Drawing.Size (144, 22);
      this.serialPort1Text.TabIndex = 44;
      this.serialPort1Text.Validated += new System.EventHandler (this.serialPort1Text_Validated);
      this.serialPort1Text.Validating += new System.ComponentModel.CancelEventHandler (this.serialPort1Text_Validating);
      // 
      // serialPort3Text
      // 
      this.serialPort3Text.Enabled = false;
      this.serialPort3Text.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.serialPort3Text.Location = new System.Drawing.Point (175, 180);
      this.serialPort3Text.Name = "serialPort3Text";
      this.serialPort3Text.Size = new System.Drawing.Size (144, 22);
      this.serialPort3Text.TabIndex = 48;
      this.serialPort3Text.Validated += new System.EventHandler (this.serialPort3Text_Validated);
      this.serialPort3Text.Validating += new System.ComponentModel.CancelEventHandler (this.serialPort3Text_Validating);
      // 
      // serialPort3
      // 
      this.serialPort3.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.serialPort3.Items.AddRange (new object[] {
            "CTD",
            "GPS",
            "Inclinometer",
            "Other"});
      this.serialPort3.Location = new System.Drawing.Point (103, 179);
      this.serialPort3.Name = "serialPort3";
      this.serialPort3.Size = new System.Drawing.Size (64, 24);
      this.serialPort3.TabIndex = 47;
      this.serialPort3.Text = "CTD";
      this.serialPort3.SelectedIndexChanged += new System.EventHandler (this.serialPort3_SelectedIndexChanged);
      // 
      // serialPort2Text
      // 
      this.serialPort2Text.Enabled = false;
      this.serialPort2Text.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.serialPort2Text.Location = new System.Drawing.Point (175, 142);
      this.serialPort2Text.Name = "serialPort2Text";
      this.serialPort2Text.Size = new System.Drawing.Size (144, 22);
      this.serialPort2Text.TabIndex = 46;
      this.serialPort2Text.Validated += new System.EventHandler (this.serialPort2Text_Validated);
      this.serialPort2Text.Validating += new System.ComponentModel.CancelEventHandler (this.serialPort2Text_Validating);
      // 
      // serialPort2
      // 
      this.serialPort2.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.serialPort2.Items.AddRange (new object[] {
            "CTD",
            "GPS",
            "Inclinometer",
            "Other"});
      this.serialPort2.Location = new System.Drawing.Point (103, 142);
      this.serialPort2.Name = "serialPort2";
      this.serialPort2.Size = new System.Drawing.Size (64, 24);
      this.serialPort2.TabIndex = 45;
      this.serialPort2.Text = "CTD";
      this.serialPort2.SelectedIndexChanged += new System.EventHandler (this.serialPort2_SelectedIndexChanged);
      // 
      // sipperFileNameLabel
      // 
      this.sipperFileNameLabel.AutoSize = true;
      this.sipperFileNameLabel.Location = new System.Drawing.Point (12, 60);
      this.sipperFileNameLabel.Name = "sipperFileNameLabel";
      this.sipperFileNameLabel.Size = new System.Drawing.Size (87, 13);
      this.sipperFileNameLabel.TabIndex = 49;
      this.sipperFileNameLabel.Text = "Sipper File Name";
      this.sipperFileNameLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // sipperFileName
      // 
      this.sipperFileName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.sipperFileName.Location = new System.Drawing.Point (15, 74);
      this.sipperFileName.Name = "sipperFileName";
      this.sipperFileName.Size = new System.Drawing.Size (300, 22);
      this.sipperFileName.TabIndex = 10;
      this.sipperFileName.Text = "fileName";
      this.sipperFileName.Validated += new System.EventHandler (this.sipperFileName_Validated);
      this.sipperFileName.Validating += new System.ComponentModel.CancelEventHandler (this.sipperFileName_Validating);
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // headerSpaceUsedLabel
      // 
      this.headerSpaceUsedLabel.AutoSize = true;
      this.headerSpaceUsedLabel.Location = new System.Drawing.Point (558, 18);
      this.headerSpaceUsedLabel.Name = "headerSpaceUsedLabel";
      this.headerSpaceUsedLabel.Size = new System.Drawing.Size (104, 13);
      this.headerSpaceUsedLabel.TabIndex = 61;
      this.headerSpaceUsedLabel.Text = "Header Space Used";
      this.headerSpaceUsedLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // headerSpaceUsed
      // 
      this.headerSpaceUsed.Enabled = false;
      this.headerSpaceUsed.Location = new System.Drawing.Point (561, 32);
      this.headerSpaceUsed.Name = "headerSpaceUsed";
      this.headerSpaceUsed.ReadOnly = true;
      this.headerSpaceUsed.Size = new System.Drawing.Size (96, 20);
      this.headerSpaceUsed.TabIndex = 62;
      this.headerSpaceUsed.Text = "0";
      // 
      // headerSpaceLeftLabel
      // 
      this.headerSpaceLeftLabel.AutoSize = true;
      this.headerSpaceLeftLabel.Location = new System.Drawing.Point (684, 18);
      this.headerSpaceLeftLabel.Name = "headerSpaceLeftLabel";
      this.headerSpaceLeftLabel.Size = new System.Drawing.Size (25, 13);
      this.headerSpaceLeftLabel.TabIndex = 63;
      this.headerSpaceLeftLabel.Text = "Left";
      this.headerSpaceLeftLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // headerSpaceLeft
      // 
      this.headerSpaceLeft.Enabled = false;
      this.headerSpaceLeft.Location = new System.Drawing.Point (687, 32);
      this.headerSpaceLeft.Name = "headerSpaceLeft";
      this.headerSpaceLeft.ReadOnly = true;
      this.headerSpaceLeft.Size = new System.Drawing.Size (96, 20);
      this.headerSpaceLeft.TabIndex = 64;
      this.headerSpaceLeft.Text = "0";
      // 
      // latitude
      // 
      this.latitude.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.latitude.InsertKeyMode = System.Windows.Forms.InsertKeyMode.Overwrite;
      this.latitude.Location = new System.Drawing.Point (89, 393);
      this.latitude.Mask = "00:00.000L";
      this.latitude.Name = "latitude";
      this.latitude.Size = new System.Drawing.Size (100, 22);
      this.latitude.TabIndex = 32;
      this.latitude.Validating += new System.ComponentModel.CancelEventHandler (this.latitude_Validating);
      this.latitude.Validated += new System.EventHandler (this.latitude_Validated);
      // 
      // longitude
      // 
      this.longitude.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.longitude.InsertKeyMode = System.Windows.Forms.InsertKeyMode.Overwrite;
      this.longitude.Location = new System.Drawing.Point (205, 392);
      this.longitude.Mask = "000:00.000L";
      this.longitude.Name = "longitude";
      this.longitude.Size = new System.Drawing.Size (100, 22);
      this.longitude.TabIndex = 33;
      this.longitude.Validating += new System.ComponentModel.CancelEventHandler (this.longitude_Validating);
      this.longitude.Validated += new System.EventHandler (this.longitude_Validated);
      // 
      // LatitudeLabel
      // 
      this.LatitudeLabel.AutoSize = true;
      this.LatitudeLabel.Location = new System.Drawing.Point (86, 379);
      this.LatitudeLabel.Name = "LatitudeLabel";
      this.LatitudeLabel.Size = new System.Drawing.Size (45, 13);
      this.LatitudeLabel.TabIndex = 67;
      this.LatitudeLabel.Text = "Latitude";
      this.LatitudeLabel.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point (202, 379);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size (54, 13);
      this.label5.TabIndex = 68;
      this.label5.Text = "Longitude";
      this.label5.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // CtdExt0CodeLabel
      // 
      this.CtdExt0CodeLabel.AutoSize = true;
      this.CtdExt0CodeLabel.Location = new System.Drawing.Point (21, 252);
      this.CtdExt0CodeLabel.Name = "CtdExt0CodeLabel";
      this.CtdExt0CodeLabel.Size = new System.Drawing.Size (56, 13);
      this.CtdExt0CodeLabel.TabIndex = 69;
      this.CtdExt0CodeLabel.Text = "CTD Ext 0";
      this.CtdExt0CodeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // CtdExt1CodeLabel
      // 
      this.CtdExt1CodeLabel.AutoSize = true;
      this.CtdExt1CodeLabel.Location = new System.Drawing.Point (21, 290);
      this.CtdExt1CodeLabel.Name = "CtdExt1CodeLabel";
      this.CtdExt1CodeLabel.Size = new System.Drawing.Size (56, 13);
      this.CtdExt1CodeLabel.TabIndex = 70;
      this.CtdExt1CodeLabel.Text = "CTD Ext 1";
      this.CtdExt1CodeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // CtdExt2CodeLabel
      // 
      this.CtdExt2CodeLabel.AutoSize = true;
      this.CtdExt2CodeLabel.Location = new System.Drawing.Point (21, 323);
      this.CtdExt2CodeLabel.Name = "CtdExt2CodeLabel";
      this.CtdExt2CodeLabel.Size = new System.Drawing.Size (56, 13);
      this.CtdExt2CodeLabel.TabIndex = 71;
      this.CtdExt2CodeLabel.Text = "CTD Ext 2";
      this.CtdExt2CodeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // CtdExt3CodeLabel
      // 
      this.CtdExt3CodeLabel.AutoSize = true;
      this.CtdExt3CodeLabel.Location = new System.Drawing.Point (21, 355);
      this.CtdExt3CodeLabel.Name = "CtdExt3CodeLabel";
      this.CtdExt3CodeLabel.Size = new System.Drawing.Size (56, 13);
      this.CtdExt3CodeLabel.TabIndex = 72;
      this.CtdExt3CodeLabel.Text = "CTD Ext 3";
      this.CtdExt3CodeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ctdExt0Code
      // 
      this.ctdExt0Code.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ctdExt0Code.Items.AddRange (new object[] {
            "FLO",
            "OXG",
            "TRN",
            "TUR"});
      this.ctdExt0Code.Location = new System.Drawing.Point (103, 247);
      this.ctdExt0Code.Name = "ctdExt0Code";
      this.ctdExt0Code.Size = new System.Drawing.Size (64, 24);
      this.ctdExt0Code.TabIndex = 73;
      this.ctdExt0Code.Text = "OXG";
      // 
      // ctdExt1Code
      // 
      this.ctdExt1Code.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ctdExt1Code.Items.AddRange (new object[] {
            "FLO",
            "OXG",
            "TRN",
            "TUR"});
      this.ctdExt1Code.Location = new System.Drawing.Point (103, 282);
      this.ctdExt1Code.Name = "ctdExt1Code";
      this.ctdExt1Code.Size = new System.Drawing.Size (64, 24);
      this.ctdExt1Code.TabIndex = 74;
      this.ctdExt1Code.Text = "FLO";
      // 
      // ctdExt2Code
      // 
      this.ctdExt2Code.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ctdExt2Code.Items.AddRange (new object[] {
            "FLO",
            "OXG",
            "TRN",
            "TUR"});
      this.ctdExt2Code.Location = new System.Drawing.Point (103, 318);
      this.ctdExt2Code.Name = "ctdExt2Code";
      this.ctdExt2Code.Size = new System.Drawing.Size (64, 24);
      this.ctdExt2Code.TabIndex = 75;
      this.ctdExt2Code.Text = "TRN";
      // 
      // ctdExt3Code
      // 
      this.ctdExt3Code.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ctdExt3Code.Items.AddRange (new object[] {
            "FLO",
            "OXG",
            "TRN",
            "TUR"});
      this.ctdExt3Code.Location = new System.Drawing.Point (103, 352);
      this.ctdExt3Code.Name = "ctdExt3Code";
      this.ctdExt3Code.Size = new System.Drawing.Size (64, 24);
      this.ctdExt3Code.TabIndex = 76;
      this.ctdExt3Code.Text = "TUR";
      // 
      // panel1
      // 
      this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.panel1.Controls.Add (this.label3);
      this.panel1.Controls.Add (this.airTemp);
      this.panel1.Controls.Add (this.airTempLabel);
      this.panel1.Controls.Add (this.weatherConditions);
      this.panel1.Controls.Add (this.weatherCondLabel);
      this.panel1.Controls.Add (this.waveHeight);
      this.panel1.Controls.Add (this.waveHeightLabel);
      this.panel1.Controls.Add (this.seaConditions);
      this.panel1.Controls.Add (this.label5);
      this.panel1.Controls.Add (this.seaConditionsLabel);
      this.panel1.Controls.Add (this.LatitudeLabel);
      this.panel1.Controls.Add (this.deploymentVehicle);
      this.panel1.Controls.Add (this.longitude);
      this.panel1.Controls.Add (this.deploymentVehicleLabel);
      this.panel1.Controls.Add (this.latitude);
      this.panel1.Controls.Add (this.cruiseDateEnd);
      this.panel1.Controls.Add (this.cruiseDateStart);
      this.panel1.Controls.Add (this.cruiseDatesLabel);
      this.panel1.Controls.Add (this.researchOrg);
      this.panel1.Controls.Add (this.researchOrgLabel);
      this.panel1.Controls.Add (this.principal);
      this.panel1.Controls.Add (this.principlaInvestigatorLabel);
      this.panel1.Controls.Add (this.objective);
      this.panel1.Controls.Add (this.cruiseLocation);
      this.panel1.Controls.Add (this.objectiveLabel);
      this.panel1.Controls.Add (this.cruiseLocationLabel);
      this.panel1.Controls.Add (this.gpsLabel);
      this.panel1.Location = new System.Drawing.Point (12, 153);
      this.panel1.Name = "panel1";
      this.panel1.Size = new System.Drawing.Size (408, 429);
      this.panel1.TabIndex = 77;
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label3.Location = new System.Drawing.Point (118, 4);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size (149, 16);
      this.label3.TabIndex = 32;
      this.label3.Text = "Deployment Information";
      // 
      // panel2
      // 
      this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.panel2.Controls.Add (this.label4);
      this.panel2.Controls.Add (this.ctdExt3Code);
      this.panel2.Controls.Add (this.ctdExt2Code);
      this.panel2.Controls.Add (this.ctdExt1Code);
      this.panel2.Controls.Add (this.ctdExt0Code);
      this.panel2.Controls.Add (this.CtdExt3CodeLabel);
      this.panel2.Controls.Add (this.CtdExt2CodeLabel);
      this.panel2.Controls.Add (this.CtdExt1CodeLabel);
      this.panel2.Controls.Add (this.CtdExt0CodeLabel);
      this.panel2.Controls.Add (this.serialPort3Text);
      this.panel2.Controls.Add (this.serialPort3);
      this.panel2.Controls.Add (this.serialPort2Text);
      this.panel2.Controls.Add (this.serialPort2);
      this.panel2.Controls.Add (this.serialPort1Text);
      this.panel2.Controls.Add (this.serialPort1);
      this.panel2.Controls.Add (this.serialPort0Text);
      this.panel2.Controls.Add (this.serialPort0);
      this.panel2.Controls.Add (this.serialPort1Label);
      this.panel2.Controls.Add (this.serialPort2Label);
      this.panel2.Controls.Add (this.serialPort3Label);
      this.panel2.Controls.Add (this.serialPot0Label);
      this.panel2.Controls.Add (this.scanRate);
      this.panel2.Controls.Add (this.scanRateLabel);
      this.panel2.Location = new System.Drawing.Point (433, 153);
      this.panel2.Name = "panel2";
      this.panel2.Size = new System.Drawing.Size (350, 429);
      this.panel2.TabIndex = 78;
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label4.Location = new System.Drawing.Point (88, 4);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size (138, 16);
      this.label4.TabIndex = 69;
      this.label4.Text = "SIPPER Configuration";
      // 
      // UseDataBase
      // 
      this.UseDataBase.AutoSize = true;
      this.UseDataBase.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
      this.UseDataBase.Location = new System.Drawing.Point (15, 115);
      this.UseDataBase.Name = "UseDataBase";
      this.UseDataBase.Size = new System.Drawing.Size (98, 17);
      this.UseDataBase.TabIndex = 11;
      this.UseDataBase.Text = "Use Data Base";
      this.UseDataBase.UseVisualStyleBackColor = true;
      // 
      // SipperConfigForm
      // 
      this.AutoScaleBaseSize = new System.Drawing.Size (5, 13);
      this.ClientSize = new System.Drawing.Size (802, 642);
      this.Controls.Add (this.UseDataBase);
      this.Controls.Add (this.panel2);
      this.Controls.Add (this.panel1);
      this.Controls.Add (this.headerSpaceLeft);
      this.Controls.Add (this.headerSpaceLeftLabel);
      this.Controls.Add (this.headerSpaceUsed);
      this.Controls.Add (this.headerSpaceUsedLabel);
      this.Controls.Add (this.sipperFileName);
      this.Controls.Add (this.sipperFileNameLabel);
      this.Controls.Add (this.bt_Save);
      this.Controls.Add (this.label2);
      this.Controls.Add (this.ipPort);
      this.Controls.Add (this.label1);
      this.Controls.Add (this.ipAddress);
      this.Name = "SipperConfigForm";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "00:00:00.00";
      this.Load += new System.EventHandler (this.SipperConfigForm_Load);
      this.Closed += new System.EventHandler (this.SipperConfig_Closed);
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.panel1.ResumeLayout (false);
      this.panel1.PerformLayout ();
      this.panel2.ResumeLayout (false);
      this.panel2.PerformLayout ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }
    #endregion



    private void bt_Save_Click (object sender, System.EventArgs e)
    {
      //if  (!PicesSipperVariables.AllowUpdates ())
      //{
      //  PicesSipperVariables.UpdatesNotAllowed ("SIPPER Configuration Maintenance");
      //  return;
      //}
      
      string[] errorMsgs = ValidateAllFields ();
      if  (errorMsgs == null)
      {
        errorProvider1.SetError (this, "");
        UpdateSipperConfigRecFromScreenFields ();
        userSavedDataToFile = true;
        Close ();
      }
      else
      {
        errorProvider1.SetError (this, "Errors were found on the form.");
        /*
                string  msg = "";
                for  (int x = 0;  x < errorMsgs.Length;  x++)
                {
                  if  (x > 0)
                    msg += "\n";
                  msg += errorMsgs[x];
                }

                MessageBox.Show (msg, "Validtion Errors");
                return;
        */        
      }			
    }  /* bt_Save_Click */




    private void SipperConfig_Closed (object sender, System.EventArgs e)
    {
      /*
      StreamWriter Writer = new StreamWriter ("SipperConfig_temp.txt");
      Writer.WriteLine ("ip="+tb_ip.Text); 
      Writer.WriteLine ("port="+tb_port.Text); 
      Writer.WriteLine ("path="+tb_path.Text); 
      Writer.WriteLine ("quicksavepath="+tb_savepath.Text); 
      Writer.Close ();	

      Save_Setting ();*/
      //UpdateSipperConfigRecFromScreenFields ();
    }	


    static  private  int  StrToInt (string  s)
    {
      int  i = 0;
      try    {i = Int32.Parse (s);}
      catch  {i = 0;}
      return i;
    }  /* StrToInt */




    private void  UpdateSipperConfigRecFromScreenFields ()
    {
      if  (sipperConfigRec == null)
        sipperConfigRec = new SipperConfigRec ();

      sipperConfigRec.IpAddress (ipAddress.Text);
      sipperConfigRec.IpPort    (StrToInt (ipPort.Text));
      sipperConfigRec.UseDataBase (UseDataBase.Checked);
      
      SipperHeaderRec headerRec = sipperConfigRec.HeaderRec ();

      UpdateSipperHeaderRecFromScreenFields (headerRec);

      sipperConfigRec.Save ();
    }  /* UpdateSipperConfigRecFromScreenFields */



    void  UpdateSipperHeaderRecFromScreenFields (SipperHeaderRec  headerRec)
    {
      headerRec.SipperFileName     (sipperFileName.Text);
      headerRec.CruiseLocation     (cruiseLocation.Text);
      headerRec.Objective          (objective.Text);
      headerRec.Principal          (principal.Text);
      headerRec.ResearchOrg        (researchOrg.Text);
      headerRec.CruiseDateStart    (cruiseDateStart.Value);
      headerRec.CruiseDateEnd      (cruiseDateEnd.Value);
      headerRec.DeploymentVehicle  (deploymentVehicle.Text);
      headerRec.SeaConditions      (seaConditions.Text);
      headerRec.WaveHeight         (waveHeight.Text);
      headerRec.WeatherConditions  (weatherConditions.Text);
      headerRec.AirTemp            (airTemp.Text);

      headerRec.Latitude           (PicesKKStr.StrToLatitude  (latitude.Text));
      headerRec.Longitude          (PicesKKStr.StrToLongitude (longitude.Text));
      headerRec.ScanRate           (PicesKKStr.StrToInt (scanRate.Text));

      headerRec.SerialPort         (0, serialPort0.Text, serialPort0Text.Text);
      headerRec.SerialPort         (1, serialPort1.Text, serialPort1Text.Text);
      headerRec.SerialPort         (2, serialPort2.Text, serialPort2Text.Text);
      headerRec.SerialPort         (3, serialPort3.Text, serialPort3Text.Text);

      headerRec.CtdExt0Code        (ctdExt0Code.Text);
      headerRec.CtdExt1Code        (ctdExt1Code.Text);
      headerRec.CtdExt2Code        (ctdExt2Code.Text);
      headerRec.CtdExt3Code        (ctdExt3Code.Text);
      
    }  /* UpdateSipperHeaderRecFromScreenFields */


    
    private void  UpdateScreenFieldsFromSipperConfigRec ()
    {

      ipAddress.Text         = sipperConfigRec.IpAddressStr ();
      ipPort.Text            = sipperConfigRec.IpPort ().ToString ();
      UseDataBase.Checked    = sipperConfigRec.UseDataBase ();
     
      SipperHeaderRec headerRec = sipperConfigRec.HeaderRec ();
      
      cruiseLocation.Text    = headerRec.CruiseLocation ();
      objective.Text         = headerRec.Objective ();
      principal.Text         = headerRec.Principal ();
      researchOrg.Text       = headerRec.ResearchOrg ();
      cruiseDateStart.Value  = headerRec.CruiseDateStart ();
      cruiseDateEnd.Value    = headerRec.CruiseDateEnd ();
      deploymentVehicle.Text = headerRec.DeploymentVehicle ();
      seaConditions.Text     = headerRec.SeaConditions ();
      waveHeight.Text        = headerRec.WaveHeight ();
      weatherConditions.Text = headerRec.WeatherConditions ();
      airTemp.Text           = headerRec.AirTemp ();
      latitude.Text          = PicesKKStr.LatitudeToStr  (headerRec.Latitude  ());
      longitude.Text         = PicesKKStr.LongitudeToStr (headerRec.Longitude ());
      scanRate.Text          = headerRec.ScanRate ().ToString ();

      serialPort0.Text       = headerRec.SerialPortShortName (0);
      serialPort1.Text       = headerRec.SerialPortShortName (1);
      serialPort2.Text       = headerRec.SerialPortShortName (2);
      serialPort3.Text       = headerRec.SerialPortShortName (3);

      serialPort0Text.Text   = headerRec.SerialPortDesc (0);
      serialPort1Text.Text   = headerRec.SerialPortDesc (1);
      serialPort2Text.Text   = headerRec.SerialPortDesc (2);
      serialPort3Text.Text   = headerRec.SerialPortDesc (3);
      
      UpdateSerialPortScreenSettings (serialPort0, serialPort0Text);
      UpdateSerialPortScreenSettings (serialPort1, serialPort1Text);
      UpdateSerialPortScreenSettings (serialPort2, serialPort2Text);
      UpdateSerialPortScreenSettings (serialPort3, serialPort3Text);

      ctdExt0Code.Text = headerRec.CtdExt0Code ();
      ctdExt1Code.Text = headerRec.CtdExt1Code ();
      ctdExt2Code.Text = headerRec.CtdExt2Code ();
      ctdExt3Code.Text = headerRec.CtdExt3Code ();

      sipperFileName.Text    = headerRec.SipperFileName ();
    }  /* UpdateScreenFieldsFromSipperConfigRec */




    private  string  ValidateIPAddress ()
    {
      try
      {
        IPAddress i = IPAddress.Parse (ipAddress.Text);
        return "";
      }
      catch  (Exception e)
      {
        return e.ToString ();
      }
    }  /* ValidateIPAddress */




    private  string  ValidateIPPort ()
    {
      try  
      {
        int  pn = Int32.Parse (ipPort.Text);
        if  ((pn < 0)  ||  (pn > 99999))
          return  "Invalid port number.";
        else
          return "";
      }
      catch  (Exception)
      {
        return  "Port number not a valid integer.";
      }
    }  /* ValidateIPPort */



    private  string  ValidateFileName (string  fn)
    {
      try
      {
        FileInfo  i = new FileInfo (fn);
        return "";
      }
      catch  (SecurityException)            {return "User lacks required permissions.";}
      catch  (UnauthorizedAccessException)  {return "Access to " + fn + " is denide.";}
      catch  (PathTooLongException)         {return "Path name is too long.";}
      catch  (NotSupportedException)        {return "Invalid File Name.";}
      catch  (Exception  e)
      {
        string[]  s = e.ToString ().Split ('\n');
        return  s[0];
      }
    }  /* ValidateFileName */




    private string ValidateDirName (string dn)
    {
      try
      {
        DirectoryInfo  i = new DirectoryInfo (dn);
        return "";
      }
      catch (ArgumentException)    { return "path contains invalid characters such as \", <, >, or |.\";";}
      catch (PathTooLongException) { return "Path name is too long."; }
      catch (SecurityException)    { return "User lacks required permissions."; }
      catch (Exception e)
      {
        string[] s = e.ToString ().Split ('\n');
        return s[0];
      }
    }  /* ValidateDirName */





    private string  ValidateSerialPort (int       serialPortNum,
      ComboBox  shortName, 
      TextBox   desc
      )
    {
      string  errMsg = "";

      if  (shortName.Text.ToLower() == "none")
      {
        desc.Text = "";
      }

      else
      {
        string spDesc = "Serial port[" + serialPortNum.ToString () + "]";
        Instrument i = Instrument.LookUpByShortName (shortName.Text);
        if  (i == null)
        {
          errMsg = spDesc + " Invalid Instrument[" + shortName + "]";
        }
        else if  (i.Other ())
        {
          if  (desc.Text == "")
          {
            errMsg = spDesc + " Missing Description.";
            errorProvider1.SetError (desc, errMsg);
          }
        }
      }

      if  (errMsg == "")
      {
        errorProvider1.SetError (shortName, "");
        errorProvider1.SetError (desc,      "");
      }

      return errMsg;
    }  /* ValidateSerialPort */




    private  string  ValidateSipperFileName ()
    {
      string t = sipperFileName.Text.Trim ();
      if  (t == "")
        return "Empty Name";

      if  (t.Length > SipperHeaderRec.MaxSipperFileNameLen)
      {
        return "Length[" + t.Length.ToString () + "] is greater than " + SipperHeaderRec.MaxSipperFileNameLen.ToString () + ".";
      }

      char fc = t[0];
      bool firstCharIsALetter = (((fc >= 'A')  &&  (fc <= 'Z')) || ((fc >= 'a')  &&  (fc <= 'z')));
      if  (!firstCharIsALetter)
      {
        return  "First Char[" + fc.ToString () + "] must be a letter[A-Z,a-z].";
      }

      string  validChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
      string invalidChractersUsed = "";
      for  (int x = 0;  x < t.Length;  x++)
      {
        char c = t[x];
        if  (validChars.IndexOf (c) < 0)
          invalidChractersUsed += c.ToString ();
      }
    
      if  (invalidChractersUsed != "")
      {
        return  "Invalid Chatracters[" + invalidChractersUsed + "]";
      }

      return "";
    }  /* ValidateSipperFileName */




    private string[]  ValidateAllFields ()
    {
      ArrayList  errors = new ArrayList ();
      string errorMsg;

    
      errorMsg = ValidateSipperFileName ();
      if  (errorMsg != "")
      {
        errorProvider1.SetError (sipperFileName, errorMsg);
        errors.Add (errorMsg);
      }
      else
      {
        errorProvider1.SetError (sipperFileName, "");
      }
      
      errorMsg = ValidateIPAddress ();
      if  (errorMsg != "")
      {
        string  t = "Invalid IP Address[" + errorMsg + "]";
        errorProvider1.SetError (ipAddress, errorMsg);
        errors.Add (t);
      }
      else
      {
        errorProvider1.SetError (ipAddress, "");
      }

      errorMsg = ValidateIPPort ();
      if  (errorMsg != "")
      {
        string  t = "Invalid Port Number[" + ipPort.Text + "]";
        errorProvider1.SetError (ipPort, errorMsg);
        errors.Add (t);
      }
      else
      {
        errorProvider1.SetError (ipPort, "");
      }


      if  (cruiseDateStart.Value.CompareTo (cruiseDateEnd.Value) > 0)
      {
        string  t = "Cruise 'Start_Date' is later that 'End_Date'.";
        errorProvider1.SetError (cruiseDateEnd, t);
        errors.Add (t);
      }
      else
      {
        errorProvider1.SetError (cruiseDateEnd, "");
      }

      
      errorMsg = ValidateSerialPort (0, serialPort0, serialPort0Text);
      if  (errorMsg != "")
      {
        errors.Add (errorMsg);
      }      

      errorMsg = ValidateSerialPort (1, serialPort1, serialPort1Text);
      if  (errorMsg != "")
      {
        errors.Add (errorMsg);
      }      

      errorMsg = ValidateSerialPort (2, serialPort2, serialPort2Text);
      if  (errorMsg != "")
      {
        errors.Add (errorMsg);
      }      

      errorMsg = ValidateSerialPort (3, serialPort3, serialPort3Text);
      if  (errorMsg != "")
      {
        errors.Add (errorMsg);
      }      

      errorMsg = UpdateHeaderSpaceFields ();
      if  (errorMsg != "")
      {
        errors.Add (errorMsg);
      }      

      if  (errors.Count == 0)
      {
        return null;
      }
      else
      {
       
        string[]  errorMsgs = (string[])errors.ToArray (typeof (string));
        /*  
        string[]  errorMsgs = new string[errors.Count];
        for  (int x = 0;  x < errors.Count;  x++)
          errors[x] = errors.
        */
        return  errorMsgs;
      }
    }  /* ValidateAllFields */



    private void SipperConfigForm_Load(object sender, System.EventArgs e)
    {
      Instrument.PopulateComboBox (serialPort0);
      Instrument.PopulateComboBox (serialPort1);
      Instrument.PopulateComboBox (serialPort2);
      Instrument.PopulateComboBox (serialPort3);

      UpdateScreenFieldsFromSipperConfigRec ();
      UpdateHeaderSpaceFields ();
    }



    private string  UpdateHeaderSpaceFields ()
    { 
      string  errMsg = "";

      SipperHeaderRec temp = new SipperHeaderRec ();
      UpdateSipperHeaderRecFromScreenFields (temp);
      string headerStr = temp.HeaderStr ();
      int headerSpaceUsed = headerStr.Length;
      int headerSpaceLeft = SipperHeaderRec.MaxHeaderLen - headerSpaceUsed;
      this.headerSpaceUsed.Text = headerSpaceUsed.ToString ();
      this.headerSpaceLeft.Text = headerSpaceLeft.ToString ();
      if  (headerSpaceLeft < 0)
      {
        errMsg = "The maximum allowable space allowed for the hearder field has been exceeded.";
        errorProvider1.SetError (this.headerSpaceLeft, errMsg);
      }
      else
      {
        errorProvider1.SetError (this.headerSpaceLeft, "");
      }

      return  errMsg;
    }  /* UpdateHeaderSpaceFields */



    private void  UpdateSerialPortScreenSettings (ComboBox  shortName,
                                                  TextBox   desc)
    {
      if  (shortName.Text == "")
      {
        shortName.Text = "None";
        desc.Text = "";
        desc.Enabled = false;
      }

      if  (shortName.Text.ToLower () == "none")
      {
        desc.Text = "";
        desc.Enabled = false;
      }
      else
      {
        Instrument i = Instrument.LookUpByShortName (shortName.Text);
        if  (i == null)  
        {
          desc.Enabled = false;
          desc.Text    = "*Unknown*";
        }

        else if  (i.Other ())
        {
          desc.Enabled = true;
        }
      
        else
        {
          desc.Enabled = false;
          desc.Text    = i.Description ();
        }
      }

      UpdateHeaderSpaceFields ();
    }  /* UpdateSerialPortScreenSettings */



    private void  ValidateLatitudeField (MaskedTextBox f, 
                                         System.ComponentModel.CancelEventArgs e
                                        )
    {
      double l = PicesKKStr.StrToLatitude (f.Text);
      f.Text = PicesKKStr.LatitudeToStr (l);
    }  /* ValidateLatitudeField */


    
    private void  ValidateLongitudeField (MaskedTextBox                         f, 
                                          System.ComponentModel.CancelEventArgs e
                                         )
    {
      double l = PicesKKStr.StrToLongitude (f.Text);
      f.Text = PicesKKStr.LongitudeToStr (l);
    }  /* ValidateLongitudeField */



    private void ipAddress_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      string  errMsg = this.ValidateIPAddress ();
      if  (errMsg != "")
      {
        // Cancel the event and select the text to be corrected by the user.
        e.Cancel = true;
        ipAddress.Select(0, ipAddress.Text.Length);

        // Set the ErrorProvider error with the text to display. 
        this.errorProvider1.SetError (ipAddress, errMsg);
      }

      UpdateHeaderSpaceFields ();
    }

    private void ipAddress_Validated(object sender, System.EventArgs e)
    {
      errorProvider1.SetError(ipAddress, "");
    }


    private void sipperFileName_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
      string  errorMsg = this.ValidateSipperFileName ();
      if  (errorMsg != "")
      {
        e.Cancel = true;
        sipperFileName.Select(0, sipperFileName.Text.Length);
        errorProvider1.SetError (sipperFileName, errorMsg);
      }
    }


    private void   sipperFileName_Validated (object sender, System.EventArgs e)
    {
      errorProvider1.SetError (sipperFileName, "");
    }


    private void serialPort0_SelectedIndexChanged(object sender, System.EventArgs e)
    {
      UpdateSerialPortScreenSettings (serialPort0, serialPort0Text);
    }


    private void serialPort1_SelectedIndexChanged(object sender, System.EventArgs e)
    {
      UpdateSerialPortScreenSettings (serialPort1, serialPort1Text);
    }


    private void serialPort2_SelectedIndexChanged(object sender, System.EventArgs e)
    {
      UpdateSerialPortScreenSettings (serialPort2, serialPort2Text);
    }


    private void serialPort3_SelectedIndexChanged(object sender, System.EventArgs e)
    {
      UpdateSerialPortScreenSettings (serialPort3, serialPort3Text);
    }


    private void ipPort_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
      string  errorMsg = this.ValidateIPPort ();
      if  (errorMsg != "")
      {
        e.Cancel = true;
        ipPort.Select(0, ipPort.Text.Length);
        errorProvider1.SetError (ipPort, errorMsg);
      }

    }

    private void ipPort_Validated(object sender, System.EventArgs e)
    {
      errorProvider1.SetError (ipPort, "");
    }

    private void cruiseLocation_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
    }

    private void objective_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
    }

    private void principal_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
    }

    private void researchOrg_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
    }

    private void deploymentVehicle_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
    }

    private void seaConditions_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
    }

    private void waveHeight_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
    }

    private void weatherConditions_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
    }

    private void airTemp_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      UpdateHeaderSpaceFields ();
    }

    private void serialPortXText_Validating (object sender, System.ComponentModel.CancelEventArgs e,
                                             ComboBox  shortName,  TextBox  desc)
    {
      desc.Text = desc.Text.Trim ();
      UpdateHeaderSpaceFields ();
      if  (shortName.Text == "")
        return;

      Instrument i = Instrument.LookUpByShortName (shortName.Text);
      if  (i == null)
        return;

      if  (!i.Other ())
        return;

      
      if  (desc.Text == "")
      {
        e.Cancel = true;
        desc.Select(0, desc.Text.Length);
        errorProvider1.SetError (desc, "'Other' Instrument requires description field.");
      }

    }  /* serialPortXText_Validating */



    private void serialPort0Text_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      serialPortXText_Validating (sender, e, serialPort0, serialPort0Text);
    }

    private void serialPort1Text_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      serialPortXText_Validating (sender, e, serialPort1, serialPort1Text);
    }

    private void serialPort2Text_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      serialPortXText_Validating (sender, e, serialPort2, serialPort2Text);
    }

    private void serialPort3Text_Validating(object sender, System.ComponentModel.CancelEventArgs e)
    {
      serialPortXText_Validating (sender, e, serialPort3, serialPort3Text);
    }

    private void serialPort0Text_Validated(object sender, System.EventArgs e)
    {
      errorProvider1.SetError (serialPort0Text, "");
    }

    private void serialPort1Text_Validated(object sender, System.EventArgs e)
    {
      errorProvider1.SetError (serialPort1Text, "");
    }

    private void serialPort2Text_Validated(object sender, System.EventArgs e)
    {
      errorProvider1.SetError (serialPort2Text, "");
    }

    private void serialPort3Text_Validated(object sender, System.EventArgs e)
    {
      errorProvider1.SetError (serialPort3Text, "");
    }

    private void latitude_Validating(object sender, CancelEventArgs e)
    {
      ValidateLatitudeField (latitude, e);
    }

    private void latitude_Validated (object sender, EventArgs e)
    {
      errorProvider1.SetError (latitude, "");
    }

    private void longitude_Validating(object sender, CancelEventArgs e)
    {
      ValidateLongitudeField (longitude, e);
    }

    private void longitude_Validated(object sender, EventArgs e)
    {
      errorProvider1.SetError(longitude, "");
    }
  }
}
