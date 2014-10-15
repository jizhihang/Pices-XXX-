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
#include <limits.h>
#include <unistd.h>
#endif


#include "MemoryDebug.h"

using namespace  std;
 


#include "BasicTypes.h"
#include "KKQueue.h"
#include "Matrix.h"
#include "OSservices.h"
#include "RunLog.h"
#include "StatisticalFunctions.h"
#include "Str.h"
using namespace  KKU;


#include "MLClass.h"
#include "MLClassConstList.h"
using namespace  MLL;


#include "BinaryJobList.h"
#include "Processor.h"



BinaryJobList::BinaryJobList (ProcessorPtr  _processor):
   KKQueue<BinaryJob> (true, 500),
   fileName            (),
   lastFilePostion     (0),
   log                 (_processor->Log ()),
   processor           (_processor),
   jobIdLookUpTable    (),
   jobIdLookUpTableIdx (),
   parmsLookUpTable    (),
   parmsLookUpTableIdx (),
   parmsLookUpJob   (_processor->Log (), _processor->FileDesc ())

{
}




BinaryJobList::BinaryJobList (ProcessorPtr _processor,
                              KKStr        _fileName
                             ):
   KKQueue<BinaryJob> (true, 500),
   fileName            (_fileName),
   lastFilePostion     (0),
   log                 (_processor->Log ()),
   processor           (_processor),
   jobIdLookUpTable    (),
   jobIdLookUpTableIdx (),
   parmsLookUpTable    (),
   parmsLookUpTableIdx (),
   parmsLookUpJob      (_processor->Log (), _processor->FileDesc ())
{
  Load ();
}



BinaryJobList::~BinaryJobList ()
{
}




FileDescPtr   BinaryJobList::FileDesc ()
{
  return  processor->FileDesc ();
}



void  BinaryJobList::Load ()
{
  FILE*  in = osFOPEN (fileName.Str (), "r");
  if  (!in)
  {
    log.Level (-1) << endl
                   << endl
                   << "BinaryJobList::Load    ***ERROR***" << endl
                   << endl
                   << "     Error Opening Status File[" << fileName << "]" << endl
                   << endl;
    exit (-1);
  }

  char  buff[10240];

  while  (fgets (buff, sizeof (buff), in))
  {
    if  ((buff[0] == '/')   &&  (buff[1] == '/'))
      continue;

    KKStrParser statusStr (buff);
    BinaryJobPtr j = new BinaryJob (processor, statusStr);
    PushOnBack (j);
  }

  lastFilePostion = ftell (in);

  fclose (in);
}  /* Load */




void  BinaryJobList::Save ()
{
  KKStr  dirPath, rootName, ext;

  osParseFileName (fileName, dirPath, rootName, ext);

  KKStr name1 = osAddSlash (dirPath) + rootName + "_Prev1" + ext;
  KKStr name2 = osAddSlash (dirPath) + rootName + "_Prev2" + ext;

  osDeleteFile (name2);
  osRenameFile (name1, name2);
  osRenameFile (fileName, name1);

  ofstream  statusFile (fileName.Str ());

  if  (!statusFile.is_open ())
  {
    log.Level (-1) << endl
                   << endl
                   << "BinaryJobList::Save    ***ERROR***" << endl
                   << endl
                   << "Error Saving to file[" << fileName << "]" << endl
                   << endl;

    osWaitForEnter ();
    exit (-1);
  }

  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    statusFile << IdxToPtr (x)->ToStatusStr ();
  }
  
  lastFilePostion = statusFile.tellp ();

  statusFile.close ();
}  /* Save */




BinaryJobPtr  BinaryJobList::LookUpByFeatureNums (const FeatureNumList&  _features)
{
  int  x;

  for  (x = 0;  x < QueueSize (); x++)
  {
    BinaryJobPtr  j = IdxToPtr (x);
    if  (j->Features () == _features)
      return  j;
  }

  return  NULL;
}  /* LookUpByFeatureNums */





BinaryJobPtr  BinaryJobList::LookUpByParameters (const  FeatureNumList&  features,
                                                 int32                   numOfRounds,
                                                 double                  cParm,
                                                 double                  gamma,
                                                 float                   aParm,
                                                 bool                    validateOnly
                                                )
{
  parmsLookUpJob.Features     (features);
  parmsLookUpJob.NumOfRounds  (numOfRounds);
  parmsLookUpJob.CParm        (cParm);
  parmsLookUpJob.GammaParm    (gamma);
  parmsLookUpJob.AParm        (aParm);
  parmsLookUpJob.ValidateOnly (validateOnly);

  ParmsKey key (&parmsLookUpJob);
  parmsLookUpTableIdx = parmsLookUpTable.find (key);
  if  (parmsLookUpTableIdx == parmsLookUpTable.end ())
    return  NULL;

  return  parmsLookUpTableIdx->second;
}  /* LookUpByParameters */




BinaryJobPtr  BinaryJobList::LookUpByJobId (int  jobId)
{
  jobIdLookUpTableIdx = jobIdLookUpTable.find (jobId);
  if  (jobIdLookUpTableIdx == jobIdLookUpTable.end ())
    return  NULL;

  return  jobIdLookUpTableIdx->second;
}  /* LookUpByJobId */




BinaryJobPtr  BinaryJobList::LocateOpenJob ()
{
  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    BinaryJobPtr  j = IdxToPtr (x);
    if  (j->Status () == bjOpen)
      return  j;
  }

  return  NULL;
}  /* LocateOpenJob */



