using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using SipperFile;
using System.Runtime.InteropServices;
using System.Windows.Forms.DataVisualization.Charting;

using PicesInterface;



namespace SipperFileViewer
{
  public partial class SipperFileViewer : Form
  {
       
    private  Queue<String>               alarms                = new Queue<string> ();
    private  Queue<String>               alarmFileNames        = new Queue<string> ();
                              
    private  int                         autoScrollRate        = 0;
    private  int                         timerTicCount         = 0;
    
    private  InstrumentData              currentDataRow        = null;
    private  SipperConfigRec             configRec             = null;
    private  InstrumentDataLogger        dataLogger            = null;
    private  SipperFileBuffered          file                  = null;
    private  InstrumentDataList          history               = null;
    private  float                       navigateChartInterval = 1.0f;
    private  int                         navigateDataFieldIdx  = 14;  // 14 = Record Rate
    private  int                         navigateDataField2Idx = 3;   // 3  = Depth
    private  InstrumentDataManager       instrumentDataManager = null;
    private  InstrumentData              lastInsrumentDataDsiplayed = null;
    private  string                      lastFileOpenedName    = "";
    private  string                      sipperFileName        = "";
    private  SipperFileViewerParameters  parameters            = null;

    private  ArrayList                   imageEditors          = null;
    private  InstrumentDataReportText    userMessages          = null;
    
    private  TextBox[]                   dataFields            = null;
    private  Button[]                    dataFieldLabels       = null;
    private  int[]                       dataFieldIdxs         = null;  // Index into InstrumentData to use for dataField
    
    private  int                         rightMargin           = 0;

    private  TimePlot                    timePlot              = null;
    private  DepthPlot2                  depthPlot             = null;
    
    private  Stream                      initialStream         = null; 
    private  string                      initialFileName       = null; 
    private  string                      windowTitle           = "";    // When not empty will replace Window Title.

    private  string                      sipperFileViewerConfigFileName = "";

    private  bool                        useDataBase           = false;

    private  PicesRunLog                 runLog                = new PicesRunLog();

    private  int                         oldWidth  = 0;
    private  int                         oldHeight = 0;

    private  PicesDataBase               dbConn       = null;
    private  PicesSipperFile             dbSipperFile = null;

    private  float                       scanRate = 24950.0f;



    private  PicesDataBase  DbConn ()
    {
      if  (dbConn != null)
        return  dbConn;
      dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      return  dbConn;
    }


    public  int  BlobMinSize  
    {
      get {return parameters.BlobMinSize ();}  
      set {parameters.BlobMinSize (value); }
    }



    public SipperFileViewer ()
    {
      sipperFileViewerConfigFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "SipperFileViewer.txt";
      InitializeComponent ();
      Initialization ();
    }



    public SipperFileViewer (string  fileName)
    {
      sipperFileViewerConfigFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "SipperFileViewer.txt";
      initialFileName = fileName;
      InitializeComponent ();
      Initialization ();
    }



    public SipperFileViewer (Stream  stream,
                             string  _windowTitle,
                             string  _sipperFileName
                            )
    {
      sipperFileViewerConfigFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "SipperFileViewer.txt";
      initialStream  = stream;
      windowTitle    = _windowTitle;
      sipperFileName = _sipperFileName;
      
      InitializeComponent ();
      Initialization ();
    }


    public static  void  SipperFileViewerRegionAroundImage (PicesDataBaseImage  image,
                                                            PicesRunLog         runLog
                                                           )
    {
      if  (image == null)
        return;

      PicesDataBase  dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      if  (dbConn == null)
        return;
      
      uint  startScanLine = image.TopLeftRow - 4000;
      uint  endScanLine   = image.TopLeftRow + 6000;
      PicesInstrumentData  id =  dbConn.InstrumentDataGetByScanLine (image.SipperFileName, startScanLine);

      String  fullSipperFileName = dbConn.SipperFileGetFullPath (image.SipperFileName);
      if  (fullSipperFileName == null)
        MessageBox.Show  ("Could not locate SIPPER File[" + image.SipperFileName + "]", "SipperFileViewer");

      if  ((id != null)  &&  (id.ScanLine <= image.TopLeftRow)  &&  (fullSipperFileName != null))
      {
        ulong  byteOffsetStart = id.ByteOffset;
        id =  dbConn.InstrumentDataGetByScanLine (image.SipperFileName, endScanLine);
        if  (id != null)
        {
          ulong  byteOffsetEnd = id.ByteOffset;

          ulong  buffLen = byteOffsetEnd - byteOffsetStart;
          byte[]  buff = new byte[buffLen];

          System.IO.FileStream  file = null;
          try
          {
            file = new System.IO.FileStream (fullSipperFileName, System.IO.FileMode.Open, System.IO.FileAccess.Read);
          }
          catch  (Exception e3)
          {
            MessageBox.Show ("Could not open Sipper File[" + fullSipperFileName + "]" + "\n\n" + e3.ToString ());
            file = null;
          }
          
          if  (file != null)
          {
            try
            {
              file.Seek ((long)byteOffsetStart, System.IO.SeekOrigin.Begin);
              file.Read (buff, 0, buff.Length);
            }
            catch (Exception e2)
            {
              buff = null;
              MessageBox.Show ("Could not Read Sipper File[" + fullSipperFileName + "]" + "\n\n" + e2.ToString ());
            }
            
            if  (buff != null)
            {
              System.IO.MemoryStream memoryStream = new System.IO.MemoryStream (buff);
              SipperFileViewer  sfv = new SipperFileViewer (memoryStream, "Locale Window", image.SipperFileName);
              sfv.ShowDialog ();
              sfv.Dispose ();
              sfv = null;
              memoryStream.Dispose ();
              memoryStream = null;
            }

            buff = null;
            file.Close ();
            file = null;
          }
        }
      }

      dbConn.Close ();
      dbConn = null;
      GC.Collect ();
    }



    private  void  Initialization ()
    {
      SipperStreamPanel.MouseWheel += new System.Windows.Forms.MouseEventHandler (this.SipperStreamPanel_MouseWheel);

      oldWidth  = Width;
      oldHeight = Height;

      configRec = new SipperConfigRec ();
      useDataBase = configRec.UseDataBase ();

      parameters = new SipperFileViewerParameters (SipperStreamPanel);
      currentDataRow = new InstrumentData ();
      imageEditors = new ArrayList ();
      BuildDataFields ();
      
      rightMargin = this.Width - FileDescription.Right;

      LoadSipperFileViewerConfiuration ();

      SetTitleBar ();

    }  /* Initialization */



