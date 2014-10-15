#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <string>


#include  <iomanip>
#include  <iostream>
#include  <fstream>
#include  <vector>

#ifdef  WIN32
#include  <ostream>
#endif



#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;



#include  "Str.h"
#include  "OSservices.h"


#include  "ResultLine.h"

#include  "FileDesc.h"
#include  "ImageClass.h"
#include  "RunLog.h"


// JobId 43094  Parent 43025  FeatureChg 32  Excluded_Features(2,5,6,9,16,17,25,26,32,43,44,52) 94.6000% -0.8500% Larvacean 94.100%  Marine_Snow_Dark 95.100%  13 secs


// C:\users\kkramer\GradSchool\Plankton\BeamSearches\GrayScalePaperFeatureSearch\ActiveLearning_ValidationImages.data.result

ResultLine::ResultLine (FileDescPtr        _fileDesc,
                        KKStr             text,
                        bool&              goodFormat,
                        ImageClassListPtr  imageClasses,
                        RunLog&            _log,
                        bool               _beamSearch,
                        bool               useNewFormat,
                        ResultLineList&    results
                       ):

  accuracy            (0.0),
  chgInAccuracy       (0.0),
  featureThatChanged  (-1),
  featureNums         (_fileDesc->NumOfFields ()),
  featureCount        (0),
  fileDesc            (_fileDesc),
  jobId               (-1),
  log                 (_log),
  parent              (NULL),
  parentId            (-1),
  class1              (NULL),
  class2              (NULL),
  classes             (new ImageClassList ()),
  beamSearch          (_beamSearch),
  thirdClassProbMean  (0.0f),
  grade               (0.0f),
  runTime             (),
  testGrade           (0.0f),
  testAccuracy        (0.0f),
  otherClassProbMean  (0.0f)

{
  // Because i have modifid th eoutput of the featire selection search several times;
  // we have to determine which format we are looking at dynamicly and then call the
  // appropriate parsing routine.

  bool  useNewFormat2 = false;
  if  (text.StrInStr ("SupPts"))
  {
    if  (text.StrInStr ("FtrsIncl")  ||  
         text.StrInStr ("FtrsExcl")  ||
         text.StrInStr ("FTRSHEX")
        )
      useNewFormat2 = true;
  }

  if  (useNewFormat2)
    ParseTextNew2 (text, goodFormat, imageClasses);

  else if  (useNewFormat)
    ParseTextNew (text, goodFormat, imageClasses);

  else
    ParseText (text, goodFormat, imageClasses);

  if  (parentId >= 0)
    parent = results.LookUpByJobId (parentId);
  else
    parent = NULL;

  featureCount = featureNums.Count ();

  VerifyAccuracyChange ();
}






ResultLine::ResultLine (ResultLine&   resultLine,
                        RunLog        _log
                       ):

  accuracy            (resultLine.accuracy),
  beamSearch          (resultLine.beamSearch),
  chgInAccuracy       (resultLine.chgInAccuracy),
  featureThatChanged  (resultLine.featureThatChanged),
  featureNums         (resultLine.featureNums),
  featureCount        (resultLine.featureCount),
  fileDesc            (resultLine.fileDesc),
  jobId               (resultLine.jobId),
  log                 (resultLine.log),
  parentId            (resultLine.parentId),
  parent              (resultLine.parent),
  class1              (resultLine.class1),
  class2              (resultLine.class2),
  classes             (new ImageClassList (*(resultLine.classes))),
  thirdClassProbMean  (resultLine.thirdClassProbMean),
  grade               (resultLine.grade),
  runTime             (resultLine.runTime),
  testGrade           (resultLine.testGrade),
  testAccuracy        (resultLine.testAccuracy)
{
}