BinaryJobPtr  BinaryJobList::LowestDeltaAccuracyProbTestJob ()
{
  if  (QueueSize () < 1)
    return NULL;

  const_iterator  idx;

  BinaryJobPtr  lowestDeltaTestJob = NULL;
  float  lowestTestDelta = 999999.99f;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  (!j->ValidateOnly ())
      continue;

    float  jTestGrade = j->TestGrade ();
    float  jTestDelta = j->TestDeltaProbAccuracy ();
    if  (jTestGrade <= 0.0f)
      continue;

    if  ((jTestDelta < lowestTestDelta)  ||  
         ((jTestDelta == lowestTestDelta) &&  (jTestGrade > lowestDeltaTestJob->TestGrade ())))
    {
      lowestTestDelta = jTestDelta;
      lowestDeltaTestJob = j;
    }
  }

  return  lowestDeltaTestJob;
}  /* LowestDeltaAccuracyProbTestJob */




/**
 * Will locate the job in the list that has the smallest 'DeltaProbAccuracy'.  If two jobs
 * If two jobs have the same 'DeltaProbAccuracy' then the one that has the highest'Grade'
 * will be selected.  After the 1st 2 criteria we will select the job with the smallest
 *@code
 *  'AParam'.
 *  1) Smallest 'DeltaProbAccuracy';
 *  2) Largest  'Grade'
 *  3) Smallest 'AParm'
 *@endcode
 */
BinaryJobPtr  BinaryJobList::LowestDeltaAccuracyProbJob ()
{
  if  (QueueSize () < 1)
    return NULL;

  const_iterator  idx;

  idx = begin ();
  BinaryJobPtr  lowestDeltaAccuracyProbJob = *idx;
  float  lowestDeltaProbAccuracy = lowestDeltaAccuracyProbJob->DeltaProbAccuracy ();

  while  (idx != end ())
  {
    BinaryJobPtr  j = *idx;
    float  jGrade = j->Grade ();
    float  jDelta = j->DeltaProbAccuracy ();
    if  (jDelta < lowestDeltaProbAccuracy)
    {
      lowestDeltaProbAccuracy = jDelta;
      lowestDeltaAccuracyProbJob = j;
    }

    else if  (jDelta == lowestDeltaProbAccuracy)
    {
      if  (jGrade > lowestDeltaAccuracyProbJob->Grade ())
      {
        lowestDeltaProbAccuracy = jDelta;
        lowestDeltaAccuracyProbJob = j;
      }
      else if  (jGrade == lowestDeltaAccuracyProbJob->Grade ())
      {
        if  (j->AParm () < lowestDeltaAccuracyProbJob->AParm ())
        {
          lowestDeltaProbAccuracy = jDelta;
          lowestDeltaAccuracyProbJob = j;
        }
      }
    }

    idx++;
  }

  return  lowestDeltaAccuracyProbJob;
}  /* LowestDeltaAccuracyProbJob2 */





BinaryJobPtr  BinaryJobList::HighestGradedJob ()
{
  const_iterator  idx;
  if  (QueueSize () < 1)
    return NULL;

  idx = begin ();
  BinaryJobPtr  highestGradeJob = *idx;
  float  highestGrade = highestGradeJob->Grade ();

  while  (idx != end ())
  {
    BinaryJobPtr  j = *idx;
    float  jGrade = j->Grade ();
    if  ((jGrade > highestGrade)  ||  ((jGrade == highestGrade) &&  (j->ProcessingTime () < highestGradeJob->ProcessingTime ())))
    {
      highestGrade = jGrade;
      highestGradeJob = j;
    }

    idx++;
  }

  return  highestGradeJob;
}  /* HighestGradedJob */




BinaryJobPtr  BinaryJobList::HighestGradedTestJob ()
{
  const_iterator  idx;
  if  (QueueSize () < 1)
    return NULL;

  idx = begin ();
  BinaryJobPtr  highestGradedTestJob = *idx;
  float  highestTestGrade = highestGradedTestJob->TestGrade ();

  while  (idx != end ())
  {
    BinaryJobPtr  j = *idx;
    float  jTestGrade = j->TestGrade ();
    if  ((jTestGrade > highestTestGrade)  ||  ((jTestGrade == highestTestGrade) &&  (j->ProcessingTime () < highestGradedTestJob->ProcessingTime ())))
    {
      highestTestGrade = jTestGrade;
      highestGradedTestJob = j;
    }

    idx++;
  }

  return  highestGradedTestJob;
}  /* HighestGradedTestJob */






BinaryJobPtr  BinaryJobList::FastestJob ()
{
  const_iterator  idx;
  idx = begin ();
  BinaryJobPtr  fastestJob = *idx;
  double  fastesProcessingTime = fastestJob->ProcessingTime ();

  while  (idx != end ())
  {
    BinaryJobPtr  j = *idx;
    double  jPT = j->ProcessingTime ();
    if  (jPT < fastesProcessingTime)
    {
      fastesProcessingTime = jPT;
      fastestJob = j;
    }

    idx++;
  }

  return  fastestJob;
}  /* FastestJob */



//void  BinaryJobList::CleanUpCrossValidations ()
//{
//  int  x;
//  for  (x = 0;  x < QueueSize (); x++)
//  {
//    BinaryJobPtr  j = IdxToPtr (x);
//    j->DeleteCrossValidation ();
//  }
//}  /* CleanUpCrossValidations */





bool  BinaryJobList::AreAllJobsDone ()
{
  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    BinaryJobPtr  j = IdxToPtr (x);
    if  ((j->Status () != bjDone)  &&  (j->Status () != bjExpanded))
      return false;
  }

  return  true;
}  /* AreAllJobsAreDone */




BinaryJobPtr  BinaryJobList::SelectOneDoneJobAtRandomFromTop10Percent ()
{
  log.Level (20) << "BinaryJobList::SelectOneDoneJobAtRandomFromTop10Percent" << endl;

  SortByGrade ();

  int  limit = Max (1, (int)((float)QueueSize () * 0.10));
  if  (limit < 0)
    return NULL;

  int  numOfTries = 0;

  while  (numOfTries < 100)
  {
    int  idx = rand () % limit;

    BinaryJobPtr  j = IdxToPtr (idx);
    if  (j->Status () == bjDone)
      return  j;
    numOfTries++;
  }

  return NULL;
}  /* SelectOneOpenJobAtRandomFromTop10Percent */






