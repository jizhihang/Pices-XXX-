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


namespace SipperFile
{
  class TimePlotParameters
  {
    int  panelHeight = 0;
    int  panelWidth  = 0;
     
    int  plotWidth  = 0;
    int  plotHeight = 0;
      
    int  plotLeftCol  = 0;
    int  plotRightCol = 0;
      
    int  plotBotRow = 0;   // Last row that can be used for charting data;  below it will be the axis data.
    int  plotTopRow = 0;
      
    Color  plotBackgroundColor = Color.DarkGray;

    private  Bitmap    plotBuffer   = null;
    private  Graphics  plotBufferDC = null;
    private  Graphics  plotDC       = null;

    private  int  yAxisCol = 0;
    private  int  xAxisRow = 0;
    private  int  xAxisLabelRow = 0;
    private  int  xAxisLabelHeight = 0;
      
    Font xAxisLabelFont = new Font ("Arial", 10);
    SolidBrush xAxisLableBrush = new SolidBrush (Color.White);


    int  curCol = 0;  // current collumn to be plotting on
      
    //int  timeSpanSecs = 300;  //  in seconds

    private  Color     axisColor = Color.Black;


    public  int  PlotTopRow ()  { return plotTopRow; }
    public  int  PlotBotRow ()  { return plotBotRow; }
      
    private  Panel   panel            = null;

    private int   [] fieldIdxs       = null;    // Specifies which InstrumentData fields are to be displayed in respective plottedSeries.
    private float [] minValues       = null;
    private float [] maxValues       = null;
    private float [] rowScaleFactors = null;
    private bool  [] plotDataSeries  = null;
    private SolidBrush[] plotBrushes = null;
    private DataSeries[] plottedSeries = null;


    //DateTime  timeLeft = DateTime.Now;
    //DateTime  timeRight = timeStart;

    private  DateTime timeRight   = DateTime.Now;
    private  DateTime timeLeft    = DateTime.Now;

    private  float   secsPerCol = 1.0f;
    private  int     timeRange = 300;   // 300 Seconds, 5 mins
      
    private  int       ticCount      = 5;       // Number of Tic marks along X Axis
    private  Pen       ticPen        = new Pen (Color.LightGray);
    private  int       ticTimeRange  = 60;
    private  DateTime  ticNextOne    = DateTime.Now;



    public  Color  PlotBackgroundColor   {get {return  plotBackgroundColor;}}




    public  TimePlotParameters (Panel        _panel,
                                int[]        _fieldIdxs,
                                float[]      _minValues,
                                float[]      _maxValues,
                                float[]      _rowScaleFactors,
                                bool[]       _plotDataSeries,
                                SolidBrush[] _plotBrushes,
                                DataSeries[] _plottedSeries
                               )
    {
      panel = _panel;
      timeLeft = DateTime.Now;
      TimeSpan ts = new TimeSpan (0, 0, timeRange);
      timeRight = timeLeft.Subtract (ts);

      fieldIdxs       = _fieldIdxs;
      minValues       = _minValues;
      maxValues       = _maxValues;
      rowScaleFactors = _rowScaleFactors;
      plotDataSeries  = _plotDataSeries;
      plotBrushes     = _plotBrushes;
      plottedSeries   = _plottedSeries;
        
      plotDC = panel.CreateGraphics ();
    }

    public int      CurCol    () { return curCol; }
    public DateTime TimeLeft  () { return timeLeft; }
    public DateTime TimeRight () { return timeRight; }


    public  int  TimeRange
    {
      get  {return timeRange;}
      set
      {
        if  (value > 0)
        {
          timeRange = value;
          ComputeParameters ();
        }
      }
    }  /* TimRange */



