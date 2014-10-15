using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Threading;
using System.Windows.Forms;

//***********************************************************************************************************
//* Developer: Kurt Kramer                                                                                  *
//* Date:      2009-07-11                                                                                   *
//*                                                                                                         *
//***********************************************************************************************************

using  PicesInterface;

namespace PicesCommander
{
  public partial class Harvester : Form
  {
    private  float   depthIncrement   = 1.0f;
    private  String  groupName        = "";
    private  int     howManyToHarvest = 5000;
    private  int     maxDepth         = 1000;  // We will max out at 1000 meters depth.  
    private  float   minimumDepth     = 0.5f;
    private  bool    weight           = true;

    private  PicesDataBase          mainWinConn = null;
    private  PicesRunLog            runLog = null;

    private  String                 cruise         = null;
    private  String                 station        = null;
    private  String                 deployment     = null;
    private  String                 sipperFileName = null;
    private  char                   classKeyToUse  = 'P';
    private  PicesClass             selClass       = null;

    private  int                    sizeMin        = 150;
    private  int                    sizeMax        = 0;

    PicesDataBaseImageList          harvestedImages = null;  

    private  String[]               sipperFileNames;


    // Harvesting Thread Related Variables.
    private  bool                   cancelHarvesting = false;
    private  bool                   harvestingStarted = false;  // Will be set to 'true' after the start button is pressed
                                                                // and allvalidations are passed.
    private  bool                   harvestingRunning   = false;
    private  bool                   harvestingCompleted = false;
    private Thread                  harvsterThread      = null;

    // 
    private bool                    validationErrorFound = false;

    private PicesMsgQueue           runLogMsgQueue = null;  // List of messages that can be added to 'RunLog'.

    public  bool                    CancelHarvesting  {get {return cancelHarvesting;}   set  {cancelHarvesting = value;}}
    public  String                  GroupNameSelected {get {return groupName;}}
    public  PicesDataBaseImageList  HarvestedImages   {get {return harvestedImages;}}
    public  bool                    HarvestingRunning {get {return harvestingRunning;}}

    String  infinityStr = ((char)8734).ToString ();

    public Harvester (String     _cruise,
                      String     _station,
                      String     _deployment,
                      String     _sipperFileName,
                      PicesClass _selClass,
                      char       _classKeyToUse,
                      int        _sizeMin,
                      int        _sizeMax
                     )
    {
      cruise         = (_cruise         == null) ? "" : _cruise;
      station        = (_station        == null) ? "" : _station;
      deployment     = (_deployment     == null) ? "" : _deployment;
      sipperFileName = (_sipperFileName == null) ? "" : _sipperFileName;
      classKeyToUse  = _classKeyToUse;
      selClass       = _selClass;

      sizeMin = _sizeMin;
      sizeMax = _sizeMax;

      runLogMsgQueue = new PicesMsgQueue ("Harvester");
      runLog       = new PicesRunLog (runLogMsgQueue);

      mainWinConn  = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      InitializeComponent();

      if  (String.IsNullOrEmpty (cruise))
      {
        DateTime d = DateTime.Now;
        groupName = "Created_" + d.Year.ToString ("0000") + "-" + d.Month.ToString ("00") + "-" + d.Day.ToString ("00") + "_" +
                                 d.Hour.ToString ("00")   + "-" + d.Minute.ToString ("00");
      }
      else
      {
        groupName = "Cruise-" + cruise;
        if  (!String.IsNullOrEmpty (station))
          groupName += "_Station-" + station;
        if  (!String.IsNullOrEmpty (deployment))
          groupName += "_Deployment-" + deployment;
        if  (!String.IsNullOrEmpty (sipperFileName))
          groupName += "_SipperFile-" + sipperFileName;
      }

      if  (selClass != null)
      {
        if (String.IsNullOrEmpty (groupName))
          groupName = selClass.Name;
        else
          groupName += "_Class-" + selClass.Name;
      }

      String  sizeMaxStr = sizeMax.ToString ();
      if  (sizeMax == 0)
        sizeMaxStr = infinityStr;

      Cruise.Text         = cruise;
      Station.Text        = station;
      Deployment.Text     = deployment;
      SipperFileName.Text = sipperFileName;
      MinimumDepth.Text   = minimumDepth.ToString ("##0.00");
      GroupName.Text      = groupName;
      SizeRange.Text      = sizeMin.ToString () + " - " + sizeMaxStr;

      if  (selClass != null)
        SelectedClass.Text = selClass.Name;

      UpdateCriteriaFields ();
    }