    private void  SetTitleBar ()
    {
      if  (dbConn == null)
      {
        Text = "Sipper File Viewer    (not conected)";
      }
      else
      {
        Text = "Sipper File Viewer    (" + dbConn.ServerDescription () + ")";
      }
    }


    private  void  BuildDataFields ()
    {
      dataFields      = new TextBox[5];
      dataFieldLabels = new Button[5];
      
      dataFieldIdxs   = new int[5];
      
      dataFields[0] = Data0;
      dataFields[1] = Data1;
      dataFields[2] = Data2;
      dataFields[3] = Data3;
      dataFields[4] = Data4;
      
      dataFieldLabels[0] = Data0Label;
      dataFieldLabels[1] = Data1Label;
      dataFieldLabels[2] = Data2Label;
      dataFieldLabels[3] = Data3Label;
      dataFieldLabels[4] = Data4Label;

      for  (int x = 0;  x < dataFields.Length;  x++)
      {
        dataFields[x].Text = "0";
        dataFieldLabels[x].Text = InstrumentData.FieldName (x);
        dataFieldLabels[x].Click += new EventHandler (DataFieldLabelClicked);
        dataFieldIdxs[x] = x;
      }

    }  /* BuildDataFields */
  


    private  void  LoadSipperFileViewerConfiuration ()
    {
      StreamReader f = null;

      int  tempCropColLeft  = -1;
      int  tempCropColRight = -1;

      try
      {
        f = new StreamReader (sipperFileViewerConfigFileName);
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
          String[]  fields = txtLine.Split ('\t');
          if  (fields.Length > 1)
          {
            string   rowType = fields[0].ToLower ().Trim ();
          
            if  (rowType== "datafield")
            {
              if  (fields.Length >= 3)
              {
                int  dataFieldNum = PicesKKStr.StrToInt (fields[1]);
                if  ((dataFieldNum >= 0)  &&  (dataFieldNum < 5))
                {
                  string  fieldName = fields[2];
                  int  dataFieldIndex = InstrumentData.GetFieldNameIndex (fieldName);
                  if  (dataFieldIndex >= 0)
                  {
                    dataFieldLabels[dataFieldNum].Text =  InstrumentData.FieldName (dataFieldIndex);
                    dataFieldIdxs  [dataFieldNum] = dataFieldIndex;
                  }
                }
              }
            }

            else if  (rowType== "cropcolleft")
              tempCropColLeft = PicesKKStr.StrToInt (fields[1]);

            else if  (rowType== "cropcolright")
              tempCropColRight = PicesKKStr.StrToInt (fields[1]);

            else if  (rowType == "lastfileopenedname")
            {
              if  (fields.Length >= 2)
                lastFileOpenedName = fields[1];
            }

            else if  (rowType == "navigationdatatoplot")
            {
              if  (fields.Length >= 2)
                NavigationDataToPlot.Text = fields[1];
            }

            else if  (rowType == "navigationdatatoplot2")
            {
              if  (fields.Length >= 2)
                NavigationDataToPlot2.Text = fields[1];
            }

            else if  (rowType == "endoffile")
              break;
          }
        }
        
        txtLine = f.ReadLine();
      }

      navigateDataFieldIdx = InstrumentData.GetFieldNameIndex (NavigationDataToPlot.Text);
      if  (navigateDataFieldIdx < 0)
        navigateDataFieldIdx = 14;
      NavigationDataToPlot.Text = InstrumentData.FieldName (navigateDataFieldIdx);
      
      navigateDataField2Idx = InstrumentData.GetFieldNameIndex (NavigationDataToPlot2.Text);
      if  (navigateDataField2Idx < 0)
        navigateDataField2Idx = 3;
      NavigationDataToPlot2.Text = InstrumentData.FieldName (navigateDataField2Idx);

      if  (tempCropColLeft >= 0)
        parameters.SetCropColumnLeft (tempCropColLeft);

      if  (tempCropColRight >= 0)
        parameters.SetCropColumnRight (tempCropColRight);

      f.Close();
    }  /* LoadSipperFileViewerConfiuration */



    private  void  SaveSipperFileViewerConfiguration ()
    {
      StreamWriter w = null;
      try
      {
        w = new StreamWriter (sipperFileViewerConfigFileName);
      }
      catch (Exception e)
      {
        MessageBox.Show ("Exception Writing File Viewer Configuration.  [" + e.ToString() + "]");
        return;
      }

      w.WriteLine ("// SipperFileViewer Configuration");
      w.WriteLine ("// DateTime" + "\t" + DateTime.Now.ToString());
      w.WriteLine ("//");
      w.WriteLine ("//");
      w.WriteLine ("//  The next line specifies that name of the last Sipper File opened.");
      w.WriteLine ("LastFileOpenedName"    + "\t" + lastFileOpenedName);
      w.WriteLine ("NavigationDataToPlot"  + "\t" + NavigationDataToPlot.Text);
      w.WriteLine ("NavigationDataToPlot2" + "\t" + NavigationDataToPlot2.Text);
      w.WriteLine ("cropColLeft"           + "\t" + parameters.CropColemnLeft  ());
      w.WriteLine ("cropColRight"          + "\t" + parameters.CropColemnRight ());
      w.WriteLine ("//");
      w.WriteLine ("//");
      w.WriteLine ("// The following [" + dataFieldLabels.Length.ToString() + "] lines specify which data fields are to be displayed.");
      int  x;
      for (x = 0;  x < dataFieldLabels.Length;  x++)
      {
        w.WriteLine ("DataField" + "\t" + x.ToString () + "\t" + dataFieldLabels[x].Text);
      }

      w.WriteLine ("EndOfFile");

      w.Close();
      
      return;
    }  /* SaveConfiguration */






    private  void  SetSipperFileName (string  fileName)
    {
      SetUpDataStructuresForNewFile ();
    
      FileStream  fs = null;
    
      try
      {
        // Open and allow others to open for read
        fs = new FileStream (fileName,FileMode.Open, FileAccess.Read, FileShare.Read);
      }
      catch  (Exception e)
      {
        MessageBox.Show (e.ToString (), "Opening SIPPER file[" + fileName + "]");
        fs = null;
      }
      
      if  (fs != null)
      {
        sipperFileName = fileName;
        InitializeForNewDataFile (fileName, fs);
        lastFileOpenedName = fileName;
        SaveSipperFileViewerConfiguration ();
      }
    }  /* SetSipperFileName */




