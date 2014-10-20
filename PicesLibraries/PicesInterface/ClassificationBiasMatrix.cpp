#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

#include  "..\BaseLibrary\GoalKeeper.h"
#include  "Matrix.h"
using namespace KKU;

#include "OSservices.h"
#include "PicesKKStr.h"

#include "ClassificationBiasMatrix.h"

using namespace  PicesInterface;



ClassificationBiasMatrix::ClassificationBiasMatrix (String^           _configFileName,
                                                    PicesClassList^   _classes,
                                                    PicesRunLog^      _runLog
                                                   ):
   biasFileName        (nullptr),
   configFileName      (gcnew String (_configFileName)),
   configFileNameFromMatrixBiasFile (nullptr),
   configDateTime      (),
   counts              (NULL),
   classes             (gcnew PicesClassList (_classes)),
   dateTimeFileWritten (),
   probabilities       (NULL),
   numClasses          (0),
   runLog              (_runLog),
   valid               (true)
{
  biasFileName = OSservices::RemoveExtension (configFileName) + ".BiasMatrix.txt";

  StreamReader^  sr = nullptr;
  try  {sr = gcnew StreamReader (biasFileName);}
  catch  (Exception^ e)  
  {
    valid = false;
    String^  errorStr = "Error opening BiasMatrixFile[" + biasFileName + "]";
    runLog->Writeln ("ClassificationBiasMatrix   ***ERROR***" + errorStr);
    throw gcnew Exception (errorStr, e);
  }

  DeclareMatrix ();

  try
  {
    Read (sr);
  }
  catch (Exception^ e2)
  {
    valid = false;
    sr->Close ();
    String^  errorStr = "Exception reasding BiasMatrixFile[" + biasFileName + "]";
    runLog->Writeln ("ClassificationBiasMatrix   ***ERROR***" + errorStr);
    throw gcnew Exception (errorStr, e2);
  }
  sr->Close ();
}



ClassificationBiasMatrix::ClassificationBiasMatrix (PicesRunLog^  _runLog):
   biasFileName        (nullptr),
   configFileName      (),
   configFileNameFromMatrixBiasFile (nullptr),
   configDateTime      (),
   counts              (NULL),
   classes             (gcnew PicesClassList ()),
   dateTimeFileWritten (),
   probabilities       (NULL),
   numClasses          (0),
   runLog              (_runLog),
   valid               (true)
{
  BuildTestMatrix ();

  StreamWriter^  sw = gcnew StreamWriter ("c:\\Temp\\ClassificationBiasMatrix_Test.txt");
  TestPaperResults (sw);
  sw->Close ();
}



ClassificationBiasMatrix::!ClassificationBiasMatrix ()
{
  delete  probabilities;
  probabilities = NULL;
  delete  counts;
  counts = NULL;
}


ClassificationBiasMatrix::~ClassificationBiasMatrix ()
{
  this->!ClassificationBiasMatrix ();
}



void  ClassificationBiasMatrix::DeclareMatrix ()
{
  numClasses = classes->Count;
  probabilities = new Matrix (numClasses, numClasses);
  counts        = new Matrix (numClasses, numClasses);
  for  (int r = 0;  r < numClasses;  r++)
  {
    for  (int c = 0;  c < numClasses;  c++)
    {
      (*probabilities)[r][c] = 0.0;
      (*counts)[r][c] = 0.0;
    }
  }
}  /* DeclareMatrix */




