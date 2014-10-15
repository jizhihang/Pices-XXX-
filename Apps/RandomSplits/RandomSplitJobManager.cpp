#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <memory>
#include  <math.h>

#include  <map>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;

#include "HTMLReport.h"
#include "OSservices.h"
#include "Str.h"
using namespace KKU;

#include "InstrumentDataFileManager.h"

#include "DataBase.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "TrainingConfiguration2.h"
using namespace MLL;

#include "JobManager.h"
using namespace  JobManagment;


#include "RandomSplitJob.h"
#include "RandomSplitJobManager.h"



RandomSplitJobManager::RandomSplitJobManager (const RandomSplitJobManager&    j):
   JobManager (j),

   config            (NULL),
   configFileName    (j.configFileName),
   data              (NULL),
   dataFileName      (j.dataFileName),
   dataIndexFileName (j.dataIndexFileName),
   fileDesc          (j.fileDesc),
   format            (j.format),
   mlClasses      (NULL),
   numFolds          (j.numFolds),
   numSplits         (j.numSplits),
   splitFraction     (j.splitFraction),
   splits            (NULL)

{
  if  (j.config)
    config = new TrainingConfiguration2 (*config);

  if  (j.data)
    data = new FeatureVectorList (*data, true);

  if  (j.mlClasses)
    mlClasses = new MLClassConstList (*mlClasses);

  if  (j.splits)
    splits = new Orderings (*splits);
}




RandomSplitJobManager::RandomSplitJobManager (const KKStr&      _configFileName,
                                              FeatureFileIOPtr  _format,
                                              const KKStr&      _dataFileName,
                                              int               _numSplits,
                                              float             _splitFraction,
                                              int               _numFolds,
                                              RunLog&           _log
                                              ):
   JobManager (NULL,
               -1,                       // _jobId
               -1,                       // _parentId
               64,                       // _numPorcessesAllowed
               "RandomSplitJobManager",  // _managerName
               1,                        // _numJobsAtATime
               _log
              ),

   config            (NULL),
   configFileName    (_configFileName),
   data              (NULL),
   dataFileName      (_dataFileName),
   dataIndexFileName (),
   fileDesc          (NULL),
   format            (_format),
   mlClasses      (NULL),
   numFolds          (_numFolds),
   numSplits         (_numSplits),
   splitFraction     (_splitFraction),
   splits            (NULL)

{
  SupportCompletedJobData (true);
}







RandomSplitJobManager::~RandomSplitJobManager ()
{
  delete  config;         config       = NULL;
  delete  data;           data         = NULL;
  delete  mlClasses;   mlClasses = NULL;
  delete  splits;         splits       = NULL;
}



const char*  RandomSplitJobManager::JobType ()  const
{
  return  "RandomSplitJobManager";
}




JobPtr  RandomSplitJobManager::Duplicate ()  const
{
  return new RandomSplitJobManager (*this);
}



void    RandomSplitJobManager::ProcessNode ()
{
}




// Derived classes use this method to seed the initial set of jobs.   
// Will be called by 'JobManager' at appropriate time.
JobListPtr  RandomSplitJobManager::JobsCreateInitialSet ()
{
  JobListPtr  initialJobs = new JobList (this);
  for  (int splitNum = 0;  splitNum < numSplits;  splitNum++)
  {
    RandomSplitJobPtr  j = new RandomSplitJob (this, GetNextJobId (), -1, splitNum, log);
    initialJobs->PushOnBack (j);
  }
  return  initialJobs;
}  /* JobsCreateInitialSet */



JobListPtr  RandomSplitJobManager::JobsExpandNextSetOfJobs (const JobListPtr  jobsJustCompletd)
{
  // Since this program only runs the initial jobs created;  we return NULL to terminate this 
  // object.
  return NULL;
}



void   RandomSplitJobManager::JobCompleted  (ostream& o, JobPtr   j)
{
  // We are called right afetr a single jobs is done.

}



