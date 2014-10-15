namespace SipperDiskManager
{
  partial class SipperDiskManager
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose (bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose ();
      }
      base.Dispose (disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent ()
    {
      this.components = new System.ComponentModel.Container ();
      this.SipperFileListView = new System.Windows.Forms.ListView ();
      this.FileName = new System.Windows.Forms.ColumnHeader ();
      this.DateStamp = new System.Windows.Forms.ColumnHeader ();
      this.FileSize = new System.Windows.Forms.ColumnHeader ();
      this.StartSector = new System.Windows.Forms.ColumnHeader ();
      this.EndSector = new System.Windows.Forms.ColumnHeader ();
      this.Description = new System.Windows.Forms.ColumnHeader ();
      this.DriveNumberlabel = new System.Windows.Forms.Label ();
      this.DriveNumber = new System.Windows.Forms.TextBox ();
      this.DiskSizeLabel = new System.Windows.Forms.Label ();
      this.DiskSize = new System.Windows.Forms.TextBox ();
      this.SpaceUsedLabel = new System.Windows.Forms.Label ();
      this.DiskSpaceUsed = new System.Windows.Forms.TextBox ();
      this.SpaceAvailableLabel1 = new System.Windows.Forms.Label ();
      this.DiskSpaceAvailabe = new System.Windows.Forms.TextBox ();
      this.Download = new System.Windows.Forms.Button ();
      this.DownLoadDir = new System.Windows.Forms.TextBox ();
      this.FreeSpace = new System.Windows.Forms.TextBox ();
      this.FreeSpaceLabel = new System.Windows.Forms.Label ();
      this.SelectedLabel = new System.Windows.Forms.Label ();
      this.SelectedSpace = new System.Windows.Forms.TextBox ();
      this.menuStrip1 = new System.Windows.Forms.MenuStrip ();
      this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.destinationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.resetDirectoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.initializeDrivToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.exitToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem ();
      this.CheckHardDriveTimer = new System.Windows.Forms.Timer (this.components);
      this.ReFresh = new System.Windows.Forms.Button ();
      this.BottomPanel = new System.Windows.Forms.FlowLayoutPanel ();
      this.DownloadPanel = new System.Windows.Forms.Panel ();
      this.FreeSpacePanel = new System.Windows.Forms.Panel ();
      this.SelectedPanel = new System.Windows.Forms.Panel ();
      this.readDirectoryFromLogFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.menuStrip1.SuspendLayout ();
      this.BottomPanel.SuspendLayout ();
      this.DownloadPanel.SuspendLayout ();
      this.FreeSpacePanel.SuspendLayout ();
      this.SelectedPanel.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // SipperFileListView
      // 
      this.SipperFileListView.CheckBoxes = true;
      this.SipperFileListView.Columns.AddRange (new System.Windows.Forms.ColumnHeader[] {
            this.FileName,
            this.DateStamp,
            this.FileSize,
            this.StartSector,
            this.EndSector,
            this.Description});
      this.SipperFileListView.Enabled = false;
      this.SipperFileListView.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SipperFileListView.FullRowSelect = true;
      this.SipperFileListView.Location = new System.Drawing.Point (13, 83);
      this.SipperFileListView.MultiSelect = false;
      this.SipperFileListView.Name = "SipperFileListView";
      this.SipperFileListView.Size = new System.Drawing.Size (980, 438);
      this.SipperFileListView.TabIndex = 0;
      this.SipperFileListView.UseCompatibleStateImageBehavior = false;
      this.SipperFileListView.View = System.Windows.Forms.View.Details;
      this.SipperFileListView.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler (this.SipperFileListView_ItemChecked);
      // 
      // FileName
      // 
      this.FileName.Text = "File Name";
      this.FileName.Width = 180;
      // 
      // DateStamp
      // 
      this.DateStamp.Text = "Date/Time";
      this.DateStamp.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      this.DateStamp.Width = 190;
      // 
      // FileSize
      // 
      this.FileSize.Text = "File Size";
      this.FileSize.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.FileSize.Width = 120;
      // 
      // StartSector
      // 
      this.StartSector.Text = "Start Sector";
      this.StartSector.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.StartSector.Width = 116;
      // 
      // EndSector
      // 
      this.EndSector.Text = "End Sector";
      this.EndSector.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.EndSector.Width = 102;
      // 
      // Description
      // 
      this.Description.Text = "Description";
      this.Description.Width = 460;
      // 
      // DriveNumberlabel
      // 
      this.DriveNumberlabel.AutoSize = true;
      this.DriveNumberlabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DriveNumberlabel.Location = new System.Drawing.Point (9, 44);
      this.DriveNumberlabel.Name = "DriveNumberlabel";
      this.DriveNumberlabel.Size = new System.Drawing.Size (104, 18);
      this.DriveNumberlabel.TabIndex = 1;
      this.DriveNumberlabel.Text = "Drive Number";
      // 
      // DriveNumber
      // 
      this.DriveNumber.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DriveNumber.Location = new System.Drawing.Point (118, 40);
      this.DriveNumber.Name = "DriveNumber";
      this.DriveNumber.ReadOnly = true;
      this.DriveNumber.Size = new System.Drawing.Size (30, 26);
      this.DriveNumber.TabIndex = 2;
      this.DriveNumber.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // DiskSizeLabel
      // 
      this.DiskSizeLabel.AutoSize = true;
      this.DiskSizeLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DiskSizeLabel.Location = new System.Drawing.Point (163, 44);
      this.DiskSizeLabel.Name = "DiskSizeLabel";
      this.DiskSizeLabel.Size = new System.Drawing.Size (39, 18);
      this.DiskSizeLabel.TabIndex = 3;
      this.DiskSizeLabel.Text = "Size";
      // 
      // DiskSize
      // 
      this.DiskSize.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DiskSize.Location = new System.Drawing.Point (204, 40);
      this.DiskSize.Name = "DiskSize";
      this.DiskSize.ReadOnly = true;
      this.DiskSize.Size = new System.Drawing.Size (100, 26);
      this.DiskSize.TabIndex = 4;
      this.DiskSize.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // SpaceUsedLabel
      // 
      this.SpaceUsedLabel.AutoSize = true;
      this.SpaceUsedLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SpaceUsedLabel.Location = new System.Drawing.Point (317, 44);
      this.SpaceUsedLabel.Name = "SpaceUsedLabel";
      this.SpaceUsedLabel.Size = new System.Drawing.Size (45, 18);
      this.SpaceUsedLabel.TabIndex = 5;
      this.SpaceUsedLabel.Text = "Used";
      // 
      // DiskSpaceUsed
      // 
      this.DiskSpaceUsed.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DiskSpaceUsed.Location = new System.Drawing.Point (364, 40);
      this.DiskSpaceUsed.Name = "DiskSpaceUsed";
      this.DiskSpaceUsed.ReadOnly = true;
      this.DiskSpaceUsed.Size = new System.Drawing.Size (100, 26);
      this.DiskSpaceUsed.TabIndex = 6;
      this.DiskSpaceUsed.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // SpaceAvailableLabel1
      // 
      this.SpaceAvailableLabel1.AutoSize = true;
      this.SpaceAvailableLabel1.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SpaceAvailableLabel1.Location = new System.Drawing.Point (477, 44);
      this.SpaceAvailableLabel1.Name = "SpaceAvailableLabel1";
      this.SpaceAvailableLabel1.Size = new System.Drawing.Size (72, 18);
      this.SpaceAvailableLabel1.TabIndex = 7;
      this.SpaceAvailableLabel1.Text = "Available";
      // 
      // DiskSpaceAvailabe
      // 
      this.DiskSpaceAvailabe.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DiskSpaceAvailabe.Location = new System.Drawing.Point (551, 40);
      this.DiskSpaceAvailabe.Name = "DiskSpaceAvailabe";
      this.DiskSpaceAvailabe.ReadOnly = true;
      this.DiskSpaceAvailabe.Size = new System.Drawing.Size (100, 26);
      this.DiskSpaceAvailabe.TabIndex = 8;
      this.DiskSpaceAvailabe.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // Download
      // 
      this.Download.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Download.Location = new System.Drawing.Point (2, 3);
      this.Download.Name = "Download";
      this.Download.Size = new System.Drawing.Size (103, 26);
      this.Download.TabIndex = 9;
      this.Download.Text = "Download";
      this.Download.UseVisualStyleBackColor = true;
      this.Download.Click += new System.EventHandler (this.Download_Click);
      // 
      // DownLoadDir
      // 
      this.DownLoadDir.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DownLoadDir.Location = new System.Drawing.Point (108, 3);
      this.DownLoadDir.Name = "DownLoadDir";
      this.DownLoadDir.ReadOnly = true;
      this.DownLoadDir.Size = new System.Drawing.Size (465, 26);
      this.DownLoadDir.TabIndex = 10;
      // 
      // FreeSpace
      // 
      this.FreeSpace.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FreeSpace.Location = new System.Drawing.Point (97, 3);
      this.FreeSpace.Name = "FreeSpace";
      this.FreeSpace.ReadOnly = true;
      this.FreeSpace.Size = new System.Drawing.Size (100, 26);
      this.FreeSpace.TabIndex = 11;
      this.FreeSpace.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // FreeSpaceLabel
      // 
      this.FreeSpaceLabel.AutoSize = true;
      this.FreeSpaceLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FreeSpaceLabel.Location = new System.Drawing.Point (4, 7);
      this.FreeSpaceLabel.Name = "FreeSpaceLabel";
      this.FreeSpaceLabel.Size = new System.Drawing.Size (91, 18);
      this.FreeSpaceLabel.TabIndex = 12;
      this.FreeSpaceLabel.Text = "Free Space";
      // 
      // SelectedLabel
      // 
      this.SelectedLabel.AutoSize = true;
      this.SelectedLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SelectedLabel.Location = new System.Drawing.Point (2, 7);
      this.SelectedLabel.Name = "SelectedLabel";
      this.SelectedLabel.Size = new System.Drawing.Size (70, 18);
      this.SelectedLabel.TabIndex = 14;
      this.SelectedLabel.Text = "Selected";
      // 
      // SelectedSpace
      // 
      this.SelectedSpace.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SelectedSpace.Location = new System.Drawing.Point (74, 3);
      this.SelectedSpace.Name = "SelectedSpace";
      this.SelectedSpace.ReadOnly = true;
      this.SelectedSpace.Size = new System.Drawing.Size (100, 26);
      this.SelectedSpace.TabIndex = 13;
      this.SelectedSpace.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // menuStrip1
      // 
      this.menuStrip1.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.exitToolStripMenuItem1});
      this.menuStrip1.Location = new System.Drawing.Point (0, 0);
      this.menuStrip1.Name = "menuStrip1";
      this.menuStrip1.Size = new System.Drawing.Size (1002, 24);
      this.menuStrip1.TabIndex = 15;
      this.menuStrip1.Text = "menuStrip1";
      // 
      // fileToolStripMenuItem
      // 
      this.fileToolStripMenuItem.DropDownItems.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.destinationToolStripMenuItem,
            this.resetDirectoryToolStripMenuItem,
            this.initializeDrivToolStripMenuItem,
            this.readDirectoryFromLogFilesToolStripMenuItem});
      this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
      this.fileToolStripMenuItem.Size = new System.Drawing.Size (37, 20);
      this.fileToolStripMenuItem.Text = "File";
      // 
      // destinationToolStripMenuItem
      // 
      this.destinationToolStripMenuItem.Name = "destinationToolStripMenuItem";
      this.destinationToolStripMenuItem.Size = new System.Drawing.Size (231, 22);
      this.destinationToolStripMenuItem.Text = "Destination";
      this.destinationToolStripMenuItem.Click += new System.EventHandler (this.destinationToolStripMenuItem_Click);
      // 
      // resetDirectoryToolStripMenuItem
      // 
      this.resetDirectoryToolStripMenuItem.Name = "resetDirectoryToolStripMenuItem";
      this.resetDirectoryToolStripMenuItem.Size = new System.Drawing.Size (231, 22);
      this.resetDirectoryToolStripMenuItem.Text = "Reset Directory";
      this.resetDirectoryToolStripMenuItem.Click += new System.EventHandler (this.resetDirectoryToolStripMenuItem_Click);
      // 
      // initializeDrivToolStripMenuItem
      // 
      this.initializeDrivToolStripMenuItem.Name = "initializeDrivToolStripMenuItem";
      this.initializeDrivToolStripMenuItem.Size = new System.Drawing.Size (231, 22);
      this.initializeDrivToolStripMenuItem.Text = "Initialize Drive";
      this.initializeDrivToolStripMenuItem.Click += new System.EventHandler (this.initializeDrivToolStripMenuItem_Click);
      // 
      // exitToolStripMenuItem1
      // 
      this.exitToolStripMenuItem1.Name = "exitToolStripMenuItem1";
      this.exitToolStripMenuItem1.Size = new System.Drawing.Size (37, 20);
      this.exitToolStripMenuItem1.Text = "Exit";
      this.exitToolStripMenuItem1.Click += new System.EventHandler (this.exitToolStripMenuItem1_Click);
      // 
      // CheckHardDriveTimer
      // 
      this.CheckHardDriveTimer.Enabled = true;
      this.CheckHardDriveTimer.Interval = 10000;
      this.CheckHardDriveTimer.Tick += new System.EventHandler (this.CheckHardDriveTimer_Tick);
      // 
      // ReFresh
      // 
      this.ReFresh.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ReFresh.Location = new System.Drawing.Point (886, 39);
      this.ReFresh.Name = "ReFresh";
      this.ReFresh.Size = new System.Drawing.Size (107, 29);
      this.ReFresh.TabIndex = 16;
      this.ReFresh.Text = "Refresh";
      this.ReFresh.UseVisualStyleBackColor = true;
      this.ReFresh.Click += new System.EventHandler (this.ReFresh_Click);
      // 
      // BottomPanel
      // 
      this.BottomPanel.Controls.Add (this.DownloadPanel);
      this.BottomPanel.Controls.Add (this.FreeSpacePanel);
      this.BottomPanel.Controls.Add (this.SelectedPanel);
      this.BottomPanel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.BottomPanel.Location = new System.Drawing.Point (12, 537);
      this.BottomPanel.Name = "BottomPanel";
      this.BottomPanel.Size = new System.Drawing.Size (980, 40);
      this.BottomPanel.TabIndex = 17;
      // 
      // DownloadPanel
      // 
      this.DownloadPanel.Controls.Add (this.DownLoadDir);
      this.DownloadPanel.Controls.Add (this.Download);
      this.DownloadPanel.Location = new System.Drawing.Point (3, 3);
      this.DownloadPanel.Name = "DownloadPanel";
      this.DownloadPanel.Size = new System.Drawing.Size (579, 33);
      this.DownloadPanel.TabIndex = 18;
      // 
      // FreeSpacePanel
      // 
      this.FreeSpacePanel.Controls.Add (this.FreeSpace);
      this.FreeSpacePanel.Controls.Add (this.FreeSpaceLabel);
      this.FreeSpacePanel.Location = new System.Drawing.Point (588, 3);
      this.FreeSpacePanel.Name = "FreeSpacePanel";
      this.FreeSpacePanel.Size = new System.Drawing.Size (204, 34);
      this.FreeSpacePanel.TabIndex = 18;
      // 
      // SelectedPanel
      // 
      this.SelectedPanel.Controls.Add (this.SelectedSpace);
      this.SelectedPanel.Controls.Add (this.SelectedLabel);
      this.SelectedPanel.Location = new System.Drawing.Point (798, 3);
      this.SelectedPanel.Name = "SelectedPanel";
      this.SelectedPanel.Size = new System.Drawing.Size (177, 34);
      this.SelectedPanel.TabIndex = 18;
      // 
      // readDirectoryFromLogFilesToolStripMenuItem
      // 
      this.readDirectoryFromLogFilesToolStripMenuItem.Name = "readDirectoryFromLogFilesToolStripMenuItem";
      this.readDirectoryFromLogFilesToolStripMenuItem.Size = new System.Drawing.Size (231, 22);
      this.readDirectoryFromLogFilesToolStripMenuItem.Text = "Read Directory From Log Files";
      this.readDirectoryFromLogFilesToolStripMenuItem.Click += new System.EventHandler (this.readDirectoryFromLogFilesToolStripMenuItem_Click);
      // 
      // SipperDiskManager
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (1002, 584);
      this.Controls.Add (this.BottomPanel);
      this.Controls.Add (this.ReFresh);
      this.Controls.Add (this.DiskSpaceAvailabe);
      this.Controls.Add (this.SpaceAvailableLabel1);
      this.Controls.Add (this.DiskSpaceUsed);
      this.Controls.Add (this.SpaceUsedLabel);
      this.Controls.Add (this.DiskSize);
      this.Controls.Add (this.DiskSizeLabel);
      this.Controls.Add (this.DriveNumber);
      this.Controls.Add (this.DriveNumberlabel);
      this.Controls.Add (this.SipperFileListView);
      this.Controls.Add (this.menuStrip1);
      this.MainMenuStrip = this.menuStrip1;
      this.MinimumSize = new System.Drawing.Size (1010, 618);
      this.Name = "SipperDiskManager";
      this.Text = "Sipper Disk Manager";
      this.Load += new System.EventHandler (this.SipperDiskManager_Load);
      this.Resize += new System.EventHandler (this.SipperDiskManager_Resize);
      this.ResizeEnd += new System.EventHandler (this.SipperDiskManager_ResizeEnd);
      this.menuStrip1.ResumeLayout (false);
      this.menuStrip1.PerformLayout ();
      this.BottomPanel.ResumeLayout (false);
      this.DownloadPanel.ResumeLayout (false);
      this.DownloadPanel.PerformLayout ();
      this.FreeSpacePanel.ResumeLayout (false);
      this.FreeSpacePanel.PerformLayout ();
      this.SelectedPanel.ResumeLayout (false);
      this.SelectedPanel.PerformLayout ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.ListView SipperFileListView;
    private System.Windows.Forms.ColumnHeader FileName;
    private System.Windows.Forms.ColumnHeader DateStamp;
    private System.Windows.Forms.ColumnHeader FileSize;
    private System.Windows.Forms.ColumnHeader Description;
    private System.Windows.Forms.Label DriveNumberlabel;
    private System.Windows.Forms.TextBox DriveNumber;
    private System.Windows.Forms.Label DiskSizeLabel;
    private System.Windows.Forms.TextBox DiskSize;
    private System.Windows.Forms.Label SpaceUsedLabel;
    private System.Windows.Forms.TextBox DiskSpaceUsed;
    private System.Windows.Forms.Label SpaceAvailableLabel1;
    private System.Windows.Forms.TextBox DiskSpaceAvailabe;
    private System.Windows.Forms.Button Download;
    private System.Windows.Forms.TextBox DownLoadDir;
    private System.Windows.Forms.TextBox FreeSpace;
    private System.Windows.Forms.Label FreeSpaceLabel;
    private System.Windows.Forms.Label SelectedLabel;
    private System.Windows.Forms.TextBox SelectedSpace;
    private System.Windows.Forms.MenuStrip menuStrip1;
    private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem destinationToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem resetDirectoryToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem initializeDrivToolStripMenuItem;
    private System .Windows .Forms .ToolStripMenuItem exitToolStripMenuItem1;
    private System .Windows .Forms .Timer CheckHardDriveTimer;
    private System .Windows .Forms .Button ReFresh;
    private System .Windows .Forms .FlowLayoutPanel BottomPanel;
    private System .Windows .Forms .Panel DownloadPanel;
    private System .Windows .Forms .Panel FreeSpacePanel;
    private System .Windows .Forms .Panel SelectedPanel;
    private System .Windows .Forms .ColumnHeader StartSector;
    private System .Windows .Forms .ColumnHeader EndSector;
    private System.Windows.Forms.ToolStripMenuItem readDirectoryFromLogFilesToolStripMenuItem;
  }
}

