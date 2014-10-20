#include "FirstIncludes.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace  KKU;


#include "MLClass.h"
#include "MLClassConstList.h"

#include "FileDesc.h"
using namespace  MLL;



MLClassListPtr  MLClass::existingMLClasses = NULL;
map<MLClassListPtr,MLClassListPtr>  MLClass::existingClassLists;

GoalKeeperPtr    MLClass::blocker = NULL;
bool             MLClass::needToRunFinalCleanUp = false;


// Will instantiate an instance of "GoalKeeper" if "blocker" does not already
// point one.
void  MLClass::CreateBlocker ()
{
  if  (!blocker)
  {
    GoalKeeper::Create ("GlobalImageClass", blocker);  // Will handle Race condition.
    blocker->StartBlock ();
    if  (!needToRunFinalCleanUp)
    {
      needToRunFinalCleanUp = true;
      atexit (MLClass::FinalCleanUp);
    }
    blocker->EndBlock ();
  }
}



MLClassListPtr  MLClass::GlobalClassList ()
{
  if  (!existingMLClasses)
  {
    CreateBlocker ();
    blocker->StartBlock ();
    if  (!existingMLClasses)
    {
      existingMLClasses = new MLClassList ();
      existingMLClasses->Owner (true);
    }
    blocker->EndBlock ();
  }
  return  existingMLClasses;
}  /* GlobalClassList */



/** @brief  Called by MLClassList constructors to add to list of existing MLClassList instances. */
void  MLClass::AddImageClassList (MLClassListPtr  list)
{
  CreateBlocker ();
  blocker->StartBlock ();
  existingClassLists.insert (pair<MLClassListPtr,MLClassListPtr> (list, list));
  blocker->EndBlock ();
}



/** @brief  Called by MLClassList destructor to remove from list of existing MLClassList instances. */
void  MLClass::DeleteImageClassList (MLClassListPtr  list)
{
  blocker->StartBlock ();

  map<MLClassListPtr,MLClassListPtr>::iterator idx;
  idx = existingClassLists.find (list);
  if  (idx != existingClassLists.end ())
  {
    existingClassLists.erase (idx);
  }
  else
  {
    cerr << endl << endl << "MLClass::DeleteImageClassList   MLClassList instance was not found." << endl << endl;
  }

  blocker->EndBlock ();
}




MLClassPtr  MLClass::CreateNewMLClass (const KKStr&  _name,
                                                int32         _classId
                                               )
{
  KKStr  upperName = _name.ToUpper ();

  MLClassListPtr  globalList = existingMLClasses;
  if  (!globalList)
    globalList = GlobalClassList ();

  MLClassPtr  mlClass = globalList->LookUpByName (_name);
  if  (mlClass == NULL)
  {
    if  (!blocker)
      CreateBlocker ();
    blocker->StartBlock ();

    mlClass = globalList->LookUpByName (_name);
    if  (mlClass == NULL)
    {
      MLClassPtr  temp = new MLClass (_name);
      temp->ClassId (_classId);
      existingMLClasses->AddMLClass (temp);
      mlClass = temp;
    }

    blocker->EndBlock ();
  }

  if  ((mlClass->ClassId () < 0)  &&  (_classId >= 0))
    ((MLClassPtr)mlClass)->ClassId (_classId);

  return  mlClass;
} /* CreateNewImagClass */



MLClassConstPtr  MLClass::GetByClassId (int32  _classId)
{
  return  GlobalClassList ()->LookUpByClassId (_classId);
}  /* GetByClassId */




MLClassConstListPtr  MLClass::BuildListOfDecendents (MLClassConstPtr  parent)
{
  if  (parent == NULL)
    return NULL;

  GlobalClassList ();  // Make sure that 'existingMLClasses'  exists.

  MLClassConstListPtr  results = new MLClassConstList ();
  results->PushOnBack (parent);

  MLClassConstPtr  startingAncestor = NULL;

  MLClassList::const_iterator  idx;

  for  (idx = existingMLClasses->begin ();  idx != existingMLClasses->end ();  ++idx)
  {
    MLClassConstPtr ancestor = *idx;
    startingAncestor = *idx;
    ancestor = startingAncestor->Parent ();
    while  (ancestor != NULL)
    {
      if  (ancestor == parent)
      {
        results->PushOnBack (startingAncestor);
        break;
      }

      ancestor = ancestor->Parent ();
      if  (ancestor == startingAncestor)
        break;
    }
  }

  return  results;
}  /* BuildListOfDecendents */



