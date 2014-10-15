using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

using  PicesInterface;
// I started by cloning PlanktonImageLoader;  meant to make the loading of 
// stats for thenavigation panel much smoother.


//*****************************************************************************************
//  Loads images from the Pices database as a seperate thread.  Images will be loaded in  *
//  blocks so that this instance can be queried periodically for what is already loaded.  *
//*****************************************************************************************


namespace PicesCommander
{
  public class PlanktonClassStatisticsLoader
  {
    public  delegate  void  AlertCompletion (PlanktonClassStatisticsLoader  sender);

    private  AlertCompletion  completionEvent  = null;   // Upon completion of load will call this method.

  
    private  bool           allSipperFiles    = false;
    private  bool           cancelRequested   = false;
    private  String[]       sipperFileNames   = null;  // List of siper files that we will have to load from.
    private  String         curSipperFileName = null;
    private  int            curSipperFileIdx  = -1;    // Index into "sipperFileNames"  that s currently being loaded.
    private  PicesRunLog    runLog            = null;
    private  PicesDataBase  dbConn            = null;


    // Run time parameters
    private  PicesDataBaseImageGroup     selectedImageGroup  = null;
    private  String                      cruiseName          = "";
    private  String                      stationName         = "";
    private  String                      deploymentNum       = "";
    private  String                      sipperFileName      = "";
    private  PicesClass                  mlClass          = null;
    private  char                        classKeyToUse       = 'P';
    private  float                       probMin             = 0.0f;
    private  float                       probMax             = 1.0f;
    private  int                         sizeMin             = 0;
    private  int                         sizeMax             = 0;
    private  float                       depthMin            = 0.0f;
    private  float                       depthMax            = 0.0f;

    private  PicesGoalKeeper             blocker             = null;
    private  bool                        doneLoading         = false;  // Set to true when succesfully loaded all data.  
    private  PicesClassStatisticList     loadedStats         = null;  // Statistics will acumulate in this list
    private  Thread                      loaderThread        = null;
    private  bool                        loaderThreadRuning  = false;
    private  uint                        imageCountTotal     = 0;
    private  bool                        newStatsAvailable   = false;   // Indicates if there have been any update to teh stats
                                                                           // that will be returned since the last time a call to
                                                                           // 'GetStatsAlreadyLoadedIfNew' was made.
    private  int                         sipperFilesCount    = 0;

    private  PicesVolumeSampledStatList  volumeWaterSampled  = null;


    public  bool                        CancelRequested         {get  {return cancelRequested;}}
    public  String                      CurSipperFileName       {get  {return curSipperFileName;}}
    public  AlertCompletion             CompletionEvent         {get  {return completionEvent;}     set {completionEvent = value;}}
    public  bool                        DoneLoading             {get  {return doneLoading;}}
    public  uint                        ImageCountTotal         {get  {return imageCountTotal;}}
    public  bool                        LoaderThreadRuning      {get  {return loaderThreadRuning;}}

    public  int                         SipperFilesCount        {get  {return sipperFilesCount;}}
    public  int                         SipperFilesLoaded       {get  {return (Math.Max (0, curSipperFileIdx));}}

    public  PicesVolumeSampledStatList  VolumeWaterSampled      {get  {return volumeWaterSampled;}}
    


    public  int   ClassesLoadedCount      
    {
      get  
      {
        int  classesLoadedCount = 0;
        blocker.StartBlock ();
        if  (loadedStats == null)
          classesLoadedCount = 0;
        else
          classesLoadedCount = loadedStats.Count;
        blocker.EndBlock ();
        return  classesLoadedCount;
      }
    }

   
    
    public  PlanktonClassStatisticsLoader (PicesDataBaseImageGroup  _selectedImageGroup,
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
                                           float                    _depthMax
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

      imageCountTotal = 0;

      blocker = new PicesGoalKeeper ("PlanktonClassStatisticsLoader");
      
      loaderThread = new Thread (new ThreadStart (LoadStatsThread));
      loaderThread.Start ();
    }
    


    public  void  CleanUpMemory ()
    {
      if  (loaderThreadRuning)
        CancelLoadWait (3000);
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

      loaderThreadRuning = false;
    }  /* CancelLoadWait */



    public  String  Status ()
    {
      String  status = "";
      blocker.StartBlock ();

      if  (cancelRequested)
      {
        if  (loaderThreadRuning)
          status = "Canceling";
        else
          status = "Canceled";
      }
      else
      {
        if  (loaderThreadRuning)
          status = (curSipperFileIdx + 1).ToString () + " of " +  sipperFilesCount.ToString ();
        else
          status = "Load Done";
      }
      blocker.EndBlock ();
      return  status;
    }



