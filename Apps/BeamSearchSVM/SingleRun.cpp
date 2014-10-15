#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <string>



#include  <iomanip>
#include  <iostream>
#include  <fstream>
#include  <vector>

#ifdef  WIN32
#include  <ostream>
#endif

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;

#include  <math.h>

#include  "Str.h"
#include  "OSservices.h"

#include  "SingleRun.h"
#include  "SingleRunList.h"

#include  "CrossValidation.h"
#include  "FeatureNumList.h"
#include  "Str.h"
#include  "SVMparam.h"
#include  "SVMModel.h"
#include  "TrainingConfiguration.h"



SingleRun::SingleRun (FileDescPtr    _fileDesc,
                      RunLog&        _log,
                      int            _jobId,
                      int            _parentJobId,
                      Algorithm      _algorithm
                     ):

  accuracy         (0.0),
  algorithm        (_algorithm),
  cancelFlag       (false),
  chgFeatureAction (faSpecifyFeatures),
  chgFeatureNum    (0),
  curStatus        ('0'),
  featureNums      (_fileDesc->NumOfFields ()),
  fileDesc         (_fileDesc),
  jobId            (_jobId),
  log              (_log),
  maxNumOfFeatures (_fileDesc->NumOfFields ()),
  numOfParents     (0),
  numOfFeatures    (0),
  parentJobId      (_parentJobId),
  results          (NULL),
  startTime        (time_t (0.0)),
  supportPoints    (0.0),
  testTimeMean     (0.0),
  trainTimeMean    (0.0),
  testTimeTotal    (0.0),
  trainTimeTotal   (0.0),
  updated          (true),
  version          (SingleRunCurVersion)
{
  for  (int x = 0; x < MAXaccuracyHistory; x++)
    accuracyHistory[x] = 0.0;
}




SingleRun::SingleRun (FileDescPtr     _fileDesc,
                      RunLog&         _log,
                      KKStr          _desc,
                      Algorithm       _algorithm,
                      SingleRunList&  _existingJobs
                     ):

  accuracy         (0.0),
  algorithm        (_algorithm),
  cancelFlag       (false),
  chgFeatureAction (faSpecifyFeatures),
  chgFeatureNum    (0),
  featureNums      (_fileDesc->NumOfFields ()),
  fileDesc         (_fileDesc),
  log              (_log),
  maxNumOfFeatures (_fileDesc->NumOfFields ()),
  numOfFeatures    (0),
  numOfParents     (0),
  results          (NULL),
  startTime        (time_t (0.0)),
  supportPoints    (0.0),
  testTimeMean     (0.0),
  trainTimeMean    (0.0),
  testTimeTotal    (0.0),
  trainTimeTotal   (0.0),
  updated          (false),
  version          (0)
{
  _desc.TrimLeft ();

  for  (int x = 0; x < MAXaccuracyHistory; x++)
    accuracyHistory[x] = 0.0;

  KKStr  curStausStr;

  KKStr  firstField =  _desc.ExtractToken (" \t\n\r");
  if  ((firstField == "Ver")  ||  (firstField == "VER"))
  {
    version = _desc.ExtractTokenInt    (" \t\n\r");
    curStausStr = _desc.ExtractToken (" \t\n\r");
  }
  else
  {
    version = 0;
    curStausStr = firstField;
  }

  curStatus = curStausStr.ExtractChar ();

  if  ((curStatus == '0')  ||  
       (curStatus == '1')  ||  
       (curStatus == '2')  ||  
       (curStatus == '3'))
  {
    // We Are Ok
  }
  else
  {
    log.Level (-1) << endl 
                   << "SingleRun:  We have an invalid Cur Status."
                   << endl;
    osWaitForEnter ();
  }

  jobId          = _desc.ExtractTokenInt    (" \t\n\r");
  parentJobId    = _desc.ExtractTokenInt    (" \t\n\r");

  if  (version > 0)
  {
    KKStr  chgFeatureActionStr = _desc.ExtractToken (" \t\n\r");
    chgFeatureAction = FeatureActionFromStr (chgFeatureActionStr);
  }

  chgFeatureNum  = _desc.ExtractTokenInt    (" \t\n\r");
  accuracy       = _desc.ExtractTokenDouble (" \t\n\r");
  trainTimeMean  = _desc.ExtractTokenDouble (" \t\n\r");
  testTimeMean   = _desc.ExtractTokenDouble (" \t\n\r");
  trainTimeTotal = _desc.ExtractTokenDouble (" \t\n\r");
  testTimeTotal  = _desc.ExtractTokenDouble (" \t\n\r");
  supportPoints  = _desc.ExtractTokenDouble (" \t\n\r");

  KKStr  histStr = _desc.ExtractToken (" \t\n\r");
  numOfParents = histStr.ExtractTokenInt (",");

  int  numOfHistorySlots = Min (numOfParents, MAXaccuracyHistory);
  for  (int generation = 0; generation < numOfHistorySlots; generation++)
    accuracyHistory[generation] = histStr.ExtractTokenDouble (" ,\t\n\r");;

  if  ((version == 0)  ||  (chgFeatureAction == faSpecifyFeatures))
  {
    KKStr  featureNumStr = _desc.ExtractToken (" \t\n\r");
    bool valid = true;
    FeatureNumList  fns (fileDesc,featureNumStr, valid);
    fns.ToBitString (featureNums);
  }
  else
  {
    SingleRunPtr  parentRun = _existingJobs.LookUpByJobId (parentJobId);
    if  (!parentRun)
    {
      log.Level (-1) << endl << endl << endl
                     << "SingleRun::SingleRun  Could bot locate Parent Job[" << parentJobId << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }
    else
    {
      featureNums = parentRun->FeatureNums ();
      
      if  (chgFeatureAction == faAddFeature)
        featureNums.Set (chgFeatureNum);

      else if  (chgFeatureAction == faDeleteFeature)
        featureNums.ReSet (chgFeatureNum);
    }
  }

  numOfFeatures = featureNums.Count ();

  updated = false;  // Since this job has already existed and all we are 
                    // doing is loading into memory, then it is not considerd
                    // updated.
}