void  MLClass::ChangeNameOfClass (MLClassPtr  mlClass, 
                                     const KKStr&   newName,
                                     bool&          successful
                                    )
{
  successful = true;
  KKStr  oldName = mlClass->Name();

  if  (!blocker)
    CreateBlocker ();
  blocker->StartBlock ();

  existingMLClasses->ChangeNameOfClass (mlClass, oldName, newName, successful);
  if  (!successful)
  {
    cerr << endl 
         << "MLClass::ChangeNameOfClass   'existingMLClasses'  failed to vahne name." << endl
         << "                        NewName[" << newName << "]" << endl
         << "                        OldName[" << mlClass->Name () << "]" << endl
         << endl;
    successful = false;
  }
  else
  {
    // It is okay to change the name;  we will also need to inform all 
    // instances of MLClassList that the name changed so they can 
    // update their nameIndex structures.
    mlClass->Name (newName);

    map<MLClassListPtr,MLClassListPtr>::iterator  idx;
    for  (idx = existingClassLists.begin ();  idx != existingClassLists.end ();  ++idx)
    {
      MLClassListPtr  list = idx->first;
      uint32  classInList = list->PtrToIdx (mlClass);
      if  (classInList >= 0)
      {
        bool  nameChangedInList = false;
        list->ChangeNameOfClass (mlClass, oldName, newName, nameChangedInList);
      }
    }
  }

  blocker->EndBlock ();

  return;
}  /* ChangeNameOfClass */


void  MLClass::ResetAllParentsToAllClasses ()
{
  CreateBlocker ();
  blocker->StartBlock ();
  
  MLClassListPtr  globalClassList = GlobalClassList ();

  MLClassConstPtr  allClasses = globalClassList->LookUpByName ("AllClasses");

  MLClassList::iterator  idx;
  for  (idx = globalClassList->begin ();  idx != globalClassList->end ();  ++idx)
  {
    MLClassPtr  ic = *idx;
    if  (ic == allClasses)
      ic->Parent (NULL);
    else
      ic->Parent (allClasses);
  }
  blocker->EndBlock ();
}  /* ResetAllParentsToNull */



void  MLClass::Name (const KKStr&  _name)
{
  name = _name;
  upperName = name.ToUpper ();
}



MLClassConstPtr  MLClass::GetUnKnownClassStatic ()
{
  return  CreateNewMLClass ("UNKNOWN");
}  /* GetUnKnownClassStatic */



/** @brief  Call this at very end of program to clean up existingMLClasses. */
void  MLClass::FinalCleanUp ()
{
  if  (!needToRunFinalCleanUp)
    return;

  blocker->StartBlock ();
  if  (needToRunFinalCleanUp)
  {
    if  (!FileDesc::FinalCleanUpRanAlready ())
    {
      //cerr << endl << "MLClass::FinalCleanUp   ***ERROR***   Need to run MLClass::FinalCleanUp  before  FileDesc::FinalCleanUp" << endl << endl;
      FileDesc::FinalCleanUp ();
    }

    if  (existingMLClasses)
    {
      delete  existingMLClasses;
      existingMLClasses = NULL;
    }

    needToRunFinalCleanUp = false;
  }
  blocker->EndBlock ();

  delete  blocker;
  blocker = NULL;
}  /* FinalCleanUp */




MLClass::MLClass (const KKStr&  _name):
    classId          (-1),
    description      (),
    name             (_name),
    parent           (NULL),
    storedOnDataBase (false)

{
  if  (name.Empty ())
  {
    cerr << endl
         << "MLClass::MLClass   ***ERROR***   Empty Name" << endl
         << endl;
  }

  upperName = name.ToUpper ();
  KKStr  topLevel = upperName;
  uint32 x = upperName.LocateCharacter ('_');
  if  (x >= 0)
    topLevel = upperName.SubStrPart (0, x - 1);

  unDefined = upperName.Empty ()           ||  
             (upperName == "UNKNOWN")      ||  
             (upperName == "UNDEFINED")    ||
             (upperName == "NONPLANKTON")  ||
             (topLevel  == "NOISE");
}





MLClass::MLClass (const MLClass&  mlClass)
{
  cerr << endl
       << "MLClass::MLClass (const MLClass&  mlClass)            *** ERROR ***" << endl
       << "Should never ever call this method." << endl
       << endl;
  osWaitForEnter ();
  exit (-1);
}




MLClass::~MLClass ()
{
}