    private  void  UpdateCriteriaFields ()
    {
      if  (depthIncrement < 0.5f)  
        depthIncrement = 0.5f;
      else if  (depthIncrement > 100.0f)
        depthIncrement = 100.0f;

      Weight.Checked        = weight;
      DepthIncrement.Text   = depthIncrement.ToString ("##0.0") + " Meters";
      HowManyToHarvest.Text = howManyToHarvest.ToString ();
    }  /* UpdateCriteriaFields */




    private void  EnableControlsStartButtonNotPressed ()
    {
      Weight.Enabled = true;
      DepthIncrement.Enabled = true;
      HowManyToHarvest.Enabled = true;
      MinimumDepth.Enabled = true;
      StartButon.Enabled = true;
      CancelHarvester.Enabled = true;
    }  /* EnableControlsStartButtonPressed */

    


    private void  EnableControlsStartButtonPressed()
    {
      Weight.Enabled           = false;
      DepthIncrement.Enabled   = false;
      HowManyToHarvest.Enabled = false;
      StartButon.Enabled       = false;
      MinimumDepth.Enabled     = false;
      CancelHarvester.Enabled     = true;
    }  /* EnableControlsStartButtonPressed */



    private  void  ValidateDepthIncrement ()
    {
      if  (!weight)
      {
        DepthIncrement.Text = "";
        DepthIncrement.Enabled = false;
        return;
      }


      char[]  delimiters = {' ', '\n', '\r', '\t'};
      String[] fields = DepthIncrement.Text.Split (delimiters);
      if  (fields.Length < 1)
      {
        errorProvider1.SetError (DepthIncrement, "Need to provide increment in meters; > 0.5");
        validationErrorFound = true;
        return;
      }
      float  temp = PicesKKStr.StrToFloat (fields[0]);
      if  (temp < 0.5f)
      {
        errorProvider1.SetError (DepthIncrement, "Need to provide increment in meters; > 0.5");
        validationErrorFound = true;
        return;
      }

      if  (temp > 100.0f)
      {
        errorProvider1.SetError (DepthIncrement, "Maximum increment is 100.0 meters.");
        validationErrorFound = true;
        return;
      }

      errorProvider1.SetError (DepthIncrement, "");
      DepthIncrement.Text = temp.ToString ("##0.0") + " " + "Meters";

      depthIncrement = temp;
    }  /* ValidateDepthIncrement*/



    private  void  ValidateHowManyToHarvest ()
    {
      howManyToHarvest = PicesKKStr.StrToInt (HowManyToHarvest.Text);
      if  (howManyToHarvest < 10)
      {
        errorProvider1.SetError (HowManyToHarvest, "You must select at least 10 images to harvest.");
        validationErrorFound = true;
        return;
      }

      errorProvider1.SetError (HowManyToHarvest, "");
      HowManyToHarvest.Text = howManyToHarvest.ToString ();
    }  /* ValidateHowManyToHarvest */



