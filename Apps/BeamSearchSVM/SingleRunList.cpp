#include  "FirstIncludes.h"
#include  <stdio.h>
#include  <string>
#include  <iomanip>
#include  <iostream>
#include  <fstream>
#include  <vector>

#ifdef  WIN32
#include  <ostream>
#else
#include <errno.h>
#endif



#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;


#include  "OSservices.h"
#include  "SingleRunList.h"




SingleRunList::SingleRunList (const KKStr&  _fileName):
     KKQueue<SingleRun> (true, 1000),
     fileName         (_fileName),
     lastReadPosValid (false),
     nextBestJobIndex (NULL),
     statusIndex      (NULL)
{
}


SingleRunList::SingleRunList (bool  _owner):
     KKQueue<SingleRun> (_owner, 1000),
     lastReadPosValid (false),
     nextBestJobIndex (NULL),
     statusIndex      (NULL)
{
}





SingleRunList::~SingleRunList ()
{
  delete  statusIndex;
  delete  nextBestJobIndex;
}







void  SingleRunList::LoadStatusFile (Algorithm      algorithm,
                                     FileDescPtr    fileDesc,
                                     RunLog&        log
                                    )
{
  log.Level (10) << "SingleRunList::LoadStatusFile - Loading done Status File." << endl;

  char  buff[40960];

  FILE*  in = osFOPEN (fileName.Str (), "r");
  if  (!in)
  {
     log.Level (-1) << endl
                    << "SingleRunList::LoadStatusFile - *** ERROR ***  Can not open Status File[" 
                    << fileName << "]." 
                    << endl;
     osWaitForEnter ();
     exit (-1);
  }

  if  (lastReadPosValid)
  {
    int returnCd = fsetpos (in, &lastReadPos);
    if  (returnCd != 0)
    {
      int  errorNum = errno;

      log.Level (-1) << endl << endl << endl
        << "SingleRunList::LoadStatusFile    *** ERROR ***   Could not set start position in status file." << endl
        << endl
        << "FileName[" << fileName                    << "]" << endl
        << "ReturnCd[" << returnCd                    << "]" << endl
        << "errno   [" << errorNum                    << "]" << endl
        << "Desc    [" << osGetErrorNoDesc (errorNum) << "]" << endl
        << endl;
      osWaitForEnter ();
      exit (-1);
    }
  }

  while  (fgets (buff, sizeof (buff), in))
  {
    KKStr  buffStr (buff);
    KKStr  fieldName = buffStr.ExtractToken ("\t\n\r ");
    fieldName.TrimLeft ();
    fieldName.TrimRight ();
    fieldName.Upper ();

    if  (fieldName == "JOBQENTRY")
    {
      SingleRunPtr  newRun = new SingleRun (fileDesc, log, buffStr, algorithm, *this);
      SingleRunPtr  existingRun = LookUpByJobId (newRun->JobId ());
      if  (existingRun)
      {
        existingRun->UpdateFromJob (*newRun);
        delete  newRun;  newRun = NULL;
      }
      else
      {
        PushOnBack (newRun);
      }
    }
    else
    {
      log.Level (-1) << endl
                     << "        *** ERROR ***        LoadStatusFileDone" << endl
                     << endl
                     << "Invalid line in Status File[" << fileName << "]" << endl
                     << "Line[" << fieldName << " " << buffStr << "]" << endl
                     << endl;
    }

    if  ((QueueSize () % 10000) == 0)
    {
      log.Level (10) << "LoadStatusFile  [" << QueueSize () << "] Recs Loaded." << endl;
    }
  }

  log.Level (10) << "LoadStatusFile  Total Recs Loaded[" << QueueSize () << "]" << endl;

  {
    int  returnCd = fgetpos (in, &lastReadPos);
    if  (returnCd != 0)
    {
      int  errorNum = errno;

      log.Level (-1) << endl << endl << endl
        << "SingleRunList::LoadStatusFile    *** ERROR ***   Could not Get Last File Position of status file." << endl
        << endl
        << "FileName[" << fileName                    << "]" << endl
        << "ReturnCd[" << returnCd                    << "]" << endl
        << "errno   [" << errorNum                    << "]" << endl
        << "Desc    [" << osGetErrorNoDesc (errorNum) << "]" << endl
        << endl;
      osWaitForEnter ();
      exit (-1);
    }

    lastReadPosValid = true;
  }

  fclose (in);

  log.Level (10) << "LoadStatusFile  Returning" << endl;
}  /* LoadStatusFile */