const KKStr&  MLClass::ParentName () const
{
  if  (parent)
    return  parent->Name ();
  return  KKStr::EmptyStr ();
}




KKStr  MLClass::ToString ()  const
{
  KKStr  str (name);
  return str;
}




void  MLClass::ProcessRawData (KKStr&  _data)
{
  name = _data.ExtractToken (" ,\n\t\r");
  name.TrimRight ();
  name.TrimLeft ();

  upperName = name;
  upperName.Upper ();

  unDefined = upperName.Empty ()           ||  
             (upperName == "UNKNOWN")      ||  
             (upperName == "UNDEFINED")    ||  
             (upperName == "NOISE")        ||
             (upperName == "NONPLANKTON");
} /* ProcessRawData */



KKStr  MLClass::GetClassNameFromDirName (const KKStr&  subDir)
{
  KKStr  className = osGetRootNameOfDirectory (subDir);
  int32 x = className.LocateLastOccurrence ('_');
  if  (x > 0)
  {
    // Now lets eliminate any sequence number in name
    // We are assuming that a underscore{"_") character separates the class name from the sequence number.
    // So if there is an underscore character,  and all the characters to the right of it are
    // numeric characters,  then we will remove the underscore and the following numbers.

    int32  y = x + 1;

    bool  allFollowingCharsAreNumeric = true;
    while  ((y < className.Len ()) &&  (allFollowingCharsAreNumeric))
    {
      char  ch = className[y];
      allFollowingCharsAreNumeric = ((ch >= '0')  &&  (ch <= '9'));
      y++;
    }

    if  (allFollowingCharsAreNumeric)
    {
      className = className.SubStrPart (0, x - 1);
    }
  }

  return  className;
}  /* GetClassNameFromDirName */




bool  MLClass::IsAnAncestor (MLClassConstPtr  c)   const
{
  if  (c == this)
    return true;

  if  (parent == NULL)
    return false;

  return  parent->IsAnAncestor (c);
}




uint16   MLClass::NumHierarchialLevels ()  const
{
  return  (uint16)name.InstancesOfChar ('_') + 1;
}




void  MLClass::WriteXML (ostream& o)  const
{
  o << "<MLClass "
    <<   "Name="             << name.QuotedStr ()                             << ","
    <<   "ClassId="          << classId                                       << ","
    <<   "UnDefined="        << (unDefined ? "Y":"N")                         << ","
    <<   "Paremt="           << ((parent == NULL) ? "\"\"" : parent->Name ()) << ","
    <<   "StoredOnDataBase=" << (storedOnDataBase ? "Y" : "N")                << ","
    <<   "Description="      << description.QuotedStr ()
    << " />"
    << endl;
}  /* WriteXML */





MLClassPtr   MLClass::MLClassForGivenHierarchialLevel (uint16 level)  const
{
  VectorKKStr  levelNames = name.Split ('_');
  KKStr fullLevelName = "";

  uint16  curLevel = 0;
  while  ((curLevel < level)  &&  (curLevel < levelNames.size ()))
  {
    if  (curLevel < 1)
      fullLevelName = levelNames[curLevel];
    else
      fullLevelName << "_" << levelNames[curLevel];

    curLevel++;
  }

  return  MLClass::CreateNewMLClass (fullLevelName);
}  /* MLClassForGivenHierarchialLevel*/




MLClassList::MLClassList ():
     KKQueue<MLClass> (false, 10),
     undefinedLoaded (false)
{
  MLClass::AddImageClassList (this);
}




MLClassList::MLClassList (const MLClassList&  _mlClasses):
  KKQueue<MLClass> (false, _mlClasses.QueueSize ())
{
  MLClass::AddImageClassList (this);

  uint32  numOfClasses = _mlClasses.QueueSize ();
  uint32  x;
  
  for  (x = 0; x < numOfClasses; x++)
  {
    AddMLClass (_mlClasses.IdxToPtr (x));  
  }
}





MLClassList::MLClassList (const KKStr&  _fileName,
                                bool&         _successfull
                               ):
     KKQueue<MLClass> (false, 10),
     undefinedLoaded (false)
{
  MLClass::AddImageClassList (this);

  Load (_fileName, _successfull);

  if  (!undefinedLoaded)
  {
    // We have to make sure that there is a UnDefined MLClass.
    AddMLClass (MLClassList::GetMLClassPtr ("UNKNOWN"));
    undefinedLoaded = true;
  }
}



     
MLClassList::~MLClassList ()
{
  MLClass::DeleteImageClassList (this);
}