    private  void  ValidateSipperFiles ()
    {
      if  (!String.IsNullOrEmpty (sipperFileName))
      {
        sipperFileNames = new String[1];
        sipperFileNames[0] = sipperFileName;
      }
      else
      {
        sipperFileNames =  mainWinConn.SipperFileGetList (Cruise.Text, Station.Text, Deployment.Text);
      }

      if  (sipperFileNames == null)
      {
        errorProvider1.SetError (this, "No SIPPER files located.");
        validationErrorFound = true;
      }
      else if  (sipperFileNames.Length < 1)
      {
        errorProvider1.SetError (this, "No SIPPER files located.");
        validationErrorFound = true;
      }
      else
      {
        errorProvider1.SetError (this, "");
      }

      //// Next 5 lines set up to help speed up debugging by reducing the number of images lioaded.
      //String[]  temp = new String[2];
      //temp[0] = sipperFileNames[1];
      //temp[1] = sipperFileNames[2];
      //sipperFileNames = temp;
    }  /* ValidateSipperFiles */



    private void ValidateMinimumDepth ()
    {
      errorProvider1.SetError (MinimumDepth, "");
      float temp = PicesKKStr.StrToFloat (MinimumDepth.Text);
      if  (temp < 0.0f)
      {
        errorProvider1.SetError (MinimumDepth, "Minimum Depth can not be lessthan 0.0");
        validationErrorFound = true;
        return;
      }

      if  (temp > 13000.0)
      {
        errorProvider1.SetError (MinimumDepth, "Minimum Depth[" + temp.ToString ("n") + "] is to Large, can not exceed 13,000 meters");
        validationErrorFound = true;
        return;
      }


      minimumDepth = temp;
      MinimumDepth.Text = minimumDepth.ToString ("##,##0.00");
    }



    private  void  ValidateGroupName ()
    {
      errorProvider1.SetError (GroupName, "");
      String   errorDesc = "";
      bool     valid = false;

      groupName = GroupName.Text;
      PicesDataBaseImageGroup.ValidName (ref groupName, ref errorDesc, ref valid);
      if  (!valid)
      {
        errorProvider1.SetError (GroupName, errorDesc);
        validationErrorFound = true;
      }
      GroupName.Text = groupName;
    }


    private  void  ValidateGroupNameDoesntExist ()
    {
      PicesDataBaseImageGroup  group = mainWinConn.ImageGroupLoad (groupName);
      if  (group != null)
      {
        DialogResult dr = MessageBox.Show 
                        (this, 
                         "Group[" + groupName + "] already exists" + "\n\n" +
                         "Do you want to yepleace it ?",
                         "Group Already Exists", 
                         MessageBoxButtons.YesNo
                        );

        if  (dr == DialogResult.No)
        {
          validationErrorFound = true;
          errorProvider1.SetError (GroupName, "Group already exists");
        }
      }
    }  /* ValidateGroupNameExists */



    private void ValidateAll ()
    {
      validationErrorFound = false;
      ValidateDepthIncrement ();
      ValidateHowManyToHarvest ();
      ValidateSipperFiles ();
      ValidateMinimumDepth ();
      ValidateGroupName ();
      if  (!validationErrorFound)
        ValidateGroupNameDoesntExist ();
    }


    private  void  RunLogAddMsg (String  msg)
    {
      runLogMsgQueue.AddMsg  (msg);
    }  /* RunLogAddMsg */



    private  void  RunLogMsgQueueFlush ()
    {
      String  msg = runLogMsgQueue.GetNextMsg ();
      while  (msg != null)
      {
        RunLog.AppendText (msg);
        msg = runLogMsgQueue.GetNextMsg ();
      }
    }  /* RunLogMsgQueueFlush */



    private  void  StartTheHarvestingProcedure ()
    {
      cancelHarvesting = false;
      harvsterThread = new Thread (new ThreadStart (HarvestImages));
      harvsterThread.Start ();
      harvesterTimer.Enabled = true;
      EnableControlsStartButtonPressed ();
    }  /* StartTheHarvestingProcedure */



    private  class   ImageEntry
    {
      public  uint   imageId;
      public  float  depth;

      public  ImageEntry (uint   _imageId,
                          float  _depth
                         )
      {
        imageId = _imageId;
        depth   = _depth;
      }
    }  /* ImageEntry */




