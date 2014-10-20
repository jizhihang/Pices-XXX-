#include "FirstIncludes.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "MemoryDebug.h"
using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
using namespace  KKU;


#include "ClassAssignments.h"
#include "MLClass.h"
#include "MLClassConstList.h"
using namespace  MLL;





ClassAssignments::ClassAssignments (RunLog&  _log):
    multimap<int16, MLClassConstPtr> (),
    log (_log)
{
}



ClassAssignments::ClassAssignments (const MLClassConstList&  classes,
                                    RunLog&                     _log
                                   ):
    multimap<int16, MLClassConstPtr> (),
    log (_log)
{
  int32  x = 0;
  for  (MLClassConstList::const_iterator idx = classes.begin ();  idx != classes.end ();  idx++)
  {
   insert (pair<int16, MLClassConstPtr> (x, *idx));
   classLookUp.insert (pair<MLClassConstPtr, int16> (*idx, x));
   x++;
  }
}





int32  ClassAssignments::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (ClassAssignments) 
    +  (classLookUp.size () * (sizeof (MLClassConstPtr) + sizeof (int16)));
  return  memoryConsumedEstimated;
}




void  ClassAssignments::AddMLClass (MLClassConstPtr  mlClass,
                                       int16               num
                                      )
{
  ClassLookUpIterator  idx;
  idx = classLookUp.find (mlClass);
  if  (idx != classLookUp.end ())
  {
    log.Level (-1) << endl << endl 
                   << "ClassAssignments::AddMLClass     ***ERROR***       Duplicate Class Being Added[" << mlClass->Name () <<  "]." << endl
                   << "              Num Found           [" << idx->second << "]" << endl
                   << "              New Num Being Added [" << num         << endl
                   << endl;
    return;
  }

  insert (pair<int16, MLClassConstPtr> (num, mlClass));
  classLookUp.insert (pair<MLClassConstPtr, int16> (mlClass, num));
}  /* AddMLClass */





MLClassConstPtr  ClassAssignments::GetMLClass (int16 num)
{
  multimap<int16, MLClassConstPtr>::iterator p;
  p = find (num);
  if (p == end ())
    return NULL;
  return p->second;
}  /* GetMLClass */





MLClassList  ClassAssignments::GetMLClasses (int16 num)  const
{
  ClassAssignments::const_iterator  idx;

  MLClassList  results;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    if  (idx->first == num)
      results.PushOnBack ((MLClassPtr)idx->second);
  }

  return  results;
}  /* GetMLClasses */




VectorShort   ClassAssignments::GetUniqueListOfAssignments ()  const
{
  VectorShort  nums;

  ClassAssignments::const_iterator  idx;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    nums.push_back (idx->first);
  }

  sort (nums.begin (), nums.end ());

  VectorShort  results;

  int16  lastNum = -999;
  VectorShort::const_iterator  idx2;
  for  (idx2 = nums.begin (); idx2 != nums.end ();  idx2++)
  {
    if  (*idx2 != lastNum)
    {
      lastNum = *idx2;
      results.push_back (lastNum);
    }
  }

  return  results;
}  /* GetUniqueListOfAssignments */





MLClassConstPtr  ClassAssignments::GetMLClassByIndex (size_t idx)
{
  if  ((idx < 0)  ||  (idx >= (size_t)size ()))
  {
    cerr << endl
         << endl
         << "ClassAssignments::GetMLClassByIndex   *** ERROR ***" << endl
         << "                                        idx[" << idx << "] is out of range." << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  iterator i = begin ();
  for  (size_t x = 0; x < idx;  x++)
    i++;

  return i->second;
}  /* GetMLClassByIndex */





int16  ClassAssignments::GetNumForClass (MLClassConstPtr  mlClass)  const
{
  ClassLookUp::const_iterator  idx;
  idx = classLookUp.find (mlClass);
  if  (idx == classLookUp.end ())
    return -1;
  else
    return idx->second;
}  /* GetNumForClass */






void  ClassAssignments::Load (const KKStr&  fileName,
                              bool&          successful
                             )
{
  log.Level (10) << "ClassAssignments::Load -  File[" << fileName << "]." << endl;

  char   buff[20480];

  FILE*  inputFile = osFOPEN (fileName.Str (), "r");
  if  (!inputFile)
  {
    log.Level (-1) << endl
                   << "ClassAssignments::Load    *** ERROR ***" << endl
                   << "                Input File[" << fileName << "] not Valid." << endl
                   << endl;
    successful = false;
    return;
  }

  while  (fgets (buff, sizeof (buff), inputFile))
  {
    log.Level (70) << "ReadInData - [" << buff << "]" << endl;

    KKStr  dataRow (buff);
    dataRow.TrimRight ();
    dataRow.TrimLeft ();

    KKStr  className = dataRow.ExtractToken2 ("\n\r\t,");
    int16   classNum  = (int16)dataRow.ExtractTokenInt ("\n\r\t,");

    MLClassConstPtr  mlClass = MLClass::CreateNewMLClass (className);
    int32  existingAssignmentNum = GetNumForClass (mlClass);
    if  (existingAssignmentNum >= 0)
    {
      // We already have this class Loaded,  for now i am obly going to
      // display a warning.
      log.Level (-1) << endl
                     << endl
                     << endl
                     << "ClassAssignments::Load    *** ERROR ***" << endl
                     << "MLClass[" << className << "] being loaded more than once." << endl
                     << endl;
    }
    else
    {
      AddMLClass (mlClass, classNum);
    }
  }

  fclose (inputFile);
  successful = true;
}  /* Load */



void  ClassAssignments::Save (const KKStr&  fileName,
                              bool&          successful
                             )
{
  log.Level (20) << "ClassAssignments::Save - Writing Out ClassAssignments File[" << fileName << "]." << endl;

  ofstream outFile (fileName.Str ());

  for  (iterator  idx = begin (); idx != end ();  idx++)
  {
    outFile << idx->second->Name () << "\t" << idx->first << endl;
  }

  successful = true;
  return;
}




KKStr  ClassAssignments::ToString ()  const
{
  KKStr  result ((int32)(size () * 20));

  result << int32 (size ());

  ClassAssignments::const_iterator idx;
  for (idx = begin ();  idx != end ();  idx++)
  {
    result << "\t" << idx->first << "\t" << idx->second->Name ();
  }

  return  result;
}  /* ToString */





/**
* @brief Loads class assignments from string that was originally generated by ToString
* @param[in] _toString,  KKStr containing class assignments info, will expect to be formated
*                        the way ToString() creates them.
*/
void   ClassAssignments::ParseToString (const KKStr&  _toString)
{
  erase (begin (), end ());

  KKStr toString (_toString);

  // Remove Class count from string
  toString.ExtractTokenInt ("\t\n\r");

  while  (!toString.Empty ())
  {
    int32 assignmentNum = toString.ExtractTokenInt ("\t\n\r");
    KKStr  className = toString.ExtractToken ("\t\n\r");

    if  (!className.Empty ())
    {
      MLClassConstPtr  mlClass = MLClass::CreateNewMLClass (className);
      AddMLClass (mlClass, assignmentNum);
    }
  }
}  /* ParseToString */
