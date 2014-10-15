#include "FirstIncludes.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <sstream>
#include <iomanip>

#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace  KKU;


#include "FeatureEncoder2.h"
#include "BinaryClassParms.h"
#include "FeatureNumList.h"
#include "FeatureVector.h"
#include "SvmWrapper.h"
using namespace  MLL;

namespace  MLL
{

/**
 * @brief Constructs a Feature Encoder object.
 * @param[in] _param 
 * @param[in] _fileDesc
 * @param[in] _attributeTypes   caller keeps ownership,  but encoder will continue to reference it.
 * @param[in] _cardinalityTable caller keeps ownership,  but encoder will continue to reference it.
 * @param[in] _log A logfile stream. All important events will be ouput to this stream
 */
FeatureEncoder2::FeatureEncoder2 (const ModelParam&  _param,
                                  FileDescPtr        _fileDesc,
                                  RunLog&            _log
                                 ):

    attributeVector      (_fileDesc->AttributeVector ()),
    cardinalityDest      (NULL),
    cardinalityVector    (_fileDesc->CardinalityVector ()),
    codedNumOfFeatures   (0),
    compressionMethod    (ModelParam::BRnoCompression),
    destFeatureNums      (NULL),
    destWhatToDo         (NULL),
    encodedFileDesc      (NULL),
    encodingMethod       (ModelParam::NoEncoding),
    fileDesc             (_fileDesc),
    log                  (_log),
    nominal_counters     (NULL),  // used by Bitreduction.
    nominalCountersSize  (0),
    numOfFeatures        (0),
    srcFeatureNums       (NULL),
    param                (_param)
    
{
  log.Level (40) << "FeatureEncoder2::FeatureEncoder2" << endl;

  numOfFeatures = param.SelectedFeatures ().NumOfFeatures ();

  compressionMethod = param.CompressionMethod ();
  encodingMethod    = param.EncodingMethod ();

  srcFeatureNums   = new int32[numOfFeatures];
  cardinalityDest  = new int32[numOfFeatures];
  destFeatureNums  = new int32[numOfFeatures];
  destWhatToDo     = new FeWhatToDo[numOfFeatures];

  VectorKKStr   destFieldNames;

  int32  x;

  for  (x = 0;  x < numOfFeatures;  x++)
  {
    int32  srcFeatureNum = param.SelectedFeatures ()[x];
    srcFeatureNums   [x] = srcFeatureNum;
    destFeatureNums  [x] = codedNumOfFeatures;
    cardinalityDest  [x] = 1;
    destWhatToDo     [x] = FeAsIs;

    Attribute  srcAttribute = (fileDesc->Attributes ())[srcFeatureNum];

    switch (encodingMethod)
    {
      case  ModelParam::BinaryEncoding:
        if  ((attributeVector[srcFeatureNums[x]] == NominalAttribute)  || (attributeVector[srcFeatureNums[x]] == SymbolicAttribute))
        {
          destWhatToDo    [x] = FeBinary;
          cardinalityDest [x] = cardinalityVector[srcFeatureNums [x]];
          codedNumOfFeatures   += cardinalityDest[x];
          for  (int32 zed = 0;  zed < cardinalityDest[x];  zed++)
          {
            KKStr  fieldName = srcAttribute.Name () + "_" + srcAttribute.GetNominalValue (zed);
            destFieldNames.push_back (fieldName);
          }
        }
        else
        {
          codedNumOfFeatures++;
          destWhatToDo [x] = FeAsIs;
          destFieldNames.push_back (srcAttribute.Name ());
        }
        break;


      case  ModelParam::ScaledEncoding:
        codedNumOfFeatures++;
        if  ((attributeVector[srcFeatureNums[x]] == NominalAttribute)  ||  (attributeVector[srcFeatureNums[x]] == SymbolicAttribute))
          destWhatToDo [x] = FeScale;
        else
          destWhatToDo [x] = FeAsIs;

        destFieldNames.push_back (srcAttribute.Name ());
        break;


      case    ModelParam::NoEncoding:
      default:
        codedNumOfFeatures++;
        destWhatToDo [x] = FeAsIs;
        destFieldNames.push_back (srcAttribute.Name ());
        break;
    }
  }

  encodedFileDesc = FileDesc::NewContinuousDataOnly (log, destFieldNames);
}




FeatureEncoder2::FeatureEncoder2 (const FeatureEncoder2&  _encoder):
    attributeVector      (_encoder.attributeVector),
    cardinalityDest      (NULL),
    cardinalityVector    (_encoder.cardinalityVector),
    codedNumOfFeatures   (_encoder.codedNumOfFeatures),
    compressionMethod    (_encoder.compressionMethod),
    destFeatureNums      (NULL),
    destWhatToDo         (NULL),
    encodedFileDesc      (_encoder.encodedFileDesc),
    encodingMethod       (_encoder.encodingMethod),
    fileDesc             (_encoder.fileDesc),
    log                  (_encoder.log),
    nominal_counters     (NULL),  // used by Bitreduction.
    nominalCountersSize  (0),
    numOfFeatures        (_encoder.numOfFeatures),
    srcFeatureNums       (NULL),
    param                (_encoder.param)
    
{
  log.Level (30) << "FeatureEncoder2::FeatureEncoder2" << endl;

  cardinalityDest  = new int32[numOfFeatures];
  destFeatureNums  = new int32[numOfFeatures];
  destWhatToDo     = new FeWhatToDo[numOfFeatures];
  srcFeatureNums   = new int32[numOfFeatures];

  int32  x;
  for  (x = 0;  x < numOfFeatures;  x++)
  {
    int32  srcFeatureNum = param.SelectedFeatures () [x];
    srcFeatureNums   [x] = _encoder.srcFeatureNums [x];
    destFeatureNums  [x] = _encoder.destFeatureNums[x];
    cardinalityDest  [x] = _encoder.cardinalityDest[x];
    destWhatToDo     [x] = _encoder.destWhatToDo   [x];
    srcFeatureNums   [x] = _encoder.srcFeatureNums [x];
  }


  SetupNominalCounters ();
}



/**
 * @brief Frees any memory allocated by, and owned by the FeatureEncoder2
 */
FeatureEncoder2::~FeatureEncoder2 ()
{
  DestroyNominalCounters ();

  delete  srcFeatureNums;
  delete  destFeatureNums;
  delete  cardinalityDest;
  delete  destWhatToDo;
}


int32  FeatureEncoder2::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (FeatureEncoder2)
    +  attributeVector.size ()   * sizeof (AttributeType)
    +  cardinalityVector.size () * sizeof (int32)
    +  nominalCountersSize  * sizeof (int32);

