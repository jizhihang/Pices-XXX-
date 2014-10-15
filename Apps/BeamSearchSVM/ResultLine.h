#ifndef  _RESULTLINE_
#define  _RESULTLINE_

#include  "DateTime.h"
#include  "FeatureNumList.h"

#include  "SingleRun.h"


#ifndef  _IMAGECLASS_
class  ImageClass;
typedef  ImageClass*  ImageClassPtr;
class  ImageClassList;
typedef  ImageClassList*  ImageClassListPtr;
#endif

#ifndef  _FILEDESC_
class  FileDesc;
typedef  FileDesc*  FileDescPtr;
#endif


class  ResultLineList;



class  ResultLine  
{
public:
  typedef  ResultLine*  ResultLinePtr;

  ResultLine (FileDescPtr        _fileDesc,
              KKStr              text,
              bool&              goodFormat,
              ImageClassListPtr  imageClasses,
              RunLog&            _log,
              bool               _beamSeasrch,
              bool               useNewFormat,   // Result file is from new 
              ResultLineList&    results         // Binary Feature selection.
             );                           

  
  ResultLine (ResultLine&   resultLine,
              RunLog        _log
             );


  ResultLine (const SingleRun&  sr,
              ImageClassList&   _imageClasses,
              FileDescPtr       _fileDesc,
              ResultLineList&    results
             );


  ~ResultLine ();


  FFLOAT             Accuracy ()               {return  accuracy;}

  bool               BeamSearch () const       {return  beamSearch;}

  FFLOAT             ChgInAccuracy ()   const  {return  chgInAccuracy;}
  
  const
  ImageClassPtr      Class1 ()  const          {return  class1;}

  const
  ImageClassPtr      Class2 ()  const          {return  class2;}

  const
  ImageClassListPtr  Classes () const          {return  classes;}

  const 
  KKStr&             Class1Name ();  

  const 
  KKStr&             Class2Name ();

  int                FeatureCount ()  const  {return  featureCount;}

  int                FeatureThatChanged ()  {return  featureThatChanged;}

  FeatureNumList     GetFeatureNums ()      {return  FeatureNumList (fileDesc, featureNums);}

  const BitString&   FeatureNums ()         {return  featureNums;}

  int                NumOfFeatures () const {return  featureNums.Count ();}

  float              Grade ()    const      {return  grade;}

  int                JobId ()    const      {return  jobId;}

  int                ParentId () const      {return  parentId;}

  float              SupportPoints      () const {return  supportPoints;}
  float              TestAccuracy       () const {return  testAccuracy;}
  float              TestGrade          () const {return  testGrade;}
  double             TestTime           () const {return  testTime;}
  double             TrainTime          () const {return  trainTime;}
  float              OtherClassProbMean () const {return  otherClassProbMean;}

  void  SupportPoints      (float  _supportPoints)       {supportPoints      = _supportPoints;}
  void  TestAccuracy       (float  _testAccuracy)        {testAccuracy       = _testAccuracy;}
  void  TestGrade          (float  _testGrade)           {testGrade          = _testGrade;}
  void  TrainTime          (double _trainTime)           {trainTime          = _trainTime;}
  void  TestTime           (double _testTime)            {testTime           = _testTime;}
  void  OtherClassProbMean (float  _otherClassProbMean)  {otherClassProbMean = _otherClassProbMean;}


private:
  
  void  GetAccuracyField (KKStr&   text,
                          bool     featuresExcluded
                         );

  void  ParseText (KKStr&             text,
                   bool&              formatGood,
                   ImageClassListPtr  imageClasses
                  );

  void  ParseTextNew (KKStr&             text,          // Used for the result files
                      bool&              formatGood,    // produced by the new Feature
                      ImageClassListPtr  imageClasses   // Selection App.
                     );

  void  ParseTextNew2 (KKStr&             text,
                       bool&              formatGood,
                       ImageClassListPtr  imageClasses
                      );

  void  VerifyAccuracyChange ();

  FFLOAT             accuracy;
  bool               beamSearch;
  FFLOAT             chgInAccuracy;
  ImageClassPtr      class1;
  ImageClassPtr      class2;
  ImageClassListPtr  classes;
  int                featureCount;
  int                featureThatChanged;
  //FeatureNumListPtr  featureNums;
  BitString          featureNums;
  FileDescPtr        fileDesc;
  int                jobId;
  ResultLinePtr      parent;
  int                parentId;

  float              thirdClassProbMean;  // aka decisiveProbMean
  float              grade;



  float              testAccuracy;
  float              testGrade;
  float              otherClassProbMean;
  DateTime           runTime;
  double             trainTime;
  double             testTime;
  float              supportPoints;

  RunLog&            log;

  static  
  KKStr              emptyStr;

};




typedef  ResultLine::ResultLinePtr  ResultLinePtr;



class  ResultLineList:  public KKQueue<ResultLine>
{
public:
  ResultLineList (int  _size);

  void  SortByAccuracy ();

  void  SortByGrade ();

  void  SortByJobId ();

  ResultLinePtr     LookUpByJobId (int _jobId);

  void              PushOnBack  (ResultLinePtr _entry);
 
  void              PushOnFront (ResultLinePtr _entry);



private:
  typedef  map<int, ResultLinePtr>    IdIndex;

  IdIndex    idIndex;

};


typedef  ResultLineList*  ResultLineListPtr;


#endif
