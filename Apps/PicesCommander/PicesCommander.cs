using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

using  PicesInterface;
using  SipperFile;

/**
 *@namespace  PicesCommander
 *@brief  Namespace for the PicesCommander Program.
 *@details The PicesCommansder application consists of several modules.  This program is meant to be the frontend to the
 * image database, provide a GUI front-end to several application ssuch as ClassificatioStatus, ImportgroupAssignments, 
 * SameImagesToDisk, Harvestor and others.
 */
namespace PicesCommander
{
  public partial class PicesCommander : Form
  {
    String  configFileName = null;

    //*  Variables needed for resizing 
    private  int   lastHeight      = 0;
    private  int   lastWidth       = 0;
    private  bool  formIsMaximized = false;

    private  int   thumbnailGridViewLastHeight = 0;
    private  int   thumbnailGridViewLastWidth  = 0;

    private  int   navigationPanelLastHeight = 0;
    private  int   navigationPanelLastWidth  = 0;

    private  int   thumbNailHeight = 180;
    private  int   thumbNailWidth  = 150;

    // For use by 'AddToThumbNailPanel'
    private  int   thumbnailCurRow = 0;
    private  int   thumbnailCurCol = 0;

    String  infinityStr = ((char)8734).ToString ();


    //*  Select Criteria Variables
    private  PicesDataBaseImageGroup  selecetedImageGroup = null;
    private  String                   cruiseName          = "";
    private  String                   stationName         = "";
    private  String                   deploymentNum       = "";
    private  String                   sipperFileName      = "";
    private  PicesClass               selectedClass       = null;
    private  PicesClass               thumbnailClass      = null;
    private  float                    depthMin            = 0;
    private  float                    depthMax            = 0;
    private  float                    probMin             = 0.0f;
    private  float                    probMax             = 1.0f;
    private  int                      sizeMin             = 0;
    private  int                      sizeMax             = 0;
    private  float                    depthIncrement      = 10.0f;

    private  String                   lastSaveImageDir = "";
    

    // Variables that specify the criteria last used to load "SipperFile.Items".  This way when the
    // user selects the dropdwon box we can reload the "SipperFile.Items" only if ther have been 
    // changes to the crieria fields.
    private  String                   SipperFileCruiseName    = "";
    private  String                   SipperFileStationName   = "";
    private  String                   SipperFileDeploymentNum = "";
   

    private  PicesDataBaseImageList.SortOrderType   sortOrder = PicesDataBaseImageList.SortOrderType.soSpatial;
    private  bool                                   reverseSort = false;

    
    //*    Variables needed for DataBase acess
    private  PicesDataBase  dbConn = null;

    private  PicesRunLog    runLog = PicesRunLog.CreateRunLog ("PicesCommander");

    private  bool  allowUpdates = false;


    //*   Variables for ThumbnailPanel Display
    private  PlanktonImageLoader  imageLoader = null;  // Used to load Images frm database.  Will be polled 
                                                       // by "ThumbNailPanelTimer"  to update ThmbNailPanel
                                                       // Will get created when user double clicks on a 
                                                       // class.  If one already exists then will cancel that
                                                       // instance and create a new one for the nex class selected.

    private  PicesDataBaseImageList  curSelImages = null;

    List<PicesClass>  lastClassesValidated = new List<PicesClass> ();

    PicesClassList  allClasses    = null;
    PicesClassList  activeClasses = null;  /**< List of classes in Currently selected training model */
    String          rootDir       = "";

    //  Navigation panel Related variables.
    private  enum  NavigationOptions  {ByDepth, ByClass};

    NavigationOptions                navigateBy                  = NavigationOptions.ByClass;
    PlanktonClassStatisticsLoader    navigationClassStatsLoader  = null;
    PicesClassStatisticList          classStats                  = null;   // The last class stats loaded.

    PlanktonDepthStatisticsLoader    navigationDepthStatsLoader  = null;
    uint[]                           depthStats                  = null;
    PicesVolumeSampledStatList       volumeStats                 = null;

    private  char                    classKeyToUse = 'P';     // 'P' = Use Predicted Class,  'V' - Use Validated Class



    public PicesCommander()
    {
      configFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "PicesCommander.cfg";

      InitializeComponent();

      allowUpdates = PicesSipperVariables.AllowUpdates ();

      // Initializing variables to deel with resizing.
      lastHeight = Height;
      lastWidth  = Width;

      thumbnailGridViewLastHeight = ThumbnailDataGridView.Height;
      thumbnailGridViewLastWidth  = ThumbnailDataGridView.Width;

      navigationPanelLastHeight = NavigationPanel.Height;
      navigationPanelLastWidth  = NavigationPanel.Width;

      PopulateTrainingLibraryFields ();
      
      PicesDataBaseImageList.PopulateComboBoxWithSortOrderOptions (SortOrder);
      sortOrder = PicesDataBaseImageList.SortOrderType.soSpatial;
      SortOrder.SelectedItem = PicesDataBaseImageList.SortOrderTypeToStr (sortOrder);
      UpdateThumbNailDisplayOptions ();
    }


    private  void  UpdateActiveClassesList ()
    {
      activeClasses = null;
      TrainingModel2  tm = ActiveTrainingLibraries.Model1 ();
      if  (tm != null)
      {
        activeClasses = tm.MLClasses ();
        rootDir = tm.RootDir;
        if  (rootDir == null)
        {
          String  modelName = tm.ModelName;
          rootDir = OSservices.AddSlash (PicesSipperVariables.PicesHomeDir ()) + OSservices.GetRootName (modelName);
        }
      }
    }


    private  void  PopulateTrainingLibraryFields ()
    {
      String[] trainingModels = TrainingModel2.GetListOfTrainingModels();
      if  (trainingModels != null)
      {
        foreach (String modelName in trainingModels)
        {
          TrainingLibrary1.Items.Add(modelName);
          TrainingLibrary2.Items.Add(modelName);
        }
      }

      if (!String.IsNullOrEmpty(ActiveTrainingLibraries.Model1Name))
      {
        TrainingLibrary1.Text = ActiveTrainingLibraries.Model1Name;
        LoadTrainingModel1 (ActiveTrainingLibraries.Model1Name, false);
      }

      if (!String.IsNullOrEmpty(ActiveTrainingLibraries.Model2Name))
      {
        TrainingLibrary2.Text = ActiveTrainingLibraries.Model2Name;
        LoadTrainingModel2 (ActiveTrainingLibraries.Model2Name, false);
      }
    }



    private void maintenaceToolStripMenuItem_Click(object sender, EventArgs e)
    {

    }




    private  void  UpdateNavigationList (PicesClassStatisticList  stats)
    //  This function isused to refresh  the NavigationList with more up-to-date stats 
    //  on classes for current selection of critera.  
    //  'NavigaionList' and 'stats'  are assumed to have there contens in alphabetical order.
    {
      if  (stats == null)
        return;

      NavigationList.BeginUpdate ();

      if  (activeClasses == null)
        UpdateActiveClassesList ();
      
      stats.SortByClassName ();

      int  npIDX    = 0;
      int  statsIDX = 0;

      while  (statsIDX < stats.Count)
      {
        String  npClassName = "ZZZZZZZZZZZZZZZZZZZZZZ";
        if  (npIDX < NavigationList.Items.Count)
          npClassName = NavigationList.Items[npIDX].Text;

        String  statsClassName = stats[statsIDX].ClassName;

        if  (statsClassName.CompareTo (npClassName) < 0)
        {
          NavigationList.Items.Insert (npIDX, statsClassName);
          NavigationList.Items[npIDX].SubItems.Add (stats[statsIDX].Count.ToString ("#,###,##0"));
          if  (activeClasses != null)
          {
            if  (activeClasses.LookUpIndex (stats[statsIDX].MLClass) >= 0)
              NavigationList.Items[npIDX].ForeColor = Color.DarkGreen;
            else
              NavigationList.Items[npIDX].ForeColor = Color.Gray;
          }

          npIDX++;
          statsIDX++;
        }

        else if  (statsClassName.CompareTo (npClassName) > 0)
        {
          npIDX++;
        }

        else if  (npClassName.CompareTo (statsClassName) == 0)
        {
          NavigationList.Items[npIDX].SubItems[1].Text = stats[statsIDX].Count.ToString ("#,###,##0");
          npIDX++;
          statsIDX++;
        }
      }

      NavigationList.EndUpdate ();

      PopulateClassDistributionChart (stats);
    }  /* UpdateNavigationList */



    private  void  PopulateClassDistributionChart (PicesClassStatisticList  stats)
    {
      DistributionChart.Titles.Clear ();
      DistributionChart.Series.Clear ();
      if  ((stats == null)  ||  (stats.Count < 1))
        return;

      PicesClassStatisticList  hierarchyStats = stats.RemoveNoiseClasses ().Summarize ((int)Level.Value);

      float countTotal = (float)hierarchyStats.CountTotal;

      DistributionChart.Titles.Clear ();
      DistributionChart.Titles.Add ("Class Distribution");

      bool labelForEveryXPoint = true;

      Series  s = new Series ("Class", hierarchyStats.Count);

      if (PlotType.SelectedOption == "Bar")
      {
        s.ChartType = SeriesChartType.Bar;
        labelForEveryXPoint = true;
      }

      else if (PlotType.SelectedOption == "Column")
      {
        s.ChartType = SeriesChartType.Column;
        s.CustomProperties = "LabelStyle=Top";
        labelForEveryXPoint = true;
      }

      else
      {
        s.ChartType = SeriesChartType.Pie;
        s.CustomProperties = "PieLabelStyle=Outside,PieLineColor=Black";
        labelForEveryXPoint = false;
      }

      s.MarkerStep = 1;

      foreach  (PicesClassStatistic classStat in hierarchyStats)
      {
        DataPoint  dp = new DataPoint (0.0, (double)classStat.Count);
        //dp.XValue = 0.0;
        //dp.YValues = new double[1];
        //dp.YValues[0] = (double)classStat.Count;

        float perc = 100.0f * (float)classStat.Count / countTotal;
        String cn = classStat.ClassName.Replace ('_', '\n');
        dp.LegendText = cn;
        if  (labelForEveryXPoint  ||(perc > 1.5f))
        {
          dp.AxisLabel = cn;
          dp.LabelFormat = "{0}";
        }
        else
        {
        }
        s.Points.Add (dp);
      }
      
      DistributionChart.Series.Add (s);
    }  /* PopulateClassDistributionChart */



    private  void  PopulateNavigationListWithDepthStats (uint[]  stats)
    {
      NavigationList.Items.Clear ();
      if  (stats == null)
        return;
      
      int  numItems = 0;

      for  (int x = 0;  x < stats.Length;  x++)
      {
        if  (stats[x] == 0)
          continue;

        float  depthMin = (float)x * depthIncrement;
        float  depthMax = depthMin + depthIncrement;

        String  desc = depthMin + "-" + depthMax;

        NavigationList.Items.Add (desc);
        NavigationList.Items[numItems].SubItems.Add (stats[x].ToString ("#,###,##0"));
        numItems++;
      }

      PopulateDepthDistributionChart (stats);
    }  /* PopulateNavigationListWithDepthStats */



