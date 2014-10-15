// #include  <stdio.h>
#include  <string>
#include  <iomanip>
#include  <iostream>
#include  <fstream>
#include  <vector>

#ifdef  WIN32
#include  <ostream>
#endif

using namespace std;



#include  <math.h>

#include  "ParameterProcess.h"

#include  "Queue.h"
#include  "Str.h"
#include  "OSservices.h"





ParameterProcess::ParameterProcess (RunLog&        _log,
                                    int            _processNum,
                                    int            _jobId,
                                    ImageClassPtr  _class0,
                                    ImageClassPtr  _class1,
                                    int            _cParm,
                                    double         _gammaParm,
                                    int            _aParm
                                   ):
  log           (_log),
  processNum    (_processNum),
  jobId         (_jobId),
  class0        (_class0),
  class1        (_class1),
  cParm         (_cParm),
  gammaParm     (_gammaParm),
  aParm         (_aParm)
{
  highestAccuracy = (FFLOAT)0.0;
  startTime       = 0;
  curStatus       = '0';
}




ParameterProcess::ParameterProcess (RunLog&          _log,
                                    ImageClassList&  imageClasses,
                                    String           _desc
                                   ):
  log (_log)
{
  highestAccuracy  = (FFLOAT)0.0;
  startTime        = 0;

  _desc.TrimLeft ();

  curStatus = _desc.ExtractChar ();
 
  if  ((curStatus == '0')  ||  
       (curStatus == '1')  ||  
       (curStatus == '2')
      )
  {
    // We Are Ok
  }
  else
  {
    log.Level (-1) << endl 
                   << "ParameterProcess:  We have an invalid Cur Status[" << curStatus << "]."
                   << endl;
    exit (-1);
  }


  {
    String  processsNumField = _desc.ExtractToken (" ,\t");
    processNum = atoi (processsNumField.Str ());
  }


  {
    String  jobIdField = _desc.ExtractToken (" ,\t");
    jobId = atoi (jobIdField.Str ());
  }


  {
    // Lets get the two Classes that we are running for

    String  class0Name = _desc.ExtractToken (" ,\t");
    String  class1Name = _desc.ExtractToken (" ,\t");


    if  (class0Name == "NoName")
    {
      class0 = class1 = NULL;
    }
    else
    {
      class0 = imageClasses.LookUpByName (class0Name);
      class1 = imageClasses.LookUpByName (class1Name);

      if  (!class0)
      {
        cerr << endl
             << "*** ERROR ***   ParameterProcess,  Invalid Class0[" << class0Name << "]." << endl
             << endl;
        exit (-1);
      }

      if  (!class1)
      {
        cerr << endl
             << "*** ERROR ***   ParameterProcess,  Invalid Class0[" << class1Name << "]." << endl
             << endl;
        exit (-1);
      }
    }
  }


  {
    String  cParmStr = _desc.ExtractToken (" ,\t");
    cParm = atoi (cParmStr.Str ());
  }


  {
    String  gammaParmStr = _desc.ExtractToken (" ,\t");
    gammaParm = atof (gammaParmStr.Str ());
  }


  {
    String  aParmStr = _desc.ExtractToken (" ,\t");
    aParm = atoi (aParmStr.Str ());
  }


  {
    String  highestAccuracyField = _desc.ExtractToken (" ,\t");
    highestAccuracy = (FFLOAT)atof (highestAccuracyField.Str ());
  }
}




ParameterProcess::ParameterProcess (ParameterProcess&  process):
  log             (process.log),
  processNum      (process.processNum),
  jobId           (process.jobId),
  highestAccuracy (process.highestAccuracy),
  curStatus       (process.curStatus),
  startTime       (process.startTime),
  class0          (process.class0),
  class1          (process.class1),
  cParm           (process.cParm),
  gammaParm       (process.gammaParm),
  aParm           (process.aParm)
{
}




ParameterProcess::~ParameterProcess ()
{
}




String  ParameterProcess::ToString ()
{
  String  statusStr ("");

  statusStr.Append (curStatus);

  String  class0Name;
  String  class1Name;

  if  (!class0)
    class0Name = "NoName";
  else
    class0Name = class0->Name ();

  if  (!class1)
    class1Name = "NoName";
  else
    class1Name = class1->Name ();
    

  statusStr << "\t" << processNum
            << "\t" << jobId 
            << "\t" << class0Name
            << "\t" << class1Name
            << "\t" << cParm
            << "\t" << gammaParm
            << "\t" << aParm
            << "\t" << highestAccuracy;
            
  
  return  statusStr;
} /* ToString */






void  ParameterProcess::ImcrementParameters (ParameterJobPtr  job)

{
  gammaParm = gammaParm * (FFLOAT)1.5;
  if  (gammaParm > (FFLOAT)11.0)
  {
    gammaParm = 0.00001;

    cParm = (int)(((FFLOAT)cParm * 1.5) + (FFLOAT)0.5);
  }

  if  (job->Accuracy () > highestAccuracy)
    highestAccuracy = job->Accuracy ();

  jobId++;
}  /* ImcrementParameters */




ParameterProcessList::ParameterProcessList (bool  _owner)  :  
               Queue<ParameterProcess> (_owner, 100)
{
}




ParameterProcessPtr  ParameterProcessList::LocateByProcessNum (int  _processNum)
{
  ParameterProcessPtr  j = NULL;

  int  idx = 0;


  while  (idx < QueueSize ())
  {
    j = IdxToPtr (idx);
    if  (j->ProcessNum () == _processNum)
      return  j;

    idx++;
  }

  return  NULL;
}





ParameterProcessListIterator::ParameterProcessListIterator (ParameterProcessList&  runs):
     QueueIterator<ParameterProcess> (runs)
{
}


