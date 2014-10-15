using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using  PicesInterface;

namespace PicesCommander
{
  public partial class SavePlanktonData : Form
  {
    String                   cruise         = "";
    String                   station        = "";
    String                   deployment     = "";
    String                   sipperFileName = "";
    PicesDataBaseImageGroup  group          = null;

    PicesClass               mlClass = null;
    char                     classKeyToUse = 'P';
    float                    probMin    = 0.0f;
    float                    probMax    = 1.0f;
    int                      sizeMin    = 0;
    int                      sizeMax    = 0;
    float                    depthMin   = 0;
    float                    depthMax   = 0;

    String                   dataFileName  = "";

    int                      includeSampleImagesNumPerClass = 0;

    PicesRunLog    runLog = null;
    PicesDataBase  dbConn = null;

    bool    errorsFound = false;

    String  infinityStr = ((char)8734).ToString ();

    // 
    Thread  backGroundThread           = null;
    bool    backGroundThreadRunning    = false;
    bool    backgroundThreadCompleted  = false;
    bool    cancelRequested            = false;
    uint    imageCount                 = 0;
    uint    imagesLoaded               = 0;
    uint    imagesWritten              = 0;

    PicesDataBaseImageList  planktonData = null;  // Feature Vectors that are going to be written to disk
    
    PicesFeatureFileIO      driver      = null;  // The Driver that will be used to write the Feature Data to disk.
                                                 // The 'BackGroundTimer' will pole this variable to
                                                 // keep track of number of records written to disk.
    
    String  backGroundStatus           = "";

    PlanktonClassStatisticsLoader  statsLoader = null;  // To determine total number of images that will be saved.

    
    public SavePlanktonData (PicesDataBase           _dbConn,
                             PicesDataBaseImageGroup _group,
                             String                  _cruise,
                             String                  _station,
                             String                  _deployment,
                             String                  _sipperFileName,
                             PicesClass              _mlClass,
                             char                    _classKeyToUse,
                             float                   _probMin,
                             float                   _probMax,
                             int                     _sizeMin,
                             int                     _sizeMax,
                             float                   _depthMin,
                             float                   _depthMax
                            )
    {
      dbConn         = _dbConn;
      group          = _group;
      cruise         = (_cruise         == null) ? "" : _cruise;
      station        = (_station        == null) ? "" : _station;
      deployment     = (_deployment     == null) ? "" : _deployment;
      sipperFileName = (_sipperFileName == null) ? "" : _sipperFileName;
      mlClass     = _mlClass;
      classKeyToUse  = _classKeyToUse;

      probMin  = _probMin;
      probMax  = _probMax;
      sizeMin  = _sizeMin;
      sizeMax  = _sizeMax;
      depthMin = _depthMin;
      depthMax = _depthMax;

      runLog = new PicesRunLog ();
      InitializeComponent ();
    }


