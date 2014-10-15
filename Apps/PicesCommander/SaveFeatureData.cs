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
  public partial class SaveFeatureData : Form
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

    String                   featureFileName  = "";
    String                   fileFormat       = "Pices";

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
    uint    imagesFilteredOut          = 0;
    uint    imagesLoaded               = 0;
    uint    imagesMissingFeatureData   = 0;
    uint    imagesWritten              = 0;

    PicesFeatureVectorList  featureData = null;  // Feature Vectors that are going to be written to disk
    
    PicesFeatureFileIO      driver      = null;  // The Driver that will be used to write the Feature Data to disk.
                                                 // The 'BackGroundTimer' will pole this variable to
                                                 // keep track of number of records written to disk.
    
    String  backGroundStatus           = "";

    PlanktonClassStatisticsLoader  statsLoader = null;  // To determine total number of images that will be saved.

    
    public SaveFeatureData (PicesDataBase           _dbConn,
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

      featureFileName = OSservices.AddSlash (PicesSipperVariables.PicesReportDir ()) + "FeatureDataFiles";
      featureFileName = OSservices.AddSlash (featureFileName);
      if  (group != null)
      {
        featureFileName += group.Name;
      }
      else
      {
        if  (!String.IsNullOrEmpty (cruise))
        {
          featureFileName += cruise;
          if  (!String.IsNullOrEmpty (station))
          {
            featureFileName += "-" + station;
            if  (!String.IsNullOrEmpty (deployment))
            {
              featureFileName += "-" + deployment;
              if  (!String.IsNullOrEmpty (sipperFileName))
                featureFileName += "-" + sipperFileName;
            }
          }
        }
      }

      if  (mlClass != null)
      {
        if  (!String.IsNullOrEmpty (featureFileName))
          featureFileName += "-";
        featureFileName += mlClass.Name;
      }

      featureFileName += ".data";

      FeatureFileName.Text = featureFileName;

      String[]  validFileFormats = PicesFeatureFileIO.RegisteredDriverNames (false, true);
      foreach  (String s in validFileFormats)
        FileFormat.Items.Add (s);
      FileFormat.SelectedItem = fileFormat;

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

    
    
    private  void  ValidateFeatureFileName ()
    {
      errorProvider1.SetError (FeatureFileName, "");

      if  (String.IsNullOrEmpty (FeatureFileName.Text))
      {
        errorsFound = true;
        errorProvider1.SetError (FeatureFileName, "Must provide a Feature File Name.");
        return;
      }

      featureFileName = FeatureFileName.Text;

      bool  fileExistsAlready = true;
      try  {fileExistsAlready = File.Exists (featureFileName);}
      catch  (Exception)  {fileExistsAlready = false;}

      if  (fileExistsAlready)
      {
        errorsFound = true;
        errorProvider1.SetError (FeatureFileName, "File already Exists.");
        return;
      }

      String  pathName   = OSservices.GetPathPartOfFile (featureFileName);
      String  rootAndExt = OSservices.GetRootNameWithExtension (featureFileName);

      if  (String.IsNullOrEmpty (rootAndExt))
      {
        errorsFound = true;
        errorProvider1.SetError (FeatureFileName, "Missing File Name.");
        return;
      }

    }  /* ValidateDestinationDirectory */



    private  void  ValidateFileFormat ()
    {
      errorProvider1.SetError (FileFormat, "");
      fileFormat = FileFormat.Text;

      if  (!PicesFeatureFileIO.ValidDriverName (fileFormat))
      {
        errorsFound = true;
        errorProvider1.SetError (FileFormat, "Invalid File Format.");
        return;
      }

      driver = new PicesFeatureFileIO (fileFormat);
      if  (!driver.CanWrite)
      {
        errorsFound = true;
        errorProvider1.SetError (FileFormat, "This driver does not support Write.");
        return;
      }
    }


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
      ValidateFeatureFileName ();
      ValidateFileFormat ();
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
      imagesFilteredOut         = 0;
      imagesMissingFeatureData  = 0;

      if  (group != null)
        featureData = SaveFeatureDataByGroup ();
      else
        featureData = SaveFeatureDataBySipperFiles ();
      
      if  (!cancelRequested)
      {
        backGroundStatus = "Saving Feature File";
        String  pathName = OSservices.GetPathPartOfFile (featureFileName);
        if  (!String.IsNullOrEmpty (pathName))
          OSservices.CreateDirectoryPath (pathName);

        if  (IncludeSampleImages.Checked)
          SaveSampleImages (featureData);

        driver = new PicesFeatureFileIO (fileFormat);
        if  (driver != null)
        {
          if  (NormalizeData.Checked)
            driver.SaveFeatureFileNormalized (featureFileName, featureData, runLog);
          else
            driver.SaveFeatureFile (featureFileName, featureData, runLog);
        }
      }

      PicesDataBase.ThreadEnd ();
      backgroundThreadCompleted = true;
      backGroundThreadRunning   = false;
    }  /* BackGroundThread */



    private  void  SaveSampleImages (PicesFeatureVectorList featureData)
    {
      PicesClassList  classes = featureData.ExtractListOfClasses ();
      if  (classes == null)
        return;

      String  sampleDir = OSservices.AddSlash (OSservices.GetPathPartOfFile (featureFileName)) +
                          OSservices.GetRootName (featureFileName) +
                          "_SampleImages";
      sampleDir = OSservices.AddSlash (sampleDir);
      OSservices.CreateDirectoryPath (sampleDir);

      foreach (PicesClass pc in classes)
      {
        String classSampleDir = OSservices.AddSlash (sampleDir + pc.Name);
        OSservices.CreateDirectoryPath (classSampleDir);
        PicesFeatureVectorList examplesThisClass = featureData.ExtractImagesForAGivenClass (pc);
        examplesThisClass.RandomizeOrder ();

        int  numWritten = 0;
        foreach  (PicesFeatureVector  fv in examplesThisClass)
        {
          String  imageRootName = OSservices.GetRootName (fv.ImageFileName);

          PicesRaster  i = dbConn.ImageFullSizeFind (imageRootName);
          if  (i != null)
          {
            String  imageFileName = classSampleDir + imageRootName + ".bmp";
            i.Save (imageFileName);
            ++numWritten;
          }
          if  (numWritten >= includeSampleImagesNumPerClass)
            break;
        }
      }
    }  /* SaveSampleImages */


    private  PicesFeatureVectorList  FilterFeatureVectorList (PicesFeatureVectorList  data,
                                                              ref uint                numFilteredOut
                                                             )
    {
      numFilteredOut = 0;

      PicesFeatureVectorList  keepers = new PicesFeatureVectorList ();

      foreach   (PicesFeatureVector fv in data)
      {
        bool  keep = true;

        if  ((probMin > 0.0f)  &&  (fv.Probability < probMin))
          keep = false;

        else if  ((probMax < 1.0f)  &&  (fv.Probability > probMax))
          keep = false;

        else if  ((sizeMin > 0)  &&  (fv.OrigSize < sizeMin))
          keep = false;

        else if  ((sizeMax > 0)  &&  (fv.OrigSize > sizeMax))
          keep = false;

        else if  ((depthMin > 0)  &&  (fv.Depth < depthMin))
          keep = false;

        else if  ((depthMax > 0)  &&  (fv.Depth > depthMax))
          keep = false;

        if  (keep)
          keepers.Add (fv);
        else
          numFilteredOut++;
      }

      return  keepers;
    }  /* FilterFeatureVectorList */



    private  uint  CountImagesMissingFeatureData (PicesFeatureVectorList  data)
    {
      uint  imagesMissingFeatureData = 0;
      foreach  (PicesFeatureVector  fv in data)
      {
        if  (fv.FeatureDataMissing)
          imagesMissingFeatureData++;
      }
      return  imagesMissingFeatureData;
    }  /* CountImagesMissingFeatureData */



    private  PicesFeatureVectorList  SaveFeatureDataBySipperFiles ()
    {
      PicesFeatureVectorList  data = new PicesFeatureVectorList ();
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

          PicesFeatureVectorList dataThisSipperFile = dbConn.FeatureDataGetOneSipperFile (curSipperFileName, mlClass, classKeyToUse, false);
          if  (dataThisSipperFile != null)
          {
            imagesLoaded += (uint)dataThisSipperFile.Count;
            uint  numFilteredOut = 0;
            dataThisSipperFile = FilterFeatureVectorList (dataThisSipperFile, ref numFilteredOut);
            imagesFilteredOut += numFilteredOut;
            imagesMissingFeatureData += CountImagesMissingFeatureData (dataThisSipperFile);

            foreach  (PicesFeatureVector fv in dataThisSipperFile)
              data.Add (fv);
          }
        }
      }

      return data;
    }  /* SaveFeatureDataBySipperFiles */

    
    
    
    private  PicesFeatureVectorList  SaveFeatureDataByGroup ()
    {
      backGroundStatus = "Group [" + group.Name + "]";

      PicesFeatureVectorList  data = dbConn.FeatureDataForImageGroup (group, mlClass, classKeyToUse);
      if  (data != null)
      {
        imagesLoaded += (uint)data.Count;
        uint  numFilteredOut = 0;
        data = FilterFeatureVectorList (data, ref numFilteredOut);
        imagesFilteredOut += numFilteredOut;
        imagesMissingFeatureData += CountImagesMissingFeatureData (data);
      }

      return  data;
    }  /* SaveFeatureDataByGroup */



    private  void  CancelBackGroundThread ()
    {
      cancelRequested = true;
      if  (driver != null)
        driver.CancelLoad = true;
      CancelSaveButton.Enabled = false;
    }  /* CancelBackGroundThread */

    
 
    private void BrowseFeatureFileNameButton_Click (object sender, EventArgs e)
    {
      String  pathName = OSservices.GetPathPartOfFile (featureFileName);
      String  rootName = OSservices.GetRootName       (featureFileName);

      SaveFileDialog sfd = new SaveFileDialog ();
      sfd.InitialDirectory = pathName;
      sfd.Title = "Featuire Data File Name";
      sfd.FileName = rootName;
      if  (sfd.ShowDialog() == DialogResult.OK)
      {
        featureFileName = sfd.FileName;
        FeatureFileName.Text = featureFileName;
      }
    }



    private void StartButton_Click(object sender, EventArgs e)
    {
      if  (backGroundThreadRunning)
        return;

      ValidateAll ();
      if  (errorsFound)
        return;
      DialogResult  dr = MessageBox.Show (this, "Start Saving Feature Data ?", "Saving Feature Data", MessageBoxButtons.YesNo);

      if  (dr == DialogResult.Yes)
      {
        StartButton.Enabled = false;

        FeatureFileName.ReadOnly = true;
        FileFormat.Enabled = false;

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
      
      ImagesLoaded.Text             = imagesLoaded.ToString             ("##,###,##0");
      ImagesFilteredOut.Text        = imagesFilteredOut.ToString        ("##,###,##0");
      ImagesMissingFeatureData.Text = imagesMissingFeatureData.ToString ("##,###,##0");
      if  (driver != null)
      {
        imagesWritten = driver.NumExamplesWritten;
        ImagesWritten.Text = imagesWritten.ToString ("##,###,##0");
      }

      BackGroundSatus.Text = backGroundStatus;
      
      SaveProgress.Maximum = 100;

      float  percentDone = 50.0f * ((float)imagesLoaded / (float)imageCount);
      if  ((featureData != null)  &&  (featureData.Count > 0)  &&  (imagesWritten > 0))
        percentDone = 50.0f + 50.0f * (float)imagesWritten / (float)(featureData.Count);
   
      SaveProgress.Maximum = 100;
      SaveProgress.Value   = (int)(percentDone);

      if  (backgroundThreadCompleted)
      {
        backGroundTimer.Enabled = false;

        if  (cancelRequested)
          MessageBox.Show (this, "Process Canceled.");
        else
          MessageBox.Show (this, "Done Saving Feature Data");
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
                                           "Saving Feature Data not Completed!",
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

    private void FileFormat_Validating (object sender, CancelEventArgs e)
    {
      ValidateFileFormat ();
    }

    private void FeatureFileName_Validating (object sender, CancelEventArgs e)
    {
      ValidateFeatureFileName ();
    }

    private void FileFormat_SelectedIndexChanged (object sender, EventArgs e)
    {
      ValidateFileFormat ();
    }
  }
}
