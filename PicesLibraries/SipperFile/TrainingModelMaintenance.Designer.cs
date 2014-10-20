namespace SipperFile
{
  partial class TrainingModelMaintenance
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
      System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Crustacean_Copepod");
      System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Crustacean", new System.Windows.Forms.TreeNode[] {
            treeNode1});
      System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Gelatinous");
      System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Elongate_Chaetognath");
      System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Elongate", new System.Windows.Forms.TreeNode[] {
            treeNode4});
      this.ClassesToIncludeLabel = new System.Windows.Forms.Label();
      this.treeView1 = new System.Windows.Forms.TreeView();
      this.splitContainer1 = new System.Windows.Forms.SplitContainer();
      this.menuStrip2 = new System.Windows.Forms.MenuStrip();
      this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.saveImagesToDirectoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.importModelFromDirectoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.ModelNameLabel = new System.Windows.Forms.Label();
      this.ModelName = new System.Windows.Forms.TextBox();
      this.splitContainer2 = new System.Windows.Forms.SplitContainer();
      this.splitContainer1.Panel1.SuspendLayout();
      this.splitContainer1.Panel2.SuspendLayout();
      this.splitContainer1.SuspendLayout();
      this.menuStrip2.SuspendLayout();
      this.splitContainer2.SuspendLayout();
      this.SuspendLayout();
      // 
      // ClassesToIncludeLabel
      // 
      this.ClassesToIncludeLabel.AutoSize = true;
      this.ClassesToIncludeLabel.Location = new System.Drawing.Point(3, 6);
      this.ClassesToIncludeLabel.Name = "ClassesToIncludeLabel";
      this.ClassesToIncludeLabel.Size = new System.Drawing.Size(93, 13);
      this.ClassesToIncludeLabel.TabIndex = 0;
      this.ClassesToIncludeLabel.Text = "Classes to Include";
      // 
      // treeView1
      // 
      this.treeView1.CheckBoxes = true;
      this.treeView1.Location = new System.Drawing.Point(3, 22);
      this.treeView1.Name = "treeView1";
      treeNode1.Name = "Crustacean_Copepod";
      treeNode1.Text = "Crustacean_Copepod";
      treeNode2.Name = "Crustacean";
      treeNode2.Text = "Crustacean";
      treeNode3.Name = "Node1";
      treeNode3.Text = "Gelatinous";
      treeNode4.Name = "Elongate_Chaetognath";
      treeNode4.Text = "Elongate_Chaetognath";
      treeNode5.Name = "Elongate";
      treeNode5.Text = "Elongate";
      this.treeView1.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode2,
            treeNode3,
            treeNode5});
      this.treeView1.Size = new System.Drawing.Size(305, 470);
      this.treeView1.TabIndex = 1;
      // 
      // splitContainer1
      // 
      this.splitContainer1.Location = new System.Drawing.Point(12, 60);
      this.splitContainer1.Name = "splitContainer1";
      // 
      // splitContainer1.Panel1
      // 
      this.splitContainer1.Panel1.Controls.Add(this.treeView1);
      this.splitContainer1.Panel1.Controls.Add(this.ClassesToIncludeLabel);
      // 
      // splitContainer1.Panel2
      // 
      this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
      this.splitContainer1.Size = new System.Drawing.Size(752, 495);
      this.splitContainer1.SplitterDistance = 311;
      this.splitContainer1.TabIndex = 2;
      // 
      // menuStrip2
      // 
      this.menuStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
      this.menuStrip2.Location = new System.Drawing.Point(0, 0);
      this.menuStrip2.Name = "menuStrip2";
      this.menuStrip2.Size = new System.Drawing.Size(947, 24);
      this.menuStrip2.TabIndex = 3;
      this.menuStrip2.Text = "menuStrip2";
      // 
      // fileToolStripMenuItem
      // 
      this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveImagesToDirectoryToolStripMenuItem,
            this.importModelFromDirectoryToolStripMenuItem,
            this.exitToolStripMenuItem});
      this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
      this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
      this.fileToolStripMenuItem.Text = "File";
      // 
      // saveImagesToDirectoryToolStripMenuItem
      // 
      this.saveImagesToDirectoryToolStripMenuItem.Name = "saveImagesToDirectoryToolStripMenuItem";
      this.saveImagesToDirectoryToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
      this.saveImagesToDirectoryToolStripMenuItem.Text = "Save Images to directory";
      // 
      // importModelFromDirectoryToolStripMenuItem
      // 
      this.importModelFromDirectoryToolStripMenuItem.Name = "importModelFromDirectoryToolStripMenuItem";
      this.importModelFromDirectoryToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
      this.importModelFromDirectoryToolStripMenuItem.Text = "Import Model from Directory";
      // 
      // exitToolStripMenuItem
      // 
      this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
      this.exitToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
      this.exitToolStripMenuItem.Text = "Exit";
      // 
      // helpToolStripMenuItem
      // 
      this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
      this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
      this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
      this.helpToolStripMenuItem.Text = "Help";
      // 
      // aboutToolStripMenuItem
      // 
      this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
      this.aboutToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
      this.aboutToolStripMenuItem.Text = "About";
      // 
      // ModelNameLabel
      // 
      this.ModelNameLabel.AutoSize = true;
      this.ModelNameLabel.Location = new System.Drawing.Point(12, 34);
      this.ModelNameLabel.Name = "ModelNameLabel";
      this.ModelNameLabel.Size = new System.Drawing.Size(67, 13);
      this.ModelNameLabel.TabIndex = 4;
      this.ModelNameLabel.Text = "Model Name";
      // 
      // ModelName
      // 
      this.ModelName.Location = new System.Drawing.Point(85, 31);
      this.ModelName.Name = "ModelName";
      this.ModelName.Size = new System.Drawing.Size(158, 20);
      this.ModelName.TabIndex = 5;
      // 
      // splitContainer2
      // 
      this.splitContainer2.Location = new System.Drawing.Point(3, 6);
      this.splitContainer2.Name = "splitContainer2";
      this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
      this.splitContainer2.Size = new System.Drawing.Size(426, 486);
      this.splitContainer2.SplitterDistance = 124;
      this.splitContainer2.TabIndex = 6;
      // 
      // TrainingModelMaintenance
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(947, 566);
      this.Controls.Add(this.ModelName);
      this.Controls.Add(this.ModelNameLabel);
      this.Controls.Add(this.splitContainer1);
      this.Controls.Add(this.menuStrip2);
      this.Name = "TrainingModelMaintenance";
      this.Text = "TrainingModelMaintenance";
      this.splitContainer1.Panel1.ResumeLayout(false);
      this.splitContainer1.Panel1.PerformLayout();
      this.splitContainer1.Panel2.ResumeLayout(false);
      this.splitContainer1.ResumeLayout(false);
      this.menuStrip2.ResumeLayout(false);
      this.menuStrip2.PerformLayout();
      this.splitContainer2.ResumeLayout(false);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label ClassesToIncludeLabel;
    private System.Windows.Forms.TreeView treeView1;
    private System.Windows.Forms.SplitContainer splitContainer1;
    private System.Windows.Forms.MenuStrip menuStrip2;
    private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem saveImagesToDirectoryToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem importModelFromDirectoryToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
    private System.Windows.Forms.Label ModelNameLabel;
    private System.Windows.Forms.TextBox ModelName;
    private System.Windows.Forms.SplitContainer splitContainer2;
  }
}