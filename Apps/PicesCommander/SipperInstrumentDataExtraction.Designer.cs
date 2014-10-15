namespace PicesCommander
{
  partial class SipperInstrumentDataExtraction
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
      this.StartButton = new System.Windows.Forms.Button ();
      this.CancelProcessButton = new System.Windows.Forms.Button ();
      this.runLogTimer = new System.Windows.Forms.Timer (this.components);
      this.CruiseLabel = new System.Windows.Forms.Label ();
      this.SipperFileLabel = new System.Windows.Forms.Label ();
      this.DeploymentLabel = new System.Windows.Forms.Label ();
      this.StationsLabel = new System.Windows.Forms.Label ();
      this.CruiseName = new System.Windows.Forms.TextBox ();
      this.StationName = new System.Windows.Forms.TextBox ();
      this.DeploymentNum = new System.Windows.Forms.TextBox ();
      this.SipperFile = new System.Windows.Forms.TextBox ();
      this.SelectionCriteria = new System.Windows.Forms.Panel ();
      this.SelectionCriteriaLabel = new System.Windows.Forms.Label ();
      this.RunLogLabel = new System.Windows.Forms.Label ();
      this.RunLogText = new System.Windows.Forms.TextBox ();
      this.SelectionCriteria.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (147, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (302, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Extract SIPPER Instrument Data";
      // 
      // StartButton
      // 
      this.StartButton.Location = new System.Drawing.Point (25, 532);
      this.StartButton.Name = "StartButton";
      this.StartButton.Size = new System.Drawing.Size (75, 23);
      this.StartButton.TabIndex = 4;
      this.StartButton.Text = "Start";
      this.StartButton.UseVisualStyleBackColor = true;
      this.StartButton.Click += new System.EventHandler (this.StartButton_Click);
      // 
      // CancelProcessButton
      // 
      this.CancelProcessButton.Location = new System.Drawing.Point (496, 532);
      this.CancelProcessButton.Name = "CancelProcessButton";
      this.CancelProcessButton.Size = new System.Drawing.Size (75, 23);
      this.CancelProcessButton.TabIndex = 5;
      this.CancelProcessButton.Text = "Cancel";
      this.CancelProcessButton.UseVisualStyleBackColor = true;
      this.CancelProcessButton.Click += new System.EventHandler (this.CancelProcess_Click);
      // 
      // runLogTimer
      // 
      this.runLogTimer.Tick += new System.EventHandler (this.runLogTimer_Tick);
      // 
      // CruiseLabel
      // 
      this.CruiseLabel.AutoSize = true;
      this.CruiseLabel.Location = new System.Drawing.Point (7, 13);
      this.CruiseLabel.Name = "CruiseLabel";
      this.CruiseLabel.Size = new System.Drawing.Size (36, 13);
      this.CruiseLabel.TabIndex = 19;
      this.CruiseLabel.Text = "Cruise";
      this.CruiseLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // SipperFileLabel
      // 
      this.SipperFileLabel.AutoSize = true;
      this.SipperFileLabel.Location = new System.Drawing.Point (7, 61);
      this.SipperFileLabel.Name = "SipperFileLabel";
      this.SipperFileLabel.Size = new System.Drawing.Size (53, 13);
      this.SipperFileLabel.TabIndex = 20;
      this.SipperFileLabel.Text = "SipperFile";
      this.SipperFileLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // DeploymentLabel
      // 
      this.DeploymentLabel.AutoSize = true;
      this.DeploymentLabel.Location = new System.Drawing.Point (328, 13);
      this.DeploymentLabel.Name = "DeploymentLabel";
      this.DeploymentLabel.Size = new System.Drawing.Size (63, 13);
      this.DeploymentLabel.TabIndex = 21;
      this.DeploymentLabel.Text = "Deployment";
      this.DeploymentLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // StationsLabel
      // 
      this.StationsLabel.AutoSize = true;
      this.StationsLabel.Location = new System.Drawing.Point (166, 13);
      this.StationsLabel.Name = "StationsLabel";
      this.StationsLabel.Size = new System.Drawing.Size (40, 13);
      this.StationsLabel.TabIndex = 22;
      this.StationsLabel.Text = "Station";
      this.StationsLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // CruiseName
      // 
      this.CruiseName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseName.Location = new System.Drawing.Point (10, 28);
      this.CruiseName.Name = "CruiseName";
      this.CruiseName.ReadOnly = true;
      this.CruiseName.Size = new System.Drawing.Size (140, 22);
      this.CruiseName.TabIndex = 11;
      this.CruiseName.TabStop = false;
      // 
      // StationName
      // 
      this.StationName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.StationName.Location = new System.Drawing.Point (169, 28);
      this.StationName.Name = "StationName";
      this.StationName.ReadOnly = true;
      this.StationName.Size = new System.Drawing.Size (140, 22);
      this.StationName.TabIndex = 23;
      this.StationName.TabStop = false;
      // 
      // DeploymentNum
      // 
      this.DeploymentNum.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DeploymentNum.Location = new System.Drawing.Point (328, 29);
      this.DeploymentNum.Name = "DeploymentNum";
      this.DeploymentNum.ReadOnly = true;
      this.DeploymentNum.Size = new System.Drawing.Size (84, 22);
      this.DeploymentNum.TabIndex = 24;
      this.DeploymentNum.TabStop = false;
      // 
      // SipperFile
      // 
      this.SipperFile.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SipperFile.Location = new System.Drawing.Point (10, 76);
      this.SipperFile.Name = "SipperFile";
      this.SipperFile.ReadOnly = true;
      this.SipperFile.Size = new System.Drawing.Size (402, 22);
      this.SipperFile.TabIndex = 25;
      this.SipperFile.TabStop = false;
      // 
      // SelectionCriteria
      // 
      this.SelectionCriteria.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.SelectionCriteria.Controls.Add (this.CruiseName);
      this.SelectionCriteria.Controls.Add (this.StationName);
      this.SelectionCriteria.Controls.Add (this.CruiseLabel);
      this.SelectionCriteria.Controls.Add (this.DeploymentNum);
      this.SelectionCriteria.Controls.Add (this.StationsLabel);
      this.SelectionCriteria.Controls.Add (this.DeploymentLabel);
      this.SelectionCriteria.Controls.Add (this.SipperFile);
      this.SelectionCriteria.Controls.Add (this.SipperFileLabel);
      this.SelectionCriteria.Location = new System.Drawing.Point (86, 87);
      this.SelectionCriteria.Name = "SelectionCriteria";
      this.SelectionCriteria.Size = new System.Drawing.Size (424, 117);
      this.SelectionCriteria.TabIndex = 26;
      // 
      // SelectionCriteriaLabel
      // 
      this.SelectionCriteriaLabel.AutoSize = true;
      this.SelectionCriteriaLabel.Location = new System.Drawing.Point (83, 71);
      this.SelectionCriteriaLabel.Name = "SelectionCriteriaLabel";
      this.SelectionCriteriaLabel.Size = new System.Drawing.Size (86, 13);
      this.SelectionCriteriaLabel.TabIndex = 27;
      this.SelectionCriteriaLabel.Text = "Selection Criteria";
      // 
      // RunLogLabel
      // 
      this.RunLogLabel.AutoSize = true;
      this.RunLogLabel.Location = new System.Drawing.Point (22, 226);
      this.RunLogLabel.Name = "RunLogLabel";
      this.RunLogLabel.Size = new System.Drawing.Size (48, 13);
      this.RunLogLabel.TabIndex = 29;
      this.RunLogLabel.Text = "Run Log";
      // 
      // RunLogText
      // 
      this.RunLogText.Location = new System.Drawing.Point (25, 242);
      this.RunLogText.Multiline = true;
      this.RunLogText.Name = "RunLogText";
      this.RunLogText.Size = new System.Drawing.Size (546, 264);
      this.RunLogText.TabIndex = 30;
      // 
      // SipperInstrumentDataExtraction
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (604, 562);
      this.Controls.Add (this.RunLogText);
      this.Controls.Add (this.RunLogLabel);
      this.Controls.Add (this.SelectionCriteriaLabel);
      this.Controls.Add (this.CancelProcessButton);
      this.Controls.Add (this.StartButton);
      this.Controls.Add (this.TitleLabel);
      this.Controls.Add (this.SelectionCriteria);
      this.MaximumSize = new System.Drawing.Size (620, 600);
      this.MinimumSize = new System.Drawing.Size (620, 600);
      this.Name = "SipperInstrumentDataExtraction";
      this.Text = "SipperInstrumentDataExtraction";
      this.Load += new System.EventHandler (this.SipperInstrumentDataExtraction_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.SipperInstrumentDataExtraction_FormClosing);
      this.SelectionCriteria.ResumeLayout (false);
      this.SelectionCriteria.PerformLayout ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Button CancelProcessButton;
    private System.Windows.Forms.Button StartButton;
    private System.Windows.Forms.Timer runLogTimer;
    private System.Windows.Forms.Label StationsLabel;
    private System.Windows.Forms.Label DeploymentLabel;
    private System.Windows.Forms.Label SipperFileLabel;
    private System.Windows.Forms.Label CruiseLabel;
    private System.Windows.Forms.TextBox SipperFile;
    private System.Windows.Forms.TextBox DeploymentNum;
    private System.Windows.Forms.TextBox StationName;
    private System.Windows.Forms.TextBox CruiseName;
    private System.Windows.Forms.Panel SelectionCriteria;
    private System.Windows.Forms.Label SelectionCriteriaLabel;
    private System.Windows.Forms.Label RunLogLabel;
    private System.Windows.Forms.TextBox RunLogText;
  }
}