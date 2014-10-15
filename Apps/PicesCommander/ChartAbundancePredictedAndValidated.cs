using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Threading;
using System.IO;

using SipperFile;
using PicesInterface;

namespace PicesCommander
{
  public partial class ChartAbundancePredictedAndValidated : Form
  {
    PicesClassList  classes       = null;
    PicesClassList  activeClasses = null;
    String          rootDir       = "";

    private  String  cruise         = null;
    private  String  station        = null;
    private  String  deployment     = null;
    private  String  groupName      = null;
    private  PicesDataBaseImageGroup  group = null;

    private  int     sizeMin        = 150;
    private  int     sizeMax        = 0;

    private  float   probMin        = 0.0f;
    private  float   probMax        = 1.0f;

    private  float   depthMin       = 0.0f;
    private  float   depthMax       = 0.0f;

    private  bool    filterSize   = false;
    private  bool    filterProb   = false;
    private  bool    filterDepth  = false;

    private  String  infinityStr = ((char)8734).ToString ();

    private  PicesDataBase    mainWinConn = null;
    private  PicesDataBase    threadConn  = null;  /**< This connection will only be use by the background thread. */

    private  PicesMsgQueue    statusMsgs = null;
    private  PicesMsgQueue    msgQueue   = null;
    private  PicesRunLog      runLog     = null;

    private  String           criteriaStr = "";

    private  float            depthIncrement = 1.0f;
    private  PicesClass       classToPlot = null;
    private  bool             includeSubClasses = false;
    private  bool             weightByVolume    = false;

    private  PicesGoalKeeper  goalie = new PicesGoalKeeper ("ChartAbundancePredictedAndValidated");

    private  bool  buildPlotDataRunning = false;
    private  bool  buildPlotDataDone    = false;
    private  bool  cancelRequested      = false;

    private  int  heightLast = 0;
    private  int  widthLast  = 0;

    private  Color[]  colorTable = {Color.Black,      Color.Red,        Color.Green,      
                                    Color.Blue,       Color.Crimson,    Color.DarkCyan,   
                                    Color.DarkOrange, Color.Lime,       Color.SeaGreen
                                   };

    private  String  configFileName = "";
    private  bool    formIsMaximized = false;


    private  class  DataSeriesToPlot
    {
      public  DataSeriesToPlot (char     _classKeyToUse,
                                String   _legend,
                                float[]  _density,
                                float[]  _countsByDepth
                               ) 
      {
        classKeyToUse      = _classKeyToUse;
        legend             = _legend;
        density            = _density;
        countsByDepth      = _countsByDepth;
      }

      public  char      classKeyToUse;
      public  float[]   countsByDepth;
      public  float[]   density;
      public  String    legend;
    }

    
    private  List<DataSeriesToPlot>  series = new List<DataSeriesToPlot> ();
    private  float[]  depthVolumeProfile = null;


    public ChartAbundancePredictedAndValidated (String                   _cruise,
                                                String                   _station,
                                                String                   _deployment,
                                                PicesDataBaseImageGroup  _group,
                                                PicesClass               _classToPlot,
                                                int                      _sizeMin,
                                                int                      _sizeMax,
                                                float                    _probMin,
                                                float                    _probMax,
                                                float                    _depthMin,
                                                float                    _depthMax,
                                                PicesClassList           _classes,
                                                PicesClassList           _activeClasses,
                                                String                   _rootDir
                                               )
    {
      cruise        = _cruise;
      station       = _station;
      deployment    = _deployment;
      group         = _group;
      if  (group != null)
        groupName = group.Name;
      classToPlot   = _classToPlot;
      sizeMin       = _sizeMin;
      sizeMax       = _sizeMax;
      probMin       = _probMin;
      probMax       = _probMax;
      depthMin      = _depthMin;
      depthMax      = _depthMax;
      classes       = _classes;
      activeClasses = _activeClasses;
      rootDir       = _rootDir;

      if  ((sizeMin > 0)      &&  (sizeMax <= 0))      sizeMax  = int.MaxValue;
      if  ((probMin >= 0.0f)  &&  (probMax <= 0.0f))   probMax  = 1.0f;
      if  ((depthMin > 0.0f)  &&  (depthMax <= 0.0f))  depthMax = float.MaxValue;

      filterSize =  (sizeMin > 0)      ||  ((sizeMax > 0)       &&  (sizeMax > sizeMin));
      filterProb  = ((probMin > 0.0f)  ||   (probMax  < 1.0f))  &&  (probMax > probMin);
      filterDepth = ((depthMax > 0)  &&  (depthMax > depthMin));

      statusMsgs = new PicesMsgQueue ("ChartAbundancePredictedAndValidated-StatusMsgs");
      msgQueue   = new PicesMsgQueue ("ChartAbundancePredictedAndValidated-RunLog");
      runLog     = new PicesRunLog (msgQueue);

      mainWinConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      configFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "ChartAbundancePredictedAndValidated.cfg";

      BuildCriteriaString ();

      InitializeComponent ();
    }

   

