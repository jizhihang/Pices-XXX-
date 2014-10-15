#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <fstream>
#include  <string>
#include  <iostream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace  KKU;


#include "FeatureNumList.h"
#include "FileDesc.h"
using namespace  MLL;


FeatureNumList::FeatureNumList (FileDescPtr _fileDesc):

  featureNums              (NULL),
  featureNumsAllocatedSize (0),
  fileDesc                 (_fileDesc),
  numOfFeatures            (0)
{
  if  (!fileDesc)
  {
    cerr << endl
         << endl
         << "FeatureNumList::FeatureNumList    *** ERROR ***    fileDesc = NULL" << endl
         << endl;
    osWaitForEnter ();
  }

  AllocateArraySize (fileDesc->NumOfFields ());
}



FeatureNumList::FeatureNumList (FileDescPtr       _fileDesc,
                                const BitString&  bitString
                               ):
  featureNums              (NULL),
  featureNumsAllocatedSize (0),
  fileDesc                 (_fileDesc),
  numOfFeatures            (0)

{
  if  (!fileDesc)
  {
    cerr << endl << endl 
         << "FeatureNumList    *** ERROR ***        No 'FileDesc' object provided." << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  if  (fileDesc->NumOfFields () != int32 (bitString.BitLen ()))
  {
    cerr << endl << endl
         << "FeatureNumList    *** ERROR ***"  << endl
         << endl
         << "    BitString.Len[" << bitString.BitLen () << "]  is different than FileDesc.NumOfFields[" << fileDesc->NumOfFields () << "]" << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  VectorUint16  listOfBits;
  bitString.ListOfSetBits16 (listOfBits);

  if  (uint32 (featureNumsAllocatedSize) < listOfBits.size ())
    AllocateArraySize ((int32)listOfBits.size ());

  for  (uint32 x = 0;  x < listOfBits.size ();  x++)
    AddFeature (listOfBits[x]);
}



FeatureNumList::FeatureNumList (const FeatureNumList&  _featureNumList):
  featureNums              (NULL),
  featureNumsAllocatedSize (0),
  fileDesc                 (_featureNumList.fileDesc),
  numOfFeatures            (_featureNumList.NumOfFeatures ())
{
  if  (fileDesc == NULL)
  {
    cerr << endl
         << "FeatureNumList::FeatureNumList (const FeatureNumList&  _featureNumList)" << endl
         << "                    fileDesc == NULL" << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  if  ((numOfFeatures < 0)  ||  (numOfFeatures > (int32)fileDesc->NumOfFields ()))
  {
    cerr << endl
         << "FeatureNumList::FeatureNumList (const FeatureNumList&  _featureNumList)" << endl
         << "                    numOfFeatures[" << numOfFeatures << "]  is out of range of fileDesc" << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  featureNums = new uint16[numOfFeatures + 1];
  featureNumsAllocatedSize = numOfFeatures + 1;
  for  (int32 x = 0; x < numOfFeatures; x++)
    featureNums[x] = _featureNumList[x];
}



FeatureNumList::FeatureNumList (FileDescPtr   _fileDesc,
                                const KKStr&  _featureListStr,
                                bool&         _valid
                               ):

  featureNums              (NULL),
  featureNumsAllocatedSize (0),
  fileDesc                 (_fileDesc),
  numOfFeatures            (0)

{
  if  (!fileDesc)
  {
    cerr << endl
         << endl
         << "FeatureNumList::FeatureNumList    *** ERROR ***    fileDesc = NULL" << endl
         << endl;
    osWaitForEnter ();
  }

  AllocateArraySize (fileDesc->NumOfFields ());

  ExtractFeatureNumsFromStr (_featureListStr, _valid);
}



FeatureNumList::FeatureNumList (FileDescPtr           _fileDesc,
                                FeatureSelectionType  _selectionType,
                                const KKStr&          _featureListStr,
                                bool&                 _valid
                               ):
  featureNums              (NULL),
  featureNumsAllocatedSize (0),
  fileDesc                 (_fileDesc),
  numOfFeatures            (0)

{
  if  (!fileDesc)
  {
    cerr << endl
         << endl
         << "FeatureNumList::FeatureNumList    *** ERROR ***    fileDesc = NULL" << endl
         << endl;
    osWaitForEnter ();
  }

  AllocateArraySize (fileDesc->NumOfFields ());

  switch  (_selectionType)
  {
    case  IncludeFeatureNums:
       ExtractFeatureNumsFromStr (_featureListStr, _valid);
       break;
    
    
    case  ExcludeFeatureNums:
       FeatureNumList  excludedFeatures (fileDesc, _featureListStr, _valid);

       numOfFeatures = fileDesc->NumOfFields () - excludedFeatures.NumOfFeatures ();
       featureNums = new uint16[numOfFeatures];
       featureNumsAllocatedSize = numOfFeatures;

       int32  y = 0;
       for  (uint16 x = 0; x < fileDesc->NumOfFields (); x++)
       {
         if  (!excludedFeatures.InList (x))
         {
           featureNums[y] = x;
           y++;
         }
       }
       break;

  }  /* End Of Switch */
}



FeatureNumList::FeatureNumList ():
  featureNums              (NULL),
  featureNumsAllocatedSize (0),
  fileDesc                 (NULL),
  numOfFeatures            (0)
{
}



FeatureNumList::~FeatureNumList ()
{
  delete [] featureNums;
}



int32  FeatureNumList::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (FeatureNumList);
  if  (featureNums)
    memoryConsumedEstimated += sizeof (uint16) * featureNumsAllocatedSize;

  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */



void  FeatureNumList::AllocateArraySize (int32 size)
{
  if  (featureNumsAllocatedSize >= size)
  {
    // We have already allocated at least  'size'  for featureNums.
    return;
  }

  if  (!featureNums)
  {
    featureNums = new uint16[size];
    featureNumsAllocatedSize = size;
    for  (int32 x = 0;  x < size;  x++)
      featureNums[x] = 0;
  }

  else
  {
    uint16*  newFeatureNums = new uint16[size];

    int32  x;

    for  (x = 0;  x < numOfFeatures; x++)
      newFeatureNums[x] = featureNums[x];

    for  (x = numOfFeatures;  x < size;  x++)
      newFeatureNums[x] = 0;


    delete  [] featureNums;
    featureNums = newFeatureNums;
    featureNumsAllocatedSize = size;
  }
}  /* AllocateArraySize */



void   FeatureNumList::ToBitString (BitString&  bitStr)  const
{
  bitStr.ReSet ();
  int32  x;

  for  (x = 0;  x < NumOfFeatures ();  x++)
    bitStr.Set (featureNums[x]);
}  /* ToBitString */



uint16*  FeatureNumList::CreateFeatureNumArray ()  const
{
  uint16*  newList = new uint16[numOfFeatures];
  for  (int32 x = 0;  x < numOfFeatures;  x++)
    newList[x] = featureNums[x];
  return  newList;
}  /* CreateFeatureNumArray */



bool    FeatureNumList::AllFeaturesSelected ()  const
{
  if  (numOfFeatures >= (int32)fileDesc->NumOfFields ())
    return true;
  return false;
}  /* AllFeaturesSelected */



void  FeatureNumList::UnSet ()
{
  if  (featureNums)
  {
    int32 x;
    for  (x = 0;  x < featureNumsAllocatedSize;  x++)
      featureNums[x] = 0;
    numOfFeatures = 0;
  }
  numOfFeatures = 0;
}  /* UnSet */



void  FeatureNumList::UnSet (uint16  featureNum)
{
  if  (!featureNums)
    return;

  int32  indexToDel = numOfFeatures - 1;  // Starting with last index

  while  ((indexToDel >= 0)  &&  (featureNums[indexToDel] > featureNum))
    indexToDel--;

  if  (indexToDel >= 0)
  {
    if  (featureNums[indexToDel] == featureNum)
    {
      // We found the index to delete.
      int32  x;
      for  (x = indexToDel;  x < (numOfFeatures - 1);  x++)
        featureNums[x] = featureNums[x + 1];
    }
 
    numOfFeatures--;
    featureNums[numOfFeatures] = 0;
  }
}  /* UnSet */



void  FeatureNumList::AddFeature (uint16  featureNum)
{
  if  (!featureNums)
  {
    featureNums = new uint16[10];
    featureNumsAllocatedSize = 10;
  }

  if  (featureNum >= fileDesc->NumOfFields ())
  {
    KKStr  errMsg =  "FeatureNumList::AddFeature  ***ERROR***   exceeded MaxNumOfFeatures.";
    errMsg << endl
           << "    FeastureNum[" << featureNum << "]  MaxNumOfFeatures[" << fileDesc->NumOfFields () << "]";

    cerr << std::endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  else if  (numOfFeatures >= featureNumsAllocatedSize)
  {
    // Expand the featureNums array
    int32   newNumOfFeatures = numOfFeatures + 10;
    uint16*  newFeatureNums = new uint16[newNumOfFeatures];

    for  (int32 x = 0;  x < numOfFeatures; x++)
      newFeatureNums[x] = featureNums[x];

    delete  [] featureNums;
    featureNums = newFeatureNums;
    featureNumsAllocatedSize = newNumOfFeatures;
  }
  
  if  (numOfFeatures == 0)
  {
    featureNums[0] = featureNum;
    numOfFeatures = 1;
  }

  else
  {
    int32  x = numOfFeatures - 1; // Setting x to end of list
    if  (featureNums[x] < featureNum)
    {
      // Adding feature to end of list.
      featureNums[numOfFeatures] = featureNum;
      numOfFeatures++;
    }

    else if  (!InList (featureNum))
    {
      while  ((x >= 0)  &&  (featureNums[x] > featureNum))
      {
        featureNums[x + 1] = featureNums[x];
        featureNums[x]     = featureNum;
        x--;
      }

      numOfFeatures++;
    }
  }

  return;
}  /* AddFeature */



FeatureNumList   FeatureNumList::AllFeatures (FileDescPtr  _fileDesc)
{
  bool  valid;
  FeatureNumList  features (_fileDesc, "All", valid);
  return  features;
}  /* AllFeatures */



void   FeatureNumList::SetAllFeatures ()
{
  for  (uint16 x = 0; x < fileDesc->NumOfFields (); x++)
  {
    if  (fileDesc->Type (x) != IgnoreAttribute)
      AddFeature (uint16 (x));
  }
  return;
}  /* SetAllFeatures */



bool  FeatureNumList::IsSubSet (const FeatureNumList&  z)
{
  bool  isSubSet = true;

  int32  idx = 0;
  while  ((idx < z.NumSelFeatures ())  &&  isSubSet)
  {
    int32 fn = z[idx];
    isSubSet = InList (fn);
    idx++;
  }
  return  isSubSet;
}



bool  FeatureNumList::InList (uint16 _featureNum)  const
{
  bool  found = false;
  int32  x = 0;

  while  ((x < numOfFeatures)  && (!found))
  {
    found = (_featureNum == featureNums[x]);
    if  (!found)
      x++;
  }

  return  found;
}  /* InList */



bool  FeatureNumList::Test (uint16 _featureNum)  const
{
  return InList (_featureNum);
}  /* Test */



/**
 *@brief Returns back the selected feature.  
 *@details  A FeatureNumList instance consists of an list of selected features.  It
 *          is logically like an array of selected features that is the same length
 *          as the number of selected features.  
 *@code
 *  Example code that scans the FeatureNumList object  'goodFeatures'
 *
 *  void  PrintSelectedFeatures (const FeatureNumList&  goodFeatures)
 *  {
 *    cout << "Selected Features: ";
 *    for  (int32 x = 0;  x < goodFeatures.NumOfFeatures ();
 *    {
 *      if  (x > 0)  cout << ",";
 *      cout << goodFeatures[x];
 *    }
 *    cout << endl;
 *  }
 *@endcode
 *@param[in]  _idx  The position in this inatnce that you want to return.
 *@return  Selected feature at position '_idx'.
 */
uint16  FeatureNumList::operator[] (int32  _idx)  const
{
  if  (_idx >= numOfFeatures)
  {
    cerr << endl << endl << endl
         << "FeatureNumList::operator[]  Invalid Index[" << _idx << "] requested." << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }
  else
  {
    return featureNums[_idx];
  }
}



KKStr  FeatureNumList::ToString ()  const
{
  KKStr  featureNumStr (numOfFeatures * 6);
  
  if  (numOfFeatures <= 0)
    return featureNumStr;

  int32  nextIdx = 0;

  while  (nextIdx < numOfFeatures)
  {
    int32  startOfGroup = nextIdx;
    int32  endOfGroup   = nextIdx;

    while  ((endOfGroup < (numOfFeatures - 1))  &&  
            (featureNums[endOfGroup] == (featureNums[endOfGroup + 1] - 1))
           )
    {
      endOfGroup++;
    }

    if  ((endOfGroup - startOfGroup) < 3)
    {
      int32  x;
      for  (x = startOfGroup;  x <= endOfGroup; x++)
      {
        if  (!featureNumStr.Empty ())
          featureNumStr << ",";
        featureNumStr << featureNums[x];
      }
    }
    else
    {
      if  (!featureNumStr.Empty ())
        featureNumStr << ",";
      featureNumStr << featureNums[startOfGroup] << "-" << featureNums[endOfGroup];
    }

    nextIdx = endOfGroup + 1;
  }

  return  featureNumStr;
}  /* ToString */



KKStr   FeatureNumList::ToCommaDelStr ()  const 
{
  return  ToString ();
}



KKStr   FeatureNumList::ToHexString ()  const
{
  BitString  bs (fileDesc->NumOfFields ());
  ToBitString (bs);
  return  bs.HexStr ();
}  /* ToHexString */



void  FeatureNumList::ExtractFeatureNumsFromStr (KKStr  _featureListStr,
                                                 bool&  _valid
                                                )
{
  _valid = true;

  KKStr  field;
  uint16  featureNum;

  _featureListStr.Upper ();

  numOfFeatures = 0;

  if  (_featureListStr == "NONE")
  {
    return;
  }
  if  (_featureListStr == "ALL")
  {
    SetAllFeatures ();
    return;
  }

  field = _featureListStr.ExtractToken (", \n\r\t");
  while  (!field.Empty ())
  {
    _featureListStr.TrimLeft ();
   
    int32 dashPos = field.LocateCharacter ('-');
    if  (dashPos < 0)
    {
      // This is not a range

      featureNum = uint16 (atoi (field.Str ()));
      //if  (field.ValidInt (featureNum))
      {
        if  (featureNum >= fileDesc->NumOfFields ())
        {
          _valid = false;
          return;
        }
        else
        {
          bool  alreadyInList = InList (featureNum);
          if  (!alreadyInList)
            AddFeature (featureNum);
        }
      }
    }
    else
    {
      // We are looking at a range
      uint16 startFeatureNum = uint16 (field.ExtractTokenInt (" -"));
      uint16 endFeatureNum   = uint16 (field.ExtractTokenInt (" -"));

      if  ((startFeatureNum >= fileDesc->NumOfFields ())  ||
           (endFeatureNum   >= fileDesc->NumOfFields ())  ||
           (startFeatureNum > endFeatureNum)
          )
      {
         _valid = false;
      }
      else
      {
        for  (featureNum = startFeatureNum; featureNum <= endFeatureNum;  featureNum++)
        {
          bool  alreadyInList = InList (featureNum);
          if  (!alreadyInList)
            AddFeature (featureNum);
        }
      }
    }
    field = _featureListStr.ExtractToken (", \n\r\t");
  }
}  /* ExtractFeatureNumsFromStr */



void  FeatureNumList::Load (const KKStr&  _fileName,
                            bool&          _successful,
                            RunLog&        _log
                           )
{
  _log.Level (20) << "FeatureNumList::Load - File["
                  << _fileName << "]."
                  << endl;

  FILE*  inputFile = osFOPEN (_fileName.Str (), "r");
  if  (!inputFile)
  {
    _log.Level (-1) << "FeatureNumList::Load      *** ERROR ***" << endl;
    _log.Level (-1) << "                 Could Not Open File[" << _fileName << "]." << endl;
    _successful = false;
  }

  char  buff [102400];

  if  (fgets (buff, sizeof (buff), inputFile))
  {
    KKStr  firstLine (buff);
    int32 fileDescNumOfFields = atoi (firstLine.Str ());
    if  (fileDesc->NumOfFields () != fileDescNumOfFields)
    {
      _log.Level (-1) << endl
                      << "FeatureNumList::Load      *** ERROR ***" << endl
                      << "                Mismatch in field count" << endl
                      << "                FileDesc->NumOfFields[" << fileDesc->NumOfFields () << "]" << endl
                      << "                From File            [" << fileDescNumOfFields      << "]" << endl
                      << endl;
      _successful = false;
      fclose (inputFile);
      return;
    }
  }
  else
  {
    _log.Level (-1) << endl
                    << "FeatureNumList::Load      *** ERROR ***" << endl
                    << "                Missing Data from File" << endl
                    << endl;
    _successful = false;
    fclose (inputFile);
    return;
  }

  if  (fgets (buff, sizeof (buff), inputFile))
  {
    _log.Level (50) << "Load - FeatureList = [" << buff << "]." << endl;
    bool  valid;
    ExtractFeatureNumsFromStr (buff, valid);
    if  (!valid)
      _successful = false;
  }
  else
  {
    _log.Level (-1) << "FeatureNumList::Load      *** ERROR ***" << endl;
    _log.Level (-1) << "                No Data in File[" << _fileName << "]." << endl;
    _successful = false;
  }

  fclose (inputFile);
  _successful = true;
}  /* Load */



void  FeatureNumList::Save (const KKStr&  _fileName,
                            bool&          _successful,
                            RunLog&        _log
                           )
{
  _log.Level (20) << "FeatureNumList::Save - File["
                  << _fileName << "]."
                  << endl;

  _successful = true;

  ofstream outFile (_fileName.Str ());

  outFile << fileDesc->NumOfFields () << endl;
  outFile << ToString ()              << endl;

  outFile.close ();
}  /* Save */




void  FeatureNumList::Save (ostream&  o)
{
  o << "<FeatureNumList>" << "\t"
    << "NumOfFeatures"    << "\t"  << numOfFeatures
    << "FeatureNums"      << "\t"  << ToString ()
    << "</FeatureNumList>";
}  /* Save */



FeatureNumList&  FeatureNumList::operator= (const FeatureNumList&  _features)
{
  if  (featureNums)
    delete  [] featureNums;

  fileDesc      = _features.fileDesc;
  numOfFeatures = _features.NumOfFeatures ();

  featureNums = new uint16[numOfFeatures];
  featureNumsAllocatedSize = numOfFeatures;

  for  (int32 fn = 0;  fn < numOfFeatures;  fn++)
    featureNums[fn] = _features[fn];

  return  *this;
}  /* operator= */





FeatureNumList&  FeatureNumList::operator=  (const FeatureNumListPtr  _features)
{
  *this = *_features;
  return  *this;
}



int32  FeatureNumList::Compare (const FeatureNumList&  _features)  const
{
  int32  x = 0;

  while  ((x < numOfFeatures)  &&  (x < _features.NumOfFeatures ()))
  {
    if  (featureNums[x] < _features.featureNums[x])
      return  -1;

    else if  (featureNums[x] > _features.featureNums[x])
      return   1;

    x++;
  }

  if  (x < numOfFeatures)
    return  1;

  else if  (x < _features.numOfFeatures)
    return   -1;

  return  0;
}  /* Compare */



bool  FeatureNumList::operator== (const FeatureNumList&  _features)  const
{
  if  (numOfFeatures != _features.numOfFeatures)
  {
    // No point even comparing the list of feature nums, if the lengths are 
    // different, then they can not be equal.
    return false;
  }

  return  Compare (_features) == 0;
}  /* operatir== */



/** @see compare */
bool  FeatureNumList::operator< (const FeatureNumList&  _features)  const
{
  return  Compare (_features) < 0;
}  /* operatir< */


/** @see compare */
bool  FeatureNumList::operator> (const FeatureNumList&  _features)  const
{
  return  Compare (_features) > 0;
}  /* operatir> */



ostream& MLL::operator<< (      ostream&          os, 
                          const FeatureNumList&   features
                         )
{
  os << features.ToString ();
  return  os;
}


ostream& MLL::operator<< (      ostream&            os, 
                          const FeatureNumListPtr&  features
                         )
{
  os << features->ToString ();
  return  os;
}


/** @details Both FeatureNumList objects must be referening the same FileDesc instance otherwise an exception will be thrown. */
FeatureNumList  FeatureNumList::operator*  (const FeatureNumList&  rightSide)  const
{
  if  (fileDesc != rightSide.FileDesc ())
  {
    KKStr errMsg = "FeatureNumList::operator*  ***ERROR***   Incompatable FileDesc's";
    errMsg << endl
           << "      The associated FileDesc instances are not the same.";
    cerr << endl << endl << errMsg << endl <<endl;
    throw KKStrException (errMsg);
  }

  FeatureNumList  result (fileDesc);
  result.AllocateArraySize (numOfFeatures);

  int32  l = 0;
  int32  r = 0;

  while  ((l < numOfFeatures)  &&  (r < rightSide.numOfFeatures))
  {
    if  (featureNums[l] < rightSide.featureNums[r])
    {
      l++;
    }

    else if  (featureNums[l] > rightSide.featureNums[r])
    {
      r++;
    }

    else
    {
      result.AddFeature (featureNums[l]);
      l++;
      r++;
    }
  }

  return  result;
}  /* operator* */



/** @details Both FeatureNumList objects must be referening the same FileDesc instance otherwise an exception will be thrown. */
FeatureNumList  FeatureNumList::operator+ (const FeatureNumList&  rightSide) const
{
  FeatureNumList  result (rightSide);

  int32  l = 0;
  for  (l = 0;  l < numOfFeatures; l++)
    result.AddFeature (featureNums[l]);

  return  result;
}  /* operator+ */



/** @details Both FeatureNumList objects must be referening the same FileDesc instance otherwise an exception will be thrown. */
FeatureNumList&  FeatureNumList::operator+= (const FeatureNumList&  rightSide)
{
  int32      newFeatureNumsAllocatedSize = Min (numOfFeatures + rightSide.numOfFeatures, (int32)fileDesc->NumOfFields ());
  uint16*  newFeatureNums = new uint16[newFeatureNumsAllocatedSize];
  int32      newNumOfFeatures = 0;

  uint16* leftFeatureNums  = featureNums;
  int32     leftNumOfFeatures = numOfFeatures;
  uint16* rightFeatureNums = rightSide.featureNums;
  int32     rightNumOfFeatures = rightSide.numOfFeatures;

  int32  l = 0;
  int32  r = 0;

  while  ((l < leftNumOfFeatures)  &&  (r < rightNumOfFeatures))
  {
    if  (leftFeatureNums[l] < rightFeatureNums[r])
    {
      newFeatureNums[newNumOfFeatures] = leftFeatureNums[l];
      newNumOfFeatures++;
      l++;
    }

    else if  (leftFeatureNums[l] > rightFeatureNums[r])
    {
      newFeatureNums[newNumOfFeatures] = rightFeatureNums[r];
      newNumOfFeatures++;
      r++;
    }

    else
    {
      newFeatureNums[newNumOfFeatures] = rightFeatureNums[r];
      newNumOfFeatures++;
      l++;
      r++;
    }
  }

  while  (l < leftNumOfFeatures)
  {
    newFeatureNums[newNumOfFeatures] = leftFeatureNums[l];
    newNumOfFeatures++;
    l++;
  }

  while  (r < rightNumOfFeatures)
  {
    newFeatureNums[newNumOfFeatures] = rightFeatureNums[r];
    newNumOfFeatures++;
    r++;
  }

  delete  [] featureNums;
  featureNums = newFeatureNums;
  featureNumsAllocatedSize = newFeatureNumsAllocatedSize;
  numOfFeatures = newNumOfFeatures;

  return  *this;
}  /* operator+= */




/** @details Both FeatureNumList objects must be referening the same FileDesc instance otherwise an exception will be thrown. */
FeatureNumList&  FeatureNumList::operator+= (uint16  featureNum)
{
  AddFeature (featureNum);
  return *this;
}  /* operator+= */





/** @details Both FeatureNumList objects must be referening the same FileDesc instance otherwise an exception will be thrown. */
FeatureNumList  FeatureNumList::operator+ (uint16  rightSide) const
{
  if  (int32 (rightSide) >= fileDesc->NumOfFields ())
  {
    KKStr errMsg = "FeatureNumList::operator+  ***ERROR***";
    errMsg <<" Feature[" << rightSide << "]  is too large.";
    cerr << endl << endl << errMsg << endl <<endl;
    throw KKStrException (errMsg);
  }
  
  FeatureNumList  result (*this);
  result.AddFeature (rightSide);
  return  result;
}  /* operator+ */



/** @details Both FeatureNumList objects must be referening the same FileDesc instance otherwise an exception will be thrown. */
FeatureNumList  FeatureNumList::operator-  (uint16  rightSide) const
{
  if  (rightSide >= fileDesc->NumOfFields ())
  {
    KKStr errMsg = "FeatureNumList::operator-  ***ERROR***";
    errMsg <<" Feature[" << rightSide << "]  is too large.";
    cerr << endl << endl << errMsg << endl <<endl;
    throw KKStrException (errMsg);
  }

  FeatureNumList  result (*this);
  result.UnSet (rightSide);

  return  result;
}  /* operator- */



/**
 *@brief Returns a new FeatureNumList instance that consists of the left side instance with the selected features in the right side removed.
 *@details Both FeatureNumList objects must be referening the same FileDesc instance otherwise an exception will be thrown.
 */
FeatureNumList  FeatureNumList::operator- (const FeatureNumList&  rightSide) const
{
  if  (fileDesc != rightSide.FileDesc ())
  {
    KKStr errMsg = "FeatureNumList::operator-  ***ERROR***   Incompatable FileDesc's";
    errMsg << endl
           << "      The associated FileDesc instances are not the same.";
    cerr << endl << endl << errMsg << endl <<endl;
    throw KKStrException (errMsg);
  }

  FeatureNumList  result (*this);
  int32  x;
  for  (x = 0;  x < rightSide.NumOfFeatures ();  x++)
    result.UnSet (rightSide[x]);

  return  result;
}  /* operator- */



/**
 *@details Both FeatureNumList objects must be referening the same FileDesc instance otherwise an exception will be thrown.
 */
FeatureNumList&  FeatureNumList::operator-= (uint16  rightSide)
{
  UnSet (rightSide);
  return  *this;
}  /* operator-= */



FeatureNumListPtr   FeatureNumList::RandomlySelectFeatures (int32  numToKeep)  const
{
  if  (numToKeep > numOfFeatures)
  {
    cerr << endl
         << endl
         << "FeatureNumList::RandomlySelectFeatures    *** ERROR ***" << endl
         << endl
         << "NumToKeep[" << numToKeep << "]  Is greater than  NumOfFeatures[" << numOfFeatures << "]" << endl
         << endl
         << endl;
    numToKeep = numOfFeatures;
  }

  FeatureNumListPtr  randomlySelectedFeatures = new FeatureNumList (fileDesc);

  int32 x, y, z;

  // Initialize Selected Featurs to the currently selected features in featureNums
  uint16*  selectedFeatures = new uint16[numOfFeatures];
  for  (x = 0; x < numOfFeatures; x++)
    selectedFeatures[x] = featureNums[x];

  // Randomize the order of the selcted featured
  //for (x = 0; x < numOfFeatures; x++)
  for (x = 0; x < numToKeep; x++)
  {
    y = LRand48() % numOfFeatures;
    z = selectedFeatures[x];
    selectedFeatures[x] = selectedFeatures[y];
    selectedFeatures[y] = z;
  }

  // Assign the first 'numToKeep'  featured from the random order of selected features
  for  (x = 0;  x < numToKeep;  x++)
    randomlySelectedFeatures->AddFeature (selectedFeatures[x]);

  delete  [] selectedFeatures;

  return  randomlySelectedFeatures;
}  /* RandomlySelectFeatures */




AttributeType  FeatureNumList::FeatureAttributeType (int32 idx)  const
{
  if  ((idx < 0)  ||  (idx >= numOfFeatures))
  {
    cerr << endl << endl
         << "FeatureNumList::AttributeType       *** ERROR ***"  << endl
         << endl
         << "                Invalid Index[" << idx << "]  Valid Range (0.." << (numOfFeatures - 1) << ")" << endl
         << endl;

    return  NULLAttribute;
  }

  if  (!fileDesc)
  {
    KKStr errMsg = "FeatureNumList::AttributeType   ***ERROR***  'fileDesc == NULL'   There is a major porgramming flaw.";
    cerr << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  return  fileDesc->Type (featureNums[idx]);
}  /* FeatureAttributeType */



FeatureNumList  FeatureNumList::Complement ()  const
{
  uint16  x;

  FeatureNumList  result;

  for  (x = 0;  x < uint16 (fileDesc->NumOfFields ());  x++)
  {
    if  (!InList (x))
      result.AddFeature (x);
  }

  return  result;
}  /* Complemnt */
