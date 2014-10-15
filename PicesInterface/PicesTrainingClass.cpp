#include "StdAfx.h"
#include "FirstIncludes.h"

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
using namespace std;


#include "MemoryDebug.h"

#include  "BasicTypes.h"
#include  "..\\BaseLibrary\\GoalKeeper.h"

#include "OSservices.h"
using namespace  KKU;

#include "TrainingClass.h"
using namespace  MLL;

#include "PicesTrainingClass.h"
#include "PicesClassList.h"
#include "PicesKKStr.h"
#include "PicesRunLog.h"
using namespace  PicesInterface;


using namespace System;
using namespace System::Threading;
using namespace System::Windows::Forms;



PicesTrainingClass::PicesTrainingClass (PicesClass^  _class,
                                        String^      _directory
                                       )
{
  MLClassConstList  classes;

  trainingClass = new TrainingClass (PicesKKStr::SystemStringToKKStr (_directory), 
                                     PicesKKStr::SystemStringToKKStr (_class->Name),
                                     "W",
                                     1.0f,
                                     classes
                                    );
}


PicesTrainingClass::PicesTrainingClass (TrainingClassPtr  _trainingClass):
  trainingClass (_trainingClass)
{
}




TrainingClassPtr  PicesTrainingClass::UnManagedClass ()
{
  if  (trainingClass)
    return trainingClass;
  return  NULL;
}




String^  PicesTrainingClass::Directory::get ()
{
  if (trainingClass == NULL)
    return  gcnew String ("");

  return  PicesKKStr::KKStrToSystenStr (trainingClass->Directory ());
}




void  PicesTrainingClass::Directory::set (String^  _directory)
{
  trainingClass->Directory (PicesKKStr::SystemStringToKKStr (_directory));
}



PicesClass^  PicesTrainingClass::MLClass::get ()
{
  return   PicesClassList::GetUniqueClass (trainingClass->MLClass ());
}


void  PicesTrainingClass::MLClass::set (PicesClass^  _mlClass)
{
  trainingClass->MLClass (_mlClass->UnmanagedMLClass());
}



String^   PicesTrainingClass::ExpandedDirectory (String^  rootDir)
{
  if  (rootDir == nullptr)
    rootDir = String::Empty;
  return  PicesKKStr::KKStrToSystenStr (trainingClass->ExpandedDirectory (PicesKKStr::SystemStringToKKStr (rootDir)));
}  /* ExpandedDirectory */




PicesTrainingClassList::PicesTrainingClassList ()
{
}



///<summary>
/// Will take ownership of the contents of 'trainingClasses'.  If 'trainingClasses'
/// does not own its its own contents; will then create new instances of 'TrainingClass' 
/// objects.
///</summary>
PicesTrainingClassList::PicesTrainingClassList (TrainingClassListPtr  trainingClasses)
{
  TrainingClassList::iterator  idx;
  for  (idx = trainingClasses->begin ();  idx != trainingClasses->end ();  idx++)
  {
    TrainingClassPtr  tc = *idx;
    if  (!trainingClasses->Owner ())
      tc = new TrainingClass (*tc);
    Add (gcnew PicesTrainingClass (tc));
  }

  trainingClasses->Owner (false);
}
