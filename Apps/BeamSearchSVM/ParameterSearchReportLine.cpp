#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <iostream>
#include  <vector>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;



#include  "ImageClass.h"
#include  "Str.h"
#include  "OSservices.h"


#include  "ParameterSearchReportLine.h"


KKStr  ParameterSearchReportLine::emptyName = "";


ParameterSearchReportLine::ParameterSearchReportLine (KKStr             text,
                                                      ImageClassListPtr  _classes,
                                                      RunLog&            _log
                                                      ):
  classAccuracies (),
  classes ()

{
  class1 = NULL;
  class2 = NULL;

  KKStr  class1Name;
  KKStr  class2Name;

  jobId = text.ExtractTokenInt ("\n\t\r ");
  class1Name = text.ExtractToken ("\n\t\r");
  KKStr  class1NameUpper = class1Name;
  class1NameUpper.Upper ();
  if  (class1NameUpper != "ALLCLASSES")
    class2Name = text.ExtractToken ("\n\t\r");

  cParm        = text.ExtractTokenInt ("\n\t\r ");
  gamma        = (float)text.ExtractTokenDouble ("\n\t\r ");
  aParm        = text.ExtractTokenInt ("\n\t\r ");
  accuracy     = (float)text.ExtractTokenDouble ("\n\t\r ");
  numSVs       = (float)text.ExtractTokenDouble ("\n\t\r ");
  avgPredProb  = (float)text.ExtractTokenDouble ("\n\t\r ");
  avgKnownProb = (float)text.ExtractTokenDouble ("\n\t\r ");
  compact      = (float)text.ExtractTokenDouble ("\n\t\r ");
  compactDev   = (float)text.ExtractTokenDouble ("\n\t\r ");
  time         = (float)text.ExtractTokenDouble ("\n\t\r ");
  trainTime    = (float)text.ExtractTokenDouble ("\n\t\r ");
  classTime    = (float)text.ExtractTokenDouble ("\n\t\r ");

  if  ((!class1Name.Empty ())  &&  (!class2Name.Empty ()))
  {
    class1 = _classes->GetImageClassPtr (class1Name);
    class2 = _classes->GetImageClassPtr (class2Name);
  }

  while  (!text.Empty ())
  {
    KKStr  className = text.ExtractToken ("\n\t\r ");
    ImageClassPtr  imageClass = _classes->LookUpByName (className);
    if  (imageClass)
    {
      classes.PushOnBack (imageClass);
      classAccuracies.push_back ((float)text.ExtractTokenDouble ("\n\t\r "));
    }
  }
}

 

ParameterSearchReportLineList::ParameterSearchReportLineList (bool  _owner):
  KKQueue<ParameterSearchReportLine> (_owner, 1000)
{
}




ParameterSearchReportLineList::ParameterSearchReportLineList (KKStr             fileName,
                                                              ImageClassListPtr  _classes,
                                                              RunLog&            _log
                                                             ):
  KKQueue<ParameterSearchReportLine> (true, 1000)
{
   FILE*  inputFile = osFOPEN (fileName.Str (), "r");

   if  (!inputFile)
   {
     cerr << endl
          << endl
          << "ParameterSearchReportLineList     *** ERROR ***,   Could not open file[" << fileName << "]." << endl
          << endl;
     osWaitForEnter ();
     exit (-1);
   }
 
   char  buff[40960];
   fgets (buff, sizeof (buff), inputFile);  // Skip first line which is a header line

   while  (fgets (buff, sizeof (buff), inputFile))
   {
     KKStr  t (buff);
     PushOnBack (new ParameterSearchReportLine (t, _classes, _log));
   }

   fclose (inputFile);
}





ParameterSearchReportLineListPtr  ParameterSearchReportLineList::ExtractTwoClasses (ImageClassPtr  class1,
                                                                                    ImageClassPtr  class2
                                                                                   )
{
  ParameterSearchReportLineListPtr  results = new ParameterSearchReportLineList (false);

  ParameterSearchReportLineList::iterator  idx;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    ParameterSearchReportLinePtr  rl = *idx;

    if  ((rl->Class1 () == class1)  &&  (rl->Class2 () == class2))
      results->PushOnBack (rl);
  }

  return  results;
}  /* ExtractTwoClasses */