bool  EffectivlyEqualAccuracy (float  p1, float p2)
{
  if  ((p1 > (p2 + 0.0001))  ||  (p1 < (p2 - 0.0001)))
    return  false;
  else
    return  true;
}  /* EffectivlyEqualAccuracy */




bool  JobAcuracyComparison (float  accuracy1, const FeatureNumList&  f1,  double procTime1, int randNum1,
                            float  accuracy2, const FeatureNumList&  f2,  double procTime2, int randNum2,
                            bool   featureCountPrefSmall
                           )
{
  if  (!EffectivlyEqualAccuracy (accuracy1, accuracy2))
    return  accuracy1 > accuracy2;

  if  (featureCountPrefSmall)
  {
    // We prefer less features if accuracy is the same.
    if  (f1.NumOfFeatures () < f2.NumOfFeatures ())
      return true;

    else if  (f1.NumOfFeatures () > f2.NumOfFeatures ())
      return false;
  }
  else
  {
    // We prefer more features if accuracy is the same.
    if  (f1.NumOfFeatures () < f2.NumOfFeatures ())
      return false;

    else if  (f1.NumOfFeatures () > f2.NumOfFeatures ())
      return true;
  }

  if  (procTime1 < procTime2)
    return true;
  
  else if  (procTime1 > procTime2)
    return false;

  return randNum1 < randNum2;
}  /* JobAcuracyComparison */





class  GradeComparison: public  QueueComparison<BinaryJob>
{
public:
   GradeComparison ():  QueueComparison<BinaryJob> () {} 


   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     return   JobAcuracyComparison (j1->Grade (), j1->Features (), j1->ProcessingTime (), j1->RandomNum (),
                                    j2->Grade (), j2->Features (), j2->ProcessingTime (), j2->RandomNum (),
                                    true  // true = We Prefer small number of featuers given equal accuracy.
                                   );
   }
};  /* GradeComparison */




void   BinaryJobList::SortByGrade ()
{
  GradeComparison  c;
  sort (begin (), end (), c);
}  /* SortByGrade */





class  ProcessingTimeComparison
{
public:
   ProcessingTimeComparison ()
   {} 


   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     if  (j1->ProcessingTime () == j2->ProcessingTime())
       return  (j1->Grade () > j2->Grade ());
     else
       return  (j1->ProcessingTime () < j2->ProcessingTime ());
   }
};  /* GradeComparison */



void   BinaryJobList::SortByProcessingTime ()
{
  ProcessingTimeComparison  c;
  sort (begin (), end (), c);
}  /* SortByProcessingTime */






class  DeltaProbAccuarcyComparison
{
public:
   DeltaProbAccuarcyComparison ()
   {} 


   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     float  delta1 = j1->DeltaProbAccuracy ();
     float  delta2 = j2->DeltaProbAccuracy ();
     if  (delta1 == delta2)
       return  (j1->Accuracy () > j2->Accuracy ());
     else
       return  delta1 < delta2;
   }
};  /* DeltaProbAccuarcyComparison */





void   BinaryJobList::SortDeltaProbAccuarcy ()
{
  DeltaProbAccuarcyComparison  c;
  sort (begin (), end (), c);
}  /* DeltaProbAccuarcyComparison */




class  DeltaTestProbAccuarcyComparison
{
public:
   DeltaTestProbAccuarcyComparison ()
   {} 


   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     float  delta1 = j1->TestDeltaProbAccuracy ();
     float  delta2 = j2->TestDeltaProbAccuracy ();

     if  (delta1 < delta2)
       return true;
     
     else if  (delta1 > delta2) 
       return false;

     else if  (j1->TestAccuracy () > j2->TestAccuracy ())
       return  true;

     else if  (j1->TestAccuracy () < j2->TestAccuracy ())
       return  false;

     else
       return  (j1->TestNumSVs () <j2->TestNumSVs ());
   }
};  /* DeltaProbAccuarcyComparison */





void   BinaryJobList::SortDeltaTestProbAccuarcy ()
{
  DeltaTestProbAccuarcyComparison  c;
  sort (begin (), end (), c);
}  /* DeltaProbAccuarcyComparison */





class  SvmParmsComparison
{
public:
  SvmParmsComparison ()  {}

   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     if  (j1->CParm () != j2->CParm ())
       return  (j1->CParm () < j2->CParm ());

     else if  (j1->GammaParm () != j2->GammaParm ())
       return  (j1->GammaParm () < j2->GammaParm ());

     else if  (j1->AParm () != j2->AParm ())
       return  (j1->AParm () < j2->AParm ());

     else 
       return  (j1->Grade () < j2->Grade ());
   }
};  /* SvmParmsComparison */




void   BinaryJobList::SortBySvmParameters ()
{
  SvmParmsComparison  c;
  sort (begin (),  end (),  c);
}  /* SortBySvmParameters */




class  AccuracyComparison
{
public:
   AccuracyComparison () {}


   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     if  (j1->Accuracy () == j2->Accuracy ())
     {
       if  (j1->Features ().NumOfFeatures () > j2->Features ().NumOfFeatures ())
         return  false;

       else if  (j1->Features ().NumOfFeatures () < j2->Features ().NumOfFeatures ())
         return  true;

       else
         return  (j1->RandomNum () > j2->RandomNum ());
     }
     else
     {
       return (j1->Accuracy () > j2->Accuracy ());
     }
   }
};  /* AccuracyComparison */




void   BinaryJobList::SortByAccuracy ()
{
  AccuracyComparison c;
  sort (begin (),  end (),  c);
}



