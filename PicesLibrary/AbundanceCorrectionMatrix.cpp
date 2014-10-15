#include "FirstIncludes.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "MemoryDebug.h"
#include "BasicTypes.h"
using namespace std;



#include "OSservices.h"
#include "RunLog.h"
using namespace KKU;

#include "SipperVariables.h"
using namespace SipperHardware;

#include "AbundanceCorrectionMatrix.h"
#include "MLLTypes.h"
#include "ClassStatistic.h"
#include "MLClass.h"
//#include  "ImageFeatures.h"
using namespace MLL;






AbundanceCorrectionMatrix::AbundanceCorrectionMatrix (MLClassConstList&  _classes,
                                                      MLClassConstPtr    _otherClass,
                                                      RunLog&               _log
                                                     ):
  classes          (_classes),
  dateTimeComputed (osGetLocalDateTime ()),
  knownByClass     (NULL),
  numCorrect       (0),
  numPredictions   (0),
  otherClass       (_otherClass),
  predByClass      (NULL),
  predictions      (NULL),
  probOfDetection  (),
  probOfFalseAlarm (),
  valid            (true)

{
  if  (otherClass == NULL)
  {
    _log.Level (-1) << endl << "AbundanceCorrectionMatrix   ***ERROR***    _otherClass == NULL  defaulting to 'OtherClass'," << endl << endl;
    otherClass = MLClass::CreateNewMLClass ("Other", -1);
    int32  seqNum = 0;
    while  (classes.PtrToIdx (otherClass) != NULL)
    {
      ++seqNum;
      KKStr  otherName = "Other_" + StrFormatInt (seqNum, "000");
      otherClass = MLClass::CreateNewMLClass (otherName, -1);
    }
  }

  numClasses = classes.size () + 1;
  AllocateMemory ();
}


AbundanceCorrectionMatrix::AbundanceCorrectionMatrix (const KKStr&  configFileName,
                                                      RunLog&       _log
                                                     ):
  classes          (),
  dateTimeComputed (osGetLocalDateTime ()),
  knownByClass     (NULL),
  numCorrect       (0),
  numPredictions   (0),
  otherClass       (NULL),
  predByClass      (NULL),
  predictions      (NULL),
  probOfDetection  (),
  probOfFalseAlarm (),
  valid            (true)
{
  ReadForConfigFileName (configFileName, _log);
}




AbundanceCorrectionMatrix::~AbundanceCorrectionMatrix ()
{
  CleanUp ();
}



void  AbundanceCorrectionMatrix::AllocateMemory ()
{
  numClasses = classes.QueueSize () + 1;
  otherClassIdx = numClasses - 1;

  AllocatePredictionsTable ();

  knownByClass = AllocateUint32Array (numClasses);
  predByClass = AllocateUint32Array (numClasses);
}  /* AllocateMemory */



void  AbundanceCorrectionMatrix::AllocatePredictionsTable ()
{
  predictions = new uint32*[numClasses];
  for  (uint32 x = 0;  x < numClasses;  ++x)
    predictions[x] = AllocateUint32Array (numClasses);
}



uint32*   AbundanceCorrectionMatrix::AllocateUint32Array (uint32 len)
{
  uint32*  a = new uint32[len];
  for  (uint32 x = 0;  x < len;  ++x)
    a[x] = 0;
  return a;
}  /* AllocateUint32Array */



void  AbundanceCorrectionMatrix::CleanUp ()
{
  if  (predictions)
  {
    for  (uint32 x = 0;  x < numClasses;  ++x)
    {
      delete  predictions[x];
      predictions[x] = NULL;
    }
    delete  predictions;
    predictions = NULL;
  }
  
  delete  knownByClass;  knownByClass = NULL;
  delete  predByClass;   predByClass  = NULL;
}  /* CleanUp */



int32  AbundanceCorrectionMatrix::LookUpClassIdx (MLClassConstPtr c)
{
  if  (c == otherClass)
    return otherClassIdx;
  else
    return classes.PtrToIdx (c);
}  /* LookUpClassIdx */