  if  (cardinalityDest)   memoryConsumedEstimated += 3 * numOfFeatures * sizeof (int32);  // For 'cardinalityDest', 'destFeatureNums', and 'srcFeatureNums'
  if  (destFeatureNums)   memoryConsumedEstimated += numOfFeatures * sizeof (int32);
  if  (destWhatToDo)      memoryConsumedEstimated += numOfFeatures * sizeof (FeWhatToDo);
  if  (nominal_counters)  memoryConsumedEstimated += sizeof (int32*) * fileDesc->NumOfFields ();

  return  memoryConsumedEstimated;
}



int32  FeatureEncoder2::NumEncodedFeatures ()  const
{
  return  encodedFileDesc->NumOfFields ();
}




FileDescPtr  FeatureEncoder2::CreateEncodedFileDesc (ostream*  o)  const
{
  log.Level (40) << "FeatureEncoder2::CreateEncodedFileDesc" << endl;
  FileDescPtr  newFileDesc = new FileDesc ();

  if  (o)
  {
    *o << endl 
       << "Orig"     << "\t" << "Orig"      << "\t" << "Field" << "\t" << "Encoded"  << "\t" << "Encoded"   << endl;
    *o << "FieldNum" << "\t" << "FieldName" << "\t" << "Type"  << "\t" << "FieldNum" << "\t" << "FieldName" << endl;
  }

  int32  x;

  bool  alreadyExist;
  
  for  (x = 0;  x < numOfFeatures; x++)
  {
    int32  srcFeatureNum = srcFeatureNums[x];
    int32  y = destFeatureNums[x];

    if  (y >= codedNumOfFeatures)
    {
      log.Level(-1) 
           << endl
           << endl
           << "FeatureEncoder2::CreateEncodedFileDesc     *** ERROR ***"           << endl
           << "             overriding number of encoded features.  This should"  << endl
           << "             never be able to happen. Something is wrong with"     << endl
           << "             object."                                              << endl
           << endl;
      osWaitForEnter ();
      exit (-1);
    }

    KKStr  origFieldDesc = StrFormatInt (srcFeatureNum, "zz0") + "\t" +
                           fileDesc->FieldName (srcFeatureNum) + "\t" +
                           fileDesc->TypeStr   (srcFeatureNum);


    switch (destWhatToDo[x])
    {
    case  FeAsIs:
      {
        newFileDesc->AddAAttribute (fileDesc->FieldName (x), NumericAttribute, alreadyExist);
        if  (o)
        {
          *o << origFieldDesc          << "\t" 
             << y                      << "\t"
             << fileDesc->FieldName (x)
             << endl;
        }
      }
      break;

    case  FeBinary:
      {
        for  (int32 z = 0;  z < cardinalityDest[x];  z++)
        {
          KKStr  nominalValue = fileDesc->GetNominalValue (srcFeatureNums[x], z);
          KKStr  encodedName  = fileDesc->FieldName (x) + "_" + nominalValue;
          newFileDesc->AddAAttribute (encodedName, NumericAttribute, alreadyExist);
          if  (o)
          {
            *o << origFieldDesc << "\t" 
               << y             << "\t"
               << encodedName
               << endl;
          }

          y++;
        }
      }

      break;

    case  FeScale:
      {
        newFileDesc->AddAAttribute (fileDesc->FieldName (x), NumericAttribute, alreadyExist);
        if  (o)
        {
          *o << origFieldDesc << "\t" 
             << y             << "\t"
             << fileDesc->FieldName (x)
             << endl;
        }
      }
      break;
    }
  }

  newFileDesc = FileDesc::GetExistingFileDesc (newFileDesc);

  return  newFileDesc;
}  /* CreateEncodedFileDesc */





