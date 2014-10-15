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

#include  "Compressor.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;

#include  "InstrumentDataFileManager.h"
using namespace  SipperHardware;

#include  "ClassificationBiasMatrix.h"
#include  "CrossValidation.h"
#include  "DataBase.h"
#include  "FeatureFileIO.h"
#include  "FeatureFileIOPices.h"
#include  "FileDesc.h"
#include  "MLClass.h"
#include  "ImageFeatures.h"
using namespace MLL;

#include  "JobManager.h"
using namespace  JobManagment;


#include  "RandomSplitJobManager.h"
#include  "RandomSplitJob.h"



RandomSplitJob::RandomSplitJob (JobManagerPtr  _manager,
                                int            _jobId,
                                int            _parentId,
                                int            _splitNum,
                                RunLog&        _log
                                ):
  Job      (_manager, _jobId, _parentId, 1, _log),
  splitNum             (_splitNum),
  biasParms            (NULL),
  randomSplitsResults  (NULL)
{
}




RandomSplitJob::RandomSplitJob (JobManagerPtr  _manager):
  Job   (_manager),
  splitNum            (0),
  biasParms           (NULL),
  randomSplitsResults (NULL)
{
}





RandomSplitJob::RandomSplitJob (const RandomSplitJob&  j):
   Job (j),
   splitNum             (j.splitNum),
   biasParms            (NULL),
   randomSplitsResults  (NULL)
{
  if  (j.biasParms)
    biasParms = new ClassificationBiasMatrix (*j.biasParms);

  if  (j.randomSplitsResults)
    randomSplitsResults = new ConfusionMatrix2 (*j.randomSplitsResults);
}




RandomSplitJob::~RandomSplitJob ()
{
}




RandomSplitJobManagerPtr   RandomSplitJob::Manager ()
{
  JobManagerPtr  m = Job::Manager ();
  return  dynamic_cast<RandomSplitJobManagerPtr>(m);
}



// All classes derived from 'Job' will be required to implement this method 
// and register it with Job::RegisterConstructor
JobPtr  RandomSplitJob::CreateEmptyInstance (JobManagerPtr  _manager)
{
  return new RandomSplitJob (_manager);
}



RandomSplitJobPtr  RandomSplitJob::Duplicate ()  const
{
  return new RandomSplitJob (*this);
}  /* Duplicate */




const char*  RandomSplitJob::JobType ()  const
{
  return  "RandomSplitJob";
}



void   RandomSplitJob::ReFresh (Job&  _j)
{
  RandomSplitJob& j = dynamic_cast<RandomSplitJob&> (_j);

  Job::ReFresh (j);
  splitNum = j.splitNum;


  delete  biasParms;  
  biasParms= NULL;
  if  (j.biasParms)
    biasParms = new ClassificationBiasMatrix (*j.biasParms);

  delete  randomSplitsResults;
  randomSplitsResults= NULL;
  if  (j.randomSplitsResults)
    randomSplitsResults = new ConfusionMatrix2 (*j.randomSplitsResults);
}  /* ReFresh */


  

// Any SubClass that implements this method must call its base class version 1st.
KKStr  RandomSplitJob::ToStatusStr ()
{
  return  Job::ToStatusStr () + "\t" + "splitNum" + "\t" + StrFormatInt (splitNum, "ZZZZZZ0");
}




// Any SubClass of Job  needs to define this method.  Whenever the 'ProcessStatusStr' method can not identify a field
// it will call this method to let the child Class process the field.
void  RandomSplitJob::ProcessStatusField (const KKStr&  fieldName,
                                          const KKStr&  fieldValue
                                         )
{
  if  (fieldName.EqualIgnoreCase ("SplitNum"))
    splitNum = fieldValue.ToInt ();
  else
    Job::ProcessStatusField (fieldName, fieldValue);
}