//void  SingleRunList::AddSingleRun (SingleRunPtr  singleRun)
//{
//  SingleRunPtr  existingJob = LookUpByJobId (singleRun->JobId ());
//  if  (existingJob)
//    DeleteEntry (existingJob);

//  PushOnBack (singleRun);
//}  /* AddSingleRun */



SingleRunListIterator::SingleRunListIterator (SingleRunList&  runs):
     QueueIterator<SingleRun> (runs)
{
}



bool  EffectivlyEqualAccuracy (double  p1,
                               double  p2
                              )
{
  if  ((p1 > (p2 + 0.0001))  ||  (p1 < (p2 - 0.0001)))
    return  false;
  else
    return  true;

}  /* EffectivlyEqualAccuracy */




class  AccuracyComparison: public  QueueComparison<SingleRun>
{
public:
   AccuracyComparison ():  QueueComparison<SingleRun> () {} 


   bool  operator () (SingleRunPtr  opr1,
                      SingleRunPtr  opr2
                     )
   {
     if  (opr1->CurStatus () < opr2->CurStatus ())
       return true;

     else if  (opr1->CurStatus () > opr2->CurStatus ())
       return false;


     if  (EffectivlyEqualAccuracy (opr1->Accuracy (), opr2->Accuracy ()))
     {
       if  (opr1->FeatureCount () == opr2->FeatureCount ())
       {
         return  (opr1->TrainTimeTotal () < opr2->TrainTimeTotal ());
       }
       else
       {
         return (opr1->FeatureCount () < opr2->FeatureCount ());
       }
     }
     else
     {
       return (opr1->Accuracy () > opr2->Accuracy ());
     }
   }
};  /* AccuracyComparison */





class  JobIdComparison: public  QueueComparison<SingleRun>
{
public:
   JobIdComparison ():  QueueComparison<SingleRun> () {} 


   bool  operator () (SingleRunPtr  opr1,
                      SingleRunPtr  opr2
                     )
   {
     return  (opr1->JobId () < opr2->JobId ());
   }
};  /* JobIdComparison */






void  SingleRunList::SortByAccuracy ()
{
  // We will use inefoicient bubble sort fr now, because 
  // that is all I know off the top of my head.

  AccuracyComparison  c;
  sort (begin (), end (), c);
} /* SortByAccuracy */




void  SingleRunList::SortByJobId ()
{
  // We will use inefoicient bubble sort fr now, because 
  // that is all I know off the top of my head.

  JobIdComparison  c;
  sort (begin (), end (), c);
} /* SortByAccuracy */





SingleRunPtr  SingleRunList::LookUpRun (const BitString&  featuresIncluded) 
{
  //BitString  bs (featuresIncluded);

  FeatureIndex::iterator p;
  p = featureIndex.find ((const BitStringPtr)(&featuresIncluded));
  //p = featureIndex.find (&bs);
  if (p == featureIndex.end ())
    return NULL;
  return p->second;
}  /* LookUpRun */




SingleRunPtr  SingleRunList::LookUpRun (const FeatureNumList&  featuresIncluded)
{
  BitString  bs (featuresIncluded.FileDesc ()->NumOfFields ());
  featuresIncluded.ToBitString (bs);
  return  LookUpRun (bs);
}  /* LookUpRun */




SingleRunPtr  SingleRunList::NextUnStartedJob ()
{
  bool          found = false;
  SingleRunPtr  nextJob = NULL;

  for  (int  idx = 0; (idx < QueueSize ())  &&  (!found); idx++)
  {
    nextJob = IdxToPtr (idx);

    found = (nextJob->CurStatus () == '0');
  }

  if  (!found)
    return NULL;

  return  nextJob;
}



