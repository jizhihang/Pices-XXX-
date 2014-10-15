using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Management;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using  PicesInterface;
using  SipperFile;

namespace PicesCommander
{
  /// <summary>
  ///**********************************************************************************
  ///* Developer:  Kurt Kramer                                                        *
  ///* Date        2009-Jan                                                           *
  ///*                                                                                *
  ///* My purpose in life is to Classify Images that meet specified criteria using    *
  ///* the current active TrainingModel2.   The user can opt to just generate the     *
  ///* classification report using current class assignments.                         *
  ///*                                                                                *
  ///*  Validated Class always trumps Predicted Class.                                *
  ///**********************************************************************************
  /// </summary>
  public partial class ClassificationStatus : Form
  {
    private  PicesCommander  manager  = null;

    private  uint                     numCpus = 0;

    private  PicesDataBaseImageGroup  imageGroup = null;
    private  String                   cruiseName     = null;
    private  String                   stationName    = null;
    private  String                   deploymentNum  = null;
    private  String                   sipperFileName = null;
    private  PicesClass               currentClass   = null;
    private  char                     classKeyToUse  = 'P';

    private  Boolean                  cancelProcessing = false;   // Set to true to stop processing NOW

    private  TrainingModel2           classifier = null;

    private  PicesClassList           classesInClassifier = null;

    private  PicesDataBase            mainWinConn = null;

    private  DateTime                 midPoint;  /**< Mid point in deploymnet,  the DateTIme from CTD when SIPPER is at its deapest. */

    private  Thread                   selectionThread          = null;
    private  bool                     selectionThreadRunning   = false;
    private  bool                     selectionProcessDone     = false;

    private  ClassifierProcess[]      classifiers              = null;
    private  uint                     classifiersCount         = 0;
    private  bool                     classifiersInitialized   = false;
    
    private  Thread                   predUpdateThread         = null;
    private  bool                     predUpdateThreadRunning  = false;
    private  bool                     predUpdateThreadDone     = false;

    private  bool                     validatedTrumps = true;
    
    private  bool                     reClassify = false;
     
    private  PicesRunLog              runLog = null;

    private  DateTime                 runTimeStart = DateTime.Now;
    private  DateTime                 runTimeEnd   = DateTime.Now;

    private  TimeSpan                 cpuTimeStart;
    private  TimeSpan                 cpuTimeEnd;


    private  PicesDataBaseLogEntry    logEntry = null;
    private  uint                     logEntryId = 0;

    private  String  reportFileName     = null;
    private  String  depthReprtFileName = null;

    private  float  depthMin = 0;
    private  float  depthMax = 0;
    private  float  probMin  = 0.0f;
    private  float  probMax  = 1.0f;
    private  int    sizeMin  = 0;
    private  int    sizeMax  = 0;

    private  bool   filterSize  = false;
    private  bool   filterProb  = false;
    private  bool   filterDepth = false;

    private  String  infinityStr = ((char)8734).ToString ();

    private  Queue<PredictionResult>     predictionUpdateQueue = null;
    private  Queue<PicesFeatureVector>   featureVectorQueue    = null;

    private Queue<String>  errorMsgs = new Queue<string> ();

    char[]  lockBuffer = new char[10];

    List<ClassStat>  stats = null;
    String  message = "";

    uint    numSipperFiles      = 0;
    uint    numSipperFilesRead  = 0;
    uint    numImagesRead       = 0;
    uint    numImagesSkipped    = 0;  /**<  Images that were skipped becuase they did not met the Min-Max parameters for Depth, Probability, Size */
    uint    numImagesClassified = 0;
    uint    numImagesUpdated    = 0;

    PicesSipperFileList  sipperFiles   = null;


    private   SizeDistribution      sizeDistribution        = null;
    private   SizeDistribution      depthDistribution_1     = null;
    private   SizeDistribution      depthDistribution_1Down = null;
    private   SizeDistribution      depthDistribution_1Up   = null;
    private   SizeDistribution      depthDistribution_10    = null;
    private   ClassStatSortedList   classCounts             = null;

    private   bool   allowUpdates = false;


    public ClassificationStatus (PicesCommander           _manager,
                                 PicesDataBaseImageGroup  _imageGroup,
                                 String                   _cruiseName,
                                 String                   _stationName,
                                 String                   _deploymentNum,
                                 String                   _sipperFileName,
                                 PicesClass               _currentClass,
                                 char                     _classKeyToUse,
                                 int                      _sizeMin,
                                 int                      _sizeMax,
                                 float                    _depthMin,
                                 float                    _depthMax,
                                 float                    _probMin,
                                 float                    _probMax
                                )
    {
      runLog = new PicesRunLog ();

      allowUpdates = PicesSipperVariables.AllowUpdates ();

      InitializeComponent ();
      manager = _manager;

      numCpus = GetNumberOfCores ();

      classifier = ActiveTrainingLibraries.Model1 ();
      if  (ActiveTrainingLibraries.ModelsAreLoading ())
        classifier = null;

      imageGroup     = _imageGroup;
      cruiseName     = _cruiseName;
      stationName    = _stationName;
      deploymentNum  = _deploymentNum;
      sipperFileName = _sipperFileName;

      currentClass   = _currentClass;
      classKeyToUse  = _classKeyToUse;

      sizeMin   = _sizeMin;
      sizeMax   = _sizeMax;
      depthMin  = _depthMin;
      depthMax  = _depthMax;
      probMin   = _probMin;
      probMax   = _probMax;

      if  ((sizeMin > 0)      &&  (sizeMax <= 0))      sizeMax  = int.MaxValue;
      if  ((probMin >= 0.0f)  &&  (probMax <= 0.0f))   probMax  = 1.0f;
      if  ((depthMin > 0.0f)  &&  (depthMax <= 0.0f))  depthMax = float.MaxValue;

      filterSize =  (sizeMin > 0)  ||  ((sizeMax > 0) && (sizeMax > sizeMin));
      filterProb  = ((probMin > 0.0f)  ||  (probMax < 1.0f))  &&  (probMax > probMin);
      filterDepth = ((depthMax > 0)  &&  (depthMax > depthMin));

      mainWinConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      if  ((classifier == null)  ||  (!classifier.Valid))
      {
        TrainingLibrary.Text = "";
        ReClassify.Checked = false;
        ReClassify.Enabled = false;
        classifier = null;
      }
      else
      {
        TrainingLibrary.Text = classifier.ConfigFileName;
        ReClassify.Checked = false;
        ReClassify.Enabled = allowUpdates;
      }

      Cruise.Text     = cruiseName;
      Station.Text    = stationName;
      Deployment.Text = deploymentNum;
      SipperFile.Text = sipperFileName;

      if  (currentClass == null)
        CurrentClass.Text = "";
      else
        CurrentClass.Text = currentClass.Name;

      if  (imageGroup != null)
        ImageGroup.Text = imageGroup.Name;

      if  (classifier != null)
      {
        ClassificationBiasMatrix bm = null;

        try  {bm = new ClassificationBiasMatrix (classifier.ConfigFileName, classifier.MLClasses (), runLog);}
        catch  (Exception)  {bm = null;}
        if  (bm != null)
          BiasMatrixDate.Text = bm.DateTimeFileWritten.ToString ("yyyy-MM-dd hh:mm:ss");
      }

      predictionUpdateQueue = new Queue<PredictionResult> ();
      featureVectorQueue    = new Queue<PicesFeatureVector> ();

      ValidatedTrumps.Checked = validatedTrumps;
      PopulateMinMaxFields ();

      classCounts = new ClassStatSortedList ();
    }