void  ClassificationBiasMatrix::BuildTestMatrix ()
{
  classes = gcnew PicesClassList ();

  classes->Add (PicesClassList::GetUniqueClass ("0", nullptr));
  classes->Add (PicesClassList::GetUniqueClass ("1", nullptr));
  classes->Add (PicesClassList::GetUniqueClass ("2", nullptr));
  classes->Add (PicesClassList::GetUniqueClass ("3", nullptr));
  classes->Add (PicesClassList::GetUniqueClass ("4", nullptr));
  classes->Add (PicesClassList::GetUniqueClass ("5", nullptr));
  classes->Add (PicesClassList::GetUniqueClass ("6", nullptr));

  numClasses = classes->Count;

  probabilities = new Matrix (7, 7);
  counts        = new Matrix (7, 7);

  Row& r0 = (*probabilities)[0];
  Row& r1 = (*probabilities)[1];
  Row& r2 = (*probabilities)[2];
  Row& r3 = (*probabilities)[3];
  Row& r4 = (*probabilities)[4];
  Row& r5 = (*probabilities)[5];
  Row& r6 = (*probabilities)[6];

  r0[0] = 0.710;  r0[1] = 0.059;  r0[2] = 0.010;  r0[3] = 0.010;  r0[4] = 0.007;  r0[5] = 0.031;  r0[6] = 0.175;
  r1[0] = 0.073;  r1[1] = 0.873;  r1[2] = 0.001;  r1[3] = 0.007;  r1[4] = 0.008;  r1[5] = 0.013;  r1[6] = 0.024;
  r2[0] = 0.078;  r2[1] = 0.012;  r2[2] = 0.556;  r2[3] = 0.035;  r2[4] = 0.066;  r2[5] = 0.179;  r2[6] = 0.074;
  r3[0] = 0.030;  r3[1] = 0.028;  r3[2] = 0.054;  r3[3] = 0.560;  r3[4] = 0.019;  r3[5] = 0.177;  r3[6] = 0.132;
  r4[0] = 0.205;  r4[1] = 0.054;  r4[2] = 0.107;  r4[3] = 0.046;  r4[4] = 0.366;  r4[5] = 0.157;  r4[6] = 0.065;
  r5[0] = 0.158;  r5[1] = 0.025;  r5[2] = 0.076;  r5[3] = 0.064;  r5[4] = 0.175;  r5[5] = 0.449;  r5[6] = 0.054;
  r6[0] = 0.289;  r6[1] = 0.096;  r6[2] = 0.033;  r6[3] = 0.065;  r6[4] = 0.018;  r6[5] = 0.072;  r6[6] = 0.427;
}  /* BuildTestMatrix  */







void  ClassificationBiasMatrix::TestPaperResults (StreamWriter^   sw)
{
  array<double>^  classCounts = gcnew array<double> (7);
  classCounts[0] = 2891;
  classCounts[1] = 1965;
  classCounts[2] = 495;
  classCounts[3] = 1399;
  classCounts[4] = 676; 
  classCounts[5] = 1191;
  classCounts[6] = 1752;

  array<double>^  adjCounts = nullptr; 
  array<double>^  stdErrors = nullptr;

  PerformAdjustmnts (classCounts, adjCounts, stdErrors);

  int  x = 0;

  sw->WriteLine ();
  sw->WriteLine ();

  PrintBiasMatrix (sw);

  sw->WriteLine ();
  sw->WriteLine ();

  sw->Write ("Desc");
  for  (x = 0;  x < 7;  x++)
    sw->Write ("\t" + x.ToString ());
  sw->WriteLine ();

  sw->Write ("ClassifiedCounts");
  for  (x = 0;  x < 7;  x++)
    sw->Write ("\t" + classCounts[x].ToString ());
  sw->WriteLine ();

  sw->Write ("adjCounts");
  for  (x = 0;  x < 7;  x++)
    sw->Write ("\t" + adjCounts[x].ToString ());
  sw->WriteLine ();

  sw->Write ("stdErrors");
  for  (x = 0;  x < 7;  x++)
    sw->Write ("\t" + stdErrors[x].ToString ());
  sw->WriteLine ();
  sw->WriteLine ();
  sw->WriteLine ();
}  /* TestPaperResults*/