void  AbundanceCorrectionMatrix::Prediction (MLClassConstPtr  knownClass,
                                             MLClassConstPtr  predClass,
                                             RunLog&             log
                                            )
{
  if  (knownClass == NULL)
  {
    log.Level (-1) << endl << "AbundanceCorrectionMatrix::Prediction   ***ERROR***   knownClass == NULL" << endl
      << endl;
    return;
  }

  if  (predClass == NULL)
  {
    log.Level (-1) << endl << "AbundanceCorrectionMatrix::Prediction   ***ERROR***   predClass == NULL" << endl
      << endl;
    return;
  }

  int32  knownClassIdx = LookUpClassIdx (knownClass);
  int32  predClassIdx  = LookUpClassIdx (predClass);

  if  (knownClassIdx < 0)
    knownClassIdx = otherClassIdx;

  if  (predClassIdx < 0)
    predClassIdx = otherClassIdx;

  ++(predictions[predClassIdx][knownClassIdx]);
  ++(knownByClass[knownClassIdx]);
  ++(predByClass[predClassIdx]);

  if  (knownClass == predClass)
    ++numCorrect;
  ++numPredictions;
}  /* Prediction */



template<typename T>
void   AbundanceCorrectionMatrix::AllocateVector (vector<T>&  v,  uint32 len)
{
  v.clear ();
  while  (v.size () < len)
    v.push_back ((T)0);
}



/** Using confusion matrix already built will compute probOfDetection(Sensitivity)  and  probOfFalseAlarm(Specifiity). */
void  AbundanceCorrectionMatrix::ComputeStatistics ()
{
  AllocateVector (probOfDetection,  numClasses);
  AllocateVector (probOfFalseAlarm, numClasses);

  //Create Known Class Factors to normalize for class distribution.
  vector<float>  knownClassFactors (numClasses, 0.0f);
  for  (uint32 x = 0;  x < numClasses;  ++x)
    knownClassFactors[x] = (float)numPredictions / (float)knownByClass[x];


  for  (uint32  classIdx = 0;  classIdx < numClasses;  ++classIdx)
  {
    float  truePositives = (float)(predictions[classIdx][classIdx]) * knownClassFactors[classIdx];
    float  falseNegatives = 0.0f;
    float  falsePositives = 0.0f;
    for  (uint32  x = 0;  x < numClasses;  ++x)
    {
      if  (x != classIdx)
      {
        falseNegatives += (float)(predictions[x][classIdx]) * knownClassFactors[classIdx];
        falsePositives += (float)(predictions[classIdx][x]) * knownClassFactors[x];
      }
    }

    if  ((truePositives + falseNegatives) == 0.0)
      probOfDetection [classIdx] = 0.0;
    else
      probOfDetection [classIdx] = ((float)truePositives / (float)(truePositives + falseNegatives));
  
    if  ((truePositives + falsePositives) == 0)
      probOfFalseAlarm[classIdx] = 0.0;
    else
      probOfFalseAlarm[classIdx] = ((float)truePositives / (float)(truePositives + falsePositives));
  }
}  /* ComputeStatistics */



void  AbundanceCorrectionMatrix::SaveForConfigFileName (const KKStr&  configFileName)
{
  KKStr  abundanceAdjDirName = SipperVariables::TrainingModelsSaveFilesDir ();
  osCreateDirectoryPath (abundanceAdjDirName);
  KKStr  fileName =  osAddSlash (abundanceAdjDirName) + osGetRootName (configFileName) + "_AbundanceAdj.txt";

  ofstream f (fileName.Str (), ios_base::trunc);
  WriteXml (f);
  f.close ();
}  /* SaveForConfigFileName */



void  AbundanceCorrectionMatrix::ReadForConfigFileName (const KKStr&  configFileName,
                                                        RunLog&       log
                                                       )
{
  KKStr  abundanceAdjDirName = SipperVariables::TrainingModelsSaveFilesDir ();
  osCreateDirectoryPath (abundanceAdjDirName);
  KKStr  fileName =  osAddSlash (abundanceAdjDirName) + osGetRootName (configFileName) + "_AbundanceAdj.txt";

  ifstream  f (fileName.Str ());
  if  (!f.is_open ())
  {
    valid = false;
    log.Level (-1) << endl
      << "ReadForConfigFileName   ***ERROR***     Could not open file[" + abundanceAdjDirName + "]." << endl
      << endl;
  }
  else
  {
    this->ReadXml (f, log);
    f.close ();
  }
}  /* ReadForConfigFileName */



