namespace SipperFile
{
  partial class ImageViewer
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
      this.AddToTrainingLibrary = new System.Windows.Forms.Button ();
      this.TrainingLibrary1Label = new System.Windows.Forms.Label ();
      this.TrainingLibrary1 = new System.Windows.Forms.TextBox ();
      this.TrainingLibrary2 = new System.Windows.Forms.TextBox ();
      this.TrainingLibrary2Label = new System.Windows.Forms.Label ();
      this.Lib1Pred1Label = new System.Windows.Forms.Label ();
      this.Lib1Pred1Class = new System.Windows.Forms.TextBox ();
      this.Lib1Pred1Prob = new System.Windows.Forms.TextBox ();
      this.Lib1Pred2Prob = new System.Windows.Forms.TextBox ();
      this.Lib1Pred2Class = new System.Windows.Forms.TextBox ();
      this.Lib1Pred2Label = new System.Windows.Forms.Label ();
      this.Lib2Pred2Prob = new System.Windows.Forms.TextBox ();
      this.Lib2Pred2Class = new System.Windows.Forms.TextBox ();
      this.Lib2Pred2Label = new System.Windows.Forms.Label ();
      this.Lib2Pred1Prob = new System.Windows.Forms.TextBox ();
      this.Lib2Pred1Class = new System.Windows.Forms.TextBox ();
      this.Lib2Pred1Label = new System.Windows.Forms.Label ();
      this.ZoomLabel = new System.Windows.Forms.Label ();
      this.ZoomFactor = new System.Windows.Forms.ComboBox ();
      this.RightPanel = new System.Windows.Forms.Panel ();
      this.EditButton = new System.Windows.Forms.Button ();
      this.ViewClassLogButton = new System.Windows.Forms.Button ();
      this.DateClass = new System.Windows.Forms.TextBox ();
      this.DateClassLabel = new System.Windows.Forms.Label ();
      this.DateExtracted = new System.Windows.Forms.TextBox ();
      this.DateExtractedLabel = new System.Windows.Forms.Label ();
      this.ConnectedComponentDist = new System.Windows.Forms.TextBox ();
      this.ConnectedComponentDistanceLabel = new System.Windows.Forms.Label ();
      this.ViewExtractionLogButton = new System.Windows.Forms.Button ();
      this.SizeCoordinatesSelEnabled = new System.Windows.Forms.Button ();
      this.ImageSizeAuto = new System.Windows.Forms.Button ();
      this.ImageSizeClearButton = new System.Windows.Forms.Button ();
      this.ImageSize = new System.Windows.Forms.TextBox ();
      this.ImageSizeLabel = new System.Windows.Forms.Label ();
      this.SelectClassButtom = new System.Windows.Forms.Button ();
      this.Lib2Pred2Votes = new System.Windows.Forms.TextBox ();
      this.Lib2Pred1Votes = new System.Windows.Forms.TextBox ();
      this.Lib1Pred2Votes = new System.Windows.Forms.TextBox ();
      this.Lib1Pred1Votes = new System.Windows.Forms.TextBox ();
      this.DisplayGridButton = new System.Windows.Forms.Button ();
      this.DataFields = new System.Windows.Forms.TableLayoutPanel ();
      this.DataLabel3 = new System.Windows.Forms.Label ();
      this.DataField1 = new System.Windows.Forms.TextBox ();
      this.DataField2 = new System.Windows.Forms.TextBox ();
      this.DataField0 = new System.Windows.Forms.TextBox ();
      this.DataLabel0 = new System.Windows.Forms.Label ();
      this.DataLabel1 = new System.Windows.Forms.Label ();
      this.DataLabel2 = new System.Windows.Forms.Label ();
      this.DataField3 = new System.Windows.Forms.TextBox ();
      this.BreakDownTrainingLibrary2Button = new System.Windows.Forms.Button ();
      this.BreakDownTrainingLibrary1Button = new System.Windows.Forms.Button ();
      this.ImageFileName = new System.Windows.Forms.TextBox ();
      this.ValidatedClassName = new System.Windows.Forms.TextBox ();
      this.ValidatedClassLabel = new System.Windows.Forms.Label ();
      this.LeftPanel = new System.Windows.Forms.Panel ();
      this.pictureBox1 = new System.Windows.Forms.PictureBox ();
      this.PlanktonImage = new System.Windows.Forms.PictureBox ();
      this.RightPanel.SuspendLayout ();
      this.DataFields.SuspendLayout ();
      this.LeftPanel.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit ();
      ((System.ComponentModel.ISupportInitialize)(this.PlanktonImage)).BeginInit ();
      this.SuspendLayout ();
      // 
      // AddToTrainingLibrary
      // 
      this.AddToTrainingLibrary.Location = new System.Drawing.Point (426, 265);
      this.AddToTrainingLibrary.Name = "AddToTrainingLibrary";
      this.AddToTrainingLibrary.Size = new System.Drawing.Size (37, 25);
      this.AddToTrainingLibrary.TabIndex = 25;
      this.AddToTrainingLibrary.TabStop = false;
      this.AddToTrainingLibrary.Text = "Add";
      this.AddToTrainingLibrary.UseVisualStyleBackColor = true;
      this.AddToTrainingLibrary.Click += new System.EventHandler (this.AddToTrainingLibrary_Click);
      // 
      // TrainingLibrary1Label
      // 
      this.TrainingLibrary1Label.AutoSize = true;
      this.TrainingLibrary1Label.Location = new System.Drawing.Point (7, 59);
      this.TrainingLibrary1Label.Name = "TrainingLibrary1Label";
      this.TrainingLibrary1Label.Size = new System.Drawing.Size (62, 13);
      this.TrainingLibrary1Label.TabIndex = 3;
      this.TrainingLibrary1Label.Text = "TrainLibrary";
      // 
      // TrainingLibrary1
      // 
      this.TrainingLibrary1.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TrainingLibrary1.Location = new System.Drawing.Point (76, 56);
      this.TrainingLibrary1.Name = "TrainingLibrary1";
      this.TrainingLibrary1.ReadOnly = true;
      this.TrainingLibrary1.Size = new System.Drawing.Size (274, 22);
      this.TrainingLibrary1.TabIndex = 4;
      this.TrainingLibrary1.TabStop = false;
      // 
      // TrainingLibrary2
      // 
      this.TrainingLibrary2.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TrainingLibrary2.Location = new System.Drawing.Point (76, 165);
      this.TrainingLibrary2.Name = "TrainingLibrary2";
      this.TrainingLibrary2.ReadOnly = true;
      this.TrainingLibrary2.Size = new System.Drawing.Size (275, 22);
      this.TrainingLibrary2.TabIndex = 6;
      this.TrainingLibrary2.TabStop = false;
      // 
      // TrainingLibrary2Label
      // 
      this.TrainingLibrary2Label.AutoSize = true;
      this.TrainingLibrary2Label.Location = new System.Drawing.Point (7, 170);
      this.TrainingLibrary2Label.Name = "TrainingLibrary2Label";
      this.TrainingLibrary2Label.Size = new System.Drawing.Size (65, 13);
      this.TrainingLibrary2Label.TabIndex = 5;
      this.TrainingLibrary2Label.Text = "Train Library";
      // 
      // Lib1Pred1Label
      // 
      this.Lib1Pred1Label.AutoSize = true;
      this.Lib1Pred1Label.Location = new System.Drawing.Point (7, 91);
      this.Lib1Pred1Label.Name = "Lib1Pred1Label";
      this.Lib1Pred1Label.Size = new System.Drawing.Size (63, 13);
      this.Lib1Pred1Label.TabIndex = 7;
      this.Lib1Pred1Label.Text = "Prediction 1";
      // 
      // Lib1Pred1Class
      // 
      this.Lib1Pred1Class.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib1Pred1Class.Location = new System.Drawing.Point (76, 88);
      this.Lib1Pred1Class.Name = "Lib1Pred1Class";
      this.Lib1Pred1Class.ReadOnly = true;
      this.Lib1Pred1Class.Size = new System.Drawing.Size (275, 22);
      this.Lib1Pred1Class.TabIndex = 8;
      this.Lib1Pred1Class.TabStop = false;
      this.Lib1Pred1Class.TextChanged += new System.EventHandler (this.Lib1Pred1Class_TextChanged);
      // 
      // Lib1Pred1Prob
      // 
      this.Lib1Pred1Prob.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib1Pred1Prob.Location = new System.Drawing.Point (357, 88);
      this.Lib1Pred1Prob.Name = "Lib1Pred1Prob";
      this.Lib1Pred1Prob.ReadOnly = true;
      this.Lib1Pred1Prob.Size = new System.Drawing.Size (60, 22);
      this.Lib1Pred1Prob.TabIndex = 9;
      this.Lib1Pred1Prob.TabStop = false;
      this.Lib1Pred1Prob.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // Lib1Pred2Prob
      // 
      this.Lib1Pred2Prob.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib1Pred2Prob.Location = new System.Drawing.Point (357, 114);
      this.Lib1Pred2Prob.Name = "Lib1Pred2Prob";
      this.Lib1Pred2Prob.ReadOnly = true;
      this.Lib1Pred2Prob.Size = new System.Drawing.Size (60, 22);
      this.Lib1Pred2Prob.TabIndex = 12;
      this.Lib1Pred2Prob.TabStop = false;
      this.Lib1Pred2Prob.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // Lib1Pred2Class
      // 
      this.Lib1Pred2Class.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib1Pred2Class.Location = new System.Drawing.Point (76, 114);
      this.Lib1Pred2Class.Name = "Lib1Pred2Class";
      this.Lib1Pred2Class.ReadOnly = true;
      this.Lib1Pred2Class.Size = new System.Drawing.Size (275, 22);
      this.Lib1Pred2Class.TabIndex = 11;
      this.Lib1Pred2Class.TabStop = false;
      this.Lib1Pred2Class.TextChanged += new System.EventHandler (this.Lib1Pred2Class_TextChanged);
      // 
      // Lib1Pred2Label
      // 
      this.Lib1Pred2Label.AutoSize = true;
      this.Lib1Pred2Label.Location = new System.Drawing.Point (7, 117);
      this.Lib1Pred2Label.Name = "Lib1Pred2Label";
      this.Lib1Pred2Label.Size = new System.Drawing.Size (63, 13);
      this.Lib1Pred2Label.TabIndex = 10;
      this.Lib1Pred2Label.Text = "Prediction 2";
      // 
      // Lib2Pred2Prob
      // 
      this.Lib2Pred2Prob.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib2Pred2Prob.Location = new System.Drawing.Point (357, 225);
      this.Lib2Pred2Prob.Name = "Lib2Pred2Prob";
      this.Lib2Pred2Prob.ReadOnly = true;
      this.Lib2Pred2Prob.Size = new System.Drawing.Size (60, 22);
      this.Lib2Pred2Prob.TabIndex = 18;
      this.Lib2Pred2Prob.TabStop = false;
      this.Lib2Pred2Prob.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // Lib2Pred2Class
      // 
      this.Lib2Pred2Class.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib2Pred2Class.Location = new System.Drawing.Point (76, 225);
      this.Lib2Pred2Class.Name = "Lib2Pred2Class";
      this.Lib2Pred2Class.ReadOnly = true;
      this.Lib2Pred2Class.Size = new System.Drawing.Size (274, 22);
      this.Lib2Pred2Class.TabIndex = 17;
      this.Lib2Pred2Class.TabStop = false;
      this.Lib2Pred2Class.TextChanged += new System.EventHandler (this.Lib2Pred2Class_TextChanged);
      // 
      // Lib2Pred2Label
      // 
      this.Lib2Pred2Label.AutoSize = true;
      this.Lib2Pred2Label.Location = new System.Drawing.Point (7, 228);
      this.Lib2Pred2Label.Name = "Lib2Pred2Label";
      this.Lib2Pred2Label.Size = new System.Drawing.Size (63, 13);
      this.Lib2Pred2Label.TabIndex = 16;
      this.Lib2Pred2Label.Text = "Prediction 2";
      // 
      // Lib2Pred1Prob
      // 
      this.Lib2Pred1Prob.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib2Pred1Prob.Location = new System.Drawing.Point (357, 199);
      this.Lib2Pred1Prob.Name = "Lib2Pred1Prob";
      this.Lib2Pred1Prob.ReadOnly = true;
      this.Lib2Pred1Prob.Size = new System.Drawing.Size (60, 22);
      this.Lib2Pred1Prob.TabIndex = 15;
      this.Lib2Pred1Prob.TabStop = false;
      this.Lib2Pred1Prob.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // Lib2Pred1Class
      // 
      this.Lib2Pred1Class.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib2Pred1Class.Location = new System.Drawing.Point (76, 199);
      this.Lib2Pred1Class.Name = "Lib2Pred1Class";
      this.Lib2Pred1Class.ReadOnly = true;
      this.Lib2Pred1Class.Size = new System.Drawing.Size (275, 22);
      this.Lib2Pred1Class.TabIndex = 14;
      this.Lib2Pred1Class.TabStop = false;
      this.Lib2Pred1Class.TextChanged += new System.EventHandler (this.Lib2Pred1Class_TextChanged);
      // 
      // Lib2Pred1Label
      // 
      this.Lib2Pred1Label.AutoSize = true;
      this.Lib2Pred1Label.Location = new System.Drawing.Point (7, 202);
      this.Lib2Pred1Label.Name = "Lib2Pred1Label";
      this.Lib2Pred1Label.Size = new System.Drawing.Size (63, 13);
      this.Lib2Pred1Label.TabIndex = 13;
      this.Lib2Pred1Label.Text = "Prediction 1";
      // 
      // ZoomLabel
      // 
      this.ZoomLabel.AutoSize = true;
      this.ZoomLabel.Location = new System.Drawing.Point (7, 9);
      this.ZoomLabel.Name = "ZoomLabel";
      this.ZoomLabel.Size = new System.Drawing.Size (34, 13);
      this.ZoomLabel.TabIndex = 20;
      this.ZoomLabel.Text = "Zoom";
      // 
      // ZoomFactor
      // 
      this.ZoomFactor.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ZoomFactor.FormattingEnabled = true;
      this.ZoomFactor.Items.AddRange (new object[] {
            "10%",
            "25%",
            "50%",
            "100%",
            "200%",
            "400%"});
      this.ZoomFactor.Location = new System.Drawing.Point (74, 6);
      this.ZoomFactor.Name = "ZoomFactor";
      this.ZoomFactor.Size = new System.Drawing.Size (79, 24);
      this.ZoomFactor.TabIndex = 21;
      this.ZoomFactor.Text = "100%";
      this.ZoomFactor.SelectedIndexChanged += new System.EventHandler (this.ZoomFactor_SelectedIndexChanged);
      this.ZoomFactor.Leave += new System.EventHandler (this.ZoomFactor_Leave);
      // 
      // RightPanel
      // 
      this.RightPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.RightPanel.Controls.Add (this.EditButton);
      this.RightPanel.Controls.Add (this.ViewClassLogButton);
      this.RightPanel.Controls.Add (this.DateClass);
      this.RightPanel.Controls.Add (this.DateClassLabel);
      this.RightPanel.Controls.Add (this.DateExtracted);
      this.RightPanel.Controls.Add (this.DateExtractedLabel);
      this.RightPanel.Controls.Add (this.ConnectedComponentDist);
      this.RightPanel.Controls.Add (this.ConnectedComponentDistanceLabel);
      this.RightPanel.Controls.Add (this.ViewExtractionLogButton);
      this.RightPanel.Controls.Add (this.SizeCoordinatesSelEnabled);
      this.RightPanel.Controls.Add (this.ImageSizeAuto);
      this.RightPanel.Controls.Add (this.ImageSizeClearButton);
      this.RightPanel.Controls.Add (this.ImageSize);
      this.RightPanel.Controls.Add (this.ImageSizeLabel);
      this.RightPanel.Controls.Add (this.SelectClassButtom);
      this.RightPanel.Controls.Add (this.Lib2Pred2Votes);
      this.RightPanel.Controls.Add (this.Lib2Pred1Votes);
      this.RightPanel.Controls.Add (this.Lib1Pred2Votes);
      this.RightPanel.Controls.Add (this.Lib1Pred1Votes);
      this.RightPanel.Controls.Add (this.DisplayGridButton);
      this.RightPanel.Controls.Add (this.DataFields);
      this.RightPanel.Controls.Add (this.BreakDownTrainingLibrary2Button);
      this.RightPanel.Controls.Add (this.BreakDownTrainingLibrary1Button);
      this.RightPanel.Controls.Add (this.ImageFileName);
      this.RightPanel.Controls.Add (this.ValidatedClassName);
      this.RightPanel.Controls.Add (this.ValidatedClassLabel);
      this.RightPanel.Controls.Add (this.ZoomFactor);
      this.RightPanel.Controls.Add (this.AddToTrainingLibrary);
      this.RightPanel.Controls.Add (this.ZoomLabel);
      this.RightPanel.Controls.Add (this.TrainingLibrary1Label);
      this.RightPanel.Controls.Add (this.TrainingLibrary1);
      this.RightPanel.Controls.Add (this.Lib2Pred2Prob);
      this.RightPanel.Controls.Add (this.TrainingLibrary2Label);
      this.RightPanel.Controls.Add (this.Lib2Pred2Class);
      this.RightPanel.Controls.Add (this.TrainingLibrary2);
      this.RightPanel.Controls.Add (this.Lib2Pred2Label);
      this.RightPanel.Controls.Add (this.Lib1Pred1Label);
      this.RightPanel.Controls.Add (this.Lib2Pred1Prob);
      this.RightPanel.Controls.Add (this.Lib1Pred1Class);
      this.RightPanel.Controls.Add (this.Lib2Pred1Class);
      this.RightPanel.Controls.Add (this.Lib1Pred1Prob);
      this.RightPanel.Controls.Add (this.Lib2Pred1Label);
      this.RightPanel.Controls.Add (this.Lib1Pred2Label);
      this.RightPanel.Controls.Add (this.Lib1Pred2Prob);
      this.RightPanel.Controls.Add (this.Lib1Pred2Class);
      this.RightPanel.Location = new System.Drawing.Point (536, 12);
      this.RightPanel.Name = "RightPanel";
      this.RightPanel.Size = new System.Drawing.Size (477, 500);
      this.RightPanel.TabIndex = 22;
      this.RightPanel.SizeChanged += new System.EventHandler (this.RightPanel_SizeChanged);
      // 
      // EditButton
      // 
      this.EditButton.Location = new System.Drawing.Point (413, 380);
      this.EditButton.Name = "EditButton";
      this.EditButton.Size = new System.Drawing.Size (51, 23);
      this.EditButton.TabIndex = 50;
      this.EditButton.TabStop = false;
      this.EditButton.Text = "Edit";
      this.EditButton.UseVisualStyleBackColor = true;
      this.EditButton.Click += new System.EventHandler (this.EditButton_Click);
      // 
      // ViewClassLogButton
      // 
      this.ViewClassLogButton.Location = new System.Drawing.Point (271, 385);
      this.ViewClassLogButton.Name = "ViewClassLogButton";
      this.ViewClassLogButton.Size = new System.Drawing.Size (35, 23);
      this.ViewClassLogButton.TabIndex = 47;
      this.ViewClassLogButton.TabStop = false;
      this.ViewClassLogButton.Text = "Log";
      this.ViewClassLogButton.UseVisualStyleBackColor = true;
      this.ViewClassLogButton.Click += new System.EventHandler (this.ViewClassLogButton_Click);
      // 
      // DateClass
      // 
      this.DateClass.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateClass.Location = new System.Drawing.Point (92, 386);
      this.DateClass.Name = "DateClass";
      this.DateClass.ReadOnly = true;
      this.DateClass.Size = new System.Drawing.Size (170, 22);
      this.DateClass.TabIndex = 46;
      this.DateClass.TabStop = false;
      this.DateClass.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // DateClassLabel
      // 
      this.DateClassLabel.AutoSize = true;
      this.DateClassLabel.Location = new System.Drawing.Point (7, 390);
      this.DateClassLabel.Name = "DateClassLabel";
      this.DateClassLabel.Size = new System.Drawing.Size (58, 13);
      this.DateClassLabel.TabIndex = 45;
      this.DateClassLabel.Text = "Date Class";
      // 
      // DateExtracted
      // 
      this.DateExtracted.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateExtracted.Location = new System.Drawing.Point (92, 355);
      this.DateExtracted.Name = "DateExtracted";
      this.DateExtracted.ReadOnly = true;
      this.DateExtracted.Size = new System.Drawing.Size (170, 22);
      this.DateExtracted.TabIndex = 44;
      this.DateExtracted.TabStop = false;
      this.DateExtracted.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // DateExtractedLabel
      // 
      this.DateExtractedLabel.AutoSize = true;
      this.DateExtractedLabel.Location = new System.Drawing.Point (7, 359);
      this.DateExtractedLabel.Name = "DateExtractedLabel";
      this.DateExtractedLabel.Size = new System.Drawing.Size (78, 13);
      this.DateExtractedLabel.TabIndex = 43;
      this.DateExtractedLabel.Text = "Date Extracted";
      // 
      // ConnectedComponentDist
      // 
      this.ConnectedComponentDist.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ConnectedComponentDist.Location = new System.Drawing.Point (418, 317);
      this.ConnectedComponentDist.Name = "ConnectedComponentDist";
      this.ConnectedComponentDist.ReadOnly = true;
      this.ConnectedComponentDist.Size = new System.Drawing.Size (46, 22);
      this.ConnectedComponentDist.TabIndex = 42;
      this.ConnectedComponentDist.TabStop = false;
      this.ConnectedComponentDist.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // ConnectedComponentDistanceLabel
      // 
      this.ConnectedComponentDistanceLabel.AutoSize = true;
      this.ConnectedComponentDistanceLabel.Location = new System.Drawing.Point (381, 321);
      this.ConnectedComponentDistanceLabel.Name = "ConnectedComponentDistanceLabel";
      this.ConnectedComponentDistanceLabel.Size = new System.Drawing.Size (29, 13);
      this.ConnectedComponentDistanceLabel.TabIndex = 41;
      this.ConnectedComponentDistanceLabel.Text = "CCD";
      // 
      // ViewExtractionLogButton
      // 
      this.ViewExtractionLogButton.Location = new System.Drawing.Point (271, 354);
      this.ViewExtractionLogButton.Name = "ViewExtractionLogButton";
      this.ViewExtractionLogButton.Size = new System.Drawing.Size (35, 23);
      this.ViewExtractionLogButton.TabIndex = 45;
      this.ViewExtractionLogButton.TabStop = false;
      this.ViewExtractionLogButton.Text = "Log";
      this.ViewExtractionLogButton.UseVisualStyleBackColor = true;
      this.ViewExtractionLogButton.Click += new System.EventHandler (this.ViewExtractionLogButton_Click);
      // 
      // SizeCoordinatesSelEnabled
      // 
      this.SizeCoordinatesSelEnabled.Location = new System.Drawing.Point (208, 316);
      this.SizeCoordinatesSelEnabled.Name = "SizeCoordinatesSelEnabled";
      this.SizeCoordinatesSelEnabled.Size = new System.Drawing.Size (51, 23);
      this.SizeCoordinatesSelEnabled.TabIndex = 39;
      this.SizeCoordinatesSelEnabled.TabStop = false;
      this.SizeCoordinatesSelEnabled.Text = "Enabled";
      this.SizeCoordinatesSelEnabled.UseVisualStyleBackColor = true;
      this.SizeCoordinatesSelEnabled.Click += new System.EventHandler (this.SizeCoordinatesSelEnabled_Click);
      // 
      // ImageSizeAuto
      // 
      this.ImageSizeAuto.Location = new System.Drawing.Point (271, 316);
      this.ImageSizeAuto.Name = "ImageSizeAuto";
      this.ImageSizeAuto.Size = new System.Drawing.Size (51, 23);
      this.ImageSizeAuto.TabIndex = 40;
      this.ImageSizeAuto.TabStop = false;
      this.ImageSizeAuto.Text = "Auto";
      this.ImageSizeAuto.UseVisualStyleBackColor = true;
      this.ImageSizeAuto.Click += new System.EventHandler (this.ImageSizeAuto_Click);
      // 
      // ImageSizeClearButton
      // 
      this.ImageSizeClearButton.Location = new System.Drawing.Point (147, 316);
      this.ImageSizeClearButton.Name = "ImageSizeClearButton";
      this.ImageSizeClearButton.Size = new System.Drawing.Size (51, 23);
      this.ImageSizeClearButton.TabIndex = 37;
      this.ImageSizeClearButton.TabStop = false;
      this.ImageSizeClearButton.Text = "Clear";
      this.ImageSizeClearButton.UseVisualStyleBackColor = true;
      this.ImageSizeClearButton.Click += new System.EventHandler (this.ImageSizeClearButton_Click);
      // 
      // ImageSize
      // 
      this.ImageSize.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ImageSize.Location = new System.Drawing.Point (76, 317);
      this.ImageSize.Name = "ImageSize";
      this.ImageSize.ReadOnly = true;
      this.ImageSize.Size = new System.Drawing.Size (60, 22);
      this.ImageSize.TabIndex = 36;
      this.ImageSize.TabStop = false;
      this.ImageSize.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // ImageSizeLabel
      // 
      this.ImageSizeLabel.AutoSize = true;
      this.ImageSizeLabel.Location = new System.Drawing.Point (7, 321);
      this.ImageSizeLabel.Name = "ImageSizeLabel";
      this.ImageSizeLabel.Size = new System.Drawing.Size (59, 13);
      this.ImageSizeLabel.TabIndex = 35;
      this.ImageSizeLabel.Text = "Image Size";
      // 
      // SelectClassButtom
      // 
      this.SelectClassButtom.Location = new System.Drawing.Point (385, 265);
      this.SelectClassButtom.Name = "SelectClassButtom";
      this.SelectClassButtom.Size = new System.Drawing.Size (37, 25);
      this.SelectClassButtom.TabIndex = 24;
      this.SelectClassButtom.TabStop = false;
      this.SelectClassButtom.Text = "Sel";
      this.SelectClassButtom.UseVisualStyleBackColor = true;
      this.SelectClassButtom.Click += new System.EventHandler (this.SelectClassButtom_Click);
      // 
      // Lib2Pred2Votes
      // 
      this.Lib2Pred2Votes.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib2Pred2Votes.Location = new System.Drawing.Point (422, 225);
      this.Lib2Pred2Votes.Name = "Lib2Pred2Votes";
      this.Lib2Pred2Votes.ReadOnly = true;
      this.Lib2Pred2Votes.Size = new System.Drawing.Size (42, 22);
      this.Lib2Pred2Votes.TabIndex = 19;
      this.Lib2Pred2Votes.TabStop = false;
      this.Lib2Pred2Votes.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // Lib2Pred1Votes
      // 
      this.Lib2Pred1Votes.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib2Pred1Votes.Location = new System.Drawing.Point (422, 199);
      this.Lib2Pred1Votes.Name = "Lib2Pred1Votes";
      this.Lib2Pred1Votes.ReadOnly = true;
      this.Lib2Pred1Votes.Size = new System.Drawing.Size (42, 22);
      this.Lib2Pred1Votes.TabIndex = 16;
      this.Lib2Pred1Votes.TabStop = false;
      this.Lib2Pred1Votes.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // Lib1Pred2Votes
      // 
      this.Lib1Pred2Votes.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib1Pred2Votes.Location = new System.Drawing.Point (422, 114);
      this.Lib1Pred2Votes.Name = "Lib1Pred2Votes";
      this.Lib1Pred2Votes.ReadOnly = true;
      this.Lib1Pred2Votes.Size = new System.Drawing.Size (42, 22);
      this.Lib1Pred2Votes.TabIndex = 31;
      this.Lib1Pred2Votes.TabStop = false;
      this.Lib1Pred2Votes.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // Lib1Pred1Votes
      // 
      this.Lib1Pred1Votes.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Lib1Pred1Votes.Location = new System.Drawing.Point (422, 88);
      this.Lib1Pred1Votes.Name = "Lib1Pred1Votes";
      this.Lib1Pred1Votes.ReadOnly = true;
      this.Lib1Pred1Votes.Size = new System.Drawing.Size (42, 22);
      this.Lib1Pred1Votes.TabIndex = 30;
      this.Lib1Pred1Votes.TabStop = false;
      this.Lib1Pred1Votes.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // DisplayGridButton
      // 
      this.DisplayGridButton.Location = new System.Drawing.Point (6, 26);
      this.DisplayGridButton.Name = "DisplayGridButton";
      this.DisplayGridButton.Size = new System.Drawing.Size (35, 23);
      this.DisplayGridButton.TabIndex = 29;
      this.DisplayGridButton.Text = "Grid";
      this.DisplayGridButton.UseVisualStyleBackColor = true;
      this.DisplayGridButton.Click += new System.EventHandler (this.DisplayGridButton_Click);
      // 
      // DataFields
      // 
      this.DataFields.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
      this.DataFields.ColumnCount = 4;
      this.DataFields.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 18F));
      this.DataFields.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 32F));
      this.DataFields.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 18F));
      this.DataFields.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 32F));
      this.DataFields.Controls.Add (this.DataLabel3, 2, 1);
      this.DataFields.Controls.Add (this.DataField1, 1, 1);
      this.DataFields.Controls.Add (this.DataField2, 3, 0);
      this.DataFields.Controls.Add (this.DataField0, 1, 0);
      this.DataFields.Controls.Add (this.DataLabel0, 0, 0);
      this.DataFields.Controls.Add (this.DataLabel1, 0, 1);
      this.DataFields.Controls.Add (this.DataLabel2, 2, 0);
      this.DataFields.Controls.Add (this.DataField3, 3, 1);
      this.DataFields.Location = new System.Drawing.Point (6, 428);
      this.DataFields.Name = "DataFields";
      this.DataFields.Padding = new System.Windows.Forms.Padding (3);
      this.DataFields.RowCount = 2;
      this.DataFields.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 50F));
      this.DataFields.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 50F));
      this.DataFields.Size = new System.Drawing.Size (458, 63);
      this.DataFields.TabIndex = 24;
      // 
      // DataLabel3
      // 
      this.DataLabel3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DataLabel3.AutoSize = true;
      this.DataLabel3.Location = new System.Drawing.Point (232, 32);
      this.DataLabel3.Name = "DataLabel3";
      this.DataLabel3.Size = new System.Drawing.Size (43, 27);
      this.DataLabel3.TabIndex = 130;
      this.DataLabel3.Text = "Oxygen";
      this.DataLabel3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      this.DataLabel3.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.DataLabel_MouseDoubleClick);
      // 
      // DataField1
      // 
      this.DataField1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DataField1.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DataField1.Location = new System.Drawing.Point (88, 35);
      this.DataField1.Name = "DataField1";
      this.DataField1.ReadOnly = true;
      this.DataField1.Size = new System.Drawing.Size (136, 22);
      this.DataField1.TabIndex = 122;
      // 
      // DataField2
      // 
      this.DataField2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DataField2.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DataField2.Location = new System.Drawing.Point (313, 7);
      this.DataField2.Name = "DataField2";
      this.DataField2.ReadOnly = true;
      this.DataField2.Size = new System.Drawing.Size (138, 22);
      this.DataField2.TabIndex = 112;
      // 
      // DataField0
      // 
      this.DataField0.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DataField0.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DataField0.Location = new System.Drawing.Point (88, 7);
      this.DataField0.Name = "DataField0";
      this.DataField0.ReadOnly = true;
      this.DataField0.Size = new System.Drawing.Size (136, 22);
      this.DataField0.TabIndex = 101;
      // 
      // DataLabel0
      // 
      this.DataLabel0.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DataLabel0.AutoSize = true;
      this.DataLabel0.Location = new System.Drawing.Point (7, 4);
      this.DataLabel0.Name = "DataLabel0";
      this.DataLabel0.Size = new System.Drawing.Size (36, 27);
      this.DataLabel0.TabIndex = 100;
      this.DataLabel0.Text = "Depth";
      this.DataLabel0.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      this.DataLabel0.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.DataLabel_MouseDoubleClick);
      // 
      // DataLabel1
      // 
      this.DataLabel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DataLabel1.AutoSize = true;
      this.DataLabel1.Location = new System.Drawing.Point (7, 32);
      this.DataLabel1.Name = "DataLabel1";
      this.DataLabel1.Size = new System.Drawing.Size (55, 27);
      this.DataLabel1.TabIndex = 120;
      this.DataLabel1.Text = "Flow Rate";
      this.DataLabel1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      this.DataLabel1.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.DataLabel_MouseDoubleClick);
      // 
      // DataLabel2
      // 
      this.DataLabel2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DataLabel2.AutoSize = true;
      this.DataLabel2.Location = new System.Drawing.Point (232, 4);
      this.DataLabel2.Name = "DataLabel2";
      this.DataLabel2.Size = new System.Drawing.Size (67, 27);
      this.DataLabel2.TabIndex = 110;
      this.DataLabel2.Text = "Temperature";
      this.DataLabel2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      this.DataLabel2.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.DataLabel_MouseDoubleClick);
      // 
      // DataField3
      // 
      this.DataField3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DataField3.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DataField3.Location = new System.Drawing.Point (313, 35);
      this.DataField3.Name = "DataField3";
      this.DataField3.ReadOnly = true;
      this.DataField3.Size = new System.Drawing.Size (138, 22);
      this.DataField3.TabIndex = 132;
      // 
      // BreakDownTrainingLibrary2Button
      // 
      this.BreakDownTrainingLibrary2Button.Location = new System.Drawing.Point (393, 165);
      this.BreakDownTrainingLibrary2Button.Name = "BreakDownTrainingLibrary2Button";
      this.BreakDownTrainingLibrary2Button.Size = new System.Drawing.Size (71, 23);
      this.BreakDownTrainingLibrary2Button.TabIndex = 28;
      this.BreakDownTrainingLibrary2Button.TabStop = false;
      this.BreakDownTrainingLibrary2Button.Text = "BreakDown";
      this.BreakDownTrainingLibrary2Button.UseVisualStyleBackColor = true;
      this.BreakDownTrainingLibrary2Button.Click += new System.EventHandler (this.BreakDownTrainingLibrary2Button_Click);
      // 
      // BreakDownTrainingLibrary1Button
      // 
      this.BreakDownTrainingLibrary1Button.Location = new System.Drawing.Point (393, 54);
      this.BreakDownTrainingLibrary1Button.Name = "BreakDownTrainingLibrary1Button";
      this.BreakDownTrainingLibrary1Button.Size = new System.Drawing.Size (71, 23);
      this.BreakDownTrainingLibrary1Button.TabIndex = 27;
      this.BreakDownTrainingLibrary1Button.TabStop = false;
      this.BreakDownTrainingLibrary1Button.Text = "BreakDown";
      this.BreakDownTrainingLibrary1Button.UseVisualStyleBackColor = true;
      this.BreakDownTrainingLibrary1Button.Click += new System.EventHandler (this.BreakDownTrainingLibrary1Button_Click);
      // 
      // ImageFileName
      // 
      this.ImageFileName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ImageFileName.Location = new System.Drawing.Point (159, 6);
      this.ImageFileName.Name = "ImageFileName";
      this.ImageFileName.ReadOnly = true;
      this.ImageFileName.Size = new System.Drawing.Size (305, 22);
      this.ImageFileName.TabIndex = 26;
      // 
      // ValidatedClassName
      // 
      this.ValidatedClassName.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
      this.ValidatedClassName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ValidatedClassName.Location = new System.Drawing.Point (76, 267);
      this.ValidatedClassName.Name = "ValidatedClassName";
      this.ValidatedClassName.ReadOnly = true;
      this.ValidatedClassName.Size = new System.Drawing.Size (300, 22);
      this.ValidatedClassName.TabIndex = 23;
      // 
      // ValidatedClassLabel
      // 
      this.ValidatedClassLabel.AutoSize = true;
      this.ValidatedClassLabel.Location = new System.Drawing.Point (7, 270);
      this.ValidatedClassLabel.Name = "ValidatedClassLabel";
      this.ValidatedClassLabel.Size = new System.Drawing.Size (51, 13);
      this.ValidatedClassLabel.TabIndex = 22;
      this.ValidatedClassLabel.Text = "Validated";
      // 
      // LeftPanel
      // 
      this.LeftPanel.AutoScroll = true;
      this.LeftPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.LeftPanel.Controls.Add (this.pictureBox1);
      this.LeftPanel.Controls.Add (this.PlanktonImage);
      this.LeftPanel.Location = new System.Drawing.Point (0, 12);
      this.LeftPanel.Name = "LeftPanel";
      this.LeftPanel.Size = new System.Drawing.Size (536, 500);
      this.LeftPanel.TabIndex = 23;
      this.LeftPanel.SizeChanged += new System.EventHandler (this.LeftPanel_SizeChanged);
      // 
      // pictureBox1
      // 
      this.pictureBox1.Image = global::SipperFile.Properties.Resources.SurfaceArrow;
      this.pictureBox1.Location = new System.Drawing.Point (3, 147);
      this.pictureBox1.Name = "pictureBox1";
      this.pictureBox1.Size = new System.Drawing.Size (20, 120);
      this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
      this.pictureBox1.TabIndex = 24;
      this.pictureBox1.TabStop = false;
      // 
      // PlanktonImage
      // 
      this.PlanktonImage.Location = new System.Drawing.Point (25, 3);
      this.PlanktonImage.Name = "PlanktonImage";
      this.PlanktonImage.Size = new System.Drawing.Size (506, 492);
      this.PlanktonImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
      this.PlanktonImage.TabIndex = 0;
      this.PlanktonImage.TabStop = false;
      this.PlanktonImage.Click += new System.EventHandler (this.PlanktonImage_Click);
      // 
      // ImageViewer
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (1024, 524);
      this.Controls.Add (this.LeftPanel);
      this.Controls.Add (this.RightPanel);
      this.MinimumSize = new System.Drawing.Size (16, 560);
      this.Name = "ImageViewer";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "ImageViewer";
      this.Load += new System.EventHandler (this.ImageViewer_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.ImageViewer_FormClosing);
      this.Resize += new System.EventHandler (this.ImageViewer_Resize);
      this.RightPanel.ResumeLayout (false);
      this.RightPanel.PerformLayout ();
      this.DataFields.ResumeLayout (false);
      this.DataFields.PerformLayout ();
      this.LeftPanel.ResumeLayout (false);
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit ();
      ((System.ComponentModel.ISupportInitialize)(this.PlanktonImage)).EndInit ();
      this.ResumeLayout (false);

    }

    #endregion

    private System.Windows.Forms.PictureBox PlanktonImage;
    private System.Windows.Forms.Button AddToTrainingLibrary;
    private System.Windows.Forms.Label TrainingLibrary1Label;
    private System.Windows.Forms.TextBox TrainingLibrary1;
    private System.Windows.Forms.TextBox TrainingLibrary2;
    private System.Windows.Forms.Label TrainingLibrary2Label;
    private System.Windows.Forms.Label Lib1Pred1Label;
    private System.Windows.Forms.TextBox Lib1Pred1Class;
    private System.Windows.Forms.TextBox Lib1Pred1Prob;
    private System.Windows.Forms.TextBox Lib1Pred2Prob;
    private System.Windows.Forms.TextBox Lib1Pred2Class;
    private System.Windows.Forms.Label Lib1Pred2Label;
    private System.Windows.Forms.TextBox Lib2Pred2Prob;
    private System.Windows.Forms.TextBox Lib2Pred2Class;
    private System.Windows.Forms.Label Lib2Pred2Label;
    private System.Windows.Forms.TextBox Lib2Pred1Prob;
    private System.Windows.Forms.TextBox Lib2Pred1Class;
    private System.Windows.Forms.Label Lib2Pred1Label;
    private System.Windows.Forms.Label ZoomLabel;
    private System.Windows.Forms.ComboBox ZoomFactor;
    private System.Windows.Forms.Panel RightPanel;
    private System.Windows.Forms.Panel LeftPanel;
    private System.Windows.Forms.TextBox ValidatedClassName;
    private System.Windows.Forms.Label ValidatedClassLabel;
    private System.Windows.Forms.TextBox ImageFileName;
    private System.Windows.Forms.Button BreakDownTrainingLibrary2Button;
    private System.Windows.Forms.Button BreakDownTrainingLibrary1Button;
    private System.Windows.Forms.TextBox DataField0;
    private System.Windows.Forms.Label DataLabel0;
    private System.Windows.Forms.Label DataLabel1;
    private System.Windows.Forms.Label DataLabel2;
    private System.Windows.Forms.TableLayoutPanel DataFields;
    private System.Windows.Forms.Label DataLabel3;
    private System.Windows.Forms.TextBox DataField1;
    private System.Windows.Forms.TextBox DataField2;
    private System.Windows.Forms.TextBox DataField3;
    private System.Windows.Forms.Button DisplayGridButton;
    private System.Windows.Forms.TextBox Lib1Pred1Votes;
    private System.Windows.Forms.TextBox Lib2Pred2Votes;
    private System.Windows.Forms.TextBox Lib2Pred1Votes;
    private System.Windows.Forms.TextBox Lib1Pred2Votes;
    private System.Windows.Forms.Button SelectClassButtom;
    private System.Windows.Forms.Button ImageSizeClearButton;
    private System.Windows.Forms.TextBox ImageSize;
    private System.Windows.Forms.Label ImageSizeLabel;
    private System.Windows.Forms.Button ImageSizeAuto;
    private System.Windows.Forms.Button SizeCoordinatesSelEnabled;
    private System.Windows.Forms.TextBox ConnectedComponentDist;
    private System.Windows.Forms.Label ConnectedComponentDistanceLabel;
    private System.Windows.Forms.Button ViewExtractionLogButton;
    private System.Windows.Forms.TextBox DateExtracted;
    private System.Windows.Forms.Label DateExtractedLabel;
    private System.Windows.Forms.TextBox DateClass;
    private System.Windows.Forms.Label DateClassLabel;
    private System.Windows.Forms.Button ViewClassLogButton;
    private System.Windows.Forms.Button EditButton;
    private System.Windows.Forms.PictureBox pictureBox1;
  }
}