    private  void  SetSipperFileStream (Stream  stream)
    {
      SetUpDataStructuresForNewFile ();
      InitializeForNewDataFile ("", stream);
    }  /* SetSipperFileStream */




    private  void  SetUpDataStructuresForNewFile ()
    {
      DisposeExistingDataStructures ();
      parameters.SetNewDisplayRowTop (0);
      parameters.SetSipperFile (null, null, null);
      parameters.ComputeParameters ();
    
      FileDescription.Text = "";
      FileName.Text = "";
      
      imageEditors = new ArrayList ();
    } /* SetUpDataStructuresForNewFile */



    
    private  void  DisposeExistingDataStructures ()
    {
      if  (imageEditors != null)
      {
        int  x;  
        for  (x = 0;  x < imageEditors.Count;  x++)
        {
          ImageEditor  ie = (ImageEditor)(imageEditors[x]);
          if  (!ie.IsDisposed)
            ie.Dispose ();
          ie = null;
          imageEditors[x] = null;
        }
        
        imageEditors = null;
      }


      if  (file != null)
      {
        file.Dispose ();
        file = null;
      }
      
      if  (timePlot != null)
      {
        timePlot.Dispose ();
        timePlot = null;
      }
      
      if  (depthPlot !=null)
      {
        depthPlot.Dispose ();
        depthPlot = null;
      }
    
      if  (instrumentDataManager != null)
      {
        instrumentDataManager.Dispose ();
        instrumentDataManager = null;
      }
      
      if  (history != null)
      {
        history.Dispose ();
        history = null;
      }
      
     
      if  (userMessages != null)
      {
        userMessages.Dispose ();
        userMessages = null;
      }

      if  (dataLogger != null)
      {
        dataLogger.Dispose ();
        dataLogger = null;
      }
    } /* DisposeExistingDataStructures */

  

    private  PicesSipperFile  GetSipperFileInfo (string fileName)
    {
      PicesSipperFile  sf = null;

      if  ((dbConn == null)  &&  (useDataBase))
      {
        dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
        if  ((dbConn == null)  ||  (!dbConn.Valid ()))
          dbConn = PicesDataBase.SelectNewDataBaseServer (runLog);
      }

      if  (dbConn != null)
        sf = dbConn.SipperFileRecLoad (OSservices.GetRootName (fileName));

      if  (sf == null)
      {
        sf  = new PicesSipperFile (fileName);

        if  (configRec == null)
          configRec = new SipperConfigRec ();

        sf.Sp0 = configRec.HeaderRec().SerialPortShortName (0);
        sf.Sp1 = configRec.HeaderRec().SerialPortShortName (1);
        sf.Sp2 = configRec.HeaderRec().SerialPortShortName (2);
        sf.Sp3 = configRec.HeaderRec().SerialPortShortName (3);

        sf.CtdExt0 = configRec.HeaderRec ().CtdExt0Code ();
        sf.CtdExt1 = configRec.HeaderRec ().CtdExt1Code ();
        sf.CtdExt2 = configRec.HeaderRec ().CtdExt2Code ();
        sf.CtdExt3 = configRec.HeaderRec ().CtdExt3Code ();
      }

      SetTitleBar ();

      return  sf;
    } /* GetSipperFileInfo */



    private  void  InitializeForNewDataFile (string  fileName,
                                             Stream  stream
                                            )
    {
      dbSipperFile = GetSipperFileInfo (OSservices.GetRootName (fileName));

      if  (dbSipperFile != null)
        scanRate = dbSipperFile.ScanRate;
      else
        scanRate = 24950.0f;

      dataLogger = new InstrumentDataLogger ();

      instrumentDataManager = new InstrumentDataManager (dataLogger, 
                                                         dbSipperFile,
                                                         configRec,
                                                         currentDataRow
                                                        );
      userMessages = new InstrumentDataReportText (instrumentDataManager, 6);
      instrumentDataManager.AssignReporter (6, userMessages);
      
      try
      {
        file = new SipperFileBuffered (stream, instrumentDataManager);
      }
      catch  (Exception e)
      {
        MessageBox.Show (e.ToString (), "Not a valid a SIPPER Format[" + fileName + "]");
        file = null;
      }
      
      if  (file != null)
      {
        history = new InstrumentDataList ();
        NavigateChart.Series[0].Points.Clear ();

        FileDescription.Text = userMessages.AllMessages ();
        
        file.FrameBytesOffsetsUpdateThreadStart (history);
        FileName.Text = sipperFileName;
        parameters.SetSipperFile (file, dbSipperFile, dbConn);
        parameters.PaintWholePanel ();
        UpdateStats ();
        UpdateVerticleScrollBar ();
      }
    }  /* InitializeForNewDataFile */



    private void  UpdateStats ()
    {
      if  (file == null)
        return;

      float  ratio = parameters.Ratio ();
    
      int  scanLineTop = (int)Math.Floor ((float)parameters.DisplayRowTop () * ratio);
      int  frameNumTop = scanLineTop / Sipper3File.ScanLinesPerFrame;
      
      int  scanLineBot = (int)Math.Ceiling ((float)(parameters.DisplayRowBot () + 1) * ratio) - 1;
      int  frameNumBot = (int)Math.Ceiling ((double)scanLineBot / (double)Sipper3File.ScanLinesPerFrame);
      if  (frameNumBot > file.LargestKnowmFrameNum ())
        frameNumBot = file.LargestKnowmFrameNum ();
        
      ScanLinesDisplayedFirst.Text = scanLineTop.ToString ("###,###,##0");
      ScanLinesDisplayedLast.Text  = scanLineBot.ToString ("###,###,##0");
      FrameNumDisplayedFirst.Text  = frameNumTop.ToString ("###,##0");
      
      cropColLeft.Text  = parameters.CropColemnLeft  ().ToString ("#,##0");
      cropColRight.Text = parameters.CropColemnRight ().ToString ("#,##0");

      if  (file != null)
          SanLinesTotal.Text = file.LargestKnownScanLine ().ToString ("###,###,##0");
       
      if  (history != null)
      {
        if  (history.Count > frameNumBot)
        {
          lastInsrumentDataDsiplayed = (InstrumentData)history[frameNumBot];
          
          for  (int x =0;  x < dataFieldIdxs.Length;  x++)
          {
            dataFields[x].Text = lastInsrumentDataDsiplayed.GetField (dataFieldIdxs[x]);
          }
        }
      }
          
    } /* UpdateStats */



