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





MLClassConstList::MLClassConstList ():
     KKQueueConst<MLClass> (),
     undefinedLoaded (false)
{
}




MLClassConstList::MLClassConstList (const MLClassConstList&  _mlClasses):
  KKQueueConst<MLClass> ()
{
  MLClassConstList::const_iterator  idx;
  for  (idx = _mlClasses.begin ();  idx != _mlClasses.end ();  ++idx)
    PushOnBack (*idx);
}





MLClassConstList::MLClassConstList (const MLClassList&  _mlClasses)
{
  MLClassList::const_iterator  idx;
  for  (idx = _mlClasses.begin ();  idx != _mlClasses.end ();  ++idx)
    PushOnBack (*idx);
}





MLClassConstList::MLClassConstList (const KKStr&  _fileName,
                                          bool&         _successfull
                                         ):
     KKQueueConst<MLClass> (),
     undefinedLoaded (false)
{
  Load (_fileName, _successfull);

  if  (!undefinedLoaded)
  {
    // We have to make sure that there is a UnDefined MLClassConst.
    AddMLClass (MLClass::CreateNewMLClass ("UNKNOWN"));
    undefinedLoaded = true;
  }
}



     
MLClassConstList::~MLClassConstList ()
{
}





int32  MLClassConstList::MemoryConsumedEstimated ()  const
{
  return  sizeof (MLClassConstList) + sizeof (MLClassConstPtr) * size ();
}



void  MLClassConstList::Clear ()
{
  clear ();
  nameIndex.clear ();
}




uint16   MLClassConstList::NumHierarchialLevels ()  const
{
  uint16  numHierarchialLevels = 0;
  MLClassConstList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
    numHierarchialLevels = Max (numHierarchialLevels, (*idx)->NumHierarchialLevels ());
  return  numHierarchialLevels;
}  /* NumHierarchialLevels*/





void  MLClassConstList::Load (const KKStr&  _fileName,
                                 bool&         _successfull
                                )
{
  char   buff[20480];
  int32    lineCount = 0;

  FILE*  inputFile = osFOPEN (_fileName.Str (), "r");
  if  (!inputFile)
  {
    cerr << "MLClassConstList::ReadInData    *** ERROR ***" << endl
         << "                Input File[" << _fileName << "] not Valid." << endl;
    _successfull = false;
    return;
  }

  while  (fgets (buff, sizeof (buff), inputFile))
  {
    KKStr  dataRow (buff);

    dataRow.TrimRight ();

    MLClassConstPtr  oneRow = MLClassConst::CreateNewMLClass (dataRow);

    AddMLClass (oneRow);

    if  (oneRow->UnDefined ())
      undefinedLoaded = true;

    lineCount++;
  }

  fclose (inputFile);
  _successfull = true;
}  /* ReadInData */
    



void  MLClassConstList::Save (KKStr  _fileName,
                                 bool&  _successfull
                                )
{
  ofstream outFile (_fileName.Str ());

  MLClassConstList::const_iterator  idx;

  for  (idx = begin ();  idx != end ();  ++idx)
  {
    outFile << (*idx)->ToString ().Str () << endl;
  }

  outFile.close ();

  _successfull = true;
  return;
}  /* WriteOutData */




void  MLClassConstList::AddMLClassToNameIndex (MLClassConstPtr  _mlClass)
{
  NameIndex::iterator  idx;
  idx = nameIndex.find (_mlClass->UpperName ());
  if  (idx == nameIndex.end ())
    nameIndex.insert (pair<KKStr,MLClassConstPtr>(_mlClass->UpperName (), _mlClass));
}



void  MLClassConstList::AddMLClass (MLClassConstPtr  _mlClass)
{
  if  (_mlClass->Name ().Empty ())
  {
    cerr << "MLClassConstList::AddMLClass   Class Name Empty" << endl;
  }
  KKQueueConst<MLClass>::PushOnBack (_mlClass);
  AddMLClassToNameIndex (_mlClass);
}



MLClassConstPtr  MLClassConstList::PopFromBack ()
{
  NameIndex::iterator  idx;
  MLClassConstPtr ic = KKQueueConst<MLClass>::PopFromBack ();
  if  (!ic)
    return NULL;

  idx = nameIndex.find (ic->UpperName ());
  if  (idx != nameIndex.end ())
    nameIndex.erase (idx);
  return  ic;
}  /* PopFromBack*/