SingleRun::SingleRun (const SingleRun&  singleRun):

  accuracy         (singleRun.accuracy),
  cancelFlag       (false),
  chgFeatureNum    (singleRun.chgFeatureNum),
  chgFeatureAction (singleRun.chgFeatureAction),
  curStatus        (singleRun.curStatus),
  featureNums      (singleRun.featureNums),
  fileDesc         (singleRun.fileDesc),
  jobId            (singleRun.jobId),
  log              (singleRun.log),
  maxNumOfFeatures (singleRun.maxNumOfFeatures),
  numOfFeatures    (singleRun.numOfFeatures),
  numOfParents     (singleRun.numOfParents),
  parentJobId      (singleRun.parentJobId),
  results          (NULL),
  startTime        (singleRun.startTime),
  supportPoints    (singleRun.supportPoints),
  testTimeMean     (singleRun.testTimeMean),
  trainTimeMean    (singleRun.trainTimeMean),
  trainTimeTotal   (singleRun.trainTimeTotal),
  testTimeTotal    (singleRun.testTimeTotal),
  updated          (singleRun.updated),
  version          (singleRun.version)
{
  for  (int  generation = 0;  generation < MAXaccuracyHistory; generation++)
  {
    accuracyHistory[generation] = singleRun.accuracyHistory[generation];
  }
}






SingleRun::~SingleRun ()
{
  delete  results;
}