ResultLine::ResultLine (const SingleRun&  sr,
                        ImageClassList&   _imageClasses,
                        FileDescPtr       _fileDesc,
                        ResultLineList&    results
                       ):

   accuracy            (float (sr.Accuracy ())),
   beamSearch          (false),
   chgInAccuracy       (float (sr.ChgInAccuracy ())),
   class1              (NULL),
   class2              (NULL),
   classes             (new ImageClassList (_imageClasses)),
   featureThatChanged  (sr.ChgFeatureNum ()),
   featureNums         (sr.FeatureNums ()),
   featureCount        (sr.FeatureNums ().Count ()),
   fileDesc            (_fileDesc),
   jobId               (sr.JobId ()),
   log                 (_fileDesc->Log ()),
   parent              (NULL),
   parentId            (sr.ParentJobId ()),
   thirdClassProbMean  (0.0f),
   grade               (0.0f),
   runTime             (),

   trainTime           (sr.TrainTimeTotal ()),
   testTime            (sr.TestTimeTotal ()),

   testGrade           (0.0f),
   testAccuracy        (0.0f),
   otherClassProbMean  (0.0f)

{
  if  (parentId >= 0)
    parent = results.LookUpByJobId (parentId);
  else
    parent = NULL;

  VerifyAccuracyChange ();
}





ResultLine::~ResultLine ()
{
  delete  classes;
}




KKStr  ResultLine::emptyStr = "";


const 
KKStr&  ResultLine::Class1Name ()
{
  if  (class1)
     return  class1->Name ();
  else
     return  emptyStr;
}



 const 
 KKStr&  ResultLine::Class2Name ()          
 {
   if  (class2)
     return  class2->Name ();
   else
     return  ResultLine::emptyStr;
 }




void  ResultLine::GetAccuracyField (KKStr&  text,
                                    bool     featuresExcluded
                                   )

{
  KKStr  accuracyStr = text.ExtractToken (" \n\r\t");

  if  (accuracyStr.LastChar () == '%')
    accuracyStr.ChopLastChar ();

  accuracy = (FFLOAT)atof (accuracyStr.Str ());

  KKStr  chgInAccuracyStr = text.ExtractToken (" \n\r\t");
  if  (chgInAccuracyStr.LastChar () == '%')
    chgInAccuracyStr.ChopLastChar ();

  chgInAccuracy = (FFLOAT)atof (chgInAccuracyStr.Str ());
  if  (parentId < 0)
    chgInAccuracy = (FFLOAT)0.0;

  if  (featuresExcluded)
    chgInAccuracy = 0.0f - chgInAccuracy;
}  /* GetAccuracyField */



void  ResultLine::VerifyAccuracyChange ()
{
  // First lets verify which feature changed.
  if  (!parent)
    return;

  FFLOAT   newChgInAccuracy = FFLOAT (0.0);

  bool  featureIsInParent = parent->featureNums.Test (featureThatChanged);
  bool  featureIsInChild  = featureNums.Test (featureThatChanged);

  if  (featureIsInChild  &&  featureIsInParent)
  {
    cerr << endl << endl << endl
         << "ResultLine::VerifyAccuracyChange    *** ERROR ***        Feature Did Not Change." << endl
         << endl
         << "               JobId[" << jobId << "]   ParentId[" << parentId << "]" << endl
         << "               FeatureThatChanged[" << featureThatChanged << "]" << endl
         << endl;
    return;
  }
  else if  ((!featureIsInChild)  &&  (!featureIsInParent))
  {
    cerr << endl << endl << endl
         << "ResultLine::VerifyAccuracyChange    *** ERROR ***    Feature not in parent or child." << endl
         << endl
         << "          JobId[" << jobId << "]   ParentId[" << parentId << "]" << endl
         << "          FeatureThatChanged[" << featureThatChanged << "]" << endl
         << endl;
    return;
  }

  if  (featureIsInChild  &&  (!featureIsInParent))
    newChgInAccuracy = parent->Accuracy () - accuracy;
  else
    newChgInAccuracy = accuracy - parent->Accuracy ();


  FFLOAT  deltaChgInAccuracy = newChgInAccuracy - chgInAccuracy;
  if  (fabs (deltaChgInAccuracy) > 0.0001)
  {
    //cerr << endl << endl << endl
    //     << "ResultLine::VerifyAccuracyChange    *** ERROR ***        Change in Accurcay was invalid." << endl
    //     << endl
    //     << "               JobId[" << jobId << "]   ParentId[" << parentId << "]" << endl
    //     << "               chgInAccuracy    [" << chgInAccuracy    << "]" << endl
    //     << "               newChgInAccuracy [" << newChgInAccuracy << "]"
    //     << endl;
    chgInAccuracy = newChgInAccuracy;
  }
}  /* VerifyAccuracyChange */





