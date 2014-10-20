#include "FirstIncludes.h"

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "MemoryDebug.h"
using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
using namespace  KKU;

#include "SipperVariables.h"
using namespace  SipperHardware;;


#include "TrainingClass.h"
#include "MLClass.h"
#include "MLClassConstList.h"
using namespace  MLL;



TrainingClass::TrainingClass (KKStr                 _directory,
                              KKStr                 _name,
                              KKStr                 _foreground,
                              float                 _weight,
                              MLClassConstList&  mlClasses
                             ):
                directory       (_directory),
                featureFileName (_name),
                foreground      (_foreground),
                weight          (_weight),
                mlClass      (NULL)
{
  featureFileName << ".data";   // Will be equal to ClassName + ".data".
                                // ex:  "Copepods.data"

  mlClass = mlClasses.GetMLClassPtr (_name);
}



TrainingClass::TrainingClass (const TrainingClass&  tc): 
    directory       (tc.directory),
    featureFileName (tc.featureFileName),
    foreground      (tc.foreground),
    mlClass      (tc.mlClass),
    weight          (tc.weight)
{
}




const 
KKStr&    TrainingClass::Name () const
{
  return  mlClass->Name ();
}



KKStr   TrainingClass::ExpandedDirectory (const KKStr&  rootDir)  
{
  KKStr  rootDirWithSlash = "";
  if  (!rootDir.Empty ())
    rootDirWithSlash = osAddSlash (rootDir);

  if  (directory.Empty ())
    return directory = rootDirWithSlash + mlClass->Name ();

  else if  (directory.LocateStr (rootDirWithSlash) == 0)
    return  directory;

  else
    return rootDirWithSlash + SipperVariables::SubstituteInEvironmentVariables (directory);
}  /* ExpandedDirectory */




TrainingClassList::TrainingClassList (const KKStr&  _rootDir,
                                      bool          owner,
                                      int32           initSize
                                     ):
             KKQueue<TrainingClass> (owner, initSize),
             rootDir                (_rootDir)
{
}



TrainingClassList::TrainingClassList (const TrainingClassList&  tcl):
           KKQueue<TrainingClass> (tcl),
           rootDir                (tcl.rootDir)
{
}



TrainingClassList::TrainingClassList (const TrainingClassList&  tcl,
                                      bool                      _owner
                                     ):
           KKQueue<TrainingClass> (tcl, _owner),
           rootDir                (tcl.rootDir)
{
}







void   TrainingClassList::AddTrainingClass (TrainingClassPtr  trainingClass)
{
   PushOnBack (trainingClass);
}



TrainingClassPtr  TrainingClassList::LocateByMLClass (MLClassConstPtr  _mlClass)  const
{
  TrainingClassList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    TrainingClassPtr  tc = *idx;
    if  (tc->MLClass () == _mlClass)
      return tc;
  }
  return  NULL;
}  /* LocateByMLClass */



TrainingClassPtr  TrainingClassList::LocateByMLClassName (const KKStr&  className)
{
  int32  size = QueueSize ();

  int32  idx;

  TrainingClassPtr  traningClass = NULL;

  for  (idx = 0; ((idx < size)  &&  (!traningClass)); idx++)
  {
    TrainingClassPtr  temp = IdxToPtr (idx);

    if  (temp->Name () == className)
        traningClass = temp;
  }

  return  traningClass;
}  /* LocateByMLClassName */




TrainingClassPtr  TrainingClassList::LocateByDirectory (const KKStr&  directory)
{
  iterator  idx;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    if  ((*idx)->ExpandedDirectory (rootDir).EqualIgnoreCase (directory))
      return  *idx;
  }

  return  NULL;
}  /* LocateByDirectory */



TrainingClassListPtr   TrainingClassList::DuplicateListAndContents ()  const
{
  TrainingClassListPtr dupList = new TrainingClassList (rootDir, true, QueueSize ());

  for  (const_iterator  idx = begin ();  idx != end ();  idx++)
  {
    const TrainingClassPtr  tc = *idx;
    dupList->PushOnBack (new TrainingClass (*tc));
  }

  return  dupList;
}  /* TrainingClassListPtr */
