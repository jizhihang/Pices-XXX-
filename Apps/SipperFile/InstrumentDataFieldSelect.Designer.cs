namespace SipperFile
{
  partial class InstrumentDataFieldSelect
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
      this.DataFieldList = new System.Windows.Forms.ListBox ();
      this.SuspendLayout ();
      // 
      // DataFieldList
      // 
      this.DataFieldList.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DataFieldList.FormattingEnabled = true;
      this.DataFieldList.ItemHeight = 20;
      this.DataFieldList.Location = new System.Drawing.Point (11, 9);
      this.DataFieldList.Name = "DataFieldList";
      this.DataFieldList.Size = new System.Drawing.Size (265, 584);
      this.DataFieldList.TabIndex = 0;
      this.DataFieldList.MouseClick += new System.Windows.Forms.MouseEventHandler (this.DataFieldList_MouseClick);
      this.DataFieldList.KeyPress += new System.Windows.Forms.KeyPressEventHandler (this.DataFieldList_KeyPress);
      // 
      // InstrumentDataFieldSelect
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (284, 604);
      this.Controls.Add (this.DataFieldList);
      this.Name = "InstrumentDataFieldSelect";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "InstrumentDataFieldSelect";
      this.ResumeLayout (false);

    }

    #endregion

    private System.Windows.Forms.ListBox DataFieldList;
  }
}