    private  void  AddToStr (ref String existingStr, String  addOn)
    {
      if  (String.IsNullOrEmpty (addOn))
        return;

      if  (!String.IsNullOrEmpty (existingStr))
        existingStr += ", ";
      existingStr += addOn;
    }



    private  void  BuildCriteriaString ()
    {
      criteriaStr = "";
      //AddToStr (ref criteriaStr, cruise);
      //AddToStr (ref criteriaStr, station);
      //if  (!String.IsNullOrEmpty (deployment))
      //  AddToStr (ref criteriaStr, deployment);

      String  sizeMaxStr = infinityStr;
      if  ((sizeMax > 0)  &&  (sizeMax < int.MaxValue))
        sizeMaxStr = sizeMax.ToString ("##,##0");

      String  depthMaxStr = infinityStr;
      if  ((depthMax > 0.0f)  &&  (depthMax < 100000.0))
        depthMaxStr = depthMax.ToString ("#,##0");

      if  (filterSize)
        AddToStr (ref criteriaStr, "size(" + sizeMin + ", " + sizeMaxStr + ")");

      if  (filterProb)
        AddToStr (ref criteriaStr, "prob(" + probMin.ToString ("p2") + ", " + probMax.ToString ("p2") + ")");

      if  (filterDepth)
        AddToStr (ref criteriaStr, "Depth(" + depthMin + ", " + depthMaxStr + ")");
    }




    private void ChartAbundanceByDeployment_Load (object sender, EventArgs e)
    {
      heightLast = Height;
      widthLast  = Width;

      Cruise_.Text     = cruise;
      Station_.Text    = station;
      Deployment_.Text = deployment;
      Group.Text       = groupName;

      if  (classToPlot != null)
        ClassToPlot.Text = classToPlot.Name;

      ContextMenuStrip cms = new ContextMenuStrip ();
      cms.Items.Add ("Copy Chart to clipboard",               null, CopyChartToClipboard);
      cms.Items.Add ("Save Chart to Disk",                    null, SaveChartToDisk);
      cms.Items.Add ("Copy Data Tab-Delimited to Clipboard",  null, SaveTabDelToClipBoard);
      cms.Items.Add ("Save Data Tab-Delimited to Disk",       null, SaveTabDelToDisk);

      ProfileChart.ContextMenuStrip = cms;

      LoadConfigurationFile ();

    }


    private  void  DisableControls ()
    {
      DepthIncr.Enabled      = false;
      PlotButton.Enabled        = false;
      DepthIncr.Enabled         = false;
      IncludeSubClasses.Enabled = false;
      WeightByVolume.Enabled    = false;
      DepthAxisAuto.Enabled     = false;
      DepthAxisMin.Enabled      = false;
      DepthAxisMax.Enabled      = false;
      DepthAxisInterval.Enabled = false;
    }


    private  void  EnableControls ()
    {
      DepthIncr.Enabled         = true;
      PlotButton.Enabled        = true;
      DepthIncr.Enabled         = true;
      IncludeSubClasses.Enabled = true;
      WeightByVolume.Enabled    = true;
      DepthAxisAuto.Enabled     = true;
      DepthAxisMin.Enabled      = true;
      DepthAxisMax.Enabled      = true;
      DepthAxisInterval.Enabled = true;
    }


    private  void  SaveConfiguration ()
    {
      System.IO.StreamWriter  o = null;
      try{o = new System.IO.StreamWriter (configFileName);}  catch  (Exception){o = null; return;}
      
      o.WriteLine ("//ChartAbundancePredictedAndValidated Configuration File");
      o.WriteLine ("//");
      o.WriteLine ("//DateTime Saved" + "\t" + DateTime.Now.ToString ("F"));
      o.WriteLine ("//");
      o.WriteLine ("WidthLast"          + "\t" + widthLast);
      o.WriteLine ("HeightLast"         + "\t" + heightLast);
      o.WriteLine ("Maximized"          + "\t" + (formIsMaximized ? "YES":"NO"));
      //o.WriteLine ("PlotAction"         + "\t" + ((String)(PlotAction.SelectedItem)));
      o.WriteLine ("DepthIncr"          + "\t" + DepthIncr.Value);
      o.WriteLine ("DepthAxisAuto"      + "\t" + (DepthAxisAuto.Checked ? "Yes" : "No"));
      o.WriteLine ("DepthAxisMin"       + "\t" + DepthAxisMin.Value);
      o.WriteLine ("DepthAxisMax"       + "\t" + DepthAxisMax.Value);
      o.WriteLine ("DepthAxisInterval"  + "\t" + DepthAxisInterval.Value);

      o.Close ();
      o = null;
    }


