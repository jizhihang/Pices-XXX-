namespace PicesCommander
{
  partial class ReconcileImagesToDataBase
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose (bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose ();
      }
      base.Dispose (disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent ()
    {
      this.components = new System.ComponentModel.Container ();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (ReconcileImagesToDataBase));
      this.FunctionDescription = new System.Windows.Forms.TextBox ();
      this.TitleLabel = new System.Windows.Forms.Label ();
      this.SourceDirectoryLabel = new System.Windows.Forms.Label ();
      this.SourceDirectory = new System.Windows.Forms.TextBox ();
      this.SourceDirectoryBrowse = new System.Windows.Forms.Button ();
      this.Cancel = new System.Windows.Forms.Button ();
      this.Start = new System.Windows.Forms.Button ();
      this.CurrentDirectoryLabel = new System.Windows.Forms.Label ();
      this.RunStatsCurDirectory_ = new System.Windows.Forms.TextBox ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.TotalImagesLabel = new System.Windows.Forms.Label ();
      this.RunStatusTotalImages_ = new System.Windows.Forms.TextBox ();
      this.RunStatsImagesFixed_ = new System.Windows.Forms.TextBox ();
      this.RunStatsImagesFixedLabel = new System.Windows.Forms.Label ();
      this.RunStatsImagesInDataBaseLabel = new System.Windows.Forms.Label ();
      this.RunStatsImagesInDataBase_ = new System.Windows.Forms.TextBox ();
      this.RunStatsImagesNotInDataBaseLabel = new System.Windows.Forms.Label ();
      this.RunStatsImagesNotInDataBase_ = new System.Windows.Forms.TextBox ();
      this.RunStatsTimer = new System.Windows.Forms.Timer (this.components);
      this.ReconciliationProgress = new System.Windows.Forms.ProgressBar ();
      this.RemoveImagesThatAreNotInDatabase = new System.Windows.Forms.CheckBox ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.SuspendLayout ();
      // 
      // FunctionDescription
      // 
      this.FunctionDescription.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FunctionDescription.Location = new System.Drawing.Point (12, 51);
      this.FunctionDescription.Multiline = true;
      this.FunctionDescription.Name = "FunctionDescription";
      this.FunctionDescription.ReadOnly = true;
      this.FunctionDescription.Size = new System.Drawing.Size (599, 180);
      this.FunctionDescription.TabIndex = 12;
      this.FunctionDescription.TabStop = false;
      this.FunctionDescription.Text = resources.GetString ("FunctionDescription.Text");
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (161, 7);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (311, 25);
      this.TitleLabel.TabIndex = 13;
      this.TitleLabel.Text = "Reconcile Images To Database";
      // 
      // SourceDirectoryLabel
      // 
      this.SourceDirectoryLabel.AutoSize = true;
      this.SourceDirectoryLabel.Location = new System.Drawing.Point (9, 252);
      this.SourceDirectoryLabel.Name = "SourceDirectoryLabel";
      this.SourceDirectoryLabel.Size = new System.Drawing.Size (83, 13);
      this.SourceDirectoryLabel.TabIndex = 14;
      this.SourceDirectoryLabel.Text = "SourceDirectory";
      this.SourceDirectoryLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // SourceDirectory
      // 
      this.SourceDirectory.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SourceDirectory.Location = new System.Drawing.Point (12, 268);
      this.SourceDirectory.Name = "SourceDirectory";
      this.SourceDirectory.Size = new System.Drawing.Size (518, 22);
      this.SourceDirectory.TabIndex = 15;
      this.SourceDirectory.Validating += new System.ComponentModel.CancelEventHandler (this.SourceDirectory_Validating);
      // 
      // SourceDirectoryBrowse
      // 
      this.SourceDirectoryBrowse.Location = new System.Drawing.Point (536, 267);
      this.SourceDirectoryBrowse.Name = "SourceDirectoryBrowse";
      this.SourceDirectoryBrowse.Size = new System.Drawing.Size (75, 23);
      this.SourceDirectoryBrowse.TabIndex = 16;
      this.SourceDirectoryBrowse.Text = "Browse";
      this.SourceDirectoryBrowse.UseVisualStyleBackColor = true;
      this.SourceDirectoryBrowse.Click += new System.EventHandler (this.SourceDirectoryBrowse_Click);
      // 
      // Cancel
      // 
      this.Cancel.Enabled = false;
      this.Cancel.Location = new System.Drawing.Point (536, 539);
      this.Cancel.Name = "Cancel";
      this.Cancel.Size = new System.Drawing.Size (75, 23);
      this.Cancel.TabIndex = 18;
      this.Cancel.Text = "Cancel";
      this.Cancel.UseVisualStyleBackColor = true;
      this.Cancel.Click += new System.EventHandler (this.Cancel_Click);
      // 
      // Start
      // 
      this.Start.Location = new System.Drawing.Point (12, 539);
      this.Start.Name = "Start";
      this.Start.Size = new System.Drawing.Size (75, 23);
      this.Start.TabIndex = 17;
      this.Start.Text = "Start";
      this.Start.UseVisualStyleBackColor = true;
      this.Start.Click += new System.EventHandler (this.Start_Click);
      // 
      // CurrentDirectoryLabel
      // 
      this.CurrentDirectoryLabel.AutoSize = true;
      this.CurrentDirectoryLabel.Location = new System.Drawing.Point (12, 339);
      this.CurrentDirectoryLabel.Name = "CurrentDirectoryLabel";
      this.CurrentDirectoryLabel.Size = new System.Drawing.Size (86, 13);
      this.CurrentDirectoryLabel.TabIndex = 19;
      this.CurrentDirectoryLabel.Text = "Current Directory";
      this.CurrentDirectoryLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // RunStatsCurDirectory_
      // 
      this.RunStatsCurDirectory_.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.RunStatsCurDirectory_.Location = new System.Drawing.Point (12, 355);
      this.RunStatsCurDirectory_.Name = "RunStatsCurDirectory_";
      this.RunStatsCurDirectory_.ReadOnly = true;
      this.RunStatsCurDirectory_.Size = new System.Drawing.Size (599, 22);
      this.RunStatsCurDirectory_.TabIndex = 20;
      this.RunStatsCurDirectory_.TabStop = false;
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // TotalImagesLabel
      // 
      this.TotalImagesLabel.AutoSize = true;
      this.TotalImagesLabel.Location = new System.Drawing.Point (12, 414);
      this.TotalImagesLabel.Name = "TotalImagesLabel";
      this.TotalImagesLabel.Size = new System.Drawing.Size (68, 13);
      this.TotalImagesLabel.TabIndex = 21;
      this.TotalImagesLabel.Text = "Total Images";
      this.TotalImagesLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // RunStatusTotalImages_
      // 
      this.RunStatusTotalImages_.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.RunStatusTotalImages_.Location = new System.Drawing.Point (12, 430);
      this.RunStatusTotalImages_.Name = "RunStatusTotalImages_";
      this.RunStatusTotalImages_.ReadOnly = true;
      this.RunStatusTotalImages_.Size = new System.Drawing.Size (107, 22);
      this.RunStatusTotalImages_.TabIndex = 22;
      this.RunStatusTotalImages_.TabStop = false;
      // 
      // RunStatsImagesFixed_
      // 
      this.RunStatsImagesFixed_.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.RunStatsImagesFixed_.Location = new System.Drawing.Point (504, 430);
      this.RunStatsImagesFixed_.Name = "RunStatsImagesFixed_";
      this.RunStatsImagesFixed_.ReadOnly = true;
      this.RunStatsImagesFixed_.Size = new System.Drawing.Size (107, 22);
      this.RunStatsImagesFixed_.TabIndex = 23;
      this.RunStatsImagesFixed_.TabStop = false;
      // 
      // RunStatsImagesFixedLabel
      // 
      this.RunStatsImagesFixedLabel.AutoSize = true;
      this.RunStatsImagesFixedLabel.Location = new System.Drawing.Point (501, 414);
      this.RunStatsImagesFixedLabel.Name = "RunStatsImagesFixedLabel";
      this.RunStatsImagesFixedLabel.Size = new System.Drawing.Size (69, 13);
      this.RunStatsImagesFixedLabel.TabIndex = 24;
      this.RunStatsImagesFixedLabel.Text = "Images Fixed";
      this.RunStatsImagesFixedLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // RunStatsImagesInDataBaseLabel
      // 
      this.RunStatsImagesInDataBaseLabel.AutoSize = true;
      this.RunStatsImagesInDataBaseLabel.Location = new System.Drawing.Point (169, 414);
      this.RunStatsImagesInDataBaseLabel.Name = "RunStatsImagesInDataBaseLabel";
      this.RunStatsImagesInDataBaseLabel.Size = new System.Drawing.Size (102, 13);
      this.RunStatsImagesInDataBaseLabel.TabIndex = 25;
      this.RunStatsImagesInDataBaseLabel.Text = "Images In Database";
      this.RunStatsImagesInDataBaseLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // RunStatsImagesInDataBase_
      // 
      this.RunStatsImagesInDataBase_.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.RunStatsImagesInDataBase_.Location = new System.Drawing.Point (172, 430);
      this.RunStatsImagesInDataBase_.Name = "RunStatsImagesInDataBase_";
      this.RunStatsImagesInDataBase_.ReadOnly = true;
      this.RunStatsImagesInDataBase_.Size = new System.Drawing.Size (107, 22);
      this.RunStatsImagesInDataBase_.TabIndex = 26;
      this.RunStatsImagesInDataBase_.TabStop = false;
      // 
      // RunStatsImagesNotInDataBaseLabel
      // 
      this.RunStatsImagesNotInDataBaseLabel.AutoSize = true;
      this.RunStatsImagesNotInDataBaseLabel.Location = new System.Drawing.Point (337, 414);
      this.RunStatsImagesNotInDataBaseLabel.Name = "RunStatsImagesNotInDataBaseLabel";
      this.RunStatsImagesNotInDataBaseLabel.Size = new System.Drawing.Size (91, 13);
      this.RunStatsImagesNotInDataBaseLabel.TabIndex = 27;
      this.RunStatsImagesNotInDataBaseLabel.Text = "NOT In Database";
      this.RunStatsImagesNotInDataBaseLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // RunStatsImagesNotInDataBase_
      // 
      this.RunStatsImagesNotInDataBase_.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.RunStatsImagesNotInDataBase_.Location = new System.Drawing.Point (340, 430);
      this.RunStatsImagesNotInDataBase_.Name = "RunStatsImagesNotInDataBase_";
      this.RunStatsImagesNotInDataBase_.ReadOnly = true;
      this.RunStatsImagesNotInDataBase_.Size = new System.Drawing.Size (107, 22);
      this.RunStatsImagesNotInDataBase_.TabIndex = 28;
      this.RunStatsImagesNotInDataBase_.TabStop = false;
      // 
      // RunStatsTimer
      // 
      this.RunStatsTimer.Tick += new System.EventHandler (this.RunStatsTimer_Tick);
      // 
      // ReconciliationProgress
      // 
      this.ReconciliationProgress.Location = new System.Drawing.Point (12, 471);
      this.ReconciliationProgress.Name = "ReconciliationProgress";
      this.ReconciliationProgress.Size = new System.Drawing.Size (599, 23);
      this.ReconciliationProgress.TabIndex = 29;
      // 
      // RemoveImagesThatAreNotInDatabase
      // 
      this.RemoveImagesThatAreNotInDatabase.AutoSize = true;
      this.RemoveImagesThatAreNotInDatabase.Location = new System.Drawing.Point (12, 296);
      this.RemoveImagesThatAreNotInDatabase.Name = "RemoveImagesThatAreNotInDatabase";
      this.RemoveImagesThatAreNotInDatabase.Size = new System.Drawing.Size (220, 17);
      this.RemoveImagesThatAreNotInDatabase.TabIndex = 30;
      this.RemoveImagesThatAreNotInDatabase.Text = "Remove Images that are not in Database";
      this.RemoveImagesThatAreNotInDatabase.UseVisualStyleBackColor = true;
      this.RemoveImagesThatAreNotInDatabase.Validating += new System.ComponentModel.CancelEventHandler (this.RemoveImagesThatAreNotInDatabase_Validating);
      // 
      // ReconcileImagesToDataBase
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (623, 570);
      this.Controls.Add (this.RemoveImagesThatAreNotInDatabase);
      this.Controls.Add (this.ReconciliationProgress);
      this.Controls.Add (this.RunStatsImagesNotInDataBase_);
      this.Controls.Add (this.RunStatsImagesNotInDataBaseLabel);
      this.Controls.Add (this.RunStatsImagesInDataBase_);
      this.Controls.Add (this.RunStatsImagesInDataBaseLabel);
      this.Controls.Add (this.RunStatsImagesFixedLabel);
      this.Controls.Add (this.RunStatsImagesFixed_);
      this.Controls.Add (this.RunStatusTotalImages_);
      this.Controls.Add (this.TotalImagesLabel);
      this.Controls.Add (this.RunStatsCurDirectory_);
      this.Controls.Add (this.CurrentDirectoryLabel);
      this.Controls.Add (this.Cancel);
      this.Controls.Add (this.Start);
      this.Controls.Add (this.SourceDirectoryBrowse);
      this.Controls.Add (this.SourceDirectory);
      this.Controls.Add (this.SourceDirectoryLabel);
      this.Controls.Add (this.TitleLabel);
      this.Controls.Add (this.FunctionDescription);
      this.MaximumSize = new System.Drawing.Size (639, 608);
      this.MinimumSize = new System.Drawing.Size (639, 608);
      this.Name = "ReconcileImagesToDataBase";
      this.Text = "ReconcileImagesToDataBase";
      this.Load += new System.EventHandler (this.ReconcileImagesToDataBase_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.ReconcileImagesToDataBase_FormClosing);
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.TextBox FunctionDescription;
    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label SourceDirectoryLabel;
    private System.Windows.Forms.TextBox SourceDirectory;
    private System.Windows.Forms.Button SourceDirectoryBrowse;
    private System.Windows.Forms.Button Cancel;
    private System.Windows.Forms.Button Start;
    private System.Windows.Forms.Label CurrentDirectoryLabel;
    private System.Windows.Forms.TextBox RunStatsCurDirectory_;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.Label TotalImagesLabel;
    private System.Windows.Forms.TextBox RunStatusTotalImages_;
    private System.Windows.Forms.TextBox RunStatsImagesFixed_;
    private System.Windows.Forms.Label RunStatsImagesFixedLabel;
    private System.Windows.Forms.Label RunStatsImagesInDataBaseLabel;
    private System.Windows.Forms.TextBox RunStatsImagesInDataBase_;
    private System.Windows.Forms.Label RunStatsImagesNotInDataBaseLabel;
    private System.Windows.Forms.TextBox RunStatsImagesNotInDataBase_;
    private System.Windows.Forms.Timer RunStatsTimer;
    private System.Windows.Forms.ProgressBar ReconciliationProgress;
    private System.Windows.Forms.CheckBox RemoveImagesThatAreNotInDatabase;
  }
}