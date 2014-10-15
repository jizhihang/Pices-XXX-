namespace SipperInterface
{
  partial class CameraLineDisplay
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
      this.Title = new System.Windows.Forms.Label ();
      this.scalingBox = new System.Windows.Forms.GroupBox ();
      this.fullSize = new System.Windows.Forms.RadioButton ();
      this.fourToOne = new System.Windows.Forms.RadioButton ();
      this.eightToOne = new System.Windows.Forms.RadioButton ();
      this.exitButton = new System.Windows.Forms.Button ();
      this.refreshButton = new System.Windows.Forms.Button ();
      this.pictureScrollBar = new System.Windows.Forms.HScrollBar ();
      this.cameraLineWindow = new System.Windows.Forms.Panel ();
      this.pollingTimer = new System.Windows.Forms.Timer (this.components);
      this.scalingBox.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // Title
      // 
      this.Title.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Title.Location = new System.Drawing.Point (382, 7);
      this.Title.Name = "Title";
      this.Title.Size = new System.Drawing.Size (263, 28);
      this.Title.TabIndex = 1;
      this.Title.Text = "SIPPER Camera Line";
      this.Title.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // scalingBox
      // 
      this.scalingBox.Controls.Add (this.fullSize);
      this.scalingBox.Controls.Add (this.fourToOne);
      this.scalingBox.Controls.Add (this.eightToOne);
      this.scalingBox.Location = new System.Drawing.Point (2, 637);
      this.scalingBox.Name = "scalingBox";
      this.scalingBox.Size = new System.Drawing.Size (261, 36);
      this.scalingBox.TabIndex = 2;
      this.scalingBox.TabStop = false;
      this.scalingBox.Text = "Scaling";
      // 
      // fullSize
      // 
      this.fullSize.AutoSize = true;
      this.fullSize.Location = new System.Drawing.Point (183, 15);
      this.fullSize.Name = "fullSize";
      this.fullSize.Size = new System.Drawing.Size (64, 17);
      this.fullSize.TabIndex = 2;
      this.fullSize.Text = "Full Size";
      this.fullSize.UseVisualStyleBackColor = true;
      this.fullSize.CheckedChanged += new System.EventHandler (this.fullSize_CheckedChanged);
      // 
      // fourToOne
      // 
      this.fourToOne.AutoSize = true;
      this.fourToOne.Location = new System.Drawing.Point (96, 15);
      this.fourToOne.Name = "fourToOne";
      this.fourToOne.Size = new System.Drawing.Size (81, 17);
      this.fourToOne.TabIndex = 1;
      this.fourToOne.Text = "Four to One";
      this.fourToOne.UseVisualStyleBackColor = true;
      this.fourToOne.CheckedChanged += new System.EventHandler (this.fourToOne_CheckedChanged);
      // 
      // eightToOne
      // 
      this.eightToOne.AutoSize = true;
      this.eightToOne.Checked = true;
      this.eightToOne.Location = new System.Drawing.Point (6, 15);
      this.eightToOne.Name = "eightToOne";
      this.eightToOne.Size = new System.Drawing.Size (84, 17);
      this.eightToOne.TabIndex = 0;
      this.eightToOne.TabStop = true;
      this.eightToOne.Tag = "";
      this.eightToOne.Text = "Eight to One";
      this.eightToOne.UseVisualStyleBackColor = true;
      this.eightToOne.CheckedChanged += new System.EventHandler (this.eightToOne_CheckedChanged);
      // 
      // exitButton
      // 
      this.exitButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.exitButton.Location = new System.Drawing.Point (949, 637);
      this.exitButton.Name = "exitButton";
      this.exitButton.Size = new System.Drawing.Size (82, 36);
      this.exitButton.TabIndex = 3;
      this.exitButton.Text = "Exit";
      this.exitButton.UseVisualStyleBackColor = true;
      this.exitButton.Click += new System.EventHandler (this.exitButton_Click);
      // 
      // refreshButton
      // 
      this.refreshButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.refreshButton.Location = new System.Drawing.Point (452, 637);
      this.refreshButton.Name = "refreshButton";
      this.refreshButton.Size = new System.Drawing.Size (132, 36);
      this.refreshButton.TabIndex = 4;
      this.refreshButton.Text = "Refresh";
      this.refreshButton.UseVisualStyleBackColor = true;
      this.refreshButton.Click += new System.EventHandler (this.refreshButton_Click);
      // 
      // pictureScrollBar
      // 
      this.pictureScrollBar.LargeChange = 512;
      this.pictureScrollBar.Location = new System.Drawing.Point (2, 592);
      this.pictureScrollBar.Maximum = 4096;
      this.pictureScrollBar.Name = "pictureScrollBar";
      this.pictureScrollBar.Size = new System.Drawing.Size (1028, 22);
      this.pictureScrollBar.TabIndex = 5;
      this.pictureScrollBar.ValueChanged += new System.EventHandler (this.pictureScrollBar_ValueChanged);
      // 
      // cameraLineWindow
      // 
      this.cameraLineWindow.BackColor = System.Drawing.SystemColors.WindowText;
      this.cameraLineWindow.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.cameraLineWindow.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.cameraLineWindow.Location = new System.Drawing.Point (2, 52);
      this.cameraLineWindow.Name = "cameraLineWindow";
      this.cameraLineWindow.Size = new System.Drawing.Size (1028, 537);
      this.cameraLineWindow.TabIndex = 6;
      this.cameraLineWindow.Paint += new System.Windows.Forms.PaintEventHandler (this.cameraLineWindow_Paint);
      // 
      // pollingTimer
      // 
      this.pollingTimer.Tick += new System.EventHandler (this.pollingTimer_Tick);
      // 
      // CameraLineDisplay
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (1035, 687);
      this.Controls.Add (this.cameraLineWindow);
      this.Controls.Add (this.pictureScrollBar);
      this.Controls.Add (this.refreshButton);
      this.Controls.Add (this.exitButton);
      this.Controls.Add (this.scalingBox);
      this.Controls.Add (this.Title);
      this.Name = "CameraLineDisplay";
      this.Text = "CameraLineDisplay";
      this.SizeChanged += new System.EventHandler (this.CameraLineDisplay_SizeChanged);
      this.scalingBox.ResumeLayout (false);
      this.scalingBox.PerformLayout ();
      this.ResumeLayout (false);

    }

    #endregion

    private System.Windows.Forms.Label Title;
    private System.Windows.Forms.GroupBox scalingBox;
    private System.Windows.Forms.RadioButton fullSize;
    private System.Windows.Forms.RadioButton fourToOne;
    private System.Windows.Forms.RadioButton eightToOne;
    private System.Windows.Forms.Button exitButton;
    private System.Windows.Forms.Button refreshButton;
    private System.Windows.Forms.HScrollBar pictureScrollBar;
    private System.Windows.Forms.Panel cameraLineWindow;
    private System.Windows.Forms.Timer pollingTimer;
  }
}