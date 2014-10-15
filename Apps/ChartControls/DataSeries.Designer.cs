namespace ChartControls
{
  partial class DataSeries
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
      this.dataLabel = new System.Windows.Forms.ComboBox ();
      this.dataMin = new System.Windows.Forms.TextBox ();
      this.dataMax = new System.Windows.Forms.TextBox ();
      this.data = new System.Windows.Forms.TextBox ();
      this.plotData = new System.Windows.Forms.CheckBox ();
      this.plotColorDialog = new System.Windows.Forms.ColorDialog ();
      this.SuspendLayout ();
      // 
      // dataLabel
      // 
      this.dataLabel.Font = new System.Drawing.Font ("Arial", 10F);
      this.dataLabel.FormattingEnabled = true;
      this.dataLabel.Items.AddRange (new object[] {
            "Depth",
            "Oxygen",
            "Pitch",
            "Roll",
            "Temperature"});
      this.dataLabel.Location = new System.Drawing.Point (3, 3);
      this.dataLabel.Name = "dataLabel";
      this.dataLabel.Size = new System.Drawing.Size (177, 24);
      this.dataLabel.TabIndex = 0;
      this.dataLabel.SelectedIndexChanged += new System.EventHandler (this.dataLabel_SelectedIndexChanged);
      // 
      // dataMin
      // 
      this.dataMin.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F);
      this.dataMin.Location = new System.Drawing.Point (187, 3);
      this.dataMin.Name = "dataMin";
      this.dataMin.Size = new System.Drawing.Size (100, 23);
      this.dataMin.TabIndex = 1;
      this.dataMin.Text = " ";
      this.dataMin.Leave += new System.EventHandler (this.dataMin_Leave);
      // 
      // dataMax
      // 
      this.dataMax.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F);
      this.dataMax.Location = new System.Drawing.Point (295, 3);
      this.dataMax.Name = "dataMax";
      this.dataMax.Size = new System.Drawing.Size (100, 23);
      this.dataMax.TabIndex = 2;
      this.dataMax.Text = " ";
      this.dataMax.Leave += new System.EventHandler (this.dataMax_Leave);
      // 
      // data
      // 
      this.data.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F);
      this.data.Location = new System.Drawing.Point (405, 3);
      this.data.Name = "data";
      this.data.ReadOnly = true;
      this.data.Size = new System.Drawing.Size (120, 23);
      this.data.TabIndex = 3;
      this.data.TabStop = false;
      this.data.Text = " ";
      // 
      // plotData
      // 
      this.plotData.AutoSize = true;
      this.plotData.Location = new System.Drawing.Point (539, 6);
      this.plotData.Name = "plotData";
      this.plotData.Size = new System.Drawing.Size (43, 17);
      this.plotData.TabIndex = 4;
      this.plotData.Text = "plot";
      this.plotData.UseVisualStyleBackColor = true;
      this.plotData.CheckedChanged += new System.EventHandler (this.plotData_CheckedChanged);
      // 
      // plotColorDialog
      // 
      this.plotColorDialog.AnyColor = true;
      this.plotColorDialog.SolidColorOnly = true;
      // 
      // DataSeries
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.Controls.Add (this.plotData);
      this.Controls.Add (this.data);
      this.Controls.Add (this.dataMax);
      this.Controls.Add (this.dataMin);
      this.Controls.Add (this.dataLabel);
      this.Name = "DataSeries";
      this.Size = new System.Drawing.Size (586, 31);
      this.DoubleClick += new System.EventHandler (this.DataSeries_DoubleClick);
      this.Load += new System.EventHandler (this.DataSeries_Load);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.ComboBox dataLabel;
    private System.Windows.Forms.TextBox dataMin;
    private System.Windows.Forms.TextBox dataMax;
    private System.Windows.Forms.TextBox data;
    private System.Windows.Forms.CheckBox plotData;
    private System.Windows.Forms.ColorDialog plotColorDialog;
  }
}
