namespace SipperFile
{
  partial class DeploymentDialog
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
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (DeploymentDialog));
      this.TitleLabel = new System.Windows.Forms.Label ();
      this.CruiseNameLabel = new System.Windows.Forms.Label ();
      this.StationNameLabel = new System.Windows.Forms.Label ();
      this.DescriptionLabel = new System.Windows.Forms.Label ();
      this.LatitudeLabel = new System.Windows.Forms.Label ();
      this.CruiseName = new System.Windows.Forms.TextBox ();
      this.StationName = new System.Windows.Forms.TextBox ();
      this.Description = new System.Windows.Forms.TextBox ();
      this.Latitude = new System.Windows.Forms.TextBox ();
      this.LongitudeLabel = new System.Windows.Forms.Label ();
      this.Longitude = new System.Windows.Forms.TextBox ();
      this.DateTimeStart = new System.Windows.Forms.DateTimePicker ();
      this.UpdateButton = new System.Windows.Forms.Button ();
      this.CancelReqButton = new System.Windows.Forms.Button ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.DeploymentNumLabel = new System.Windows.Forms.Label ();
      this.DeploymentNum = new System.Windows.Forms.TextBox ();
      this.DateEndLabel = new System.Windows.Forms.Label ();
      this.DateTimeEnd = new System.Windows.Forms.DateTimePicker ();
      this.StartEndLabel = new System.Windows.Forms.Label ();
      this.SipperFiles = new System.Windows.Forms.TreeView ();
      this.SipperFilesLabel = new System.Windows.Forms.Label ();
      this.AddSipperFileButton = new System.Windows.Forms.Button ();
      this.SyncFieldsHelp = new System.Windows.Forms.TextBox ();
      this.SyncTimeStampGPS = new System.Windows.Forms.DateTimePicker ();
      this.SyncTimeStampGPSLabel = new System.Windows.Forms.Label ();
      this.SyncTimeStampCTDLabel = new System.Windows.Forms.Label ();
      this.SyncTimeStampCTD = new System.Windows.Forms.DateTimePicker ();
      this.SyncTimeStampActualLabel = new System.Windows.Forms.Label ();
      this.SyncTimeStampActual = new System.Windows.Forms.DateTimePicker ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (250, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (225, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Deployment Maintenance";
      // 
      // CruiseNameLabel
      // 
      this.CruiseNameLabel.AutoSize = true;
      this.CruiseNameLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseNameLabel.Location = new System.Drawing.Point (33, 58);
      this.CruiseNameLabel.Name = "CruiseNameLabel";
      this.CruiseNameLabel.Size = new System.Drawing.Size (36, 13);
      this.CruiseNameLabel.TabIndex = 1;
      this.CruiseNameLabel.Text = "Cruise";
      // 
      // StationNameLabel
      // 
      this.StationNameLabel.AutoSize = true;
      this.StationNameLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.StationNameLabel.Location = new System.Drawing.Point (191, 58);
      this.StationNameLabel.Name = "StationNameLabel";
      this.StationNameLabel.Size = new System.Drawing.Size (40, 13);
      this.StationNameLabel.TabIndex = 2;
      this.StationNameLabel.Text = "Station";
      // 
      // DescriptionLabel
      // 
      this.DescriptionLabel.AutoSize = true;
      this.DescriptionLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DescriptionLabel.Location = new System.Drawing.Point (33, 109);
      this.DescriptionLabel.Name = "DescriptionLabel";
      this.DescriptionLabel.Size = new System.Drawing.Size (60, 13);
      this.DescriptionLabel.TabIndex = 3;
      this.DescriptionLabel.Text = "Description";
      // 
      // LatitudeLabel
      // 
      this.LatitudeLabel.AutoSize = true;
      this.LatitudeLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.LatitudeLabel.Location = new System.Drawing.Point (33, 221);
      this.LatitudeLabel.Name = "LatitudeLabel";
      this.LatitudeLabel.Size = new System.Drawing.Size (45, 13);
      this.LatitudeLabel.TabIndex = 4;
      this.LatitudeLabel.Text = "Latitude";
      // 
      // CruiseName
      // 
      this.CruiseName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseName.Location = new System.Drawing.Point (34, 74);
      this.CruiseName.Name = "CruiseName";
      this.CruiseName.ReadOnly = true;
      this.CruiseName.Size = new System.Drawing.Size (137, 22);
      this.CruiseName.TabIndex = 6;
      this.CruiseName.TabStop = false;
      // 
      // StationName
      // 
      this.StationName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.StationName.Location = new System.Drawing.Point (194, 74);
      this.StationName.Name = "StationName";
      this.StationName.ReadOnly = true;
      this.StationName.Size = new System.Drawing.Size (137, 22);
      this.StationName.TabIndex = 7;
      this.StationName.TabStop = false;
      // 
      // Description
      // 
      this.Description.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Description.Location = new System.Drawing.Point (34, 124);
      this.Description.Multiline = true;
      this.Description.Name = "Description";
      this.Description.Size = new System.Drawing.Size (675, 78);
      this.Description.TabIndex = 9;
      // 
      // Latitude
      // 
      this.Latitude.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Latitude.Location = new System.Drawing.Point (36, 236);
      this.Latitude.Name = "Latitude";
      this.Latitude.Size = new System.Drawing.Size (145, 22);
      this.Latitude.TabIndex = 10;
      this.Latitude.Validating += new System.ComponentModel.CancelEventHandler (this.Latitude_Validating);
      // 
      // LongitudeLabel
      // 
      this.LongitudeLabel.AutoSize = true;
      this.LongitudeLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.LongitudeLabel.Location = new System.Drawing.Point (191, 221);
      this.LongitudeLabel.Name = "LongitudeLabel";
      this.LongitudeLabel.Size = new System.Drawing.Size (54, 13);
      this.LongitudeLabel.TabIndex = 10;
      this.LongitudeLabel.Text = "Longitude";
      // 
      // Longitude
      // 
      this.Longitude.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Longitude.Location = new System.Drawing.Point (194, 236);
      this.Longitude.Name = "Longitude";
      this.Longitude.Size = new System.Drawing.Size (145, 22);
      this.Longitude.TabIndex = 11;
      this.Longitude.Validating += new System.ComponentModel.CancelEventHandler (this.Longitude_Validating);
      // 
      // DateTimeStart
      // 
      this.DateTimeStart.CustomFormat = "yyyy-MMM-dd (dddd)  HH:mm:ss";
      this.DateTimeStart.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateTimeStart.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
      this.DateTimeStart.Location = new System.Drawing.Point (36, 396);
      this.DateTimeStart.Name = "DateTimeStart";
      this.DateTimeStart.Size = new System.Drawing.Size (270, 22);
      this.DateTimeStart.TabIndex = 40;
      this.DateTimeStart.TabStop = false;
      // 
      // UpdateButton
      // 
      this.UpdateButton.Location = new System.Drawing.Point (33, 633);
      this.UpdateButton.Name = "UpdateButton";
      this.UpdateButton.Size = new System.Drawing.Size (75, 23);
      this.UpdateButton.TabIndex = 60;
      this.UpdateButton.Text = "Update";
      this.UpdateButton.UseVisualStyleBackColor = true;
      this.UpdateButton.Click += new System.EventHandler (this.UpdateButton_Click);
      // 
      // CancelReqButton
      // 
      this.CancelReqButton.Location = new System.Drawing.Point (633, 633);
      this.CancelReqButton.Name = "CancelReqButton";
      this.CancelReqButton.Size = new System.Drawing.Size (75, 23);
      this.CancelReqButton.TabIndex = 65;
      this.CancelReqButton.Text = "Cancel";
      this.CancelReqButton.UseVisualStyleBackColor = true;
      this.CancelReqButton.Click += new System.EventHandler (this.CancelReqButton_Click);
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // DeploymentNumLabel
      // 
      this.DeploymentNumLabel.AutoSize = true;
      this.DeploymentNumLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DeploymentNumLabel.Location = new System.Drawing.Point (347, 58);
      this.DeploymentNumLabel.Name = "DeploymentNumLabel";
      this.DeploymentNumLabel.Size = new System.Drawing.Size (85, 13);
      this.DeploymentNumLabel.TabIndex = 16;
      this.DeploymentNumLabel.Text = "DeploymentNum";
      // 
      // DeploymentNum
      // 
      this.DeploymentNum.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DeploymentNum.Location = new System.Drawing.Point (350, 74);
      this.DeploymentNum.Name = "DeploymentNum";
      this.DeploymentNum.ReadOnly = true;
      this.DeploymentNum.Size = new System.Drawing.Size (68, 22);
      this.DeploymentNum.TabIndex = 8;
      this.DeploymentNum.Validating += new System.ComponentModel.CancelEventHandler (this.textBox1_Validating);
      // 
      // DateEndLabel
      // 
      this.DateEndLabel.AutoSize = true;
      this.DateEndLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateEndLabel.Location = new System.Drawing.Point (332, 382);
      this.DateEndLabel.Name = "DateEndLabel";
      this.DateEndLabel.Size = new System.Drawing.Size (52, 13);
      this.DateEndLabel.TabIndex = 18;
      this.DateEndLabel.Text = "End Time";
      // 
      // DateTimeEnd
      // 
      this.DateTimeEnd.CustomFormat = "yyyy-MMM-dd (dddd)  HH:mm:ss";
      this.DateTimeEnd.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateTimeEnd.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
      this.DateTimeEnd.Location = new System.Drawing.Point (335, 396);
      this.DateTimeEnd.Name = "DateTimeEnd";
      this.DateTimeEnd.Size = new System.Drawing.Size (270, 22);
      this.DateTimeEnd.TabIndex = 43;
      this.DateTimeEnd.TabStop = false;
      // 
      // StartEndLabel
      // 
      this.StartEndLabel.AutoSize = true;
      this.StartEndLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.StartEndLabel.Location = new System.Drawing.Point (33, 382);
      this.StartEndLabel.Name = "StartEndLabel";
      this.StartEndLabel.Size = new System.Drawing.Size (55, 13);
      this.StartEndLabel.TabIndex = 20;
      this.StartEndLabel.Text = "Start Time";
      // 
      // SipperFiles
      // 
      this.SipperFiles.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SipperFiles.Location = new System.Drawing.Point (34, 449);
      this.SipperFiles.Name = "SipperFiles";
      this.SipperFiles.Size = new System.Drawing.Size (675, 160);
      this.SipperFiles.TabIndex = 50;
      // 
      // SipperFilesLabel
      // 
      this.SipperFilesLabel.AutoSize = true;
      this.SipperFilesLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SipperFilesLabel.Location = new System.Drawing.Point (33, 434);
      this.SipperFilesLabel.Name = "SipperFilesLabel";
      this.SipperFilesLabel.Size = new System.Drawing.Size (61, 13);
      this.SipperFilesLabel.TabIndex = 22;
      this.SipperFilesLabel.Text = "Sipper Files";
      // 
      // AddSipperFileButton
      // 
      this.AddSipperFileButton.Location = new System.Drawing.Point (618, 586);
      this.AddSipperFileButton.Name = "AddSipperFileButton";
      this.AddSipperFileButton.Size = new System.Drawing.Size (91, 23);
      this.AddSipperFileButton.TabIndex = 23;
      this.AddSipperFileButton.Text = "Add Sipper Files";
      this.AddSipperFileButton.UseVisualStyleBackColor = true;
      this.AddSipperFileButton.Click += new System.EventHandler (this.AddSipperFileButton_Click);
      // 
      // SyncFieldsHelp
      // 
      this.SyncFieldsHelp.BorderStyle = System.Windows.Forms.BorderStyle.None;
      this.SyncFieldsHelp.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8F);
      this.SyncFieldsHelp.ForeColor = System.Drawing.SystemColors.WindowText;
      this.SyncFieldsHelp.Location = new System.Drawing.Point (33, 278);
      this.SyncFieldsHelp.Multiline = true;
      this.SyncFieldsHelp.Name = "SyncFieldsHelp";
      this.SyncFieldsHelp.ReadOnly = true;
      this.SyncFieldsHelp.Size = new System.Drawing.Size (669, 33);
      this.SyncFieldsHelp.TabIndex = 26;
      this.SyncFieldsHelp.TabStop = false;
      this.SyncFieldsHelp.Text = resources.GetString ("SyncFieldsHelp.Text");
      // 
      // SyncTimeStampGPS
      // 
      this.SyncTimeStampGPS.CustomFormat = "yyyy/MM/dd  HH:mm:ss";
      this.SyncTimeStampGPS.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SyncTimeStampGPS.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
      this.SyncTimeStampGPS.Location = new System.Drawing.Point (449, 337);
      this.SyncTimeStampGPS.Name = "SyncTimeStampGPS";
      this.SyncTimeStampGPS.Size = new System.Drawing.Size (156, 22);
      this.SyncTimeStampGPS.TabIndex = 34;
      // 
      // SyncTimeStampGPSLabel
      // 
      this.SyncTimeStampGPSLabel.AutoSize = true;
      this.SyncTimeStampGPSLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SyncTimeStampGPSLabel.Location = new System.Drawing.Point (446, 323);
      this.SyncTimeStampGPSLabel.Name = "SyncTimeStampGPSLabel";
      this.SyncTimeStampGPSLabel.Size = new System.Drawing.Size (82, 13);
      this.SyncTimeStampGPSLabel.TabIndex = 28;
      this.SyncTimeStampGPSLabel.Text = "Sync GPS Time";
      // 
      // SyncTimeStampCTDLabel
      // 
      this.SyncTimeStampCTDLabel.AutoSize = true;
      this.SyncTimeStampCTDLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SyncTimeStampCTDLabel.Location = new System.Drawing.Point (238, 323);
      this.SyncTimeStampCTDLabel.Name = "SyncTimeStampCTDLabel";
      this.SyncTimeStampCTDLabel.Size = new System.Drawing.Size (82, 13);
      this.SyncTimeStampCTDLabel.TabIndex = 30;
      this.SyncTimeStampCTDLabel.Text = "Sync CTD Time";
      // 
      // SyncTimeStampCTD
      // 
      this.SyncTimeStampCTD.CustomFormat = "yyyy/MM/dd  HH:mm:ss";
      this.SyncTimeStampCTD.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SyncTimeStampCTD.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
      this.SyncTimeStampCTD.Location = new System.Drawing.Point (241, 337);
      this.SyncTimeStampCTD.Name = "SyncTimeStampCTD";
      this.SyncTimeStampCTD.Size = new System.Drawing.Size (156, 22);
      this.SyncTimeStampCTD.TabIndex = 32;
      // 
      // SyncTimeStampActualLabel
      // 
      this.SyncTimeStampActualLabel.AutoSize = true;
      this.SyncTimeStampActualLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SyncTimeStampActualLabel.Location = new System.Drawing.Point (33, 323);
      this.SyncTimeStampActualLabel.Name = "SyncTimeStampActualLabel";
      this.SyncTimeStampActualLabel.Size = new System.Drawing.Size (90, 13);
      this.SyncTimeStampActualLabel.TabIndex = 67;
      this.SyncTimeStampActualLabel.Text = "Sync Actual Time";
      // 
      // SyncTimeStampActual
      // 
      this.SyncTimeStampActual.CustomFormat = "yyyy/MM/dd  HH:mm:ss";
      this.SyncTimeStampActual.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SyncTimeStampActual.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
      this.SyncTimeStampActual.Location = new System.Drawing.Point (33, 337);
      this.SyncTimeStampActual.Name = "SyncTimeStampActual";
      this.SyncTimeStampActual.Size = new System.Drawing.Size (156, 22);
      this.SyncTimeStampActual.TabIndex = 30;
      // 
      // DeploymentDialog
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (741, 665);
      this.Controls.Add (this.SyncTimeStampActualLabel);
      this.Controls.Add (this.SyncTimeStampActual);
      this.Controls.Add (this.SyncTimeStampCTDLabel);
      this.Controls.Add (this.SyncTimeStampCTD);
      this.Controls.Add (this.SyncTimeStampGPSLabel);
      this.Controls.Add (this.SyncTimeStampGPS);
      this.Controls.Add (this.SyncFieldsHelp);
      this.Controls.Add (this.AddSipperFileButton);
      this.Controls.Add (this.SipperFilesLabel);
      this.Controls.Add (this.SipperFiles);
      this.Controls.Add (this.StartEndLabel);
      this.Controls.Add (this.DateTimeEnd);
      this.Controls.Add (this.DateEndLabel);
      this.Controls.Add (this.DeploymentNum);
      this.Controls.Add (this.DeploymentNumLabel);
      this.Controls.Add (this.CancelReqButton);
      this.Controls.Add (this.UpdateButton);
      this.Controls.Add (this.DateTimeStart);
      this.Controls.Add (this.Longitude);
      this.Controls.Add (this.LongitudeLabel);
      this.Controls.Add (this.Latitude);
      this.Controls.Add (this.Description);
      this.Controls.Add (this.StationName);
      this.Controls.Add (this.CruiseName);
      this.Controls.Add (this.LatitudeLabel);
      this.Controls.Add (this.DescriptionLabel);
      this.Controls.Add (this.StationNameLabel);
      this.Controls.Add (this.CruiseNameLabel);
      this.Controls.Add (this.TitleLabel);
      this.Name = "DeploymentDialog";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "DeploymentDialog";
      this.Load += new System.EventHandler (this.DeploymentDialog_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.DeploymentDialog_FormClosing);
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label CruiseNameLabel;
    private System.Windows.Forms.Label StationNameLabel;
    private System.Windows.Forms.Label DescriptionLabel;
    private System.Windows.Forms.Label LatitudeLabel;
    private System.Windows.Forms.TextBox CruiseName;
    private System.Windows.Forms.TextBox StationName;
    private System.Windows.Forms.TextBox Description;
    private System.Windows.Forms.TextBox Latitude;
    private System.Windows.Forms.Label LongitudeLabel;
    private System.Windows.Forms.TextBox Longitude;
    private System.Windows.Forms.DateTimePicker DateTimeStart;
    private System.Windows.Forms.Button UpdateButton;
    private System.Windows.Forms.Button CancelReqButton;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.TextBox DeploymentNum;
    private System.Windows.Forms.Label DeploymentNumLabel;
    private System.Windows.Forms.Label DateEndLabel;
    private System.Windows.Forms.DateTimePicker DateTimeEnd;
    private System.Windows.Forms.Label StartEndLabel;
    private System.Windows.Forms.Label SipperFilesLabel;
    private System.Windows.Forms.TreeView SipperFiles;
    private System.Windows.Forms.Button AddSipperFileButton;
    private System.Windows.Forms.TextBox SyncFieldsHelp;
    private System.Windows.Forms.DateTimePicker SyncTimeStampGPS;
    private System.Windows.Forms.Label SyncTimeStampCTDLabel;
    private System.Windows.Forms.DateTimePicker SyncTimeStampCTD;
    private System.Windows.Forms.Label SyncTimeStampGPSLabel;
    private System.Windows.Forms.Label SyncTimeStampActualLabel;
    private System.Windows.Forms.DateTimePicker SyncTimeStampActual;
  }
}