    private  void  PopulateDepthDistributionChart (uint[]  stats)
    {
      DistributionChart.Titles.Clear ();
      DistributionChart.Titles.Add ("Depth Distribution");
      DistributionChart.Series.Clear ();

      float[]  sliceValues = new float[stats.Length];
      if  (WeightByVolume.Checked  &&  (volumeStats != null))
      {
        for (int idx = 0;  idx < stats.Length;  ++idx)
        {
          PicesVolumeSampledStat  stat = volumeStats.Locate (idx);
          if  ((stat != null)  ||  (stat.VolumeSampled != 0.0f))
            sliceValues[idx] = ((float)stats[idx] / stat.VolumeSampled);
          else
            sliceValues[idx] = 0.0f;
        }
      }
      else
      {
        for (int idx = 0;  idx < stats.Length;  ++idx)
          sliceValues[idx] = (float)stats[idx];
      }

      Series  s = new Series ("Depth", 1);

      if  (PlotType.SelectedOption == "Bar")
        s.ChartType = SeriesChartType.Bar;

      else if  (PlotType.SelectedOption == "Column")
        s.ChartType = SeriesChartType.Column;

      else
      {
        s.ChartType = SeriesChartType.Pie;
        s.CustomProperties = "PieLabelStyle=Outside,PieLineColor=Black";
      }

      for  (int x = 0;  x < sliceValues.Length;  x++)
      {
        if  (stats[x] == 0)
          continue;

        float  depthMin = (float)x * depthIncrement;
        float  depthMax = depthMin + depthIncrement;
        String  desc = depthMin + "-" + depthMax;

        DataPoint  dp = new DataPoint ((double)x, (double)sliceValues[x]);
        dp.AxisLabel   = desc;
        dp.LabelFormat = "{0}";
        dp.LegendText  = desc;
        //dp.IsValueShownAsLabel = true;
        s.Points.Add (dp);
      }
      
      DistributionChart.Series.Add (s);
     }  /* PopulateDepthDistributionChart*/



    
    private  void  NavigationPanelButtonsDisable ()
    {
      RefreshButton.Enabled    = false;
      NavigationButton.Enabled = false;
    }


    private  void  NavigationPanelButtonsEnable ()
    {
      RefreshButton.Enabled    = true;
      NavigationButton.Enabled = true;
    }


    private  void  RefreshNavigationPanel ()
    {
      if  (navigationClassStatsLoader != null)
      {
        navigationClassStatsLoader.CancelLoadWait (2000);
        navigationClassStatsLoader = null;
      }

      if  (navigationDepthStatsLoader != null)
      {
        navigationDepthStatsLoader.CancelLoadWait (2000);
        navigationDepthStatsLoader = null;
      }
      
      if  (navigateBy == NavigationOptions.ByClass)
      {
        navigationClassStatsLoader = new PlanktonClassStatisticsLoader 
                (selecetedImageGroup, Cruise.Text, Station.Text, Deployment.Text, 
                 sipperFileName, 
                 selectedClass, classKeyToUse,
                 probMin,   probMax, 
                 sizeMin,   sizeMax, 
                 depthMin,  depthMax
                );
        navigationPanelTimer.Enabled = true;
        NavigationList.Items.Clear ();
        //PopulateNavigationList (stats);
      }
      else
      {
        navigationDepthStatsLoader = new PlanktonDepthStatisticsLoader 
                (selecetedImageGroup, 
                 Cruise.Text, Station.Text, Deployment.Text, sipperFileName, 
                 depthIncrement,
                 selectedClass,  classKeyToUse,
                 probMin,   probMax, 
                 sizeMin,   sizeMax
                );

        navigationPanelTimer.Enabled = true;
        NavigationList.Items.Clear ();
      }

      NavigationButton.Text = (navigateBy == NavigationOptions.ByClass) ? "by Depth" : "by Class";
    }  /* RefreshNavigationPanel */



    private  void  UpdateThumbNailDisplayOptions ()
    {
      if  (navigateBy == NavigationOptions.ByDepth)
        ThumbNailImageCell.DisplayClass1 (true);
      else
        ThumbNailImageCell.DisplayClass1 (false);

      if  (sortOrder == PicesDataBaseImageList.SortOrderType.soClass2)
        ThumbNailImageCell.DisplayClass2 (true);
      else
        ThumbNailImageCell.DisplayClass2 (false);
    }  /* UpdateThumbNailDisplayOptions */



    public bool GetThumbnailImageAbort()
    {
      return true;
    }  /* GetThumbnailImageAbort */


    private  System.IntPtr  thumbNailCallBackData = IntPtr.Zero;


    private void  AddToThumbNailPanel (PicesDataBaseImageList  images)
    {
      if  (images == null)
        return;

      int  numCols = ThumbnailDataGridView.Columns.Count;

      thumbnailCurRow = ThumbnailDataGridView.Rows.Count - 1;
      if  (ThumbnailDataGridView.Rows.Count == 0)
        thumbnailCurCol = numCols;

      int  idx = 0;
      while  (idx < images.Count)
      {
        PicesDataBaseImage  i = images[idx];
         
        if  (thumbnailCurCol >= numCols)
        {
          object[] row = new object[numCols];
          ThumbnailDataGridView.Rows.Add (row);
          thumbnailCurRow = ThumbnailDataGridView.Rows.Count - 1;
          thumbnailCurCol = 0;
          ThumbnailDataGridView.Rows[thumbnailCurRow].Height = thumbNailHeight + 2;
        }
      
        ThumbnailDataGridView.Rows[thumbnailCurRow].Cells[thumbnailCurCol].Value = i;
        thumbnailCurCol++;
        idx++;
      }
    }  /* AddToThumbNailPanel */




    private  void  LoadStations ()
    {
      Station.Items.Clear ();
      PicesSipperStationList  stations = null;
      Station.Items.Add ("");
      if  (!String.IsNullOrEmpty (cruiseName))
      {
        stations = dbConn.SipperStationLoad (cruiseName);
        if  (stations != null)
        {
          foreach  (PicesSipperStation  station  in  stations)
            Station.Items.Add (station.StationName);
        }
      }

      if  (!String.IsNullOrEmpty (stationName))
      {
        if  (!Station.Items.Contains (stationName))
        {
          stationName     = "";
          deploymentNum   = "";
          sipperFileName  = "";
          Station.Text    = "";
          Deployment.Text = "";
          SipperFile.Text = "";
        }
      }
    }  /* LoadStations */



    private  void  LoadDeployments ()
    {
      Deployment.Items.Clear ();
      PicesSipperDeploymentList  deployments = null;

      Deployment.Items.Add ("");

      if  ((!String.IsNullOrEmpty (cruiseName))  &&  (!String.IsNullOrEmpty (stationName)))
      {
        deployments = dbConn.SipperDeploymentLoad (cruiseName, stationName);
        if  (deployments != null)
        {
          foreach  (PicesSipperDeployment  d in deployments)
            Deployment.Items.Add (d.DeploymentNum);
        }
      }

      if  (!String.IsNullOrEmpty (deploymentNum))
      {
        if  (!Deployment.Items.Contains (deploymentNum))
        {
          deploymentNum = "";
          Deployment.Text = "";
        }
      }
    }  /* LoadDeployments */



    private  void  LoadSipperFiles ()
    {
      if  ((SipperFileCruiseName.Equals  (cruiseName))     &&  
           (SipperFileStationName.Equals (stationName))    &&
           (SipperFileDeploymentNum.Equals (deploymentNum))
          )
      {
        // Nothing has changed since the last time we loaded "SipperFile.Items" so no point reloading again.
        return;
      }

      SipperFileCruiseName    = cruiseName;
      SipperFileStationName   = stationName;
      SipperFileDeploymentNum = deploymentNum;

      //SipperFile.Items.Clear ();
      PicesSipperFileList  sipperFiles = dbConn.SipperFileLoad (cruiseName, stationName, deploymentNum);
      SipperFile.DataSource = sipperFiles;
      SipperFile.DisplayMember = "SipperFileName";
      //SipperFile.ValueMember = 

      /*



      SipperFile.Items.Add ("");
      if  (sipperFiles != null)
      {
        foreach  (PicesSipperFile sf in sipperFiles)
          SipperFile.Items.Add (sf.SipperFileName);
      }
      if  (!String.IsNullOrEmpty (sipperFileName))
      {
        if  (!SipperFile.Items.Contains (sipperFileName))
        {
          sipperFileName = "";
          SipperFile.Text = "";
        }
      }
       */
    }  /* LoadSipperFiles */


    private  void  LoadDataBaseVariables ()
    {
      if  (dbConn == null)
        dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      if  ((dbConn == null)  ||  (!dbConn.Valid ()))
        dbConn = PicesDataBase.SelectNewDataBaseServer (runLog);

      if  ((dbConn != null)  &&  (!dbConn.Valid ()))
      {
        dbConn.Close ();
        dbConn = null;
        GC.Collect ();
      }

      if  (dbConn == null)
        Close ();

      Cruise.Items.Clear ();
      PlanktonClass.Items.Clear ();
      allClasses = null;
      ImageGroup.Items.Clear ();
      Station.Items.Clear ();

      if  ((dbConn == null)  ||  (!dbConn.Valid ()))
      {
        this.Text = "PicesCommander          *** Not Connected ***";
        allClasses = null;
      }
      else
      {
        this.Text = "PicesCommander          " + dbConn.ServerDescription ();
         // Load Cruise Info
        Cruise.Items.Add ("");
        PicesSipperCruiseList  cruises = dbConn.SipperCruiseLoadAllCruises ();
        if  (cruises != null)
        {
          foreach  (PicesSipperCruise  cruise  in  cruises)
            Cruise.Items.Add (cruise.CruiseName);
        }
        
        // Load Image Class info
        allClasses = dbConn.MLClassLoadList ();
        if  (allClasses == null)
        {
					MessageBox.Show (this, "Failed to load existing class list.  This will cause problems in program execution.");
					allClasses = new PicesClassList ();
        }

        lastClassesValidated = new List<PicesClass> ();
        if  (allClasses != null)
        {
          foreach  (PicesClass  c in allClasses)
          {
            PlanktonClass.Items.Add (c.Name);
            if  (c.Name != "AllClasses")
              lastClassesValidated.Add (c);
          }
        }

        LoadImageGroupChoices ();
      }
    }  /* LoadDataBaseVariables */




    private  void  LoadImageGroupChoices ()
    {
      ImageGroup.Items.Clear ();
      PicesDataBaseImageGroupList  groups = dbConn.ImageGroupLoad ();
      ImageGroup.Items.Add ("");
      if  (groups != null)
      {
        foreach  (PicesDataBaseImageGroup g in groups)
          ImageGroup.Items.Add (g.Name + "   (" + g.Count + ")");
      }
    }



    private  void  ImageGroupSet (String  newGroupName)
    {
      String  temp = newGroupName.Trim ();
      if  (String.IsNullOrEmpty (temp))
      {
        selecetedImageGroup = null;
        ImageGroup.Text = "";
        return;
      }

      int  x = temp.LastIndexOf ('(');
      if  (x >= 0)
        temp = temp.Substring (0, x).Trim ();

      if  ((selecetedImageGroup != null)  &&  (temp == selecetedImageGroup.Name))
      {
        // Nothing has actually changed;  nothing to do.
        return;
      }

      selecetedImageGroup = dbConn.ImageGroupLoad (temp);
      if  (selecetedImageGroup == null)
      {
        ImageGroup.Text = "";
        return;
      }

      ImageGroup.Text = selecetedImageGroup.Name;
        
      cruiseName      = "";
      stationName     = "";
      deploymentNum   = "";
      sipperFileName  = "";
      selectedClass   = null;
      depthMin        = 0;
      depthMax        = 0;
      probMin         = 0.0f;
      probMax         = 1.0f;
      sizeMin         = 0;
      sizeMax         = 0;

      LoadCriteriaFields ();
    }   /* ImageGroupSet */



    private  void  ImageGroupSelected ()
    {
      ImageGroupSet  (ImageGroup.SelectedItem.ToString ());
    }  /* ImageGroupSelected */



    private  void  ImageGroupUpdated ()
    {
      ImageGroupSet  (ImageGroup.Text);
    }  /* ImageGroupUpdated */



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
        if  (fields.Length != 2)
          continue;

        String  fieldName  = fields[0];
        String  fieldValue = fields[1];