    private  void   LoadConfigurationFile ()
    {
      System.IO.StreamReader i = null;

      try {i = new System.IO.StreamReader (configFileName);}  catch  (Exception) {i = null;}
      if  (i == null)
        return;

      int  savedWidth  = 0;
      int  savedHeight = 0;
      bool screenWasMaximized = false;

      String  nextLine = null;

      while  (true)
      {
        try  {nextLine = i.ReadLine ();}  catch (Exception) {break;}
        if  (nextLine == null)
          break;

        nextLine = nextLine.Trim ();
        
        if  ((nextLine.Length < 3)  ||  (nextLine.Substring (0, 2) == "//"))
          continue;

        String[] fields = nextLine.Split ('\t');
        if  (fields.Length < 2)
          continue;

        String  fieldName  = fields[0];
        String  fieldValue = fields[1];

        switch  (fieldName)
        { 
          case  "WidthLast":
            savedWidth = PicesKKStr.StrToInt (fieldValue);
            break;

          case  "HeightLast":
            savedHeight = PicesKKStr.StrToInt (fieldValue);
            break;

          case  "Maximized":
            screenWasMaximized  = (fieldValue.ToUpper () == "YES");
            break;

          case  "TimeInterval":
          case  "DepthIncr":
            depthIncrement = (int)PicesKKStr.StrToFloat (fieldValue);
            if  (depthIncrement > 0)
              DepthIncr.Value = (decimal)depthIncrement;
            break;
        }
      }

      i.Close ();

      if  (savedWidth > Screen.PrimaryScreen.Bounds.Width)
        savedWidth = Screen.PrimaryScreen.Bounds.Width;

      if  (savedHeight > Screen.PrimaryScreen.Bounds.Height)
        savedHeight = Screen.PrimaryScreen.Bounds.Height;

      if  (screenWasMaximized)
      {
        //savedWidth = Screen.PrimaryScreen.Bounds.Width;
        //savedHeight = savedHeight = Screen.PrimaryScreen.Bounds.Height;
        this.WindowState = FormWindowState.Maximized; 
      }
      else
      {
        Height = Math.Max (savedHeight, MinimumSize.Height);
        Width  = Math.Max (savedWidth,  MinimumSize.Width);
      }

      OnResizeEnd (new EventArgs ());
    }  /* LoadConfigurationFile */


    private  void   ValidateDepthAxisSettings (ref List<String>  errors)
    {
      if  (DepthAxisAuto.Checked)
      {
        if  (DepthAxisMin.Value >= DepthAxisMax.Value)
          errors.Add ("Min-Depth must be less than Max-Depth");

        if  (DepthAxisInterval.Value <= 0)
          errors.Add ("The increment must be greater than 0.");

        else
        {
          Decimal  range = (DepthAxisMax.Value - DepthAxisMin.Value);
          if  ((range > 0)  &&  (range < DepthAxisInterval.Value))
            errors.Add ("Interval size is less than specified range.");
        }
      }
      return;
    }


    private  List<String>  ValidateFields ()
    {
      List<String>  errors = new List<string> ();

      weightByVolume = WeightByVolume.Checked;
      includeSubClasses = IncludeSubClasses.Checked;

      depthIncrement = (int)DepthIncr.Value;
      if  (depthIncrement < 1)
        errors.Add ("Depth Increment must be greater than zero.");

      if  (String.IsNullOrEmpty (ClassToPlot.Text))
        errors.Add ("You must specify class to plot.");
      else
        classToPlot = PicesClassList.GetUniqueClass (ClassToPlot.Text, "");

      ValidateDepthAxisSettings (ref errors);

      if  (errors.Count < 1)
        return null;
      else
        return errors;
    }



    private  float[]  MergeTwoLists (float[]  l1, float[] l2)
    {
      if  (l1 == null)       return l2;
      else if  (l2 == null)  return l1;

      int  minLen = 0;

      float[]  from = null;
      float[]  to   = null;

      if  (l1.Length > l2.Length)
      {from = l2;   to = l1;  minLen = l2.Length;}
      else
      {from = l1;   to = l2;  minLen = l1.Length;}

      for  (int idx = 0;  idx < minLen;  ++idx)
        to[idx] += from[idx];

      return  to;
    }
    

    private  uint[]  MergeTwoLists (uint[]  l1, uint[] l2)
    {
      if  (l1 == null)       return l2;
      else if  (l2 == null)  return l1;

      int  minLen = 0;

      uint[]  from = null;
      uint[]  to   = null;

      if  (l1.Length > l2.Length)
      {from = l2;   to = l1;  minLen = l2.Length;}
      else
      {from = l1;   to = l2;  minLen = l1.Length;}

      for  (int idx = 0;  idx < minLen;  ++idx)
        to[idx] += from[idx];

      return  to;
    }
    