    private  uint  GetNumberOfCores ()
    {
      uint coreCount = 0;
      foreach (var item in new System.Management.ManagementObjectSearcher("Select * from Win32_Processor").Get())
      {
        coreCount += uint.Parse(item["NumberOfCores"].ToString());
      }

      if  (coreCount < 1)
        coreCount = (uint)Environment.ProcessorCount;

      return  coreCount;
    }


    public  void  CleanUpMemory ()
    {
    }


    private  String  ToCmdLine ()
    {
      String  cmdLine = "";

      if  (classifier != null)
        cmdLine += ("-Classfier " + OSservices.GetRootName (classifier.ConfigFileName));

      if  (mainWinConn == null)
        cmdLine += " -NoDatabase ";
      else
        cmdLine += (" -DataBase " + mainWinConn.Server.HostName);

      if  (imageGroup != null)
        cmdLine += (" -ImageGroupe " + imageGroup.Name);

      if  (!String.IsNullOrEmpty (cruiseName))
        cmdLine += (" -Cruise " + cruiseName);
      
      if  (!String.IsNullOrEmpty (stationName))
        cmdLine += (" -Station " + stationName);
        
      if  (!String.IsNullOrEmpty (deploymentNum))
        cmdLine += (" -Deployment " + deploymentNum);

      if  (!String.IsNullOrEmpty (sipperFileName))
        cmdLine += (" -SipperFile " + sipperFileName);

      if  (currentClass != null)
        cmdLine += (" -Class " + currentClass.Name);

      if  (classKeyToUse == 'V')
        cmdLine += (" -ValidatedOnly");

      if  (validatedTrumps)
        cmdLine += " -ValidatedTrumps";

      cmdLine += (" -ReClassify " + (reClassify ? "Yes" : "No"));

      if  ((depthMin > 0)  ||  (depthMax > 0))
        cmdLine += (" -DepthMin " + depthMin + " " + "-DepthMax " + depthMax);

      if  ((probMin > 0.0f)  ||  (probMax < 1.0f))
        cmdLine += (" -probMin " + probMin.ToString ("P") + " " + "-probMax " + probMax.ToString ("P"));

      if  ((sizeMin > 0)  ||  (sizeMax > 0))
        cmdLine += (" -SizeMin " + sizeMin + " " + "-SizeMax " + sizeMax);

      cmdLine = cmdLine.TrimStart (' ');

      return  cmdLine;
    }  /* ToCmdLine */



    private  void  MakeSureAllClassesExist (PicesDataBase  threadConn)
    {
      if  ((classifier == null)  ||  (threadConn == null))
        return;

      PicesClassList  classes = classifier.MLClasses ();
      foreach  (PicesClass pc in classes)
      {
        if  (pc.ClassId < 0)
        {
          PicesClass  newClass = threadConn.MLClassLoad (pc.Name);
          if  (newClass == null)
          {
            if  (String.IsNullOrEmpty (pc.ParentName))
              pc.Parent = PicesClassList.GetUniqueClass ("AllClasses", "");
            
            pc.Description = "Added by 'Classification'";

            threadConn.MLClassInsert (pc);
            newClass = threadConn.MLClassLoad (pc.Name);
          }
        }
      }
    }  /* MakeSureAllClassesExist */



    private  void  PopulateMinMaxFields ()
    {
      if  ((sizeMin > 0)  ||  (sizeMax > 0))
      {
        string   m;
        if  (sizeMin <= 0)
          m = "0";
        else
          m = sizeMin.ToString ();
        m += " - ";

        if  ((sizeMax <= 0)  ||  (sizeMax == int.MaxValue))
          m += infinityStr;
        else
          m += sizeMax.ToString ();
        SizeMinMax.Text = m;
      }

      if  ((depthMin > 0)  ||  (depthMax > 0))
      {
        string   m;
        if  (depthMin <= 0)
          m = "0";
        else
          m = depthMin.ToString ();
        m += " - ";

        if  ((depthMax <= 0)  ||  (depthMax == float.MaxValue))
          m += infinityStr;
        else
          m += depthMax.ToString ();
        DepthMinMax.Text = m;
      }

      if  (filterProb)
      {
        string   m = probMin.ToString ("P") + " - " + probMax.ToString ("P");
        ProbMinMax.Text = m;
      }
    }  /* PopulateMinMaxFields */


    
    private void  CancelProcesses ()
    {
      cancelProcessing = true;
      for  (int x = 0;  x < classifiersCount;  ++x)
      {
        if  (classifiers[x] != null)
          classifiers[x].ThreadCancel ();
      }
    }


    private  void  TerminateClassifierProcesses ()
    {
      while  (!classifiersInitialized)
      {
        // Wait until teClassification poesses are done with the StartUp cycle.
        Thread.Sleep (200);
      }

      for  (int x = 0;  x < classifiersCount;  ++x)
      {
        if  (classifiers[x] != null)
          classifiers[x].ThreadTerminate ();
      }
    }  /* TerminateClassifierProcesses */



