using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using  SipperFile;
using  PicesInterface;
//**********************************************************************************
//* Developer:  Kurt Kramer                                                        *
//* Date        2010-01-08                                                         *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//**********************************************************************************

namespace PicesCommander
{
  public partial class InstrumentDataByDeploymentProfile : Form
  {
    // Importing Thread Related Variables.
    private  bool                    cancelBackGround   = false;
    private  bool                    backGroundStarted  = false;  // Will be set to 'true' after the start button is pressed
                                                                 // and allvalidations are passed.
    private  bool                    backGroundRunning   = false;
    private  Thread                  backGroundThread    = null;
    private  PicesRunLog             backGroundLog       = null;


    private  PicesGoalKeeper         runLogGoalie   = null;
    private  Queue<String>           runLogMsgQueue = null;  // List of messages that can be added to 'RunLog'.

    public   bool                    CancelBackGround     {get {return cancelBackGround;}   set  {cancelBackGround = value;}}
    public   bool                    BackGroundRunning    {get {return backGroundRunning;}}


    private  String                  cruiseName    = "";
    private  String                  stationName   = "";
    private  String                  deploymentNum = "";
    private  PicesClass              mlClass    = null;

    private  String                  reportFileDir = "";

    private  String                  reportFileName = "";

    private  bool                    validationErrorFound = true;


    public InstrumentDataByDeploymentProfile (String      _cruiseName,
                                              String      _stationName,
                                              String      _deploymentNum,
                                              PicesClass  _mlClass
                                             )
    {
      cruiseName    = _cruiseName;
      stationName   = _stationName;
      deploymentNum = _deploymentNum;
      mlClass    = _mlClass;

      reportFileDir = OSservices.AddSlash (PicesSipperVariables.PicesReportDir ()) + "InstrumentSummariesByDeployment";

      runLogGoalie = new PicesGoalKeeper ("InstrumentDataByDeploymentProfile");

      InitializeComponent();
    }



    private void  RunLogAddMsg (String msg)
    {
      runLogGoalie.StartBlock();

      if (runLogMsgQueue == null)
        runLogMsgQueue = new Queue<String>();

      runLogMsgQueue.Enqueue(msg);

      runLogGoalie.EndBlock();
    }  /* RunLogAddMsg */



    private void  RunLogMsgQueueFlush ()
    {
      runLogGoalie.StartBlock();
      if (runLogMsgQueue != null)
      {
        while (runLogMsgQueue.Count > 0)
        {
          String msg = runLogMsgQueue.Dequeue ();
          RunLog.AppendText (msg + "\n");
          backGroundLog.Writeln (msg);
        }
      }

      runLogGoalie.EndBlock ();
    }  /* RunLogMsgQueueFlush */



      
    private void textBox1_TextChanged(object sender, EventArgs e)
    {

    }


    private void InstrumentDataByDeploymentProfile_Load(object sender, EventArgs e)
    {
      Cruise.Text     = cruiseName;
      Station.Text    = stationName;
      Deployment.Text = deploymentNum;
    }

    private void  ValidateAll ()
    {
      validationErrorFound = false;
    }


    private  void  CancelBackGroundProcess ()
    {
      cancelBackGround = true;
    }  /* CancelBackGroundProcess */



    private  void  StartTheBackGroundProcedure ()
    {
      Start.Enabled    = false;
      Cancel.Enabled   = true;
      cancelBackGround = false;
      backGroundThread = new Thread (new ThreadStart (BackGroundProcess));
      backGroundThread.Start ();

      BackGroundTimer.Enabled = true;
    }  /* StartTheBackGroundProcedure */



