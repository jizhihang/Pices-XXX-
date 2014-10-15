namespace SipperFileViewer
{
  partial class AutoDetectionParamters
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
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent ()
    {
      this.MinimumSizeLabel = new System.Windows.Forms.Label();
      this.MinBlobSize = new System.Windows.Forms.MaskedTextBox();
      this.TrainingLibrary1Label = new System.Windows.Forms.Label();
      this.TrainingLibrary1 = new System.Windows.Forms.ComboBox();
      this.UpdateButton = new System.Windows.Forms.Button();
      this.TrainingLibrary2 = new System.Windows.Forms.ComboBox();
      this.TrainingLibrary2Label = new System.Windows.Forms.Label();
      this.rebuildTrainingLibrary2 = new System.Windows.Forms.CheckBox();
      this.rebuildTrainingLibrary1 = new System.Windows.Forms.CheckBox();
      this.SuspendLayout();
      // 
      // MinimumSizeLabel
      // 
      this.MinimumSizeLabel.AutoSize = true;
      this.MinimumSizeLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MinimumSizeLabel.Location = new System.Drawing.Point(12, 46);
      this.MinimumSizeLabel.Name = "MinimumSizeLabel";
      this.MinimumSizeLabel.Size = new System.Drawing.Size(90, 16);
      this.MinimumSizeLabel.TabIndex = 0;
      this.MinimumSizeLabel.Text = "Minimum Size";
      // 
      // MinBlobSize
      // 
      this.MinBlobSize.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MinBlobSize.Location = new System.Drawing.Point(118, 44);
      this.MinBlobSize.Mask = "9990";
      this.MinBlobSize.Name = "MinBlobSize";
      this.MinBlobSize.Size = new System.Drawing.Size(42, 22);
      this.MinBlobSize.TabIndex = 1;
      // 
      // TrainingLibrary1Label
      // 
      this.TrainingLibrary1Label.AutoSize = true;
      this.TrainingLibrary1Label.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TrainingLibrary1Label.Location = new System.Drawing.Point(12, 81);
      this.TrainingLibrary1Label.Name = "TrainingLibrary1Label";
      this.TrainingLibrary1Label.Size = new System.Drawing.Size(101, 16);
      this.TrainingLibrary1Label.TabIndex = 2;
      this.TrainingLibrary1Label.Text = "Training Library";
      // 
      // TrainingLibrary1
      // 
      this.TrainingLibrary1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TrainingLibrary1.FormattingEnabled = true;
      this.TrainingLibrary1.Location = new System.Drawing.Point(118, 78);
      this.TrainingLibrary1.Name = "TrainingLibrary1";
      this.TrainingLibrary1.Size = new System.Drawing.Size(175, 24);
      this.TrainingLibrary1.TabIndex = 2;
      this.TrainingLibrary1.SelectedIndexChanged += new System.EventHandler(this.TrainingLibrary1_SelectedIndexChanged);
      // 
      // UpdateButton
      // 
      this.UpdateButton.Location = new System.Drawing.Point(15, 174);
      this.UpdateButton.Name = "UpdateButton";
      this.UpdateButton.Size = new System.Drawing.Size(75, 23);
      this.UpdateButton.TabIndex = 4;
      this.UpdateButton.Text = "Accept";
      this.UpdateButton.UseVisualStyleBackColor = true;
      this.UpdateButton.Click += new System.EventHandler(this.UpdateButton_Click);
      // 
      // TrainingLibrary2
      // 
      this.TrainingLibrary2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TrainingLibrary2.FormattingEnabled = true;
      this.TrainingLibrary2.Location = new System.Drawing.Point(118, 121);
      this.TrainingLibrary2.Name = "TrainingLibrary2";
      this.TrainingLibrary2.Size = new System.Drawing.Size(175, 24);
      this.TrainingLibrary2.TabIndex = 3;
      this.TrainingLibrary2.SelectedIndexChanged += new System.EventHandler(this.TrainingLibrary2_SelectedIndexChanged);
      // 
      // TrainingLibrary2Label
      // 
      this.TrainingLibrary2Label.AutoSize = true;
      this.TrainingLibrary2Label.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TrainingLibrary2Label.Location = new System.Drawing.Point(12, 124);
      this.TrainingLibrary2Label.Name = "TrainingLibrary2Label";
      this.TrainingLibrary2Label.Size = new System.Drawing.Size(101, 16);
      this.TrainingLibrary2Label.TabIndex = 5;
      this.TrainingLibrary2Label.Text = "Training Library";
      // 
      // rebuildTrainingLibrary2
      // 
      this.rebuildTrainingLibrary2.AutoSize = true;
      this.rebuildTrainingLibrary2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.rebuildTrainingLibrary2.Location = new System.Drawing.Point(301, 123);
      this.rebuildTrainingLibrary2.Name = "rebuildTrainingLibrary2";
      this.rebuildTrainingLibrary2.Size = new System.Drawing.Size(74, 20);
      this.rebuildTrainingLibrary2.TabIndex = 8;
      this.rebuildTrainingLibrary2.Text = "Rebuild";
      this.rebuildTrainingLibrary2.UseVisualStyleBackColor = true;
      // 
      // rebuildTrainingLibrary1
      // 
      this.rebuildTrainingLibrary1.AutoSize = true;
      this.rebuildTrainingLibrary1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.rebuildTrainingLibrary1.Location = new System.Drawing.Point(301, 80);
      this.rebuildTrainingLibrary1.Name = "rebuildTrainingLibrary1";
      this.rebuildTrainingLibrary1.Size = new System.Drawing.Size(74, 20);
      this.rebuildTrainingLibrary1.TabIndex = 9;
      this.rebuildTrainingLibrary1.Text = "Rebuild";
      this.rebuildTrainingLibrary1.UseVisualStyleBackColor = true;
      // 
      // AutoDetectionParamters
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(387, 227);
      this.Controls.Add(this.rebuildTrainingLibrary1);
      this.Controls.Add(this.rebuildTrainingLibrary2);
      this.Controls.Add(this.TrainingLibrary2);
      this.Controls.Add(this.TrainingLibrary2Label);
      this.Controls.Add(this.UpdateButton);
      this.Controls.Add(this.TrainingLibrary1);
      this.Controls.Add(this.TrainingLibrary1Label);
      this.Controls.Add(this.MinBlobSize);
      this.Controls.Add(this.MinimumSizeLabel);
      this.Name = "AutoDetectionParamters";
      this.Text = "AutoDetectionParamters";
      this.Load += new System.EventHandler(this.AutoDetectionParamters_Load);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label MinimumSizeLabel;
    private System.Windows.Forms.MaskedTextBox MinBlobSize;
    private System.Windows.Forms.Label TrainingLibrary1Label;
    private System.Windows.Forms.ComboBox TrainingLibrary1;
    private System.Windows.Forms.Button UpdateButton;
    private System.Windows.Forms.ComboBox TrainingLibrary2;
    private System.Windows.Forms.Label TrainingLibrary2Label;
    private System.Windows.Forms.CheckBox rebuildTrainingLibrary2;
    private System.Windows.Forms.CheckBox rebuildTrainingLibrary1;
  }
}