class  AccuracyNormComparison
{
public:
   AccuracyNormComparison () {}


   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     if  (j1->AccuracyNorm () == j2->AccuracyNorm ())
     {
       if  (j1->Features ().NumOfFeatures () > j2->Features ().NumOfFeatures ())
         return  false;

       else if  (j1->Features ().NumOfFeatures () < j2->Features ().NumOfFeatures ())
         return  true;

       else
         return  (j1->RandomNum () > j2->RandomNum ());
     }
     else
     {
       return (j1->AccuracyNorm () > j2->AccuracyNorm ());
     }
   }
};  /* AccuracyComparison */




void   BinaryJobList::SortByAccuracyNorm ()
{
  AccuracyNormComparison c;
  sort (begin (),  end (),  c);
}




class  TestGradeComparison
{
public:
  TestGradeComparison (bool  _featureCountPrefSmall):
      featureCountPrefSmall (_featureCountPrefSmall)
  {}


   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     if  (j1->ValidateOnly ())
     {
       if  (!(j2->ValidateOnly ()))
         return true;
       else
       {
         return JobAcuracyComparison (j1->TestGrade (), j1->Features (), j1->ProcessingTime (), j1->RandomNum (),
                                      j2->TestGrade (), j2->Features (), j2->ProcessingTime (), j2->RandomNum (),
                                      featureCountPrefSmall
                                     );
       }
     }

     else
     {
       if  (j2->ValidateOnly ())
         return false;
       else
       {
         return JobAcuracyComparison (j1->Grade (), j1->Features (), j1->ProcessingTime (), j1->RandomNum (),
                                      j2->Grade (), j2->Features (), j2->ProcessingTime (), j2->RandomNum (),
                                      featureCountPrefSmall
                                     );
       }
     }
   }

private:
  bool  featureCountPrefSmall;
};  /* TestGradeComparison */



void  BinaryJobList::SortByTestGrade (bool  featureCountPrefSmall)
{
  TestGradeComparison* c = new TestGradeComparison (featureCountPrefSmall);
  sort (begin (),  end (),  *c);
  delete  c;
  c = NULL;
}



class  TestAccuracyComparison
{
public:
  TestAccuracyComparison () {}


   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     if  (j1->ValidateOnly ())
     {
       if  (j2->ValidateOnly ())
       {
         if  (j1->TestAccuracy () != j2->TestAccuracy ())
           return (j1->TestAccuracy () > j2->TestAccuracy ());
       }
       else
       {
         return true;
       }
     }
     else 
     {
       if  (j2->ValidateOnly ())
       {
         return false;
       }
       else
       {
         if  (j1->Accuracy () != j2->Accuracy ())
           return  (j1->Accuracy () > j2->Accuracy ());
       }
     }

     // If we reached this point then both jobs have the 
     // same accuracy so we need to use other factors.

     if  (j1->Features ().NumOfFeatures () != j2->Features ().NumOfFeatures ())
       return (j1->Features ().NumOfFeatures () < j2->Features ().NumOfFeatures ());

     if  (j1->ProcessingTime () != j2->ProcessingTime ())
       return (j1->ProcessingTime () < j2->ProcessingTime ()); 

     return  (j1->RandomNum () > j2->RandomNum ());
   }
};  /* TestAccuracyComparison */




void  BinaryJobList::SortByTestAccuracy ()
{
  TestAccuracyComparison c;
  sort (begin (),  end (),  c);
}





class  TestAccuracyNormComparison
{
public:
  TestAccuracyNormComparison () {}


   bool  operator () (BinaryJobPtr  j1,
                      BinaryJobPtr  j2
                     )
   {
     if  (j1->ValidateOnly ())
     {
       if  (j2->ValidateOnly ())
       {
         if  (j1->TestAccuracyNorm () != j2->TestAccuracyNorm ())
           return (j1->TestAccuracyNorm () > j2->TestAccuracyNorm ());
       }
       else
       {
         return true;
       }
     }
     else 
     {
       if  (j2->ValidateOnly ())
       {
         return false;
       }
       else
       {
         if  (j1->AccuracyNorm () != j2->AccuracyNorm ())
           return  (j1->AccuracyNorm () > j2->AccuracyNorm ());
       }
     }

     // If we reached this point then both jobs have the 
     // same accuracy so we need to use other factors.

     if  (j1->Features ().NumOfFeatures () != j2->Features ().NumOfFeatures ())
       return (j1->Features ().NumOfFeatures () < j2->Features ().NumOfFeatures ());

     if  (j1->ProcessingTime () != j2->ProcessingTime ())
       return (j1->ProcessingTime () < j2->ProcessingTime ()); 

     return  (j1->RandomNum () > j2->RandomNum ());
   }
};  /* TestAccuracyComparison */




void  BinaryJobList::SortByTestAccuracyNorm ()
{
  TestAccuracyNormComparison c;
  sort (begin (),  end (),  c);
}



int   BinaryJobList::SmallestNumberOfFeaturesExpanded ()  const
{
  int  smallestNumberOfFeaturesExpanded = INT_MAX;

  const_iterator idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    const BinaryJobPtr  j = *idx;
    if  (j->Status () == bjExpanded)
      smallestNumberOfFeaturesExpanded = Min (smallestNumberOfFeaturesExpanded, j->Features ().NumSelFeatures ());
  }

  return smallestNumberOfFeaturesExpanded;
}  /* SmallestNumberOfFeaturesExpanded */




