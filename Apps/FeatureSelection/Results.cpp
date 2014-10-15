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
#include "FeatureVector.h"
#include "TrainingProcess2.h"
using namespace  MLL;


#include "Results.h"
#include "BinaryJob.h"
#include "FeatureSelectionTypes.h"
//#include "JobRandomSplit.h"
#include "Processor.h"



Results::Results (RunLog&  _log):
    configFileName      (),
    confusionMatrix     (NULL),
    jobId               (0),
    hostName            (""),
    log                 (_log),
    machineType         (MachineType_NULL),
    numFeatures         (0.0),
    numFeaturesWeighted (0.0f),
    numSVs              (0.0),
    resultType          (frtNULL),
    selectionMethod     (SelectionMethod_NULL),
    svmParameters       (),
    testTime            (0.0),
    totalNumSVs         (0.0),
    trainingTime        (0.0)
{
}


Results::Results (FinalResultType          _resultType,
                  TrainingConfiguration2Ptr _config,
                  CrossValidationPtr        _crossValidation,
                  FeatureVectorListPtr      _trainData,
                  const KKStr&              _hostName,
                  BinaryJobPtr              _job,
                  RunLog&                   _log
                 ):
  configFileName      (),
  confusionMatrix     (NULL),
  hostName            (_hostName),
  jobId               (0),
  log                 (_log),
  machineType         (MachineType_NULL),
  numFeatures         (0.0),
  numFeaturesWeighted (0.0f),
  numSVs              (0.0),
  resultType          (_resultType),
  selectionMethod     (SelectionMethod_NULL),
  svmParameters       (),
  testTime            (0.0),
  totalNumSVs         (0.0),
  trainingTime        (0.0)

{
  confusionMatrix = new ConfusionMatrix2 (*(_crossValidation->ConfussionMatrix ()));
  jobId               = _job->JobId ();
  numFeatures         = _config->AvgNumOfFeatures ();
  numFeaturesWeighted = _config->AvgNumOfFeatures (_trainData);
  numSVs              = _crossValidation->NumSVs ();
  testTime            = _crossValidation->TestTimeTotal  ();
  totalNumSVs         = _crossValidation->TotalNumSVs    ();
  trainingTime        = _crossValidation->TrainTimeTotal ();

  configFileName      = _config->FileName ();
  machineType         = _config->MachineType     ();
  selectionMethod     = _config->SelectionMethod ();
  svmParameters       = _config->SVMparamREF     ().ToString ();
}




Results::~Results ()
{
  delete  confusionMatrix;
}



KKStr  Results::MachineTypeStr () const
{
  return  MachineTypeToStr (machineType);
}



KKStr  Results::SelectionMethodStr () const
{
  return  SelectionMethodToStr (selectionMethod);
}


KKStr  Results::ResultTypeStr ()  const
{
  return FinalResultTypeToStr (resultType);
}




float  Results::Accuracy ()  const
{
  if  (!confusionMatrix)
    return 0.0f;
  else
    return  (float)(confusionMatrix->Accuracy ());
}  /* Accuracy */




VectorFloat Results::AccuracyByClass  ()  const
{
  if  (!confusionMatrix)
    return VectorFloat ();
  else
    return  confusionMatrix->AccuracyByClass ();
}



float  Results::AccuracyNorm ()  const
{
  if  (!confusionMatrix)
    return 0.0f;
  else
    return  (float)(confusionMatrix->AccuracyNorm ());
}  /* Accuracy */




KKStr  Results::Description ()  const
{
  KKStr d (128);
  d = ResultTypeStr () + "-" + MachineTypeStr () + "-" + SelectionMethodStr ();
  return d;
}




void  Results::Write (ostream&  o)
{
  o << "<Results>"                                                        << endl
    << "ConfigFileName"      << "\t" << configFileName                     << endl
    << "JobId"               << "\t" << jobId                              << endl
    << "ResultType"          << "\t" << FinalResultTypeToStr (resultType) << endl
    << "MachineType"         << "\t" << MachineTypeStr ()                  << endl
    << "NumFeatures"         << "\t" << numFeatures                        << endl
    << "NumFeaturesWeighted" << "\t" << numFeaturesWeighted                << endl
    << "NumSVs"              << "\t" << numSVs                             << endl
    << "SelectionMethod"     << "\t" << SelectionMethodStr ()              << endl
    << "SvmParameters"       << "\t" << svmParameters                      << endl
    << "TestTime"            << "\t" << testTime                           << endl
    << "TotalNumSVs"         << "\t" << totalNumSVs                        << endl
    << "TrainingTime"        << "\t" << trainingTime                       << endl
    << "HostName"            << "\t" << hostName                           << endl;

  if  (confusionMatrix)
    confusionMatrix->WriteXML (o);
  o << "</Results>" << endl;
}  /* Write */