    private  void  UpdateDisplay ()
    {
      lock  (lockBuffer)
      {
        SipperFilesRead.Text = numSipperFilesRead.ToString () + " - " + numSipperFiles.ToString ();
        ImagesRead.Text = numImagesRead.ToString ("###,###,##0");

        uint  awaitingClassification = 0;
        lock (featureVectorQueue)
        {awaitingClassification = (uint)featureVectorQueue.Count;}
        AwaitingClassification.Text = awaitingClassification.ToString ("###,###,##0");

        Classified.Text = numImagesClassified.ToString ("###,###,##0");

        uint  awaitingUpdate = 0;
        lock (predictionUpdateQueue)
        {awaitingUpdate = (uint)predictionUpdateQueue.Count;}
        AwaitingUpdate.Text = awaitingUpdate.ToString ("###,###,##0");

        Updated.Text = numImagesUpdated.ToString ("###,###,##0");

        ErrorsDetected.Text = errorMsgs.Count.ToString ("###,###,##0");

        bool  rebuildGrid = false;
        
        if  (stats == null)
          return;

        if  (stats.Count !=  ImagesClassifiedGrid.Rows.Count)
        {
          rebuildGrid = true;
        }
        else
        {
          int  idx = 0;
          foreach  (ClassStat cs in stats)
          {
            if  ((String)(ImagesClassifiedGrid.Rows[idx].Cells[0].Value) != stats[idx].mlClass.Name)
            {
              rebuildGrid = true;
              break;
            }

            ImagesClassifiedGrid.Rows[idx].Cells[1].Value = stats[idx].count.ToString ("###,##0");
            idx++;
          }
        }
        
        if  (rebuildGrid)
        {
          ImagesClassifiedGrid.Rows.Clear ();
          //stats.Sort ();
          int  idx = 0;
          foreach  (ClassStat cs in stats)
          {
            object[] row = new object[2];
            row[0] = cs.mlClass.Name;
            row[1] = cs.count.ToString ("###,##0");
            ImagesClassifiedGrid.Rows.Add (row);
            ImagesClassifiedGrid.Rows[idx].Cells[1].Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
            idx++;
          }
        }
      }
    }  /* UpdateDisplay */



    private  void  UpdateDisplayVariables ()
    {
      if  (!classifiersInitialized)
        return;
      lock  (lockBuffer)
      {
        numImagesClassified = 0;
        classCounts = new ClassStatSortedList ();
        for  (int x = 0;  x < classifiersCount;  ++x)
          classifiers[x].ContributeOurClassCounts (classCounts, ref numImagesClassified);
       
        if  (stats == null)
          stats = new List<ClassStat> ();
        else
          stats.Clear ();

        foreach  (KeyValuePair<String, ClassStat>  cs in classCounts)
        {
          stats.Add ((ClassStat)cs.Value);
        }
      }
    }  /* UpdateDisplayVariables */


    private DateTime ExecutableDateTimeStamp ()
    {
      DateTime  compilationDateTime;
      System.IO.FileInfo fi = new System.IO.FileInfo(Application.ExecutablePath.Trim());
      try
      {
        compilationDateTime =  fi.LastWriteTime;
      }
      catch (Exception)
      {
        compilationDateTime = DateTime.Now;
      }
      finally
      {
        fi = null;
      }

      return  compilationDateTime;
    }


    
    private  void  StartClassificationThreads ()
    {
      String  sipperRootName = "";
      if  (!String.IsNullOrEmpty (sipperFileName))
        sipperRootName = OSservices.GetRootName (sipperFileName);

      logEntry = mainWinConn.LogEntriesProcessStart ("CL", 
                                                     "ClassificationStatus",  
                                                     ExecutableDateTimeStamp (),
                                                     ToCmdLine (),
                                                     sipperRootName
                                                    );
      if  (logEntry != null)
        logEntryId = logEntry.LogEntryId;

      cpuTimeStart = Process.GetCurrentProcess ().TotalProcessorTime;

      MakeSureAllClassesExist (mainWinConn);

      predUpdateThread = new Thread (new ThreadStart (PredictionsUpdateProcess));
      predUpdateThread.Name = "PredictionUpdate";
      predUpdateThread.Start ();
      predUpdateThreadRunning = true;

      selectionThread = new Thread (new ThreadStart (SelectionProcess));
      selectionThread.Name = "ImageSelection";
      selectionThread.Start ();
      selectionThreadRunning = true;

      if  (PicesDataBase.EmbeddedServerRunning ())
        classifiersCount = Math.Min (2, numCpus);

      else
      {
        classifiersCount = Math.Min (5, Math.Max (1, numCpus));
      }

      //classifiersCount = 1;

      PicesDataBaseServer dbServer = null;
      if  (mainWinConn != null)
        dbServer = mainWinConn.Server;

      classifiers = new ClassifierProcess[classifiersCount];
      for  (int x = 0;  x < classifiersCount;  ++x)
      {
        if  (x == 0)
        {
          classifiers[x] = new ClassifierProcess (dbServer,
                                                  x, 
                                                  classifier,
                                                  reClassify, 
                                                  validatedTrumps, 
                                                  predictionUpdateQueue, 
                                                  featureVectorQueue,
                                                  midPoint,
                                                  errorMsgs
                                                 );
        }
        else
        {
          Thread.Sleep (5000);
          classifiers[x] = new ClassifierProcess (dbServer,
                                                  x, 
                                                  (classifier == null) ? "" : classifier.ModelName,
                                                  reClassify, 
                                                  validatedTrumps, 
                                                  predictionUpdateQueue, 
                                                  featureVectorQueue,
                                                  midPoint,
                                                  errorMsgs
                                                 );
        }

        classifiers[x].StartThread ();
      }
      classifiersInitialized = true;

      timer1.Enabled = true;
    }  /* StartClassificationThreads */



    private  bool  ClassifierProcessesAreDone ()
    {
      for  (int x = 0;  x < classifiersCount;  ++x)
      {
        if  (!classifiers[x].ThreadDone)
          return false;
      }
      return true;
    }  /* ClassifierProcessesAreDone */



    private  void  SelectionProcess ()
    {
      PicesDataBase.ThreadInit ();

      PicesRunLog  threadRunLog = new PicesRunLog ();

      PicesDataBase  threadConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (threadRunLog);

      runTimeStart = DateTime.Now;

      if  (classifier != null)
        classesInClassifier = classifier.MLClasses ();

      selectionThreadRunning = true;

      sipperFiles = null;
      if  (!String.IsNullOrEmpty (sipperFileName))
      {
        PicesSipperFile sf = threadConn.SipperFileRecLoad (sipperFileName);
        if  (sf != null)
        {
          sipperFiles = new PicesSipperFileList ();
          sipperFiles.Add (sf);
        }
      }
      else 
      {
        sipperFiles = threadConn.SipperFileLoad (cruiseName, stationName, deploymentNum);
      }

      if  (imageGroup == null)
        ProcessBySipperFiles (threadConn);
      else
        ProcessByImageGroup (threadConn);

      // Now that we have selected all the images that need to be classified we need to wait for 
      // the classification processes to be done before we can generate the reports.

      TerminateClassifierProcesses ();  //  This tells the ClassifierProcess threads to terminate when they are done with what is on the queue.
      while  (!ClassifierProcessesAreDone ()  &&  !cancelProcessing)
      {
        Thread.Sleep (1000);
      }

      runTimeEnd = DateTime.Now;
      
      if  (!cancelProcessing)
        PrintReport (threadConn);

      threadConn.Close ();
      threadConn = null;
      threadRunLog = null;
      GC.Collect ();

      PicesDataBase.ThreadEnd ();

      selectionThreadRunning = false;
      selectionProcessDone   = true;
    }  /* SelectionProcess */