template<typename T>
KKStr  AbundanceCorrectionMatrix::VectorToStr (vector<T>&  v)
{
  KKStr  result (v.size () * 7);
  for  (uint32  x = 0;  x < v.size ();  ++x)
  {
    if  (x > 0)  result << "\t";
    result << v[x];
  }
  return  result;
}  /* VectorToStr */



template<typename T>
istream&  AbundanceCorrectionMatrix::ReadVector (vector<T>&  v,
                                                 istream&    i
                                                )
{
  v.clear ();
  bool  eof = false;
  bool  eol = false;

  KKStr  token = osReadNextToken (i, "\t", eof, eol);
  while  ((!eol)  &&  (!eof))
  {
    v.push_back ((T)(token.ToDouble ()));
    token = osReadNextToken (i, "\t", eof, eol);
  }
  return  i;
}  /* ReadVector */



template<typename T>
KKStr  AbundanceCorrectionMatrix::ArrayToStr (T* a, uint32 len)
{
  KKStr  result (7 * len);
  for  (uint32  x = 0;  x < len;  ++x)
  {
    if  (x > 0)  result << "\t";
    result << a[x];
  }
  return  result;
}  /* ArrayToStr */



template<typename T>
void  AbundanceCorrectionMatrix::ReadArray (uint32    len,
                                            T*&       a,
                                            istream&  i
                                           )
{
  delete  a;
  a = new T[len];
  uint32  idx = 0;
  bool eof = false;
  bool eol = false;

  KKStr  token = osReadNextToken (i, "\t", eof, eol);
  while  ((!eol)  &&  (!eof))
  {
    if  (idx < len)
      a[idx] = (T)(token.ToDouble ());
    token = osReadNextToken (i, "\t", eof, eol);
    ++idx;
  }
  return;
}  /* ReadArray */



void  AbundanceCorrectionMatrix::WriteXml (ostream&  o)
{
  o << "<AbundanceCorrectionStats>"                                          << endl
    << "DateTimeComputed"  << "\t" << dateTimeComputed                       << endl
    << "NumClasses"        << "\t" << numClasses                             << endl
    << "MLClasses"      << "\t" << classes.ToCommaDelimitedStr ()         << endl
    << "OtherClass"        << "\t" << otherClass->Name ()                    << endl
    << "OtherClassIdx"     << "\t" << otherClassIdx                          << endl
    << "NumCorrect"        << "\t" << numCorrect                             << endl
    << "NumPredictions"    << "\t" << numPredictions                         << endl
    << "KnownByClass"      << "\t" << ArrayToStr (knownByClass, numClasses)  << endl
    << "PredByClass"       << "\t" << ArrayToStr (predByClass,  numClasses)  << endl
    << "ProbOfDetection"   << "\t" << VectorToStr (probOfDetection)          << endl
    << "ProbOfFalseAlarm"  << "\t" << VectorToStr (probOfFalseAlarm)         << endl;

  if  (predictions)
  {
    for  (uint x = 0;  x < numClasses;  ++x)
    {
      o << "PredictionsRow" << "\t" << x;
      for  (uint y = 0;  y < numClasses;  ++y)
      {
        o << "\t" << predictions[x][y];
      }
      o << endl;
    }
  }

  o << "</AbundanceCorrectionStats>"                                         << endl;
}  /* WriteXml*/



