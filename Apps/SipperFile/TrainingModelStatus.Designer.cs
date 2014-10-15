namespace SipperFile
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
      this.components = new System.ComponentModel.Container ();
      this.CancelLoadButton = new System.Windows.Forms.Button ();
      this.TrainingModelOutput = new System.Windows.Forms.TextBox ();
      this.ModelName = new System.Windows.Forms.TextBox ();
      this.timer1 = new System.Windows.Forms.Timer (this.components);
      this.SuspendLayout ();
      // 
      // CancelLoadButton
      // 
      this.CancelLoadButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CancelLoadButton.Location = new System.Drawing.Point (321, 444);
      this.CancelLoadButton.Name = "CancelLoadButton";
      this.CancelLoadButton.Size = new System.Drawing.Size (164, 43);
      this.CancelLoadButton.TabIndex = 10;
      this.CancelLoadButton.Text = "Cancel";
      this.CancelLoadButton.UseVisualStyleBackColor = true;
      this.CancelLoadButton.Click += new System.EventHandler (this.CancelLoadButton_Click);
      // 
      // TrainingModelOutput
      // 
      this.TrainingModelOutput.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.TrainingModelOutput.Location = new System.Drawing.Point (10, 56);
      this.TrainingModelOutput.Multiline = true;
      this.TrainingModelOutput.Name = "TrainingModelOutput";
      this.TrainingModelOutput.ReadOnly = true;
      this.TrainingModelOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
      this.TrainingModelOutput.Size = new System.Drawing.Size (786, 378);
      this.TrainingModelOutput.TabIndex = 11;
      // 
      // ModelName
      // 
      this.ModelName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.ModelName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ModelName.Location = new System.Drawing.Point (232, 13);
      this.ModelName.Name = "ModelName";
      this.ModelName.ReadOnly = true;
      this.ModelName.Size = new System.Drawing.Size (326, 31);
      this.ModelName.TabIndex = 9;
      this.ModelName.Text = "Model Name";
      this.ModelName.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // timer1
      // 
      this.timer1.Tick += new System.EventHandler (this.timer1_Tick);
      // 
      // TrainingModelStatus
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (810, 496);
      this.Controls.Add (this.CancelLoadButton);
      this.Controls.Add (this.TrainingModelOutput);
      this.Controls.Add (this.ModelName);
      this.MaximumSize = new System.Drawing.Size (826, 534);
      this.MinimumSize = new System.Drawing.Size (826, 534);
      this.Name = "TrainingModelStatus";
      this.Text = "TrainingModelStatus";
      this.Load += new System.EventHandler (this.TrainingModelStatus_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.TrainingModelStatus_FormClosing);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Button CancelLoadButton;
    private System.Windows.Forms.TextBox TrainingModelOutput;
    private System.Windows.Forms.TextBox ModelName;
    private System.Windows.Forms.Timer timer1;
  }
}