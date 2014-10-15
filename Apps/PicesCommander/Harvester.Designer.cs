namespace PicesCommander
{
  partial class Harvester
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container ();
      this.TitleLabel = new System.Windows.Forms.Label ();
      this.SelectionCriteriaPanel = new System.Windows.Forms.Panel ();
      this.SelectedClassLabel = new System.Windows.Forms.Label ();
      this.SelectedClass = new System.Windows.Forms.TextBox ();
      this.SizeRange = new System.Windows.Forms.TextBox ();
      this.SizePrametersLabel = new System.Windows.Forms.Label ();
      this.SipperFileName = new System.Windows.Forms.TextBox ();
      this.SipperFileLabel = new System.Windows.Forms.Label ();
      this.GroupName = new System.Windows.Forms.TextBox ();
      this.GroupNameLabel = new System.Windows.Forms.Label ();
      this.MinimumDepth = new System.Windows.Forms.TextBox ();
      this.MinimumDepthLabel = new System.Windows.Forms.Label ();
      this.HowManyToHarvest = new System.Windows.Forms.TextBox ();
      this.NumberToHarvestLabel = new System.Windows.Forms.Label ();
      this.DepthIncrement = new System.Windows.Forms.ComboBox ();
      this.DepthIncrementLabel = new System.Windows.Forms.Label ();
      this.Weight = new System.Windows.Forms.CheckBox ();
      this.WeightLabel = new System.Windows.Forms.Label ();
      this.Deployment = new System.Windows.Forms.TextBox ();
      this.DeploymentLabel = new System.Windows.Forms.Label ();
      this.CruiseLabel = new System.Windows.Forms.Label ();
      this.Station = new System.Windows.Forms.TextBox ();
      this.Cruise = new System.Windows.Forms.TextBox ();
      this.StationLabel = new System.Windows.Forms.Label ();
      this.RunLog = new System.Windows.Forms.TextBox ();
      this.StartButon = new System.Windows.Forms.Button ();
      this.CancelHarvester = new System.Windows.Forms.Button ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.harvesterTimer = new System.Windows.Forms.Timer (this.components);
      this.SelectionCriteriaPanel.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (229, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (99, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Harvester";
      // 
      // SelectionCriteriaPanel
      // 
      this.SelectionCriteriaPanel.BackColor = System.Drawing.Color.FromArgb (((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
      this.SelectionCriteriaPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.SelectionCriteriaPanel.Controls.Add (this.SelectedClassLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.SelectedClass);
      this.SelectionCriteriaPanel.Controls.Add (this.SizeRange);
      this.SelectionCriteriaPanel.Controls.Add (this.SizePrametersLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.SipperFileName);
      this.SelectionCriteriaPanel.Controls.Add (this.SipperFileLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.GroupName);
      this.SelectionCriteriaPanel.Controls.Add (this.GroupNameLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.MinimumDepth);
      this.SelectionCriteriaPanel.Controls.Add (this.MinimumDepthLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.HowManyToHarvest);
      this.SelectionCriteriaPanel.Controls.Add (this.NumberToHarvestLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.DepthIncrement);
      this.SelectionCriteriaPanel.Controls.Add (this.DepthIncrementLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.Weight);
      this.SelectionCriteriaPanel.Controls.Add (this.WeightLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.Deployment);
      this.SelectionCriteriaPanel.Controls.Add (this.DeploymentLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.CruiseLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.Station);
      this.SelectionCriteriaPanel.Controls.Add (this.Cruise);
      this.SelectionCriteriaPanel.Controls.Add (this.StationLabel);
      this.SelectionCriteriaPanel.Location = new System.Drawing.Point (38, 49);
      this.SelectionCriteriaPanel.Name = "SelectionCriteriaPanel";
      this.SelectionCriteriaPanel.Size = new System.Drawing.Size (462, 243);
      this.SelectionCriteriaPanel.TabIndex = 23;
      // 
      // SelectedClassLabel
      // 
      this.SelectedClassLabel.AutoSize = true;
      this.SelectedClassLabel.Location = new System.Drawing.Point (14, 139);
      this.SelectedClassLabel.Name = "SelectedClassLabel";
      this.SelectedClassLabel.Size = new System.Drawing.Size (50, 13);
      this.SelectedClassLabel.TabIndex = 28;
      this.SelectedClassLabel.Text = "Sel Class";
      // 
      // SelectedClass
      // 
      this.SelectedClass.Location = new System.Drawing.Point (85, 135);
      this.SelectedClass.Name = "SelectedClass";
      this.SelectedClass.ReadOnly = true;
      this.SelectedClass.Size = new System.Drawing.Size (102, 20);
      this.SelectedClass.TabIndex = 27;
      this.SelectedClass.TabStop = false;
      // 
      // SizeRange
      // 
      this.SizeRange.Location = new System.Drawing.Point (85, 172);
      this.SizeRange.Name = "SizeRange";
      this.SizeRange.ReadOnly = true;
      this.SizeRange.Size = new System.Drawing.Size (102, 20);
      this.SizeRange.TabIndex = 26;
      this.SizeRange.TabStop = false;
      // 
      // SizePrametersLabel
      // 
      this.SizePrametersLabel.AutoSize = true;
      this.SizePrametersLabel.Location = new System.Drawing.Point (14, 175);
      this.SizePrametersLabel.Name = "SizePrametersLabel";
      this.SizePrametersLabel.Size = new System.Drawing.Size (62, 13);
      this.SizePrametersLabel.TabIndex = 25;
      this.SizePrametersLabel.Text = "Size Range";
      // 
      // SipperFileName
      // 
      this.SipperFileName.Location = new System.Drawing.Point (85, 101);
      this.SipperFileName.Name = "SipperFileName";
      this.SipperFileName.ReadOnly = true;
      this.SipperFileName.Size = new System.Drawing.Size (102, 20);
      this.SipperFileName.TabIndex = 25;
      this.SipperFileName.TabStop = false;
      // 
      // SipperFileLabel
      // 
      this.SipperFileLabel.AutoSize = true;
      this.SipperFileLabel.Location = new System.Drawing.Point (14, 104);
      this.SipperFileLabel.Name = "SipperFileLabel";
      this.SipperFileLabel.Size = new System.Drawing.Size (53, 13);
      this.SipperFileLabel.TabIndex = 23;
      this.SipperFileLabel.Text = "SipperFile";
      // 
      // GroupName
      // 
      this.GroupName.Location = new System.Drawing.Point (85, 211);
      this.GroupName.Name = "GroupName";
      this.GroupName.Size = new System.Drawing.Size (356, 20);
      this.GroupName.TabIndex = 22;
      // 
      // GroupNameLabel
      // 
      this.GroupNameLabel.AutoSize = true;
      this.GroupNameLabel.Location = new System.Drawing.Point (14, 214);
      this.GroupNameLabel.Name = "GroupNameLabel";
      this.GroupNameLabel.Size = new System.Drawing.Size (67, 13);
      this.GroupNameLabel.TabIndex = 21;
      this.GroupNameLabel.Text = "Group Name";
      // 
      // MinimumDepth
      // 
      this.MinimumDepth.Location = new System.Drawing.Point (339, 100);
      this.MinimumDepth.Name = "MinimumDepth";
      this.MinimumDepth.Size = new System.Drawing.Size (102, 20);
      this.MinimumDepth.TabIndex = 20;
      this.MinimumDepth.Leave += new System.EventHandler (this.MinimumDepth_Leave);
      // 
      // MinimumDepthLabel
      // 
      this.MinimumDepthLabel.AutoSize = true;
      this.MinimumDepthLabel.Location = new System.Drawing.Point (248, 103);
      this.MinimumDepthLabel.Name = "MinimumDepthLabel";
      this.MinimumDepthLabel.Size = new System.Drawing.Size (80, 13);
      this.MinimumDepthLabel.TabIndex = 19;
      this.MinimumDepthLabel.Text = "Minimum Depth";
      // 
      // HowManyToHarvest
      // 
      this.HowManyToHarvest.Location = new System.Drawing.Point (339, 69);
      this.HowManyToHarvest.Name = "HowManyToHarvest";
      this.HowManyToHarvest.Size = new System.Drawing.Size (102, 20);
      this.HowManyToHarvest.TabIndex = 18;
      this.HowManyToHarvest.Leave += new System.EventHandler (this.HowManyToHarvest_Leave);
      // 
      // NumberToHarvestLabel
      // 
      this.NumberToHarvestLabel.AutoSize = true;
      this.NumberToHarvestLabel.Location = new System.Drawing.Point (248, 74);
      this.NumberToHarvestLabel.Name = "NumberToHarvestLabel";
      this.NumberToHarvestLabel.Size = new System.Drawing.Size (58, 13);
      this.NumberToHarvestLabel.TabIndex = 17;
      this.NumberToHarvestLabel.Text = "How Many";
      // 
      // DepthIncrement
      // 
      this.DepthIncrement.FormattingEnabled = true;
      this.DepthIncrement.Items.AddRange (new object[] {
            "1 Meter",
            "2 Meters",
            "5 Meters",
            "10 Meters",
            "15 Meters",
            "20 Meters",
            "30 Meters",
            "50 Meters",
            "100 Meters"});
      this.DepthIncrement.Location = new System.Drawing.Point (339, 39);
      this.DepthIncrement.Name = "DepthIncrement";
      this.DepthIncrement.Size = new System.Drawing.Size (102, 21);
      this.DepthIncrement.TabIndex = 16;
      this.DepthIncrement.SelectedIndexChanged += new System.EventHandler (this.DepthIncrement_SelectedIndexChanged);
      // 
      // DepthIncrementLabel
      // 
      this.DepthIncrementLabel.AutoSize = true;
      this.DepthIncrementLabel.Location = new System.Drawing.Point (248, 44);
      this.DepthIncrementLabel.Name = "DepthIncrementLabel";
      this.DepthIncrementLabel.Size = new System.Drawing.Size (86, 13);
      this.DepthIncrementLabel.TabIndex = 15;
      this.DepthIncrementLabel.Text = "Depth Increment";
      // 
      // Weight
      // 
      this.Weight.AutoSize = true;
      this.Weight.Location = new System.Drawing.Point (339, 12);
      this.Weight.Name = "Weight";
      this.Weight.Size = new System.Drawing.Size (15, 14);
      this.Weight.TabIndex = 14;
      this.Weight.UseVisualStyleBackColor = true;
      this.Weight.CheckedChanged += new System.EventHandler (this.Weight_CheckedChanged);
      // 
      // WeightLabel
      // 
      this.WeightLabel.AutoSize = true;
      this.WeightLabel.Location = new System.Drawing.Point (248, 13);
      this.WeightLabel.Name = "WeightLabel";
      this.WeightLabel.Size = new System.Drawing.Size (41, 13);
      this.WeightLabel.TabIndex = 13;
      this.WeightLabel.Text = "Weight";
      // 
      // Deployment
      // 
      this.Deployment.Location = new System.Drawing.Point (85, 69);
      this.Deployment.Name = "Deployment";
      this.Deployment.ReadOnly = true;
      this.Deployment.Size = new System.Drawing.Size (102, 20);
      this.Deployment.TabIndex = 8;
      this.Deployment.TabStop = false;
      // 
      // DeploymentLabel
      // 
      this.DeploymentLabel.AutoSize = true;
      this.DeploymentLabel.Location = new System.Drawing.Point (14, 74);
      this.DeploymentLabel.Name = "DeploymentLabel";
      this.DeploymentLabel.Size = new System.Drawing.Size (63, 13);
      this.DeploymentLabel.TabIndex = 7;
      this.DeploymentLabel.Text = "Deployment";
      // 
      // CruiseLabel
      // 
      this.CruiseLabel.AutoSize = true;
      this.CruiseLabel.Location = new System.Drawing.Point (14, 14);
      this.CruiseLabel.Name = "CruiseLabel";
      this.CruiseLabel.Size = new System.Drawing.Size (36, 13);
      this.CruiseLabel.TabIndex = 3;
      this.CruiseLabel.Text = "Cruise";
      // 
      // Station
      // 
      this.Station.Location = new System.Drawing.Point (85, 39);
      this.Station.Name = "Station";
      this.Station.ReadOnly = true;
      this.Station.Size = new System.Drawing.Size (102, 20);
      this.Station.TabIndex = 6;
      this.Station.TabStop = false;
      // 
      // Cruise
      // 
      this.Cruise.Location = new System.Drawing.Point (85, 9);
      this.Cruise.Name = "Cruise";
      this.Cruise.ReadOnly = true;
      this.Cruise.Size = new System.Drawing.Size (102, 20);
      this.Cruise.TabIndex = 4;
      this.Cruise.TabStop = false;
      // 
      // StationLabel
      // 
      this.StationLabel.AutoSize = true;
      this.StationLabel.Location = new System.Drawing.Point (14, 44);
      this.StationLabel.Name = "StationLabel";
      this.StationLabel.Size = new System.Drawing.Size (40, 13);
      this.StationLabel.TabIndex = 5;
      this.StationLabel.Text = "Station";
      // 
      // RunLog
      // 
      this.RunLog.BackColor = System.Drawing.SystemColors.ControlLight;
      this.RunLog.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.RunLog.Location = new System.Drawing.Point (38, 315);
      this.RunLog.Multiline = true;
      this.RunLog.Name = "RunLog";
      this.RunLog.ReadOnly = true;
      this.RunLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
      this.RunLog.Size = new System.Drawing.Size (462, 301);
      this.RunLog.TabIndex = 24;
      // 
      // StartButon
      // 
      this.StartButon.Location = new System.Drawing.Point (38, 635);
      this.StartButon.Name = "StartButon";
      this.StartButon.Size = new System.Drawing.Size (75, 23);
      this.StartButon.TabIndex = 25;
      this.StartButon.Text = "Start";
      this.StartButon.UseVisualStyleBackColor = true;
      this.StartButon.Click += new System.EventHandler (this.StartButon_Click);
      // 
      // CancelHarvester
      // 
      this.CancelHarvester.Location = new System.Drawing.Point (425, 635);
      this.CancelHarvester.Name = "CancelHarvester";
      this.CancelHarvester.Size = new System.Drawing.Size (75, 23);
      this.CancelHarvester.TabIndex = 26;
      this.CancelHarvester.Text = "Cancel";
      this.CancelHarvester.UseVisualStyleBackColor = true;
      this.CancelHarvester.Click += new System.EventHandler (this.CancelButton_Click);
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // harvesterTimer
      // 
      this.harvesterTimer.Tick += new System.EventHandler (this.harvesterTimer_Tick);
      // 
      // Harvester
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (544, 670);
      this.Controls.Add (this.CancelHarvester);
      this.Controls.Add (this.StartButon);
      this.Controls.Add (this.RunLog);
      this.Controls.Add (this.SelectionCriteriaPanel);
      this.Controls.Add (this.TitleLabel);
      this.MaximumSize = new System.Drawing.Size (560, 708);
      this.MinimumSize = new System.Drawing.Size (560, 708);
      this.Name = "Harvester";
      this.Text = "Harvester";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.Harvester_FormClosing);
      this.SelectionCriteriaPanel.ResumeLayout (false);
      this.SelectionCriteriaPanel.PerformLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Panel SelectionCriteriaPanel;
    private System.Windows.Forms.TextBox Deployment;
    private System.Windows.Forms.Label DeploymentLabel;
    private System.Windows.Forms.Label CruiseLabel;
    private System.Windows.Forms.TextBox Station;
    private System.Windows.Forms.TextBox Cruise;
    private System.Windows.Forms.Label StationLabel;
    private System.Windows.Forms.Label WeightLabel;
    private System.Windows.Forms.ComboBox DepthIncrement;
    private System.Windows.Forms.Label DepthIncrementLabel;
    private System.Windows.Forms.CheckBox Weight;
    private System.Windows.Forms.TextBox HowManyToHarvest;
    private System.Windows.Forms.Label NumberToHarvestLabel;
    private System.Windows.Forms.TextBox RunLog;
    private System.Windows.Forms.Button StartButon;
    private System.Windows.Forms.Button CancelHarvester;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.Timer harvesterTimer;
    private System.Windows.Forms.TextBox MinimumDepth;
    private System.Windows.Forms.Label MinimumDepthLabel;
    private System.Windows.Forms.TextBox GroupName;
    private System.Windows.Forms.Label GroupNameLabel;
    private System.Windows.Forms.TextBox SipperFileName;
    private System.Windows.Forms.Label SipperFileLabel;
    private System.Windows.Forms.TextBox SizeRange;
    private System.Windows.Forms.Label SizePrametersLabel;
    private System.Windows.Forms.Label SelectedClassLabel;
    private System.Windows.Forms.TextBox SelectedClass;
  }
}