MLClassConstPtr  MLClassConstList::PopFromFront ()
{
  NameIndex::iterator  idx;
  MLClassConstPtr ic = KKQueueConst<MLClass>::PopFromFront ();
  if  (!ic)
    return NULL;

  idx = nameIndex.find (ic->UpperName ());
  if  (idx != nameIndex.end ())
    nameIndex.erase (idx);
  return  ic;
}  /* PopFromFront*/


 
void  MLClassConstList::PushOnBack (MLClassConstPtr  _mlClass)
{
  if  (_mlClass->Name ().Empty ())
  {
    cerr << "MLClassConstList::PushOnBack   Class Name Empty" << endl;
  }

  KKQueueConst<MLClass>::PushOnBack (_mlClass);
  AddMLClassToNameIndex (_mlClass);
}



void  MLClassConstList::PushOnFront (MLClassConstPtr  _mlClass)
{
  if  (_mlClass->Name ().Empty ())
  {
    cerr << "MLClassConstList::PushOnFront   Class Name Empty" << endl;
  }

  KKQueueConst<MLClass>::PushOnBack (_mlClass);
  AddMLClassToNameIndex (_mlClass);
}



/**                 
 *@brief  Returns a pointer of MLClassConst object with name (_name);  if none 
 *        in list will then return NULL.
 *@param[in]  _name  Name of MLClassConst to search for.
 *@return  Pointer to MLClassConst or NULL  if not Found.
 */
MLClassConstPtr  MLClassConstList::LookUpByName (const KKStr&  _name)  const
{
  NameIndex::const_iterator  idx;
  idx = nameIndex.find (_name.ToUpper ());
  if  (idx == nameIndex.end ())
    return NULL;
  else
    return idx->second;
}  /* LookUpByName */





MLClassConstPtr  MLClassConstList::LookUpByClassId (int32  _classId)  const
{
  MLClassConstList::const_iterator  idx;
  MLClassConstPtr  mlClass;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    mlClass = *idx;
    if  (mlClass->ClassId () == _classId)
      return  mlClass;
  }

  return  NULL;
}  /* LookUpClassId */




/** @brief  return pointer to instance with '_name';  if none exists, create one and add to list. */
MLClassConstPtr  MLClassConstList::GetMLClassPtr (const KKStr& _name)
{
  MLClassConstPtr ic = LookUpByName (_name);
  if  (ic == NULL)
  {
    ic = MLClass::CreateNewMLClass (_name);
    AddMLClass (ic);
  }
  return  ic;
}  /* GetMLClassPtr */



MLClassConstPtr  MLClassConstList::GetNoiseClass ()  const
{
  const_iterator  idx;

  for  (idx = begin ();  idx != end ();  ++idx)
  {
    MLClassConstPtr  ic = *idx;
    if  (ic->UnDefined ())
      return ic;
  }

  return NULL;
}  /* GetNoiseClass */





MLClassConstPtr  MLClassConstList::GetUnKnownClass ()
{
  MLClassConstPtr  unKnownClass = LookUpByName ("UNKNOWN");
  if  (!unKnownClass)
  {
    unKnownClass = MLClass::CreateNewMLClass ("UNKNOWN");
    PushOnBack (unKnownClass);
  }

  return  unKnownClass;
}  /* GetUnKnownClass */





class  MLClassConstList::mlClassNameComparison: public  QueueComparison<MLClassConst>
{
public:
   mlClassNameComparison ():  QueueComparison<MLClassConst> () {} 

   bool  operator () (MLClassConstPtr  p1,
                      MLClassConstPtr  p2
                     )
   {
     return  (p1->UpperName () < p2->UpperName ());
   }

};  /* mlClassNameComparison */





void  MLClassConstList::SortByName ()
{
  mlClassNameComparison* c = new mlClassNameComparison ();
  sort (begin (), end (), *c);
  delete  c;
}  /* SortByName */





KKStr  MLClassConstList::ToString ()  const
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




KKStr  MLClassConstList::ToTabDelimitedStr ()  const
{
  KKStr s (10 * QueueSize ());
  for (int32 i = 0;  i < QueueSize ();  i++)
  {
    if  (i > 0)  s << "\t";
    s << IdxToPtr (i)->Name ();
  }

  return  s;
}  /* ToTabDelimitedStr */




KKStr  MLClassConstList::ToCommaDelimitedStr ()  const
{
  KKStr s (10 * QueueSize ());
  for (int32 i = 0;  i < QueueSize ();  i++)
  {
    if  (i > 0)  s << ",";
    s << IdxToPtr (i)->Name ();
  }

  return  s;
}  /* ToCommaDelimitedStr */




