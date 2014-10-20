using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using ChartControls;
using SipperFile;

using PicesInterface;
//using SipperFile;


namespace SipperFile
{
  public partial class TimePlot : Form
  {
    // InstrumentData support variables,  one entry for each posible entry in InstrumentData. 
    private  float[]          minValues       = null;
    private  float[]          maxValues       = null;
    private  bool []          plotDataSeries  = null;
    private  float[]          rowScaleFactors = null;  // Reflects the number units per pixel row for the respective data (minValues - maxValues)/numOfRows
                                                       // Recalcilated every time the ChartPanel is resized.
    
    // These variables support the actual data to be plotted
    private  int[]            fieldIdxs       = null;    // Specifies which InstrumentData fields are to be displayed in respective plottedSeries.
    private  int              numDataSeries   = 6;
    private  SolidBrush []    plotBrushes     = null;
    private  DataSeries []    plottedSeries   = null;

    //
    private  InstrumentDataList      history;  // the CommandWindow module will be appending new data to end
                                        // of this data structure.

    private  InstrumentData   leftMostDataRow;  // Should always point to the Data Row displayed on
                                                // left side of screen.

    //
    private  SipperConfigRec     sipperConfigRec  = null;
    
    private  TimePlotParameters  chartParms       = null;

 
    private int   lastHistoryRowDisplayed = -1;  // Indictes which history  row was last updated to the 
                                                 // plot panel.  Any rows beyont this value will need to 
                                                 // be painted to the panel.

    private  bool  liveUpdate = true;  // true  = Update display every clock tic
                                       // false = historical display.


    private int    ticCount = 0;  // Used by 'updateChartTimer_Tick' to keep track of number of times ticker was envoked.

    private DateTime  timeDomainScrollBarMin = DateTime.Now;   // Used by TimeDomainScrollBar  to keep track of what part 
    private DateTime  timeDomainScrollBarMax = DateTime.Now;   // of the time domain it is relating to.  This way we can 
                                                                 // keep track as time goes along to make sure that the
                                                                 // scrollBar acuratly reflects what data we have.

    private string    timePlotConfigFileName  = "";

    private  DateTime        selectedDateTime  = DateTime.Now;
    private  InstrumentData  selectedHistory   = null;

    public  TimePlot (SipperConfigRec      _sipperConfigRec,
                      InstrumentDataList   _history
                     )
    {
      OSservices.CreateDirectory (PicesSipperVariables.PicesConfigurationDirectory ());
      timePlotConfigFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "TimePlotConfig.txt";

      sipperConfigRec = _sipperConfigRec;
      history         = _history;
      InitializeComponent ();
      InitializeVariables ();
      LoadConfiguration ();
      
      SetToLiveMode ();
      updateChartTimer.Enabled = true;
    }


    protected override void OnClosing (CancelEventArgs e)
    {
      SaveConfiguration ();
      base.OnClosing (e);
    }


    private void  InitializeVariables ()
    {
      int x = 0;

      string[] dataLabels = new string[InstrumentData.NumDataFields()];
      for (x = 0; x < InstrumentData.NumDataFields(); x++)
        dataLabels[x] = InstrumentData.FieldName(x);

      minValues        = new float[InstrumentData.NumDataFields ()];
      maxValues        = new float[InstrumentData.NumDataFields ()];
      plotDataSeries   = new bool [InstrumentData.NumDataFields ()];
      rowScaleFactors  = new float[InstrumentData.NumDataFields ()];
      plotBrushes      = new SolidBrush[numDataSeries];
      fieldIdxs        = new int  [numDataSeries];
      
      plottedSeries = new DataSeries[numDataSeries];
      plottedSeries[0] = dataSeries0;
      plottedSeries[1] = dataSeries1;
      plottedSeries[2] = dataSeries2;
      plottedSeries[3] = dataSeries3;
      plottedSeries[4] = dataSeries4;
      plottedSeries[5] = dataSeries5;


      SuspendAllControls ();

      for  (x = 0;  x < numDataSeries;  x++)
      {
        plottedSeries[x].SetDataLabels (dataLabels);
        plottedSeries[x].SetSelectedDataLabel  (x);
        plottedSeries[x].MinMaxCalcRequest += ReCalcMinMaxForSpecificPlottedSeries;
        fieldIdxs[x] = x;
        plotBrushes[x] = new SolidBrush (plottedSeries[x].PlotColor);
      }

      chartParms = new TimePlotParameters (chartPanel, fieldIdxs, minValues, maxValues, rowScaleFactors, plotDataSeries, plotBrushes, plottedSeries);

      DateTime  startingLeftTime = DateTime.Now;
      lock (history)
      {
        if  (history.Count > 0)
          startingLeftTime = history.TimeMax ();
      }
      
      chartParms.SetNewTimeLeft (startingLeftTime);
      
      UpdateTimeDomainScrollingRanges ();
      
      ResumeAllControls ();
    }  /* InitializeVariables */



