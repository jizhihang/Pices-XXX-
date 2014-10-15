namespace PicesCommander
{
  partial class ChartGPS
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
      System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea ();
      System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend ();
      System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series ();
      System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint5 = new System.Windows.Forms.DataVisualization.Charting.DataPoint (-87, 29);
      System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint6 = new System.Windows.Forms.DataVisualization.Charting.DataPoint (-86.8, 28.2);
      System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint7 = new System.Windows.Forms.DataVisualization.Charting.DataPoint (-86.9, 29.1);
      System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint8 = new System.Windows.Forms.DataVisualization.Charting.DataPoint (-86.9, 29.3);
      System.Windows.Forms.DataVisualization.Charting.Title title2 = new System.Windows.Forms.DataVisualization.Charting.Title ();
      this.SelectionCriteriaPanel = new System.Windows.Forms.Panel ();
      this.Deployment = new System.Windows.Forms.TextBox ();
      this.DeploymentLabel = new System.Windows.Forms.Label ();
      this.CruiseLabel = new System.Windows.Forms.Label ();
      this.Station = new System.Windows.Forms.TextBox ();
      this.Cruise = new System.Windows.Forms.TextBox ();
      this.StationLabel = new System.Windows.Forms.Label ();
      this.PlotButton = new System.Windows.Forms.Button ();
      this.CancelPlotButton = new System.Windows.Forms.Button ();
      this.ProfileChart = new System.Windows.Forms.DataVisualization.Charting.Chart ();
      this.TimeInterval = new System.Windows.Forms.NumericUpDown ();
      this.timer1 = new System.Windows.Forms.Timer (this.components);
      this.TimeIntervalLabel = new System.Windows.Forms.Label ();
      this.Status = new System.Windows.Forms.TextBox ();
      this.DeploymentsToPlot = new System.Windows.Forms.CheckedListBox ();
      this.CurGPSLocation = new System.Windows.Forms.TextBox ();
      this.CurrentGPSLocationLabel = new System.Windows.Forms.Label ();
      this.ClearAllButton = new System.Windows.Forms.Button ();
      this.SelectAllButton = new System.Windows.Forms.Button ();
      this.SelectionCriteriaPanel.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.ProfileChart)).BeginInit ();
      ((System.ComponentModel.ISupportInitialize)(this.TimeInterval)).BeginInit ();
      this.SuspendLayout ();
      // 
      // SelectionCriteriaPanel
      // 
      this.SelectionCriteriaPanel.BackColor = System.Drawing.Color.FromArgb (((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
      this.SelectionCriteriaPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.SelectionCriteriaPanel.Controls.Add (this.Deployment);
      this.SelectionCriteriaPanel.Controls.Add (this.DeploymentLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.CruiseLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.Station);
      this.SelectionCriteriaPanel.Controls.Add (this.Cruise);
      this.SelectionCriteriaPanel.Controls.Add (this.StationLabel);
      this.SelectionCriteriaPanel.Location = new System.Drawing.Point (12, 12);
      this.SelectionCriteriaPanel.Name = "SelectionCriteriaPanel";
      this.SelectionCriteriaPanel.Size = new System.Drawing.Size (345, 72);
      this.SelectionCriteriaPanel.TabIndex = 24;
      // 
      // Deployment
      // 
      this.Deployment.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Deployment.Location = new System.Drawing.Point (270, 30);
      this.Deployment.Name = "Deployment";
      this.Deployment.ReadOnly = true;
      this.Deployment.Size = new System.Drawing.Size (54, 23);
      this.Deployment.TabIndex = 8;
      this.Deployment.TabStop = false;
      // 
      // DeploymentLabel
      // 
      this.DeploymentLabel.AutoSize = true;
      this.DeploymentLabel.Location = new System.Drawing.Point (267, 14);
      this.DeploymentLabel.Name = "DeploymentLabel";
      this.DeploymentLabel.Size = new System.Drawing.Size (63, 13);
      this.DeploymentLabel.TabIndex = 7;
      this.DeploymentLabel.Text = "Deployment";
      // 
      // CruiseLabel
      // 
      this.CruiseLabel.AutoSize = true;
      this.CruiseLabel.Location = new System.Drawing.Point (8, 14);
      this.CruiseLabel.Name = "CruiseLabel";
      this.CruiseLabel.Size = new System.Drawing.Size (36, 13);
      this.CruiseLabel.TabIndex = 3;
      this.CruiseLabel.Text = "Cruise";
      // 
      // Station
      // 
      this.Station.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Station.Location = new System.Drawing.Point (161, 30);
      this.Station.Name = "Station";
      this.Station.ReadOnly = true;
      this.Station.Size = new System.Drawing.Size (103, 23);
      this.Station.TabIndex = 6;
      this.Station.TabStop = false;
      // 
      // Cruise
      // 
      this.Cruise.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Cruise.Location = new System.Drawing.Point (5, 30);
      this.Cruise.Name = "Cruise";
      this.Cruise.ReadOnly = true;
      this.Cruise.Size = new System.Drawing.Size (150, 23);
      this.Cruise.TabIndex = 4;
      this.Cruise.TabStop = false;
      // 
      // StationLabel
      // 
      this.StationLabel.AutoSize = true;
      this.StationLabel.Location = new System.Drawing.Point (164, 14);
      this.StationLabel.Name = "StationLabel";
      this.StationLabel.Size = new System.Drawing.Size (40, 13);
      this.StationLabel.TabIndex = 5;
      this.StationLabel.Text = "Station";
      // 
      // PlotButton
      // 
      this.PlotButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.PlotButton.Location = new System.Drawing.Point (12, 456);
      this.PlotButton.Name = "PlotButton";
      this.PlotButton.Size = new System.Drawing.Size (75, 23);
      this.PlotButton.TabIndex = 28;
      this.PlotButton.Text = "Plot";
      this.PlotButton.UseVisualStyleBackColor = true;
      this.PlotButton.Click += new System.EventHandler (this.PlotButton_Click);
      // 
      // CancelPlotButton
      // 
      this.CancelPlotButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CancelPlotButton.Location = new System.Drawing.Point (282, 456);
      this.CancelPlotButton.Name = "CancelPlotButton";
      this.CancelPlotButton.Size = new System.Drawing.Size (75, 23);
      this.CancelPlotButton.TabIndex = 29;
      this.CancelPlotButton.Text = "Cancel";
      this.CancelPlotButton.UseVisualStyleBackColor = true;
      this.CancelPlotButton.Click += new System.EventHandler (this.CancelPlotButton_Click);
      // 
      // ProfileChart
      // 
      this.ProfileChart.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
      this.ProfileChart.CausesValidation = false;
      chartArea2.AlignmentOrientation = ((System.Windows.Forms.DataVisualization.Charting.AreaAlignmentOrientations)((System.Windows.Forms.DataVisualization.Charting.AreaAlignmentOrientations.Vertical | System.Windows.Forms.DataVisualization.Charting.AreaAlignmentOrientations.Horizontal)));
      chartArea2.AlignWithChartArea = "ChartArea1";
      chartArea2.AxisX.LabelAutoFitMaxFontSize = 12;
      chartArea2.AxisX.LabelStyle.Format = "##0.000";
      chartArea2.AxisX.MajorGrid.Enabled = false;
      chartArea2.AxisX.Title = "Longitude";
      chartArea2.AxisX2.LabelStyle.Format = "##0.00";
      chartArea2.AxisX2.MajorGrid.Enabled = false;
      chartArea2.AxisY.IsStartedFromZero = false;
      chartArea2.AxisY.LabelAutoFitMaxFontSize = 12;
      chartArea2.AxisY.LabelStyle.Format = "#0.000";
      chartArea2.AxisY.MajorGrid.Enabled = false;
      chartArea2.AxisY.MajorTickMark.Interval = 0;
      chartArea2.AxisY.MajorTickMark.IntervalOffset = 0;
      chartArea2.AxisY.MajorTickMark.IntervalOffsetType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Number;
      chartArea2.AxisY.MajorTickMark.IntervalType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Number;
      chartArea2.AxisY.Title = "Latitude";
      chartArea2.AxisY2.MajorGrid.Enabled = false;
      chartArea2.BorderDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
      chartArea2.Name = "ChartArea1";
      this.ProfileChart.ChartAreas.Add (chartArea2);
      legend2.Alignment = System.Drawing.StringAlignment.Far;
      legend2.DockedToChartArea = "ChartArea1";
      legend2.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom;
      legend2.IsDockedInsideChartArea = false;
      legend2.Name = "Legend1";
      this.ProfileChart.Legends.Add (legend2);
      this.ProfileChart.Location = new System.Drawing.Point (380, 12);
      this.ProfileChart.Name = "ProfileChart";
      series2.BorderWidth = 2;
      series2.ChartArea = "ChartArea1";
      series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Point;
      series2.Legend = "Legend1";
      series2.LegendText = "A (2/2/13 12:32)";
      series2.Name = "Deployment(XXX)";
      dataPoint5.Label = "12:32";
      dataPoint5.LegendText = "";
      series2.Points.Add (dataPoint5);
      series2.Points.Add (dataPoint6);
      series2.Points.Add (dataPoint7);
      series2.Points.Add (dataPoint8);
      this.ProfileChart.Series.Add (series2);
      this.ProfileChart.Size = new System.Drawing.Size (575, 467);
      this.ProfileChart.TabIndex = 30;
      this.ProfileChart.Text = "GPS Plot";
      title2.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      title2.Name = "GPS";
      title2.Text = "GPS";
      this.ProfileChart.Titles.Add (title2);
      this.ProfileChart.MouseClick += new System.Windows.Forms.MouseEventHandler (this.ProfileChart_MouseClick);
      // 
      // TimeInterval
      // 
      this.TimeInterval.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TimeInterval.Location = new System.Drawing.Point (15, 377);
      this.TimeInterval.Minimum = new decimal (new int[] {
            5,
            0,
            0,
            65536});
      this.TimeInterval.Name = "TimeInterval";
      this.TimeInterval.Size = new System.Drawing.Size (63, 23);
      this.TimeInterval.TabIndex = 31;
      this.TimeInterval.Value = new decimal (new int[] {
            10,
            0,
            0,
            65536});
      // 
      // timer1
      // 
      this.timer1.Interval = 50;
      this.timer1.Tick += new System.EventHandler (this.timer1_Tick);
      // 
      // TimeIntervalLabel
      // 
      this.TimeIntervalLabel.AutoSize = true;
      this.TimeIntervalLabel.Location = new System.Drawing.Point (12, 361);
      this.TimeIntervalLabel.Name = "TimeIntervalLabel";
      this.TimeIntervalLabel.Size = new System.Drawing.Size (68, 13);
      this.TimeIntervalLabel.TabIndex = 32;
      this.TimeIntervalLabel.Text = "Time Interval";
      // 
      // Status
      // 
      this.Status.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Status.Location = new System.Drawing.Point (12, 414);
      this.Status.Name = "Status";
      this.Status.ReadOnly = true;
      this.Status.Size = new System.Drawing.Size (345, 23);
      this.Status.TabIndex = 35;
      this.Status.TabStop = false;
      // 
      // DeploymentsToPlot
      // 
      this.DeploymentsToPlot.CheckOnClick = true;
      this.DeploymentsToPlot.Font = new System.Drawing.Font ("Courier New", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DeploymentsToPlot.FormattingEnabled = true;
      this.DeploymentsToPlot.Location = new System.Drawing.Point (12, 125);
      this.DeploymentsToPlot.Name = "DeploymentsToPlot";
      this.DeploymentsToPlot.Size = new System.Drawing.Size (345, 232);
      this.DeploymentsToPlot.TabIndex = 46;
      this.DeploymentsToPlot.Format += new System.Windows.Forms.ListControlConvertEventHandler (this.DeploymentsToPlot_Format);
      // 
      // CurGPSLocation
      // 
      this.CurGPSLocation.Location = new System.Drawing.Point (191, 377);
      this.CurGPSLocation.Name = "CurGPSLocation";
      this.CurGPSLocation.Size = new System.Drawing.Size (166, 20);
      this.CurGPSLocation.TabIndex = 48;
      this.CurGPSLocation.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // CurrentGPSLocationLabel
      // 
      this.CurrentGPSLocationLabel.AutoSize = true;
      this.CurrentGPSLocationLabel.Location = new System.Drawing.Point (190, 361);
      this.CurrentGPSLocationLabel.Name = "CurrentGPSLocationLabel";
      this.CurrentGPSLocationLabel.Size = new System.Drawing.Size (73, 13);
      this.CurrentGPSLocationLabel.TabIndex = 49;
      this.CurrentGPSLocationLabel.Text = "GPS Location";
      // 
      // ClearAllButton
      // 
      this.ClearAllButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ClearAllButton.Location = new System.Drawing.Point (12, 93);
      this.ClearAllButton.Name = "ClearAllButton";
      this.ClearAllButton.Size = new System.Drawing.Size (75, 22);
      this.ClearAllButton.TabIndex = 50;
      this.ClearAllButton.Text = "Clear All";
      this.ClearAllButton.UseVisualStyleBackColor = true;
      this.ClearAllButton.Click += new System.EventHandler (this.ClearAllButton_Click);
      // 
      // SelectAllButton
      // 
      this.SelectAllButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SelectAllButton.Location = new System.Drawing.Point (93, 93);
      this.SelectAllButton.Name = "SelectAllButton";
      this.SelectAllButton.Size = new System.Drawing.Size (75, 22);
      this.SelectAllButton.TabIndex = 51;
      this.SelectAllButton.Text = "Select All";
      this.SelectAllButton.UseVisualStyleBackColor = true;
      this.SelectAllButton.Click += new System.EventHandler (this.SelectAllButton_Click);
      // 
      // ChartGPS
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (967, 498);
      this.Controls.Add (this.SelectAllButton);
      this.Controls.Add (this.ClearAllButton);
      this.Controls.Add (this.CurrentGPSLocationLabel);
      this.Controls.Add (this.CurGPSLocation);
      this.Controls.Add (this.DeploymentsToPlot);
      this.Controls.Add (this.Status);
      this.Controls.Add (this.TimeIntervalLabel);
      this.Controls.Add (this.TimeInterval);
      this.Controls.Add (this.ProfileChart);
      this.Controls.Add (this.CancelPlotButton);
      this.Controls.Add (this.PlotButton);
      this.Controls.Add (this.SelectionCriteriaPanel);
      this.Name = "ChartGPS";
      this.Text = "Chart GPS";
      this.Load += new System.EventHandler (this.ChartGPS_Load);
      this.SizeChanged += new System.EventHandler (this.ChartGPS_SizeChanged);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.ChartGPS_FormClosing);
      this.Resize += new System.EventHandler (this.ChartGPS_Resize);
      this.SelectionCriteriaPanel.ResumeLayout (false);
      this.SelectionCriteriaPanel.PerformLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.ProfileChart)).EndInit ();
      ((System.ComponentModel.ISupportInitialize)(this.TimeInterval)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Panel SelectionCriteriaPanel;
    private System.Windows.Forms.TextBox Deployment;
    private System.Windows.Forms.Label DeploymentLabel;
    private System.Windows.Forms.Label CruiseLabel;
    private System.Windows.Forms.TextBox Station;
    private System.Windows.Forms.TextBox Cruise;
    private System.Windows.Forms.Label StationLabel;
    private System.Windows.Forms.Button PlotButton;
    private System.Windows.Forms.Button CancelPlotButton;
    private System.Windows.Forms.DataVisualization.Charting.Chart ProfileChart;
    private System.Windows.Forms.NumericUpDown TimeInterval;
    private System.Windows.Forms.Timer timer1;
    private System.Windows.Forms.Label TimeIntervalLabel;
    private System.Windows.Forms.TextBox Status;
    private System.Windows.Forms.CheckedListBox DeploymentsToPlot;
    private System.Windows.Forms.TextBox CurGPSLocation;
    private System.Windows.Forms.Label CurrentGPSLocationLabel;
    private System.Windows.Forms.Button ClearAllButton;
    private System.Windows.Forms.Button SelectAllButton;


  }
}