    private  float[]  CountsByDepth (PicesClass  c,
                                     char        classKeyToUse
                                    )
    {
      String  predOrValStr = "Validated";
      if  (classKeyToUse != 'V')
        predOrValStr =  "Predicted";

      String msg = "Extracting Counts for \"" + c.Name + "\"" + "  " + predOrValStr;
      statusMsgs.AddMsg (msg);
      msgQueue.AddMsg (msg);

      uint[]  counts = null;

      PicesSipperFileList  sipperFiles = threadConn.SipperFileLoad (cruise, station, deployment);
      foreach  (PicesSipperFile sf in sipperFiles)
      {
        if  (cancelRequested)
          break;

        msg = "Retrieving counts for Class[" + c.Name + "]  SipperFile[" + sf.SipperFileName + "]"  + predOrValStr;
        statusMsgs.AddMsg ("Retrieving counts for Class[" + c.Name + "]  SipperFile[" + sf.SipperFileName + "]");
    
        uint[]  countsThisSipperFile 
          = threadConn.ImageGetDepthStatistics (group,              // PicesDataBaseImageGroup^  imageGroup,
                                                sf.SipperFileName,
                                                depthIncrement,     // depthIncrements,
                                                c,                  // mlClass,
                                                classKeyToUse,      // classKeyToUse,
                                                probMin,  probMax,
                                                sizeMin,  sizeMax
                                               );

        counts = MergeTwoLists (counts, countsThisSipperFile);
      }

      if  (cancelRequested)
        return null;
      
      if  (counts == null)
      {
        counts = new uint[1];
        counts[0] = 0;
      }

      float[]  countsFloat = new float[counts.Length];
      for  (int idx = 0;  idx < counts.Length;  ++idx)
        countsFloat[idx] = counts[idx];

      if  (!includeSubClasses)
        return  countsFloat;
      
      List<PicesClass> children = c.Children;
      if  (children != null)
      {
        foreach  (PicesClass  pc in children)
        {
          if  (cancelRequested)
            break;

          float[] countsForChild = CountsByDepth (pc, classKeyToUse);
          if  (countsForChild != null)
            countsFloat = MergeTwoLists (countsFloat, countsForChild);
        }
      }
      return  countsFloat;
    }  /* CountsByDepth */




    private  float[]  GetDepthVolumeProfile  ()
    {
      String  msg = "Retrieving Depth-Volume profile Station[" + cruise + "]  Station[" + station + "]  Deployment[" + deployment + "].";
      msgQueue.AddMsg (msg);
      statusMsgs.AddMsg (msg);
      PicesVolumeSampledStatList 
        volStats = threadConn.InstrumentDataGetVolumePerMeterDepth (cruise, station, deployment, depthIncrement);
      if  (volStats == null)
        return  null;

      return  volStats.ToArray ();
    }


    
    private  void  AddToSeries (char  classKeyToUse)
    {
      if  (depthVolumeProfile == null)
        return;

      float[] counts = CountsByDepth (classToPlot, classKeyToUse);
      if  ((counts == null)  ||  cancelRequested)
        return;

      float[]  density = new float[counts.Length];
      
      for  (int idx = 0;  idx < counts.Length;  ++idx)
      {
        float vol = 0;
        if  (idx < depthVolumeProfile.Length)
          vol = depthVolumeProfile[idx];
        if  (vol == 0.0f)
          density[idx] = 0.0f;
        else
          density[idx] = counts[idx] / vol;
      }

      String legend = (classKeyToUse == 'V') ? "Validated" : "Predicted";
      
      goalie.StartBlock ();
      series.Add (new DataSeriesToPlot (classKeyToUse, legend, density, counts));
      goalie.EndBlock ();
    }  /* AddToSeries */




    /// <summary>
    /// This method will be ran as a separate thread; it is respnable for collecting all the data needed to generate the plot.
    /// </summary>
    private  void  BuildPlotData ()
    {
      if  (buildPlotDataRunning)
        return;

      PicesDataBase.ThreadInit ();

      threadConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      buildPlotDataRunning = true;

      depthIncrement = (int)DepthIncr.Value;
      classToPlot    = PicesClassList.GetUniqueClass (ClassToPlot.Text, "");

      series = new List<DataSeriesToPlot> ();

      depthVolumeProfile = GetDepthVolumeProfile ();

      AddToSeries ('P');
      AddToSeries ('V');

      threadConn.Close ();
      threadConn = null;
      GC.Collect ();
      PicesDataBase.ThreadEnd ();

      if  (cancelRequested)
        statusMsgs.AddMsg ("Plotting of data CANCELLED!!!");
      else
        statusMsgs.AddMsg ("Building of plot data completed !!!");

      buildPlotDataRunning = false;
      buildPlotDataDone = true;
    }  /* BuildPlotData */


    private  void  CancelBackGroundThread ()
    {
      cancelRequested = true;
      if  (threadConn != null)
        threadConn.CancelFlag = true;
      statusMsgs.AddMsg ("Cancel Requested");
    }


    private int  FindBestInetrvaleSize (int minSize, int maxSize, int range)
    {
      int  smallestInterval = range % maxSize;
      int  intervalThatIsSmallest = maxSize;

      int zed = maxSize - 1;
      while  (zed >= minSize)
      {
        int  mod = range % zed;
        if  (mod <= smallestInterval)
        {
          smallestInterval = mod;
          intervalThatIsSmallest = zed;
        }
        --zed;
      }
      return  intervalThatIsSmallest;
    }