    private  void  ProcessBySipperFiles (PicesDataBase  threadConn)
    {
      if  (sipperFiles == null)
      {
        MessageBox.Show (this, "No SIPPER Files found for given criteria");
        selectionThreadRunning = false;
        return;
      }

      numSipperFiles  = (uint)sipperFiles.Count;

      foreach  (PicesSipperFile  sipperFile in sipperFiles)
      {
        while  ((featureVectorQueue.Count > 50000)  &&  (!cancelProcessing))
        {
          Thread.Sleep (100);
        }
        
        if  (cancelProcessing)
          break;

        ScanOneSipperFile (threadConn, sipperFile);
        numSipperFilesRead++;
      }    
    }  /* ProcessBySipperFiles */
  



    private  void  ScanOneSipperFile (PicesDataBase    threadConn,
                                      PicesSipperFile  sf
                                     )
    {
      message = "Loading FeatureData for Sipper File[" + sf.SipperFileName + "]";
      PicesFeatureVectorList  examples = threadConn.FeatureDataGetOneSipperFile 
                                                  (OSservices.GetRootName (sf.SipperFileName), 
                                                   currentClass, 
                                                   classKeyToUse, 
                                                   false
                                                  );
      if  (examples == null)
        return;
     
      message = "Classifying Sipper File[" + sf.SipperFileName + "]   Number of Images[" + examples.Count.ToString ("###,##0") + "]";

      ClassifyFeatureVectorList ("SipperFile[" + sf.SipperFileName + "]"  ,examples);

      examples = null;
      GC.Collect ();
    }  /* ScanOneSipperFile*/


        
    private  void  ProcessByImageGroup (PicesDataBase  threadConn)
    {
      // Since we are not retrieving data by Sipper files there will be no scanLinesPer meter to work with  
      // so we will create a one that will be be updated with images;  assuming that there are 4096 scan 
      // lines per image at what ever depth that image was at.

      numSipperFiles = 1;

      message = "Loading FeatureData for ImageGroup[" + imageGroup.Name + "]";

      PicesFeatureVectorList  examples = threadConn.FeatureDataForImageGroup (imageGroup, currentClass, classKeyToUse);
      if  (examples == null)
        return;

      message = "Classifying Image Group[" + imageGroup.Name + "]   Number of Images[" + examples.Count.ToString ("###,##0") + "]";

      if  (!cancelProcessing)
      {
        bool  allSipperFiles = String.IsNullOrEmpty (cruiseName)  &&  String.IsNullOrEmpty (sipperFileName);
        if  (!allSipperFiles)
        {
          // We only want images from a given set of sipper files.
          PicesFeatureVectorList  filteredList = new PicesFeatureVectorList ();

          String[]  sipperFileNames = null;

          if  (String.IsNullOrEmpty (sipperFileName))
          {
            sipperFileNames = threadConn.SipperFileGetList (cruiseName, stationName, deploymentNum);
          }
          else
          {
            sipperFileNames = new String[1];
            sipperFileNames[0] = sipperFileName;
          }

          if  (sipperFiles != null)
          {
            Dictionary<String, String>  sipperFileDictionary = new Dictionary<string,string> ();
            foreach  (String sfn in sipperFileNames)
              sipperFileDictionary.Add (sfn, sfn);

            foreach  (PicesFeatureVector fv in examples)
            {
              String sfn = PicesMethods.SipperFileNameFromImageFileName (fv.ImageFileName);
              if  (sipperFileDictionary.ContainsKey (sfn))
                filteredList.Add (fv);
            }
          }
          examples = filteredList;
        }

        ClassifyFeatureVectorList ("Image Group[" + imageGroup.Name + "]", examples);
      }

      numSipperFilesRead++;

      examples = null;
    }  /* ProcessByImageGroup */
  


    PicesClass  AssignVallidatedClassAsPredictedClass (PicesFeatureVector   fv)
    {
      PicesClass  validatedClass = fv.MLClass;
      if  ((validatedClass == null)  ||  (classesInClassifier == null))
        return  validatedClass;

      if  (classesInClassifier.LookUpIndex (validatedClass) >= 0)
        return  validatedClass;
      
      // Lets see if the Validated Class is a decendent of one of the classes in the Classifier.
      PicesClass  candidate = null;
      int         numGenerations = 99999;

      foreach  (PicesClass pc in classesInClassifier)
      {
        int zed = pc.NumGenerations (validatedClass);
        if  (zed >= 0)
        {
          if  (zed < numGenerations)
          {
            candidate = pc;
            numGenerations = zed;
          }
        }
      }

      if  (candidate != null)
        return  candidate;

      return  validatedClass;
    }  /* AssignVallidatedClassAsPredictedClass */




    /// <summary>Clasifies the contents of the supplied FeatureVectorList;</summary>
    /// <param name="desc"></param>
    /// <param name="examples">Feature vectors to be classified.</param>
    private  void  ClassifyFeatureVectorList (String                  desc,
                                              PicesFeatureVectorList  examples
                                             )
    {
      uint  numImages = (uint)examples.Count;

      int  numImagesReadThisFeatureVector = 0;

      foreach  (PicesFeatureVector  fv in examples)
      {
        if  (cancelProcessing)
          break;
        
        numImagesReadThisFeatureVector++;

        float  origDepth = fv.Depth;  // We need to remember size before it gets normalized.

        bool  passFilter = true;
        if  (filterDepth)
        {
          float depth = fv.Depth;
          passFilter = (depth >= depthMin)  &&  (depth <= depthMax);
        }

        if  (passFilter  &&  filterProb)
        {
          float prob = fv.Probability;
          passFilter = (prob >= probMin)  &&  (prob <= probMax);
        }

        if  (passFilter  &&  filterSize)
        {
          float size = fv.OrigSize;
          if  (size < sizeMin)
            passFilter = false;
          else
            passFilter = (sizeMax <= 0) || (size <= sizeMax);
        }

        if  (!passFilter)
        {
          ++numImagesSkipped;
        }
        else
        {
          lock  (featureVectorQueue)
          {
            featureVectorQueue.Enqueue (fv);
            ++numImagesRead;
          }
        }
      }
    }  /* ClassifyFeatureVectorList */



        
    private void  PredictionsUpdateProcess ()
    {
      predUpdateThreadRunning = true;

      PicesDataBase.ThreadInit ();
      PicesRunLog  runLog = new PicesRunLog ();

      PicesDataBase  dbUpdateConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      while  (!cancelProcessing)
      {
        int  queueSize = 0;
        lock  (predictionUpdateQueue)
        {
          queueSize = predictionUpdateQueue.Count;
        }
        if  (queueSize == 0)
        {
          if  (selectionProcessDone)
            break;
          Thread.Sleep (10);
        }
        else
        {
          uint numUpdatedThisSqlCall = 0;
          String  updateStr = "";
          lock (predictionUpdateQueue)
          {
            int  numToUpdate = Math.Min (50, queueSize);
            for  (int x = 0;  x < numToUpdate;  ++x)
            {
              if  (x > 0)
                updateStr += "\n";
              PredictionResult pr = predictionUpdateQueue.Dequeue ();
              updateStr += pr.imageRootName          + "\t" +
                           pr.class1Id.ToString   () + "\t" +
                           pr.class1Prob.ToString () + "\t" +
                           pr.class2Id.ToString   () + "\t" +
                           pr.class2Prob.ToString ();
              ++numUpdatedThisSqlCall;
            }
          }

          if  (numUpdatedThisSqlCall > 0)
            dbUpdateConn.ImagesUpdatePredictionsList (logEntryId, updateStr);

          numImagesUpdated += numUpdatedThisSqlCall;
        }
      }

      dbUpdateConn.Close ();
      dbUpdateConn = null;
      runLog = null;
      GC.Collect ();

      PicesDataBase.ThreadEnd ();
      
      predUpdateThreadRunning = false;
      predUpdateThreadDone    = true;
    }  /* PredictionsUpdateProcess */



