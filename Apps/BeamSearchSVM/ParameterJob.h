#ifndef  _PARAMETERJOB_
#define  _PARAMETERJOB_


#include  <iomanip>
#include  <iostream>
#include  <fstream>
//#include  <ostream>


using namespace std;

#include  <time.h>

#include  "RunLog.h"
#include  "ImageFeatures.h"
#include  "ImageClass.h"
#include  "ConfussionMatrix2.h"



class  ParameterJobList;

typedef  ParameterJobList*  ParameterJobListPtr;

class  ParameterJob 
{

public:
  ParameterJob (RunLog&    _log,
                int        _processNum,
                int        _jobId,
                int        _cParm,
                double     _gammaParm,
                float      _aParm
               );


  ParameterJob (RunLog&    _log,
                KKStr      desc
               );  


  ParameterJob (ParameterJob&  ParameterJob);

  ~ParameterJob ();



  void    CrossValidation (ImageFeaturesList&    images,
                           ImageClassList&       imageClasses,
                           FeatureNumList&       selectedFeatures,
                           KKStr                 svmParamStr,
                           int                   numOfFolds,
                           ConfussionMatrix2&    results,
                           double&               numOfSupportVectors,
                           float&                avgProbability,
                           float&                avgKnowClassProb,
                           double&               avgCompact,
                           double&               compactDev
                          );

  KKStr  ToString ();


  void    Accuracy (double _accuracy)   {accuracy = _accuracy;}
  double  Accuracy ()                   {return  accuracy;}

  void    CurStatus (char  _curStatus)  {curStatus = _curStatus;}
  char    CurStatus ()                  {return  curStatus;}

  float   ClassTime  ()                 {return  classTime;}
  int     JobId      ()                 {return  jobId;}
  int     ProcessNum ()                 {return  processNum;}
  float   TrainTime  ()                 {return  trainTime;}
  int     CParm      ()                 {return  cParm;}
  double  GammaParm  ()                 {return  gammaParm;}
  float   AParm      ()                 {return  aParm;}
  float   NumSVs     ()                 {return  numSVs;}


private:
  double     accuracy;

  bool       cancelFlag;

  char       curStatus; //  '0' - Scheduled.
                        //  '1' - Selecfted for Processing.
                        //  '2' - Processed.
                        //  '3' - Has been searched already, or  search has started.
                        //        Used for BestFirstSearch.
                        //  '4' - Flagged as Don't Process.  Probably because
                        //        Termination condition for whole run was reaced.


  int        processNum;

  int        jobId;

  RunLog&    log;

  time_t     startTime;
  float      trainTime;
  float      classTime;
  float      numSVs;

  
  int        cParm;
  double     gammaParm;
  float      aParm;
  
};



typedef  ParameterJob*  ParameterJobPtr;




class  ParameterJobList:  public KKQueue<ParameterJob>
{
public:
  ParameterJobList (bool  _owner);

  ParameterJobPtr  LocateByProcessNum (int  _processNum);
};


typedef  ParameterJobList*  ParameterJobListPtr;




class  ParameterJobListIterator: public  QueueIterator<ParameterJob>
{
public:
  ParameterJobListIterator (ParameterJobList&  runs);
};


#endif