void  ResultLine::ParseText (KKStr&            text,
                             bool&              formatGood,
                             ImageClassListPtr  imageClasses
                            )
{
  KKStr  field = text.ExtractToken (" \n\r\t");
  field.Upper ();
  if  (field != "JOBID")
  {
    formatGood = false;
    return;
  }

  featureNums.ReSet ();

  jobId = text.ExtractTokenInt (" \n\t\r");

  field = text.ExtractToken (" \n\r\t");
  field.Upper ();
  if  (field != "PARENT")
  {
    formatGood = false;
    return;
  }
  parentId = text.ExtractTokenInt (" \n\t\r");


  field = text.ExtractToken (" \n\r\t");
  field.Upper ();
  if  (field != "FEATURECHG")
  {
    formatGood = false;
    return;
  }
  featureThatChanged = text.ExtractTokenInt (" \n\r\t");


  field = text.ExtractToken (" \n\r\t");
  field.Upper ();

  if  (field.SubStrPart (0, 16) == "EXCLUDED_FEATURES")
  {
    field = field.SubStrPart (18);
    int x = field.LocateCharacter (')');
    if  (x < 0)
    {
      formatGood = false;
      return;
    }

    KKStr  excludedFeatrureStr = field.SubStrPart (0, x - 1);

    bool  valid;
    FeatureNumList fns (fileDesc, ExcludeFeatureNums, excludedFeatrureStr, valid);
    fns.ToBitString (featureNums);
    GetAccuracyField (text, true);
  }

  else if  (field.SubStrPart (0, 16) == "INCLUDED_FEATURES")
  {
    field = field.SubStrPart (18);
    int x = field.LocateCharacter (')');
    if  (x < 0)
    {
      formatGood = false;
      return;
    }

    KKStr  includedFeatrureStr = field.SubStrPart (0, x - 1);
    field = field.SubStrPart (x + 1);

    bool  valid;
    FeatureNumList fns (fileDesc, 
                        IncludeFeatureNums, 
                        includedFeatrureStr,
                        valid
                       );
    fns.ToBitString (featureNums);

    GetAccuracyField (text, false);
  }

  else
  {
    featureNums.ReSet ();
    formatGood = false;
    return;
  }

  //if  (!featureNums)
  //{
  //  featureNums = new FeatureNumList (fileDesc);
  //}


  // Lets get the classes Involved
  KKStr  className = text.ExtractToken (" \n\r\t");
  className.Upper ();

  int  classCount = 0;

  while  ((!className.Empty ())  &&  (className.FirstChar () >= 'A')  &&  (className.FirstChar () <= 'Z'))
  {
    if  (className.Len () > 8)
    {
      if  (className.SubStrPart (className.Len () - 8) == "100.000%")
      {
        className = className.SubStrPart (0, className.Len () - 9);
        text = "100.0% " + text;
      }
    }

    ImageClassPtr  tempClass = imageClasses->GetImageClassPtr (className);

    classes->AddImageClass (tempClass);

    if  (classCount == 0)
    {
      class1 = tempClass;
    }

    else if  (classCount == 1)
    {
      class2 = tempClass;
    }

    KKStr  classAccuracyStr = text.ExtractToken (" \n\r\t");

    classCount++;

    className = text.ExtractToken (" \n\r\t");
    className.Upper ();
  }

  formatGood = true;
  return;
}  /* ParseText */