    private void  AdjustMinMaxes ()
    {
      int x, y;
      int  numHistoryRows = 0;
      lock  (history)
      {
        numHistoryRows = history.Count;
      }
      
      if  (numHistoryRows < 2)
        return;

      double[] means   = new double [InstrumentData.NumDataFields ()];
      double[] vars    = new double [InstrumentData.NumDataFields ()];
      double[] stdDevs = new double [InstrumentData.NumDataFields ()];
      for (x = 0; x < means.Length; x++)
      {
        means     [x] = 0.0;
        vars      [x] = 0.0;
        stdDevs   [x] = 0.0;
        minValues [x] = 9999.999f;
        maxValues [x] = -9999.999f;
      }

      lock  (history)
      {
        for  (y = 0;  y < numHistoryRows;  y++)
        {
          InstrumentData dr = (InstrumentData)(history[y]);
          float[] data = dr.Data();
          for (x = 0; x < means.Length; x++)
          {
            means[x] += data[x];
            if  (data[x] < minValues[x])
              minValues[x] = data[x];

            if (data[x] > maxValues[x])
              maxValues[x] = data[x];
          }
        }
      }

      for (x = 0; x < means.Length; x++)
        means[x] = means[x] / (double)numHistoryRows;

      lock  (history)
      {
        for  (y = 0;  y < numHistoryRows;  y++)
        {
          InstrumentData dr = (InstrumentData)(history[y]);
          float[] data = dr.Data();
          for (x = 0; x < means.Length; x++)
          {
            double delta = data[x] - means[x];
            double deltaSquared = delta * delta;
            vars[x] += deltaSquared;
          }
        }
      }

      for (x = 0; x < means.Length; x++)
      {
        vars[x] = vars[x] / (double)(numHistoryRows - 1);
        stdDevs[x] = Math.Sqrt(vars[x]);
      }


      for (x = 0; x < means.Length; x++)
      {
        if  (stdDevs[x] != 0.0)
        {
          float newMin = (float)(means[x] - (3.0 * stdDevs[x]));
          float newMax = (float)(means[x] + (3.0 * stdDevs[x]));
        
          if  (newMin > minValues[x])
            minValues[x] = newMin;
            
          if  (newMax < maxValues[x])
            maxValues[x] = newMax;
        }
      }
      
      for  (y = 0;  y < plottedSeries.Length;   y++)
      {
        x = fieldIdxs[y];
        plottedSeries[y].Minimum = minValues[x];
        plottedSeries[y].Maximum = maxValues[x];
      }
      
      chartParms.ComputeParameters ();
    }  /* AdjustMinMaxes */



    private void  AdjustMinMaxForLabelIdx (int  labelIdx)
    {
      int x, y;
      int  numHistoryRows = 0;
      lock  (history)
      {
        numHistoryRows = history.Count;
      }
      
      if  (numHistoryRows < 2)
        return;

      double  mean   = 0.0;
      double  vars   = 0.0;
      double  stdDev = 0.0;

      minValues [labelIdx] = 9999.999f;
      maxValues [labelIdx] = -9999.999f;

      lock  (history)
      {
        for  (y = 0;  y < numHistoryRows;  y++)
        {
          InstrumentData dr = (InstrumentData)(history[y]);
          float[] data = dr.Data();
          mean += data[labelIdx];

          if  (data[labelIdx] < minValues [labelIdx])
            minValues[labelIdx] = data[labelIdx];

          if (data[labelIdx] > maxValues[labelIdx])
            maxValues[labelIdx] = data[labelIdx];
        }
      }

      mean = mean / (double)numHistoryRows;

      lock  (history)
      {
        for  (y = 0;  y < numHistoryRows;  y++)
        {
          InstrumentData dr = (InstrumentData)(history[y]);
          float[] data = dr.Data();
          double delta = data[labelIdx] - mean;
          double deltaSquared = delta * delta;
          vars += deltaSquared;
        }
      }

      vars = vars / (double)(numHistoryRows - 1);
      stdDev = Math.Sqrt(vars);

      if  (stdDev != 0.0)
      {
        float newMin = (float)(mean - (3.0 * stdDev));
        float newMax = (float)(mean + (3.0 * stdDev));
        
        if  (newMin > minValues[labelIdx])
          minValues[labelIdx] = newMin;
            
        if  (newMax < maxValues[labelIdx])
          maxValues[labelIdx] = newMax;
      }
      
      for  (y = 0;  y < plottedSeries.Length;   y++)
      {
        if  (plottedSeries[y].SelectedIndex == labelIdx)
        {
          x = fieldIdxs[y];
          plottedSeries[y].Minimum = minValues[x];
          plottedSeries[y].Maximum = maxValues[x];
        }
      }
      
      chartParms.ComputeParameters ();
    }  /* AdjustMinMaxForLabelIdx */



