#include "StdAfx.h"
#include "FirstIncludes.h"

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <strstream>
#include <vector>
using namespace std;

#include "MemoryDebug.h"
#include "BasicTypes.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

#include "..\BaseLibrary\GoalKeeper.h"
#include "Matrix.h"
using namespace KKU;

#include "AbundanceCorrectionMatrix.h"
#include "ClassStatistic.h"
#include "MLClass.h"
using namespace MLL;

#include "OSservices.h"
#include "PicesKKStr.h"
#include "PicesClass.h"
#include "PicesClassList.h"
#include "PicesMethods.h"

#include "PicesAbundanceCorrectionMatrix.h"

using namespace  PicesInterface;



PicesAbundanceCorrectionMatrix::PicesAbundanceCorrectionMatrix (PicesClassList^ _classes,
                                                                PicesClass^     _otherClass,
                                                                PicesRunLog^    _runLog
                                                               ):
   abundanceMatrix (NULL)
{
  MLClassConstListPtr  unManagedClasses = _classes->ToMLClassConstList ();
  abundanceMatrix = new AbundanceCorrectionMatrix (*unManagedClasses, _otherClass->UnmanagedMLClass (), _runLog->Log ());
  delete  unManagedClasses;
  unManagedClasses= NULL;
}



PicesAbundanceCorrectionMatrix::PicesAbundanceCorrectionMatrix (String^       _configFileName,
                                                                PicesRunLog^  _runLog
                                                               ):
   abundanceMatrix (NULL)
{
  abundanceMatrix = new AbundanceCorrectionMatrix (PicesKKStr::SystemStringToKKStr (_configFileName), _runLog->Log ());
}



PicesAbundanceCorrectionMatrix::!PicesAbundanceCorrectionMatrix ()
{
  delete  abundanceMatrix;
  abundanceMatrix = NULL;
}


PicesAbundanceCorrectionMatrix::~PicesAbundanceCorrectionMatrix ()
{
  this->!PicesAbundanceCorrectionMatrix ();
}


System::DateTime  PicesAbundanceCorrectionMatrix::DateTimeComputed ()
{
  return  PicesMethods::DateTimeKKUtoSystem (abundanceMatrix->DateTimeComputed ());
}


array<float>^  PicesAbundanceCorrectionMatrix::ProbOfDetection ()
{
  const vector<float>&  v = abundanceMatrix->ProbOfDetection ();
  array<float>^  result = gcnew array<float> (v.size ());
  for  (uint32 x = 0;  x < v.size ();  ++x)
    result[x] = v[x];

  return  result;
}



array<float>^     PicesAbundanceCorrectionMatrix::ProbOfFalseAlarm ()
{
  const vector<float>&  v = abundanceMatrix->ProbOfFalseAlarm ();
  array<float>^  result = gcnew array<float> (v.size ());
  for  (uint32 x = 0;  x < v.size ();  ++x)
    result[x] = v[x];

  return  result;
}

bool  PicesAbundanceCorrectionMatrix::Valid ()
{
  if  (!abundanceMatrix)
    return false;
  else
    return abundanceMatrix->Valid ();
}



void  PicesAbundanceCorrectionMatrix::AddIn (PicesAbundanceCorrectionMatrix^  matrix,
                                             PicesRunLog^                     runLog
                                            )
{
  abundanceMatrix->AddIn (*(matrix->abundanceMatrix), runLog->Log ());
}


void  PicesAbundanceCorrectionMatrix::ComputeStatistics ()
{
  abundanceMatrix->ComputeStatistics ();
}


void  PicesAbundanceCorrectionMatrix::Prediction (PicesClass^   knownClass,
                                                  PicesClass^   predClass,
                                                  PicesRunLog^  log
                                                 )
{
  abundanceMatrix->Prediction (knownClass->UnmanagedMLClass (), predClass->UnmanagedMLClass (), log->Log ());
}


void  PicesAbundanceCorrectionMatrix::PrintConfusionMatrixTabDelimited (StreamWriter^  outFile)
{
  ostrstream o;
  abundanceMatrix->PrintConfusionMatrixTabDelimited (o);
  char*  orig = o.str ();
  String^ systemString = gcnew String (orig);
  outFile->Write (systemString);
  delete systemString;
  systemString = nullptr;
}  /* PrintConfusionMatrixTabDelimited */



void  PicesAbundanceCorrectionMatrix::SaveForConfigFileName (String^  configFileName)
{
  abundanceMatrix->SaveForConfigFileName (PicesKKStr::SystemStringToKKStr (configFileName));
}



