using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

using  PicesInterface;


namespace PicesCommander
{
  public  class  ClassifierProcess
  {
    private  int                        processorNum            = -1;
    private  bool                       cancelFlag              = false;
    private  bool                       reClassify              = false;
    private  bool                       validatedTrumps         = false;
    private  Queue<PredictionResult>    predictionUpdateQueue   = null;
    private  Queue<PicesFeatureVector>  featureVectorQueue      = null;

    private  SizeDistribution           sizeDistribution        = null;
    private  SizeDistribution           depthDistribution_1     = null;
    private  SizeDistribution           depthDistribution_1Down = null;
    private  SizeDistribution           depthDistribution_1Up   = null;
    private  SizeDistribution           depthDistribution_10    = null;
    private  ClassStatSortedList        classCounts             = null;
    private  uint                       numImagesClassified     = 0;

    private  PicesDataBaseServer        dbServer              = null;

    private  Thread  thread          = null;
    private  bool    threadDone      = false;  
    private  bool    threadRunning   = false;
    private  bool    threadTerminate = false;

    private  PicesPrediction   pred1 = null;
    private  PicesPrediction   pred2 = null;

    private  PicesRunLog     runLog = null;   // Will only get instantiated if it is needed.
    private  PicesDataBase   dbConn = null;   // This database connection belongs to the thread "ThreadProcess".
    
    private  TrainingModel2  classifier = null;
    private  bool            classifierWeOwnIt = false;
    private  String          classifierName = "";

    private  PicesClassList  classesInClassifier = null;

    private  Queue<String>   errorMsgs = null;

    private  DateTime        midPoint;

    
    public  ClassifierProcess (PicesDataBaseServer       _dbServer,
                               int                       _processorNum,
                               TrainingModel2            _classifier,
                               bool                      _reClassify,
                               bool                      _validatedTrumps,
                               Queue<PredictionResult>   _predictionUpdateQueue,
                               Queue<PicesFeatureVector> _featureVectorQueue,
                               DateTime                  _midPoint,
                               Queue<String>             _errorMsgs
                              )
    {
      errorMsgs             = _errorMsgs;
      processorNum          = _processorNum;
      classifier            = _classifier;
      reClassify            = _reClassify;
      validatedTrumps       = _validatedTrumps;
      predictionUpdateQueue = _predictionUpdateQueue;
      featureVectorQueue    = _featureVectorQueue;
      dbServer              = _dbServer;
      midPoint              = _midPoint;
      
      classifierWeOwnIt = false;
      if  (classifier != null)
        classifierName  = classifier.ModelName;
      ClassInitialization ();
    }



    public  ClassifierProcess (PicesDataBaseServer       _dbServer,
                               int                       _processorNum,
                               String                    _classifierName,
                               bool                      _reClassify,
                               bool                      _validatedTrumps,
                               Queue<PredictionResult>   _predictionUpdateQueue,
                               Queue<PicesFeatureVector> _featureVectorQueue,
                               DateTime                  _midPoint,
                               Queue<String>             _errorMsgs
                              )
    {
      errorMsgs             = _errorMsgs;
      processorNum          = _processorNum;
      classifier            = null;
      classifierName        = _classifierName;
      reClassify            = _reClassify;
      validatedTrumps       = _validatedTrumps;
      predictionUpdateQueue = _predictionUpdateQueue;
      featureVectorQueue    = _featureVectorQueue;
      dbServer              = _dbServer;
      midPoint              = _midPoint;

      classifierWeOwnIt = false;
      
      ClassInitialization ();
    }




    private  void  ClassInitialization ()
    {
      sizeDistribution        = new  SizeDistribution ( 50, 100);
      depthDistribution_1     = new  SizeDistribution (500,   1);
      depthDistribution_1Down = new  SizeDistribution (500,   1);
      depthDistribution_1Up   = new  SizeDistribution (500,   1);
      depthDistribution_10    = new  SizeDistribution ( 50,  10);
      classCounts             = new  ClassStatSortedList ();

      numImagesClassified = 0;

      pred1 = new PicesPrediction (null, 0, 0.0f);
      pred2 = new PicesPrediction (null, 0, 0.0f);
      if  (classifier != null)
        classesInClassifier = classifier.MLClasses ();
    }

    public  uint  NumImagesClassified  {get  {return numImagesClassified;}}
    public  bool  ThreadRunning        {get  {return threadRunning;}}
    public  bool  ThreadDone           {get  {return threadDone;}}


    private  PicesRunLog  RunLog ()
    {
      if  (runLog == null)
        runLog = new PicesRunLog ();
      return runLog;
    }


    private  PicesDataBase  DbConn ()
    {
      // I am locking errorMsgs because it is shared bv all the classifier threads.
      lock (errorMsgs)
      {
        if  (dbConn == null)
        {
          if  (dbServer == null)
            dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (RunLog ());
          else
            dbConn = new PicesDataBase (dbServer, RunLog ());
        }
      }
      return  dbConn;
    }