    private  void  UpdateScreenForNewSize ()
    {
      if  (parameters == null)
        return;

      if  ((this.Height < 10)  ||  (this.Width < 10))
        return;
    
      int  changeInWidth  = Width  - oldWidth;
      int  changeInHeight = Height - oldHeight;
    
      oldWidth  = Width;
      oldHeight = Height;
    
      int  newWidth  = this.Width;
      int  newHeight = this.Height;
      
      int  panelLeftCol = SipperStreamPanel.Left;
      int  panelTopRow  = SipperStreamPanel.Top;
      int  scrollBarWidth = vScrollBar1.Width;
      
      NavigationDataToPlot.Left  += changeInWidth;
      NavigationDataToPlot2.Left += changeInWidth;
      
      int  avaiableColsToWorkWith = Math.Max (100, newWidth  - (panelLeftCol + scrollBarWidth + 20));
      int  avaiableRowsToWorkWith = Math.Max (100, (newHeight - (panelTopRow + 50)));
      
      SipperStreamPanel.Width  += changeInWidth;
      SipperStreamPanel.Height += changeInHeight;
      
      vScrollBar1.Left   += changeInWidth;
      vScrollBar1.Height += changeInHeight;

      NavigateChart.Left   += changeInWidth;
      NavigateChart.Height += changeInHeight;
      
      ScanLinesDisplayedLast.Top      += changeInHeight;
      ScanLinesDisplayedLastLabel.Top += changeInHeight;
      
      int  slw =  TotalScanLinesLabel.Width + SanLinesTotal.Width;
      TotalScanLinesLabel.Left = newWidth - slw - rightMargin;
      SanLinesTotal.Left       = TotalScanLinesLabel.Left + TotalScanLinesLabel.Width;
      
      FileName.Width         =  TotalScanLinesLabel.Left - FileName.Left - 5;
      FileDescription.Width  =  newWidth - FileDescription.Left - rightMargin;
      
      int  mcw = MouseCoordinatesLabel.Width + MouseCoordinates.Width;
      MouseCoordinatesLabel.Left =  newWidth - mcw - rightMargin;
      MouseCoordinates.Left      =  MouseCoordinatesLabel.Left + MouseCoordinatesLabel.Width;
            
      cropColRight.Left = cropColRight.Left + changeInWidth;
            
      int  newAutoScrollingPanelLeft = autoDetect.Right + (((MouseCoordinatesLabel.Left - autoDetect.Right) - AutoScrollingPanel.Size.Width) / 2);
      AutoScrollingPanel.Left = newAutoScrollingPanelLeft;
      
      ClearRectangle ();

      parameters.ComputeParameters ();
      parameters.PaintWholePanel ();

      UpdateStats ();
      UpdateVerticleScrollBar ();
    }  /* UpdateScreenForNewSize */


    
    private  void  UpdateVerticleScrollBar ()
    {
      int   maxNumDisplayRows = 20000;
      if  (file != null)
        maxNumDisplayRows = (int)((float)file.LargestKnownScanLine() / parameters.Ratio ());
      
      vScrollBar1.Minimum = 0;
      vScrollBar1.Maximum = maxNumDisplayRows;

      int  value = (int)parameters.DisplayRowTop ();
      if  (value > maxNumDisplayRows)
      {
        //value = maxNumDisplayRows - Sipper3File.ScanLinesPerFrame; 
        value = maxNumDisplayRows - parameters.DisplayRowsToDisplay ();
        if  (value < 0)
          value = 0;
      }

      vScrollBar1.Value   = value;
      //vScrollBar1.LargeChange = Sipper3File.ScanLinesPerFrame;
      vScrollBar1.LargeChange = parameters.DisplayRowsToDisplay ();
      vScrollBar1.SmallChange = 2;
    }  /* UpdateVerticleScrollBar */


    
    int  chartStripLineCurPosIdx   = -1;
    int  chartStripLineStartPosIdx = -1;
    int  chartStripLineEndPosIdx   = -1;


    private  void  ChartAddCurrentPosition ()
    {
      UpdateSripLine ((uint)(0.5f + parameters.Ratio () * (float)parameters.DisplayRowTop ()), 
                      ref chartStripLineCurPosIdx, 
                      Color.DarkSlateBlue
                     );

      if  ((dbSipperFile == null) ||  (file == null))
        return;

      if  (dbSipperFile.ExtractionScanLineStart <= file.LargestKnownScanLine ())
        UpdateSripLine (dbSipperFile.ExtractionScanLineStart, ref chartStripLineStartPosIdx, Color.DarkGreen);

      if  (dbSipperFile.ExtractionScanLineEnd <= file.LargestKnownScanLine ())
        UpdateSripLine (dbSipperFile.ExtractionScanLineEnd, ref chartStripLineEndPosIdx, Color.DarkRed);
    }  /* ChartAddCurrentPosition */



    private  void  UpdateSripLine (uint     scanLine,
                                   ref int  idx,
                                   Color    c
                                  )
    {
      if  (scanRate < 1000)
        scanRate = 24950.0f;
      double  offsetInSecs = ((float)(scanLine) / scanRate);

      if  (idx < 0)
      {
        StripLine  sl = new StripLine ();
        sl.BorderColor = c;
        sl.Interval = 1000000;
        sl.IntervalOffset = offsetInSecs;
        sl.IntervalType = DateTimeIntervalType.Seconds;
        sl.StripWidth = 0.0f;
        sl.BorderWidth = 2;
        sl.StripWidthType = DateTimeIntervalType.Seconds;
        idx = NavigateChart.ChartAreas[0].AxisY.StripLines.Count;
        NavigateChart.ChartAreas[0].AxisY.StripLines.Add (sl);
      }
      else
      {
        NavigateChart.ChartAreas[0].AxisY.StripLines[idx].IntervalOffset = offsetInSecs;
      }
    }   /* UpdateSripLine */