PicesClassStatisticList^  PicesAbundanceCorrectionMatrix::LumpCounts (PicesClassStatisticList^  inputCounts)
{
  ClassStatisticListPtr  unmanagedInputCounts = inputCounts->ToUnmanagedClassStatisticList ();
  ClassStatisticListPtr  lumpedCounts = abundanceMatrix->LumpCounts (*unmanagedInputCounts);

  PicesClassStatisticList^  results = gcnew PicesClassStatisticList (*lumpedCounts);

  delete  lumpedCounts;          lumpedCounts         = NULL;
  delete  unmanagedInputCounts;  unmanagedInputCounts = NULL;
  return  results;
}


PicesClassStatisticList^  PicesAbundanceCorrectionMatrix::AdjustClassificationCounts 
                                          (PicesClassStatisticList^  inputCounts,
                                           PicesRunLog^              log
                                          )
{
  ClassStatisticListPtr  unmanagedInputCounts = inputCounts->ToUnmanagedClassStatisticList ();
  ClassStatisticListPtr  adjustedCounts = abundanceMatrix->AdjustClassificationCounts (*unmanagedInputCounts, log->Log ());

  PicesClassStatisticList^  results = gcnew PicesClassStatisticList (*adjustedCounts);

  delete  adjustedCounts;       adjustedCounts       = NULL;
  delete  unmanagedInputCounts; unmanagedInputCounts = NULL;

  return  results;
}  /* AdjustClassificationCounts */



void  PicesAbundanceCorrectionMatrix::PrintAdjustedResults (StreamWriter^             sw,
                                                            PicesClassStatisticList^  classifiedCounts,
                                                            PicesRunLog^              runLog
                                                           )
{
  PicesClassStatisticList^  lumpedCounts = nullptr;
  PicesClassStatisticList^  adjCounts    = nullptr;

  System::DateTime  dateTimeParametersComputed = PicesMethods::DateTimeKKUtoSystem (abundanceMatrix->DateTimeComputed ());

  sw->WriteLine ();
  sw->WriteLine ();
  sw->WriteLine ("Abundance Adjustment:   Parameters Computed on " + dateTimeParametersComputed.ToLongTimeString ());
  sw->WriteLine ();


  try
  {
    lumpedCounts = LumpCounts (classifiedCounts);
    adjCounts = AdjustClassificationCounts (lumpedCounts, runLog);
  }
  catch  (Exception^  eeee)
  {
    sw->WriteLine ();
    sw->WriteLine ("     ***ERROR***          ***ERROR***          ***ERROR***");
    sw->WriteLine ();
    sw->WriteLine ("Exception occured attempting to compute abundance adjusted results.");
    sw->WriteLine ();
    sw->WriteLine (eeee->ToString ());
    sw->WriteLine ();
    return;
  }

  if  (adjCounts == nullptr)
  {
    sw->WriteLine ();
    sw->WriteLine ("        ***ERROR***          ***ERROR***          ***ERROR***");
    sw->WriteLine ();
    sw->WriteLine ("'AdjustClassificationCounts'  failed to compute abundance adjusted results.");
    sw->WriteLine ();
    return;
  }

  PicesClassList^  classOrder = adjCounts->ToPicesClassList ();
  int  size = adjCounts->Count;

  List<String^>^  classTitles = classOrder->ExtractThreeTitleLines ();
  if  (classTitles != nullptr)
  {
    for  (int z = 0;  z < classTitles->Count;  ++z)
    {
      if  (z >= (classTitles->Count - 1))
        sw->Write ("Description");

      sw->WriteLine ("\t" + "\t" + classTitles[z]);
    }
  }

  int  col = 0;

  array<float>^  probOfFalseAlarm = ProbOfFalseAlarm ();
  array<float>^  probOfDetection  = ProbOfDetection ();

  if  (probOfFalseAlarm->Length == size)
  {
    sw->Write ("ProbOfFalseAlarm" + "\t");
    for  (col = 0;  col < size;  col++)
      sw->Write ("\t" + (100.0f * probOfFalseAlarm[col]).ToString ("##0.000") + "%");
    sw->WriteLine ();
  }

  if  (probOfDetection->Length == size)
  {
    sw->Write ("ProbOfDetection" + "\t");
    for  (col = 0;  col < size;  col++)
      sw->Write ("\t" + (100.0f * probOfDetection[col]).ToString ("##0.000") + "%");
    sw->WriteLine ();
  }

  sw->Write ("Classified Results" + "\t");
  for  (col = 0;  col < size;  col++)
    sw->Write ("\t" + lumpedCounts[col]->Count.ToString ("###,##0"));
  sw->WriteLine ();

  sw->Write ("Adjusted Results"   + "\t");
  for  (col = 0;  col < size;  col++)
    sw->Write ("\t" + adjCounts[col]->Count.ToString ("###,##0"));
  sw->WriteLine ();

  sw->Flush ();
}  /* PrintAdjustedResults */