    private  void  BackGroundProcess ()
    {
      backGroundRunning   = true;

      PicesDataBase.ThreadInit ();


      OSservices.CreateDirectoryPath (reportFileDir);

      DateTime n = DateTime.Now;
      String  logFileName = OSservices.AddSlash(PicesSipperVariables.PicesTempDirectory()) + "InstrumentDataByDeployment_Log_" +
                             n.Year.ToString("0000") + "-" + n.Month.ToString("00") + "-" + n.Day.ToString("00") + "_" +
                             n.Hour.ToString("00") + "-" + n.Minute.ToString("00") + "-" + n.Hour.ToString("00") +
                             ".txt";

      backGroundLog = new PicesRunLog (logFileName);
      PicesDataBase  threadConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (backGroundLog);

      RunLogAddMsg ("Cruise           [" + cruiseName                                    + "]" + "\n");
      RunLogAddMsg ("Station          [" + stationName                                   + "]" + "\n");
      RunLogAddMsg ("Deployment       [" + deploymentNum                                 + "]" + "\n");
      RunLogAddMsg ("Start Date/Time  [" + DateTime.Now.ToString("yyyy-MMM-dd HH:mm:ss") + "]" + "\n");
      RunLogAddMsg ("Active DataBase  [" + threadConn.Server.Description                 + "]" + "\n");

      PicesSipperDeploymentList  deployments = null;
      
      if  (!String.IsNullOrEmpty (deploymentNum))
      {
        PicesSipperDeployment  d = threadConn.SipperDeploymentLoad (cruiseName, stationName, deploymentNum);
        if  (d != null)
        {
          deployments = new PicesSipperDeploymentList ();
          deployments.Add (d);
        }
      }
      else
      {
        deployments = threadConn.SipperDeploymentLoad (cruiseName, stationName);
      }

      if  (deployments == null)
      {
        RunLogAddMsg ("\n\nNo Deployments for Specified Criteria!\n\n");
        return;
      }

      int  reportNum = 0;
      foreach (PicesSipperDeployment d in deployments)
      {
        if  (cancelBackGround)
          break;
        
        reportNum++;

        RunLogAddMsg ("Generating Report " + reportNum.ToString () + " of " + deployments.Count.ToString ());

        SummarizeInstrumentDataForADeployment (threadConn, d.CruiseName, d.StationName, d.DeploymentNum);
      }

      threadConn.Close ();
      threadConn = null;
      GC.Collect ();

      PicesDataBase.ThreadEnd ();

      backGroundRunning = false;
    }  /* BackGroundProcess */

     
    
    
    private  int  DatePicesInstrumentComparer (PicesInstrumentData  i1,
                                               PicesInstrumentData  i2
                                              )
    {
      if  (i1.CtdDate < i2.CtdDate)
        return -1;

      else if  (i1.CtdDate > i2.CtdDate)
        return  1;

      else if  (i1.Depth < i2.Depth)
        return -1;

      else if  (i1.Depth > i2.Depth)
        return 1;

      return 0;
    }  /* DatePicesInstrumentComparer */

    
    
    private  void  FindScanRates (PicesDataBase  threadConn,
                                  String         cruiseName, 
                                  String         stationName,
                                  String         deploymentNum,
                                  ref float      scanRateMin,
                                  ref float      scanRateMax
                                 )  
    {
      // Because Scan Rates are stored with each individule SIPPER File we will need to 
      // peruse though all of them for this deployment.  Idealy they will all have the 
      // same scan rate.
      scanRateMin = scanRateMax = 24950.0f;
      PicesSipperFileList  sipperFiles = threadConn.SipperFileLoad (cruiseName, stationName, deploymentNum);
      if  (sipperFiles == null)
        return;
      scanRateMin = float.MaxValue;
      scanRateMax = float.MinValue;

      foreach  (PicesSipperFile sf in sipperFiles)
      {
        scanRateMin = Math.Min (scanRateMin, sf.ScanRate);
        scanRateMax = Math.Max (scanRateMax, sf.ScanRate);
      }
    }  /* FindScanRates */