    private void SaveFeatureData_Load(object sender, EventArgs e)
    {
      CruiseName.Text    = cruise;
      StationName.Text   = station;
      DeploymentNum.Text = deployment;
      SipperFile.Text    = sipperFileName;
      if  (group != null)
        GroupName.Text = group.Name;

      if  (mlClass != null)
        ClassName.Text = mlClass.Name;

      ProbMin.Text  = (100.0f * probMin).ToString ("##0.0") + "%";
      ProbMax.Text  = (100.0f * probMax).ToString ("##0.0") + "%";
      SizeMin.Text = sizeMin.ToString ("#,###,##0");
      if  (sizeMax <= 0)
        SizeMax.Text = infinityStr;
      else
        SizeMax.Text = sizeMax.ToString ("#,###,##0");

      DepthMin.Text = depthMin.ToString ("##,##0.00");
      if  (depthMax <= 0.0)
        DepthMax.Text = infinityStr;
      else
        DepthMax.Text = depthMax.ToString ("##,##0.00");

      dataFileName = OSservices.AddSlash (PicesSipperVariables.PicesReportDir ()) + "PlanktonDataFiles";
      dataFileName = OSservices.AddSlash (dataFileName);
      if  (group != null)
      {
        dataFileName += group.Name;
      }
      else
      {
        if  (!String.IsNullOrEmpty (cruise))
        {
          dataFileName += cruise;
          if  (!String.IsNullOrEmpty (station))
          {
            dataFileName += "-" + station;
            if  (!String.IsNullOrEmpty (deployment))
            {
              dataFileName += "-" + deployment;
              if  (!String.IsNullOrEmpty (sipperFileName))
                dataFileName += "-" + sipperFileName;
            }
          }
        }
      }

      if  (mlClass != null)
      {
        if  (!String.IsNullOrEmpty (dataFileName))
          dataFileName += "-";
        dataFileName += mlClass.Name;
      }

      dataFileName += ".data";

      FeatureFileName.Text = dataFileName;

      IncludeSampleImagesNumPerClass.Text = includeSampleImagesNumPerClass.ToString ();

      StartLoadingStatusOnTotalImageCount ();
    }  /* SaveImagesToDisk2_Load */ 


    
    private  void  StartLoadingStatusOnTotalImageCount ()
    {
       statsLoader = new PlanktonClassStatisticsLoader
           (group, 
            cruise, station, deployment, 
            sipperFileName, 
            mlClass,
            classKeyToUse,
            probMin,  probMax,
            sizeMin,  sizeMax,
            depthMin, depthMax
           );
      StatsTimer.Enabled = true;

      // Don't want to start saving images until we are done loading stats.
      StartButton.Enabled = false;

    }  /* StartLoadingStatusOnTotalImageCount */

    
    
    private  void  ValidateDataFileName ()
    {
      errorProvider1.SetError (FeatureFileName, "");

      if  (String.IsNullOrEmpty (FeatureFileName.Text))
      {
        errorsFound = true;
        errorProvider1.SetError (FeatureFileName, "Must provide a Data File Name.");
        return;
      }

      dataFileName = FeatureFileName.Text;

      bool  fileExistsAlready = true;
      try  {fileExistsAlready = File.Exists (dataFileName);}
      catch  (Exception)  {fileExistsAlready = false;}

      if  (fileExistsAlready)
      {
        errorsFound = true;
        errorProvider1.SetError (FeatureFileName, "File already Exists.");
        return;
      }

      String  pathName   = OSservices.GetPathPartOfFile (dataFileName);
      String  rootAndExt = OSservices.GetRootNameWithExtension (dataFileName);

      if  (String.IsNullOrEmpty (rootAndExt))
      {
        errorsFound = true;
        errorProvider1.SetError (FeatureFileName, "Missing File Name.");
        return;
      }

    }  /* ValidateDestinationDirectory */



    private  void  ValidateIncludeSampleImagesNumPerClass ()
    {
      errorProvider1.SetError (IncludeSampleImagesNumPerClass, "");
      if (IncludeSampleImages.Checked)
      {
        includeSampleImagesNumPerClass = PicesKKStr.StrToInt (IncludeSampleImagesNumPerClass.Text);
        if  (includeSampleImagesNumPerClass < 1)
        {
          errorsFound = true;
          errorProvider1.SetError (IncludeSampleImagesNumPerClass, "Must be greater than zero.");
          return;
        }
      }
    }


    private  void  ValidateAll ()
    {
      errorsFound = false;
      ValidateDataFileName ();
      ValidateIncludeSampleImagesNumPerClass ();
    }


    private  void  StartBackGroundThread ()
    {
      SaveProgress.Maximum = (int)imageCount;
      SaveProgress.Value   = 0;
      backGroundThread = new Thread (new ThreadStart (BackGroundThread));
      backGroundThread.Start ();
      Thread.Sleep (10);
      backGroundTimer.Enabled = true;
    }


