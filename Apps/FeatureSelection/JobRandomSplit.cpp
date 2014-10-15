#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>

#include  <fstream>
#include  <iostream>
#include  <map>
#include  <vector>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif


#include  "MemoryDebug.h"

using namespace std;



#include "BasicTypes.h"
#include "KKQueue.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;



#include "ConfusionMatrix2.h"
#include "CrossValidation.h"
#include "TrainingProcess2.h"
using namespace  MLL;


#include "JobRandomSplit.h"
#include "BinaryJobList.h"
#include "FeatureSelectionTypes.h"
#include "Processor.h"
#include "RandomSplitResults.h"



JobRandomSplit::JobRandomSplit (const JobRandomSplit&  j):
  BinaryJob (j)

{
  splitNum = j.splitNum;
}



JobRandomSplit::JobRandomSplit (ProcessorPtr            _processor,
                                int                     _parentId,
                                const FeatureNumList&   _features,
                                int                     _chgFeatureNum,
                                int32                   _numOfRounds,
                                double                  _cParm,
                                double                  _gammaParm,
                                float                   _aParm,
                                int                     _splitNum
                               ):
     BinaryJob (_processor, _parentId, _features, _chgFeatureNum, _numOfRounds, _cParm, _gammaParm, _aParm),
     splitNum (_splitNum)
{
}




JobRandomSplit::JobRandomSplit (ProcessorPtr  _processor,
                                KKStrParser&  _statusLine
                               ):
     BinaryJob (_processor, _statusLine),
     splitNum (-1)
{
  // The call to the 'BinaryJob' constructor would have arsed the '_statusLine' string for fields that are particular to
  // 'BinaryJob'.  Now we will have to scan for fields that are particular to 'JobRandomSplit'.
  ProcessStatusStr (_statusLine);
}





JobRandomSplit::~JobRandomSplit ()
{
}




JobTypes  JobRandomSplit::JobType ()  const
{
  return  jtRandomSplit;
}




KKStr   JobRandomSplit::ToStatusStr ()
{
  KKStr  statusStr = BinaryJob::ToStatusStr () + "\t" +
    "SplitNum" + "\t" + StrFormatInt (splitNum, "ZZZZZ0");
  return  statusStr;
}




void   JobRandomSplit::EvaluateNode ()
{
  log.Level (9) << "  " << endl;
  log.Level (9) << "JobRandomSplit::EvaluteNode JobId[" << jobId << "]  SplitNum[" << splitNum << "]" << endl;
  status = bjStarted;

  TrainingConfiguration2Ptr  config = processor->Config ();

  config->SetFeatureNums (features);
  config->C_Param     (cParm);
  config->Gamma       (gammaParm);
  config->A_Param     (aParm);
  config->NumOfRounds (numOfRounds);
  config->SelectionMethod (processor->SelectionMethod ());

  bool  cancelFlag = false;

  FeatureVectorListPtr  randSplitTrainData = NULL;
  FeatureVectorListPtr  randSplitTestData  = NULL;

  CreateRandomSplitTrainAndTestExamples (randSplitTrainData, randSplitTestData);

  CrossValidationPtr  crossValidation = new CrossValidation  
                                           (config,
                                            randSplitTrainData,
                                            processor->MLClasses (),
                                            processor->NumOfFolds (),
                                            processor->AlreadyNormalized (),
                                            processor->FileDesc (),
                                            log,
                                            cancelFlag
                                           );

  crossValidation->RunValidationOnly (randSplitTestData);

  testAccuracy     = crossValidation->Accuracy ();
  testAccuracyNorm = crossValidation->AccuracyNorm ();
  testAvgPredProb  = (float)crossValidation->AvgPredProb () * 100.0f;
  testFMeasure     = crossValidation->ConfussionMatrix ()->FMeasure (processor->PositiveClass (), log);

  if  (processor->GradingMethod () == gmAccuracy)
    testGrade = testAccuracy;

  else if  (processor->GradingMethod () == gmAccuracyNorm)
    testGrade = testAccuracyNorm;

  else if  (processor->GradingMethod () == gmFMeasure)
    testGrade = testFMeasure;

  else
    testGrade = testAccuracy;

  testNumSVs  = crossValidation->NumOfSupportVectors ();

  {
    // Save results of this Split in Results file.
    processor->Block ();

    ofstream  f (processor->SummaryResultsFileName ().Str (), ios_base::app);
    RandomSplitResults r (processor->ResultType (), 
                          config, 
                          crossValidation, 
                          randSplitTrainData,
                          osGetHostName (),
                          this, 
                          log
                         );
    r.Write (f);
    f.close ();

    processor->EndBlock ();
  }

  delete  crossValidation;     crossValidation    = NULL;
  delete  randSplitTrainData;  randSplitTrainData = NULL;
  delete  randSplitTestData;   randSplitTestData  = NULL;

  status = bjDone;
}  /* EvaluateNode */