    private  void  DetermineDepthMinMaxToPlot (ref float   depthMinToPlot,
                                               ref float   depthMaxToPlot,
                                               ref float   numIntervals,
                                               ref float   intervalSize,
                                               ref string  depthFormatStr
                                              )
    {
      float  min = float.MaxValue;
      float  max = float.MinValue;
      numIntervals = 10.0f;
      intervalSize = 10.0f;

      foreach  (DataSeriesToPlot dstp in series)
      {
        for  (int depthIDX = 0;  depthIDX < dstp.countsByDepth.Length;  ++depthIDX)
        {
          float  depth = depthIDX * depthIncrement;
          if  (dstp.countsByDepth[depthIDX] == 0.0)
            continue;
          if  (depth < min)
            min = depth;

          if  (depth > max)
            max = depth;
        }
      }

      float  depthDelta = max - min;
      float  depthRange = 2.0f;
      numIntervals = 0.25f;
      depthFormatStr = "##,##0";
      intervalSize = -1.0f;

      if  (depthDelta <= 2.0f)
      {
        depthRange = 2.0f;
        intervalSize = 0.25f;
        depthFormatStr = "#,##0.00";
      }

      else if  (depthDelta <= 5.0f)
      {
        depthRange = (float)Math.Ceiling (depthDelta);
        intervalSize = 0.5f;
        depthFormatStr = "#,##0.0";
      }

      else if  (depthDelta <= 10.0f)
      {
        depthRange = (float)Math.Ceiling (depthDelta);
        intervalSize = FindBestInetrvaleSize (1, 3, (int)depthRange);
      }

      else if  (depthDelta < 15.0f)
      {
        depthRange = 1.0f * (float)Math.Ceiling ((depthDelta / 1.0f));
        intervalSize = FindBestInetrvaleSize (2, 4, (int)depthRange);
      }

      else if  (depthDelta < 20.0f)
      {
        depthRange = 2.0f * (float)Math.Ceiling ((depthDelta / 2.0f));
        intervalSize = FindBestInetrvaleSize (2, 5, (int)depthRange);
      }

      else if  (depthDelta < 50.0f)
      {
        depthRange = 5.0f * (float)Math.Ceiling ((depthDelta / 5.0f));
        intervalSize = FindBestInetrvaleSize (5, 10, (int)depthRange);
      }

      else if  (depthDelta < 75.0f)
      {
        depthRange = 5.0f * (float)Math.Ceiling ((depthDelta / 5.0f));
        intervalSize = FindBestInetrvaleSize (10, 15, (int)depthRange);
      }

      else if  (depthDelta < 100.0f)
        depthRange = 10.0f * (float)Math.Ceiling ((depthDelta / 10.0f));

      else if  (depthDelta <= 200f)
        depthRange = 20.0f * (float)Math.Ceiling ((depthDelta / 20.0f));

      else if  (depthDelta <= 300.0f)
        depthRange = 30.0f * (float)Math.Ceiling ((depthDelta / 30.0f));

      else if  (depthDelta <= 400.0f)
        depthRange = 40.0f * (float)Math.Ceiling ((depthDelta / 30.0f));

      else
        depthRange = 100.0f * (float)Math.Floor (depthDelta / 100.0f);

      if  (intervalSize < 0.0f)
        intervalSize = depthRange / 10.0f;

      numIntervals = (int)(0.5f + depthRange / intervalSize);

      float incrPerMeter = 1.0f / intervalSize;
      depthMinToPlot = (float)Math.Floor (min * incrPerMeter) / incrPerMeter;
      depthMaxToPlot = depthMinToPlot + intervalSize * numIntervals;
    }