void  RandomSplitJob::CompletedJobDataWrite (ostream& o)
{
  o << "<RandomSplitJob>"  << endl;

  if  (biasParms)
    biasParms->WriteXML (o);

  if  (randomSplitsResults)
    randomSplitsResults->WriteXML (o);

  o << "</RandomSplitJob>"  << endl;
}  /* CompletedJobDataWrite */



 
// Works with 'WriteCompletedJobData';  You use this to load in data written by 'WriteCompletedJobData'
void  RandomSplitJob::CompletedJobDataRead (istream& i)
{
  char buff[10240];
  KKStr  ln (512);

  if  (i.eof ())
  {
    // We are already at the end of the file;   there is no data for us to read.
    return;
  }

  i.getline (buff, sizeof (buff));
  while  (!i.eof ())
  {
    ln = buff;
    ln.TrimLeft ();
    ln.TrimRight ();
    if  (ln.CompareIgnoreCase ("</RandomSplitJob>") == 0)
    {
      // We have reached end of 'RandomSplitJob' text.
      break;
    }

    if  (ln.CompareIgnoreCase ("<ClassificationBiasMatrix>") == 0)
    {
      delete  biasParms;
      biasParms = ClassificationBiasMatrix::BuildFromIstreamXML (i, *(Manager ()->MLClasses ()), log);
    }

    else if  (ln.CompareIgnoreCase ("<ConfusionMatrix2>") == 0)
    {
      delete  randomSplitsResults;
      randomSplitsResults = ConfusionMatrix2::BuildFromIstreamXML (i, log);
    }

    if  (!i.eof ())
      i.getline (buff, sizeof (buff));
  }
}  /* CompletedJobDataRead*/





void  RandomSplitJob::ProcessNode ()
{
  Status (Job::jsStarted);

  FeatureVectorListPtr  trainData = NULL;
  FeatureVectorListPtr  testData  = NULL;

  Manager ()->RetrieveRandomSplit (splitNum, trainData, testData);

  delete  biasParms;            biasParms           = NULL;
  delete  randomSplitsResults;  randomSplitsResults = NULL;

  int  numFolds = Manager ()->NumFolds ();

  // Run 'numFolds' Fold on TrainData to derive Bias Adjustment Parameters.
  {
    FeatureVectorListPtr  trainDataStratified = trainData->StratifyAmoungstClasses (numFolds);

    bool  cancelFlag = false;
    CrossValidation  cv (Manager ()->Config (), trainDataStratified, Manager ()->MLClasses (), numFolds, false, trainData->FileDesc (), log, cancelFlag);
    cv.RunCrossValidation ();
    biasParms = new ClassificationBiasMatrix (*(cv.ConfussionMatrix ()), log);

    delete  trainDataStratified;
    trainDataStratified = NULL;
  }

  {
    bool  cancelFlag = false;
    CrossValidation  cv (Manager ()->Config (), trainData, Manager ()->MLClasses (), numFolds, false, trainData->FileDesc (), log, cancelFlag);
    cv.RunValidationOnly (testData, NULL);

    randomSplitsResults = new ConfusionMatrix2 (*cv.ConfussionMatrix ());

    VectorDouble  knownCounts = randomSplitsResults->CountsByKnownClass ();
    VectorDouble  predCounts  = randomSplitsResults->PredictedCounts ();

    VectorDouble  adjCounts;
    VectorDouble  adjCountsStdError;

    biasParms->PerformAdjustmnts (predCounts, adjCounts, adjCountsStdError);
  }

  Status (Job::jsDone);
}  /* ProcessNode */




void  RandomSplitJob::GetClassCounts (VectorFloat&   classAccs,
                                      VectorDouble&  knownCounts,
                                      VectorDouble&  predCounts,
                                      VectorDouble&  adjCounts,
                                      VectorDouble&  adjCountsStdError,
                                      VectorDouble&  predDelta,        // The difference between Predicted Counts and Known
                                      VectorDouble&  adjDelta          // The difference between Adjusted  Counts and Known
                                     )
{
  classAccs   = randomSplitsResults->AccuracyByClass ();
  knownCounts = randomSplitsResults->CountsByKnownClass ();
  predCounts  = randomSplitsResults->PredictedCounts ();
  biasParms->PerformAdjustmnts (predCounts, adjCounts, adjCountsStdError);

  predDelta.clear ();
  adjDelta.clear ();
  
  for  (KKU::uint x = 0;  x < knownCounts.size ();  x++)
  {
    predDelta.push_back (predCounts[x] - knownCounts[x]);
    adjDelta.push_back  (adjCounts[x]  - knownCounts[x]);
  }
}  /* GetClassCounts */




