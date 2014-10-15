#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>
#include  <memory>
#include  <math.h>
#include  <limits.h>
#include  <float.h>


#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;


#include  "OSservices.h"

#include  "ClassGroupHistory.h"


#include  "ClassGroupTotals.h"





HistoryBit::HistoryBit ()
{}




inline
int  CompareDouble (FFLOAT&  x,  FFLOAT& y)
{
  if  (x < y)  
    return  -1;

  if  (x > y)
    return 1;
  
  return 0;
}


inline
int  CompareInt (int&  x,  int& y)
{
  if  (x < y)  
    return  -1;

  if  (x > y)
    return 1;
  
  return 0;
}


inline
int  CompareUint (uint&  x,  uint& y)
{
  if  (x < y)  
    return  -1;

  if  (x > y)
    return 1;
  
  return 0;
}




PredictionHistoryBit::PredictionHistoryBit (MLClassList&  mlClasses, 
                                            KKStr           l
                                           )
{
  KKStr  knownClassName = l.ExtractToken ();
  knownClass = mlClasses.LookUpByName (knownClassName);

  KKStr  predClassName = l.ExtractToken ();
  predClass = mlClasses.LookUpByName (predClassName);

  probability = (FFLOAT)l.ExtractTokenDouble (" ,\n\t\r");
}



int  PredictionHistoryBit::Compare (PredictionHistoryBit*  r)
{
  int  x = strcmp (knownClass->Name ().Str (), r->knownClass->Name ().Str ());
  if  (x != 0)
    return x;

   
  x = strcmp (predClass->Name ().Str (), r->predClass->Name ().Str ());
  if  (x != 0)
    return x;


  return  CompareDouble (probability, r->probability);
}



KKStr  PredictionHistoryBit::ToString ()
{
  KKStr  r (knownClass->Name ());
  r << "\t" << predClass->Name ()
    << "\t" << StrFormatDouble (probability, "-ZZ0.00000");
  return  r;
}






ProbabilityHistoryBit::ProbabilityHistoryBit (MLClassList&  mlClasses, 
                                              KKStr           l
                                             )
{
  position    = 0;
  percentile = 0.0f;

  KKStr  className = l.ExtractToken ();
  mlClass    = mlClasses.LookUpByName (className);
  probability   = (FFLOAT)l.ExtractTokenDouble (" ,\n\t\r");
  imageFileName = l.ExtractToken ("\t\n\r");
  if  (!l.Empty ())
  {
    position   = l.ExtractTokenInt ("\n\r\t");
    percentile = (float)l.ExtractTokenDouble ("\n\r\t");
  }
}



int  ProbabilityHistoryBit::Compare (ProbabilityHistoryBit*  r)
{
  int  x = strcmp (mlClass->Name ().Str (), r->mlClass->Name ().Str ());
  if  (x != 0)
    return x;
      
  return  CompareDouble (probability, r->probability);
}  /* Compare */




KKStr  ProbabilityHistoryBit::ToString ()
{
  KKStr  r (mlClass->Name ());
  r << "\t" << StrFormatDouble (probability, "-ZZ0.00000")
    << "\t" << imageFileName
    << "\t" << position
    << "\t" << percentile;

  return  r;
}




SortMethodHistoryBit::SortMethodHistoryBit (MLClassPtr  _mlClass,
                                            SortOrderType  _sortMethod
                                           )
{
  mlClass = _mlClass;
  sortMethod = _sortMethod;
}
      


SortMethodHistoryBit::SortMethodHistoryBit (MLClassList&  mlClasses, 
                                            KKStr           l)
{
  KKStr  className = l.ExtractToken ();
  mlClass = mlClasses.LookUpByName (className);

  KKStr  sortMethodStr = l.ExtractToken ();
  sortMethod = SortOrderTypeFromStr (sortMethodStr);
}
      


int  SortMethodHistoryBit::Compare (SortMethodHistoryBit* r)
{
  int  x = strcmp (mlClass->Name ().Str (), r->mlClass->Name ().Str ());
  if  (x != 0)
   return x;

   if  (sortMethod < r->sortMethod)
     return -1;

   if  (sortMethod > r->sortMethod)
     return  1;

   return 0;
}