void  AbundanceCorrectionMatrix::ReadXml (istream&  i,
                                          RunLog&   log
                                         )
{
  CleanUp ();

  bool  eof = false;
  bool  eol = false;

  otherClass    = NULL;
  otherClassIdx = -1;
  numClasses    = 0;
  valid         = true;

  KKStr  token = osReadNextToken (i, "\t", eof, eol);
  while  (!eof)
  {
    if  (!eol)
    {
      if  (token.EqualIgnoreCase ("</AbundanceCorrectionStats>"))
        break;

      else if  (token.EqualIgnoreCase ("DateTimeComputed"))
        dateTimeComputed = DateTime (osReadNextToken (i, "\t", eof, eol));

      else if  (token.EqualIgnoreCase ("NumClasses"))
        numClasses = osReadNextToken (i, "\t", eof, eol).ToInt ();

      else if  (token.EqualIgnoreCase ("MLClasses"))
      {
        classes.Clear ();
        eol = false;
        KKStr  className = osReadNextToken (i, ",", eof, eol);
        while  (!eol)
        {
          if  (!className.Empty ())
            classes.PushOnBack (MLClass::CreateNewMLClass (className, -1));
          className = osReadNextToken (i, ",", eof, eol);
        }
      }

      else if  (token.EqualIgnoreCase ("OtherClass"))
        otherClass = MLClass::CreateNewMLClass (osReadNextToken (i, "\t", eof, eol), -1);

      else if  (token.EqualIgnoreCase ("OtherClassIdx"))
        otherClassIdx = osReadNextToken (i, "\t", eof, eol).ToInt ();

      else if  (token.EqualIgnoreCase ("KnownByClass"))
        ReadArray (numClasses, knownByClass, i);

      else if  (token.EqualIgnoreCase ("PredByClass"))
        ReadArray (numClasses, predByClass, i);

      else if  (token.EqualIgnoreCase ("NumCorrect"))
        numCorrect = osReadNextToken (i, "\t", eof, eol).ToInt ();

      else if  (token.EqualIgnoreCase ("NumPredictions"))
        numPredictions = osReadNextToken (i, "\t", eof, eol).ToInt ();

      else if  (token.EqualIgnoreCase ("PredictionsRow"))
      {
        if  (!predictions)
          AllocatePredictionsTable ();
        int32  rowNum = osReadNextToken (i, "\t", eof, eol).ToInt ();
        if  ((rowNum < 0)  ||  (rowNum >= (int)numClasses))
        {
          valid = false;
          log.Level (-1) << endl
            << "ReadXml  'PredictionsRow'  ***ERROR***    Invalid RowNum[" << rowNum << "]." << endl
            << endl;
        }
        else
        {
          ReadArray (numClasses, predictions[rowNum], i);
        }
      }

      else if  (token.EqualIgnoreCase ("ProbOfDetection"))
        ReadVector (probOfDetection, i);

      else if  (token.EqualIgnoreCase ("ProbOfFalseAlarm"))
        ReadVector (probOfFalseAlarm, i);
    }
    if  (!i.eof ())
      token = osReadNextToken (i, "\t", eof, eol);
  }

  KKStr  errMsgs; 

  if  (otherClass == NULL)
  {
    valid = false;
    errMsgs << "No 'OtherClass' defined." << endl;
  }
  else if  (otherClassIdx < 0)
  {
    otherClassIdx = classes.size ();
  }
  else if  ((uint32)otherClassIdx >= numClasses)
  {
    valid = false;
    errMsgs << "otherClassIdx[" << otherClassIdx << "] i sot of the range of numClasses[" << numClasses << "]." << endl;
  }

  if  (numClasses != (classes.size () + 1))
  {
    valid = false;
    errMsgs << "NumClasses[" << numClasses << "] does not agree with classes.size()[" << classes.size () << "] plus one for other class." << endl;
  }

  if  (numClasses < 1)
  {
    valid = false;
    errMsgs << "'numClasses' was not defined." << endl;
  }

  if  (probOfDetection.size () != numClasses)
  {
    valid = false;
    errMsgs << "ProbOfDetection.size[" << probOfDetection.size () << "] not equal NumClasses[" << numClasses << "]." << endl;
  }

  if  (probOfFalseAlarm.size () != numClasses)
  {
    valid = false;
    errMsgs << "ProbOfFalseAlarm.size[" << probOfFalseAlarm.size () << "] not equal NumClasses[" << numClasses << "]." << endl;
  }

  if  (!valid)
  {
    log.Level (-1) << endl
      << "AbundanceCorrectionMatrix::ReadXml     ***ERROR***     Had the following errors." << endl
      << errMsgs << endl;
  }
}  /* ReadXml */



