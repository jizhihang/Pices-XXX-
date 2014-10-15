namespace FlatFieldSipper4Image
{
  partial class Sipper4ImageViewer
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
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (Sipper4ImageViewer));
      this.Sipper4Image = new System.Windows.Forms.PictureBox ();
      this.LoadImage = new System.Windows.Forms.Button ();
      this.panel1 = new System.Windows.Forms.Panel ();
      this.FlatFieldButon = new System.Windows.Forms.Button ();
      this.SaveButton = new System.Windows.Forms.Button ();
      this.IntensityHistogram = new System.Windows.Forms.DataGridView ();
      this.IntensityCol = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.FreqCol = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      ((System.ComponentModel.ISupportInitialize)(this.Sipper4Image)).BeginInit ();
      this.panel1.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.IntensityHistogram)).BeginInit ();
      this.SuspendLayout ();
      // 
      // Sipper4Image
      // 
      this.Sipper4Image.Image = ((System.Drawing.Image)(resources.GetObject ("Sipper4Image.Image")));
      this.Sipper4Image.Location = new System.Drawing.Point (3, 2);
      this.Sipper4Image.Name = "Sipper4Image";
      this.Sipper4Image.Size = new System.Drawing.Size (733, 727);
      this.Sipper4Image.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
      this.Sipper4Image.TabIndex = 0;
      this.Sipper4Image.TabStop = false;
      // 
      // LoadImage
      // 
      this.LoadImage.Location = new System.Drawing.Point (13, 67);
      this.LoadImage.Name = "LoadImage";
      this.LoadImage.Size = new System.Drawing.Size (75, 23);
      this.LoadImage.TabIndex = 1;
      this.LoadImage.Text = "Load Image";
      this.LoadImage.UseVisualStyleBackColor = true;
      this.LoadImage.Click += new System.EventHandler (this.LoadImage_Click);
      // 
      // panel1
      // 
      this.panel1.AutoScroll = true;
      this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.panel1.Controls.Add (this.Sipper4Image);
      this.panel1.Location = new System.Drawing.Point (94, 51);
      this.panel1.Name = "panel1";
      this.panel1.Size = new System.Drawing.Size (741, 734);
      this.panel1.TabIndex = 2;
      // 
      // FlatFieldButon
      // 
      this.FlatFieldButon.Location = new System.Drawing.Point (13, 108);
      this.FlatFieldButon.Name = "FlatFieldButon";
      this.FlatFieldButon.Size = new System.Drawing.Size (75, 23);
      this.FlatFieldButon.TabIndex = 3;
      this.FlatFieldButon.Text = "Flat Field";
      this.FlatFieldButon.UseVisualStyleBackColor = true;
      this.FlatFieldButon.Click += new System.EventHandler (this.FlatFieldButon_Click);
      // 
      // SaveButton
      // 
      this.SaveButton.Location = new System.Drawing.Point (12, 151);
      this.SaveButton.Name = "SaveButton";
      this.SaveButton.Size = new System.Drawing.Size (75, 23);
      this.SaveButton.TabIndex = 4;
      this.SaveButton.Text = "Save";
      this.SaveButton.UseVisualStyleBackColor = true;
      this.SaveButton.Click += new System.EventHandler (this.SaveButton_Click);
      // 
      // IntensityHistogram
      // 
      this.IntensityHistogram.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.IntensityHistogram.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.IntensityCol,
            this.FreqCol});
      this.IntensityHistogram.Location = new System.Drawing.Point (841, 51);
      this.IntensityHistogram.Name = "IntensityHistogram";
      this.IntensityHistogram.Size = new System.Drawing.Size (300, 727);
      this.IntensityHistogram.TabIndex = 5;
      // 
      // IntensityCol
      // 
      this.IntensityCol.HeaderText = "Intensity";
      this.IntensityCol.Name = "IntensityCol";
      this.IntensityCol.ReadOnly = true;
      // 
      // FreqCol
      // 
      this.FreqCol.HeaderText = "Freq";
      this.FreqCol.Name = "FreqCol";
      this.FreqCol.ReadOnly = true;
      // 
      // Sipper4ImageViewer
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (1153, 795);
      this.Controls.Add (this.IntensityHistogram);
      this.Controls.Add (this.SaveButton);
      this.Controls.Add (this.FlatFieldButon);
      this.Controls.Add (this.panel1);
      this.Controls.Add (this.LoadImage);
      this.Name = "Sipper4ImageViewer";
      this.Text = "FlatFieldSipper4Image";
      ((System.ComponentModel.ISupportInitialize)(this.Sipper4Image)).EndInit ();
      this.panel1.ResumeLayout (false);
      ((System.ComponentModel.ISupportInitialize)(this.IntensityHistogram)).EndInit ();
      this.ResumeLayout (false);

    }

    #endregion

    private System.Windows.Forms.PictureBox Sipper4Image;
    private System.Windows.Forms.Button LoadImage;
    private System.Windows.Forms.Panel panel1;
    private System.Windows.Forms.Button FlatFieldButon;
    private System.Windows.Forms.Button SaveButton;
    private System.Windows.Forms.DataGridView IntensityHistogram;
    private System.Windows.Forms.DataGridViewTextBoxColumn IntensityCol;
    private System.Windows.Forms.DataGridViewTextBoxColumn FreqCol;
  }
}