KKStr  SortMethodHistoryBit::ToString ()
{
  KKStr  r (mlClass->Name ());
  r << "\t" << SortOrderDescr (sortMethod);
  return  r;
}
      




SupportVectorsHistoryBit::SupportVectorsHistoryBit (KKStr l)
{
  group             = l.ExtractTokenInt    (" ,\n\t\r");
  pass              = l.ExtractTokenInt    (" ,\n\t\r");
  numSupportVectors = l.ExtractTokenInt    (" ,\n\t\r");
  trainingTime      = (float)l.ExtractTokenDouble (" ,\n\t\r");
  testingTime       = (float)l.ExtractTokenDouble (" ,\n\t\r");
}      


int  SupportVectorsHistoryBit::Compare (SupportVectorsHistoryBit*  r)
{
  int x = CompareInt (group, r->group);
  if  (x != 0)
    return x;

  x = CompareInt (pass, r->pass);
  if  (x != 0)
    return x;

  return  CompareUint (numSupportVectors, r->numSupportVectors);
}


KKStr  SupportVectorsHistoryBit::ToString ()
{
  KKStr  r (30);
  r << group             << "\t" 
    << pass              << "\t"
    << numSupportVectors << "\t"
    << trainingTime      << "\t"
    << testingTime;
  return  r;
}
      





void  PredictionHistoryBit::UpdateClassTotals (ClassGroupTotalsPtr  totals,
                                               int                  group,
                                               int                  pass
                                              )
{
  totals->Prediction (group, pass, knownClass, predClass, probability);
}




void  ProbabilityHistoryBit::UpdateClassTotals (ClassGroupTotalsPtr  totals,
                                                int                  group,
                                                int                  pass
                                               )
{
  totals->AddAProbability (group, pass, mlClass, probability, imageFileName, position, percentile);
}





void  SortMethodHistoryBit::UpdateClassTotals (ClassGroupTotalsPtr  totals,
                                               int                  group,
                                               int                  pass)
{
  totals->SetSortMethodUsed (group, mlClass, pass, sortMethod);
}




void  SupportVectorsHistoryBit::UpdateClassTotals (ClassGroupTotalsPtr  totals,
                                                   int                  _group,
                                                   int                  _pass
                                                  )
{
  totals->NumOfSupportVectors (group, pass, numSupportVectors, trainingTime, testingTime);
}





class  HistoryBitSort:  public  QueueComparison<HistoryBit>
{
public: 
  bool  operator ()  (HistoryBitPtr  p1,
                      HistoryBitPtr  p2
                     )
        {      

          if  (p1->TypeHistory () > p2->TypeHistory ())
            return  false;

          else if  (p1->TypeHistory () < p2->TypeHistory ())
            return true;


          int  x;

          switch  (p1->TypeHistory ())
          {
            case  PredictionHist:     x = ((PredictionHistoryBitPtr)p1)     -> Compare ((PredictionHistoryBitPtr)p2);
                  break;

            case  ProbabilityHist:    x = ((ProbabilityHistoryBitPtr)p1)    -> Compare ((ProbabilityHistoryBitPtr)p2);
                  break;

            case  SortMethodHist:     x = ((SortMethodHistoryBitPtr)p1)     -> Compare ((SortMethodHistoryBitPtr)p2);
                  break;

            case  SupportVectorsHist: x = ((SupportVectorsHistoryBitPtr)p1) -> Compare ((SupportVectorsHistoryBitPtr)p2);
                  break;
          }

          return  (x < 0);
        }
};





void  HistoryBitList::WriteOutContents (ostream&  o)
{
  for  (int i = 0;  i < QueueSize ();  i++)
  {
    HistoryBitPtr  h = IdxToPtr (i);

    o << h->ToString () << endl;
  }
}


void  HistoryBitList::SortHistory ()
{
  HistoryBitSort  s;
  sort (begin (), end (), s);
}


