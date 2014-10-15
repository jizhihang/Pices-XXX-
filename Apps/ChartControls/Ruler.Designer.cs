namespace ChartControls
{
  partial class Ruler
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
      this.minValueField = new System.Windows.Forms.TextBox ();
      this.maxValueField = new System.Windows.Forms.TextBox ();
      this.plot = new System.Windows.Forms.CheckBox ();
      this.SuspendLayout ();
      // 
      // minValueField
      // 
      this.minValueField.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.minValueField.Location = new System.Drawing.Point (0, 0);
      this.minValueField.Margin = new System.Windows.Forms.Padding (4);
      this.minValueField.Name = "minValueField";
      this.minValueField.Size = new System.Drawing.Size (132, 26);
      this.minValueField.TabIndex = 0;
      this.minValueField.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      this.minValueField.Leave += new System.EventHandler (this.minValueField_Leave);
      // 
      // maxValueField
      // 
      this.maxValueField.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.maxValueField.Location = new System.Drawing.Point (693, 0);
      this.maxValueField.Margin = new System.Windows.Forms.Padding (4);
      this.maxValueField.Name = "maxValueField";
      this.maxValueField.Size = new System.Drawing.Size (132, 26);
      this.maxValueField.TabIndex = 1;
      this.maxValueField.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      this.maxValueField.Leave += new System.EventHandler (this.maxValueField_Leave);
      // 
      // plot
      // 
      this.plot.AutoSize = true;
      this.plot.Font = new System.Drawing.Font ("Microsoft Sans Serif", 3.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.plot.ForeColor = System.Drawing.Color.Silver;
      this.plot.Location = new System.Drawing.Point (141, 5);
      this.plot.Margin = new System.Windows.Forms.Padding (4);
      this.plot.Name = "plot";
      this.plot.RightToLeft = System.Windows.Forms.RightToLeft.No;
      this.plot.Size = new System.Drawing.Size (18, 17);
      this.plot.TabIndex = 2;
      this.plot.UseVisualStyleBackColor = true;
      this.plot.CheckedChanged += new System.EventHandler (this.plot_CheckedChanged);
      // 
      // Ruler
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.BackColor = System.Drawing.Color.Silver;
      this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.Controls.Add (this.plot);
      this.Controls.Add (this.maxValueField);
      this.Controls.Add (this.minValueField);
      this.Margin = new System.Windows.Forms.Padding (4);
      this.Name = "Ruler";
      this.Size = new System.Drawing.Size (824, 47);
      this.Load += new System.EventHandler (this.Ruler_Load);
      this.Layout += new System.Windows.Forms.LayoutEventHandler (this.Ruler_Layout);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.TextBox minValueField;
    private System.Windows.Forms.TextBox maxValueField;
    private System.Windows.Forms.CheckBox plot;
  }
}