void  RandomSplitJob::PrintClassCounts (ostream&  o)
{
  VectorDouble  knownCounts;
  VectorDouble  predCounts;
  VectorDouble  adjCounts;
  VectorDouble  adjCountsStdError;
  VectorDouble  predDelta;       
  VectorDouble  adjDelta;
  VectorFloat   accuracies;
  KKU::uint  x = 0;

  GetClassCounts (accuracies, knownCounts, predCounts, adjCounts, adjCountsStdError, predDelta, adjDelta);

  KKStr  l1, l2, l3, l4, l5, l6;

  Manager ()->MLClasses ()->ExtractThreeTitleLines (l1, l2, l3);

  o << ""            << "\t" << ""       << "\t" << l1 << endl
    << ""            << "\t" << ""       << "\t" << l2 << endl
    << "Description" << "\t" << "Totals" << "\t" << l3 << endl;

  double  totalKnownCount = 0.0;
  double  totalPredCount  = 0.0;
  double  totalAdjCounts  = 0.0;
  double  totalStdError   = 0.0;
  double  totalPredDelta  = 0.0;
  double  totalAdjDelta   = 0.0;

  for  (x = 0;  x < knownCounts.size ();  x++)
  {
    totalKnownCount += knownCounts       [x];
    totalPredCount  += predCounts        [x];
    totalAdjCounts  += adjCounts         [x];
    totalStdError   += adjCountsStdError [x];
    totalPredDelta  += predDelta         [x];
    totalAdjDelta   += adjDelta          [x];
  }

  l1 = "";
  l2 = "";
  l3 = "";
  l4 = "";
  l5 = "";
  l6 = "";

  l1 << "Known Count"  << "\t" << StrFormatDouble (totalKnownCount, "-Z,ZZZ,ZZ0.0");
  l2 << "Pred  Count"  << "\t" << StrFormatDouble (totalPredCount,  "-Z,ZZZ,ZZ0.0");
  l3 << "Adj   Count"  << "\t" << StrFormatDouble (totalAdjCounts,  "-Z,ZZZ,ZZ0.0");
  l4 << "Adj StdError" << "\t" << StrFormatDouble (totalStdError,   "-Z,ZZZ,ZZ0.0");
  l5 << "Pred Delta"   << "\t" << StrFormatDouble (totalPredDelta,  "-Z,ZZZ,ZZ0.0");
  l6 << "Adj Delta"    << "\t" << StrFormatDouble (totalAdjDelta,   "-Z,ZZZ,ZZ0.0");

  for  (x = 0;  x < knownCounts.size ();  x++)
  {
    l1 << "\t" << StrFormatDouble (knownCounts        [x], "-Z,ZZZ,ZZ0.0");
    l2 << "\t" << StrFormatDouble (predCounts         [x], "-Z,ZZZ,ZZ0.0");
    l3 << "\t" << StrFormatDouble (adjCounts          [x], "-Z,ZZZ,ZZ0.0");
    l4 << "\t" << StrFormatDouble (adjCountsStdError  [x], "-Z,ZZZ,ZZ0.0");
    l5 << "\t" << StrFormatDouble (predDelta          [x], "-Z,ZZZ,ZZ0.0");
    l6 << "\t" << StrFormatDouble (adjDelta           [x], "-Z,ZZZ,ZZ0.0");
  }

  o << l1 << endl;
  o << l2 << endl;
  o << l3 << endl;
  o << l4 << endl;
  o << l5 << endl;
  o << l6 << endl;
}  /* PrintClassCounts */
