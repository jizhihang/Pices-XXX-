#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>

#include  <fstream>
#include  <iostream>
#include  <map>
#include  <vector>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif


#include  "MemoryDebug.h"

using namespace std;



#include "BasicTypes.h"
#include "KKQueue.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;



#include "Job.h"
#include "JobManager.h"
using namespace  JobManagment;



Job::Job (const Job&  j):

  log                  (j.log),
  manager              (j.manager),
  jobId                (j.jobId),
  parentId             (j.parentId),
  numProcessors        (j.numProcessors),
  numPorcessesAllowed  (j.numPorcessesAllowed),
  prerequisites        (j.prerequisites),
  status               (j.status)

{
}
 

  
//  To create a brand new job that has not been proceesed yet.  
Job::Job (JobManagerPtr  _manager,
          int            _jobId,
          int            _parentId,
          int            _numPorcessesAllowed,
          RunLog&        _log
         ):

  manager              (_manager),
  jobId                (_jobId),
  parentId             (_parentId),
  numProcessors        (0),
  numPorcessesAllowed  (_numPorcessesAllowed),
  prerequisites        (),
  status               (jsOpen),
  log                  (_log)

{
}





Job::Job (JobManagerPtr  _manager):
  manager              (_manager),
  jobId                (-1),
  parentId             (-1),
  numProcessors        (0),
  numPorcessesAllowed  (1),
  prerequisites        (),
  status               (jsNULL),
  log                  (_manager->Log ())
{
}




Job::~Job ()
{
}




const char*   Job::JobType ()  const
{
  return  "Job";
}





typedef  JobPtr (*ConstructorPtr)(const KKStr&);

map<KKStr, Job::ConstructorPtr>  Job::registeredConstructors;



void  Job::RegisterConstructor (const KKStr&    _name,
                                ConstructorPtr  _constructor
                               )
{
  registeredConstructors.insert (pair<KKStr, ConstructorPtr>(_name, _constructor));
}
                                       



JobPtr  Job::CallAppropriateConstructor (JobManagerPtr  _manager,
                                         const KKStr&   _jobTypeName,
                                         const KKStr&   _statusStr
                                        )
{
  ConstructorIndex::iterator  idx;
  idx = registeredConstructors.find (_jobTypeName);
  if  (idx == registeredConstructors.end ())
    return NULL;
  JobPtr  j = idx->second (_manager);
  j->ProcessStatusStr (_statusStr);
  return  j;
}  /* CallAppropriateConstructor */





void    Job::ReFresh (Job&  j)
{
  jobId     = j.jobId;
  parentId  = j.parentId;
  manager   = j.manager;
  status    = j.status;


  numProcessors        = j.numProcessors;
  numPorcessesAllowed  = j.numPorcessesAllowed;
  prerequisites        = j.prerequisites;
  status               = j.status;
}  /* ReFresh */



KKStr  Job::JobStatusToStr (JobStatus  status)
{
  if  (status == jsOpen)
    return "Open";

  if  (status == jsStarted)
    return "Started";

  if  (status == jsDone)
    return "Done";

  if  (status == jsExpanded)
    return "Expanded";

  return  "";
}  /* JobStatusToStr */




Job::JobStatus  Job::JobStatusFromStr (const KKStr&  statusStr)
{
  if  (statusStr.CompareIgnoreCase ("OPEN") == 0)
    return  jsOpen;

  if  (statusStr.CompareIgnoreCase ("STARTED") == 0)
    return jsStarted;

  if  (statusStr.CompareIgnoreCase ("DONE") == 0)
    return jsDone;

  if  (statusStr.CompareIgnoreCase ("Expanded") == 0)
    return jsExpanded;

  return jsNULL;
}  /* JobStatusToStr */




KKStr  Job::StatusStr () const
{
  return  JobStatusToStr (status);
}  /* StatusStr */



KKStr  Job::PrerequisitesToStr ()  const
{
  if  (prerequisites.size () < 1)
    return  "None";

  KKStr  s (5 + prerequisites.size () * 5);
  for  (uint x = 0;  x < prerequisites.size ();  x++)
  {
    if  (x > 0)
      s << ",";
    s << prerequisites[x];
  }
  return  s;
}  /* PrerequisitesToStr */




void  Job::PrerequisitesFromStr (const KKStr&  s)
{
  prerequisites.clear ();

  if  (s.CompareIgnoreCase ("None") != 0)
  {
    VectorKKStr fields = s.Split (',');
    for  (uint x = 0;  x < fields.size ();  x++)
    {
      int p = fields[x].ToInt ();
      prerequisites.push_back (p);
    }
  }
}  /* PrerequisitesFromStr */




KKStr  Job::ToStatusStr ()
{
  KKStr  statusStr (200);  // Preallocating 200 bytes.

  statusStr << "JobId"                << "\t" << jobId                << "\t"
            << "ParentId"             << "\t" << parentId             << "\t"
            << "Status"               << "\t" << StatusStr ()         << "\t"
            << "NumProcessors"        << "\t" << numProcessors        << "\t"
            << "NumPorcessesAllowed"  << "\t" << numPorcessesAllowed  << "\t"
            << "Prerequisites"        << "\t" << PrerequisitesToStr ();

  return  statusStr;
}  /* ToStatusStr */