    private void  NavigateChart_MouseClick (object sender, MouseEventArgs e)
    {
      Point  p = e.Location;
      ChartArea  ca = NavigateChart.ChartAreas[0];
      double yValue = ca.AxisY.PixelPositionToValue ((double)p.Y);
      double offsetInSecs = DateTime.FromOADate (yValue).TimeOfDay.TotalSeconds;

      int  offsetInScanLines = (int)(0.5 + offsetInSecs * scanRate);
      if  (offsetInScanLines > file.LargestKnownScanLine ())
        offsetInScanLines = (int)file.LargestKnownScanLine ();
      
      int  displayRow = (int)(0.5f + (float)offsetInScanLines / file.Ratio ());
      if  (displayRow > vScrollBar1.Maximum)
        displayRow = vScrollBar1.Maximum;

      vScrollBar1.Value = displayRow;
      ScrollToPropperPlaceInSipperFile2 (displayRow);
    }





    private  void  ComputeOutlierBounds (float[]    data, 
                                         ref float  lowerBound, 
                                         ref float  upperBound
                                        )
    {
      if  ((data == null)  ||  (data.Length < 1))
        return;

      int  x = 0;

      float  total = 0.0f;
      for  (x = 0;  x < data.Length;  ++x)
        total += data[x];
      float  mean = total / data.Length;

      double  totalDeltaSquared = 0.0f;
      for  (x = 0;  x < data.Length;  ++x)
      {
        float  delta = data[x] - mean;
        double  deltaSquared = delta * delta;
        totalDeltaSquared += deltaSquared;
      }

      double variance = totalDeltaSquared / (double)data.Length;
      float stdDev = (float)Math.Sqrt (variance);

      lowerBound = mean - stdDev * 6;
      upperBound = mean + stdDev * 6;
    }





    private  List<double>  GetNavigationPlotData (ref int   instrDataIdx,
                                                  Button    button
                                                 )
    {
      if  (history == null)
        return null;

      if  ((instrDataIdx < 0)  ||  (instrDataIdx >= InstrumentData.NumDataFields ()))
        instrDataIdx = InstrumentData.NumDataFields () - 1;
      button.Text = InstrumentData.FieldName (instrDataIdx);

      if  (scanRate < 1000)
        scanRate = 24950.0f;

      double  timePerHistoryRow = 4098.0f / scanRate;
      double  rowsPerChartPoint = (navigateChartInterval / timePerHistoryRow);
      int  historySize = history.Count;
      int  z = (int)(0.5 + (double)historySize / rowsPerChartPoint);

      int x = 0;

      float[]  data = new float[historySize];
      for  (x = 0;  x < historySize;  ++x)
      {
        InstrumentData id = (InstrumentData)(history[x]);
        data[x] = id.Data () [instrDataIdx];
      }

      float  lowerBound = 0.0f, upperBound = 0.0f;
      ComputeOutlierBounds (data, ref lowerBound, ref upperBound);

      List<double>  results = new List<double> (z);
      int historyLimit = (int)((double)historySize - rowsPerChartPoint);



      int  histRangeStartIdx = 0;

      for  (int resultIdx = 0;  resultIdx < z;  ++resultIdx)
      {
        int  histRangeEndIdx = (int)(0.5 + rowsPerChartPoint * (double)(resultIdx + 1));
        if  (histRangeEndIdx > historySize)
          histRangeEndIdx = historySize;


        int  histIdx = 0;
        float  dv = 0.0f;

        int  rowsIncluded = 0;

        for  (histIdx = histRangeStartIdx;  histIdx < histRangeEndIdx;  ++histIdx)
        {
          float f = data[histIdx];
          if  ((f >= lowerBound)  &&  (f <= upperBound))
          {
            dv += f;
            ++rowsIncluded;
          }
          ++x;
        }
        if  (rowsIncluded > 0)
          dv /= (float)rowsIncluded; 
        else
          dv = (lowerBound + upperBound) / 2.0f;
        results.Add (dv);

        histRangeStartIdx = histRangeEndIdx;
      }

      return  results;
    }  /* GetNavigationPlotData */



    private  DateTime  navigateChartStartTime = new DateTime (2000, 1, 1);


    private  void  UpdateNavigateChart ()
    {
      NavigateChart.Series[0].Points.Clear ();
      NavigateChart.Series[1].Points.Clear ();
      List<double>  plotData  = GetNavigationPlotData (ref navigateDataFieldIdx,  NavigationDataToPlot);
      List<double>  plotData2 = GetNavigationPlotData (ref navigateDataField2Idx, NavigationDataToPlot2);
      if  (plotData != null)
      {
        double  offset = 0.0;
        for  (int x = 0;  x < plotData.Count;  ++x)
        {
          DateTime d = navigateChartStartTime.AddSeconds (offset);
          NavigateChart.Series[0].Points.AddXY (plotData[x], d);
          NavigateChart.Series[1].Points.AddXY (plotData2[x], d);
          offset += navigateChartInterval;
        }

        NavigateChart.ChartAreas[0].RecalculateAxesScale ();
      }
    }  /* UpdateNavigateChart */



    private  void  ScrollToPropperPlaceInSipperFile2 (int  displayRow)
    {
      if  (file == null)
        return;
        
      int  largestKnownDisplayRow =  (int)((float)file.LargestKnownScanLine () / parameters.Ratio ());
        
      if  ((file.EofReached) &&  (displayRow > (largestKnownDisplayRow - parameters.DisplayRowsToDisplay ())))
        return;
    
      int  deltaDisplayRows = (int)(displayRow - parameters.DisplayRowTop ());
     
      if  (Math.Abs (deltaDisplayRows) < 200)
      {
        parameters.ScrollRows2 (deltaDisplayRows);
      }
      else
      {
        parameters.SetNewDisplayRowTop (displayRow);
        parameters.PaintWholePanel ();
      }

      ChartAddCurrentPosition ();
    }  /* ScrollToPropperPlaceInSipperFile2 */




    private void openSipperFileToolStripMenuItem_Click (object sender, EventArgs e)
    {
      OpenFileDialog  fileDialog = new OpenFileDialog ();
      fileDialog.InitialDirectory = "";
      fileDialog.FileName = "";

      if  (lastFileOpenedName != "")
      {
        FileInfo fi = new FileInfo (lastFileOpenedName);
        fileDialog.InitialDirectory = fi.Directory.ToString ();

        fileDialog.FileName = fi.Name;
      }

      if  (fileDialog.InitialDirectory == "")
      {
        fileDialog.InitialDirectory = PicesSipperVariables.SipperFilesDirectory ();
      }

      fileDialog.Filter = "Sipper Files|*.spr;|All Files|*.*";
      fileDialog.FilterIndex = 0;
      fileDialog.RestoreDirectory = true;
      fileDialog.CheckFileExists = true;
      if (fileDialog.ShowDialog() == DialogResult.OK)
      {
        SetSipperFileName (fileDialog.FileName);
      }
    }  /* openSipperFileToolStripMenuItem_Click */