    private  void  HarvestImages ()
    {
      PicesDataBase.ThreadInit ();
      harvestingRunning   = true;
      harvestingCompleted = false;

      PicesDataBase  threadConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      List<ImageEntry>    allImages        = null;
      List<ImageEntry>[]  imagesByDepth    = null;
      double[]            depthTimeProfile = null;
      double[]            fracPartByDepth  = null;
      List<ImageEntry>    randDistByDepth  = null;
      
      //if  (!cbOilDeployments.Checked)
        allImages = LoadAllImages (threadConn);
      //else
      //  allImages = LoadOilDeploymentImages();

      if  (!cancelHarvesting)
      {
        if  (weight)
        {
          depthTimeProfile = LoadDepthTimeProfile (threadConn);
          if  (!cancelHarvesting)
          {
            imagesByDepth   = DeriveImagesByDepth     (allImages,        depthTimeProfile.Length);
            fracPartByDepth = DeriveFracPartByDepth   (depthTimeProfile, imagesByDepth);

            randDistByDepth = DeriveRandomDistByDepth (imagesByDepth,    fracPartByDepth);
            harvestedImages = GetDatabaseImageRecords (threadConn, randDistByDepth);
            UpdateImageGroupTables (threadConn, harvestedImages);

            RunLogAddMsg  ("\n" + "Harvesting Completed Sucessfully." + "\n");
          }
        }
        else
        {
          harvestedImages = DeriveRandomDistribution (threadConn, allImages);
          if  (!cancelHarvesting)
             UpdateImageGroupTables (threadConn, harvestedImages);
        }

        if  (!cancelHarvesting)
          harvestingCompleted = true;
      }

      threadConn.Close ();
      threadConn = null;
      GC.Collect ();

      PicesDataBase.ThreadEnd ();
      harvestingRunning = false;
    }  /* HarvestImages */




    private  void  CancelHarvestingProcess ()
    {
      cancelHarvesting = true;
      int  loopCount = 0;

      while  (harvestingRunning  && (loopCount < 200)  &&  harvsterThread.IsAlive)
      {
        Thread.Sleep (100);
        RunLogMsgQueueFlush ();
        loopCount++;
      }

      if  ((harvsterThread != null)  &&  harvsterThread.IsAlive)
        harvsterThread.Abort ();
    }  /* CancelHarvestingProcess */



    
    private List<ImageEntry>  LoadAllImages (PicesDataBase  threadConn)
    {
      RunLogAddMsg ("Loading Source Images\n");

      List<ImageEntry>  allImages = new List<ImageEntry> ();

      foreach  (String sipperFileName in sipperFileNames)
      {
        if  (cancelHarvesting)
          break;

        RunLogAddMsg ("Loading Sipper File[" + sipperFileName + "]" + "\n");

        PicesDataBaseImageList  images = threadConn.ImagesQuery (null,              // ImageGroup
                                                                 sipperFileName, 
                                                                 selClass,          // 
                                                                 classKeyToUse,     //
                                                                 0.0f, 1.0f,        //  Prob  Min and Max
                                                                 sizeMin,  sizeMax, //  Size  Min and Max
                                                                 0,    0,           //  Depth Min and Max
                                                                 0,                 //  restartImageFileName
                                                                 -1,                //  limit to load  -1 = Load All
                                                                 false              //  false = Don't load thumbnail
                                                                );

        if  (images == null)
        {
          RunLogAddMsg ("Loading Sipper File[" + sipperFileName + "]   No images found in database." + "\n");
        }
        else
        {
          foreach  (PicesDataBaseImage  i in images)
          {
            float  depth = i.Depth;
            if  (depth >= minimumDepth)
              allImages.Add (new ImageEntry (i.ImageId, depth));
          }
          RunLogAddMsg ("Loading Sipper File[" + sipperFileName + "]   [" + images.Count.ToString ("##,###,##0") + "] Loaded" + "\n");
        }
      }

      return  allImages;
    }  /* LoadAllImages */



