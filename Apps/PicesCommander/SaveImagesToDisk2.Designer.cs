namespace PicesCommander
{
  partial class SaveImagesToDisk2
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
      this.FailedToExtractLabel = new System.Windows.Forms.Label ();
      this.ImagesSavedLabel = new System.Windows.Forms.Label ();
      this.CurDirLabel = new System.Windows.Forms.Label ();
      this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel ();
      this.ByDepthIncrements = new System.Windows.Forms.RadioButton ();
      this.ByClass = new System.Windows.Forms.RadioButton ();
      this.IncrementLabel = new System.Windows.Forms.Label ();
      this.DestinationDiirectoryLabel = new System.Windows.Forms.Label ();
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
      this.RunTimeParameters = new System.Windows.Forms.Panel ();
      this.RunTimeStatus = new System.Windows.Forms.Panel ();
      this.SaveProgress = new System.Windows.Forms.ProgressBar ();
      this.SipperFileSatus = new System.Windows.Forms.TextBox ();
      this.SipperFileStatusLabel = new System.Windows.Forms.Label ();
      this.CruiseLabel = new System.Windows.Forms.Label ();
      this.SipperFileLabel = new System.Windows.Forms.Label ();
      this.DeploymentLabel = new System.Windows.Forms.Label ();
      this.StationsLabel = new System.Windows.Forms.Label ();
      this.CruiseName = new System.Windows.Forms.TextBox ();
      this.StationName = new System.Windows.Forms.TextBox ();
      this.DeploymentNum = new System.Windows.Forms.TextBox ();
      this.SipperFile = new System.Windows.Forms.TextBox ();
      this.SelectionCriteria = new System.Windows.Forms.Panel ();
      this.DepthLabel = new System.Windows.Forms.Label ();
      this.DepthMax = new System.Windows.Forms.TextBox ();
      this.DepthMin = new System.Windows.Forms.TextBox ();
      this.SizeMax = new System.Windows.Forms.TextBox ();
      this.SizeMin = new System.Windows.Forms.TextBox ();
      this.SizeLabel = new System.Windows.Forms.Label ();
      this.ProbMax = new System.Windows.Forms.TextBox ();
      this.ProbMin = new System.Windows.Forms.TextBox ();
      this.ProbabilityLabel = new System.Windows.Forms.Label ();
      this.GroupName = new System.Windows.Forms.TextBox ();
      this.GroupNameLabel = new System.Windows.Forms.Label ();
      this.SelectionCriteriaLabel = new System.Windows.Forms.Label ();
      this.RunTimeParametersLabel = new System.Windows.Forms.Label ();
      this.RunTimeStatusLabel = new System.Windows.Forms.Label ();
      this.StatsTimer = new System.Windows.Forms.Timer (this.components);
      this.flowLayoutPanel1.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.RunTimeParameters.SuspendLayout ();
      this.RunTimeStatus.SuspendLayout ();
      this.SelectionCriteria.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (262, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (197, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Save Images to Disk";
      // 
      // ImageCountlabel
      // 
      this.ImageCountlabel.AutoSize = true;
      this.ImageCountlabel.Location = new System.Drawing.Point (9, 114);
      this.ImageCountlabel.Name = "ImageCountlabel";
      this.ImageCountlabel.Size = new System.Drawing.Size (67, 13);
      this.ImageCountlabel.TabIndex = 1;
      this.ImageCountlabel.Text = "Image Count";
      this.ImageCountlabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ImageCount
      // 
      this.ImageCount.CausesValidation = false;
      this.ImageCount.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ImageCount.Location = new System.Drawing.Point (12, 130);
      this.ImageCount.Name = "ImageCount";
      this.ImageCount.ReadOnly = true;
      this.ImageCount.Size = new System.Drawing.Size (100, 22);
      this.ImageCount.TabIndex = 2;
      this.ImageCount.TabStop = false;
      this.ImageCount.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // FailedToExtractLabel
      // 
      this.FailedToExtractLabel.AutoSize = true;
      this.FailedToExtractLabel.Location = new System.Drawing.Point (329, 114);
      this.FailedToExtractLabel.Name = "FailedToExtractLabel";
      this.FailedToExtractLabel.Size = new System.Drawing.Size (83, 13);
      this.FailedToExtractLabel.TabIndex = 15;
      this.FailedToExtractLabel.Text = "Failed to Extract";
      this.FailedToExtractLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ImagesSavedLabel
      // 
      this.ImagesSavedLabel.AutoSize = true;
      this.ImagesSavedLabel.Location = new System.Drawing.Point (171, 114);
      this.ImagesSavedLabel.Name = "ImagesSavedLabel";
      this.ImagesSavedLabel.Size = new System.Drawing.Size (75, 13);
      this.ImagesSavedLabel.TabIndex = 13;
      this.ImagesSavedLabel.Text = "Images Saved";
      this.ImagesSavedLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // CurDirLabel
      // 
      this.CurDirLabel.AutoSize = true;
      this.CurDirLabel.Location = new System.Drawing.Point (7, 11);
      this.CurDirLabel.Name = "CurDirLabel";
      this.CurDirLabel.Size = new System.Drawing.Size (57, 13);
      this.CurDirLabel.TabIndex = 11;
      this.CurDirLabel.Text = "Current Dir";
      this.CurDirLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // flowLayoutPanel1
      // 
      this.flowLayoutPanel1.Anchor = System.Windows.Forms.AnchorStyles.None;
      this.flowLayoutPanel1.Controls.Add (this.ByDepthIncrements);
      this.flowLayoutPanel1.Controls.Add (this.ByClass);
      this.flowLayoutPanel1.Location = new System.Drawing.Point (9, 77);
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
      this.ByDepthIncrements.TabIndex = 10;
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
      this.ByClass.TabIndex = 12;
      this.ByClass.Text = "Class";
      this.ByClass.UseVisualStyleBackColor = true;
      this.ByClass.CheckedChanged += new System.EventHandler (this.ByClass_CheckedChanged);
      // 
      // IncrementLabel
      // 
      this.IncrementLabel.Anchor = System.Windows.Forms.AnchorStyles.None;
      this.IncrementLabel.AutoSize = true;
      this.IncrementLabel.Location = new System.Drawing.Point (241, 64);
      this.IncrementLabel.Name = "IncrementLabel";
      this.IncrementLabel.Size = new System.Drawing.Size (59, 13);
      this.IncrementLabel.TabIndex = 5;
      this.IncrementLabel.Text = "Increments";
      this.IncrementLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // DestinationDiirectoryLabel
      // 
      this.DestinationDiirectoryLabel.Anchor = System.Windows.Forms.AnchorStyles.None;
      this.DestinationDiirectoryLabel.AutoSize = true;
      this.DestinationDiirectoryLabel.Location = new System.Drawing.Point (7, 10);
      this.DestinationDiirectoryLabel.Name = "DestinationDiirectoryLabel";
      this.DestinationDiirectoryLabel.Size = new System.Drawing.Size (105, 13);
      this.DestinationDiirectoryLabel.TabIndex = 3;
      this.DestinationDiirectoryLabel.Text = "Destination Directory";
      this.DestinationDiirectoryLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // DestinationDirectory
      // 
      this.DestinationDirectory.Anchor = System.Windows.Forms.AnchorStyles.None;
      this.DestinationDirectory.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DestinationDirectory.Location = new System.Drawing.Point (10, 26);
      this.DestinationDirectory.Name = "DestinationDirectory";
      this.DestinationDirectory.Size = new System.Drawing.Size (556, 22);
      this.DestinationDirectory.TabIndex = 4;
      // 
      // BrowseDestinationDir
      // 
      this.BrowseDestinationDir.Anchor = System.Windows.Forms.AnchorStyles.None;
      this.BrowseDestinationDir.Location = new System.Drawing.Point (570, 26);
      this.BrowseDestinationDir.Name = "BrowseDestinationDir";
      this.BrowseDestinationDir.Size = new System.Drawing.Size (75, 20);
      this.BrowseDestinationDir.TabIndex = 7;
      this.BrowseDestinationDir.Text = "Browse";
      this.BrowseDestinationDir.UseVisualStyleBackColor = true;
      this.BrowseDestinationDir.Click += new System.EventHandler (this.BrowseDestinationDir_Click);
      // 
      // DirectoryStructureLabel
      // 
      this.DirectoryStructureLabel.Anchor = System.Windows.Forms.AnchorStyles.None;
      this.DirectoryStructureLabel.AutoSize = true;
      this.DirectoryStructureLabel.Location = new System.Drawing.Point (7, 61);
      this.DirectoryStructureLabel.Name = "DirectoryStructureLabel";
      this.DirectoryStructureLabel.Size = new System.Drawing.Size (95, 13);
      this.DirectoryStructureLabel.TabIndex = 4;
      this.DirectoryStructureLabel.Text = "Directory Structure";
      this.DirectoryStructureLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // Increments
      // 
      this.Increments.Anchor = System.Windows.Forms.AnchorStyles.None;
      this.Increments.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Increments.FormattingEnabled = true;
      this.Increments.Items.AddRange (new object[] {
            "1 Meter",
            "2 Meter",
            "5 Meter",
            "10 Meter",
            "20 Meter",
            "50 Meter",
            "100 Meter"});
      this.Increments.Location = new System.Drawing.Point (244, 80);
      this.Increments.Name = "Increments";
      this.Increments.Size = new System.Drawing.Size (101, 24);
      this.Increments.TabIndex = 15;
      // 
      // CurrentDirectory
      // 
      this.CurrentDirectory.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CurrentDirectory.Location = new System.Drawing.Point (10, 27);
      this.CurrentDirectory.Name = "CurrentDirectory";
      this.CurrentDirectory.ReadOnly = true;
      this.CurrentDirectory.Size = new System.Drawing.Size (630, 22);
      this.CurrentDirectory.TabIndex = 12;
      // 
      // ImagesSaved
      // 
      this.ImagesSaved.CausesValidation = false;
      this.ImagesSaved.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ImagesSaved.Location = new System.Drawing.Point (174, 130);
      this.ImagesSaved.Name = "ImagesSaved";
      this.ImagesSaved.ReadOnly = true;
      this.ImagesSaved.Size = new System.Drawing.Size (121, 22);
      this.ImagesSaved.TabIndex = 14;
      this.ImagesSaved.TabStop = false;
      this.ImagesSaved.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // ImagesFailedToExtract
      // 
      this.ImagesFailedToExtract.CausesValidation = false;
      this.ImagesFailedToExtract.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ImagesFailedToExtract.Location = new System.Drawing.Point (332, 130);
      this.ImagesFailedToExtract.Name = "ImagesFailedToExtract";
      this.ImagesFailedToExtract.ReadOnly = true;
      this.ImagesFailedToExtract.Size = new System.Drawing.Size (121, 22);
      this.ImagesFailedToExtract.TabIndex = 16;
      this.ImagesFailedToExtract.TabStop = false;
      this.ImagesFailedToExtract.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // StartButton
      // 
      this.StartButton.Location = new System.Drawing.Point (25, 662);
      this.StartButton.Name = "StartButton";
      this.StartButton.Size = new System.Drawing.Size (75, 23);
      this.StartButton.TabIndex = 4;
      this.StartButton.Text = "Start";
      this.StartButton.UseVisualStyleBackColor = true;
      this.StartButton.Click += new System.EventHandler (this.StartButton_Click);
      // 
      // CancelSaveButton
      // 
      this.CancelSaveButton.Location = new System.Drawing.Point (605, 662);
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
      // RunTimeParameters
      // 
      this.RunTimeParameters.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.RunTimeParameters.Controls.Add (this.BrowseDestinationDir);
      this.RunTimeParameters.Controls.Add (this.DestinationDirectory);
      this.RunTimeParameters.Controls.Add (this.IncrementLabel);
      this.RunTimeParameters.Controls.Add (this.Increments);
      this.RunTimeParameters.Controls.Add (this.flowLayoutPanel1);
      this.RunTimeParameters.Controls.Add (this.DirectoryStructureLabel);
      this.RunTimeParameters.Controls.Add (this.DestinationDiirectoryLabel);
      this.RunTimeParameters.Location = new System.Drawing.Point (23, 295);
      this.RunTimeParameters.Name = "RunTimeParameters";
      this.RunTimeParameters.Size = new System.Drawing.Size (657, 118);
      this.RunTimeParameters.TabIndex = 17;
      // 
      // RunTimeStatus
      // 
      this.RunTimeStatus.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.RunTimeStatus.Controls.Add (this.SaveProgress);
      this.RunTimeStatus.Controls.Add (this.SipperFileSatus);
      this.RunTimeStatus.Controls.Add (this.SipperFileStatusLabel);
      this.RunTimeStatus.Controls.Add (this.ImagesFailedToExtract);
      this.RunTimeStatus.Controls.Add (this.FailedToExtractLabel);
      this.RunTimeStatus.Controls.Add (this.ImagesSaved);
      this.RunTimeStatus.Controls.Add (this.ImagesSavedLabel);
      this.RunTimeStatus.Controls.Add (this.CurrentDirectory);
      this.RunTimeStatus.Controls.Add (this.CurDirLabel);
      this.RunTimeStatus.Controls.Add (this.ImageCount);
      this.RunTimeStatus.Controls.Add (this.ImageCountlabel);
      this.RunTimeStatus.Location = new System.Drawing.Point (23, 457);
      this.RunTimeStatus.Name = "RunTimeStatus";
      this.RunTimeStatus.Size = new System.Drawing.Size (657, 199);
      this.RunTimeStatus.TabIndex = 18;
      // 
      // SaveProgress
      // 
      this.SaveProgress.Location = new System.Drawing.Point (12, 171);
      this.SaveProgress.Name = "SaveProgress";
      this.SaveProgress.Size = new System.Drawing.Size (633, 17);
      this.SaveProgress.TabIndex = 19;
      // 
      // SipperFileSatus
      // 
      this.SipperFileSatus.CausesValidation = false;
      this.SipperFileSatus.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SipperFileSatus.Location = new System.Drawing.Point (9, 76);
      this.SipperFileSatus.Name = "SipperFileSatus";
      this.SipperFileSatus.ReadOnly = true;
      this.SipperFileSatus.Size = new System.Drawing.Size (631, 22);
      this.SipperFileSatus.TabIndex = 18;
      this.SipperFileSatus.TabStop = false;
      // 
      // SipperFileStatusLabel
      // 
      this.SipperFileStatusLabel.AutoSize = true;
      this.SipperFileStatusLabel.Location = new System.Drawing.Point (9, 60);
      this.SipperFileStatusLabel.Name = "SipperFileStatusLabel";
      this.SipperFileStatusLabel.Size = new System.Drawing.Size (89, 13);
      this.SipperFileStatusLabel.TabIndex = 17;
      this.SipperFileStatusLabel.Text = "Sipper File Status";
      this.SipperFileStatusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // CruiseLabel
      // 
      this.CruiseLabel.AutoSize = true;
      this.CruiseLabel.Location = new System.Drawing.Point (7, 13);
      this.CruiseLabel.Name = "CruiseLabel";
      this.CruiseLabel.Size = new System.Drawing.Size (36, 13);
      this.CruiseLabel.TabIndex = 19;
      this.CruiseLabel.Text = "Cruise";
      this.CruiseLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // SipperFileLabel
      // 
      this.SipperFileLabel.AutoSize = true;
      this.SipperFileLabel.Location = new System.Drawing.Point (7, 61);
      this.SipperFileLabel.Name = "SipperFileLabel";
      this.SipperFileLabel.Size = new System.Drawing.Size (53, 13);
      this.SipperFileLabel.TabIndex = 20;
      this.SipperFileLabel.Text = "SipperFile";
      this.SipperFileLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // DeploymentLabel
      // 
      this.DeploymentLabel.AutoSize = true;
      this.DeploymentLabel.Location = new System.Drawing.Point (328, 13);
      this.DeploymentLabel.Name = "DeploymentLabel";
      this.DeploymentLabel.Size = new System.Drawing.Size (63, 13);
      this.DeploymentLabel.TabIndex = 21;
      this.DeploymentLabel.Text = "Deployment";
      this.DeploymentLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // StationsLabel
      // 
      this.StationsLabel.AutoSize = true;
      this.StationsLabel.Location = new System.Drawing.Point (166, 13);
      this.StationsLabel.Name = "StationsLabel";
      this.StationsLabel.Size = new System.Drawing.Size (40, 13);
      this.StationsLabel.TabIndex = 22;
      this.StationsLabel.Text = "Station";
      this.StationsLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // CruiseName
      // 
      this.CruiseName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseName.Location = new System.Drawing.Point (10, 28);
      this.CruiseName.Name = "CruiseName";
      this.CruiseName.ReadOnly = true;
      this.CruiseName.Size = new System.Drawing.Size (140, 22);
      this.CruiseName.TabIndex = 11;
      this.CruiseName.TabStop = false;
      // 
      // StationName
      // 
      this.StationName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.StationName.Location = new System.Drawing.Point (169, 28);
      this.StationName.Name = "StationName";
      this.StationName.ReadOnly = true;
      this.StationName.Size = new System.Drawing.Size (140, 22);
      this.StationName.TabIndex = 23;
      this.StationName.TabStop = false;
      // 
      // DeploymentNum
      // 
      this.DeploymentNum.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DeploymentNum.Location = new System.Drawing.Point (328, 29);
      this.DeploymentNum.Name = "DeploymentNum";
      this.DeploymentNum.ReadOnly = true;
      this.DeploymentNum.Size = new System.Drawing.Size (84, 22);
      this.DeploymentNum.TabIndex = 24;
      this.DeploymentNum.TabStop = false;
      // 
      // SipperFile
      // 
      this.SipperFile.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SipperFile.Location = new System.Drawing.Point (10, 76);
      this.SipperFile.Name = "SipperFile";
      this.SipperFile.ReadOnly = true;
      this.SipperFile.Size = new System.Drawing.Size (402, 22);
      this.SipperFile.TabIndex = 25;
      this.SipperFile.TabStop = false;
      // 
      // SelectionCriteria
      // 
      this.SelectionCriteria.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.SelectionCriteria.Controls.Add (this.DepthLabel);
      this.SelectionCriteria.Controls.Add (this.DepthMax);
      this.SelectionCriteria.Controls.Add (this.DepthMin);
      this.SelectionCriteria.Controls.Add (this.SizeMax);
      this.SelectionCriteria.Controls.Add (this.SizeMin);
      this.SelectionCriteria.Controls.Add (this.SizeLabel);
      this.SelectionCriteria.Controls.Add (this.ProbMax);
      this.SelectionCriteria.Controls.Add (this.ProbMin);
      this.SelectionCriteria.Controls.Add (this.ProbabilityLabel);
      this.SelectionCriteria.Controls.Add (this.CruiseName);
      this.SelectionCriteria.Controls.Add (this.StationName);
      this.SelectionCriteria.Controls.Add (this.CruiseLabel);
      this.SelectionCriteria.Controls.Add (this.DeploymentNum);
      this.SelectionCriteria.Controls.Add (this.GroupName);
      this.SelectionCriteria.Controls.Add (this.StationsLabel);
      this.SelectionCriteria.Controls.Add (this.GroupNameLabel);
      this.SelectionCriteria.Controls.Add (this.DeploymentLabel);
      this.SelectionCriteria.Controls.Add (this.SipperFile);
      this.SelectionCriteria.Controls.Add (this.SipperFileLabel);
      this.SelectionCriteria.Location = new System.Drawing.Point (23, 83);
      this.SelectionCriteria.Name = "SelectionCriteria";
      this.SelectionCriteria.Size = new System.Drawing.Size (657, 168);
      this.SelectionCriteria.TabIndex = 26;
      // 
      // DepthLabel
      // 
      this.DepthLabel.AutoSize = true;
      this.DepthLabel.Location = new System.Drawing.Point (513, 113);
      this.DepthLabel.Name = "DepthLabel";
      this.DepthLabel.Size = new System.Drawing.Size (36, 13);
      this.DepthLabel.TabIndex = 36;
      this.DepthLabel.Text = "Depth";
      this.DepthLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // DepthMax
      // 
      this.DepthMax.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DepthMax.Location = new System.Drawing.Point (538, 129);
      this.DepthMax.Name = "DepthMax";
      this.DepthMax.ReadOnly = true;
      this.DepthMax.Size = new System.Drawing.Size (57, 22);
      this.DepthMax.TabIndex = 35;
      this.DepthMax.TabStop = false;
      this.DepthMax.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // DepthMin
      // 
      this.DepthMin.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DepthMin.Location = new System.Drawing.Point (465, 129);
      this.DepthMin.Name = "DepthMin";
      this.DepthMin.ReadOnly = true;
      this.DepthMin.Size = new System.Drawing.Size (57, 22);
      this.DepthMin.TabIndex = 34;
      this.DepthMin.TabStop = false;
      this.DepthMin.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // SizeMax
      // 
      this.SizeMax.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SizeMax.Location = new System.Drawing.Point (538, 76);
      this.SizeMax.Name = "SizeMax";
      this.SizeMax.ReadOnly = true;
      this.SizeMax.Size = new System.Drawing.Size (57, 22);
      this.SizeMax.TabIndex = 33;
      this.SizeMax.TabStop = false;
      this.SizeMax.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // SizeMin
      // 
      this.SizeMin.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SizeMin.Location = new System.Drawing.Point (465, 76);
      this.SizeMin.Name = "SizeMin";
      this.SizeMin.ReadOnly = true;
      this.SizeMin.Size = new System.Drawing.Size (57, 22);
      this.SizeMin.TabIndex = 32;
      this.SizeMin.TabStop = false;
      this.SizeMin.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // SizeLabel
      // 
      this.SizeLabel.AutoSize = true;
      this.SizeLabel.Location = new System.Drawing.Point (516, 61);
      this.SizeLabel.Name = "SizeLabel";
      this.SizeLabel.Size = new System.Drawing.Size (27, 13);
      this.SizeLabel.TabIndex = 31;
      this.SizeLabel.Text = "Size";
      this.SizeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ProbMax
      // 
      this.ProbMax.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ProbMax.Location = new System.Drawing.Point (538, 28);
      this.ProbMax.Name = "ProbMax";
      this.ProbMax.ReadOnly = true;
      this.ProbMax.Size = new System.Drawing.Size (57, 22);
      this.ProbMax.TabIndex = 30;
      this.ProbMax.TabStop = false;
      this.ProbMax.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // ProbMin
      // 
      this.ProbMin.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ProbMin.Location = new System.Drawing.Point (465, 28);
      this.ProbMin.Name = "ProbMin";
      this.ProbMin.ReadOnly = true;
      this.ProbMin.Size = new System.Drawing.Size (57, 22);
      this.ProbMin.TabIndex = 29;
      this.ProbMin.TabStop = false;
      this.ProbMin.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // ProbabilityLabel
      // 
      this.ProbabilityLabel.AutoSize = true;
      this.ProbabilityLabel.Location = new System.Drawing.Point (501, 12);
      this.ProbabilityLabel.Name = "ProbabilityLabel";
      this.ProbabilityLabel.Size = new System.Drawing.Size (55, 13);
      this.ProbabilityLabel.TabIndex = 28;
      this.ProbabilityLabel.Text = "Probability";
      this.ProbabilityLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // GroupName
      // 
      this.GroupName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.GroupName.Location = new System.Drawing.Point (10, 129);
      this.GroupName.Name = "GroupName";
      this.GroupName.ReadOnly = true;
      this.GroupName.Size = new System.Drawing.Size (402, 22);
      this.GroupName.TabIndex = 27;
      this.GroupName.TabStop = false;
      // 
      // GroupNameLabel
      // 
      this.GroupNameLabel.AutoSize = true;
      this.GroupNameLabel.Location = new System.Drawing.Point (7, 113);
      this.GroupNameLabel.Name = "GroupNameLabel";
      this.GroupNameLabel.Size = new System.Drawing.Size (36, 13);
      this.GroupNameLabel.TabIndex = 26;
      this.GroupNameLabel.Text = "Group";
      this.GroupNameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // SelectionCriteriaLabel
      // 
      this.SelectionCriteriaLabel.AutoSize = true;
      this.SelectionCriteriaLabel.Location = new System.Drawing.Point (20, 67);
      this.SelectionCriteriaLabel.Name = "SelectionCriteriaLabel";
      this.SelectionCriteriaLabel.Size = new System.Drawing.Size (86, 13);
      this.SelectionCriteriaLabel.TabIndex = 27;
      this.SelectionCriteriaLabel.Text = "Selection Criteria";
      // 
      // RunTimeParametersLabel
      // 
      this.RunTimeParametersLabel.AutoSize = true;
      this.RunTimeParametersLabel.Location = new System.Drawing.Point (20, 279);
      this.RunTimeParametersLabel.Name = "RunTimeParametersLabel";
      this.RunTimeParametersLabel.Size = new System.Drawing.Size (109, 13);
      this.RunTimeParametersLabel.TabIndex = 28;
      this.RunTimeParametersLabel.Text = "Run Time Parameters";
      // 
      // RunTimeStatusLabel
      // 
      this.RunTimeStatusLabel.AutoSize = true;
      this.RunTimeStatusLabel.Location = new System.Drawing.Point (20, 441);
      this.RunTimeStatusLabel.Name = "RunTimeStatusLabel";
      this.RunTimeStatusLabel.Size = new System.Drawing.Size (86, 13);
      this.RunTimeStatusLabel.TabIndex = 29;
      this.RunTimeStatusLabel.Text = "Run Time Status";
      // 
      // StatsTimer
      // 
      this.StatsTimer.Interval = 300;
      this.StatsTimer.Tick += new System.EventHandler (this.StatsTimer_Tick);
      // 
      // SaveImagesToDisk2
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (702, 695);
      this.Controls.Add (this.RunTimeStatusLabel);
      this.Controls.Add (this.RunTimeParametersLabel);
      this.Controls.Add (this.SelectionCriteriaLabel);
      this.Controls.Add (this.RunTimeStatus);
      this.Controls.Add (this.RunTimeParameters);
      this.Controls.Add (this.CancelSaveButton);
      this.Controls.Add (this.StartButton);
      this.Controls.Add (this.TitleLabel);
      this.Controls.Add (this.SelectionCriteria);
      this.MaximumSize = new System.Drawing.Size (718, 733);
      this.MinimumSize = new System.Drawing.Size (718, 733);
      this.Name = "SaveImagesToDisk2";
      this.Text = "SaveImagesToDisk2";
      this.Load += new System.EventHandler (this.SaveImagesToDisk2_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.SaveImagesToDisk2_FormClosing);
      this.flowLayoutPanel1.ResumeLayout (false);
      this.flowLayoutPanel1.PerformLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.RunTimeParameters.ResumeLayout (false);
      this.RunTimeParameters.PerformLayout ();
      this.RunTimeStatus.ResumeLayout (false);
      this.RunTimeStatus.PerformLayout ();
      this.SelectionCriteria.ResumeLayout (false);
      this.SelectionCriteria.PerformLayout ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label ImageCountlabel;
    private System.Windows.Forms.TextBox ImageCount;
    private System.Windows.Forms.Label DestinationDiirectoryLabel;
    private System.Windows.Forms.Label DirectoryStructureLabel;
    private System.Windows.Forms.TextBox DestinationDirectory;
    private System.Windows.Forms.Button BrowseDestinationDir;
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
    private System.Windows.Forms.Panel RunTimeParameters;
    private System.Windows.Forms.Panel RunTimeStatus;
    private System.Windows.Forms.Label StationsLabel;
    private System.Windows.Forms.Label DeploymentLabel;
    private System.Windows.Forms.Label SipperFileLabel;
    private System.Windows.Forms.Label CruiseLabel;
    private System.Windows.Forms.TextBox SipperFile;
    private System.Windows.Forms.TextBox DeploymentNum;
    private System.Windows.Forms.TextBox StationName;
    private System.Windows.Forms.TextBox CruiseName;
    private System.Windows.Forms.Panel SelectionCriteria;
    private System.Windows.Forms.TextBox GroupName;
    private System.Windows.Forms.Label GroupNameLabel;
    private System.Windows.Forms.Label SelectionCriteriaLabel;
    private System.Windows.Forms.Label RunTimeStatusLabel;
    private System.Windows.Forms.Label RunTimeParametersLabel;
    private System.Windows.Forms.TextBox SipperFileSatus;
    private System.Windows.Forms.Label SipperFileStatusLabel;
    private System.Windows.Forms.Timer StatsTimer;
    private System.Windows.Forms.ProgressBar SaveProgress;
    private System.Windows.Forms.TextBox ProbMin;
    private System.Windows.Forms.Label ProbabilityLabel;
    private System.Windows.Forms.TextBox ProbMax;
    private System.Windows.Forms.Label DepthLabel;
    private System.Windows.Forms.TextBox DepthMax;
    private System.Windows.Forms.TextBox DepthMin;
    private System.Windows.Forms.TextBox SizeMax;
    private System.Windows.Forms.TextBox SizeMin;
    private System.Windows.Forms.Label SizeLabel;
  }
}