void  RandomSplitJobManager::GenerateFinalResultsReport ()
{
  KKStr reportFileName = osGetRootName (ManagerName ()) + "_Results.html;";

  ofstream f (reportFileName.Str ());

  f << "Run Time Parameters" << endl
    << "Run Time"          << "\t" << osGetLocalDateTime ()  << endl
    << "configFileName"    << "\t" << configFileName         << endl
    << "DataFileName"      << "\t" << dataFileName           << endl
    << "Format"            << "\t" << format->DriverName ()  << endl
    << "DataIndexFileName" << "\t" << dataIndexFileName      << endl
    << "NumFolds"          << "\t" << numFolds               << endl
    << "NumSplits"         << "\t" << numSplits              << endl
    << "splitFraction"     << "\t" << splitFraction          << endl
    << endl;


  JobList::const_iterator  idx;

  ConfusionMatrix2  avgResults (*(this->MLClasses ()));
  KKU::uint  x = 0;

  for  (idx = Jobs ()->begin ();  idx != Jobs ()->end ();  idx++)
  {
    RandomSplitJobPtr j = dynamic_cast<RandomSplitJobPtr> (*idx);
    if  (j->RandomSplitsResults () != NULL)
    {
      f << endl
        << "Random Split[" << j->SplitNum () << "]" << endl;
      j->RandomSplitsResults ()->PrintConfusionMatrixTabDelimited (f);
      f << endl << endl;

      j->PrintClassCounts (f);
      f << endl << endl;

      avgResults.AddIn (*(j->RandomSplitsResults ()), log);
      x++;
    }
  }

  f << endl << "Mean Avarage of all andom Splits." << endl;
  avgResults.FactorCounts (1.0 / (double)x);
  avgResults.PrintConfusionMatrixTabDelimited (f);
  f << endl 
    << endl
    << endl
    << endl
    << "Class Counts" << endl
    << endl;

  KKU::uint  numClasses = mlClasses->size ();

  VectorFloat   classAccs;
  VectorDouble  knownCounts;
  VectorDouble  predCounts;
  VectorDouble  adjCounts;
  VectorDouble  adjCountsStdError;
  VectorDouble  predDelta;       
  VectorDouble  adjDelta;
  
  KKStr l1, l2, l3;
  mlClasses->ExtractThreeTitleLines (l1, l2, l3);

  VectorKKStr  knownCountLines;
  VectorKKStr  predCountLines;
  VectorKKStr  adjCountLines;
  VectorKKStr  deltaPredCountLines;
  VectorKKStr  deltaAdjCountLines;
  VectorKKStr  accLines;


  ConfusionMatrix2  totalCM (*MLClasses ());
  int  totalCmCount = 0;


  // Known Counts
  for  (idx = Jobs ()->begin ();  idx != Jobs ()->end ();  idx++)
  {
    RandomSplitJobPtr j = dynamic_cast<RandomSplitJobPtr> (*idx);
    if  (j->RandomSplitsResults () != NULL)
    {
      KKStr splitNumStr = StrFormatInt (j->SplitNum (), "ZZZ0");
      j->GetClassCounts (classAccs, knownCounts, predCounts, adjCounts, adjCountsStdError, predDelta, adjDelta);

      totalCM.AddIn (*(j->RandomSplitsResults ()), log);
      totalCmCount++;
      KKStr accLine        = "Acc By Class\t" + splitNumStr;
      KKStr knownLine      = "Known\t"        + splitNumStr;
      KKStr predLine       = "Predicted\t"    + splitNumStr;
      KKStr adjLine        = "Adjusted\t"     + splitNumStr;
      KKStr deltaPredLine  = "Delta Pred\t"   + splitNumStr;
      KKStr deltaAdjLine   = "Delta Adj\t"    + splitNumStr;


      double  totalAcc       = 0.0;
      double  totalDeltaPred = 0.0;
      double  totalDeltaAdj  = 0.0;

      for  (x = 0;  x < numClasses;  x++)
      {
        accLine       << "\t" << StrFormatDouble (classAccs   [x], "zz0.00") << "%";
        knownLine     << "\t" << StrFormatDouble (knownCounts [x], "-Z,ZZZ,ZZ0.0");
        predLine      << "\t" << StrFormatDouble (predCounts  [x], "-Z,ZZZ,ZZ0.0");
        adjLine       << "\t" << StrFormatDouble (adjCounts   [x], "-Z,ZZZ,ZZ0.0");
        deltaPredLine << "\t" << StrFormatDouble (predDelta   [x], "-Z,ZZZ,ZZ0.0");
        deltaAdjLine  << "\t" << StrFormatDouble (adjDelta    [x], "-Z,ZZZ,ZZ0.0");
        totalAcc       += classAccs [x];
        totalDeltaPred += fabs (predDelta[x]);
        totalDeltaAdj  += fabs (adjDelta[x]);
      }


      accLine       << "\t" << StrFormatDouble ((totalAcc        / (double)classAccs.size ()), "ZZ0.00") << "%";
      deltaPredLine << "\t" << StrFormatDouble ((totalDeltaPred  / (double)predDelta.size ()), "ZZ0.00");
      deltaAdjLine  << "\t" << StrFormatDouble ((totalDeltaAdj   / (double)adjDelta.size  ()), "ZZ0.00");

      accLines.push_back            (accLine);
      knownCountLines.push_back     (knownLine);
      predCountLines.push_back      (predLine);
      adjCountLines.push_back       (adjLine);
      deltaPredCountLines.push_back (deltaPredLine);
      deltaAdjCountLines.push_back  (deltaAdjLine);
    }
  }
  double  factor = 0.0;
  if  (totalCmCount > 0)
    factor = 1.0 / (double)totalCmCount;

  totalCM.FactorCounts (factor);

  f << endl << endl
    << "Average Confusion  Matrix" << endl
    << endl;
  totalCM.PrintConfusionMatrixTabDelimited (f);



  f << ""            << "\t" << ""      << "\t" << l1 << endl
    << ""            << "\t" << "Split" << "\t" << l2 << endl
    << "Description" << "\t" << "Num"   << "\t" << l3 << endl;

  f << endl << endl;
  for  (x = 0;  x < knownCountLines.size ();  x++)
    f << knownCountLines[x] << endl;

  f << endl << endl;
  for  (x = 0;  x < predCountLines.size ();  x++)
    f << predCountLines[x] << endl;

  f << endl << endl;
  for  (x = 0;  x < adjCountLines.size ();  x++)
    f << adjCountLines[x] << endl;

  f << endl << endl;
  for  (x = 0;  x < deltaPredCountLines.size ();  x++)
    f << deltaPredCountLines[x] << endl;

  f << endl << endl;
  for  (x = 0;  x < deltaAdjCountLines.size ();  x++)
    f << deltaAdjCountLines[x] << endl;

  f << endl << endl;
  for  (x = 0;  x < knownCountLines.size ();  x++)
    f << accLines[x] << endl;

  VectorFloat  avgAccuracies = totalCM.AccuracyByClass ();
  f << "Avg-Accuracies";
  for  (x = 0;  x < avgAccuracies.size ();  x++)
    f << "\t" << StrFormatDouble (avgAccuracies[x], "zz0.00") << "%";
  f << "\t" << StrFormatDouble (totalCM.Accuracy (), "zz0.00") << "%";
  f << endl;

  f << endl << endl;

  f.close ();
}  /* GenerateFinalResultsReport */




