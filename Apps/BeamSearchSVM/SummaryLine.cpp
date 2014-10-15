#include  "FirstIncludes.h"

#include  <fstream>
#include  <iostream>
#include  <stdlib.h>
#include  <stdio.h>
#include  <vector>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;





#include  "SummaryLine.h"

#include  "ImageClass.h"
#include  "OSservices.h"
#include  "StatisticalFunctions.h"




SummaryLine::SummaryLine (FileDescPtr  _fileDesc):

  class1         (NULL),
  class2         (NULL),
  class1Count    (0),
  class2Count    (0),
  jobId          (0),
  trainAccuracy  (0.0f),
  testAccuracy   (0.0f),
  supportPoints  (0.0f),
  foldAccuracies (),
  features       (_fileDesc)
{
}



SummaryLine::SummaryLine (FileDescPtr        _fileDesc,
                          const KKStr&      ln,
                          ImageClassListPtr  imageClasses
                         ):

  class1         (NULL),
  class2         (NULL),
  class1Count    (0),
  class2Count    (0),
  jobId          (0),
  trainAccuracy  (0.0f),
  testAccuracy   (0.0f),
  supportPoints  (0.0f),
  foldAccuracies (),
  features       (_fileDesc)
{
  ProcessTextLIne (ln, imageClasses);
}




SummaryLine::~SummaryLine ()
{
}



void  SummaryLine::ProcessTextLIne (KKStr             ln,
                                    ImageClassListPtr  imageClasses
                                   )
{
  KKStr  fieldName = ln.ExtractToken ("\t\n\r");

  while  (!fieldName.Empty ())
  {
    fieldName.TrimLeft ();
    fieldName.TrimRight ();

    KKStr  data = ln.ExtractToken ("\t\n\r");
    data.TrimLeft ();
    data.TrimRight ();

    fieldName.Upper ();
    if  (fieldName == "CLASS1")
      class1 = imageClasses->GetImageClassPtr (data);

    else if  (fieldName == "CLASS2")
      class2 = imageClasses->GetImageClassPtr (data);

    else if  (fieldName == "CLASS1COUNT")
      class1Count = atoi (data.Str ());

    else if  (fieldName == "CLASS2COUNT")
      class2Count = atoi (data.Str ());

    else if  (fieldName == "JOBID")
      jobId = atoi (data.Str ());

    else if  (fieldName == "TRAINACC")
      trainAccuracy = (float)atof (data.Str ());

    else if  (fieldName == "TESTACC")
      testAccuracy = (float)atof (data.Str ());

    else if  (fieldName == "SP")
      supportPoints = (float)atof (data.Str ());

    else if  (fieldName == "FOLDS")
    {
      while  (!data.Empty ())
      {
        float accuracy = (float)data.ExtractTokenDouble (" ,\t\n\r");
        foldAccuracies.push_back (accuracy);
      }
    }

    else if  (fieldName == "FC")
      int featureCount = atoi (data.Str ());

    else if  (fieldName == "FEATURES")
    {
      bool valid;
      features.ExtractFeatureNumsFromStr (data, valid);
    }

    fieldName = ln.ExtractToken ("\t\n\r");
  }
} /* ProcessTextLIne */





KKStr  SummaryLine::Class1Name ()  const
{
  if  (!class1)
    return  "";
  else
    return  class1->Name ();
}  /* Class1Name */



KKStr  SummaryLine::Class2Name ()  const
{
  if  (!class2)
    return  "";
  else
    return  class2->Name ();
}  /* Class2Name */




SummaryLineList::SummaryLineList (RunLog&      _log,
                                  bool         _owner,
                                  FileDescPtr  _fileDesc
                                 ):
    KKQueue<SummaryLine> (_owner),
    log                (_log),
    fileDesc           (_fileDesc),
    fileName           ()
{
}