    private  void  GetClassificationStats ()
    {
      sizeDistribution        = new  SizeDistribution ( 50, 100);
      depthDistribution_1     = new  SizeDistribution (500,   1);
      depthDistribution_1Down = new  SizeDistribution (500,   1);
      depthDistribution_1Up   = new  SizeDistribution (500,   1);
      depthDistribution_10    = new  SizeDistribution ( 50,  10);
      numImagesClassified     = 0;

      for  (int x = 0;  x < this.classifiersCount;  ++x)
      {
        classifiers[x].UpdateClassificationStats (sizeDistribution, 
                                                  depthDistribution_1,
                                                  depthDistribution_1Down,
                                                  depthDistribution_1Up,
                                                  depthDistribution_10,
                                                  ref numImagesClassified
                                                 );
      }
    }  /* GetClassificationStats */



    private  void  PrintReportRunTimeParameters (PicesDataBase           threadConn,
                                                 System.IO.StreamWriter  o
                                                )
    {
      if  (threadConn == null)
        o.WriteLine ("Database" + "\t" + "*** NO DATABASE SEPCIFIED ***");
      else
        o.WriteLine ("Database" + "\t" + threadConn.Server.ToString ());

      o.WriteLine ("MachineName"    + "\t" + System.Environment.MachineName);
      o.WriteLine ("UserName"       + "\t" + System.Environment.UserName);
      o.WriteLine ("UserDomainName" + "\t" + System.Environment.UserDomainName);
     
      if  (classifier == null)
      {
        o.WriteLine ("Configuration File" + "\t" + "None Was Specified");
      }
      else
      {
        o.WriteLine ("Configuration File" + "\t" + classifier.ConfigFileName);
        o.WriteLine ("BuildDateTime"      + "\t" + classifier.BuildDateTime.ToString ("yyyy/MM/dd HH:mm:ss"));
        o.WriteLine ("Parameters"         + "\t" + classifier.ParameterStr);
        o.WriteLine ("ImagesPerClass"     + "\t" + classifier.ImagesPerClass);
      }

      o.WriteLine ();

      o.WriteLine ("Re-Classify"      + "\t" + (reClassify      ? "Yes" : "No"));
      o.WriteLine ("Validated Trumps" + "\t" + (validatedTrumps ? "Yes" : "No"));

      if  (imageGroup != null)
        o.WriteLine ("Group Name" + "\t" + imageGroup.Name);
        
      o.WriteLine ("Cruise Name"  + "\t" + cruiseName);
      if  (!String.IsNullOrEmpty (stationName))
        o.WriteLine ("Station Name" + "\t" + stationName);

      if  (!String.IsNullOrEmpty (deploymentNum))
        o.WriteLine ("Deployment Num" + "\t" + deploymentNum);
 
      if  (!String.IsNullOrEmpty (sipperFileName))
        o.WriteLine ("SipperFile Name" + "\t" + sipperFileName);

      if  (currentClass != null)
        o.WriteLine ("From Class" + "\t" + currentClass.Name);

      o.WriteLine ("Class Key" + "\t" + classKeyToUse);

      o.WriteLine ("Size Filter"  + "\t" + SizeMinMax.Text);
      o.WriteLine ("Depth Filter" + "\t" + DepthMinMax.Text);
      o.WriteLine ("Prob Filter"  + "\t" + ProbMinMax.Text);

      o.WriteLine ("Start Time" + "\t" + runTimeStart.ToString ("yyyy/MM/dd - HH:mm:ss"));
      o.WriteLine ("End Time" + "\t"   + runTimeEnd.ToString  ("yyyy/MM/dd - HH:mm:ss"));
      TimeSpan  elaspeTime = runTimeEnd - runTimeStart;
      o.WriteLine ("Elasped Time" + "\t" + elaspeTime.ToString ());

      if  (errorMsgs.Count > 0)
      {
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ("Errors Detected" + "\t" + errorMsgs.Count.ToString ("###,###,##0"));
        o.WriteLine ();
        foreach (String s in errorMsgs)
          o.WriteLine (s);
        o.WriteLine ();
      }
    }  /* PrintReportRunTimeParameters */
 
 