        switch  (fieldName)
        { 
          case  "CruiseName":         cruiseName          = fieldValue;                         break;
          case  "StationName":        stationName         = fieldValue;                         break;
          case  "DeploymentNum":      deploymentNum       = fieldValue;                         break;
          case  "SipperFileName":     sipperFileName      = fieldValue;                         break;
          case  "LastWidth":          savedWidth          = PicesKKStr.StrToInt (fieldValue);   break;
          case  "LastHeight":         savedHeight         = PicesKKStr.StrToInt (fieldValue);   break;
          case  "LastSaveImageDir":   lastSaveImageDir    = fieldValue;                         break;
          case  "SelectedImageGroup": selecetedImageGroup = dbConn.ImageGroupLoad (fieldValue); break;
          case  "Maximized":          screenWasMaximized  = (fieldValue.ToUpper () == "YES");   break;

          case  "SizeMin":            sizeMin             = PicesKKStr.StrToInt (fieldValue);   break;
          case  "SizeMax":            sizeMax             = PicesKKStr.StrToInt (fieldValue);   break;
          case  "DepthMin":           depthMin            = PicesKKStr.StrToInt (fieldValue);   break;
          case  "DepthMax":           depthMax            = PicesKKStr.StrToInt (fieldValue);   break;
          case  "ProbMin":            probMin             = PicesKKStr.StrToFloat (fieldValue); break;
          case  "ProbMax":            probMax             = PicesKKStr.StrToFloat (fieldValue); break;

          case  "ClassKeyToUse":      {
                                        fieldValue = fieldValue.Trim ();
                                        if  (fieldValue.Length > 0)
                                          classKeyToUse = fieldValue[0];
                                        if  ((classKeyToUse != 'P')  &&  (classKeyToUse != 'V'))
                                          classKeyToUse = 'P';
                                        break;
                                      }
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



    private  void  SaveConfiguration ()
    {
      System.IO.StreamWriter  o = null;
      try{o = new System.IO.StreamWriter (configFileName);}  catch  (Exception){o = null; return;}
      
      o.WriteLine ("//PicesCommander Configuration File");
      o.WriteLine ("//");
      o.WriteLine ("//DateTime Saved" + "\t" + DateTime.Now.ToString ("F"));
      o.WriteLine ("//");
      o.WriteLine ("CruiseName"         + "\t" + cruiseName);
      o.WriteLine ("StationName"        + "\t" + stationName);
      o.WriteLine ("DeploymentNum"      + "\t" + deploymentNum);
      o.WriteLine ("SipperFileName"     + "\t" + sipperFileName);
      o.WriteLine ("LastWidth"          + "\t" + lastWidth);
      o.WriteLine ("LastHeight"         + "\t" + lastHeight);
      o.WriteLine ("Maximized"          + "\t" + (formIsMaximized ? "YES":"NO"));
      o.WriteLine ("LastSaveImageDir"   + "\t" + lastSaveImageDir);
      o.WriteLine ("ClassKeyToUse"      + "\t" + classKeyToUse);
      
      o.WriteLine ("SizeMin"            + "\t" + sizeMin);
      o.WriteLine ("SizeMax"            + "\t" + sizeMax);
      o.WriteLine ("DepthMin"           + "\t" + depthMin);
      o.WriteLine ("DepthMax"           + "\t" + depthMax);
      o.WriteLine ("ProbMin"            + "\t" + probMin);
      o.WriteLine ("ProbMax"            + "\t" + probMax);

      if  (selecetedImageGroup != null)
        o.WriteLine ("SelectedImageGroup" + "\t" + selecetedImageGroup.Name);

      o.Close ();
      o = null;
    }


    private  void  UpdateClassKeyToUseButtons ()
    {
      if  (classKeyToUse == 'V')
      {
        ClassKeyTypeValidatedButton.Checked = true;
        ClassKeyTypePredictedButton.Checked = false;
      }
      else
      {
        ClassKeyTypeValidatedButton.Checked = false;
        ClassKeyTypePredictedButton.Checked = true;
      }
    }



    private void   PicesCommander_Load (object sender, EventArgs e)
    {
      LoadDataBaseVariables ();
      LoadConfigurationFile ();
      DefineThumbnailGridViewRowAndColumns ();
      UpdateClassKeyToUseButtons ();

      if  (!String.IsNullOrEmpty (cruiseName))
        LoadStations ();

      if  (!String.IsNullOrEmpty (stationName))
        LoadDeployments ();

      LoadCriteriaFields ();
    }  /* PicesCommander_Load*/




    private  void  LoadCriteriaFields ()
    {
      Cruise.Text      = cruiseName;
      Station.Text     = stationName;
      Deployment.Text  = deploymentNum;
      SipperFile.Text  = sipperFileName;

      if (depthMin <= 0.0f)    depthMin = 0.0f;
      if ((depthMax < depthMin)  &&  (depthMax > 0.0f)) depthMax = depthMin;
      DepthMin.Text = depthMin.ToString ("#,##0.00");

      if  (depthMax <= 0.0f)  
      {
        depthMax = 0.0f; 
        DepthMax.Text = infinityStr;
      }  
      else  
      {
        DepthMax.Text = depthMax.ToString ("#,###0.00");
      } 

      if  (probMax < probMin)  probMax = 0;
      probMin = Math.Min (1.0f, Math.Max (0.0f, probMin));
      probMax = Math.Min (1.0f, Math.Max (0.0f, probMax));
      ProbabilityMin.Text = probMin.ToString ("p1");
      ProbabilityMax.Text = probMax.ToString ("p1");

      if  (sizeMax < sizeMin)  sizeMax = 0;
      sizeMin = Math.Max (0, sizeMin);
      SizeMin2.Text = sizeMin.ToString ("##,###,##0");

      sizeMax = Math.Max (0, sizeMax);
      if  (sizeMax <= 0)  
      {
        SizeMax2.Text = infinityStr;
        SizeMax2.Font = new Font (SizeMax2.Font.FontFamily, 12); 
        sizeMax = 0;
      }  
      else  
      {
        SizeMax2.Text = sizeMax.ToString ("##,###,##0");
        SizeMax2.Font = new Font (SizeMax2.Font.FontFamily, 10); 
      }

      if  (selectedClass == null)
        PlanktonClass.Text = "";
      else
        PlanktonClass.Text = selectedClass.Name;

      if  (selecetedImageGroup == null)
        ImageGroup.Text = "";
      else
        ImageGroup.Text = selecetedImageGroup.Name;

      DepthIncrement.Text = depthIncrement.ToString ("##0.0");
    }  /* LoadCriteriaFields */





    private  void  LoadTrainingModel1 (String  modelName,
                                       bool    forceRebuild
                                      )
    {
      TrainingLibrary1.Enabled = false;
      //TrainingLibrary2.Enabled = false;
      RebuildTrainLibrary1Button.Enabled = false;
      //RebuildTrainLibrary2Button.Enabled = false;
      ActiveTrainingLibraries.SetModel1 (modelName, forceRebuild, Model1AlertCompletion);
    }  /* LoadTrainingModel1 */



    private  void  Model1AlertCompletion (TrainingModel2 sender)
    {
      if ((sender == null) || (!sender.Valid))
        TrainingLibrary1.Text = "";

      TrainingLibrary1.Enabled = true;
      //TrainingLibrary2.Enabled = true;
      RebuildTrainLibrary1Button.Enabled = true;
      //RebuildTrainLibrary2Button.Enabled = true;
    }  /* Model1AlertCompletion*/



    private  void  LoadTrainingModel2 (String  modelName,
                                       bool    forceRebuild
                                      )
    {
      //TrainingLibrary1.Enabled = false;
      TrainingLibrary2.Enabled = false;
      //RebuildTrainLibrary1Button.Enabled = false;
      RebuildTrainLibrary2Button.Enabled = false;
      ActiveTrainingLibraries.SetModel2 (modelName, forceRebuild, Model2AlertCompletion);
    }  /* LoadTrainingModel2 */



    private  void  Model2AlertCompletion (TrainingModel2 sender)
    {
      if ((sender == null) || (!sender.Valid))
        TrainingLibrary2.Text = "";

      //TrainingLibrary1.Enabled = true;
      TrainingLibrary2.Enabled = true;
      //RebuildTrainLibrary1Button.Enabled = true;
      RebuildTrainLibrary2Button.Enabled = true;
    }  /* Model2AlertCompletion*/



    private void NavigationPanel_Resize(object sender, EventArgs e)
    {
      if  (navigationPanelLastHeight == 0)
      {
        // If 'navigationPanelLastHeight' == 0  indicates that we have not set up the initial screen yet and are not ready to resize.
        return;
      }

      int  deltaHeight = NavigationPanel.Height - navigationPanelLastHeight;
      int  deltaWidth  = NavigationPanel.Width  - navigationPanelLastWidth;

      navigationPanelLastHeight = NavigationPanel.Height;
      navigationPanelLastWidth  = NavigationPanel.Width;

      NavigationList.Height += deltaHeight;
      NavigationList.Width  += deltaWidth;

      RefreshButton.Top += deltaHeight;
      NavigationPanaelStatus.Top += deltaHeight;

      DepthIncrement.Top += deltaHeight;
      
      NavigationButton.Top  += deltaHeight;
      NavigationButton.Left += deltaWidth;
    }



    private  void  DefineThumbnailGridViewRowAndColumns ()
    {
      int  width  = ThumbnailDataGridView.Width;
      int  height = ThumbnailDataGridView.Height;

      int  deltaHeight = height - thumbnailGridViewLastHeight;
      int  deltaWidth  = width  - thumbnailGridViewLastWidth;

      thumbnailGridViewLastHeight = ThumbnailDataGridView.Height;
      thumbnailGridViewLastWidth  = ThumbnailDataGridView.Width;

      int  requiredHeight = thumbNailHeight;
      int  requiredWidth  = thumbNailWidth;

      int  numCols = (int)Math.Floor ((float)(width - 15)  / (float)requiredWidth);
      if  (numCols < 1)  
        numCols = 1;

      int  numRows = (int)Math.Floor ((float)height / (float)requiredHeight);
      if  (numRows < 1)
        numRows = 1;

      ThumbnailDataGridView.Rows.Clear ();
      ThumbnailDataGridView.Columns.Clear ();
      //ThumbnailDataGridView.ColumnCount = numCols;

      for  (int colIDX = 0;  colIDX < numCols;  colIDX++)
      {
        //DataGridViewImageColumn  dgvic = new DataGridViewImageColumn ();
        ThumbNailImageColumn  dgvic = new ThumbNailImageColumn (this);
        
        dgvic.Name = "ThumbCol" + colIDX;
        dgvic.HeaderText = "";
        dgvic.Width = thumbNailWidth;
        ThumbnailDataGridView.Columns.Add (dgvic);
      }

      {
        // Define Row Info
        DataGridViewCellStyle  dgvcs = new DataGridViewCellStyle ();
        dgvcs.Alignment = DataGridViewContentAlignment.MiddleCenter;
        ThumbnailDataGridView.RowsDefaultCellStyle = dgvcs;

        try  {ThumbnailDataGridView.RowCount = 0;}   
        catch  (Exception e)  
        {
          String  s = e.ToString ();
          runLog.Writeln ("DefineThumbnailGridViewRowAndColumns  \n" + s);
        }
      }

      BuildThumbnailGridViewConetextMenu ();
      AddToThumbNailPanel (curSelImages);
    }  /* DefineThumbnailGridViewRowAndColumns */



    public  void  UpdateLastClassesValidated (PicesClass  updatedClass)
    {
      int  idx  = 0;

      PicesClass next2Add = updatedClass;

      idx = 0;
      while  (true)
      {
        if  (idx >= lastClassesValidated.Count)
        {
          lastClassesValidated.Add (next2Add);
          break;
        }

        PicesClass temp = lastClassesValidated[idx];
        lastClassesValidated[idx] = next2Add;
        if  (temp == updatedClass)
          break;
        next2Add = temp;
        idx++;
      }
    }   /* UpdateLastClassesValidated */



    private  PicesDataBaseImageList  GetSelectedImages ()
    {
      PicesDataBaseImageList  selectedImages = new PicesDataBaseImageList ();
      foreach  (DataGridViewCell  dgvc in  ThumbnailDataGridView.SelectedCells)
      {
        int  colIndex = dgvc.ColumnIndex;
        int  rowIndex = dgvc.RowIndex;

        PicesDataBaseImage  i = (PicesDataBaseImage)ThumbnailDataGridView.Rows[rowIndex].Cells[colIndex].Value;
        if  (i != null)
          selectedImages.Add (i);
      }

      return  selectedImages;         
    }  /* GetSelectedImages */



    private  PicesDataBaseImageList  GetSelectedValidatedImages ()
    {
      PicesDataBaseImageList  selectedImages = GetSelectedImages ();
      PicesDataBaseImageList  selectedValidatedImages = new PicesDataBaseImageList ();
      if  (selectedImages == null)
        return selectedValidatedImages;

      foreach  (PicesDataBaseImage  dbi in selectedImages)
      {
        if  (dbi.ValidatedClass != null)
          selectedValidatedImages.Add (dbi);
      }
      return  selectedValidatedImages;         
    }  /* GetSelectedValidatedImages */




    private  void  AddImagesToTrainingLibrary (PicesDataBaseImageList  images)
    {
      TrainingModel2  tm1 = ActiveTrainingLibraries.Model1 ();
      if  (tm1 == null)
         tm1 = ActiveTrainingLibraries.Model2 ();

      if  (tm1 == null)
      {
        MessageBox.Show (this, "There is no Active Training Library specified.");
        return;
      }

      foreach  (PicesDataBaseImage i in images)
      {
        if  (i == null)
          continue;

        PicesRaster  origImage = dbConn.ImageFullSizeFind (i.ImageFileName);
        if  (origImage == null)
        {
          MessageBox.Show (this, "Was unable to get original image for [" + i.ImageFileName + "]" );
          continue;
        }

        try 
        {
          tm1.AddImageToTrainingLibray (i.ImageFileName, origImage, i.ValidatedClass, true);
        }
        catch (Exception e)
        {
          MessageBox.Show (this, e.ToString (), "Add Images to Training Library");
        }
      }
    }  /* AddImagesToTrainingLibrary */



   
    
    private  void  AddToTrainingLibrary (Object sender, EventArgs e)
    {
      if  (ActiveTrainingLibraries.Model1 () == null)
      {
        MessageBox.Show (this, "There is no currently selected training library.", "AddToTrainingLibrary", MessageBoxButtons.OK);
        return;
      }

      PicesDataBaseImageList  images = GetSelectedImages ();
    
      if  (images.Count < 1)
      {
        MessageBox.Show (this, "There are no selected images.");
        return;
      }

      {
        DialogResult dr = MessageBox.Show (this, "Add the selected images to Training Library[" + ActiveTrainingLibraries.Model1Name + "]", "AddToTrainingLibrary", MessageBoxButtons.YesNo);
        if  (dr == DialogResult.Yes)
          AddImagesToTrainingLibrary (images);
      }
    }  /* AddToTrainingLibrary */



    private  void  SaveImage (Object sender, EventArgs e)
    {
      PicesDataBaseImageList  images = GetSelectedImages ();
      if  (images.Count < 1)
      {
        MessageBox.Show (this, "There are no selected images.");
        return;
      }
      
      FolderBrowserDialog  fbd = new FolderBrowserDialog ();

      fbd.Description         = "Select directory to save images to";
      fbd.ShowNewFolderButton = true;
      fbd.SelectedPath        = lastSaveImageDir;
      DialogResult  dr = fbd.ShowDialog ();
      if  (dr == DialogResult.OK)
      {
        if  (lastSaveImageDir != fbd.SelectedPath)
        {
          lastSaveImageDir = fbd.SelectedPath;
          SaveConfiguration ();
        }
      
        String  destDir = OSservices.AddSlash (fbd.SelectedPath);
        
        System.IO.DirectoryInfo di = new System.IO.DirectoryInfo (destDir);
        if  (!di.Exists)
        {
          MessageBox.Show (this, "No Such Directory [" + destDir + "]", "Save Images", MessageBoxButtons.OK);
          return;
        }
        
        String  errors = "";        
        foreach  (PicesDataBaseImage  i in images)
        {
          PicesRaster raster = dbConn.ImageFullSizeFind (i.ImageFileName);
          if  (raster == null)
            errors += i.ImageFileName + " Error getting original image from sipper file." + "\n";
          else
          {
            String  fullFileName = destDir + i.ImageFileName + ".bmp";

            // kak 2010-05-08  Changed the save code to use the Save method in the Raster class which will
            // use the BaseLibrary::ImageIO Save routine.  Images eing saved by .net were not cmpatable
            // with the KKU::BmpImage class which the Feature Extraction routines will want to use.
            try { raster.Save(fullFileName); }
            catch (Exception)
            {
              errors += "Exception saving image[" + i.ImageFileName + "]";
            }
          }
        }
        
        if  (!String.IsNullOrEmpty (errors))
          MessageBox.Show (this, "Following erros occured:" + "\n\n" + errors, "Save Images", MessageBoxButtons.OK);
      }
    }  /* SaveImage */



    
    private  void  CopyImageToClipboard (Object sender, EventArgs e)
    {
      PicesDataBaseImageList  images = GetSelectedImages ();
      if  (images == null)  return;

      if  (images.Count != 1)
      {
        MessageBox.Show (this, "Only one image can be coppied to clipboard.");
        return;
      }
      
      PicesRaster  r = dbConn.ImageFullSizeFind (images[0].ImageFileName);
      if  (r == null)
      {
        MessageBox.Show (this, "Could not load image[" + images[0].ImageFileName + "].");
        return;
      }

      Image  i = r.BuildBitmap ();
      if  (i != null)
        Clipboard.SetImage (i);
    }  /* CopyImageToClipboard */




    private  void  CopyImageToClipboardWithGrid (Object sender, EventArgs e)
    {
      PicesDataBaseImageList  images = GetSelectedImages ();
      if  (images == null)  return;

      if  (images.Count != 1)
      {
        MessageBox.Show (this, "Only one image can be coppied to clipboard.");
        return;
      }
      
      PicesRaster  r = dbConn.ImageFullSizeFind (images[0].ImageFileName);
      if  (r == null)
      {
        MessageBox.Show (this, "Could not load image[" + images[0].ImageFileName + "].");
        return;
      }

      if  (!r.Color ())
      {
        r = r.ReversedImage ();
        r = r.ToColor ();
      }

      //r.DrawGrid (29.41176f, 10, Color.Red, Color.FromArgb (30, 255, 0, 0));
      r.DrawGrid (35.8842f, 10, Color.Red, Color.FromArgb (30, 255, 0, 0));
      
      Image  i = r.BuildBitmap ();
      if  (i != null)
        Clipboard.SetImage (i);
    }  /* CopyImageToClipboardWithGrid */



    private  void  CallSipperFileViewer (Object sender, EventArgs e)
    {
      PicesDataBaseImageList  images = GetSelectedImages ();
      if  (images == null)
        return;

      if  (images.Count < 1)
      {
        MessageBox.Show (this, "There is not image selected.");
        return;
      }

      if  (images.Count > 1)
      {
        MessageBox.Show (this, "More than one image is selected.\n\n   Select Only One Image.");
        return;
      }

      SipperFileViewer.SipperFileViewer.SipperFileViewerRegionAroundImage (images[0], runLog);
    }  /* CallSipperFileViewer */




    private  void  ValidateSelectedImagesWithThisClass (PicesClass  validatedClass)
    {
      PicesDataBaseImageList  selectedImages = GetSelectedImages ();

      foreach  (PicesDataBaseImage  i  in  selectedImages)
      {
        if  (i == null)
          return;

        i.ValidatedClass = validatedClass;
        try {dbConn.ImagesUpdateValidatedClass (i.ImageFileName, validatedClass);}
        catch  (Exception e)
        {
          MessageBox.Show (this, "Exception occured calling dbConn.ImagesUpdateValidatedClass for image [" + i.ImageFileName + "]\n\n" + 
                           e.ToString () + "\n\n" +
                           e.StackTrace, 
                           "ValidateSelectedImagesWithThisClass");
        }
      }

      UpdateLastClassesValidated (validatedClass);
      BuildThumbnailGridViewConetextMenu ();
    }  /*  ValidateSelectedImagesWithThisClass */



    private  void  DeleteImage (Object sender, EventArgs e)
    {
      PicesDataBaseImageList  selectedImages = GetSelectedImages ();
      if  ((selectedImages == null)  ||  (selectedImages.Count < 1))
      {
        MessageBox.Show (this, "No images selected", "Delete Image", MessageBoxButtons.OK);
        return;
      }

      DialogResult dr = VerifyImagesDeletion.Show (this, selectedImages);
      if  (dr == DialogResult.Yes)
      {
        foreach  (PicesDataBaseImage i in selectedImages)
        {
          dbConn.ImagesDeleteOneImage (i.ImageFileName);
          if  (!dbConn.LastOpSuccessful)
          {
            MessageBox.Show (this, "Error deleting image  \"" + i.ImageFileName + "\" "  + dbConn.LastErrorDesc (), "Delete Images");
            break;
          }
          else
          {
            curSelImages.Remove (i);
          }
        }
        
        DefineThumbnailGridViewRowAndColumns ();
      }
    }  /* DeleteImage */





    private  void  RemoveValidation (Object sender, EventArgs e)
    {
      PicesDataBaseImageList  selectedImages = GetSelectedValidatedImages ();
      if  ((selectedImages == null)  ||  (selectedImages.Count < 1))
      {
        MessageBox.Show (this, "No validated images selected", "Remove Validation", MessageBoxButtons.OK);
        return;
      }

      DialogResult dr = DialogResult.No;
      if  (selectedImages.Count < 2)
        dr = MessageBox.Show (this, "Remove validation from selected images ?", "Remove Validation", MessageBoxButtons.YesNo); 
      else
        dr = MessageBox.Show (this, "Remove validation from these " + selectedImages.Count + " selected images ?", "Remove Validation", MessageBoxButtons.YesNo); 

      if  (dr == DialogResult.Yes)
      {
        List<String>  errorMsgs = new List<string> ();
        foreach  (PicesDataBaseImage i in selectedImages)
        {
          dbConn.ImageRemoveValidation (i.ImageFileName);
          if  (!dbConn.LastOpSuccessful)
            errorMsgs.Add ("Error removing validation from \"" + i.ImageFileName + "\" "  + dbConn.LastErrorDesc ());
          else
            i.ValidatedClass = null;
        }
        if  (errorMsgs.Count > 0)
        {
          MessageBox.Show (this, "Errors occured removing validation from images", "Remove Validation");
        }
      }
    }  /* RemoveValidation */



    private  void  ValidateSelectAClass (Object sender, EventArgs e)
    {
      String  rootDir = "";
      //PicesClassList  activeClasses = null;

      if  (activeClasses == null)
        UpdateActiveClassesList ();

      PicesClass likelyClass = null;
      PicesDataBaseImageList  selectedImages = GetSelectedImages ();
      if  ((selectedImages != null)  &&  (selectedImages.Count > 0))
      {
        PicesDataBaseImage  dbi = selectedImages[0];

        likelyClass = dbi.ValidatedClass;
        if  (likelyClass == null)
          likelyClass = dbi.Class1;
      }

      SelectAPicesClass sac = new SelectAPicesClass (allClasses, rootDir, activeClasses);
      sac.SelectedClass = likelyClass;
      sac.ShowDialog ();
      if  (sac.SelectionMade  &&  (sac.SelectedClass != null))
         ValidateSelectedImagesWithThisClass (sac.SelectedClass);
    }  /*  ValidateSelectAClass */



    private  void  ValidateTheseClasses (Object sender, EventArgs e)
    {
      ToolStripItem item = (ToolStripItem)sender;
      String  validatedClassName = item.Text;
      PicesClass validatedClass = PicesClassList.GetUniqueClass (validatedClassName, "");
      ValidateSelectedImagesWithThisClass (validatedClass);
    }


    
    private  void  DisplayTheOriginalSipperImage (PicesDataBaseImage i)
    {
      PicesRaster  pr = null;
      try  {pr = dbConn.ImageFullSizeFind (i.ImageFileName);}
      catch  (Exception e)
      {
        MessageBox.Show (this, "Exception occured attempting to call 'ImageFullSizeFind'\n\nException Text is:" + 
                         e.ToString () + "\n\n" +
                         e.StackTrace, 
                         "DisplayTheOriginalSipperImage");
        pr = null;
        return;
      }
      if  (pr == null)
      {
        MessageBox.Show (this, "Could not locate source image or SipperFile[" + i.SipperFileName + "]");
        return;
      }

      SipperFile.ImageViewer iv = new SipperFile.ImageViewer (pr, i, allClasses);
      iv.ShowDialog ();

      if  (iv.ClassUserValidatesAs != null)
        UpdateLastClassesValidated (iv.ClassUserValidatesAs);

      if  (ActiveTrainingLibraries.Model1Name.CompareTo (TrainingLibrary1.Text) != 0)
        TrainingLibrary1.Text = ActiveTrainingLibraries.Model1Name;

      if  (ActiveTrainingLibraries.Model2Name.CompareTo (TrainingLibrary2.Text) != 0)
        TrainingLibrary2.Text = ActiveTrainingLibraries.Model2Name;

      iv = null;
    }  /* DisplayTheOriginalSipperImage */


  
    private  void  BuildThumbnailGridViewConetextMenu ()
    {
      ContextMenuStrip cms = new ContextMenuStrip ();

      if  (allowUpdates)
        cms.Items.Add ("Add to Training Library",    null, AddToTrainingLibrary);

      cms.Items.Add ("Sipper File Viewer",           null, CallSipperFileViewer);
      cms.Items.Add ("Save Image",                   null, SaveImage);
      cms.Items.Add ("Copy Image to Clipboard",      null, CopyImageToClipboard);
      cms.Items.Add ("Copy Image to Clipboard with Grid",   null, CopyImageToClipboardWithGrid);

      if  (allowUpdates)
      {
        cms.Items.Add ("Delete Image",                 null, DeleteImage);
        cms.Items.Add ("Remove Validation",            null, RemoveValidation);
        cms.Items.Add ("Select Class", null, ValidateSelectAClass);
        foreach  (PicesClass pc in lastClassesValidated)
        {
          cms.Items.Add (pc.Name, null, ValidateTheseClasses);
          if  (cms.Items.Count > 10)
            break;
        }
      }

      foreach  (DataGridViewColumn  dgvc in ThumbnailDataGridView.Columns)
        dgvc.ContextMenuStrip = cms;
    }  /* BuildThumbnailGridViewConetextMenu */




    private  void  PicesCommanderFormResized ()
    {
      if  (lastHeight == 0)
        return;

      int  deltaHeight = Height - lastHeight;
      int  deltaWidth  = Width  - lastWidth;

      lastHeight = Height;
      lastWidth  = Width;

      NavigationPanel.Height += deltaHeight;

      TabControl.Height += deltaHeight;
      TabControl.Width  += deltaWidth;

      ThumbnailDataGridView.Height  += deltaHeight;
      ThumbnailDataGridView.Width   += deltaWidth;

      DistributionChart.Height += deltaHeight;
      DistributionChart.Width  += deltaWidth;

      DefineThumbnailGridViewRowAndColumns ();
    }  /* PicesCommanderFormResized */
    

    
    
    private void PicesCommander_ResizeEnd(object sender, EventArgs e)
    {
      PicesCommanderFormResized ();
    }



    private void Cruise_SelectedIndexChanged(object sender, EventArgs e)
    {
      Cruise_ValidateField ();
    }



    private void Cruise_Validating(object sender, CancelEventArgs e)
    {
      Cruise_ValidateField ();
    }


    private  void  Cruise_ValidateField ()
    {
      String  newCruiseName = Cruise.Text;
      if  (newCruiseName != cruiseName)
      {
        cruiseName = newCruiseName;
        LoadStations ();
        LoadDeployments ();
      }
    }



    private void Station_SelectedIndexChanged(object sender, EventArgs e)
    {
      Station_ValidateField ();
    }


    private void Station_Validating(object sender, CancelEventArgs e)
    {
      Station_ValidateField ();
    }


    private void  Station_ValidateField ()
    {
      String  newStationName = Station.Text;
      if  (newStationName != stationName)
      {
        stationName = newStationName;
        LoadDeployments ();
      }
    }



    private void Deployment_SelectedIndexChanged(object sender, EventArgs e)
    {
      Deployment_ValidateField ();
    }


    private void Deployment_Leave(object sender, EventArgs e)
    {
      Deployment_ValidateField ();
    }


    private void  Deployment_ValidateField ()
    {
      String  newDeploymentNum = Deployment.Text;
      if  (newDeploymentNum != deploymentNum)
      {
        deploymentNum = newDeploymentNum;
        LoadSipperFiles ();
      }
    }

    private void  SipperFileName_ValidateField ()
    {
      String  newSipperFile = (String)(SipperFile.Text);
      if  (newSipperFile != sipperFileName)
      {
        sipperFileName = newSipperFile;
      }
    }

    private void  SizeMin_ValidateField ()
    {
      sizeMin = PicesKKStr.StrToInt (SizeMin2.Text);
      if  (String.IsNullOrEmpty (SizeMin2.Text) ||  (sizeMin < 1))
        sizeMin = 0;
      SizeMin2.Text = sizeMin.ToString ("##,###,##0");
    }


    private void  SizeMax_ValidateField ()
    {
      sizeMax = PicesKKStr.StrToInt (SizeMax2.Text);
      if  (String.IsNullOrEmpty (SizeMax2.Text) ||  (sizeMax < 1)  ||  (SizeMax2.Text == infinityStr))
      {
        sizeMax = 0;
        SizeMax2.Text = infinityStr;
        SizeMax2.Font = new Font (SizeMax2.Font.FontFamily, 12); 
        return;
      }
      else
      {
        SizeMax2.Font = new Font (SizeMax2.Font.FontFamily, 10); 
      }
      
      if  (sizeMax < sizeMin)
        sizeMax = sizeMin;
      SizeMax2.Text = sizeMax.ToString ("##,###,##0");
    }  /* SizeMax_ValidateField */





    private void SipperFile_Validating(object sender, CancelEventArgs e)
    {
      SipperFileName_ValidateField ();
    }


    private void  DepthMin_ValidateField ()
    {
      depthMin = PicesKKStr.StrToFloat (DepthMin.Text);
      if  (String.IsNullOrEmpty (SizeMin2.Text) ||  (depthMin < 0.0f))
        depthMin = 0.0f;
        
      DepthMin.Text = depthMin.ToString ("#,##0.0");
    }



    private void DepthMax_ValidateField ()
    {
      depthMax = PicesKKStr.StrToFloat (DepthMax.Text);
      if  (String.IsNullOrEmpty (DepthMax.Text) ||  (depthMax <= 0.0f)  ||  (DepthMax.Text == infinityStr))
      {
        depthMax = 0.0f;
        DepthMax.Text = infinityStr;
        return;
      }
      if  (depthMax <= depthMin)
        depthMax = depthMin + 1.0f;
      DepthMax.Text = depthMax.ToString ("#,##0.0");
    }



    private void ProbabilityMin_ValidateField ()
    {
      float  newProbMin = PicesKKStr.StrToFloat (ProbabilityMin.Text);
      if  (newProbMin != probMin)
      {
        probMin = newProbMin;
        if  (probMin < 0.0f) 
        {
          probMin = 0.0f;
          ProbabilityMin.Focus ();
        }
        else if  (probMin > 1.0f)
        {
          probMin = 1.0f;
          ProbabilityMin.Focus ();
        }

        ProbabilityMin.Text = probMin.ToString ("p2");
      }
    }


    private void ProbabilityMax_ValidateField ()
    {
      float  newProbMax = PicesKKStr.StrToFloat (ProbabilityMax.Text);
      if  (newProbMax != probMax)
      {
        probMax = newProbMax;
        if  (probMax < probMin) 
        {
          probMax = probMin;
          ProbabilityMax.Focus ();
        }
        else if  (probMax > 1.0f)
        {
          probMax = 1.0f;
          ProbabilityMax.Focus ();
        }

        ProbabilityMax.Text = probMax.ToString ("p2");
      }
    }



    private  void  PlanktonClass_ValidateField ()
    {
      String  curClassName = "";
      if  (selectedClass != null)
        curClassName = selectedClass.Name;
      String  newClassName = (String)(PlanktonClass.Text).Trim ();
      if  (newClassName != curClassName)
      {
        if  (String.IsNullOrEmpty (newClassName))
        {
          selectedClass = null;
        }
        else
        {
          selectedClass = dbConn.MLClassLoad (newClassName);
          if  (selectedClass == null)
          {
            MessageBox.Show (this, "No such Plankton Class[" + newClassName + "]  in PICES database", "Validate Plankton Class");
            PlanktonClass.Focus ();
          }
        }
      }
    }  /* PlanktonClass_ValidateField */




    private  void  ImageGroup_ValidateField ()
    {
      ImageGroupSet  (ImageGroup.Text);
    }  /* ImageGroupUpdated */


    private  void  ValidateAllFields ()
    {
      Cruise_ValidateField ();
      Station_ValidateField ();
      Deployment_ValidateField ();
      SipperFileName_ValidateField ();
      SizeMin_ValidateField ();
      SizeMax_ValidateField ();
      DepthMin_ValidateField ();
      DepthMax_ValidateField ();
      ProbabilityMin_ValidateField ();
      ProbabilityMax_ValidateField ();
      PlanktonClass_ValidateField ();
      ImageGroup_ValidateField ();
    }  /* ValidateAllFields */

    
    private  void  ValidateAndRefreshNavigationPanel ()
    {
      ValidateAllFields ();
      NavigationPanelButtonsDisable ();
      SaveConfiguration ();
      RefreshNavigationPanel ();
      NavigationPanelButtonsEnable ();
    }


    private void RefreshButton_Click (object sender, EventArgs e)
    {
      ValidateAndRefreshNavigationPanel ();
    }


    private void connectToDatabaseToolStripMenuItem_Click(object sender, EventArgs e)
    {
      PicesDataBaseServer  curServer = null;
      if  (dbConn != null)
      {
        curServer = dbConn.Server;
        dbConn.Close ();
        GC.Collect ();
      }

      PicesDataBase.CloseGlobalDbConn ();

      dbConn = PicesDataBase.SelectNewDataBaseServer (runLog);
      if  ((dbConn == null)  ||  (!dbConn.Valid ()))
      {
        if  (curServer != null)
          dbConn = new PicesDataBase (curServer, runLog);
      }

      if  ((dbConn == null)  ||  (!dbConn.Valid ()))
      {
        ClosePicesCommanderWindow ();
        return;
      }

      if  ((curServer == null)  ||  (curServer.Description != dbConn.Server.Description))
      {
        // We switched servers;  should terminate any threads that are loading data from the old server.
        CancelImageLoaderThreadIfRunning ();
        ValidateAndRefreshNavigationPanel ();
      }
      
      LoadDataBaseVariables ();
    }



    private  void  UpdateCriteria (PicesClass  mlClass)
    {
      if  (selecetedImageGroup == null)
        Criteria.Text = cruiseName;
      else
      {
        Criteria.Text = selecetedImageGroup.Name;
        if  (!String.IsNullOrEmpty (cruiseName))
          Criteria.Text += cruiseName;
      }

      if  (!String.IsNullOrEmpty (stationName))
        Criteria.Text += ", " + stationName;

      if  (!String.IsNullOrEmpty (deploymentNum))
        Criteria.Text += ", " + deploymentNum;

      if  (!String.IsNullOrEmpty (sipperFileName))
        Criteria.Text += ", " + sipperFileName;

      if  (mlClass != null)
        Criteria.Text += ", " + mlClass.Name;

      if  ((sizeMin > 0)  ||  (sizeMax > 0))
      {
        Criteria.Text += ", Size(" + sizeMin + "-";
        if  (sizeMax < 1)
          Criteria.Text += infinityStr;
        else
          Criteria.Text += sizeMax;
        Criteria.Text += ")";
      }

      if  ((depthMin > 0.0f)  ||  (depthMax > 0.0f))
      {
        Criteria.Text += ", Depth(" + depthMin + "-";
        if  (depthMax <= 0.0f)
          Criteria.Text += infinityStr;
        else
          Criteria.Text += depthMax;
        Criteria.Text += ")";
      }

      if  ((probMin > 0.0f)  ||  (probMax <  1.0f))
        Criteria.Text += ", Prob(" + probMin.ToString ("p2") + "-" + probMax.ToString ("p2") + ")";
    }  /* UpdateCriteriaBox */



    private  void  CancelImageLoaderThreadIfRunning ()
    {
      if  (imageLoader != null)
      {
        if  (imageLoader.LoaderThreadRuning)
        {
          imageLoader.CancelLoadWait (6000);  // Cancel current loaded;  wait ut to 6 seconds for gracefull cancelation
        }

        imageLoader = null;
      }
    }  /* CancelImageLoaderThreadIfRunning*/


    private  void  ResetThumbNailPanel ()
    {
      curSelImages = new PicesDataBaseImageList ();
      thumbnailCurRow = 0;
      thumbnailCurCol = 0;

      DefineThumbnailGridViewRowAndColumns ();
      GC.Collect ();
    }


    private void  NavigationList_MouseDoubleClickLeftButton (PicesClass  mlClass)
    {
      CancelImageLoaderThreadIfRunning ();
      ResetThumbNailPanel ();

      PicesClass  classToUse = mlClass;
      if  (selectedClass != null)
        classToUse = selectedClass;

      UpdateCriteria (classToUse);
      thumbnailClass = classToUse;

      // A timer object "ThumbNailPanelTimer_Tick"  will poll "imageLoader" periodicly to add images to the display.
      imageLoader = new PlanktonImageLoader (selecetedImageGroup,
                                             cruiseName, stationName, deploymentNum, sipperFileName,
                                             classToUse, classKeyToUse,
                                             probMin, probMax,  sizeMin, 
                                             sizeMax, depthMin, depthMax, 
                                             0
                                            );
      ThumbNailPanelTimer.Enabled = true;
    }  /* NavigationList_MouseDoubleClickLeftButton */




    private void  NavigationList_MouseDoubleClickRightButton (PicesClass  mlClass)
    {
      String  dir = null;
      if  (ActiveTrainingLibraries.Model1 () != null)
        dir = ActiveTrainingLibraries.Model1 ().DirectoryPathForClass (mlClass);
      if  ((dir == null)   &&   (ActiveTrainingLibraries.Model2 () != null))
        dir = ActiveTrainingLibraries.Model2 ().DirectoryPathForClass (mlClass);
      
      if  (String.IsNullOrEmpty (dir))
        return;

      DisplayPicesImages  dpi = new DisplayPicesImages (dir);
      dpi.ShowDialog ();
      dpi.Dispose ();
      dpi = null;
    }  /* NavigationList_MouseDoubleClickRightButton */



    private void NavigationList_MouseDoubleClick (object sender, MouseEventArgs e)
    {
      // User has requested to display contents for a goven class.
      ListView  lv = (ListView)sender;
      ListView.SelectedIndexCollection  indexes =   lv.SelectedIndices;

      if  (indexes.Count != 1)
        return;
        
      int  index = indexes[0];

      if  (navigateBy == NavigationOptions.ByClass)
      {
        String  className = lv.Items[index].Text;
        PicesClass  mlClass = PicesClassList.GetUniqueClass (className, "");
      
        if  (e.Button == MouseButtons.Left)
          NavigationList_MouseDoubleClickLeftButton (mlClass);
        else
          NavigationList_MouseDoubleClickRightButton (mlClass);
      }

      else if  (e.Button == MouseButtons.Left)
      {
        // We are going to display images from the selected depth range.
        String[]  depthStr = lv.Items[index].Text.Split ('-');
        if  (depthStr.Length != 2)
          return;
        depthMin = PicesKKStr.StrToFloat (depthStr[0]);
        depthMax = PicesKKStr.StrToFloat (depthStr[1]);
        LoadCriteriaFields ();
        NavigationList_MouseDoubleClickLeftButton (null);
      }
    }  /* NavigationList_MouseDoubleClick */



    private void SizeMin_Leave (object sender, EventArgs e)
    {
      SizeMin_ValidateField ();
    }


    private void  SizeMax_Leave (object sender, EventArgs e)
    {
      SizeMax_ValidateField ();
    }


    private void DepthMin_Leave(object sender, EventArgs e)
    {
      DepthMin_ValidateField ();
    }


    private void DepthMax_Leave(object sender, EventArgs e)
    {
      DepthMax_ValidateField ();
    }


    private void ProbabilityMin_Leave(object sender, EventArgs e)
    {
      ProbabilityMin_ValidateField ();
    }


    private void ProbabilityMax_Leave(object sender, EventArgs e)
    {
      ProbabilityMax_ValidateField ();
    }


    private  void  ValidateDepthIncrement ()
    {
      depthIncrement = PicesKKStr.StrToFloat (DepthIncrement.Text);
      depthIncrement = Math.Min (100.0f, depthIncrement);
      depthIncrement = Math.Max (0.1f, depthIncrement);
      DepthIncrement.Text = depthIncrement.ToString ("##0.0");
    }



    private void PlanktonClass_Leave(object sender, EventArgs e)
    {
      PlanktonClass_ValidateField ();
    }


    private void PlanktonClass_SelectedIndexChanged(object sender, EventArgs e)
    {
      PlanktonClass_ValidateField ();
    }



    private void ThumbNailPanelTimer_Tick(object sender, EventArgs e)
    {
      if  (imageLoader == null)
      {
        ThumbNailPanelTimer.Enabled = false;
        return;
      }

      // We need to check if the LoaderThread is still running before we get anymore images from
      // the object "imageLoader".  If it is no longer running we can then get rid of it after 
      // we retrieve what ever images it still has  otherwise we we retrieve what imeages it has
      // and then leave it alone.
      bool  loaderThreadRuning = imageLoader.LoaderThreadRuning;

      PicesDataBaseImageList  newlyLoadedImages = imageLoader.GetImagesAlreadyLoaded ();
      if  (newlyLoadedImages != null)
      {
        if  (curSelImages == null)
          curSelImages = new PicesDataBaseImageList ();

        if  ((sortOrder != PicesDataBaseImageList.SortOrderType.soSpatial)  &&  (curSelImages.Count == 0))
          newlyLoadedImages.Sort (sortOrder, reverseSort);

        String  curSipperFile = imageLoader.CurSipperFileName;
        if  (curSipperFile == null)
          curSipperFile = "";

        LoadStatus.Text = curSipperFile + " (" + imageLoader.SipperFilesLoaded + " of " + imageLoader.SipperFilesCount + ")  " +
                          imageLoader.ImagesLoadedCount.ToString ("###,##0") + "  images";

        curSelImages.AddList (newlyLoadedImages);
        AddToThumbNailPanel (newlyLoadedImages);
      }

      if  (!loaderThreadRuning)
      {
        LoadStatus.Text = "Done   " + imageLoader.ImagesLoadedCount.ToString ("###,##0") + "  images loaded";
        ThumbNailPanelTimer.Enabled = false;
        imageLoader.CleanUpMemory ();
        imageLoader = null;
        GC.Collect ();
      }
    }





    private void ThumbnailDataGridView_CellFormatting (object sender, DataGridViewCellFormattingEventArgs e)
    {
      return;
    }



    public  void ThumbnailDataGridView_CellContentDoubleClick (object sender, DataGridViewCellEventArgs e)
    {
      int  colIDX = e.ColumnIndex;
      int  rowIDX = e.RowIndex;

      PicesDataBaseImage image = (PicesDataBaseImage)ThumbnailDataGridView.Rows[rowIDX].Cells[colIDX].Value;
      if  (image == null)
        return;
      DisplayTheOriginalSipperImage (image);
    }



    private void TrainingLibrary1_SelectedIndexChanged(object sender, EventArgs e)
    {
      String newTrainingLibraryName = TrainingLibrary1.Text;
      if (newTrainingLibraryName != ActiveTrainingLibraries.Model1Name)
        LoadTrainingModel1 (newTrainingLibraryName, false);  // false = Dont Force Rebuild of Model.
    }


    private void RebuildTrainLibrary1Button_Click(object sender, EventArgs e)
    {
      if  (!String.IsNullOrEmpty (TrainingLibrary1.Text))
        LoadTrainingModel1 (TrainingLibrary1.Text, true);  // true = Force Rebuild of Traning Model.
    }

    private void TrainingLibrary2_SelectedIndexChanged(object sender, EventArgs e)
    {
      String newTrainingLibraryName = TrainingLibrary2.Text;
      if (newTrainingLibraryName != ActiveTrainingLibraries.Model2Name)
        LoadTrainingModel2 (newTrainingLibraryName, false);
    }

    private void RebuildTrainLibrary2Button_Click(object sender, EventArgs e)
    {
      if  (!String.IsNullOrEmpty (TrainingLibrary2.Text))
        LoadTrainingModel2 (TrainingLibrary2.Text, true);
    }


    private  void  ValidateSortOrder ()
    {
      PicesDataBaseImageList.SortOrderType  newSortOrder = PicesDataBaseImageList.SortOrderType.soNULL;
      String  newSortOrderStr = SortOrder.Text.Trim ();
      if  (!String.IsNullOrEmpty (newSortOrderStr))
      {
        newSortOrder = PicesDataBaseImageList.SortOrderTypeFromStr (newSortOrderStr);
        if  (newSortOrder  == PicesDataBaseImageList.SortOrderType.soNULL)
        {
          // Invalid Sort Order Entered.
          MessageBox.Show (this, "Invalid Sort Order[" + newSortOrderStr + "]");
          SortOrder.Focus ();
        }
        else
        {
          sortOrder = newSortOrder;
          UpdateThumbNailDisplayOptions ();
          if  (curSelImages != null)
          {
            curSelImages.Sort (sortOrder, reverseSort);
            DefineThumbnailGridViewRowAndColumns ();
            ThumbnailDataGridView.Focus ();
          }
        }
      }
    }  /* ValidateSortOrder */





    private void SortOrder_SelectedIndexChanged(object sender, EventArgs e)
    {
      ValidateSortOrder ();
    }



    private void UpDownButton_Click (object sender, EventArgs e)
    {
      if  (UpDownButton.Text == "Up")
      {
        reverseSort = false;
        UpDownButton.Text = "Down";
      }
      else
      {
        reverseSort = true;
        UpDownButton.Text = "Up";
      }
      
      curSelImages.Sort (sortOrder, reverseSort);
      DefineThumbnailGridViewRowAndColumns ();
    }

    
    
    private void PicesCommander_SizeChanged(object sender, EventArgs e)
    {
      if  (WindowState == FormWindowState.Maximized)
      {
        // Looks like user has pressed the Maximized button.  We have to trap it here because
        // the ResizeEnd envent does not trap when form is Maximized.
        PicesCommanderFormResized ();
        formIsMaximized = true;
      }
      else if  (WindowState == FormWindowState.Normal)
      {
        if  (formIsMaximized)
        {
          // We normally trap the ResizeEnd event;  but when the form was already maximized and the user
          // presses the button to unmaximize.  the ResizeEnd does not trap that.  So we check to 
          // see if the form was already maximize.  If so then we resized the form.
          PicesCommanderFormResized ();
          formIsMaximized = false;
        }
      }
    }


    private void ThumbnailDataGridView_DataError (object sender, DataGridViewDataErrorEventArgs e)
    {
      int  colIdx = e.ColumnIndex;
      int  rowIdx = e.RowIndex;
    }



    private void classificationRunToolStripMenuItem_Click (object sender, EventArgs e)
    {
      ValidateAllFields ();
      SaveConfiguration ();

      ClassificationStatus  classificationReport = new ClassificationStatus (this, 
                                                                             selecetedImageGroup,
                                                                             cruiseName, 
                                                                             stationName,
                                                                             deploymentNum,
                                                                             sipperFileName,
                                                                             selectedClass,
                                                                             classKeyToUse,
                                                                             sizeMin,   sizeMax,   
                                                                             depthMin,  depthMax,  
                                                                             probMin,   probMax
                                                                            );
      classificationReport.ShowDialog ();
      classificationReport.CleanUpMemory ();
      classificationReport = null;
    }


    private  void  CancelAllThreadsInPreperationForExitingProgram ()
    {
      ThumbNailPanelTimer.Enabled = false;
      navigationPanelTimer.Enabled = false;

      if  (imageLoader != null)
        imageLoader.CancelLoad ();
      
      if  (navigationClassStatsLoader != null)
        navigationClassStatsLoader.CancelLoad ();

      if  (navigationDepthStatsLoader != null)
        navigationDepthStatsLoader.CancelLoad ();

      ActiveTrainingLibraries.CancelLoads ();

      if  (imageLoader != null)
        imageLoader.CancelLoadWait (3000);
      
      if  (navigationClassStatsLoader != null)
        navigationClassStatsLoader.CancelLoadWait (3000);

      if  (navigationDepthStatsLoader != null)
        navigationDepthStatsLoader.CancelLoadWait (3000);

      ActiveTrainingLibraries.CancelLoadWait (3000);
    }  /* CancelAllThreadsInPreperationForExitingProgram */



    private void PicesCommander_FormClosing (object sender, FormClosingEventArgs e)
    {
      SaveConfiguration ();
      CancelAllThreadsInPreperationForExitingProgram ();
      if  (dbConn != null)
      {
        dbConn.Close ();
        dbConn = null;
        PicesDataBase.CloseGlobalDbConn ();
        GC.Collect ();
      }

      /** I need to call "PicesDataBase.FinalCleanUp" at the same level that "PicesDataBase.Initialization" gets called.
       * The first time a "PicesDataBase" instance is created it will call the MySQL initailzation code.  And since in this 
       * application it is occuring in "PicesCommander" we need to call "PicesDataBase.FinalCleanUp" before we exit.
       */
      PicesDataBase.FinalCleanUp ();
    }



    private void createTrainingModelFromDirectoryStructureToolStripMenuItem_Click(object sender, EventArgs e)
    {
    }



    private void RandomlyHarvestToolStripMenuItem_Click (object sender, EventArgs e)
    {
      ValidateAllFields ();
      CancelImageLoaderThreadIfRunning ();
      ResetThumbNailPanel ();

      PicesClass  selClass = null;
      if  (!String.IsNullOrEmpty (PlanktonClass.Text))
        selClass = PicesClassList.GetUniqueClass (PlanktonClass.Text, "");

      Harvester  harvester = new Harvester (cruiseName, 
                                            stationName, 
                                            deploymentNum, 
                                            sipperFileName, 
                                            selClass,        classKeyToUse, 
                                            sizeMin,         sizeMax
                                           );
      harvester.ShowDialog ();
      if  (!harvester.CancelHarvesting)
      {
        PicesDataBaseImageList  newlyLoadedImages = harvester.HarvestedImages;
        if  (newlyLoadedImages != null)
        {
          ResetThumbNailPanel ();
          sortOrder = PicesDataBaseImageList.SortOrderType.soDepth;
          UpdateThumbNailDisplayOptions ();
          newlyLoadedImages.Sort (sortOrder, reverseSort);
          curSelImages.AddList (newlyLoadedImages);
          AddToThumbNailPanel (newlyLoadedImages);
          LoadImageGroupChoices ();
          ImageGroup.Text = harvester.GroupNameSelected;
        }
      }
    }  /* RandomlyHarvestToolStripMenuItem_Click */



    private void saveImagesToDiskToolStripMenuItem_Click(object sender, EventArgs e)
    {
      ValidateAllFields ();
      SaveImagesToDisk2  sitd = new SaveImagesToDisk2 (dbConn, 
                                                       null,   // List of images; (if == null)  then use the criteria in the following parameters to select images from database.
                                                       selecetedImageGroup,
                                                       cruiseName, stationName, deploymentNum, 
                                                       sipperFileName,  
                                                       selectedClass,
                                                       classKeyToUse,
                                                       probMin,  probMax,
                                                       sizeMin,  sizeMax,
                                                       depthMin, depthMax
                                                      );
      sitd.ShowDialog ();
      return;
    }



    private void saveFeatureDataToolStripMenuItem_Click(object sender, EventArgs e)
    {
      ValidateAllFields ();
      SaveFeatureData sfd = new SaveFeatureData (dbConn, 
                                                 selecetedImageGroup,
                                                 cruiseName, stationName, deploymentNum, 
                                                 sipperFileName,  
                                                 selectedClass,
                                                 classKeyToUse,
                                                 probMin,  probMax,
                                                 sizeMin,  sizeMax,
                                                 depthMin, depthMax
                                                );
      sfd.ShowDialog ();
      return;
    }



    private void savePlanktonDataToolStripMenuItem_Click (object sender, EventArgs e)
    {
      ValidateAllFields ();
      SavePlanktonData spd = new SavePlanktonData (dbConn, 
                                                   selecetedImageGroup,
                                                   cruiseName, stationName, deploymentNum, 
                                                   sipperFileName,  
                                                   selectedClass,
                                                   classKeyToUse,
                                                   probMin,  probMax,
                                                   sizeMin,  sizeMax,
                                                   depthMin, depthMax
                                                  );
      spd.ShowDialog ();
      return;
    }




    private void ImageGroup_SelectedIndexChanged(object sender, EventArgs e)
    {
      ImageGroupSelected ();
    }


    
    private void ImageGroup_Leave(object sender, EventArgs e)
    {
      ImageGroup.SelectedItem = ImageGroup.Text;
      ImageGroupUpdated ();
    }



    private void NavigationButton_Click(object sender, EventArgs e)
    {
      NavigationPanelButtonsDisable ();

      ValidateAllFields ();

      if  (navigateBy == NavigationOptions.ByClass)
        navigateBy = NavigationOptions.ByDepth;
      else
        navigateBy = NavigationOptions.ByClass;

      UpdateThumbNailDisplayOptions ();

      RefreshNavigationPanel ();

      NavigationPanelButtonsEnable ();
    }


    private void importFromADirectoryToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
      {
        UpdatesNotAllowedMsg ();
        return;
      }

      ImportGroupAssignments  iga = new ImportGroupAssignments ();
      iga.ShowDialog ();
      if  (!iga.CancelBackGround)
      {
        LoadImageGroupChoices ();
        if  (iga.Group != null)
        {
          ImageGroup.Text = iga.Group.Name;
          ImageGroupUpdated ();
        }
      }
    }



