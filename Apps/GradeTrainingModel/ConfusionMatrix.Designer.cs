namespace GradeTrainingModel
{
  partial class ConfusionMatrix
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
      System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem (new string[] {
            "This is a test",
            "Copepod",
            "100",
            "70",
            "30",
            ""}, -1);
      System.Windows.Forms.ListViewItem listViewItem4 = new System.Windows.Forms.ListViewItem ("Second Line");
      this.NameLabel = new System.Windows.Forms.Label ();
      this.Matrix = new System.Windows.Forms.ListView ();
      this.ClassName = new System.Windows.Forms.ColumnHeader ();
      this.Count = new System.Windows.Forms.ColumnHeader ();
      this.Class1 = new System.Windows.Forms.ColumnHeader ();
      this.Class2 = new System.Windows.Forms.ColumnHeader ();
      this.PercentagesButton = new System.Windows.Forms.Button ();
      this.MinProbability = new System.Windows.Forms.ComboBox ();
      this.MinProbabilityLabel = new System.Windows.Forms.Label ();
      this.MaxProbability = new System.Windows.Forms.ComboBox ();
      this.MaxProbabilityLabel = new System.Windows.Forms.Label ();
      this.ReTallyButton = new System.Windows.Forms.Button ();
      this.AllExamplesLabel = new System.Windows.Forms.Label ();
      this.TotalAll = new System.Windows.Forms.TextBox ();
      this.TotalCorrectLabel = new System.Windows.Forms.Label ();
      this.CorrectAll = new System.Windows.Forms.TextBox ();
      this.AccuracyLabel = new System.Windows.Forms.Label ();
      this.AccuracyAll = new System.Windows.Forms.TextBox ();
      this.TrainLevelLabel = new System.Windows.Forms.Label ();
      this.TrainingLevel = new System.Windows.Forms.TextBox ();
      this.PredictionLevelLabel = new System.Windows.Forms.Label ();
      this.PredictionLevel = new System.Windows.Forms.ComboBox ();
      this.SaveButton = new System.Windows.Forms.Button ();
      this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog ();
      this.ModelName = new System.Windows.Forms.TextBox ();
      this.GroundTruthLabel = new System.Windows.Forms.Label ();
      this.GroundTruth = new System.Windows.Forms.TextBox ();
      this.CorrectNonNoise = new System.Windows.Forms.TextBox ();
      this.AccuracyPartOfClassifier = new System.Windows.Forms.TextBox ();
      this.TotalNonNoise2 = new System.Windows.Forms.TextBox ();
      this.NonNoiseLabel = new System.Windows.Forms.Label ();
      this.PartOfClassfierLabel = new System.Windows.Forms.Label ();
      this.TotalPartOfClassifier = new System.Windows.Forms.TextBox ();
      this.TotalLabel = new System.Windows.Forms.Label ();
      this.CorrectPartOfClassifier = new System.Windows.Forms.TextBox ();
      this.AccuracyNonNoise = new System.Windows.Forms.TextBox ();
      this.Summary = new System.Windows.Forms.Panel ();
      this.NoiseLabel = new System.Windows.Forms.Label ();
      this.AccuracyNoise2 = new System.Windows.Forms.TextBox ();
      this.TotalNoise2 = new System.Windows.Forms.TextBox ();
      this.CorrectNoise2 = new System.Windows.Forms.TextBox ();
      this.AcuracyNPCLabel = new System.Windows.Forms.Label ();
      this.AccuracyAll_NPC = new System.Windows.Forms.TextBox ();
      this.AccuracyPartOfClassifier_NPC = new System.Windows.Forms.TextBox ();
      this.AccuracyNoise2_NPC = new System.Windows.Forms.TextBox ();
      this.AccuracyNonNoise_NPC = new System.Windows.Forms.TextBox ();
      this.Summary.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // NameLabel
      // 
      this.NameLabel.AutoSize = true;
      this.NameLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.NameLabel.Location = new System.Drawing.Point (28, 19);
      this.NameLabel.Name = "NameLabel";
      this.NameLabel.Size = new System.Drawing.Size (86, 16);
      this.NameLabel.TabIndex = 0;
      this.NameLabel.Text = "Model Name";
      this.NameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // Matrix
      // 
      this.Matrix.Activation = System.Windows.Forms.ItemActivation.TwoClick;
      this.Matrix.BackColor = System.Drawing.SystemColors.Window;
      this.Matrix.Columns.AddRange (new System.Windows.Forms.ColumnHeader[] {
            this.ClassName,
            this.Count,
            this.Class1,
            this.Class2});
      this.Matrix.FullRowSelect = true;
      this.Matrix.GridLines = true;
      this.Matrix.Items.AddRange (new System.Windows.Forms.ListViewItem[] {
            listViewItem3,
            listViewItem4});
      this.Matrix.Location = new System.Drawing.Point (20, 107);
      this.Matrix.MultiSelect = false;
      this.Matrix.Name = "Matrix";
      this.Matrix.Size = new System.Drawing.Size (891, 447);
      this.Matrix.TabIndex = 1;
      this.Matrix.UseCompatibleStateImageBehavior = false;
      this.Matrix.View = System.Windows.Forms.View.Details;
      this.Matrix.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.Matrix_MouseDoubleClick);
      // 
      // ClassName
      // 
      this.ClassName.Text = "ClassName";
      this.ClassName.Width = 150;
      // 
      // Count
      // 
      this.Count.Text = "Count";
      this.Count.Width = 62;
      // 
      // Class1
      // 
      this.Class1.Text = "Class 1";
      this.Class1.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // Class2
      // 
      this.Class2.Text = "Class 2";
      this.Class2.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // PercentagesButton
      // 
      this.PercentagesButton.Location = new System.Drawing.Point (24, 68);
      this.PercentagesButton.Name = "PercentagesButton";
      this.PercentagesButton.Size = new System.Drawing.Size (75, 23);
      this.PercentagesButton.TabIndex = 2;
      this.PercentagesButton.Text = "Percentages";
      this.PercentagesButton.UseVisualStyleBackColor = true;
      this.PercentagesButton.Click += new System.EventHandler (this.PercentagesButton_Click);
      // 
      // MinProbability
      // 
      this.MinProbability.FormattingEnabled = true;
      this.MinProbability.Items.AddRange (new object[] {
            "0",
            "10",
            "20",
            "30",
            "40",
            "50",
            "60",
            "70",
            "80",
            "90"});
      this.MinProbability.Location = new System.Drawing.Point (171, 68);
      this.MinProbability.Name = "MinProbability";
      this.MinProbability.Size = new System.Drawing.Size (68, 21);
      this.MinProbability.TabIndex = 3;
      this.MinProbability.Leave += new System.EventHandler (this.MinProbability_Leave);
      // 
      // MinProbabilityLabel
      // 
      this.MinProbabilityLabel.AutoSize = true;
      this.MinProbabilityLabel.Location = new System.Drawing.Point (122, 71);
      this.MinProbabilityLabel.Name = "MinProbabilityLabel";
      this.MinProbabilityLabel.Size = new System.Drawing.Size (49, 13);
      this.MinProbabilityLabel.TabIndex = 4;
      this.MinProbabilityLabel.Text = "Min Prob";
      // 
      // MaxProbability
      // 
      this.MaxProbability.FormattingEnabled = true;
      this.MaxProbability.Items.AddRange (new object[] {
            "0",
            "10",
            "20",
            "30",
            "40",
            "50",
            "60",
            "70",
            "80",
            "90"});
      this.MaxProbability.Location = new System.Drawing.Point (307, 68);
      this.MaxProbability.Name = "MaxProbability";
      this.MaxProbability.Size = new System.Drawing.Size (68, 21);
      this.MaxProbability.TabIndex = 5;
      this.MaxProbability.Leave += new System.EventHandler (this.MaxProbability_Leave);
      // 
      // MaxProbabilityLabel
      // 
      this.MaxProbabilityLabel.AutoSize = true;
      this.MaxProbabilityLabel.Location = new System.Drawing.Point (255, 71);
      this.MaxProbabilityLabel.Name = "MaxProbabilityLabel";
      this.MaxProbabilityLabel.Size = new System.Drawing.Size (52, 13);
      this.MaxProbabilityLabel.TabIndex = 6;
      this.MaxProbabilityLabel.Text = "Max Prob";
      // 
      // ReTallyButton
      // 
      this.ReTallyButton.Location = new System.Drawing.Point (836, 68);
      this.ReTallyButton.Name = "ReTallyButton";
      this.ReTallyButton.Size = new System.Drawing.Size (75, 23);
      this.ReTallyButton.TabIndex = 7;
      this.ReTallyButton.Text = "Re-Tally";
      this.ReTallyButton.UseVisualStyleBackColor = true;
      this.ReTallyButton.Click += new System.EventHandler (this.ReTallyButton_Click);
      // 
      // AllExamplesLabel
      // 
      this.AllExamplesLabel.AutoSize = true;
      this.AllExamplesLabel.Location = new System.Drawing.Point (112, 10);
      this.AllExamplesLabel.Name = "AllExamplesLabel";
      this.AllExamplesLabel.Size = new System.Drawing.Size (55, 13);
      this.AllExamplesLabel.TabIndex = 8;
      this.AllExamplesLabel.Text = "All Images";
      // 
      // TotalAll
      // 
      this.TotalAll.Location = new System.Drawing.Point (96, 29);
      this.TotalAll.Name = "TotalAll";
      this.TotalAll.Size = new System.Drawing.Size (86, 20);
      this.TotalAll.TabIndex = 9;
      this.TotalAll.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // TotalCorrectLabel
      // 
      this.TotalCorrectLabel.AutoSize = true;
      this.TotalCorrectLabel.Location = new System.Drawing.Point (3, 63);
      this.TotalCorrectLabel.Name = "TotalCorrectLabel";
      this.TotalCorrectLabel.Size = new System.Drawing.Size (68, 13);
      this.TotalCorrectLabel.TabIndex = 10;
      this.TotalCorrectLabel.Text = "Total Correct";
      // 
      // CorrectAll
      // 
      this.CorrectAll.Location = new System.Drawing.Point (96, 59);
      this.CorrectAll.Name = "CorrectAll";
      this.CorrectAll.Size = new System.Drawing.Size (86, 20);
      this.CorrectAll.TabIndex = 11;
      this.CorrectAll.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // AccuracyLabel
      // 
      this.AccuracyLabel.AutoSize = true;
      this.AccuracyLabel.Location = new System.Drawing.Point (4, 93);
      this.AccuracyLabel.Name = "AccuracyLabel";
      this.AccuracyLabel.Size = new System.Drawing.Size (52, 13);
      this.AccuracyLabel.TabIndex = 12;
      this.AccuracyLabel.Text = "Accuracy";
      // 
      // AccuracyAll
      // 
      this.AccuracyAll.Location = new System.Drawing.Point (96, 89);
      this.AccuracyAll.Name = "AccuracyAll";
      this.AccuracyAll.Size = new System.Drawing.Size (86, 20);
      this.AccuracyAll.TabIndex = 13;
      this.AccuracyAll.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // TrainLevelLabel
      // 
      this.TrainLevelLabel.AutoSize = true;
      this.TrainLevelLabel.Location = new System.Drawing.Point (397, 71);
      this.TrainLevelLabel.Name = "TrainLevelLabel";
      this.TrainLevelLabel.Size = new System.Drawing.Size (74, 13);
      this.TrainLevelLabel.TabIndex = 14;
      this.TrainLevelLabel.Text = "Training Level";
      // 
      // TrainingLevel
      // 
      this.TrainingLevel.Location = new System.Drawing.Point (471, 69);
      this.TrainingLevel.Name = "TrainingLevel";
      this.TrainingLevel.ReadOnly = true;
      this.TrainingLevel.Size = new System.Drawing.Size (30, 20);
      this.TrainingLevel.TabIndex = 15;
      this.TrainingLevel.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // PredictionLevelLabel
      // 
      this.PredictionLevelLabel.AutoSize = true;
      this.PredictionLevelLabel.Location = new System.Drawing.Point (527, 71);
      this.PredictionLevelLabel.Name = "PredictionLevelLabel";
      this.PredictionLevelLabel.Size = new System.Drawing.Size (83, 13);
      this.PredictionLevelLabel.TabIndex = 16;
      this.PredictionLevelLabel.Text = "Prediction Level";
      // 
      // PredictionLevel
      // 
      this.PredictionLevel.FormattingEnabled = true;
      this.PredictionLevel.Items.AddRange (new object[] {
            "0",
            "10",
            "20",
            "30",
            "40",
            "50",
            "60",
            "70",
            "80",
            "90"});
      this.PredictionLevel.Location = new System.Drawing.Point (613, 68);
      this.PredictionLevel.Name = "PredictionLevel";
      this.PredictionLevel.Size = new System.Drawing.Size (34, 21);
      this.PredictionLevel.TabIndex = 17;
      // 
      // SaveButton
      // 
      this.SaveButton.Location = new System.Drawing.Point (836, 668);
      this.SaveButton.Name = "SaveButton";
      this.SaveButton.Size = new System.Drawing.Size (75, 23);
      this.SaveButton.TabIndex = 18;
      this.SaveButton.Text = "Save";
      this.SaveButton.UseVisualStyleBackColor = true;
      this.SaveButton.Click += new System.EventHandler (this.SaveButton_Click);
      // 
      // ModelName
      // 
      this.ModelName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ModelName.Location = new System.Drawing.Point (118, 16);
      this.ModelName.Name = "ModelName";
      this.ModelName.ReadOnly = true;
      this.ModelName.Size = new System.Drawing.Size (320, 22);
      this.ModelName.TabIndex = 19;
      // 
      // GroundTruthLabel
      // 
      this.GroundTruthLabel.AutoSize = true;
      this.GroundTruthLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.GroundTruthLabel.Location = new System.Drawing.Point (468, 19);
      this.GroundTruthLabel.Name = "GroundTruthLabel";
      this.GroundTruthLabel.Size = new System.Drawing.Size (85, 16);
      this.GroundTruthLabel.TabIndex = 20;
      this.GroundTruthLabel.Text = "Ground Truth";
      this.GroundTruthLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // GroundTruth
      // 
      this.GroundTruth.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.GroundTruth.Location = new System.Drawing.Point (555, 16);
      this.GroundTruth.Name = "GroundTruth";
      this.GroundTruth.ReadOnly = true;
      this.GroundTruth.Size = new System.Drawing.Size (356, 22);
      this.GroundTruth.TabIndex = 21;
      // 
      // CorrectNonNoise
      // 
      this.CorrectNonNoise.Location = new System.Drawing.Point (486, 59);
      this.CorrectNonNoise.Name = "CorrectNonNoise";
      this.CorrectNonNoise.Size = new System.Drawing.Size (86, 20);
      this.CorrectNonNoise.TabIndex = 23;
      this.CorrectNonNoise.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // AccuracyPartOfClassifier
      // 
      this.AccuracyPartOfClassifier.Location = new System.Drawing.Point (226, 89);
      this.AccuracyPartOfClassifier.Name = "AccuracyPartOfClassifier";
      this.AccuracyPartOfClassifier.Size = new System.Drawing.Size (86, 20);
      this.AccuracyPartOfClassifier.TabIndex = 25;
      this.AccuracyPartOfClassifier.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // TotalNonNoise2
      // 
      this.TotalNonNoise2.Location = new System.Drawing.Point (486, 29);
      this.TotalNonNoise2.Name = "TotalNonNoise2";
      this.TotalNonNoise2.Size = new System.Drawing.Size (86, 20);
      this.TotalNonNoise2.TabIndex = 27;
      this.TotalNonNoise2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // NonNoiseLabel
      // 
      this.NonNoiseLabel.AutoSize = true;
      this.NonNoiseLabel.Location = new System.Drawing.Point (499, 10);
      this.NonNoiseLabel.Name = "NonNoiseLabel";
      this.NonNoiseLabel.Size = new System.Drawing.Size (57, 13);
      this.NonNoiseLabel.TabIndex = 26;
      this.NonNoiseLabel.Text = "Non Noise";
      // 
      // PartOfClassfierLabel
      // 
      this.PartOfClassfierLabel.AutoSize = true;
      this.PartOfClassfierLabel.Location = new System.Drawing.Point (228, 10);
      this.PartOfClassfierLabel.Name = "PartOfClassfierLabel";
      this.PartOfClassfierLabel.Size = new System.Drawing.Size (82, 13);
      this.PartOfClassfierLabel.TabIndex = 28;
      this.PartOfClassfierLabel.Text = "Part of Classifier";
      // 
      // TotalPartOfClassifier
      // 
      this.TotalPartOfClassifier.Location = new System.Drawing.Point (226, 29);
      this.TotalPartOfClassifier.Name = "TotalPartOfClassifier";
      this.TotalPartOfClassifier.Size = new System.Drawing.Size (86, 20);
      this.TotalPartOfClassifier.TabIndex = 29;
      this.TotalPartOfClassifier.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // TotalLabel
      // 
      this.TotalLabel.AutoSize = true;
      this.TotalLabel.Location = new System.Drawing.Point (4, 33);
      this.TotalLabel.Name = "TotalLabel";
      this.TotalLabel.Size = new System.Drawing.Size (31, 13);
      this.TotalLabel.TabIndex = 30;
      this.TotalLabel.Text = "Total";
      // 
      // CorrectPartOfClassifier
      // 
      this.CorrectPartOfClassifier.Location = new System.Drawing.Point (226, 59);
      this.CorrectPartOfClassifier.Name = "CorrectPartOfClassifier";
      this.CorrectPartOfClassifier.Size = new System.Drawing.Size (86, 20);
      this.CorrectPartOfClassifier.TabIndex = 31;
      this.CorrectPartOfClassifier.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // AccuracyNonNoise
      // 
      this.AccuracyNonNoise.Location = new System.Drawing.Point (486, 89);
      this.AccuracyNonNoise.Name = "AccuracyNonNoise";
      this.AccuracyNonNoise.Size = new System.Drawing.Size (86, 20);
      this.AccuracyNonNoise.TabIndex = 32;
      this.AccuracyNonNoise.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // Summary
      // 
      this.Summary.Controls.Add (this.AccuracyNonNoise_NPC);
      this.Summary.Controls.Add (this.AccuracyNoise2_NPC);
      this.Summary.Controls.Add (this.AccuracyPartOfClassifier_NPC);
      this.Summary.Controls.Add (this.AccuracyAll_NPC);
      this.Summary.Controls.Add (this.AcuracyNPCLabel);
      this.Summary.Controls.Add (this.NoiseLabel);
      this.Summary.Controls.Add (this.AccuracyNoise2);
      this.Summary.Controls.Add (this.TotalNoise2);
      this.Summary.Controls.Add (this.CorrectNoise2);
      this.Summary.Controls.Add (this.TotalLabel);
      this.Summary.Controls.Add (this.NonNoiseLabel);
      this.Summary.Controls.Add (this.PartOfClassfierLabel);
      this.Summary.Controls.Add (this.AccuracyNonNoise);
      this.Summary.Controls.Add (this.TotalCorrectLabel);
      this.Summary.Controls.Add (this.CorrectPartOfClassifier);
      this.Summary.Controls.Add (this.TotalNonNoise2);
      this.Summary.Controls.Add (this.CorrectNonNoise);
      this.Summary.Controls.Add (this.AccuracyLabel);
      this.Summary.Controls.Add (this.TotalPartOfClassifier);
      this.Summary.Controls.Add (this.AccuracyPartOfClassifier);
      this.Summary.Controls.Add (this.AllExamplesLabel);
      this.Summary.Controls.Add (this.TotalAll);
      this.Summary.Controls.Add (this.CorrectAll);
      this.Summary.Controls.Add (this.AccuracyAll);
      this.Summary.Location = new System.Drawing.Point (20, 575);
      this.Summary.Name = "Summary";
      this.Summary.Size = new System.Drawing.Size (580, 148);
      this.Summary.TabIndex = 33;
      // 
      // NoiseLabel
      // 
      this.NoiseLabel.AutoSize = true;
      this.NoiseLabel.Location = new System.Drawing.Point (384, 9);
      this.NoiseLabel.Name = "NoiseLabel";
      this.NoiseLabel.Size = new System.Drawing.Size (34, 13);
      this.NoiseLabel.TabIndex = 34;
      this.NoiseLabel.Text = "Noise";
      // 
      // AccuracyNoise2
      // 
      this.AccuracyNoise2.Location = new System.Drawing.Point (356, 89);
      this.AccuracyNoise2.Name = "AccuracyNoise2";
      this.AccuracyNoise2.Size = new System.Drawing.Size (86, 20);
      this.AccuracyNoise2.TabIndex = 36;
      this.AccuracyNoise2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // TotalNoise2
      // 
      this.TotalNoise2.Location = new System.Drawing.Point (356, 29);
      this.TotalNoise2.Name = "TotalNoise2";
      this.TotalNoise2.Size = new System.Drawing.Size (86, 20);
      this.TotalNoise2.TabIndex = 35;
      this.TotalNoise2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // CorrectNoise2
      // 
      this.CorrectNoise2.Location = new System.Drawing.Point (356, 59);
      this.CorrectNoise2.Name = "CorrectNoise2";
      this.CorrectNoise2.Size = new System.Drawing.Size (86, 20);
      this.CorrectNoise2.TabIndex = 33;
      this.CorrectNoise2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // AcuracyNPCLabel
      // 
      this.AcuracyNPCLabel.AutoSize = true;
      this.AcuracyNPCLabel.Location = new System.Drawing.Point (4, 123);
      this.AcuracyNPCLabel.Name = "AcuracyNPCLabel";
      this.AcuracyNPCLabel.Size = new System.Drawing.Size (77, 13);
      this.AcuracyNPCLabel.TabIndex = 37;
      this.AcuracyNPCLabel.Text = "Accuracy NPC";
      // 
      // AccuracyAll_NPC
      // 
      this.AccuracyAll_NPC.Location = new System.Drawing.Point (96, 119);
      this.AccuracyAll_NPC.Name = "AccuracyAll_NPC";
      this.AccuracyAll_NPC.Size = new System.Drawing.Size (86, 20);
      this.AccuracyAll_NPC.TabIndex = 38;
      this.AccuracyAll_NPC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // AccuracyPartOfClassifier_NPC
      // 
      this.AccuracyPartOfClassifier_NPC.Location = new System.Drawing.Point (226, 119);
      this.AccuracyPartOfClassifier_NPC.Name = "AccuracyPartOfClassifier_NPC";
      this.AccuracyPartOfClassifier_NPC.Size = new System.Drawing.Size (86, 20);
      this.AccuracyPartOfClassifier_NPC.TabIndex = 39;
      this.AccuracyPartOfClassifier_NPC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // AccuracyNoise2_NPC
      // 
      this.AccuracyNoise2_NPC.Location = new System.Drawing.Point (356, 119);
      this.AccuracyNoise2_NPC.Name = "AccuracyNoise2_NPC";
      this.AccuracyNoise2_NPC.Size = new System.Drawing.Size (86, 20);
      this.AccuracyNoise2_NPC.TabIndex = 40;
      this.AccuracyNoise2_NPC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // AccuracyNonNoise_NPC
      // 
      this.AccuracyNonNoise_NPC.Location = new System.Drawing.Point (486, 119);
      this.AccuracyNonNoise_NPC.Name = "AccuracyNonNoise_NPC";
      this.AccuracyNonNoise_NPC.Size = new System.Drawing.Size (86, 20);
      this.AccuracyNonNoise_NPC.TabIndex = 41;
      this.AccuracyNonNoise_NPC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // ConfusionMatrix
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (939, 728);
      this.Controls.Add (this.Summary);
      this.Controls.Add (this.GroundTruth);
      this.Controls.Add (this.GroundTruthLabel);
      this.Controls.Add (this.ModelName);
      this.Controls.Add (this.SaveButton);
      this.Controls.Add (this.PredictionLevel);
      this.Controls.Add (this.PredictionLevelLabel);
      this.Controls.Add (this.TrainingLevel);
      this.Controls.Add (this.TrainLevelLabel);
      this.Controls.Add (this.ReTallyButton);
      this.Controls.Add (this.MaxProbabilityLabel);
      this.Controls.Add (this.MaxProbability);
      this.Controls.Add (this.MinProbabilityLabel);
      this.Controls.Add (this.MinProbability);
      this.Controls.Add (this.PercentagesButton);
      this.Controls.Add (this.Matrix);
      this.Controls.Add (this.NameLabel);
      this.Name = "ConfusionMatrix";
      this.Text = "ConfusionMatrix";
      this.Load += new System.EventHandler (this.ConfusionMatrix_Load);
      this.SizeChanged += new System.EventHandler (this.ConfusionMatrix_SizeChanged);
      this.Summary.ResumeLayout (false);
      this.Summary.PerformLayout ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label NameLabel;
    private System.Windows.Forms.ListView Matrix;
    private System.Windows.Forms.ColumnHeader ClassName;
    private System.Windows.Forms.ColumnHeader Count;
    private System.Windows.Forms.ColumnHeader Class1;
    private System.Windows.Forms.ColumnHeader Class2;
    private System.Windows.Forms.Button PercentagesButton;
    private System.Windows.Forms.ComboBox MinProbability;
    private System.Windows.Forms.Label MinProbabilityLabel;
    private System.Windows.Forms.ComboBox MaxProbability;
    private System.Windows.Forms.Label MaxProbabilityLabel;
    private System.Windows.Forms.Button ReTallyButton;
    private System.Windows.Forms.Label AllExamplesLabel;
    private System.Windows.Forms.TextBox TotalAll;
    private System.Windows.Forms.Label TotalCorrectLabel;
    private System.Windows.Forms.TextBox CorrectAll;
    private System.Windows.Forms.Label AccuracyLabel;
    private System.Windows.Forms.TextBox AccuracyAll;
    private System.Windows.Forms.Label TrainLevelLabel;
    private System.Windows.Forms.TextBox TrainingLevel;
    private System.Windows.Forms.Label PredictionLevelLabel;
    private System.Windows.Forms.ComboBox PredictionLevel;
    private System.Windows.Forms.Button SaveButton;
    private System.Windows.Forms.SaveFileDialog saveFileDialog1;
    private System.Windows.Forms.TextBox ModelName;
    private System.Windows.Forms.Label GroundTruthLabel;
    private System.Windows.Forms.TextBox GroundTruth;
    private System.Windows.Forms.TextBox CorrectNonNoise;
    private System.Windows.Forms.TextBox AccuracyPartOfClassifier;
    private System.Windows.Forms.TextBox TotalNonNoise2;
    private System.Windows.Forms.Label NonNoiseLabel;
    private System.Windows.Forms.Label PartOfClassfierLabel;
    private System.Windows.Forms.TextBox TotalPartOfClassifier;
    private System.Windows.Forms.Label TotalLabel;
    private System.Windows.Forms.TextBox CorrectPartOfClassifier;
    private System.Windows.Forms.TextBox AccuracyNonNoise;
    private System.Windows.Forms.Panel Summary;
    private System.Windows.Forms.Label NoiseLabel;
    private System.Windows.Forms.TextBox AccuracyNoise2;
    private System.Windows.Forms.TextBox TotalNoise2;
    private System.Windows.Forms.TextBox CorrectNoise2;
    private System.Windows.Forms.TextBox AccuracyNonNoise_NPC;
    private System.Windows.Forms.TextBox AccuracyNoise2_NPC;
    private System.Windows.Forms.TextBox AccuracyPartOfClassifier_NPC;
    private System.Windows.Forms.TextBox AccuracyAll_NPC;
    private System.Windows.Forms.Label AcuracyNPCLabel;
  }
}