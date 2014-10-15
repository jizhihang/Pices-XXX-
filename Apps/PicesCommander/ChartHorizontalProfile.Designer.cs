namespace PicesCommander
{
  partial class ChartHorizontalProfile
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
      System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea ();
      System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend ();
      System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series ();
      System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint1 = new System.Windows.Forms.DataVisualization.Charting.DataPoint (4098, 46);
      System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint2 = new System.Windows.Forms.DataVisualization.Charting.DataPoint (9128, 76);
      System.Windows.Forms.DataVisualization.Charting.Title title1 = new System.Windows.Forms.DataVisualization.Charting.Title ();
      System.Windows.Forms.DataVisualization.Charting.Title title2 = new System.Windows.Forms.DataVisualization.Charting.Title ();
      this.ClassesToPlot = new System.Windows.Forms.DataGridView ();
      this.Class = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.BrowseClasses = new System.Windows.Forms.DataGridViewButtonColumn ();
      this.Remove = new System.Windows.Forms.DataGridViewButtonColumn ();
      this.ColorToUse = new System.Windows.Forms.DataGridViewButtonColumn ();
      this.IncludeSubClasses = new System.Windows.Forms.CheckBox ();
      this.AddClassButton = new System.Windows.Forms.Button ();
      this.WeightByVolume = new System.Windows.Forms.CheckBox ();
      this.Inst1ToPlotLabel = new System.Windows.Forms.Label ();
      this.SelectionCriteriaPanel = new System.Windows.Forms.Panel ();
      this.SizeProbDepthCriteria = new System.Windows.Forms.TextBox ();
      this.SizePrametersLabel = new System.Windows.Forms.Label ();
      this.CruiseLabel = new System.Windows.Forms.Label ();
      this.CruiseStationDeployment = new System.Windows.Forms.TextBox ();
      this.Inst1ToPlot = new System.Windows.Forms.ComboBox ();
      this.PlotButton = new System.Windows.Forms.Button ();
      this.CancelPlotButton = new System.Windows.Forms.Button ();
      this.ProfileChart = new System.Windows.Forms.DataVisualization.Charting.Chart ();
      this.TimeIncr = new System.Windows.Forms.NumericUpDown ();
      this.timer1 = new System.Windows.Forms.Timer (this.components);
      this.TimeIncrLabel = new System.Windows.Forms.Label ();
      this.Inst1ColorButton = new System.Windows.Forms.Button ();
      this.Status = new System.Windows.Forms.TextBox ();
      this.RePlotButton = new System.Windows.Forms.Button ();
      this.ClassPlottingCriteriaPanel = new System.Windows.Forms.Panel ();
      ((System.ComponentModel.ISupportInitialize)(this.ClassesToPlot)).BeginInit ();
      this.SelectionCriteriaPanel.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.ProfileChart)).BeginInit ();
      ((System.ComponentModel.ISupportInitialize)(this.TimeIncr)).BeginInit ();
      this.ClassPlottingCriteriaPanel.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // ClassesToPlot
      // 
      this.ClassesToPlot.AllowUserToAddRows = false;
      this.ClassesToPlot.AllowUserToDeleteRows = false;
      this.ClassesToPlot.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.ClassesToPlot.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.Class,
            this.BrowseClasses,
            this.Remove,
            this.ColorToUse});
      this.ClassesToPlot.Location = new System.Drawing.Point (3, 3);
      this.ClassesToPlot.Name = "ClassesToPlot";
      this.ClassesToPlot.RowHeadersVisible = false;
      this.ClassesToPlot.Size = new System.Drawing.Size (469, 158);
      this.ClassesToPlot.TabIndex = 6;
      this.ClassesToPlot.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.ClassesToPlot_CellFormatting);
      this.ClassesToPlot.RowsAdded += new System.Windows.Forms.DataGridViewRowsAddedEventHandler (this.ClassesToPlot_RowsAdded);
      this.ClassesToPlot.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler (this.ClassesToPlot_CellClick);
      // 
      // Class
      // 
      this.Class.HeaderText = "Class";
      this.Class.Name = "Class";
      this.Class.Width = 280;
      // 
      // BrowseClasses
      // 
      this.BrowseClasses.HeaderText = "Browse";
      this.BrowseClasses.Name = "BrowseClasses";
      this.BrowseClasses.Text = "Browse";
      this.BrowseClasses.UseColumnTextForButtonValue = true;
      this.BrowseClasses.Width = 60;
      // 
      // Remove
      // 
      this.Remove.HeaderText = "Remove";
      this.Remove.Name = "Remove";
      this.Remove.Text = "Remove";
      this.Remove.UseColumnTextForButtonValue = true;
      this.Remove.Width = 60;
      // 
      // ColorToUse
      // 
      this.ColorToUse.HeaderText = "Color";
      this.ColorToUse.Name = "ColorToUse";
      this.ColorToUse.Text = "Color";
      this.ColorToUse.Width = 60;
      // 
      // IncludeSubClasses
      // 
      this.IncludeSubClasses.AutoSize = true;
      this.IncludeSubClasses.Checked = true;
      this.IncludeSubClasses.CheckState = System.Windows.Forms.CheckState.Checked;
      this.IncludeSubClasses.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.IncludeSubClasses.Location = new System.Drawing.Point (154, 167);
      this.IncludeSubClasses.Name = "IncludeSubClasses";
      this.IncludeSubClasses.Size = new System.Drawing.Size (155, 21);
      this.IncludeSubClasses.TabIndex = 7;
      this.IncludeSubClasses.Text = "Include Sub-Classes";
      this.IncludeSubClasses.UseVisualStyleBackColor = true;
      // 
      // AddClassButton
      // 
      this.AddClassButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.AddClassButton.Location = new System.Drawing.Point (3, 167);
      this.AddClassButton.Name = "AddClassButton";
      this.AddClassButton.Size = new System.Drawing.Size (75, 27);
      this.AddClassButton.TabIndex = 8;
      this.AddClassButton.Text = "Add Class";
      this.AddClassButton.UseVisualStyleBackColor = true;
      this.AddClassButton.Click += new System.EventHandler (this.AddClassButton_Click);
      // 
      // WeightByVolume
      // 
      this.WeightByVolume.AutoSize = true;
      this.WeightByVolume.Checked = true;
      this.WeightByVolume.CheckState = System.Windows.Forms.CheckState.Checked;
      this.WeightByVolume.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.WeightByVolume.Location = new System.Drawing.Point (331, 167);
      this.WeightByVolume.Name = "WeightByVolume";
      this.WeightByVolume.Size = new System.Drawing.Size (141, 21);
      this.WeightByVolume.TabIndex = 9;
      this.WeightByVolume.Text = "Weight by Volume";
      this.WeightByVolume.UseVisualStyleBackColor = true;
      // 
      // Inst1ToPlotLabel
      // 
      this.Inst1ToPlotLabel.AutoSize = true;
      this.Inst1ToPlotLabel.Location = new System.Drawing.Point (795, 23);
      this.Inst1ToPlotLabel.Name = "Inst1ToPlotLabel";
      this.Inst1ToPlotLabel.Size = new System.Drawing.Size (57, 13);
      this.Inst1ToPlotLabel.TabIndex = 11;
      this.Inst1ToPlotLabel.Text = "Inst to Plot";
      // 
      // SelectionCriteriaPanel
      // 
      this.SelectionCriteriaPanel.BackColor = System.Drawing.Color.FromArgb (((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
      this.SelectionCriteriaPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.SelectionCriteriaPanel.Controls.Add (this.SizeProbDepthCriteria);
      this.SelectionCriteriaPanel.Controls.Add (this.SizePrametersLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.CruiseLabel);
      this.SelectionCriteriaPanel.Controls.Add (this.CruiseStationDeployment);
      this.SelectionCriteriaPanel.Location = new System.Drawing.Point (12, 12);
      this.SelectionCriteriaPanel.Name = "SelectionCriteriaPanel";
      this.SelectionCriteriaPanel.Size = new System.Drawing.Size (250, 130);
      this.SelectionCriteriaPanel.TabIndex = 24;
      // 
      // SizeProbDepthCriteria
      // 
      this.SizeProbDepthCriteria.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SizeProbDepthCriteria.Location = new System.Drawing.Point (12, 86);
      this.SizeProbDepthCriteria.Name = "SizeProbDepthCriteria";
      this.SizeProbDepthCriteria.ReadOnly = true;
      this.SizeProbDepthCriteria.Size = new System.Drawing.Size (223, 23);
      this.SizeProbDepthCriteria.TabIndex = 26;
      this.SizeProbDepthCriteria.TabStop = false;
      // 
      // SizePrametersLabel
      // 
      this.SizePrametersLabel.AutoSize = true;
      this.SizePrametersLabel.Location = new System.Drawing.Point (14, 71);
      this.SizePrametersLabel.Name = "SizePrametersLabel";
      this.SizePrametersLabel.Size = new System.Drawing.Size (123, 13);
      this.SizePrametersLabel.TabIndex = 25;
      this.SizePrametersLabel.Text = "Size/Prob/Depth Criteria";
      // 
      // CruiseLabel
      // 
      this.CruiseLabel.AutoSize = true;
      this.CruiseLabel.Location = new System.Drawing.Point (14, 14);
      this.CruiseLabel.Name = "CruiseLabel";
      this.CruiseLabel.Size = new System.Drawing.Size (135, 13);
      this.CruiseLabel.TabIndex = 3;
      this.CruiseLabel.Text = "Cruise/Station/Deployment";
      // 
      // CruiseStationDeployment
      // 
      this.CruiseStationDeployment.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CruiseStationDeployment.Location = new System.Drawing.Point (12, 29);
      this.CruiseStationDeployment.Name = "CruiseStationDeployment";
      this.CruiseStationDeployment.ReadOnly = true;
      this.CruiseStationDeployment.Size = new System.Drawing.Size (223, 23);
      this.CruiseStationDeployment.TabIndex = 4;
      this.CruiseStationDeployment.TabStop = false;
      // 
      // Inst1ToPlot
      // 
      this.Inst1ToPlot.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Inst1ToPlot.FormattingEnabled = true;
      this.Inst1ToPlot.Location = new System.Drawing.Point (792, 37);
      this.Inst1ToPlot.Name = "Inst1ToPlot";
      this.Inst1ToPlot.Size = new System.Drawing.Size (326, 24);
      this.Inst1ToPlot.TabIndex = 25;
      this.Inst1ToPlot.Enter += new System.EventHandler (this.Inst1ToPlot_Enter);
      // 
      // PlotButton
      // 
      this.PlotButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.PlotButton.Location = new System.Drawing.Point (791, 188);
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
      this.CancelPlotButton.Location = new System.Drawing.Point (1097, 188);
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
      chartArea1.AlignmentOrientation = ((System.Windows.Forms.DataVisualization.Charting.AreaAlignmentOrientations)((System.Windows.Forms.DataVisualization.Charting.AreaAlignmentOrientations.Vertical | System.Windows.Forms.DataVisualization.Charting.AreaAlignmentOrientations.Horizontal)));
      chartArea1.AlignWithChartArea = "ChartArea1";
      chartArea1.AxisX.MajorGrid.Enabled = false;
      chartArea1.AxisX.Minimum = 0;
      chartArea1.AxisX.Title = "Count/m^3";
      chartArea1.AxisX2.MajorGrid.Enabled = false;
      chartArea1.AxisY.LabelAutoFitMaxFontSize = 12;
      chartArea1.AxisY.MajorGrid.Enabled = false;
      chartArea1.AxisY.MajorTickMark.Interval = 0;
      chartArea1.AxisY.MajorTickMark.IntervalOffset = 0;
      chartArea1.AxisY.MajorTickMark.IntervalOffsetType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Number;
      chartArea1.AxisY.MajorTickMark.IntervalType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Number;
      chartArea1.AxisY.Minimum = 0;
      chartArea1.AxisY.MinorTickMark.Enabled = true;
      chartArea1.AxisY.Title = "Count";
      chartArea1.AxisY2.MajorGrid.Enabled = false;
      chartArea1.BorderDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
      chartArea1.Name = "ChartArea1";
      this.ProfileChart.ChartAreas.Add (chartArea1);
      legend1.Alignment = System.Drawing.StringAlignment.Far;
      legend1.DockedToChartArea = "ChartArea1";
      legend1.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom;
      legend1.IsDockedInsideChartArea = false;
      legend1.Name = "Legend1";
      this.ProfileChart.Legends.Add (legend1);
      this.ProfileChart.Location = new System.Drawing.Point (12, 233);
      this.ProfileChart.Name = "ProfileChart";
      series1.ChartArea = "ChartArea1";
      series1.Legend = "Legend1";
      series1.Name = "Series1";
      dataPoint1.AxisLabel = "12:30";
      dataPoint1.IsValueShownAsLabel = false;
      dataPoint1.Label = "";
      dataPoint2.AxisLabel = "12:45";
      series1.Points.Add (dataPoint1);
      series1.Points.Add (dataPoint2);
      this.ProfileChart.Series.Add (series1);
      this.ProfileChart.Size = new System.Drawing.Size (1160, 566);
      this.ProfileChart.TabIndex = 30;
      this.ProfileChart.Text = "Depth Profile";
      title1.Name = "Title1";
      title1.Text = "Verticle Profile";
      title2.Name = "Title2";
      title2.Text = "Cruise: jksdjsk  Station:XXX  Deployment: Alpa";
      this.ProfileChart.Titles.Add (title1);
      this.ProfileChart.Titles.Add (title2);
      // 
      // TimeIncr
      // 
      this.TimeIncr.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TimeIncr.Location = new System.Drawing.Point (30, 171);
      this.TimeIncr.Maximum = new decimal (new int[] {
            3600,
            0,
            0,
            0});
      this.TimeIncr.Minimum = new decimal (new int[] {
            1,
            0,
            0,
            0});
      this.TimeIncr.Name = "TimeIncr";
      this.TimeIncr.Size = new System.Drawing.Size (76, 23);
      this.TimeIncr.TabIndex = 31;
      this.TimeIncr.Value = new decimal (new int[] {
            60,
            0,
            0,
            0});
      // 
      // timer1
      // 
      this.timer1.Interval = 50;
      this.timer1.Tick += new System.EventHandler (this.timer1_Tick);
      // 
      // TimeIncrLabel
      // 
      this.TimeIncrLabel.AutoSize = true;
      this.TimeIncrLabel.Location = new System.Drawing.Point (27, 155);
      this.TimeIncrLabel.Name = "TimeIncrLabel";
      this.TimeIncrLabel.Size = new System.Drawing.Size (80, 13);
      this.TimeIncrLabel.TabIndex = 32;
      this.TimeIncrLabel.Text = "Time Increment";
      // 
      // Inst1ColorButton
      // 
      this.Inst1ColorButton.Location = new System.Drawing.Point (1124, 37);
      this.Inst1ColorButton.Name = "Inst1ColorButton";
      this.Inst1ColorButton.Size = new System.Drawing.Size (49, 23);
      this.Inst1ColorButton.TabIndex = 33;
      this.Inst1ColorButton.Text = "Color";
      this.Inst1ColorButton.UseVisualStyleBackColor = true;
      this.Inst1ColorButton.Click += new System.EventHandler (this.Inst1ColorButton_Click);
      // 
      // Status
      // 
      this.Status.Font = new System.Drawing.Font ("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Status.Location = new System.Drawing.Point (791, 84);
      this.Status.Multiline = true;
      this.Status.Name = "Status";
      this.Status.ReadOnly = true;
      this.Status.Size = new System.Drawing.Size (381, 93);
      this.Status.TabIndex = 35;
      this.Status.TabStop = false;
      // 
      // RePlotButton
      // 
      this.RePlotButton.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.RePlotButton.Location = new System.Drawing.Point (941, 188);
      this.RePlotButton.Name = "RePlotButton";
      this.RePlotButton.Size = new System.Drawing.Size (75, 23);
      this.RePlotButton.TabIndex = 43;
      this.RePlotButton.Text = "Re-Plot";
      this.RePlotButton.UseVisualStyleBackColor = true;
      this.RePlotButton.Click += new System.EventHandler (this.RePlotButton_Click);
      // 
      // ClassPlottingCriteriaPanel
      // 
      this.ClassPlottingCriteriaPanel.Controls.Add (this.ClassesToPlot);
      this.ClassPlottingCriteriaPanel.Controls.Add (this.AddClassButton);
      this.ClassPlottingCriteriaPanel.Controls.Add (this.IncludeSubClasses);
      this.ClassPlottingCriteriaPanel.Controls.Add (this.WeightByVolume);
      this.ClassPlottingCriteriaPanel.Location = new System.Drawing.Point (283, 12);
      this.ClassPlottingCriteriaPanel.Name = "ClassPlottingCriteriaPanel";
      this.ClassPlottingCriteriaPanel.Size = new System.Drawing.Size (484, 204);
      this.ClassPlottingCriteriaPanel.TabIndex = 44;
      // 
      // ChartHorizontalProfile
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (1184, 811);
      this.Controls.Add (this.ClassPlottingCriteriaPanel);
      this.Controls.Add (this.RePlotButton);
      this.Controls.Add (this.Status);
      this.Controls.Add (this.Inst1ColorButton);
      this.Controls.Add (this.TimeIncr);
      this.Controls.Add (this.ProfileChart);
      this.Controls.Add (this.TimeIncrLabel);
      this.Controls.Add (this.CancelPlotButton);
      this.Controls.Add (this.PlotButton);
      this.Controls.Add (this.Inst1ToPlot);
      this.Controls.Add (this.SelectionCriteriaPanel);
      this.Controls.Add (this.Inst1ToPlotLabel);
      this.MinimumSize = new System.Drawing.Size (983, 635);
      this.Name = "ChartHorizontalProfile";
      this.Text = "Depth Profile Chart";
      this.Load += new System.EventHandler (this.ChartHorizontalProfile_Load);
      this.SizeChanged += new System.EventHandler (this.ChartHorizontalProfile_SizeChanged);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.ChartHorizontalProfile_FormClosing);
      this.Resize += new System.EventHandler (this.ChartHorizontalProfile_Resize);
      ((System.ComponentModel.ISupportInitialize)(this.ClassesToPlot)).EndInit ();
      this.SelectionCriteriaPanel.ResumeLayout (false);
      this.SelectionCriteriaPanel.PerformLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.ProfileChart)).EndInit ();
      ((System.ComponentModel.ISupportInitialize)(this.TimeIncr)).EndInit ();
      this.ClassPlottingCriteriaPanel.ResumeLayout (false);
      this.ClassPlottingCriteriaPanel.PerformLayout ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.DataGridView ClassesToPlot;
    private System.Windows.Forms.CheckBox IncludeSubClasses;
    private System.Windows.Forms.Button AddClassButton;
    private System.Windows.Forms.CheckBox WeightByVolume;
    private System.Windows.Forms.Label Inst1ToPlotLabel;
    private System.Windows.Forms.Panel SelectionCriteriaPanel;
    private System.Windows.Forms.TextBox SizeProbDepthCriteria;
    private System.Windows.Forms.Label SizePrametersLabel;
    private System.Windows.Forms.Label CruiseLabel;
    private System.Windows.Forms.TextBox CruiseStationDeployment;
    private System.Windows.Forms.ComboBox Inst1ToPlot;
    private System.Windows.Forms.Button PlotButton;
    private System.Windows.Forms.Button CancelPlotButton;
    private System.Windows.Forms.DataVisualization.Charting.Chart ProfileChart;
    private System.Windows.Forms.NumericUpDown TimeIncr;
    private System.Windows.Forms.Timer timer1;
    private System.Windows.Forms.Label TimeIncrLabel;
    private System.Windows.Forms.DataGridViewTextBoxColumn Class;
    private System.Windows.Forms.DataGridViewButtonColumn BrowseClasses;
    private System.Windows.Forms.DataGridViewButtonColumn Remove;
    private System.Windows.Forms.DataGridViewButtonColumn ColorToUse;
    private System.Windows.Forms.Button Inst1ColorButton;
    private System.Windows.Forms.TextBox Status;
    private System.Windows.Forms.Button RePlotButton;
    private System.Windows.Forms.Panel ClassPlottingCriteriaPanel;


  }
}