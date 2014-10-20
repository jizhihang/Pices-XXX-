namespace SipperFile
{
  partial class SaveRawCTDData
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
      this.SipperFileNameLabel = new System.Windows.Forms.Label ();
      this.SipperFileName = new System.Windows.Forms.TextBox ();
      this.CtdFileName = new System.Windows.Forms.TextBox ();
      this.CtdFileNameLabel = new System.Windows.Forms.Label ();
      this.RunTimeMsgs = new System.Windows.Forms.TextBox ();
      this.RunTimeMsgsLabel = new System.Windows.Forms.Label ();
      this.timer1 = new System.Windows.Forms.Timer (this.components);
      this.SuspendLayout ();
      // 
      // SipperFileNameLabel
      // 
      this.SipperFileNameLabel.AutoSize = true;
      this.SipperFileNameLabel.Location = new System.Drawing.Point (13, 32);
      this.SipperFileNameLabel.Name = "SipperFileNameLabel";
      this.SipperFileNameLabel.Size = new System.Drawing.Size (87, 13);
      this.SipperFileNameLabel.TabIndex = 0;
      this.SipperFileNameLabel.Text = "Sipper File Name";
      // 
      // SipperFileName
      // 
      this.SipperFileName.Location = new System.Drawing.Point (106, 29);
      this.SipperFileName.Name = "SipperFileName";
      this.SipperFileName.ReadOnly = true;
      this.SipperFileName.Size = new System.Drawing.Size (550, 20);
      this.SipperFileName.TabIndex = 1;
      // 
      // CtdFileName
      // 
      this.CtdFileName.Location = new System.Drawing.Point (106, 71);
      this.CtdFileName.Name = "CtdFileName";
      this.CtdFileName.ReadOnly = true;
      this.CtdFileName.Size = new System.Drawing.Size (550, 20);
      this.CtdFileName.TabIndex = 4;
      // 
      // CtdFileNameLabel
      // 
      this.CtdFileNameLabel.AutoSize = true;
      this.CtdFileNameLabel.Location = new System.Drawing.Point (13, 74);
      this.CtdFileNameLabel.Name = "CtdFileNameLabel";
      this.CtdFileNameLabel.Size = new System.Drawing.Size (79, 13);
      this.CtdFileNameLabel.TabIndex = 3;
      this.CtdFileNameLabel.Text = "CTD File Name";
      // 
      // RunTimeMsgs
      // 
      this.RunTimeMsgs.Location = new System.Drawing.Point (16, 127);
      this.RunTimeMsgs.Multiline = true;
      this.RunTimeMsgs.Name = "RunTimeMsgs";
      this.RunTimeMsgs.ReadOnly = true;
      this.RunTimeMsgs.Size = new System.Drawing.Size (640, 339);
      this.RunTimeMsgs.TabIndex = 5;
      // 
      // RunTimeMsgsLabel
      // 
      this.RunTimeMsgsLabel.AutoSize = true;
      this.RunTimeMsgsLabel.Location = new System.Drawing.Point (13, 111);
      this.RunTimeMsgsLabel.Name = "RunTimeMsgsLabel";
      this.RunTimeMsgsLabel.Size = new System.Drawing.Size (104, 13);
      this.RunTimeMsgsLabel.TabIndex = 6;
      this.RunTimeMsgsLabel.Text = "Run Time Messages";
      // 
      // timer1
      // 
      this.timer1.Enabled = true;
      this.timer1.Tick += new System.EventHandler (this.timer1_Tick);
      // 
      // SaveRawCTDData
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (672, 478);
      this.Controls.Add (this.RunTimeMsgsLabel);
      this.Controls.Add (this.RunTimeMsgs);
      this.Controls.Add (this.CtdFileName);
      this.Controls.Add (this.CtdFileNameLabel);
      this.Controls.Add (this.SipperFileName);
      this.Controls.Add (this.SipperFileNameLabel);
      this.Name = "SaveRawCTDData";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "Save Raw CTD Data";
      this.Load += new System.EventHandler (this.SaveRawCTDData_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.SaveRawCTDData_FormClosing);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label SipperFileNameLabel;
    private System.Windows.Forms.TextBox SipperFileName;
    private System.Windows.Forms.TextBox CtdFileName;
    private System.Windows.Forms.Label CtdFileNameLabel;
    private System.Windows.Forms.TextBox RunTimeMsgs;
    private System.Windows.Forms.Label RunTimeMsgsLabel;
    private System.Windows.Forms.Timer timer1;
  }
}