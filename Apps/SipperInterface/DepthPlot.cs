using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.IO;
using System.Windows.Forms;
//using System.Drawing;
//using System.Drawing.Pen;
//using System.Drawing.Point;
using System.Drawing.Drawing2D;

using ChartControls;
using PicesInterface;
using SipperFile;



namespace SipperInterface
{
	/// <summary>
	/// Summary description for DataWindow.
	/// </summary>
	public class DepthPlot : System.Windows.Forms.Form
	{
    private System.Windows.Forms.Panel ChartPanel;
    private System.ComponentModel.IContainer components;
    private System.Windows.Forms.Timer fetchDataTimer;
    private System.Windows.Forms.TextBox data0;
    private System.Windows.Forms.TextBox data1;
    private System.Windows.Forms.TextBox data2;
    private TextBox data3;
    private TextBox data4;
    private TextBox data5;
    private TextBox data6;
    private TextBox data7;
    private TextBox data8;
    private TextBox data9;
    private TextBox data10;  // Will be owned by "Form1",  we will only
    // have a refrence to it.
    private TextBox data11;
    private TextBox data12;
    private ComboBox data0Label;
    private ComboBox data1Label;
    private ComboBox data2Label;
    private ComboBox data3Label;
    private ComboBox data9Label;
    private ComboBox data8Label;
    private ComboBox data7Label;
    private ComboBox data6Label;
    private ComboBox data5Label;
    private ComboBox data4Label;
    private ComboBox data10Label;
    private ComboBox data11Label;
    private ComboBox data12Label;

    private VScrollBar domainScrollBar;
    private ChartControls.Ruler data0Ruler;
    private ChartControls.Ruler data1Ruler;
    private ChartControls.Ruler data2Ruler;
    private ChartControls.Ruler data3Ruler;
    private ChartControls.Ruler data4Ruler;
    private ChartControls.Ruler data5Ruler;
    private ChartControls.InclinationPanel pitchMeter;
    private ChartControls.InclinationPanel rollMeter;
    private ChartControls.BatteryPack sipperBatteryPack;
    private RadioButtonList depthRange;
    private RadioButtonList timeRange;
    private RadioButtonList plottingAxis;
    private Label           minLabel;
    private Label maxLabel;


    private DepthPlotParameters  chart              = null;
    private bool                 chartRealTime      = true;
    private TextBox[]            dataFields         = null;
    private ComboBox[]           dataLabels         = null;
    private float                depthRangeInMeters = 100;
    private float                depthTop           = 0.0f;
    private float                depthBot           = 100.0f;
    private DateTime             domainScrollBarStartTime  = DateTime.Now;
    private int[,]               fieldIdxs          = new int[2, InstrumentData.NumOfFields ()];
    private InstrumentDataList   history            = null;
    private InstrumentData       lastDataRow        = new InstrumentData ();
    private int                  maxDataFields      = 13;
    private float[]              minValues          = null;
    private float[]              maxValues          = null;
    private bool[]               plotData           = null;
    private int                  plottingAxisIdx    = 0;
    private CommandWindow        motherWindow       = null;
    private int                  numOfRulers        = 6;
    private SipperConfigRec      sipperConfigRec    = null;
    private Ruler[]              rulers             = null;
    private bool                 setSipperBatteryPack_RequestedBatteryEnabled = false; 
    private int                  ticsSinceLastDisplay = 0;
    private int                  timeRangeInSecs = 300;  // 5 Minutes
    private DateTime             timeTop        = DateTime.Now;
    private TextBox              depthTextBox;
    private Label                DepthLabel;
    private DateTime             timeBot        = DateTime.Now;
    private string               dataWindowConfigFileName = "";

    private int lastHeight = 0;
    private  int                 lastWidth       = 0;


		public  DepthPlot (CommandWindow       _motherWindow,
		                   SipperConfigRec     _sipperConfigRec,
                       InstrumentDataList  _history
                      )
		{
      OSservices.CreateDirectoryPath (PicesSipperVariables.PicesConfigurationDirectory ());
      dataWindowConfigFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "DepthPlotConfig.txt";

			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//

      motherWindow    = _motherWindow;
      sipperConfigRec = _sipperConfigRec;
      
      history = _history;
      
      {
        // Update Field Indexes.
        fieldIdxs = new int[2, InstrumentData.NumOfFields ()];
        int x;
        for (x = 0;  x < InstrumentData.NumOfFields();  x++)
        {
          fieldIdxs[0, x] = x;
          fieldIdxs[1, x] = x;
        }
      }
      
      {
        // Initialize min and max Values
        int  x = 0;
        DateTime minDate = new DateTime (1999, 1, 1);
        DateTime maxDate = new DateTime (9999,12,31);
        history.CalcMinMaxValues (minDate, maxDate);
        minValues = new float[history.MinValues ().Length];
        maxValues = new float[history.MinValues ().Length];
        plotData  = new bool [history.MinValues ().Length];
        for  (x = 0;  x < minValues.Length;  x++)
        {
          minValues[x] = (history.MinValues ())[x];
          maxValues[x] = (history.MaxValues ())[x];
          plotData [x] = true;
        }
      }

      {
        // Create dataLabels  &  dataFields
        dataLabels = new ComboBox [maxDataFields];
        dataFields = new TextBox  [maxDataFields];

        dataLabels [0] = data0Label;
        dataLabels [1] = data1Label;
        dataLabels [2] = data2Label;
        dataLabels [3] = data3Label;
        dataLabels [4] = data4Label;
        dataLabels [5] = data5Label;
        dataLabels [6] = data6Label;
        dataLabels [7] = data7Label;
        dataLabels [8] = data8Label;
        dataLabels [9] = data9Label;
        dataLabels[10] = data10Label;
        dataLabels[11] = data11Label;
        dataLabels[12] = data12Label;

        dataFields[0] = data0;
        dataFields[1] = data1;
        dataFields[2] = data2;
        dataFields[3] = data3;
        dataFields[4] = data4;
        dataFields[5] = data5;
        dataFields[6] = data6;
        dataFields[7] = data7;
        dataFields[8] = data8;
        dataFields[9] = data9;
        dataFields[10] = data10;
        dataFields[11] = data11;
        dataFields[12] = data12;

        int x, y;
        for (x = 0; x < maxDataFields; x++)
        {
          dataFields[x].ForeColor = Color.Black;
   
          dataLabels[x].Items.Clear ();
          for  (y = 0;  y < InstrumentData.NumDataFields ();  y++)
            dataLabels[x].Items.Add (InstrumentData.FieldName (y));

          dataLabels[x].SelectedIndex = fieldIdxs[plottingAxisIdx, x];
        }
      }
      
      
      {
        // Create Rulers array
        rulers = new Ruler[numOfRulers];
        rulers[0] = data0Ruler;
        rulers[1] = data1Ruler;
        rulers[2] = data2Ruler;
        rulers[3] = data3Ruler;
        rulers[4] = data4Ruler;
        rulers[5] = data5Ruler;
      }

      chart = new DepthPlotParameters (ChartPanel, rulers, domainScrollBar, fieldIdxs, history);

      chart.UpdateMinMaxValues (minValues, maxValues);

      chart.AssignSeriesToPlot (plotData);
      //PopulateSeriesColors ();

      SetChartRanges ();
      UpdatePlotingAxis ();
      PopulateDataFieldLabels ();

      lastHeight = Height;
      lastWidth  = Width;

    }  /* DataWindow */



		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
      SaveConfiguration ();