    /// <summary>Call this method when ready to terminate thread processing.</summary>
    public  void  ThreadTerminate ()
    {
      threadTerminate = true;
    }


    private  void  ErrorMsgsAdd (String  msg)
    {
      lock  (errorMsgs)
      {
        errorMsgs.Enqueue (msg);
      }
    }



    /// <summary>Thread is to terminate as quick as possible.</summary>
    public  void  ThreadCancel ()
    {
      cancelFlag = true;
    }


    public  void  StartThread ()
    {
      thread = new Thread (new ThreadStart (ThreadProcess));
      thread.Name = "ClassifierProcess_" + processorNum.ToString ();
      thread.Start ();
      threadRunning = true;
    }  /* StartThread */



    private  PicesFeatureVector  GetNextFeatureVector ()
    {
      PicesFeatureVector  nextFv = null;
      lock  (featureVectorQueue)
      {
        if  (featureVectorQueue.Count > 0)
          nextFv = featureVectorQueue.Dequeue ();
      }
      return  nextFv;
    }  /* GetNextFeatureVector*/
    

    private  void  ThreadProcess ()
    {
      threadRunning = true;
      PicesDataBase.ThreadInit ();


      if  ((classifier == null)  &&  (!String.IsNullOrEmpty (classifierName)))
      {
        PicesRunLog  log = new PicesRunLog ();
        classifier = new TrainingModel2 (log, classifierName);
        classifier.LoadExistingModelOtherwiseBuild (null);
        classifierWeOwnIt = true;
      }

      while  (!cancelFlag)
      {
        PicesFeatureVector  fv = GetNextFeatureVector ();
        if  (fv != null)
        {
          ProcessOneFeatureVector (fv);
        }
        else
        {
          if  (threadTerminate)
            break;
          Thread.Sleep (10);
        }
      }

      if  (dbConn != null)
      {
        dbConn.Close ();
        dbConn = null;
        GC.Collect ();
      }

      PicesDataBase.ThreadEnd ();

      threadRunning = false;
      threadDone    = true;
    }  /* ThreadProcess */



    private  void  ProcessOneFeatureVector (PicesFeatureVector fv)
    {
      int x = midPoint.CompareTo (fv.CtdDateTime);
      
      bool  downCast = (x < 0);

      if  (reClassify)
      {
        String  imageFileName = fv.ImageFileName;
        String  imageFileRootName = OSservices.GetRootName (imageFileName);

        if  ((fv.Validated)  &&  (validatedTrumps))
        {
          PicesClass validatedClass = AssignVallidatedClassAsPredictedClass (fv);
          PredictClasses (fv, pred1, pred2, imageFileName, imageFileRootName);
          if  (validatedClass != null)
          {
            if  (pred1.MLClass != validatedClass)
              pred2.MLClass   = pred1.MLClass;
            pred2.Probability  = 0.0f;
              
            pred1.MLClass   = validatedClass;
            pred1.Probability  = 1.0f;
          }
        }
        else
        {
          PredictClasses (fv, pred1, pred2, imageFileName, imageFileRootName);
        }

        String  imageRootName = OSservices.GetRootName (fv.ImageFileName);
        lock  (predictionUpdateQueue)
        {
          if  (pred1.MLClass.ClassId < 0)
          {
            pred1.MLClass = UpdateClassId (pred1.MLClass);
          }
          if  (pred2.MLClass.ClassId < 0)
          {
            pred2.MLClass = UpdateClassId (pred2.MLClass);
          }

          predictionUpdateQueue.Enqueue (new PredictionResult (imageRootName, 
                                                               pred1.MLClass.ClassId, 
                                                               pred1.Probability, 
                                                               pred2.MLClass.ClassId, 
                                                               pred2.Probability
                                                              )
                                        );
        }
      }
      else
      {
        if  ((fv.Validated)  &&  (validatedTrumps))
        {
          pred1.MLClass  =  fv.MLClass;
          pred1.Probability = 1.0f;
        }
        else
        {
          pred1.MLClass  = fv.MLClass;
          pred1.Probability = fv.Probability;
        }
      }

      lock  (classCounts)
      {
        classCounts.AddOne (pred1.MLClass);
        sizeDistribution.Increment     (pred1.MLClass, (int)fv.OrigSize);
        depthDistribution_1.Increment  (pred1.MLClass, (int)fv.Depth);
        if  (downCast)
          depthDistribution_1Up.Increment   (pred1.MLClass, (int)fv.Depth);
        else
          depthDistribution_1Down.Increment (pred1.MLClass, (int)fv.Depth);
        depthDistribution_10.Increment (pred1.MLClass, (int)fv.Depth);
        numImagesClassified++;
      }
    }  /* ProcessOneFeatureVector */