    private void SipperStreamPanel_Paint (object sender, PaintEventArgs e)
    {
      parameters.PaintFromBuffer ();
    }



    private void  configurationToolStripMenuItem_Click(object sender, EventArgs e)
    {
      SipperConfigForm configForm = new SipperConfigForm (configRec);
      configForm.ShowDialog();
      if (configForm.UserSavedDataToFile())
      {
        useDataBase = configRec.UseDataBase ();
        //sipperFileName.Text = sipperConfigRec.HeaderRec().SipperFileName();
      }
    }



    private void SipperFileViewer_Load (object sender, EventArgs e)
    {
      StatusUpdateTimer.Enabled = true;
      if  (initialStream != null)
      {
        // We were provided with a file stream that we should start with.
        SetSipperFileStream (initialStream);
        menuStrip1.Enabled = false;
        if  (windowTitle != "")
          this.Text = windowTitle;
      }
      
      else if  ((initialFileName != null)  &&  (initialFileName != ""))
      {
        SetSipperFileName (initialFileName);
      }
            
      autoDetect.Checked = parameters.ExtractBlobs ();
    }  /* SipperFileViewer_Load */




    private void StatusUpdateTimer_Tick (object sender, EventArgs e)
    {
      timerTicCount++;
      AutoScrollSipperData ();
      if  ((timerTicCount % 20) == 0)
      {
        UpdateStats ();
        UpdateVerticleScrollBar ();
        UpdateNavigateChart ();
      }

      if  (alarms.Count > 0)
      {
        StatusUpdateTimer.Enabled = false;

        while  (alarms.Count > 0)
        {
          Thread.Sleep (100);
          String  alarmMsg = "";
          String  alarmLogFileName = "";
          lock  (alarms)
          {
            alarmMsg = alarms.Dequeue ();
            alarmLogFileName = alarmFileNames.Dequeue ();
          }

          if  (String.IsNullOrEmpty (alarmLogFileName))
          {
            MessageBox.Show (this, alarmMsg, "Alarm",  MessageBoxButtons.OK);
          }
          else
          {
            alarmMsg += "\n";
            alarmMsg += "\n";
            alarmMsg += "Click on Yes to see log file.";

            DialogResult  dr = MessageBox.Show (this, alarmMsg, "View Log File",  MessageBoxButtons.YesNo);
            if  (dr == DialogResult.Yes)
            {
              Process proc = new Process();
     
              proc.StartInfo.WorkingDirectory = @"";
              proc.StartInfo.FileName = "notepad.exe";
              proc.StartInfo.Arguments = alarmLogFileName;
              proc.StartInfo.UseShellExecute = false;
              proc.StartInfo.RedirectStandardOutput = false;
              proc.StartInfo.RedirectStandardError = true;
              proc.Start();
              proc.WaitForExit();
              proc.Close();
            }
          }
        }

        StatusUpdateTimer.Enabled = true;
      }
    }



    private void SipperFileViewer_SizeChanged(object sender, EventArgs e)
    {
      if  (WindowState == FormWindowState.Minimized)
        return;
      UpdateScreenForNewSize();
    }


    int  x1, y1, x2, y2;
    bool  definingRectangle = false;
    
    
    
    private  void  ClearRectangle ()
    {
      definingRectangle = false;
    }
    
    
    
    private  void  PaintRectangle ()
    {
      float  ratio = parameters.Ratio ();
      float  oneHalfRatio = (ratio / 2.0f);
      int  middleRow = (int)(0.5f + oneHalfRatio + ratio * ((y1 + y2) / 2.0f));
      int  middleCol = (int)(0.5f + oneHalfRatio + ratio * ((x1 + x2) / 2.0f));
      int  scanLine =  (int)((float)parameters.DisplayRowTop () * ratio) + middleRow;
      MouseCoordinates.Text = scanLine.ToString() + ", " + middleCol.ToString ();    
      parameters.PaintTemporayRectangle (Color.Red, x1, y1, x2, y2);
    }  /* PaintRectangle */

    

    

    //*********************************************************************************************
    //
    //  Folowing code was lifted from web site below
    // http://groups.google.com/group/microsoft.public.dotnet.languages.csharp/browse_thread/thread/4c541e081043cf4b/99e885ef8f09cc78?lnk=st&q=c%23++MouseWheel++Event++Panel&rnum=2&hl=en#99e885ef8f09cc78
    //
    //  It intercepts the Wheel mouse movements and forces them to generate an
    //  event.
    //
    //
    //*********************************************************************************************
    bool sendingMessage = false; 
    protected override void WndProc (ref Message m) 
    { 
      Point mp = MousePosition; 
      if (m.Msg == 522) 
      { 
         if (sendingMessage) 
         { 
           m.Result = (IntPtr)0; 
          return; 
         } 
          
         //Debug.Print("Mouse Wheel: lparam = {0},WParam = {1}, Result ={2}", 
         //            m.LParam, m.WParam, m.Result
         //           );
          
         try
         { 
           sendingMessage = true; 
           SendMessage (GetChildAtPoint (PointToClient(mp)).Handle, 
                        m.Msg, 
                        m.WParam.ToInt32(), 
                        m.LParam.ToString()
                       ); 
           sendingMessage = false; 
         } 
         catch  (Exception exception)
        { 
           string s = exception.ToString (); 
         } 
         m.Result = (IntPtr)0; 
      } 
      base.WndProc(ref m);
    } /* WndProc */



    [DllImport("user32.dll")] 
      public static extern int SendMessage (IntPtr hwnd, 
                                            [MarshalAs(UnmanagedType.U4)] 
                                            int Msg, 
                                            int wParam, 
                                            [MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)] 
                                            string lParam
                                           );
