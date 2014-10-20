namespace SipperFile
{
  partial class FeatureDataDisplay
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
      this.FeatureDataList = new System.Windows.Forms.ListView ();
      this.FeatureName = new System.Windows.Forms.ColumnHeader ();
      this.CalculatedValue = new System.Windows.Forms.ColumnHeader ();
      this.DataBaseValue = new System.Windows.Forms.ColumnHeader ();
      this.Difference = new System.Windows.Forms.ColumnHeader ();
      this.SuspendLayout ();
      // 
      // FeatureDataList
      // 
      this.FeatureDataList.Columns.AddRange (new System.Windows.Forms.ColumnHeader[] {
            this.FeatureName,
            this.CalculatedValue,
            this.DataBaseValue,
            this.Difference});
      this.FeatureDataList.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FeatureDataList.Location = new System.Drawing.Point (12, 12);
      this.FeatureDataList.Name = "FeatureDataList";
      this.FeatureDataList.Size = new System.Drawing.Size (747, 570);
      this.FeatureDataList.TabIndex = 0;
      this.FeatureDataList.UseCompatibleStateImageBehavior = false;
      this.FeatureDataList.View = System.Windows.Forms.View.Details;
      // 
      // FeatureName
      // 
      this.FeatureName.Text = "Feature Name";
      this.FeatureName.Width = 200;
      // 
      // CalculatedValue
      // 
      this.CalculatedValue.Text = "Calculated";
      this.CalculatedValue.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      this.CalculatedValue.Width = 170;
      // 
      // DataBaseValue
      // 
      this.DataBaseValue.Text = "Database";
      this.DataBaseValue.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      this.DataBaseValue.Width = 170;
      // 
      // Difference
      // 
      this.Difference.Text = "Difference";
      this.Difference.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      this.Difference.Width = 170;
      // 
      // FeatureDataDisplay
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (771, 594);
      this.Controls.Add (this.FeatureDataList);
      this.Name = "FeatureDataDisplay";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "Feature Data Display";
      this.ResumeLayout (false);

    }

    #endregion

    private System.Windows.Forms.ListView FeatureDataList;
    private System.Windows.Forms.ColumnHeader FeatureName;
    private System.Windows.Forms.ColumnHeader CalculatedValue;
    private System.Windows.Forms.ColumnHeader DataBaseValue;
    private System.Windows.Forms.ColumnHeader Difference;
  }
}