    private void DepthIncrement_SelectedIndexChanged(object sender, EventArgs e)
    {
      DepthIncrement.Text = DepthIncrement.SelectedItem.ToString ();
      ValidateDepthIncrement ();
    }


    private void DepthIncrement_Leave(object sender, EventArgs e)
    {
      ValidateDepthIncrement ();
    }



    private void importValidatedClassAssignmentsFromDirectoryToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
      {
        UpdatesNotAllowedMsg ();
        return;
      }

      ValidateAllFields ();
      ImportValidatdClass ivc = new ImportValidatdClass ();
      ivc.ShowDialog ();
      if  (!ivc.CancelImporting)
        RefreshNavigationPanel ();
    }



    private void classMaintenanceToolStripMenuItem_Click(object sender, EventArgs e)
    {
      ClassMaintenance  cm = new ClassMaintenance ();
      cm.ShowDialog ();
    }


    private void navigationPanelTimer_Tick(object sender, EventArgs e)
    {
      if  (navigationClassStatsLoader != null)
      {
        bool  loaderThreadRuning = navigationClassStatsLoader.LoaderThreadRuning;

        PicesClassStatisticList stats = navigationClassStatsLoader.GetStatsAlreadyLoadedIfNew ();
        if  (stats != null)
        {
          classStats = stats;
          UpdateNavigationList (classStats);
        }

        NavigationPanaelStatus.Text = navigationClassStatsLoader.Status ();
        if  (!loaderThreadRuning)
          navigationClassStatsLoader = null;
      }

      if  (navigationDepthStatsLoader != null)
      {
        bool  loaderThreadRuning = navigationDepthStatsLoader.LoaderThreadRuning;
        uint[]  stats = navigationDepthStatsLoader.GetStatsAlreadyLoadedIfNew ();
        if  (stats != null)
        {
          depthStats = stats;
          volumeStats = navigationDepthStatsLoader.VolumeWaterSampled;
          PopulateNavigationListWithDepthStats (depthStats);
        }

        NavigationPanaelStatus.Text = navigationDepthStatsLoader.Status ();
        if  (!loaderThreadRuning)
          navigationDepthStatsLoader = null;
      }

      if  ((navigationClassStatsLoader == null)  &&  (navigationDepthStatsLoader == null))
        navigationPanelTimer.Enabled = false;
    }


