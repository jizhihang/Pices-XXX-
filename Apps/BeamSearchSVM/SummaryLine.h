#ifndef   _SUMMARYLINE_
#define   _SUMMARYLINE_

#include  "BasicTypes.h"
#include  "FeatureNumList.h"
#include  "FileDesc.h"
#include  "KKQueue.h"


#ifndef  _IMAGECLASS_
class ImageClass;
typedef  ImageClass*  ImageClassPtr;

class  ImageClassList;
typedef  ImageClassList*  ImageClassListPtr;
#endif


class  SummaryLine
{
public:
   SummaryLine (FileDescPtr  _fileDesc
               );

   SummaryLine (FileDescPtr        _fileDesc,
                const KKStr&      ln,
                ImageClassListPtr  imageClasses
               );

  ~SummaryLine ();

  ImageClassPtr    Class1         ()  const  {return class1;}
  KKStr           Class1Name     ()  const;

  ImageClassPtr    Class2         ()  const  {return class2;}
  KKStr           Class2Name     ()  const;

  int              FeatureCount   ()  const  {return features.NumOfFeatures ();}

  const
  FeatureNumList&  Features       ()  const  {return features;}

  const
  VectorFloat&     FoldAccuracies () const  {return foldAccuracies;}

  float            SupportPoints  ()  const  {return supportPoints;}
  float            TrainAccuracy  ()  const  {return trainAccuracy;}
  float            TestAccuracy   ()  const  {return testAccuracy;}


private:
  void  ProcessTextLIne (KKStr             ln,
                         ImageClassListPtr  imageClasses
                        );

  ImageClassPtr   class1;
  ImageClassPtr   class2;
  int             class1Count;
  int             class2Count;
  int             jobId;
  float           trainAccuracy;
  float           testAccuracy;
  float           supportPoints;
  VectorFloat     foldAccuracies;
  FeatureNumList  features;

};  /* SummaryLine */

typedef  SummaryLine*   SummaryLinePtr;




class  SummaryLineList: public KKQueue<SummaryLine>
{
public:
  SummaryLineList (RunLog&      _log,
                   bool         _owner,
                   FileDescPtr  _fileDesc
                  );


  SummaryLineList (RunLog&            _log,
                   const KKStr&      _fileName,
                   FileDescPtr        _fileDesc,
                   ImageClassListPtr  _imageClasses,
                   bool&              sucessfull
                  );

  ~SummaryLineList ();

  SummaryLineList*  GetSummaryLinesForTheseClasses (ImageClassPtr  class1,
                                                    ImageClassPtr  class2
                                                   );

  ImageClassListPtr  GetListOfClasses ()  const;
  
  void  SortHighAccuracyLowSupportPoints ();

  void  SortLowSuportPoints ();





private:
  KKStr       fileName;
  FileDescPtr  fileDesc;
  RunLog&      log;
};


typedef  SummaryLineList* SummaryLineListPtr;

#endif