//***********************************************************
//*       To support the new Binary Feature Search          *
//***********************************************************
void  ResultLine::ParseTextNew (KKStr&            text,
                                bool&              formatGood,
                                ImageClassListPtr  imageClasses
                               )
{
  featureNums.ReSet ();

  KKStr  field = text.ExtractToken (" \n\r\t");
  field.Upper ();
  if  (field != "JOBID")
  {
    formatGood = false;
    return;
  }
  jobId = text.ExtractTokenInt (" \n\t\r");


  field = text.ExtractToken (" \n\r\t");
  field.Upper ();
  if  (field != "PARENT")
  {
    formatGood = false;
    return;
  }
  parentId = text.ExtractTokenInt (" \n\t\r");


  field = text.ExtractToken (" \n\r\t");
  field.Upper ();
  if  (field != "FEATURECHG")
  {
    formatGood = false;
    return;
  }
  featureThatChanged = text.ExtractTokenInt (" \n\r\t");

  field = text.ExtractToken (" \n\r\t");
  field.Upper ();

  if  (field.SubStrPart (0, 17) == "INCLUDED_FEATURES(")
  {
    field = field.SubStrPart (18);
    int x = field.LocateCharacter (')');
    if  (x < 0)
    {
      formatGood = false;
      return;
    }

    KKStr  includedFeatrureStr = field.SubStrPart (0, x - 1);
    field = field.SubStrPart (x + 1);

    bool  valid;
    FeatureNumList fns (fileDesc, IncludeFeatureNums, includedFeatrureStr, valid);
    fns.ToBitString (featureNums);

    accuracy           = (FFLOAT)text.ExtractTokenDouble (" \n\t\r%");
    chgInAccuracy      = (FFLOAT)text.ExtractTokenDouble (" \n\t\r%");
    thirdClassProbMean = (FFLOAT)text.ExtractTokenDouble (" \n\t\r%");  // aka decisiveProbMean
    grade              = (FFLOAT)text.ExtractTokenDouble (" \n\t\r%");  
  }

  else
  {
    featureNums.ReSet ();
    formatGood = false;
    return;
  }

  // Lets get the classes Involved
  KKStr  className = text.ExtractToken (" \n\r\t");
  className.Upper ();

  int  classCount = 0;

  while  ((!className.Empty ())  &&  (className.FirstChar () >= 'A')  &&  (className.FirstChar () <= 'Z'))
  {
    if  (className.Len () > 8)
    {
      if  (className.SubStrPart (className.Len () - 8) == "100.000%")
      {
        className = className.SubStrPart (0, className.Len () - 9);
        text = "100.0% " + text;
      }
    }

    ImageClassPtr  tempClass = imageClasses->GetImageClassPtr (className);
    classes->AddImageClass (tempClass);

    if  (classCount == 0)
    {
      class1 = tempClass;
    }

    else if  (classCount == 1)
    {
      class2 = tempClass;
    }

    KKStr  classAccuracyStr = text.ExtractToken (" \n\r\t");

    classCount++;

    className = text.ExtractToken (" \n\r\t");
    className.Upper ();
  }

  formatGood = true;
  return;
}  /* ParseTextNew */