    private  PicesInstrumentDataList  StripOutObviouslyBadrecs (PicesInstrumentDataList src)
    {
      PicesInstrumentDataList  dst = new PicesInstrumentDataList ();
      foreach  (PicesInstrumentData  id in  src)
      {
        if  ((id.Temperature  >  0.0f)  &&  (id.Temperature  <= 40.0f)  &&
             (id.Salinity     > 20.0f)  &&  (id.Salinity     <= 40.0f)  &&
             (id.Density      > 18.0f)  &&  (id.Density      <= 40.0f)  &&
             (id.Fluorescence >  0.0f)  &&  (id.CtdDate.Year > 2000)
            )
        {
          dst.Add (id);
        }

      }

      return  dst;
    }  /* StripOutObviouslyBadrecs */




      
    private  class  ImagesDepthStats
    {
      public bool  upCast;
      public int   bucketIdx;
      public float bucketDepth;
      public int   imageCount;
      public int   totalPixelCount;

      public bool  counted;

      public  ImagesDepthStats (bool  _upCast,
                                int   _bucketIdx,
                                float _bucketDepth,
                                int   _imageCount,
                                int   _totalPixelCount
                               )
      {
        upCast           = _upCast;
        bucketIdx        = _bucketIdx;
        bucketDepth      = _bucketDepth;
        imageCount       = _imageCount;
        totalPixelCount  = _totalPixelCount;
        counted          = false;
      }
    };




    private  void  LoadImageDepthStats (PicesDataBase               threadConn,
                                        String                      cruiseName,
                                        String                      stationName,
                                        String                      deploymentNum,
                                        ref List<ImagesDepthStats>  downCast,
                                        ref List<ImagesDepthStats>  upCast
                                       )
    {
      String  sqlStr = "call ImagesStatsByUpAndDownCast(";
      sqlStr += "\"" + cruiseName    + "\"" + ", ";
      sqlStr += "\"" + stationName   + "\"" + ", ";
      sqlStr += "\"" + deploymentNum + "\"" + ", ";
      if  (mlClass == null)
        sqlStr += "\"\", ";
      else
        sqlStr = "\"" + mlClass.Name + "\"" + ", ";
      sqlStr += "1.0);";

      downCast = null;
      upCast   = null;

      String[]  cols = {"UpCast", "BucketIdx", "BucketDepth", "ImageCount", "TotalPixelCount"};
      String[][] results  = threadConn.QueryStatement (sqlStr, cols);

      if  (results == null)
      {
        RunLogAddMsg ("Error Retrieving Images Cout Statistics.");
        RunLogAddMsg (threadConn.LastErrorDesc ());
        return;
      }

      downCast = new List<ImagesDepthStats> ();
      upCast   = new List<ImagesDepthStats> ();

      foreach  (String[] row in results)
      {
        bool   goingUp         = (row[0] == "1");
        int    bucketIdx       = PicesKKStr.StrToInt   (row[1]);
        float  bucketDepth     = PicesKKStr.StrToFloat (row[2]);
        int    imageCount      = PicesKKStr.StrToInt   (row[3]);
        int    totalPixelCount = PicesKKStr.StrToInt   (row[4]);

        ImagesDepthStats  stats = new ImagesDepthStats (goingUp, bucketIdx, bucketDepth, imageCount, totalPixelCount);
        if  (!goingUp)
          downCast.Add (stats);
        else
          upCast.Add (stats);
      }

      return;
    }  /* LoadImageDepthStats */
                                  



