namespace SipperFile
{
  partial class StationDialog
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
      this.DateStartLabel = new System.Windows.Forms.Label ();
      this.DateTimeStart = new System.Windows.Forms.DateTimePicker ();
      this.UpdateButton = new System.Windows.Forms.Button ();
      this.CancelReqButton = new System.Windows.Forms.Button ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.Deployments = new System.Windows.Forms.TreeView ();
      this.DeploymentsLabel = new System.Windows.Forms.Label ();
      this.AddDeploymentButton = new System.Windows.Forms.Button ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (249, 11);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (180, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Station Maintenance";
      // 
      // CruiseNameLabel
      // 
      this.CruiseNameLabel.AutoSize = true;
      this.CruiseNameLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseNameLabel.Location = new System.Drawing.Point (28, 56);
      this.CruiseNameLabel.Name = "CruiseNameLabel";
      this.CruiseNameLabel.Size = new System.Drawing.Size (36, 13);
      this.CruiseNameLabel.TabIndex = 1;
      this.CruiseNameLabel.Text = "Cruise";
      // 
      // StationNameLabel
      // 
      this.StationNameLabel.AutoSize = true;
      this.StationNameLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.StationNameLabel.Location = new System.Drawing.Point (186, 58);
      this.StationNameLabel.Name = "StationNameLabel";
      this.StationNameLabel.Size = new System.Drawing.Size (40, 13);
      this.StationNameLabel.TabIndex = 2;
      this.StationNameLabel.Text = "Station";
      // 
      // DescriptionLabel
      // 
      this.DescriptionLabel.AutoSize = true;
      this.DescriptionLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DescriptionLabel.Location = new System.Drawing.Point (28, 118);
      this.DescriptionLabel.Name = "DescriptionLabel";
      this.DescriptionLabel.Size = new System.Drawing.Size (60, 13);
      this.DescriptionLabel.TabIndex = 3;
      this.DescriptionLabel.Text = "Description";
      // 
      // LatitudeLabel
      // 
      this.LatitudeLabel.AutoSize = true;
      this.LatitudeLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.LatitudeLabel.Location = new System.Drawing.Point (28, 220);
      this.LatitudeLabel.Name = "LatitudeLabel";
      this.LatitudeLabel.Size = new System.Drawing.Size (45, 13);
      this.LatitudeLabel.TabIndex = 4;
      this.LatitudeLabel.Text = "Latitude";
      // 
      // CruiseName
      // 
      this.CruiseName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseName.Location = new System.Drawing.Point (31, 72);
      this.CruiseName.Name = "CruiseName";
      this.CruiseName.ReadOnly = true;
      this.CruiseName.Size = new System.Drawing.Size (137, 22);
      this.CruiseName.TabIndex = 6;
      this.CruiseName.TabStop = false;
      // 
      // StationName
      // 
      this.StationName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.StationName.Location = new System.Drawing.Point (189, 72);
      this.StationName.Name = "StationName";
      this.StationName.ReadOnly = true;
      this.StationName.Size = new System.Drawing.Size (137, 22);
      this.StationName.TabIndex = 7;
      this.StationName.TabStop = false;
      this.StationName.Validating += new System.ComponentModel.CancelEventHandler (this.StationName_Validating);
      // 
      // Description
      // 
      this.Description.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Description.Location = new System.Drawing.Point (31, 134);
      this.Description.Multiline = true;
      this.Description.Name = "Description";
      this.Description.Size = new System.Drawing.Size (608, 60);
      this.Description.TabIndex = 8;
      // 
      // Latitude
      // 
      this.Latitude.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Latitude.Location = new System.Drawing.Point (31, 236);
      this.Latitude.Name = "Latitude";
      this.Latitude.Size = new System.Drawing.Size (137, 22);
      this.Latitude.TabIndex = 9;
      this.Latitude.Validating += new System.ComponentModel.CancelEventHandler (this.Latitude_Validating);
      // 
      // LongitudeLabel
      // 
      this.LongitudeLabel.AutoSize = true;
      this.LongitudeLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.LongitudeLabel.Location = new System.Drawing.Point (177, 220);
      this.LongitudeLabel.Name = "LongitudeLabel";
      this.LongitudeLabel.Size = new System.Drawing.Size (54, 13);
      this.LongitudeLabel.TabIndex = 10;
      this.LongitudeLabel.Text = "Longitude";
      // 
      // Longitude
      // 
      this.Longitude.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Longitude.Location = new System.Drawing.Point (180, 236);
      this.Longitude.Name = "Longitude";
      this.Longitude.Size = new System.Drawing.Size (137, 22);
      this.Longitude.TabIndex = 11;
      this.Longitude.Validating += new System.ComponentModel.CancelEventHandler (this.Longitude_Validating);
      // 
      // DateStartLabel
      // 
      this.DateStartLabel.AutoSize = true;
      this.DateStartLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateStartLabel.Location = new System.Drawing.Point (366, 220);
      this.DateStartLabel.Name = "DateStartLabel";
      this.DateStartLabel.Size = new System.Drawing.Size (83, 13);
      this.DateStartLabel.TabIndex = 12;
      this.DateStartLabel.Text = "Start Date/Time";
      // 
      // DateTimeStart
      // 
      this.DateTimeStart.CustomFormat = "yyyy-MMM-dd (dddd)  HH:mm:ss";
      this.DateTimeStart.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateTimeStart.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
      this.DateTimeStart.Location = new System.Drawing.Point (369, 234);
      this.DateTimeStart.Name = "DateTimeStart";
      this.DateTimeStart.Size = new System.Drawing.Size (270, 22);
      this.DateTimeStart.TabIndex = 13;
      // 
      // UpdateButton
      // 
      this.UpdateButton.Location = new System.Drawing.Point (31, 523);
      this.UpdateButton.Name = "UpdateButton";
      this.UpdateButton.Size = new System.Drawing.Size (75, 23);
      this.UpdateButton.TabIndex = 14;
      this.UpdateButton.Text = "Update";
      this.UpdateButton.UseVisualStyleBackColor = true;
      this.UpdateButton.Click += new System.EventHandler (this.UpdateButton_Click);
      // 
      // CancelReqButton
      // 
      this.CancelReqButton.Location = new System.Drawing.Point (564, 523);
      this.CancelReqButton.Name = "CancelReqButton";
      this.CancelReqButton.Size = new System.Drawing.Size (75, 23);
      this.CancelReqButton.TabIndex = 15;
      this.CancelReqButton.Text = "Cancel";
      this.CancelReqButton.UseVisualStyleBackColor = true;
      this.CancelReqButton.Click += new System.EventHandler (this.CancelReqButton_Click);
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // Deployments
      // 
      this.Deployments.Location = new System.Drawing.Point (31, 300);
      this.Deployments.Name = "Deployments";
      this.Deployments.Size = new System.Drawing.Size (608, 199);
      this.Deployments.TabIndex = 16;
      // 
      // DeploymentsLabel
      // 
      this.DeploymentsLabel.AutoSize = true;
      this.DeploymentsLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DeploymentsLabel.Location = new System.Drawing.Point (28, 285);
      this.DeploymentsLabel.Name = "DeploymentsLabel";
      this.DeploymentsLabel.Size = new System.Drawing.Size (68, 13);
      this.DeploymentsLabel.TabIndex = 17;
      this.DeploymentsLabel.Text = "Deployments";
      // 
      // AddDeploymentButton
      // 
      this.AddDeploymentButton.Location = new System.Drawing.Point (506, 476);
      this.AddDeploymentButton.Name = "AddDeploymentButton";
      this.AddDeploymentButton.Size = new System.Drawing.Size (133, 23);
      this.AddDeploymentButton.TabIndex = 22;
      this.AddDeploymentButton.Text = "Add a new Deployment";
      this.AddDeploymentButton.UseVisualStyleBackColor = true;
      this.AddDeploymentButton.Click += new System.EventHandler (this.AddDeploymentButton_Click);
      // 
      // StationDialog
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (662, 559);
      this.Controls.Add (this.AddDeploymentButton);
      this.Controls.Add (this.DeploymentsLabel);
      this.Controls.Add (this.Deployments);
      this.Controls.Add (this.CancelReqButton);
      this.Controls.Add (this.UpdateButton);
      this.Controls.Add (this.DateTimeStart);
      this.Controls.Add (this.DateStartLabel);
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
      this.Name = "StationDialog";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "StationDialog";
      this.Load += new System.EventHandler (this.StationDialog_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.StationDialog_FormClosing);
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
    private System.Windows.Forms.Label DateStartLabel;
    private System.Windows.Forms.DateTimePicker DateTimeStart;
    private System.Windows.Forms.Button UpdateButton;
    private System.Windows.Forms.Button CancelReqButton;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.Label DeploymentsLabel;
    private System.Windows.Forms.TreeView Deployments;
    private System.Windows.Forms.Button AddDeploymentButton;
  }
}