SummaryLineList::SummaryLineList (RunLog&            _log,
                                  const KKStr&      _fileName,
                                  FileDescPtr        _fileDesc,
                                  ImageClassListPtr  _imageClasses,
                                  bool&              sucessfull
                                 ):
    KKQueue<SummaryLine> (),
    fileDesc           (_fileDesc),
    fileName           (_fileName),
    log                (_log)
{
  FILE*  f = osFOPEN (fileName.Str (), "r");
  if  (!f)
  {
    sucessfull = false;
    return;
  }

  char  buff[10240];


  ImageClassPtr  oldClass1 = NULL;
  ImageClassPtr  oldClass2 = NULL;

  while  (fgets (buff, sizeof (buff), f))
  {
    KKStr  ln (buff);
    ln.TrimLeft ();
    ln.TrimRight ();

    if  (!ln.Empty ())
    {
      SummaryLinePtr  s = new SummaryLine (_fileDesc, ln, _imageClasses);
      if  ((s->Class1 () != oldClass1)  ||  (s->Class2 () != oldClass2))
      {
        cout << s->Class1Name () << "\t" << s->Class2Name () << endl;
        oldClass1 = s->Class1 ();
        oldClass2 = s->Class2 ();
      }

      PushOnBack (s);
    }
  }

  fclose (f);
}




SummaryLineList::~SummaryLineList ()
{
}



SummaryLineListPtr  SummaryLineList::GetSummaryLinesForTheseClasses (ImageClassPtr  class1,
                                                                     ImageClassPtr  class2
                                                                    )
{
  SummaryLineListPtr  results = new SummaryLineList (log, false, fileDesc);

  iterator  idx;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    SummaryLinePtr  s = *idx;

    if  ((s->Class1 () == class1)  &&  (s->Class2 () == class2))
      results->PushOnBack (s);

    else if  ((s->Class1 () == class2)  &&  (s->Class2 () == class1))
      results->PushOnBack (s);
  }

  return  results;
} /* GetSummaryLinesForTheseClasses */



ImageClassListPtr  SummaryLineList::GetListOfClasses ()  const
{
  ImageClassListPtr  classes = new ImageClassList ();

  const_iterator  idx;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    const SummaryLinePtr s = *idx;
    ImageClassPtr  ic = NULL;
    ic = classes->GetImageClassPtr (s->Class1Name ());
    ic = classes->GetImageClassPtr (s->Class2Name ());
  }

  return  classes;
}  /* GetListOfClasses */




class  SummaryLineComparrisonHighAccuracyLowSP: public QueueComparison<SummaryLine>
{
public:
   SummaryLineComparrisonHighAccuracyLowSP ():  QueueComparison<SummaryLine> () {} 

   bool  operator() (SummaryLinePtr  p1,
                     SummaryLinePtr  p2
                    )
   {
     if  (p1->TestAccuracy () > p2->TestAccuracy ())
       return  true;

     else if  (p1->TestAccuracy () < p2->TestAccuracy ())
       return  false;

     else if  (p1->SupportPoints () < p2->SupportPoints ())
       return true;

     else if  (p1->SupportPoints () > p2->SupportPoints ())
       return false;

     else
       return  p1->FeatureCount () < p2->FeatureCount ();
   }
};  /* SummaryLineComparrisonHighAccuracyLowSP */





void  SummaryLineList::SortHighAccuracyLowSupportPoints ()
{
  SummaryLineComparrisonHighAccuracyLowSP  c;
  sort (begin (), end (), c);
}




class  SummaryLineComparrisonSupportPoints: public QueueComparison<SummaryLine>
{
public:
   SummaryLineComparrisonSupportPoints ():  QueueComparison<SummaryLine> () {} 

   bool  operator() (SummaryLinePtr  p1,
                     SummaryLinePtr  p2
                    )
   {
     if  (p1->SupportPoints () < p2->SupportPoints ())
       return true;

     else if  (p1->SupportPoints () > p2->SupportPoints ())
       return false;

     else
       return  p1->FeatureCount () < p2->FeatureCount ();
   }
};  /* SummaryLineComparrisonHighAccuracyLowSP */




void  SummaryLineList::SortLowSuportPoints ()
{
  SummaryLineComparrisonHighAccuracyLowSP  c;
  sort (begin (), end (), c);
}