FeatureVectorPtr  FeatureEncoder2::EncodeAExample (FeatureVectorPtr  src)  const
{
  FeatureVectorPtr  encodedImage = new FeatureVector (codedNumOfFeatures);
  encodedImage->MLClass     (src->MLClass     ());
  encodedImage->PredictedClass (src->PredictedClass ());
  encodedImage->TrainWeight    (src->TrainWeight    ());

  const FFLOAT*  featureData = src->FeatureData ();
  int32  x;

  for  (x = 0;  x < numOfFeatures; x++)
  {
    FFLOAT  featureVal = featureData [srcFeatureNums[x]];
    int32  y = destFeatureNums[x];

    switch (destWhatToDo[x])
    {
    case  FeAsIs:
      {
        encodedImage->AddFeatureData (y, featureVal);
      }
      break;

    case  FeBinary:
      {
        for  (int32 z = 0; z < cardinalityDest[x]; z++)
        {
          FFLOAT  bVal = ((int32)featureVal == z);
          encodedImage->AddFeatureData (y, bVal);
          y++;
        }
      }

      break;

    case  FeScale:
      {
        encodedImage->AddFeatureData (y, (featureVal / (FFLOAT)cardinalityDest[x]));
      }
      break;
    }
  }

  return  encodedImage;
}  /* EncodeAExample */





FeatureVectorListPtr  FeatureEncoder2::EncodeAllExamples (const FeatureVectorListPtr  srcData)
{
  FeatureVectorListPtr  encodedExamples = new FeatureVectorList (encodedFileDesc, 
                                                                 true,                  // Will own the contents 
                                                                 log,  
                                                                 srcData->QueueSize ()
                                                                );

  FeatureVectorList::const_iterator  idx;

  for  (idx = srcData->begin ();  idx !=  srcData->end ();   idx++)
  {
    const FeatureVectorPtr srcExample = *idx;
    FeatureVectorPtr  encodedExample = EncodeAExample (srcExample);
    encodedExamples->PushOnBack (encodedExample);
  }

  return  encodedExamples;
}  /* EncodeAllImages */