void  AbundanceCorrectionMatrix::AddIn (const AbundanceCorrectionMatrix&  m,
                                        RunLog&                           log
                                       )
{
  if  (classes != m.classes)
  {
    KKStr  errMsg = "AbundanceCorrectionMatrix::AddIn  ***ERROR***   Trying to add in a matrix that has a different class makeup.";
    log.Level (-1) << endl << endl
      << errMsg << endl
      << endl;
    throw KKStrException (errMsg);
  }

  for  (uint32 x = 0;  x < numClasses;  ++x)
  {
    knownByClass[x] += m.knownByClass[x];
    predByClass[x]  += m.predByClass[x];
    for  (uint32 y = 0;  y < numClasses;  ++y)
    {
      predictions[x][y] += m.predictions[x][y];
    }
  }

  numCorrect     += m.numCorrect;
  numPredictions += m.numPredictions;
}  /* AddIn */



void   AbundanceCorrectionMatrix::PrintConfusionMatrixTabDelimited (ostream&  outFile)
{
  ComputeStatistics ();
  uint32   x;

  // Lets generate Titles first
  outFile << endl;

  float  overallAccuracy = 0.0;
  if  (numPredictions > 0)  overallAccuracy = 100.0f * (float)numCorrect / (float)numPredictions;

  float   totalAccuracyByClass = 0.0f;
  float*  accuracyByClass = new float[numClasses];
  for  (x = 0;  x < numClasses;  ++x)
  {
    if  (knownByClass[x] > 0)
      accuracyByClass[x] = 100.0f * (float)predictions[x][x] / (float)knownByClass[x];
    else
      accuracyByClass[x] = 0.0f;
    totalAccuracyByClass += accuracyByClass[x];
  }
  float  classWeightedEquallyAccuracy = totalAccuracyByClass / (float)numClasses;

  outFile  << "Column represent Known     class" << endl
           << "Row    represent Predicted class" << endl
           << endl
           << endl
           << "Overall Accuracy:"       << "\t" << StrFormatDouble (overallAccuracy,              "ZZZ0.000") << "%" << endl
           << "Class Weighted Equally:" << "\t" << StrFormatDouble (classWeightedEquallyAccuracy, "ZZZ0.000") << "%" << endl
           << endl;

  KKStr  titleLine1, titleLine2, titleLine3;
  {
    MLClassConstListPtr  allClasses = new MLClassConstList (classes);
    allClasses->PushOnBack ((MLClassPtr)otherClass);
    allClasses->ExtractThreeTitleLines (titleLine1, titleLine2, titleLine3);
    delete  allClasses;
    allClasses = NULL;
  }

  // Report by count.
  titleLine1 = "\t"      + titleLine1 + "\t" + "Prob of";
  titleLine2 = "Class\t" + titleLine2 + "\t" + "False";
  titleLine3 = "Names\t" + titleLine3 + "\t" + "Alarm(SP)";

  outFile << titleLine1 << endl
          << titleLine2 << endl
          << titleLine3 << endl;

  for  (uint32  predClassIdx = 0;  predClassIdx < numClasses;  predClassIdx++)
  {
    if  (predClassIdx < classes.size ())
      outFile << classes[predClassIdx].Name ();
    else
      outFile << otherClass->Name ();

    for  (uint32  knownClassIdx = 0;  knownClassIdx < numClasses;  knownClassIdx++)
      outFile << "\t" << predictions[predClassIdx][knownClassIdx];

    outFile << "\t" << StrFormatDouble (100.0f * probOfFalseAlarm[predClassIdx], "##0.00") << "%"
            << endl;
  }
  outFile << "ProbOfDetection";
  for  (x = 0;  x < numClasses;  ++x)
    outFile << "\t" << StrFormatDouble (100.0f * probOfDetection[x], "##0.00") << "%";
  outFile << endl << endl << endl;


  // Report by percentage.
  outFile << titleLine1 << endl
          << titleLine2 << endl
          << titleLine3 << endl;

  for  (uint32  predClassIdx = 0;  predClassIdx < numClasses;  predClassIdx++)
  {
    if  (predClassIdx < classes.size ())
      outFile << classes[predClassIdx].Name ();
    else
      outFile << otherClass->Name ();

    for  (uint32  knownClassIdx = 0;  knownClassIdx < numClasses;  knownClassIdx++)
    {
      float  acc = 0.0f;
      if  (knownByClass[knownClassIdx] > 0)
        acc = 100.0f * (float)predictions[predClassIdx][knownClassIdx] / knownByClass[knownClassIdx];
      outFile << "\t" << StrFormatDouble (acc, "##0.00") << "%";
    }

    outFile << "\t" << StrFormatDouble (100.0f * probOfFalseAlarm[predClassIdx], "##0.00") << "%"
            << endl;
  }
  outFile << "ProbOfDestection";
  for  (x = 0;  x < numClasses;  ++x)
    outFile << "\t" << StrFormatDouble (100.0f * probOfDetection[x], "##0.00") << "%";
  outFile << endl << endl << endl;

  delete  []accuracyByClass;  accuracyByClass = NULL;
}  /* PrintConfusionMatrixTabDelimited */