    private  int  suspendCount = 0;  // Number of levels that SuspendAllControls has been called.
    
    
    private  void  SuspendAllControls ()
    {
      int  x;
      
      suspendCount++;
      
      if  (suspendCount > 1)
      {
        // All components are suspended.
        return;
      }
      
      SuspendLayout ();
      for  (x = 0;  x < numDataSeries;  x++)
      {
        plottedSeries[x].SuspendLayout ();
      }
      FindMimMaxButton.SuspendLayout ();
      chartPanel.SuspendLayout ();
      timeRange.SuspendLayout ();
      TimeDomainScrollBar.SuspendLayout ();
      LiveOrHistory.SuspendLayout ();
    }  /* DisableAllControls */



    private void  ResumeAllControls ()
    {
      suspendCount--;
      if  (suspendCount > 0)
      {
        // We nned to be called until 'suspendCount' == 0  before we actuall ResumeLayout
        return;
      }

      int x;
      LiveOrHistory.ResumeLayout ();
      TimeDomainScrollBar.ResumeLayout ();
      timeRange.ResumeLayout ();
      chartPanel.ResumeLayout ();
      FindMimMaxButton.ResumeLayout ();
      for (x = numDataSeries - 1; x >= 0;  x--)
      {
        plottedSeries[x].ResumeLayout ();
      }
      
      ResumeLayout ();
    }  /* DisableAllControls */



    private void  SaveConfiguration ()
    {
      StreamWriter w = null;
      try
      {
        w = new StreamWriter (timePlotConfigFileName);
      }
      catch (Exception e)
      {
        MessageBox.Show ("Exception Writing DataWindow configuration.  [" + e.ToString () + "]");
        return;
      }
      
      int x;

      w.WriteLine ("// TimePlotWindow  Configuration");
      w.WriteLine ("// DateTime" + "\t" + DateTime.Now.ToString ());
      w.WriteLine ("//");
      w.WriteLine ("TimeRange" + "\t" + chartParms.TimeRange);

      w.WriteLine ("// The following [" + InstrumentData.NumDataFields() + "]  rows represent the min/max values of each DataField from InstrumentData.");

      for (x = 0; x < InstrumentData.NumDataFields(); x++)
      {
        w.WriteLine ("InstrumentData" + "\t" +
                     "Label"          + "\t" + InstrumentData.FieldName(x) + "\t" +
                     "Min"            + "\t" + minValues[x]                + "\t"  +
                     "Max"            + "\t" + maxValues[x]                + "\t"  +
                     "PlotData"       + "\t" + plotDataSeries[x]           + "\t"
                    );
      }

      w.WriteLine ("//");
      w.WriteLine ("//");
      w.WriteLine ("// The following [" + numDataSeries + "] rows list contents of the dataSeries to be plotted.");

      for (x = 0;  x < plottedSeries.Length;  x++)
      {
        w.Write ("PlottedDataSeries"  +  "\t"  +
                 "PlotDataSeriesIdx"  +  "\t"  +  x.ToString ()                   +  "\t"  +
                 "SelectedLabel"      +  "\t"  +  plottedSeries[x].SelectedLabel  +  "\t" 
                );
                
        Color  c = plottedSeries[x].PlotColor;
        
        if  (c.IsKnownColor)
        {
          w.Write ("PlotColor" + "\t" + c.Name);
        }
        else
        {
          w.Write ("PlotColorAlpha"  +  "\t"  +  c.A  +  "\t"  +
                   "PlotColorRed"    +  "\t"  +  c.R  +  "\t"  +
                   "PlotColorGreen"  +  "\t"  +  c.G  +  "\t"  +
                   "PlotColorBlue"   +  "\t"  +  c.B
                  );
        }
        
        w.WriteLine ();
      }

      w.WriteLine ("//");
      w.WriteLine ("//");
      w.WriteLine ("//  End Of File.");

      w.Close ();

      return;
    }  /* SaveConfiguration */