    private void cruiseStationDeploymentsToolStripMenuItem_Click(object sender, EventArgs e)
    {
      CruiseMaintenance  cm = new CruiseMaintenance (dbConn, runLog);
      cm.ShowDialog (this);
    }


    private void sIPPERDeviceParametersToolStripMenuItem_Click(object sender, EventArgs e)
    {
      SipperConfigRec  configRec = new SipperConfigRec ();
      SipperConfigForm  cf = new SipperConfigForm (configRec);
      cf.ShowDialog (this);
    }



    private void extractActiveModelFeatureDataToolStripMenuItem_Click(object sender, EventArgs e)
    {
      SaveActiveTrainingModelFeatureData  
        satmd = new SaveActiveTrainingModelFeatureData (ActiveTrainingLibraries.Model1Name);
      satmd.ShowDialog (this);
/*

      TrainingModel2 tm = ActiveTrainingLibraries.Model1 ();
      if  (tm == null)
        tm = ActiveTrainingLibraries.Model2 ();

      if  (tm == null)
      {
        MessageBox.Show (this, "There is no Active Training Model selected.", "Extract Training Model Feature Data", MessageBoxButtons.OK);
        return;
      }

      PicesTrainingConfiguration  config = new PicesTrainingConfiguration (tm.ModelName, runLog);
      if  (!config.Valid ())
      {
        MessageBox.Show (this, "Invalid Taining Model[" + tm.ModelName + "]  can not retrieve feature data.", "Extract Training Model Feature Data", MessageBoxButtons.OK);
        return;
      }

      bool  changesMade = false;

      PicesFeatureVectorList featureData = config.LoadFeatureDataFromTrainingLibraries (changesMade);
      if  (featureData == null)
      {
        MessageBox.Show (this, "No Feature Data Loaded", "Extract Training Model Feature Data", MessageBoxButtons.OK);
        return;
      }

      String  featureDataFileName = OSservices.GetRootName (tm.ModelName) + ".data";
      FeatureFileNamePrompter  dialog = new FeatureFileNamePrompter (true);
      dialog.FileName = featureDataFileName;
      dialog.ShowDialog ();
      DialogResult  dr = dialog.Result;
      if  (dr == DialogResult.OK)
      {
        featureDataFileName = dialog.FullFileName;
        try 
        {
          featureData.Save (featureDataFileName, "", runLog);
        }
        catch  (Exception e2)  
        {
          MessageBox.Show (this, "Error trying to save Feature Data\n\n" + e2.ToString (), "");
          return;
        }
        MessageBox.Show (this, "Feature File[" + featureDataFileName + "]  Saved.", "Extract Training Model Feature Data");
      }
 */
    }  /* extractActiveModelFeatureDataToolStripMenuItem_Click */