/**
 * @brief Attempts to reduce the number of images in the images_list via compression
 *        if no compression specified will just copy refrences to images to compressed
 *        list, and flag as Opwner(false).
 * @param[in] images_list The list of images you want to attempt to reduce
 * @param[out] compressed_images_list The reduced list of images
 * @return a CompressionStats struct containing the details of the compression
 */
CompressionStats  FeatureEncoder2::CompressExamples (const FeatureVectorListPtr  src,
                                                     FeatureVectorListPtr&       dest
                                                    )
{
  CompressionStats  stats;

  switch  (compressionMethod)
  {
  case ModelParam::BRcompressionPost:
    {
      stats = compress (*src, dest);
      dest->Owner (true);
    }
    break;

  case  ModelParam::BRunBalancedVariance:
  case  ModelParam::BRunBalancedSpecified:
    {
      stats = compress (*src, dest);
      dest->Owner (true);
    }
    break;


  default:
    double time_before, time_after;
    
    time_before = osGetSystemTimeUsed ();
    dest->AddQueue (*src);
    time_after = osGetSystemTimeUsed ();
    dest->Owner (false);

    stats.num_images_before = (long)src->size();
    stats.num_images_after  = (long)dest->size();
    stats.compression_ratio = 0.0f;
    stats.elapsed_cpu_time = time_after-time_before;
    break;
  }

  log.Level (20) << "CompressImages: # images before was "   << stats.num_images_before  << endl;
  log.Level (20) << "CompressImages: # images after was "    << stats.num_images_after   << endl;
  log.Level (20) << "CompressImages: Compression ratio was " << stats.compression_ratio  << endl;
  log.Level (20) << "CompressImages: Reduction Time was "    << stats.elapsed_cpu_time   << endl;

  return stats;
}  /* CompressImages */




FeatureVectorListPtr  FeatureEncoder2::EncodedFeatureVectorList (const FeatureVectorList&  srcData)  const
{
  if  (srcData.AllFieldsAreNumeric ())
    return  srcData.DuplicateListAndContents ();

  FeatureVectorListPtr  encodedFeatureVectorList = new FeatureVectorList (encodedFileDesc, true, log, srcData.QueueSize ());

  FeatureVectorList::const_iterator  idx;
  for  (idx = srcData.begin ();   idx != srcData.end ();  idx++)
  {
    FeatureVectorPtr  srcExample = *idx;
    FeatureVectorPtr  encodedFeatureVector = EncodeAExample (srcExample);
    encodedFeatureVector->MLClass (srcExample->MLClass ());
    encodedFeatureVectorList->PushOnBack (encodedFeatureVector);
  }

  return  encodedFeatureVectorList;
}  /* EncodedFeatureVectorList */







/**
 * @brief Performs bit reduction on the parameter p
 * @param[in,out] p The svm_problem you wish to apply bit reduction to
 * @param[in] param Needed for various reasons. Just accept it
 * @param[out] ratio The compression ratio
 */