    private  void  LoadConfiguration ()
    {
      SuspendAllControls ();
    
      StreamReader f = null;

      try
      {
        f = new StreamReader (timePlotConfigFileName);
      }
      catch (Exception)
      {
        return;
      }

      string txtLine = f.ReadLine ();
      while (txtLine != null)
      {
        if (txtLine.Substring (0, 2) != "//")
        {
          String[] fields = txtLine.Split ('\t');
          if (fields.Length > 1)
          {
            string rowType = fields[0].ToLower ().Trim ();
            
            if  (rowType == "timerange")
            {
              if  (fields.Length > 1)
              {
                chartParms.TimeRange = PicesKKStr.StrToInt (fields[1]);
                UpdateTimeRangeControl ();
              }
            }

            if  (rowType == "plotteddataseries")
            {
              LoadConfigurationPlottedDataSeries (fields);
            }
            
            else if  (rowType == "instrumentdata")
            {
              LoadConfigurationDataRow (fields);
            }
          }
        }

        txtLine = f.ReadLine ();
      }

      f.Close ();


      {
        int  x;
        for (x = 0;  x < numDataSeries;  x++)
        {
          int  z = fieldIdxs[x];
          if  (z >= 0)
          {
            plottedSeries[x].SetMinMax (minValues[z], maxValues[z]);
            plottedSeries[x].Plot = plotDataSeries[z];
          }
        }
      }


      {
        // If no TimeRange was loaded we need to default to shortest setting
        if  (timeRange.SelectedOptionInt < 0)
        {
          timeRange.SelectedOptionInt = 0;
        }
      }

      chartParms.ComputeParameters ();
      UpdateTimeDomainScrollingRanges ();
      
      ResumeAllControls ();

      return;
    }  /* LoadConfiguration */




    private  void  LoadConfigurationPlottedDataSeries (string[] fields)
    {
      int     idx            = -1;
      string  label          = "";
      int     labelIdx       = -1;
      bool    plot           = false;
      Color   plotColor      = Color.Black;
      int     plotColorAlpha = 255;
      int     plotColorRed   = 0;
      int     plotColorGreen = 0;
      int     plotColorBlue  = 0;
      bool    plotColorKnown = false;

      bool  errorsFound = false;
      
      int  fieldNum = 0;
      if  (fields[0].ToLower () == "plotteddataseries")
        fieldNum = 1;

      while  (fieldNum < fields.Length)
      {
        string  fieldName = fields[fieldNum].ToLower ();
        fieldNum++;
        
        string  fieldValue = "";
        if  (fieldNum < fields.Length)
        {
          fieldValue = fields[fieldNum];
          fieldNum++;
        }
        
        if  (fieldName == "plotdataseriesidx")
          idx = PicesKKStr.StrToInt (fieldValue);
          
        else if  ((fieldName == "label")  ||  (fieldName == "selectedlabel"))
        {
          label = fieldValue;
          labelIdx = InstrumentData.GetFieldNameIndex(label);
        }
        
        else if  (fieldName == "plot")
          plot = PicesKKStr.StrToBool (fieldValue);

        else if ((fieldName == "plotcoloralpha")  ||  (fieldName == "alpha"))
        {
          plotColorAlpha  = PicesKKStr.StrToInt (fieldValue);
          plotColorKnown  = false;
        }

        else if ((fieldName == "plotcolorred")    ||  (fieldName == "red"))
        {
          plotColorRed  = PicesKKStr.StrToInt (fieldValue);
          plotColorKnown  = false;
        }

        else if ((fieldName == "plotcolorgreen")  ||  (fieldName == "green"))
        {
          plotColorGreen  = PicesKKStr.StrToInt (fieldValue);
          plotColorKnown  = false;
        }

        else if ((fieldName == "plotcolorblue")  ||  (fieldName == "blue"))
        {
          plotColorBlue  = PicesKKStr.StrToInt (fieldValue);
          plotColorKnown  = false;
        }
        
        else if  (fieldName == "plotcolor")
        {
          plotColorKnown  = true;
          plotColor = Color.FromName (fieldValue);
          plotColorAlpha  = plotColor.A;
          plotColorRed    = plotColor.R;
          plotColorGreen  = plotColor.G;
          plotColorBlue   = plotColor.B;
        }
      }
      
      
      if  ((idx < 0)  ||  (idx > numDataSeries))
      {
        errorsFound = true;
      }
      
      if  (labelIdx < 0)
      {
        errorsFound = true;
      }
      
      if  (!errorsFound)
      {
        fieldIdxs[idx] = labelIdx;

        if ((plotColorRed == 0)  &&  (plotColorGreen == 0)  &&  (plotColorBlue == 0))
        {
          plotColor = plottedSeries[idx].PlotColor;
        }
        else if  (!plotColorKnown)
        {
           plotColor = Color.FromArgb (plotColorAlpha, plotColorRed, plotColorGreen, plotColorBlue);
        }
        
        plottedSeries[idx].SuspendLayout ();
        plottedSeries[idx].Enabled = false;
        plottedSeries[idx].SetParameters (labelIdx, plot, plotColor);
        plottedSeries[idx].Enabled = true;
        plottedSeries[idx].ResumeLayout();

        plotBrushes[idx] = new SolidBrush (plotColor);
      }

    }  /* LoadConfigurationPlottedDataSeries */



