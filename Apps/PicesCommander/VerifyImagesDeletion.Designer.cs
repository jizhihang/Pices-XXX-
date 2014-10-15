namespace PicesCommander
{
  partial class VerifyImagesDeletion
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
      this.ThumbnailDataGridView = new System.Windows.Forms.DataGridView ();
      this.ThumbNail = new System.Windows.Forms.DataGridViewImageColumn ();
      this.MessageLabel = new System.Windows.Forms.Label ();
      this.TypeYesLabel = new System.Windows.Forms.Label ();
      this.YesToConfirm = new System.Windows.Forms.TextBox ();
      this.DeleteButton = new System.Windows.Forms.Button ();
      this.CancelButton = new System.Windows.Forms.Button ();
      ((System.ComponentModel.ISupportInitialize)(this.ThumbnailDataGridView)).BeginInit ();
      this.SuspendLayout ();
      // 
      // ThumbnailDataGridView
      // 
      this.ThumbnailDataGridView.AllowUserToAddRows = false;
      this.ThumbnailDataGridView.AllowUserToDeleteRows = false;
      this.ThumbnailDataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.ThumbnailDataGridView.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.ThumbNail});
      this.ThumbnailDataGridView.Location = new System.Drawing.Point (12, 131);
      this.ThumbnailDataGridView.Name = "ThumbnailDataGridView";
      this.ThumbnailDataGridView.RowHeadersVisible = false;
      this.ThumbnailDataGridView.ShowEditingIcon = false;
      this.ThumbnailDataGridView.Size = new System.Drawing.Size (624, 492);
      this.ThumbnailDataGridView.TabIndex = 0;
      this.ThumbnailDataGridView.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.ThumbnailDataGridView_CellFormatting);
      this.ThumbnailDataGridView.DataError += new System.Windows.Forms.DataGridViewDataErrorEventHandler (this.ThumbnailDataGridView_DataError);
      // 
      // ThumbNail
      // 
      this.ThumbNail.HeaderText = "Column1";
      this.ThumbNail.Name = "ThumbNail";
      this.ThumbNail.Width = 150;
      // 
      // MessageLabel
      // 
      this.MessageLabel.AutoSize = true;
      this.MessageLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MessageLabel.ForeColor = System.Drawing.Color.Red;
      this.MessageLabel.Location = new System.Drawing.Point (6, 27);
      this.MessageLabel.Name = "MessageLabel";
      this.MessageLabel.Size = new System.Drawing.Size (630, 31);
      this.MessageLabel.TabIndex = 1;
      this.MessageLabel.Text = "Are you sure you want to delete these images ?";
      // 
      // TypeYesLabel
      // 
      this.TypeYesLabel.AutoSize = true;
      this.TypeYesLabel.Location = new System.Drawing.Point (260, 80);
      this.TypeYesLabel.Name = "TypeYesLabel";
      this.TypeYesLabel.Size = new System.Drawing.Size (102, 13);
      this.TypeYesLabel.TabIndex = 2;
      this.TypeYesLabel.Text = "Type yes to confirm.";
      // 
      // YesToConfirm
      // 
      this.YesToConfirm.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.YesToConfirm.Location = new System.Drawing.Point (263, 96);
      this.YesToConfirm.Name = "YesToConfirm";
      this.YesToConfirm.Size = new System.Drawing.Size (99, 29);
      this.YesToConfirm.TabIndex = 3;
      this.YesToConfirm.Text = "NO";
      this.YesToConfirm.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // DeleteButton
      // 
      this.DeleteButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DeleteButton.ForeColor = System.Drawing.Color.Red;
      this.DeleteButton.Location = new System.Drawing.Point (12, 639);
      this.DeleteButton.Name = "DeleteButton";
      this.DeleteButton.Size = new System.Drawing.Size (99, 33);
      this.DeleteButton.TabIndex = 4;
      this.DeleteButton.TabStop = false;
      this.DeleteButton.Text = "Delete";
      this.DeleteButton.UseVisualStyleBackColor = true;
      this.DeleteButton.Click += new System.EventHandler (this.DeleteButton_Click);
      // 
      // CancelButton
      // 
      this.CancelButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CancelButton.Location = new System.Drawing.Point (495, 639);
      this.CancelButton.Name = "CancelButton";
      this.CancelButton.Size = new System.Drawing.Size (141, 33);
      this.CancelButton.TabIndex = 6;
      this.CancelButton.TabStop = false;
      this.CancelButton.Text = "Cancel";
      this.CancelButton.UseVisualStyleBackColor = true;
      this.CancelButton.Click += new System.EventHandler (this.CancelButton_Click);
      // 
      // VerifyImagesDeletion
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (647, 680);
      this.Controls.Add (this.CancelButton);
      this.Controls.Add (this.DeleteButton);
      this.Controls.Add (this.YesToConfirm);
      this.Controls.Add (this.TypeYesLabel);
      this.Controls.Add (this.MessageLabel);
      this.Controls.Add (this.ThumbnailDataGridView);
      this.MinimumSize = new System.Drawing.Size (663, 718);
      this.Name = "VerifyImagesDeletion";
      this.Text = "VerifyImagesDeletion";
      this.Load += new System.EventHandler (this.VerifyImagesDeletion_Load);
      this.SizeChanged += new System.EventHandler (this.VerifyImagesDeletion_SizeChanged);
      this.ResizeEnd += new System.EventHandler (this.VerifyImagesDeletion_ResizeEnd);
      ((System.ComponentModel.ISupportInitialize)(this.ThumbnailDataGridView)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.DataGridView ThumbnailDataGridView;
    private System.Windows.Forms.DataGridViewImageColumn ThumbNail;
    private System.Windows.Forms.Label MessageLabel;
    private System.Windows.Forms.Label TypeYesLabel;
    private System.Windows.Forms.TextBox YesToConfirm;
    private System.Windows.Forms.Button DeleteButton;
    private System.Windows.Forms.Button CancelButton;
  }
}