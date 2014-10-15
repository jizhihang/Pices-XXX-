namespace ChartControls
{
  partial class ClassSelect
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
      this.Browse = new System.Windows.Forms.Button ();
      this.ClassName = new System.Windows.Forms.TextBox ();
      this.SuspendLayout ();
      // 
      // Browse
      // 
      this.Browse.Location = new System.Drawing.Point (321, 1);
      this.Browse.Name = "Browse";
      this.Browse.Size = new System.Drawing.Size (75, 23);
      this.Browse.TabIndex = 3;
      this.Browse.Text = "button1";
      this.Browse.UseVisualStyleBackColor = true;
      // 
      // ClassName
      // 
      this.ClassName.Location = new System.Drawing.Point (3, 3);
      this.ClassName.Name = "ClassName";
      this.ClassName.ReadOnly = true;
      this.ClassName.Size = new System.Drawing.Size (312, 20);
      this.ClassName.TabIndex = 2;
      // 
      // ClassSelect
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add (this.Browse);
      this.Controls.Add (this.ClassName);
      this.Name = "ClassSelect";
      this.Size = new System.Drawing.Size (400, 29);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Button Browse;
    private System.Windows.Forms.TextBox ClassName;
  }
}