    private  void  BackGroundThread ()
    {
      PicesDataBase.ThreadInit ();
      backGroundThreadRunning   = true;
      imagesLoaded              = 0;

      if  (group != null)
        planktonData = RetrievePlanktonDataByGroup ();
      else
        planktonData = RetrievePlanktonDataBySipperFiles ();
      
      if  (!cancelRequested)
      {
        backGroundStatus = "Saving Data File";
        String  pathName = OSservices.GetPathPartOfFile (dataFileName);
        if  (!String.IsNullOrEmpty (pathName))
          OSservices.CreateDirectoryPath (pathName);

        if  (IncludeSampleImages.Checked)
          SaveSampleImages (planktonData);
       
        SavePlanktonDataFile (planktonData);
      }

      PicesDataBase.ThreadEnd ();
      backgroundThreadCompleted = true;
      backGroundThreadRunning   = false;
    }  /* BackGroundThread */


    private  void  WriteValue (StreamWriter sw, String  fieldName,  String value)
    {
      if  (!String.IsNullOrEmpty (value))
        sw.WriteLine (fieldName + "\t" + value);
    }


    private  void  SavePlanktonDataFile (PicesDataBaseImageList  planktonData)
    {
      String  errMsg = "";
      StreamWriter sw = null;
      try {sw = new StreamWriter (dataFileName);}
      catch  (Exception e1)
      {
        errMsg = e1.ToString ();
      }
      if  (sw == null)
      {
        errMsg = "Could not open Data File[" + dataFileName + "]\n" + errMsg;
        return;
      }

      sw.WriteLine ("//This file was generated by the PICESComander application.");
      sw.WriteLine ();
      sw.WriteLine ("DateTime"    + "\t" + DateTime.Now.ToString ("yyyy/MM/dd - hh:mm:ss"));
      sw.WriteLine ("DataBase"    + "\t" + dbConn.ServerDescription ());
      sw.WriteLine ("MachineName" + "\t" + System.Environment.MachineName);
      sw.WriteLine ();
      sw.WriteLine ("//Selection Criteria");
      
      WriteValue (sw, "GroupName",     GroupName.Text);
      WriteValue (sw, "CruiseName",    CruiseName.Text);
      WriteValue (sw, "StationName",   StationName.Text);
      WriteValue (sw, "DeploymentNum", DeploymentNum.Text);
      WriteValue (sw, "SipperFile",    SipperFile.Text);

      if  (mlClass != null)
        WriteValue (sw, "MLClass", mlClass.Name);

      sw.WriteLine ("Probability Min-Max" + "\t" + (100.0f * probMin).ToString ("##0.00") + "\t" + (100.0f * probMax).ToString ("##0.00"));

      if  ((depthMin > 0.0f)  ||  (depthMax > 0.0f))
         sw.WriteLine ("Depth"  + "\t" + depthMin.ToString ("###0.00") + "\t" + depthMax.ToString ("###0.00"));

      if  ((sizeMin > 0)  ||  (sizeMax > 0))
         sw.WriteLine ("Size"   + "\t" + sizeMin.ToString ("#####0") + "\t" + sizeMax.ToString ("######0"));

      sw.WriteLine ("ClassKeyToUse" + "\t" + classKeyToUse.ToString ());

      sw.WriteLine ();
      sw.WriteLine ("Count" + "\t" + planktonData.Count);
      sw.WriteLine ();
      sw.WriteLine ("//Following specifies column headers.");
      sw.Write ("ImageFileName");  sw.Write ("\t");
      sw.Write ("Class1Name");     sw.Write ("\t");
      sw.Write ("Class1Prob");     sw.Write ("\t");
      sw.Write ("Class2Name");     sw.Write ("\t");
      sw.Write ("Class2Prob");     sw.Write ("\t");
      sw.Write ("Depth(M)");       sw.Write ("\t");
      sw.Write ("PixelCount");     sw.Write ("\t");
      sw.Write ("Height");         sw.Write ("\t");
      sw.Write ("Width");          sw.Write ("\t");
      sw.Write ("TopLeftRow");     sw.Write ("\t");
      sw.Write ("TopLeftCol");     sw.Write ("\t");
      sw.Write ("CtdDate");        sw.Write ("\t");
      sw.Write ("Latitude");       sw.Write ("\t");
      sw.Write ("Longitude");      sw.Write ("\t");
      sw.Write ("Temperature(C)");
      sw.WriteLine ();

      planktonData.Sort (PicesDataBaseImageList.SortOrderType.soSpatial, false);

      PicesInstrumentData id = null;
      uint  nextScanLineThreshold = 0;
      String  lastSipperFileName = "";

      foreach (PicesDataBaseImage i in planktonData)
      {
        if  ((i.TopLeftRow >= nextScanLineThreshold)  ||  
             (i.SipperFileName != lastSipperFileName)
            )
        {
          uint  scanLineCleared = 0;
          uint  scanLine = i.TopLeftRow;
          scanLine = 4096 * (uint)(scanLine / 4096);

          id = dbConn.InstrumentDataGetByScanLine (i.SipperFileName, scanLine);
          lastSipperFileName = i.SipperFileName;
          if  (id == null)
          {
            scanLineCleared = scanLine;
          }
          else
          {
            scanLineCleared = id.ScanLine;
            if  (scanLineCleared > scanLine)
            {
              if  ((scanLineCleared - scanLine) > 4096)
                id = null;
            }
          }
          nextScanLineThreshold = scanLineCleared + 4096;
        }

        sw.Write (i.ImageFileName);              sw.Write ("\t");
        sw.Write (i.Class1Name);                 sw.Write ("\t");
        sw.Write (i.Class1Prob.ToString ("p"));  sw.Write ("\t");
        sw.Write (i.Class2Name);                 sw.Write ("\t");
        sw.Write (i.Class2Prob.ToString ("p"));  sw.Write ("\t");
        sw.Write (i.Depth);                      sw.Write ("\t");
        sw.Write (i.PixelCount);                 sw.Write ("\t");
        sw.Write (i.Height);                     sw.Write ("\t");
        sw.Write (i.Width);                      sw.Write ("\t");
        sw.Write (i.TopLeftRow);                 sw.Write ("\t");
        sw.Write (i.TopLeftCol);

        if  (id != null)
        {
          sw.Write ("\t");
          sw.Write (id.CtdDate.ToString ("yyyy/MM/dd - HH:mm:ss"));  sw.Write ("\t");
          sw.Write (id.Latitude);                                    sw.Write ("\t");
          sw.Write (id.Longitude);                                   sw.Write ("\t");
          sw.Write (id.Temperature);
        }
        sw.WriteLine ();
        ++imagesWritten;
      }

      sw.WriteLine ("//All Data Written.");
      sw.Close();
    }