    private void  LoadConfigurationDataRow (string[] fields)
    {
      string label     = "";
      int    labelIdx  = -1;
      float  min       = 0.0f;
      float  max       = 0.0f;
      bool   plotData  = false;

      bool errorsFound = false;

      int fieldNum = 0;
      if (fields[0].ToLower () == "instrumentdata")
        fieldNum = 1;

      while (fieldNum < fields.Length)
      {
        string fieldName = fields[fieldNum].ToLower ();
        fieldNum++;

        string fieldValue = "";
        if (fieldNum < fields.Length)
        {
          fieldValue = fields[fieldNum];
          fieldNum++;
        }

        if  (fieldName == "label")
        {
          label = fieldValue;
          labelIdx = InstrumentData.GetFieldNameIndex(label);
        }

        else if ((fieldName == "min")  ||  (fieldName == "minimum"))
          min = PicesKKStr.StrToFloat (fieldValue);

        else if ((fieldName == "max")  ||  (fieldName == "maximum"))
          max = PicesKKStr.StrToFloat (fieldValue);

        else if (fieldName == "plotdata")
          plotData = PicesKKStr.StrToBool (fieldValue);
      }

      if  ((labelIdx < 0)  ||  (labelIdx >= InstrumentData.NumDataFields ()))
      {
        errorsFound = true;
      }

      if  (min > max)
      {
        errorsFound = true;
      }


      if (!errorsFound)
      {
        minValues [labelIdx] = min;
        maxValues [labelIdx] = max;
        plotDataSeries[labelIdx] = plotData;
      }
    }  /* LoadConfigurationDataRow */




    private void  DrawSingleDataRow (InstrumentData dr)    
    {
      chartParms.PlotDataRow (dr);
    }  /* DrawSingleDataRowOnSpecifiedCol */



    private  void  ClearSelectedVerticalLine ()
    {
      if  (selectedHistory == null)
        return;

      chartParms.PaintSelectedLine (selectedHistory.Time, new SolidBrush (chartParms.PlotBackgroundColor));
      DrawSingleDataRow (selectedHistory);
    }  /* ClearSelectedVerticalLine */


    private  void  DrawSelectedVerticalLine ()
    {
      chartParms.PaintSelectedLine (selectedHistory.Time, Brushes.BlueViolet);
    }  /* DrawSelectedVerticalLine */



    private void  UpdateDataSeriesValues (InstrumentData dr)
    {
      int  dsIDX = 0;
      
      if  (dr == null)
        return;
      
      float[]  data = dr.Data ();
      
      for  (dsIDX = 0;  dsIDX < numDataSeries;  dsIDX++)
      {
        DataSeries  ds = plottedSeries[dsIDX];
        ds.Data = data[fieldIdxs[dsIDX]];
      }
    }  /* UpdateDataSeriesValues */



    private void  UpdateTimePlot ()
    {
      // This fuction gets called by the timer;  we are to update the 
      // plot area with any new data that has arived.
      
      if  (lastHistoryRowDisplayed < 0)
        lastHistoryRowDisplayed = 0;
        
      int  lastRowInHistory = -1;
      lock  (history)
      {
        lastRowInHistory = history.Count - 1;
      }

      if  (lastHistoryRowDisplayed > lastRowInHistory)
      {
        // Histry must have been cleaed.  We have to reset 'lastHistoryRowDisplayed'
        lastHistoryRowDisplayed = lastRowInHistory;
      }


      if  (lastHistoryRowDisplayed >= lastRowInHistory)
        return;


      // Make sure that the last row in 'history' is visible in plot 
      // area as currently defined by 'timeLeft' and 'timeRight'.

      // Compute instatanious Delta-Depth


      InstrumentData  lastHistRow = (InstrumentData)(history[lastRowInHistory]);

      int  colThatWillDisplayOn = chartParms.ComputeColFromTime (lastHistRow.Time);
      if  (colThatWillDisplayOn < -15)
      {
        DateTime  dt = lastHistRow.Time;
        if  (dt.Year >= 2006)
        {
          chartParms.SetNewTimeLeft (dt);
          RePlotData ();
        }
      }
      else
      {
        chartParms.MakeTimeVisible (((InstrumentData)(history[lastRowInHistory])).Time);
      }
      
      while (lastHistoryRowDisplayed < lastRowInHistory)
      {
        lastHistoryRowDisplayed++;
        leftMostDataRow = (InstrumentData)history[lastHistoryRowDisplayed];
        DrawSingleDataRow (leftMostDataRow);
      }

      if  (selectedHistory == null)
        UpdateDataSeriesValues (leftMostDataRow);
      else
        UpdateDataSeriesValues (selectedHistory);

    }  /* UpdateTimePlot */