SingleRunListPtr  SingleRunList::GetJobsForNumOfFeatures (int numOfFeatures)
{
  SingleRunListPtr result = new SingleRunList (false);

  if  (!result)
  {
    cerr << endl;
    cerr << "SingleRunList  **** ERROR ****" << endl;
    cerr << "               Could not Allocate SingleRunList Object." << endl;
    cerr << endl;
    osWaitForEnter ();
    exit (-1);
  }

  SingleRunListIterator  srIDX (*this);
  for (srIDX.Reset (); srIDX.CurPtr (); ++srIDX)
  {
    if  ((srIDX->FeatureCount () == numOfFeatures)  &&
         (srIDX->CurStatus ()    == '2'))
    {
      result->PushOnBack (srIDX.CurPtr ());
    }
  }

  return  result;  
}  /* GetJobsForNumOfFeatures */






SingleRunPtr  SingleRunList::LookUpByJobId (int id)
{
  if  (id < 0)
    return NULL;

  map<int, SingleRunPtr>::iterator p;
  p = idIndex.find (id);
  if (p == idIndex.end ())
    return NULL;
  return p->second;
}  /* LookUpById */




void  SingleRunList::PushOnBack (SingleRunPtr _entry)
{
  KKQueue<SingleRun>::PushOnBack (_entry);

  SingleRunPtr  existingJob = LookUpByJobId (_entry->JobId ());
  if  (existingJob)
  {
    cerr << endl << endl << endl
         << "SingleRunList::PushOnBack    *** ERROR ***" << endl
         << endl
         << "          Attempted to add duplicate JobId[" << _entry->JobId () << "]" << endl
         << endl;

    osWaitForEnter ();
    exit (-1);
  }

  idIndex.insert (pair<int, SingleRunPtr> (_entry->JobId (), _entry));

  featureIndex.insert (pair<BitStringPtr, SingleRunPtr> (&(_entry->featureNums), _entry));

  if  (statusIndex)
    statusIndex->insert (pair<StatusKey,SingleRunPtr> (StatusKey (_entry->CurStatus (), _entry->JobId ()), _entry));

  if  ((nextBestJobIndex)  &&  (_entry->CurStatus () >= '2'))
    nextBestJobIndex->insert (NextBestJobPair (AccuracyKey (_entry), _entry));
}  /* PushOnBack */



void  SingleRunList::PushOnFront (SingleRunPtr _entry)
{
  KKQueue<SingleRun>::PushOnFront (_entry);
  idIndex.insert (pair<int, SingleRunPtr> (_entry->JobId (), _entry));
  featureIndex.insert (pair<BitStringPtr, SingleRunPtr> (&(_entry->featureNums), _entry));

  if  (statusIndex)
    statusIndex->insert (pair<StatusKey,SingleRunPtr> (StatusKey (_entry->CurStatus (), _entry->JobId ()), _entry));

  if  ((nextBestJobIndex)  &&  (_entry->CurStatus () >= '2'))
    nextBestJobIndex->insert (NextBestJobPair (AccuracyKey (_entry), _entry));
}  /* PushOnBack */




void  SingleRunList::DeleteEntry (SingleRunPtr _entry)
{
  KKQueue<SingleRun>::DeleteEntry (_entry);
  idIndex.erase (_entry->JobId ());

  featureIndex.erase (&(_entry->featureNums));

  if  (statusIndex)
  {
    StatusKey  sk (_entry->CurStatus (), _entry->JobId ());
    statusIndex->erase (sk);
  }

  if  ((nextBestJobIndex)  &&  (_entry->CurStatus () >= '2'))
  {
    AccuracyKey  ak (_entry);
    nextBestJobIndex->erase (ak);
  }

} /* DeleteEntry */