void  RandomSplitJobManager::StatusFileInitialize (ostream& o)
{
  o << "//  RandomSplitJobManager"                        << endl
    << "//  Initialized  " << osGetLocalDateTime ()       << endl
    << endl
    << "ConfigFileName"    << "\t" << configFileName                        << endl
    << "DataFileName"      << "\t" << dataFileName                          << endl
    << "DataIndexFileName" << "\t" << dataIndexFileName                     << endl  
    << "Format"            << "\t" << format->DriverName ()                 << endl
    << "NumFolds"          << "\t" << numFolds                              << endl
    << "NumSplits"         << "\t" << numSplits                             << endl
    << "SplitFraction"     << "\t" << splitFraction                         << endl
    << "MLClasses"      << "\t" << mlClasses->ToCommaDelimitedStr ()  << endl
    << endl;
}  /* StatusFileInitialize */






void  RandomSplitJobManager::LoadRunTimeData ()
{
  bool  cancelFlag  = false;
  bool  successful  = false;
  bool  changesMade = false;

  delete  config;        config       = NULL;
  delete  splits;        splits       = NULL; 
  delete  mlClasses;  mlClasses = NULL;
  delete  data;          data         = NULL;


  if  (!mlClasses)
    mlClasses = new MLClassConstList ();

  data = format->LoadFeatureFile (dataFileName, 
                                  *mlClasses, 
                                  -1,                // _maxCount,
                                  cancelFlag,
                                  successful,
                                  changesMade,
                                  log
                                 );
  if  ((!successful)  &&  (!data))
  {
    KKStr  errorMsg = "RandomSplitJobManager::LoadRunTimeData    *** Error Loading DataFile[" + dataFileName + "] ***";
    throw  errorMsg;
  }

  mlClasses->SortByName ();

  fileDesc = data->FileDesc ();

  config = new TrainingConfiguration2 (fileDesc, configFileName, log, false);
  if  (!config->FormatGood ())
  {
    KKStr  errorMsg = "RandomSplitJobManager::LoadRunTimeData    ***ERROR***     Format of Configuration File[" + configFileName + "] is invalid.";
    throw  errorMsg;
  }

  if  (dataIndexFileName.Empty ())
    dataIndexFileName = osRemoveExtension (dataFileName) + "_RandomSlits_" + StrFormatInt (numSplits, "ZZ00") + ".idx";

  splits = new Orderings (data, dataIndexFileName, numSplits, 1);
}  /* LoadRunTimeData */





