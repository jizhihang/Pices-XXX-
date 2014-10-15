using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

using  PicesInterface;

//*****************************************************************************************
//* Developer:  Kurt Kramer                                                               *
//* Date        2009-03-21                                                                *
//*                                                                                       *
//* Loads images from the Pices database as a seperate thread.  Images will be loaded in  *
//* blocks so that this instance can be queried periodically for what is already loaded.  *
//*****************************************************************************************


namespace PicesCommander
{
  public class PlanktonImageLoader
  {
    public  delegate  void  AlertCompletion (PlanktonImageLoader  sender);

    private  AlertCompletion  completionEvent  = null;   // Upon completion of load will call this method.

  
    private  bool           allSipperFiles    = false;
    private  bool           cancelRequested   = false;
    private  String[]       sipperFileNames   = null;   // List of siper files that we will have to load from.
    private  String         curSipperFileName = null;
    private  int            curSipperFileIdx  = -1;     // Index into "sipperFileNames"  thats currently being loaded.
    private  PicesRunLog    runLog            = null;
    private  PicesDataBase  dbConn            = null;


    // Run time parameters
    private  PicesDataBaseImageGroup  selectedImageGroup = null;
    private  String                   cruiseName     = "";
    private  String                   stationName    = "";
    private  String                   deploymentNum  = "";
    private  String                   sipperFileName = "";
    private  PicesClass               mlClass     = null;
    private  char                     classKeyToUse  = 'P';
    private  float                    probMin        = 0.0f;
    private  float                    probMax        = 1.0f;
    private  int                      sizeMin        = 0;
    private  int                      sizeMax        = 0;
    private  float                    depthMin       = 0.0f;
    private  float                    depthMax       = 0.0f;
    private  uint                     restartImageId = 0;
                                
    private  PicesGoalKeeper          blocker                = null;
    private  bool                     doneLoading            = false;
    private  PicesDataBaseImageList   loadedImages           = null;
    private  int                      imagesLoadedCount      = 0;
    private  Thread                   loaderThread           = null;
    private  bool                     loaderThreadRuning     = false;
    
    
    public  bool             CancelRequested         {get  {return cancelRequested;}}
    public  String           CurSipperFileName       {get  {return curSipperFileName;}}
    public  AlertCompletion  CompletionEvent         {get  {return completionEvent;}     set {completionEvent = value;}}
    public  bool             DoneLoading             {get  {return doneLoading;}}
    public  bool             LoaderThreadRuning      {get  {return loaderThreadRuning;}}
    public  int              ImagesLoadedCount       {get  {return imagesLoadedCount;}}

    public  int              SipperFilesCount        {get  {return  ((sipperFileNames == null) ? 0 : sipperFileNames.Length);}}
    public  int              SipperFilesLoaded       {get  {return  (Math.Max (0, curSipperFileIdx));}}
    


    int  LoadedImagesCount    // Number images that have ben loaded so far but not retrieved by "GetImagesAlreadyLoaded"
    {
      get  
      {
        int  loadedImagesCount = 0;
        blocker.StartBlock ();
        if  (loadedImages != null)
          loadedImagesCount = loadedImages.Count;
        blocker.EndBlock ();
        return  LoadedImagesCount;
      }
    }

    
    
    public  PlanktonImageLoader (PicesDataBaseImageGroup  _selectedImageGroup,
                                 String                   _cruiseName,
                                 String                   _stationName,
                                 String                   _deploymentNum,
                                 String                   _sipperFileName,
                                 PicesClass               _mlClass,
                                 char                     _classKeyToUse,
                                 float                    _probMin,
                                 float                    _probMax,
                                 int                      _sizeMin,
                                 int                      _sizeMax,
                                 float                    _depthMin,
                                 float                    _depthMax,
                                 uint                     _restartImageId
                                )
    {
      runLog = new PicesRunLog ();
      dbConn               = null;
      selectedImageGroup   = _selectedImageGroup;
      cruiseName           = _cruiseName;
      stationName          = _stationName;
      deploymentNum        = _deploymentNum;
      sipperFileName       = _sipperFileName;
      mlClass           = _mlClass;
      classKeyToUse        = _classKeyToUse;
      probMin              = _probMin;
      probMax              = _probMax;
      sizeMin              = _sizeMin;
      sizeMax              = _sizeMax;
      depthMin             = _depthMin;
      depthMax             = _depthMax;
      restartImageId       = _restartImageId;

      blocker = new PicesGoalKeeper ("PlanktonImageLoader");
      
      loaderThread = new Thread (new ThreadStart (LoadImagesThread));
      loaderThread.Start ();
    }
    


    public  void  CleanUpMemory ()
    {
      if  (loaderThreadRuning)
        CancelLoadWait (2000);

      loadedImages.CleanUpMemory ();
      blocker = null;
    }  /* CleanUpMemory */



    public  void  CancelLoad ()   // Will set "cancelRequested" flag to true and return imediately.
    {
      cancelRequested = true;
    }


    public  void  CancelLoadWait (int maxMilliSecsToWait)  // Will wait upto "maxMilliSecsToWait" for loadThread to terminate before returning.  Then it will force terminatiom of thread.
    {
      CancelLoad ();
      int  milliSecsWaitedSoFar = 0;
      
      while  ((loaderThreadRuning)  &&  (milliSecsWaitedSoFar <= maxMilliSecsToWait))
      {
        Thread.Sleep (10);
        milliSecsWaitedSoFar += 10;
      }

      if  (loaderThread.IsAlive)
      {
        int  milliSecsWaitingForThreadToAbort = 0;
        loaderThread.Abort ();
        while ((loaderThread.IsAlive)  &&  (milliSecsWaitingForThreadToAbort < 1000))
        {
          Thread.Sleep (10);
          milliSecsWaitingForThreadToAbort += 10;
        }
      }
    }  /* CancelLoadWait */