ClassStatisticListPtr   AbundanceCorrectionMatrix::LumpCounts (const ClassStatisticList&  inputCounts)
{
  ClassStatisticListPtr  results = new ClassStatisticList (true);
  vector<uint32>  outputCounts (numClasses, 0);
  ClassStatisticList::const_iterator  idx;
  for  (idx = inputCounts.begin ();  idx != inputCounts.end ();  ++idx)
  {
    ClassStatisticPtr  cs = *idx;
    int32  outputCountsIdx = LookUpClassIdx (cs->MLClass ());
    if  (outputCountsIdx < 0)
      outputCountsIdx = otherClassIdx;
    outputCounts[outputCountsIdx] += cs->Count ();
  }

  for  (uint32  outputCountsIdx = 0;  outputCountsIdx < numClasses;  ++outputCountsIdx)
  {
    MLClassConstPtr  ic = NULL;
    if  (outputCountsIdx == otherClassIdx)
      ic = otherClass;
    else
      ic = classes.IdxToPtr (outputCountsIdx);

    results->PushOnBack (new ClassStatistic (ic, outputCounts[outputCountsIdx]));
  }

  return  results;
}  /* LumpCounts */




ClassStatisticListPtr   AbundanceCorrectionMatrix::AdjustClassificationCounts (const ClassStatisticList&  inputCounts,
                                                                               RunLog&                    log
                                                                              )
{
  if  ((probOfDetection.size () < numClasses)  ||  (probOfFalseAlarm.size () < numClasses))
  {
    log.Level (-1) << endl
      << "AdjustClassificationCounts   ***ERROR***    parameters have not been computed;  can not perform adjustment." << endl
      << endl;
    return  NULL;
  }

  ClassStatisticListPtr  results = new ClassStatisticList (true);
  vector<uint32>  outputCounts (numClasses, 0);
  ClassStatisticList::const_iterator  idx;
  for  (idx = inputCounts.begin ();  idx != inputCounts.end ();  ++idx)
  {
    ClassStatisticPtr  cs = *idx;
    int32  outputCountsIdx = LookUpClassIdx (cs->MLClass ());
    if  (outputCountsIdx < 0)
      outputCountsIdx = otherClassIdx;
    outputCounts[outputCountsIdx] += cs->Count ();
  }

  for  (uint32  outputCountsIdx = 0;  outputCountsIdx < numClasses;  ++outputCountsIdx)
  {
    MLClassConstPtr  ic = NULL;
    if  (outputCountsIdx == otherClassIdx)
      ic = otherClass;
    else
      ic = classes.IdxToPtr (outputCountsIdx);

    uint32  adjCount = 0;
    float  pfa = probOfFalseAlarm[outputCountsIdx];
    float  pd  = probOfDetection[outputCountsIdx];
    if  ((pfa == 0.0f)  &&  (pd == 0.0f))
      adjCount = outputCounts[outputCountsIdx];
    else
      adjCount = (uint32)(0.5f + outputCounts[outputCountsIdx] * pfa / pd);
    results->PushOnBack (new ClassStatistic (ic, adjCount));
  }

  return  results;
}  /* AdjustClassificationCounts */