void  MLClassConstList::ExtractTwoTitleLines (KKStr&  titleLine1,
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




void  MLClassConstList::ExtractThreeTitleLines (KKStr&  titleLine1,
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




void  MLClassConstList::ExtractThreeTitleLines (KKStr&  titleLine1,
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




KKStr   MLClassConstList::ExtractHTMLTableHeader () const
{
  KKStr  header (QueueSize () * 50);

  uint16 x;

  MLClassConstList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;

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



MLClassConstListPtr  MLClassConstList::ExtractListOfClassesForAGivenHierarchialLevel (int32 level)  const
{
  MLClassConstListPtr  newList = new MLClassConstList ();

  MLClassConstList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    MLClassConstPtr c = *idx;
    MLClassConstPtr classForLevel = c->MLClassForGivenHierarchialLevel (level);

    if  (newList->PtrToIdx (classForLevel) < 0)
      newList->AddMLClass (classForLevel);
  }

  newList->SortByName ();
  return  newList;
}  /* ExtractListOfClassesForAGivenHierarchialLevel */





MLClassConstListPtr  MLClassConstList::MergeClassList (const MLClassConstList&  list1,
                                                             const MLClassConstList&  list2
                                                            )
{
  MLClassConstListPtr  result = new MLClassConstList (list1);
  MLClassConstList::const_iterator idx;

  for  (idx = list2.begin ();  idx != list2.end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;
    if  (result->PtrToIdx (ic) < 0)
    {
      // This entry (*idx) from list2 was not in list 1
      result->AddMLClass (ic);
    }
  }

  return  result;
}  /* MergeClassList */




MLClassConstListPtr  MLClassConstList::BuildListFromDelimtedStr (const KKStr&  s,
                                                             char          delimiter
                                                            )
{
  VectorKKStr  names = s.Split (',');
  MLClassConstListPtr  classList = new MLClassConstList ();

  VectorKKStr::iterator  idx;
  for  (idx = names.begin ();  idx != names.end ();  idx++)
    MLClassConstPtr c = classList->GetMLClassPtr (*idx);

  return  classList;
}  /* BuildListFromCommaDelimtedStr */




void   MLClassConstList::WriteXML (std::ostream&  o)  const
{
  o << "<MLClassConstList  NumOfClasses=" << QueueSize () << " >" << endl;

  MLClassConstList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;
    ic->WriteXML (o);
  }

  o << "</MLClassConstList>" << endl;
}  /* WriteXML */



bool  MLClassConstList::operator== (const MLClassConstList&  right)  const
{
  if  (QueueSize () != right.QueueSize ())
    return  false;

  for  (int32 i = 0;  i < QueueSize ();  i++)
  {
    MLClassConstPtr  mlClass = IdxToPtr (i);

    if  (right.LookUpByName (mlClass->Name ()) == NULL)
      return  false;
  }

  return  true;
}  /* operator== */




bool  MLClassConstList::operator!= (const MLClassConstList&  right)  const
{
  return  (!operator== (right));
}  /* operator== */




MLClassConstList&  MLClassConstList::operator-= (const MLClassConstList&  right) 
{
  if  (&right == this)
  {
    while  (QueueSize () > 0)
      PopFromBack ();
    return  *this;
  }


  MLClassConstList::const_iterator  idx;
  for  (idx = right.begin ();  idx != right.end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;
    DeleteEntry (ic);  // if  'ic'  exists in our list it will be deleted.
  }

  return  *this;
}  /* operator=-  */



MLClassConstList&  MLClassConstList::operator+= (const MLClassConstList&  right)  // add all classes that are in the 'right' parameter
{
  if  (this == &right)
    return  *this;

  MLClassConstList::const_iterator  idx;
  for  (idx = right.begin ();  idx != right.end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;
    if  (PtrToIdx (ic) < 0)
      PushOnBack (ic);
  }

  return  *this;
}




MLClassConstList&  MLClassConstList::operator= (const MLClassConstList&  right)
{
  if  (&right == this)
    return *this;

  Clear ();

  MLClassConstList::const_iterator  idx;
  for  (idx = right.begin ();  idx != right.end ();  ++idx)
  {
    MLClassConstPtr  ic = *idx;
    PushOnBack (ic);
  }

  return  *this;
}  /* operator= */



MLClassConstList  MLClassConstList::operator- (const MLClassConstList&  right)  const
{
  MLClassConstList  result;

  MLClassConstList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;
    if  (right.PtrToIdx (ic) < 0)
       result.PushOnBack (ic);
  }

  return  result;
}  /* operator- */



ostream&  MLL::operator<< (      ostream&              os, 
                           const MLClassConstList&  classList
                          )
{
  os << classList.ToString ();
  return  os;
}



KKStr&  MLL::operator<< (      KKStr&                str, 
                         const MLClassConstList&  classList
                        )
{
  str << classList.ToString ();
  return  str;
}