    /*
     * 2013-03-14  Kurt Kramer,  as per email from A. Remsen
     * Commenting out this procedure because the database only consists of Oil cruises
     * It was originally created when there were obnly a few deployments that had images 
     * identified as oil.
     * 
     * 
    private List<ImageEntry>  LoadOilDeploymentImages ()
    {
      RunLogAddMsg ("Loading Source Images\n");

      List<ImageEntry>  allImages = new List<ImageEntry> ();

      {
        List<String[]> sipperFileNamesList = new List<String[]>();
        sipperFileNamesList.Add(dbConn.SipperFileGetList ("SMP751001", "034", ""));
        sipperFileNamesList.Add(dbConn.SipperFileGetList ("SMP751001", "035", ""));
        sipperFileNamesList.Add(dbConn.SipperFileGetList ("SMP751001", "037", ""));

        int  totalNumSipperFiles = sipperFileNamesList[0].Length + sipperFileNamesList[1].Length + sipperFileNamesList[2].Length;
        sipperFileNames = new String[totalNumSipperFiles];
        int  sipperFileNum = 0;
        foreach  (String[]  names in sipperFileNamesList)
        {
          foreach  (String  sipperFileName in names)
          {
            sipperFileNames[sipperFileNum] = sipperFileName;
            sipperFileNum++;
          }
        }
      }

      foreach  (String sipperFileName in sipperFileNames)
      {
        if  (cancelHarvesting)
          break;

        RunLogAddMsg ("Loading Sipper File[" + sipperFileName + "]" + "\n");  

        PicesDataBaseImageList  images = dbConn.ImagesQuery (null,               // ImageGroup
                                                             sipperFileName, 
                                                             selClass,           //
                                                             classKeyToUse,      //
                                                             0.0f, 1.0f,         //  Prob  Min and Max
                                                             sizeMin,  sizeMax,  //  Size  Min and Max
                                                             0,    0,            //  Depth Min and Max
                                                             0,                  //  restartImageFileName
                                                             -1,                 //  limit to load  -1 = Load All
                                                             false               //  false = Don't load thumbnail
                                                            );

        if  (images == null)
        {
          RunLogAddMsg ("Loading Sipper File[" + sipperFileName + "]   No images found in database." + "\n");
        }
        else
        {
          foreach  (PicesDataBaseImage  i in images)
          {
            float  depth = i.Depth;
            if  (depth >= minimumDepth)
              allImages.Add (new ImageEntry (i.ImageId, depth));
          }
          RunLogAddMsg ("Loading Sipper File[" + sipperFileName + "]   [" + images.Count.ToString ("##,###,##0") + "] Loaded" + "\n");
        }
      }
        
      return  allImages;
    }  /* LoadOilDeploymentImages * /
    */



    //* Will build an array that will represent the times spent at each depth increment.
    //* This will be done by scanning the InstrumentData for all the sipper files involved
    //* and counting the number of scan lines at each depth.
    private double[]   LoadDepthTimeProfile (PicesDataBase  connThread)
    {
      RunLogAddMsg ("Extracting Depth/Time profile" + "\n");

      int  numIncrements =  (int)Math.Floor ((float)maxDepth / (float)depthIncrement);

      double[]  depthTimeProfile = new double[numIncrements];


      foreach  (String sipperFileName in sipperFileNames)
      {
        if  (cancelHarvesting)
          break;

        RunLogAddMsg ("Loading InstrumentData   Sipper File[" + sipperFileName + "]" + "\n");

        PicesInstrumentDataList  data = connThread.InstrumentDataLoad (sipperFileName);
        if  ((data == null)  ||  (data.Count < 1))
        {
          RunLogAddMsg ("Loading InstrumentData   Sipper File[" + sipperFileName + "]  NO DATA LOADED" + "\n");
        }
        else
        {
          RunLogAddMsg ("Loading InstrumentData   Sipper File[" + sipperFileName + "]  [" + data.Count.ToString ("###,##0") + "] Enries loaded" + "\n");
          uint  lastScanLine = data[0].ScanLine;
          for  (int idx = 1;  idx < data.Count;  idx++)
          {
            PicesInstrumentData id = data[idx];
            uint  scanLine = id.ScanLine;
            uint  deltaScanLines = scanLine - lastScanLine;

            int  depthIdx = (int)Math.Floor (id.Depth / depthIncrement);
            if  (depthIdx < 0)
              depthIdx = 0;
            else if  (depthIdx >= depthTimeProfile.Length)
              depthIdx = depthTimeProfile.Length - 1;

            depthTimeProfile[depthIdx] += (double)deltaScanLines;

            lastScanLine = scanLine;
          }
        }
      }

      return  depthTimeProfile;
    }  /* LoadDepthTimeProfile */


