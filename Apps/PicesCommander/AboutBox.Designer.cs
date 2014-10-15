namespace PicesCommander
{
  partial class AboutBox
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
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
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (AboutBox));
      this.tableLayoutPanel = new System.Windows.Forms.TableLayoutPanel ();
      this.logoPictureBox = new System.Windows.Forms.PictureBox ();
      this.labelProductName = new System.Windows.Forms.Label ();
      this.labelVersion = new System.Windows.Forms.Label ();
      this.textBoxDescription = new System.Windows.Forms.TextBox ();
      this.okButton = new System.Windows.Forms.Button ();
      this.tableLayoutPanel.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.logoPictureBox)).BeginInit ();
      this.SuspendLayout ();
      // 
      // tableLayoutPanel
      // 
      this.tableLayoutPanel.ColumnCount = 2;
      this.tableLayoutPanel.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 22.34146F));
      this.tableLayoutPanel.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 77.65854F));
      this.tableLayoutPanel.Controls.Add (this.logoPictureBox, 0, 0);
      this.tableLayoutPanel.Controls.Add (this.labelProductName, 1, 0);
      this.tableLayoutPanel.Controls.Add (this.labelVersion, 1, 1);
      this.tableLayoutPanel.Controls.Add (this.textBoxDescription, 1, 4);
      this.tableLayoutPanel.Controls.Add (this.okButton, 1, 5);
      this.tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.tableLayoutPanel.Location = new System.Drawing.Point (9, 9);
      this.tableLayoutPanel.Name = "tableLayoutPanel";
      this.tableLayoutPanel.RowCount = 6;
      this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 10F));
      this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 10F));
      this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 10F));
      this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 5.331179F));
      this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 54.44265F));
      this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 10F));
      this.tableLayoutPanel.Size = new System.Drawing.Size (1025, 248);
      this.tableLayoutPanel.TabIndex = 0;
      // 
      // logoPictureBox
      // 
      this.logoPictureBox.Dock = System.Windows.Forms.DockStyle.Fill;
      this.logoPictureBox.Image = ((System.Drawing.Image)(resources.GetObject ("logoPictureBox.Image")));
      this.logoPictureBox.Location = new System.Drawing.Point (3, 3);
      this.logoPictureBox.Name = "logoPictureBox";
      this.tableLayoutPanel.SetRowSpan (this.logoPictureBox, 6);
      this.logoPictureBox.Size = new System.Drawing.Size (222, 242);
      this.logoPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
      this.logoPictureBox.TabIndex = 12;
      this.logoPictureBox.TabStop = false;
      // 
      // labelProductName
      // 
      this.labelProductName.Dock = System.Windows.Forms.DockStyle.Fill;
      this.labelProductName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.labelProductName.Location = new System.Drawing.Point (234, 0);
      this.labelProductName.Margin = new System.Windows.Forms.Padding (6, 0, 3, 0);
      this.labelProductName.MaximumSize = new System.Drawing.Size (0, 17);
      this.labelProductName.Name = "labelProductName";
      this.labelProductName.Size = new System.Drawing.Size (788, 17);
      this.labelProductName.TabIndex = 19;
      this.labelProductName.Text = "PICES Commander  (Plankton Imaging Classification and Evaluation System)";
      this.labelProductName.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // labelVersion
      // 
      this.labelVersion.Dock = System.Windows.Forms.DockStyle.Fill;
      this.labelVersion.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.labelVersion.Location = new System.Drawing.Point (234, 24);
      this.labelVersion.Margin = new System.Windows.Forms.Padding (6, 0, 3, 0);
      this.labelVersion.MaximumSize = new System.Drawing.Size (0, 17);
      this.labelVersion.Name = "labelVersion";
      this.labelVersion.Size = new System.Drawing.Size (788, 17);
      this.labelVersion.TabIndex = 0;
      this.labelVersion.Text = "Version 1.02   April-02-2013";
      this.labelVersion.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      this.labelVersion.Click += new System.EventHandler (this.labelVersion_Click);
      // 
      // textBoxDescription
      // 
      this.textBoxDescription.Dock = System.Windows.Forms.DockStyle.Fill;
      this.textBoxDescription.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.textBoxDescription.Location = new System.Drawing.Point (234, 88);
      this.textBoxDescription.Margin = new System.Windows.Forms.Padding (6, 3, 3, 3);
      this.textBoxDescription.Multiline = true;
      this.textBoxDescription.Name = "textBoxDescription";
      this.textBoxDescription.ReadOnly = true;
      this.textBoxDescription.ScrollBars = System.Windows.Forms.ScrollBars.Both;
      this.textBoxDescription.Size = new System.Drawing.Size (788, 129);
      this.textBoxDescription.TabIndex = 23;
      this.textBoxDescription.TabStop = false;
      this.textBoxDescription.Text = resources.GetString ("textBoxDescription.Text");
      // 
      // okButton
      // 
      this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
      this.okButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this.okButton.Location = new System.Drawing.Point (947, 223);
      this.okButton.Name = "okButton";
      this.okButton.Size = new System.Drawing.Size (75, 22);
      this.okButton.TabIndex = 24;
      this.okButton.Text = "&OK";
      // 
      // AboutBox
      // 
      this.AcceptButton = this.okButton;
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (1043, 266);
      this.Controls.Add (this.tableLayoutPanel);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "AboutBox";
      this.Padding = new System.Windows.Forms.Padding (9);
      this.ShowIcon = false;
      this.ShowInTaskbar = false;
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "AboutBox";
      this.tableLayoutPanel.ResumeLayout (false);
      this.tableLayoutPanel.PerformLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.logoPictureBox)).EndInit ();
      this.ResumeLayout (false);

    }

    #endregion

    private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
    private System.Windows.Forms.PictureBox logoPictureBox;
    private System.Windows.Forms.Label labelProductName;
    private System.Windows.Forms.Label labelVersion;
    private System.Windows.Forms.TextBox textBoxDescription;
    private System.Windows.Forms.Button okButton;
  }
}