void  SingleRunList::UpdateStatusIndex (SingleRunPtr  j,
                                        char          oldStatus,
                                        char          newStatus
                                       )
{
  if  (statusIndex)
  {
    StatusKey  sk (oldStatus, j->JobId ());

    statusIndex->erase  (sk);

    sk.curStatus = newStatus;
    statusIndex->insert (pair<StatusKey,SingleRunPtr> (sk, j));
  }
}  /* UpdateStatusIndex */




void  SingleRunList::BuildNextBestJobIndex ()
{
  delete  nextBestJobIndex;
  nextBestJobIndex = new NextBestJobIndex ();

  const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    const SingleRunPtr  r = *idx;
    nextBestJobIndex->insert (NextBestJobPair (AccuracyKey (r), r));
  }
}  /* BuildNextBestJobIndex */






void  SingleRunList::BuildStatusIndex ()
{
  delete  statusIndex;
  statusIndex = new StatusKeyIndex ();

  const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    const SingleRunPtr  r = *idx;
    statusIndex->insert (pair<StatusKey,SingleRunPtr> (StatusKey (r->CurStatus (), r->JobId ()), r));
  }
}  /* BuildStatusIndex */



SingleRunPtr  SingleRunList::GetNextOpenJobUsingStatusIndex ()
{
  if  (!statusIndex)
    BuildStatusIndex ();

  StatusKeyIndex::iterator idx;

  StatusKey  key ('0', 0);
  idx = statusIndex->lower_bound (key);
  if  (idx == statusIndex->end ())
    return NULL;

  if  (idx->first.curStatus != '0')
    return NULL;

  return  idx->second;
}  /* GetNextOpenJobUsingStatusIndex */



SingleRunListPtr  SingleRunList::GetNextBestJobs ()
{
  if  (!nextBestJobIndex)
    BuildNextBestJobIndex ();

  NextBestJobIndex::iterator idx;

  SingleRunPtr  j = NULL;

  AccuracyKey  key (0, float (9999.9), 0);

  //  Look for 1st un-expanded job
  idx = nextBestJobIndex->lower_bound (key);
  while  (idx != nextBestJobIndex->end ())
  {
    j = idx->second;
    if  (j->CurStatus () == '2')
      break;
    idx++;
  }

  if  (idx == nextBestJobIndex->end ())
    return NULL;

  SingleRunList  nextBestJobsCandidates (false);

  double  highestAccuracyWeAreLookingAt = j->Accuracy () - 0.001;
  int     smallestFeatureCount = j->FeatureCount ();
  double  bestTrainTime = 99999.99;

  while  (idx != nextBestJobIndex->end ())
  {
    j = idx->second;
    if  (j->Accuracy () < highestAccuracyWeAreLookingAt)
      break;

    if  (j->FeatureCount () > smallestFeatureCount)
      break;
    
    if  (j->CurStatus () == '2')  
    {
      nextBestJobsCandidates.PushOnBack (j);
      bestTrainTime = Min (bestTrainTime, j->TrainTimeTotal ());
    }

    idx++;
  }

  SingleRunListPtr  nextBestJobs = new SingleRunList (false);

  double  acceptableTrainTime = bestTrainTime * 1.2;

  SingleRunList::const_iterator  idx2;
  for  (idx2 = nextBestJobsCandidates.begin ();  idx2 != nextBestJobsCandidates.end ();  idx2++)
  {
    SingleRunPtr  j = *idx2;
    if  (j->TrainTimeTotal () <= acceptableTrainTime)
      nextBestJobs->PushOnBack (*idx2);
  }

  nextBestJobs->RandomizeOrder ();
  
  return  nextBestJobs;
}  /* GetNextBestJobs */




bool  SingleRunList::AllJobsEvaluated ()  const
{
  bool  allJobsEvaluated = true;

  SingleRunList::const_iterator  idx;
  for  (idx = begin ();  ((idx != end ())  &&  (allJobsEvaluated));  idx++)
  {
    SingleRunPtr  j = *idx;
    if  (j->CurStatus () < '2')
      allJobsEvaluated = false;
  }

  return  allJobsEvaluated;
}  /* AllJobsEvaluated */


