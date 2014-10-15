using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;
using ChartControls;
using SipperFile;


namespace SipperInterface
{
  class DepthPlotParameters
  {
    Panel         chartPanel = null;
    Ruler[]       rulers     = null;
    VScrollBar    domainScrollBar = null;
    
    int[,]        fieldIdxs  = null;
    InstrumentDataList   history    = null;

    float         depthTop       = 0.0f;             // Meters
    float         depthBot       = 100.0f;           // meters
    DateTime      timeTop        = DateTime.Now;
    DateTime      timeBot        = DateTime.Now;

    bool          timePlot       = true;     // works with depthPlot;  only one can be true
    bool          depthPlot      = false;

    int           maxNumDataSets = 6;

    bool[]        seriesToPlot   = null;
    SolidBrush[]  seriesBrushes  = null;

    int           chartWidth     = 0;
    int           chartFirstCol  = 0;
    int           chartLastCol   = 0;
    int           chartRowWidth  = 2;
    int           chartColWidth  = 2;
    
    int           rowFirst       = 0;
    int           rowLast        = 0;

    Font          drawFont       = new Font ("Arial", 8);
    SolidBrush    drawBrush      = new SolidBrush (Color.White);

    float[]       minValues = null;
    float[]       maxValues = null;
    float[]       colFactors = null;

    float         secsPerPixelRow   = 0.0f;
    float         metersPerPixelRow = 0.0f;

    public bool   TimePlot  ()  {return  timePlot;}
    public bool   DepthPlot ()  {return  depthPlot;}

    private int   plottingAxisIdx = 0;

    private Bitmap    plotBuffer   = null;
    private Graphics  plotBufferDC = null;
    private Graphics  plotDC       = null;




    public  DepthPlotParameters (Panel       _chartPanel,
                                 Ruler[]     _rulers,
                                 VScrollBar  _domainScrollBar,
                                 int[,]      _fieldIdxs,
                                 InstrumentDataList _history
                                )
    {
      int x;

      chartPanel      = _chartPanel;
      rulers          = _rulers;
      domainScrollBar = _domainScrollBar;

      fieldIdxs  = _fieldIdxs;
      history    = _history;

      seriesToPlot = new bool[maxNumDataSets];
      for (x = 0; x < maxNumDataSets; x++)
        seriesToPlot[x] = true;

      AssignColors ();
      
      CalcChartVariables ();
    }




    private  void  CalcChartVariables ()
    {
      chartFirstCol = rulers[0].FirstCol ();
      chartLastCol  = rulers[0].LastCol ();

      chartWidth = chartLastCol - chartFirstCol + 1;
      rowFirst = 10;
      rowLast  = chartPanel.Height - 10;

      int  totalPixelRows = 1 + rowLast - rowFirst;

      if  ((minValues == null)  ||  (maxValues == null))
      {
        history.CalcMinMaxValues (timeBot, timeTop);
        minValues = history.MinValues ();
        maxValues = history.MaxValues ();
      }
      
      colFactors = new float[minValues.Length];

      for  (int x = 0;  x < minValues.Length;  x++)
      {
        float  range = maxValues[x] - minValues[x];
        if  (range == 0.0f)
          colFactors[x] = 1.0f;
        else
          colFactors[x] = chartWidth / range;
      }  
      
      TimeSpan  timeRange = timeTop - timeBot;
      int       timeRangeSecs = (int)timeRange.TotalSeconds;
      secsPerPixelRow   = (float)timeRangeSecs         / (float)totalPixelRows;
      metersPerPixelRow = (float)(depthBot - depthTop) / (float)totalPixelRows;
      

      if  (plotBuffer != null)
      {
        plotBuffer.Dispose ();
        plotBuffer = null;
      }

      plotDC        = chartPanel.CreateGraphics ();
      plotBuffer    = new Bitmap (chartPanel.Width, chartPanel.Height);
      plotBufferDC  = Graphics.FromImage(plotBuffer);
    }  /*  CalcChartVariables */


 

    public  void  ReSize ()
    {
      CalcChartVariables ();
    }  /* ReSize */




    public  void  SetFieldIdxs (int[,]  _fieldIdxs)
    {
      fieldIdxs = _fieldIdxs;
    }




    public  void  SetPlotByDepth ()
    {
      depthPlot       = true;
      timePlot        = false;
      plottingAxisIdx = 0;
    }  /* SetChartByDepth */



    public  void  SetPlotByTime ()
    {
      depthPlot       = false;
      timePlot        = true;
      plottingAxisIdx = 1;
    }  /* SetChartByDepth */