void  Results::Read (istream&  i)
{
  char  buff[50000];

  delete  confusionMatrix;
  confusionMatrix = NULL;

  i.getline (buff, sizeof (buff));
  KKStr l  (buff);

  while  ((!i.eof ())  &&  (!l.EqualIgnoreCase ("</Results>")))
  {
    KKStr  fieldName = l.ExtractToken2 ("\t");

    if  (fieldName.EqualIgnoreCase ("ConfigFileName"))
      configFileName = l.ExtractToken2 ("\t");

    else if  (fieldName.EqualIgnoreCase ("<ConfusionMatrix2>"))
    {
      delete  confusionMatrix;  confusionMatrix = NULL;
      confusionMatrix = ConfusionMatrix2::BuildFromIstreamXML (i, log);
    }

    else if  (fieldName.EqualIgnoreCase ("HostName"))
      hostName = l.ExtractToken2 ("\n\r\t");

    else if  (fieldName.EqualIgnoreCase ("JobId"))
      jobId = l.ExtractTokenInt ("\t");

    else if  (fieldName.EqualIgnoreCase ("MachineType"))
      machineType = MachineTypeFromStr (l.ExtractToken2 ("\t"));

    else if  (fieldName.EqualIgnoreCase ("NumFeatures"))
      numFeatures = l.ExtractTokenDouble ("\t");

    else if  (fieldName.EqualIgnoreCase ("NumFeaturesWeighted"))
      numFeaturesWeighted = (float)l.ExtractTokenDouble ("\t");

    else if  (fieldName.EqualIgnoreCase ("NumSupportVectors"))
      numSVs = l.ExtractTokenDouble ("\t");

    else if  (fieldName.EqualIgnoreCase ("NumSVs"))
      numSVs = l.ExtractTokenDouble ("\t");

    else if  (fieldName.EqualIgnoreCase ("ResultType"))
      resultType = FinalResultTypeFromStr (l.ExtractToken2 ("\t"));
    
    else if  (fieldName.EqualIgnoreCase ("SelectionMethod"))
      selectionMethod = SelectionMethodFromStr (l.ExtractToken2 ("\t"));

    else if  (fieldName.EqualIgnoreCase ("svmParameters"))
      svmParameters = l.ExtractToken2 ("\t");

    else if  (fieldName.EqualIgnoreCase ("TestTime"))
      testTime = l.ExtractTokenDouble("\t");

    else if  (fieldName.EqualIgnoreCase ("TotalNumSVs"))
      totalNumSVs = l.ExtractTokenDouble ("\t");

    else if  (fieldName.EqualIgnoreCase ("TrainingTime"))
      trainingTime = l.ExtractTokenDouble("\t");


    i.getline (buff, sizeof (buff));
    l = buff;
  }

}  /* Read */





ResultsPtr  Results::ConstructFromStream (istream&  i,
                                          RunLog&   log
                                         )
{
  ResultsPtr  results = new Results (log);
  results->Read (i);
  return  results;
} /* ConstructFromStream */





Results&   Results::operator+= (const Results&  right)
{
  if  (right.ConfusionMatrix () != NULL)
  {
    if  (confusionMatrix == NULL)
      confusionMatrix = new ConfusionMatrix2 (*(right.ConfusionMatrix ()));
    else
      confusionMatrix->AddIn (*(right.ConfusionMatrix ()), log);
  }

  numFeatures         += right.numFeatures;
  numFeaturesWeighted += right.numFeaturesWeighted;
  numSVs              += right.numSVs;
  testTime            += right.testTime;
  totalNumSVs         += right.totalNumSVs;
  trainingTime        += right.trainingTime;

  if  (configFileName.Empty ())
    configFileName = right.configFileName;

  else if  (!configFileName.EqualIgnoreCase (right.configFileName))
    configFileName = "";


  if  (machineType == MachineType_NULL)
    machineType = right.machineType;

  else if  (machineType != right.machineType)
    machineType = MachineType_NULL;


  if  (resultType == frtNULL)
    resultType = right.resultType;

  else if  (resultType != right.resultType)
    resultType = frtNULL;


  if  (selectionMethod ==  SelectionMethod_NULL)
    selectionMethod = right.selectionMethod;

  else if  (selectionMethod != right.selectionMethod)
    selectionMethod = SelectionMethod_NULL;


  if  (svmParameters.Empty ())
    svmParameters = right.svmParameters;

  else if  (!svmParameters.EqualIgnoreCase (right.svmParameters))
    svmParameters = "";

  return  *this;
}  /* operator+= */




Results&   Results::operator/= (float divisor)
{
  if  (divisor == 0.0f)
    return  *this;

  if  (confusionMatrix)
    confusionMatrix->FactorCounts (1.0f / divisor);

  numFeatures         /= divisor;
  numFeaturesWeighted /= divisor;
  numSVs              /= divisor;
  testTime            /= divisor;
  totalNumSVs         /= divisor;
  trainingTime        /= divisor;

  return *this;
}  /* operator/= */




Results&   Results::operator/= (int divisor)
{
  if  (divisor == 0)
    return *this;

  if  (confusionMatrix)
    confusionMatrix->FactorCounts (1.0f / (float)divisor);

  numFeatures         /= (double)divisor;
  numFeaturesWeighted /= (float)divisor;
  numSVs              /= (double)divisor;
  testTime            /= (double)divisor;
  totalNumSVs         /= (double)divisor;
  trainingTime        /= (double)divisor;

  return *this;
}  /* operator/= */




/*!
 \brief  Creates a List of Classes from associated Confussion Matrix.
 \details  caller will be responsavle for deleting this list.
 */
MLClassConstListPtr  Results::MLClasses ()  const
{
  if  (!confusionMatrix)
    return  new MLClassConstList ();
  else
    return  new MLClassConstList (confusionMatrix->MLClasses ());
}