void  ResultLine::ParseTextNew2 (KKStr&             text,
                                 bool&              formatGood,
                                 ImageClassListPtr  imageClasses
                                )
{
  // In this format each logical field will consist of 2 physical fields
  // The firt field will be the label and the second the value that coresponds with the label.
  //
  //  Example Record:
  // JobId	1	Parent	0	FtrChg	0	FtrsIncl	1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48	Acc	81.9000%	AccChg	0.0000%	TrainTime	0.25	TestTime	0.052	SupPts	492	ClassAcc	Copepod 85.000%  Larvacean 75.500%  Marine_Snow 85.000%  Oithona 85.500%  Tricho 78.500%
  // Logical fields are  (JobId	1), (Parent	0), (FtrChg	0), (FtrsIncl	1,2,3 ... 48), etc etc etc
  // 
  // Each physical; field will be seperated with a tab(\t) character.
  //

  // due to poor programming practices by the writer; who for selfish reason i would like to have remainnameless
  // there is some extra hitches to the above description.  In some result files we will find spaces seperating 
  // physical fields, that is the label field will be seperated from its value field by a space(' ') character.


  formatGood = true;

  //JobId 3	Parent 0	FeatureChg 2	FtrsExcl	2	Acc	98.3333%	AccChg	0.0000%	SupPts	27.3	ClassAcc	-1  94.737%  1 100.000%	RunTime	Sep/28/2006-20:19:22


  KKStr  label, value;

  while  (!text.Empty ())
  {
    text.TrimLeft (" ");  // Eliminate any leading space charaters.

    int  nextSpaceChar = text.LocateCharacter (' ');
    int  nextTabChar   = text.LocateCharacter ('\t');

    if  (nextSpaceChar < 0)
    {
      label = text.ExtractToken2 ("\t\r\n");
      value = text.ExtractToken2 ("\t\r\n");
    }

    else if  (nextTabChar < 0)
    {
      // There are no tab delimiter chraters left,  but there is a space left.
      // Tere is no way that a space chrater can be in a lable so the space must be
      // our delimiter character.
      label = text.ExtractToken2 ("\t\r\n ");
      value = text.ExtractToken2 ("\t\r\n ");
    }

    else
    {
      // There are both a space character and a tab character left,  
      // the one that is next is our delimter charater.
      if  (nextSpaceChar < nextTabChar)
      {
        label = text.ExtractToken2 (" ");
        value = text.ExtractToken2 ("\t\r\n");
      }
      else
      {
        label = text.ExtractToken2 ("\t\r\n");
        value = text.ExtractToken2 ("\t\r\n");
      }
    }


    label.Upper ();

    if  (label == "JOBID")
      jobId = atoi (value.Str ());

    else if  (label == "PARENT")
      parentId = atoi (value.Str ());

    else if  ((label == "FTRCHG")  ||  (label == "FEATURECHG"))
      featureThatChanged = atoi (value.Str ());

    else if  (label == "FTRSINCL")
    {
      bool  valid;
      featureNums.ReSet ();
      FeatureNumList fns (fileDesc, value, valid);
      fns.ToBitString (featureNums);
    }

    else if  (label == "FTRSEXCL")
    {
      featureNums.ReSet ();
      bool  valid;
      FeatureNumList fns (FeatureNumList (fileDesc, ExcludeFeatureNums, value, valid));
      fns.ToBitString (featureNums);
    }

    else if  (label == "FTRSHEX")
    {
      bool  validHexStr = false;
      featureNums = BitString::FromHexStr (value, validHexStr);
      if  (!validHexStr)
      {
        cerr << endl << endl << endl
             << "ResultLine::ParseTextNew2     *** ERROR ***   Invalid Hex KKStr for FeatureNums" << endl
             << endl
             << "  JobId[" << jobId << "] HexStr[" << value << "]" << endl
             << endl;
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (label == "ACC")
      accuracy = (FFLOAT)atof (value.Str ());

    else if  (label == "ACCCHG")
      chgInAccuracy = (FFLOAT)atof (value.Str ());

    else if  ((label == "TRAINTIME")  ||  (label == "TRAINTIMETOTAL"))
      trainTime = atof (value.Str ());

    else if  ((label == "TESTTIME")   ||  (label == "TESTTIMETOTAL"))
      testTime = atof (value.Str ());

    else if  (label == "SUPPTS")
      supportPoints = float (atof (value.Str ()));

    else if  (label == "CLASSACC")
    {
      // 'value' should be equal entire class line, 
      // becuase each field is seprated by spaces only.

      delete  classes;
      classes = new ImageClassList ();

      while  (!value.Empty ())
      {
        KKStr  className   = value.ExtractToken (" \t\n\r");
        KKStr  accuracyStr = value.ExtractToken (" \t\n\r");

        ImageClassPtr  tempClass = imageClasses->GetImageClassPtr (className);

        classes->AddImageClass (tempClass);
        if  (classes->QueueSize () == 1)
          class1 = tempClass;

        else if  (classes->QueueSize () == 2)
          class2 = tempClass;
      }
    }


    else if  ((label == "NEWHIGH")  ||  (label.SubStrPart (0, 9) == "***NEWHIGH"))
    {
    }

    else if  (label == "RUNTIME")
    {
      runTime = DateTime (value);
    }

    else
    {
      // We have an unrecognized field.
      log.Level (-1) << endl << endl << endl
                     << "ResultLine::ParseTextNew2     *** ERROR ***" << endl
                     << endl
                     << "                    Unrecognized Field[" << label << "]" << endl
                     << "                    JobId[" << jobId << "]" << endl
                     << endl;

      formatGood = false;
    }
  }

  return;
}  /* ParseTextNew2 */








class  CompareResultLineByAccuracy: public  QueueComparison<ResultLine>
{
public:

   bool  operator () (ResultLinePtr  p1,  
                      ResultLinePtr  p2
                     )

   {
     if  (p1->Accuracy () > p2->Accuracy ())
       return  true;

     else if  (p1->Accuracy () < p2->Accuracy ())
       return false;

     else if  (p1->FeatureCount () < p2->FeatureCount ())
       return true;

     else if  (p1->FeatureCount () > p2->FeatureCount ())
       return false;

     else if  (p1->TrainTime () < p2->TrainTime ())
       return true;

     else if  (p1->TrainTime () > p2->TrainTime ())
       return false;

     else if  (p1->JobId ()  > p2->JobId ())
       return  true;

     else if  (p1->JobId ()  < p2->JobId ())
       return false;

     return false;
  }
};



class  CompareResultLineByGrade: public  QueueComparison<ResultLine>
{
public:

   bool  operator () (ResultLinePtr  p1,  
                      ResultLinePtr  p2
                     )

   {
     if  (p1->Grade () > p2->Grade ())
       return  true;

     else if  (p1->Grade () < p2->Grade ())
       return false;

     else if  (p1->JobId ()  > p2->JobId ())
       return  true;

     else if  (p1->JobId ()  < p2->JobId ())
       return false;

     return false;
  }
};





class  CompareResultLineByJobId: public  QueueComparison<ResultLine>
{
public:

   bool  operator () (ResultLinePtr  p1,  
                      ResultLinePtr  p2
                     )

   {
     return  (p1->JobId () < p2->JobId ());
   }
};







ResultLineList::ResultLineList (int  _size):

  KKQueue<ResultLine> (true, _size)

{
}


ResultLinePtr  ResultLineList::LookUpByJobId (int _jobId)
{
  IdIndex::iterator p;
  p = idIndex.find (_jobId);
  if (p == idIndex.end ())
    return NULL;
  return p->second;
}  /* LookUpByJobId */





void  ResultLineList::PushOnBack  (ResultLinePtr _entry)
{
  KKQueue<ResultLine>::PushOnBack (_entry);
  idIndex.insert (pair<int, ResultLinePtr> (_entry->JobId (), _entry));
}  /* PushOnBack */
 


void  ResultLineList::PushOnFront (ResultLinePtr _entry)
{
  KKQueue<ResultLine>::PushOnFront (_entry);
  idIndex.insert (pair<int, ResultLinePtr> (_entry->JobId (), _entry));
}  /* PushOnFront */



void  ResultLineList::SortByAccuracy ()
{
  CompareResultLineByAccuracy  c;
  sort (begin (), end (), c);
}  /* Sort */



void  ResultLineList::SortByGrade ()
{
  CompareResultLineByGrade  c;
  sort (begin (), end (), c);
}  /* Sort */





void  ResultLineList::SortByJobId ()
{
  CompareResultLineByJobId  c;
  sort (begin (), end (), c);
}  /* Sort */
