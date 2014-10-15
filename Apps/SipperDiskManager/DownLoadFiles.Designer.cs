namespace SipperDiskManager
{
  partial class DownLoadFiles
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System .ComponentModel .IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose (bool disposing)
    {
      if (disposing && (components != null))
      {
        components .Dispose ();
      }
      base .Dispose (disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent ()
    {
      this.components = new System.ComponentModel.Container ();
      this.TitleLabel = new System.Windows.Forms.Label ();
      this.CurrentFileProgress = new System.Windows.Forms.ProgressBar ();
      this.AllFilesProgress = new System.Windows.Forms.ProgressBar ();
      this.CurrentFilePercentDone = new System.Windows.Forms.TextBox ();
      this.AllFilesPercentDone = new System.Windows.Forms.TextBox ();
      this.FileListView = new System.Windows.Forms.ListView ();
      this.FileName = new System.Windows.Forms.ColumnHeader ();
      this.FileSize = new System.Windows.Forms.ColumnHeader ();
      this.Copied = new System.Windows.Forms.ColumnHeader ();
      this.LeftToCopy = new System.Windows.Forms.ColumnHeader ();
      this.FilesLabel = new System.Windows.Forms.Label ();
      this.FilesTotal = new System.Windows.Forms.TextBox ();
      this.FilesCopied = new System.Windows.Forms.TextBox ();
      this.MegTotal = new System.Windows.Forms.TextBox ();
      this.MegLabel = new System.Windows.Forms.Label ();
      this.CopiedLabel = new System.Windows.Forms.Label ();
      this.TotalLabel = new System.Windows.Forms.Label ();
      this.MegCopied = new System.Windows.Forms.TextBox ();
      this.LeftLabel = new System.Windows.Forms.Label ();
      this.FilesLeft = new System.Windows.Forms.TextBox ();
      this.MegLeft = new System.Windows.Forms.TextBox ();
      this.panel1 = new System.Windows.Forms.Panel ();
      this.CancelRequestButton = new System.Windows.Forms.Button ();
      this.ScreenUpdateTimer = new System.Windows.Forms.Timer (this.components);
      this.BytesSkipped = new System.Windows.Forms.ColumnHeader ();
      this.panel1.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.TitleLabel.Font = new System.Drawing.Font ("Arial", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (165, 11);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (309, 34);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Download SIPPER Files";
      // 
      // CurrentFileProgress
      // 
      this.CurrentFileProgress.Location = new System.Drawing.Point (18, 470);
      this.CurrentFileProgress.Name = "CurrentFileProgress";
      this.CurrentFileProgress.Size = new System.Drawing.Size (543, 22);
      this.CurrentFileProgress.TabIndex = 0;
      this.CurrentFileProgress.Value = 20;
      // 
      // AllFilesProgress
      // 
      this.AllFilesProgress.Location = new System.Drawing.Point (18, 511);
      this.AllFilesProgress.Name = "AllFilesProgress";
      this.AllFilesProgress.Size = new System.Drawing.Size (543, 22);
      this.AllFilesProgress.TabIndex = 1;
      this.AllFilesProgress.Value = 20;
      // 
      // CurrentFilePercentDone
      // 
      this.CurrentFilePercentDone.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CurrentFilePercentDone.Location = new System.Drawing.Point (561, 470);
      this.CurrentFilePercentDone.Name = "CurrentFilePercentDone";
      this.CurrentFilePercentDone.ReadOnly = true;
      this.CurrentFilePercentDone.Size = new System.Drawing.Size (86, 22);
      this.CurrentFilePercentDone.TabIndex = 2;
      // 
      // AllFilesPercentDone
      // 
      this.AllFilesPercentDone.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.AllFilesPercentDone.Location = new System.Drawing.Point (561, 511);
      this.AllFilesPercentDone.Name = "AllFilesPercentDone";
      this.AllFilesPercentDone.ReadOnly = true;
      this.AllFilesPercentDone.Size = new System.Drawing.Size (86, 22);
      this.AllFilesPercentDone.TabIndex = 3;
      // 
      // FileListView
      // 
      this.FileListView.Columns.AddRange (new System.Windows.Forms.ColumnHeader[] {
            this.FileName,
            this.FileSize,
            this.Copied,
            this.LeftToCopy,
            this.BytesSkipped});
      this.FileListView.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FileListView.Location = new System.Drawing.Point (12, 198);
      this.FileListView.Name = "FileListView";
      this.FileListView.Size = new System.Drawing.Size (635, 254);
      this.FileListView.TabIndex = 4;
      this.FileListView.UseCompatibleStateImageBehavior = false;
      this.FileListView.View = System.Windows.Forms.View.Details;
      // 
      // FileName
      // 
      this.FileName.Text = "File Name";
      this.FileName.Width = 182;
      // 
      // FileSize
      // 
      this.FileSize.Text = "Size";
      this.FileSize.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.FileSize.Width = 110;
      // 
      // Copied
      // 
      this.Copied.Text = "Copied";
      this.Copied.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.Copied.Width = 110;
      // 
      // LeftToCopy
      // 
      this.LeftToCopy.Text = "Left";
      this.LeftToCopy.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.LeftToCopy.Width = 110;
      // 
      // FilesLabel
      // 
      this.FilesLabel.AutoSize = true;
      this.FilesLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FilesLabel.Location = new System.Drawing.Point (22, 36);
      this.FilesLabel.Name = "FilesLabel";
      this.FilesLabel.Size = new System.Drawing.Size (42, 18);
      this.FilesLabel.TabIndex = 5;
      this.FilesLabel.Text = "Files";
      // 
      // FilesTotal
      // 
      this.FilesTotal.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FilesTotal.Location = new System.Drawing.Point (74, 32);
      this.FilesTotal.Name = "FilesTotal";
      this.FilesTotal.ReadOnly = true;
      this.FilesTotal.Size = new System.Drawing.Size (95, 26);
      this.FilesTotal.TabIndex = 6;
      this.FilesTotal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // FilesCopied
      // 
      this.FilesCopied.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FilesCopied.Location = new System.Drawing.Point (210, 32);
      this.FilesCopied.Name = "FilesCopied";
      this.FilesCopied.ReadOnly = true;
      this.FilesCopied.Size = new System.Drawing.Size (95, 26);
      this.FilesCopied.TabIndex = 8;
      this.FilesCopied.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // MegTotal
      // 
      this.MegTotal.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MegTotal.Location = new System.Drawing.Point (74, 68);
      this.MegTotal.Name = "MegTotal";
      this.MegTotal.ReadOnly = true;
      this.MegTotal.Size = new System.Drawing.Size (95, 26);
      this.MegTotal.TabIndex = 10;
      this.MegTotal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // MegLabel
      // 
      this.MegLabel.AutoSize = true;
      this.MegLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MegLabel.Location = new System.Drawing.Point (24, 72);
      this.MegLabel.Name = "MegLabel";
      this.MegLabel.Size = new System.Drawing.Size (39, 18);
      this.MegLabel.TabIndex = 9;
      this.MegLabel.Text = "Meg";
      // 
      // CopiedLabel
      // 
      this.CopiedLabel.AutoSize = true;
      this.CopiedLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CopiedLabel.Location = new System.Drawing.Point (227, 9);
      this.CopiedLabel.Name = "CopiedLabel";
      this.CopiedLabel.Size = new System.Drawing.Size (60, 18);
      this.CopiedLabel.TabIndex = 7;
      this.CopiedLabel.Text = "Copied";
      // 
      // TotalLabel
      // 
      this.TotalLabel.AutoSize = true;
      this.TotalLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TotalLabel.Location = new System.Drawing.Point (100, 9);
      this.TotalLabel.Name = "TotalLabel";
      this.TotalLabel.Size = new System.Drawing.Size (40, 18);
      this.TotalLabel.TabIndex = 11;
      this.TotalLabel.Text = "Total";
      // 
      // MegCopied
      // 
      this.MegCopied.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MegCopied.Location = new System.Drawing.Point (209, 68);
      this.MegCopied.Name = "MegCopied";
      this.MegCopied.ReadOnly = true;
      this.MegCopied.Size = new System.Drawing.Size (95, 26);
      this.MegCopied.TabIndex = 12;
      this.MegCopied.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // LeftLabel
      // 
      this.LeftLabel.AutoSize = true;
      this.LeftLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.LeftLabel.Location = new System.Drawing.Point (368, 9);
      this.LeftLabel.Name = "LeftLabel";
      this.LeftLabel.Size = new System.Drawing.Size (34, 18);
      this.LeftLabel.TabIndex = 13;
      this.LeftLabel.Text = "Left";
      // 
      // FilesLeft
      // 
      this.FilesLeft.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FilesLeft.Location = new System.Drawing.Point (342, 32);
      this.FilesLeft.Name = "FilesLeft";
      this.FilesLeft.ReadOnly = true;
      this.FilesLeft.Size = new System.Drawing.Size (95, 26);
      this.FilesLeft.TabIndex = 14;
      this.FilesLeft.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // MegLeft
      // 
      this.MegLeft.Font = new System.Drawing.Font ("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MegLeft.Location = new System.Drawing.Point (342, 68);
      this.MegLeft.Name = "MegLeft";
      this.MegLeft.ReadOnly = true;
      this.MegLeft.Size = new System.Drawing.Size (95, 26);
      this.MegLeft.TabIndex = 15;
      this.MegLeft.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // panel1
      // 
      this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.panel1.Controls.Add (this.FilesCopied);
      this.panel1.Controls.Add (this.MegLeft);
      this.panel1.Controls.Add (this.FilesLabel);
      this.panel1.Controls.Add (this.FilesLeft);
      this.panel1.Controls.Add (this.FilesTotal);
      this.panel1.Controls.Add (this.LeftLabel);
      this.panel1.Controls.Add (this.CopiedLabel);
      this.panel1.Controls.Add (this.MegCopied);
      this.panel1.Controls.Add (this.MegLabel);
      this.panel1.Controls.Add (this.TotalLabel);
      this.panel1.Controls.Add (this.MegTotal);
      this.panel1.Location = new System.Drawing.Point (87, 64);
      this.panel1.Name = "panel1";
      this.panel1.Size = new System.Drawing.Size (466, 114);
      this.panel1.TabIndex = 16;
      // 
      // CancelRequestButton
      // 
      this.CancelRequestButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CancelRequestButton.Location = new System.Drawing.Point (281, 550);
      this.CancelRequestButton.Name = "CancelRequestButton";
      this.CancelRequestButton.Size = new System.Drawing.Size (122, 44);
      this.CancelRequestButton.TabIndex = 17;
      this.CancelRequestButton.Text = "Cancel";
      this.CancelRequestButton.UseVisualStyleBackColor = true;
      this.CancelRequestButton.Click += new System.EventHandler (this.CancelRequestButton_Click);
      // 
      // ScreenUpdateTimer
      // 
      this.ScreenUpdateTimer.Enabled = true;
      this.ScreenUpdateTimer.Interval = 250;
      this.ScreenUpdateTimer.Tick += new System.EventHandler (this.ScreenUpdateTimer_Tick);
      // 
      // BytesSkipped
      // 
      this.BytesSkipped.Text = "Skipped";
      this.BytesSkipped.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.BytesSkipped.Width = 110;
      // 
      // DownLoadFiles
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (673, 606);
      this.Controls.Add (this.CancelRequestButton);
      this.Controls.Add (this.panel1);
      this.Controls.Add (this.FileListView);
      this.Controls.Add (this.AllFilesPercentDone);
      this.Controls.Add (this.CurrentFilePercentDone);
      this.Controls.Add (this.AllFilesProgress);
      this.Controls.Add (this.CurrentFileProgress);
      this.Controls.Add (this.TitleLabel);
      this.Name = "DownLoadFiles";
      this.Text = "DownLoadFiles";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.DownLoadFiles_FormClosing);
      this.panel1.ResumeLayout (false);
      this.panel1.PerformLayout ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System .Windows .Forms .Label TitleLabel;
    private System .Windows .Forms .ProgressBar CurrentFileProgress;
    private System .Windows .Forms .ProgressBar AllFilesProgress;
    private System .Windows .Forms .TextBox CurrentFilePercentDone;
    private System .Windows .Forms .TextBox AllFilesPercentDone;
    private System .Windows .Forms .ListView FileListView;
    private System .Windows .Forms .Label FilesLabel;
    private System .Windows .Forms .TextBox FilesTotal;
    private System .Windows .Forms .TextBox FilesCopied;
    private System .Windows .Forms .TextBox MegTotal;
    private System .Windows .Forms .Label MegLabel;
    private System .Windows .Forms .Label CopiedLabel;
    private System .Windows .Forms .Label TotalLabel;
    private System .Windows .Forms .TextBox MegCopied;
    private System .Windows .Forms .Label LeftLabel;
    private System .Windows .Forms .TextBox FilesLeft;
    private System .Windows .Forms .TextBox MegLeft;
    private System .Windows .Forms .Panel panel1;
    private System .Windows .Forms .ColumnHeader FileName;
    private System .Windows .Forms .ColumnHeader FileSize;
    private System .Windows .Forms .ColumnHeader Copied;
    private System .Windows .Forms .ColumnHeader LeftToCopy;
    private System.Windows.Forms.Button CancelRequestButton;
    private System.Windows.Forms.Timer ScreenUpdateTimer;
    private System.Windows.Forms.ColumnHeader BytesSkipped;
  }
}