CompressionStats  FeatureEncoder2::compress (const FeatureVectorList&  examples,
                                             FeatureVectorListPtr      compressed_example_list
                                            )
{
  double time_before, time_after;

  FileDescPtr file_desc = examples.FileDesc ();

  int32  numSelFeatures    = param.SelectedFeatures ().NumOfFeatures ();
  int32  featursPerExample = file_desc->NumOfFields ();

  time_before = osGetSystemTimeUsed();

  ClassIndexList  classIndexes;

  CompressionStats stats;

  int32 nodeLength     = numSelFeatures;
  int32 hashSize       = int32(examples.QueueSize() * 1.5);

  ChainHashNode::setLength           (nodeLength);
  ChainHashNode::setHashSize         (hashSize);
  ChainHashNode::SetSelectedFeatures (param.SelectedFeatures ());
  ChainHashNode::SetFeatureTypes     (attributeVector);

  KKStr  unBalFeatureStr;

  VectorInt  bitsToReduceByFeature;

  if  (param.CompressionMethod () == ModelParam::BRunBalancedVariance)
    bitsToReduceByFeature = DeriveBitReductionForEachFeatureByVariance  (examples, unBalFeatureStr);

  else if  (param.CompressionMethod () == ModelParam::BRunBalancedSpecified)
    bitsToReduceByFeature = DeriveBitReductionForEachFeatureFromSpecifoedParameter ();

  else
    bitsToReduceByFeature = DeriveBitReductionPlain (examples);


  ChainHashNode::SetBitsToReduceByFeature (bitsToReduceByFeature);

  ChainHashNode notFound (0, 0, 0);

  // build the hash table
  ChainHash<ChainHashNode> ch (notFound, hashSize);
  FeatureVectorList::const_iterator  i;
  FeatureVectorPtr example;
  int32 example_index = 0;
  bool success = false;  // indicates if example was succesfully inserted into hash table

  example_index = 0;

  MLClassConstPtr  lastMLClass = NULL;
  int32               lastClassNum   = -1;

  for (i = examples.begin ();  i != examples.end ();  ++i)
  {
    example = *i;
    if  (example->MLClass () != lastMLClass)
    {
      lastMLClass = example->MLClass ();
      lastClassNum   = classIndexes.GetClassIndex (lastMLClass);
    }

    ChainHashNodePtr  node 
      = new ChainHashNode(example_index, 
                          lastClassNum, 
                          example,
                          success
                         );
    ch.insert (node);
    example_index++;
  }

  ch.DoSomeStats ();


  // setup the nominal counters  
  SetupNominalCounters ();

  // walk the hash table and build the newly compressed example list
  vector< list<int32> >&                  bucketList  = ch.getBucketList();
  vector< list<int32> >::const_iterator   it; 
  list<int32>::const_iterator             it2;

  //int32  num_buckets = bucketList.size();
  int32  size        = 1;

  // each entry in the bucketList contains a list of examples that need to be merged
  for  (it = bucketList.begin(); it != bucketList.end(); it++)
  {
    // create a new example using the feature values from the first example in the current bucket
    size = 1;
    it2 = it->begin();
    //FFLOAT *crap = (*features_list)[*it2].FeatureData ( );
    FeatureVectorPtr example = new FeatureVector (featursPerExample);
    
    
    //FeatureVectorPtr srcExample = features_list->IdxToPtr (*it2);
    FeatureVectorPtr srcExample = examples.IdxToPtr (*it2);


    example->MLClass (srcExample->MLClass ());
    add (example, srcExample);
    it2++;

    // now add in the rest the features (cumulatively)
    for(; it2 != it->end(); it2++)
    {
      //srcExample = features_list->IdxToPtr (*it2);
      srcExample = examples.IdxToPtr (*it2);
      add (example, srcExample);
      size++;
    }

    divide (example, size);
    example->TrainWeight ((float)size);
    ZeroNominalCounters ();

    compressed_example_list->PushOnBack (example);
  }

  //stats.num_images_before = features_list->size();
  stats.num_images_before = (long)examples.size();

  stats.num_images_after  = (long)compressed_example_list->size();
  stats.compression_ratio = (float)stats.num_images_after / (float)stats.num_images_before;

  // free the memory used for the nominal counters
  DestroyNominalCounters ();

  time_after = osGetSystemTimeUsed();
  stats.elapsed_cpu_time = time_after - time_before;

  stats.bitReductionByFeature = BitReductionByFeature (bitsToReduceByFeature);
  stats.unBalFeatures         = unBalFeatureStr;

  return stats;
}  /* compress */




KKStr  FeatureEncoder2::BitReductionByFeature (VectorInt&  bitsToReduceByFeature)
{
  KKStr  o (100);

  int32  x;

  for  (x = 0;  x < param.SelectedFeatures ().NumOfFeatures ();  x++)
  {
    if  (x > 0)
      o << ", ";
    o << param.SelectedFeatures ()[x] << "(" << bitsToReduceByFeature[x] << ")";
  }

  return  o;
}  /* BitReductionByFeature */