    public  void  ComputeParameters ()
    {
      panelHeight = panel.Height;
      panelWidth  = panel.Width;
        
      xAxisLabelHeight = xAxisLabelFont.Height;

      yAxisCol = 2;
      xAxisRow = panelHeight - (xAxisLabelHeight + 4);
      xAxisLabelRow = xAxisRow + 2;

      plotLeftCol  = yAxisCol + 1;
      plotRightCol = panelWidth - 2;
        
      plotTopRow = 1;
      plotBotRow = xAxisRow - 1;

      plotWidth  = 1 + plotRightCol - plotLeftCol;
      plotHeight = 1 + plotBotRow   - plotTopRow;
        
      plotBuffer    = new Bitmap (panelWidth, panelHeight);
      plotBufferDC  = Graphics.FromImage(plotBuffer);

      TimeSpan ts = new TimeSpan (0, 0, timeRange);
      timeRight = timeLeft.Subtract (ts);

      secsPerCol = (float)timeRange / (float)plotWidth;

      ticTimeRange = (int)(((float)timeRange / (float)ticCount) + 0.5);

      RecalcColScaleFactors ();
    }  /* ComputeParameters */
      
      
      
    public  void  ClearPlotArea ()
    {
      plotDC.FillRectangle (new SolidBrush (plotBackgroundColor), 0, 0, panelWidth, panelHeight);
      plotBufferDC.FillRectangle (new SolidBrush (plotBackgroundColor), 0, 0, panelWidth, panelHeight);
      DrawXAxisWithLabels ();
    }   /* ClearPlotArea */
      
      
      
      
    public  void  SetNewTimeLeft (DateTime  _timeLeft)
    {
      timeLeft = _timeLeft;
      ComputeParameters ();
    }


    private void  RecalcColScaleFactors ()
    {
      // Calculate rowScaleFactors
      int  x;
      for  (x = 0;  x < InstrumentData.NumDataFields ();  x++)
      {
        rowScaleFactors[x] = plotHeight / (maxValues[x] - minValues[x]);
      }
    }   /* RecalcRowScaleValues */




    public  int  ComputeColFromTime (DateTime t)
    {
      //DateTime timeRight = DateTime.Now;
      //DateTime timeLeft  = DateTime.Now;
      //
      //float secsPerCol = 1.0f;
       
      //if  (t.CompareTo (timeRight) < 0)
      //  return  999999;
          
      //if  (t.CompareTo (timeLeft) > 0)
      //  return -1;
          
      TimeSpan  offset = timeLeft - t;
      double  offsetSecs = offset.TotalSeconds;
        
      int  computedCol = plotLeftCol + (int) ((offsetSecs / secsPerCol) + 0.5);
      return  computedCol;
    }  /* ComputeColFromTime */




    public  void DrawXAxisWithLabels ()
    {
      Pen p = new Pen (axisColor);
      plotDC.DrawLine (p, 1, xAxisRow, plotRightCol, xAxisRow);
      plotBufferDC.DrawLine (p, 1, xAxisRow, plotRightCol, xAxisRow);

      ticNextOne = timeLeft.AddSeconds (ticTimeRange);
      
      int  tic = 0;
      DateTime  t = timeLeft;

      for  (tic = 0;  tic < ticCount;  tic++)
      {
        int col = ComputeColFromTime (t);

        string  labelStr = t.ToString ("hh:mm:ss");


        plotDC.DrawLine (ticPen, col, plotTopRow, col, xAxisRow);
        plotBufferDC.DrawLine (ticPen, col, plotTopRow, col, xAxisRow);

        PointF point = new PointF (col, this.xAxisLabelRow);
          
        plotDC.DrawString        (labelStr, xAxisLabelFont, xAxisLableBrush, point);
        plotBufferDC.DrawString (labelStr, xAxisLabelFont, xAxisLableBrush, point);

        t = t.AddSeconds (0 - ticTimeRange);
      }
    }  /* DrawXAxisWithLabels */



