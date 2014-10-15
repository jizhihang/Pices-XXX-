namespace GradeTrainingModel
{
  partial class ImageListDisplay
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
      this.ImageGrid = new System.Windows.Forms.DataGridView ();
      this.ThumbNail = new System.Windows.Forms.DataGridViewImageColumn ();
      this.Probability = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.FileName = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.KnownClassLabel = new System.Windows.Forms.Label ();
      this.PredictedClassLabel = new System.Windows.Forms.Label ();
      this.KnownClass = new System.Windows.Forms.TextBox ();
      this.PredictedClass = new System.Windows.Forms.TextBox ();
      this.ClassificationProbabilities = new System.Windows.Forms.DataGridView ();
      this.ImageFileName = new System.Windows.Forms.TextBox ();
      this.BinaryProbabilities = new System.Windows.Forms.DataGridView ();
      this.CounterClassName = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.CounterProbability = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.BinaryClassName = new System.Windows.Forms.TextBox ();
      this.FeatureCalcImages = new System.Windows.Forms.DataGridView ();
      this.FeatureCalcedImage = new System.Windows.Forms.DataGridViewImageColumn ();
      this.OperationDescription = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.ClassName = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.PredProbability = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.PredVotes = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      ((System.ComponentModel.ISupportInitialize)(this.ImageGrid)).BeginInit ();
      ((System.ComponentModel.ISupportInitialize)(this.ClassificationProbabilities)).BeginInit ();
      ((System.ComponentModel.ISupportInitialize)(this.BinaryProbabilities)).BeginInit ();
      ((System.ComponentModel.ISupportInitialize)(this.FeatureCalcImages)).BeginInit ();
      this.SuspendLayout ();
      // 
      // ImageGrid
      // 
      this.ImageGrid.AllowUserToAddRows = false;
      this.ImageGrid.AllowUserToDeleteRows = false;
      this.ImageGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.ImageGrid.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.ThumbNail,
            this.Probability,
            this.FileName});
      this.ImageGrid.Location = new System.Drawing.Point (12, 110);
      this.ImageGrid.Name = "ImageGrid";
      this.ImageGrid.Size = new System.Drawing.Size (505, 600);
      this.ImageGrid.TabIndex = 0;
      this.ImageGrid.RowEnter += new System.Windows.Forms.DataGridViewCellEventHandler (this.ImageGrid_RowEnter);
      this.ImageGrid.CellContentDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler (this.ImageGrid_CellContentDoubleClick);
      this.ImageGrid.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.ImageGrid_CellFormatting);
      // 
      // ThumbNail
      // 
      this.ThumbNail.HeaderText = "ThumbNail";
      this.ThumbNail.ImageLayout = System.Windows.Forms.DataGridViewImageCellLayout.Zoom;
      this.ThumbNail.Name = "ThumbNail";
      this.ThumbNail.ReadOnly = true;
      this.ThumbNail.Width = 350;
      // 
      // Probability
      // 
      this.Probability.HeaderText = "Probability";
      this.Probability.MinimumWidth = 60;
      this.Probability.Name = "Probability";
      this.Probability.ReadOnly = true;
      // 
      // FileName
      // 
      this.FileName.HeaderText = "FileName";
      this.FileName.MinimumWidth = 50;
      this.FileName.Name = "FileName";
      this.FileName.ReadOnly = true;
      this.FileName.Visible = false;
      this.FileName.Width = 50;
      // 
      // KnownClassLabel
      // 
      this.KnownClassLabel.AutoSize = true;
      this.KnownClassLabel.Location = new System.Drawing.Point (12, 20);
      this.KnownClassLabel.Name = "KnownClassLabel";
      this.KnownClassLabel.Size = new System.Drawing.Size (68, 13);
      this.KnownClassLabel.TabIndex = 1;
      this.KnownClassLabel.Text = "Known Class";
      // 
      // PredictedClassLabel
      // 
      this.PredictedClassLabel.AutoSize = true;
      this.PredictedClassLabel.Location = new System.Drawing.Point (12, 54);
      this.PredictedClassLabel.Name = "PredictedClassLabel";
      this.PredictedClassLabel.Size = new System.Drawing.Size (80, 13);
      this.PredictedClassLabel.TabIndex = 2;
      this.PredictedClassLabel.Text = "Predicted Class";
      // 
      // KnownClass
      // 
      this.KnownClass.Location = new System.Drawing.Point (92, 17);
      this.KnownClass.Name = "KnownClass";
      this.KnownClass.Size = new System.Drawing.Size (405, 20);
      this.KnownClass.TabIndex = 3;
      // 
      // PredictedClass
      // 
      this.PredictedClass.Location = new System.Drawing.Point (92, 51);
      this.PredictedClass.Name = "PredictedClass";
      this.PredictedClass.Size = new System.Drawing.Size (405, 20);
      this.PredictedClass.TabIndex = 4;
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
      this.ClassificationProbabilities.Location = new System.Drawing.Point (535, 110);
      this.ClassificationProbabilities.Name = "ClassificationProbabilities";
      this.ClassificationProbabilities.ReadOnly = true;
      this.ClassificationProbabilities.Size = new System.Drawing.Size (426, 235);
      this.ClassificationProbabilities.TabIndex = 5;
      this.ClassificationProbabilities.RowEnter += new System.Windows.Forms.DataGridViewCellEventHandler (this.ClassificationProbabilities_RowEnter);
      this.ClassificationProbabilities.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.ClassificationProbabilities_CellFormatting);
      // 
      // ImageFileName
      // 
      this.ImageFileName.Location = new System.Drawing.Point (535, 85);
      this.ImageFileName.Name = "ImageFileName";
      this.ImageFileName.Size = new System.Drawing.Size (426, 20);
      this.ImageFileName.TabIndex = 6;
      // 
      // BinaryProbabilities
      // 
      this.BinaryProbabilities.AllowUserToAddRows = false;
      this.BinaryProbabilities.AllowUserToDeleteRows = false;
      this.BinaryProbabilities.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.BinaryProbabilities.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.CounterClassName,
            this.CounterProbability});
      this.BinaryProbabilities.Location = new System.Drawing.Point (535, 391);
      this.BinaryProbabilities.Name = "BinaryProbabilities";
      this.BinaryProbabilities.ReadOnly = true;
      this.BinaryProbabilities.Size = new System.Drawing.Size (426, 319);
      this.BinaryProbabilities.TabIndex = 7;
      this.BinaryProbabilities.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.BinaryProbabilities_CellFormatting);
      // 
      // CounterClassName
      // 
      this.CounterClassName.HeaderText = "Counter Class Name";
      this.CounterClassName.Name = "CounterClassName";
      this.CounterClassName.ReadOnly = true;
      this.CounterClassName.Width = 255;
      // 
      // CounterProbability
      // 
      this.CounterProbability.HeaderText = "Probability";
      this.CounterProbability.Name = "CounterProbability";
      this.CounterProbability.ReadOnly = true;
      // 
      // BinaryClassName
      // 
      this.BinaryClassName.Location = new System.Drawing.Point (535, 366);
      this.BinaryClassName.Name = "BinaryClassName";
      this.BinaryClassName.Size = new System.Drawing.Size (426, 20);
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
      this.FeatureCalcImages.Location = new System.Drawing.Point (983, 113);
      this.FeatureCalcImages.Name = "FeatureCalcImages";
      this.FeatureCalcImages.Size = new System.Drawing.Size (527, 597);
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
      // ClassName
      // 
      this.ClassName.HeaderText = "Class Name";
      this.ClassName.Name = "ClassName";
      this.ClassName.ReadOnly = true;
      this.ClassName.Width = 220;
      // 
      // PredProbability
      // 
      this.PredProbability.HeaderText = "Probabilty";
      this.PredProbability.Name = "PredProbability";
      this.PredProbability.ReadOnly = true;
      this.PredProbability.Width = 80;
      // 
      // PredVotes
      // 
      this.PredVotes.HeaderText = "Votes";
      this.PredVotes.Name = "PredVotes";
      this.PredVotes.ReadOnly = true;
      this.PredVotes.Width = 70;
      // 
      // ImageListDisplay
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (1537, 747);
      this.Controls.Add (this.FeatureCalcImages);
      this.Controls.Add (this.BinaryClassName);
      this.Controls.Add (this.BinaryProbabilities);
      this.Controls.Add (this.ImageFileName);
      this.Controls.Add (this.ClassificationProbabilities);
      this.Controls.Add (this.PredictedClass);
      this.Controls.Add (this.KnownClass);
      this.Controls.Add (this.PredictedClassLabel);
      this.Controls.Add (this.KnownClassLabel);
      this.Controls.Add (this.ImageGrid);
      this.Name = "ImageListDisplay";
      this.Text = "ImageListDisplay";
      this.Load += new System.EventHandler (this.ImageListDisplay_Load);
      this.Resize += new System.EventHandler (this.ImageListDisplay_Resize);
      ((System.ComponentModel.ISupportInitialize)(this.ImageGrid)).EndInit ();
      ((System.ComponentModel.ISupportInitialize)(this.ClassificationProbabilities)).EndInit ();
      ((System.ComponentModel.ISupportInitialize)(this.BinaryProbabilities)).EndInit ();
      ((System.ComponentModel.ISupportInitialize)(this.FeatureCalcImages)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.DataGridView ImageGrid;
    private System.Windows.Forms.Label KnownClassLabel;
    private System.Windows.Forms.Label PredictedClassLabel;
    private System.Windows.Forms.TextBox KnownClass;
    private System.Windows.Forms.TextBox PredictedClass;
    private System.Windows.Forms.DataGridViewImageColumn ThumbNail;
    private System.Windows.Forms.DataGridViewTextBoxColumn Probability;
    private System.Windows.Forms.DataGridViewTextBoxColumn FileName;
    private System.Windows.Forms.DataGridView ClassificationProbabilities;
    private System.Windows.Forms.TextBox ImageFileName;
    private System.Windows.Forms.DataGridView BinaryProbabilities;
    private System.Windows.Forms.DataGridViewTextBoxColumn CounterClassName;
    private System.Windows.Forms.DataGridViewTextBoxColumn CounterProbability;
    private System.Windows.Forms.TextBox BinaryClassName;
    private System.Windows.Forms.DataGridView FeatureCalcImages;
    private System.Windows.Forms.DataGridViewImageColumn FeatureCalcedImage;
    private System.Windows.Forms.DataGridViewTextBoxColumn OperationDescription;
    private System.Windows.Forms.DataGridViewTextBoxColumn ClassName;
    private System.Windows.Forms.DataGridViewTextBoxColumn PredProbability;
    private System.Windows.Forms.DataGridViewTextBoxColumn PredVotes;
  }
}