namespace SipperFile
{
  partial class ImageEditor
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void  Dispose (bool disposing)
    {
      if  (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
      
      if  (image != null)
      {
        image.Dispose();
        image = null;
      }
      raster = null;
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.pictureBox1 = new System.Windows.Forms.PictureBox ();
      this.SaveButton = new System.Windows.Forms.Button ();
      this.ImagePanel = new System.Windows.Forms.Panel ();
      this.SizeOptions = new ChartControls.RadioButtonList ();
      this.pred1ClassName = new System.Windows.Forms.TextBox ();
      this.pred2ClassName = new System.Windows.Forms.TextBox ();
      this.pred1Prob = new System.Windows.Forms.TextBox ();
      this.pred2Prob = new System.Windows.Forms.TextBox ();
      this.lib2Pred2Prob = new System.Windows.Forms.TextBox ();
      this.lib2Pred1Prob = new System.Windows.Forms.TextBox ();
      this.lib2Pred2ClassName = new System.Windows.Forms.TextBox ();
      this.lib2Pred1ClassName = new System.Windows.Forms.TextBox ();
      this.TrainLib1Name = new System.Windows.Forms.TextBox ();
      this.TrainLib2Name = new System.Windows.Forms.TextBox ();
      this.Colorize = new System.Windows.Forms.CheckBox ();
      this.ClossingButton = new System.Windows.Forms.Button ();
      this.OpeningButton = new System.Windows.Forms.Button ();
      this.ErosionButton = new System.Windows.Forms.Button ();
      this.DialationButton = new System.Windows.Forms.Button ();
      this.EdgeButton = new System.Windows.Forms.Button ();
      this.FillHolesButton = new System.Windows.Forms.Button ();
      this.SmoothingButton = new System.Windows.Forms.Button ();
      this.MediumButton = new System.Windows.Forms.Button ();
      this.panel1 = new System.Windows.Forms.Panel ();
      this.MaskTypesComboBox = new System.Windows.Forms.ComboBox ();
      this.ThinningButton = new System.Windows.Forms.Button ();
      this.UndoButton = new System.Windows.Forms.Button ();
      this.OperationsList = new System.Windows.Forms.ListBox ();
      this.ConnectedComponentButton = new System.Windows.Forms.Button ();
      this.BinarizeButton = new System.Windows.Forms.Button ();
      this.ConvexHullButon = new System.Windows.Forms.Button ();
      this.BandPassButton = new System.Windows.Forms.Button ();
      this.LowerBound = new System.Windows.Forms.TextBox ();
      this.UpperBound = new System.Windows.Forms.TextBox ();
      this.BinarizeTHButton = new System.Windows.Forms.Button ();
      this.ThLowerBound = new System.Windows.Forms.TextBox ();
      this.ThUpperBound = new System.Windows.Forms.TextBox ();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit ();
      this.ImagePanel.SuspendLayout ();
      this.panel1.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // pictureBox1
      // 
      this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.pictureBox1.Location = new System.Drawing.Point (-1, 3);
      this.pictureBox1.Name = "pictureBox1";
      this.pictureBox1.Size = new System.Drawing.Size (670, 601);
      this.pictureBox1.TabIndex = 0;
      this.pictureBox1.TabStop = false;
      // 
      // SaveButton
      // 
      this.SaveButton.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SaveButton.Location = new System.Drawing.Point (790, 23);
      this.SaveButton.Name = "SaveButton";
      this.SaveButton.Size = new System.Drawing.Size (88, 44);
      this.SaveButton.TabIndex = 1;
      this.SaveButton.Text = "Save";
      this.SaveButton.UseVisualStyleBackColor = true;
      this.SaveButton.Click += new System.EventHandler (this.SaveButton_Click);
      // 
      // ImagePanel
      // 
      this.ImagePanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.ImagePanel.Controls.Add (this.pictureBox1);
      this.ImagePanel.Location = new System.Drawing.Point (204, 87);
      this.ImagePanel.Name = "ImagePanel";
      this.ImagePanel.Size = new System.Drawing.Size (674, 607);
      this.ImagePanel.TabIndex = 2;
      // 
      // SizeOptions
      // 
      this.SizeOptions.Location = new System.Drawing.Point (13, 10);
      this.SizeOptions.Name = "SizeOptions";
      this.SizeOptions.NumColumns = 3;
      this.SizeOptions.Options = new string[] {
        "10%",
        "25%",
        "50%",
        "100%",
        "150%",
        "200%"};
      this.SizeOptions.OptionsFont = new System.Drawing.Font ("Microsoft Sans Serif", 6.75F);
      this.SizeOptions.SelectedOption = "100%";
      this.SizeOptions.SelectedOptionInt = 3;
      this.SizeOptions.Size = new System.Drawing.Size (165, 69);
      this.SizeOptions.TabIndex = 3;
      this.SizeOptions.Title = "Sizing";
      this.SizeOptions.SelectedOptionChanged += new ChartControls.RadioButtonList.SelectedOptionChangedHandler (this.SizeOptions_SelectedOptionChanged);
      // 
      // pred1ClassName
      // 
      this.pred1ClassName.Location = new System.Drawing.Point (250, 36);
      this.pred1ClassName.Name = "pred1ClassName";
      this.pred1ClassName.ReadOnly = true;
      this.pred1ClassName.Size = new System.Drawing.Size (193, 20);
      this.pred1ClassName.TabIndex = 7;
      // 
      // pred2ClassName
      // 
      this.pred2ClassName.Location = new System.Drawing.Point (250, 61);
      this.pred2ClassName.Name = "pred2ClassName";
      this.pred2ClassName.ReadOnly = true;
      this.pred2ClassName.Size = new System.Drawing.Size (192, 20);
      this.pred2ClassName.TabIndex = 8;
      // 
      // pred1Prob
      // 
      this.pred1Prob.Location = new System.Drawing.Point (204, 36);
      this.pred1Prob.Name = "pred1Prob";
      this.pred1Prob.ReadOnly = true;
      this.pred1Prob.Size = new System.Drawing.Size (43, 20);
      this.pred1Prob.TabIndex = 9;
      // 
      // pred2Prob
      // 
      this.pred2Prob.Location = new System.Drawing.Point (204, 61);
      this.pred2Prob.Name = "pred2Prob";
      this.pred2Prob.ReadOnly = true;
      this.pred2Prob.Size = new System.Drawing.Size (43, 20);
      this.pred2Prob.TabIndex = 10;
      // 
      // lib2Pred2Prob
      // 
      this.lib2Pred2Prob.Location = new System.Drawing.Point (493, 61);
      this.lib2Pred2Prob.Name = "lib2Pred2Prob";
      this.lib2Pred2Prob.ReadOnly = true;
      this.lib2Pred2Prob.Size = new System.Drawing.Size (43, 20);
      this.lib2Pred2Prob.TabIndex = 14;
      // 
      // lib2Pred1Prob
      // 
      this.lib2Pred1Prob.Location = new System.Drawing.Point (493, 36);
      this.lib2Pred1Prob.Name = "lib2Pred1Prob";
      this.lib2Pred1Prob.ReadOnly = true;
      this.lib2Pred1Prob.Size = new System.Drawing.Size (43, 20);
      this.lib2Pred1Prob.TabIndex = 13;
      // 
      // lib2Pred2ClassName
      // 
      this.lib2Pred2ClassName.Location = new System.Drawing.Point (539, 61);
      this.lib2Pred2ClassName.Name = "lib2Pred2ClassName";
      this.lib2Pred2ClassName.ReadOnly = true;
      this.lib2Pred2ClassName.Size = new System.Drawing.Size (192, 20);
      this.lib2Pred2ClassName.TabIndex = 12;
      // 
      // lib2Pred1ClassName
      // 
      this.lib2Pred1ClassName.Location = new System.Drawing.Point (539, 36);
      this.lib2Pred1ClassName.Name = "lib2Pred1ClassName";
      this.lib2Pred1ClassName.ReadOnly = true;
      this.lib2Pred1ClassName.Size = new System.Drawing.Size (193, 20);
      this.lib2Pred1ClassName.TabIndex = 11;
      // 
      // TrainLib1Name
      // 
      this.TrainLib1Name.Location = new System.Drawing.Point (204, 10);
      this.TrainLib1Name.Name = "TrainLib1Name";
      this.TrainLib1Name.ReadOnly = true;
      this.TrainLib1Name.Size = new System.Drawing.Size (239, 20);
      this.TrainLib1Name.TabIndex = 15;
      this.TrainLib1Name.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // TrainLib2Name
      // 
      this.TrainLib2Name.Location = new System.Drawing.Point (493, 10);
      this.TrainLib2Name.Name = "TrainLib2Name";
      this.TrainLib2Name.ReadOnly = true;
      this.TrainLib2Name.Size = new System.Drawing.Size (239, 20);
      this.TrainLib2Name.TabIndex = 16;
      this.TrainLib2Name.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // Colorize
      // 
      this.Colorize.AutoSize = true;
      this.Colorize.Location = new System.Drawing.Point (16, 92);
      this.Colorize.Name = "Colorize";
      this.Colorize.Size = new System.Drawing.Size (63, 17);
      this.Colorize.TabIndex = 17;
      this.Colorize.Text = "Colorize";
      this.Colorize.UseVisualStyleBackColor = true;
      this.Colorize.CheckedChanged += new System.EventHandler (this.Colorize_CheckedChanged);
      // 
      // ClossingButton
      // 
      this.ClossingButton.Location = new System.Drawing.Point (3, 14);
      this.ClossingButton.Name = "ClossingButton";
      this.ClossingButton.Size = new System.Drawing.Size (76, 22);
      this.ClossingButton.TabIndex = 18;
      this.ClossingButton.Text = "Closing";
      this.ClossingButton.UseVisualStyleBackColor = true;
      this.ClossingButton.Click += new System.EventHandler (this.ClossingButton_Click);
      // 
      // OpeningButton
      // 
      this.OpeningButton.Location = new System.Drawing.Point (95, 14);
      this.OpeningButton.Name = "OpeningButton";
      this.OpeningButton.Size = new System.Drawing.Size (76, 22);
      this.OpeningButton.TabIndex = 19;
      this.OpeningButton.Text = "Opening";
      this.OpeningButton.UseVisualStyleBackColor = true;
      this.OpeningButton.Click += new System.EventHandler (this.OpeningButton_Click);
      // 
      // ErosionButton
      // 
      this.ErosionButton.Location = new System.Drawing.Point (3, 42);
      this.ErosionButton.Name = "ErosionButton";
      this.ErosionButton.Size = new System.Drawing.Size (76, 22);
      this.ErosionButton.TabIndex = 20;
      this.ErosionButton.Text = "Erosion";
      this.ErosionButton.UseVisualStyleBackColor = true;
      this.ErosionButton.Click += new System.EventHandler (this.ErosionButton_Click);
      // 
      // DialationButton
      // 
      this.DialationButton.Location = new System.Drawing.Point (95, 42);
      this.DialationButton.Name = "DialationButton";
      this.DialationButton.Size = new System.Drawing.Size (76, 22);
      this.DialationButton.TabIndex = 21;
      this.DialationButton.Text = "Dialation";
      this.DialationButton.UseVisualStyleBackColor = true;
      this.DialationButton.Click += new System.EventHandler (this.DialationButton_Click);
      // 
      // EdgeButton
      // 
      this.EdgeButton.Location = new System.Drawing.Point (16, 267);
      this.EdgeButton.Name = "EdgeButton";
      this.EdgeButton.Size = new System.Drawing.Size (76, 22);
      this.EdgeButton.TabIndex = 22;
      this.EdgeButton.Text = "Edge";
      this.EdgeButton.UseVisualStyleBackColor = true;
      this.EdgeButton.Click += new System.EventHandler (this.EdgeButton_Click);
      // 
      // FillHolesButton
      // 
      this.FillHolesButton.Location = new System.Drawing.Point (108, 267);
      this.FillHolesButton.Name = "FillHolesButton";
      this.FillHolesButton.Size = new System.Drawing.Size (76, 22);
      this.FillHolesButton.TabIndex = 23;
      this.FillHolesButton.Text = "Fill Holes";
      this.FillHolesButton.UseVisualStyleBackColor = true;
      this.FillHolesButton.Click += new System.EventHandler (this.FillHolesButton_Click);
      // 
      // SmoothingButton
      // 
      this.SmoothingButton.Location = new System.Drawing.Point (3, 70);
      this.SmoothingButton.Name = "SmoothingButton";
      this.SmoothingButton.Size = new System.Drawing.Size (76, 22);
      this.SmoothingButton.TabIndex = 24;
      this.SmoothingButton.Text = "Smoothing";
      this.SmoothingButton.UseVisualStyleBackColor = true;
      this.SmoothingButton.Click += new System.EventHandler (this.SmoothingButton_Click);
      // 
      // MediumButton
      // 
      this.MediumButton.Location = new System.Drawing.Point (95, 70);
      this.MediumButton.Name = "MediumButton";
      this.MediumButton.Size = new System.Drawing.Size (76, 22);
      this.MediumButton.TabIndex = 25;
      this.MediumButton.Text = "Medium";
      this.MediumButton.UseVisualStyleBackColor = true;
      this.MediumButton.Click += new System.EventHandler (this.MediumButton_Click);
      // 
      // panel1
      // 
      this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.panel1.Controls.Add (this.MaskTypesComboBox);
      this.panel1.Controls.Add (this.OpeningButton);
      this.panel1.Controls.Add (this.MediumButton);
      this.panel1.Controls.Add (this.ClossingButton);
      this.panel1.Controls.Add (this.SmoothingButton);
      this.panel1.Controls.Add (this.ErosionButton);
      this.panel1.Controls.Add (this.DialationButton);
      this.panel1.Location = new System.Drawing.Point (13, 122);
      this.panel1.Name = "panel1";
      this.panel1.Size = new System.Drawing.Size (174, 129);
      this.panel1.TabIndex = 26;
      // 
      // MaskTypesComboBox
      // 
      this.MaskTypesComboBox.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MaskTypesComboBox.FormattingEnabled = true;
      this.MaskTypesComboBox.Location = new System.Drawing.Point (27, 98);
      this.MaskTypesComboBox.Name = "MaskTypesComboBox";
      this.MaskTypesComboBox.Size = new System.Drawing.Size (121, 24);
      this.MaskTypesComboBox.TabIndex = 26;
      // 
      // ThinningButton
      // 
      this.ThinningButton.Location = new System.Drawing.Point (16, 295);
      this.ThinningButton.Name = "ThinningButton";
      this.ThinningButton.Size = new System.Drawing.Size (76, 22);
      this.ThinningButton.TabIndex = 27;
      this.ThinningButton.Text = "Thinning";
      this.ThinningButton.UseVisualStyleBackColor = true;
      this.ThinningButton.Click += new System.EventHandler (this.ThinningButton_Click);
      // 
      // UndoButton
      // 
      this.UndoButton.Location = new System.Drawing.Point (59, 460);
      this.UndoButton.Name = "UndoButton";
      this.UndoButton.Size = new System.Drawing.Size (76, 22);
      this.UndoButton.TabIndex = 28;
      this.UndoButton.Text = "Undo";
      this.UndoButton.UseVisualStyleBackColor = true;
      this.UndoButton.Click += new System.EventHandler (this.UndoButton_Click);
      // 
      // OperationsList
      // 
      this.OperationsList.Font = new System.Drawing.Font ("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.OperationsList.FormattingEnabled = true;
      this.OperationsList.ItemHeight = 15;
      this.OperationsList.Location = new System.Drawing.Point (13, 495);
      this.OperationsList.MultiColumn = true;
      this.OperationsList.Name = "OperationsList";
      this.OperationsList.Size = new System.Drawing.Size (174, 199);
      this.OperationsList.TabIndex = 29;
      // 
      // ConnectedComponentButton
      // 
      this.ConnectedComponentButton.Location = new System.Drawing.Point (108, 295);
      this.ConnectedComponentButton.Name = "ConnectedComponentButton";
      this.ConnectedComponentButton.Size = new System.Drawing.Size (76, 22);
      this.ConnectedComponentButton.TabIndex = 30;
      this.ConnectedComponentButton.Text = "Connected";
      this.ConnectedComponentButton.UseVisualStyleBackColor = true;
      this.ConnectedComponentButton.Click += new System.EventHandler (this.ConnectedComponentButton_Click);
      // 
      // BinarizeButton
      // 
      this.BinarizeButton.Location = new System.Drawing.Point (16, 323);
      this.BinarizeButton.Name = "BinarizeButton";
      this.BinarizeButton.Size = new System.Drawing.Size (76, 22);
      this.BinarizeButton.TabIndex = 31;
      this.BinarizeButton.Text = "Binarize";
      this.BinarizeButton.UseVisualStyleBackColor = true;
      this.BinarizeButton.Click += new System.EventHandler (this.BinarizeButton_Click);
      // 
      // ConvexHullButon
      // 
      this.ConvexHullButon.Location = new System.Drawing.Point (108, 323);
      this.ConvexHullButon.Name = "ConvexHullButon";
      this.ConvexHullButon.Size = new System.Drawing.Size (76, 22);
      this.ConvexHullButon.TabIndex = 32;
      this.ConvexHullButon.Text = "ConvexHull";
      this.ConvexHullButon.UseVisualStyleBackColor = true;
      this.ConvexHullButon.Click += new System.EventHandler (this.ConvexHullButon_Click);
      // 
      // BandPassButton
      // 
      this.BandPassButton.Location = new System.Drawing.Point (16, 351);
      this.BandPassButton.Name = "BandPassButton";
      this.BandPassButton.Size = new System.Drawing.Size (76, 22);
      this.BandPassButton.TabIndex = 33;
      this.BandPassButton.Text = "Band Pass";
      this.BandPassButton.UseVisualStyleBackColor = true;
      this.BandPassButton.Click += new System.EventHandler (this.BandPassButton_Click);
      // 
      // LowerBound
      // 
      this.LowerBound.Location = new System.Drawing.Point (108, 353);
      this.LowerBound.Name = "LowerBound";
      this.LowerBound.Size = new System.Drawing.Size (29, 20);
      this.LowerBound.TabIndex = 34;
      this.LowerBound.Text = "0";
      // 
      // UpperBound
      // 
      this.UpperBound.Location = new System.Drawing.Point (148, 353);
      this.UpperBound.Name = "UpperBound";
      this.UpperBound.Size = new System.Drawing.Size (31, 20);
      this.UpperBound.TabIndex = 35;
      this.UpperBound.Text = "100";
      // 
      // BinarizeTHButton
      // 
      this.BinarizeTHButton.Location = new System.Drawing.Point (16, 379);
      this.BinarizeTHButton.Name = "BinarizeTHButton";
      this.BinarizeTHButton.Size = new System.Drawing.Size (76, 22);
      this.BinarizeTHButton.TabIndex = 32;
      this.BinarizeTHButton.Text = "Binarize TH";
      this.BinarizeTHButton.UseVisualStyleBackColor = true;
      this.BinarizeTHButton.Click += new System.EventHandler (this.BinarizeTHButton_Click);
      // 
      // ThLowerBound
      // 
      this.ThLowerBound.Location = new System.Drawing.Point (108, 381);
      this.ThLowerBound.Name = "ThLowerBound";
      this.ThLowerBound.Size = new System.Drawing.Size (29, 20);
      this.ThLowerBound.TabIndex = 36;
      this.ThLowerBound.Text = "0";
      // 
      // ThUpperBound
      // 
      this.ThUpperBound.Location = new System.Drawing.Point (148, 381);
      this.ThUpperBound.Name = "ThUpperBound";
      this.ThUpperBound.Size = new System.Drawing.Size (30, 20);
      this.ThUpperBound.TabIndex = 37;
      this.ThUpperBound.Text = "0";
      // 
      // ImageEditor
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (905, 711);
      this.Controls.Add (this.ThUpperBound);
      this.Controls.Add (this.ThLowerBound);
      this.Controls.Add (this.BinarizeTHButton);
      this.Controls.Add (this.UpperBound);
      this.Controls.Add (this.LowerBound);
      this.Controls.Add (this.BandPassButton);
      this.Controls.Add (this.ConvexHullButon);
      this.Controls.Add (this.BinarizeButton);
      this.Controls.Add (this.ConnectedComponentButton);
      this.Controls.Add (this.OperationsList);
      this.Controls.Add (this.UndoButton);
      this.Controls.Add (this.ThinningButton);
      this.Controls.Add (this.panel1);
      this.Controls.Add (this.FillHolesButton);
      this.Controls.Add (this.EdgeButton);
      this.Controls.Add (this.Colorize);
      this.Controls.Add (this.TrainLib2Name);
      this.Controls.Add (this.TrainLib1Name);
      this.Controls.Add (this.lib2Pred2Prob);
      this.Controls.Add (this.lib2Pred1Prob);
      this.Controls.Add (this.lib2Pred2ClassName);
      this.Controls.Add (this.lib2Pred1ClassName);
      this.Controls.Add (this.pred2Prob);
      this.Controls.Add (this.pred1Prob);
      this.Controls.Add (this.pred2ClassName);
      this.Controls.Add (this.pred1ClassName);
      this.Controls.Add (this.SizeOptions);
      this.Controls.Add (this.ImagePanel);
      this.Controls.Add (this.SaveButton);
      this.Name = "ImageEditor";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "ImageEditor";
      this.Load += new System.EventHandler (this.ImageEditor_Load);
      this.SizeChanged += new System.EventHandler (this.ImageEditor_SizeChanged);
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit ();
      this.ImagePanel.ResumeLayout (false);
      this.panel1.ResumeLayout (false);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.PictureBox pictureBox1;
    private System.Windows.Forms.Button SaveButton;
    private System.Windows.Forms.Panel ImagePanel;
    private ChartControls.RadioButtonList SizeOptions;
    private System.Windows.Forms.TextBox pred1ClassName;
    private System.Windows.Forms.TextBox pred2ClassName;
    private System.Windows.Forms.TextBox pred1Prob;
    private System.Windows.Forms.TextBox pred2Prob;
    private System.Windows.Forms.TextBox lib2Pred2Prob;
    private System.Windows.Forms.TextBox lib2Pred1Prob;
    private System.Windows.Forms.TextBox lib2Pred2ClassName;
    private System.Windows.Forms.TextBox lib2Pred1ClassName;
    private System.Windows.Forms.TextBox TrainLib1Name;
    private System.Windows.Forms.TextBox TrainLib2Name;
    private System.Windows.Forms.CheckBox Colorize;
    private System.Windows.Forms.Button ClossingButton;
    private System.Windows.Forms.Button OpeningButton;
    private System.Windows.Forms.Button ErosionButton;
    private System.Windows.Forms.Button DialationButton;
    private System.Windows.Forms.Button EdgeButton;
    private System.Windows.Forms.Button FillHolesButton;
    private System.Windows.Forms.Button SmoothingButton;
    private System.Windows.Forms.Button MediumButton;
    private System.Windows.Forms.Panel panel1;
    private System.Windows.Forms.ComboBox MaskTypesComboBox;
    private System.Windows.Forms.Button ThinningButton;
    private System.Windows.Forms.Button UndoButton;
    private System.Windows.Forms.ListBox OperationsList;
    private System.Windows.Forms.Button ConnectedComponentButton;
    private System.Windows.Forms.Button BinarizeButton;
    private System.Windows.Forms.Button ConvexHullButon;
      private System.Windows.Forms.Button BandPassButton;
    private System.Windows.Forms.TextBox LowerBound;
    private System.Windows.Forms.TextBox UpperBound;
    private System.Windows.Forms.Button BinarizeTHButton;
    private System.Windows.Forms.TextBox ThLowerBound;
    private System.Windows.Forms.TextBox ThUpperBound;
  }
}