FeatureNumList  BinaryJobList::SelectFeaturesByMostFeaturesWithHighestGrade ()  const
{
  FeatureNumList  features (processor->FileDesc ());

  BinaryJobList  jobsWithHighestGrade (processor);
  jobsWithHighestGrade.Owner (false);
  float  highestGradeFound  = 0.0f;
  int    maxNumOfFeatures = 0;

  const_iterator idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    const BinaryJobPtr  j = *idx;
    if  (j->Grade () > highestGradeFound)
    {
      highestGradeFound = j->Grade ();
      jobsWithHighestGrade.DeleteContents ();
      jobsWithHighestGrade.PushOnBack (j);
      maxNumOfFeatures = j->FeatureCount ();
    }

    else if  (j->Grade () == highestGradeFound)
    {
      if  (j->FeatureCount () > maxNumOfFeatures)
      {
        jobsWithHighestGrade.DeleteContents ();
        jobsWithHighestGrade.PushOnBack (j);
        maxNumOfFeatures = j->FeatureCount ();
      }

      else if  (j->FeatureCount () == maxNumOfFeatures)
      {
        jobsWithHighestGrade.PushOnBack (j);
      }
    }
  }

  int x =  (int)(LRand48 () % (long)(jobsWithHighestGrade.QueueSize ()));

  return  jobsWithHighestGrade.IdxToPtr (x)->Features ();
}  /* SelectFeaturesByMostFeaturesWithHighestGrade */





void   BinaryJobList::PushOnBack (BinaryJobPtr  j)
{
  jobIdLookUpTableIdx = jobIdLookUpTable.find (j->JobId ());
  if  (jobIdLookUpTableIdx != jobIdLookUpTable.end ())
  {
    log.Level (-1) << endl
                   << endl
                   << "BinaryJobList::PushOnBack      ***ERROR***" << endl 
                   << endl 
                   << "BinaryJobList::PushOnBack        Duplicate JobId[" << j->JobId () << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  if  (j->JobType () != jtRandomSplit)
  {
    // Non RandomSplit jobs are required to have unique parameters.
    ParmsKey key (j);
    parmsLookUpTableIdx = parmsLookUpTable.find (key);
    if  (parmsLookUpTableIdx != parmsLookUpTable.end ())
    {
      BinaryJobPtr j2 = parmsLookUpTableIdx->second;

      log.Level (-1) << endl << endl
                     << "BinaryJobList::PushOnBack        ***ERROR***     Duplicate Parameters"  << endl
                     << endl  
                     << "        Existing JobId[" << j2->JobId () << "]  C[" << j2->CParm () << "]  Gamma[" << j2->GammaParm () << "]  A[" << j2->AParm () << "]  ValidateOnly[" << (j2->ValidateOnly () ? "Yes" : "No") << "]" << endl
                     << "        New      JobId[" << j->JobId  () << "]  C[" << j->CParm  () << "]  Gamma[" << j->GammaParm  () << "]  A[" << j->AParm  () << "]  ValidateOnly[" << (j->ValidateOnly  () ? "Yes" : "No") << "]" << endl
                     << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }
  }

  KKQueue<BinaryJob>::PushOnBack (j);
  jobIdLookUpTable.insert (JobIdLookUpTablePair (j->JobId (), j));

  if  (j->JobType () != jtRandomSplit)
    parmsLookUpTable.insert (ParmsLookUpTablePair (j, j));

  return;
}  /* PushOnBack */




//  Will open data file and load any new entries in data file that have been added.  Uses
//  lastFilePostion to determine where to read from.
void   BinaryJobList::ReFresh ()
{
  FILE*  in = osFOPEN (fileName.Str (), "r");
  if  (!in)
  {
    log.Level (-1) << endl
                   << endl
                   << "BinaryJobList::ReFresh    ***ERROR***" << endl
                   << endl
                   << "     Error Opening Status File[" << fileName << "]" << endl
                   << endl;
    exit (-1);
  }


  fseek (in, lastFilePostion, SEEK_SET);

  char  buff[10240];

  while  (fgets (buff, sizeof (buff), in))
  {
    if  ((buff[0] == '/')   &&  (buff[1] == '/'))
      continue;

    KKStrParser statusStr (buff);
    BinaryJobPtr j = new BinaryJob (processor, statusStr);
    BinaryJobPtr  existingJob = LookUpByJobId (j->JobId ());

    if  (!existingJob)
    {
      PushOnBack (j);
    }
    else
    {
      existingJob->ReFresh (*j);
      delete  j;
      j = NULL;
    }
  }

  lastFilePostion = ftell (in);

  fclose (in);
}  /* ReFresh */





bool  BinaryJobList::JobsStillRunning ()
{
  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  (j->Status () == bjStarted)
      return true;
  }
  return  false;
}  /* JobsStillRunning */




BinaryJobListPtr  BinaryJobList::ExtractHighestGrade (int  numToExtract)
{
  BinaryJobListPtr  highestGradeJobs = new BinaryJobList (processor);
  highestGradeJobs->Owner (false);

  if  (QueueSize () < 1)
    return highestGradeJobs;

  SortByGrade ();

  BinaryJobList::iterator  idx;
  for  (idx = begin ();  (idx != end ())  &&  (highestGradeJobs->QueueSize () < numToExtract);  idx++)
  {
    BinaryJobPtr  j = *idx;
    highestGradeJobs->PushOnBack (j);
  }

  return  highestGradeJobs;
}  /* ExtractHighestGrade */




BinaryJobListPtr  BinaryJobList::ExtractHighestGrade (float  percentageFromTop)  // Get all jobs who's grade is within 'percentageFromTop' 
                                                                                 // of the highest grade.

{
  BinaryJobListPtr  highestGradeJobs = new BinaryJobList (processor);
  highestGradeJobs->Owner (false);
  if  (QueueSize () < 1)
    return highestGradeJobs;

  SortByGrade ();

  float  highGrade = this->FrontOfQueue ()->Grade ();
  float  lowGradeThreshold = highGrade - percentageFromTop;
 
  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  (j->Grade () <= lowGradeThreshold)
      break;

    highestGradeJobs->PushOnBack (j);
  }

  return  highestGradeJobs;
}  /* ExtractHighestGrade */




