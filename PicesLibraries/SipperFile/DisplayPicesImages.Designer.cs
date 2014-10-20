namespace SipperFile
{
  partial class DisplayPicesImages
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
      this.ImageDisplayPanel = new System.Windows.Forms.FlowLayoutPanel ();
      this.UpdateDisplayTimer = new System.Windows.Forms.Timer (this.components);
      this.SuspendLayout ();
      // 
      // ImageDisplayPanel
      // 
      this.ImageDisplayPanel.AutoScroll = true;
      this.ImageDisplayPanel.Location = new System.Drawing.Point (12, 12);
      this.ImageDisplayPanel.Name = "ImageDisplayPanel";
      this.ImageDisplayPanel.Size = new System.Drawing.Size (712, 679);
      this.ImageDisplayPanel.TabIndex = 0;
      // 
      // UpdateDisplayTimer
      // 
      this.UpdateDisplayTimer.Interval = 50;
      this.UpdateDisplayTimer.Tick += new System.EventHandler (this.UpdateDisplayTimer_Tick);
      // 
      // DisplayPicesImages
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
      this.ClientSize = new System.Drawing.Size (733, 702);
      this.Controls.Add (this.ImageDisplayPanel);
      this.Name = "DisplayPicesImages";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "DisplayPicesImages";
      this.Load += new System.EventHandler (this.DisplayPicesImages_Load);
      this.ResizeEnd += new System.EventHandler (this.DisplayPicesImages_ResizeEnd);
      this.ResumeLayout (false);

    }

    #endregion

    private System.Windows.Forms.FlowLayoutPanel ImageDisplayPanel;
    private System.Windows.Forms.Timer UpdateDisplayTimer;
  }
}