    private  void  PaintEntirePlotArea ()
    {
      chartParms.ClearPlotArea ();
      
      DateTime timeLeft  = chartParms.TimeLeft ();
      DateTime timeRight = chartParms.TimeRight ();
      
      int  rowsInHistory = 0;
      lock (history)
      {
        rowsInHistory = history.Count;
      }

      int  row = 0;

      // lets skip past all the old hostory.
      while  (row < rowsInHistory)
      {
        InstrumentData dr = (InstrumentData)history[row];
        if  (dr.Time.CompareTo (timeRight) > 0)
          break;
        row++;
      }
      
      while  (row < rowsInHistory)
      {
        InstrumentData dr = (InstrumentData)history[row];
        if (dr.Time.CompareTo (timeLeft) > 0)
        {
          // We are past the left side of the plot
          break;
        }
        
        DrawSingleDataRow (dr);
        
        leftMostDataRow = dr;
        row++;
      } 

      if  (selectedHistory != null)
      {
        DrawSelectedVerticalLine ();
      }
    }  /* PaintEntirePlotArea */

    
    
    
    
    
    private void  UpdateTimeDomainScrollingRanges ()
    {
      //SuspendAllControls ();

      TimeDomainScrollBar.Minimum = 0;

      DateTime minTime = DateTime.Now;
      DateTime maxTime = DateTime.Now;
      int historyCount = 0;

      lock (history)
      {
        minTime = history.TimeMin ();
        maxTime = history.TimeMax ();
        historyCount = history.Count;
      }

      // Update Scroll Bar to reflect ranges
      if  (historyCount < 1)
      {
        TimeDomainScrollBar.Minimum     = 0;
        TimeDomainScrollBar.Maximum     = chartParms.TimeRange;
        TimeDomainScrollBar.LargeChange = chartParms.TimeRange;
        TimeDomainScrollBar.Value       = 0;
        timeDomainScrollBarMax = DateTime.Now;
        timeDomainScrollBarMin = timeDomainScrollBarMax.AddSeconds (0 - chartParms.TimeRange);
      }
      else
      {
        TimeSpan ts = maxTime - minTime;
        TimeDomainScrollBar.Minimum     = 0;
        TimeDomainScrollBar.Maximum     = (int)ts.TotalSeconds;
        TimeDomainScrollBar.LargeChange = chartParms.TimeRange;
        
        if  (liveUpdate)
        {
          TimeDomainScrollBar.Value = 0;
        }
        else
        {
          if  (chartParms.TimeLeft ().CompareTo (minTime) < 0)
          {
            // This should never be able to happen;  but unfortunatly it seems to be happening in test.
            ts = new TimeSpan (0, 0, 0);
          }
          else
          {
            ts = maxTime - chartParms.TimeLeft ();
          }
        
          TimeDomainScrollBar.Value       = (int)ts.TotalSeconds;
        }

        timeDomainScrollBarMax = maxTime;
        timeDomainScrollBarMin = minTime;
      }

      //ResumeAllControls ();
    }  /* UpdateTimeDomainScrollingRanges  */  
    



    private void TimePlot_Load (object sender, EventArgs e)
    {

    }


    private void dataSeries2_Load (object sender, EventArgs e)
    {

    }


    private void dataSeries2_Load_1 (object sender, EventArgs e)
    {

    }


    private void updateChartTimer_Tick (object sender, EventArgs e)
    {
      ticCount++;
      
      bool  thirtySecInterval = ((ticCount % 300) == 0);
      if  (liveUpdate)
      {
        UpdateTimePlot ();
      }

      if (thirtySecInterval)
      {
        UpdateTimeDomainScrollingRanges ();
      }

    }  /* updateChartTimer_Tick */




    private  void  UpdateTimeRangeFromStr (String  trStr)
    {
      trStr = trStr.ToUpper ();
      switch (trStr)
      {
        case "1 MIN"  : chartParms.TimeRange = 1 * 60; break;
        case "5 MINS" : chartParms.TimeRange = 5 * 60; break;
        case "10 MINS": chartParms.TimeRange = 10 * 60; break;
        case "15 MINS": chartParms.TimeRange = 15 * 60; break;
        case "20 MINS": chartParms.TimeRange = 20 * 60; break;
        case "30 MINS": chartParms.TimeRange = 30 * 60; break;
        case "45 MINS": chartParms.TimeRange = 45 * 60; break;
        case "1 HOUR" : chartParms.TimeRange = 60 * 60; break;
        case "ALL":
          {
            lock (history)
            {
              TimeSpan ts = (history.TimeMax() - history.TimeMin());
              chartParms.TimeRange = (int)(Math.Max(ts.TotalSeconds, 300.0) + 0.5);
            }
            chartParms.TimeRange = 60 * 60; break;
          }
      }
    }  /* UpdateTimeRangeFromStr */


