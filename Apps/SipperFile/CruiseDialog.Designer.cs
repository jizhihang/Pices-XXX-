namespace SipperFile
{
  partial class CruiseDialog
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
      this.ShipNameLabel = new System.Windows.Forms.Label ();
      this.DescriptionLabel = new System.Windows.Forms.Label ();
      this.DatesLabel = new System.Windows.Forms.Label ();
      this.DateEndLabel = new System.Windows.Forms.Label ();
      this.ObjectiveLabel = new System.Windows.Forms.Label ();
      this.LocationLabel = new System.Windows.Forms.Label ();
      this.PrincipalLabel = new System.Windows.Forms.Label ();
      this.ResearchOrgLabel = new System.Windows.Forms.Label ();
      this.ShipName = new System.Windows.Forms.TextBox ();
      this.Description = new System.Windows.Forms.TextBox ();
      this.DateStart = new System.Windows.Forms.DateTimePicker ();
      this.DateEnd = new System.Windows.Forms.DateTimePicker ();
      this.CruiseLocation = new System.Windows.Forms.TextBox ();
      this.Objective = new System.Windows.Forms.TextBox ();
      this.Principal = new System.Windows.Forms.TextBox ();
      this.ResearchOrg = new System.Windows.Forms.TextBox ();
      this.UpdateButon = new System.Windows.Forms.Button ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.CancelReqButton = new System.Windows.Forms.Button ();
      this.CruiseDetials = new System.Windows.Forms.TreeView ();
      this.StationssLabel = new System.Windows.Forms.Label ();
      this.AddStationButton = new System.Windows.Forms.Button ();
      this.CruiseNameLabel = new System.Windows.Forms.Label ();
      this.CruiseName = new System.Windows.Forms.TextBox ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (329, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (64, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Cruise";
      this.TitleLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // ShipNameLabel
      // 
      this.ShipNameLabel.AutoSize = true;
      this.ShipNameLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ShipNameLabel.Location = new System.Drawing.Point (39, 87);
      this.ShipNameLabel.Name = "ShipNameLabel";
      this.ShipNameLabel.Size = new System.Drawing.Size (75, 16);
      this.ShipNameLabel.TabIndex = 1;
      this.ShipNameLabel.Text = "Ship Name";
      this.ShipNameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // DescriptionLabel
      // 
      this.DescriptionLabel.AutoSize = true;
      this.DescriptionLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DescriptionLabel.Location = new System.Drawing.Point (39, 116);
      this.DescriptionLabel.Name = "DescriptionLabel";
      this.DescriptionLabel.Size = new System.Drawing.Size (76, 16);
      this.DescriptionLabel.TabIndex = 2;
      this.DescriptionLabel.Text = "Description";
      this.DescriptionLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // DatesLabel
      // 
      this.DatesLabel.AutoSize = true;
      this.DatesLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DatesLabel.Location = new System.Drawing.Point (39, 190);
      this.DatesLabel.Name = "DatesLabel";
      this.DatesLabel.Size = new System.Drawing.Size (85, 16);
      this.DatesLabel.TabIndex = 3;
      this.DatesLabel.Text = "Cruise Dates";
      this.DatesLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // DateEndLabel
      // 
      this.DateEndLabel.AutoSize = true;
      this.DateEndLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateEndLabel.Location = new System.Drawing.Point (391, 190);
      this.DateEndLabel.Name = "DateEndLabel";
      this.DateEndLabel.Size = new System.Drawing.Size (32, 16);
      this.DateEndLabel.TabIndex = 4;
      this.DateEndLabel.Text = "End";
      this.DateEndLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ObjectiveLabel
      // 
      this.ObjectiveLabel.AutoSize = true;
      this.ObjectiveLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ObjectiveLabel.Location = new System.Drawing.Point (38, 251);
      this.ObjectiveLabel.Name = "ObjectiveLabel";
      this.ObjectiveLabel.Size = new System.Drawing.Size (65, 16);
      this.ObjectiveLabel.TabIndex = 5;
      this.ObjectiveLabel.Text = "Objective";
      this.ObjectiveLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // LocationLabel
      // 
      this.LocationLabel.AutoSize = true;
      this.LocationLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.LocationLabel.Location = new System.Drawing.Point (39, 221);
      this.LocationLabel.Name = "LocationLabel";
      this.LocationLabel.Size = new System.Drawing.Size (59, 16);
      this.LocationLabel.TabIndex = 6;
      this.LocationLabel.Text = "Location";
      this.LocationLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // PrincipalLabel
      // 
      this.PrincipalLabel.AutoSize = true;
      this.PrincipalLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.PrincipalLabel.Location = new System.Drawing.Point (38, 281);
      this.PrincipalLabel.Name = "PrincipalLabel";
      this.PrincipalLabel.Size = new System.Drawing.Size (60, 16);
      this.PrincipalLabel.TabIndex = 7;
      this.PrincipalLabel.Text = "Principal";
      this.PrincipalLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ResearchOrgLabel
      // 
      this.ResearchOrgLabel.AutoSize = true;
      this.ResearchOrgLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ResearchOrgLabel.Location = new System.Drawing.Point (38, 311);
      this.ResearchOrgLabel.Name = "ResearchOrgLabel";
      this.ResearchOrgLabel.Size = new System.Drawing.Size (95, 16);
      this.ResearchOrgLabel.TabIndex = 8;
      this.ResearchOrgLabel.Text = "Research Org.";
      this.ResearchOrgLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ShipName
      // 
      this.ShipName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ShipName.Location = new System.Drawing.Point (136, 84);
      this.ShipName.Name = "ShipName";
      this.ShipName.Size = new System.Drawing.Size (560, 22);
      this.ShipName.TabIndex = 9;
      this.ShipName.Validating += new System.ComponentModel.CancelEventHandler (this.ShipName_Validating);
      // 
      // Description
      // 
      this.Description.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Description.Location = new System.Drawing.Point (136, 114);
      this.Description.Multiline = true;
      this.Description.Name = "Description";
      this.Description.Size = new System.Drawing.Size (560, 60);
      this.Description.TabIndex = 10;
      // 
      // DateStart
      // 
      this.DateStart.CustomFormat = "yyyy-MMM-dd (dddd) ";
      this.DateStart.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateStart.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
      this.DateStart.Location = new System.Drawing.Point (136, 186);
      this.DateStart.MinDate = new System.DateTime (1990, 1, 1, 0, 0, 0, 0);
      this.DateStart.Name = "DateStart";
      this.DateStart.Size = new System.Drawing.Size (223, 22);
      this.DateStart.TabIndex = 11;
      // 
      // DateEnd
      // 
      this.DateEnd.CustomFormat = "yyyy-MMM-dd (dddd) ";
      this.DateEnd.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateEnd.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
      this.DateEnd.Location = new System.Drawing.Point (426, 186);
      this.DateEnd.MinDate = new System.DateTime (1990, 1, 1, 0, 0, 0, 0);
      this.DateEnd.Name = "DateEnd";
      this.DateEnd.Size = new System.Drawing.Size (223, 22);
      this.DateEnd.TabIndex = 12;
      this.DateEnd.Validating += new System.ComponentModel.CancelEventHandler (this.DateEnd_Validating);
      // 
      // CruiseLocation
      // 
      this.CruiseLocation.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseLocation.Location = new System.Drawing.Point (136, 216);
      this.CruiseLocation.Name = "CruiseLocation";
      this.CruiseLocation.Size = new System.Drawing.Size (560, 22);
      this.CruiseLocation.TabIndex = 13;
      // 
      // Objective
      // 
      this.Objective.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Objective.Location = new System.Drawing.Point (136, 249);
      this.Objective.Name = "Objective";
      this.Objective.Size = new System.Drawing.Size (560, 22);
      this.Objective.TabIndex = 14;
      // 
      // Principal
      // 
      this.Principal.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Principal.Location = new System.Drawing.Point (136, 279);
      this.Principal.Name = "Principal";
      this.Principal.Size = new System.Drawing.Size (560, 22);
      this.Principal.TabIndex = 15;
      // 
      // ResearchOrg
      // 
      this.ResearchOrg.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ResearchOrg.Location = new System.Drawing.Point (136, 309);
      this.ResearchOrg.Name = "ResearchOrg";
      this.ResearchOrg.Size = new System.Drawing.Size (560, 22);
      this.ResearchOrg.TabIndex = 16;
      // 
      // UpdateButon
      // 
      this.UpdateButon.Location = new System.Drawing.Point (42, 743);
      this.UpdateButon.Name = "UpdateButon";
      this.UpdateButon.Size = new System.Drawing.Size (75, 23);
      this.UpdateButon.TabIndex = 17;
      this.UpdateButon.Text = "Update";
      this.UpdateButon.UseVisualStyleBackColor = true;
      this.UpdateButon.Click += new System.EventHandler (this.UpdateButon_Click);
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // CancelReqButton
      // 
      this.CancelReqButton.Location = new System.Drawing.Point (621, 743);
      this.CancelReqButton.Name = "CancelReqButton";
      this.CancelReqButton.Size = new System.Drawing.Size (75, 23);
      this.CancelReqButton.TabIndex = 18;
      this.CancelReqButton.Text = "Cancel";
      this.CancelReqButton.UseVisualStyleBackColor = true;
      this.CancelReqButton.Click += new System.EventHandler (this.CancelButton_Click);
      // 
      // CruiseDetials
      // 
      this.CruiseDetials.Font = new System.Drawing.Font ("Courier New", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseDetials.ItemHeight = 20;
      this.CruiseDetials.Location = new System.Drawing.Point (41, 363);
      this.CruiseDetials.Name = "CruiseDetials";
      this.CruiseDetials.Size = new System.Drawing.Size (655, 364);
      this.CruiseDetials.TabIndex = 19;
      // 
      // StationssLabel
      // 
      this.StationssLabel.AutoSize = true;
      this.StationssLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.StationssLabel.Location = new System.Drawing.Point (39, 344);
      this.StationssLabel.Name = "StationssLabel";
      this.StationssLabel.Size = new System.Drawing.Size (56, 16);
      this.StationssLabel.TabIndex = 20;
      this.StationssLabel.Text = "Stations";
      this.StationssLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // AddStationButton
      // 
      this.AddStationButton.Location = new System.Drawing.Point (586, 704);
      this.AddStationButton.Name = "AddStationButton";
      this.AddStationButton.Size = new System.Drawing.Size (110, 23);
      this.AddStationButton.TabIndex = 21;
      this.AddStationButton.Text = "Add a new Station";
      this.AddStationButton.UseVisualStyleBackColor = true;
      this.AddStationButton.Click += new System.EventHandler (this.AddStationButton_Click);
      // 
      // CruiseNameLabel
      // 
      this.CruiseNameLabel.AutoSize = true;
      this.CruiseNameLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseNameLabel.Location = new System.Drawing.Point (38, 55);
      this.CruiseNameLabel.Name = "CruiseNameLabel";
      this.CruiseNameLabel.Size = new System.Drawing.Size (83, 16);
      this.CruiseNameLabel.TabIndex = 23;
      this.CruiseNameLabel.Text = "CruiseName";
      this.CruiseNameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // CruiseName
      // 
      this.CruiseName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseName.Location = new System.Drawing.Point (136, 52);
      this.CruiseName.Name = "CruiseName";
      this.CruiseName.Size = new System.Drawing.Size (120, 22);
      this.CruiseName.TabIndex = 1;
      this.CruiseName.Validating += new System.ComponentModel.CancelEventHandler (this.CruiseName_Validating);
      // 
      // CruiseDialog
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (745, 778);
      this.Controls.Add (this.CruiseName);
      this.Controls.Add (this.CruiseNameLabel);
      this.Controls.Add (this.AddStationButton);
      this.Controls.Add (this.StationssLabel);
      this.Controls.Add (this.CruiseDetials);
      this.Controls.Add (this.CancelReqButton);
      this.Controls.Add (this.UpdateButon);
      this.Controls.Add (this.ResearchOrg);
      this.Controls.Add (this.Principal);
      this.Controls.Add (this.Objective);
      this.Controls.Add (this.CruiseLocation);
      this.Controls.Add (this.DateEnd);
      this.Controls.Add (this.DateStart);
      this.Controls.Add (this.Description);
      this.Controls.Add (this.ShipName);
      this.Controls.Add (this.ResearchOrgLabel);
      this.Controls.Add (this.PrincipalLabel);
      this.Controls.Add (this.LocationLabel);
      this.Controls.Add (this.ObjectiveLabel);
      this.Controls.Add (this.DateEndLabel);
      this.Controls.Add (this.DatesLabel);
      this.Controls.Add (this.DescriptionLabel);
      this.Controls.Add (this.ShipNameLabel);
      this.Controls.Add (this.TitleLabel);
      this.Name = "CruiseDialog";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "CruiseDialog";
      this.Load += new System.EventHandler (this.CruiseDialog_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.CruiseDialog_FormClosing);
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label ShipNameLabel;
    private System.Windows.Forms.Label DescriptionLabel;
    private System.Windows.Forms.Label DatesLabel;
    private System.Windows.Forms.Label DateEndLabel;
    private System.Windows.Forms.Label ObjectiveLabel;
    private System.Windows.Forms.Label LocationLabel;
    private System.Windows.Forms.Label PrincipalLabel;
    private System.Windows.Forms.Label ResearchOrgLabel;
    private System.Windows.Forms.TextBox ShipName;
    private System.Windows.Forms.TextBox Description;
    private System.Windows.Forms.DateTimePicker DateStart;
    private System.Windows.Forms.DateTimePicker DateEnd;
    private System.Windows.Forms.TextBox CruiseLocation;
    private System.Windows.Forms.TextBox Objective;
    private System.Windows.Forms.TextBox Principal;
    private System.Windows.Forms.TextBox ResearchOrg;
    private System.Windows.Forms.Button UpdateButon;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.Button CancelReqButton;
    private System.Windows.Forms.TreeView CruiseDetials;
    private System.Windows.Forms.Label StationssLabel;
    private System.Windows.Forms.Button AddStationButton;
    private System.Windows.Forms.TextBox CruiseName;
    private System.Windows.Forms.Label CruiseNameLabel;
  }
}