    //SMP751001034_02_00025491_3588



    private  void  SaveSampleImages (PicesDataBaseImageList planktonData)
    {
      PicesClassList  classes = planktonData.ExtractListOfClasses ();
      if  (classes == null)
        return;

      String  sampleDir = OSservices.AddSlash (OSservices.GetPathPartOfFile (dataFileName)) +
                          OSservices.GetRootName (dataFileName) +
                          "_SampleImages";
      sampleDir = OSservices.AddSlash (sampleDir);
      OSservices.CreateDirectoryPath (sampleDir);

      foreach (PicesClass pc in classes)
      {
        String classSampleDir = OSservices.AddSlash (sampleDir + pc.Name);
        OSservices.CreateDirectoryPath (classSampleDir);
        PicesDataBaseImageList examplesThisClass = planktonData.ExtractImagesForAGivenClass (pc);
        examplesThisClass.RandomizeOrder ();

        int  numWritten = 0;
        foreach  (PicesDataBaseImage  i in examplesThisClass)
        {
          String  imageRootName = OSservices.GetRootName (i.ImageFileName);

          PicesRaster  fullSizeImage = dbConn.ImageFullSizeFind (imageRootName);
          if  (fullSizeImage != null)
          {
            String  imageFileName = classSampleDir + imageRootName + ".bmp";
            fullSizeImage.Save (imageFileName);
            ++numWritten;
          }
          if  (numWritten >= includeSampleImagesNumPerClass)
            break;
        }
      }
    }  /* SaveSampleImages */



