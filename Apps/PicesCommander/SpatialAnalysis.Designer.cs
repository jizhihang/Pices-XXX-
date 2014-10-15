namespace PicesCommander
{
  partial class SpatialAnalysis
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
      this.NumSipperFileImagesLabel = new System.Windows.Forms.Label();
      this.NumSipperFileImages = new System.Windows.Forms.TextBox();
      this.Message = new System.Windows.Forms.TextBox();
      this.timer1 = new System.Windows.Forms.Timer(this.components);
      this.CurrentSipperFile = new System.Windows.Forms.TextBox();
      this.SipperFileNameLabel = new System.Windows.Forms.Label();
      this.cancelButton = new System.Windows.Forms.Button();
      this.StartButton = new System.Windows.Forms.Button();
      this.TitleLabel = new System.Windows.Forms.Label();
      this.Deployment = new System.Windows.Forms.TextBox();
      this.DeploymentLabel = new System.Windows.Forms.Label();
      this.CruiseLabel = new System.Windows.Forms.Label();
      this.Station = new System.Windows.Forms.TextBox();
      this.SelectionCriteriaPanel = new System.Windows.Forms.Panel();
      this.Cruise = new System.Windows.Forms.TextBox();
      this.StationLabel = new System.Windows.Forms.Label();
      this.SipperFileLabel = new System.Windows.Forms.Label();
      this.textBox1 = new System.Windows.Forms.TextBox();
      this.ClassLabel = new System.Windows.Forms.Label();
      this.MLClasses = new System.Windows.Forms.ComboBox();
      this.SelectionCriteriaPanel.SuspendLayout();
      this.SuspendLayout();
      // 
      // NumSipperFileImagesLabel
      // 
      this.NumSipperFileImagesLabel.AutoSize = true;
      this.NumSipperFileImagesLabel.Location = new System.Drawing.Point(276, 208);
      this.NumSipperFileImagesLabel.Name = "NumSipperFileImagesLabel";
      this.NumSipperFileImagesLabel.Size = new System.Drawing.Size(41, 13);
      this.NumSipperFileImagesLabel.TabIndex = 32;
      this.NumSipperFileImagesLabel.Text = "Images";
      // 
      // NumSipperFileImages
      // 
      this.NumSipperFileImages.Location = new System.Drawing.Point(321, 204);
      this.NumSipperFileImages.Name = "NumSipperFileImages";
      this.NumSipperFileImages.ReadOnly = true;
      this.NumSipperFileImages.Size = new System.Drawing.Size(80, 20);
      this.NumSipperFileImages.TabIndex = 31;
      // 
      // Message
      // 
      this.Message.Location = new System.Drawing.Point(102, 233);
      this.Message.Name = "Message";
      this.Message.ReadOnly = true;
      this.Message.Size = new System.Drawing.Size(459, 20);
      this.Message.TabIndex = 30;
      // 
      // CurrentSipperFile
      // 
      this.CurrentSipperFile.Location = new System.Drawing.Point(102, 204);
      this.CurrentSipperFile.Name = "CurrentSipperFile";
      this.CurrentSipperFile.ReadOnly = true;
      this.CurrentSipperFile.Size = new System.Drawing.Size(150, 20);
      this.CurrentSipperFile.TabIndex = 27;
      // 
      // SipperFileNameLabel
      // 
      this.SipperFileNameLabel.AutoSize = true;
      this.SipperFileNameLabel.Location = new System.Drawing.Point(9, 207);
      this.SipperFileNameLabel.Name = "SipperFileNameLabel";
      this.SipperFileNameLabel.Size = new System.Drawing.Size(87, 13);
      this.SipperFileNameLabel.TabIndex = 26;
      this.SipperFileNameLabel.Text = "Sipper File Name";
      // 
      // cancelButton
      // 
      this.cancelButton.Location = new System.Drawing.Point(486, 735);
      this.cancelButton.Name = "cancelButton";
      this.cancelButton.Size = new System.Drawing.Size(75, 23);
      this.cancelButton.TabIndex = 25;
      this.cancelButton.Text = "Cancel";
      this.cancelButton.UseVisualStyleBackColor = true;
      // 
      // StartButton
      // 
      this.StartButton.Location = new System.Drawing.Point(12, 735);
      this.StartButton.Name = "StartButton";
      this.StartButton.Size = new System.Drawing.Size(75, 23);
      this.StartButton.TabIndex = 24;
      this.StartButton.Text = "Start";
      this.StartButton.UseVisualStyleBackColor = true;
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point(221, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size(139, 24);
      this.TitleLabel.TabIndex = 21;
      this.TitleLabel.Text = "Spatial Analysis";
      // 
      // Deployment
      // 
      this.Deployment.Location = new System.Drawing.Point(85, 70);
      this.Deployment.Name = "Deployment";
      this.Deployment.ReadOnly = true;
      this.Deployment.Size = new System.Drawing.Size(102, 20);
      this.Deployment.TabIndex = 8;
      // 
      // DeploymentLabel
      // 
      this.DeploymentLabel.AutoSize = true;
      this.DeploymentLabel.Location = new System.Drawing.Point(14, 74);
      this.DeploymentLabel.Name = "DeploymentLabel";
      this.DeploymentLabel.Size = new System.Drawing.Size(63, 13);
      this.DeploymentLabel.TabIndex = 7;
      this.DeploymentLabel.Text = "Deployment";
      // 
      // CruiseLabel
      // 
      this.CruiseLabel.AutoSize = true;
      this.CruiseLabel.Location = new System.Drawing.Point(14, 14);
      this.CruiseLabel.Name = "CruiseLabel";
      this.CruiseLabel.Size = new System.Drawing.Size(36, 13);
      this.CruiseLabel.TabIndex = 3;
      this.CruiseLabel.Text = "Cruise";
      // 
      // Station
      // 
      this.Station.Location = new System.Drawing.Point(85, 40);
      this.Station.Name = "Station";
      this.Station.ReadOnly = true;
      this.Station.Size = new System.Drawing.Size(102, 20);
      this.Station.TabIndex = 6;
      // 
      // SelectionCriteriaPanel
      // 
      this.SelectionCriteriaPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
      this.SelectionCriteriaPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.SelectionCriteriaPanel.Controls.Add(this.MLClasses);
      this.SelectionCriteriaPanel.Controls.Add(this.ClassLabel);
      this.SelectionCriteriaPanel.Controls.Add(this.textBox1);
      this.SelectionCriteriaPanel.Controls.Add(this.SipperFileLabel);
      this.SelectionCriteriaPanel.Controls.Add(this.Deployment);
      this.SelectionCriteriaPanel.Controls.Add(this.DeploymentLabel);
      this.SelectionCriteriaPanel.Controls.Add(this.CruiseLabel);
      this.SelectionCriteriaPanel.Controls.Add(this.Station);
      this.SelectionCriteriaPanel.Controls.Add(this.Cruise);
      this.SelectionCriteriaPanel.Controls.Add(this.StationLabel);
      this.SelectionCriteriaPanel.Location = new System.Drawing.Point(12, 48);
      this.SelectionCriteriaPanel.Name = "SelectionCriteriaPanel";
      this.SelectionCriteriaPanel.Size = new System.Drawing.Size(547, 136);
      this.SelectionCriteriaPanel.TabIndex = 22;
      // 
      // Cruise
      // 
      this.Cruise.Location = new System.Drawing.Point(85, 10);
      this.Cruise.Name = "Cruise";
      this.Cruise.ReadOnly = true;
      this.Cruise.Size = new System.Drawing.Size(102, 20);
      this.Cruise.TabIndex = 4;
      // 
      // StationLabel
      // 
      this.StationLabel.AutoSize = true;
      this.StationLabel.Location = new System.Drawing.Point(14, 44);
      this.StationLabel.Name = "StationLabel";
      this.StationLabel.Size = new System.Drawing.Size(40, 13);
      this.StationLabel.TabIndex = 5;
      this.StationLabel.Text = "Station";
      // 
      // SipperFileLabel
      // 
      this.SipperFileLabel.AutoSize = true;
      this.SipperFileLabel.Location = new System.Drawing.Point(14, 104);
      this.SipperFileLabel.Name = "SipperFileLabel";
      this.SipperFileLabel.Size = new System.Drawing.Size(56, 13);
      this.SipperFileLabel.TabIndex = 9;
      this.SipperFileLabel.Text = "Sipper File";
      // 
      // textBox1
      // 
      this.textBox1.Location = new System.Drawing.Point(85, 100);
      this.textBox1.Name = "textBox1";
      this.textBox1.ReadOnly = true;
      this.textBox1.Size = new System.Drawing.Size(102, 20);
      this.textBox1.TabIndex = 10;
      // 
      // ClassLabel
      // 
      this.ClassLabel.AutoSize = true;
      this.ClassLabel.Location = new System.Drawing.Point(248, 14);
      this.ClassLabel.Name = "ClassLabel";
      this.ClassLabel.Size = new System.Drawing.Size(32, 13);
      this.ClassLabel.TabIndex = 11;
      this.ClassLabel.Text = "Class";
      // 
      // MLClasses
      // 
      this.MLClasses.FormattingEnabled = true;
      this.MLClasses.Location = new System.Drawing.Point(286, 10);
      this.MLClasses.Name = "MLClasses";
      this.MLClasses.Size = new System.Drawing.Size(236, 21);
      this.MLClasses.TabIndex = 12;
      // 
      // SpatialAnalysis
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(583, 765);
      this.Controls.Add(this.NumSipperFileImagesLabel);
      this.Controls.Add(this.NumSipperFileImages);
      this.Controls.Add(this.Message);
      this.Controls.Add(this.CurrentSipperFile);
      this.Controls.Add(this.SipperFileNameLabel);
      this.Controls.Add(this.cancelButton);
      this.Controls.Add(this.StartButton);
      this.Controls.Add(this.TitleLabel);
      this.Controls.Add(this.SelectionCriteriaPanel);
      this.Name = "SpatialAnalysis";
      this.Text = "SpatialAnalysis";
      this.SelectionCriteriaPanel.ResumeLayout(false);
      this.SelectionCriteriaPanel.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label NumSipperFileImagesLabel;
    private System.Windows.Forms.TextBox NumSipperFileImages;
    private System.Windows.Forms.TextBox Message;
    private System.Windows.Forms.Timer timer1;
    private System.Windows.Forms.TextBox CurrentSipperFile;
    private System.Windows.Forms.Label SipperFileNameLabel;
    private System.Windows.Forms.Button cancelButton;
    private System.Windows.Forms.Button StartButton;
    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.TextBox Deployment;
    private System.Windows.Forms.Label DeploymentLabel;
    private System.Windows.Forms.Label CruiseLabel;
    private System.Windows.Forms.TextBox Station;
    private System.Windows.Forms.Panel SelectionCriteriaPanel;
    private System.Windows.Forms.TextBox textBox1;
    private System.Windows.Forms.Label SipperFileLabel;
    private System.Windows.Forms.TextBox Cruise;
    private System.Windows.Forms.Label StationLabel;
    private System.Windows.Forms.Label ClassLabel;
    private System.Windows.Forms.ComboBox MLClasses;
  }
}