namespace SipperFile
{
  partial class TimePlot
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
      this.components = new System.ComponentModel.Container ();
      this.chartPanel = new System.Windows.Forms.Panel ();
      this.TimeDomainScrollBar = new System.Windows.Forms.HScrollBar ();
      this.updateChartTimer = new System.Windows.Forms.Timer (this.components);
      this.LiveOrHistory = new System.Windows.Forms.Button ();
      this.MinLabel = new System.Windows.Forms.Label ();
      this.MinLabel2 = new System.Windows.Forms.Label ();
      this.MaximumLabel = new System.Windows.Forms.Label ();
      this.MaximumLabel2 = new System.Windows.Forms.Label ();
      this.FindMimMaxButton = new System.Windows.Forms.Button ();
      this.timeRange = new ChartControls.RadioButtonList ();
      this.dataSeries5 = new ChartControls.DataSeries ();
      this.dataSeries4 = new ChartControls.DataSeries ();
      this.dataSeries3 = new ChartControls.DataSeries ();
      this.dataSeries2 = new ChartControls.DataSeries ();
      this.dataSeries1 = new ChartControls.DataSeries ();
      this.dataSeries0 = new ChartControls.DataSeries ();
      this.SuspendLayout ();
      // 
      // chartPanel
      // 
      this.chartPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.chartPanel.Location = new System.Drawing.Point (34, 158);
      this.chartPanel.Name = "chartPanel";
      this.chartPanel.Size = new System.Drawing.Size (1256, 522);
      this.chartPanel.TabIndex = 0;
      this.chartPanel.Paint += new System.Windows.Forms.PaintEventHandler (this.chartPanel_Paint);
      this.chartPanel.MouseClick += new System.Windows.Forms.MouseEventHandler (this.chartPanel_MouseClick);
      // 
      // TimeDomainScrollBar
      // 
      this.TimeDomainScrollBar.Location = new System.Drawing.Point (34, 681);
      this.TimeDomainScrollBar.Name = "TimeDomainScrollBar";
      this.TimeDomainScrollBar.Size = new System.Drawing.Size (1256, 21);
      this.TimeDomainScrollBar.TabIndex = 7;
      this.TimeDomainScrollBar.ValueChanged += new System.EventHandler (this.TimeDomainScrollBar_ValueChanged);
      // 
      // updateChartTimer
      // 
      this.updateChartTimer.Tick += new System.EventHandler (this.updateChartTimer_Tick);
      // 
      // LiveOrHistory
      // 
      this.LiveOrHistory.FlatAppearance.BorderSize = 2;
      this.LiveOrHistory.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.LiveOrHistory.Location = new System.Drawing.Point (742, 712);
      this.LiveOrHistory.Name = "LiveOrHistory";
      this.LiveOrHistory.Size = new System.Drawing.Size (87, 45);
      this.LiveOrHistory.TabIndex = 9;
      this.LiveOrHistory.Text = "Live";
      this.LiveOrHistory.UseVisualStyleBackColor = true;
      this.LiveOrHistory.Click += new System.EventHandler (this.LiveOrHistory_Click);
      // 
      // MinLabel
      // 
      this.MinLabel.AutoSize = true;
      this.MinLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MinLabel.Location = new System.Drawing.Point (237, 5);
      this.MinLabel.Name = "MinLabel";
      this.MinLabel.Size = new System.Drawing.Size (71, 18);
      this.MinLabel.TabIndex = 10;
      this.MinLabel.Text = "Minimum";
      this.MinLabel.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // MinLabel2
      // 
      this.MinLabel2.AutoSize = true;
      this.MinLabel2.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MinLabel2.Location = new System.Drawing.Point (897, 5);
      this.MinLabel2.Name = "MinLabel2";
      this.MinLabel2.Size = new System.Drawing.Size (71, 18);
      this.MinLabel2.TabIndex = 11;
      this.MinLabel2.Text = "Minimum";
      this.MinLabel2.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // MaximumLabel
      // 
      this.MaximumLabel.AutoSize = true;
      this.MaximumLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MaximumLabel.Location = new System.Drawing.Point (343, 5);
      this.MaximumLabel.Name = "MaximumLabel";
      this.MaximumLabel.Size = new System.Drawing.Size (75, 18);
      this.MaximumLabel.TabIndex = 11;
      this.MaximumLabel.Text = "Maximum";
      this.MaximumLabel.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // MaximumLabel2
      // 
      this.MaximumLabel2.AutoSize = true;
      this.MaximumLabel2.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MaximumLabel2.Location = new System.Drawing.Point (1002, 5);
      this.MaximumLabel2.Name = "MaximumLabel2";
      this.MaximumLabel2.Size = new System.Drawing.Size (75, 18);
      this.MaximumLabel2.TabIndex = 12;
      this.MaximumLabel2.Text = "Maximum";
      this.MaximumLabel2.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // FindMimMaxButton
      // 
      this.FindMimMaxButton.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FindMimMaxButton.Location = new System.Drawing.Point (635, 34);
      this.FindMimMaxButton.Name = "FindMimMaxButton";
      this.FindMimMaxButton.Size = new System.Drawing.Size (58, 77);
      this.FindMimMaxButton.TabIndex = 13;
      this.FindMimMaxButton.Text = "Find Min Max";
      this.FindMimMaxButton.UseVisualStyleBackColor = true;
      this.FindMimMaxButton.Click += new System.EventHandler (this.FindMimMaxButton_Click);
      // 
      // timeRange
      // 
      this.timeRange.Location = new System.Drawing.Point (34, 706);
      this.timeRange.Margin = new System.Windows.Forms.Padding (4);
      this.timeRange.Name = "timeRange";
      this.timeRange.NumColumns = 8;
      this.timeRange.Options = new string[] {
        "5 Mins",
        "10 Mins",
        "15 Mins",
        "20 Mins",
        "30 Mins",
        "45 Mins",
        "1 Hour",
        "All"};
      this.timeRange.OptionsFont = new System.Drawing.Font ("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.timeRange.SelectedOption = "15 Mins";
      this.timeRange.SelectedOptionInt = 2;
      this.timeRange.Size = new System.Drawing.Size (674, 53);
      this.timeRange.TabIndex = 8;
      this.timeRange.Title = "Time Range";
      this.timeRange.SelectedOptionChanged += new ChartControls.RadioButtonList.SelectedOptionChangedHandler (this.TimeRange_SelectedOptionChanged);
      // 
      // dataSeries5
      // 
      this.dataSeries5.BackColor = System.Drawing.Color.Magenta;
      this.dataSeries5.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.dataSeries5.Labels = new string[] {
        "Depth",
        "Oxygen",
        "Pitch",
        "Roll",
        "Temperature"};
      this.dataSeries5.Location = new System.Drawing.Point (697, 115);
      this.dataSeries5.Margin = new System.Windows.Forms.Padding (4);
      this.dataSeries5.Name = "dataSeries5";
      this.dataSeries5.Plot = true;
      this.dataSeries5.PlotColor = System.Drawing.Color.Magenta;
      this.dataSeries5.SelectedIndex = 3;
      this.dataSeries5.SelectedLabel = "Roll";
      this.dataSeries5.Size = new System.Drawing.Size (593, 35);
      this.dataSeries5.TabIndex = 6;
      this.dataSeries5.RangeChanged += new ChartControls.DataSeries.RangeChangedHandler (this.dataSeries5_RangeChanged);
      this.dataSeries5.PlotColorChanged += new ChartControls.DataSeries.PlotColorChangedHandler (this.dataSeries5_PlotColorChanged);
      this.dataSeries5.SelectedLabelChanged += new ChartControls.DataSeries.SelectedLabelChangedHandler (this.dataSeries5_SelectedLabelChanged);
      this.dataSeries5.PlotChanged += new ChartControls.DataSeries.PlotChangedHandler (this.dataSeries5_PlotChanged);
      // 
      // dataSeries4
      // 
      this.dataSeries4.BackColor = System.Drawing.Color.White;
      this.dataSeries4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.dataSeries4.Labels = new string[] {
        "Depth",
        "Oxygen",
        "Pitch",
        "Roll",
        "Temperature"};
      this.dataSeries4.Location = new System.Drawing.Point (697, 75);
      this.dataSeries4.Margin = new System.Windows.Forms.Padding (4);
      this.dataSeries4.Name = "dataSeries4";
      this.dataSeries4.Plot = true;
      this.dataSeries4.PlotColor = System.Drawing.Color.White;
      this.dataSeries4.SelectedIndex = 2;
      this.dataSeries4.SelectedLabel = "Pitch";
      this.dataSeries4.Size = new System.Drawing.Size (593, 35);
      this.dataSeries4.TabIndex = 5;
      this.dataSeries4.RangeChanged += new ChartControls.DataSeries.RangeChangedHandler (this.dataSeries4_RangeChanged);
      this.dataSeries4.PlotColorChanged += new ChartControls.DataSeries.PlotColorChangedHandler (this.dataSeries4_PlotColorChanged);
      this.dataSeries4.SelectedLabelChanged += new ChartControls.DataSeries.SelectedLabelChangedHandler (this.dataSeries4_SelectedLabelChanged);
      this.dataSeries4.PlotChanged += new ChartControls.DataSeries.PlotChangedHandler (this.dataSeries4_PlotChanged);
      // 
      // dataSeries3
      // 
      this.dataSeries3.BackColor = System.Drawing.Color.Yellow;
      this.dataSeries3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.dataSeries3.Labels = new string[] {
        "Depth",
        "Oxygen",
        "Pitch",
        "Roll",
        "Temperature"};
      this.dataSeries3.Location = new System.Drawing.Point (697, 34);
      this.dataSeries3.Margin = new System.Windows.Forms.Padding (4);
      this.dataSeries3.Name = "dataSeries3";
      this.dataSeries3.Plot = true;
      this.dataSeries3.PlotColor = System.Drawing.Color.Yellow;
      this.dataSeries3.SelectedIndex = 0;
      this.dataSeries3.SelectedLabel = "Depth";
      this.dataSeries3.Size = new System.Drawing.Size (593, 35);
      this.dataSeries3.TabIndex = 4;
      this.dataSeries3.RangeChanged += new ChartControls.DataSeries.RangeChangedHandler (this.dataSeries3_RangeChanged);
      this.dataSeries3.PlotColorChanged += new ChartControls.DataSeries.PlotColorChangedHandler (this.dataSeries3_PlotColorChanged);
      this.dataSeries3.SelectedLabelChanged += new ChartControls.DataSeries.SelectedLabelChangedHandler (this.dataSeries3_SelectedLabelChanged);
      this.dataSeries3.PlotChanged += new ChartControls.DataSeries.PlotChangedHandler (this.dataSeries3_PlotChanged);
      // 
      // dataSeries2
      // 
      this.dataSeries2.BackColor = System.Drawing.Color.Blue;
      this.dataSeries2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.dataSeries2.Labels = new string[] {
        "Depth",
        "Oxygen",
        "Pitch",
        "Roll",
        "Temperature"};
      this.dataSeries2.Location = new System.Drawing.Point (34, 115);
      this.dataSeries2.Margin = new System.Windows.Forms.Padding (4);
      this.dataSeries2.Name = "dataSeries2";
      this.dataSeries2.Plot = true;
      this.dataSeries2.PlotColor = System.Drawing.Color.Blue;
      this.dataSeries2.SelectedIndex = 0;
      this.dataSeries2.SelectedLabel = "Depth";
      this.dataSeries2.Size = new System.Drawing.Size (597, 35);
      this.dataSeries2.TabIndex = 3;
      this.dataSeries2.RangeChanged += new ChartControls.DataSeries.RangeChangedHandler (this.dataSeries2_RangeChanged);
      this.dataSeries2.PlotColorChanged += new ChartControls.DataSeries.PlotColorChangedHandler (this.dataSeries2_PlotColorChanged);
      this.dataSeries2.SelectedLabelChanged += new ChartControls.DataSeries.SelectedLabelChangedHandler (this.dataSeries2_SelectedLabelChanged);
      this.dataSeries2.PlotChanged += new ChartControls.DataSeries.PlotChangedHandler (this.dataSeries2_PlotChanged);
      // 
      // dataSeries1
      // 
      this.dataSeries1.BackColor = System.Drawing.Color.PaleGreen;
      this.dataSeries1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.dataSeries1.Labels = new string[] {
        "Depth",
        "Oxygen",
        "Pitch",
        "Roll",
        "Temperature"};
      this.dataSeries1.Location = new System.Drawing.Point (34, 75);
      this.dataSeries1.Margin = new System.Windows.Forms.Padding (4);
      this.dataSeries1.Name = "dataSeries1";
      this.dataSeries1.Plot = true;
      this.dataSeries1.PlotColor = System.Drawing.Color.PaleGreen;
      this.dataSeries1.SelectedIndex = 0;
      this.dataSeries1.SelectedLabel = "Depth";
      this.dataSeries1.Size = new System.Drawing.Size (597, 35);
      this.dataSeries1.TabIndex = 2;
      this.dataSeries1.RangeChanged += new ChartControls.DataSeries.RangeChangedHandler (this.dataSeries1_RangeChanged);
      this.dataSeries1.PlotColorChanged += new ChartControls.DataSeries.PlotColorChangedHandler (this.dataSeries1_PlotColorChanged);
      this.dataSeries1.SelectedLabelChanged += new ChartControls.DataSeries.SelectedLabelChangedHandler (this.dataSeries1_SelectedLabelChanged);
      this.dataSeries1.PlotChanged += new ChartControls.DataSeries.PlotChangedHandler (this.dataSeries1_PlotChanged);
      // 
      // dataSeries0
      // 
      this.dataSeries0.BackColor = System.Drawing.Color.Red;
      this.dataSeries0.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.dataSeries0.Labels = new string[] {
        "Depth",
        "Oxygen",
        "Pitch",
        "Roll",
        "Temperature"};
      this.dataSeries0.Location = new System.Drawing.Point (34, 34);
      this.dataSeries0.Margin = new System.Windows.Forms.Padding (4);
      this.dataSeries0.Name = "dataSeries0";
      this.dataSeries0.Plot = true;
      this.dataSeries0.PlotColor = System.Drawing.Color.Red;
      this.dataSeries0.SelectedIndex = 0;
      this.dataSeries0.SelectedLabel = "Depth";
      this.dataSeries0.Size = new System.Drawing.Size (597, 35);
      this.dataSeries0.TabIndex = 1;
      this.dataSeries0.RangeChanged += new ChartControls.DataSeries.RangeChangedHandler (this.dataSeries0_RangeChanged);
      this.dataSeries0.PlotColorChanged += new ChartControls.DataSeries.PlotColorChangedHandler (this.dataSeries0_PlotColorChanged);
      this.dataSeries0.SelectedLabelChanged += new ChartControls.DataSeries.SelectedLabelChangedHandler (this.dataSeries0_SelectedLabelChanged);
      this.dataSeries0.PlotChanged += new ChartControls.DataSeries.PlotChangedHandler (this.dataSeries0_PlotChanged);
      // 
      // TimePlot
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 12F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (1320, 769);
      this.Controls.Add (this.FindMimMaxButton);
      this.Controls.Add (this.MaximumLabel2);
      this.Controls.Add (this.MaximumLabel);
      this.Controls.Add (this.MinLabel2);
      this.Controls.Add (this.MinLabel);
      this.Controls.Add (this.LiveOrHistory);
      this.Controls.Add (this.timeRange);
      this.Controls.Add (this.TimeDomainScrollBar);
      this.Controls.Add (this.dataSeries5);
      this.Controls.Add (this.dataSeries4);
      this.Controls.Add (this.dataSeries3);
      this.Controls.Add (this.dataSeries2);
      this.Controls.Add (this.dataSeries1);
      this.Controls.Add (this.dataSeries0);
      this.Controls.Add (this.chartPanel);
      this.Font = new System.Drawing.Font ("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Name = "TimePlot";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "TimePlot";
      this.Load += new System.EventHandler (this.TimePlot_Load);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Panel chartPanel;
    private ChartControls.DataSeries dataSeries0;
    private ChartControls.DataSeries dataSeries1;
    private ChartControls.DataSeries dataSeries2;
    private ChartControls.DataSeries dataSeries3;
    private ChartControls.DataSeries dataSeries4;
    private ChartControls.DataSeries dataSeries5;
    private System.Windows.Forms.HScrollBar TimeDomainScrollBar;
    private System.Windows.Forms.Timer updateChartTimer;
    private ChartControls.RadioButtonList timeRange;
    private System.Windows.Forms.Button LiveOrHistory;
    private System.Windows.Forms.Label MinLabel;
    private System.Windows.Forms.Label MinLabel2;
    private System.Windows.Forms.Label MaximumLabel;
    private System.Windows.Forms.Label MaximumLabel2;
    private System.Windows.Forms.Button FindMimMaxButton;
  }
}