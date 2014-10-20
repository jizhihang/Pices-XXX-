namespace SipperFile
{
  partial class DataRowListLoadStatus
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
      this.TitleLabel = new System.Windows.Forms.Label();
      this.fileNameLabel = new System.Windows.Forms.Label();
      this.DateTimeLabel = new System.Windows.Forms.Label();
      this.RecordCountLabel = new System.Windows.Forms.Label();
      this.fileName = new System.Windows.Forms.TextBox();
      this.dateTime = new System.Windows.Forms.TextBox();
      this.recordCount = new System.Windows.Forms.TextBox();
      this.SuspendLayout();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point(131, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size(140, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Loading History";
      // 
      // fileNameLabel
      // 
      this.fileNameLabel.AutoSize = true;
      this.fileNameLabel.Font = new System.Drawing.Font("Courier New", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.fileNameLabel.Location = new System.Drawing.Point(24, 55);
      this.fileNameLabel.Name = "fileNameLabel";
      this.fileNameLabel.Size = new System.Drawing.Size(134, 17);
      this.fileNameLabel.TabIndex = 1;
      this.fileNameLabel.Text = "File Name    :";
      // 
      // DateTimeLabel
      // 
      this.DateTimeLabel.AutoSize = true;
      this.DateTimeLabel.Font = new System.Drawing.Font("Courier New", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateTimeLabel.Location = new System.Drawing.Point(24, 91);
      this.DateTimeLabel.Name = "DateTimeLabel";
      this.DateTimeLabel.Size = new System.Drawing.Size(134, 17);
      this.DateTimeLabel.TabIndex = 2;
      this.DateTimeLabel.Text = "Date/Time    :";
      // 
      // RecordCountLabel
      // 
      this.RecordCountLabel.AutoSize = true;
      this.RecordCountLabel.Font = new System.Drawing.Font("Courier New", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.RecordCountLabel.Location = new System.Drawing.Point(24, 130);
      this.RecordCountLabel.Name = "RecordCountLabel";
      this.RecordCountLabel.Size = new System.Drawing.Size(134, 17);
      this.RecordCountLabel.TabIndex = 3;
      this.RecordCountLabel.Text = "Record Count :";
      // 
      // fileName
      // 
      this.fileName.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.fileName.Location = new System.Drawing.Point(160, 53);
      this.fileName.Name = "fileName";
      this.fileName.Size = new System.Drawing.Size(236, 24);
      this.fileName.TabIndex = 4;
      // 
      // dateTime
      // 
      this.dateTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.dateTime.Location = new System.Drawing.Point(160, 89);
      this.dateTime.Name = "dateTime";
      this.dateTime.Size = new System.Drawing.Size(107, 24);
      this.dateTime.TabIndex = 5;
      // 
      // recordCount
      // 
      this.recordCount.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.recordCount.Location = new System.Drawing.Point(160, 128);
      this.recordCount.Name = "recordCount";
      this.recordCount.Size = new System.Drawing.Size(107, 24);
      this.recordCount.TabIndex = 6;
      // 
      // DataRowListLoadStatus
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(412, 180);
      this.Controls.Add(this.recordCount);
      this.Controls.Add(this.dateTime);
      this.Controls.Add(this.fileName);
      this.Controls.Add(this.RecordCountLabel);
      this.Controls.Add(this.DateTimeLabel);
      this.Controls.Add(this.fileNameLabel);
      this.Controls.Add(this.TitleLabel);
      this.Name = "DataRowListLoadStatus";
      this.Text = "DataRowListLoadStatus";
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label fileNameLabel;
    private System.Windows.Forms.Label DateTimeLabel;
    private System.Windows.Forms.Label RecordCountLabel;
    private System.Windows.Forms.TextBox fileName;
    private System.Windows.Forms.TextBox dateTime;
    private System.Windows.Forms.TextBox recordCount;
  }
}