BinaryJobListPtr  BinaryJobList::ExtractHighestGrade (float  topPercentage,
                                                      int    minNumOfJobs
                                                     )
{
  BinaryJobListPtr  highestGradeJobs = new BinaryJobList (processor);
  highestGradeJobs->Owner (false);

  if  (QueueSize () < 1)
    return highestGradeJobs;

  SortByGrade ();

  float  highGrade = this->FrontOfQueue ()->Grade ();
  float  lowGradeThreshold = highGrade * (100.0f - topPercentage) / 100.0f;

 
  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  ((j->Grade () <= lowGradeThreshold)  &&  (highestGradeJobs->QueueSize () >= minNumOfJobs))
      break;

    highestGradeJobs->PushOnBack (j);
  }

  return  highestGradeJobs;
}  /* ExtractHighestGrade */





BinaryJobListPtr  BinaryJobList::ExtractLowDeltaProbAccuarcy (float acceptableDelta)
{
  BinaryJobListPtr  lowDeltaProbAccuarcyJobs = new BinaryJobList (processor);
  lowDeltaProbAccuarcyJobs->Owner (false);
  if  (QueueSize () < 1)
    return lowDeltaProbAccuarcyJobs;

  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  (j->DeltaProbAccuracy () <= acceptableDelta)
      lowDeltaProbAccuarcyJobs->PushOnBack (j);
  }

  return  lowDeltaProbAccuarcyJobs;
}  /* ExtractLowDeltaProbAccuarcy */



BinaryJobListPtr  BinaryJobList::ExtractByMinimumProcessintTime (double  maxProcessingTime)
{
  BinaryJobListPtr  jobs = new BinaryJobList (processor);
  jobs->Owner (false);
  if  (QueueSize () < 1)
    return jobs;

  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  (j->ProcessingTime () <= maxProcessingTime)
      jobs->PushOnBack (j);
  }

  return  jobs;
}  /* ExtractByMinimumProcessintTime */



BinaryJobListPtr  BinaryJobList::ExtractHighestTestGrade (float  topPercentage,
                                                          bool   featureCountPrefSmall
                                                         )
{
  BinaryJobListPtr  highestTestGradeJobs = new BinaryJobList (processor);
  highestTestGradeJobs->Owner (false);

  if  (QueueSize () < 1)
    return highestTestGradeJobs;

  SortByTestGrade (featureCountPrefSmall);

  float highGrade = FrontOfQueue ()->TestGrade ();
  float lowGradeThershold = highGrade * (100.0f - topPercentage) / 100.0f;
 
  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  (j->TestGrade () < lowGradeThershold)
      break;

    highestTestGradeJobs->PushOnBack (j);
  }

  return  highestTestGradeJobs;

}  /* ExtractHighestTestGrade */




BinaryJobListPtr  BinaryJobList::LookUpByParameters (double  minC,
                                                     double  maxC,
                                                     double  minGamma,
                                                     double  maxGamma
                                                    )
{
  BinaryJobListPtr  jobs = new BinaryJobList (processor);
  jobs->Owner (false);

  if  (QueueSize () < 1)
    return  jobs;

  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  ((j->CParm () >= minC)  &&  
         (j->CParm () <= maxC)  &&
         (j->GammaParm () >= minGamma)  &&
         (j->GammaParm () <= maxGamma)
        )
    jobs->PushOnBack (j);
  }
  
  return  jobs;
}  /* LookUpByParameters */





BinaryJobListPtr  BinaryJobList::ExtractFastestJobs (double  minTime,     
                                                     float   topPercentage
                                                    )
{
  SortByProcessingTime ();

  double  fastestJob = FrontOfQueue ()->ProcessingTime ();
  double  slowestJob = BackOfQueue ()->ProcessingTime ();
  double  timeSpan = slowestJob - fastestJob;
  double  maxAcceptableTime = fastestJob + timeSpan * topPercentage / 100.0;
  if  (maxAcceptableTime < minTime)
    maxAcceptableTime = minTime;

  BinaryJobListPtr  fastestJobs = ExtractByMinimumProcessintTime (maxAcceptableTime);
  return  fastestJobs;
}  /* ExtractFastestJobs */





BinaryJobListPtr  BinaryJobList::ExtractJobsWithSmallestDeltaAccProbForEachSvmParameterSet ()
{
  BinaryJobListPtr  jobs = new BinaryJobList (processor);
  jobs->Owner (false);

  if  (QueueSize () < 1)
    return  jobs;

  SortBySvmParameters ();
  
  BinaryJobList::iterator  idx = begin ();
  BinaryJobPtr  j = *idx;


  while  (j != NULL)
  {
    double  lastCParm     = j->CParm ();
    double  lastGammaParm = j->GammaParm ();

    BinaryJobPtr  jobWithSmallestDelta = j;

    while  ((j != NULL)                        &&
            (j->CParm     () == lastCParm)     &&
            (j->GammaParm () == lastGammaParm)
           )
    {
      if  (j->DeltaProbAccuracy () < jobWithSmallestDelta->DeltaProbAccuracy ())
        jobWithSmallestDelta = j;

      idx++;
      if  (idx != end ())
        j = *idx;
      else
        j = NULL;
    }

    jobs->PushOnBack (jobWithSmallestDelta);
  }
  
  return  jobs;
}  /* ExtractJobsWithSmallestDeltaAccProbForEachSvmParameterSet */





BinaryJobListPtr  BinaryJobList::ExtractTestJobs ()
{
  BinaryJobListPtr  testJobs = new BinaryJobList (processor);
  testJobs->Owner (false);

  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  (j->TestGrade () > 0.0f)
      testJobs->PushOnBack (j);
  }

  return  testJobs;
}  /* ExtractTestJobs */



