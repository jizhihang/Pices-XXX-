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
  public partial class ChartGPS : Form
  {
    private  String  cruise         = null;
    private  String  station        = null;
    private  String  deployment     = null;
    private  int     timeInterval   = 10;   /**< Interval in Seconds between GPS datapoints to  plot. */

    private  PicesDataBase   mainWinConn = null;
    private  PicesDataBase   threadConn  = null;  /**< This connection will only be use by the background thread. */

    private  PicesMsgQueue   statusMsgs = null;
    private  PicesMsgQueue   msgQueue   = null;
    private  PicesRunLog     runLog     = null;

    private  PicesGoalKeeper  goalie = new PicesGoalKeeper ("ChartGPS");

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

    private  class  PlotRequest
    {
      public  PlotRequest (PicesSipperDeployment  _deployment, 
                           Color                  _color
                          )
      {
        deployment = _deployment;
        color      = _color;
      }

      public  PicesSipperDeployment  deployment  = null;        // Class or Instrument name.
      public  Color                  color       = Color.Black;
    }


    List<PlotRequest>  plotRequests = null;


    private  class  DataSeriesToPlot
    {
      public  DataSeriesToPlot (PicesSipperDeployment  _deployment,
                                PicesGPSDataPointList  _data
                               ) 
      {
        deployment = _deployment;
        data       = _data;
      }

      public  void  LocateGPSBounds ()
      {
        foreach  (PicesGPSDataPoint  dp in data)
        {
          if  (dp.AvgLatitude > latitudeMax)
            latitudeMax = dp.AvgLatitude;
          if  (dp.AvgLatitude < latitudeMin)
            latitudeMin = dp.AvgLatitude;

          if  (dp.AvgLongitude > longitudeMax)
            longitudeMax = dp.AvgLongitude;
          if  (dp.AvgLongitude < longitudeMin)
            longitudeMin = dp.AvgLongitude;
        }
      }

      /** Returns true if this series Min/Max place it with in the threshold area odf the specified point. */
      public  bool  WithInThreshold (double  longitude,  double longTh,
                                     double  latitude,   double latTh
                                    )
      {
        return  (((longitude + longTh) > longitudeMin)   &&  ((longitude - longTh) < longitudeMax)  &&
                 ((latitude  + latTh)  > latitudeMin)    &&  ((latitude  - latTh)  < latitudeMax)
                );
      }


      public  PicesGPSDataPoint ClosestPoint (double  longitude,  double longTh,
                                              double  latitude,   double latTh,
                                              ref int closestPointIndex,
                                              ref double  closestPointSquareDist
                                             
                                             )
      {
        PicesGPSDataPoint   closestPoint = null;
        closestPointSquareDist = double.MaxValue;
        closestPointIndex = -1;

        double longitudeMin = longitude - longTh;
        double longitudeMax = longitude + longTh;
        double latitudeMin  = latitude  - longTh;
        double latitudeMax  = latitude  + longTh;

        int  index = 0;
        foreach  (PicesGPSDataPoint dp in data)
        {
          if  ((dp.AvgLongitude >= longitudeMin)  &&  (dp.AvgLongitude <= longitudeMax))
          {
            if  ((dp.AvgLatitude >= latitudeMin)  &&  (dp.AvgLatitude <= latitudeMax))
            {
              double  deltaLatitude  = (dp.AvgLatitude  - latitude)  / latTh;
              double  deltaLongitude = (dp.AvgLongitude - longitude) / longTh;

              double  squareDist = deltaLatitude * deltaLatitude + deltaLongitude * deltaLongitude;
              if  (squareDist < closestPointSquareDist)
              {
                closestPoint = dp;
                closestPointSquareDist = squareDist;
                closestPointIndex = index;
              }
            }
          }
          index++;
        }

        return  closestPoint;
      }

      public  PicesSipperDeployment  deployment     = null;
      public  PicesGPSDataPointList  data           = null;
      public  double                 latitudeMin    = double.MaxValue;
      public  double                 latitudeMax    = double.MinValue;
      public  double                 longitudeMin   = double.MaxValue;
      public  double                 longitudeMax   = double.MinValue;
    }

    
    private  List<DataSeriesToPlot>  series = new List<DataSeriesToPlot> ();




    public ChartGPS (String  _cruise,
                     String  _station,
                     String  _deployment
                    )
    {
      cruise        = _cruise;
      station       = _station;
      deployment    = _deployment;

      statusMsgs = new PicesMsgQueue ("ChartGPS-StatusMsgs");
      msgQueue   = new PicesMsgQueue ("ChartGPS-RunLog");
      runLog     = new PicesRunLog (msgQueue);

      mainWinConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      configFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "ChartGPS.cfg";

      InitializeComponent ();
    }

   
    private void ChartGPS_Load (object sender, EventArgs e)
    {
      heightLast = Height;
      widthLast  = Width;

      DeploymentsToPlot.Items.Clear ();
      
      Cruise.Text     = cruise;
      Station.Text    = station;
      Deployment.Text = deployment;

      ContextMenuStrip cms = new ContextMenuStrip ();
      cms.Items.Add ("Copy Chart to clipboard",               null, CopyChartToClipboard);
      cms.Items.Add ("Save Chart to Disk",                    null, SaveChartToDisk);
      cms.Items.Add ("Copy Data Tab-Delimited to Clipboard",  null, SaveTabDelToClipBoard);
      cms.Items.Add ("Save Data Tab-Delimited to Disk",       null, SaveTabDelToDisk);

      ProfileChart.ContextMenuStrip = cms;

      LoadConfigurationFile ();

      PopulatDeploymentsToPlot ();

      PlotButton_Click (this, null);
    }


    private  void  PopulatDeploymentsToPlot ()
    {
      DeploymentsToPlot.Items.Clear ();
      PicesSipperDeploymentList  deployments = mainWinConn.SipperDeploymentLoad (cruise, "");
      if  (deployments == null)
        return;
      foreach  (PicesSipperDeployment  deployment in deployments)
      {
        DeploymentsToPlot.Items.Add (deployment, true);
      }
    }



    private  PicesSipperDeploymentList  CurrentlySelectedDeployments ()
    {
      PicesSipperDeploymentList  deployments = new PicesSipperDeploymentList ();

      foreach  (Object so in DeploymentsToPlot.CheckedItems)
        deployments.Add ((PicesSipperDeployment)so);

      return  deployments;
    }


    private  PicesSipperDeployment  CurrentlySelectedDeployment ()
    {
      PicesSipperDeploymentList  deployments = CurrentlySelectedDeployments ();
      if  ((deployments == null)  ||  (deployments.Count < 1))
        return null;
      else
        return  deployments[0];
    }


    private  void  DisableControls ()
    {
      TimeInterval.Enabled      = false;
      PlotButton.Enabled        = false;
      DeploymentsToPlot.Enabled = false;
      //PlotAction.Enabled        = false;
      TimeInterval.Enabled      = false;
    }


    private  void  EnableControls ()
    {
      TimeInterval.Enabled      = true;
      PlotButton.Enabled        = true;
      DeploymentsToPlot.Enabled = true;
      //PlotAction.Enabled        = true;
      TimeInterval.Enabled      = true;
    }


    private  void  SaveConfiguration ()
    {
      System.IO.StreamWriter  o = null;
      try{o = new System.IO.StreamWriter (configFileName);}  catch  (Exception){o = null; return;}
      
      o.WriteLine ("//ChartGPS Configuration File");
      o.WriteLine ("//");
      o.WriteLine ("//DateTime Saved" + "\t" + DateTime.Now.ToString ("F"));
      o.WriteLine ("//");
      o.WriteLine ("WidthLast"     + "\t" + widthLast);
      o.WriteLine ("HeightLast"    + "\t" + heightLast);
      o.WriteLine ("Maximized"     + "\t" + (formIsMaximized ? "YES":"NO"));
      //o.WriteLine ("PlotAction"    + "\t" + ((String)(PlotAction.SelectedItem)));
      o.WriteLine ("TimeInterval"  + "\t" + TimeInterval.Value);

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
            timeInterval = (int)PicesKKStr.StrToFloat (fieldValue);
            if  (timeInterval > 0)
              TimeInterval.Value = (decimal)timeInterval;
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


    private  List<String>  ValidateFields ()
    {
      List<String>  errors = new List<string> ();

      timeInterval = (int)TimeInterval.Value;
      if  (timeInterval < 1)
        errors.Add ("TimeInterval must be greater than zero.");

      if  (DeploymentsToPlot.CheckedItems.Count < 1)
        errors.Add ("You must have at least one Deployment Checked.");
      
      if  (errors.Count < 1)
        return null;
      else
        return errors;
    }





    private  void  AddDeploymentToSeries (PicesSipperDeployment  deployment,
                                          Color                  color
                                         )
    {
      PicesGPSDataPointList  gpsData = threadConn.InstrumentDataRetrieveGPSInfo (deployment.CruiseName, deployment.StationName, deployment.DeploymentNum, (int)TimeInterval.Value);
      if  ((gpsData == null)  ||  cancelRequested)
        return;

      // Filter out noisey GPS data.

      PicesGPSDataPointList  fliteredGPSData = new PicesGPSDataPointList ();
      double  lastLat = 0.0;
      double  lastLong = 0.0;

      foreach (PicesGPSDataPoint dp in gpsData)
      {
        if  ((Math.Abs (dp.AvgLongitude ) < 0.1)  ||  (Math.Abs (dp.AvgLatitude) < 0.1))
          continue;
        if  ((dp.AvgLatitude == lastLat)  &&  (dp.AvgLongitude == lastLong))
          continue;
        fliteredGPSData.Add (dp);
        lastLong = dp.AvgLongitude;
        lastLat  = dp.AvgLatitude;
      }

      gpsData = fliteredGPSData;
      fliteredGPSData = null;

      if  (gpsData.Count < 2)
      {
        fliteredGPSData = gpsData;
      }
      else
      {
        double  totalSquareDist = 0.0;
        double[] distBP = new double[gpsData.Count - 1];
        double  totalSquareDistSquare = 0.0;
        for  (int x = 0;  x < (gpsData.Count - 1);  ++x)
        {
          double  deltaLong  = gpsData[x + 1].AvgLongitude - gpsData[x].AvgLongitude;
          double  deltaLat   = gpsData[x + 1].AvgLatitude  - gpsData[x].AvgLatitude;
          double  squareDist = deltaLong * deltaLong + deltaLat * deltaLat;
          totalSquareDist += squareDist;
          totalSquareDistSquare += squareDist * squareDist;
          distBP[x] = squareDist;
        }

        double  meanSquareDist = totalSquareDist / distBP.Length;
        double  variance = (totalSquareDistSquare / distBP.Length) - (meanSquareDist * meanSquareDist);
        double  stdDev = Math.Sqrt (variance);

        double  distSquareTH = stdDev* 10;
        int  firstIdx = 0;
        if  (distBP.Length > 11)
        {
          for  (int x = 0;  x < 10;  ++x)
          {
            if  (distBP[x] > distSquareTH)
              firstIdx = x + 1;
          }
        }
        
        fliteredGPSData = new PicesGPSDataPointList ();
        for  (int x = firstIdx;  x < gpsData.Count;  ++x)
        {
          PicesGPSDataPoint dp = gpsData[x];
          fliteredGPSData.Add (dp);
        }
      }

      DataSeriesToPlot  seriesToPlot = new DataSeriesToPlot (deployment, fliteredGPSData);
      seriesToPlot.LocateGPSBounds ();

      goalie.StartBlock ();
      series.Add (seriesToPlot);
      goalie.EndBlock ();
    }  /* AddClassToSeries */




    /// <summary>
    /// This next methid will be ran as a separate thread; it is respnable for collecting all the data needed to generate the plot.
    /// </summary>
    private  void  BuildPlotData ()
    {
      if  (buildPlotDataRunning)
        return;

      PicesDataBase.ThreadInit ();

      threadConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      buildPlotDataRunning = true;

      timeInterval = (int)TimeInterval.Value;

      series = new List<DataSeriesToPlot> ();

      foreach  (PlotRequest pr in plotRequests)
      {
        if  (cancelRequested)
          break;
        AddDeploymentToSeries (pr.deployment, pr.color);
      }

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




    private  void  AddSeriesToChart (DataSeriesToPlot  dataSeries)
    {
      PicesGPSDataPointList  gpsData    = dataSeries.data;
      PicesSipperDeployment  deployment = dataSeries.deployment;
        
      TimeSpan adjToTime = deployment.SyncTimeStampActual - deployment.SyncTimeStampCTD;

      DateTime startDateTime = new DateTime (1, 1, 1, 1, 1, 1);
      TimeSpan startTime = new TimeSpan (0, 0, 0);
      TimeSpan endTime   = new TimeSpan (0, 0, 0);
      if  (gpsData.Count > 0)
      {
        startDateTime = gpsData[0].CtdDateTime;
        startTime = gpsData[0].GPSStartTime;
      }

      if  (gpsData.Count > 1)
        endTime = gpsData[gpsData.Count - 1].GPSStartTime;

      startDateTime.Add (adjToTime);
      startTime.Add (adjToTime);
      endTime.Add (adjToTime);

      String  startTimeStr = startTime.Hours.ToString ("00") + ":" + startTime.Minutes.ToString ("00");
      String  endTimeStr   = endTime.Hours.ToString   ("00") + ":" + endTime.Minutes.ToString   ("00");

      String  legend = deployment.StationName;
      if  (!String.IsNullOrEmpty (deployment.DeploymentNum))
        legend += "-" + deployment.DeploymentNum;

      legend += " (" + startDateTime.ToString ("MMM/dd HH:mm") +")";

      Series s = new Series (legend);
      s.ChartType = SeriesChartType.Point;
      s.ChartArea = "ChartArea1";
      s.Name = legend;
      s.XAxisType = AxisType.Primary;
      s.BorderWidth = 2;

      s.Points.Clear ();
      foreach  (PicesGPSDataPoint  dp in gpsData)
      {
        DataPoint dataPoint = new DataPoint (dp.AvgLongitude, dp.AvgLatitude);
        //dataPoint.AxisLabel = PicesMethods.LongitudeToString (dp.AvgLongitude, 2);
        //= PicesMethods.LatitudeToString (dp.AvgLatitude, 3);
        s.Points.Add (dataPoint);
      }

      if  (s.Points.Count > 0)
        s.Points[0].Label = startTimeStr;

      if  (s.Points.Count > 10)
        s.Points[s.Points.Count - 1].Label = endTimeStr;


      ProfileChart.Series.Add (s);
    }


    private  void  UpdateChartAreas ()
    {
      goalie.StartBlock ();

      timeInterval = (int)TimeInterval.Value;
      //String  plotAction = PlotAction.SelectedText;

      PicesSipperDeploymentList  selectedDeployments = CurrentlySelectedDeployments (); 

      String titleLine = "Cruise: " + cruise;
      
      titleLine += "  Time-Interval: " + timeInterval;
     
      ProfileChart.Titles.Clear ();
      ProfileChart.Titles.Add (titleLine);

      if  (series.Count < 1)
      {
        ProfileChart.Series.Clear ();
        goalie.EndBlock ();
        return;
      }

      ChartArea ca = ProfileChart.ChartAreas[0];
      //ca.AxisY.Title = "Latitude";

      //ca.AxisX.Title = "Longitude";
      Font f = new Font (ca.AxisX.TitleFont.FontFamily, 10);
      ca.AxisX.TitleFont = f;
      f = new Font (ca.AxisY.TitleFont.FontFamily, 10);
      ca.AxisY.TitleFont = f;

      ProfileChart.Series.Clear ();

      // First we will plot class counts on CharArea1
      foreach  (DataSeriesToPlot dstp in series)
        AddSeriesToChart (dstp);

      ca.AxisX.Minimum = Double.NaN;
      ca.AxisX.Maximum = Double.NaN;
      ca.AxisY.Minimum = Double.NaN;
      ca.AxisY.Maximum = Double.NaN;

      ProfileChart.ChartAreas[0].RecalculateAxesScale ();

      goalie.EndBlock ();
    }  /* UpdateChartAreas */



    private  void  SchedulePlotRequests ()
    {
      //String  plotAction = PlotAction.SelectedText;

      PicesSipperDeploymentList  deploymentsRequested = CurrentlySelectedDeployments ();
      if  (deploymentsRequested == null)  
        deploymentsRequested = new PicesSipperDeploymentList ();

      plotRequests = new List<PlotRequest> ();

      foreach  (PicesSipperDeployment  deployment in  deploymentsRequested)
      {
        plotRequests.Add (new PlotRequest (deployment, Color.Black));
      }
         
    }  /* SchedulePlotRequests */



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

        SchedulePlotRequests ();

        timer1.Enabled = true;
        Thread t = new Thread (new ThreadStart (BuildPlotData));
        t.Start ();
      }
    }


    private void RePlotButton_Click (object sender, EventArgs e)
    {
      UpdateChartAreas ();
    }


    int ticks = 0;
    private void timer1_Tick (object sender, EventArgs e)
    {
      ++ticks;

      String msg = statusMsgs.GetNextMsg ();
      while  (msg != null)
      {
        Status.Text = msg;
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


    private void ChartGPS_Resize (object sender, EventArgs e)
    {
      int  heightDelta = Height - heightLast;
      int  widthDelta  = Width  - widthLast;

      ProfileChart.Height += heightDelta;
      ProfileChart.Width  += widthDelta;

      heightLast = Height;
      widthLast  = Width;
    }



    private void ChartGPS_SizeChanged (object sender, EventArgs e)
    {
      if  (WindowState == FormWindowState.Maximized)
      {
        // Looks like user has pressed the Maximized button.  We have to trap it here because
        // the ResizeEnd envent does not trap when form is Maximized.
        //PicesCommanderFormResized ();
        ChartGPS_Resize (sender, e);
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
          ChartGPS_Resize (sender, e);
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
      sfd.Title = "Save GPS info as tab-delimited data to disk.";
      sfd.DefaultExt = "txt";
      sfd.AddExtension = true;
      sfd.OverwritePrompt = true;
      //sfd.CheckFileExists = true;
      DialogResult dr = sfd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        String  fn = sfd.FileName;
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
      tw.WriteLine ("ChartGPS"); 
      tw.WriteLine ("DateTime" + "\t" + curTime.ToString ("yyyy/MM/dd HH:mm:ss")); 
      tw.WriteLine ();
      foreach (DataSeriesToPlot dstp in series)
      {
        PicesGPSDataPointList  gpsData    = dstp.data;
        PicesSipperDeployment  deployment = dstp.deployment;

        tw.WriteLine ("Deployment" + "\t" + deployment.CruiseName + "\t" + deployment.StationName + "\t" + deployment.DeploymentNum);
        tw.WriteLine ("DeploymentDescription" + "\t" +  deployment.Description);

        tw.WriteLine ("CtdDateTime" + "\t" + "AvgLongitude" + "\t" + "AvgLatitude" + "\t" + "StdGps");
        foreach  (PicesGPSDataPoint dp in gpsData)
        {
          tw.WriteLine (dp.CtdDateTime.ToString ("yyyy/MM/dd HH:mm:ss") + "\t" + dp.AvgLongitude.ToString ("##0.00000000") + "\t" + dp.AvgLatitude.ToString ("##0.00000000") + "\t" + PicesMethods.LatitudeLongitudeToString (dp.AvgLatitude , dp.AvgLongitude));;
        }
        tw.WriteLine ("----EndOfDeoploymemt----");
        tw.WriteLine ();
      }

      tw.WriteLine ("End of ChartGPS");
    }



    private void ChartGPS_FormClosing (object sender, FormClosingEventArgs e)
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


    PicesGPSDataPoint  lastClosestPoint       = null;
    int                lastClosestSeriesIndex = -1;
    int                lastClosestPointIndex  = -1;


    private  void  HighLightClosestPoint (int                closestPointSeriesIndex,
                                          int                closestPointPointIndex,
                                          PicesGPSDataPoint  closestPoint
                                         )
    {
      if  ((lastClosestSeriesIndex >= 0)  &&  (lastClosestSeriesIndex < ProfileChart.Series.Count))
      {
        if  ((lastClosestPointIndex >= 0)  &&  (lastClosestPointIndex < ProfileChart.Series[lastClosestSeriesIndex].Points.Count))
        {
          ProfileChart.Series[lastClosestSeriesIndex].Points[lastClosestPointIndex].Label = "";
          ProfileChart.Series[lastClosestSeriesIndex].Points[lastClosestPointIndex].MarkerSize = ProfileChart.Series[closestPointSeriesIndex].MarkerSize;
        }
      }

      lastClosestPoint       = null;
      lastClosestSeriesIndex = -1;
      lastClosestPointIndex  = -1;

      if  ((closestPointSeriesIndex >= 0)  &&  (closestPointSeriesIndex < ProfileChart.Series.Count))
      {
        if  ((closestPointPointIndex >= 0)  &&  (closestPointPointIndex < ProfileChart.Series[closestPointSeriesIndex].Points.Count))
        {
          ProfileChart.Series[closestPointSeriesIndex].Points[closestPointPointIndex].Label = closestPoint.CtdDateTime.ToString ("HH:mm");
          ProfileChart.Series[closestPointSeriesIndex].Points[closestPointPointIndex].MarkerSize = ProfileChart.Series[closestPointSeriesIndex].MarkerSize * 3;
          lastClosestPoint       = closestPoint;
          lastClosestSeriesIndex = closestPointSeriesIndex;
          lastClosestPointIndex  = closestPointPointIndex;
        }
      }
    }




    private void ProfileChart_MouseClick (object sender, MouseEventArgs e)
    {
      Point  p = e.Location;
      //MouseClickLocation.Text = p.X.ToString () + "," + p.Y.ToString ();
      ChartArea  ca = ProfileChart.ChartAreas[0];
      double longitude = ca.AxisX.PixelPositionToValue ((double)p.X);
      double latitude  = ca.AxisY.PixelPositionToValue ((double)p.Y);

      // Determine Threshold.
      int  thInPixels = 25;
      double  x = ca.AxisX.PixelPositionToValue ((double)(p.X + thInPixels));
      double  degLongPerPixel = Math.Abs (x - longitude) / thInPixels;

      double  y = ca.AxisY.PixelPositionToValue ((double)(p.Y + thInPixels));
      double  degLatPerPixel = Math.Abs (y - latitude) / thInPixels;

      double  longTH = degLongPerPixel * thInPixels;
      double  latTH  = degLatPerPixel  * thInPixels;

      PicesGPSDataPoint  closestPoint = null;
      int                closestPointIndex = -1;
      int                closestPointSeriesIndex = -1;
      int                seriesIndex = 0;
      double             closestPointDistSquare = double.MaxValue;
      foreach  (DataSeriesToPlot  dstp in series)
      {
        if  (dstp.WithInThreshold (longitude, longTH, latitude, latTH))
        {
          double distSquare = double.MaxValue;
          int    closestPointInSeriesIndex = -1;
          PicesGPSDataPoint  closestPointInSeries = dstp.ClosestPoint (longitude, longTH, latitude, latTH, ref closestPointInSeriesIndex, ref distSquare);
          if  (closestPointInSeries != null)
          {
            if  (distSquare < closestPointDistSquare)
            {
              closestPointIndex       = closestPointInSeriesIndex;
              closestPointSeriesIndex = seriesIndex;
              closestPointDistSquare  = distSquare;
              closestPoint            = closestPointInSeries;
            }
          }
        }
        seriesIndex++;
      }

      if  (closestPoint != null)
        HighLightClosestPoint (closestPointSeriesIndex, closestPointIndex, closestPoint);

      String  gpsStr = PicesMethods.LatitudeLongitudeToString (latitude, longitude);

      CurGPSLocation.Text = gpsStr;
    }



    private void ClearAllButton_Click (object sender, EventArgs e)
    {
      for  (int x = 0;  x < DeploymentsToPlot.Items.Count;  ++x)
        DeploymentsToPlot.SetItemCheckState (x, CheckState.Unchecked);
    }



    private void SelectAllButton_Click (object sender, EventArgs e)
    {
      for  (int x = 0;  x < DeploymentsToPlot.Items.Count;  ++x)
        DeploymentsToPlot.SetItemCheckState (x, CheckState.Checked);
    }
  }
}