    private  void  PrintReport (PicesDataBase threadConn)
    {
      GetClassificationStats ();

      String  reportDir = OSservices.AddSlash (OSservices.AddSlash (PicesSipperVariables.PicesReportDir ()) + "ClassificationResults");

      OSservices.CreateDirectoryPath (reportDir);

      String  baseReportName = reportDir + "ClassificationReport";

      List<String>  subNames = new List<string> ();
      if  (imageGroup != null)
        subNames.Add ("Group-" + imageGroup.Name);

      if  (!String.IsNullOrEmpty (cruiseName))
        subNames.Add ("Cruise-" + cruiseName);

      if  (!String.IsNullOrEmpty (stationName))
        subNames.Add ("Station-" + stationName);

      if  (!String.IsNullOrEmpty (deploymentNum))
        subNames.Add ("Deployment-" + deploymentNum);

      if  (!String.IsNullOrEmpty (sipperFileName))
        subNames.Add ("SipperFile-" + sipperFileName);

      foreach (String s in subNames)
        baseReportName += "_" + s;

      DateTime  dt = DateTime.Now;

      String  timeStr = dt.ToString ("yyyyMMdd-HHmmss");

      baseReportName = baseReportName + "_" + timeStr;
      
      message = "Retrieving Scan Lines per Meter of Depth";
      List<uint>  totalScanLinesPerMeter     = null;
      List<uint>  totalScanLinesPerMeterDown = null;
      List<uint>  totalScanLinesPerMeterUp   = null;
      GetScanLinesPerMeterProfile (threadConn, ref totalScanLinesPerMeter, ref totalScanLinesPerMeterDown, ref totalScanLinesPerMeterUp);
     
      message = "Retrieving Volume per Meter of Depth";

      List<InstrumentStatsByDepth>  totalVolumePerMeter     = null;
      List<InstrumentStatsByDepth>  totalVolumePerMeterDown = null;
      List<InstrumentStatsByDepth>  totalVolumePerMeterUp   = null;
      GetVolumePerMeterProfile (threadConn, ref totalVolumePerMeter, ref totalVolumePerMeterDown, ref totalVolumePerMeterUp);

      reportFileName     = baseReportName + ".txt";
      depthReprtFileName = baseReportName + "_Depth.txt";

      {
        System.IO.StreamWriter  o = new System.IO.StreamWriter (reportFileName);

        o.WriteLine ("Classification Report");
        o.WriteLine ();

        PrintReportRunTimeParameters (threadConn, o);

        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ("Size Distribution");
        o.WriteLine ();
        sizeDistribution.PrintTabDelDistributionMatrix (o);
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();

        o.WriteLine ("Abundance Distribution   - Class, Depth by 1 Meter Increments");
        o.WriteLine ();
        depthDistribution_1.PrintByClassCollumns (o, totalScanLinesPerMeter, totalVolumePerMeter, false);
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();

        o.WriteLine ("Abundance Density Distribution  - Class, Depth by 1 Meter Increments");
        o.WriteLine ();
        depthDistribution_1.PrintByClassCollumns (o, totalScanLinesPerMeter, totalVolumePerMeter, true);
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();

        o.WriteLine ("DownCast"  + "\t\t" + "Down-Cast"  + "\t\t" + "Down-Cast"  + "\t\t" + "Down-Cast"  + "\t\t" + "Down-Cast"  + "\t\t" + "Down-Cast");
        o.WriteLine ("Abundance Density Distribution  - Class, Depth by 1 Meter Increments       Down-Cast");
        o.WriteLine ();
        depthDistribution_1Down.PrintByClassCollumns (o, totalScanLinesPerMeterDown, totalVolumePerMeterDown, true);
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        
        o.WriteLine ("Up-Cast"  + "\t\t" + "Up-Cast"  + "\t\t" + "Up-Cast"  + "\t\t" + "Up-Cast"  + "\t\t" + "Up-Cast"  + "\t\t" + "Up-Cast");
        o.WriteLine ("Abundance Density Distribution  - Class, Depth by 1 Meter Increments      Up-Cast");
        o.WriteLine ();
        depthDistribution_1Up.PrintByClassCollumns (o, totalScanLinesPerMeterUp, totalVolumePerMeterUp, true);
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        
        o.WriteLine ("Abundance Distribution   - Class, Depth by 10 Meter Increments");
        o.WriteLine ();
        depthDistribution_10.PrintByClassCollumns (o, totalScanLinesPerMeter, totalVolumePerMeter, false);
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
                
        o.WriteLine ("Abundance Density Distribution  - Class, Depth by 10 Meter Increments");
        o.WriteLine ();
        depthDistribution_10.PrintByClassCollumns (o, totalScanLinesPerMeter, totalVolumePerMeter, true);
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ();

        PrintAbundanceAdjustedResults (o);
        o.WriteLine ();
        o.WriteLine ();
        PrintBiasAdjustedResults  (o);

        o.Close ();
      }

      {
        // Depth Report 
        System.IO.StreamWriter  o = new System.IO.StreamWriter (depthReprtFileName);

        PrintReportRunTimeParameters (threadConn, o);

        o.WriteLine ("Abundance Distribution   - Class, Depth by 1 Meter Increments");
        o.WriteLine ();
        depthDistribution_1.PrintByClassCollumns (o, totalScanLinesPerMeter, totalVolumePerMeter, false);
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ("Abundance Density Distribution  - Class, Depth by 1 Meter Increments");
        o.WriteLine ();
        depthDistribution_1.PrintByClassCollumns (o, totalScanLinesPerMeter, totalVolumePerMeter, true);
        o.WriteLine ();

        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ("Abundance Distribution   - Class, Depth by 10 Meter Increments");
        o.WriteLine ();
        depthDistribution_10.PrintByClassCollumns (o, totalScanLinesPerMeter, totalVolumePerMeter, false);
        o.WriteLine ();
        o.WriteLine ();
        o.WriteLine ("Abundance Density Distribution  - Class, Depth by 10 Meter Increments");
        o.WriteLine ();
        depthDistribution_10.PrintByClassCollumns (o, totalScanLinesPerMeter, totalVolumePerMeter, true);
        
        o.Close ();
      }
    }  /* PrintReport */


    private  void  PrintAbundanceAdjustedResults (StreamWriter  sw)
    {
      if  (classifier == null)
      {
        // No classifier;  can not do a Bias Adjustment.
        sw.WriteLine ();
        sw.WriteLine ();
        sw.WriteLine ("No Classifier     Can not do a Abundance Adjustment.");
        sw.WriteLine ();
        sw.WriteLine ();
        return;
      }

      PicesClassStatisticList  classificationResults = classCounts.ToPicesClassStatisticList ();
      try
      {
        PicesAbundanceCorrectionMatrix  aam = new PicesAbundanceCorrectionMatrix (classifier.ConfigFileName, runLog);
        if  (!aam.Valid ())
        {
          sw.WriteLine ();
          sw.WriteLine ();
          sw.WriteLine ("Abundance adjustment matrix is not valid for Classifier[" + classifier.ConfigFileName + "].");
          sw.WriteLine ();
          sw.WriteLine ();
          return;
        }
        else
        {
          aam.PrintAdjustedResults (sw, classificationResults, runLog);
        }
      }
      catch  (Exception  e2)
      {
        sw.WriteLine ();
        sw.WriteLine ("**** Exception performing abundance adjusment *****");
        sw.WriteLine ();
        sw.WriteLine (e2.ToString ());
        sw.WriteLine ();
      }

      sw.Flush ();
    }  /* PrintAbundanceAdjustedResults */



    
    private  void  PrintBiasAdjustedResults  (StreamWriter  sw)
    {
      if  (classifier == null)
      {
        // No classifier;  can not do a Bias Adjustment.
        sw.WriteLine ();
        sw.WriteLine ();
        sw.WriteLine ("No Classifier     Can not do a Classifier Adjustment.");
        sw.WriteLine ();
        sw.WriteLine ();
        return;
      }
      
      PicesClassList  masterClassList = classifier.MLClasses ();
      masterClassList.SortByName ();
      double[]   observedCounts = new double[masterClassList.Count];

      PicesClassStatisticList list = classCounts.ToPicesClassStatisticList ();
      foreach (PicesClassStatistic cs in list)
      {
        int  idx = masterClassList.IndexOf (cs.MLClass);
        if  (idx < 0)
        {
          sw.WriteLine ();
          sw.WriteLine ("******  Class[" + cs.MLClass.Name + "]  Count[" + cs.Count + "]  is not part of the classifier. ********");
          sw.WriteLine ();
        }
        else
        {
          observedCounts[idx] = (double)cs.Count;
        }
      }

      try
      {
        ClassificationBiasMatrix  cbm = new ClassificationBiasMatrix (classifier.ConfigFileName, masterClassList, runLog);

        if  (!cbm.Valid ())
        {
          sw.WriteLine ();
          sw.WriteLine ("***** NO VALID BIAS MATRIX *****");
          sw.WriteLine ();
        }
        else
        {
          cbm.PrintBiasMatrix (sw);
          cbm.PrintAdjustedResults (sw, observedCounts);
        }
      }
      catch  (Exception  e2)
      {
        sw.WriteLine ();
        sw.WriteLine ();
        sw.WriteLine ("***** NO VALID BIAS MATRIX *****");
        sw.WriteLine ();
        sw.WriteLine ("Exception Buiolding Bias Matrix[" + e2.ToString () + "]");
        sw.WriteLine ();
      }
    }  /* PrintBiasAdjustedResults */