int32  MLClassList::MemoryConsumedEstimated ()  const
{
  return  sizeof (MLClassList) + sizeof (MLClassPtr) * size ();
}



void  MLClassList::Clear ()
{
  clear ();
  nameIndex.clear ();
}



/** @brief  Update the nameIndex structure about a class name change. */
void  MLClassList::ChangeNameOfClass (MLClassPtr  mlClass, 
                                         const KKStr&   oldName,
                                         const KKStr&   newName,
                                         bool&          successful
                                        )
{
  MLClassPtr  existingClass = this->LookUpByName (newName);
  if  ((existingClass != NULL)  &&  (existingClass != mlClass))
  {
    cerr << endl
      << "MLClassList::ChangeNameOfClass   NewName[" << newName << "]  alreadty used." << endl
      << endl;
    successful = false;
  }
  else
  {
    NameIndex::iterator idx = nameIndex.find (oldName.ToUpper ());
    if  (idx == nameIndex.end ())
    {
      cerr << endl
        << "MLClassList::ChangeNameOfClass   NewName[" << oldName << "]  not found." << endl
        << endl;
      successful = false;
    }
    else
    {
      nameIndex.erase (idx);
      nameIndex.insert (pair<KKStr,MLClassPtr> (newName.ToUpper (), mlClass));
      successful = true;
    }
  }
}  /* ChangeNameOfClass */




uint16   MLClassList::NumHierarchialLevels ()  const
{
  uint16  numHierarchialLevels = 0;
  MLClassList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
    numHierarchialLevels = Max (numHierarchialLevels, (*idx)->NumHierarchialLevels ());
  return  numHierarchialLevels;
}  /* NumHierarchialLevels*/





void  MLClassList::Load (const KKStr&  _fileName,
                            bool&         _successfull
                           )
{
  char   buff[20480];
  int32    lineCount = 0;

  FILE*  inputFile = osFOPEN (_fileName.Str (), "r");
  if  (!inputFile)
  {
    cerr << "MLClassList::ReadInData    *** ERROR ***" << endl
         << "                Input File[" << _fileName << "] not Valid." << endl;
    _successfull = false;
    return;
  }

  while  (fgets (buff, sizeof (buff), inputFile))
  {
    KKStr  dataRow (buff);

    dataRow.TrimRight ();

    MLClassPtr  oneRow = MLClass::CreateNewMLClass (dataRow);

    AddMLClass (oneRow);

    if  (oneRow->UnDefined ())
      undefinedLoaded = true;

    lineCount++;
  }

  fclose (inputFile);
  _successfull = true;
}  /* ReadInData */
    



void  MLClassList::Save (KKStr   _fileName,
                            bool&    _successfull
                           )
{
  ofstream outFile (_fileName.Str ());

  int32             idx;
  int32             qSize = QueueSize ();
  MLClassPtr   mlClass = NULL;

  for  (idx = 0; idx < qSize; idx++)
  {
    mlClass = IdxToPtr (idx);
    outFile << mlClass->ToString ().Str () << endl;
  }

  _successfull = true;
  return;
}  /* WriteOutData */




void  MLClassList::AddMLClassToNameIndex (MLClassPtr  _mlClass)
{
  NameIndex::iterator  idx;
  idx = nameIndex.find (_mlClass->UpperName ());
  if  (idx == nameIndex.end ())
    nameIndex.insert (pair<KKStr,MLClassPtr>(_mlClass->UpperName (), _mlClass));
}



void  MLClassList::AddMLClass (MLClassPtr  _mlClass)
{
  if  (_mlClass->Name ().Empty ())
  {
    cerr << "MLClassList::AddMLClass   Class Name Empty" << endl;
  }
  KKQueue<MLClass>::PushOnBack (_mlClass);
  AddMLClassToNameIndex (_mlClass);
}



MLClassPtr  MLClassList::PopFromBack ()
{
  NameIndex::iterator  idx;
  MLClassPtr ic = KKQueue<MLClass>::PopFromBack ();
  if  (!ic)
    return NULL;

  idx = nameIndex.find (ic->UpperName ());
  if  (idx != nameIndex.end ())
    nameIndex.erase (idx);
  return  ic;
}  /* PopFromBack*/



