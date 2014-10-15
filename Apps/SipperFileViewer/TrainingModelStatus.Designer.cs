namespace SipperFileViewer
{
  partial class TrainingModelStatus
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
      this.modelName = new System.Windows.Forms.TextBox();
      this.TrainingModelOutput = new System.Windows.Forms.TextBox();
      this.CancelLoadButton = new System.Windows.Forms.Button();
      this.timer1 = new System.Windows.Forms.Timer(this.components);
      this.SuspendLayout();
      // 
      // modelName
      // 
      this.modelName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.modelName.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.modelName.Location = new System.Drawing.Point(243, 12);
      this.modelName.Name = "modelName";
      this.modelName.ReadOnly = true;
      this.modelName.Size = new System.Drawing.Size(326, 31);
      this.modelName.TabIndex = 0;
      this.modelName.Text = "Model Name";
      this.modelName.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // TrainingModelOutput
      // 
      this.TrainingModelOutput.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.TrainingModelOutput.Location = new System.Drawing.Point(21, 55);
      this.TrainingModelOutput.Multiline = true;
      this.TrainingModelOutput.Name = "TrainingModelOutput";
      this.TrainingModelOutput.ReadOnly = true;
      this.TrainingModelOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
      this.TrainingModelOutput.Size = new System.Drawing.Size(786, 378);
      this.TrainingModelOutput.TabIndex = 8;
      // 
      // CancelLoadButton
      // 
      this.CancelLoadButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CancelLoadButton.Location = new System.Drawing.Point(324, 443);
      this.CancelLoadButton.Name = "CancelLoadButton";
      this.CancelLoadButton.Size = new System.Drawing.Size(164, 43);
      this.CancelLoadButton.TabIndex = 2;
      this.CancelLoadButton.Text = "Cancel";
      this.CancelLoadButton.UseVisualStyleBackColor = true;
      this.CancelLoadButton.Click += new System.EventHandler(this.CancelButton_Click);
      // 
      // timer1
      // 
      this.timer1.Enabled = true;
      this.timer1.Interval = 200;
      this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
      // 
      // TrainingModelStatus
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(819, 494);
      this.Controls.Add(this.CancelLoadButton);
      this.Controls.Add(this.TrainingModelOutput);
      this.Controls.Add(this.modelName);
      this.Name = "TrainingModelStatus";
      this.Text = "Training Model Status";
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.TextBox modelName;
    private System.Windows.Forms.TextBox TrainingModelOutput;
    private System.Windows.Forms.Button  CancelLoadButton;
    private System.Windows.Forms.Timer   timer1;
  }
}