    private  void  SummarizeInstrumentDataForADeployment (PicesDataBase  threadConn,
                                                          String         cruiseName, 
                                                          String         stationName,
                                                          String         deploymentNum
                                                         )
    {
      RunLogAddMsg ("\nStart Report " + cruiseName + "-" + stationName + "-" + deploymentNum);

      PicesInstrumentDataList  data = threadConn.InstrumentDataLoad (cruiseName, stationName, deploymentNum);
      // We will assume that all entries are in order of date and time.  We want 
      // to first decide where the break in UpCast and Down cast occur.
       
      if (cancelBackGround)
        return;

      data = StripOutObviouslyBadrecs (data);
      if  ((data == null)  ||  (data.Count < 1))
      {
        RunLogAddMsg ("There is no Instrument Data for Cruise[" + cruiseName + "]  Station[" +  stationName + "]  Deployment [" + deploymentNum + "]");
        return;
      }

      RunLogAddMsg ("Num Entries Loaded[" + data.Count.ToString () + "]");
      
      data.Sort (DatePicesInstrumentComparer);

      int    idx;
      float  maxDepth = -9999.9f;
      int    maxDepthIdx = - 9999;

      for  (idx = 0;  idx < data.Count;  idx++)
      {
        if  (data[idx].Depth > maxDepth)
        {
          maxDepth = data[idx].Depth;
          maxDepthIdx = idx;
        }
      }
      
      DateTime  refTime   = new DateTime (1999, 1, 1);
      DateTime  startTime = new DateTime (1999, 1, 1);
      bool  startTimeSet = false;

      InstrumentDataList  downCast = new InstrumentDataList ();
      InstrumentDataList  upCast   = new InstrumentDataList ();


      for  (idx = 0;  idx < data.Count;  idx++)
      {
        PicesInstrumentData pid = data[idx];
        if  (pid.CtdDate.CompareTo (refTime) < 0)
          continue;

        if  (!startTimeSet)
        {
          startTime = pid.CtdDate;
          startTimeSet = true;
        }

        if  (idx <= maxDepthIdx)
          downCast.Add (new InstrumentData (pid));
        else
          upCast.Add   (new InstrumentData (pid));
      }

      DateTime  endTime = data[data.Count - 1].CtdDate;

      data = null;

      String  title = cruiseName  + "-" + stationName + "-" + deploymentNum + "  From [" + startTime.ToString ("u") + "]  to  [" + endTime.ToString ("u") + "]";

      reportFileName = OSservices.AddSlash (reportFileDir) +
                       cruiseName    + "-" + 
                       stationName   + "-" + 
                       deploymentNum;
      
      if  (mlClass != null)
        reportFileName += "-" + mlClass.Name;
      reportFileName += "_InsrumentSumary.txt";

      System.IO.StreamWriter  o = null;

      try
      {
        o = new System.IO.StreamWriter (reportFileName);
      }
      catch  (Exception e)
      {
        RunLogAddMsg ("Error opening file[" + reportFileName + "] for output." + "\n\n" + e.ToString ());
        return;
      }

      if  (threadConn != null)
        o.WriteLine ("DataBase"    + "\t" + threadConn.Server.DataBaseName);

      o.WriteLine ("Cruise"      + "\t" + cruiseName);
      o.WriteLine ("Station"     + "\t" + stationName);
      o.WriteLine ("Deployment"  + "\t" + deploymentNum);
      o.WriteLine ("StartTime"   + "\t" + startTime.ToString ("u"));
      o.WriteLine ("EndTime"     + "\t" + endTime.ToString ("u"));
      o.WriteLine ();

      float  scanRateMin = 0.0f;
      float  scanRateMax = 0.0f;
      float  scanRate    = 0.0f;


      RunLogAddMsg ("Getting Particle Counts");

      List<ImagesDepthStats>  downCastImageStats = null;
      List<ImagesDepthStats>  upCastImageStats   = null;

      LoadImageDepthStats (threadConn, cruiseName, stationName, deploymentNum, ref downCastImageStats, ref upCastImageStats);
      
      RunLogAddMsg ("Determining Scan Rate");
      FindScanRates (threadConn, cruiseName, stationName, deploymentNum, ref scanRateMin, ref scanRateMax);

      if  (scanRateMin == scanRateMax)
      {
        scanRate = scanRateMin;
        RunLogAddMsg ("ScanRate ["  + scanRateMin.ToString ("###,##0.00") + "]");
      }
      else
      {
        RunLogAddMsg ("Scan Rates Were not consistent,  you should update Sipper Files.");
        RunLogAddMsg ("   scanRateMin [" + scanRateMin.ToString ("###,##0.00") + "]" +
                      "   scanRateMax [" + scanRateMax.ToString ("###,##0.00") + "]"
                     );
        
        o.WriteLine ("Scan Rates Were not consistent,  you should update Sipper Files.");
        o.WriteLine ("   scanRateMin [" + scanRateMin.ToString ("###,##0.00") + "]" +
                     "   scanRateMax [" + scanRateMax.ToString ("###,##0.00") + "]"
                    );
        o.WriteLine ();
      }

      o.WriteLine ("ScanRate"  + "\t" + scanRateMin.ToString ("###,##0.00"));

      o.WriteLine ();
      o.WriteLine ();

      o.WriteLine ("Depth"                 + "\t" +
                   "ScanLines"             + "\t" +
                   "Duration"              + "\t" +
                   "ImageCount"            + "\t" +
                   "TotalPixelCount"       + "\t" +
                   "FlowRate"              + "\t" + "\t" +
                   "Temperature"           + "\t" + "\t" +
                   "Oxygen"                + "\t" + "\t" +
                   "OxygenSensor"          + "\t" + "\t" +
                   "Salinity"              + "\t" + "\t" +
                   "Density"               + "\t" + "\t" +
                   "Fluorensce"            + "\t" + "\t" +
                   "FluorensceSensor"      + "\t" + "\t" +
                   "Transmissivity"        + "\t" + "\t" +
                   "TransmissivitySensor"  + "\t" + "\t" +
                   "Turbidity"             + "\t" + "\t" +
                   "TurbiditySensor"       + "\t" + "\t" +
                   "RecordRate"
                  );

      WriteDepthSummary (o, downCast, downCastImageStats, scanRate);
      o.WriteLine ();
      if  (!cancelBackGround)
        WriteDepthSummary (o, upCast, upCastImageStats, scanRate);

      if  (cancelBackGround)
      {
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ("Report Canceled by user request");
      }

      o.Close ();
   }  /* SummarizeInstrumentDataForADeployment */