MLClassPtr  MLClassList::PopFromFront ()
{
  NameIndex::iterator  idx;
  MLClassPtr ic = KKQueue<MLClass>::PopFromFront ();
  if  (!ic)
    return NULL;

  idx = nameIndex.find (ic->UpperName ());
  if  (idx != nameIndex.end ())
    nameIndex.erase (idx);
  return  ic;
}  /* PopFromFront*/


 
void  MLClassList::PushOnBack (MLClassPtr  _mlClass)
{
  if  (_mlClass->Name ().Empty ())
  {
    cerr << "MLClassList::PushOnBack   Class Name Empty" << endl;
  }

  KKQueue<MLClass>::PushOnBack (_mlClass);
  AddMLClassToNameIndex (_mlClass);
}



void  MLClassList::PushOnFront (MLClassPtr  _mlClass)
{
  if  (_mlClass->Name ().Empty ())
  {
    cerr << "MLClassList::PushOnFront   Class Name Empty" << endl;
  }

  KKQueue<MLClass>::PushOnBack (_mlClass);
  AddMLClassToNameIndex (_mlClass);
}



/**                 
 *@brief  Returns a pointer of MLClass object with name (_name);  if none 
 *        in list will then return NULL.
 *@param[in]  _name  Name of MLClass to search for.
 *@return  Pointer to MLClass or NULL  if not Found.
 */
MLClassPtr  MLClassList::LookUpByName (const KKStr&  _name)  const
{
  NameIndex::const_iterator  idx;
  idx = nameIndex.find (_name.ToUpper ());
  if  (idx == nameIndex.end ())
    return NULL;
  else
    return idx->second;
}  /* LookUpByName */





MLClassPtr  MLClassList::LookUpByClassId (int32  _classId)  const
{
  MLClassList::const_iterator  idx;
  MLClassPtr  mlClass;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    mlClass = *idx;
    if  (mlClass->ClassId () == _classId)
      return  mlClass;
  }

  return  NULL;
}  /* LookUpClassId */




/** @brief  return pointer to instance with '_name';  if none exists, create one and add to list. */
MLClassPtr  MLClassList::GetMLClassPtr (const KKStr& _name)
{
  MLClassPtr ic = LookUpByName (_name);
  if  (ic == NULL)
  {
    ic = MLClass::CreateNewMLClass (_name);
    AddMLClass (ic);
  }
  return  ic;
}  /* GetMLClassPtr */






MLClassPtr  MLClassList::GetNoiseClass ()  const
{
  int32          count      = QueueSize ();
  MLClassPtr  noiseClass = NULL;
  int32          x;

  for  (x = 0; ((x < count)  &&  (!noiseClass)); x++)
  {
    MLClassPtr  mlClass = IdxToPtr (x);
    if  (mlClass->UnDefined ())
       noiseClass = mlClass;
  }

  return  noiseClass;
}  /* GetNoiseClass */





MLClassPtr  MLClassList::GetUnKnownClass ()
{
  MLClassPtr  unKnownClass = LookUpByName ("UNKNOWN");
  if  (!unKnownClass)
  {
    unKnownClass = MLClass::CreateNewMLClass ("UNKNOWN");
    PushOnBack (unKnownClass);
  }

  return  unKnownClass;
}  /* GetUnKnownClass */





class  MLClassList::mlClassNameComparison: public  QueueComparison<MLClass>
{
public:
   mlClassNameComparison ():  QueueComparison<MLClass> () {} 

   bool  operator () (MLClassPtr  p1,
                      MLClassPtr  p2
                     )
   {
     return  (p1->UpperName () < p2->UpperName ());
   }

};  /* mlClassNameComparison */





void  MLClassList::SortByName ()
{
  mlClassNameComparison* c = new mlClassNameComparison ();
  sort (begin (), end (), *c);
  delete  c;
}  /* SortByName */





KKStr  MLClassList::ToString ()  const
{
  KKStr s (10 * QueueSize ());

  for (int32 i = 0;  i < QueueSize ();  i++)
  {
    if  (i > 0)
      s << "\t";

    s << IdxToPtr (i)->Name ();
  }

  return  s;
}  /* ToString */




KKStr  MLClassList::ToTabDelimitedStr ()  const
{
  KKStr s (10 * QueueSize ());
  for (int32 i = 0;  i < QueueSize ();  i++)
  {
    if  (i > 0)  s << "\t";
    s << IdxToPtr (i)->Name ();
  }

  return  s;
}  /* ToTabDelimitedStr */