void  ClassificationBiasMatrix::Read (StreamReader^  sr)
{
  String^ trimDelStr = "\n\r ";
  array<Char>^ trimDel = trimDelStr->ToCharArray();

  String^  l = nullptr;
  while  (l = sr->ReadLine ())
  {
    l = l->Trim (trimDel);
    if  (l->CompareTo ("</BiasMatrix>") == 0)
      break;

    array<String^>^ fields = l->Split ('\t');
    if  (fields->Length == 0)
      continue;

    int  idx = 0;

    String^  fieldValue = "";
    if  (fields->Length > 1)
      fieldValue = fields[1];

    if  (fields[idx]->CompareTo ("FileName") == 0)
    {
    }

    else if  (fields[idx]->CompareTo ("DateTime") == 0)
    {
      dateTimeFileWritten = PicesKKStr::StrToDateTime (fieldValue);
    }

    else if  (fields[idx]->CompareTo ("ConfigFileName") == 0)
    {
      configFileNameFromMatrixBiasFile = fieldValue;
    }

    else if  (fields[idx]->CompareTo ("ConfigFileDateTime") == 0)
    {
      configDateTime = PicesKKStr::StrToDateTime (fieldValue);
    }

    else if  (fields[idx]->CompareTo ("<SimpleConfusionMatrix>") == 0)
    {
      ReadSimpleConfusionMatrix (sr);
    }
  }
}  /* read */




void  ClassificationBiasMatrix::ReadSimpleConfusionMatrix (StreamReader^  sr)
{
  String^ trimDelStr = "\n\r ";
  array<Char>^ trimDel = trimDelStr->ToCharArray();

  PicesClassList^  fileClasses = nullptr;

  int  classesRowIdx = 0;
  int  classesColIdx = 0;

  String^  l = nullptr;
  while  (l = sr->ReadLine ())
  {
    l = l->Trim (trimDel);
    if  (l->CompareTo ("</SimpleConfusionMatrix>") == 0)
      break;

    array<String^>^ fields = l->Split ('\t');
    if  (fields->Length == 0)
      continue;

    if  (fields[0]->CompareTo ("Classes") == 0)
    {
      fileClasses = PicesClassList::CreateFromCommaDelStr (fields[1]);
      if  (fileClasses->Count != numClasses)
      {
        valid = false;
        String^  errStr = "ReadSimpleConfusionMatrix  ***ERROR*** 'Classes' data line incorrect number of classes;  Found[" + fileClasses->Count + "]  Expected[" + numClasses + "]";
        runLog->Writeln (errStr);
        throw gcnew Exception (errStr);
      }

      for  (int x = 0;  x < numClasses;  x++)
      {
        PicesClass^  y = fileClasses[x];
        if  (classes->LookUpIndex (y) < 0)
        {
          valid = false;
          String^  errStr = "ReadSimpleConfusionMatrix  ***ERROR***   unexpected Class[" + y->Name + "] indicated on 'Classes' line.";
          runLog->Writeln (errStr);
          throw gcnew Exception (errStr);
        }
      }
    }


    if  (fields[0]->CompareTo ("DataRow") == 0)
    {
      if  (fileClasses == nullptr)
      {
        valid = false;
        String^  errMsg = "ReadSimpleConfusionMatrix   ***ERROR***  'Classes'  was not provided before 'DataRow'.";
        runLog->Writeln (errMsg);
        throw gcnew Exception (errMsg);
      }

      if  (fields->Length < 3)
        continue;

      String^  className = fields[1];
      String^  data = fields[2];

      PicesClass^ pc = PicesClassList::GetUniqueClass (className, nullptr);
      int  classesIdx     = classes->LookUpIndex (pc);
      int  fileClassesIdx = fileClasses->LookUpIndex (pc);

      if  (classesIdx < 0)
      {
        valid = false;
        String^  errMsg = "ReadSimpleConfusionMatrix   ***ERROR***  DataRow specifies class[" + className + "] which is not defined by caller";
        runLog->Writeln (errMsg);
        throw gcnew Exception (errMsg);
      }

      if  (fileClassesIdx < 0)
      {
        valid = false;
        String^  errMsg = "ReadSimpleConfusionMatrix   ***ERROR***  DataRow specifies class[" + className + "] was not defined in 'Classes' line.";
        runLog->Writeln (errMsg);
        throw gcnew Exception (errMsg);
      }

      int  classesRowIdx = classesIdx;

      array<String^>^  dataFields = data->Split (',');
      if  (dataFields->Length != numClasses)
      {
        valid = false;
        String^  errMsg = "ReadSimpleConfusionMatrix   ***ERROR***  DataRow Class[" + className + "]  number[" + dataFields->Length + "] of values provided does not match number of Classes.";
        runLog->Writeln (errMsg);
        throw gcnew Exception (errMsg);
      }

      for  (int c = 0;  c < numClasses;  c++)
      {
        pc = fileClasses[c];
        classesColIdx = classes->LookUpIndex (pc);

        array<String^>^  parts = dataFields[c]->Split (':');
        if  (parts->Length > 1)
        {
          (*counts)       [classesRowIdx][classesColIdx] = PicesKKStr::StrToDouble (parts[0]);
          (*probabilities)[classesRowIdx][classesColIdx] = PicesKKStr::StrToDouble (parts[1]);
        }
      }
    }
  }

  if  (fileClasses == nullptr)
  {
    valid = false;
    String^  errMsg = "ReadSimpleConfusionMatrix   ***ERROR***  A  'Classes'  line was never provided.";
    runLog->Writeln (errMsg);
    throw gcnew Exception (errMsg);
  }
}  /* ReadSimpleConfusionMatrix */





