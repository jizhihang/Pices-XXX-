namespace SipperFileViewer
{
  partial class SipperFileViewer
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
      System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series ();
      this.SipperStreamPanel = new System.Windows.Forms.Panel ();
      this.vScrollBar1 = new System.Windows.Forms.VScrollBar ();
      this.ScanLinesDisplayedFirst = new System.Windows.Forms.TextBox ();
      this.ScanLinesDisplayedLast = new System.Windows.Forms.TextBox ();
      this.FileName = new System.Windows.Forms.TextBox ();
      this.FileNameLabel = new System.Windows.Forms.Label ();
      this.menuStrip1 = new System.Windows.Forms.MenuStrip ();
      this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.openSipperFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.connectDatabaseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.configurationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.saveInstrumentDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.autoDetectionToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem ();
      this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.chartingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.timeChartToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.depthChartToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.navigateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.jumpToStartExtractionPointToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.jumpToEndExtractionPointToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.FileDescription = new System.Windows.Forms.TextBox ();
      this.FileDescriptionLabel = new System.Windows.Forms.Label ();
      this.StatusUpdateTimer = new System.Windows.Forms.Timer (this.components);
      this.FrameNumDisplayedFirst = new System.Windows.Forms.TextBox ();
      this.FrameNumDisplayedFirstLabel = new System.Windows.Forms.Label ();
      this.ScanLinesDisplayedFirstLabel = new System.Windows.Forms.Label ();
      this.TotalScanLinesLabel = new System.Windows.Forms.Label ();
      this.SanLinesTotal = new System.Windows.Forms.TextBox ();
      this.Data0 = new System.Windows.Forms.TextBox ();
      this.Data2 = new System.Windows.Forms.TextBox ();
      this.Data3 = new System.Windows.Forms.TextBox ();
      this.ScanLinesDisplayedLastLabel = new System.Windows.Forms.Label ();
      this.Data1 = new System.Windows.Forms.TextBox ();
      this.MouseCoordinates = new System.Windows.Forms.TextBox ();
      this.MouseCoordinatesLabel = new System.Windows.Forms.Label ();
      this.Data0Label = new System.Windows.Forms.Button ();
      this.Data1Label = new System.Windows.Forms.Button ();
      this.Data2Label = new System.Windows.Forms.Button ();
      this.Data3Label = new System.Windows.Forms.Button ();
      this.Data4 = new System.Windows.Forms.TextBox ();
      this.Data4Label = new System.Windows.Forms.Button ();
      this.AutoScrollStop = new System.Windows.Forms.Button ();
      this.AutoScrollingPanel = new System.Windows.Forms.Panel ();
      this.AutoScrollingRate = new System.Windows.Forms.TextBox ();
      this.AutoScrollingLabel = new System.Windows.Forms.Label ();
      this.AutoScrollDown = new System.Windows.Forms.Button ();
      this.AutoScrollUp = new System.Windows.Forms.Button ();
      this.autoDetect = new System.Windows.Forms.CheckBox ();
      this.colorize = new System.Windows.Forms.CheckBox ();
      this.cropColLeft = new System.Windows.Forms.TextBox ();
      this.cropColRight = new System.Windows.Forms.TextBox ();
      this.NavigateChart = new System.Windows.Forms.DataVisualization.Charting.Chart ();
      this.NavigationDataToPlot = new System.Windows.Forms.Button ();
      this.NavigationDataToPlot2 = new System.Windows.Forms.Button ();
      this.editSipperFileParametersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
      this.menuStrip1.SuspendLayout ();
      this.AutoScrollingPanel.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.NavigateChart)).BeginInit ();
      this.SuspendLayout ();
      // 
      // SipperStreamPanel
      // 
      this.SipperStreamPanel.BackColor = System.Drawing.SystemColors.Window;
      this.SipperStreamPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.SipperStreamPanel.Location = new System.Drawing.Point (95, 144);
      this.SipperStreamPanel.Name = "SipperStreamPanel";
      this.SipperStreamPanel.Size = new System.Drawing.Size (602, 540);
      this.SipperStreamPanel.TabIndex = 0;
      this.SipperStreamPanel.Paint += new System.Windows.Forms.PaintEventHandler (this.SipperStreamPanel_Paint);
      this.SipperStreamPanel.MouseMove += new System.Windows.Forms.MouseEventHandler (this.SipperStreamPanel_MouseMove);
      this.SipperStreamPanel.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.SipperStreamPanel_MouseDoubleClick);
      this.SipperStreamPanel.MouseDown += new System.Windows.Forms.MouseEventHandler (this.SipperStreamPanel_MouseDown);
      this.SipperStreamPanel.MouseUp += new System.Windows.Forms.MouseEventHandler (this.SipperStreamPanel_MouseUp);
      // 
      // vScrollBar1
      // 
      this.vScrollBar1.Location = new System.Drawing.Point (697, 144);
      this.vScrollBar1.Margin = new System.Windows.Forms.Padding (1);
      this.vScrollBar1.Name = "vScrollBar1";
      this.vScrollBar1.Size = new System.Drawing.Size (22, 540);
      this.vScrollBar1.TabIndex = 1;
      this.vScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler (this.vScrollBar1_Scroll);
      // 
      // ScanLinesDisplayedFirst
      // 
      this.ScanLinesDisplayedFirst.Enabled = false;
      this.ScanLinesDisplayedFirst.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanLinesDisplayedFirst.Location = new System.Drawing.Point (9, 144);
      this.ScanLinesDisplayedFirst.Name = "ScanLinesDisplayedFirst";
      this.ScanLinesDisplayedFirst.Size = new System.Drawing.Size (86, 22);
      this.ScanLinesDisplayedFirst.TabIndex = 2;
      this.ScanLinesDisplayedFirst.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // ScanLinesDisplayedLast
      // 
      this.ScanLinesDisplayedLast.Enabled = false;
      this.ScanLinesDisplayedLast.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanLinesDisplayedLast.Location = new System.Drawing.Point (9, 662);
      this.ScanLinesDisplayedLast.Name = "ScanLinesDisplayedLast";
      this.ScanLinesDisplayedLast.Size = new System.Drawing.Size (86, 22);
      this.ScanLinesDisplayedLast.TabIndex = 3;
      this.ScanLinesDisplayedLast.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // FileName
      // 
      this.FileName.Enabled = false;
      this.FileName.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FileName.Location = new System.Drawing.Point (96, 34);
      this.FileName.Name = "FileName";
      this.FileName.Size = new System.Drawing.Size (453, 22);
      this.FileName.TabIndex = 6;
      // 
      // FileNameLabel
      // 
      this.FileNameLabel.AutoSize = true;
      this.FileNameLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FileNameLabel.Location = new System.Drawing.Point (7, 36);
      this.FileNameLabel.Name = "FileNameLabel";
      this.FileNameLabel.Size = new System.Drawing.Size (80, 18);
      this.FileNameLabel.TabIndex = 8;
      this.FileNameLabel.Text = "File Name";
      // 
      // menuStrip1
      // 
      this.menuStrip1.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.chartingToolStripMenuItem,
            this.navigateToolStripMenuItem,
            this.helpToolStripMenuItem});
      this.menuStrip1.Location = new System.Drawing.Point (0, 0);
      this.menuStrip1.Name = "menuStrip1";
      this.menuStrip1.Size = new System.Drawing.Size (976, 24);
      this.menuStrip1.TabIndex = 9;
      this.menuStrip1.Text = "menuStrip1";
      // 
      // fileToolStripMenuItem
      // 
      this.fileToolStripMenuItem.DropDownItems.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.openSipperFileToolStripMenuItem,
            this.connectDatabaseToolStripMenuItem,
            this.editSipperFileParametersToolStripMenuItem,
            this.configurationToolStripMenuItem,
            this.saveInstrumentDataToolStripMenuItem,
            this.autoDetectionToolStripMenuItem1,
            this.exitToolStripMenuItem});
      this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
      this.fileToolStripMenuItem.Size = new System.Drawing.Size (37, 20);
      this.fileToolStripMenuItem.Text = "&File";
      // 
      // openSipperFileToolStripMenuItem
      // 
      this.openSipperFileToolStripMenuItem.Name = "openSipperFileToolStripMenuItem";
      this.openSipperFileToolStripMenuItem.Size = new System.Drawing.Size (213, 22);
      this.openSipperFileToolStripMenuItem.Text = "&Open Sipper File";
      this.openSipperFileToolStripMenuItem.Click += new System.EventHandler (this.openSipperFileToolStripMenuItem_Click);
      // 
      // connectDatabaseToolStripMenuItem
      // 
      this.connectDatabaseToolStripMenuItem.Name = "connectDatabaseToolStripMenuItem";
      this.connectDatabaseToolStripMenuItem.Size = new System.Drawing.Size (213, 22);
      this.connectDatabaseToolStripMenuItem.Text = "Connect Database";
      this.connectDatabaseToolStripMenuItem.Click += new System.EventHandler (this.connectDatabaseToolStripMenuItem_Click);
      // 
      // configurationToolStripMenuItem
      // 
      this.configurationToolStripMenuItem.Name = "configurationToolStripMenuItem";
      this.configurationToolStripMenuItem.Size = new System.Drawing.Size (213, 22);
      this.configurationToolStripMenuItem.Text = "&Configuration";
      this.configurationToolStripMenuItem.Click += new System.EventHandler (this.configurationToolStripMenuItem_Click);
      // 
      // saveInstrumentDataToolStripMenuItem
      // 
      this.saveInstrumentDataToolStripMenuItem.Name = "saveInstrumentDataToolStripMenuItem";
      this.saveInstrumentDataToolStripMenuItem.Size = new System.Drawing.Size (213, 22);
      this.saveInstrumentDataToolStripMenuItem.Text = "&Save Raw CTD Data";
      this.saveInstrumentDataToolStripMenuItem.Click += new System.EventHandler (this.saveRawCTDDataMenuItem_Click_1);
      // 
      // autoDetectionToolStripMenuItem1
      // 
      this.autoDetectionToolStripMenuItem1.Name = "autoDetectionToolStripMenuItem1";
      this.autoDetectionToolStripMenuItem1.Size = new System.Drawing.Size (213, 22);
      this.autoDetectionToolStripMenuItem1.Text = "&Auto Detection";
      this.autoDetectionToolStripMenuItem1.Click += new System.EventHandler (this.autoDetectionToolStripMenuItem1_Click);
      // 
      // exitToolStripMenuItem
      // 
      this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
      this.exitToolStripMenuItem.Size = new System.Drawing.Size (213, 22);
      this.exitToolStripMenuItem.Text = "Exit";
      this.exitToolStripMenuItem.Click += new System.EventHandler (this.exitToolStripMenuItem_Click);
      // 
      // chartingToolStripMenuItem
      // 
      this.chartingToolStripMenuItem.DropDownItems.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.timeChartToolStripMenuItem,
            this.depthChartToolStripMenuItem});
      this.chartingToolStripMenuItem.Name = "chartingToolStripMenuItem";
      this.chartingToolStripMenuItem.Size = new System.Drawing.Size (65, 20);
      this.chartingToolStripMenuItem.Text = "&Charting";
      // 
      // timeChartToolStripMenuItem
      // 
      this.timeChartToolStripMenuItem.Name = "timeChartToolStripMenuItem";
      this.timeChartToolStripMenuItem.Size = new System.Drawing.Size (138, 22);
      this.timeChartToolStripMenuItem.Text = "Time Chart";
      this.timeChartToolStripMenuItem.Click += new System.EventHandler (this.timeChartToolStripMenuItem_Click);
      // 
      // depthChartToolStripMenuItem
      // 
      this.depthChartToolStripMenuItem.Name = "depthChartToolStripMenuItem";
      this.depthChartToolStripMenuItem.Size = new System.Drawing.Size (138, 22);
      this.depthChartToolStripMenuItem.Text = "Depth Chart";
      this.depthChartToolStripMenuItem.Click += new System.EventHandler (this.depthChartToolStripMenuItem_Click);
      // 
      // navigateToolStripMenuItem
      // 
      this.navigateToolStripMenuItem.DropDownItems.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.jumpToStartExtractionPointToolStripMenuItem,
            this.jumpToEndExtractionPointToolStripMenuItem});
      this.navigateToolStripMenuItem.Name = "navigateToolStripMenuItem";
      this.navigateToolStripMenuItem.Size = new System.Drawing.Size (66, 20);
      this.navigateToolStripMenuItem.Text = "Navigate";
      // 
      // jumpToStartExtractionPointToolStripMenuItem
      // 
      this.jumpToStartExtractionPointToolStripMenuItem.Name = "jumpToStartExtractionPointToolStripMenuItem";
      this.jumpToStartExtractionPointToolStripMenuItem.Size = new System.Drawing.Size (230, 22);
      this.jumpToStartExtractionPointToolStripMenuItem.Text = "Jump to Start Extraction Point";
      this.jumpToStartExtractionPointToolStripMenuItem.Click += new System.EventHandler (this.jumpToStartExtractionPointToolStripMenuItem_Click);
      // 
      // jumpToEndExtractionPointToolStripMenuItem
      // 
      this.jumpToEndExtractionPointToolStripMenuItem.Name = "jumpToEndExtractionPointToolStripMenuItem";
      this.jumpToEndExtractionPointToolStripMenuItem.Size = new System.Drawing.Size (230, 22);
      this.jumpToEndExtractionPointToolStripMenuItem.Text = "Jump to End Extraction Point";
      this.jumpToEndExtractionPointToolStripMenuItem.Click += new System.EventHandler (this.jumpToEndExtractionPointToolStripMenuItem_Click);
      // 
      // helpToolStripMenuItem
      // 
      this.helpToolStripMenuItem.DropDownItems.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
      this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
      this.helpToolStripMenuItem.Size = new System.Drawing.Size (44, 20);
      this.helpToolStripMenuItem.Text = "&Help";
      // 
      // aboutToolStripMenuItem
      // 
      this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
      this.aboutToolStripMenuItem.Size = new System.Drawing.Size (107, 22);
      this.aboutToolStripMenuItem.Text = "About";
      // 
      // FileDescription
      // 
      this.FileDescription.Enabled = false;
      this.FileDescription.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FileDescription.Location = new System.Drawing.Point (97, 64);
      this.FileDescription.Name = "FileDescription";
      this.FileDescription.Size = new System.Drawing.Size (622, 22);
      this.FileDescription.TabIndex = 3;
      // 
      // FileDescriptionLabel
      // 
      this.FileDescriptionLabel.AutoSize = true;
      this.FileDescriptionLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FileDescriptionLabel.Location = new System.Drawing.Point (7, 66);
      this.FileDescriptionLabel.Name = "FileDescriptionLabel";
      this.FileDescriptionLabel.Size = new System.Drawing.Size (88, 18);
      this.FileDescriptionLabel.TabIndex = 10;
      this.FileDescriptionLabel.Text = "Description";
      // 
      // StatusUpdateTimer
      // 
      this.StatusUpdateTimer.Interval = 33;
      this.StatusUpdateTimer.Tick += new System.EventHandler (this.StatusUpdateTimer_Tick);
      // 
      // FrameNumDisplayedFirst
      // 
      this.FrameNumDisplayedFirst.Enabled = false;
      this.FrameNumDisplayedFirst.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FrameNumDisplayedFirst.Location = new System.Drawing.Point (9, 191);
      this.FrameNumDisplayedFirst.Name = "FrameNumDisplayedFirst";
      this.FrameNumDisplayedFirst.Size = new System.Drawing.Size (86, 22);
      this.FrameNumDisplayedFirst.TabIndex = 11;
      this.FrameNumDisplayedFirst.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // FrameNumDisplayedFirstLabel
      // 
      this.FrameNumDisplayedFirstLabel.AutoSize = true;
      this.FrameNumDisplayedFirstLabel.Font = new System.Drawing.Font ("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.FrameNumDisplayedFirstLabel.Location = new System.Drawing.Point (29, 216);
      this.FrameNumDisplayedFirstLabel.Name = "FrameNumDisplayedFirstLabel";
      this.FrameNumDisplayedFirstLabel.Size = new System.Drawing.Size (46, 14);
      this.FrameNumDisplayedFirstLabel.TabIndex = 12;
      this.FrameNumDisplayedFirstLabel.Text = "Frame #";
      this.FrameNumDisplayedFirstLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
      // 
      // ScanLinesDisplayedFirstLabel
      // 
      this.ScanLinesDisplayedFirstLabel.AutoSize = true;
      this.ScanLinesDisplayedFirstLabel.Font = new System.Drawing.Font ("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanLinesDisplayedFirstLabel.Location = new System.Drawing.Point (26, 166);
      this.ScanLinesDisplayedFirstLabel.Name = "ScanLinesDisplayedFirstLabel";
      this.ScanLinesDisplayedFirstLabel.Size = new System.Drawing.Size (55, 14);
      this.ScanLinesDisplayedFirstLabel.TabIndex = 13;
      this.ScanLinesDisplayedFirstLabel.Text = "Scan Line";
      this.ScanLinesDisplayedFirstLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
      // 
      // TotalScanLinesLabel
      // 
      this.TotalScanLinesLabel.AutoSize = true;
      this.TotalScanLinesLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TotalScanLinesLabel.Location = new System.Drawing.Point (554, 36);
      this.TotalScanLinesLabel.Name = "TotalScanLinesLabel";
      this.TotalScanLinesLabel.Size = new System.Drawing.Size (86, 18);
      this.TotalScanLinesLabel.TabIndex = 14;
      this.TotalScanLinesLabel.Text = "Scan Lines";
      // 
      // SanLinesTotal
      // 
      this.SanLinesTotal.Enabled = false;
      this.SanLinesTotal.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SanLinesTotal.Location = new System.Drawing.Point (642, 34);
      this.SanLinesTotal.Name = "SanLinesTotal";
      this.SanLinesTotal.Size = new System.Drawing.Size (76, 22);
      this.SanLinesTotal.TabIndex = 15;
      this.SanLinesTotal.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // Data0
      // 
      this.Data0.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data0.Location = new System.Drawing.Point (9, 255);
      this.Data0.Name = "Data0";
      this.Data0.ReadOnly = true;
      this.Data0.Size = new System.Drawing.Size (86, 22);
      this.Data0.TabIndex = 16;
      this.Data0.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // Data2
      // 
      this.Data2.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data2.Location = new System.Drawing.Point (9, 385);
      this.Data2.Name = "Data2";
      this.Data2.ReadOnly = true;
      this.Data2.Size = new System.Drawing.Size (86, 22);
      this.Data2.TabIndex = 18;
      this.Data2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // Data3
      // 
      this.Data3.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data3.Location = new System.Drawing.Point (9, 450);
      this.Data3.Name = "Data3";
      this.Data3.ReadOnly = true;
      this.Data3.Size = new System.Drawing.Size (86, 22);
      this.Data3.TabIndex = 20;
      this.Data3.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // ScanLinesDisplayedLastLabel
      // 
      this.ScanLinesDisplayedLastLabel.AutoSize = true;
      this.ScanLinesDisplayedLastLabel.Font = new System.Drawing.Font ("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanLinesDisplayedLastLabel.Location = new System.Drawing.Point (26, 646);
      this.ScanLinesDisplayedLastLabel.Name = "ScanLinesDisplayedLastLabel";
      this.ScanLinesDisplayedLastLabel.Size = new System.Drawing.Size (55, 14);
      this.ScanLinesDisplayedLastLabel.TabIndex = 22;
      this.ScanLinesDisplayedLastLabel.Text = "Scan Line";
      this.ScanLinesDisplayedLastLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
      // 
      // Data1
      // 
      this.Data1.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data1.Location = new System.Drawing.Point (9, 320);
      this.Data1.Name = "Data1";
      this.Data1.ReadOnly = true;
      this.Data1.Size = new System.Drawing.Size (86, 22);
      this.Data1.TabIndex = 23;
      this.Data1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // MouseCoordinates
      // 
      this.MouseCoordinates.Enabled = false;
      this.MouseCoordinates.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MouseCoordinates.Location = new System.Drawing.Point (624, 99);
      this.MouseCoordinates.Name = "MouseCoordinates";
      this.MouseCoordinates.Size = new System.Drawing.Size (95, 22);
      this.MouseCoordinates.TabIndex = 16;
      this.MouseCoordinates.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // MouseCoordinatesLabel
      // 
      this.MouseCoordinatesLabel.AutoSize = true;
      this.MouseCoordinatesLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MouseCoordinatesLabel.Location = new System.Drawing.Point (527, 101);
      this.MouseCoordinatesLabel.Name = "MouseCoordinatesLabel";
      this.MouseCoordinatesLabel.Size = new System.Drawing.Size (94, 18);
      this.MouseCoordinatesLabel.TabIndex = 22;
      this.MouseCoordinatesLabel.Text = "Coordinates";
      this.MouseCoordinatesLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
      // 
      // Data0Label
      // 
      this.Data0Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data0Label.ForeColor = System.Drawing.Color.Gray;
      this.Data0Label.Location = new System.Drawing.Point (9, 276);
      this.Data0Label.Margin = new System.Windows.Forms.Padding (0);
      this.Data0Label.Name = "Data0Label";
      this.Data0Label.Size = new System.Drawing.Size (86, 22);
      this.Data0Label.TabIndex = 25;
      this.Data0Label.Text = "Temperature";
      this.Data0Label.UseVisualStyleBackColor = true;
      // 
      // Data1Label
      // 
      this.Data1Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data1Label.ForeColor = System.Drawing.Color.Gray;
      this.Data1Label.Location = new System.Drawing.Point (9, 341);
      this.Data1Label.Margin = new System.Windows.Forms.Padding (0);
      this.Data1Label.Name = "Data1Label";
      this.Data1Label.Size = new System.Drawing.Size (86, 22);
      this.Data1Label.TabIndex = 26;
      this.Data1Label.Text = "Temperature";
      this.Data1Label.UseVisualStyleBackColor = true;
      // 
      // Data2Label
      // 
      this.Data2Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data2Label.ForeColor = System.Drawing.Color.Gray;
      this.Data2Label.Location = new System.Drawing.Point (9, 406);
      this.Data2Label.Margin = new System.Windows.Forms.Padding (0);
      this.Data2Label.Name = "Data2Label";
      this.Data2Label.Size = new System.Drawing.Size (86, 22);
      this.Data2Label.TabIndex = 27;
      this.Data2Label.Text = "Temperature";
      this.Data2Label.UseVisualStyleBackColor = true;
      // 
      // Data3Label
      // 
      this.Data3Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data3Label.ForeColor = System.Drawing.Color.Gray;
      this.Data3Label.Location = new System.Drawing.Point (9, 472);
      this.Data3Label.Margin = new System.Windows.Forms.Padding (0);
      this.Data3Label.Name = "Data3Label";
      this.Data3Label.Size = new System.Drawing.Size (86, 22);
      this.Data3Label.TabIndex = 28;
      this.Data3Label.Text = "Temperature";
      this.Data3Label.UseVisualStyleBackColor = true;
      // 
      // Data4
      // 
      this.Data4.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data4.Location = new System.Drawing.Point (8, 515);
      this.Data4.Name = "Data4";
      this.Data4.ReadOnly = true;
      this.Data4.Size = new System.Drawing.Size (86, 22);
      this.Data4.TabIndex = 21;
      this.Data4.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // Data4Label
      // 
      this.Data4Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Data4Label.ForeColor = System.Drawing.Color.Gray;
      this.Data4Label.Location = new System.Drawing.Point (9, 537);
      this.Data4Label.Margin = new System.Windows.Forms.Padding (0);
      this.Data4Label.Name = "Data4Label";
      this.Data4Label.Size = new System.Drawing.Size (86, 22);
      this.Data4Label.TabIndex = 29;
      this.Data4Label.Text = "Temperature";
      this.Data4Label.UseVisualStyleBackColor = true;
      // 
      // AutoScrollStop
      // 
      this.AutoScrollStop.Location = new System.Drawing.Point (151, 3);
      this.AutoScrollStop.Name = "AutoScrollStop";
      this.AutoScrollStop.Size = new System.Drawing.Size (38, 23);
      this.AutoScrollStop.TabIndex = 31;
      this.AutoScrollStop.Text = "Stop";
      this.AutoScrollStop.UseVisualStyleBackColor = true;
      this.AutoScrollStop.Click += new System.EventHandler (this.AutoScrollStop_Click);
      // 
      // AutoScrollingPanel
      // 
      this.AutoScrollingPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.AutoScrollingPanel.Controls.Add (this.AutoScrollingRate);
      this.AutoScrollingPanel.Controls.Add (this.AutoScrollStop);
      this.AutoScrollingPanel.Controls.Add (this.AutoScrollingLabel);
      this.AutoScrollingPanel.Controls.Add (this.AutoScrollDown);
      this.AutoScrollingPanel.Controls.Add (this.AutoScrollUp);
      this.AutoScrollingPanel.Location = new System.Drawing.Point (224, 94);
      this.AutoScrollingPanel.Name = "AutoScrollingPanel";
      this.AutoScrollingPanel.Size = new System.Drawing.Size (244, 30);
      this.AutoScrollingPanel.TabIndex = 32;
      // 
      // AutoScrollingRate
      // 
      this.AutoScrollingRate.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.AutoScrollingRate.ForeColor = System.Drawing.SystemColors.WindowText;
      this.AutoScrollingRate.Location = new System.Drawing.Point (195, 4);
      this.AutoScrollingRate.Name = "AutoScrollingRate";
      this.AutoScrollingRate.ReadOnly = true;
      this.AutoScrollingRate.Size = new System.Drawing.Size (41, 22);
      this.AutoScrollingRate.TabIndex = 32;
      this.AutoScrollingRate.Text = "0";
      this.AutoScrollingRate.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // AutoScrollingLabel
      // 
      this.AutoScrollingLabel.AutoSize = true;
      this.AutoScrollingLabel.Location = new System.Drawing.Point (3, 8);
      this.AutoScrollingLabel.Name = "AutoScrollingLabel";
      this.AutoScrollingLabel.Size = new System.Drawing.Size (72, 13);
      this.AutoScrollingLabel.TabIndex = 0;
      this.AutoScrollingLabel.Text = "Auto Scrolling";
      this.AutoScrollingLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // AutoScrollDown
      // 
      this.AutoScrollDown.Image = global::SipperFileViewer.Properties.Resources.DownArrow;
      this.AutoScrollDown.Location = new System.Drawing.Point (114, 3);
      this.AutoScrollDown.Name = "AutoScrollDown";
      this.AutoScrollDown.Size = new System.Drawing.Size (31, 23);
      this.AutoScrollDown.TabIndex = 31;
      this.AutoScrollDown.UseVisualStyleBackColor = true;
      this.AutoScrollDown.Click += new System.EventHandler (this.AutoScrollDown_Click);
      // 
      // AutoScrollUp
      // 
      this.AutoScrollUp.Image = global::SipperFileViewer.Properties.Resources.UpArrow;
      this.AutoScrollUp.Location = new System.Drawing.Point (77, 3);
      this.AutoScrollUp.Name = "AutoScrollUp";
      this.AutoScrollUp.Size = new System.Drawing.Size (31, 23);
      this.AutoScrollUp.TabIndex = 30;
      this.AutoScrollUp.UseVisualStyleBackColor = true;
      this.AutoScrollUp.Click += new System.EventHandler (this.AutoScrollUp_Click);
      // 
      // autoDetect
      // 
      this.autoDetect.AutoSize = true;
      this.autoDetect.Location = new System.Drawing.Point (98, 101);
      this.autoDetect.Name = "autoDetect";
      this.autoDetect.Size = new System.Drawing.Size (83, 17);
      this.autoDetect.TabIndex = 34;
      this.autoDetect.Text = "Auto Detect";
      this.autoDetect.UseVisualStyleBackColor = true;
      this.autoDetect.CheckedChanged += new System.EventHandler (this.autoDetect_CheckedChanged);
      // 
      // colorize
      // 
      this.colorize.AutoSize = true;
      this.colorize.Location = new System.Drawing.Point (24, 102);
      this.colorize.Name = "colorize";
      this.colorize.Size = new System.Drawing.Size (63, 17);
      this.colorize.TabIndex = 35;
      this.colorize.Text = "Colorize";
      this.colorize.UseVisualStyleBackColor = true;
      this.colorize.CheckedChanged += new System.EventHandler (this.colorize_CheckedChanged);
      // 
      // cropColLeft
      // 
      this.cropColLeft.Enabled = false;
      this.cropColLeft.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.cropColLeft.Location = new System.Drawing.Point (76, 124);
      this.cropColLeft.Name = "cropColLeft";
      this.cropColLeft.ReadOnly = true;
      this.cropColLeft.Size = new System.Drawing.Size (39, 20);
      this.cropColLeft.TabIndex = 36;
      this.cropColLeft.Text = "0";
      this.cropColLeft.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // cropColRight
      // 
      this.cropColRight.Enabled = false;
      this.cropColRight.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.cropColRight.Location = new System.Drawing.Point (679, 124);
      this.cropColRight.Name = "cropColRight";
      this.cropColRight.ReadOnly = true;
      this.cropColRight.Size = new System.Drawing.Size (39, 20);
      this.cropColRight.TabIndex = 37;
      this.cropColRight.Text = "4095";
      this.cropColRight.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // NavigateChart
      // 
      chartArea1.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount;
      chartArea1.AxisX.LabelStyle.Format = "##0.0";
      chartArea1.AxisX.LabelStyle.Interval = 0;
      chartArea1.AxisX.MajorGrid.Enabled = false;
      chartArea1.AxisX.MajorGrid.Interval = 0;
      chartArea1.AxisX.MajorGrid.LineColor = System.Drawing.Color.Transparent;
      chartArea1.AxisX.MajorTickMark.Interval = 0;
      chartArea1.AxisX2.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount;
      chartArea1.AxisX2.LabelStyle.Format = "##0.0";
      chartArea1.AxisX2.LabelStyle.Interval = 0;
      chartArea1.AxisX2.MajorGrid.Enabled = false;
      chartArea1.AxisX2.MajorGrid.Interval = 0;
      chartArea1.AxisX2.MajorGrid.LineColor = System.Drawing.Color.Transparent;
      chartArea1.AxisX2.MajorTickMark.Interval = 0;
      chartArea1.AxisY.IsReversed = true;
      chartArea1.AxisY.LabelStyle.Format = "mm:ss";
      chartArea1.AxisY.LabelStyle.Interval = 0;
      chartArea1.AxisY.LabelStyle.IntervalType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Seconds;
      chartArea1.AxisY.MajorGrid.Interval = 0;
      chartArea1.AxisY.MajorGrid.LineColor = System.Drawing.Color.DarkGray;
      chartArea1.AxisY.MajorGrid.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Dash;
      chartArea1.AxisY.MajorTickMark.Interval = 0;
      chartArea1.Name = "ChartArea1";
      this.NavigateChart.ChartAreas.Add (chartArea1);
      legend1.Enabled = false;
      legend1.Name = "Legend1";
      this.NavigateChart.Legends.Add (legend1);
      this.NavigateChart.Location = new System.Drawing.Point (724, 144);
      this.NavigateChart.Name = "NavigateChart";
      series1.ChartArea = "ChartArea1";
      series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Point;
      series1.Legend = "Legend1";
      series1.Name = "Series1";
      series1.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Time;
      series2.ChartArea = "ChartArea1";
      series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Point;
      series2.Legend = "Legend1";
      series2.Name = "Series2";
      series2.XAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;
      series2.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Time;
      this.NavigateChart.Series.Add (series1);
      this.NavigateChart.Series.Add (series2);
      this.NavigateChart.Size = new System.Drawing.Size (239, 540);
      this.NavigateChart.TabIndex = 38;
      this.NavigateChart.Text = "chart1";
      this.NavigateChart.MouseClick += new System.Windows.Forms.MouseEventHandler (this.NavigateChart_MouseClick);
      // 
      // NavigationDataToPlot
      // 
      this.NavigationDataToPlot.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.NavigationDataToPlot.ForeColor = System.Drawing.Color.Gray;
      this.NavigationDataToPlot.Location = new System.Drawing.Point (724, 85);
      this.NavigationDataToPlot.Margin = new System.Windows.Forms.Padding (0);
      this.NavigationDataToPlot.Name = "NavigationDataToPlot";
      this.NavigationDataToPlot.Size = new System.Drawing.Size (239, 22);
      this.NavigationDataToPlot.TabIndex = 39;
      this.NavigationDataToPlot.Text = "Temperature";
      this.NavigationDataToPlot.UseVisualStyleBackColor = true;
      this.NavigationDataToPlot.Click += new System.EventHandler (this.NavigationDataToPlot_Click);
      // 
      // NavigationDataToPlot2
      // 
      this.NavigationDataToPlot2.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.NavigationDataToPlot2.ForeColor = System.Drawing.Color.Gray;
      this.NavigationDataToPlot2.Location = new System.Drawing.Point (724, 115);
      this.NavigationDataToPlot2.Margin = new System.Windows.Forms.Padding (0);
      this.NavigationDataToPlot2.Name = "NavigationDataToPlot2";
      this.NavigationDataToPlot2.Size = new System.Drawing.Size (239, 22);
      this.NavigationDataToPlot2.TabIndex = 40;
      this.NavigationDataToPlot2.Text = "Depth";
      this.NavigationDataToPlot2.UseVisualStyleBackColor = true;
      this.NavigationDataToPlot2.Click += new System.EventHandler (this.NavigationDataToPlot2_Click);
      // 
      // editSipperFileParametersToolStripMenuItem
      // 
      this.editSipperFileParametersToolStripMenuItem.Name = "editSipperFileParametersToolStripMenuItem";
      this.editSipperFileParametersToolStripMenuItem.Size = new System.Drawing.Size (213, 22);
      this.editSipperFileParametersToolStripMenuItem.Text = "Edit Sipper File Parameters";
      this.editSipperFileParametersToolStripMenuItem.Click += new System.EventHandler (this.editSipperFileParametersToolStripMenuItem_Click);
      // 
      // SipperFileViewer
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (976, 711);
      this.Controls.Add (this.NavigationDataToPlot2);
      this.Controls.Add (this.NavigationDataToPlot);
      this.Controls.Add (this.NavigateChart);
      this.Controls.Add (this.cropColRight);
      this.Controls.Add (this.cropColLeft);
      this.Controls.Add (this.colorize);
      this.Controls.Add (this.autoDetect);
      this.Controls.Add (this.AutoScrollingPanel);
      this.Controls.Add (this.Data4Label);
      this.Controls.Add (this.Data4);
      this.Controls.Add (this.Data3Label);
      this.Controls.Add (this.Data2Label);
      this.Controls.Add (this.Data1Label);
      this.Controls.Add (this.Data0Label);
      this.Controls.Add (this.MouseCoordinatesLabel);
      this.Controls.Add (this.MouseCoordinates);
      this.Controls.Add (this.Data1);
      this.Controls.Add (this.ScanLinesDisplayedLastLabel);
      this.Controls.Add (this.Data3);
      this.Controls.Add (this.Data2);
      this.Controls.Add (this.Data0);
      this.Controls.Add (this.SanLinesTotal);
      this.Controls.Add (this.TotalScanLinesLabel);
      this.Controls.Add (this.ScanLinesDisplayedFirstLabel);
      this.Controls.Add (this.FrameNumDisplayedFirstLabel);
      this.Controls.Add (this.FrameNumDisplayedFirst);
      this.Controls.Add (this.FileDescriptionLabel);
      this.Controls.Add (this.FileDescription);
      this.Controls.Add (this.FileNameLabel);
      this.Controls.Add (this.FileName);
      this.Controls.Add (this.ScanLinesDisplayedLast);
      this.Controls.Add (this.ScanLinesDisplayedFirst);
      this.Controls.Add (this.vScrollBar1);
      this.Controls.Add (this.SipperStreamPanel);
      this.Controls.Add (this.menuStrip1);
      this.KeyPreview = true;
      this.MainMenuStrip = this.menuStrip1;
      this.MinimumSize = new System.Drawing.Size (662, 749);
      this.Name = "SipperFileViewer";
      this.Text = "Sipper File Viewer";
      this.Load += new System.EventHandler (this.SipperFileViewer_Load);
      this.SizeChanged += new System.EventHandler (this.SipperFileViewer_SizeChanged);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.SipperFileViewer_FormClosing);
      this.KeyDown += new System.Windows.Forms.KeyEventHandler (this.SipperFileViewer_KeyDown);
      this.menuStrip1.ResumeLayout (false);
      this.menuStrip1.PerformLayout ();
      this.AutoScrollingPanel.ResumeLayout (false);
      this.AutoScrollingPanel.PerformLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.NavigateChart)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Panel SipperStreamPanel;
    private System.Windows.Forms.VScrollBar vScrollBar1;
    private System.Windows.Forms.TextBox ScanLinesDisplayedFirst;
    private System.Windows.Forms.TextBox ScanLinesDisplayedLast;
    private System.Windows.Forms.TextBox FileName;
    private System.Windows.Forms.Label FileNameLabel;
    private System.Windows.Forms.MenuStrip menuStrip1;
    private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem openSipperFileToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem configurationToolStripMenuItem;
    private System.Windows.Forms.TextBox FileDescription;
    private System.Windows.Forms.Label FileDescriptionLabel;
    private System.Windows.Forms.Timer StatusUpdateTimer;
    private System.Windows.Forms.TextBox FrameNumDisplayedFirst;
    private System.Windows.Forms.Label FrameNumDisplayedFirstLabel;
    private System.Windows.Forms.Label ScanLinesDisplayedFirstLabel;
    private System.Windows.Forms.Label TotalScanLinesLabel;
    private System.Windows.Forms.TextBox SanLinesTotal;
    private System.Windows.Forms.TextBox Data0;
    private System.Windows.Forms.TextBox Data2;
    private System.Windows.Forms.TextBox Data3;
    private System.Windows.Forms.Label ScanLinesDisplayedLastLabel;
    private System.Windows.Forms.TextBox Data1;
    private System.Windows.Forms.ToolStripMenuItem saveInstrumentDataToolStripMenuItem;
    private System.Windows.Forms.TextBox MouseCoordinates;
    private System.Windows.Forms.Label MouseCoordinatesLabel;
    private System.Windows.Forms.Button Data0Label;
    private System.Windows.Forms.Button Data1Label;
    private System.Windows.Forms.Button Data2Label;
    private System.Windows.Forms.Button Data3Label;
    private System.Windows.Forms.TextBox Data4;
    private System.Windows.Forms.Button Data4Label;
    private System.Windows.Forms.ToolStripMenuItem chartingToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem timeChartToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem depthChartToolStripMenuItem;
    private System.Windows.Forms.Button AutoScrollUp;
    private System.Windows.Forms.Button AutoScrollDown;
    private System.Windows.Forms.Button AutoScrollStop;
    private System.Windows.Forms.Panel AutoScrollingPanel;
    private System.Windows.Forms.Label AutoScrollingLabel;
    private System.Windows.Forms.TextBox AutoScrollingRate;
    private System.Windows.Forms.CheckBox autoDetect;
    private System.Windows.Forms.ToolStripMenuItem autoDetectionToolStripMenuItem1;
    private System.Windows.Forms.CheckBox colorize;
    private System.Windows.Forms.TextBox cropColLeft;
    private System.Windows.Forms.TextBox cropColRight;
    private System.Windows.Forms.ToolStripMenuItem connectDatabaseToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem navigateToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem jumpToStartExtractionPointToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem jumpToEndExtractionPointToolStripMenuItem;
    private System.Windows.Forms.DataVisualization.Charting.Chart NavigateChart;
    private System.Windows.Forms.Button NavigationDataToPlot;
    private System.Windows.Forms.Button NavigationDataToPlot2;
    private System.Windows.Forms.ToolStripMenuItem editSipperFileParametersToolStripMenuItem;
  }
}