    private  void  UpdateChartAreas ()
    {
      goalie.StartBlock ();
      
      String  t1 = "Cruise: " + cruise + "  Station: " + station;
      if  (String.IsNullOrEmpty (deployment))
        t1 += "  Deployment: " + deployment;

      t1 += "  Class: " + classToPlot.Name;

      ProfileChart.Titles.Clear ();
      ProfileChart.Titles.Add (t1);

      String  t2 = "";
      if  (group != null)
        AddToStr (ref t2, group.Name);

      if  (!String.IsNullOrEmpty (criteriaStr))
        AddToStr (ref t2, criteriaStr);

      if  (!String.IsNullOrEmpty (t2))
        ProfileChart.Titles.Add (t2);

      ProfileChart.Series.Clear ();

      if  (series.Count < 1)
      {
        goalie.EndBlock ();
        return;
      }

      float   depthMinToPlot = 0.0f;
      float   depthMaxToPlot = 100.0f;
      float   numIntervals   = 10.0f;
      float   intervalSize   = 10.0f;
      String  depthFormatStr = "##,##0";

      DetermineDepthMinMaxToPlot (ref depthMinToPlot,
                                  ref depthMaxToPlot,
                                  ref numIntervals,
                                  ref intervalSize,
                                  ref depthFormatStr
                                 );
      if  (!DepthAxisAuto.Checked)
      {
        depthMinToPlot = (float)DepthAxisMin.Value;
        depthMaxToPlot = (float)DepthAxisMax.Value;
        intervalSize   = (float)DepthAxisInterval.Value;
      }
      else
      {
        DepthAxisInterval.Value = (Decimal)intervalSize;
        DepthAxisMin.Value = (Decimal)depthMinToPlot;
        DepthAxisMax.Value = (Decimal)depthMaxToPlot;
      }

      ChartArea ca = ProfileChart.ChartAreas[0];
      ca.AxisY.IsReversed = true;
      ca.AxisY.Minimum = 0;
      ca.AxisY.Title = "Depth(m)";
      //ca.AxisY.Title = "Depth(" + depthMinToPlot + " - " + depthMaxToPlot + ")";

      ca.AxisX.Minimum = 0;
      ca.AxisX.Title = "Count/m-3";
      ca.AxisX2.MajorGrid.Enabled = false;

      ca.AxisY.IsReversed = true;
      ca.AxisY.Maximum = depthMaxToPlot;
      ca.AxisY.Minimum = depthMinToPlot;
      ca.AxisY.Interval = intervalSize;
      ca.AxisY.LabelStyle.Format = depthFormatStr;

      Decimal  distFromFirstInterval = (Decimal)depthMinToPlot % (Decimal)intervalSize;
      double  intervalOffSet = (double)((Decimal)intervalSize - distFromFirstInterval);
      if  (intervalOffSet == intervalSize)
        intervalOffSet = 0;

      ca.AxisY.IntervalOffset = intervalOffSet;
      ca.AxisY.MajorTickMark.Interval = intervalSize;
      ca.AxisY.MajorTickMark.IntervalOffset = intervalOffSet;

      float   intervalMinorSize = intervalSize / 2.0f;
      Decimal  distFromFirstIntervalMinor = (Decimal)depthMinToPlot % (Decimal)intervalMinorSize;
      double  intervalMinorOffSet = (double)((Decimal)intervalMinorSize - distFromFirstIntervalMinor);
      if  (intervalMinorOffSet == intervalMinorSize)
        intervalMinorOffSet = 0.0;

      ca.AxisY.MinorTickMark.Interval = intervalMinorSize;
      ca.AxisY.MinorTickMark.IntervalOffset = intervalMinorOffSet;

      ProfileChart.Series.Clear ();

      String  uintOfMeasure = "Count";
      if  (weightByVolume)
        uintOfMeasure = "Count/m_3";

      // First we will plot class counts on CharArea1
      foreach  (DataSeriesToPlot dstp in series)
      {
        Series s = new Series (dstp.legend);
        s.ChartArea = "ChartArea1";
        s.ChartType =  SeriesChartType.Line;
        s.Name = dstp.legend;

        ca.AxisX.Title = uintOfMeasure;
        ca.AxisX.LabelStyle.Format =  "###,##0";
        ca.AxisX.Minimum = 0;
        s.XAxisType = AxisType.Primary;
 
        s.BorderWidth = 2;

        for  (int depthIDX = 0;  depthIDX < dstp.countsByDepth.Length;  ++depthIDX)
        {
          float  yValue = depthIDX * depthIncrement;
          if  ((yValue >= depthMinToPlot)  &&  (yValue <= depthMaxToPlot))
          {
            if  (weightByVolume)
              s.Points.Add (new DataPoint (dstp.density[depthIDX], yValue));
            else
              s.Points.Add (new DataPoint (dstp.countsByDepth[depthIDX], yValue));
          }
        }

        ProfileChart.Series.Add (s);
      }

      ProfileChart.ChartAreas[0].RecalculateAxesScale ();

      goalie.EndBlock ();
    }  /* UpdateChartAreas */



    private void PlotButton_Click (object sender, EventArgs e)
    {
      List<String>  errors = ValidateFields ();
      if  (errors != null)
      {
        String  errMsg = "Errors is class selection";
        foreach (String s in errors)
          errMsg += "\n" + s;
        MessageBox.Show (this, errMsg, "Validating Class Selection", MessageBoxButtons.OK);
      }
      else
      {
        DisableControls ();
        SaveConfiguration ();
        PlotButton.Enabled = false;
        if  (buildPlotDataRunning)
          return;

        timer1.Enabled = true;
        Thread t = new Thread (new ThreadStart (BuildPlotData));
        t.Start ();
      }
    }


    int ticks = 0;
    private void timer1_Tick (object sender, EventArgs e)
    {
      ++ticks;

      String msg = statusMsgs.GetNextMsg ();
      while  (msg != null)
      {
        Status.AppendText (msg);
        msg = statusMsgs.GetNextMsg ();
      }

      if   (buildPlotDataRunning)
      {
        if  ((ticks % 20) == 0)
          UpdateChartAreas ();
      }

      if  (buildPlotDataDone)
      {
        UpdateChartAreas ();
        PlotButton.Enabled = true;
        buildPlotDataDone = false;
        cancelRequested = false;
        EnableControls ();
      }
    }


