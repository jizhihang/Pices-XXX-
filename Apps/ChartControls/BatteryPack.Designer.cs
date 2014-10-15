namespace ChartControls
{
  partial class BatteryPack
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

    #region Component Designer generated code

    /// <summary> 
    /// Required method for Designer support - do not modify 
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent ()
    {
      this.Title = new System.Windows.Forms.Label ();
      this.autoSelectButton = new System.Windows.Forms.Button ();
      this.battery4 = new ChartControls.Battery ();
      this.battery3 = new ChartControls.Battery ();
      this.battery2 = new ChartControls.Battery ();
      this.battery1 = new ChartControls.Battery ();
      this.SuspendLayout ();
      // 
      // Title
      // 
      this.Title.AutoSize = true;
      this.Title.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Title.Location = new System.Drawing.Point (48, 2);
      this.Title.Margin = new System.Windows.Forms.Padding (4, 0, 4, 0);
      this.Title.Name = "Title";
      this.Title.Size = new System.Drawing.Size (198, 25);
      this.Title.TabIndex = 4;
      this.Title.Text = "SIPPER Battery Pack";
      this.Title.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // autoSelectButton
      // 
      this.autoSelectButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.autoSelectButton.Location = new System.Drawing.Point (64, 135);
      this.autoSelectButton.Margin = new System.Windows.Forms.Padding (4, 4, 4, 4);
      this.autoSelectButton.Name = "autoSelectButton";
      this.autoSelectButton.Size = new System.Drawing.Size (183, 32);
      this.autoSelectButton.TabIndex = 5;
      this.autoSelectButton.Text = "Enable Auto Select";
      this.autoSelectButton.UseVisualStyleBackColor = true;
      this.autoSelectButton.Click += new System.EventHandler (this.autoSelectButton_Click);
      // 
      // battery4
      // 
      this.battery4.Alive = false;
      this.battery4.ColorEmpty = System.Drawing.Color.Gray;
      this.battery4.ColorFull = System.Drawing.Color.Green;
      this.battery4.ColorNearEmpty = System.Drawing.Color.Orange;
      this.battery4.Location = new System.Drawing.Point (244, 30);
      this.battery4.Margin = new System.Windows.Forms.Padding (5, 5, 5, 5);
      this.battery4.MaxVoltage = 12F;
      this.battery4.Name = "battery4";
      this.battery4.Size = new System.Drawing.Size (67, 95);
      this.battery4.TabIndex = 3;
      this.battery4.Voltage = 12F;
      this.battery4.VoltageFont = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.battery4.DoubleClick += new System.EventHandler (this.battery4_DoubleClick);
      // 
      // battery3
      // 
      this.battery3.Alive = false;
      this.battery3.ColorEmpty = System.Drawing.Color.Gray;
      this.battery3.ColorFull = System.Drawing.Color.Green;
      this.battery3.ColorNearEmpty = System.Drawing.Color.Orange;
      this.battery3.Location = new System.Drawing.Point (164, 30);
      this.battery3.Margin = new System.Windows.Forms.Padding (5, 5, 5, 5);
      this.battery3.MaxVoltage = 12F;
      this.battery3.Name = "battery3";
      this.battery3.Size = new System.Drawing.Size (67, 95);
      this.battery3.TabIndex = 2;
      this.battery3.Voltage = 12F;
      this.battery3.VoltageFont = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.battery3.DoubleClick += new System.EventHandler (this.battery3_DoubleClick);
      // 
      // battery2
      // 
      this.battery2.Alive = false;
      this.battery2.BackColor = System.Drawing.SystemColors.Control;
      this.battery2.ColorEmpty = System.Drawing.Color.Gray;
      this.battery2.ColorFull = System.Drawing.Color.Green;
      this.battery2.ColorNearEmpty = System.Drawing.Color.Orange;
      this.battery2.Location = new System.Drawing.Point (84, 30);
      this.battery2.Margin = new System.Windows.Forms.Padding (5, 5, 5, 5);
      this.battery2.MaxVoltage = 12F;
      this.battery2.Name = "battery2";
      this.battery2.Size = new System.Drawing.Size (67, 95);
      this.battery2.TabIndex = 1;
      this.battery2.Voltage = 12F;
      this.battery2.VoltageFont = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.battery2.DoubleClick += new System.EventHandler (this.battery2_DoubleClick);
      // 
      // battery1
      // 
      this.battery1.Alive = false;
      this.battery1.ColorEmpty = System.Drawing.Color.Gray;
      this.battery1.ColorFull = System.Drawing.Color.Green;
      this.battery1.ColorNearEmpty = System.Drawing.Color.Orange;
      this.battery1.Location = new System.Drawing.Point (4, 30);
      this.battery1.Margin = new System.Windows.Forms.Padding (5, 5, 5, 5);
      this.battery1.MaxVoltage = 12F;
      this.battery1.Name = "battery1";
      this.battery1.Size = new System.Drawing.Size (67, 95);
      this.battery1.TabIndex = 0;
      this.battery1.Voltage = 12F;
      this.battery1.VoltageFont = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.battery1.Load += new System.EventHandler (this.battery1_Load);
      this.battery1.DoubleClick += new System.EventHandler (this.battery1_DoubleClick);
      // 
      // BatteryPack
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add (this.autoSelectButton);
      this.Controls.Add (this.Title);
      this.Controls.Add (this.battery4);
      this.Controls.Add (this.battery3);
      this.Controls.Add (this.battery2);
      this.Controls.Add (this.battery1);
      this.Margin = new System.Windows.Forms.Padding (4, 4, 4, 4);
      this.Name = "BatteryPack";
      this.Size = new System.Drawing.Size (319, 174);
      this.Layout += new System.Windows.Forms.LayoutEventHandler (this.BatteryPack_Layout);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private Battery battery1;
    private Battery battery2;
    private Battery battery3;
    private Battery battery4;
    private System.Windows.Forms.Label Title;
    private System.Windows.Forms.Button autoSelectButton;
  }
}