void  SingleRun::UpdateFromJob (const SingleRun&  j)
{
  if  (accuracy != j.accuracy)
    Accuracy (j.accuracy);

  if  (accuracyHistory[0] != j.accuracyHistory[0])
  {
    int  x;
    for  (x = 0;  x < MAXaccuracyHistory;  x++)
      accuracyHistory[x] = j.accuracyHistory[x];
    updated = true;
  }

  //Algorithm      algorithm;

  if  (curStatus != j.curStatus)
    CurStatus (j.curStatus);

  //BitString      featureNums;
  //FileDescPtr    fileDesc;
  //int            jobId;
  //RunLog&        log;
  //int            chgFeatureNum; // The number of the feature that was added or subtracted
  //FeatureAction  chgFeatureAction;
  //int            maxNumOfFeatures;
  //int            numOfParents;
  //int            parentJobId;
  //ConfussionMatrix2Ptr  results;
  
  numOfFeatures = j.numOfFeatures;

  if  ((supportPoints   != j.supportPoints)  ||
       (startTime       != j.startTime)      ||
       (testTimeMean    != j.testTimeMean)   ||
       (testTimeTotal   != j.testTimeTotal)  ||
       (trainTimeMean   != j.trainTimeMean)  ||
       (trainTimeTotal  != j.trainTimeTotal)
      )
  {
    supportPoints   = j.supportPoints;
    startTime       = j.startTime;
    testTimeMean    = j.testTimeMean;
    testTimeTotal   = j.testTimeTotal;
    trainTimeMean   = j.trainTimeMean;
    trainTimeTotal  = j.trainTimeTotal;
    updated = true;
  }

}  /* UpdateFromJob */




void  SingleRun::DisplayLeftOvers ()
{
}




double  SingleRun::AccuracyHistory (int idx)
{
  if  ((idx < 0)  ||  (idx >= MAXaccuracyHistory))
    return  0;
  else
    return accuracyHistory[idx];
}


void  SingleRun::Accuracy (double _accuracy)   
{
  accuracy = _accuracy;
  updated = true;
}



void  SingleRun::ChgFeatureNum (int            _chgFeatureNum,
                                FeatureAction  _chgFeatureAction
                               )
{
  chgFeatureNum    = _chgFeatureNum;
  chgFeatureAction = _chgFeatureAction;
  updated = true;
}


double  SingleRun::ChgInAccuracy ()  const
{
  return  accuracy - accuracyHistory[0];
}  /* ChgInAccuracy */




void  SingleRun::CurStatus (char  _curStatus)  
{
  curStatus = _curStatus;
  updated = true;
}



void  SingleRun::AddFeatureNum (int  featureNum)
{
  if  (!featureNums.Test (featureNum))
    numOfFeatures++;

  featureNums.Set (featureNum);
  updated = true;
}



void  SingleRun::FeatureNums (const FeatureNumList&  _featureNums)
{
  _featureNums.ToBitString (featureNums);
  numOfFeatures = featureNums.Count ();
  updated = true;
}  /* FeatureNums */




void  SingleRun::FeatureNums (const BitString&   _featureNums)
{
  featureNums = _featureNums;
  numOfFeatures = featureNums.Count ();
  updated = true;
}  /* FeatureNums */



void  SingleRun::FeatureNums (KKStr featureNumStr)
{
  bool  valid = true;
  FeatureNumList fns (fileDesc, IncludeFeatureNums, featureNumStr, valid);
  fns.ToBitString (featureNums);
  numOfFeatures = featureNums.Count ();
  updated = true;
}




void  SingleRun::SetFeatureNums (const FeatureNumList&  _featureNums)
{
  FeatureNums (_featureNums);
  updated = true;
}  /* SetFeatureNums */