    private void ChartAbundanceByDeployment_Resize (object sender, EventArgs e)
    {
      int  heightDelta = Height - heightLast;
      int  widthDelta  = Width  - widthLast;

      ProfileChart.Height += heightDelta;
      ProfileChart.Width  += widthDelta;

      heightLast = Height;
      widthLast  = Width;
    }



    private void ChartAbundanceByDeployment_SizeChanged (object sender, EventArgs e)
    {
      if  (WindowState == FormWindowState.Maximized)
      {
        // Looks like user has pressed the Maximized button.  We have to trap it here because
        // the ResizeEnd envent does not trap when form is Maximized.
        //PicesCommanderFormResized ();
        ChartAbundanceByDeployment_Resize (sender, e);
        formIsMaximized = true;
      }
      else if  (WindowState == FormWindowState.Normal)
      {
        if  (formIsMaximized)
        {
          // We normally trap the ResizeEnd event;  but when the form was already maximized and the user
          // presses the button to unmaximize.  the ResizeEnd does not trap that.  So we check to 
          // see if the form was already maximize.  If so then we resized the form.
          //PicesCommanderFormResized ();
          ChartAbundanceByDeployment_Resize (sender, e);
          formIsMaximized = false;
        }
      }
    }



    private  void  CopyChartToClipboard (Object sender, EventArgs e)
    {
      using (System.IO.MemoryStream ms = new System.IO.MemoryStream ())
      {
        ProfileChart.SaveImage (ms, ChartImageFormat.Bmp);
        Bitmap bm = new Bitmap(ms);
        Clipboard.SetImage(bm);
      }
    }


