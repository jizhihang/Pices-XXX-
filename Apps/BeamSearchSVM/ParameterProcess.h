#ifndef  _PARAMETERPROCESS_
#define  _PARAMETERPROCESS_


#include  <iomanip>
#include  <iostream>
#include  <fstream>


using namespace std;

#include  <time.h>

#include  "RunLog.h"
#include  "ParameterJob.h"

class  ParameterProcessList;

typedef  ParameterProcessList*  ParameterProcessListPtr;

class  ParameterProcess 
{

public:
  ParameterProcess (RunLog&        _log,
                    int            _processNum,
                    int            _jobId,
                    ImageClassPtr  _class0,
                    ImageClassPtr  _class1,
                    int            _cParm,
                    double         _gammaParm,
                    int            _aParm
                   );


  ParameterProcess (RunLog&          _log,
                    ImageClassList&  imageClasses,
                    String           _desc
                   );

  ParameterProcess (ParameterProcess&  ParameterProcess);

  ~ParameterProcess ();

  int            AParm      ()       {return  aParm;}
  ImageClassPtr  Class0     ()       {return  class0;}
  ImageClassPtr  Class1     ()       {return  class1;}
  int            CParm      ()       {return  cParm;}
  char           CurStatus  ()       {return  curStatus;}
  double         GammaParm  ()       {return  gammaParm;}
  FFLOAT         HighestAccuracy ()  {return  highestAccuracy;}
  int            JobId      ()       {return  jobId;}
  int            ProcessNum ()       {return  processNum;}




  void    AParm     (int     _aParm)      {aParm     = _aParm;}
  void    CParm     (int     _cParm)      {cParm     = _cParm;}
  void    CurStatus (char    _curStatus)  {curStatus = _curStatus;}
  void    GammaParm (double  _gammaParm)  {gammaParm = _gammaParm;}
  void    JobId     (int     _jobId)      {jobId     = _jobId;}


  void    ImcrementParameters (ParameterJobPtr  job);

  String  ToString ();


private:
  char           curStatus; //  '0' - Running
                            //  '1' - All Done
                            //  '2' - Needs to be restarted.

  int            processNum;

  ImageClassPtr  class0;

  ImageClassPtr  class1;

  FFLOAT         highestAccuracy;

  int            jobId;

  RunLog&        log;

  time_t         startTime;
  
  int            cParm;
  double         gammaParm;
  int            aParm;
};



typedef  ParameterProcess*  ParameterProcessPtr;




class  ParameterProcessList:  public Queue<ParameterProcess>
{
public:
  ParameterProcessList (bool  _owner);

  ParameterProcessPtr  LocateByProcessNum (int  _processNum);
};


typedef  ParameterProcessList*  ParameterProcessListPtr;




class  ParameterProcessListIterator: public  QueueIterator<ParameterProcess>
{
public:
  ParameterProcessListIterator (ParameterProcessList&  runs);
};


#endif



