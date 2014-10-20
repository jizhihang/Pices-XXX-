#include  "FirstIncludes.h"


#include  <ctype.h>
#include  <time.h>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <math.h>
#include  <vector>

 
#include  "MemoryDebug.h"

using namespace  std;

#include "BasicTypes.h"
#include "DateTime.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace  KKU;


#include "FeatureVector.h"
#include "DuplicateImages.h"
#include "FeatureNumList.h"
#include "FeatureFileIO.h"
#include "MLClass.h"
#include "MLClassConstList.h"
using namespace  MLL;



FeatureVector::FeatureVector (int32  _numOfFeatures):

        featureData      (NULL),
        numOfFeatures    (_numOfFeatures),
        breakTie         (0.0f),
        mlClass       (NULL),
        imageFileName    (),
        missingData      (false),
        origSize         (0.0f),
        predictedClass   (NULL),
        probability      (-1.0),
        trainWeight      (1.0f),
        validated        (false)
{
  AllocateFeatureDataArray ();
}




FeatureVector::FeatureVector (const FeatureVector&  _example):
  featureData      (NULL),
  numOfFeatures    (_example.numOfFeatures),
  breakTie         (_example.breakTie),
  mlClass       (_example.mlClass),
  imageFileName    (_example.imageFileName),

  missingData      (false),
  origSize         (_example.origSize),
  predictedClass   (_example.predictedClass),
  probability      (_example.probability),

  trainWeight      (_example.trainWeight),

  validated        (_example.validated)

{
  if  (_example.featureData)
  {
    AllocateFeatureDataArray ();
    for  (int32 x = 0; x < numOfFeatures; x++)
      featureData[x] = _example.featureData[x];
  }
}




FeatureVector::~FeatureVector ()
{
  delete[] featureData;  featureData = NULL;
}



int32   FeatureVector::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (FeatureVector)
    +  imageFileName.MemoryConsumedEstimated ();

  if  (featureData)
    memoryConsumedEstimated += sizeof (FFLOAT) * numOfFeatures;
  
  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */



void  FeatureVector::ResetNumOfFeatures (int32  newNumOfFeatures)
{
  if  (newNumOfFeatures < 1)  
  {
    cerr << endl
         << endl
         << "FeatureVector::ResetNumOfFeatures    *** ERROR ***" << endl
         << "                                     NewNumOfFeatures[" << newNumOfFeatures << "] is invalid." << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  int32  x;
  FFLOAT*  newFeatureData = new FFLOAT[newNumOfFeatures];
  for  (x = 0;  x < newNumOfFeatures;  x++)
  {
    if  (x < numOfFeatures)
      newFeatureData[x] = featureData[x];
    else
      newFeatureData[x] = 0.0f;
  }

  delete  featureData;
  featureData   = newFeatureData;
  numOfFeatures = newNumOfFeatures;
}  /* ResetNumOfFeatures */



void  FeatureVector::AllocateFeatureDataArray ()
{
  if  (featureData)
    delete  featureData;

  featureData = new FFLOAT [numOfFeatures];

  int32  x;

  for  (x = 0; x < numOfFeatures; x++)
    featureData[x] = 0;
}  /* AllocateFeatureDataArray */




FFLOAT  FeatureVector::FeatureData (int32 featureNum)  const
{
  if  ((featureNum < 0)  ||  (featureNum >= NumOfFeatures ()))
  {
    cerr << endl
         << endl
         << "*** ERROR ***   FeatureVector::FeatureData(" << featureNum << ")  Imndex out of bounds." << endl
         << endl;
    return 0.0f;
  }

  return featureData[featureNum];
}  /* FeatureData */




void  FeatureVector::FeatureData (int32   _featureNum,
                                  FFLOAT  _featureValue
                                 )
{
  if  ((_featureNum < 0)  ||  (_featureNum >= NumOfFeatures ()))
  {
    cerr << endl
         << endl
         << "*** ERROR ***   FeatureVector::FeatureData(" << _featureNum << ")  Imndex out of bounds,    no value set." << endl
         << endl;
    return;
  }

  featureData[_featureNum] = _featureValue;
}  /* FeatureData */




float  FeatureVector::TotalOfFeatureData ()  const
{
  float  totalOfFeatureData = 0.0f;
  for  (int x = 0;  x < NumOfFeatures ();  ++x)
    totalOfFeatureData += featureData[x];
  return  totalOfFeatureData;
}






const  
KKStr&   FeatureVector::ClassName ()  const
{
  static  const
  KKStr  imageFeaturesClassNameInvalid = "*INVALID*";

  if  (!mlClass)
  {
    cerr << endl
         << "FeatureVector::ClassName   *** ERROR ***" << endl
         << endl
         << "               Attempt to get ClassName,  But not pointing to valid MLClass Object" << endl
         << endl;
    return  imageFeaturesClassNameInvalid;
  }
  else
  {
    return  mlClass->Name ();
  }
}  /* ClassName */





void  FeatureVector::AddFeatureData (int32     _featureNum,
                                     FFLOAT  _featureData
                                    )
{
  if  ((_featureNum < 0)  ||  (_featureNum >= numOfFeatures))
  {
    cerr << endl
         << endl
         << "AddFeatureData  *** ERROR ***  FeatureNum[" 
         << _featureNum << "] exceeds maximum allowed Feature Num["
         << numOfFeatures << "]."
         << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  featureData[_featureNum] = _featureData;
}  /* AddFeatureData */




bool  FeatureVector::FeatureDataValid ()
{
  int32  featureNum;

  for  (featureNum = 0;  featureNum < numOfFeatures;  featureNum++)
  {
    if  ((featureData[featureNum] == FFLOAT_MIN)  ||  (featureData[featureNum] == FFLOAT_MAX))
      return  false;
  }

  return true;
}  /* FeatureDataValid */





const KKStr&  FeatureVector::PredictedClassName ()  const
{
  if  (predictedClass)
    return  predictedClass->Name ();
  else
    return  KKStr::EmptyStr ();
}  /* PredictedClasseName */




const KKStr&  FeatureVector::MLClassName  ()  const
{
  if  (mlClass)
    return mlClass->Name ();
  else
    return KKStr::EmptyStr ();
}  /* MLClassName */




bool FeatureVector::operator== (FeatureVector &other_image)  const
{
  if (numOfFeatures != other_image.numOfFeatures)
    return false;

  for (int32 i = 0; i < numOfFeatures; i++)
  {
    if (featureData[i] != other_image.featureData[i])
    {
      return false;
    }
  }

  return true;
}  /* operator== */




FeatureVectorList::FeatureVectorList (FileDescPtr  _fileDesc,
                                      bool         _owner,
                                      RunLog&      _log,
                                      int32          _size
                                     ):
  KKQueue<FeatureVector> (_owner, _size),

  log           (_log),
  curSortOrder  (IFL_UnSorted),
  fileDesc      (_fileDesc),
  fileName      (),
  numOfFeatures (0)

{
  if  (!fileDesc)
  {
    log.Level (-1) << endl
                   << "FeatureVectorList::FeatureVectorList    *** ERROR ***" << endl
                   << "                                     FileDesc == NULL" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  numOfFeatures = fileDesc->NumOfFields ();
}




FeatureVectorList::FeatureVectorList (FeatureVectorList&  examples):
     KKQueue<FeatureVector> (examples),

     log                  (examples.log),
     curSortOrder         (IFL_UnSorted),
     fileDesc             (examples.fileDesc),
     fileName             (examples.fileName),
     numOfFeatures        (examples.numOfFeatures)

{
}


/**
 *@brief  Constructor that create a duplicate list of FeatureVectors;  
 *@details 
 *@code
 *if (owner == true)  then will also
 *   1) duplicate the cotents.  That is each example in the list will be 
 *      duplicated by calling the copy constructor.
 *   2) And the new list will own these newly created examples.
 *@endcode
 */
FeatureVectorList::FeatureVectorList (const FeatureVectorList&  examples,
                                      bool                      _owner
                                     ):
   KKQueue<FeatureVector> (examples, _owner),

   log                  (examples.log),
   curSortOrder         (IFL_UnSorted),
   fileDesc             (examples.fileDesc),
   fileName             (examples.fileName),
   numOfFeatures        (examples.numOfFeatures)
{
}



/**
 *@brief  Constructs a list of examples that are a subset of the ones in _examples as dictatd by _mlClasses
 *@details
 * The subset will consist of the examples who's mlClass is one of the ones in mlClasses.  The new 
 * instance cratd will not own the contents;  it will just point to the existing examples that were in
 * '_examples'.
 *
 *@param[in] _mlClasses  List of classes that the subset of examples will comde from.
 *@param[in] _examples  Examples that th esubset will bederived from.
 *@param[in] _log  Logging file.
 */
FeatureVectorList::FeatureVectorList (MLClassConstList&  _mlClasses,
                                      FeatureVectorList&    _examples,
                                      RunLog&               _log
                                     ):
  KKQueue<FeatureVector> (false, _examples.QueueSize ()),

  log           (_log),
  curSortOrder  (IFL_UnSorted),
  fileDesc      (_examples.fileDesc),
  fileName      (_examples.fileName),
  numOfFeatures (_examples.numOfFeatures)

{
  FeatureVectorList::const_iterator  idx;
  for  (idx = _examples.begin ();  idx != _examples.end ();  ++idx)
  {
    FeatureVectorPtr  example = *idx;
    if  (_mlClasses.PtrToIdx (example->MLClass ()) >= 0)
      AddSingleExample (example);
  }
}



FeatureVectorList::~FeatureVectorList ()
{
}



int32  FeatureVectorList::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (FeatureVectorList) + fileName.MemoryConsumedEstimated ();
  FeatureVectorList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  ++idx)
  {
    FeatureVectorPtr  fv = *idx;
    memoryConsumedEstimated += fv->MemoryConsumedEstimated ();
  }
  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */



/**
 *@details   
 *  Determines if the other FeatreVectorList has the same underlining layout;  that is each
 *  field is of the same type and meaning.  This way we can determine if one list contains
 *  Apples while the other contains Oranges.
 */
bool  FeatureVectorList::SameExceptForSymbolicData (const FeatureVectorList&  otherData)  const
{
  return  fileDesc->SameExceptForSymbolicData (*(otherData.FileDesc ()), log);
}



void  FeatureVectorList::RemoveEntriesWithMissingFeatures ()
{
  log.Level (50) << "FeatureVectorList::RemoveEntriesWithMissingFeatures" << endl;

  vector<FeatureVectorPtr>  entriesToBeDeleted;

  for  (iterator idx = begin ();  idx != end ();  idx++)
  {
    FeatureVectorPtr example = *idx;
    if  (example->MissingData ())
      entriesToBeDeleted.push_back (example);
  }

  for  (int32 x = 0;  x < (int32)entriesToBeDeleted.size ();  x++)
  {
    FeatureVectorPtr example = entriesToBeDeleted[x];
    DeleteEntry (example);
    if  (Owner ())
      delete  example;
  }

}  /* RemoveEntriesWithMissingFeatures */



void  FeatureVectorList::ValidateFileDescAndFieldNum (int32          fieldNum,
                                                      const char*  funcName
                                                     )  const
{
  if  (!fileDesc)
  {
    // This should never ever be able to happen,  but will check 
    // any way.  If missing something has gone very very wring.
    KKStr  msg (200);
    msg << "FeatureVectorList::" << funcName << "      *** ERROR ***  'fileDesc == NULL'";
    log.Level (-1) << endl << endl << msg << endl << endl;
    throw KKStrException (msg);
  }

  if  ((fieldNum < 0)  ||  (fieldNum >= (int32)fileDesc->NumOfFields ()))
  {
    KKStr  msg (200);
    msg << "FeatureVectorList::" << funcName << "    *** ERROR ***    FeatureNum[" << fieldNum << "] is out of range.";
    log.Level (-1) << endl << endl << msg << endl << endl;
    throw KKStrException (msg);
  }
} /* ValidateFileDescAndFieldNum */



bool   FeatureVectorList::AllFieldsAreNumeric ()  const
{
  return  fileDesc->AllFieldsAreNumeric ();
}



const KKStr&  FeatureVectorList::FieldName (int32 featureNum) const
{
  ValidateFileDescAndFieldNum (featureNum, "FieldName");
  return  fileDesc->FieldName (featureNum);
}  /* FeatureName */




AttributeType  FeatureVectorList::FeatureType (int32 featureNum) const
{
  ValidateFileDescAndFieldNum (featureNum, "FeatureType");
  return  fileDesc->Type (featureNum);
}  /* FeatureType */




KKStr  FeatureVectorList::FeatureTypeStr (int32 featureNum) const
{
  ValidateFileDescAndFieldNum (featureNum, "FeatureTypeStr");
  return  AttributeTypeToStr (fileDesc->Type (featureNum));
}  /* FeatureType */



int32   FeatureVectorList::FeatureCardinality (int32 featureNum)  const
{
  ValidateFileDescAndFieldNum (featureNum, "FeatureCardinality");
  return  fileDesc->Cardinality (featureNum, log);
}




AttributeTypeVector  FeatureVectorList::CreateAttributeTypeTable ()  const
{
  ValidateFileDescAndFieldNum (0, "CreateAttributeTypeTable");
  return  fileDesc->CreateAttributeTypeTable ();
}  /* CreateAttributeTypeTable */




vector<int32>  FeatureVectorList::CreateCardinalityTable ()  const
{
  ValidateFileDescAndFieldNum (0, "CreateCardinalityTable");
  return  fileDesc->CreateCardinalityTable ();
}  /* CreateAttributeTypeTable */





FeatureNumList  FeatureVectorList::AllFeatures ()
{
  return FeatureNumList::AllFeatures (fileDesc);
}



void  FeatureVectorList::ResetNumOfFeaturs (int32 newNumOfFeatures)
{
  numOfFeatures = newNumOfFeatures;

  for  (iterator idx = begin ();  idx != end ();  idx++)
  {
    FeatureVectorPtr  i = *idx;
    i->ResetNumOfFeatures (newNumOfFeatures);
  }
}  /* ResetNumOfFeaturs */




void  FeatureVectorList::ResetFileDesc (FileDescPtr  newFileDesc)
{
  if  (!newFileDesc)
  {
    cerr << endl
         << endl
         << "FeatureVectorList::ResetFileDesc   *** ERROR ***" << endl
         << "                  newFileDesc == NULL"            << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  fileDesc = newFileDesc;

  numOfFeatures = fileDesc->NumOfFields ();

  for  (iterator idx = begin ();  idx != end ();  idx++)
  {
    FeatureVectorPtr  i = *idx;
    i->ResetNumOfFeatures (numOfFeatures);
  }
}  /* ResetFileDesc */




void  FeatureVectorList::PushOnBack (FeatureVectorPtr  example)
{
  if  (example->NumOfFeatures () != numOfFeatures)
  {
    cerr << endl
         << "FeatureVectorList::PushOnBack      *** ERROR ***  numOfFeatures mismatch"
         << "                         numOfFeatures        [" << numOfFeatures           << "]" << endl
         << "                         example->NumOfFeaturess[" << example->NumOfFeatures () << "]" << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  KKQueue<FeatureVector>::PushOnBack (example);
  curSortOrder = IFL_UnSorted;
}  /* PushOnBack */




void  FeatureVectorList::PushOnFront (FeatureVectorPtr  example)
{
  if  (example->NumOfFeatures () != numOfFeatures)
  {
    cerr << endl
         << "FeatureVectorList::PushOnFront     *** ERROR ***  numOfFeatures mismatch"
         << "                         numOfFeatures        [" << numOfFeatures           << "]" << endl
         << "                         example->NumOfFeaturess[" << example->NumOfFeatures () << "]" << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  KKQueue<FeatureVector>::PushOnFront (example);
  curSortOrder = IFL_UnSorted;
}  /* PushOnFront */




/*
MLClassListPtr  FeatureVectorList::ExtractListOfClasses ()  const
{
  MLClassConstPtr  lastClass = NULL;
  map<MLClassConstPtr,MLClassConstPtr>  ptrIndex;
  map<MLClassConstPtr,MLClassConstPtr>::iterator  ptrIndexItr;
  FeatureVectorList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  ++idx)
  {
    FeatureVectorPtr example = *idx;
    MLClassConstPtr  newClass = example->MLClass ();
    if  (newClass == lastClass)
      continue;

    lastClass  = newClass;
    ptrIndexItr = ptrIndex.find (newClass);
    if  (ptrIndexItr == ptrIndex.end ())
    {
      lastClass = newClass;
      ptrIndex.insert (pair<MLClassConstPtr,MLClassConstPtr> (newClass, newClass));
    }
  }

  MLClassListPtr  classes = new MLClassList ();
  for  (ptrIndexItr = ptrIndex.begin ();  ptrIndexItr != ptrIndex.end ();  ++ptrIndexItr)
    classes->PushOnBack ((MLClassPtr)(ptrIndexItr->first));

  return   classes;
}//  /* ExtractListOfClasses *  /
*/





MLClassConstListPtr  FeatureVectorList::ExtractMLClassConstList ()  const
{
  MLClassConstPtr  lastClass = NULL;
  map<MLClassConstPtr,MLClassConstPtr>  ptrIndex;
  map<MLClassConstPtr,MLClassConstPtr>::iterator  ptrIndexItr;
  FeatureVectorList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  ++idx)
  {
    FeatureVectorPtr example = *idx;
    MLClassConstPtr  newClass = example->MLClass ();
    if  (newClass == lastClass)
      continue;

    lastClass  = newClass;
    ptrIndexItr = ptrIndex.find (newClass);
    if  (ptrIndexItr == ptrIndex.end ())
    {
      lastClass = newClass;
      ptrIndex.insert (pair<MLClassConstPtr,MLClassConstPtr> (newClass, newClass));
    }
  }

  MLClassConstListPtr  classes = new MLClassConstList ();
  for  (ptrIndexItr = ptrIndex.begin ();  ptrIndexItr != ptrIndex.end ();  ++ptrIndexItr)
    classes->PushOnBack (ptrIndexItr->first);

  return   classes;
}  /* ExtractMLClassConstList */





void  FeatureVectorList::AddSingleExample (FeatureVectorPtr  _imageFeatures)
{
  PushOnBack (_imageFeatures);
  curSortOrder = IFL_UnSorted;
}



void  FeatureVectorList::RemoveDuplicateEntries ()
{
  DuplicateImages  dupDetector (this, 
                                log
                               );
  dupDetector.PurgeDuplicates (this, NULL);
}  /* RemoveDuplicateEntries */



void  FeatureVectorList::AddQueue (const FeatureVectorList&  examplesToAdd)
{
  if  (numOfFeatures != examplesToAdd.NumOfFeatures ())
  {
    KKStr  errMsg = "FeatureVectorList::AddQueue   ***ERROR***    'examplesToAdd' has different 'NumOfFeatures'.";
    errMsg << endl << "       numOfFeatures              [" << numOfFeatures               << "]" << endl
                   << "       examplesToAdd.numOfFeatures[" << examplesToAdd.numOfFeatures << "]";
    log.Level (-1) << endl << errMsg << endl << endl;
    throw  errMsg;
  }

  KKQueue<FeatureVector>::AddQueue (examplesToAdd);
}  /* AddQueue */




int32  FeatureVectorList::GetClassCount (MLClassConstPtr  c)  const
{
  int32  count =0;
  FeatureVectorList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    if  ((*idx)->MLClass () == c)
      count++;
  }

  return  count;
}  /* GetClassCount */




FeatureVectorListPtr   FeatureVectorList::ExtractExamplesForHierarchyLevel (uint32 level)
{
  FeatureVectorListPtr  examples = new FeatureVectorList (fileDesc, true, log, QueueSize ());
  FeatureVectorList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    const FeatureVectorPtr  fv = *idx;

    FeatureVectorPtr  newFV = new FeatureVector (*fv);
    newFV->MLClass (fv->MLClass ()->MLClassForGivenHierarchialLevel (level));
    examples->PushOnBack (newFV);
  }

  return  examples;
}  /* ExtractExamplesForHierarchyLevel */





FeatureVectorListPtr   FeatureVectorList::ExtractImagesForAGivenClass (MLClassConstPtr  _mlClass,
                                                                       int32               _maxToExtract,
                                                                       FFLOAT              _minSize
                                                                      )  const
{
  int32                idx;
  int32                qSize = QueueSize ();
  FeatureVectorPtr   example;
  int32                numExtracted = 0;

  if  (_maxToExtract < 1)
    _maxToExtract = QueueSize ();

  // Create a new list structure that does not own the Images it contains.  This way when 
  // this structure is deleted.  The example it contains are not deleted.
  FeatureVectorListPtr  extractedImages = new FeatureVectorList (fileDesc, false, log, _maxToExtract);

  if  (!extractedImages)
  {
    KKStr  err = "***ERROR***, ExtractImagesForAGivenClass,  Could not allocate more space.";
    cerr << endl << err << endl;
    osDisplayWarning (err);
    exit (-1);
  }

  for  (idx = 0; ((idx < qSize)); idx++)
  {
    example = IdxToPtr (idx);
    if  (((example->MLClass () == _mlClass)  ||  (!_mlClass)))
    {
      if  (example->OrigSize () >= _minSize)
      {
        extractedImages->AddSingleExample (example);
        numExtracted++;
        if  (numExtracted >= _maxToExtract)
          break;
      }
    }
  }

  extractedImages->Compress ();

  return  extractedImages;
}  /*  ExtractImagesForAGivenClass  */





KKStrListPtr   FeatureVectorList::ExtractDuplicatesByImageFileName () 
{
  SortByImageFileName ();

  KKStrListPtr  duplicateImages = new KKStrList (true, 100);

  if  (QueueSize () < 2)
    return  duplicateImages;

  FeatureVectorList::iterator  iIDX;

  iIDX = this->begin ();
  FeatureVectorPtr  lastImage = *iIDX;  ++iIDX;


  while  (iIDX != end ())
  {
    FeatureVectorPtr  example = *iIDX;  ++iIDX;

    if  (example->ImageFileName () == lastImage->ImageFileName ())
    {
      duplicateImages->PushOnBack (new KKStr (example->ImageFileName ()));

      if  (iIDX != end ())
      {
        example = *iIDX;
        ++iIDX;
      }
      else
      {
        example = NULL;
      }

      while  ((example != NULL)   &&   (example->ImageFileName () == lastImage->ImageFileName ()))
      {
        if  (iIDX != end ())
        {
          example = *iIDX;
          ++iIDX;
        }
        else
        {
          example = NULL;
        }
      }
    }

    lastImage = example;
  }

  return  duplicateImages;
}  /*  ExtractDuplicateImageFileNames  */






FeatureVectorPtr  FeatureVectorList::BinarySearchByName (const KKStr&  _imageFileName)  const
{
  if  ((curSortOrder != IFL_ByName) && (curSortOrder != IFL_ByRootName))
  {
    cerr << endl
         << "FeatureVectorList::BinarySearchByName    ****  ERROR ****"  << endl
         << endl
         << "                   List is Not sorted in ImageFileName Order"  << endl
         << endl;
    osDisplayWarning ("FeatureVectorList::BinarySearchByName  Invalid Sort Order.");
    exit (-1);
  }

  int32  low  = 0;
  int32  high = QueueSize () - 1;
  int32  mid;

  FeatureVectorPtr  example = NULL;

  while  (low <= high)
  {
    mid = (low + high) / 2;

    example = IdxToPtr (mid);

    if  (example->ImageFileName () < _imageFileName)
    {
      low = mid + 1;
    }

    else if  (example->ImageFileName () > _imageFileName)
    {
      high = mid - 1;
    }

    else
    {
      return  example;
    }
  }

  return  NULL;
}  /* BinarySearchByName */





FeatureVectorPtr  FeatureVectorList::LookUpByRootName (const KKStr&  _rootName)
{
  FeatureVectorPtr  example = NULL;

  if  (curSortOrder != IFL_ByRootName)
  {
    log.Level (-1)  
         << endl
         << "FeatureVectorList::LookUpByRootName   ***WARNING***  List is NOT SORTED by RootName."  << endl
         << endl;

    FeatureVectorList::iterator  idx;
    for  (idx = begin ();  idx != end ();  idx++)
    {
      example = *idx;
      if  (_rootName == osGetRootName (example->ImageFileName ()))
        return example;
    }
    return NULL;
  }
  else
  {
    int32  low  = 0;
    int32  high = QueueSize () - 1;
    int32  mid;

    while  (low <= high)
    {
      mid = (low + high) / 2;

      example = IdxToPtr (mid);

      KKStr  tempName = osGetRootName (example->ImageFileName ());

      if  (tempName < _rootName)
      {
        low = mid + 1;
      }

      else if  (tempName > _rootName)
      {
        high = mid - 1;
      }

      else
      {
        return  example;
      }
    }
  }

  return  NULL;
}  /* LookUpByRootName */






FeatureVectorPtr  FeatureVectorList::LookUpByImageFileName (const KKStr&  _imageFileName)  const
{
  if  (curSortOrder == IFL_ByName)
  {
    return  BinarySearchByName (_imageFileName);
  }

  else
  {
    int32                idx;
    int32                qSize = QueueSize ();
    FeatureVectorPtr   example = NULL;
    FeatureVectorPtr   tempImage;

    for  (idx = 0; ((idx < qSize) && (!example)); idx++)
    {
      tempImage = IdxToPtr (idx);
      if  (_imageFileName == tempImage->ImageFileName ())   
         example = tempImage;
    }

    return  example;
  }
}  /* LookUpByImageFileName */




FeatureVectorListPtr  FeatureVectorList::OrderUsingNamesFromAFile (const KKStr&  fileName)
{
  FILE*  in = osFOPEN (fileName.Str (), "r");
  if  (!in)
  {
    log.Level (-1) << endl
                   << "FeatureVectorList::OrderUsingNamesFromAFile   *** ERROR ***" << endl
                   << "                               Could not open file[" << fileName << "]." << endl
                   << endl;
    return NULL;
  }

  FeatureVectorPtr      example = NULL;
  FeatureVectorListPtr  orderedImages = new FeatureVectorList (fileDesc, false, log, QueueSize ());

  char buff[1024];
  while  (fgets (buff, sizeof (buff), in))
  {
    KKStr  txtLine (buff);

    if  (txtLine.SubStrPart (0, 1) == "//")
    {
      // Coment line, wil ignore.
      continue;
    }

    KKStr imageFileName = txtLine.ExtractToken ("\n\r\t");
    if  (orderedImages->LookUpByImageFileName (imageFileName))
    {
      // Image file name used more than once, will treat as error
      log.Level (-1) << endl
                     << "FeatureVectorList::OrderUsingNamesFromAFile   *** ERROR ***" << endl
                     << "                      ImageFileName[" << imageFileName << "] occured more than once in file." << endl
                     << endl;
      fclose (in);
      delete  orderedImages;
      return NULL;
    }

    example = LookUpByImageFileName (imageFileName);
    if  (!example)
    {
      // Image file name not in list, will treet as error.
      log.Level (-1) << endl
                     << "FeatureVectorList::OrderUsingNamesFromAFile   *** ERROR ***" << endl
                     << "                      ImageFileName[" << imageFileName << "] Not in list." << endl
                     << endl;
      fclose (in);
      delete  orderedImages;
      return NULL;
    }

    orderedImages->PushOnBack (example);
  }

  fclose (in);

  return  orderedImages;
}  /* OrderUsingNamesFromAFile */




void  FeatureVectorList::SaveOrderingOfImages (const KKStr&  _fileName,
                                               bool&          _successful
                                              )
{
  _successful = true;

  ofstream o (_fileName.Str ());
  if  (!o.is_open ())
  {
    log.Level (-1) << endl
                   << "FeatureVectorList::SaveOrderingOfImages   *** ERROR ***" << endl
                   << "                       Could not open file[" << _fileName << "]." << endl
                   << endl;
    _successful = false;
    return;
  }

  o << "// " << "Time Written  [" << osGetLocalDateTime () << "]" << endl;
  o << "// " << "File Name     [" << _fileName             << "]" << endl;
  o << "// " << "Size          [" << QueueSize ()          << "]" << endl;
  o << "//"                                                       << endl;


  FeatureVectorList::iterator  idx;

  for  (idx = begin ();  idx != end ();  idx++)
    o << (*idx)->ImageFileName () << endl;

  o.close ();

  return;
}  /* OrderUsingNamesFormAFile */







/**
 * @brief  Creates a duplicate of list and also dupliactes it contents.
 * @return Duplicated list with hardcopy of its contents.
 */
FeatureVectorListPtr  FeatureVectorList::DuplicateListAndContents ()  const
{
  FeatureVectorListPtr  copyiedList = new FeatureVectorList (fileDesc, true, log, QueueSize ());

  for  (int32 idx = 0;  idx < QueueSize ();  idx++)
  {
    FeatureVectorPtr  curImage = IdxToPtr (idx);
    copyiedList->AddSingleExample (new FeatureVector (*curImage));
  }
  
  return  copyiedList;
}  /* DuplicateListAndContents */





KKStr  FeatureVectorList::ClassStatisticsStr ()  const
{
  ClassStatisticListPtr  stats = GetClassStatistics ();

  KKStr  s (stats->QueueSize () * 30 + 100);

  s << "Total_Images\t"  << QueueSize ()    << endl;
  s << endl; 
  s << "Total_Classes\t" << stats->QueueSize () << endl;
  s << endl;

  s << "Class_Name" << "\t" << "Count" << endl;
  ClassStatisticList::iterator  statsIDX;
  for  (statsIDX = stats->begin (); statsIDX != stats->end ();  statsIDX++)
  {
    ClassStatisticPtr  cs = *statsIDX;
    s << cs->Name () << "\t" << cs->Count () << endl;
  }

  delete  stats;
  return  s;
}  /* ClassStatisticsStr */





ClassStatisticListPtr  FeatureVectorList::GetClassStatistics ()  const
{
  ClassStatisticListPtr  classStatistics = new ClassStatisticList (true);

  int32 x;

  MLClassConstPtr  mlClass = NULL;
  FeatureVectorPtr    example    = NULL;


  FeatureVectorList::const_iterator  idx;

  for  (idx = begin ();  idx != end ();  ++idx)
  {
    example = *idx;
    mlClass = example->MLClass ();
    
    ClassStatisticPtr  classStatistic = NULL;

    x = 0;
    
    while  (x < classStatistics->QueueSize ())
    {
      classStatistic = classStatistics->IdxToPtr (x);
      if  (classStatistic->MLClass () == mlClass)
      {
        break;
      }
      else
      {
        x++;
      }
    }

    if  (x >= classStatistics->QueueSize ())
    {
      classStatistic = new ClassStatistic (mlClass, 0);
      classStatistics->PushOnBack (classStatistic);
    }

    classStatistic->Increment ();
  }


  classStatistics->SortByClassName ();

  return  classStatistics;
}  /* GetClassStatistics */




FeatureVectorListPtr  FeatureVectorList::ExtractDuplicatesByRootImageFileName ()
{
  FeatureVectorListPtr  duplicateList = new FeatureVectorList (fileDesc, false, log, 100);

  if  (QueueSize () < 2)
  {
    // Since there are less than 2 elements in the list,  
    // There is no way that there can be any dupliactes in the list.
    return  duplicateList;
  }


  FeatureVectorList  workList (*this, 
                               false    // owner = false,  only create a list of pointers to existinmg instances
                              );  // 

  workList.SortByRootName (false);
  
  FeatureVectorList::iterator  idx;
  idx = workList.begin ();

  FeatureVectorPtr  lastImage = *idx;  ++idx;
  FeatureVectorPtr  example   = *idx;  ++idx;

  KKStr  lastRootName = osGetRootName (lastImage->ImageFileName ());
  KKStr  rootName;

  while  (example)
  {
    rootName = osGetRootName (example->ImageFileName ());
    if  (rootName != lastRootName)
    {
      lastRootName = rootName;
      lastImage    = example;
      example = *idx;  ++idx;
    }
    else
    {
      duplicateList->PushOnBack (lastImage);

      while  ((example)  &&  (rootName == lastRootName))
      {
        duplicateList->PushOnBack (example);
        example = *idx;  ++idx;
        if  (example)
          rootName = osGetRootName (example->ImageFileName ());
      }
    }
  }

  return  duplicateList;
}  /* ExtractDuplicatesByRootImageFileName */




void  SplitImagesAmongstFolds (int32                  numOfFolds,
                               int32                  maxImagesPerClass,
                               FeatureVectorListPtr*  folds,
                               FeatureVectorListPtr   src
                              )
{
  src->RandomizeOrder ();
  src->RandomizeOrder ();

  int32  imagesInThisList = src->QueueSize ();
  if  (maxImagesPerClass > 0)
    imagesInThisList = Min (imagesInThisList, maxImagesPerClass);

  int32  x;

  for  (x = 0; x < imagesInThisList; x++)
  {
    folds[x % numOfFolds]->AddSingleExample (src->IdxToPtr (x));
  }
}  /* SplitImagesAmongstFolds */




void  FeatureVectorList::CalcStatsForFeatureNum (int32   _featureNum,
                                                 int32&  _count,
                                                 float&  _total,
                                                 float&  _mean,
                                                 float&  _var,
                                                 float&  _stdDev
                                                )
{
  _count  = 0;
  _total  = 0.0f;
  _mean   = 0.0f;
  _var    = 0.0f;
  _stdDev = 0.0f;

  if  ((_featureNum < 0)  ||  (_featureNum >= NumOfFeatures ()))
  {
    log.Level (-1) << endl << endl
                   << "FeatureVectorList::CalcStatsForFeatureNum    *** ERROR ***  Invalid FeatureNum[" << _featureNum << "]" << endl
                   << "                                            FeatureNum   [" << _featureNum      << "]" << endl
                   << "                                            NumOfFeatures[" << NumOfFeatures () << "]" << endl
                   << endl;
    _count = -1;
    return;
  }

  if  (QueueSize () == 0)
    return;
    

  iterator  idx;

  for  (idx = begin (); idx != end ();  idx++)
  {
    FeatureVectorPtr  i = *idx;
    _total += i->FeatureData (_featureNum);
    _count++;
  }

  if  (_count < 1)
  {
    _mean   = 0.0f;
    _var    = 0.0f;
    _stdDev = 0.0f;
    return;
  }

  _mean = _total / (float)_count;

  float  totalSquareDelta = 0.0f;
  for  (idx = begin (); idx != end ();  idx++)
  {
    FeatureVectorPtr  i = *idx;
    float  delta = i->FeatureData (_featureNum) - _mean;
    totalSquareDelta += delta * delta;
  }

  _var     = totalSquareDelta / _count;
  _stdDev  = sqrt (_var);
}  /* CalcStatsForFeatureNum */



FeatureVectorListPtr  FeatureVectorList::StratifyAmoungstClasses (int32  numOfFolds)
{
  MLClassConstListPtr  classes = ExtractMLClassConstList ();

  FeatureVectorListPtr stratifiedExamples = StratifyAmoungstClasses (classes, -1, numOfFolds);
  delete  classes;  classes = NULL;

  return  stratifiedExamples;
}  /* StratifyAmoungstClasses */





FeatureVectorListPtr  FeatureVectorList::StratifyAmoungstClasses (MLClassConstListPtr  mlClasses,
                                                                  int32                   maxImagesPerClass,
                                                                  int32                   numOfFolds
                                                                 )
{
  log.Level (10) << "FeatureVectorList::StratifyAmoungstClasses" << endl;

  int32  foldNum;
  int32  x;

  FeatureVectorListPtr*  folds = new FeatureVectorListPtr[numOfFolds];
  for  (x = 0; x < numOfFolds; x++)
    folds[x] = new FeatureVectorList (fileDesc, false, log, maxImagesPerClass);

  MLClassConstPtr  mlClass = NULL;
  MLClassConstList::iterator  icIDX;

  for  (icIDX = mlClasses->begin ();  icIDX != mlClasses->end ();  ++icIDX)
  {
    mlClass = *icIDX;
    FeatureVectorListPtr  imagesInClass = ExtractImagesForAGivenClass (mlClass);

    if  (imagesInClass->QueueSize () < numOfFolds)
    {
      log.Level (-1) << endl
                     << endl
                     << "FeatureVectorList::DistributesImagesRandomlyWithInFolds    **** ERROR ****" << endl
                     << endl
                     << endl
                     << "*** ERROR ***,  Not enough examples to split amongst the different folds."   << endl
                     << "                Class           [" << mlClass->Name ()         << "]."  << endl
                     << "                Number of Images[" << imagesInClass->QueueSize () << "]."  << endl
                     << "                Number of Folds [" << numOfFolds                  << "]."  << endl
                     << endl;

    
      KKStr  msg;
      msg << "Not enough Images[" << imagesInClass->QueueSize ()  << "] "
          << "for Class["         << mlClass->Name ()          << "] "
          << "to distribute in Folds.";

      if  (!osIsBackGroundProcess ())
        osDisplayWarning (msg);
    }

    imagesInClass->RandomizeOrder ();
    imagesInClass->RandomizeOrder ();

    SplitImagesAmongstFolds (numOfFolds, maxImagesPerClass, folds, imagesInClass);
    delete  imagesInClass;
    imagesInClass = NULL;
  }
  
  FeatureVectorListPtr stratafiedImages = new FeatureVectorList (fileDesc, false, log, QueueSize ());

  for  (foldNum = 0; foldNum < numOfFolds; foldNum++)
  {
    folds[foldNum]->RandomizeOrder ();
    folds[foldNum]->RandomizeOrder ();
    stratafiedImages->AddQueue (*folds[foldNum]);
    delete  folds[foldNum];
    folds[foldNum] = NULL;
  }

  stratafiedImages->Compress ();

  delete  folds;
  folds = NULL;
  return  stratafiedImages;
}  /* StratifyAmoungstClasses */




float    FeatureVectorList::MajorityClassFraction ()  const
{
  ClassStatisticListPtr  classStats = GetClassStatistics ();

  if  (!classStats)
    return 0.0f;


  ClassStatisticPtr  largestClassStat = NULL;
  uint32 largestClassSize = 0;

  ClassStatisticList::const_iterator  idx;
  for  (idx = classStats->begin ();  idx != classStats->end ();  idx++)
  {
    ClassStatisticPtr   classStats = *idx;
    if  (classStats->Count () > largestClassSize)
    {
      largestClassSize = classStats->Count ();
      largestClassStat = classStats;
    }
  }

  float  fraction = float (largestClassSize) / float (size ());

  delete  classStats;

  return  fraction;
}  /* MajorityClassFraction */




void  FeatureVectorList::PrintClassStatistics (ostream&  o)  const
{
  ClassStatisticListPtr  stats = GetClassStatistics ();

  o << "Total_Images\t"  << QueueSize ()    << endl;
  o << endl; 
  o << "Total_Classes\t" << stats->QueueSize () << endl;
  o << endl;

  ClassStatisticList::const_iterator  statsIDX;
  int32  index = 0;

  o << "Class_Name" << "\t" << "Index" << "\t" << "Count" << endl;
  for  (statsIDX = stats->begin ();  statsIDX != stats->end ();  ++statsIDX)
  {
    o << (*statsIDX)->Name () << "\t" << index << "\t" << (*statsIDX)->Count () << endl;
    index++;
  }

  delete  stats;
  stats = NULL;
  return;
}  /* PrintClassStatistics */




void  FeatureVectorList::PrintClassStatisticsHTML (ostream&  o)  const
{
  ClassStatisticListPtr  stats = GetClassStatistics ();

  o << "<table align=\"center\"  border=\"2\" cellpadding=\"4\">"  << endl
    << "<thead>" << endl;

  o << "<tr>"
    << "<th align=\"left\">Class<br />Name</th>"
    << "<th align=\"center\">Index</th>"
    << "<th align=\"center\">Count</th>"
    << "</tr>" 
    << endl
    << "</thead>" << endl;

  o << "<tbody>" << endl;

  ClassStatisticList::iterator  statsIDX;
  int32  index = 0;
  for  (statsIDX = stats->begin (); statsIDX != stats->end ();  statsIDX++)
  {
    ClassStatistic& statistic = **statsIDX;
    o << "<tr>"
      << "<td align=\"left\">"    << statistic.Name ()  << "</td>"
      << "<td align=\"center\">"  << index              << "</td>" 
      << "<td align=\"center\">"  << statistic.Count () << "</td>" 
      << "</tr>"
      << endl;
    index++;
  }

  o << "<tr>"
    << "<td align=\"left\">"    << "Total"      << "</td>"
    << "<td align=\"center\">"  << "&nbsp"      << "</td>" 
    << "<td align=\"center\">"  << QueueSize () << "</td>" 
    << "</tr>"
    << endl;

  o << "</tbody>" << endl;
  o << "</table>" << endl;


  delete  stats;
  return;
}  /* PrintClassStatisticsHTML */



void  FeatureVectorList::PrintFeatureStatisticsByClass (ostream&  o)  const
{
  o << "Class"      << "\t"
    << "ClassIdx"   << "\t"
    << "FeatureNum" << "\t"
    << "FieldName"  << "\t"
    << "Type"       << "\t"
    << "Count"      << "\t"
    << "Total"      << "\t"
    << "Mean"       << "\t"
    << "Var"        << "\t"
    << "StdDev"
    << endl;

  MLClassConstListPtr  mlClasses = ExtractMLClassConstList ();   

  MLClassConstList::const_iterator cIDX;

  int32  classIdx = 0;

  for  (cIDX = mlClasses->begin ();  cIDX != mlClasses->end ();  cIDX++)
  {
    MLClassConstPtr  mlClass = *cIDX;
    FeatureVectorListPtr  imagesThisClass = ExtractImagesForAGivenClass (mlClass);
      
    int32  featureNum;
    for  (featureNum = 0;  featureNum < imagesThisClass->NumOfFeatures ();  featureNum++)
    {
      int32    count;
      float  total, mean, var, stdDev;
      imagesThisClass->CalcStatsForFeatureNum (featureNum, count, total, mean, var, stdDev);
      o << mlClass->Name ()          << "\t"
        << classIdx                     << "\t"
        << featureNum                   << "\t"
        << FieldName (featureNum)       << "\t"
        << FeatureTypeStr (featureNum)  << "\t"
        << count                        << "\t"
        << total                        << "\t"
        << mean                         << "\t"
        << var                          << "\t"
        << stdDev
        << endl;
    }

    delete  imagesThisClass;
    o << endl;

    classIdx++;
  }

  delete  mlClasses;
  mlClasses = NULL;
}  /* PrintFeatureStatisticsByClass */




VectorDouble  FeatureVectorList::ExtractMeanFeatureValues ()
{
  int32  fn = 0;
  FeatureVectorList::const_iterator  idx;
  VectorDouble  totals (this->NumOfFeatures (), 0.0);
  VectorDouble  means (this->NumOfFeatures (), 0.0);
  for  (idx = begin ();  idx != end ();  idx++)
  {
    const FeatureVectorPtr fv = *idx;
    const float*  fd = fv->FeatureDataConst ();

    for  (fn = 0;  fn < NumOfFeatures ();  fn++)
      totals[fn] += fd[fn];
  }


  for  (fn = 0;  fn < NumOfFeatures ();  fn++)
    means[fn] = totals[fn] / (double)this->QueueSize ();

  return  means;
}  /* ExtractMeanFeatureValues */




FeatureVectorListPtr   FeatureVectorList::ExtractRandomSampling (float  percentage,   // 0.0 -> 100.0
                                                                 int32   minClassCount
                                                                )
{


  if  (percentage <= 0.0f)
  {
    log.Level (-1) << endl << endl 
                   << "ImageFeaturesList::ExtractRandomSampling   ***ERROR***    Percantage[" << percentage << "]  Will return empty list" << endl 
                   << endl;
    percentage = 0.0f;
  }

  if  (percentage > 100.0f)
  {
    log.Level (-1) << endl << endl 
                   << "ImageFeaturesList::ExtractRandomSampling   ***ERROR***    Percantage[" << percentage << "]  will default it to 1.0f" << endl 
                   << endl;
    percentage = 1.0f;
  }

  int32  newSize = (int32)(0.5f + (float)QueueSize () * percentage / 100.0f);
  FeatureVectorListPtr  randomSampled = new FeatureVectorList (fileDesc, false, log, newSize);

  MLClassConstListPtr  classes = ExtractMLClassConstList ();
  classes->SortByName ();
  MLClassConstList::iterator  idx;
  for  (idx = classes->begin ();  idx != classes->end ();  idx++)
  {
    MLClassConstPtr ic = *idx;
    FeatureVectorListPtr  examplesThisClass = ExtractImagesForAGivenClass (ic);
    examplesThisClass->RandomizeOrder ();

    int32  numExamplesThisClass = Max (minClassCount, ((int32)(0.5f + (float)(examplesThisClass->QueueSize ()) * percentage / 100.0f)));
    if  (numExamplesThisClass > examplesThisClass->QueueSize ())
      numExamplesThisClass = examplesThisClass->QueueSize ();
    for  (int32 zed = 0;  zed < numExamplesThisClass;  zed++)
      randomSampled->PushOnBack (examplesThisClass->IdxToPtr (zed));
    delete  examplesThisClass;  examplesThisClass = NULL;
  }

  delete  classes;   classes = NULL;

  randomSampled->RandomizeOrder ();
  return  randomSampled; 
}  /* ExtractRandomSampling */




bool  FeatureVectorList::MissingData () const  // Returns true if 1 or more entries have missing data.
{
  FeatureVectorList::const_iterator  idx;

  FeatureVectorPtr  i = NULL;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    i = *idx;
    if  (i->MissingData ())
      return true;
  }

  return  false;
}  /* MissingData */







void  FeatureVectorList::ReSyncSymbolicData (FileDescPtr  newFileDesc)

{
  int32  fieldNum;

  VectorInt  symbolicFields;
  vector<VectorInt>  lookUpTables;

  for  (fieldNum = 0;  fieldNum < (int32)newFileDesc->NumOfFields ();  fieldNum++)
  {
    if  (newFileDesc->Type (fieldNum) == SymbolicAttribute)
    {
      symbolicFields.push_back (fieldNum);
      VectorInt  lookUpTable;

      int32  x;
      for  (x = 0;  x < fileDesc->Cardinality (fieldNum, log);  x++)
      {
        const KKStr&  nominalValue = fileDesc->GetNominalValue (fieldNum, x);
        int32  newCd = newFileDesc->LookUpNominalCode (fieldNum, nominalValue);
        if  (newCd < 0)
        {
          log.Level (-1) << endl << endl << endl
                         << "FeatureVectorList::ReSyncSymbolicData      ***  ERROR  ***" << endl
                         << endl
                         << "FieldNum[" << fieldNum << "]  FieldName[" << newFileDesc->FieldName (fieldNum) << "]" << endl
                         << "Nominal Value[" << nominalValue << "]    is missing." << endl
                         << endl;
          osWaitForEnter ();
          exit (-1);
        }

        lookUpTable.push_back (newCd);
      }

      lookUpTables.push_back (lookUpTable);
    }
  }


  FeatureVectorList::iterator  idx;
  for  (idx = begin ();  idx !=  end ();  idx++)
  {
    FeatureVectorPtr i = *idx;

    uint32  x;

    for  (x = 0;  x < symbolicFields.size ();  x++)
    {
      fieldNum = symbolicFields[x];
      int32  oldCode = int32 (0.5f + i->FeatureData (fieldNum));
      int32  newCode = lookUpTables[x][oldCode];
      i->AddFeatureData (fieldNum, FFLOAT (newCode));
    }
  }

  fileDesc = newFileDesc;
}  /* ReSyncSymbolicData */



void  FeatureVectorList::SynchronizeSymbolicData (FeatureVectorList& otherData)
{
  if  (!fileDesc->SameExceptForSymbolicData (*(otherData.FileDesc ()), log))
  {
    log.Level (-1) << endl << endl << endl
                   << "FeatureVectorList::SynchronizeSymbolicData     *** ERROR ***" << endl
                   << endl
                   << "The two datasets have more than SymbolicData differences." << endl
                   << endl;

    osWaitForEnter ();
    exit (-1);
  }

  FileDescPtr  newFileDesc = FileDesc::MergeSymbolicFields (*fileDesc, *(otherData.FileDesc ()), log);

  ReSyncSymbolicData (newFileDesc);
  otherData.ReSyncSymbolicData (newFileDesc);
}  /* SynchronizeSymbolicData */



KKStr  GetClassNameByHierarchyLevel (KKStr  className, 
                                     int32    level
                                    )
{
  int32  curLevel = 0;
  KKStr  fullLevelName = "";
  KKStr  nextLevelName = className.ExtractToken ("_");
  while  ((curLevel < level)  &&  (!nextLevelName.Empty ()))
  {
    curLevel++;
    if  (!fullLevelName.Empty ())
      fullLevelName << "_";
    fullLevelName << nextLevelName;
    nextLevelName = className.ExtractToken ("_");
  }

  return  fullLevelName;
}  /* GetClassNameByHierarchyLevel */




FeatureVectorListPtr  FeatureVectorList::CreateListForAGivenLevel (int32  level)
{
  FeatureVectorListPtr  examplesLabeledForAppropriateLevel = new FeatureVectorList (fileDesc, true, log);

  MLClassConstListPtr  allClasses = ExtractMLClassConstList ();
  allClasses->SortByName ();

  MLClassConstList::iterator  idx;
  idx = allClasses->begin ();

  while  (idx != allClasses->end ())
  {
    MLClassConstPtr  curClass = *idx;
    KKStr  curClassNameForThisLevel = GetClassNameByHierarchyLevel (curClass->Name (), level);
    if  (curClassNameForThisLevel.Empty ())
    {
      idx++;
      continue;
    }
    else
    {
      MLClassConstPtr  curClassForThisLevel = MLClass::CreateNewMLClass (curClassNameForThisLevel);
      MLClassConstPtr  nextClassForThisLevel = curClassForThisLevel;

      while  ((idx != allClasses->end ())  &&  (nextClassForThisLevel == curClassForThisLevel))
      {
        {
          FeatureVectorListPtr  examplesForCurClass = ExtractImagesForAGivenClass (curClass);
          FeatureVectorListPtr  reLabeledExamples = examplesForCurClass->DuplicateListAndContents ();
          delete  examplesForCurClass;  examplesForCurClass = NULL;

          FeatureVectorList::iterator  idx2;
          for  (idx2 = reLabeledExamples->begin ();  idx2 != reLabeledExamples->end ();  idx2++)
          {
            FeatureVectorPtr  fv = *idx2;
            fv->MLClass (curClassForThisLevel);
          }
          examplesLabeledForAppropriateLevel->AddQueue (*reLabeledExamples);
          reLabeledExamples->Owner (false);
          delete  reLabeledExamples;  reLabeledExamples = NULL;
        }

        // Get next Class to process.
        idx++;
        if  (idx != allClasses->end ())
        {
          curClass = *idx;
          curClassNameForThisLevel = GetClassNameByHierarchyLevel (curClass->Name (), level);
          nextClassForThisLevel = MLClass::CreateNewMLClass (curClassNameForThisLevel);
        }
      }
    }
  }

  delete  allClasses;  allClasses = NULL;

  return  examplesLabeledForAppropriateLevel;
}  /* CreateListForAGivenLevel */






class  FeatureVectorList::BreakTieComparison
{

public:
   BreakTieComparison ()
   {}

   bool  operator() (FeatureVectorPtr  p1,  
                     FeatureVectorPtr  p2
                    )
   {
     return  (p1->BreakTie () < p2->BreakTie ());
   }
}; /* BreakTieComparison */



class  FeatureVectorList::BreakTieComparisonReversed
{

public:
   BreakTieComparisonReversed ()
   {}

   bool  operator() (FeatureVectorPtr  p1,  
                     FeatureVectorPtr  p2
                    )
   {
     return  (p1->BreakTie () > p2->BreakTie ());
   }
}; /* BreakTieComparisonReversed */





class  FeatureVectorList::ProbabilityComparison
{

public:
   ProbabilityComparison ()
   {}

   bool  operator() (FeatureVectorPtr  p1,  
                     FeatureVectorPtr  p2
                    )
   {
     return  (p1->Probability () < p2->Probability ());
   }
}; /* ProbabilityComparison */



class  FeatureVectorList::ProbabilityComparisonReversed
{

public:
   ProbabilityComparisonReversed ()
   {}

   bool  operator() (FeatureVectorPtr  p1,  
                     FeatureVectorPtr  p2
                    )
   {
     return  (p1->Probability () > p2->Probability ());
   }
}; /* ProbabilityComparisonReversed */




class  FeatureVectorList::ImageFileNameComparison
{
public:
  ImageFileNameComparison ()  {}


  bool  operator () (FeatureVectorPtr  p1,
                     FeatureVectorPtr  p2
                    )
  {
    return  (p1->ImageFileName () < p2->ImageFileName ());
  }
};  /* ImageFileNameComparison */




class  FeatureVectorList::ImageFileNameComparisonReversed
{
public:
  ImageFileNameComparisonReversed ()  {}


  bool  operator ()  (FeatureVectorPtr  p1,
                      FeatureVectorPtr  p2
                     )
  {
    return  (p1->ImageFileName () > p2->ImageFileName ());
  }

};  /* ImageFileNameComparison */






class  FeatureVectorList::RootNameComparrison
{
public:
  RootNameComparrison ()  {}

  bool  operator () (FeatureVectorPtr  p1,
                     FeatureVectorPtr  p2
                    )
  {
    KKStr  root1 = osGetRootNameWithExtension (p1->ImageFileName ());
    KKStr  root2 = osGetRootNameWithExtension (p2->ImageFileName ());

    return  (root1 < root2);
  }
};  /* RootNameComparrison */



class  FeatureVectorList::RootNameComparrisonReversed
{
public:
  RootNameComparrisonReversed ()  {}

  bool  operator() (FeatureVectorPtr  p1,
                    FeatureVectorPtr  p2
                   )
  {
    KKStr  root1 = osGetRootName (p1->ImageFileName ());
    KKStr  root2 = osGetRootName (p2->ImageFileName ());

    return  root1 > root2;
  }
};  /* RootNameComparrisonReversed */





class  FeatureVectorList::ClassNameComparrison
{
public:
  ClassNameComparrison ()  {}

  bool  operator()  (FeatureVectorPtr  p1,
                     FeatureVectorPtr  p2
                    )
  {
    if  (p1->MLClass () == NULL)
    {
      if  (p2->MLClass () != NULL)
        return  true;
    }

    else if  (p2->MLClass () != NULL)
    {
      const KKStr& n1 = p1->MLClass ()->UpperName ();
      const KKStr& n2 = p2->MLClass ()->UpperName ();

      if  (n1 < n2)
        return true;

      else if  (n1 > n2)
        return false;
    }

    return p1->ImageFileName () < p2->ImageFileName ();
  }
};  /* ClassNameComparrison */





class   FeatureVectorList::ClassNameComparrisonReversed
{
public:
  ClassNameComparrisonReversed ()  {}


  bool  operator()  (FeatureVectorPtr  p1,
                     FeatureVectorPtr  p2
                    )
  {
    if  (p1->MLClass () == NULL)
    {
      if  (p2->MLClass () != NULL)
        return  false;
    }

    else if  (p2->MLClass () != NULL)
    {
      const KKStr& n1 = p1->MLClass ()->UpperName ();
      const KKStr& n2 = p2->MLClass ()->UpperName ();

      if  (n1 < n2)
        return false;

      else if  (n1 > n2)
        return true;
    }

    return p1->ImageFileName () > p2->ImageFileName ();
  }
};  /* ClassNameComparrisonReversed */








void  FeatureVectorList::SortByRootName (bool  reversedOrder)
{
 if  (!reversedOrder)
  {
    RootNameComparrison  c;
    sort (begin (), end (), c);
  }
  else
  {
    RootNameComparrisonReversed  c;
    sort (begin (), end (), c);
  }

  curSortOrder = IFL_ByRootName;
}  /* SortByRootName */



void  FeatureVectorList::SortByClass (bool  reversedOrder)
{
 if  (!reversedOrder)
  {
    ClassNameComparrison  c;
    sort (begin (), end (), c);
  }
  else
  {
    ClassNameComparrisonReversed  c;
    sort (begin (), end (), c);
  }

  curSortOrder = IFL_ByClassName;
}  /* SortByClass */





void  FeatureVectorList::SortByProbability (bool  reversedOrder)
{
  if  (!reversedOrder)
  {
    ProbabilityComparison  c;
    sort (begin (), end (), c);
  }
  else
  {
    ProbabilityComparisonReversed  c;
    sort (begin (), end (), c);
  }

  curSortOrder = IFL_ByProbability;
}  /* SortByProbability */




void  FeatureVectorList::SortByBreakTie (bool  reversedOrder)
{
  if  (!reversedOrder)
  {
    BreakTieComparison   c;
    sort (begin (), end (), c);
  }
  else
  {
    BreakTieComparisonReversed  c;
    sort (begin (), end (), c);
  }

  curSortOrder = IFL_ByBreakTie;
}  /* SortByProbability */





void  FeatureVectorList::SortByImageFileName (bool  reversedOrder)
{
  if  (!reversedOrder)
  {
    ImageFileNameComparison  c;
    sort (begin (), end (), c);
  }
  else
  {
    ImageFileNameComparisonReversed  c;
    sort (begin (), end (), c);
  }

  curSortOrder = IFL_ByName;
}  /* SortByImageFileName */