    private  void  UpdateTimeRangeControl ()
    {
      int  tr = chartParms.TimeRange;
      
      switch  (chartParms.TimeRange)
      {
        case   60: timeRange.SelectedOption =  "1 Mins";   break;
        case  120: timeRange.SelectedOption =  "2 Mins";   break;
        case  300: timeRange.SelectedOption =  "5 Mins";   break;
        case  600: timeRange.SelectedOption = "10 Mins";   break;
        case  900: timeRange.SelectedOption = "15 Mins";   break;
        case 1200: timeRange.SelectedOption = "20 Mins";   break;
        case 1800: timeRange.SelectedOption = "30 Mins";   break;
        case 2700: timeRange.SelectedOption = "45 Mins";   break;
        case 3600: timeRange.SelectedOption = "1 Hour";    break;
        
        default:  timeRange.SelectedOption = "All";    break;
      }
    }

    
    private void  RePlotData ()
    {
      chartParms.ComputeParameters();
      PaintEntirePlotArea();

      if  (selectedHistory == null)
        UpdateDataSeriesValues (leftMostDataRow);
      else
        UpdateDataSeriesValues (selectedHistory);
    }


    private void  TimeRange_SelectedOptionChanged ()
    {
      SuspendAllControls ();
      UpdateTimeRangeFromStr (timeRange.SelectedOption);
      RePlotData ();
      UpdateTimeDomainScrollingRanges ();
      ResumeAllControls ();
    }


    private void chartPanel_Paint(object sender, PaintEventArgs e)
    {
      PaintEntirePlotArea ();
    }


    private  void  UpdateSelectedLabel (int dsIDX)
    {
      if  (chartParms == null)
      {
       // We are not done initializing Variables yet.
       return;
      }

      int x;
      DataSeries  ds = plottedSeries[dsIDX];
      
      int  newSelectedLabelIndex  = ds.SelectedIndex;
      int  origSelectedLabelIndex = fieldIdxs[dsIDX];

      // See if any other plottedSeries are using the same SelectedIndex.
      for  (x = 0;  x < plottedSeries.Length;  x++)
      {
        if  (x == dsIDX)
          continue;

        if  (newSelectedLabelIndex == fieldIdxs[x])
        {
          // we need to swap assignments.
          DataSeries otherDS = plottedSeries[x];
          otherDS.Enabled = false;
          fieldIdxs[x] = fieldIdxs[dsIDX];
          otherDS.SelectedIndex = origSelectedLabelIndex;
          otherDS.Minimum = minValues[origSelectedLabelIndex];
          otherDS.Maximum = maxValues[origSelectedLabelIndex];
          otherDS.Plot    = plotDataSeries[origSelectedLabelIndex];
          otherDS.Enabled = true;
          break;
        }
      }
      
      
      if  (newSelectedLabelIndex != fieldIdxs[dsIDX])
      {
        ds.SuspendLayout ();

        int dataIDX = newSelectedLabelIndex;
        fieldIdxs[dsIDX] = newSelectedLabelIndex;
        ds.Minimum = minValues[dataIDX];
        ds.Maximum = maxValues[dataIDX];
        ds.Plot    = plotDataSeries[dataIDX];
        RePlotData ();
        
        ds.ResumeLayout ();
      }
    }


    private void dataSeries0_SelectedLabelChanged()
    {
      UpdateSelectedLabel (0);
    }


    private void dataSeries1_SelectedLabelChanged()
    {
      UpdateSelectedLabel (1);
    }
 
    private void dataSeries2_SelectedLabelChanged()
    {
      UpdateSelectedLabel (2);
    }

    private void dataSeries3_SelectedLabelChanged()
    {
      UpdateSelectedLabel (3);
    }

    private void dataSeries4_SelectedLabelChanged()
    {
      UpdateSelectedLabel (4);
    }

    private void dataSeries5_SelectedLabelChanged()
    {
      UpdateSelectedLabel (5);
    }
    
 
    private void  RangeChanged (int dsIDX)
    {
      if (chartParms == null)
      {
        // We are not done initializing Variables yet.
        return;
      }

      DataSeries ds = plottedSeries[dsIDX];
      float  newMin = ds.Minimum;
      float  newMax = ds.Maximum;

      int dataIDX = fieldIdxs[dsIDX]; 
      
      if  ((minValues[dataIDX] == newMin)  &&  (maxValues[dataIDX] == newMax)) 
        return;
        
      if  (newMin >= newMax)
        return;
        
      ds.SuspendLayout ();

      minValues[dataIDX] = newMin;
      maxValues[dataIDX] = newMax;
      RePlotData ();
      
      ds.ResumeLayout ();
    }  /* RangeChanged */
    
   
    private void  dataSeries0_RangeChanged ()
    {
      RangeChanged (0);
    }

    private void  dataSeries1_RangeChanged ()
    {
      RangeChanged (1);
    }

    private void  dataSeries2_RangeChanged ()
    {
      RangeChanged (2);
    }

    
    private void  dataSeries3_RangeChanged ()
    {
      RangeChanged (3);
    }


    private void  dataSeries4_RangeChanged ()
    {
      RangeChanged (4);
    }


    private void  dataSeries5_RangeChanged ()
    {
      RangeChanged (5);
    }