void   BinaryJobList::ComputeMinAndMaxParmValues (double&  cMin,  
                                                  double&  cMax,
                                                  double&  gammaMin,
                                                  double&  gammaMax,
                                                  float&   aMin,
                                                  float&   aMax
                                                 )
{
  cMin = DBL_MAX;
  cMax = DBL_MIN;
  gammaMin = 9999.99;
  gammaMax = 0.0;
  aMin     = 9999.0f;
  aMax     = 0.0f;

  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr j = *idx;
    cMax     = Max (j->CParm (), cMax);
    cMin     = Min (j->CParm (), cMin);
    gammaMin = Min (j->GammaParm (), gammaMin);
    gammaMax = Max (j->GammaParm (), gammaMax);
    aMin     = Min (j->AParm     (), aMin);
    aMax     = Max (j->AParm     (), aMax);
  }

  return;
}  /* ComputeMinAndMaxParmValues */



void  BinaryJobList::ReportResults (ostream&  r)
{
  BinaryJobList::iterator  idx;

  r << BinaryJob::ReportHeaderLine1 ()  << endl;
  r << BinaryJob::ReportHeaderLine2 ()  << endl;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    r << j->ReportLine () << endl;
  }
}  /* ReportResults */





void  BinaryJobList::ReportResultsHTML (ostream&  r)
{
  r << "<table align=\"center\" border=\"2\" cellpadding=\"3\" cellspacing=\"0\" frame=\"box\"  summary=\"Feature Impact Table\">"
    << "  <thead valign=\"bottom\">" << endl
    << "     <tr>" << BinaryJob::ReportHeaderLineHTML () << "</tr>" << endl
    << "  </thead" << endl
    << "  <tbody>" << endl;

  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    r << "     <TR>" << j->ReportLineHTML () << "</tr>" << endl;
  }

  r << "  </tbody>" << endl
    << "</table>"   << endl;
}  /* ReportResults */



map<double, int>*  BinaryJobList::GetListOfUniqueCValues ()
{
  map<double, int>* uniqueValues = new map<double, int> ();
  map<double, int>::iterator  idx2;

  BinaryJobList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr j = *idx;

    idx2 = uniqueValues->find (j->CParm ());
    if  (idx2 == uniqueValues->end ())
      uniqueValues->insert (pair <double, int> (j->CParm (), 0));
  }

  int  indexNum = 0;
  for  (idx2 = uniqueValues->begin ();  idx2 != uniqueValues->end ();  idx2++)
  {
    idx2->second = indexNum;
    indexNum++;
  }
  
  return  uniqueValues;
}  /* GetListOfUniqueCValues */



map<double, int>*  BinaryJobList::GetListOfUniqueGammaValues ()
{
  map<double, int>* uniqueValues = new map<double, int> ();
  map<double, int>::iterator  idx2;

  BinaryJobList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr j = *idx;

    idx2 = uniqueValues->find (j->GammaParm ());
    if  (idx2 == uniqueValues->end ())
      uniqueValues->insert (pair <double, int> (j->GammaParm (), 0));
  }

  int  indexNum = 0;
  for  (idx2 = uniqueValues->begin ();  idx2 != uniqueValues->end ();  idx2++)
  {
    idx2->second = indexNum;
    indexNum++;
  }
  
  return  uniqueValues;
} /* GetListOfUniqueGammaValues */



void  BinaryJobList::GenrateSvmResponseSheet (ostream&  r)
{
  map<double, int>*  uniqueCs = GetListOfUniqueCValues ();
  map<double, int>*  uniqueGs = GetListOfUniqueGammaValues ();
  map<double, int>::iterator  idxC;
  map<double, int>::iterator  idxG;

  uint  height = (uint)uniqueCs->size ();
  uint  width  = (uint)uniqueGs->size ();

  uint  row, col;

  Matrix  accuracies (height, width);
  Matrix  times      (height, width);

  BinaryJobList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr j = *idx;

    idxC = uniqueCs->find (j->CParm ());
    if  (idxC == uniqueCs->end ())
      continue;

    idxG = uniqueGs->find (j->GammaParm ());
    if  (idxG == uniqueGs->end ())
      continue;

    row = idxC->second;
    col = idxG->second;

    accuracies[row][col] = j->AccuracyNorm ();
    times[row][col]      = j->ProcessingTime ();
  }


  r << endl
    << "Normalized Accuracy Response" << endl << endl;

  r << "C-Param";
  for  (idxG = uniqueGs->begin ();  idxG != uniqueGs->end ();  idxG++)
    r << "\t" << idxG->first;
  r << endl;

  row = 0;
  for  (idxC = uniqueCs->begin ();  idxC != uniqueCs->end ();  idxC++)
  {
    r << idxC->first;
    for (col = 0;  col < width;  col++)
    {
      r << "\t";
      if  (accuracies[row][col] != 0.0)
        r << accuracies[row][col];
    }
    r << endl;
    row++;
  }
  r << endl;


  r << endl
    << endl
    << "Processing Time Response" << endl 
    << endl;

  r << "C-Param";
  for  (idxG = uniqueGs->begin ();  idxG != uniqueGs->end ();  idxG++)
  {
    r << "\t" << idxG->first;
  }
  r << endl;

  row = 0;
  for  (idxC = uniqueCs->begin ();  idxC != uniqueCs->end ();  idxC++)
  {
    r << idxC->first;
    for (col = 0;  col < width;  col++)
    {
      r << "\t";
      if  (times[row][col] != 0.0)
        r << times[row][col];
    }
    r << endl;
    row++;
  }
  r << endl
    << endl;

} /* GenrateSvmResponseSheet */






BinaryJobListPtr  BinaryJobList::ExtractJobsForAGivenFeatureCount (int featureCount)
{
  BinaryJobListPtr  jobsWithFeatureCount = new BinaryJobList (processor);
  jobsWithFeatureCount->Owner (false);

  BinaryJobList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  (j->Features ().NumOfFeatures () == featureCount)
      jobsWithFeatureCount->PushOnBack (j);
  }

  return  jobsWithFeatureCount;
}  /* ExtractJobsForAGivenFeatureCount*/