    /// <summary>
    /// Builds an array that represents the time spent at each depth increment.
    /// This will be done by scanning the InstrumentData for all the sipper files involved
    /// and counting the number of scan lines at each depth.
    /// </summary>
    /// <returns>
    /// Array where each element represents the number of scan lines for the depth; ex the
    /// array index '1' would be the number of scan lines that ccured at greater or equal 1.0
    /// and less than 2.0.
    /// </returns>
    private List<uint>   GetScanLinesPerMeterProfile (PicesDataBase   threadConn,       
                                                      ref List<uint>  depthTimeProfile,
                                                      ref List<uint>  depthTimeProfileDown,
                                                      ref List<uint>  depthTimeProfileUp
                                                     )
    {
      message = "Retrieving Time/Meter";

      if  (sipperFiles == null)
        return null;

      depthTimeProfile     = new List<uint> ();
      depthTimeProfileDown = new List<uint> ();
      depthTimeProfileUp   = new List<uint> ();

      foreach  (PicesSipperFile sf in sipperFiles)
      {
        if  (cancelProcessing)
          break;

        String  sipperFileName = sf.SipperFileName;

         message = "Loading InstrumentData   Sipper File[" + sipperFileName + "]";

        PicesInstrumentDataList  data = threadConn.InstrumentDataLoad (sipperFileName);
        if  ((data == null)  ||  (data.Count < 1))
        {
          message = "Loading InstrumentData   Sipper File[" + sipperFileName + "]  NO DATA LOADED" + "\n";
        }
        else
        {
          message = "Loading InstrumentData   Sipper File[" + sipperFileName + "]  [" + data.Count.ToString ("###,##0") + "] Entries loaded";
          uint  lastScanLine = data[0].ScanLine;
          for  (int idx = 1;  idx < data.Count;  idx++)
          {
            PicesInstrumentData id = data[idx];
            uint  scanLine = id.ScanLine;
            uint  deltaScanLines = scanLine - lastScanLine;

            int  depthIdx = (int)Math.Floor (id.Depth);
            if  (depthIdx < 0)
              depthIdx = 0;
            else if  (depthIdx >= 3000)
              depthIdx = 3000;

            while  (depthTimeProfile.Count <= depthIdx)
            {
              depthTimeProfile.Add     (0);
              depthTimeProfileDown.Add (0);
              depthTimeProfileUp.Add   (0);
            }

            depthTimeProfile[depthIdx] += deltaScanLines;
            if  (id.CtdDateTime < midPoint)
              depthTimeProfileDown[depthIdx] += deltaScanLines;
            else
              depthTimeProfileUp  [depthIdx] += deltaScanLines;

            lastScanLine = scanLine;
          }
        }
      }

      return  depthTimeProfile;
    }  /* GetScanLinesPerMeterProfile */



    private void  GetVolumePerMeterProfile (PicesDataBase                     threadConn,
                                            ref List<InstrumentStatsByDepth>  totalVolumePerMeter,
                                            ref List<InstrumentStatsByDepth>  totalVolumePerMeterDown,
                                            ref List<InstrumentStatsByDepth>  totalVolumePerMeterUp
                                           )
    {
      message = "Deriving Volume/Meter Profile";

      if  (sipperFiles == null)
        return;

      totalVolumePerMeter     = new List<InstrumentStatsByDepth> ();
      totalVolumePerMeterDown = new List<InstrumentStatsByDepth> ();
      totalVolumePerMeterUp   = new List<InstrumentStatsByDepth> ();

      String[]  cols = {"DownCast",               // 0
                        "BinId",                  // 1
                        "BinDepth",               // 2
                        "ScanLines",              // 3
                        "VolumeSampled",          // 4
                        "TemperatureMean",        // 5
                        "SalinityMean",           // 6
                        "DenisityMean",            // 7
                        "FluorescenceMean",       // 8
                        "FluorescenceSensorMean", // 9
                        "OxygenMean",             // 10
                        "DepthMean",              // 11
                        "TransmisivityMean",      // 12
                        "TurbidityMean",          // 13
                        "CdomFluorescenceMean"    // 14
                      };

      String  sqlCmd = "call InstrumentDataBinByMeterDepth (\"" + this.cruiseName + "\", \"" + this.stationName + "\", \"" + this.deploymentNum + "\", 1.0)";
      String[][]  results = threadConn.QueryStatement (sqlCmd, cols);
      if  (results == null)
        return;

      for  (int row = 0;  row < results.Length;  ++row)
      {
        float   cdomFluorescenceMean   = 0.0f;

        int  numCols = results[row].Length;
        if  (numCols < 14)
          continue;
        bool    downCast               = PicesKKStr.StrToBool  (results[row][0]);
        int     binId                  = PicesKKStr.StrToInt   (results[row][1]);
        float   binDepth               = PicesKKStr.StrToFloat (results[row][2]);
        float   scanLines              = PicesKKStr.StrToFloat (results[row][3]);
        float   volumeSampled          = PicesKKStr.StrToFloat (results[row][4]);
        float   temperatureMean        = PicesKKStr.StrToFloat (results[row][5]);
        float   salinityMean           = PicesKKStr.StrToFloat (results[row][6]);
        float   densityMean            = PicesKKStr.StrToFloat (results[row][7]);
        float   fluorescenceMean       = PicesKKStr.StrToFloat (results[row][8]);
        float   fluorescenceSensorMean = PicesKKStr.StrToFloat (results[row][9]);
        float   oxygenMean             = PicesKKStr.StrToFloat (results[row][10]);
        float   depthMean              = PicesKKStr.StrToFloat (results[row][11]);
        float   transmisivityMean      = PicesKKStr.StrToFloat (results[row][12]);
        float   turbidityMean          = PicesKKStr.StrToFloat (results[row][13]);
        if  (numCols > 14)
          cdomFluorescenceMean   = PicesKKStr.StrToFloat (results[row][14]);

        InstrumentStatsByDepth  isbd 
          = new InstrumentStatsByDepth (binId, binDepth, scanLines, volumeSampled, 
                                        temperatureMean, 
                                        salinityMean, 
                                        densityMean,
                                        fluorescenceMean,
                                        fluorescenceSensorMean,
                                        oxygenMean, 
                                        depthMean, 
                                        transmisivityMean, 
                                        turbidityMean, 
                                        cdomFluorescenceMean
                                       );

        while  (totalVolumePerMeter.Count <= binId)
          totalVolumePerMeter.Add (new InstrumentStatsByDepth (totalVolumePerMeter.Count));
        while  (totalVolumePerMeterDown.Count <= binId)
          totalVolumePerMeterDown.Add (new InstrumentStatsByDepth (totalVolumePerMeterDown.Count));
        while  (totalVolumePerMeterUp.Count <= binId)
          totalVolumePerMeterUp.Add (new InstrumentStatsByDepth (totalVolumePerMeterUp.Count));

        totalVolumePerMeter[binId].Add (isbd);

        if  (downCast)
          totalVolumePerMeterDown[binId].Add (isbd);
        else
          totalVolumePerMeterUp[binId].Add (isbd);
      }

      for  (int x = 0;  x < totalVolumePerMeter.Count;  ++x)
        totalVolumePerMeter    [x].ComputeMean ();

      for  (int x = 0;  x < totalVolumePerMeterDown.Count;  ++x)
        totalVolumePerMeterDown[x].ComputeMean ();
  
      for  (int x = 0;  x < totalVolumePerMeterUp.Count;  ++x)
        totalVolumePerMeterUp  [x].ComputeMean ();
    }  /* GetVolumePerMeterProfile */




