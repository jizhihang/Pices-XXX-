namespace PicesCommander
{
    partial class SaveActiveTrainingModelFeatureData
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
          this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel ();
          this.FormatLabel = new System.Windows.Forms.Label ();
          this.panel1 = new System.Windows.Forms.Panel ();
          this.DestinationBrowseButton = new System.Windows.Forms.Button ();
          this.DestinationFileName = new System.Windows.Forms.TextBox ();
          this.DestinationFleNameLabel = new System.Windows.Forms.Label ();
          this.FileFormat = new System.Windows.Forms.ComboBox ();
          this.CreateFeatureFiles = new System.Windows.Forms.Button ();
          this.CancelCreateFeatureSelectionButton = new System.Windows.Forms.Button ();
          this.Dialog = new System.Windows.Forms.TextBox ();
          this.DialogTimer = new System.Windows.Forms.Timer (this.components);
          this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
          this.RunLogLabel = new System.Windows.Forms.Label ();
          this.textBox1 = new System.Windows.Forms.TextBox ();
          this.tableLayoutPanel1.SuspendLayout ();
          this.panel1.SuspendLayout ();
          ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
          this.SuspendLayout ();
          // 
          // TitleLabel
          // 
          this.TitleLabel.AutoSize = true;
          this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
          this.TitleLabel.Location = new System.Drawing.Point (250, 9);
          this.TitleLabel.Name = "TitleLabel";
          this.TitleLabel.Size = new System.Drawing.Size (296, 24);
          this.TitleLabel.TabIndex = 0;
          this.TitleLabel.Text = "Save Training Model Feature Data";
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
          this.ModelName.Size = new System.Drawing.Size (290, 21);
          this.ModelName.TabIndex = 20;
          this.ModelName.Validating += new System.ComponentModel.CancelEventHandler (this.ModelName_Validating);
          this.ModelName.SelectedIndexChanged += new System.EventHandler (this.ModelName_SelectedIndexChanged);
          // 
          // tableLayoutPanel1
          // 
          this.tableLayoutPanel1.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
          this.tableLayoutPanel1.ColumnCount = 2;
          this.tableLayoutPanel1.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 19.34605F));
          this.tableLayoutPanel1.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 80.65395F));
          this.tableLayoutPanel1.Controls.Add (this.FormatLabel, 0, 2);
          this.tableLayoutPanel1.Controls.Add (this.panel1, 1, 1);
          this.tableLayoutPanel1.Controls.Add (this.ModelNamLabel, 0, 0);
          this.tableLayoutPanel1.Controls.Add (this.ModelName, 1, 0);
          this.tableLayoutPanel1.Controls.Add (this.DestinationFleNameLabel, 0, 1);
          this.tableLayoutPanel1.Controls.Add (this.FileFormat, 1, 2);
          this.tableLayoutPanel1.Location = new System.Drawing.Point (24, 155);
          this.tableLayoutPanel1.Name = "tableLayoutPanel1";
          this.tableLayoutPanel1.Padding = new System.Windows.Forms.Padding (2);
          this.tableLayoutPanel1.RowCount = 3;
          this.tableLayoutPanel1.RowStyles.Add (new System.Windows.Forms.RowStyle ());
          this.tableLayoutPanel1.RowStyles.Add (new System.Windows.Forms.RowStyle ());
          this.tableLayoutPanel1.RowStyles.Add (new System.Windows.Forms.RowStyle ());
          this.tableLayoutPanel1.Size = new System.Drawing.Size (739, 108);
          this.tableLayoutPanel1.TabIndex = 7;
          // 
          // FormatLabel
          // 
          this.FormatLabel.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
          this.FormatLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.FormatLabel.AutoSize = true;
          this.FormatLabel.Location = new System.Drawing.Point (6, 72);
          this.FormatLabel.Name = "FormatLabel";
          this.FormatLabel.Padding = new System.Windows.Forms.Padding (2);
          this.FormatLabel.Size = new System.Drawing.Size (135, 33);
          this.FormatLabel.TabIndex = 21;
          this.FormatLabel.Text = "File Format";
          this.FormatLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
          // 
          // panel1
          // 
          this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.panel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
          this.panel1.Controls.Add (this.DestinationBrowseButton);
          this.panel1.Controls.Add (this.DestinationFileName);
          this.panel1.Location = new System.Drawing.Point (148, 34);
          this.panel1.Name = "panel1";
          this.panel1.Padding = new System.Windows.Forms.Padding (2);
          this.panel1.Size = new System.Drawing.Size (585, 34);
          this.panel1.TabIndex = 10;
          // 
          // DestinationBrowseButton
          // 
          this.DestinationBrowseButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.DestinationBrowseButton.Location = new System.Drawing.Point (503, 4);
          this.DestinationBrowseButton.Name = "DestinationBrowseButton";
          this.DestinationBrowseButton.Size = new System.Drawing.Size (75, 26);
          this.DestinationBrowseButton.TabIndex = 31;
          this.DestinationBrowseButton.Text = "Browse";
          this.DestinationBrowseButton.UseVisualStyleBackColor = true;
          this.DestinationBrowseButton.Click += new System.EventHandler (this.DestinationBrowseButton_Click);
          // 
          // DestinationFileName
          // 
          this.DestinationFileName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)));
          this.DestinationFileName.Location = new System.Drawing.Point (5, 7);
          this.DestinationFileName.Name = "DestinationFileName";
          this.DestinationFileName.Size = new System.Drawing.Size (492, 20);
          this.DestinationFileName.TabIndex = 30;
          this.DestinationFileName.Validating += new System.ComponentModel.CancelEventHandler (this.DestinationFileName_Validating);
          // 
          // DestinationFleNameLabel
          // 
          this.DestinationFleNameLabel.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
          this.DestinationFleNameLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                      | System.Windows.Forms.AnchorStyles.Left)
                      | System.Windows.Forms.AnchorStyles.Right)));
          this.DestinationFleNameLabel.AutoSize = true;
          this.DestinationFleNameLabel.Location = new System.Drawing.Point (6, 31);
          this.DestinationFleNameLabel.Name = "DestinationFleNameLabel";
          this.DestinationFleNameLabel.Padding = new System.Windows.Forms.Padding (2);
          this.DestinationFleNameLabel.Size = new System.Drawing.Size (135, 40);
          this.DestinationFleNameLabel.TabIndex = 10;
          this.DestinationFleNameLabel.Text = "Destinaion File Name";
          this.DestinationFleNameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
          // 
          // FileFormat
          // 
          this.FileFormat.FormattingEnabled = true;
          this.FileFormat.Location = new System.Drawing.Point (148, 75);
          this.FileFormat.Name = "FileFormat";
          this.FileFormat.Size = new System.Drawing.Size (290, 21);
          this.FileFormat.TabIndex = 22;
          this.FileFormat.Validating += new System.ComponentModel.CancelEventHandler (this.FileFormat_Validating);
          this.FileFormat.SelectedIndexChanged += new System.EventHandler (this.FileFormat_SelectedIndexChanged);
          // 
          // CreateFeatureFiles
          // 
          this.CreateFeatureFiles.Location = new System.Drawing.Point (24, 559);
          this.CreateFeatureFiles.Name = "CreateFeatureFiles";
          this.CreateFeatureFiles.Size = new System.Drawing.Size (141, 23);
          this.CreateFeatureFiles.TabIndex = 50;
          this.CreateFeatureFiles.TabStop = false;
          this.CreateFeatureFiles.Text = "Create Feature Data File";
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
          this.Dialog.Location = new System.Drawing.Point (24, 295);
          this.Dialog.Multiline = true;
          this.Dialog.Name = "Dialog";
          this.Dialog.ReadOnly = true;
          this.Dialog.Size = new System.Drawing.Size (739, 241);
          this.Dialog.TabIndex = 10;
          this.Dialog.TabStop = false;
          // 
          // DialogTimer
          // 
          this.DialogTimer.Enabled = true;
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
          this.RunLogLabel.Location = new System.Drawing.Point (21, 276);
          this.RunLogLabel.Name = "RunLogLabel";
          this.RunLogLabel.Padding = new System.Windows.Forms.Padding (2);
          this.RunLogLabel.Size = new System.Drawing.Size (52, 17);
          this.RunLogLabel.TabIndex = 11;
          this.RunLogLabel.Text = "Run Log";
          this.RunLogLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
          // 
          // textBox1
          // 
          this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
          this.textBox1.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
          this.textBox1.Location = new System.Drawing.Point (24, 49);
          this.textBox1.Multiline = true;
          this.textBox1.Name = "textBox1";
          this.textBox1.ReadOnly = true;
          this.textBox1.Size = new System.Drawing.Size (739, 79);
          this.textBox1.TabIndex = 53;
          this.textBox1.TabStop = false;
          this.textBox1.Text = "This function will create a Feature Data file containing feature vectors for all " +
              "the training examples in the specified training library.  ";
          // 
          // SaveActiveTrainingModelFeatureData
          // 
          this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
          this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
          this.ClientSize = new System.Drawing.Size (789, 591);
          this.Controls.Add (this.textBox1);
          this.Controls.Add (this.RunLogLabel);
          this.Controls.Add (this.Dialog);
          this.Controls.Add (this.CancelCreateFeatureSelectionButton);
          this.Controls.Add (this.CreateFeatureFiles);
          this.Controls.Add (this.tableLayoutPanel1);
          this.Controls.Add (this.TitleLabel);
          this.MaximumSize = new System.Drawing.Size (805, 629);
          this.MinimumSize = new System.Drawing.Size (805, 629);
          this.Name = "SaveActiveTrainingModelFeatureData";
          this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
          this.Text = "SaveActiveTrainingModelFeatureData";
          this.Load += new System.EventHandler (this.SaveActiveTrainingModelFeatureData_Load);
          this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.SaveActiveTrainingModelFeatureData_FormClosing);
          this.tableLayoutPanel1.ResumeLayout (false);
          this.tableLayoutPanel1.PerformLayout ();
          this.panel1.ResumeLayout (false);
          this.panel1.PerformLayout ();
          ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
          this.ResumeLayout (false);
          this.PerformLayout ();

        }

        #endregion

        private System.Windows.Forms.Label TitleLabel;
        private System.Windows.Forms.Label ModelNamLabel;
        private System.Windows.Forms.ComboBox ModelName;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Button CreateFeatureFiles;
        private System.Windows.Forms.Button CancelCreateFeatureSelectionButton;
        private System.Windows.Forms.TextBox Dialog;
        private System.Windows.Forms.Timer DialogTimer;
        private System.Windows.Forms.ErrorProvider errorProvider1;
        private System.Windows.Forms.Label RunLogLabel;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button DestinationBrowseButton;
        private System.Windows.Forms.TextBox DestinationFileName;
        private System.Windows.Forms.Label DestinationFleNameLabel;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label FormatLabel;
        private System.Windows.Forms.ComboBox FileFormat;
    }
}