//effects: divided each feature of example by t
void FeatureEncoder2::divide (FeatureVectorPtr example, double t)
{
  FFLOAT*  feature_data = example->FeatureDataAlter ();
  for (int32 i = 0;  i < example->NumOfFeatures ();  i++)
  {
    if (((attributeVector[i] == NominalAttribute) || (attributeVector[i] == SymbolicAttribute))  &&  (encodingMethod != ModelParam::NoEncoding))
    {
      feature_data[i] = (FFLOAT)NominalFeatureAverage (i);
    }
    else 
    {
      feature_data[i] /= (FFLOAT)t;
    }
  }
}  /* divide */




// effects: adds the feature values of b to the feature values of a
void  FeatureEncoder2::add (      FeatureVectorPtr a, 
                            const FeatureVectorPtr b
                           )
{
  FFLOAT*        feature_data_a = a->FeatureDataAlter ();
  const FFLOAT*  feature_data_b = b->FeatureData      ();
  
  for (int32 i = 0;  i < a->NumOfFeatures();  i++)
  {
    if ((
          (attributeVector[i] == NominalAttribute)  ||  (attributeVector[i] == SymbolicAttribute)
        )  &&  
        (encodingMethod != ModelParam::NoEncoding)
       )
    {
      IncrementNominalCounters (i, (int32)feature_data_b[i]);
    }
    else
    {
      feature_data_a[i] += feature_data_b[i];
    }
  }
}  /* Add */





void  FeatureEncoder2::ZeroNominalCounters ()
{
  if  (!nominal_counters)
    return;

  for  (uint32 i = 0;  i < fileDesc->NumOfFields ();  i++)
  {
    if  ((attributeVector[i] != NominalAttribute)  &&  (attributeVector[i] != SymbolicAttribute))
      continue;

    for (int32 j = 0;  j < cardinalityVector[i];  j++)
    {
      nominal_counters[i][j] = 0;
    }
  }
}  /* ZeroNominalCounters */




void FeatureEncoder2::DestroyNominalCounters ()
{
  if (!nominal_counters)
    return;

  for (uint32 i = 0;  i < fileDesc->NumOfFields ();  i++)
  {
    if ((attributeVector[i] == NominalAttribute)  ||  (attributeVector[i] == SymbolicAttribute))
    {
      delete [] nominal_counters[i];
    }
    nominal_counters[i] = NULL;
  }

  delete [] nominal_counters;
  nominal_counters = NULL;
}  /* DestroyNominalCounters */




void FeatureEncoder2::SetupNominalCounters ()
{
  // destroy the existing counters, if they exist
  DestroyNominalCounters ();

  // create the new counters
  nominal_counters = new int32 *[fileDesc->NumOfFields ()];
  for (uint32 i = 0;  i < fileDesc->NumOfFields ();  i++)
  {
    if  ((attributeVector[i] == NominalAttribute)  ||  (attributeVector[i] == SymbolicAttribute))
    {
      nominal_counters[i] = new int32[cardinalityVector[i]];
      nominalCountersSize += cardinalityVector[i];
    }
    else
      nominal_counters[i] = NULL;
  }

  // set all of the counters to zero
  ZeroNominalCounters ();
}



void  FeatureEncoder2::IncrementNominalCounters (uint32 feature_num, 
                                                 int32  value
                                                )
{
  if (feature_num >= fileDesc->NumOfFields ())
  {
    return;
  }

  if  ((value < 0)  ||  (value >= fileDesc->Cardinality (feature_num, log)))
  {
    log.Level (-1) << endl << endl  << "FeatureEncoder2::IncrementNominalCounters   ***ERROR***  Unable to increment the nominal counter: Bad value" << endl << endl;
    osWaitForEnter ();
    exit(-1);
  }

  nominal_counters[feature_num][value]++;
}  /* IncrementNominalCounters */