			if  (disposing)
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
      this.components = new System.ComponentModel.Container ();
      this.ChartPanel = new System.Windows.Forms.Panel ();
      this.domainScrollBar = new System.Windows.Forms.VScrollBar ();
      this.fetchDataTimer = new System.Windows.Forms.Timer (this.components);
      this.data1 = new System.Windows.Forms.TextBox ();
      this.data2 = new System.Windows.Forms.TextBox ();
      this.data3 = new System.Windows.Forms.TextBox ();
      this.data4 = new System.Windows.Forms.TextBox ();
      this.data5 = new System.Windows.Forms.TextBox ();
      this.data6 = new System.Windows.Forms.TextBox ();
      this.data7 = new System.Windows.Forms.TextBox ();
      this.data8 = new System.Windows.Forms.TextBox ();
      this.data9 = new System.Windows.Forms.TextBox ();
      this.data10 = new System.Windows.Forms.TextBox ();
      this.data0 = new System.Windows.Forms.TextBox ();
      this.data11 = new System.Windows.Forms.TextBox ();
      this.data12 = new System.Windows.Forms.TextBox ();
      this.minLabel = new System.Windows.Forms.Label ();
      this.maxLabel = new System.Windows.Forms.Label ();
      this.data0Label = new System.Windows.Forms.ComboBox ();
      this.data1Label = new System.Windows.Forms.ComboBox ();
      this.data2Label = new System.Windows.Forms.ComboBox ();
      this.data3Label = new System.Windows.Forms.ComboBox ();
      this.data11Label = new System.Windows.Forms.ComboBox ();
      this.data10Label = new System.Windows.Forms.ComboBox ();
      this.data9Label = new System.Windows.Forms.ComboBox ();
      this.data8Label = new System.Windows.Forms.ComboBox ();
      this.data7Label = new System.Windows.Forms.ComboBox ();
      this.data6Label = new System.Windows.Forms.ComboBox ();
      this.data5Label = new System.Windows.Forms.ComboBox ();
      this.data4Label = new System.Windows.Forms.ComboBox ();
      this.data12Label = new System.Windows.Forms.ComboBox ();
      this.depthTextBox = new System.Windows.Forms.TextBox ();
      this.DepthLabel = new System.Windows.Forms.Label ();
      this.plottingAxis = new ChartControls.RadioButtonList ();
      this.timeRange = new ChartControls.RadioButtonList ();
      this.depthRange = new ChartControls.RadioButtonList ();
      this.sipperBatteryPack = new ChartControls.BatteryPack ();
      this.rollMeter = new ChartControls.InclinationPanel ();
      this.pitchMeter = new ChartControls.InclinationPanel ();
      this.data5Ruler = new ChartControls.Ruler ();
      this.data4Ruler = new ChartControls.Ruler ();
      this.data3Ruler = new ChartControls.Ruler ();
      this.data2Ruler = new ChartControls.Ruler ();
      this.data1Ruler = new ChartControls.Ruler ();
      this.data0Ruler = new ChartControls.Ruler ();
      this.SuspendLayout ();
      // 
      // ChartPanel
      // 
      this.ChartPanel.BackColor = System.Drawing.Color.Silver;
      this.ChartPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.ChartPanel.Location = new System.Drawing.Point (291, 270);
      this.ChartPanel.Name = "ChartPanel";
      this.ChartPanel.Size = new System.Drawing.Size (620, 467);
      this.ChartPanel.TabIndex = 0;
      this.ChartPanel.Paint += new System.Windows.Forms.PaintEventHandler (this.ChartPanel_Paint);
      // 
      // domainScrollBar
      // 
      this.domainScrollBar.Location = new System.Drawing.Point (911, 270);
      this.domainScrollBar.Maximum = 1000;
      this.domainScrollBar.Name = "domainScrollBar";
      this.domainScrollBar.Size = new System.Drawing.Size (25, 469);
      this.domainScrollBar.TabIndex = 60;
      this.domainScrollBar.Value = 100;
      this.domainScrollBar.ValueChanged += new System.EventHandler (this.domainScrollBar_ValueChanged);
      // 
      // fetchDataTimer
      // 
      this.fetchDataTimer.Enabled = true;
      this.fetchDataTimer.Tick += new System.EventHandler (this.fetchDataTimer_Tick);
      // 
      // data1
      // 
      this.data1.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data1.Location = new System.Drawing.Point (187, 70);
      this.data1.Name = "data1";
      this.data1.ReadOnly = true;
      this.data1.Size = new System.Drawing.Size (100, 26);
      this.data1.TabIndex = 15;
      this.data1.Text = "0.00";
      // 
      // data2
      // 
      this.data2.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data2.Location = new System.Drawing.Point (186, 110);
      this.data2.Name = "data2";
      this.data2.ReadOnly = true;
      this.data2.Size = new System.Drawing.Size (100, 26);
      this.data2.TabIndex = 17;
      this.data2.Text = "0.00";
      // 
      // data3
      // 
      this.data3.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data3.Location = new System.Drawing.Point (186, 150);
      this.data3.Name = "data3";
      this.data3.ReadOnly = true;
      this.data3.Size = new System.Drawing.Size (100, 26);
      this.data3.TabIndex = 22;
      this.data3.Text = "0.00";
      // 
      // data4
      // 
      this.data4.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data4.Location = new System.Drawing.Point (184, 190);
      this.data4.Name = "data4";
      this.data4.ReadOnly = true;
      this.data4.Size = new System.Drawing.Size (100, 26);
      this.data4.TabIndex = 18;
      this.data4.Text = "0.00";
      // 
      // data5
      // 
      this.data5.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data5.Location = new System.Drawing.Point (186, 230);
      this.data5.Name = "data5";
      this.data5.ReadOnly = true;
      this.data5.Size = new System.Drawing.Size (100, 26);
      this.data5.TabIndex = 23;
      this.data5.Text = "0.00";
      // 
      // data6
      // 
      this.data6.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data6.Location = new System.Drawing.Point (186, 270);
      this.data6.Name = "data6";
      this.data6.ReadOnly = true;
      this.data6.Size = new System.Drawing.Size (100, 26);
      this.data6.TabIndex = 24;
      this.data6.Text = "0.00";
      // 
      // data7
      // 
      this.data7.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data7.Location = new System.Drawing.Point (184, 310);
      this.data7.Name = "data7";
      this.data7.ReadOnly = true;
      this.data7.Size = new System.Drawing.Size (100, 26);
      this.data7.TabIndex = 29;
      this.data7.Text = "0.00";
      // 
      // data8
      // 
      this.data8.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data8.Location = new System.Drawing.Point (184, 350);
      this.data8.Name = "data8";
      this.data8.ReadOnly = true;
      this.data8.Size = new System.Drawing.Size (100, 26);
      this.data8.TabIndex = 30;
      this.data8.Text = "0.00";
      // 
      // data9
      // 
      this.data9.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data9.Location = new System.Drawing.Point (184, 390);
      this.data9.Name = "data9";
      this.data9.ReadOnly = true;
      this.data9.Size = new System.Drawing.Size (100, 26);
      this.data9.TabIndex = 31;
      this.data9.Text = "0.00";
      // 
      // data10
      // 
      this.data10.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data10.Location = new System.Drawing.Point (184, 430);
      this.data10.Name = "data10";
      this.data10.ReadOnly = true;
      this.data10.Size = new System.Drawing.Size (100, 26);
      this.data10.TabIndex = 32;
      this.data10.Text = "0.00";
      // 
      // data0
      // 
      this.data0.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data0.ForeColor = System.Drawing.Color.Black;
      this.data0.Location = new System.Drawing.Point (186, 30);
      this.data0.Name = "data0";
      this.data0.ReadOnly = true;
      this.data0.Size = new System.Drawing.Size (100, 26);
      this.data0.TabIndex = 34;
      this.data0.Text = "0.00";
      // 
      // data11
      // 
      this.data11.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data11.Location = new System.Drawing.Point (184, 470);
      this.data11.Name = "data11";
      this.data11.ReadOnly = true;
      this.data11.Size = new System.Drawing.Size (100, 26);
      this.data11.TabIndex = 46;
      this.data11.Text = "0.00";
      // 
      // data12
      // 
      this.data12.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data12.Location = new System.Drawing.Point (184, 510);
      this.data12.Name = "data12";
      this.data12.ReadOnly = true;
      this.data12.Size = new System.Drawing.Size (100, 26);
      this.data12.TabIndex = 50;
      this.data12.Text = "0.00";
      // 
      // minLabel
      // 
      this.minLabel.AutoSize = true;
      this.minLabel.Location = new System.Drawing.Point (320, 15);
      this.minLabel.Name = "minLabel";
      this.minLabel.Size = new System.Drawing.Size (44, 12);
      this.minLabel.TabIndex = 85;
      this.minLabel.Text = "Minimum";
      this.minLabel.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // maxLabel
      // 
      this.maxLabel.AutoSize = true;
      this.maxLabel.Location = new System.Drawing.Point (843, 15);
      this.maxLabel.Name = "maxLabel";
      this.maxLabel.Size = new System.Drawing.Size (47, 12);
      this.maxLabel.TabIndex = 86;
      this.maxLabel.Text = "Maximum";
      this.maxLabel.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // data0Label
      // 
      this.data0Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data0Label.FormattingEnabled = true;
      this.data0Label.Location = new System.Drawing.Point (30, 30);
      this.data0Label.Name = "data0Label";
      this.data0Label.Size = new System.Drawing.Size (151, 26);
      this.data0Label.TabIndex = 87;
      this.data0Label.SelectionChangeCommitted += new System.EventHandler (this.data0Label_SelectionChangeCommitted);
      // 
      // data1Label
      // 
      this.data1Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data1Label.FormattingEnabled = true;
      this.data1Label.Location = new System.Drawing.Point (30, 70);
      this.data1Label.Name = "data1Label";
      this.data1Label.Size = new System.Drawing.Size (151, 26);
      this.data1Label.TabIndex = 88;
      this.data1Label.SelectionChangeCommitted += new System.EventHandler (this.data1Label_SelectionChangeCommitted);
      // 
      // data2Label
      // 
      this.data2Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data2Label.FormattingEnabled = true;
      this.data2Label.Location = new System.Drawing.Point (30, 110);
      this.data2Label.Name = "data2Label";
      this.data2Label.Size = new System.Drawing.Size (151, 26);
      this.data2Label.TabIndex = 89;
      this.data2Label.SelectionChangeCommitted += new System.EventHandler (this.data2Label_SelectionChangeCommitted);
      // 
      // data3Label
      // 
      this.data3Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data3Label.FormattingEnabled = true;
      this.data3Label.Location = new System.Drawing.Point (30, 150);
      this.data3Label.Name = "data3Label";
      this.data3Label.Size = new System.Drawing.Size (151, 26);
      this.data3Label.TabIndex = 90;
      this.data3Label.SelectionChangeCommitted += new System.EventHandler (this.data3Label_SelectionChangeCommitted);
      // 
      // data11Label
      // 
      this.data11Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data11Label.FormattingEnabled = true;
      this.data11Label.Location = new System.Drawing.Point (30, 470);
      this.data11Label.Name = "data11Label";
      this.data11Label.Size = new System.Drawing.Size (149, 26);
      this.data11Label.TabIndex = 91;
      this.data11Label.SelectionChangeCommitted += new System.EventHandler (this.data11Label_SelectionChangeCommitted);
      // 
      // data10Label
      // 
      this.data10Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data10Label.FormattingEnabled = true;
      this.data10Label.Location = new System.Drawing.Point (30, 430);
      this.data10Label.Name = "data10Label";
      this.data10Label.Size = new System.Drawing.Size (149, 26);
      this.data10Label.TabIndex = 92;
      this.data10Label.SelectionChangeCommitted += new System.EventHandler (this.data10Label_SelectionChangeCommitted);
      // 
      // data9Label
      // 
      this.data9Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data9Label.FormattingEnabled = true;
      this.data9Label.Location = new System.Drawing.Point (30, 390);
      this.data9Label.Name = "data9Label";
      this.data9Label.Size = new System.Drawing.Size (151, 26);
      this.data9Label.TabIndex = 93;
      this.data9Label.SelectionChangeCommitted += new System.EventHandler (this.data9Label_SelectionChangeCommitted);
      // 
      // data8Label
      // 
      this.data8Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data8Label.FormattingEnabled = true;
      this.data8Label.Location = new System.Drawing.Point (30, 350);
      this.data8Label.Name = "data8Label";
      this.data8Label.Size = new System.Drawing.Size (151, 26);
      this.data8Label.TabIndex = 94;
      this.data8Label.SelectionChangeCommitted += new System.EventHandler (this.data8Label_SelectionChangeCommitted);
      // 
      // data7Label
      // 
      this.data7Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data7Label.FormattingEnabled = true;
      this.data7Label.Location = new System.Drawing.Point (30, 310);
      this.data7Label.Name = "data7Label";
      this.data7Label.Size = new System.Drawing.Size (151, 26);
      this.data7Label.TabIndex = 95;
      this.data7Label.SelectionChangeCommitted += new System.EventHandler (this.data7Label_SelectionChangeCommitted);
      // 
      // data6Label
      // 
      this.data6Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data6Label.FormattingEnabled = true;
      this.data6Label.Location = new System.Drawing.Point (30, 270);
      this.data6Label.Name = "data6Label";
      this.data6Label.Size = new System.Drawing.Size (151, 26);
      this.data6Label.TabIndex = 96;
      this.data6Label.SelectionChangeCommitted += new System.EventHandler (this.data6Label_SelectionChangeCommitted);
      // 
      // data5Label
      // 
      this.data5Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data5Label.FormattingEnabled = true;
      this.data5Label.Location = new System.Drawing.Point (30, 230);
      this.data5Label.Name = "data5Label";
      this.data5Label.Size = new System.Drawing.Size (151, 26);
      this.data5Label.TabIndex = 97;
      this.data5Label.SelectionChangeCommitted += new System.EventHandler (this.data5Label_SelectionChangeCommitted);
      // 
      // data4Label
      // 
      this.data4Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data4Label.FormattingEnabled = true;
      this.data4Label.Location = new System.Drawing.Point (30, 190);
      this.data4Label.Name = "data4Label";
      this.data4Label.Size = new System.Drawing.Size (151, 26);
      this.data4Label.TabIndex = 98;
      this.data4Label.SelectionChangeCommitted += new System.EventHandler (this.data4Label_SelectionChangeCommitted);
      // 
      // data12Label
      // 
      this.data12Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.data12Label.FormattingEnabled = true;
      this.data12Label.Location = new System.Drawing.Point (30, 510);
      this.data12Label.Name = "data12Label";
      this.data12Label.Size = new System.Drawing.Size (149, 26);
      this.data12Label.TabIndex = 99;
      this.data12Label.SelectionChangeCommitted += new System.EventHandler (this.data12Label_SelectionChangeCommitted);
      // 
      // depthTextBox
      // 
      this.depthTextBox.Font = new System.Drawing.Font ("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.depthTextBox.Location = new System.Drawing.Point (945, 350);
      this.depthTextBox.Name = "depthTextBox";
      this.depthTextBox.Size = new System.Drawing.Size (160, 24);
      this.depthTextBox.TabIndex = 100;
      this.depthTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // DepthLabel
      // 
      this.DepthLabel.AutoSize = true;
      this.DepthLabel.Font = new System.Drawing.Font ("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DepthLabel.Location = new System.Drawing.Point (995, 327);
      this.DepthLabel.Name = "DepthLabel";
      this.DepthLabel.Size = new System.Drawing.Size (50, 18);
      this.DepthLabel.TabIndex = 101;
      this.DepthLabel.Text = "Depth";
      this.DepthLabel.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
      // 
      // plottingAxis
      // 
      this.plottingAxis.Location = new System.Drawing.Point (985, 230);
      this.plottingAxis.Margin = new System.Windows.Forms.Padding (4);
      this.plottingAxis.Name = "plottingAxis";
      this.plottingAxis.NumColumns = 1;
      this.plottingAxis.Options = new string[] {
        "Depth",
        "Time"};
      this.plottingAxis.OptionsFont = new System.Drawing.Font ("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.plottingAxis.SelectedOption = "Depth";
      this.plottingAxis.Size = new System.Drawing.Size (73, 68);
      this.plottingAxis.TabIndex = 84;
      this.plottingAxis.Title = "Plotting Axis";
      this.plottingAxis.SelectedOptionChanged += new ChartControls.RadioButtonList.SelectedOptionChangedHandler (this.plottingAxis_SelectedOptionChanged);
      // 
      // timeRange
      // 
      this.timeRange.Location = new System.Drawing.Point (932, 30);
      this.timeRange.Margin = new System.Windows.Forms.Padding (4);
      this.timeRange.Name = "timeRange";
      this.timeRange.NumColumns = 1;
      this.timeRange.Options = new string[] {
        "1 Min",
        "5 Mins",
        "10 Mins",
        "20 Mins",
        "30 Mins",
        "1 Hour",
        "All"};
      this.timeRange.OptionsFont = new System.Drawing.Font ("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.timeRange.SelectedOption = "5 Mins";
      this.timeRange.SelectedOptionInt = 1;
      this.timeRange.Size = new System.Drawing.Size (82, 160);
      this.timeRange.TabIndex = 83;
      this.timeRange.Title = "Time Range";
      this.timeRange.SelectedOptionChanged += new ChartControls.RadioButtonList.SelectedOptionChangedHandler (this.timeRange_SelectedOptionChanged);
      // 
      // depthRange
      // 
      this.depthRange.Location = new System.Drawing.Point (1020, 30);
      this.depthRange.Margin = new System.Windows.Forms.Padding (4);
      this.depthRange.Name = "depthRange";
      this.depthRange.NumColumns = 1;
      this.depthRange.Options = new string[] {
        "10 Meters",
        "20 Meters",
        "30 Meters",
        "50 Meters",
        "100 Meters",
        "200 Meters",
        "300 Meters"};
      this.depthRange.OptionsFont = new System.Drawing.Font ("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.depthRange.SelectedOption = "200 Meters";
      this.depthRange.SelectedOptionInt = 5;
      this.depthRange.Size = new System.Drawing.Size (88, 159);
      this.depthRange.TabIndex = 82;
      this.depthRange.Title = "Depth Range";
      this.depthRange.SelectedOptionChanged += new ChartControls.RadioButtonList.SelectedOptionChangedHandler (this.depthRange_SelectedOptionChanged);
      // 
      // sipperBatteryPack
      // 
      this.sipperBatteryPack.Battery1Voltage = 12F;
      this.sipperBatteryPack.Battery2Voltage = 12F;
      this.sipperBatteryPack.Battery3Voltage = 12F;
      this.sipperBatteryPack.Battery4Voltage = 12F;
      this.sipperBatteryPack.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.sipperBatteryPack.ColorEmpty = System.Drawing.Color.Red;
      this.sipperBatteryPack.ColorFull = System.Drawing.Color.Green;
      this.sipperBatteryPack.ColorNearEmpty = System.Drawing.Color.Orange;
      this.sipperBatteryPack.Location = new System.Drawing.Point (30, 587);
      this.sipperBatteryPack.Margin = new System.Windows.Forms.Padding (4);
      this.sipperBatteryPack.MaxVoltage = 26F;
      this.sipperBatteryPack.Name = "sipperBatteryPack";
      this.sipperBatteryPack.RequestedBattery = -1;
      this.sipperBatteryPack.RequestedBatteryEnabled = false;
      this.sipperBatteryPack.Size = new System.Drawing.Size (254, 150);
      this.sipperBatteryPack.TabIndex = 81;
      this.sipperBatteryPack.VoltageFont = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.sipperBatteryPack.BatteryClicked += new ChartControls.BatteryPack.BatteryClickedHandler (this.sipperBatteryPack_BatteryClicked);
      // 
      // rollMeter
      // 
      this.rollMeter.Inclination = 18F;
      this.rollMeter.LineColor = System.Drawing.Color.Blue;
      this.rollMeter.Location = new System.Drawing.Point (945, 580);
      this.rollMeter.Margin = new System.Windows.Forms.Padding (4);
      this.rollMeter.Name = "rollMeter";
      this.rollMeter.Size = new System.Drawing.Size (160, 157);
      this.rollMeter.TabIndex = 80;
      this.rollMeter.Title = "Roll";
      // 
      // pitchMeter
      // 
      this.pitchMeter.Inclination = -11F;
      this.pitchMeter.LineColor = System.Drawing.Color.Blue;
      this.pitchMeter.Location = new System.Drawing.Point (945, 397);
      this.pitchMeter.Margin = new System.Windows.Forms.Padding (4);
      this.pitchMeter.Name = "pitchMeter";
      this.pitchMeter.Size = new System.Drawing.Size (160, 161);
      this.pitchMeter.TabIndex = 79;
      // 
      // data5Ruler
      // 
      this.data5Ruler.BackColor = System.Drawing.Color.Silver;
      this.data5Ruler.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.data5Ruler.LineColor = System.Drawing.Color.Magenta;
      this.data5Ruler.Location = new System.Drawing.Point (291, 230);
      this.data5Ruler.Margin = new System.Windows.Forms.Padding (4);
      this.data5Ruler.MaxValue = 100F;
      this.data5Ruler.MinValue = 0F;
      this.data5Ruler.Name = "data5Ruler";
      this.data5Ruler.NumTics = 3;
      this.data5Ruler.Plot = true;
      this.data5Ruler.Size = new System.Drawing.Size (620, 40);
      this.data5Ruler.TabIndex = 78;
      this.data5Ruler.MinMaxChanged += new ChartControls.Ruler.MinMaxChangedHandler (this.data5Ruler_MinMaxChanged);
      this.data5Ruler.PlotChanged += new ChartControls.Ruler.PlotChangedHandler (this.data5Ruler_PlotChanged);
      // 
      // data4Ruler
      // 
      this.data4Ruler.BackColor = System.Drawing.Color.Silver;
      this.data4Ruler.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.data4Ruler.LineColor = System.Drawing.Color.Yellow;
      this.data4Ruler.Location = new System.Drawing.Point (291, 190);
      this.data4Ruler.Margin = new System.Windows.Forms.Padding (4);
      this.data4Ruler.MaxValue = 100F;
      this.data4Ruler.MinValue = 0F;
      this.data4Ruler.Name = "data4Ruler";
      this.data4Ruler.NumTics = 3;
      this.data4Ruler.Plot = true;
      this.data4Ruler.Size = new System.Drawing.Size (620, 40);
      this.data4Ruler.TabIndex = 77;
      this.data4Ruler.MinMaxChanged += new ChartControls.Ruler.MinMaxChangedHandler (this.data4Ruler_MinMaxChanged);
      this.data4Ruler.PlotChanged += new ChartControls.Ruler.PlotChangedHandler (this.data4Ruler_PlotChanged);
      // 
      // data3Ruler
      // 
      this.data3Ruler.BackColor = System.Drawing.Color.Silver;
      this.data3Ruler.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.data3Ruler.LineColor = System.Drawing.Color.Blue;
      this.data3Ruler.Location = new System.Drawing.Point (291, 150);
      this.data3Ruler.Margin = new System.Windows.Forms.Padding (4);
      this.data3Ruler.MaxValue = 100F;
      this.data3Ruler.MinValue = 0F;
      this.data3Ruler.Name = "data3Ruler";
      this.data3Ruler.NumTics = 3;
      this.data3Ruler.Plot = true;
      this.data3Ruler.Size = new System.Drawing.Size (620, 40);
      this.data3Ruler.TabIndex = 76;
      this.data3Ruler.MinMaxChanged += new ChartControls.Ruler.MinMaxChangedHandler (this.data3Ruler_MinMaxChanged);
      this.data3Ruler.PlotChanged += new ChartControls.Ruler.PlotChangedHandler (this.data3Ruler_PlotChanged);
      // 
      // data2Ruler
      // 
      this.data2Ruler.BackColor = System.Drawing.Color.Silver;
      this.data2Ruler.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.data2Ruler.LineColor = System.Drawing.Color.Red;
      this.data2Ruler.Location = new System.Drawing.Point (291, 110);
      this.data2Ruler.Margin = new System.Windows.Forms.Padding (4);
      this.data2Ruler.MaxValue = 100F;
      this.data2Ruler.MinValue = 0F;
      this.data2Ruler.Name = "data2Ruler";
      this.data2Ruler.NumTics = 3;
      this.data2Ruler.Plot = true;
      this.data2Ruler.Size = new System.Drawing.Size (620, 40);
      this.data2Ruler.TabIndex = 75;
      this.data2Ruler.MinMaxChanged += new ChartControls.Ruler.MinMaxChangedHandler (this.data2Ruler_MinMaxChanged);
      this.data2Ruler.PlotChanged += new ChartControls.Ruler.PlotChangedHandler (this.data2Ruler_PlotChanged);
      // 
      // data1Ruler
      // 
      this.data1Ruler.BackColor = System.Drawing.Color.Silver;
      this.data1Ruler.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.data1Ruler.LineColor = System.Drawing.Color.White;
      this.data1Ruler.Location = new System.Drawing.Point (291, 70);
      this.data1Ruler.Margin = new System.Windows.Forms.Padding (4);
      this.data1Ruler.MaxValue = 100F;
      this.data1Ruler.MinValue = 0F;
      this.data1Ruler.Name = "data1Ruler";
      this.data1Ruler.NumTics = 3;
      this.data1Ruler.Plot = true;
      this.data1Ruler.Size = new System.Drawing.Size (620, 40);
      this.data1Ruler.TabIndex = 74;
      this.data1Ruler.MinMaxChanged += new ChartControls.Ruler.MinMaxChangedHandler (this.data1Ruler_MinMaxChanged);
      this.data1Ruler.PlotChanged += new ChartControls.Ruler.PlotChangedHandler (this.data1Ruler_PlotChanged);
      // 
      // data0Ruler
      // 
      this.data0Ruler.BackColor = System.Drawing.Color.Silver;
      this.data0Ruler.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.data0Ruler.LineColor = System.Drawing.Color.Green;
      this.data0Ruler.Location = new System.Drawing.Point (291, 31);
      this.data0Ruler.Margin = new System.Windows.Forms.Padding (4);
      this.data0Ruler.MaxValue = 100F;
      this.data0Ruler.MinValue = 0F;
      this.data0Ruler.Name = "data0Ruler";
      this.data0Ruler.NumTics = 3;
      this.data0Ruler.Plot = true;
      this.data0Ruler.Size = new System.Drawing.Size (620, 41);
      this.data0Ruler.TabIndex = 73;
      this.data0Ruler.MinMaxChanged += new ChartControls.Ruler.MinMaxChangedHandler (this.data0Ruler_MinMaxChanged);
      this.data0Ruler.PlotChanged += new ChartControls.Ruler.PlotChangedHandler (this.data0Ruler_PlotChanged);
      // 
      // DepthPlot
      // 
      this.AutoScaleBaseSize = new System.Drawing.Size (4, 11);
      this.BackColor = System.Drawing.Color.Gainsboro;
      this.ClientSize = new System.Drawing.Size (1126, 752);
      this.Controls.Add (this.DepthLabel);
      this.Controls.Add (this.depthTextBox);
      this.Controls.Add (this.data12Label);
      this.Controls.Add (this.data4Label);
      this.Controls.Add (this.data5Label);
      this.Controls.Add (this.data6Label);
      this.Controls.Add (this.data7Label);
      this.Controls.Add (this.data8Label);
      this.Controls.Add (this.data9Label);
      this.Controls.Add (this.data10Label);
      this.Controls.Add (this.data11Label);
      this.Controls.Add (this.data3Label);
      this.Controls.Add (this.data2Label);
      this.Controls.Add (this.data1Label);
      this.Controls.Add (this.data0Label);
      this.Controls.Add (this.domainScrollBar);
      this.Controls.Add (this.maxLabel);
      this.Controls.Add (this.minLabel);
      this.Controls.Add (this.plottingAxis);
      this.Controls.Add (this.timeRange);
      this.Controls.Add (this.depthRange);
      this.Controls.Add (this.sipperBatteryPack);
      this.Controls.Add (this.rollMeter);
      this.Controls.Add (this.pitchMeter);
      this.Controls.Add (this.data5Ruler);
      this.Controls.Add (this.data4Ruler);
      this.Controls.Add (this.data3Ruler);
      this.Controls.Add (this.data2Ruler);
      this.Controls.Add (this.data1Ruler);
      this.Controls.Add (this.data0Ruler);
      this.Controls.Add (this.data12);
      this.Controls.Add (this.data11);
      this.Controls.Add (this.data0);
      this.Controls.Add (this.data10);
      this.Controls.Add (this.data9);
      this.Controls.Add (this.data8);
      this.Controls.Add (this.data7);
      this.Controls.Add (this.data6);
      this.Controls.Add (this.data5);
      this.Controls.Add (this.data4);
      this.Controls.Add (this.data3);
      this.Controls.Add (this.data2);
      this.Controls.Add (this.data1);
      this.Controls.Add (this.ChartPanel);
      this.Font = new System.Drawing.Font ("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MinimumSize = new System.Drawing.Size (1140, 790);
      this.Name = "DepthPlot";
      this.Text = "DepthPlot";
      this.Load += new System.EventHandler (this.DataWindow_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.DataWindow_FormClosing);
      this.Resize += new System.EventHandler (this.DepthPlot_Resize);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }
		#endregion






    private  void  PopulateDataFieldLabels ()
    {
      int  x;

      pitchMeter.LineColor = Color.Black;
      rollMeter.LineColor  = Color.Black;
     
      for  (x = 0;  x < rulers.Length;  x++)
      {
        rulers[x].MinValue = minValues[fieldIdxs[plottingAxisIdx, x]];
        rulers[x].MaxValue = maxValues[fieldIdxs[plottingAxisIdx, x]];
        rulers[x].Plot     = plotData [fieldIdxs[plottingAxisIdx, x]];
        rulers[x].NumTics  = 5;

        if  (fieldIdxs[plottingAxisIdx, x] == InstrumentData.PitchIndex)
          pitchMeter.LineColor = rulers[x].LineColor;

        if  (fieldIdxs[plottingAxisIdx, x] == InstrumentData.RollIndex)
          rollMeter.LineColor =  rulers[x].LineColor;
      }


      for  (x = 0;  x < maxDataFields;  x++)
      {
        dataFields[x].ForeColor = Color.Black;
        dataLabels[x].SelectedIndex = fieldIdxs[plottingAxisIdx, x];
      }

    }  /* PopulateDataFieldLabels */




    private void  PopulateDataFields (InstrumentData r)
    {
      float[] data = r.Data ();

      int x;
      for  (x = 0;  (x < data.Length) && (x < maxDataFields);  x++)
        dataFields[x].Text = data[fieldIdxs[plottingAxisIdx, x]].ToString ();
      
      PopulatePitchAndRollOnly (r);
      
      depthTextBox.Text = r.Depth ().ToString ();
    }  /* PopulateDataFields */



    private void  PopulateBatteryPackFields (InstrumentData r)
    {
      r.UpdateBatteryPack (sipperBatteryPack);
    }  /* PopulateBatteryPackFields */



    private void  PopulatePitchAndRollOnly (InstrumentData r)
    {
      pitchMeter.Inclination = r.Pitch ();
      rollMeter.Inclination = r.Roll ();
    }  /* PopulatePitchAndRollOnly */
    
    
    
    
    
    private  void  ChangeDataFieldLabel (int  dataLabelIdx)
    {
      if  ((dataLabelIdx < 0)  ||  (dataLabelIdx >= InstrumentData.NumOfFields ())) 
      {
        MessageBox.Show ("'ChangeDataFieldLabel'  Invalid  dataLabelIdx[" + dataLabelIdx.ToString () + "].");
        return;
      }
      
      int  dataFieldIdx = dataLabels[dataLabelIdx].SelectedIndex;

      if  ((dataFieldIdx < 0)  ||  (dataFieldIdx >= InstrumentData.NumDataFields ()))
      {
        MessageBox.Show ("'ChangeDataFieldLabel'  Invalid  dataFieldIdx[" + dataFieldIdx.ToString () + "].");
        return;
      }
      
      
      if  (fieldIdxs[plottingAxisIdx, dataLabelIdx] == dataFieldIdx)
      {
        //  Still pointiing to same label,  there is nothing to do.
        return;
      }
      
      int  x;
      
      // First see if another dataLabel is already pointing to this data field.
      for  (x = 0;  x < InstrumentData.NumOfFields();  x++)
      {
        if  (fieldIdxs[plottingAxisIdx, x] == dataFieldIdx)
        {
          // Another label is already referencing the data label;  so we will change 
          // its assignment to the one that 'dataLabelIdx' is referencing.
          fieldIdxs[plottingAxisIdx, x] = fieldIdxs[plottingAxisIdx, dataLabelIdx];
          if  (x < dataLabels.Length)
            dataLabels[x].SelectedIndex = fieldIdxs[plottingAxisIdx, x];
          break;
        }
      }
      
      fieldIdxs[plottingAxisIdx, dataLabelIdx]  = dataFieldIdx;
      dataLabels[dataLabelIdx].SelectedIndex    = dataFieldIdx;

      PopulateDataFieldLabels ();
      PopulateDataFields (history.CurrentDataRow ());
      PaintChart ();
    }  /* ChangeDataFieldLabel */
                                      
    
    
    
    private void ChartPanel_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
    {
      PaintChart ();
    }



    private void DataWindow_Load(object sender, System.EventArgs e)
    {
      LoadConfiguration ();
      lastHeight = Height;
      lastWidth  = Width;
      Text = "Depth Plot (" + Height.ToString () + " - " + Width.ToString () + ")";
    }



    private  void  SaveConfiguration ()
    {
      StreamWriter w = null;
      try
      {
        w = new StreamWriter (dataWindowConfigFileName);
      }
      catch (Exception e)
      {
        MessageBox.Show ("Exception Writing DataWindow configuration.  [" + e.ToString() + "]");
        return;
      }
      w.WriteLine ("// DataWindow  Configuration");
      w.WriteLine ("// DateTime" + "\t" + DateTime.Now.ToString());
      w.WriteLine ("//");
      w.WriteLine ("// The following [" + InstrumentData.NumOfFields().ToString() + "] rows list the order the order data fields are displayed in.");
      
      int  x;
      for (x = 0;  x < InstrumentData.NumOfFields();  x++)
      {
        w.WriteLine ("FieldIdx" + "\t" + InstrumentData.FieldName (fieldIdxs[0, x]) + "\t" + InstrumentData.FieldName (fieldIdxs[1, x]));
      }

      w.WriteLine ("//");
      w.WriteLine ("//");
      w.WriteLine ("//");
      w.WriteLine ("// Ruler Values");
      w.WriteLine ("//");
      w.WriteLine ("//  The following rows conatin the MinMax values and plot flag specified by the user.");
      w.WriteLine ("//  Each line consists 5 fields ['MinMax' '\t'  <FieldName>  '\t'  <MinValue>  '\t'  <MaxValue> '\t' <True/False>");
      w.WriteLine ("//");
      
      for  (x = 0;  x < minValues.Length;  x++)
      {
        w.WriteLine ("MinMax"                    + "\t" +
                     InstrumentData.FieldName(x) + "\t" + 
                     minValues[x].ToString ()    + "\t" + 
                     maxValues[x].ToString ()    + "\t" +
                     plotData [x].ToString ()
                    );
      }


      w.WriteLine ("plottingaxis" + "\t" + plottingAxis.SelectedOption);

      w.Close();
      
      return;
    }  /* SaveConfiguration */



    private  float  StrToFloat (string s)
    {
      float f = 0.0f;
      try  {f = float.Parse (s);}
      catch (Exception) {f = 0.0f;}
      return  f;    
    }
    

    void  LoadConfiguration ()
    {
      StreamReader f = null;

      try
      {
        f = new StreamReader (dataWindowConfigFileName);
      }
      catch (Exception)
      {
        return;
      }

      int[,]   newFieldIdxs = new int  [2, InstrumentData.NumOfFields ()];
      bool[,]  fieldsFound  = new bool [2, InstrumentData.NumOfFields ()];
      int  z;

      for  (z = 0;  z < InstrumentData.NumOfFields ();  z++)
      {
        newFieldIdxs[0, z] = 0;
        newFieldIdxs[1, z] = 0;
        fieldsFound [0, z]  = false;
        fieldsFound [1, z]  = false;
      }

      int  numFieldsFound0 = 0;
      int  numFieldsFound1 = 0;


      string txtLine = f.ReadLine ();
      while (txtLine != null)
      {
        if (txtLine.Substring (0, 2) != "//")
        {
          String[]  fields = txtLine.Split ('\t');
          if  (fields.Length > 1)
          {
            string   rowType = fields[0].ToLower ().Trim ();
          
            if  (rowType== "fieldidx")
            {
              int  idx0 = InstrumentData.GetFieldNameIndex (fields[1]);
              int  idx1 = idx0;
              if  (fields.Length > 2)
                idx1 = InstrumentData.GetFieldNameIndex (fields[2]);
                
              if  (idx0 >= 0)
              {
                // Make sure we have not already seen this idx already.

                if (!fieldsFound[0, idx0])
                {
                  newFieldIdxs[0, numFieldsFound0] = idx0;
                  fieldsFound[0, idx0] = true;
                  numFieldsFound0++;
                }
              }

              if  (idx1 >= 0)
              {
                // Make sure we have not already seen this idx already.

                if (!fieldsFound[1, idx1])
                {
                  newFieldIdxs[1, numFieldsFound1] = idx1;
                  fieldsFound[1, idx1] = true;
                  numFieldsFound1++;
                }
              }
            }
            else if  (rowType == "minmax")
            {
              if  (fields.Length < 4)
                continue;

              int idx = InstrumentData.GetFieldNameIndex (fields[1]);
              if  ((idx >= 0)  &&  (idx < minValues.Length))
              {                
                minValues[idx] = StrToFloat (fields[2]);
                maxValues[idx] = StrToFloat (fields[3]);
                if  (fields.Length < 5)
                {
                  plotData [idx] = true;
                }
                else
                {
                  plotData [idx] = PicesKKStr.StrToBool  (fields[4]);
                }
              }
            }
            
            else if  (rowType == "plottingaxis")
            {
              if  (fields.Length == 2)
              {
                plottingAxisEventsAreSuspended = true;
                plottingAxis.SelectedOption = fields[1];
                UpdatePlotingAxis ();
                PopulateDataFieldLabels ();

                plottingAxisEventsAreSuspended = false;
              }
            }
          }
        }
        
        txtLine = f.ReadLine();
      }

      f.Close();
      
      // Lets account for missing fields.  This should never be able to happen unless a 
      // another entity/user modifies the configuration file.
      for  (z = 0;  z < InstrumentData.NumOfFields();  z++)
      {
        if  (!fieldsFound[0, z])
        {
          newFieldIdxs[0, numFieldsFound0] = z;
          numFieldsFound0++;
        }
      }

      for  (z = 0;  z < InstrumentData.NumOfFields();  z++)
      {
        if (!fieldsFound[1, z])
        {
          newFieldIdxs[1, numFieldsFound1] = z;
          numFieldsFound1++;
        }
      }

      fieldIdxs = newFieldIdxs;
      
      for  (z = 0;  z < dataLabels.Length;  z++)
        dataLabels[z].SelectedIndex = fieldIdxs[plottingAxisIdx, z];
      
      chart.SetFieldIdxs (fieldIdxs);
      
      PopulateDataFieldLabels ();
      SetChartRanges ();
      
      return;
    }  /* LoadConfiguration */



    private  void  PaintChart ()
    {
      chart.PaintChart ();
    }  /* PaintChart */




    private void fetchDataTimer_Tick (object sender, System.EventArgs e)
    {
      ticsSinceLastDisplay++;

      InstrumentData  currentDataRow = history.CurrentDataRow ();
      
      if  (currentDataRow == null)
      {
        // We have not collected any data yet.  We need a blank one to work with in the meen time.
        currentDataRow = new InstrumentData ();
      }

      if  (ticsSinceLastDisplay > 9)
      {
        if  (chartRealTime)
          domainScrollBarStartTime = currentDataRow.Time;

        PopulateDataFields (currentDataRow);
        PopulateBatteryPackFields (currentDataRow);
        SetChartRanges ();
          
        if  (plottingAxisIdx == 0)
        {
          // We are plotting a depth chart.
          chart.PaintCurrentRow (currentDataRow);          
        }
        else
        {
          PaintChart ();
        }
        ticsSinceLastDisplay = 0;
      }
      else
      {
        PopulatePitchAndRollOnly (currentDataRow);
      }

      if  (setSipperBatteryPack_RequestedBatteryEnabled)
      {
        sipperBatteryPack.Enabled = true;
        sipperBatteryPack.RequestedBatteryEnabled = true;
        setSipperBatteryPack_RequestedBatteryEnabled = false;
      }
    }  /* fetchDataTimer_Tick */



    private void DataWindow_FormClosing(object sender, FormClosingEventArgs e)
    {
    }


    private bool  plottingAxisEventsAreSuspended = false;



    private  void  UpdatePlotingAxis ()
    {
      switch  (plottingAxis.SelectedOption)
      {
        case  "Depth":
            chartRealTime = true;
            chart.SetPlotByDepth ();
            plottingAxisIdx = 0;
            break;

        case  "Time":  
            chart.SetPlotByTime  ();  
            plottingAxisIdx = 1;
            break;
      }
    }  /* UpdatePlotingAxis */



    private void plottingAxis_SelectedOptionChanged ()
    {
      if  (plottingAxisEventsAreSuspended)
        return;

      UpdatePlotingAxis ();
      PopulateDataFieldLabels ();

      SetChartRanges ();
      PaintChart ();
    }  /* plottingAxis_SelectedOptionChanged */



    private void  SetDepthRange ()
    {
      switch  (depthRange.SelectedOption)
      {
        case   "10 Meters": depthRangeInMeters =  10.0f; break;
        case   "20 Meters": depthRangeInMeters =  20.0f; break;
        case   "30 Meters": depthRangeInMeters =  30.0f; break;
        case   "50 Meters": depthRangeInMeters =  50.0f; break;
        case  "100 Meters": depthRangeInMeters = 100.0f; break;
        case  "200 Meters": depthRangeInMeters = 200.0f; break;
        case  "300 Meters": depthRangeInMeters = 300.0f; break;
      }  /* switch */

      SetChartRanges ();
    }  /* SetDepthRange */



    private void  SetTimeRange ()
    {
      if  (history.CurrentOrder () != InstrumentDataList.DataOrder.TimeOrder)
        history.SortByTime ();

      switch  (timeRange.SelectedOption)
      {
        case   "1 Min": timeRangeInSecs =  1 * 60; break;
        case  "5 Mins": timeRangeInSecs =  5 * 60; break;
        case "10 Mins": timeRangeInSecs = 10 * 60; break;
        case "20 Mins": timeRangeInSecs = 20 * 60; break;
        case "30 Mins": timeRangeInSecs = 30 * 60; break;
        case  "1 Hour": timeRangeInSecs = 60 * 60; break;
        case     "All": timeRangeInSecs = 60 * 60; break;
      }

      SetChartRanges ();
    }  /* SetTimeRange */




    private void  SetChartRanges ()
    {
      if  (chartRealTime)
      {
        if  (history.Count < 1)
          timeTop = DateTime.Now;
        else
          timeTop = history.TimeMax ();
        
        domainScrollBarStartTime = timeTop;
      }

      timeBot = timeTop.AddSeconds (0 - timeRangeInSecs);
      depthBot = depthTop + depthRangeInMeters;

      UpdateScrollingRanges ();

      chart.UpdateRanges (depthTop, depthBot, timeTop, timeBot);
    }  /* SetChartRanges */



    private void  UpdateScrollingRanges ()
    {
      SuspendLayout ();
      domainScrollBarEventsAreSuspended = true;

      domainScrollBar.SuspendLayout ();

      domainScrollBar.Minimum = 0;

      // Update Scroll Bar to reflect ranges
      if (chart.TimePlot ())
      {
        if  (history.Count < 1)
        {
          domainScrollBar.Minimum     = 0;
          domainScrollBar.Maximum     = timeRangeInSecs;
          domainScrollBar.LargeChange = timeRangeInSecs;
          domainScrollBar.Value       = 0;
        }
        else
        {
          TimeSpan ts = history.TimeMax () - history.TimeMin ();
          domainScrollBar.Minimum     = 0;
          domainScrollBar.Maximum     = (int)ts.TotalSeconds;
          domainScrollBar.LargeChange = timeRangeInSecs;
          ts = domainScrollBarStartTime - timeTop;
          domainScrollBar.Value       = (int)ts.TotalSeconds;
        }
      }
      else
      {
        domainScrollBar.Minimum     = 0;
        domainScrollBar.Maximum     = (int)history.DepthMax ();
        domainScrollBar.LargeChange = (int)depthRangeInMeters;

        int newValue = (int)Math.Min (depthTop, history.DepthMax ());
        if  (newValue < 0)
          newValue = 0;
        
        domainScrollBar.Value = newValue;
      }

     domainScrollBar.ResumeLayout (false);
     domainScrollBarEventsAreSuspended = false;
     domainScrollBar.Enabled = true;
     ResumeLayout (false);

     domainScrollBar.Enabled = true;

    }  /* UpdateScrollingRanges */



    private void timeRange_SelectedOptionChanged ()
    {
      SetTimeRange ();
      PaintChart ();
    }



    private void depthRange_SelectedOptionChanged ()
    {
      SetDepthRange ();
      PaintChart ();
    }



    private  void  UpdateMinMax (int  rulerIdx)
    {
      if  ((chart == null)  ||  (rulerIdx < 0)  ||  (rulerIdx >= numOfRulers))
        return;

      SuspendLayout ();
      if  (rulers[rulerIdx].MinValue > rulers[rulerIdx].MaxValue)
        rulers[rulerIdx].MaxValue = rulers[rulerIdx].MinValue;

      minValues[fieldIdxs[plottingAxisIdx, rulerIdx]] = rulers[rulerIdx].MinValue;
      maxValues[fieldIdxs[plottingAxisIdx, rulerIdx]] = rulers[rulerIdx].MaxValue;
      ResumeLayout (false);

      chart.UpdateMinMaxValues (minValues, maxValues);

      return;
    }  /* UpdateMinMax */




    private void  data0Ruler_MinMaxChanged ()
    {
      UpdateMinMax (0);
    }

    private void data1Ruler_MinMaxChanged ()
    {
      UpdateMinMax (1);
    }

    private void data2Ruler_MinMaxChanged ()
    {
      UpdateMinMax (2);
    }

    private void data3Ruler_MinMaxChanged ()
    {
      UpdateMinMax (3);
    }

    private void data4Ruler_MinMaxChanged ()
    {
      UpdateMinMax (4);
    }

    private void data5Ruler_MinMaxChanged ()
    {
      UpdateMinMax (5);
    }

    private void  UpdateSeriesToPlot ()
    {
      chart.AssignSeriesToPlot  (plotData);
    }


    private void data0Ruler_PlotChanged()
    {
      plotData[fieldIdxs[plottingAxisIdx, 0]] = rulers[0].Plot;
      UpdateSeriesToPlot ();
    }

    private void data1Ruler_PlotChanged()
    {
      plotData[fieldIdxs[plottingAxisIdx, 1]] = rulers[1].Plot;
      UpdateSeriesToPlot();
    }

    private void data2Ruler_PlotChanged()
    {
      plotData[fieldIdxs[plottingAxisIdx, 2]] = rulers[2].Plot;
      UpdateSeriesToPlot();
    }

    private void data3Ruler_PlotChanged()
    {
      plotData[fieldIdxs[plottingAxisIdx, 3]] = rulers[3].Plot;
      UpdateSeriesToPlot();
    }

    private void data4Ruler_PlotChanged()
    {
      plotData[fieldIdxs[plottingAxisIdx, 4]] = rulers[4].Plot;
      UpdateSeriesToPlot();
    }

    private void data5Ruler_PlotChanged()
    {
      plotData[fieldIdxs[plottingAxisIdx, 5]] = rulers[5].Plot;
      UpdateSeriesToPlot();
    }


    bool  domainScrollBarEventsAreSuspended = false;

    private void  domainScrollBar_ValueChanged (object sender, EventArgs e)
    {
      if  (domainScrollBarEventsAreSuspended)
        return;

      int  v = domainScrollBar.Value;

      if  (chart.TimePlot ())
      {
        if  (chartRealTime)
        {
          if  (v > 0)
            chartRealTime = false;
        }
        else
        {
          if  (v == 0)
            chartRealTime = true;
        }

        // v = Seconds since 'domainScrollBarStartTime'
        timeTop = domainScrollBarStartTime.AddSeconds (0 - v);
      }
      else
      {
//        depthTop = depthTop + (float)v;
        depthTop = (float)v;
      }

      SetChartRanges ();
      PaintChart ();
    }  /* domainScrollBar_ValueChanged */




    private void sipperBatteryPack_BatteryClicked ()
    {
      int  requestedBattery = sipperBatteryPack.RequestedBattery;
      sipperBatteryPack.Enabled = false;
      motherWindow.ChangeBattery (requestedBattery);
    }


    
    // Ths method will be called from other modules/Threads.  
    // Becuase it is being called from another thread it will not beable 
    // to access "sipperBatteryPack" directly;  it will have to set a 
    // flag that the timer loop will monitor. As a result it will be the 
    // timer loop that will actually re-enable the sipperBatteryPack
    public  void  ActivateChangeBatteryOptions ()
    {
      setSipperBatteryPack_RequestedBatteryEnabled = true;
    }
    
    

    private void data0Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (0);
    }

    private void data1Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (1);
    }

    private void data2Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (2);
    }

    private void data3Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (3);
    }

    private void data4Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (4);
    }  
    
