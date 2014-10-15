namespace PicesCommander
{
  partial class TrainingModelCreateFromDirectory
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
      this.TitleLabel = new System.Windows.Forms.Label();
      this.DirectoryNameLabel = new System.Windows.Forms.Label();
      this.SourceDirectory = new System.Windows.Forms.TextBox();
      this.SourceDirectoryBrowseButton = new System.Windows.Forms.Button();
      this.ModelName = new System.Windows.Forms.TextBox();
      this.ModelNameLabel = new System.Windows.Forms.Label();
      this.CreateButton = new System.Windows.Forms.Button();
      this.CancelAppButton = new System.Windows.Forms.Button();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
      this.ClassList = new System.Windows.Forms.ListView();
      this.ClassName = new System.Windows.Forms.ColumnHeader();
      this.ClassDirectory = new System.Windows.Forms.ColumnHeader();
      this.Count = new System.Windows.Forms.ColumnHeader();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
      this.SuspendLayout();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point(144, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size(341, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Create Training Model From a Directory";
      // 
      // DirectoryNameLabel
      // 
      this.DirectoryNameLabel.AutoSize = true;
      this.DirectoryNameLabel.Location = new System.Drawing.Point(35, 69);
      this.DirectoryNameLabel.Name = "DirectoryNameLabel";
      this.DirectoryNameLabel.Size = new System.Drawing.Size(86, 13);
      this.DirectoryNameLabel.TabIndex = 1;
      this.DirectoryNameLabel.Text = "Source Directory";
      // 
      // SourceDirectory
      // 
      this.SourceDirectory.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SourceDirectory.Location = new System.Drawing.Point(38, 85);
      this.SourceDirectory.Name = "SourceDirectory";
      this.SourceDirectory.Size = new System.Drawing.Size(503, 22);
      this.SourceDirectory.TabIndex = 2;
      this.SourceDirectory.Validating += new System.ComponentModel.CancelEventHandler(this.SourceDirectory_Validating);
      // 
      // SourceDirectoryBrowseButton
      // 
      this.SourceDirectoryBrowseButton.Location = new System.Drawing.Point(541, 84);
      this.SourceDirectoryBrowseButton.Name = "SourceDirectoryBrowseButton";
      this.SourceDirectoryBrowseButton.Size = new System.Drawing.Size(75, 23);
      this.SourceDirectoryBrowseButton.TabIndex = 3;
      this.SourceDirectoryBrowseButton.Text = "Browse";
      this.SourceDirectoryBrowseButton.UseVisualStyleBackColor = true;
      this.SourceDirectoryBrowseButton.Click += new System.EventHandler(this.SourceDirectoryBrowseButton_Click);
      // 
      // ModelName
      // 
      this.ModelName.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ModelName.Location = new System.Drawing.Point(38, 148);
      this.ModelName.Name = "ModelName";
      this.ModelName.Size = new System.Drawing.Size(204, 22);
      this.ModelName.TabIndex = 4;
      this.ModelName.Validating += new System.ComponentModel.CancelEventHandler(this.ModelName_Validating);
      // 
      // ModelNameLabel
      // 
      this.ModelNameLabel.AutoSize = true;
      this.ModelNameLabel.Location = new System.Drawing.Point(35, 132);
      this.ModelNameLabel.Name = "ModelNameLabel";
      this.ModelNameLabel.Size = new System.Drawing.Size(67, 13);
      this.ModelNameLabel.TabIndex = 5;
      this.ModelNameLabel.Text = "Model Name";
      // 
      // CreateButton
      // 
      this.CreateButton.Location = new System.Drawing.Point(38, 497);
      this.CreateButton.Name = "CreateButton";
      this.CreateButton.Size = new System.Drawing.Size(75, 23);
      this.CreateButton.TabIndex = 6;
      this.CreateButton.Text = "Create";
      this.CreateButton.UseVisualStyleBackColor = true;
      this.CreateButton.Click += new System.EventHandler(this.CreateButtonClick);
      // 
      // CancelAppButton
      // 
      this.CancelAppButton.Location = new System.Drawing.Point(541, 497);
      this.CancelAppButton.Name = "CancelAppButton";
      this.CancelAppButton.Size = new System.Drawing.Size(75, 23);
      this.CancelAppButton.TabIndex = 7;
      this.CancelAppButton.Text = "Cancel";
      this.CancelAppButton.UseVisualStyleBackColor = true;
      this.CancelAppButton.Click += new System.EventHandler(this.CancelAppButton_Click);
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // ClassList
      // 
      this.ClassList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.ClassName,
            this.ClassDirectory,
            this.Count});
      this.ClassList.Location = new System.Drawing.Point(38, 198);
      this.ClassList.Name = "ClassList";
      this.ClassList.Size = new System.Drawing.Size(578, 274);
      this.ClassList.TabIndex = 8;
      this.ClassList.TabStop = false;
      this.ClassList.UseCompatibleStateImageBehavior = false;
      this.ClassList.View = System.Windows.Forms.View.Details;
      // 
      // ClassName
      // 
      this.ClassName.Text = "Class Name";
      this.ClassName.Width = 173;
      // 
      // ClassDirectory
      // 
      this.ClassDirectory.Text = "Directory";
      this.ClassDirectory.Width = 303;
      // 
      // Count
      // 
      this.Count.Text = "Count";
      this.Count.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.Count.Width = 80;
      // 
      // TrainingModelCreateFromDirectory
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(644, 532);
      this.Controls.Add(this.ClassList);
      this.Controls.Add(this.CancelAppButton);
      this.Controls.Add(this.CreateButton);
      this.Controls.Add(this.ModelNameLabel);
      this.Controls.Add(this.ModelName);
      this.Controls.Add(this.SourceDirectoryBrowseButton);
      this.Controls.Add(this.SourceDirectory);
      this.Controls.Add(this.DirectoryNameLabel);
      this.Controls.Add(this.TitleLabel);
      this.Name = "TrainingModelCreateFromDirectory";
      this.Text = "TrainingModelCreateFromDirectory";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.TrainingModelCreateFromDirectory_FormClosing);
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label DirectoryNameLabel;
    private System.Windows.Forms.TextBox SourceDirectory;
    private System.Windows.Forms.Button SourceDirectoryBrowseButton;
    private System.Windows.Forms.TextBox ModelName;
    private System.Windows.Forms.Label ModelNameLabel;
    private System.Windows.Forms.Button CreateButton;
    private System.Windows.Forms.Button CancelAppButton;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.ListView ClassList;
    private System.Windows.Forms.ColumnHeader ClassName;
    private System.Windows.Forms.ColumnHeader ClassDirectory;
    private System.Windows.Forms.ColumnHeader Count;
  }
}