class  SortByGammaCostComparator: public  QueueComparison<ParameterSearchReportLine>
{
  public:
   SortByGammaCostComparator ():  QueueComparison<ParameterSearchReportLine> () {} 


   bool  operator () (ParameterSearchReportLine*  p1,
                      ParameterSearchReportLine*  p2
                     )
   {
     if  (p1->Gamma () > p2->Gamma ())
       return false;

     else if  (p1->Gamma () < p2->Gamma ())
       return  true;

     else if  (p1->CParm () > p2->CParm ())
       return  false;

     else if  (p1->CParm () < p2->CParm ())
       return   true;

     else if  (p1->Accuracy () > p2->Accuracy ())
       return  false;

     else if  (p1->Accuracy () < p2->Accuracy ())
       return   true;

     return  false;
   }
};  /* SortByGammaCostComparator */



void  ParameterSearchReportLineList::SortByGammaCost ()
{
  SortByGammaCostComparator*  c = new SortByGammaCostComparator ();
  sort (begin (), end (), *c);
  delete  c;
}  /* SortByGammaCost */






class  SortByAccuracyComparator: public  QueueComparison<ParameterSearchReportLine>
{
  public:
   SortByAccuracyComparator ():  QueueComparison<ParameterSearchReportLine> () {} 


   bool  operator () (ParameterSearchReportLine*  p1,
                      ParameterSearchReportLine*  p2
                     )
   {
     if  (p1->Class1Name () < p2->Class1Name ())
       return  false;

     else if  (p1->Class1Name () > p2->Class1Name ())
       return  true;

     else if  (p1->Class2Name () < p2->Class2Name ())
       return  false;

     else if  (p1->Class2Name () > p2->Class2Name ())
       return  true;

     else if  (p1->Accuracy () > p2->Accuracy ())
       return  true;

     else if  (p1->Accuracy () < p2->Accuracy ())
       return   false;

     else if  (p1->NumSVs () < p2->NumSVs ())
       return false;

     else if  (p1->NumSVs () > p2->NumSVs ())
       return true;

     else 
       return  p1->Time () < p2->Time ();
   }
};  /* SortByAccuracyComparator */





void  ParameterSearchReportLineList::SortByAccuracy ()
{
  SortByAccuracyComparator*  c = new SortByAccuracyComparator ();
  sort (begin (), end (), *c);
  delete  c;
}  /* SortByGammaCost */




ostream&  operator<< (ostream&                         o,  
                      const ParameterSearchReportLine& rl
                     )
{
  o <<          rl.JobId ();

  if  (rl.Class1 ())
    o << "\t"  << rl.Class1Name ()
      << "\t"  << rl.Class2Name ();
  else
    o << "\t" << "AllClasses";

  o << "\t"  << rl.CParm      ()
    << "\t"  << StrFormatDouble (rl.Gamma (), "-zz0.000000")
    << "\t"  << rl.AParm ()
    << "\t"  << StrFormatDouble (rl.Accuracy (), "-zz0.00") << "%"
    << "\t"  << rl.NumSVs ()
    << "\t"  << StrFormatDouble (rl.AvgPredProb  (), "-zz0.00") << "%"
    << "\t"  << StrFormatDouble (rl.AvgKnownProb (), "-zz0.00") << "%"
    << "\t"  << rl.Compact ()
    << "\t"  << rl.CompactDev ()
    << "\t"  << StrFormatDouble (rl.Time      (), "ZZZ,ZZ0.000")
    << "\t"  << StrFormatDouble (rl.TrainTime (), "ZZZ,ZZ0.000")
    << "\t"  << StrFormatDouble (rl.ClassTime (), "ZZZ,ZZ0.000");

  return  o;
}