void  SingleRun::BuildSubSets (SingleRunListPtr  singleRuns,
                               int&              nextJobId,
                               int&              numJobsSelected,
                               SingleRunListPtr  doneJobs
                              )
{
  int  x;
  int  y;

  log.Level (20) << "SingleRun::BuildSubSets   jobId[" << jobId << "]." << endl;

  numJobsSelected = 0;

  FeatureNumList fns (fileDesc, featureNums);

  if  (fns.NumOfFeatures () < 2)
  {
    log.Level (30) << "BuildSubSets   jobId[" << jobId << "]  featureNums.NumOfFeatures () < 2." << endl;
    return;
  }

  for  (x = 0; x < fns.NumOfFeatures (); x++)
  {
    SingleRunPtr  run = new SingleRun (fileDesc, log, nextJobId, jobId, algorithm);
    nextJobId++;

    // Lets give the children a history of there ancestors accuracy.
    run->numOfParents = numOfParents + 1;
    for  (int  generation = 0; generation < (MAXaccuracyHistory - 1); generation++)
    {
      run->accuracyHistory[generation + 1] = accuracyHistory[generation];
    }
    run->accuracyHistory[0] = accuracy;

    for  (y = 0; y < fns.NumOfFeatures (); y++)
    {
      if  (y != x)
      {
        run->AddFeatureNum (fns[y]);
      }
      else
      {
        run->ChgFeatureNum (fns[y], faDeleteFeature);
      }
    }

    // Lets make sure that we don't already have a run with these parameters scheduled.
    SingleRunPtr  tempRun         = singleRuns->LookUpRun (run->FeatureNums ());
    SingleRunPtr  existingDoneJob = doneJobs->LookUpRun   (run->FeatureNums ());

    if  (tempRun  ||  existingDoneJob)
    {
      // Looks Like this job already exists, will need to delete the new one.
      delete  run;
      run = NULL;
      nextJobId--;
    }
    else
    {
      singleRuns->PushOnBack (run);
      numJobsSelected++;
    }
  }

  log.Level (20) << "BuildSubSets   jobId[" << jobId << "]    Exiting." << endl;

}  /* BuildSubSets  */






void  SingleRun::BuildSubSetsOut (SingleRunListPtr   singleRuns,
                                  int&               nextJobId,
                                  int&               numJobsSelected,
                                  FeatureNumListPtr  initialFeatures,
                                  SingleRunListPtr   doneJobs
                                 )
{
  log.Level (20) << "SingleRun::BuildSubSetsOut." << endl;

  FeatureNumList  fns (fileDesc, featureNums);

  FeatureNumList unSelectedFeatures (fileDesc);

  if  (initialFeatures)
    unSelectedFeatures = (*initialFeatures) - fns;
  else
    unSelectedFeatures = fns.Complement ();


  int x;
  for  (x = 0; x < unSelectedFeatures.NumOfFeatures (); x++)
  {
    log.Level (30) << "BuildSubSetsOut - Creating New Job for feature["
                   << unSelectedFeatures[x] << "]."
                   << endl;

    SingleRunPtr  run = new SingleRun (fileDesc, log, nextJobId, jobId, algorithm);
    nextJobId++;

    // Lets give the children a history of there ancestors accuracy.
    run->numOfParents = numOfParents + 1;
    for  (int  generation = 0; generation < (MAXaccuracyHistory - 1); generation++)
    {
      run->accuracyHistory[generation + 1] = accuracyHistory[generation];
    }
    run->accuracyHistory[0] = accuracy;

    FeatureNumList  expandedFeatures = fns + unSelectedFeatures[x];
    run->FeatureNums (expandedFeatures);
    run->ChgFeatureNum (unSelectedFeatures[x], faAddFeature);

    // Lets make sure that we don't already have a run with these parameters scheduled.
    SingleRunPtr  tempRun         = singleRuns->LookUpRun (run->FeatureNums ());
    SingleRunPtr  existingDoneJob = doneJobs->LookUpRun   (run->FeatureNums ());
    if  (tempRun  ||  existingDoneJob)
    {
      log.Level (30) << "BuildSubSetsOut Job[" << run->FeaturesIncluded () << "], Already Processed." << endl;
      delete  run;
      run = NULL;
      nextJobId--;
    }
    else
    {
      singleRuns->PushOnBack (run);
      numJobsSelected++;
      log.Level (30) << "BuildSubSetsOut Job[" << run->FeaturesIncluded () << "], Added To KKQueue." << endl;
    }
  }

}  /* BuildSubSetsOut  */








void  SingleRun::PerformC45Algorithm (ImageClassList&      imageClasses,
                                      ImageFeaturesListPtr trainingImages,
                                      ImageFeaturesListPtr testImages,
                                      int                  foldNum,
                                      int&                 correct,
                                      int&                 total
                                     )