void   RandomSplitJobManager::StatusFileProcessLine (const KKStr&  _ln,
                                                     istream&      statusFile
                                                    )
{
  KKStr  ln (_ln);
  KKStr  fieldName  = ln.ExtractToken2 ("\t");
  KKStr  fieldValue = ln.ExtractToken2 ("\t");
  
  if  (fieldName.CompareIgnoreCase ("MLClasses") == 0)
    mlClasses = MLClassConstList::BuildListFromDelimtedStr (fieldValue, ',');

  else if  (fieldName.CompareIgnoreCase ("ConfigFileName") == 0)
    configFileName = fieldValue;

  else if  (fieldName.CompareIgnoreCase ("DataFileName") == 0)
    dataFileName = fieldValue;

  else if  (fieldName.CompareIgnoreCase ("DataIndexFileName") == 0)
    dataIndexFileName = fieldValue;

  else if  (fieldName.CompareIgnoreCase ("Format") == 0)
    format = FeatureFileIO::FileFormatFromStr (fieldValue);

  else if  (fieldName.CompareIgnoreCase ("NumFolds") == 0)
    numFolds = fieldValue.ToInt ();

  else if  (fieldName.CompareIgnoreCase ("NumSplits") == 0)
    numSplits = fieldValue.ToInt ();

  else if  (fieldName.CompareIgnoreCase ("SplitFraction") == 0)
  {
    bool  percentage = (fieldValue.LastChar () == '%');
    if  (percentage)
    {
      fieldValue.ChopLastChar ();
      splitFraction = fieldValue.ToFloat () / 100.0f;
    }
    else
    {
      splitFraction = fieldValue.ToFloat ();
      if  (splitFraction > 1.0f)
        splitFraction = splitFraction / 100.0f;
    }
  }

  else
    JobManager::StatusFileProcessLine (_ln, statusFile);
}  /* StatusFileProcessLine */





void   RandomSplitJobManager::RetrieveRandomSplit (int                    splitNum,
                                                   FeatureVectorListPtr&  trainData,
                                                   FeatureVectorListPtr&  testData
                                                  )
{
  trainData = NULL;
  testData  = NULL;

  if  ((splitNum < 0)  ||  (splitNum >= (int)splits->NumOfOrderings ()))
  {
    log.Level (-1) << endl << endl
                   << "RandomSplitJobManager::RetrieveRandomSplit    ***ERROR***   Invalid SplitNum[" << splitNum << "]" << endl
                   << endl;
    return;
  }

  trainData = new FeatureVectorList (fileDesc, false, log, 1000);
  testData  = new FeatureVectorList (fileDesc, false, log, 1000);

  const
  FeatureVectorListPtr  ordering = splits->Ordering (splitNum);

  MLClassConstList::const_iterator  classIDX;
  for  (classIDX = mlClasses->begin ();  classIDX != mlClasses->end ();  classIDX++)
  {
    MLClassConstPtr  ic = *classIDX;

    FeatureVectorListPtr  examplesThisClass = ordering->ExtractImagesForAGivenClass (ic);
    int  numTrainExamplesNeeded = (int)(0.5 + (double)(examplesThisClass->QueueSize ()) * (double)splitFraction);

    int  numExamplesAddToTrainSet = 0;

    FeatureVectorList::const_iterator  idx;
    for  (idx = examplesThisClass->begin ();  idx != examplesThisClass->end ();  idx++)
    {
      FeatureVectorPtr  example = *idx;

      if  (numExamplesAddToTrainSet < numTrainExamplesNeeded)
      {
        trainData->PushOnBack (example);
        numExamplesAddToTrainSet++;
      }
      else
      {
        testData->PushOnBack (example);
      }
    }
  }
}  /* RetrieveRandomSplit */