    private void  UpdatePlotColor (int dsIDX)
    {
      if (chartParms == null)
      {
        // We are not done initializing Variables yet.
        return;
      }
    

      DataSeries  ds = plottedSeries[dsIDX];
      int dataIDX = fieldIdxs[dsIDX];
      
      ds.SuspendLayout ();
      
      Color  newColor = ds.PlotColor;
      
      if  (newColor != plotBrushes[dsIDX].Color)
      {
        plotBrushes[dsIDX] = new SolidBrush (newColor);
        RePlotData ();
      }
      
      ds.ResumeLayout ();
    }  /* UpdatePlotColor */


    private void dataSeries0_PlotColorChanged()
    {
      UpdatePlotColor (0);
    }

    private void dataSeries1_PlotColorChanged()
    {
      UpdatePlotColor (1);
    }

    private void dataSeries2_PlotColorChanged()
    {
      UpdatePlotColor (2);
    }

    private void dataSeries3_PlotColorChanged()
    {
      UpdatePlotColor (3);
    }

    private void dataSeries4_PlotColorChanged()
    {
      UpdatePlotColor (4);
    }

    private void dataSeries5_PlotColorChanged()
    {
      UpdatePlotColor (5);
    }

    
    private  void  UpdatePlotData (int dsIDX)
    {
      DataSeries ds = plottedSeries[dsIDX];
      int  dataIDX = fieldIdxs[dsIDX];
      
      if  (ds.Plot == plotDataSeries[dataIDX])
        return;
      
      ds.SuspendLayout ();
      plotDataSeries[dataIDX] = ds.Plot;
      RePlotData ();
      ds.ResumeLayout ();
    }
    

    private  void  ReCalcMinMaxForSpecificPlottedSeries (DataSeries  ds)
    {
      int  selectedLabelIdx = ds.SelectedIndex;
      AdjustMinMaxForLabelIdx (selectedLabelIdx);
    }

    
    private void dataSeries0_PlotChanged ()
    {
      UpdatePlotData (0);
    }

    private void dataSeries1_PlotChanged ()
    {
      UpdatePlotData (1);
    }

    private void dataSeries2_PlotChanged ()
    {
      UpdatePlotData (2);
    }

    private void dataSeries3_PlotChanged ()
    {
      UpdatePlotData (3);
    }

    private void dataSeries4_PlotChanged ()
    {
      UpdatePlotData (4);
    }

    private void dataSeries5_PlotChanged ()
    {
      UpdatePlotData (5);
    }


    private void  SetToHistoryMode ()
    {
      LiveOrHistory.Text = "Live";
      liveUpdate = false;
    }


    private  void  SetToLiveMode ()
    {
      selectedHistory = null;
      LiveOrHistory.Text = "History";
      liveUpdate = true;
        
      DateTime  t = DateTime.Now;
      lock (history)
      {
        t = history.TimeMax ();
      }
        
      chartParms.SetNewTimeLeft (t);
      RePlotData ();
      UpdateTimeDomainScrollingRanges ();
    }



    private void LiveOrHistory_Click (object sender, EventArgs e)
    {
      if  (liveUpdate)
        SetToHistoryMode ();
      else
        SetToLiveMode ();
    }



    private void  TimeDomainScrollBar_ValueChanged (object sender, EventArgs e)
    {
      DateTime  timeMax = DateTime.Now;
      lock  (history)
      {
        timeMax = history.TimeMax ();
      }
      int  offset = TimeDomainScrollBar.Value;

      DateTime  newPlotLeftTime = timeDomainScrollBarMax.AddSeconds (0 - offset);
      chartParms.SetNewTimeLeft (newPlotLeftTime);

      if  (liveUpdate)
      {
        if  (offset > 0)
        {
          liveUpdate = false;
          LiveOrHistory.Text = "Live";
        }
      }

      else if  ((offset == 0)  &&  (timeDomainScrollBarMax.Equals (timeMax)))
      {
        liveUpdate = true;
        LiveOrHistory.Text = "History";
      }
      
      RePlotData ();
    }




    private  void   SelectTimeInstant (int  pixelCol)
    {
      if  (selectedHistory != null)
        ClearSelectedVerticalLine ();

      selectedDateTime = chartParms.ComputeDateTimeFromPixelCol (pixelCol);
      selectedHistory = history.FindClosestEntryByTime (selectedDateTime);
      SetToHistoryMode ();
      DrawSelectedVerticalLine ();
      UpdateDataSeriesValues (selectedHistory);
    }  /* SelectTimeInstant */




    private void FindMimMaxButton_Click(object sender, EventArgs e)
    {
      AdjustMinMaxes ();
    }

    private void chartPanel_MouseClick (object sender, MouseEventArgs e)
    {
      if  (e.Button == MouseButtons.Left)
        SelectTimeInstant (e.Location.X);
    }  /* TimeRange_SelectedOptionChanged */
  }
}