    public  void  UpdateRanges (float     _depthTop,
                                float     _depthBot,
                                DateTime  _timeTop,
                                DateTime  _timeBot
                               )
    {
      depthTop  = _depthTop;
      depthBot  = _depthBot;
      timeTop   = _timeTop;
      timeBot   = _timeBot;
    } /* UpdateRanges */




    public  void  UpdateMinMaxValues (float[] _minValues, 
                                      float[] _maxValues
                                     )
    {
      minValues = _minValues;
      maxValues = _maxValues;
      this.PaintChart ();
    }



    private   void  AssignColors ()
    {
      seriesBrushes = new SolidBrush[maxNumDataSets];
      int  x;

      for (x = 0; x < rulers.Length; x++)
      {
        seriesBrushes[x] = new SolidBrush (rulers[x].LineColor);
      }

    }  /* AssignColors */



    /// <summary>
    /// Specify which data series are to be plotted.
    /// </summary>
    /// <param name="dataToPlot"></param>
    public  void  AssignSeriesToPlot  (bool[]  dataToPlot)
    {
      seriesToPlot = new bool[maxNumDataSets];
      
      for  (int x = 0;  x < maxNumDataSets;  x++)
        seriesToPlot[x] = dataToPlot[fieldIdxs[plottingAxisIdx, x]];
    }  /* AssignSeriesToPlot */                                    
    


    private int  CalcRowForDepth (float d)
    {
      if  (d < depthTop)
        return  rowFirst;
      
      else if  (d > depthBot)
        return  rowLast;
       
    
      float  depthRange = depthBot - depthTop;
      float  offset = d - depthTop;
      float  rowRange = (float)(rowLast - rowFirst);
      
      int  r = rowFirst + (int)(0.5 + (offset / depthRange) * rowRange);

      return r;
    }  /* CalcRowForDepth */




    private int CalcRowForTime (DateTime  t)
    {
      if  (t.CompareTo (timeTop) > 0)
        return  rowFirst;
        
      else if  (t.CompareTo (timeBot) < 0)
        return  rowLast;
        
      TimeSpan  offset = timeTop - t;
      float     offsetSecs = (int)offset.TotalSeconds;

      int r = rowFirst + (int)(0.5f + offsetSecs / secsPerPixelRow);
      
      return  r;
    }  /* CalcRowForTime */
    



    private void DrawAxisLabelsDepth ()
    {
      int    numLabels = 11;
      float  numPixelsBetweenLabels = (float)(rowLast - rowFirst + 1) / (float)(numLabels - 1);
      float  unitsBetweenLabels     = (float)(depthBot - depthTop)    / (float)(numLabels - 1);
      Pen chartLine = new Pen (Color.DarkGray, 1);

      for (int x = 0; x < numLabels; x++)
      {
        float labelValue = depthTop + x * unitsBetweenLabels;
        int labelRow   = CalcRowForDepth (labelValue);
        plotDC.DrawLine (chartLine, chartFirstCol, labelRow, chartLastCol, labelRow);
        plotBufferDC.DrawLine(chartLine, chartFirstCol, labelRow, chartLastCol, labelRow);

        string label = labelValue.ToString ("###0.0").PadLeft (6);

        int labelTop = labelRow - drawFont.Height / 2;
        PointF drawPoint = new PointF (2, (float)labelTop);
             
        // Draw string to screen.
        plotDC.DrawString (label, drawFont, drawBrush, drawPoint);
        plotBufferDC.DrawString(label, drawFont, drawBrush, drawPoint);
      }
    }  /* DrawAxisLabelsDepth */



    private void DrawAxisLabelsTime ()
    {
      int     numLabels = 11;
      float   numPixelsBetweenLabels = (float)(rowLast - rowFirst + 1) / (float)(numLabels - 1);
      
      TimeSpan  timeRange = timeTop - timeBot;
      int       timeRangeSecs = (int)timeRange.TotalSeconds;
      float     secsBetweenLabels = ((float)timeRangeSecs / (float)(numLabels - 1));
      
      Pen chartLine = new Pen (Color.DarkGray , 1);

      for (int x = 0; x < numLabels; x++)
      {
        int offsetInSecs = (int)(0.5f + (float)x * secsBetweenLabels);
        DateTime  label = timeTop.AddSeconds (0 - offsetInSecs);
        string labelStr = label.ToString ("hh:mm:ss");

        int labelRow   = CalcRowForTime (label);
        plotDC.DrawLine(chartLine, chartFirstCol, labelRow, chartLastCol, labelRow);
        plotBufferDC.DrawLine(chartLine, chartFirstCol, labelRow, chartLastCol, labelRow);

        int labelTop = labelRow - drawFont.Height / 2;

        PointF drawPoint = new PointF(2, (float)labelTop);
        
        // Draw string to screen.
        plotDC.DrawString(labelStr, drawFont, drawBrush, drawPoint);
        plotBufferDC.DrawString(labelStr, drawFont, drawBrush, drawPoint);
      }

    }  /* DrawAxisLabelsTime */