void   ClassificationBiasMatrix::PerformAdjustmnts (array<double>^  classifiedCounts,
                                                    array<double>^ &adjCounts,
                                                    array<double>^ &stdErrors
                                                   )
{
  // For description of calc's reed the paper: 
  //    "Estimating the Taxonomic composition of a sample when individules are classified with error"
  //     by Andrew Solow, Cabll Davis, Qiao Hu
  //     Woods Hole Ocanographic Institution, Woods Hole Massachusetts
  //     Marine Ecology Progresss Series
  //     published 2006-july-06;  vol 216:309-311

  if  (classifiedCounts == nullptr)
    throw gcnew Exception ("ClassificationBiasMatrix::PerformAdjustmnts    ***ERROR***   classifiedCounts==nullptr");

  if  (classifiedCounts->Length != numClasses)
  {
    String^  errMsg = "ClassificationBiasMatrix::PerformAdjustmnts  ***ERROR***   Disagreement in length of classifiedCounts[" + classifiedCounts->Length + "]  and Prev Defined ClassList[" + numClasses + "].";
    runLog->Writeln (errMsg);
    throw gcnew Exception (errMsg);
  }

  int x = 0;
  int  i, j, k;

  // We need to deal with the special case when one entry in the probability diagonal is zero.
  {
    for (x = 0;  x < numClasses;  x++)
    {
      if  ((*probabilities)[x][x] == 0.0)
      {
        // This will cause the inversion of the diagonal marix to fail.  To deal
        // with this situation; I will steal some probability from other buckets on 
        // same row.

        double  totalAmtStolen = 0.0;
        double  percentToSteal = 0.01;
        for (i = 0;  i < numClasses;  i++)
        {
          if  ((*probabilities)[x][i] != 0.0)
          {
            double amtToSteal = (*probabilities)[x][i] * percentToSteal;
            (*probabilities)[x][i] = (*probabilities)[x][i] - amtToSteal;
            totalAmtStolen += amtToSteal;
          }
        }

        (*probabilities)[x][x] = totalAmtStolen;
      }
    }
  }

  Matrix  m (numClasses, 1);
  for  (x = 0;  x < numClasses;  x++)
    m[x][0] = classifiedCounts[x];

  Matrix  transposed = probabilities->Transpose ();
  Matrix  Q = transposed.Inverse ();


  if  (false)
  {
    ofstream  db ("c:\\Temp\\InverseMatric.txt");

    int r,c;
    db << "Probabilities Matrix" << endl;
    for  (c = 0;  c < probabilities->NumOfCols ();  c++)
       db << "\t" << c;
    db << endl;
    for  (r = 0; r < probabilities->NumOfRows ();  r++)
    {
      db << r;
      for  (c = 0;  c < probabilities->NumOfCols ();  c++)
        db << "\t" << (*probabilities)[r][c];
      db << endl;
    }


    db << endl << endl << "Transposed Matrix" << endl;
    for  (c = 0;  c < probabilities->NumOfCols ();  c++)
       db << "\t" << c;
    db << endl;
    for  (r = 0; r < transposed.NumOfRows ();  r++)
    {
      db << r;
      for  (c = 0;  c < transposed.NumOfCols ();  c++)
        db << "\t" << transposed[r][c];
      db << endl;
    }


    db << endl << endl << "Inveset Matrix" << endl;
    for  (c = 0;  c < probabilities->NumOfCols ();  c++)
       db << "\t" << c;
    db << endl;
    for  (r = 0; r < Q.NumOfRows ();  r++)
    {
      db << r;
      for  (c = 0;  c < Q.NumOfCols ();  c++)
        db << "\t" << Q[r][c];
      db << endl;
    }
  }

  Matrix  n = Q * m;

  Matrix  varM (numClasses, numClasses);
  for  (j = 0;  j < numClasses;  j++)
  {
    double  varM_j = 0.0;
    for  (i = 0;  i < numClasses;  i++)
    {
      double  p = (*probabilities)[i][j];
      varM_j += n[i][0] * p * (1.0 - p);
    }
    varM[j][j] = varM_j;
  }

  for (j = 0;  j < numClasses;  j++)
  {
    for  (k = 0;  k < numClasses;  k++)
    {
      if  (j != k)
      {
        double  covM_jk = 0.0;
        for  (i = 0;  i < numClasses;  i++)
          covM_jk -= n[i][0] * (*probabilities)[i][j] * (*probabilities)[j][k];
        varM[j][k] = covM_jk;
      }
    }
  }

  Matrix  varN = Q * varM * Q.Transpose ();

  adjCounts = gcnew array<double> (numClasses);
  stdErrors = gcnew array<double> (numClasses);
  for  (x = 0;  x < numClasses;  x++)
  {
    adjCounts[x] = n[x][0];
    stdErrors[x] = sqrt (varN[x][x]);
  }

  return;
}  /* PerformAdjustmnts */