    private  PicesDataBaseImageList  RetrievePlanktonDataBySipperFiles ()
    {
      PicesDataBaseImageList  data = new PicesDataBaseImageList ();
      String[]  sipperFileNames = null;
      if  (String.IsNullOrEmpty (sipperFileName))
        sipperFileNames = dbConn.SipperFileGetList (cruise, station, deployment);
      else
      {
        sipperFileNames = new String[1];
        sipperFileNames[0] = sipperFileName;
      }

      if  (sipperFileNames != null)
      {
        for  (int idx = 0;  (idx < sipperFileNames.Length)  &&  (!cancelRequested);  idx++)
        {
          String  curSipperFileName = sipperFileNames[idx];
          backGroundStatus = "Loading[" + (idx + 1).ToString () + " of " + sipperFileNames.Length.ToString () + "]  File[" + curSipperFileName + "]";

          PicesDataBaseImageList dataThisSipperFile = dbConn.ImagesQuery (null,
                                                                          curSipperFileName,
                                                                          mlClass,
                                                                          classKeyToUse,
                                                                          probMin,  probMax,
                                                                          sizeMin,  sizeMax,
                                                                          depthMin, depthMax,
                                                                          0,                  // Restart ImageId
                                                                          -1,                 // limit  (Max number of images).
                                                                          false               // false = Do not include Thumbnail.
                                                                         );
            
            
          if  (dataThisSipperFile != null)
          {
            imagesLoaded += (uint)dataThisSipperFile.Count;

            foreach  (PicesDataBaseImage i in dataThisSipperFile)
              data.Add (i);
          }
        }
      }

      return data;
    }  /* RetrievePlanktonDataBySipperFiles */

    
    
    
    private  PicesDataBaseImageList  RetrievePlanktonDataByGroup ()
    {
      backGroundStatus = "Group [" + group.Name + "]";

      PicesDataBaseImageList  data = dbConn.ImagesQueryByGrouop (this.group, 
                                                                 CruiseName.Text, 
                                                                 StationName.Text, 
                                                                 DeploymentNum.Text, 
                                                                 SipperFile.Text,
                                                                 mlClass,
                                                                 classKeyToUse,
                                                                 probMin,  probMax,
                                                                 sizeMin,  sizeMax,
                                                                 depthMin, depthMax,
                                                                 0,         // Restart Image Id
                                                                 -1,        // limit (Max num Images to load.
                                                                 false      // false = Do not include Thumbnail
                                                                );
      if  (data != null)
        imagesLoaded += (uint)data.Count;
 
      return  data;
    }  /* RetrievePlanktonDataByGroup */



    private  void  CancelBackGroundThread ()
    {
      cancelRequested = true;
      if  (driver != null)
        driver.CancelLoad = true;
      CancelSaveButton.Enabled = false;
    }  /* CancelBackGroundThread */

    
 
    private void BrowsePlanktonDataFileNameButton_Click (object sender, EventArgs e)
    {
      String  pathName = OSservices.GetPathPartOfFile (dataFileName);
      String  rootName = OSservices.GetRootName       (dataFileName);

      SaveFileDialog sfd = new SaveFileDialog ();
      sfd.InitialDirectory = pathName;
      sfd.Title = "Featuire Data File Name";
      sfd.FileName = rootName;
      if  (sfd.ShowDialog() == DialogResult.OK)
      {
        dataFileName = sfd.FileName;
        FeatureFileName.Text = dataFileName;
      }
    }