{
  log.Level (10) << "SingleRun::PerformC45Algorithm" << endl;

  if  (!results)
    results = new ConfussionMatrix2 (imageClasses, log);

  KKStr  rootFileName ("C45_");
  rootFileName << jobId << "_" << foldNum;

  KKStr  trainingFileName (rootFileName);
  trainingFileName << ".data";

  KKStr  testFileName (rootFileName);
  testFileName << ".test";

  KKStr  namesFileName (rootFileName);
  namesFileName << ".names";

  KKStr  c45RunLog (rootFileName);
  c45RunLog << "_run.log";

  KKStr  predFileName (rootFileName);
  predFileName << ".pred";

  KKStr  weightedFileName (rootFileName);
  weightedFileName << ".weighted_pred";

  ImageClassPtr  unknownClass = ImageClass::CreateNewImageClass ("UnKnown");

  FeatureNumList  fns (fileDesc, featureNums);

  // Names File
  {
    ofstream namesFile (namesFileName.Str ());

    ImageClassListIterator  icIDX (imageClasses);

    ImageClassPtr  imageClass;

    int  classCount = 0;
    for  (icIDX.Reset ();  imageClass = icIDX.CurPtr (); ++icIDX)
    {
      if  (classCount > 0)
        namesFile << ", ";

      namesFile << icIDX->Name ();
      classCount++;
    }
  
    namesFile << endl;

    int  featureIdx = 0;

    for  (featureIdx = 0; featureIdx < fns.NumOfFeatures (); featureIdx++)
    {
      int  featureNum = fns[featureIdx];
      namesFile << fileDesc->FieldName (featureNum) << ":continuous" << endl;
    }

    namesFile.close ();
  }


  // Training File  .data
  {
    ofstream trainingFile (trainingFileName.Str ());

    ImageFeaturesListIterator  tiIDX (*trainingImages);

    int  featureIdx;
    int  featureNum; 
    
    for  (tiIDX.Reset ();  tiIDX.CurPtr (); ++tiIDX)
    {
      for  (featureIdx = 0; featureIdx < fns.NumOfFeatures (); featureIdx++)
      {
        featureNum = fns[featureIdx];
        trainingFile << tiIDX->FeatureData (featureNum) << ", ";
      }

      trainingFile << tiIDX->ClassName () << endl;
    }

    trainingFile.close ();
  }


  // Test File  .test
  {
    ofstream  testFile (testFileName.Str ());

    ImageFeaturesListIterator  tiIDX (*testImages);

    int  featureIdx;
    int  featureNum; 
    
    for  (tiIDX.Reset ();  tiIDX.CurPtr (); ++tiIDX)
    {
      for  (featureIdx = 0; featureIdx < fns.NumOfFeatures (); featureIdx++)
      {
        featureNum = fns[featureIdx];
        testFile << tiIDX->FeatureData (featureNum) << ", ";
      }

      testFile << tiIDX->ClassName () << endl;
    }

    testFile.close ();
  }


  KKStr  cmdLine ("usfcascor -P -f ");
  cmdLine << rootFileName;

  //        << "  >& " 
  //        << c45RunLog;


  int  returnCd;

  returnCd = system (cmdLine.Str ());


  // Process Results of Classification.
  {
    FILE*  predFile = osFOPEN (predFileName.Str (), "r");

    if  (!predFile)
    {
      log.Level (-1)                                                             << endl 
                     << "SingleRun::PerformC45Algorithm  *** ERROR ***"          << endl
                     << "Error Opening Prediction File[" << predFileName << "]." << endl;
      osWaitForEnter ();
      exit (-1);
    }

    char  buff[10240];
    KKStr  predictedClassName;

    int     lineCount = 0;

    ImageClassPtr     knownClass     = NULL;
    ImageClassPtr     predictedClass = NULL;
    ImageFeaturesPtr  image          = NULL;
  
    int  numOfTestImages = testImages->QueueSize ();

    while  ((fgets (buff, sizeof (buff), predFile))  &&  (lineCount < numOfTestImages))
    {
      predictedClassName = buff;

      predictedClassName.TrimRight ();
      predictedClassName.TrimLeft ();

      image = testImages->IdxToPtr (lineCount);
      knownClass     = image->ImageClass ();

      predictedClass = imageClasses.LookUpByName (predictedClassName);

      if  (predictedClass == NULL)
      {
        log.Level (-1)                                                                     << endl
                       << "SingleRun::PerformC45Algorithm  *** ERROR ***"                  << endl
                       << "Test Image[" << image->ImageFileName () << "] Was assigned an"  << endl
                       << "unknown class[" << buff << "]."                                 << endl
                                                                                           << endl;

        predictedClass = unknownClass;
      }

      total++;
      if  (knownClass->UpperName () == predictedClass->UpperName ())
        correct++;
             
      results->Increment (knownClass, 
                          predictedClass, 
                          (int) (image->OrigSize ()), 
                          image->Probability ()
                         );

      lineCount++;
    }

    fclose (predFile);
  }

  osDeleteFile (trainingFileName);
  osDeleteFile (testFileName);
  osDeleteFile (namesFileName);
  osDeleteFile (predFileName);
  osDeleteFile (weightedFileName);
  //delete  unknownClass;
}  /* BuildC45Files */