    /// <summary>
    /// Bins all 'allImages' by depth increments.
    /// </summary>
    /// <param name="allImages">Images that are to be bined.</param>
    /// <param name="numIncrements">Total number of bins.</param>
    /// <returns>An array of lists where each element of the array represents the images in that depth bin.</returns>
    private  List<ImageEntry>[]   DeriveImagesByDepth (List<ImageEntry> allImages, 
                                                       int              numIncrements
                                                      )
    {
      List<ImageEntry>[] imagesByDepth = new List<ImageEntry>[numIncrements];

      int  depthIdx = 0;
      for  (depthIdx = 0;  depthIdx < numIncrements;  depthIdx++)
        imagesByDepth[depthIdx] = new List<ImageEntry> ();

      foreach  (ImageEntry ie in allImages)
      {
        depthIdx = (int)Math.Floor (ie.depth / depthIncrement);
        if  (depthIdx < 0)
          depthIdx = 0;
        else if  (depthIdx >= imagesByDepth.Length)
          depthIdx = imagesByDepth.Length - 1;

        imagesByDepth[depthIdx].Add (ie);
      }

      return  imagesByDepth;
    }  /* DeriveImagesByDepth */



    private  double[] DeriveFracPartByDepth (double[]            depthTimeProfile, 
                                             List<ImageEntry>[]  imagesByDepth
                                            )
    {
      int  x = 0;

      int  len = Math.Min (depthTimeProfile.Length, imagesByDepth.Length);

      double[]  densityByDepth  = new double[len];
      double  totalDensity = 0.0;

      for  (x = 0;  x < len;  x++)
      {
        if  (depthTimeProfile[x] > 0.0)
          densityByDepth[x] = (double)(imagesByDepth[x].Count) / depthTimeProfile[x];
        else
          densityByDepth[x] = 0.0;

        totalDensity += densityByDepth[x];
      }

      double[]  fracPartByDepth = new double[len];
      for  (x = 0;  x < len;  x++)
      {
        if  (totalDensity > 0.0)
          fracPartByDepth[x] = densityByDepth[x] / totalDensity;
        else
          fracPartByDepth[x] = 0.0;
      }

      return  fracPartByDepth;
    }  /* DeriveFracPartByDepth */



    private  PicesDataBaseImageList  DeriveRandomDistribution (PicesDataBase     connThread,
                                                               List<ImageEntry>  images
                                                              )
    {
      RunLogAddMsg ("Extracting Images Randomly." + "\n");

      PicesDataBaseImageList  randDist = new PicesDataBaseImageList ();

      DateTime  d = DateTime.Now;
      int  miliSecsInDay = d.Millisecond + d.Second * 1000 + d.Minute * 60000  +  d.Hour * 360000;
      Random  randomGenerator  = new Random (miliSecsInDay);

      RandomSortImageEntryList (images, randomGenerator);

      foreach (ImageEntry ie in images)
      {
        PicesDataBaseImage  dbi = connThread.ImageLoad (ie.imageId);
        if  (dbi == null)
        {
          RunLogAddMsg ("DeriveRandomDistribution   ***ERROR***   Could not get ImageId[" + ie.imageId + "]" + "\n");
        }
        else
        {
          randDist.Add (dbi);
          if  ((randDist.Count % 100) == 0)
            RunLogAddMsg ("Images Extracted " + randDist.Count + " of " + images.Count + "\n");
        }

        if  (randDist.Count >= howManyToHarvest)
          break;
      }

      return  randDist;
    }  /* DeriveRandomDistribution */