    private  void  SaveChartToDisk (Object sender, EventArgs e)
    {
      SaveFileDialog sfd = new SaveFileDialog ();
      sfd.Filter = "JPeg Image|*.jpg|Bitmap Image|*.bmp|Gif Image|*.gif|Tab-Del|*.txt";
      sfd.Title = "Save chart as Image File";
      sfd.DefaultExt = "jpg";
      sfd.AddExtension = true;
      sfd.OverwritePrompt = true;
      //sfd.CheckFileExists = true;
      DialogResult dr = sfd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        String  fn = sfd.FileName;
        System.IO.FileInfo fi = new System.IO.FileInfo (fn);
        String ext = fi.Extension.ToLower ();

        ChartImageFormat cif = ChartImageFormat.Jpeg;
        if  (ext == ".jpg")
          cif = ChartImageFormat.Jpeg;

        else if  (ext == ".bmp")
          cif = ChartImageFormat.Bmp;

        else if  (ext == ".gif")
          cif = ChartImageFormat.Gif;
        
        else if  (ext == ".txt")
        {
          SaveTabDelToDisk (fn);
          return;
        }
        
        ProfileChart.SaveImage (fn, cif);
      }
    }


    
    private  void  SaveTabDelToDisk (Object sender, EventArgs e)
    {
      SaveFileDialog sfd = new SaveFileDialog ();
      sfd.Filter = "Tab delimited data to disk|*.txt";
      sfd.Title = "Save Abundance info as tab-delimited data to disk.";
      sfd.DefaultExt = "txt";
      sfd.AddExtension = true;
      sfd.OverwritePrompt = true;

      String fn = cruise + "_AbundanceByDeployment";
      if  (classToPlot != null)
        fn += ("_" + classToPlot.Name);
      fn += ".txt";
      sfd.FileName = fn;

      //sfd.CheckFileExists = true;
      DialogResult dr = sfd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        fn = sfd.FileName;
        SaveTabDelToDisk (fn);
      }
    }



    private  void  SaveTabDelToDisk (String fileName)
    {
      System.IO.StreamWriter sw = null;
      try  {sw = new System.IO.StreamWriter (fileName, false, System.Text.Encoding.ASCII);}
      catch (Exception) {sw = null;}

      if  (sw == null)
      {
        MessageBox.Show (this, "Could not open file[" + fileName + "]", "Save Tab-Delimited", MessageBoxButtons.OK);
        return;
      }
      WriteTabDelToStream (sw);
      sw.Close ();
      sw = null;

      MessageBox.Show (this, "GPS Data written to \"" + fileName + "\".", "", MessageBoxButtons.OK);
    }


    private  void  SaveTabDelToClipBoard (Object sender, EventArgs e)
    {
      StringWriter  sw = new StringWriter ();
      WriteTabDelToStream (sw);
      Clipboard.SetText (sw.ToString ());
      sw.Close ();
      sw = null;
    }


    
    private  void  WriteTabDelToStream (TextWriter tw)
    {
      DateTime  curTime = DateTime.Now;
      tw.WriteLine ("ChartAbundancePredictedAndValidated"); 
      tw.WriteLine ("DateTime" + "\t" + curTime.ToString ("yyyy/MM/dd HH:mm:ss")); 
      tw.WriteLine ();
      tw.WriteLine ("Cruise"               + "\t" + cruise);
      tw.WriteLine ("Station"              + "\t" + station);
      tw.WriteLine ("Deployment"           + "\t" + deployment);
      tw.WriteLine ("Group"                + "\t" + groupName);
      tw.WriteLine ("Class"                + "\t" + classToPlot.Name);
      tw.WriteLine ("Weight-by-Volume"     + "\t" + (weightByVolume    ? "Yes" : "No"));
      tw.WriteLine ("Include-Sub-Classes"  + "\t" + (includeSubClasses ? "Yes" : "No"));
      tw.WriteLine ("Depth-Increment"      + "\t" + depthIncrement);
      tw.WriteLine ();
      tw.WriteLine ("There will be two tables below: 1-Density, 2-Counts.");
      tw.WriteLine ();
      tw.WriteLine ();
      tw.WriteLine ();

      tw.WriteLine ("Density");
      tw.WriteLine ();
      int  largestIdx = 0;
      tw.Write ("Depth" + "\t" + "Vol-Sampled");
      foreach (DataSeriesToPlot dstp in series)
      {
        String s = dstp.legend;
        tw.Write ("\t" + s);
        if  (dstp.density.Length > largestIdx)
          largestIdx = dstp.density.Length;
      }
      tw.WriteLine ();

      for  (int  depthIdx = 0;  depthIdx < largestIdx;  ++depthIdx)
      {
        tw.Write (depthIdx * depthIncrement);
        tw.Write ("\t");
        if  (depthIdx < depthVolumeProfile.Length)
          tw.Write (depthVolumeProfile[depthIdx]);

        foreach  (DataSeriesToPlot dstp in series)
        {
          tw.Write ("\t");
          if  (depthIdx < dstp.density.Length)
            tw.Write (dstp.density[depthIdx]);
        }
        tw.WriteLine ();
      }

      tw.WriteLine ();
      tw.WriteLine ();
      tw.WriteLine ();

      tw.WriteLine ("Counts");
      tw.WriteLine ();
      largestIdx = 0;
      tw.Write ("Depth" + "\t" + "Vol-Sampled");
      foreach (DataSeriesToPlot dstp in series)
      {
        String s = dstp.legend;
        tw.Write ("\t" + s);
        if  (dstp.countsByDepth.Length > largestIdx)
          largestIdx = dstp.countsByDepth.Length;
      }
      tw.WriteLine ();

      for  (int  depthIdx = 0;  depthIdx < largestIdx;  ++depthIdx)
      {
        tw.Write (depthIdx * depthIncrement);

        tw.Write ("\t");
        if  (depthIdx < depthVolumeProfile.Length)
          tw.Write (depthVolumeProfile[depthIdx]);
        
        foreach  (DataSeriesToPlot dstp in series)
        {
          tw.Write ("\t");
          if  (depthIdx < dstp.countsByDepth.Length)
            tw.Write (dstp.countsByDepth[depthIdx]);
        }
        tw.WriteLine ();
      }

      tw.WriteLine ();
      tw.WriteLine ("End of ChartAbundancePredictedAndValidated");
    }  /* WriteTabDelToStream */



    private void ChartAbundanceByDeployment_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (buildPlotDataRunning)
      {
        if (e.CloseReason == CloseReason.WindowsShutDown)
        {
          CancelBackGroundThread ();
          while  (buildPlotDataRunning)
          {
            Thread.Sleep (100);
          }
        }
        else
        {
          MessageBox.Show (this, "Plot data is being collected; press \"Cancel Button\" to Stop before exiting.", "Form Closing", MessageBoxButtons.OK);
          e.Cancel = true;
          return;
        }
      }

      SaveConfiguration ();
      
      mainWinConn.Close ();
      mainWinConn = null;
      GC.Collect ();
    }



    private void CancelPlotButton_Click (object sender, EventArgs e)
    {
      if  (buildPlotDataRunning)
        CancelBackGroundThread ();
    }


    private void DeploymentsToPlot_Format (object sender, ListControlConvertEventArgs e)
    {
      if  (e.ListItem == null)
        return;
      PicesSipperDeployment  d = (PicesSipperDeployment)e.ListItem;
      String  m = d.StationName;
      if (!String.IsNullOrEmpty (d.DeploymentNum))
        m += "-" + d.DeploymentNum + "  " + d.Description;
      e.Value = m;
    }


    private void SelectClassButton_Click (object sender, EventArgs e)
    {
      SelectAPicesClass  sapc = new SelectAPicesClass (classes, rootDir, activeClasses);

      if  (!String.IsNullOrEmpty (ClassToPlot.Text))
      sapc.SelectedClass = PicesClassList.GetUniqueClass (ClassToPlot.Text, "");
      sapc.ShowDialog (this);
      if  (sapc.SelectionMade)
      {
        PicesClass pc = sapc.SelectedClass;
        if  (pc != null)
          ClassToPlot.Text = pc.Name;
      }
    }
  }
}