    private void createTrainingModelFromDirectoryStructureToolStripMenuItem1_Click(object sender, EventArgs e)
    {
      TrainingModelCreateFromDirectory  tmcd = new TrainingModelCreateFromDirectory ();
      tmcd.ShowDialog (this);
      PopulateTrainingLibraryFields ();
    }

    private void SetupFilesForParameterTuning_Click(object sender, EventArgs e)
    {
      CreateFeatureSelectionDataSets  cfsda = new CreateFeatureSelectionDataSets (TrainingLibrary1.Text);
      cfsda.ShowDialog ();
    }

    private void SipperInterfaceDepthPlotToolStripMenuItem_Click (object sender, EventArgs e)
    {
      DepthPlotDisplay  dpd = new DepthPlotDisplay (this, dbConn, cruiseName, stationName, deploymentNum);
      dpd.Show ();
    }

    private void  reconcileImagesToDatabaseToolStripMenuItem_Click (object sender, EventArgs e)
    {
      ValidateAllFields ();
      ReconcileImagesToDataBase  ritdb = new ReconcileImagesToDataBase ();
      ritdb.ShowDialog (this);
    }

    private void ThumbnailDataGridView_CellContentClick (object sender, DataGridViewCellEventArgs e)
    {

    }

    private void tenFoldCrossValidationToolStripMenuItem_Click (object sender, EventArgs e)
    {
      String modelName = OSservices.GetRootName (ActiveTrainingLibraries.Model1Name);
      if  (String.IsNullOrEmpty (modelName))
      {
        MessageBox.Show (this, "There is not training model specified.", "10 Fold C/V");
        return;
      }

      DialogResult  dr = MessageBox.Show (this, "Run a 10 Fold Crosss Validation for '" + modelName + "' ?", "Cross Validation", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
        return;
        

      DateTime  dt = DateTime.Now;

      String  tempDirPath = PicesSipperVariables.PicesTempDirectory ();
      OSservices.CreateDirectoryPath (tempDirPath);

      String  reportDir = OSservices.AddSlash (PicesSipperVariables.PicesReportDir ()) + "CrossValiations";
      OSservices.CreateDirectoryPath (reportDir);

      String  reportFileName = OSservices.AddSlash (reportDir) + modelName + "_" + dt.ToString ("yyyyMMdd-hhmmss") + ".txt";
      String  programName = OSservices.AddSlash (PicesSipperVariables.PicesCmdLineProgDir ()) + "CrossValidation.exe";
      String  arguments = "-c " + modelName + "  -Folds 10  -R " + reportFileName + "  -BiasMatrix";
      
      Process proc = new Process();
      proc.StartInfo.WorkingDirectory = @"";
      proc.StartInfo.FileName = programName;
      proc.StartInfo.Arguments = arguments;
      //proc.StartInfo.FileName = batFileName;
      //proc.StartInfo.Arguments = arguments;
      proc.StartInfo.UseShellExecute = true;
      proc.StartInfo.RedirectStandardOutput = false;
      proc.StartInfo.RedirectStandardError = false;
      proc.Start();
    }



    private void SipperFile_DropDown (object sender, EventArgs e)
    {
      LoadSipperFiles ();
    }


    private void ClassKeyTypePredictedButton_Click (object sender, EventArgs e)
    {
      classKeyToUse = 'P';
      UpdateClassKeyToUseButtons ();
    }

    private void ClassKeyTypeValidatedButton_Click (object sender, EventArgs e)
    {
      classKeyToUse = 'V';
      UpdateClassKeyToUseButtons ();
    }

    private void instrumentDataSummaryByDepthProfileToolStripMenuItem_Click(object sender, EventArgs e)
    {
      ValidateAllFields ();
      InstrumentDataByDeploymentProfile  iddp 
        = new InstrumentDataByDeploymentProfile (cruiseName, stationName, deploymentNum, selectedClass);

      iddp.ShowDialog (this);
    }


    private void exportFeatureDataToolStripMenuItem_Click(object sender, EventArgs e)
    {
      ValidateAllFields ();
      SaveFeatureData sfd = new SaveFeatureData (dbConn, 
                                                 selecetedImageGroup,
                                                 cruiseName, stationName, deploymentNum, 
                                                 sipperFileName,  
                                                 selectedClass,
                                                 classKeyToUse,
                                                 probMin,  probMax,
                                                 sizeMin,  sizeMax,
                                                 depthMin, depthMax
                                                );
      sfd.ShowDialog ();
      return;
    }


    private void reExtractInstrumentDataToolStripMenuItem_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
      {
        UpdatesNotAllowedMsg ();
        return;
      }

      ValidateAllFields ();
      SipperInstrumentDataExtraction side 
        = new SipperInstrumentDataExtraction (dbConn, cruiseName, stationName, deploymentNum, sipperFileName);
      side.ShowDialog ();
    }