    private   List<ImageEntry>  DeriveRandomDistByDepth (List<ImageEntry>[]  imagesByDepth,    
                                                         double[]            fracPartByDepth
                                                        )
    {
      RunLogAddMsg ("Extracting Images Randomly by Depth" + "\n");

      List<ImageEntry>  randDistByDepth = new List<ImageEntry> ();

      int  size = Math.Min (imagesByDepth.Length, fracPartByDepth.Length);

      DateTime  d = DateTime.Now;
      int  miliSecsInDay = d.Millisecond + d.Second * 1000 + d.Minute * 60000  +  d.Hour * 360000;
      Random  randomGenerator  = new Random (miliSecsInDay);

      for  (int  depthIdx = 0;  depthIdx < size;  depthIdx++)
      {
        List<ImageEntry>  imagesThisDepth = imagesByDepth[depthIdx];

        int  howManyThisDepth = (int)((double)howManyToHarvest * fracPartByDepth[depthIdx] + 0.5);
        if  (howManyThisDepth > imagesThisDepth.Count)
          howManyThisDepth = imagesThisDepth.Count;

        RandomSortImageEntryList (imagesThisDepth, randomGenerator);

        for  (int x = 0;  x < howManyThisDepth;  x++)
          randDistByDepth.Add (imagesThisDepth[x]);
      }

      return  randDistByDepth;
    }  /* DeriveRandomDistByDepth */




    private  PicesDataBaseImageList  GetDatabaseImageRecords (PicesDataBase     threadConn,
                                                              List<ImageEntry>  images
                                                             )
    {
      RunLogAddMsg ("Extracting Images from the database." + "\n");
      PicesDataBaseImageList  dataBaseImages = new PicesDataBaseImageList ();

      foreach  (ImageEntry  ie  in  images)
      {
        if  (cancelHarvesting)
          break;

        PicesDataBaseImage  dbi = threadConn.ImageLoad (ie.imageId);
        if  (dbi == null)
        {
          RunLogAddMsg ("GetDatabaseImageRecords   ***ERROR***   Could not get ImageId[" + ie.imageId + "]" + "\n");
        }
        else
        {
          dataBaseImages.Add (dbi);
          if  ((dataBaseImages.Count % 100) == 0)
            RunLogAddMsg ("Images Extracted " + dataBaseImages.Count + " of " + images.Count + "\n");
        }
      }

      return  dataBaseImages;
    }  /* GetDatabaseImageRecords */



