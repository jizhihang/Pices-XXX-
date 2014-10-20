namespace SipperFile
{
  partial class PredictionBreakDownDisplay
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
      System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle ();
      System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle ();
      System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle ();
      this.ClassificationProbabilities = new System.Windows.Forms.DataGridView ();
      this.ClassName = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.PredProbability = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.PredVotes = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.BinaryProbabilities = new System.Windows.Forms.DataGridView ();
      this.CounterClassName = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.CounterProbability = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.BinaryClassName = new System.Windows.Forms.TextBox ();
      this.FeatureCalcImages = new System.Windows.Forms.DataGridView ();
      this.FeatureCalcedImage = new System.Windows.Forms.DataGridViewImageColumn ();
      this.OperationDescription = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.BinaryClassVsClassProbablities = new System.Windows.Forms.TextBox ();
      this.DisplayFeatureDataButton = new System.Windows.Forms.Button ();
      ((System.ComponentModel.ISupportInitialize)(this.ClassificationProbabilities)).BeginInit ();
      ((System.ComponentModel.ISupportInitialize)(this.BinaryProbabilities)).BeginInit ();
      ((System.ComponentModel.ISupportInitialize)(this.FeatureCalcImages)).BeginInit ();
      this.SuspendLayout ();
      // 
      // ClassificationProbabilities
      // 
      this.ClassificationProbabilities.AllowUserToAddRows = false;
      this.ClassificationProbabilities.AllowUserToDeleteRows = false;
      this.ClassificationProbabilities.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.ClassificationProbabilities.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.ClassName,
            this.PredProbability,
            this.PredVotes});
      this.ClassificationProbabilities.Location = new System.Drawing.Point (10, 12);
      this.ClassificationProbabilities.Name = "ClassificationProbabilities";
      this.ClassificationProbabilities.ReadOnly = true;
      this.ClassificationProbabilities.RowHeadersVisible = false;
      this.ClassificationProbabilities.RowTemplate.Height = 24;
      this.ClassificationProbabilities.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
      this.ClassificationProbabilities.Size = new System.Drawing.Size (425, 250);
      this.ClassificationProbabilities.TabIndex = 5;
      this.ClassificationProbabilities.RowEnter += new System.Windows.Forms.DataGridViewCellEventHandler (this.ClassificationProbabilities_RowEnter);
      this.ClassificationProbabilities.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.ClassificationProbabilities_MouseDoubleClick);
      this.ClassificationProbabilities.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.ClassificationProbabilities_CellFormatting);
      // 
      // ClassName
      // 
      this.ClassName.HeaderText = "Class Name";
      this.ClassName.Name = "ClassName";
      this.ClassName.ReadOnly = true;
      this.ClassName.Width = 240;
      // 
      // PredProbability
      // 
      dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
      this.PredProbability.DefaultCellStyle = dataGridViewCellStyle1;
      this.PredProbability.HeaderText = "Probabilty";
      this.PredProbability.Name = "PredProbability";
      this.PredProbability.ReadOnly = true;
      this.PredProbability.Width = 80;
      // 
      // PredVotes
      // 
      dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
      this.PredVotes.DefaultCellStyle = dataGridViewCellStyle2;
      this.PredVotes.HeaderText = "Votes";
      this.PredVotes.Name = "PredVotes";
      this.PredVotes.ReadOnly = true;
      this.PredVotes.Width = 80;
      // 
      // BinaryProbabilities
      // 
      this.BinaryProbabilities.AllowUserToAddRows = false;
      this.BinaryProbabilities.AllowUserToDeleteRows = false;
      this.BinaryProbabilities.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.BinaryProbabilities.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.CounterClassName,
            this.CounterProbability});
      this.BinaryProbabilities.Location = new System.Drawing.Point (10, 292);
      this.BinaryProbabilities.Name = "BinaryProbabilities";
      this.BinaryProbabilities.ReadOnly = true;
      this.BinaryProbabilities.RowHeadersVisible = false;
      this.BinaryProbabilities.RowTemplate.Height = 24;
      this.BinaryProbabilities.Size = new System.Drawing.Size (425, 298);
      this.BinaryProbabilities.TabIndex = 7;
      this.BinaryProbabilities.RowEnter += new System.Windows.Forms.DataGridViewCellEventHandler (this.BinaryProbabilities_RowEnter);
      this.BinaryProbabilities.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.BinaryProbabilities_MouseDoubleClick);
      this.BinaryProbabilities.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.BinaryProbabilities_CellFormatting);
      // 
      // CounterClassName
      // 
      this.CounterClassName.HeaderText = "Counter Class Name";
      this.CounterClassName.Name = "CounterClassName";
      this.CounterClassName.ReadOnly = true;
      this.CounterClassName.Width = 260;
      // 
      // CounterProbability
      // 
      dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
      this.CounterProbability.DefaultCellStyle = dataGridViewCellStyle3;
      this.CounterProbability.HeaderText = "Probability";
      this.CounterProbability.Name = "CounterProbability";
      this.CounterProbability.ReadOnly = true;
      this.CounterProbability.Width = 80;
      // 
      // BinaryClassName
      // 
      this.BinaryClassName.Location = new System.Drawing.Point (10, 269);
      this.BinaryClassName.Name = "BinaryClassName";
      this.BinaryClassName.ReadOnly = true;
      this.BinaryClassName.Size = new System.Drawing.Size (425, 20);
      this.BinaryClassName.TabIndex = 8;
      this.BinaryClassName.TabStop = false;
      // 
      // FeatureCalcImages
      // 
      this.FeatureCalcImages.AllowUserToAddRows = false;
      this.FeatureCalcImages.AllowUserToDeleteRows = false;
      this.FeatureCalcImages.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.FeatureCalcImages.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.FeatureCalcedImage,
            this.OperationDescription});
      this.FeatureCalcImages.Location = new System.Drawing.Point (452, 13);
      this.FeatureCalcImages.Name = "FeatureCalcImages";
      this.FeatureCalcImages.RowHeadersVisible = false;
      this.FeatureCalcImages.RowTemplate.Height = 24;
      this.FeatureCalcImages.Size = new System.Drawing.Size (497, 577);
      this.FeatureCalcImages.TabIndex = 9;
      this.FeatureCalcImages.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.FeatureCalcImages_CellFormatting);
      // 
      // FeatureCalcedImage
      // 
      this.FeatureCalcedImage.HeaderText = "FeatureCalc Images";
      this.FeatureCalcedImage.Name = "FeatureCalcedImage";
      this.FeatureCalcedImage.Resizable = System.Windows.Forms.DataGridViewTriState.True;
      this.FeatureCalcedImage.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
      this.FeatureCalcedImage.Width = 300;
      // 
      // OperationDescription
      // 
      this.OperationDescription.HeaderText = "Description";
      this.OperationDescription.Name = "OperationDescription";
      this.OperationDescription.Width = 150;
      // 
      // BinaryClassVsClassProbablities
      // 
      this.BinaryClassVsClassProbablities.Location = new System.Drawing.Point (10, 593);
      this.BinaryClassVsClassProbablities.Name = "BinaryClassVsClassProbablities";
      this.BinaryClassVsClassProbablities.ReadOnly = true;
      this.BinaryClassVsClassProbablities.Size = new System.Drawing.Size (405, 20);
      this.BinaryClassVsClassProbablities.TabIndex = 10;
      this.BinaryClassVsClassProbablities.TabStop = false;
      // 
      // DisplayFeatureDataButton
      // 
      this.DisplayFeatureDataButton.Location = new System.Drawing.Point (452, 593);
      this.DisplayFeatureDataButton.Name = "DisplayFeatureDataButton";
      this.DisplayFeatureDataButton.Size = new System.Drawing.Size (115, 23);
      this.DisplayFeatureDataButton.TabIndex = 11;
      this.DisplayFeatureDataButton.Text = "Display Feature Data";
      this.DisplayFeatureDataButton.UseVisualStyleBackColor = true;
      this.DisplayFeatureDataButton.Click += new System.EventHandler (this.DisplayFeatureDataButton_Click);
      // 
      // PredictionBreakDownDisplay
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (962, 627);
      this.Controls.Add (this.DisplayFeatureDataButton);
      this.Controls.Add (this.BinaryClassVsClassProbablities);
      this.Controls.Add (this.FeatureCalcImages);
      this.Controls.Add (this.BinaryClassName);
      this.Controls.Add (this.BinaryProbabilities);
      this.Controls.Add (this.ClassificationProbabilities);
      this.Name = "PredictionBreakDownDisplay";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "ImageListDisplay";
      this.Load += new System.EventHandler (this.ImageListDisplay_Load);
      this.Resize += new System.EventHandler (this.ImageListDisplay_Resize);
      ((System.ComponentModel.ISupportInitialize)(this.ClassificationProbabilities)).EndInit ();
      ((System.ComponentModel.ISupportInitialize)(this.BinaryProbabilities)).EndInit ();
      ((System.ComponentModel.ISupportInitialize)(this.FeatureCalcImages)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.DataGridView ClassificationProbabilities;
    private System.Windows.Forms.DataGridView BinaryProbabilities;
    private System.Windows.Forms.TextBox BinaryClassName;
    private System.Windows.Forms.DataGridView FeatureCalcImages;
    private System.Windows.Forms.DataGridViewImageColumn FeatureCalcedImage;
    private System.Windows.Forms.DataGridViewTextBoxColumn OperationDescription;
    private System.Windows.Forms.TextBox BinaryClassVsClassProbablities;
    private System.Windows.Forms.Button DisplayFeatureDataButton;
    private System.Windows.Forms.DataGridViewTextBoxColumn CounterClassName;
    private System.Windows.Forms.DataGridViewTextBoxColumn CounterProbability;
    private System.Windows.Forms.DataGridViewTextBoxColumn ClassName;
    private System.Windows.Forms.DataGridViewTextBoxColumn PredProbability;
    private System.Windows.Forms.DataGridViewTextBoxColumn PredVotes;
  }
}