int32  FeatureEncoder2::NominalFeatureAverage (uint32  feature_num)
{
  int32 max = -1;
  int32 max_index = -1;

  if (feature_num >= fileDesc->NumOfFields () || (feature_num < 0))
  {
    return max;
  }

  for (int32 j = 0;  j < fileDesc->Cardinality (feature_num, log);  j++)
  {
    if (nominal_counters[feature_num][j] > max)
    {
       max = nominal_counters[feature_num][j];
       max_index = j;
    }
  }

  return max_index;
}  /* NominalFeatureAverage */







struct  FeatureEncoder2::FeatureVar2
{
  FeatureVar2 (int32          _featureNum,  
              AttributeType  _attributeType,
              int32          _idx,  
              double         _var
             ):
          attributeType (_attributeType),
          featureNum    (_featureNum), 
          idx           (_idx),
          var           (_var)
        {}

    MLL::AttributeType  attributeType;
    int32               featureNum;
    int32               idx;
    double              var;
};


//typedef  FeatureEncoder2::FeatureVar2* FeatureVar2Ptr;

class  FeatureEncoder2::FeatureVar2List: public KKQueue<FeatureEncoder2::FeatureVar2>
{
public:
  FeatureVar2List (bool _owner):
     KKQueue<FeatureVar2> (_owner)
     {}

  ~FeatureVar2List () 
  {}
};





class  FeatureEncoder2::FeatureVarComparrison2
{
public:
  FeatureVarComparrison2 ()  {};

  int32  AttributeValue (AttributeType  attributeType)
  {
    if  (attributeType == NumericAttribute)
      return 0;
    
    else if  (attributeType == OrdinalAttribute)
      return 1;

    else if  (attributeType == NominalAttribute)
      return 2;

    else if  (attributeType == SymbolicAttribute)
      return 3;

    else
      return 9;
  }
    
    
  bool  operator () (FeatureVar2Ptr  p1,
                     FeatureVar2Ptr  p2
                    )
  {
    int32 p1AttrVal = AttributeValue (p1->attributeType);
    int32 p2AttrVal = AttributeValue (p2->attributeType);

    if  (p1AttrVal < p2AttrVal)
      return true;

    if  (p1AttrVal > p2AttrVal)
      return false;

    else
      return  (p1->var < p2->var);
  }
};  /* FeatureVarComparrison2 */



/**
 * @brief Uses one 'BitsToReduceBy' value for all features.
 * @param[in] Not Used
 * @param[in] Not Used
 */
VectorInt  FeatureEncoder2::DeriveBitReductionPlain (const FeatureVectorList&  examples)
{
  int32 numOfFeatures = param.SelectedFeatures ().NumOfFeatures ();
  VectorInt  bitsToReduceBy (numOfFeatures, param.BitsToReduceBy ());
  return  bitsToReduceBy;
}
                                                      


VectorInt  FeatureEncoder2::DeriveBitReductionForEachFeatureFromSpecifoedParameter ()
{
  int32 numSelFeatures = param.SelectedFeatures ().NumOfFeatures ();
  VectorInt  bitsToReduceByFeature (numSelFeatures, param.BitsToReduceBy ());

  KKStr  s = param.UnBalancedBitsStr ();

  while  (!s.Empty ())
  {
    int32  featureNum = s.ExtractTokenInt (",");
    if  ((featureNum < 0)  ||  (featureNum >= numSelFeatures))
    {
      cerr << endl << endl
           << "FeatureEncoder2::DeriveBitReductionForEachFeatureFromSpecifoedParameter     ***WARNING***" << endl
           << endl
           << "*** Invalid Unbalanced Feature Specified[" << featureNum << "]" << endl
           << endl;
    }
    else
    {
      bitsToReduceByFeature[featureNum]++;
    }
  }

  return  bitsToReduceByFeature;
}  /* DeriveBitReductionForEachFeatureFromSpecifoedParameter */



