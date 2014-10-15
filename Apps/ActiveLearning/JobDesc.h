#ifndef  _JOBDESC_
#define  _JOBDESC_

#include  "KKQueue.h"
#include  "Str.h"
#include  "SortOrderType.h"


#ifndef  _MLCLASS_
namespace MLL
{
  class  MLClass;
  typedef  MLClass*  MLClassPtr;

  class  MLClassList;
  typedef  MLClassList*  MLClassListPtr;
}
#endif



class  JobDesc  
{
public:
  JobDesc (JobDesc&  j); 

  /** @brief  Create Finale Report Job */
  JobDesc ();

  /**  @brief  Creates a Job from a Input KKStr. */
  JobDesc (MLL::MLClassList&  imageClasses,
           KKStr                 l
          );

  JobDesc (SortOrderType  _sortOrder,
           int            _pass,
           int            _initialTrainingImagesPerClass,
           int            _newImagesPerRetraining
          );                                // Creates a Combined Classes Job

  int                 JobID                         () const  {return  jobID;}
  char                CurStatus                     () const  {return  curStatus;}
  int                 InitialTrainingImagesPerClass () const  {return  initialTrainingImagesPerClass;}
  char                JobType                       () const  {return  jobType;}
  MLL::MLClassPtr  LeftClass                     () const  {return  leftClass;}
  int                 NewImagesPerRetraining        () const  {return  newImagesPerRetraining;}
  MLL::MLClassPtr  RightClass                    () const  {return  rightClass;}
  int                 Pass                          () const  {return  pass;}
  SortOrderType       SortOrder                     () const  {return  sortOrder;}
   
  void  CurStatus (char _curStatus)  {curStatus = _curStatus;}


  KKStr  ToString ();

  static  void  InitializeNextJobID ();

private:
  int     jobID;
  char    curStatus;   /**< '0' = Not Started
                        *   '1' = Started
                        *   '2' = Done
                        */

  int     initialTrainingImagesPerClass;

  char    jobType;     /**< '2' = Two Class
                        *   'C' = Combined
                        *   'F' = Final Report
                        */

  MLL::MLClassPtr   leftClass;


  int                  newImagesPerRetraining;

  MLL::MLClassPtr   rightClass;

  int                  pass;

  static int           nextJobID;


  SortOrderType        sortOrder;

};



typedef  JobDesc*  JobDescPtr;



class  JobDescList:  public KKQueue<JobDesc>
{
public:
  JobDescList (bool owner = true);
  ~JobDescList ();
};

typedef  JobDescList*   JobDescListPtr;


#endif
