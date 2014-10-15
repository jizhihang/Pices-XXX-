namespace ChartControls
{
  partial class Battery
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
      this.voltageField = new System.Windows.Forms.TextBox();
      this.SuspendLayout();
      // 
      // voltageField
      // 
      this.voltageField.Location = new System.Drawing.Point(46, 130);
      this.voltageField.Name = "voltageField";
      this.voltageField.Size = new System.Drawing.Size(45, 20);
      this.voltageField.TabIndex = 0;
      this.voltageField.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // Battery
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.voltageField);
      this.Name = "Battery";
      this.Size = new System.Drawing.Size(139, 150);
      this.EnabledChanged += new System.EventHandler(this.Battery_EnabledChanged);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.TextBox voltageField;
  }
}