BinaryJobListPtr  BinaryJobList::CreateTestJobsForHighetsGradePerFeatureCount ()
{
  BinaryJobListPtr  testJobs = new BinaryJobList (processor);
  testJobs->Owner (true);

  int  maxNumOfFeatures = processor->InitialFeatures ().NumOfFeatures ();
  int  featureCount = 0;
  for  (featureCount = 1;  featureCount <= maxNumOfFeatures;  featureCount++)
  {
    BinaryJobListPtr  jobsForThisFeatureCount = ExtractJobsForAGivenFeatureCount (featureCount);
    if  (!jobsForThisFeatureCount)
      continue;

    if  (jobsForThisFeatureCount->QueueSize () > 0)
    {
      jobsForThisFeatureCount->SortByGrade ();
      float  highGrade = jobsForThisFeatureCount->FrontOfQueue ()->Grade ();
      int    numCreatedForThisFeatureCount = 0;

      BinaryJobList::iterator  idx = jobsForThisFeatureCount->begin ();
      while  ((idx != jobsForThisFeatureCount->end ())  &&  (numCreatedForThisFeatureCount < 3))
      {
        BinaryJobPtr  j = *idx;
        if  (j->Grade () < highGrade)
          break;

        BinaryJobPtr  existingJob = this->LookUpByParameters (j->Features    (),
                                                              j->NumOfRounds (),
                                                              j->CParm       (),
                                                              j->GammaParm   (),
                                                              j->AParm       (),
                                                              true
                                                             );
        if  (!existingJob)
        {
          existingJob = testJobs->LookUpByParameters (j->Features    (),
                                                      j->NumOfRounds (),
                                                      j->CParm       (),
                                                      j->GammaParm   (),
                                                      j->AParm       (),
                                                      true
                                                     );
        }

        if  (!existingJob)
        {
          BinaryJobPtr testJob = BinaryJob::CreateTestJob (j);
          testJobs->PushOnBack (testJob);
          numCreatedForThisFeatureCount++;
        }

        idx++;
      }
    }

    delete  jobsForThisFeatureCount;  jobsForThisFeatureCount = NULL;
  }

  return  testJobs;
}  /* CreateTestJobsForHighetsGradePerFeatureCount */




void  BinaryJobList::FindNeighboringParameterValues (BinaryJobPtr  targetJob,
                                                     double&       cMin,
                                                     double&       cMax,
                                                     float         cMaxGrowthFactor,
                                                     double&       gammaMin,
                                                     double&       gammaMax,
                                                     float         gammaMaxGrowthFactor,
                                                     float&        aMin,
                                                     float&        aMax,
                                                     float         aMaxGrowthFactor
                                                    )
{
  double  targetC     = targetJob->CParm     ();
  double  targetGamma = targetJob->GammaParm ();
  float   targetA     = targetJob->AParm     ();
   
  cMin = -1;
  cMax = 999999;

  gammaMin = -1.0;
  gammaMax = 999999.9;

  aMin = -1.0f;
  aMax = 99999.9f;

  BinaryJobList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    if  ((j->CParm () == targetC)  &&  (j->AParm () == targetA))
    {
      if  (j->GammaParm () < targetGamma)
      {
        if  (j->GammaParm () > gammaMin)
          gammaMin = j->GammaParm ();
      }
      else if  (j->GammaParm () > targetGamma)
      {
        if  (j->GammaParm () < gammaMax)
          gammaMax = j->GammaParm ();
      }
    }

    if  ((j->GammaParm () == targetGamma)  &&  (j->AParm () == targetA))
    {
      if  (j->CParm () < targetC)
      {
        if  (j->CParm () > cMin)
          cMin = j->CParm ();
      }
      else if  (j->CParm () > targetC)
      {
        if  (j->CParm () < cMax)
          cMax = j->CParm ();
      }
    }

    if  ((j->GammaParm () == targetGamma)  &&  (j->CParm () == targetC))
    {
      if  (j->AParm () < targetA)
      {
        if  (j->AParm () > aMin)
          aMin = j->AParm ();
      }
      else if  (j->AParm () > targetA)
      {
        if  (j->AParm () < aMax)
          aMax = j->AParm ();
      }
    }
  }

  cMin = cMin + 1;
  cMax = cMax - 1;

  if  (cMin > cMax)
    cMin = cMax = targetC;

  float largestMaxAAllowable  = targetA * aMaxGrowthFactor;
  float smallestMinAAllowable = targetA / aMaxGrowthFactor;

  double  largestMaxCAllowable  = targetC * cMaxGrowthFactor;
  double  smallestMinCAllowable = targetC / cMaxGrowthFactor;
  double  largestMaxGammaAllowable  = targetGamma * gammaMaxGrowthFactor;
  double  smallestMinGammaAllowable = targetGamma / gammaMaxGrowthFactor;

  aMax     = Min (aMax,     largestMaxAAllowable);
  aMin     = Max (aMin,     smallestMinAAllowable);
  cMax     = Min (cMax,     largestMaxCAllowable);
  cMin     = Max (cMin,     smallestMinCAllowable);
  gammaMax = Min (gammaMax, largestMaxGammaAllowable);
  gammaMin = Max (gammaMin, smallestMinGammaAllowable);

}  /* FindNeighboringCandGammaValues */



double   BinaryJobList::TotalProcessingTime ()  const  // returns the total processing time from all jobs in the list.
{
  BinaryJobList::const_iterator  idx;

  double  totalProcessingTime = 0.0;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    const BinaryJobPtr  j = *idx;
    totalProcessingTime += j->ProcessingTime ();
  }

  return   totalProcessingTime;
}  /* TotalProcessingTime */