    public  void  ScrollPlotAreaRightOneCol ()
    {
      int  height = plotBuffer.Height;
      
      
      Bitmap  nextBuffer = new Bitmap (plotBuffer.Width, plotBuffer.Height);
      Graphics nextBufferDC = Graphics.FromImage (nextBuffer);
      nextBufferDC.Clear (plotBackgroundColor);
      nextBufferDC.DrawImageUnscaled (plotBuffer, 1, 0);

      plotDC.Clear (plotBackgroundColor);
      plotDC.DrawImageUnscaled (nextBuffer, 0, 0);
      
      plotBufferDC.Dispose ();
      plotBufferDC = null;
      
      plotBuffer.Dispose ();
      plotBuffer = null;
      
      plotBuffer   = nextBuffer;
      plotBufferDC = nextBufferDC;
        
      timeLeft  = timeLeft.AddSeconds  (secsPerCol);
      timeRight = timeRight.AddSeconds (secsPerCol);

      // Extend x-Axis
      Pen p = new Pen (axisColor);
      plotDC.DrawLine (p, 1, xAxisRow, 2, xAxisRow);
      plotBufferDC.DrawLine (p, 1, xAxisRow, 2, xAxisRow);
      
      // Determine if we just crossed a tic boundary
      if  (ticNextOne.CompareTo (timeLeft) <= 0)
      {
        int ticCol = ComputeColFromTime (ticNextOne);

        plotDC.DrawLine (ticPen, ticCol, plotTopRow, ticCol, xAxisRow);
        plotBufferDC.DrawLine (ticPen, ticCol, plotTopRow, ticCol, xAxisRow);

        PointF point = new PointF (ticCol, this.xAxisLabelRow);

        string  labelStr = ticNextOne.ToString ("hh:mm:ss");

        plotDC.DrawString (labelStr, xAxisLabelFont, xAxisLableBrush, point);  
        plotBufferDC.DrawString (labelStr, xAxisLabelFont, xAxisLableBrush, point);
        ticNextOne = ticNextOne.AddSeconds (ticTimeRange);
      }
    }  /* ScrollPlotAreaRightOneCol */



    public  void  MakeTimeVisible (DateTime  t)
    {
      // Scroll the plot area over such that 't' is visible in left column
      int  colThatItWouldDisplayIn = ComputeColFromTime (t);
        
      
      if  ((colThatItWouldDisplayIn >= plotRightCol)  &&  (colThatItWouldDisplayIn <= plotLeftCol))
        return;
          

      if (colThatItWouldDisplayIn < plotLeftCol)
      {
        // We need to scroll plot area right until time 't' becomes visible.
        while  (colThatItWouldDisplayIn < plotLeftCol)
        {
          ScrollPlotAreaRightOneCol ();
          colThatItWouldDisplayIn = ComputeColFromTime (t);
        }
      }
    }



    public  void  SetCurCollumn (DateTime  t)
    {
      curCol = ComputeColFromTime (t);
    }  /* SetCurCollumn */




    public void  PlotDataRow (InstrumentData dr)
    {
      SetCurCollumn (dr.Time);
      float[]  data = dr.Data ();
       
      int  x;
      for  (x = 0;  x < 6;  x++)
      {
        int  y = fieldIdxs[x];
        if  (plotDataSeries[y])
        {
          float  dataValue = data[y];
          int row = (int)(((dataValue - minValues[y]) * this.rowScaleFactors[y]) + 0.5);
          if  (row < 0)
            row = 0;

          else if  (row >= plotBotRow)
             row = plotBotRow;

          if  (curCol < 0)
            curCol = 0;


          plotDC.FillRectangle       (plotBrushes[x], curCol, row, 2, 2);
          plotBufferDC.FillRectangle (plotBrushes[x], curCol, row, 2, 2);
        }
      }
    }  /* PlotDataItem */




    public  void  PaintSelectedLine (DateTime  dt,
                                     Brush     b
                                    )
    {
      SetCurCollumn (dt);
      plotDC.FillRectangle (b, curCol, 1, 1, plotHeight - 1);
      plotBufferDC.FillRectangle (b, curCol, 1, 1, plotHeight - 1);
    }  /* PaintSelectedLine */



  
    public  DateTime  ComputeDateTimeFromPixelCol (int pixelCol)
    {
      int  colOffset = (pixelCol - plotLeftCol);
      double  offsetSecs = (double)colOffset * (double)secsPerCol;

      DateTime  dt = timeLeft.AddSeconds (0.0 - offsetSecs);
      return  dt;
    }  /* ComputeDateTimeFromPixelCol */
  
 
  }  /* TimePlotParameters  */
}
