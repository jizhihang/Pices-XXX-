//*****************************************************************************
//*                           SingleRunTree                                   *
//*                                                                           *
//*  Started to make thois object to help BeamSearch runn quicker,  Still     *
//*  have some worlk to do.                                                   *
//*                                                                           *
//*                                                                           *
//*---------------------------------------------------------------------------*
//*                                                                           *
//*     DATE     Programmer   Description                                     *
//*  ----------  ------------ ----------------------------------------------- *
//*  2005-01-09  Kurt Kramer  Intial development                              *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*****************************************************************************


#include  <string>
#include  <iomanip>
#include  <iostream>
#include  <fstream>
#include  <vector>


#ifdef  WIN32
#include  <ostream>
#endif


using namespace std;

#include  "OSservices.h"
#include  "SingleRunTree.h"



#ifdef  WIN32
  #ifdef _DEBUG
  #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
  #endif
#endif









class  CompareSingleRunJobID
{
public:
  int  CompareSingleRun (SingleRunPtr  n1,  
                         SingleRunPtr  n2
                        )
          {
            if  (n1->JobId () < n2->JobId ())
              return -1;

            else if  (n1->JobId () > n2->JobId ())
              return  1;

            else
              return  0;
          }


  const int&  ExtractKey  (SingleRunPtr  singleRun)
           {
             return  singleRun->JobId ();
           }
};  /* CompareSingleRunJobID */







class  CompareSingleRunFeatureNums
{
public:
  int  CompareSingleRunFeatureNums (SingleRunPtr  n1,  
                                    SingleRunPtr  n2
                                   )
          {
            return n1->FeatureNums ()->Compare (*(n2->FeatureNums ()));
          }


  const FeatureNumList&  ExtractKey  (SingleRunPtr  singleRun)
           {
             return  *(singleRun->FeatureNums ());
           }
};  /* CompareSingleRunFeatureNums */





CompareSingleRun  compSingleRun;



SingleRunTree::SingleRunTree (bool  _owner):
    RBTree<SingleRun, CompareSingleRun, int> (compSingleRun, _owner)
{
}



SingleRunTree::~SingleRunTree ()
{
}




void  SingleRunTree::AddSingleRun (SingleRunPtr  singleRun)
{
  RBInsert (singleRun);
}



SingleRunListIterator::SingleRunListIterator (SingleRunTree&  runs):
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






SingleRunPtr  SingleRunTree::LookUpRun (String  featuresIncluded)
{
  int           idx;
  int           qSize = QueueSize ();
  SingleRunPtr  run = NULL;
  SingleRunPtr  tempRun;


  for  (idx = 0; ((idx < qSize) && (!run)); idx++)
  {
    tempRun = IdxToPtr (idx);
    if  (featuresIncluded == tempRun->FeaturesIncluded ())   
       run = tempRun;
  }

  return  run;
}



SingleRunPtr  SingleRunTree::NextUnStartedJob ()
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



SingleRunListPtr  SingleRunTree::GetJobsForNumOfFeatures (int numOfFeatures)
{
  SingleRunListPtr result = new SingleRunTree (false);

  if  (!result)
  {
    cerr << endl;
    cerr << "SingleRunTree  **** ERROR ****" << endl;
    cerr << "               Could not Allocate SingleRunTree Object." << endl;
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
      result->AddSingleRun (srIDX.CurPtr ());
    }
  }

  return  result;  
}  /* GetJobsForNumOfFeatures */