    //  return to the caller current acumulated statistics
    //  then remove from my own internal list 'loadedStats'.
    public  PicesClassStatisticList  GetStatsAlreadyLoaded ()
    {
      PicesClassStatisticList statsToReturn = null;
      blocker.StartBlock (); 
      if  (loadedStats != null)
      {
        statsToReturn = new PicesClassStatisticList ();
        foreach (PicesClassStatistic stat in loadedStats)
          statsToReturn.Add (stat);
      }
      blocker.EndBlock ();
      return  statsToReturn;
    }



    //  return to the caller current acumulated statistics
    //  then remove from my own internal list 'loadedStats'.
    public  PicesClassStatisticList  GetStatsAlreadyLoadedIfNew ()
    {
      PicesClassStatisticList statsToReturn = null;
      blocker.StartBlock (); 

      if  (newStatsAvailable)
      {
        if  (loadedStats != null)
        {
          statsToReturn = new PicesClassStatisticList ();
          foreach (PicesClassStatistic stat in loadedStats)
            statsToReturn.Add (stat);
        }
        newStatsAvailable = false;
      }

      blocker.EndBlock ();
      return  statsToReturn;
    }




    private  void  LoadStatsForSelectedGroup ()
    {
      PicesClassStatisticList  nextBunch = null;
      if  (allSipperFiles  ||  (!String.IsNullOrEmpty (sipperFileName)))
      {
        nextBunch = dbConn.ImageGetClassStatistics 
                      (selectedImageGroup,   // ImageGroup
                       sipperFileName,
                       mlClass,
                       classKeyToUse,
                       probMin,  probMax, 
                       sizeMin,  sizeMax, 
                       depthMin, depthMax
                      );
      }
      else
      {
        nextBunch = dbConn.ImageGetClassStatistics
                      (selectedImageGroup,   // ImageGroup
                       cruiseName,
                       stationName,
                       deploymentNum,
                       mlClass,
                       classKeyToUse,
                       probMin,  probMax, 
                       sizeMin,  sizeMax, 
                       depthMin, depthMax
                      );
      }

      if  (nextBunch == null)
        return;

      blocker.StartBlock ();
      if  (loadedStats == null)
        loadedStats = new PicesClassStatisticList ();
      loadedStats.Add (nextBunch);
      imageCountTotal += nextBunch.ImageCountTotal;
      newStatsAvailable = true;
      blocker.EndBlock ();

      nextBunch = null;
    }  /* LoadStatsForSelectedGroup */




    private  void  LoadStatsForOneSipperFile (String  sfn)
    {
      curSipperFileName = sfn;
      PicesClassStatisticList  nextBunch = dbConn.ImageGetClassStatistics 
                      (null,   // ImageGroup
                       sfn,
                       mlClass,
                       classKeyToUse,
                       probMin, probMax, 
                       sizeMin, sizeMax, 
                       depthMin, depthMax
                      );

      if  ((nextBunch == null)  ||  (nextBunch.Count < 1))
        return;

      blocker.StartBlock ();
      if  (loadedStats == null)
        loadedStats = new PicesClassStatisticList ();
      loadedStats.Add (nextBunch);
      imageCountTotal += nextBunch.ImageCountTotal;

      newStatsAvailable = true;
      blocker.EndBlock ();
    }  /* LoadStatsForOneSipperFile */
    


    private  void  LoadStatsThread ()
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

      sipperFilesCount = ((sipperFileNames == null) ? 0 : sipperFileNames.Length);

      if  (selectedImageGroup != null)
      {
        LoadStatsForSelectedGroup ();
      }
      else
      {
        if  (sipperFileNames != null)
        {
          for  (curSipperFileIdx = 0;  (curSipperFileIdx < sipperFileNames.Length)  &&  (!cancelRequested);  curSipperFileIdx++)
            LoadStatsForOneSipperFile (sipperFileNames[curSipperFileIdx]);
        }
      }

      if  (!cancelRequested)
      {
        volumeWaterSampled 
          = dbConn.InstrumentDataGetVolumePerMeterDepth (cruiseName,  
                                                         stationName,
                                                         deploymentNum,
                                                         1.0f
                                                        );
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
    }  /* LoadStatsThread */
  }  /* PlanktonStatisticsLoader */
}