    private void Deployment_Validating (object sender, CancelEventArgs e)
    {
      Deployment_ValidateField ();
    }


    private void exitToolStripMenuItem_Click (object sender, EventArgs e)
    {
      DialogResult  dr = MessageBox.Show (this, "Exit PicesComander Yes/No?", "PicesComander", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        CancelAllThreadsInPreperationForExitingProgram ();
        ClosePicesCommanderWindow ();
      }
    }

    private void maintainExistingGrpupNamesToolStripMenuItem_Click (object sender, EventArgs e)
    {
      GroupMaintenance  gm = new GroupMaintenance ();
      gm.ShowDialog (this);
    }


    private  void  ClosePicesCommanderWindow ()
    {
      if  (dbConn != null)
      {
        //dbConn.Close ();
        dbConn = null;
      }

      Close ();
    }

    private void abunAbundanceAdjustmentParaeterDeterminationToolStripMenuItem_Click (object sender, EventArgs e)
    {
      String modelName = OSservices.GetRootName (ActiveTrainingLibraries.Model1Name);
      if  (String.IsNullOrEmpty (modelName))
      {
        MessageBox.Show (this, "There is not training model specified.", "Abundance Parameter Computation");
        return;
      }

      DialogResult  dr = MessageBox.Show (this, "Run Abundance Adjustment Parameter computation (10 Fold Crosss Validation) for '" + modelName + "' ?", "Cross Validation", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
        return;
        

      DateTime  dt = DateTime.Now;

      String  tempDirPath = PicesSipperVariables.PicesTempDirectory ();
      OSservices.CreateDirectoryPath (tempDirPath);

      String  reportDir = OSservices.AddSlash (PicesSipperVariables.PicesReportDir ()) + "AbundanceAdjustments";
      OSservices.CreateDirectoryPath (reportDir);

      String  reportFileName = OSservices.AddSlash (reportDir) + modelName + "_" + dt.ToString ("yyyyMMdd-hhmmss") + ".txt";
      String  programName = OSservices.AddSlash (PicesSipperVariables.PicesCmdLineProgDir ()) + "AbundanceCorrectionStatsBuilder.exe";
      String  arguments = "-c " + modelName + "  -Folds 10  -R " + reportFileName + "";
      
      Process proc = new Process();
      proc.StartInfo.WorkingDirectory = @"";
      proc.StartInfo.FileName = programName;
      proc.StartInfo.Arguments = arguments;
      //proc.StartInfo.FileName = batFileName;
      //proc.StartInfo.Arguments = arguments;
      proc.StartInfo.UseShellExecute = true;
      proc.StartInfo.RedirectStandardOutput = false;
      proc.StartInfo.RedirectStandardError = false;
      proc.Start();
    }



    private void assignNewGropNameToolStripMenuItem_Click (object sender, EventArgs e)
    {
      if  (!allowUpdates)
      {
        UpdatesNotAllowedMsg ();
        return;
      }

      if  (curSelImages == null)
      {
        MessageBox.Show (this, "Assign Group to Selected Images", "There are no images curently selected.", MessageBoxButtons.OK);
        return;
      }

      AssignSelectedImagesToGroup aig = new AssignSelectedImagesToGroup (curSelImages, Criteria.Text);
      aig.ShowDialog (this);
      LoadImageGroupChoices ();
    }


    private void DepthProfileOneChart_Click (object sender, EventArgs e)
    {
      ClassDepthProfileChart  pc 
        = new ClassDepthProfileChart (Cruise.Text, 
                                      Station.Text, 
                                      Deployment.Text,
                                      sizeMin,    sizeMax,
                                      probMin,    probMax,
                                      depthMin,   depthMax,
                                      classKeyToUse,
                                      allClasses, 
                                      activeClasses, 
                                      rootDir
                                     );
      pc.ShowDialog (this);
    }



    //  DepthProfileTwoCharts
    private void DepthPlotButton_Click (object sender, EventArgs e)
    {
      ChartDistribution cd = new ChartDistribution (Cruise.Text, 
                                                    Station.Text, 
                                                    Deployment.Text,
                                                    sizeMin,    sizeMax,
                                                    probMin,    probMax,
                                                    depthMin,   depthMax,
                                                    classKeyToUse,
                                                    Criteria.Text,
                                                    allClasses, 
                                                    activeClasses, 
                                                    rootDir
                                                   );
      cd.ShowDialog (this);

    }




    private void gPSChartsToolStripMenuItem_Click (object sender, EventArgs e)
    {
      ChartGPS  cGPS = new ChartGPS (Cruise.Text, Station.Text, Deployment.Text);
      cGPS.ShowDialog (this);
    }




    private void GPSChartButton_Click (object sender, EventArgs e)
    {
      ChartGPS  cGPS = new ChartGPS (Cruise.Text, Station.Text, Deployment.Text);
      cGPS.ShowDialog (this);
    }


    private  void  UpdatesNotAllowedMsg ()
    {
      PicesSipperVariables.UpdatesNotAllowed ("Pices Commander");
    }



    private void aboutToolStripMenuItem_Click (object sender, EventArgs e)
    {
      AboutBox  ab = new AboutBox ();
      ab.ShowDialog (this);
    }

    private void AbundanceByDeploymentButton_Click (object sender, EventArgs e)
    {
      ChartAbundanceByDeployment abdc = new ChartAbundanceByDeployment (Cruise.Text, 
                                                                        Station.Text, 
                                                                        Deployment.Text,
                                                                        thumbnailClass,
                                                                        sizeMin,    sizeMax,
                                                                        probMin,    probMax,
                                                                        depthMin,   depthMax,
                                                                        classKeyToUse,
                                                                        allClasses, 
                                                                        activeClasses, 
                                                                        rootDir
                                                                       );
      abdc.ShowDialog (this);    
    }

    private void horizontalProfileToolStripMenuItem_Click (object sender, EventArgs e)
    {
      //MessageBox.Show (this, "Not Functioning Yet", "Chart Horizontal Profile", MessageBoxButtons.OK);
      //return;

      ChartHorizontalProfile  chp 
        = new ChartHorizontalProfile (Cruise.Text, 
                                      Station.Text, 
                                      Deployment.Text,
                                      sizeMin,    sizeMax,
                                      probMin,    probMax,
                                      depthMin,   depthMax,
                                      classKeyToUse,
                                      allClasses, 
                                      activeClasses, 
                                      rootDir
                                     );
      chp.ShowDialog (this);
    }



    private void  AbundancePredValidatedButton_Click (object sender, EventArgs e)
    {
      if  (String.IsNullOrEmpty (Cruise.Text)  ||  String.IsNullOrEmpty (Station.Text))
      {
        MessageBox.Show (this, "A single deployment must be specified!", "Chart Abundance (Pred - Validated)", MessageBoxButtons.OK);
        return;
      }

      if  (String.IsNullOrEmpty (Deployment.Text))
      {
        PicesSipperDeploymentList deployments = dbConn.SipperDeploymentLoad (Cruise.Text, Station.Text);
        if  ((deployments == null)  ||  (deployments.Count != 1))
        {
          MessageBox.Show (this, "A single deployment must be specified!", "Chart Abundance (Pred - Validated)", MessageBoxButtons.OK);
          return;
        }
      }

      ChartAbundancePredictedAndValidated abdc 
        = new ChartAbundancePredictedAndValidated (Cruise.Text, 
                                                   Station.Text, 
                                                   Deployment.Text,
                                                   selecetedImageGroup,
                                                   thumbnailClass,
                                                   sizeMin,    sizeMax,
                                                   probMin,    probMax,
                                                   depthMin,   depthMax,
                                                   allClasses, 
                                                   activeClasses, 
                                                   rootDir
                                                  );
      abdc.ShowDialog (this);    
    }

  }
}