    private  void  ClassificationDone ()
    {
      DialogResult dr = MessageBox.Show (this,
                                         "Classification Process Completed." + "\n" +
                                         "\n" +
                                         "Do you want to open the report ?",
                                         "Classification Process",
                                         MessageBoxButtons.YesNo
                                        );
      if  (dr == DialogResult.No)
        return;

      Process proc = new Process();
     
      proc.StartInfo.WorkingDirectory = @"";
//      proc.StartInfo.FileName = "Excell";
//      proc.StartInfo.Arguments = reportFileName;
      proc.StartInfo.FileName = reportFileName;
//      proc.StartInfo.Arguments = reportFileName;
      proc.StartInfo.UseShellExecute = true;
      proc.StartInfo.RedirectStandardOutput = false;
      proc.StartInfo.RedirectStandardError = false;
      proc.Start();
      //proc.WaitForExit();
      //proc.Close();
      
    }  /* ClassificationDone */


    private  ulong ticCount = 0;

    private void timer1_Tick (object sender, EventArgs e)
    {
      ticCount++;
      if  ((ticCount % 20) == 0)
      {
        UpdateDisplayVariables ();
        UpdateDisplay ();
      }

      if  ((!selectionThreadRunning)  &&  (!predUpdateThreadRunning))
      {
        timer1.Enabled = false;

        cpuTimeEnd = Process.GetCurrentProcess ().TotalProcessorTime;

        double  cpuTimeUsed = cpuTimeEnd.TotalSeconds - cpuTimeStart.TotalSeconds;

        if  (cancelProcessing)
        {
          mainWinConn.LogEntriesProcessEnd (logEntry, (float)cpuTimeUsed, "Canceled");
          MessageBox.Show (this, "Classification Process Canceled.");
        }
        else
        {
          UpdateDisplayVariables ();
          mainWinConn.LogEntriesProcessEnd (logEntry, (float)cpuTimeUsed, "Done");
          UpdateDisplay ();
          ClassificationDone ();
        }

        this.Close ();
      }
    }


    private void ClassificationStatus_Load (object sender, EventArgs e)
    {
      if  ((classifier != null)  &&  (!classifier.Valid))
      {
        MessageBox.Show (this,
                         "Classifier[" + classifier.ConfigFileName + "] is not Built, Loaded or has errors.", 
                         "Classifier not Built or Loaded", 
                         MessageBoxButtons.OK
                        );
      }
    }



    private void StartButton_Click(object sender, EventArgs e)
    {
      if  (allowUpdates)
      {
        if  (ReClassify.Checked)
        {
          DialogResult dr = MessageBox.Show (this,
                                             "You have choosen to Re-Classify images before tabulating,  is this correct ?", 
                                             "Start Classification Report", 
                                             MessageBoxButtons.YesNo
                                            );
          if  (dr == DialogResult.No)
            return;
        }
        else
        {
          DialogResult dr = MessageBox.Show (this, 
                                             "You have choosen NOT to Classify images before tabulating,  is this correct ?", 
                                             "Start Classification Report", 
                                             MessageBoxButtons.YesNo
                                            );
          if  (dr == DialogResult.No)
            return;
        }
      }

      reClassify = ReClassify.Checked  &&  allowUpdates;
      validatedTrumps = ValidatedTrumps.Checked;
      
      StartButton.Enabled = false;
      cancelButton.Enabled = true;
      timer1.Enabled = true;
      midPoint = GetMidPoint ();
      StartClassificationThreads ();
    }


    private  DateTime  GetMidPoint ()
    {
      String  sqlStr = "call  InstrumentDataGetMidPointOfDeployment(\"" + cruiseName + "\", \"" + stationName + "\", \"" + deploymentNum + "\")";

      String[]  colNames = {"MidPoint"};
      String[][]  results = mainWinConn.QueryStatement (sqlStr, colNames);
      if  ((results == null)  ||  (results.Length < 1)  ||  (results[0].Length < 1))
        return  DateTime.Now;
      else
        return  PicesKKStr.StrToDateTime (results[0][0]);
    }



    private void cancelButton_Click(object sender, EventArgs e)
    {
      DialogResult dr = MessageBox.Show (this, "Are you sure you want to cancel this Classification Process ?", 
                                         "Cancel", 
                                         MessageBoxButtons.YesNo
                                        );
      if  (dr == DialogResult.Yes)
      {
        cancelButton.Enabled = false;
        CancelProcesses ();
        cancelProcessing = true;
      }
    }

    private void ClassificationStatus_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (selectionThreadRunning)
        e.Cancel = true;

    }
  }  /* ClassificationStatus*/
}
