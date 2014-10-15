namespace PicesCommander
{
  partial class ImportGroupAssignments
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
      this.components = new System.ComponentModel.Container();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ImportGroupAssignments));
      this.TitleLabel = new System.Windows.Forms.Label();
      this.SourceDirectoryLabel = new System.Windows.Forms.Label();
      this.GroupNameLabel = new System.Windows.Forms.Label();
      this.SourceDirectory = new System.Windows.Forms.TextBox();
      this.GroupName = new System.Windows.Forms.TextBox();
      this.BrowseSourceDir = new System.Windows.Forms.Button();
      this.Start = new System.Windows.Forms.Button();
      this.Cancel = new System.Windows.Forms.Button();
      this.RunLog = new System.Windows.Forms.TextBox();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
      this.BackGroundTimer = new System.Windows.Forms.Timer(this.components);
      this.GroupDescription = new System.Windows.Forms.TextBox();
      this.GroupDescLabel = new System.Windows.Forms.Label();
      this.textBox1 = new System.Windows.Forms.TextBox();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
      this.SuspendLayout();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point(198, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size(233, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Import Group Assignments";
      // 
      // SourceDirectoryLabel
      // 
      this.SourceDirectoryLabel.AutoSize = true;
      this.SourceDirectoryLabel.Location = new System.Drawing.Point(16, 71);
      this.SourceDirectoryLabel.Name = "SourceDirectoryLabel";
      this.SourceDirectoryLabel.Size = new System.Drawing.Size(86, 13);
      this.SourceDirectoryLabel.TabIndex = 1;
      this.SourceDirectoryLabel.Text = "Source Directory";
      // 
      // GroupNameLabel
      // 
      this.GroupNameLabel.AutoSize = true;
      this.GroupNameLabel.Location = new System.Drawing.Point(16, 105);
      this.GroupNameLabel.Name = "GroupNameLabel";
      this.GroupNameLabel.Size = new System.Drawing.Size(67, 13);
      this.GroupNameLabel.TabIndex = 2;
      this.GroupNameLabel.Text = "Group Name";
      // 
      // SourceDirectory
      // 
      this.SourceDirectory.Location = new System.Drawing.Point(105, 68);
      this.SourceDirectory.Name = "SourceDirectory";
      this.SourceDirectory.Size = new System.Drawing.Size(433, 20);
      this.SourceDirectory.TabIndex = 3;
      this.SourceDirectory.Leave += new System.EventHandler(this.SourceDirectory_Leave);
      // 
      // GroupName
      // 
      this.GroupName.Location = new System.Drawing.Point(105, 102);
      this.GroupName.Name = "GroupName";
      this.GroupName.Size = new System.Drawing.Size(433, 20);
      this.GroupName.TabIndex = 7;
      this.GroupName.Leave += new System.EventHandler(this.GroupName_Leave);
      // 
      // BrowseSourceDir
      // 
      this.BrowseSourceDir.Location = new System.Drawing.Point(543, 66);
      this.BrowseSourceDir.Name = "BrowseSourceDir";
      this.BrowseSourceDir.Size = new System.Drawing.Size(75, 23);
      this.BrowseSourceDir.TabIndex = 5;
      this.BrowseSourceDir.Text = "Browse";
      this.BrowseSourceDir.UseVisualStyleBackColor = true;
      this.BrowseSourceDir.Click += new System.EventHandler(this.BrowseSourceDir_Click);
      // 
      // Start
      // 
      this.Start.Location = new System.Drawing.Point(19, 541);
      this.Start.Name = "Start";
      this.Start.Size = new System.Drawing.Size(75, 23);
      this.Start.TabIndex = 6;
      this.Start.Text = "Start";
      this.Start.UseVisualStyleBackColor = true;
      this.Start.Click += new System.EventHandler(this.Start_Click);
      // 
      // Cancel
      // 
      this.Cancel.Location = new System.Drawing.Point(543, 541);
      this.Cancel.Name = "Cancel";
      this.Cancel.Size = new System.Drawing.Size(75, 23);
      this.Cancel.TabIndex = 7;
      this.Cancel.Text = "Cancel";
      this.Cancel.UseVisualStyleBackColor = true;
      this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
      // 
      // RunLog
      // 
      this.RunLog.Location = new System.Drawing.Point(19, 407);
      this.RunLog.Multiline = true;
      this.RunLog.Name = "RunLog";
      this.RunLog.ReadOnly = true;
      this.RunLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
      this.RunLog.Size = new System.Drawing.Size(599, 118);
      this.RunLog.TabIndex = 8;
      this.RunLog.TabStop = false;
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // BackGroundTimer
      // 
      this.BackGroundTimer.Tick += new System.EventHandler(this.BackgroundTimer_Tick);
      // 
      // GroupDescription
      // 
      this.GroupDescription.Location = new System.Drawing.Point(19, 150);
      this.GroupDescription.Multiline = true;
      this.GroupDescription.Name = "GroupDescription";
      this.GroupDescription.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
      this.GroupDescription.Size = new System.Drawing.Size(596, 85);
      this.GroupDescription.TabIndex = 9;
      this.GroupDescription.Leave += new System.EventHandler(this.GroupDescription_Leave);
      // 
      // GroupDescLabel
      // 
      this.GroupDescLabel.AutoSize = true;
      this.GroupDescLabel.Location = new System.Drawing.Point(16, 134);
      this.GroupDescLabel.Name = "GroupDescLabel";
      this.GroupDescLabel.Size = new System.Drawing.Size(60, 13);
      this.GroupDescLabel.TabIndex = 10;
      this.GroupDescLabel.Text = "Description";
      // 
      // textBox1
      // 
      this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.textBox1.Location = new System.Drawing.Point(19, 250);
      this.textBox1.Multiline = true;
      this.textBox1.Name = "textBox1";
      this.textBox1.ReadOnly = true;
      this.textBox1.Size = new System.Drawing.Size(599, 151);
      this.textBox1.TabIndex = 11;
      this.textBox1.Text = resources.GetString("textBox1.Text");
      // 
      // ImportGroupAssignments
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(630, 574);
      this.Controls.Add(this.textBox1);
      this.Controls.Add(this.GroupDescLabel);
      this.Controls.Add(this.GroupDescription);
      this.Controls.Add(this.RunLog);
      this.Controls.Add(this.Cancel);
      this.Controls.Add(this.Start);
      this.Controls.Add(this.BrowseSourceDir);
      this.Controls.Add(this.GroupName);
      this.Controls.Add(this.SourceDirectory);
      this.Controls.Add(this.GroupNameLabel);
      this.Controls.Add(this.SourceDirectoryLabel);
      this.Controls.Add(this.TitleLabel);
      this.MaximumSize = new System.Drawing.Size(646, 610);
      this.MinimumSize = new System.Drawing.Size(646, 610);
      this.Name = "ImportGroupAssignments";
      this.Text = "ImportGroupAssignments";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ImportGroupAssignments_FormClosing);
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label SourceDirectoryLabel;
    private System.Windows.Forms.Label GroupNameLabel;
    private System.Windows.Forms.TextBox SourceDirectory;
    private System.Windows.Forms.TextBox GroupName;
    private System.Windows.Forms.Button BrowseSourceDir;
    private System.Windows.Forms.Button Start;
    private System.Windows.Forms.Button Cancel;
    private System.Windows.Forms.TextBox RunLog;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.Timer BackGroundTimer;
    private System.Windows.Forms.Label GroupDescLabel;
    private System.Windows.Forms.TextBox GroupDescription;
    private System.Windows.Forms.TextBox textBox1;
  }
}