    private void PlotOneDataRow (InstrumentData r,
                                 int           row
                                )
    {

      int seriesIdx;
      
      //g.FillRectangle (new SolidBrush (chartPanel.BackColor), 1, row, chartWidth - 2, this.chartRowWidth);

      float[] data = r.Data ();

      for (seriesIdx = 0;  seriesIdx < maxNumDataSets;  seriesIdx++)
      {
        if (!seriesToPlot[seriesIdx])
          continue;

        int dataIdx = fieldIdxs[plottingAxisIdx, seriesIdx];

        int   col = chartFirstCol;
        float d   = data[dataIdx];
        
        if  (d == float.NaN)
          col = chartLastCol;

        else if (d < minValues[dataIdx])
          col = chartFirstCol;

        else if (d > maxValues[dataIdx])
          col = chartLastCol;

        else
          col = chartFirstCol + (int)((d - minValues[dataIdx]) * colFactors[dataIdx] + 0.5f);
          
        if  (col < chartFirstCol)
          col = chartFirstCol;
          
        else if  (col > chartLastCol)
          col = chartLastCol;

        plotDC.FillRectangle(seriesBrushes[seriesIdx], col, row, chartColWidth, chartRowWidth);
        plotBufferDC.FillRectangle(seriesBrushes[seriesIdx], col, row, chartColWidth, chartRowWidth);
      }
    }   /* PlotOneDataRow */




    
    public  void  PaintChart ()
    {
      if  (timeBot == timeTop)
        timeBot = timeTop.AddSeconds (-3600.0);

      TimeSpan timeRange = timeTop - timeBot;
    
      //Graphics g = chartPanel.CreateGraphics ();
      
      plotDC.Clear (Color.Gray);
      plotBufferDC.Clear(Color.Gray);
      
      CalcChartVariables ();
    
      if  (timePlot)
        DrawTimeChart ();
        
      else  
        DrawDepthChart ();
    }  /* PaintChart */




    public void  PaintCurrentRow (InstrumentData curRow)
    {
      if  (timePlot)
      {
        DateTime t = curRow.Time;
        if (t.CompareTo (timeTop) > 0)
          return;

        if (t.CompareTo (timeBot) < 0)
          return;

        int row = CalcRowForTime(t);
        
        PlotOneDataRow (curRow, row);
      }
      else
      {
        float depth = curRow.Depth();
        if (depth < depthTop)
          return;

        if (depth > depthBot)
          return;

        int row = CalcRowForDepth (depth);
        PlotOneDataRow (curRow, row);
      }
      
    }  /* PaintCurrentRow */




    private  void  DrawTimeChart ()
    {
      DrawAxisLabelsTime ();

      int  numHistRecs = 0;
      lock (history)
      {
        numHistRecs = history.Count;
      }

      int  x;
      for  (x = 0;  x < numHistRecs;  x++)
      {
        InstrumentData  dataRow = (InstrumentData)(history[x]);
        DateTime  t = dataRow.Time;
        if  (t.CompareTo (timeTop) > 0)
          continue;
          
        if  (t.CompareTo (timeBot) < 0)
          continue;
          
        int  row = CalcRowForTime (t);
        PlotOneDataRow (dataRow, row);
      }
    }  /* DrawTimeChart */




    private void  DrawDepthChart ()
    {
      DrawAxisLabelsDepth ();
       
      int  historyNumOfRows = 0;
      lock (history)
      {
        historyNumOfRows = history.Count;
      }
      int  y;
      for  (y = 0;  y < historyNumOfRows;  y++)
      {
        InstrumentData  dataRow = (InstrumentData)(history[y]);
        float  depth = dataRow.Depth ();
        if  (depth < depthTop)
          continue;

        if (depth > depthBot)
          continue;

        int row = CalcRowForDepth (depth);
        PlotOneDataRow (dataRow, row);
      }
    }
  }  /* ChartSeries */
}