   float  Mean (List<float>  nums)
   {
     int  x;
     float  total = 0.0f;
     for  (x = 0;  x < nums.Count;  x++)
       total += nums[x];

     return  total / (float)nums.Count;
   }



   float  StdDev (List<float>  nums)
   {
     float mean = Mean (nums);

     int x = 0;

     float  totalSquared = 0.0f;

     for  (x = 0;  x < nums.Count;  x++)
     {
       float delta = nums[x] - mean;
       float squared = delta * delta;
       totalSquared += squared;
     }

     float variance = totalSquared / nums.Count;
     return  (float)Math.Sqrt (variance);
   }  /* StdDev */





   private  ImagesDepthStats  LocateImageDepthStats (List<ImagesDepthStats>  imageStats, 
                                                     float                   depth
                                                    )
   {
     ImagesDepthStats  result = null;

     if  (imageStats == null)
       return  null;

     if  (imageStats.Count < 2)
       return  null;

     float  bucketSize = 1.0f;

     foreach  (ImagesDepthStats  ids in imageStats)
     {
       float  top = ids.bucketDepth;
       float  bot = top + bucketSize;

       if  ((depth >= top)  &&  (depth < bot))
       {
         result = ids;
         break;
       }
     }

     return  result;
   }  /* LocateImageDepthStats */






