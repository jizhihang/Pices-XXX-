#include "FirstIncludes.h"

#include <stdlib.h>
#include <memory>
#include <sys/types.h>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "MemoryDebug.h"
#include "BasicTypes.h"

using namespace std;
using namespace KKU;



#include "JobDesc.h"

#include "MLClass.h"
using namespace MLL;




JobDesc::JobDesc (JobDesc&  j):
  jobID                  (j.jobID),
  curStatus              (j.curStatus),
  jobType                (j.jobType),
  leftClass              (j.leftClass),
  rightClass             (j.rightClass),
  pass                   (j.pass),
  sortOrder              (j.sortOrder),
  newImagesPerRetraining (j.newImagesPerRetraining),
  initialTrainingImagesPerClass (j.initialTrainingImagesPerClass)
{
}




JobDesc::JobDesc (MLClassList&  mlClasses,
                  KKStr           l
                 ):
  jobID      (-1),
  curStatus  ('0'),
  jobType    ('2'),
  leftClass  (NULL),
  rightClass (NULL),
  pass       (-1),
  sortOrder  (NULL_SortOrder),
  newImagesPerRetraining (1),
  initialTrainingImagesPerClass (-1)
{
  jobID = l.ExtractTokenInt (" ,\t\n\r");

  {
    KKStr  statusStr = l.ExtractToken (",\t\n\r");
    curStatus = statusStr[0];
  }

  {
    KKStr  jobTypeStr = l.ExtractToken (",\t\n\r");
    jobType = jobTypeStr[0];
  }

  if  ((jobType == '2')  ||  (jobType == 'C'))
  {
    KKStr  sortOrderStr = l.ExtractToken (",\t\n\r");
    sortOrder = SortOrderTypeFromStr (sortOrderStr);
  }

  if  (jobType == '2')
  {
    KKStr  leftClassName = l.ExtractToken (",\t\n\r");
    leftClass = mlClasses.GetMLClassPtr (leftClassName);

    KKStr  rightClassName = l.ExtractToken (",\t\n\r");
    rightClass = mlClasses.GetMLClassPtr (rightClassName);
  }

  else if  (jobType == 'C')
  {
    pass =  l.ExtractTokenInt (" ,\t\n\r");
    initialTrainingImagesPerClass = l.ExtractTokenInt (" ,\t\n\r");
    newImagesPerRetraining =  l.ExtractTokenInt (" ,\t\n\r");
  }
}







JobDesc::JobDesc (SortOrderType  _sortOrder,
                  int            _pass,
                  int            _initialTrainingImagesPerClass,
                  int            _newImagesPerRetraining
                 ):
  jobID                  (-1),
  curStatus              ('0'),
  jobType                ('C'),
  leftClass              (NULL),
  rightClass             (NULL),
  pass                   (_pass),
  sortOrder              (_sortOrder),
  newImagesPerRetraining (_newImagesPerRetraining),
  initialTrainingImagesPerClass (_initialTrainingImagesPerClass)
{
  jobID = nextJobID;
  nextJobID++;
}



JobDesc::JobDesc ():
  jobID                  (-1),
  curStatus              ('0'),
  jobType                ('F'),
  leftClass              (NULL),
  rightClass             (NULL),
  pass                   (-1),
  sortOrder              (NULL_SortOrder),
  newImagesPerRetraining (1),
  initialTrainingImagesPerClass (-1)
{
  jobID = nextJobID;
  nextJobID++;
}




KKStr  JobDesc::ToString ()
{
  KKStr  s (200);

  s << jobID << "\t";
  s.Append (curStatus);
  s << "\t";
  s.Append (jobType);
  s << "\t";

  switch  (jobType)
  {
    case '2': s << SortOrderDescr (sortOrder)  << "\t"
                << leftClass->Name ()          << "\t"
                << rightClass->Name ();
              break;                  

    case 'C': s << SortOrderDescr (sortOrder)     << "\t"
                << pass                           << "\t"
                << initialTrainingImagesPerClass  << "\t"
                << newImagesPerRetraining;
              break;

    case 'F': break;

     default: break;

  }

  return  s;
}  /* ToString */




void  JobDesc::InitializeNextJobID ()
{
  nextJobID = 0;
}


int   JobDesc::nextJobID = 0;




JobDescList::JobDescList (bool owner):
  KKQueue<JobDesc> (owner)
{
}



JobDescList::~JobDescList ()
{
}