KKStr  MLClassList::ToCommaDelimitedStr ()  const
{
  KKStr s (10 * QueueSize ());
  for (int32 i = 0;  i < QueueSize ();  i++)
  {
    if  (i > 0)  s << ",";
    s << IdxToPtr (i)->Name ();
  }

  return  s;
}  /* ToCommaDelimitedStr */




void  MLClassList::ExtractTwoTitleLines (KKStr&  titleLine1,
                                            KKStr&  titleLine2 
                                           ) const
{
  titleLine1 = "";
  titleLine2 = "";

  int32 x;
  for  (x = 0;  x < QueueSize ();  x++)
  {
    if  (x > 0)
    {
      titleLine1 << "\t";
      titleLine2 << "\t";
    }

    KKStr  className = IdxToPtr (x)->Name ();
    int32  y = className.LocateCharacter ('_');
    if  (y < 0)
    {
      titleLine2 << className;
    }
    else
    {
      titleLine1 << className.SubStrPart (0, y - 1);
      titleLine2 << className.SubStrPart (y + 1);
    }
  }
}  /* ExtractTwoTitleLines */




void  MLClassList::ExtractThreeTitleLines (KKStr&  titleLine1,
                                              KKStr&  titleLine2, 
                                              KKStr&  titleLine3 
                                             ) const
{
  titleLine1 = "";
  titleLine2 = "";
  titleLine3 = "";

  int32 x;
  for  (x = 0;  x < QueueSize ();  x++)
  {
    if  (x > 0)
    {
      titleLine1 << "\t";
      titleLine2 << "\t";
      titleLine3 << "\t";
    }

    KKStr  part1, part2, part3;
    part1 = part2 = part3 = "";
    int32  numOfParts = 0;

    KKStr  className = IdxToPtr (x)->Name ();
    className.TrimLeft ();
    className.TrimRight ();
    
    numOfParts = 1;
    part1 = className.ExtractToken ("_");
    if  (!className.Empty ())
    {
      numOfParts = 2;
      part2 = className.ExtractToken ("_");

      if  (!className.Empty ())
      {
        numOfParts = 3;
        part3 = className;
      }
    }


    switch  (numOfParts)
    {
      case 1: titleLine3 << part1;
              break;

      case 2: titleLine2 << part1;
              titleLine3 << part2;
              break;

      case 3: titleLine1 << part1;
              titleLine2 << part2;
              titleLine3 << part3;
              break;
    }
  }
}  /* ExtractThreeTitleLines */




void  MLClassList::ExtractThreeTitleLines (KKStr&  titleLine1,
                                              KKStr&  titleLine2, 
                                              KKStr&  titleLine3,
                                              int32     fieldWidth
                                             ) const
{
  titleLine1 = "";
  titleLine2 = "";
  titleLine3 = "";

  KKStr blankField;
  blankField.RightPad (fieldWidth);

  int32 x;
  for  (x = 0;  x < QueueSize ();  x++)
  {
    KKStr  part1, part2, part3;
    part1 = part2 = part3 = "";
    int32  numOfParts = 0;

    KKStr  className = IdxToPtr (x)->Name ();
    className.TrimLeft ();
    className.TrimRight ();
    
    numOfParts = 1;
    part1 = className.ExtractToken ("_");
    if  (!className.Empty ())
    {
      numOfParts = 2;
      part2 = className.ExtractToken ("_");

      if  (!className.Empty ())
      {
        numOfParts = 3;
        part3 = className;
      }
    }

    part1.LeftPad (fieldWidth);
    part2.LeftPad (fieldWidth);
    part3.LeftPad (fieldWidth);

    switch  (numOfParts)
    {
      case 1: titleLine1 << blankField;
              titleLine2 << blankField;
              titleLine3 << part1;
              break;

      case 2: titleLine1 << blankField;
              titleLine2 << part1;
              titleLine3 << part2;
              break;

      case 3: titleLine1 << part1;
              titleLine2 << part2;
              titleLine3 << part3;
              break;
    }
  }
}  /* ExtractThreeTitleLines */




KKStr   MLClassList::ExtractHTMLTableHeader () const
{
  KKStr  header (QueueSize () * 50);

  uint16 x;

  MLClassList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    MLClassPtr  ic = *idx;

    VectorKKStr  parts = ic->Name ().Split ('_');

    header << "<th>";
    for  (x = 0;  x < parts.size ();  x++)
    {
      if  (x > 0)
        header << "<br />";
      header << parts[x];
    }
    header << "</th>";
  }

  return  header;
}  /* ExtractHTMLTableHeader */



