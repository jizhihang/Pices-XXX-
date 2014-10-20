namespace SipperFile
{
  partial class CruiseMaintenance
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
      this.TitleLabel = new System.Windows.Forms.Label ();
      this.CruiseTreeList = new System.Windows.Forms.TreeView ();
      this.AddNewCruiseButton = new System.Windows.Forms.Button ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (212, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (178, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Cruise Maintenance";
      // 
      // CruiseTreeList
      // 
      this.CruiseTreeList.Font = new System.Drawing.Font ("Courier New", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseTreeList.Location = new System.Drawing.Point (24, 50);
      this.CruiseTreeList.Name = "CruiseTreeList";
      this.CruiseTreeList.Size = new System.Drawing.Size (556, 411);
      this.CruiseTreeList.TabIndex = 1;
      // 
      // AddNewCruiseButton
      // 
      this.AddNewCruiseButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.AddNewCruiseButton.Location = new System.Drawing.Point (234, 467);
      this.AddNewCruiseButton.Name = "AddNewCruiseButton";
      this.AddNewCruiseButton.Size = new System.Drawing.Size (141, 23);
      this.AddNewCruiseButton.TabIndex = 2;
      this.AddNewCruiseButton.Text = "Add a New Cruise";
      this.AddNewCruiseButton.UseVisualStyleBackColor = true;
      this.AddNewCruiseButton.Click += new System.EventHandler (this.AddNewCruiseButton_Click);
      // 
      // CruiseMaintenance
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (607, 514);
      this.Controls.Add (this.AddNewCruiseButton);
      this.Controls.Add (this.CruiseTreeList);
      this.Controls.Add (this.TitleLabel);
      this.MinimumSize = new System.Drawing.Size (300, 200);
      this.Name = "CruiseMaintenance";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "CruiseMaintenance";
      this.Load += new System.EventHandler (this.CruiseMaintenance_Load);
      this.SizeChanged += new System.EventHandler (this.CruiseMaintenance_SizeChanged);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.CruiseMaintenance_FormClosing);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.TreeView CruiseTreeList;
    private System.Windows.Forms.Button AddNewCruiseButton;
  }
}