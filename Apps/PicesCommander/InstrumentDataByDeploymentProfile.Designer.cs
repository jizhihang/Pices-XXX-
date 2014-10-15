namespace PicesCommander
{
  partial class InstrumentDataByDeploymentProfile
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
      this.components = new System.ComponentModel.Container();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(InstrumentDataByDeploymentProfile));
      this.SelectionCriteriaPanel = new System.Windows.Forms.Panel();
      this.Deployment = new System.Windows.Forms.TextBox();
      this.DeploymentLabel = new System.Windows.Forms.Label();
      this.CruiseLabel = new System.Windows.Forms.Label();
      this.Station = new System.Windows.Forms.TextBox();
      this.Cruise = new System.Windows.Forms.TextBox();
      this.StationLabel = new System.Windows.Forms.Label();
      this.RunLog = new System.Windows.Forms.TextBox();
      this.Cancel = new System.Windows.Forms.Button();
      this.Start = new System.Windows.Forms.Button();
      this.textBox1 = new System.Windows.Forms.TextBox();
      this.BackGroundTimer = new System.Windows.Forms.Timer(this.components);
      this.SelectionCriteriaPanel.SuspendLayout();
      this.SuspendLayout();
      // 
      // SelectionCriteriaPanel
      // 
      this.SelectionCriteriaPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
      this.SelectionCriteriaPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.SelectionCriteriaPanel.Controls.Add(this.Deployment);
      this.SelectionCriteriaPanel.Controls.Add(this.DeploymentLabel);
      this.SelectionCriteriaPanel.Controls.Add(this.CruiseLabel);
      this.SelectionCriteriaPanel.Controls.Add(this.Station);
      this.SelectionCriteriaPanel.Controls.Add(this.Cruise);
      this.SelectionCriteriaPanel.Controls.Add(this.StationLabel);
      this.SelectionCriteriaPanel.Location = new System.Drawing.Point(101, 12);
      this.SelectionCriteriaPanel.Name = "SelectionCriteriaPanel";
      this.SelectionCriteriaPanel.Size = new System.Drawing.Size(329, 64);
      this.SelectionCriteriaPanel.TabIndex = 10;
      // 
      // Deployment
      // 
      this.Deployment.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Deployment.Location = new System.Drawing.Point(237, 22);
      this.Deployment.Name = "Deployment";
      this.Deployment.ReadOnly = true;
      this.Deployment.Size = new System.Drawing.Size(63, 22);
      this.Deployment.TabIndex = 8;
      // 
      // DeploymentLabel
      // 
      this.DeploymentLabel.AutoSize = true;
      this.DeploymentLabel.Location = new System.Drawing.Point(235, 8);
      this.DeploymentLabel.Name = "DeploymentLabel";
      this.DeploymentLabel.Size = new System.Drawing.Size(63, 13);
      this.DeploymentLabel.TabIndex = 7;
      this.DeploymentLabel.Text = "Deployment";
      // 
      // CruiseLabel
      // 
      this.CruiseLabel.AutoSize = true;
      this.CruiseLabel.Location = new System.Drawing.Point(2, 8);
      this.CruiseLabel.Name = "CruiseLabel";
      this.CruiseLabel.Size = new System.Drawing.Size(36, 13);
      this.CruiseLabel.TabIndex = 3;
      this.CruiseLabel.Text = "Cruise";
      // 
      // Station
      // 
      this.Station.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Station.Location = new System.Drawing.Point(120, 22);
      this.Station.Name = "Station";
      this.Station.ReadOnly = true;
      this.Station.Size = new System.Drawing.Size(102, 22);
      this.Station.TabIndex = 6;
      // 
      // Cruise
      // 
      this.Cruise.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Cruise.Location = new System.Drawing.Point(3, 22);
      this.Cruise.Name = "Cruise";
      this.Cruise.ReadOnly = true;
      this.Cruise.Size = new System.Drawing.Size(102, 22);
      this.Cruise.TabIndex = 4;
      // 
      // StationLabel
      // 
      this.StationLabel.AutoSize = true;
      this.StationLabel.Location = new System.Drawing.Point(117, 8);
      this.StationLabel.Name = "StationLabel";
      this.StationLabel.Size = new System.Drawing.Size(40, 13);
      this.StationLabel.TabIndex = 5;
      this.StationLabel.Text = "Station";
      // 
      // RunLog
      // 
      this.RunLog.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.RunLog.Location = new System.Drawing.Point(12, 278);
      this.RunLog.Multiline = true;
      this.RunLog.Name = "RunLog";
      this.RunLog.ReadOnly = true;
      this.RunLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
      this.RunLog.Size = new System.Drawing.Size(501, 191);
      this.RunLog.TabIndex = 11;
      this.RunLog.TabStop = false;
      // 
      // Cancel
      // 
      this.Cancel.Enabled = false;
      this.Cancel.Location = new System.Drawing.Point(438, 475);
      this.Cancel.Name = "Cancel";
      this.Cancel.Size = new System.Drawing.Size(75, 23);
      this.Cancel.TabIndex = 13;
      this.Cancel.Text = "Cancel";
      this.Cancel.UseVisualStyleBackColor = true;
      this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
      // 
      // Start
      // 
      this.Start.Location = new System.Drawing.Point(12, 475);
      this.Start.Name = "Start";
      this.Start.Size = new System.Drawing.Size(75, 23);
      this.Start.TabIndex = 12;
      this.Start.Text = "Start";
      this.Start.UseVisualStyleBackColor = true;
      this.Start.Click += new System.EventHandler(this.Start_Click);
      // 
      // textBox1
      // 
      this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.textBox1.Location = new System.Drawing.Point(12, 82);
      this.textBox1.Multiline = true;
      this.textBox1.Name = "textBox1";
      this.textBox1.ReadOnly = true;
      this.textBox1.Size = new System.Drawing.Size(501, 190);
      this.textBox1.TabIndex = 14;
      this.textBox1.Text = resources.GetString("textBox1.Text");
      this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
      // 
      // BackGroundTimer
      // 
      this.BackGroundTimer.Tick += new System.EventHandler(this.BackGroundTimer_Tick);
      // 
      // InstrumentDataByDeploymentProfile
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(530, 510);
      this.Controls.Add(this.textBox1);
      this.Controls.Add(this.Cancel);
      this.Controls.Add(this.Start);
      this.Controls.Add(this.RunLog);
      this.Controls.Add(this.SelectionCriteriaPanel);
      this.Name = "InstrumentDataByDeploymentProfile";
      this.Text = "InstrumentDataByDeploymentProfile";
      this.Load += new System.EventHandler(this.InstrumentDataByDeploymentProfile_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.InstrumentDataByDeploymentProfile_FormClosing);
      this.SelectionCriteriaPanel.ResumeLayout(false);
      this.SelectionCriteriaPanel.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Panel SelectionCriteriaPanel;
    private System.Windows.Forms.TextBox Deployment;
    private System.Windows.Forms.Label DeploymentLabel;
    private System.Windows.Forms.Label CruiseLabel;
    private System.Windows.Forms.TextBox Station;
    private System.Windows.Forms.TextBox Cruise;
    private System.Windows.Forms.Label StationLabel;
    private System.Windows.Forms.TextBox RunLog;
    private System.Windows.Forms.Button Cancel;
    private System.Windows.Forms.Button Start;
    private System.Windows.Forms.TextBox textBox1;
    private System.Windows.Forms.Timer BackGroundTimer;
  }
}