MLClassListPtr  MLClassList::ExtractListOfClassesForAGivenHierarchialLevel (int32 level)
{
  MLClassListPtr  newList = new MLClassList ();

  MLClassList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    MLClassPtr c = *idx;
    MLClassPtr classForLevel = c->MLClassForGivenHierarchialLevel (level);

    if  (newList->PtrToIdx (classForLevel) < 0)
      newList->AddMLClass (classForLevel);
  }

  newList->SortByName ();
  return  newList;
}  /* ExtractListOfClassesForAGivenHierarchialLevel */





MLClassListPtr  MLClassList::MergeClassList (const MLClassList&  list1,
                                                   const MLClassList&  list2
                                                  )
{
  MLClassListPtr  result = new MLClassList (list1);
  MLClassList::const_iterator idx;

  for  (idx = list2.begin ();  idx != list2.end ();  idx++)
  {
    MLClassPtr  ic = *idx;
    if  (result->PtrToIdx (ic) < 0)
    {
      // This entry (*idx) from list2 was not in list 1
      result->AddMLClass (ic);
    }
  }

  return  result;
}  /* MergeClassList */




MLClassListPtr  MLClassList::BuildListFromDelimtedStr (const KKStr&  s,
                                                             char          delimiter
                                                            )
{
  VectorKKStr  names = s.Split (',');
  MLClassListPtr  classList = new MLClassList ();

  VectorKKStr::iterator  idx;
  for  (idx = names.begin ();  idx != names.end ();  idx++)
    MLClassPtr c = classList->GetMLClassPtr (*idx);

  return  classList;
}  /* BuildListFromCommaDelimtedStr */




void   MLClassList::WriteXML (std::ostream&  o)  const
{
  o << "<MLClassList  NumOfClasses=" << QueueSize () << " >" << endl;

  MLClassList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;
    ic->WriteXML (o);
  }

  o << "</MLClassList>" << endl;
}  /* WriteXML */



bool  MLClassList::operator== (const MLClassList&  right)  const
{
  if  (QueueSize () != right.QueueSize ())
    return  false;

  for  (int32 i = 0;  i < QueueSize ();  i++)
  {
    MLClassPtr  mlClass = IdxToPtr (i);

    if  (right.LookUpByName (mlClass->Name ()) == NULL)
      return  false;
  }

  return  true;
}  /* operator== */




bool  MLClassList::operator!= (const MLClassList&  right)  const
{
  return  (!operator== (right));
}  /* operator== */




MLClassList&  MLClassList::operator-= (const MLClassList&  right) 
{
  if  (&right == this)
  {
    while  (QueueSize () > 0)
      PopFromBack ();
    return  *this;
  }


  MLClassList::const_iterator  idx;
  for  (idx = right.begin ();  idx != right.end ();  idx++)
  {
    MLClassPtr  ic = *idx;
    DeleteEntry (ic);  // if  'ic'  exists in our list it will be deleted.
  }

  return  *this;
}  /* operator=-  */



MLClassList&  MLClassList::operator+= (const MLClassList&  right)  // add all classes that are in the 'right' parameter
{
  if  (this == &right)
    return  *this;

  MLClassList::const_iterator  idx;
  for  (idx = right.begin ();  idx != right.end ();  idx++)
  {
    MLClassPtr  ic = *idx;
    if  (PtrToIdx (ic) < 0)
      PushOnBack (ic);
  }

  return  *this;
}




MLClassList&  MLClassList::operator= (const MLClassList&  right)
{
  if  (&right == this)
    return *this;

  Clear ();

  MLClassList::const_iterator  idx;
  for  (idx = right.begin ();  idx != right.end ();  idx++)
  {
    MLClassPtr  ic = *idx;
    PushOnBack (ic);
  }

  return  *this;
}  /* operator= */



MLClassList  MLClassList::operator- (const MLClassList&  right)  const
{
  MLClassList  result;

  MLClassList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    MLClassPtr  ic = *idx;
    if  (right.PtrToIdx (ic) < 0)
       result.PushOnBack (ic);
  }

  return  result;
}  /* operator- */



ostream&  MLL::operator<< (      ostream&          os, 
                           const MLClassList&   classList
                          )
{
  os << classList.ToString ();
  return  os;
}



KKStr&  MLL::operator<< (      KKStr&            str, 
                         const MLClassList&   classList
                        )
{
  str << classList.ToString ();
  return  str;
}






ClassIndexList::ClassIndexList ():
   largestIndex (-1)
{
}