   private  void  WriteDepthSummary (System.IO.StreamWriter  o,
                                     InstrumentDataList      cast,
                                     List<ImagesDepthStats>  imageStats,
                                     float                   scanRate
                                   )
   {
     int  zed = 0;

     cast.SortByDepth ();

     InstrumentData id = (InstrumentData)(cast[0]);

     while  ((id != null)  &&  (!cancelBackGround))
     {
       id = (InstrumentData)cast[zed];
       int  lastDepth = (int)Math.Round (id.Depth ());

       uint   scanLinesThisDepth = 0;

       List<float>  flowRate         =  new List<float> ();
       List<float>  temperature      =  new List<float> ();
       List<float>  oxygen           =  new List<float> ();
       List<float>  oxygenSensor     =  new List<float> ();
       List<float>  salinity         =  new List<float> ();
       List<float>  density          =  new List<float> ();
       List<float>  fluorensce       =  new List<float> ();
       List<float>  fluorensceSensor =  new List<float> ();
       List<float>  transmissivity   =  new List<float> ();
       List<float>  transmissivitySensor =  new List<float> ();
       List<float>  turbidity        =  new List<float> ();
       List<float>  turbiditySensor  =  new List<float> ();
       List<float>  recordRate       =  new List<float> ();

       while  ((lastDepth == (int)Math.Round (id.Depth ()))  &&  (id != null))
       {
         if  ((id.Temperature  () >  0.0f)  &&  (id.Temperature  () <= 40.0f)  &&
              (id.Salinity     () > 20.0f)  &&  (id.Salinity     () <= 40.0f)  &&
              (id.Density      () > 18.0f)  &&  (id.Density      () <= 40.0f)  &&
              (id.Fluorescence () >  -2.0f)
             )
         {
           flowRate.Add             (id.FlowRate1              );
           temperature.Add          (id.Temperature          ());
           salinity.Add             (id.Salinity             ());
           density.Add              (id.Density              ());
           fluorensce.Add           (id.Fluorescence         ());
           fluorensceSensor.Add     (id.FluorescenceSensor   ());
           transmissivity.Add       (id.Transmissivity       ());
           transmissivitySensor.Add (id.TransmissivitySensor ());
           turbidity.Add            (id.Turbidity            ());
           turbiditySensor.Add      (id.TurbiditySensor      ());
           recordRate.Add           (id.RecordingRate         );
           oxygen.Add               (id.Oxygen               ());
           oxygenSensor.Add         (id.OxygenSensor         ());
           scanLinesThisDepth += 4096;
         }

         zed++;
         if  (zed < cast.Count)
           id = (InstrumentData)cast[zed];
         else
         {
           id = null;
           break;
         }
       }

       float  timeAtThisDepth = 0.0f;
       if  (scanRate > 0.0f)
         timeAtThisDepth = scanLinesThisDepth / scanRate;

       int  imageCount      = 0;
       int  totalPixelCount = 0;
       ImagesDepthStats ids = LocateImageDepthStats (imageStats, lastDepth);
       if  (ids != null)
       {
         if  (ids.counted)
         {
           RunLogAddMsg ("WriteDepthSummary   Duplicate Use of Cast[" + (ids.upCast ? "Up":"Down") + "]  Depth[" + ids.bucketDepth + "]   Count[" + ids.imageCount + "]");
         }
         else
         {
           imageCount      = ids.imageCount;
           totalPixelCount = ids.totalPixelCount;
           ids.counted = true;
         }
       }

       o.WriteLine (lastDepth.ToString                   () + "\t" +
                    scanLinesThisDepth.ToString          () + "\t" +
                    timeAtThisDepth.ToString             () + "\t" +
                    imageCount.ToString                  () + "\t" +
                    totalPixelCount.ToString             () + "\t" +
                    Mean (flowRate).ToString             () + "\t" + StdDev (flowRate).ToString             () + "\t" +
                    Mean (temperature).ToString          () + "\t" + StdDev (temperature).ToString          () + "\t" +
                    Mean (oxygen).ToString               () + "\t" + StdDev (oxygen).ToString               () + "\t" +
                    Mean (oxygenSensor).ToString         () + "\t" + StdDev (oxygenSensor).ToString         () + "\t" +
                    Mean (salinity).ToString             () + "\t" + StdDev (salinity).ToString             () + "\t" +
                    Mean (density).ToString              () + "\t" + StdDev (density).ToString              () + "\t" +
                    Mean (fluorensce).ToString           () + "\t" + StdDev (fluorensce).ToString           () + "\t" +
                    Mean (fluorensceSensor).ToString     () + "\t" + StdDev (fluorensceSensor).ToString     () + "\t" +
                    Mean (transmissivity).ToString       () + "\t" + StdDev (transmissivity).ToString       () + "\t" +
                    Mean (transmissivitySensor).ToString () + "\t" + StdDev (transmissivitySensor).ToString () + "\t" +
                    Mean (turbidity).ToString            () + "\t" + StdDev (turbidity).ToString            () + "\t" +
                    Mean (turbiditySensor).ToString      () + "\t" + StdDev (turbiditySensor).ToString      () + "\t" +
                    Mean (recordRate).ToString           () + "\t" + StdDev (recordRate).ToString           ()
                   );
      }

      foreach  (ImagesDepthStats  ids in imageStats)
      {
        if  (!ids.counted)
        {
          RunLogAddMsg ("WriteDepthSummary   UnCounted [" + (ids.upCast ? "Up":"Down") + "]  Depth[" + ids.bucketDepth + "]   Count[" + ids.imageCount + "]");
          o.WriteLine (ids.bucketDepth.ToString ()     + "\t" +
                       "---"                           + "\t" +
                       "---"                           + "\t" +
                       ids.imageCount.ToString      () + "\t" +
                       ids.totalPixelCount.ToString () + "\t"
                      );
        }
      }
    }  /* WriteDepthSummary */




