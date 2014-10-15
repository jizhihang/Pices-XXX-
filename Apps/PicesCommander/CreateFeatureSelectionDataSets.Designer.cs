namespace PicesCommander
{
    partial class CreateFeatureSelectionDataSets
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
          this.TitleLabel = new System.Windows.Forms.Label ();
          this.ModelNamLabel = new System.Windows.Forms.Label ();
          this.ModelName = new System.Windows.Forms.ComboBox ();
          this.TrainingDataPercentageLabel = new System.Windows.Forms.Label ();
          this.TestDataPercentageLabel = new System.Windows.Forms.Label ();
          this.ValidationDataPercentageLabel2 = new System.Windows.Forms.Label ();
          this.DestinationDirectoryLabel = new System.Windows.Forms.Label ();
          this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel ();
          this.panel1 = new System.Windows.Forms.Panel ();
          this.DirectoryBrowseButton = new System.Windows.Forms.Button ();
          this.DestinationDirectory = new System.Windows.Forms.TextBox ();
          this.Test2DataPercentage = new System.Windows.Forms.TextBox ();
          this.ValidaionDataPercentage = new System.Windows.Forms.TextBox ();
          this.panel2 = new System.Windows.Forms.Panel ();
          this.TrainingMaxImagesPerClass = new System.Windows.Forms.TextBox ();
          this.TraiingMaxImagsPerClassLabel = new System.Windows.Forms.Label ();
          this.Training2DataPercentage = new System.Windows.Forms.TextBox ();
          this.CreateFeatureFiles = new System.Windows.Forms.Button ();
          this.CancelCreateFeatureSelectionButton = new System.Windows.Forms.Button ();
          this.Dialog = new System.Windows.Forms.TextBox ();
          this.DialogTimer = new System.Windows.Forms.Timer (this.components);
          this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
          this.RunLogLabel = new System.Windows.Forms.Label ();
          this.tableLayoutPanel1.SuspendLayout ();
          this.panel1.SuspendLayout ();
          this.panel2.SuspendLayout ();
          ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
          this.SuspendLayout ();
          // 
          // TitleLabel
          // 
          this.TitleLabel.AutoSize = true;
          this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
          this.TitleLabel.Location = new System.Drawing.Point (170, 9);
          this.TitleLabel.Name = "TitleLabel";
          this.TitleLabel.Size = new System.Drawing.Size (437, 24);
          this.TitleLabel.TabIndex = 0;
          this.TitleLabel.Text = "Create Feature Selection Files from Training Model";
          // 
          // ModelNamLabel
          // 
          this.ModelNamLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.ModelNamLabel.AutoSize = true;
          this.ModelNamLabel.Location = new System.Drawing.Point (6, 3);
          this.ModelNamLabel.Name = "ModelNamLabel";
          this.ModelNamLabel.Size = new System.Drawing.Size (135, 27);
          this.ModelNamLabel.TabIndex = 1;
          this.ModelNamLabel.Text = "Model Name";
          this.ModelNamLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
          // 
          // ModelName
          // 
          this.ModelName.FormattingEnabled = true;
          this.ModelName.Location = new System.Drawing.Point (148, 6);
          this.ModelName.Name = "ModelName";
          this.ModelName.Size = new System.Drawing.Size (172, 21);
          this.ModelName.TabIndex = 20;
          this.ModelName.Validating += new System.ComponentModel.CancelEventHandler (this.ModelName_Validating);
          // 
          // TrainingDataPercentageLabel
          // 
          this.TrainingDataPercentageLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.TrainingDataPercentageLabel.AutoSize = true;
          this.TrainingDataPercentageLabel.Location = new System.Drawing.Point (6, 31);
          this.TrainingDataPercentageLabel.Name = "TrainingDataPercentageLabel";
          this.TrainingDataPercentageLabel.Padding = new System.Windows.Forms.Padding (2);
          this.TrainingDataPercentageLabel.Size = new System.Drawing.Size (135, 33);
          this.TrainingDataPercentageLabel.TabIndex = 3;
          this.TrainingDataPercentageLabel.Text = "Training Data Percntage";
          this.TrainingDataPercentageLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
          // 
          // TestDataPercentageLabel
          // 
          this.TestDataPercentageLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.TestDataPercentageLabel.AutoSize = true;
          this.TestDataPercentageLabel.Location = new System.Drawing.Point (6, 65);
          this.TestDataPercentageLabel.Name = "TestDataPercentageLabel";
          this.TestDataPercentageLabel.Padding = new System.Windows.Forms.Padding (2);
          this.TestDataPercentageLabel.Size = new System.Drawing.Size (135, 26);
          this.TestDataPercentageLabel.TabIndex = 7;
          this.TestDataPercentageLabel.Text = "Test Data Percntage";
          this.TestDataPercentageLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
          // 
          // ValidationDataPercentageLabel2
          // 
          this.ValidationDataPercentageLabel2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.ValidationDataPercentageLabel2.AutoSize = true;
          this.ValidationDataPercentageLabel2.Location = new System.Drawing.Point (6, 92);
          this.ValidationDataPercentageLabel2.Name = "ValidationDataPercentageLabel2";
          this.ValidationDataPercentageLabel2.Padding = new System.Windows.Forms.Padding (2);
          this.ValidationDataPercentageLabel2.Size = new System.Drawing.Size (135, 26);
          this.ValidationDataPercentageLabel2.TabIndex = 9;
          this.ValidationDataPercentageLabel2.Text = "Validation Data Percntage";
          this.ValidationDataPercentageLabel2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
          // 
          // DestinationDirectoryLabel
          // 
          this.DestinationDirectoryLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.DestinationDirectoryLabel.AutoSize = true;
          this.DestinationDirectoryLabel.Location = new System.Drawing.Point (6, 119);
          this.DestinationDirectoryLabel.Name = "DestinationDirectoryLabel";
          this.DestinationDirectoryLabel.Padding = new System.Windows.Forms.Padding (2);
          this.DestinationDirectoryLabel.Size = new System.Drawing.Size (135, 36);
          this.DestinationDirectoryLabel.TabIndex = 10;
          this.DestinationDirectoryLabel.Text = "Destinaion Directory";
          this.DestinationDirectoryLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
          // 
          // tableLayoutPanel1
          // 
          this.tableLayoutPanel1.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
          this.tableLayoutPanel1.ColumnCount = 2;
          this.tableLayoutPanel1.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 19.34605F));
          this.tableLayoutPanel1.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 80.65395F));
          this.tableLayoutPanel1.Controls.Add (this.panel1, 1, 4);
          this.tableLayoutPanel1.Controls.Add (this.ModelNamLabel, 0, 0);
          this.tableLayoutPanel1.Controls.Add (this.ModelName, 1, 0);
          this.tableLayoutPanel1.Controls.Add (this.DestinationDirectoryLabel, 0, 4);
          this.tableLayoutPanel1.Controls.Add (this.TrainingDataPercentageLabel, 0, 1);
          this.tableLayoutPanel1.Controls.Add (this.ValidationDataPercentageLabel2, 0, 3);
          this.tableLayoutPanel1.Controls.Add (this.TestDataPercentageLabel, 0, 2);
          this.tableLayoutPanel1.Controls.Add (this.Test2DataPercentage, 1, 2);
          this.tableLayoutPanel1.Controls.Add (this.ValidaionDataPercentage, 1, 3);
          this.tableLayoutPanel1.Controls.Add (this.panel2, 1, 1);
          this.tableLayoutPanel1.Location = new System.Drawing.Point (24, 36);
          this.tableLayoutPanel1.Name = "tableLayoutPanel1";
          this.tableLayoutPanel1.Padding = new System.Windows.Forms.Padding (2);
          this.tableLayoutPanel1.RowCount = 5;
          this.tableLayoutPanel1.RowStyles.Add (new System.Windows.Forms.RowStyle ());
          this.tableLayoutPanel1.RowStyles.Add (new System.Windows.Forms.RowStyle ());
          this.tableLayoutPanel1.RowStyles.Add (new System.Windows.Forms.RowStyle ());
          this.tableLayoutPanel1.RowStyles.Add (new System.Windows.Forms.RowStyle ());
          this.tableLayoutPanel1.RowStyles.Add (new System.Windows.Forms.RowStyle ());
          this.tableLayoutPanel1.Size = new System.Drawing.Size (739, 158);
          this.tableLayoutPanel1.TabIndex = 7;
          // 
          // panel1
          // 
          this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.panel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
          this.panel1.Controls.Add (this.DirectoryBrowseButton);
          this.panel1.Controls.Add (this.DestinationDirectory);
          this.panel1.Location = new System.Drawing.Point (148, 122);
          this.panel1.Name = "panel1";
          this.panel1.Padding = new System.Windows.Forms.Padding (2);
          this.panel1.Size = new System.Drawing.Size (585, 30);
          this.panel1.TabIndex = 10;
          // 
          // DirectoryBrowseButton
          // 
          this.DirectoryBrowseButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.DirectoryBrowseButton.Location = new System.Drawing.Point (503, 4);
          this.DirectoryBrowseButton.Name = "DirectoryBrowseButton";
          this.DirectoryBrowseButton.Size = new System.Drawing.Size (75, 22);
          this.DirectoryBrowseButton.TabIndex = 31;
          this.DirectoryBrowseButton.Text = "Browse";
          this.DirectoryBrowseButton.UseVisualStyleBackColor = true;
          this.DirectoryBrowseButton.Click += new System.EventHandler (this.DirectoryBrowseButton_Click);
          // 
          // DestinationDirectory
          // 
          this.DestinationDirectory.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)));
          this.DestinationDirectory.Location = new System.Drawing.Point (5, 5);
          this.DestinationDirectory.Name = "DestinationDirectory";
          this.DestinationDirectory.Size = new System.Drawing.Size (492, 20);
          this.DestinationDirectory.TabIndex = 30;
          this.DestinationDirectory.Validating += new System.ComponentModel.CancelEventHandler (this.DestinationDirectory_Validating);
          // 
          // Test2DataPercentage
          // 
          this.Test2DataPercentage.Location = new System.Drawing.Point (148, 68);
          this.Test2DataPercentage.Name = "Test2DataPercentage";
          this.Test2DataPercentage.Size = new System.Drawing.Size (57, 20);
          this.Test2DataPercentage.TabIndex = 26;
          this.Test2DataPercentage.Validating += new System.ComponentModel.CancelEventHandler (this.Test2DataPercentage_Validating);
          // 
          // ValidaionDataPercentage
          // 
          this.ValidaionDataPercentage.Location = new System.Drawing.Point (148, 95);
          this.ValidaionDataPercentage.Name = "ValidaionDataPercentage";
          this.ValidaionDataPercentage.Size = new System.Drawing.Size (57, 20);
          this.ValidaionDataPercentage.TabIndex = 28;
          this.ValidaionDataPercentage.Validating += new System.ComponentModel.CancelEventHandler (this.ValidaionDataPercentage_Validating);
          // 
          // panel2
          // 
          this.panel2.Controls.Add (this.TrainingMaxImagesPerClass);
          this.panel2.Controls.Add (this.TraiingMaxImagsPerClassLabel);
          this.panel2.Controls.Add (this.Training2DataPercentage);
          this.panel2.Location = new System.Drawing.Point (148, 34);
          this.panel2.Name = "panel2";
          this.panel2.Size = new System.Drawing.Size (585, 27);
          this.panel2.TabIndex = 14;
          // 
          // TrainingMaxImagesPerClass
          // 
          this.TrainingMaxImagesPerClass.Location = new System.Drawing.Point (207, 3);
          this.TrainingMaxImagesPerClass.Name = "TrainingMaxImagesPerClass";
          this.TrainingMaxImagesPerClass.Size = new System.Drawing.Size (57, 20);
          this.TrainingMaxImagesPerClass.TabIndex = 23;
          this.TrainingMaxImagesPerClass.Validating += new System.ComponentModel.CancelEventHandler (this.TrainingMaxImagesPerClass_Validating);
          // 
          // TraiingMaxImagsPerClassLabel
          // 
          this.TraiingMaxImagsPerClassLabel.AutoSize = true;
          this.TraiingMaxImagsPerClassLabel.Location = new System.Drawing.Point (97, 7);
          this.TraiingMaxImagsPerClassLabel.Name = "TraiingMaxImagsPerClassLabel";
          this.TraiingMaxImagsPerClassLabel.Size = new System.Drawing.Size (104, 13);
          this.TraiingMaxImagsPerClassLabel.TabIndex = 5;
          this.TraiingMaxImagsPerClassLabel.Text = "Max Imags per Class";
          // 
          // Training2DataPercentage
          // 
          this.Training2DataPercentage.Location = new System.Drawing.Point (5, 3);
          this.Training2DataPercentage.Name = "Training2DataPercentage";
          this.Training2DataPercentage.Size = new System.Drawing.Size (57, 20);
          this.Training2DataPercentage.TabIndex = 22;
          this.Training2DataPercentage.Validating += new System.ComponentModel.CancelEventHandler (this.Training2DataPercentage_Validating);
          // 
          // CreateFeatureFiles
          // 
          this.CreateFeatureFiles.Location = new System.Drawing.Point (24, 559);
          this.CreateFeatureFiles.Name = "CreateFeatureFiles";
          this.CreateFeatureFiles.Size = new System.Drawing.Size (115, 23);
          this.CreateFeatureFiles.TabIndex = 50;
          this.CreateFeatureFiles.TabStop = false;
          this.CreateFeatureFiles.Text = "Create Feature Files";
          this.CreateFeatureFiles.UseVisualStyleBackColor = true;
          this.CreateFeatureFiles.Click += new System.EventHandler (this.CreateFeatureFiles_Click);
          // 
          // CancelCreateFeatureSelectionButton
          // 
          this.CancelCreateFeatureSelectionButton.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
          this.CancelCreateFeatureSelectionButton.Enabled = false;
          this.CancelCreateFeatureSelectionButton.Location = new System.Drawing.Point (688, 559);
          this.CancelCreateFeatureSelectionButton.Name = "CancelCreateFeatureSelectionButton";
          this.CancelCreateFeatureSelectionButton.Size = new System.Drawing.Size (75, 23);
          this.CancelCreateFeatureSelectionButton.TabIndex = 52;
          this.CancelCreateFeatureSelectionButton.TabStop = false;
          this.CancelCreateFeatureSelectionButton.Text = "Cancel";
          this.CancelCreateFeatureSelectionButton.UseVisualStyleBackColor = true;
          this.CancelCreateFeatureSelectionButton.Click += new System.EventHandler (this.CancelButton_Click);
          // 
          // Dialog
          // 
          this.Dialog.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
          this.Dialog.Location = new System.Drawing.Point (24, 228);
          this.Dialog.Multiline = true;
          this.Dialog.Name = "Dialog";
          this.Dialog.ReadOnly = true;
          this.Dialog.Size = new System.Drawing.Size (739, 308);
          this.Dialog.TabIndex = 10;
          this.Dialog.TabStop = false;
          // 
          // DialogTimer
          // 
          this.DialogTimer.Tick += new System.EventHandler (this.DialogTimer_Tick);
          // 
          // errorProvider1
          // 
          this.errorProvider1.ContainerControl = this;
          // 
          // RunLogLabel
          // 
          this.RunLogLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.RunLogLabel.AutoSize = true;
          this.RunLogLabel.Location = new System.Drawing.Point (21, 210);
          this.RunLogLabel.Name = "RunLogLabel";
          this.RunLogLabel.Padding = new System.Windows.Forms.Padding (2);
          this.RunLogLabel.Size = new System.Drawing.Size (52, 17);
          this.RunLogLabel.TabIndex = 11;
          this.RunLogLabel.Text = "Run Log";
          this.RunLogLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
          // 
          // CreateFeatureSelectionDataSets
          // 
          this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
          this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
          this.ClientSize = new System.Drawing.Size (789, 591);
          this.Controls.Add (this.RunLogLabel);
          this.Controls.Add (this.Dialog);
          this.Controls.Add (this.CancelCreateFeatureSelectionButton);
          this.Controls.Add (this.CreateFeatureFiles);
          this.Controls.Add (this.tableLayoutPanel1);
          this.Controls.Add (this.TitleLabel);
          this.MaximumSize = new System.Drawing.Size (805, 629);
          this.MinimumSize = new System.Drawing.Size (805, 629);
          this.Name = "CreateFeatureSelectionDataSets";
          this.Text = "CreateFeatureSelectionDataSets";
          this.Load += new System.EventHandler (this.CreateFeatureSelectionDataSets_Load);
          this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.CreateFeatureSelectionDataSets_FormClosing);
          this.tableLayoutPanel1.ResumeLayout (false);
          this.tableLayoutPanel1.PerformLayout ();
          this.panel1.ResumeLayout (false);
          this.panel1.PerformLayout ();
          this.panel2.ResumeLayout (false);
          this.panel2.PerformLayout ();
          ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
          this.ResumeLayout (false);
          this.PerformLayout ();

        }

        #endregion

        private System.Windows.Forms.Label TitleLabel;
        private System.Windows.Forms.Label ModelNamLabel;
        private System.Windows.Forms.ComboBox ModelName;
        private System.Windows.Forms.Label TrainingDataPercentageLabel;
        private System.Windows.Forms.Label TestDataPercentageLabel;
        private System.Windows.Forms.Label ValidationDataPercentageLabel2;
        private System.Windows.Forms.Label DestinationDirectoryLabel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TextBox DestinationDirectory;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button DirectoryBrowseButton;
        private System.Windows.Forms.Button CreateFeatureFiles;
        private System.Windows.Forms.Button CancelCreateFeatureSelectionButton;
        private System.Windows.Forms.TextBox Dialog;
        private System.Windows.Forms.Timer DialogTimer;
        private System.Windows.Forms.TextBox Training2DataPercentage;
        private System.Windows.Forms.TextBox Test2DataPercentage;
        private System.Windows.Forms.TextBox ValidaionDataPercentage;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TextBox TrainingMaxImagesPerClass;
        private System.Windows.Forms.Label TraiingMaxImagsPerClassLabel;
        private System.Windows.Forms.ErrorProvider errorProvider1;
        private System.Windows.Forms.Label RunLogLabel;
    }
}