ClassIndexList::ClassIndexList (const  ClassIndexList&  _list)
{
  const_iterator  idx;
  bool  dupEntry = false;
  for  (idx = _list.begin ();  idx != _list.end ();  idx++)
  {
    AddClassIndexAssignment (idx->first, idx->second, dupEntry);
  }
}  



ClassIndexList::ClassIndexList (const MLClassList&  classes):
    largestIndex(-1)
{
  MLClassList::const_iterator  idx;
  for  (idx = classes.begin ();  idx != classes.end ();  idx++)
  {
    largestIndex++;
    insert   (pair<MLClassConstPtr, short> (*idx, largestIndex));
    shortIdx.insert (pair<short, MLClassConstPtr> (largestIndex, *idx));
  }
}



ClassIndexList::ClassIndexList (const MLClassConstList&  classes):
    largestIndex(-1)
{
  MLClassConstList::const_iterator  idx;
  for  (idx = classes.begin ();  idx != classes.end ();  idx++)
  {
    largestIndex++;
    insert   (pair<MLClassConstPtr, short> (*idx, largestIndex));
    shortIdx.insert (pair<short, MLClassConstPtr> (largestIndex, *idx));
  }
}






void  ClassIndexList::Clear ()
{
  clear ();
  shortIdx.clear ();
  largestIndex = 0;
}





int32  ClassIndexList::MemoryConsumedEstimated ()  const
{
  return sizeof (ClassIndexList) + (shortIdx.size () * (sizeof (short) + sizeof (MLClassConstPtr) + 10));  // added 10- bytes per entry for overhead.
}



void  ClassIndexList::AddClass (MLClassConstPtr  _ic,
                                bool&               _dupEntry
                               )
{
  _dupEntry = false;
  map<MLClassConstPtr, short>::iterator p;
  p = find (_ic);
  if  (p != end ())
  {
    _dupEntry = true;
    return;
  }

  int32  index = largestIndex + 1;
  largestIndex = index;

  insert (pair<MLClassConstPtr, short> (_ic, index));
  shortIdx.insert (pair<short, MLClassConstPtr> (index, _ic));
}  /* AddClass */



void  ClassIndexList::AddClassIndexAssignment (MLClassConstPtr  _ic,
                                               short               _classIndex,
                                               bool&               _dupEntry
                                              )
{
  _dupEntry = false;
  map<MLClassConstPtr, short>::iterator p;
  p = find (_ic);
  if  (p != end ())
  {
    _dupEntry = true;
    return;
  }

  insert (pair<MLClassConstPtr, short> (_ic, _classIndex));
  shortIdx.insert (pair<short, MLClassConstPtr> (_classIndex, _ic));

  if  (_classIndex > largestIndex)
    largestIndex = _classIndex;
}  /* AddClassIndexAssignment */



short  ClassIndexList::GetClassIndex (MLClassConstPtr  c)
{
  int32  index = -1;
  map<MLClassConstPtr, short>::iterator p;
  p = find (c);
  if  (p == end ())
    index = -1;
  else
    index = p->second;

  return  index;
}  /* GetClassIndex */



MLClassConstPtr  ClassIndexList::GetMLClass (short  classIndex)
{
  map<short, MLClassConstPtr>::iterator p;
  p = shortIdx.find (classIndex);
  if  (p == shortIdx.end ())
    return NULL;
  else
    return p->second;
}  /* GetMLClass */



void  ClassIndexList::ParseClassIndexList (const KKStr&  s)
{
  Clear ();
  largestIndex = 0;

  bool   duplicate = false;
  int32    index  = 0;
  KKStr  name   = "";

  VectorKKStr  pairs = s.Split (",");
  for  (uint16 x = 0;  x < pairs.size ();  x++)
  {
    KKStr  pair = pairs[x];
    name  = pair.ExtractToken2 (":");
    index = pair.ExtractTokenInt ("\n\t\r");
    AddClassIndexAssignment (MLClass::CreateNewMLClass (name), index, duplicate);
  }
}  /* ParseClassIndexList */



KKStr  ClassIndexList::ToCommaDelString ()
{
  KKStr  delStr (255);
  map<short, MLClassConstPtr>::const_iterator  idx;
  for  (idx = shortIdx.begin ();  idx != shortIdx.end ();  idx++)
  {
    if  (!delStr.Empty ())
      delStr << ",";
    delStr << idx->second->Name () << ":" << idx->first;
  }
  return  delStr;
}  /* ToCommaDelString */