void  Job::ProcessStatusStr (const KKStr&  statusStr)
{
  log.Level (30) << "Job::ProcessStatusStr[" << statusStr << "]" << endl;
  KKStr  fieldName;
  KKStr  fieldValue;

  VectorKKStr fields = statusStr.Split ('\t');
  uint  fieldNum = 0;

  while  (fieldNum < fields.size ())
  {
    fieldName = fields[fieldNum];
    fieldNum++;
    if  (fieldNum < fields.size ())
    {
      fieldValue = fields[fieldNum];
      fieldNum++;
    }
    else
    {
      fieldValue = "";
    }

    fieldName.Upper ();
    fieldValue.TrimLeft ("\n\r\t ");
    fieldValue.TrimRight ("\n\r\t ");

    if  (fieldName.CompareIgnoreCase ("JOBID") == 0)
      jobId = atoi (fieldValue.Str ());

    else  if  (fieldName.CompareIgnoreCase ("PARENTID") == 0)
      parentId = atoi (fieldValue.Str ());

    else  if  (fieldName.CompareIgnoreCase ("STATUS") == 0)
      status = JobStatusFromStr (fieldValue);
   
    else  if  (fieldName.CompareIgnoreCase ("NumProcessors") == 0)
      numProcessors = fieldValue.ToInt ();

    else  if  (fieldName.CompareIgnoreCase ("NumPorcessesAllowed") == 0)
      numPorcessesAllowed = fieldValue.ToInt ();

    else  if  (fieldName.CompareIgnoreCase ("Prerequisites") == 0)
      PrerequisitesFromStr (fieldValue);
      
    else
    {
      ProcessStatusField (fieldName, fieldValue);
    }
  }
}  /* ProcessStatusStr */





void  Job::ProcessStatusField (const KKStr&  fieldName,
                               const KKStr&  fieldValue
                              )
{
   log.Level (-1) << "Job::ProcessStatusField  Invalid Field Name[" << fieldName << "]." << endl;
}  /* ProcessStatusField */




void  Job::CompletedJobDataWrite (ostream& o)
{
}


 
// Works with 'WriteCompletedJobData';  You use this to load in data written by 'WriteCompletedJobData'
void  Job::CompletedJobDataRead (istream& i)
{
}





JobList::JobList (JobManagerPtr  _manager):

   KKQueue<Job> (true, 500),
   log          (_manager->Log ()),
   manager      (_manager)

{
}



JobList::JobList (const JobList&  jobs):
     KKQueue<Job>        (jobs.Owner (), jobs.QueueSize ()),
     jobIdLookUpTable    (),
     jobIdLookUpTableIdx (),
     log                 (jobs.log),
     manager             (jobs.manager)
  
{
  JobList::const_iterator  idx;
  for  (idx = jobs.begin ();  idx != jobs.end ();  idx++)
  {
    JobPtr j = *idx;
    if  (Owner ())
      PushOnBack (j->Duplicate ());
    else
      PushOnBack (j);
  }
}



JobPtr  JobList::LookUpByJobId (int  jobId)
{
  jobIdLookUpTableIdx = jobIdLookUpTable.find (jobId);
  if  (jobIdLookUpTableIdx == jobIdLookUpTable.end ())
    return  NULL;

  return  jobIdLookUpTableIdx->second;
}  /* LookUpByJobId */





bool  JobList::AllPrequisitesDone (JobPtr job)
{
  const VectorInt&  p = job->Prerequisites ();
  for  (uint z = 0;  z < p.size ();  z++)
  {
    int jobId = p[z];
    JobPtr pj = LookUpByJobId (jobId);
    if  ((pj->Status () != Job::jsDone)  &&  (pj->Status () != Job::jsExpanded))
      return false;
  }
  return true;
}  /* AllPrequisitesDone */




JobPtr  JobList::LocateOpenJob ()
{
  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    JobPtr  j = IdxToPtr (x);
    if  (j->Status () == Job::jsOpen)
      return  j;
  }

  return  NULL;
}  /* LocateOpenJob */




bool  JobList::AreAllJobsDone ()
{
  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    JobPtr  j = IdxToPtr (x);
    if  ((j->Status () != Job::jsDone)  &&  (j->Status () != Job::jsExpanded))
      return false;
  }

  return  true;
}  /* AreAllJobsAreDone */








void   JobList::PushOnBack (JobPtr  j)
{
  jobIdLookUpTableIdx = jobIdLookUpTable.find (j->JobId ());
  if  (jobIdLookUpTableIdx != jobIdLookUpTable.end ())
  {
    log.Level (-1) << endl
                   << endl
                   << "JobList::PushOnBack      ***ERROR***" << endl 
                   << endl 
                   << "JobList::PushOnBack        Duplicate JobId[" << j->JobId () << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  KKQueue<Job>::PushOnBack (j);
  jobIdLookUpTable.insert (JobIdLookUpTablePair (j->JobId (), j));

  return;
}  /* PushOnBack */






bool  JobList::JobsStillRunning ()
{
  JobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    JobPtr  j = *idx;
    if  (j->Status () == Job::jsStarted)
      return true;
  }
  return  false;
}  /* JobsStillRunning */


