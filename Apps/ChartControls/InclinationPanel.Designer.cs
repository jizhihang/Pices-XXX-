namespace ChartControls
{
  partial class InclinationPanel
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

    #region Component Designer generated code

    /// <summary> 
    /// Required method for Designer support - do not modify 
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent ()
    {
      this.chartPanel = new System.Windows.Forms.Panel ();
      this.inclinationDisplay = new System.Windows.Forms.TextBox ();
      this.titleLabel = new System.Windows.Forms.Label ();
      this.SuspendLayout ();
      // 
      // chartPanel
      // 
      this.chartPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.chartPanel.Location = new System.Drawing.Point (0, 37);
      this.chartPanel.Margin = new System.Windows.Forms.Padding (4, 4, 4, 4);
      this.chartPanel.Name = "chartPanel";
      this.chartPanel.Size = new System.Drawing.Size (213, 123);
      this.chartPanel.TabIndex = 0;
      this.chartPanel.Layout += new System.Windows.Forms.LayoutEventHandler (this.chartPanel_Layout);
      // 
      // inclinationDisplay
      // 
      this.inclinationDisplay.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.inclinationDisplay.Location = new System.Drawing.Point (0, 160);
      this.inclinationDisplay.Margin = new System.Windows.Forms.Padding (4, 4, 4, 4);
      this.inclinationDisplay.Name = "inclinationDisplay";
      this.inclinationDisplay.Size = new System.Drawing.Size (212, 30);
      this.inclinationDisplay.TabIndex = 1;
      this.inclinationDisplay.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // titleLabel
      // 
      this.titleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.titleLabel.Location = new System.Drawing.Point (0, 0);
      this.titleLabel.Margin = new System.Windows.Forms.Padding (4, 0, 4, 0);
      this.titleLabel.Name = "titleLabel";
      this.titleLabel.Size = new System.Drawing.Size (213, 33);
      this.titleLabel.TabIndex = 2;
      this.titleLabel.Text = "Pitch";
      this.titleLabel.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // InclinationPanel
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add (this.titleLabel);
      this.Controls.Add (this.inclinationDisplay);
      this.Controls.Add (this.chartPanel);
      this.Margin = new System.Windows.Forms.Padding (4, 4, 4, 4);
      this.Name = "InclinationPanel";
      this.Size = new System.Drawing.Size (213, 197);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Panel chartPanel;
    private System.Windows.Forms.TextBox inclinationDisplay;
    private System.Windows.Forms.Label titleLabel;
  }
}