// Will scan for fields that are specific to 'JobRandomSplit'
void  JobRandomSplit::ProcessStatusStr (KKStrParser&  statusStr)
{
  if  (log.Level () > 20)
    log.Level (30) << "BinaryJob::ProcessStatusStr[" << statusStr.Str () << "]" << endl;
  KKStr  fieldName;
  KKStr  fieldValue;

  fieldName = statusStr.GetNextToken ("\t\n\r");

  while  (!fieldName.Empty ())
  {
    fieldName.Upper ();

    fieldValue = statusStr.GetNextToken ("\t");
    fieldValue.TrimLeft ("\n\r\t ");
    fieldValue.TrimRight ("\n\r\t ");
    ProcessStatusField (fieldName, fieldValue);

    fieldName = statusStr.GetNextToken ("\t\n\r");
  }
}  /* ProcessStatusStr */




void  JobRandomSplit::ProcessStatusField (const KKStr&  fieldName,
                                          const KKStr&  fieldValue
                                         )
{
  if  (fieldName.CompareIgnoreCase ("SplitNum") == 0)
  {
    splitNum = fieldValue.ToInt ();
  }
}  /* ProcessStatusField */




void  JobRandomSplit::CreateRandomSplitTrainAndTestExamples (FeatureVectorListPtr&  randSplitTrainData,
                                                             FeatureVectorListPtr&  randSplitTestData
                                                            )
{
  FeatureVectorListPtr  randSplitData = Processor ()->RandomSplit (splitNum);
  if  (randSplitData == NULL)
  {
    log.Level (-1) << endl 
                   << "JobRandomSplit::::CreateRandomSplitTrainAndTestExamples   ***ERROR***   Can not get SplitNum[" << splitNum << "]" << endl
                   << endl;
    return;
  }

  FeatureVectorListPtr  trainData = processor->TrainingData ();
  FeatureVectorListPtr  testData  = processor->TestData     ();
  FeatureVectorListPtr  validationData  = processor->ValidationData ();


  randSplitTrainData = new FeatureVectorList (randSplitData->FileDesc (), false, log);
  randSplitTestData  = new FeatureVectorList (randSplitData->FileDesc (), false, log);


  MLClassConstList::const_iterator  idx;
  for  (idx = processor->MLClasses ()->begin ();  idx != processor->MLClasses ()->end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;

    int  numTrainThisClass = 0;
    if  (trainData)
      numTrainThisClass += trainData->GetClassCount (ic);

    if  (testData)
      numTrainThisClass += testData->GetClassCount (ic);

    if  (!validationData)
      numTrainThisClass = (int)(0.90 * numTrainThisClass + 0.5);

    int  numInTrainDataSet = 0;

    FeatureVectorList::iterator  idx2;
    for  (idx2 = randSplitData->begin ();  idx2 != randSplitData->end ();  idx2++)
    {
      FeatureVectorPtr fv = *idx2;
      if  (fv->MLClass () == ic)
      {
        if  (numInTrainDataSet < numTrainThisClass)
        {
          randSplitTrainData->PushOnBack (fv);
          numInTrainDataSet++;
        }
        else
        {
          randSplitTestData->PushOnBack (fv);
        }
      }
    }
  }
}  /* CreateRandomSplitTrainAndTestExamples */




void  JobRandomSplit::ReFresh (BinaryJob&  j)
{
  BinaryJob::ReFresh (j);
  splitNum = -1;

  if  (j.JobType () == jtRandomSplit)
  {
    JobRandomSplit&  randomSplitJob = dynamic_cast<JobRandomSplit&> (j);
    splitNum = randomSplitJob.splitNum;
  } 
}  /* ReFresh */



void  JobRandomSplit::ReFresh (JobRandomSplit&  j)
{
  BinaryJob::ReFresh (j);
  splitNum = j.splitNum;
}  /* ReFresh */
