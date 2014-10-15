namespace GradeTrainingModel
{
  partial class GetRunTimeParameters
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
      this.components = new System.ComponentModel.Container ();
      this.TrainingModelSelected = new System.Windows.Forms.ComboBox ();
      this.GroundTruth = new System.Windows.Forms.TextBox ();
      this.TitleLabel = new System.Windows.Forms.Label ();
      this.TrainingModelLabel = new System.Windows.Forms.Label ();
      this.GroundTruthLabel = new System.Windows.Forms.Label ();
      this.groundTruthDialog = new System.Windows.Forms.FolderBrowserDialog ();
      this.GroundTruthButton = new System.Windows.Forms.Button ();
      this.TrainingLevelLabel = new System.Windows.Forms.Label ();
      this.PredictionLevelLabel = new System.Windows.Forms.Label ();
      this.TrainingLevel = new System.Windows.Forms.ComboBox ();
      this.PredictionLevel = new System.Windows.Forms.ComboBox ();
      this.StartButon = new System.Windows.Forms.Button ();
      this.CancelButon = new System.Windows.Forms.Button ();
      this.RunLogOutPut = new System.Windows.Forms.TextBox ();
      this.timer1 = new System.Windows.Forms.Timer (this.components);
      this.PredictionProgressBar = new System.Windows.Forms.ProgressBar ();
      this.GroundTruthCount = new System.Windows.Forms.TextBox ();
      this.GroundTruthProcessed = new System.Windows.Forms.TextBox ();
      this.CrossValidation = new System.Windows.Forms.CheckBox ();
      this.EditModel = new System.Windows.Forms.Button ();
      this.SuspendLayout ();
      // 
      // TrainingModelSelected
      // 
      this.TrainingModelSelected.FormattingEnabled = true;
      this.TrainingModelSelected.Location = new System.Drawing.Point (116, 77);
      this.TrainingModelSelected.Name = "TrainingModelSelected";
      this.TrainingModelSelected.Size = new System.Drawing.Size (299, 21);
      this.TrainingModelSelected.TabIndex = 0;
      this.TrainingModelSelected.SelectedValueChanged += new System.EventHandler (this.TrainingModel_SelectedValueChanged);
      // 
      // GroundTruth
      // 
      this.GroundTruth.Location = new System.Drawing.Point (116, 117);
      this.GroundTruth.Name = "GroundTruth";
      this.GroundTruth.Size = new System.Drawing.Size (538, 20);
      this.GroundTruth.TabIndex = 1;
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (242, 15);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (194, 24);
      this.TitleLabel.TabIndex = 2;
      this.TitleLabel.Text = "Grade Training Model";
      // 
      // TrainingModelLabel
      // 
      this.TrainingModelLabel.AutoSize = true;
      this.TrainingModelLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TrainingModelLabel.Location = new System.Drawing.Point (12, 81);
      this.TrainingModelLabel.Name = "TrainingModelLabel";
      this.TrainingModelLabel.Size = new System.Drawing.Size (98, 16);
      this.TrainingModelLabel.TabIndex = 3;
      this.TrainingModelLabel.Text = "Training Model";
      // 
      // GroundTruthLabel
      // 
      this.GroundTruthLabel.AutoSize = true;
      this.GroundTruthLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.GroundTruthLabel.Location = new System.Drawing.Point (12, 120);
      this.GroundTruthLabel.Name = "GroundTruthLabel";
      this.GroundTruthLabel.Size = new System.Drawing.Size (85, 16);
      this.GroundTruthLabel.TabIndex = 4;
      this.GroundTruthLabel.Text = "Ground Truth";
      // 
      // groundTruthDialog
      // 
      this.groundTruthDialog.Description = "Select Directory where ground truth is located.";
      this.groundTruthDialog.RootFolder = System.Environment.SpecialFolder.MyComputer;
      this.groundTruthDialog.SelectedPath = "C:\\PICES\\ExtractedImages";
      this.groundTruthDialog.ShowNewFolderButton = false;
      this.groundTruthDialog.HelpRequest += new System.EventHandler (this.groundTruthDialog_HelpRequest);
      // 
      // GroundTruthButton
      // 
      this.GroundTruthButton.Location = new System.Drawing.Point (660, 117);
      this.GroundTruthButton.Name = "GroundTruthButton";
      this.GroundTruthButton.Size = new System.Drawing.Size (53, 20);
      this.GroundTruthButton.TabIndex = 5;
      this.GroundTruthButton.Text = "Browse";
      this.GroundTruthButton.UseVisualStyleBackColor = true;
      this.GroundTruthButton.Click += new System.EventHandler (this.GroundTruthButton_Click);
      // 
      // TrainingLevelLabel
      // 
      this.TrainingLevelLabel.AutoSize = true;
      this.TrainingLevelLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TrainingLevelLabel.Location = new System.Drawing.Point (12, 162);
      this.TrainingLevelLabel.Name = "TrainingLevelLabel";
      this.TrainingLevelLabel.Size = new System.Drawing.Size (93, 16);
      this.TrainingLevelLabel.TabIndex = 6;
      this.TrainingLevelLabel.Text = "Training Level";
      // 
      // PredictionLevelLabel
      // 
      this.PredictionLevelLabel.AutoSize = true;
      this.PredictionLevelLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.PredictionLevelLabel.Location = new System.Drawing.Point (197, 162);
      this.PredictionLevelLabel.Name = "PredictionLevelLabel";
      this.PredictionLevelLabel.Size = new System.Drawing.Size (104, 16);
      this.PredictionLevelLabel.TabIndex = 7;
      this.PredictionLevelLabel.Text = "Prediction Level";
      // 
      // TrainingLevel
      // 
      this.TrainingLevel.FormattingEnabled = true;
      this.TrainingLevel.Location = new System.Drawing.Point (116, 160);
      this.TrainingLevel.Name = "TrainingLevel";
      this.TrainingLevel.Size = new System.Drawing.Size (41, 21);
      this.TrainingLevel.TabIndex = 8;
      this.TrainingLevel.SelectionChangeCommitted += new System.EventHandler (this.TrainingLevel_SelectionChangeCommitted);
      // 
      // PredictionLevel
      // 
      this.PredictionLevel.FormattingEnabled = true;
      this.PredictionLevel.Location = new System.Drawing.Point (301, 160);
      this.PredictionLevel.Name = "PredictionLevel";
      this.PredictionLevel.Size = new System.Drawing.Size (41, 21);
      this.PredictionLevel.TabIndex = 9;
      this.PredictionLevel.SelectionChangeCommitted += new System.EventHandler (this.PredictionLevel_SelectionChangeCommitted);
      // 
      // StartButon
      // 
      this.StartButon.Location = new System.Drawing.Point (15, 219);
      this.StartButon.Name = "StartButon";
      this.StartButon.Size = new System.Drawing.Size (75, 23);
      this.StartButon.TabIndex = 10;
      this.StartButon.Text = "Start";
      this.StartButon.UseVisualStyleBackColor = true;
      this.StartButon.Click += new System.EventHandler (this.StartButon_Click);
      // 
      // CancelButon
      // 
      this.CancelButon.Enabled = false;
      this.CancelButon.Location = new System.Drawing.Point (638, 219);
      this.CancelButon.Name = "CancelButon";
      this.CancelButon.Size = new System.Drawing.Size (75, 23);
      this.CancelButon.TabIndex = 11;
      this.CancelButon.Text = "Cancel";
      this.CancelButon.UseVisualStyleBackColor = true;
      this.CancelButon.Click += new System.EventHandler (this.CancelButon_Click);
      // 
      // RunLogOutPut
      // 
      this.RunLogOutPut.Enabled = false;
      this.RunLogOutPut.Location = new System.Drawing.Point (15, 264);
      this.RunLogOutPut.Multiline = true;
      this.RunLogOutPut.Name = "RunLogOutPut";
      this.RunLogOutPut.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
      this.RunLogOutPut.Size = new System.Drawing.Size (698, 382);
      this.RunLogOutPut.TabIndex = 12;
      // 
      // timer1
      // 
      this.timer1.Enabled = true;
      this.timer1.Tick += new System.EventHandler (this.timer1_Tick);
      // 
      // PredictionProgressBar
      // 
      this.PredictionProgressBar.Location = new System.Drawing.Point (15, 686);
      this.PredictionProgressBar.Name = "PredictionProgressBar";
      this.PredictionProgressBar.Size = new System.Drawing.Size (698, 22);
      this.PredictionProgressBar.TabIndex = 13;
      // 
      // GroundTruthCount
      // 
      this.GroundTruthCount.Location = new System.Drawing.Point (15, 658);
      this.GroundTruthCount.Name = "GroundTruthCount";
      this.GroundTruthCount.Size = new System.Drawing.Size (100, 20);
      this.GroundTruthCount.TabIndex = 14;
      this.GroundTruthCount.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // GroundTruthProcessed
      // 
      this.GroundTruthProcessed.Location = new System.Drawing.Point (613, 658);
      this.GroundTruthProcessed.Name = "GroundTruthProcessed";
      this.GroundTruthProcessed.Size = new System.Drawing.Size (100, 20);
      this.GroundTruthProcessed.TabIndex = 15;
      this.GroundTruthProcessed.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // CrossValidation
      // 
      this.CrossValidation.AutoSize = true;
      this.CrossValidation.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CrossValidation.Location = new System.Drawing.Point (589, 76);
      this.CrossValidation.Name = "CrossValidation";
      this.CrossValidation.Size = new System.Drawing.Size (129, 21);
      this.CrossValidation.TabIndex = 16;
      this.CrossValidation.Text = "Cross Validation";
      this.CrossValidation.UseVisualStyleBackColor = true;
      this.CrossValidation.CheckedChanged += new System.EventHandler (this.CrossValidation_CheckedChanged);
      // 
      // EditModel
      // 
      this.EditModel.Enabled = false;
      this.EditModel.Location = new System.Drawing.Point (423, 76);
      this.EditModel.Name = "EditModel";
      this.EditModel.Size = new System.Drawing.Size (75, 22);
      this.EditModel.TabIndex = 17;
      this.EditModel.Text = "Edit";
      this.EditModel.UseVisualStyleBackColor = true;
      this.EditModel.Click += new System.EventHandler (this.EditModel_Click);
      // 
      // GetRunTimeParameters
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (730, 750);
      this.Controls.Add (this.EditModel);
      this.Controls.Add (this.CrossValidation);
      this.Controls.Add (this.GroundTruthProcessed);
      this.Controls.Add (this.GroundTruthCount);
      this.Controls.Add (this.PredictionProgressBar);
      this.Controls.Add (this.RunLogOutPut);
      this.Controls.Add (this.CancelButon);
      this.Controls.Add (this.StartButon);
      this.Controls.Add (this.PredictionLevel);
      this.Controls.Add (this.TrainingLevel);
      this.Controls.Add (this.PredictionLevelLabel);
      this.Controls.Add (this.TrainingLevelLabel);
      this.Controls.Add (this.GroundTruthButton);
      this.Controls.Add (this.GroundTruthLabel);
      this.Controls.Add (this.TrainingModelLabel);
      this.Controls.Add (this.TitleLabel);
      this.Controls.Add (this.GroundTruth);
      this.Controls.Add (this.TrainingModelSelected);
      this.Name = "GetRunTimeParameters";
      this.Text = "Grade Training Model";
      this.Load += new System.EventHandler (this.GetRunTimeParameters_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.GetRunTimeParameters_FormClosing);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.ComboBox TrainingModelSelected;
    private System.Windows.Forms.TextBox GroundTruth;
    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label TrainingModelLabel;
    private System.Windows.Forms.Label GroundTruthLabel;
    private System.Windows.Forms.FolderBrowserDialog groundTruthDialog;
    private System.Windows.Forms.Button GroundTruthButton;
    private System.Windows.Forms.Label TrainingLevelLabel;
    private System.Windows.Forms.Label PredictionLevelLabel;
    private System.Windows.Forms.ComboBox TrainingLevel;
    private System.Windows.Forms.ComboBox PredictionLevel;
    private System.Windows.Forms.Button StartButon;
    private System.Windows.Forms.Button CancelButon;
    private System.Windows.Forms.TextBox RunLogOutPut;
    private System.Windows.Forms.Timer timer1;
    private System.Windows.Forms.ProgressBar PredictionProgressBar;
    private System.Windows.Forms.TextBox GroundTruthCount;
    private System.Windows.Forms.TextBox GroundTruthProcessed;
    private System.Windows.Forms.CheckBox CrossValidation;
    private System.Windows.Forms.Button EditModel;
  }
}

