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
  public partial class SaveImagesToDisk2 : Form
  {
    PicesDataBase            dbConn         = null;

    PicesDataBaseImageList   images         = null;
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

    int     increments            = 1;
    bool    saveByDepthIncrements = true;
    bool    saveByClass           = false;

    int     imagsPerDir = 10000;


    PicesRunLog  runLog = null;

    bool    errorsFound = false;

    String  infinityStr = ((char)8734).ToString ();

    // 
    Thread  savingThread           = null;
    bool    savingThreadRunning    = false;
    bool    savingThreadCompleted  = false;
    bool    cancelRequested        = false;
    uint    imageCount             = 0;
    uint    imagesSaved            = 0;
    uint    imagesFailedToExtract  = 0;
    String  curDir                 = "";
    String  sipperFileStatus       = "";

    PlanktonClassStatisticsLoader  statsLoader = null;  // To determine total number of images that will be saved.

    
    public SaveImagesToDisk2 (PicesDataBase           _dbConn,
                              PicesDataBaseImageList  _images,    // If '_images' is not null then you do not need to include any of the following parameters.
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
      images         = (_images         == null) ? null : _images;
      group          = _group;
      cruise         = (_cruise         == null) ? ""   : _cruise;
      station        = (_station        == null) ? ""   : _station;
      deployment     = (_deployment     == null) ? ""   : _deployment;
      sipperFileName = (_sipperFileName == null) ? ""   : _sipperFileName;
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



    private void SaveImagesToDisk2_Load(object sender, EventArgs e)
    {
      CruiseName.Text    = cruise;
      StationName.Text   = station;
      DeploymentNum.Text = deployment;
      SipperFile.Text    = sipperFileName;
      if  (group != null)
        GroupName.Text = group.Name;

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

      DestinationDirectory.Text =  OSservices.AddSlash  (OSservices.AddSlash (PicesSipperVariables.PicesHomeDir ()) + "SavedImages");
      OSservices.CreateDirectoryPath (DestinationDirectory.Text);
       
      //  PicesTempDirectory ());
      if  (group != null)
      {
        DestinationDirectory.Text += group.Name;
      }
      else
      {
        if  (!String.IsNullOrEmpty (cruise))
        {
          DestinationDirectory.Text += cruise;
          if  (!String.IsNullOrEmpty (station))
          {
            DestinationDirectory.Text += "-" + station;
            if  (!String.IsNullOrEmpty (deployment))
            {
              DestinationDirectory.Text += "-" + deployment;
              if  (!String.IsNullOrEmpty (sipperFileName))
                DestinationDirectory.Text += "-" + sipperFileName;
            }
          }
        }
      }

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

    


    private  bool  ImagesInDirectoryAlready (DirectoryInfo  dir)
    {
      if  (dir == null)
        return false;
      
      FileInfo[] files = dir.GetFiles ("*.bmp");
      if  (files != null)
      {
        if  (files.Length > 0)
          return true;
      }

      DirectoryInfo[]  subDirectories = dir.GetDirectories ();
      if  (subDirectories != null)
      {
        foreach (DirectoryInfo sd in subDirectories)
        {
          if  (ImagesInDirectoryAlready (sd))
            return  true;
        }
      }

      return  false;
    }  /* ImagesInDirectoryAlready */




    private  void  ValidateDestinationDirectory ()
    {
      errorProvider1.SetError (DestinationDirectory, "");

      if  (String.IsNullOrEmpty (DestinationDirectory.Text))
      {
        errorsFound = true;
        errorProvider1.SetError (DestinationDirectory, "You must provide a destination directory.");
        return;
      }

      String  parentDir = OSservices.GetParentDirectoryOfDirPath (DestinationDirectory.Text);
      DirectoryInfo di = new DirectoryInfo (parentDir);
      if  (!di.Exists)
      {
        errorsFound = true;
        errorProvider1.SetError (DestinationDirectory, "No such directory[" + parentDir + "].");
      }
    }  /* ValidateDestinationDirectory */



    private  void  ValidateIncrements ()
    {
      errorProvider1.SetError (Increments, "");

      if  (saveByClass)
        return;

      char[]  delimeters = {' ', '\n', '\r'};
      String[]  fields = Increments.Text.Split  (delimeters);
      if  (fields.Length < 1)
      {
        errorsFound = true;
        errorProvider1.SetError (Increments, "No Increment specified.");
      }

      int  temp = PicesKKStr.StrToInt (fields[0]);
      if  ((temp < 1)  ||  (temp > 100))
      {
        errorsFound = true;
        errorProvider1.SetError (Increments, "Valid Increment range is (1 - 100).");

      }
      increments  = temp;
    }



    private  void  ValidateAll ()
    {
      errorsFound = false;
      ValidateDestinationDirectory ();
      ValidateIncrements ();
    }


    private  void  StartSavingImagesThread ()
    {
      SaveProgress.Maximum = (int)imageCount;
      SaveProgress.Value   = 0;
      OSservices.CreateDirectoryPath (DestinationDirectory.Text);
      savingThread = new Thread (new ThreadStart (SaveImages));
      savingThread.Start ();
      Thread.Sleep (10);
      savingTimer.Enabled = true;
    }


    private  void  SaveImages ()
    {
      PicesDataBase.ThreadInit ();
      savingThreadRunning   = true;
      imagesSaved           = 0;
      imagesFailedToExtract = 0;

      if  (images != null)
        SaveGivenImageList ();

      else if  (group != null)
        SaveImagesByGroup ();

      else
        SaveImagesBySipperFiles ();

      savingThreadCompleted = true;
      savingThreadRunning   = false;
      PicesDataBase.ThreadEnd ();
    }  /* SaveImages */



    private  void  SaveGivenImageList ()
    {
      imageCount = (uint)images.Count;
      SaveOneBatchOfImages (images);
    }  /* SaveGivenImageList */



    private  void  SaveImagesBySipperFiles ()
    {
      PicesSipperFileList  sipperFiles = null;
      if  (String.IsNullOrEmpty (sipperFileName))
      {
        sipperFiles = dbConn.SipperFileLoad (cruise, station, deployment);
      }
      else
      {
        PicesSipperFile sf = dbConn.SipperFileRecLoad (sipperFileName);
        if  (sf != null)
        {
          sipperFiles = new PicesSipperFileList ();
          sipperFiles.Add (sf);
        }
      }

      if  (sipperFiles != null)
      {
        String  lastCruise = "";
        String  lastStation = "";
        String  lastDeployment = "";
        String  dirPath = "";

        Dictionary<String,int>  imagesPerClass = new Dictionary<string,int> ();

        foreach  (PicesSipperFile  sf in sipperFiles)
        {
          if  ((sf.CruiseName    != lastCruise)  ||
               (sf.StationName   != lastStation) ||
               (sf.DeploymentNum != lastDeployment)
              )
          {
            lastCruise     = sf.CruiseName;
            lastStation    = sf.StationName;
            lastDeployment = sf.DeploymentNum;
            dirPath = OSservices.AddSlash (DestinationDirectory.Text);

            if  (!String.IsNullOrEmpty (lastCruise))
              dirPath = OSservices.AddSlash (dirPath) + lastCruise;

            if  (!String.IsNullOrEmpty (lastStation))
              dirPath = OSservices.AddSlash (dirPath) + lastStation;

            if  (!String.IsNullOrEmpty (lastDeployment))
              dirPath = OSservices.AddSlash (dirPath) + lastDeployment;

            OSservices.CreateDirectoryPath (dirPath);
            imagesPerClass = new Dictionary<string,int> ();
          }

          SaveImagesForOneSipperFile (dirPath, sf, imagesPerClass);
        }
      }
    }  /* SaveImagesBySipperFiles */



    private  void  SaveImagesForOneSipperFile (String                  dirPath, 
                                               PicesSipperFile         sf, 
                                               Dictionary<string,int>  imagesPerClass
                                              )
    {
      uint  lastImageId = 0;

      sipperFileName = sf.SipperFileName;

      while  (!cancelRequested)
      {
        int  limit = -1;
        if  (mlClass == null)
        {
          // With out the Class Key;  mySql will probably not be able to restart the query from a known spot and 
          // get all the images that is intended.  So in this case we want to retrieve all images that meet the 
          // criteria from this SIPPER file.
          limit = -1;
        }
        PicesDataBaseImageList  nextBunch = dbConn.ImagesQuery 
                      (null,     // ImageGroup
                       sipperFileName, 
                       mlClass,
                       classKeyToUse,
                       probMin,     probMax, 
                       sizeMin,     sizeMax, 
                       depthMin,    depthMax, 
                       lastImageId, 
                       limit,
                       false      // false = Dont't load Thumbnail images.
                      );

        if  ((nextBunch == null)  ||  (nextBunch.Count < 1))
          break;

        SaveOneBatchOfImages (dirPath, imagesPerClass, nextBunch);
        lastImageId = nextBunch[nextBunch.Count - 1].ImageId;
        nextBunch = null;

        if  ((nextBunch == null)  ||  (nextBunch.Count < limit)  ||  (limit < 0))
        {
          // Since we loaded less images than 'limit' was set for we are done loading images from this sipperFileName.
          break;
        }

        nextBunch = null;
      }
    }  /* SaveImagesForOneSipperFile */






    private  void  SaveImagesForOneSipperFile (String  sipperFileName)
    {
      uint  lastImageId = 0;

      while  (!cancelRequested)
      {
        int  limit = -1;
        if  (mlClass == null)
        {
          // With out the Class Key;  mySql will probably not be able to restart the query from a known spot and 
          // get all the images that is intended.  So in this case we want to retrieve all images that meet the 
          // criteria from this SIPPER file.
          limit = -1;
        }
        PicesDataBaseImageList  nextBunch = dbConn.ImagesQuery 
                      (null,     // ImageGroup
                       sipperFileName, 
                       mlClass,
                       classKeyToUse,
                       probMin,     probMax, 
                       sizeMin,     sizeMax, 
                       depthMin,    depthMax, 
                       lastImageId, 
                       limit,
                       false      // false = Dont't load Thumbnail images.
                      );

        if  ((nextBunch == null)  ||  (nextBunch.Count < 1))
          break;

        SaveOneBatchOfImages (nextBunch);
        lastImageId = nextBunch[nextBunch.Count - 1].ImageId;
        nextBunch = null;

        if  ((nextBunch == null)  ||  (nextBunch.Count < limit)  ||  (limit < 0))
        {
          // Since we loaded less images than 'limit' was set for we are done loading images from this sipperFileName.
          break;
        }

        nextBunch = null;
      }
    }  /* SaveImagesForOneSipperFile */
    


    
    
    private  void  SaveImagesByGroup ()
    {
      uint  lastImageId  = 0;
      sipperFileStatus = "Group [" + group.Name + "]";

      while  (!cancelRequested)
      {
        int  limit = -1;
        PicesDataBaseImageList  nextBunch = dbConn.ImagesQueryByGrouop 
                                  (group, 
                                   cruise,
                                   station,
                                   deployment,
                                   sipperFileName,
                                   mlClass,
                                   classKeyToUse,
                                   probMin,   probMax,
                                   sizeMin,   sizeMax,
                                   depthMin,  depthMax,
                                   lastImageId,
                                   limit,            // Max # of rows to return.  -1 indicates no limit.,
                                   false             // false = Don't Load IncludeThumbnail
                                  );
        if  ((nextBunch == null)  ||  (nextBunch.Count < 1))
        {
          // Sine we loaded less images than 'limit' was set for we are done loading images from this sipperFileName.
          break;
        }

        // Get the 'lastImagId'  before calling 'SaveOneBatchOfImages' which will sort it into a diff order.
        if  (nextBunch.Count > 0)
          lastImageId = nextBunch[nextBunch.Count - 1].ImageId;
        SaveOneBatchOfImages (nextBunch);
        nextBunch = null;
      }

    }  /* SaveImagesByGroup */




    private  void  SaveOneBatchOfImages (PicesDataBaseImageList  batch)
    {
      if  (saveByDepthIncrements)
        SaveImagesByDepthImcrements (batch);
      else
        SaveImagesByClass (batch);
    }


    
    
    private  void  SaveOneBatchOfImages (String                    dirPath, 
                                         Dictionary<String, int>   imagesPerClass, 
                                         PicesDataBaseImageList    batch
                                       )
    {
      if  (classKeyToUse == 'V')
        batch.Sort (PicesDataBaseImageList.SortOrderType.soValidatedClassName, false);
      else
        batch.Sort (PicesDataBaseImageList.SortOrderType.soClassName, false);

      int  idx = 0;
      PicesDataBaseImage i = batch[0];

      while  ((idx < batch.Count)  &&  (!cancelRequested))
      {
        String  className     = i.Class1Name;
        if  (classKeyToUse == 'V')
          className = i.ValidatedClassName;

        String  curClassName  = className;

        String  classDir = OSservices.AddSlash (dirPath) + className;

        try {OSservices.CreateDirectoryPath (classDir);}  catch (Exception)  {}

        int  countThisClass = 0;
        if  (imagesPerClass.ContainsKey (curClassName))
        {
          countThisClass = imagesPerClass[curClassName];
        }
        else
        {
          imagesPerClass.Add (curClassName, 0);
        }

        int  lastFoldNum = -1;
        String  foldDir = "";

        while  ((idx < batch.Count)  &&  (!cancelRequested)  &&  (className == curClassName))
        {
          int  foldNum = countThisClass / imagsPerDir;

          if  (foldNum != lastFoldNum)
          {
            foldDir = OSservices.AddSlash (classDir) + curClassName + "_" + foldNum.ToString ("000");
            
            OSservices.CreateDirectoryPath (foldDir);
            lastFoldNum = foldNum;
          }

          String fullName = OSservices.AddSlash (foldDir) + i.ImageFileName + ".bmp";
          PicesRaster r = dbConn.ImageFullSizeFind (i.ImageFileName);
          if  (r == null)
            imagesFailedToExtract++;
          else
            r.Save (fullName);
          imagesSaved++;

          // Get the next image to save.
          idx++;
          if  (idx < batch.Count)
          {
            i = batch[idx];
            className = i.Class1Name;
            if  (classKeyToUse == 'V')
              className = i.ValidatedClassName;
          }

          countThisClass++;
        }

        imagesPerClass[curClassName] = countThisClass;
      }
    }  /* SaveOneBatchOfImages */



    private  int  DepthIndex (double  depth)
    {
      double  incrementdDbl = (double)increments;

      int  depthIndex = (int)Math.Floor (depth / incrementdDbl);
      if  (depthIndex < 0)
        depthIndex = 0;

      return  depthIndex;
    }



    private  void  SaveImagesByDepthImcrements (PicesDataBaseImageList  batch)
    {
      batch.Sort (PicesDataBaseImageList.SortOrderType.soDepth, false);

      int  idx = 0;
      PicesDataBaseImage i = batch[0];

      while  ((idx < batch.Count)  &&  (!cancelRequested))
      {
        int  depthIndex = DepthIndex (i.Depth);
        int  curDepthIndex = depthIndex;

        int  fromDepth = depthIndex * increments;
        int  toDepth   = (depthIndex + 1) * increments;

        String  depthSubDirName = fromDepth.ToString ("000") + "-" + toDepth.ToString ("000");
        curDir = OSservices.AddSlash (DestinationDirectory.Text) + depthSubDirName;

        try {Directory.CreateDirectory (curDir);}  catch (Exception)  {}

        PicesDataBaseImageList  imagesThisDepthIndex = new PicesDataBaseImageList ();

        while  ((idx < batch.Count)  &&  (!cancelRequested)  &&  (depthIndex == curDepthIndex))
        {
          imagesThisDepthIndex.Add (i);
          idx++;
          if  (idx < batch.Count)
          {
            i = batch[idx];
            depthIndex = DepthIndex (i.Depth);
          }
        }

        SaveImagesInList (imagesThisDepthIndex, curDir);
      }
    }  /* SaveImagesByDepthImcrements */





    private  void  SaveImagesByClass (PicesDataBaseImageList  batch)
    {
      if  (classKeyToUse == 'V')
        batch.Sort (PicesDataBaseImageList.SortOrderType.soValidatedClassName, false);
      else
        batch.Sort (PicesDataBaseImageList.SortOrderType.soClassName, false);

      int  idx = 0;
      PicesDataBaseImage i = batch[0];

      while  ((idx < batch.Count)  &&  (!cancelRequested))
      {
        String  className     = i.Class1Name;
        if  (classKeyToUse == 'V')
          className = i.ValidatedClassName;

        String  curClassName  = className;

        curDir = OSservices.AddSlash (DestinationDirectory.Text) + className;

        try {Directory.CreateDirectory (curDir);}  catch (Exception)  {}

        PicesDataBaseImageList  imagesThisClass = new PicesDataBaseImageList ();

        while  ((idx < batch.Count)  &&  (!cancelRequested)  &&  (className == curClassName))
        {
          imagesThisClass.Add (i);
          idx++;
          if  (idx < batch.Count)
          {
            i = batch[idx];
            className = i.Class1Name;
            if  (classKeyToUse == 'V')
              className = i.ValidatedClassName;
          }
        }

        SaveImagesInList (imagesThisClass, curDir);
      }
    }  /* SaveImagesByClass */




    private  void  SaveImagesInList (PicesDataBaseImageList  list,
                                     String                  dirName
                                    )
    {
      dirName = OSservices.AddSlash (dirName);
      curDir = dirName;
      foreach (PicesDataBaseImage i in list)
      {
        if  (cancelRequested)
          break;

        String fullName = dirName + i.ImageFileName + ".bmp";
        PicesRaster r = dbConn.ImageFullSizeFind (i.ImageFileName);
        if  (r == null)
          imagesFailedToExtract++;
        else
          r.Save (fullName);
        imagesSaved++;
      }
    }  /* SaveImagesInList */



    private  void  CancelImageSaveThread ()
    {
      cancelRequested = true;

      // Will wait up till 4 seconds for savingprocess to pause.
      int x = 0;
      while  ((savingThreadRunning) && (x < 40))
      {
        Thread.Sleep (100);
        x++;
      }

      if  (savingThreadRunning)
      {
        savingThread.Abort ();
      }
    }  /* CancelImageSaveThread */



    private void ByDepthIncrements_CheckedChanged(object sender, EventArgs e)
    {
      saveByDepthIncrements = ByDepthIncrements.Checked;
      if  (ByDepthIncrements.Checked)
      {
        if  (ByClass.Checked)
          ByClass.Checked = false;
        Increments.Enabled = true;
      }
      else
      {
        if  (!ByClass.Checked)
          ByClass.Checked = true;
        Increments.Enabled = false;
      }
    }

    private void ByClass_CheckedChanged(object sender, EventArgs e)
    {
      saveByClass = ByClass.Checked;
      if  (ByClass.Checked)
      {
        if  (ByDepthIncrements.Checked)
          ByDepthIncrements.Checked = false;
      }
      else
      {
        if  (!ByDepthIncrements.Checked)
          ByDepthIncrements.Checked = true;
      }
    }


    private void BrowseDestinationDir_Click (object sender, EventArgs e)
    {
      FolderBrowserDialog  fbd = new FolderBrowserDialog ();
      fbd.SelectedPath = DestinationDirectory.Text;

      DialogResult dr = fbd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        DestinationDirectory.Text = fbd.SelectedPath;
        ValidateDestinationDirectory ();
      }
    }



    private void StartButton_Click(object sender, EventArgs e)
    {
      if  (savingThreadRunning)
        return;

      ValidateAll ();
      if  (errorsFound)
        return;
      DialogResult  dr = MessageBox.Show (this, "Start Saving Images ?", "Saving Images", MessageBoxButtons.YesNo);

      if  (dr == DialogResult.Yes)
      {
        StartButton.Enabled = false;
        StartSavingImagesThread ();
      }
    }



    private void savingTimer_Tick(object sender, EventArgs e)
    {
      // Because 'imagesSaved' is being updated by another thread we want 
      // to get a picture of it to use so thatit does not change while
      // we process.
      uint xxx = imagesSaved;
      if  (xxx > imageCount)
        imageCount = xxx;;

      if  (imagesSaved > imageCount)
        imageCount = imagesSaved;

      SaveProgress.Maximum = (int)imageCount;
      SaveProgress.Value   = (int)xxx;

      ImagesSaved.Text           = imagesSaved.ToString           ("##,###,##0");
      ImagesFailedToExtract.Text = imagesFailedToExtract.ToString ("##,###,##0");
      CurrentDirectory.Text      = curDir;
      SipperFileSatus.Text       = sipperFileStatus;
      if  (savingThreadCompleted)
      {
        savingTimer.Enabled = false;
        MessageBox.Show (this, "Done Saving Images");
        Close ();
      }
    }


    private void CancelSaveButton_Click(object sender, EventArgs e)
    {
      if  (!savingThreadRunning)
        this.Close ();

      DialogResult  dr = MessageBox.Show (this, "Cancel Saving of Images ?", "Cancel", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        savingTimer.Enabled = false;
        CancelImageSaveThread ();
        this.Close ();
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


    private void SaveImagesToDisk2_FormClosing(object sender, FormClosingEventArgs e)
    {
      if  ((savingThreadRunning)  &&  (e.CloseReason == CloseReason.UserClosing))
      {
        DialogResult dr = MessageBox.Show (this, 
                                           "The saving of images has not completed!   Are you sure that you want to exit?", 
                                           "Saving Images not Completed!",
                                           MessageBoxButtons.YesNo
                                          );
        if  (dr == DialogResult.No)
        {
          e.Cancel = true;
          return;
        }
      }
      
      savingTimer.Enabled = false;
      if   (savingThreadRunning)
        CancelImageSaveThread ();
    }
  }
}