//*************************************************************************
//*  Determines how many bits to reduce each feature by.  It is meant to  *
//*  be called when BRunBalancedVariance is specified in SvmParam.  The   *
//*  features with the lowest variance are to be reduced by one extra     *
//*  bit.  The vector returened specifues the num of bits by feature to   *
//*  reduce by.                                                           *
//*                                                                       *
//*  The index to the returned vector will be with respect to the selected*
//*  features.  Ex: selectdFeaturs = {0,2,5,7,8},                         *
//*                 Returned Vector  {7,7,8,8,7}                          *
//*************************************************************************
VectorInt  FeatureEncoder2::DeriveBitReductionForEachFeatureByVariance (const FeatureVectorList&  examples,
                                                                              KKStr&              unBalFeatures
                                                                       )
                                                     
{
  int32 idx;
  int32 numSelFeatures = param.SelectedFeatures ().NumOfFeatures ();

  FeatureVar2List  featureVariances (true);

  vector<int32>     totals    (numSelFeatures, 0);
  vector<double>  means     (numSelFeatures, 0.0);
  vector<double>  sqrTotals (numSelFeatures, 0.0);

  int32  numBitsToReduceBy = param.BitsToReduceBy ();
  //int32  numBitsToReduceBy = 8;

  FeatureVectorList::const_iterator imageIDX;
  for  (imageIDX = examples.begin ();  imageIDX != examples.end ();  imageIDX++)
  {
    FeatureVectorPtr  example = *imageIDX;

    const FFLOAT*  featureData = example->FeatureData ();

    for  (idx = 0;  idx < numSelFeatures;  idx++)
    {
      int32  featureNum = param.SelectedFeatures ()[idx];
    
      int32 featureDataInt = int32 (featureData[featureNum] * 100000.0);
      if  (featureDataInt > 0)
        featureDataInt = featureDataInt >> numBitsToReduceBy;
      else
        featureDataInt = -1 * ((-1 * featureDataInt) >> numBitsToReduceBy);

      totals[idx] += featureDataInt;
    }
  }

  for  (idx = 0;  idx < numSelFeatures;  idx++)
  {
    means[idx] = double (totals[idx]) / double (examples.QueueSize ());
  }


  for  (imageIDX = examples.begin ();  imageIDX != examples.end ();  imageIDX++)
  {
    FeatureVectorPtr  example = *imageIDX;

    const FFLOAT*  featureData = example->FeatureData ();

    for  (idx = 0;  idx < numSelFeatures;  idx++)
    {
      int32  featureNum = param.SelectedFeatures ()[idx];

      int32 featureDataInt = int32 (featureData[featureNum] * 100000.0);
      if  (featureDataInt > 0)
        featureDataInt = featureDataInt >> numBitsToReduceBy;
      else
        featureDataInt = -1 * ((-1 * featureDataInt) >> numBitsToReduceBy);

      double  delta = means[idx] - double (featureDataInt);
      sqrTotals[idx] += delta * delta;
    }
  }


  cout << endl << "Variances:";
  for  (idx = 0;  idx < numSelFeatures;  idx++)
  {
    int32  featureNum = param.SelectedFeatures ()[idx];
    double  var = sqrTotals[idx] / double (examples.QueueSize ());
    featureVariances.PushOnBack (new FeatureVar2 (featureNum, attributeVector[featureNum], idx, var));
    cout << ", " << var;
  }
  cout << endl;


  FeatureVarComparrison2  comp;
  sort (featureVariances.begin (),  featureVariances.end (), comp);

  VectorInt  bitsToReduceByFeature (numSelFeatures, param.BitsToReduceBy ());
  int32  numToReduceByOneMore = int32 (param.UnBalancedBits ());

  unBalFeatures = "";

  cout << endl << endl << "UnBAl Bit Order:";
  for (int32 x = 0;  x < numToReduceByOneMore; x++)
  {
    //int32 z = numSelFeatures - (x + 1);
    int32 z = x;  // reduce features that have smallest var by 1 extra bit.
    idx = featureVariances[z].idx;
    bitsToReduceByFeature[idx]++;
    cout << ", " << idx;

    if  (x > 0)
      unBalFeatures << ",";
    unBalFeatures << featureVariances[z].featureNum;
  }
  cout << endl;

  return  bitsToReduceByFeature;
} /* DeriveBitReductionForEachFeatureByVariance */


} /* MLL */