    private  PicesClass  UpdateClassId (PicesClass mlClass)
    {
      // We lock 'predictionUpdateQueue' because all other classification processs refer to it.
      // 
      lock  (predictionUpdateQueue)
      {
        // Maybe another Classification process already updated the database and hence updated the classId field.
        if  (mlClass.ClassId >= 0)
        {
          int zed = 888;
        }
        else
        {
          String  className  = mlClass.Name;
          String  parentName = mlClass.ParentName;
          if  (String.IsNullOrEmpty (parentName))
          {
            parentName = "AllClasses";
            mlClass.Parent = PicesClassList.GetUniqueClass (parentName, "");
          }

          if  (runLog == null)
            runLog = new PicesRunLog ();
          PicesDataBase  dbConn = null;
          if  (dbServer == null)
            dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
          else
            dbConn = new PicesDataBase (dbServer, runLog);

          mlClass.Description = "Added by 'Classification'";

          dbConn.MLClassInsert (mlClass);

          if  (mlClass.ClassId < 0)
          {
            PicesClass pc = dbConn.MLClassLoad (className);
            if  (mlClass.ClassId < 0)
            {
              runLog.Writeln ("ClassifierProcess.UpdateClassId    ***ERROR***     mlClass.ClassId < 0      ClassName[" + className + "]");
            }
          }

          // I do not know what the impact of opening and closing a lot of conections 
          // to the MySQL data base will be, and since this method should only get called
          // when a new MLClass is introduced I want to sleep a little to make
          // sure that we do not over stress resources that I am not aware of.
          Thread.Sleep (100);
          //dbConn.Close ();
          dbConn = null;
          GC.Collect ();
        }
      }

      return  mlClass;
    }



    private void  PredictClasses (PicesFeatureVector  fv,
                                  PicesPrediction     pred1, 
                                  PicesPrediction     pred2,
                                  String              imageFileName,
                                  String              imageFileRootName
                                 )
    {
      PicesFeatureVector  fvToUse = fv;

      if  (fvToUse.FeatureDataMissing)
      {
        fvToUse = null;

        // Feature data is missing.  we need to compute from originbal image.
        PicesDataBaseImage pdbi = DbConn ().ImageLoad (imageFileRootName);
        if  (pdbi != null)
        {
          PicesRaster pr = DbConn ().ImageFullSizeFind (imageFileRootName);
          if  (pr == null)
          {
            ErrorMsgsAdd ("Failed to Load FullSize image [" + imageFileRootName + "]");
          }
          else
          {
            fvToUse = new PicesFeatureVector (pr, imageFileName, null, runLog);
            // Since we had to compute the FeatureDatya from the raster image we now need to
            // get the instrument data that matches it.
            PicesInstrumentData id = DbConn ().InstrumentDataGetByScanLine (pdbi.SipperFileName, pdbi.TopLeftRow);
            if (id != null)
              fvToUse.AddInstrumentData (id);
            DbConn ().FeatureDataInsertRow (pdbi.SipperFileName, fvToUse);
            pr.Dispose ();
          }

          pr = null; 
        }
      }

      if  (fvToUse != null)
      {
        classifier.PredictClass (fvToUse, pred1, pred2);
      }
      else
      {
        pred1.MLClass = PicesClassList.GetUnKnownClassStatic ();   pred1.Probability = 1.0f;
        pred2.MLClass = PicesClassList.GetUnKnownClassStatic ();   pred2.Probability = 0.0f;
      }
    }  /* PredictClasses */


    
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



    public  void  UpdateClassificationStats (SizeDistribution  _sizeDistribution, 
                                             SizeDistribution  _depthDistribution_1, 
                                             SizeDistribution  _depthDistribution_1Down, 
                                             SizeDistribution  _depthDistribution_1Up, 
                                             SizeDistribution  _depthDistribution_10,
                                             ref uint          _numImagesClassified
                                            )
    {
      _sizeDistribution.Add        (sizeDistribution);
      _depthDistribution_1.Add     (depthDistribution_1);
      _depthDistribution_1Down.Add (depthDistribution_1Down);
      _depthDistribution_1Up.Add   (depthDistribution_1Up);
      _depthDistribution_10.Add    (depthDistribution_10);
      _numImagesClassified += numImagesClassified;
    }  /* UpdateClassificationStats */



    public  void  ContributeOurClassCounts (ClassStatSortedList  _classCounts,
                                            ref uint             _numImagesClassified
                                           )
    {
      lock  (classCounts)
      {
        foreach  (ClassStat cs in classCounts.Values)
        {
          _classCounts.AddCount (cs.mlClass, cs.count);
        }
        _numImagesClassified += numImagesClassified;
      }
    }  /* ContributeOurClassCounts */


  }  /* ClassifierProcess */

}