    //  return to the caller a list of images that I have loaded and
    //  then remove from my own internal list 'loadedImages'.
    public  PicesDataBaseImageList  GetImagesAlreadyLoaded ()
    {
      blocker.StartBlock ();
      PicesDataBaseImageList imagesToReturn = loadedImages;
      loadedImages = new PicesDataBaseImageList ();
      blocker.EndBlock ();

      return  imagesToReturn;
    }



    private  void  LoadImagesForSelectedGroup ()
    {
      curSipperFileName = "";
      uint  lastImageId  = restartImageId;

      while  (!cancelRequested)
      {
        int  limit = 100;
        PicesDataBaseImageList  nextBunch = dbConn.ImagesQueryByGrouop 
                                  (selectedImageGroup, 
                                   cruiseName,
                                   stationName,
                                   deploymentNum,
                                   sipperFileName,
                                   mlClass,        classKeyToUse,
                                   probMin,        probMax,
                                   sizeMin,        sizeMax,
                                   depthMin,       depthMax,
                                   lastImageId,
                                   limit,            // Max # of rows to return.  -1 indicates no limit.,
                                   true              // true = IncludeThumbnail
                                  );
        if  ((nextBunch == null)  ||  (nextBunch.Count < 1))
          return;

        blocker.StartBlock ();

        if  (loadedImages == null)
          loadedImages = new PicesDataBaseImageList ();

        uint  largestImageId  = nextBunch[0].ImageId;

        foreach  (PicesDataBaseImage  i  in  nextBunch)
        {
          if  (i.ImageId != lastImageId)
          {
            loadedImages.Add (i);
            imagesLoadedCount++;
          }

          if  (i.ImageId > largestImageId)
            largestImageId = i.ImageId;
        }

        lastImageId = largestImageId;

        blocker.EndBlock ();

        if  ((nextBunch.Count < limit)  ||  (limit < 0))
        {
          // Sine we loaded less images than 'limit' was set for we are done loading images from this sipperFileName.
          break;
        }

        nextBunch = null;
      }

    }  /* LoadImagesForSelectedGroup */





    private  void  LoadImagesForOneSipperFile (String  sipperFileName)
    {
      curSipperFileName = sipperFileName;
      uint    lastImageId  = restartImageId;

      while  (!cancelRequested)
      {
        int  limit = 100;
        if  (mlClass == null)
        {
          // With out the Class Key;  mySql will probably not be able to restart the query from a know spot and 
          // get all the images that is intended.  So in this case we want to retrieve all images that meet the 
          // criteria from this SIPPER file.
          limit = -1;
        }
        PicesDataBaseImageList  nextBunch = dbConn.ImagesQuery 
                      (null,   // ImageGroup
                       sipperFileName, mlClass, classKeyToUse,
                       probMin, probMax, sizeMin, sizeMax, depthMin, depthMax, 
                       lastImageId, limit,
                       true      // Load Thumbnail image also.
                      );

        if  ((nextBunch == null)  ||  (nextBunch.Count < 1))
          return;

        blocker.StartBlock ();

        if  (loadedImages == null)
          loadedImages = new PicesDataBaseImageList ();

        uint  largestImageId  = nextBunch[0].ImageId;

        foreach  (PicesDataBaseImage  i  in  nextBunch)
        {
          if  (i.ImageId != lastImageId)
          {
            loadedImages.Add (i);
            imagesLoadedCount++;
          }

          if  (i.ImageId > largestImageId)
            largestImageId = i.ImageId;
        }

        lastImageId = largestImageId;

        blocker.EndBlock ();

        if  ((nextBunch.Count < limit)  ||  (limit < 0))
        {
          // Sine we loaded less images than 'limit' was set for we are done loading images from this sipperFileName.
          break;
        }

        nextBunch = null;
      }

      curSipperFileName = "";
    }  /* LoadImagesForOneSipperFile */
    


    private  void  LoadImagesThread ()
    {
      blocker.StartBlock ();
      loaderThreadRuning = true;
      PicesDataBase.ThreadInit ();
      blocker.EndBlock ();

      dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      allSipperFiles = (String.IsNullOrEmpty (cruiseName)     &&  
                        String.IsNullOrEmpty (stationName)    &&  
                        String.IsNullOrEmpty (deploymentNum)  &&
                        String.IsNullOrEmpty (sipperFileName)
                       );

      if  (!String.IsNullOrEmpty (sipperFileName))
      {
        sipperFileNames = new String[1];
        sipperFileNames[0] = sipperFileName;
      }
      else
      {
        sipperFileNames = dbConn.SipperFileGetList (cruiseName, stationName, deploymentNum);
      }

      if  (selectedImageGroup != null)
      {
        LoadImagesForSelectedGroup ();
      }
      else
      {
        if  (sipperFileNames != null)
        {
          for  (curSipperFileIdx = 0;  (curSipperFileIdx < sipperFileNames.Length)  &&  (!cancelRequested);  curSipperFileIdx++)
            LoadImagesForOneSipperFile (sipperFileNames[curSipperFileIdx]);
        }
      }

      blocker.StartBlock ();
      if  (!cancelRequested)
        doneLoading = true;
      loaderThreadRuning = false;
      blocker.EndBlock ();

      if  (completionEvent != null)
        completionEvent (this);

      dbConn.Close ();
      dbConn = null;
      GC.Collect ();

      PicesDataBase.ThreadEnd ();
    }  /* LoadImagesTread */
  
  
  }  /* PlanktonImageLoader */
}