void  ClassificationBiasMatrix::PrintBiasMatrix (StreamWriter^  sw)
{
  if  (classes == nullptr)
  {
    String^  errMsg = "ClassificationBiasMatrix::PrintBiasMatrix  ***ERROR***   'Classes' not defined;  this indicates that this object was not properly initialized.";
    runLog->Writeln (errMsg);
    throw gcnew Exception (errMsg);
  }

  sw->WriteLine ("BiasMatrix File Name            [" + biasFileName + "]");
  sw->WriteLine ("Date Bias Matrix was Created    [" + dateTimeFileWritten.ToString ("yyyy-MMM-dd HH:mm:ss") + "]");
  sw->WriteLine ("Configuration File Name         [" + configFileName + "]");
  sw->WriteLine ("ConfigFile Used for Bias Matrix [" + configFileNameFromMatrixBiasFile + "]");
  sw->WriteLine ();
  sw->WriteLine ();

  List<String^>^  classTitles = classes->ExtractThreeTitleLines ();
  if  ((classTitles != nullptr)  &&  (classTitles->Count >= 3))
  {
    sw->WriteLine (""      + "\t" + ""      + "\t" + classTitles[0]);
    sw->WriteLine ("Class" + "\t" + ""      + "\t" + classTitles[1]);
    sw->WriteLine ("Name"  + "\t" + "Count" + "\t" + classTitles[2]);
  }

  int  row = 0;
  int  col = 0;
  
  double  total = 0.0;

  VectorDouble colTotals (numClasses, 0.0);
  VectorDouble rowTotals (numClasses, 0.0);
  

  for  (row = 0; row < numClasses;  row++)
  {
    double  rowTotal = 0;
    for  (col = 0;  col < numClasses;  col++)
    {
      rowTotal += (*counts)[row][col];
      colTotals[col] += (*counts)[row][col];
    }
    rowTotals[row] = rowTotal;

    sw->Write (classes[row]->Name + "\t" + rowTotal.ToString ("###,##0.00"));
    for  (col = 0;  col < numClasses;  col++)
      sw->Write ("\t" + (*counts)[row][col].ToString ("###,##0.00"));
    sw->WriteLine ();
    total += rowTotal;
  }
  sw->Write ("Total" + "\t" + total.ToString ("###,##0.00"));
  for  (col = 0;  col < numClasses;  col++)
     sw->Write ("\t" + colTotals[col].ToString ("###,##0.00"));
  sw->WriteLine ();
 

  sw->WriteLine ();
 
  for  (row = 0; row < numClasses;  row++)
  {
    sw->Write (classes[row]->Name);
    double  rowPercent = 100.0 * rowTotals[row] / total;
    sw->Write ("\t" + rowPercent.ToString ("##0.0000") + "%");

    for  (col = 0;  col < numClasses;  col++)
      sw->Write ("\t" + (100.0 * (*probabilities)[row][col]).ToString ("##0.0000") + "%");
    sw->WriteLine ();
  }
  sw->WriteLine ();

  sw->Flush ();
}  /* PrintBiasMatrix */