    private void StartButton_Click(object sender, EventArgs e)
    {
      if  (backGroundThreadRunning)
        return;

      ValidateAll ();
      if  (errorsFound)
        return;
      DialogResult  dr = MessageBox.Show (this, "Start Saving Plankton Data ?", "Saving Plankton Data", MessageBoxButtons.YesNo);

      if  (dr == DialogResult.Yes)
      {
        StartButton.Enabled = false;

        FeatureFileName.ReadOnly = true;

        StartBackGroundThread ();
      }
    }



    private void  backGroundTimer_Tick(object sender, EventArgs e)
    {
      // Because 'imagesLoaded' is being updated by another thread we want 
      // to get a picture of it to use so that it does not change while
      // we process.
      uint xxx = imagesLoaded;
      if  (xxx > imageCount)
        imageCount = xxx;;

      if  (imagesLoaded > imageCount)
      {
        imageCount = imagesLoaded;
      }
      
      ImagesLoaded.Text = imagesLoaded.ToString ("##,###,##0");
      ImagesWritten.Text = imagesWritten.ToString ("##,###,##0");

      BackGroundSatus.Text = backGroundStatus;
      
      SaveProgress.Maximum = 100;

      float  percentDone = 50.0f * ((float)imagesLoaded / (float)imageCount);
      if  ((planktonData != null)  &&  (planktonData.Count > 0)  &&  (imagesWritten > 0))
        percentDone = 50.0f + 50.0f * (float)imagesWritten / (float)(planktonData.Count);
   
      SaveProgress.Maximum = 100;
      SaveProgress.Value   = (int)(percentDone);

      if  (backgroundThreadCompleted)
      {
        backGroundTimer.Enabled = false;

        if  (cancelRequested)
          MessageBox.Show (this, "Process Canceled.");
        else
          MessageBox.Show (this, "Done Saving Data File");
        Close ();
      }

    }



    private void CancelSaveButton_Click(object sender, EventArgs e)
    {
      DialogResult  dr = MessageBox.Show (this, 
                                          "Cancel Saving of Images ?", 
                                          "Cancel", 
                                          MessageBoxButtons.YesNo
                                         );
      if  (dr == DialogResult.Yes)
      {
        CancelBackGroundThread ();
      }
    }


    private void StatsTimer_Tick(object sender, EventArgs e)
    {
      if  (statsLoader == null)
      {
        StatsTimer.Enabled = false;
        StartButton.Enabled = true;
        return;
      }
 
      PicesClassStatisticList  stats = statsLoader.GetStatsAlreadyLoaded ();
      if  (stats != null)
      {
        imageCount = 0;
        foreach  (PicesClassStatistic stat in stats)
          imageCount += stat.Count;
        ImageCount.Text = imageCount.ToString ("##,###,##0");
      }

      if  (statsLoader.DoneLoading)
      {
        statsLoader = null;
        StatsTimer.Enabled = false;
        StartButton.Enabled = true;
      }
    }


    private void SaveFeatureData_FormClosing(object sender, FormClosingEventArgs e)
    {
      if  ((backGroundThreadRunning)  &&  (e.CloseReason == CloseReason.UserClosing))
      {
        DialogResult dr = MessageBox.Show (this, 
                                           "This process has not completed!   Are you sure that you want to exit?", 
                                           "Saving Data File not Completed!",
                                           MessageBoxButtons.YesNo
                                          );
        if  (dr == DialogResult.No)
        {
          e.Cancel = true;
          return;
        }
      }
      
      CancelBackGroundThread ();
    }



    private void FeatureFileName_Validating (object sender, CancelEventArgs e)
    {
      ValidateDataFileName ();
    }

  }
}
