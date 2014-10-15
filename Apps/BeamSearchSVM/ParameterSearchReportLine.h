#ifndef  _PARAMETERSEARCHREPORTLINE_
#define  _PARAMETERSEARCHREPORTLINE_


#include  "Str.h"


#ifndef  _IMAGECLASS_
class  ImageClass;
typedef  ImageClass*  ImageClassPtr;

class  ImageClassList;
typedef  ImageClassList*  ImageClassListPtr;
#endif


class  ParameterSearchReportLine
{
public:
  ParameterSearchReportLine (KKStr             text,
                             ImageClassListPtr  classes,
                             RunLog&            _log
                            );

  int            JobId        () const {return  jobId;}
  ImageClassPtr  Class1       () const {return  class1;}
  ImageClassPtr  Class2       () const {return  class2;}
  int            CParm        () const {return  cParm;}
  float          Gamma        () const {return  gamma;}
  int            AParm        () const {return  aParm;}
  float          Accuracy     () const {return  accuracy;}
  float          NumSVs       () const {return  numSVs;}
  float          AvgPredProb  () const {return  avgPredProb;}
  float          AvgKnownProb () const {return  avgKnownProb;}
  float          Compact      () const {return  compact;}
  float          CompactDev   () const {return  compactDev;}
  float          Time         () const {return  time;}
  float          TrainTime    () const {return  trainTime;}
  float          ClassTime    () const {return  classTime;}

  const KKStr&  Class1Name ()  const
  {
    if  (class1)  
      return  class1->Name ();
    else
      return  emptyName;
  }

  const KKStr&  Class2Name ()  const
  {
    if  (class2)  
      return  class2->Name ();
    else
      return  emptyName;
  }

private:
  int            jobId;
  ImageClassPtr  class1;
  ImageClassPtr  class2;
  int            cParm;
  float          gamma;
  int            aParm;
  float          accuracy;
  float          numSVs;
  float          avgPredProb;
  float          avgKnownProb;
  float          compact;
  float          compactDev;
  float          time;
  float          trainTime;
  float          classTime;
  ImageClassList classes;
  VectorFloat    classAccuracies;

  static KKStr emptyName;
};


typedef  ParameterSearchReportLine*  ParameterSearchReportLinePtr;




class  ParameterSearchReportLineList:  public KKQueue<ParameterSearchReportLine>
{
public:
  ParameterSearchReportLineList (bool  _owner);

  ParameterSearchReportLineList (KKStr              fileName,
                                 ImageClassListPtr  classes,
                                 RunLog&            _log
                                );

  ParameterSearchReportLineList*    ExtractTwoClasses (ImageClassPtr  class1,
                                                       ImageClassPtr  class2
                                                      );


  void  SortByGammaCost ();
  void  SortByAccuracy  ();
};


typedef  ParameterSearchReportLineList*  ParameterSearchReportLineListPtr;


ostream&  operator<< (ostream&                         o,  
                      const ParameterSearchReportLine& rl
                     );

#endif