void  ClassificationBiasMatrix::PrintAdjustedResults (StreamWriter^   sw,
                                                      array<double>^  classifiedCounts
                                                     )
{
  if  (classifiedCounts->Length != numClasses)
  {
    String^  errMsg = "ClassificationBiasMatrix::PrintAdjustedResults  ***ERROR***  Number of entries in 'classifiedCounts' not equal the number of classes";
    runLog->Writeln (errMsg);
    throw gcnew Exception (errMsg);
  }

  try
  {
    array<double>^  adjustedReults = nullptr;
    array<double>^  stdErrors      = nullptr;
    
    PerformAdjustmnts (classifiedCounts, adjustedReults, stdErrors);

    List<String^>^  classTitles = classes->ExtractThreeTitleLines ();
    if  ((classTitles != nullptr)  &&  (classTitles->Count >= 3))
    {
      sw->WriteLine (""             + "\t" + "\t" + classTitles[0]);
      sw->WriteLine (""             + "\t" + "\t" + classTitles[1]);
      sw->WriteLine ("Description"  + "\t" + "\t" + classTitles[2]);
    }

    int  col = 0;

    sw->Write ("Classified Results" + "\t");
    for  (col = 0;  col < numClasses;  col++)
      sw->Write ("\t" + classifiedCounts[col].ToString ("###,##0.0"));
    sw->WriteLine ();

    sw->Write ("Adjusted Results"   + "\t");
    for  (col = 0;  col < numClasses;  col++)
      sw->Write ("\t" + adjustedReults[col].ToString ("###,##0.0"));
    sw->WriteLine ();

    sw->Write ("Standard Errors"    + "\t");
    for  (col = 0;  col < numClasses;  col++)
      sw->Write ("\t" + stdErrors[col].ToString ("###,##0.0"));
    sw->WriteLine ();
    sw->Flush ();
  }
  catch  (Exception^ e)
  {
    sw->Flush ();
    throw gcnew Exception ("ClassificationBiasMatrix::PrintAdjustedResults  ***ERROR***  Error claculating Adjusted Results.", e);
  }
}  /* PrintAdjustedResults */