void  SingleRun::RunCrossValidation (ImageFeaturesList&        images,
                                     ImageClassList&           imageClasses,
                                     TrainingConfigurationPtr  config,
                                     int                       numOfFolds,
                                     ostream*                  errorFile
                                    )
{
  log.Level (10) << "SingleRun::CrossValidation" << endl;

  delete  results;

  config->SetFeatureNums (FeatureNumList (fileDesc, featureNums));

  CrossValidation  cv (config, 
                       &images, 
                       &imageClasses, 
                       numOfFolds, 
                       true,          // true = featuers are already normalized.
                       images.FileDesc (),
                       log,
                       cancelFlag
                      );
  cv.RunCrossValidation ();

  results = cv.GiveMeOwnershipOfConfusionMatrix ();

  accuracy       = cv.AccuracyMean ();
  trainTimeMean  = cv.TrainTimeMean ();
  testTimeMean   = cv.TestTimeMean ();
  trainTimeTotal = cv.TrainTimeTotal ();
  testTimeTotal  = cv.TestTimeTotal ();
  supportPoints  = cv.SupportPointsMean ();

  updated = true;
}  /* CrossValidation */





void   SingleRun::RunCrossValidation (ImageFeaturesList&        images,
                                      ImageFeaturesList&        validationData,
                                      ImageClassList&           imageClasses,
                                      TrainingConfigurationPtr  config,
                                      ostream*                  errorFile
                                     )

{
  log.Level (10) << "SingleRun::CrossValidation" << endl;

  delete  results;

  config->SetFeatureNums (FeatureNumList (fileDesc, featureNums));

  CrossValidation  cv (config, 
                       &images, 
                       &imageClasses, 
                       1,                    // numOfFolds = 1
                       true,                 // true = featuers are already normalized.
                       images.FileDesc (),
                       log,
                       cancelFlag
                      );

  cv.RunValidationOnly (&validationData, NULL);

  results = cv.GiveMeOwnershipOfConfusionMatrix ();

  accuracy       = cv.AccuracyMean ();
  trainTimeMean  = cv.TrainTimeMean ();
  testTimeMean   = cv.TestTimeMean ();
  trainTimeTotal = cv.TrainTimeTotal ();
  testTimeTotal  = cv.TestTimeTotal ();
  supportPoints  = cv.SupportPointsMean ();

  updated = true;
}  /* CrossValidation */   // Using Validation Data








KKStr  SingleRun::FeaturesIncluded ()  const
{
  return  FeatureNumList (fileDesc, featureNums).ToString ();
}




