namespace PicesCommander
{
  partial class SaveImagesToDisk
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
      this.ImageCountlabel = new System.Windows.Forms.Label ();
      this.ImageCount = new System.Windows.Forms.TextBox ();
      this.CriteriaPanel = new System.Windows.Forms.TableLayoutPanel ();
      this.FailedToExtractLabel = new System.Windows.Forms.Label ();
      this.ImagesSavedLabel = new System.Windows.Forms.Label ();
      this.CurDirLabel = new System.Windows.Forms.Label ();
      this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel ();
      this.ByDepthIncrements = new System.Windows.Forms.RadioButton ();
      this.ByClass = new System.Windows.Forms.RadioButton ();
      this.IncrementLabel = new System.Windows.Forms.Label ();
      this.DestinationDiirectoryLabel = new System.Windows.Forms.Label ();
      this.panel1 = new System.Windows.Forms.Panel ();
      this.DestinationDirectory = new System.Windows.Forms.TextBox ();
      this.BrowseDestinationDir = new System.Windows.Forms.Button ();
      this.DirectoryStructureLabel = new System.Windows.Forms.Label ();
      this.Increments = new System.Windows.Forms.ComboBox ();
      this.CurrentDirectory = new System.Windows.Forms.TextBox ();
      this.ImagesSaved = new System.Windows.Forms.TextBox ();
      this.ImagesFailedToExtract = new System.Windows.Forms.TextBox ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.StartButton = new System.Windows.Forms.Button ();
      this.CancelSaveButton = new System.Windows.Forms.Button ();
      this.savingTimer = new System.Windows.Forms.Timer (this.components);
      this.textBox1 = new System.Windows.Forms.TextBox ();
      this.CriteriaPanel.SuspendLayout ();
      this.flowLayoutPanel1.SuspendLayout ();
      this.panel1.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (268, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (185, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Save Imags to Disk";
      // 
      // ImageCountlabel
      // 
      this.ImageCountlabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.ImageCountlabel.AutoSize = true;
      this.ImageCountlabel.Location = new System.Drawing.Point (4, 1);
      this.ImageCountlabel.Name = "ImageCountlabel";
      this.ImageCountlabel.Size = new System.Drawing.Size (67, 32);
      this.ImageCountlabel.TabIndex = 1;
      this.ImageCountlabel.Text = "Image Count";
      this.ImageCountlabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ImageCount
      // 
      this.ImageCount.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.ImageCount.CausesValidation = false;
      this.ImageCount.Font = new System.Drawing.Font ("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ImageCount.Location = new System.Drawing.Point (151, 4);
      this.ImageCount.Name = "ImageCount";
      this.ImageCount.ReadOnly = true;
      this.ImageCount.Size = new System.Drawing.Size (100, 21);
      this.ImageCount.TabIndex = 2;
      this.ImageCount.TabStop = false;
      // 
      // CriteriaPanel
      // 
      this.CriteriaPanel.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
      this.CriteriaPanel.ColumnCount = 2;
      this.CriteriaPanel.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 21.21212F));
      this.CriteriaPanel.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 78.78788F));
      this.CriteriaPanel.Controls.Add (this.FailedToExtractLabel, 0, 6);
      this.CriteriaPanel.Controls.Add (this.ImagesSavedLabel, 0, 5);
      this.CriteriaPanel.Controls.Add (this.CurDirLabel, 0, 4);
      this.CriteriaPanel.Controls.Add (this.flowLayoutPanel1, 1, 2);
      this.CriteriaPanel.Controls.Add (this.IncrementLabel, 0, 3);
      this.CriteriaPanel.Controls.Add (this.DestinationDiirectoryLabel, 0, 1);
      this.CriteriaPanel.Controls.Add (this.ImageCountlabel, 0, 0);
      this.CriteriaPanel.Controls.Add (this.panel1, 1, 1);
      this.CriteriaPanel.Controls.Add (this.ImageCount, 1, 0);
      this.CriteriaPanel.Controls.Add (this.DirectoryStructureLabel, 0, 2);
      this.CriteriaPanel.Controls.Add (this.Increments, 1, 3);
      this.CriteriaPanel.Controls.Add (this.CurrentDirectory, 1, 4);
      this.CriteriaPanel.Controls.Add (this.ImagesSaved, 1, 5);
      this.CriteriaPanel.Controls.Add (this.ImagesFailedToExtract, 1, 6);
      this.CriteriaPanel.Location = new System.Drawing.Point (12, 119);
      this.CriteriaPanel.Name = "CriteriaPanel";
      this.CriteriaPanel.RowCount = 7;
      this.CriteriaPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 32F));
      this.CriteriaPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 34F));
      this.CriteriaPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 35F));
      this.CriteriaPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 32F));
      this.CriteriaPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 30F));
      this.CriteriaPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 31F));
      this.CriteriaPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 12F));
      this.CriteriaPanel.Size = new System.Drawing.Size (696, 232);
      this.CriteriaPanel.TabIndex = 3;
      // 
      // FailedToExtractLabel
      // 
      this.FailedToExtractLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.FailedToExtractLabel.AutoSize = true;
      this.FailedToExtractLabel.Location = new System.Drawing.Point (4, 201);
      this.FailedToExtractLabel.Name = "FailedToExtractLabel";
      this.FailedToExtractLabel.Size = new System.Drawing.Size (83, 30);
      this.FailedToExtractLabel.TabIndex = 15;
      this.FailedToExtractLabel.Text = "Failed to Extract";
      this.FailedToExtractLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ImagesSavedLabel
      // 
      this.ImagesSavedLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.ImagesSavedLabel.AutoSize = true;
      this.ImagesSavedLabel.Location = new System.Drawing.Point (4, 169);
      this.ImagesSavedLabel.Name = "ImagesSavedLabel";
      this.ImagesSavedLabel.Size = new System.Drawing.Size (75, 31);
      this.ImagesSavedLabel.TabIndex = 13;
      this.ImagesSavedLabel.Text = "Images Saved";
      this.ImagesSavedLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // CurDirLabel
      // 
      this.CurDirLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.CurDirLabel.AutoSize = true;
      this.CurDirLabel.Location = new System.Drawing.Point (4, 138);
      this.CurDirLabel.Name = "CurDirLabel";
      this.CurDirLabel.Size = new System.Drawing.Size (57, 30);
      this.CurDirLabel.TabIndex = 11;
      this.CurDirLabel.Text = "Current Dir";
      this.CurDirLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // flowLayoutPanel1
      // 
      this.flowLayoutPanel1.Controls.Add (this.ByDepthIncrements);
      this.flowLayoutPanel1.Controls.Add (this.ByClass);
      this.flowLayoutPanel1.Location = new System.Drawing.Point (151, 72);
      this.flowLayoutPanel1.Name = "flowLayoutPanel1";
      this.flowLayoutPanel1.Size = new System.Drawing.Size (194, 29);
      this.flowLayoutPanel1.TabIndex = 5;
      // 
      // ByDepthIncrements
      // 
      this.ByDepthIncrements.AutoSize = true;
      this.ByDepthIncrements.Checked = true;
      this.ByDepthIncrements.Location = new System.Drawing.Point (3, 3);
      this.ByDepthIncrements.Name = "ByDepthIncrements";
      this.ByDepthIncrements.Padding = new System.Windows.Forms.Padding (2);
      this.ByDepthIncrements.Size = new System.Drawing.Size (113, 21);
      this.ByDepthIncrements.TabIndex = 6;
      this.ByDepthIncrements.TabStop = true;
      this.ByDepthIncrements.Text = "Depth Increments";
      this.ByDepthIncrements.UseVisualStyleBackColor = true;
      this.ByDepthIncrements.CheckedChanged += new System.EventHandler (this.ByDepthIncrements_CheckedChanged);
      // 
      // ByClass
      // 
      this.ByClass.AutoSize = true;
      this.ByClass.Location = new System.Drawing.Point (122, 3);
      this.ByClass.Name = "ByClass";
      this.ByClass.Padding = new System.Windows.Forms.Padding (2);
      this.ByClass.Size = new System.Drawing.Size (54, 21);
      this.ByClass.TabIndex = 8;
      this.ByClass.Text = "Class";
      this.ByClass.UseVisualStyleBackColor = true;
      this.ByClass.CheckedChanged += new System.EventHandler (this.ByClass_CheckedChanged);
      // 
      // IncrementLabel
      // 
      this.IncrementLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.IncrementLabel.AutoSize = true;
      this.IncrementLabel.Location = new System.Drawing.Point (4, 105);
      this.IncrementLabel.Name = "IncrementLabel";
      this.IncrementLabel.Size = new System.Drawing.Size (59, 32);
      this.IncrementLabel.TabIndex = 5;
      this.IncrementLabel.Text = "Increments";
      this.IncrementLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // DestinationDiirectoryLabel
      // 
      this.DestinationDiirectoryLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DestinationDiirectoryLabel.AutoSize = true;
      this.DestinationDiirectoryLabel.Location = new System.Drawing.Point (4, 34);
      this.DestinationDiirectoryLabel.Name = "DestinationDiirectoryLabel";
      this.DestinationDiirectoryLabel.Size = new System.Drawing.Size (105, 34);
      this.DestinationDiirectoryLabel.TabIndex = 3;
      this.DestinationDiirectoryLabel.Text = "Destination Directory";
      this.DestinationDiirectoryLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // panel1
      // 
      this.panel1.Controls.Add (this.DestinationDirectory);
      this.panel1.Controls.Add (this.BrowseDestinationDir);
      this.panel1.Location = new System.Drawing.Point (151, 37);
      this.panel1.Name = "panel1";
      this.panel1.Size = new System.Drawing.Size (541, 27);
      this.panel1.TabIndex = 7;
      // 
      // DestinationDirectory
      // 
      this.DestinationDirectory.Location = new System.Drawing.Point (3, 3);
      this.DestinationDirectory.Name = "DestinationDirectory";
      this.DestinationDirectory.Size = new System.Drawing.Size (457, 20);
      this.DestinationDirectory.TabIndex = 4;
      // 
      // BrowseDestinationDir
      // 
      this.BrowseDestinationDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.BrowseDestinationDir.Location = new System.Drawing.Point (466, 3);
      this.BrowseDestinationDir.Name = "BrowseDestinationDir";
      this.BrowseDestinationDir.Size = new System.Drawing.Size (75, 21);
      this.BrowseDestinationDir.TabIndex = 7;
      this.BrowseDestinationDir.Text = "Browse";
      this.BrowseDestinationDir.UseVisualStyleBackColor = true;
      this.BrowseDestinationDir.Click += new System.EventHandler (this.BrowseDestinationDir_Click);
      // 
      // DirectoryStructureLabel
      // 
      this.DirectoryStructureLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.DirectoryStructureLabel.AutoSize = true;
      this.DirectoryStructureLabel.Location = new System.Drawing.Point (4, 69);
      this.DirectoryStructureLabel.Name = "DirectoryStructureLabel";
      this.DirectoryStructureLabel.Size = new System.Drawing.Size (95, 35);
      this.DirectoryStructureLabel.TabIndex = 4;
      this.DirectoryStructureLabel.Text = "Directory Structure";
      this.DirectoryStructureLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // Increments
      // 
      this.Increments.FormattingEnabled = true;
      this.Increments.Items.AddRange (new object[] {
            "1 Meter",
            "2 Meter",
            "5 Meter",
            "10 Meter",
            "20 Meter",
            "50 Meter",
            "100 Meter"});
      this.Increments.Location = new System.Drawing.Point (151, 108);
      this.Increments.Name = "Increments";
      this.Increments.Size = new System.Drawing.Size (121, 21);
      this.Increments.TabIndex = 10;
      // 
      // CurrentDirectory
      // 
      this.CurrentDirectory.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.CurrentDirectory.Location = new System.Drawing.Point (151, 141);
      this.CurrentDirectory.Name = "CurrentDirectory";
      this.CurrentDirectory.ReadOnly = true;
      this.CurrentDirectory.Size = new System.Drawing.Size (541, 20);
      this.CurrentDirectory.TabIndex = 12;
      // 
      // ImagesSaved
      // 
      this.ImagesSaved.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.ImagesSaved.CausesValidation = false;
      this.ImagesSaved.Font = new System.Drawing.Font ("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ImagesSaved.Location = new System.Drawing.Point (151, 172);
      this.ImagesSaved.Name = "ImagesSaved";
      this.ImagesSaved.ReadOnly = true;
      this.ImagesSaved.Size = new System.Drawing.Size (121, 21);
      this.ImagesSaved.TabIndex = 14;
      this.ImagesSaved.TabStop = false;
      // 
      // ImagesFailedToExtract
      // 
      this.ImagesFailedToExtract.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                  | System.Windows.Forms.AnchorStyles.Left)));
      this.ImagesFailedToExtract.CausesValidation = false;
      this.ImagesFailedToExtract.Font = new System.Drawing.Font ("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ImagesFailedToExtract.Location = new System.Drawing.Point (151, 204);
      this.ImagesFailedToExtract.Name = "ImagesFailedToExtract";
      this.ImagesFailedToExtract.ReadOnly = true;
      this.ImagesFailedToExtract.Size = new System.Drawing.Size (121, 21);
      this.ImagesFailedToExtract.TabIndex = 16;
      this.ImagesFailedToExtract.TabStop = false;
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // StartButton
      // 
      this.StartButton.Location = new System.Drawing.Point (12, 404);
      this.StartButton.Name = "StartButton";
      this.StartButton.Size = new System.Drawing.Size (75, 23);
      this.StartButton.TabIndex = 4;
      this.StartButton.Text = "Start";
      this.StartButton.UseVisualStyleBackColor = true;
      this.StartButton.Click += new System.EventHandler (this.StartButton_Click);
      // 
      // CancelSaveButton
      // 
      this.CancelSaveButton.Location = new System.Drawing.Point (633, 404);
      this.CancelSaveButton.Name = "CancelSaveButton";
      this.CancelSaveButton.Size = new System.Drawing.Size (75, 23);
      this.CancelSaveButton.TabIndex = 5;
      this.CancelSaveButton.Text = "Cancel";
      this.CancelSaveButton.UseVisualStyleBackColor = true;
      this.CancelSaveButton.Click += new System.EventHandler (this.CancelSaveButton_Click);
      // 
      // savingTimer
      // 
      this.savingTimer.Tick += new System.EventHandler (this.savingTimer_Tick);
      // 
      // textBox1
      // 
      this.textBox1.Location = new System.Drawing.Point (166, 53);
      this.textBox1.Multiline = true;
      this.textBox1.Name = "textBox1";
      this.textBox1.ReadOnly = true;
      this.textBox1.Size = new System.Drawing.Size (418, 37);
      this.textBox1.TabIndex = 6;
      this.textBox1.TabStop = false;
      this.textBox1.Text = "This function wil save all the images that are currently in the Thumbnail Display" +
          " to the directory that you specify.";
      // 
      // SaveImagesToDisk
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (720, 440);
      this.Controls.Add (this.textBox1);
      this.Controls.Add (this.CancelSaveButton);
      this.Controls.Add (this.StartButton);
      this.Controls.Add (this.CriteriaPanel);
      this.Controls.Add (this.TitleLabel);
      this.Name = "SaveImagesToDisk";
      this.Text = "SaveImagesToDisk";
      this.CriteriaPanel.ResumeLayout (false);
      this.CriteriaPanel.PerformLayout ();
      this.flowLayoutPanel1.ResumeLayout (false);
      this.flowLayoutPanel1.PerformLayout ();
      this.panel1.ResumeLayout (false);
      this.panel1.PerformLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label ImageCountlabel;
    private System.Windows.Forms.TextBox ImageCount;
    private System.Windows.Forms.TableLayoutPanel CriteriaPanel;
    private System.Windows.Forms.Label DestinationDiirectoryLabel;
    private System.Windows.Forms.Label DirectoryStructureLabel;
    private System.Windows.Forms.TextBox DestinationDirectory;
    private System.Windows.Forms.Button BrowseDestinationDir;
    private System.Windows.Forms.Panel panel1;
    private System.Windows.Forms.Label IncrementLabel;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
    private System.Windows.Forms.RadioButton ByDepthIncrements;
    private System.Windows.Forms.RadioButton ByClass;
    private System.Windows.Forms.ComboBox Increments;
    private System.Windows.Forms.Label CurDirLabel;
    private System.Windows.Forms.TextBox CurrentDirectory;
    private System.Windows.Forms.Button CancelSaveButton;
    private System.Windows.Forms.Button StartButton;
    private System.Windows.Forms.Label ImagesSavedLabel;
    private System.Windows.Forms.TextBox ImagesSaved;
    private System.Windows.Forms.Timer savingTimer;
    private System.Windows.Forms.Label FailedToExtractLabel;
    private System.Windows.Forms.TextBox ImagesFailedToExtract;
    private System.Windows.Forms.TextBox textBox1;
  }
}