    private  void  UpdateImageGroupTables (PicesDataBase           threadConn,
                                           PicesDataBaseImageList  harvestedImages
                                          )
    {
      RunLogAddMsg ("Create Image Group Entries" + "\n");
      PicesDataBaseImageGroup  existingGroup = threadConn.ImageGroupLoad (groupName);
      if  (existingGroup != null)
      {
        // Since the group already exists we will have to delete it.
        threadConn.ImageGroupDelete (existingGroup.ImageGroupId);
      }

      String  description = "Created by Harvestor" + "\n" +
                            "DateTime"  + "\t" + DateTime.Now.ToLongDateString () + "\n" +
                            "NumImages" + "\t" + harvestedImages.Count.ToString ("#,###,##0") + "\n";

      PicesDataBaseImageGroup  group = new PicesDataBaseImageGroup (-1, groupName, description, (uint)harvestedImages.Count);
      threadConn.ImageGroupInsert (group);

      int  idx = 0;
      while  (idx < harvestedImages.Count)
      {
        PicesDataBaseImageList  updateGroup = new PicesDataBaseImageList ();

        while  ((idx < harvestedImages.Count)  &&  (updateGroup.Count < 50))
        {
          updateGroup.Add (harvestedImages[idx]);
          idx++;
        }

        threadConn.ImageGroupEntriesInsert (group.ImageGroupId,  updateGroup);
        RunLogAddMsg ("Added To ImageGroup[" + idx + "]  of  [" + harvestedImages.Count + "]" + "\n");
      }

      if  (threadConn.Valid ())
        RunLogAddMsg ("Image Group Tables Updated" + "\n");
      else
        RunLogAddMsg ("\n" + "\n" +
                      "Image Group Tables Update FAILED   ***ERROR***" + "\n"
                     );
    }  /* UpdateImageGroupTables*/



    private  void  RandomSortImageEntryList (List<ImageEntry>  images,
                                             Random            randomGenerator
                                            )
    {
      int  x;
      int  y;

      int  count = images.Count;

      for  (x = 0;  x < images.Count;  x++)
      {
        y = randomGenerator.Next (count);
        ImageEntry  ie = images[x];
        images[x] = images[y];
        images[y] = ie;
      }
    }  /* RandomSortImageEntryList */



    private  void  DepthIncrement_SelectedIndexChanged (object sender, EventArgs e)
    {
      DepthIncrement.Text = DepthIncrement.SelectedItem.ToString ();
      ValidateDepthIncrement ();
    }


    private  void  Weight_CheckedChanged(object sender, EventArgs e)
    {
      if  (Weight.Checked)
      {
        weight = true;
        DepthIncrement.Enabled = true;
      }
      else
      {
        weight = false;
        DepthIncrement.Enabled = false;
      }
    }



    private void StartButon_Click (object sender, EventArgs e)
    {
      if  (harvestingStarted)
      {
        // This should not have been able to have happened.
        EnableControlsStartButtonPressed ();
        return;
      }

      EnableControlsStartButtonPressed ();

      ValidateAll ();
      if  (validationErrorFound)
      {
        EnableControlsStartButtonNotPressed ();
        return;
      }

      
      DialogResult  dr = MessageBox.Show (this, "Start Harvesting Images ?", "Harvester", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.No)
      {
        EnableControlsStartButtonNotPressed ();
        return;
      }

      StartTheHarvestingProcedure ();
    }



    private void harvesterTimer_Tick(object sender, EventArgs e)
    {
      RunLogMsgQueueFlush ();
      if  (harvestingCompleted)
      {
        harvesterTimer.Enabled = false;
        MessageBox.Show (this, "Harvesting has Completed.");
        //CloseMySQL ();
        Close ();
      }
    }



    private void CancelButton_Click(object sender, EventArgs e)
    {
      CancelHarvester.Enabled = false;
      if  (!harvestingRunning)
      {
        //CloseMySQL ();
        this.Close ();
      }

      DialogResult dr = MessageBox.Show (this, "Cancel Harvesting", "Harvesting", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        CancelHarvestingProcess ();
        //CloseMySQL ();
        this.Close ();
      }

      CancelHarvester.Enabled = true;
    }



    private void MinimumDepth_Leave(object sender, EventArgs e)
    {
      ValidateMinimumDepth ();
    }



    private void HowManyToHarvest_Leave(object sender, EventArgs e)
    {
      ValidateHowManyToHarvest ();
    }

    private void Harvester_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (harvestingRunning)
      {
        MessageBox.Show (this, "Harvesting is still running; need to cancel.", "Harvesting");
        e.Cancel = true;
        return;
      }

      if  (mainWinConn != null)
      {
        mainWinConn.Close ();
        mainWinConn = null;
        GC.Collect ();
      }

      return;
    }
  }
 }