//*********************************************************************************************



 
    private void  SipperStreamPanel_MouseWheel (object sender, MouseEventArgs e)
    {
      int delta = e.Delta / 3;
      
      int  newTopDisplayRow = (int)(parameters.DisplayRowTop () - delta);
      if  (newTopDisplayRow < 0)
        newTopDisplayRow = 0;
      
      ScrollToPropperPlaceInSipperFile2 (newTopDisplayRow);
      ClearRectangle ();
      UpdateVerticleScrollBar ();
    }  /* SipperStreamPanel_MouseWheel */



    private  void  AutoScrollingRateTextSet ()
    {
      AutoScrollingRate.Text = autoScrollRate.ToString ();
      if  (autoScrollRate >= 0)
        AutoScrollingRate.ForeColor = Color.Blue;
      else
        AutoScrollingRate.ForeColor = Color.Red;
    }  /* AutoScrollingRateTextSet */



    private  void  AutoScrollSipperData ()
    {
      if  (autoScrollRate == 0)
        return;

      int  newTopDisplayRow = (int)(parameters.DisplayRowTop () + autoScrollRate);
      if  (newTopDisplayRow < 0)
      {
        autoScrollRate = 0;
        newTopDisplayRow = 0;
        AutoScrollingRateTextSet ();
      }

      ScrollToPropperPlaceInSipperFile2 (newTopDisplayRow);
    }  /* AutoScrollSipperData */


    
    private void SipperFileViewer_KeyDown (object sender, KeyEventArgs e)
    {
      Keys k = e.KeyCode;

      int  newTopDisplayRow = -1;
      if  (k == Keys.PageDown)
        newTopDisplayRow = (int)(parameters.DisplayRowTop () + parameters.DisplayRowsToDisplay ());

      else if (k == Keys.PageUp)
        newTopDisplayRow = (int)(parameters.DisplayRowTop () - parameters.DisplayRowsToDisplay ());

      if  (newTopDisplayRow > 0)
      {
        ScrollToPropperPlaceInSipperFile2 (newTopDisplayRow);
        ClearRectangle();
        UpdateVerticleScrollBar();
      }
    }


 
    private void SipperStreamPanel_MouseDoubleClick (object sender, MouseEventArgs e)
    {
      Blob  selectedBlob = parameters.SelectedBlob (e.X, e.Y);
      
      if  (selectedBlob != null)
      {
        // User has selected region to work on seperatly.
        
        long  startScanLine = 0;
        int   startCol      = 0;
        
        byte[][]  selectedRegion = parameters.ExtractImageFromSipper (selectedBlob,
                                                                      ref startScanLine,
                                                                      ref startCol
                                                                     );
        if  (selectedRegion == null)
          return;

        string  fileName = OSservices.GetRootName (sipperFileName) + "_" +
                           startScanLine.ToString ("00000000")     + "_" +
                           startCol.ToString ("0000") +
                           ".bmp";
                           
        int  height = selectedRegion.Length;
        int  width  = selectedRegion[0].Length;
        
        ImageEditor  imageEditor = new ImageEditor (fileName, 
                                                    height, 
                                                    width, 
                                                    selectedRegion, 
                                                    lastInsrumentDataDsiplayed
                                                   );
        
        imageEditor.Show ();
        imageEditors.Add (imageEditor);
        ClearRectangle ();
      }
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
   


    private void SipperStreamPanel_MouseDown (object sender, MouseEventArgs e)
    {
      if  (parameters.SelectedBlob (e.X, e.Y) == null)
      {
        x1 = e.X;
        y1 = e.Y;
        definingRectangle = true;
      }
    }


    private void SipperStreamPanel_MouseUp (object sender, MouseEventArgs e)
    {
      if  (definingRectangle)
      {
        x2 = e.X;
        y2 = e.Y;
        
        int  top, bot, left, right;
         
        top   = Math.Max (y1, y2);
        bot   = Math.Min (y1, y2);
        left  = Math.Min (x1, x2);
        right = Math.Max (x1, x2);

        parameters.AddUserSpecifiedBlob (top, left, bot, right);
        definingRectangle = false;

        parameters.PaintAllBlobs ();
      }
    }



    private void  SipperStreamPanel_MouseMove (object sender, MouseEventArgs e)
    {
      if  (definingRectangle)
      {
        x2 = e.X;
        y2 = e.Y;
        PaintRectangle ();
      }
    }



    private void   saveInstrumentDataToolStripMenuItem_Click (object sender, EventArgs e)
    {
      if  (history == null)
      {
        MessageBox.Show ("No Instrument Data Loaded", "Save Instrument Data", MessageBoxButtons.OK);
        return;
      }

      string  fileName = OSservices.GetRootName (sipperFileName) + "_InstrumentData.txt";

      SaveFileDialog  saveDialog = new SaveFileDialog ();
      saveDialog.InitialDirectory = "";
      saveDialog.Filter = "(TextFile)|*.txt";
      saveDialog.FilterIndex = 0;
      saveDialog.RestoreDirectory = true;
      saveDialog.FileName = fileName;
      if  (saveDialog.ShowDialog() == DialogResult.OK)
      {
        history.Save (saveDialog.FileName);
      }
    }  /* saveInstrumentDataToolStripMenuItem_Click */



    private void exitToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if  (file != null)
        file.FrameBytesOffsetsUpdateThreadTerminate();

      Close();
    
//      file.FrameBytesOffsetsUpdateThreadTerminate ();
//      Application.Exit();
    }
    
    

    private void SipperFileViewer_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (file != null)
        file.FrameBytesOffsetsUpdateThreadTerminate();
      SaveSipperFileViewerConfiguration ();

      DisposeExistingDataStructures ();
      
      if  (parameters != null)
      {
        parameters.Dispose ();
        parameters = null;
      }

      if  (dbConn != null)
      {
        dbConn.Close ();
        dbConn = null;
        GC.Collect ();
      }
    }



    private void  DataFieldLabelClicked (object sender, EventArgs e)
    {
      Button b = (Button)sender;
      
      int  x;
      
      int  dataFieldNum = -1;
      for  (x = 0;  x < dataFields.Length;  x++)
      {
        if  (dataFieldLabels[x].Text == b.Text)
        {
          dataFieldNum = x;
          break;
        }
      }
      
      if  (dataFieldNum < 0)
        return;
        
      int  fieldIdxOrig = InstrumentData.GetFieldNameIndex (b.Text);
    
      SelectAInstrumentDataField  f = new SelectAInstrumentDataField (b.Text, true);
      f.ShowDialog ();
      
      string  newSelectedDataFieldStr = f.SelectedDataField;
      int  fieldIdxNew  =  InstrumentData.GetFieldNameIndex (newSelectedDataFieldStr);
      
      if  (fieldIdxNew < 0)
        return;
        
      dataFieldLabels[dataFieldNum].Text = newSelectedDataFieldStr;
      dataFieldIdxs[dataFieldNum] = fieldIdxNew;

      for  (x = 0;  x < dataFields.Length;  x++)
      {
        if  (x == dataFieldNum)
          continue;
          
        if  (dataFieldIdxs[x] == fieldIdxNew)
        {
          // Another data label button has the same field selected as the one we just selced for thos button.
          // we will change it to the one that this button used to use.
          dataFieldIdxs[x] = fieldIdxOrig;
          dataFieldLabels[x].Text = InstrumentData.FieldName (fieldIdxOrig);
          break;
        }
      }

      UpdateStats ();
      SaveSipperFileViewerConfiguration ();
    }  /* DataFieldLabelClicked */



    private void timeChartToolStripMenuItem_Click(object sender, EventArgs e)
    {
      timePlot = new TimePlot(configRec, history);
      timePlot.Show();
    }


    private void depthChartToolStripMenuItem_Click(object sender, EventArgs e)
    {
      depthPlot = new DepthPlot2 (configRec, history, "");
      depthPlot.Show ();
    }


    private void AutoScrollUp_Click (object sender, EventArgs e)
    {
      int  magnitude = Math.Abs (autoScrollRate);

      if  (magnitude > 400)
          autoScrollRate -= 100;

      else if  (magnitude > 200)
          autoScrollRate -= 50;

      else if  (magnitude > 50)
          autoScrollRate -= 25;
          
      else
          autoScrollRate -= 10;
      
      AutoScrollingRateTextSet ();
    }



    private void AutoScrollDown_Click (object sender, EventArgs e)
    {
      int  magnitude = Math.Abs (autoScrollRate);

      if  (magnitude >= 400)
          autoScrollRate += 100;

      else if  (magnitude >= 200)
          autoScrollRate += 50;

      else if  (magnitude >= 50)
          autoScrollRate += 25;

      else
          autoScrollRate += 10;

      AutoScrollingRateTextSet ();
    }



    private void AutoScrollStop_Click (object sender, EventArgs e)
    {
      autoScrollRate = 0;
      AutoScrollingRateTextSet ();
    }



    private void vScrollBar1_Scroll (object sender, ScrollEventArgs e)
    {
      autoScrollRate = 0;
      AutoScrollingRateTextSet ();
      ScrollToPropperPlaceInSipperFile2 (vScrollBar1.Value);
      ClearRectangle ();
    }



    private void autoDetect_CheckedChanged (object sender, EventArgs e)
    {
      parameters.ExtractBlobs (autoDetect.Checked);
    }



    private void autoDetectionToolStripMenuItem1_Click (object sender, EventArgs e)
    {
      if  (ActiveTrainingLibraries.ModelsAreLoading ())
      {
        MessageBox.Show (this, "Training Models are being loaded, can not change parametres until completed.",
                         "Training Models are Loading", 
                         MessageBoxButtons.OK
                        );
        return;
      }

      AutoDetectionParamters  ad = new AutoDetectionParamters (this);
      ad.RebuildTrainingLibrary1 = false;
      ad.RebuildTrainingLibrary2 = false;
      
      ad.ShowDialog ();
    }



    public  void  CancelLoadingModels ()
    {
      ActiveTrainingLibraries.CancelLoads ();
    }  /* CancelLoadingModels */



    private  void  RightMouseMenu (int  displayRow,
                                   int  displayCol
                                  )
    {
    }  /* RightMouseMenu */



    private void colorize_CheckedChanged(object sender, EventArgs e)
    {
      if  (parameters.Colorize != colorize.Checked)
      {
        parameters.Colorize = colorize.Checked;
        parameters.PaintWholePanel ();
      }
    }



    private void connectDatabaseToolStripMenuItem_Click (object sender, EventArgs e)
    {
      dbConn = null;
      dbConn = PicesDataBase.SelectNewDataBaseServer (runLog);
      parameters.SetDataBase (dbConn);
      SetTitleBar ();
    }



    private void jumpToStartExtractionPointToolStripMenuItem_Click (object sender, EventArgs e)
    {
      if  ((dbSipperFile == null)  ||  (file == null))
        return;

      if  (dbSipperFile.ExtractionScanLineStart > file.LargestKnownScanLine())
        return;

      int  displayLine = (int)((float)dbSipperFile.ExtractionScanLineStart / parameters.Ratio ());

      ScrollToPropperPlaceInSipperFile2 (displayLine);
    }



    private void jumpToEndExtractionPointToolStripMenuItem_Click (object sender, EventArgs e)
    {
      if  ((dbSipperFile == null)  ||  (file == null))
        return;

      if  (dbSipperFile.ExtractionScanLineEnd > file.LargestKnownScanLine())
        return;

      int  displayLine = (int)((float)dbSipperFile.ExtractionScanLineEnd / parameters.Ratio ());

      ScrollToPropperPlaceInSipperFile2 (displayLine);
    }


    private void saveRawCTDDataMenuItem_Click_1 (object sender, EventArgs e)
    {
      SaveRawCTDData  srcd = new SaveRawCTDData (sipperFileName);
      srcd.ShowDialog (this);
    }


    private void NavigationDataToPlot_Click(object sender, EventArgs e)
    {
      SelectAInstrumentDataField  f = new SelectAInstrumentDataField (NavigationDataToPlot.Text, false);
      f.ShowDialog ();
      string  newSelectedDataFieldStr = f.SelectedDataField;
      int  fieldIdxNew  =  InstrumentData.GetFieldNameIndex (newSelectedDataFieldStr);
      if  (fieldIdxNew < 0)
        return;
      navigateDataFieldIdx = fieldIdxNew;
      UpdateNavigateChart ();
    }



    private void NavigationDataToPlot2_Click (object sender, EventArgs e)
    {
      SelectAInstrumentDataField  f = new SelectAInstrumentDataField (NavigationDataToPlot2.Text, false);
      f.ShowDialog ();
      string  newSelectedDataFieldStr = f.SelectedDataField;
      int  fieldIdxNew  =  InstrumentData.GetFieldNameIndex (newSelectedDataFieldStr);
      if  (fieldIdxNew < 0)
        return;
      navigateDataField2Idx = fieldIdxNew;
      UpdateNavigateChart ();
    }



    private void editSipperFileParametersToolStripMenuItem_Click (object sender, EventArgs e)
    {
      if  (configRec.UseDataBase ()  &&  (dbSipperFile != null))
      {
        SipperFileDialog  sfd = new SipperFileDialog (dbSipperFile, runLog);
        sfd.ShowDialog (this);
      }
    }


  }  /* SipperFileViewer */
}