    private void BackGroundTimer_Tick(object sender, EventArgs e)
    {
      RunLogMsgQueueFlush ();

      if  (!backGroundRunning)
      {
        BackGroundTimer.Enabled = false;

        if (cancelBackGround)
        {
          MessageBox.Show (this, "Instrument Report Canceled.");
        }
        else
        {
          DialogResult dr = MessageBox.Show (this,
                                             "Report has completed.  Do you want to load ?", 
                                             "Instrument Data Report by Deployment Profile",
                                             MessageBoxButtons.YesNo
                                            );
          if  (dr == DialogResult.Yes)
          {
            Process proc = new Process();
            proc.StartInfo.WorkingDirectory = @"";
            proc.StartInfo.FileName               = reportFileName;
            proc.StartInfo.UseShellExecute        = true;
            proc.StartInfo.RedirectStandardOutput = false;
            proc.StartInfo.RedirectStandardError  = false;
            proc.Start();
          }
        }

        this.Close ();
      }
    }



    private void Start_Click(object sender, EventArgs e)
    {
      if  (backGroundStarted)
      {
        // This should not have been able to have happened.
        Start.Enabled = false;
        return;
      }

      ValidateAll ();
      if  (validationErrorFound)
        return;

      DialogResult  dr = MessageBox.Show (this, "Start Report generation?", "Instrument Data Summary Report", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
        return;

      Start.Enabled  = false;
      Cancel.Enabled = true;

      StartTheBackGroundProcedure ();
    }


    
    private void Cancel_Click(object sender, EventArgs e)
    {
      Cancel.Enabled = false;

      if  (!backGroundRunning)
      {
        this.Close ();
      }

      DialogResult dr = MessageBox.Show (this, "Cancel 'Instrument Data Summary Report' ?", "Inatrument Data Summary report", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        CancelBackGroundProcess ();
        RunLogAddMsg ("\nCancel Requested\n");
      }
      else
      {
        Cancel.Enabled = true;
      }
    }


    private void InstrumentDataByDeploymentProfile_FormClosing(object sender, FormClosingEventArgs e)
    {
      if  (backGroundRunning)
      {
        MessageBox.Show (this, "Report Generation is still running,  can not exit window.", "Instrument Data Summary Report");
        e.Cancel = true;
      }
    }
  }
}