    private void data5Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (5);
    }

    private void data6Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (6);
    }

    private void data7Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (7);
    }

    private void data8Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (8);
    }

    private void data9Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (9);
    }

    private void data10Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (10);
    }

    private void data11Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (11);
    }

    private void data12Label_SelectionChangeCommitted (object sender, EventArgs e)
    {
      ChangeDataFieldLabel (12);
    }
    


    private void DepthPlot_Resize (object sender, EventArgs e)
    {
      if  (Height < this.MinimumSize.Height)
        return;

      int  deltaHeight = Height - lastHeight;
      int  deltaWidth  = Width  - lastWidth;

      this.Text = "Depth Plot (" + Height.ToString () + " - " + Width.ToString () + ")";
 
      depthRange.Left      += deltaWidth;
      timeRange.Left       += deltaWidth;
      plottingAxis.Left    += deltaWidth;
      DepthLabel.Left      += deltaWidth;
      depthTextBox.Left    += deltaWidth;
      pitchMeter.Left      += deltaWidth;
      rollMeter.Left       += deltaWidth;
      domainScrollBar.Left += deltaWidth;
      
      for  (int x = 0;  x < numOfRulers;  x++)
      {
        rulers[x].Width += deltaWidth;
        rulers[x].Refresh ();
      }

      ChartPanel.Width += deltaWidth;
      maxLabel.Left    += deltaWidth;

      ChartPanel.Height += deltaHeight;
      ChartPanel.Refresh ();

      domainScrollBar.Height += deltaHeight;
      domainScrollBar.Refresh ();


      lastHeight = Height;
      lastWidth  = Width;
      chart.ReSize ();
    }
  }
}  /* DataWindow */