KKStr  SingleRun::FeaturesExcluded (int maxFeatureNum)  const
{
  FeatureNumList  fns (fileDesc, featureNums);
  fns.Complement();
  return  fns.ToString ();
} /* FeaturesExcluded */





KKStr  SingleRun::RunStatusStr ()
{
  KKStr  statusStr (200);

  if  (version > 0)
    statusStr << "Ver" << "\t" << version << "\t";


  statusStr.Append (curStatus);
  if  ((curStatus < '0')  ||  (curStatus > '3'))
  {
    log.Level (-1) << endl
                   << "SingleRun::RunStatusStr - Invalid Status Character."
                   << endl
                   << "   StatusStr[" << curStatus << "]."
                   << endl
                   << endl;
    osWaitForEnter ();
  }

  statusStr << "\t"; 

  statusStr << jobId               << "\t" 
            << parentJobId         << "\t" ;

  if  (version > 0)
  {
    statusStr << FeatureActionToStr (chgFeatureAction) << "\t";
  }

  statusStr << chgFeatureNum                                    << "\t"
            << StrFormatDouble (accuracy,       "##0.00000")    << "\t"
            << StrFormatDouble (trainTimeMean,  "###0.0000")    << "\t"
            << StrFormatDouble (testTimeMean,   "###0.0000")    << "\t"
            << StrFormatDouble (trainTimeTotal, "###0.0000")    << "\t"
            << StrFormatDouble (testTimeTotal,  "###0.0000")    << "\t"
            << StrFormatDouble (supportPoints,  "#####0.000")   << "\t"
            << HistoryToStr ()                                  << "\t";

  if  (version == 0)
  {
    statusStr << FeaturesIncluded ();
  }
  else if  (chgFeatureAction == faSpecifyFeatures)
  {
    statusStr << FeaturesIncluded ();
  }


  return  statusStr;
} /* RunStatusStr */



void  SingleRun::DeleteResults ()
{
  delete  results;
  results = NULL;
}



KKStr  SingleRun::HistoryToStr ()
{
  KKStr  historyStr (200);
  historyStr << numOfParents;
  int  numOfHistorySlots = Min (numOfParents, MAXaccuracyHistory);
  for  (int generation = 0;  generation < numOfHistorySlots; generation++)
  {
    historyStr << "," << StrFormatDouble (accuracyHistory[generation], "ZZ0.0000");
    while  (historyStr.LastChar () == '0')
      historyStr.ChopLastChar ();

    if  (historyStr.LastChar () == '.')
      historyStr.Append ('0');
  }
  return  historyStr;
}  /* HistoryToStr */





#ifdef  WIN32
ostream& __cdecl  operator<< (      ostream&    os, 
                              const SingleRun&  singleRun
                             )
{
  os << "JobID[" << singleRun.JobId () << "]  Features Included[" << singleRun.FeaturesIncluded () << "]";
  return os;
}

#else

ostream& operator<< (      ostream&    os, 
                     const SingleRun&  singleRun
                    )
{
  os << "JobID[" << singleRun.JobId ()  << "]  Features Included[" << singleRun.FeaturesIncluded () << "]";
  return os;
}
#endif




KKStr  FeatureActionToStr (FeatureAction  fa)
{
  switch  (fa)
  {
  case       faAddFeature:  return  "AddFA";
  case    faDeleteFeature:  return  "DelFA";
  case  faSpecifyFeatures:  return  "SpecifyFA";
                  default:  return  "UnKnownFA";
  }

  return  "UnKnownFA";
}  /* FeatureActionToStr */



FeatureAction  FeatureActionFromStr (const KKStr&  faStr)
{
  KKStr  faStrUpper = faStr.ToUpper ();
  if  (faStrUpper == "ADDFA")
    return faAddFeature;

  else if  (faStrUpper == "DELFA")
    return faDeleteFeature;

  else if  (faStrUpper == "SPECIFYFA")
    return  faSpecifyFeatures;

  else
    return  faNULL;
}  /* FeatureActionFromStr */

