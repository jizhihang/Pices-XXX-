#include "FirstIncludes.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <string>
#include <iomanip>

#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace  KKU;


#include "FeatureEncoder.h"
#include "BitReduction.h"
#include "BinaryClassParms.h"
#include "FeatureNumList.h"
#include "FeatureVector.h"
#include "SvmWrapper.h"
using namespace  MLL;


/**
 * @brief Constructs a Feature Encoder object.
 * @param[in] _svmParam 
 * @param[in] _fileDesc 
 * @param[in] _class1 
 * @param[in] _class2 
 * @param[in] _log A logfile stream. All important events will be ouput to this stream
 */
FeatureEncoder::FeatureEncoder (const SVMparam&       _svmParam,
                                FileDescPtr           _fileDesc,
                                AttributeTypeVector&  _attributeTypes,
                                vector<int32>&        _cardinalityTable,
                                MLClassConstPtr    _class1,
                                MLClassConstPtr    _class2,
                                RunLog&               _log
                               ):

    attributeTypes           (_attributeTypes),
    cardinalityDest          (NULL),
    cardinalityTable         (_cardinalityTable),
    class1                   (_class1),
    class2                   (_class2),
    codedNumOfFeatures       (0),
    compressionMethod        (BRnoCompression),
    destFeatureNums          (NULL),
    destFileDesc             (NULL),
    destWhatToDo             (NULL),
    encodingMethod           (NoEncoding),
    fileDesc                 (_fileDesc),
    log                      (_log),
    numEncodedFeatures       (0),
    numOfFeatures            (0),
    selectedFeatures         (_fileDesc),
    srcFeatureNums           (NULL),
    svmParam                 (_svmParam),
    xSpaceNeededPerImage     (0)
    
{
  log.Level (40) << "FeatureEncoder::FeatureEncoder" << endl;

  if  (class1  &&  class2)
    selectedFeatures = svmParam.GetFeatureNums (class1, class2);
  else
    selectedFeatures = svmParam.GetFeatureNums ();

  numOfFeatures = selectedFeatures.NumOfFeatures ();

  compressionMethod = svmParam.CompressionMethod ();
  encodingMethod    = svmParam.EncodingMethod ();

  xSpaceNeededPerImage = 0;
  srcFeatureNums   = new int32[numOfFeatures];
  cardinalityDest  = new int32[numOfFeatures];
  destFeatureNums  = new int32[numOfFeatures];
  destWhatToDo     = new FeWhatToDo[numOfFeatures];

  VectorKKStr   destFieldNames;

  int32  x;

  for  (x = 0;  x < numOfFeatures;  x++)
  {
    int32  srcFeatureNum = selectedFeatures[x];
    srcFeatureNums    [x] = srcFeatureNum;
    destFeatureNums   [x] = xSpaceNeededPerImage;
    cardinalityDest   [x] = 1;
    destWhatToDo      [x] = FeAsIs;

    Attribute  srcAttribute = (fileDesc->Attributes ())[srcFeatureNum];

    switch (encodingMethod)
    {
      case BinaryEncoding:
        if  ((attributeTypes[srcFeatureNums[x]] == NominalAttribute)  || (attributeTypes[srcFeatureNums[x]] == SymbolicAttribute))
        {
          destWhatToDo    [x] = FeBinary;
          cardinalityDest [x] = cardinalityTable[srcFeatureNums [x]];
          xSpaceNeededPerImage += cardinalityDest[x];
          numEncodedFeatures   += cardinalityDest[x];
          for  (int32 zed = 0;  zed < cardinalityDest[x];  zed++)
          {
            KKStr  fieldName = srcAttribute.Name () + "_" + srcAttribute.GetNominalValue (zed);
            destFieldNames.push_back (fieldName);
          }
        }
        else
        {
          xSpaceNeededPerImage++;
          numEncodedFeatures++;
          destWhatToDo [x] = FeAsIs;
          destFieldNames.push_back (srcAttribute.Name ());
        }
        break;


      case  ScaledEncoding:
        xSpaceNeededPerImage++;
        numEncodedFeatures++;
        if  ((attributeTypes[srcFeatureNums[x]] == NominalAttribute)  ||  (attributeTypes[srcFeatureNums[x]] == SymbolicAttribute))
          destWhatToDo [x] = FeScale;
        else
          destWhatToDo [x] = FeAsIs;

        destFieldNames.push_back (srcAttribute.Name ());
        break;


      case     NoEncoding:
      default:
        xSpaceNeededPerImage++;
        numEncodedFeatures++;
        destWhatToDo [x] = FeAsIs;
        destFieldNames.push_back (srcAttribute.Name ());
        break;
    }
  }

  codedNumOfFeatures = xSpaceNeededPerImage;

  destFileDesc = FileDesc::NewContinuousDataOnly (log, destFieldNames);

  xSpaceNeededPerImage++;  // Add one more for the terminating (-1)
}







/**
 * @brief Frees any memory allocated by, and owned by the FeatureEncoder
 */
FeatureEncoder::~FeatureEncoder ()
{
  delete  srcFeatureNums;
  delete  destFeatureNums;
  delete  cardinalityDest;
  delete  destWhatToDo;
}



int32  FeatureEncoder::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (FeatureEncoder) 
    + attributeTypes.size () * sizeof (AttributeType)
    + selectedFeatures.MemoryConsumedEstimated ()
    + numOfFeatures * sizeof (int32);

  if  (cardinalityDest)
    memoryConsumedEstimated += 3 * sizeof (int32) * numOfFeatures;    // cardinalityDest + destFeatureNums + srcFeatureNums

  //  We do not own 'destFileDesc'  and  'fileDesc'
  if  (destWhatToDo)
    memoryConsumedEstimated += sizeof (FeWhatToDo) * numOfFeatures;

  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */




FileDescPtr  FeatureEncoder::CreateEncodedFileDesc (ostream*  o)
{
  log.Level (40) << "FeatureEncoder::CreateEncodedFileDesc" << endl;
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

    if  (y >= numEncodedFeatures)
    {
      cerr << endl
           << endl
           << "FeatureEncoder::CreateEncodedFileDesc     *** ERROR ***"           << endl
           << "             overriding number of encoded features.  This should"  << endl
           << "             never be able to happen. Something is wrong with"     << endl
           << "             object."                                              << endl
           << endl;
      osWaitForEnter ();
      exit (-1);
    }

    KKStr  origFieldDesc = StrFormatInt (srcFeatureNum, "zz0") + "\t" +
                            fileDesc->FieldName (srcFeatureNum) + "\t" +
                            fileDesc->TypeStr (srcFeatureNum);


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
          *o << origFieldDesc           << "\t" 
             << y                       << "\t"
             << fileDesc->FieldName (x)
             << endl;
        }
      }
      break;
    }
  }

  return  newFileDesc;
}  /* CreateEncodedFileDesc */






/**
 * @brief Converts a single image into the svm_problem format, using the method specified 
 * by the EncodingMethod() value returned by svmParam
 * @param[in] image That we're converting
 */
XSpacePtr  FeatureEncoder::EncodeAImage (FeatureVectorPtr  image)
{
  // XSpacePtr  xSpace  = (struct svm_node*)malloc (xSpaceNeededPerImage * sizeof (struct svm_node));
  XSpacePtr  xSpace  = new svm_node[xSpaceNeededPerImage];
  int32  xSpaceUsed = 0;
  EncodeAImage (image, xSpace, xSpaceUsed);
  return  xSpace;
}  /* EncodeAImage */




FeatureVectorPtr  FeatureEncoder::EncodeAImage (FileDescPtr       encodedFileDesc,
                                                FeatureVectorPtr  src
                                               )
{
  FeatureVectorPtr  encodedImage = new FeatureVector (numEncodedFeatures);
  encodedImage->MLClass     (src->MLClass     ());
  encodedImage->PredictedClass (src->PredictedClass ());
  //encodedImage->Version        (src->Version        ());
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
}  /* EncodeAImage */





FeatureVectorListPtr  FeatureEncoder::EncodeAllExamples (const FeatureVectorListPtr  srcData)
{
  FileDescPtr  encodedFileDesc = CreateEncodedFileDesc (NULL);

  FeatureVectorListPtr  encodedExamples = new FeatureVectorList (encodedFileDesc, 
                                                                 true,                  // Will own the contents 
                                                                 log,  
                                                                 srcData->QueueSize ()
                                                                );

  FeatureVectorList::const_iterator  idx;

  for  (idx = srcData->begin ();  idx !=  srcData->end ();   idx++)
  {
    const FeatureVectorPtr srcExample = *idx;
    FeatureVectorPtr  encodedExample = EncodeAImage (encodedFileDesc, srcExample);
    encodedExamples->PushOnBack (encodedExample);
  }

  return  encodedExamples;
}  /* EncodeAllImages */




/**
 * @brief Converts a single image into the svm_problem format, using the method specified 
 * by the EncodingMethod() value returned by svmParam
 * @param[in] The image That we're converting
 * @param[in] The row int32 he svm_problem structue that the converted data will be stored
 */
void  FeatureEncoder::EncodeAImage (FeatureVectorPtr  image,
                                    svm_node*         xSpace,
                                    int32&            xSpaceUsed
                                   )
{
  const FFLOAT*  featureData = image->FeatureData ();
  int32  x;

  xSpaceUsed = 0;

  for  (x = 0;  x < numOfFeatures; x++)
  {
    FFLOAT  featureVal = featureData [srcFeatureNums[x]];
    int32  y = destFeatureNums[x];

    if  (y >= xSpaceNeededPerImage)
    {
      cerr << endl
           << endl
           << "FeatureEncoder::EncodeAImage     *** ERROR ***"  << endl
           << "             We are overring end of xSpace"      << endl
           << endl;
      osWaitForEnter ();
      exit (-1);
    }

    switch (destWhatToDo[x])
    {
    case  FeAsIs:
      {
        if  (featureVal != 0.0)
        {
          xSpace[xSpaceUsed].index = y;
          xSpace[xSpaceUsed].value = featureVal;
          xSpaceUsed++;
        }
      }
      break;

    case  FeBinary:
      {
        for  (int32 z = 0; z < cardinalityDest[x]; z++)
        {
          FFLOAT  bVal = ((int32)featureVal == z);
          if  (bVal != 0.0)
          {
            xSpace[xSpaceUsed].index = y;
            xSpace[xSpaceUsed].value = bVal;
            xSpaceUsed++;
          }
          y++;
        }
      }

      break;

    case  FeScale:
      {
        if  (featureVal != (FFLOAT)0.0)
        {
          xSpace[xSpaceUsed].index = y;
          xSpace[xSpaceUsed].value = featureVal / (FFLOAT)cardinalityDest[x];
          xSpaceUsed++;
        }
      }
      break;
    }
  }

  xSpace[xSpaceUsed].index = -1;
  xSpace[xSpaceUsed].value = -1;
  xSpaceUsed++;
}  /* EncodeAImage */



int32  FeatureEncoder::DetermineNumberOfNeededXspaceNodes (FeatureVectorListPtr   src)  const
{
  int32  xSpaceNodesNeeded = 0;
  FeatureVectorList::const_iterator  idx;
  for  (idx = src->begin ();  idx != src->end ();  ++idx)
  {
    FeatureVectorPtr fv = *idx;
    const FFLOAT*  featureData = fv->FeatureData ();

    for  (int32 x = 0;  x < numOfFeatures; x++)
    {
      FFLOAT  featureVal = featureData [srcFeatureNums[x]];
      int32  y = destFeatureNums[x];
  
      switch (destWhatToDo[x])
      {
      case  FeAsIs:
        if  (featureVal != 0.0)
          xSpaceNodesNeeded++;
        break;

      case  FeBinary:
        for  (int32 z = 0; z < cardinalityDest[x]; z++)
        {
          FFLOAT  bVal = ((int32)featureVal == z);
          if  (bVal != 0.0)
            xSpaceNodesNeeded++;
          y++;
         }
         break;

      case  FeScale:
         if  (featureVal != (FFLOAT)0.0)
           xSpaceNodesNeeded++;
         break;
      }
    }
    xSpaceNodesNeeded++;
  }

  return xSpaceNodesNeeded;
}  /* DetermineNumberOfNeededXspaceNodes */



CompressionStats  FeatureEncoder::Compress (FeatureVectorListPtr   src,
                                            ClassAssignments&      assignments,
                                            XSpacePtr&             xSpace,          
                                            int32&                 totalxSpaceUsed,
                                            struct svm_problem&    prob
                                           )

{
  FeatureVectorListPtr  compressedImages    = NULL;
  FeatureVectorListPtr  imagesToUseFoXSpace = NULL;
  CompressionStats      stats;
  int32                   xSpaceUsed = 0;

  totalxSpaceUsed = 0;

  if  ((compressionMethod == BRcompressionPost)   ||
       (compressionMethod == BRunBalancedVariance) 
      )
  {
    compressedImages = new FeatureVectorList (fileDesc, true, log);
    stats = CompressImages (src, compressedImages, assignments);
    imagesToUseFoXSpace = compressedImages;
  }
  else
  {
    imagesToUseFoXSpace = src;

    stats.elapsed_cpu_time  = 0.0f;
    stats.num_images_before = 0;
    stats.num_images_after  = 0;
    stats.compression_ratio = 1.0f;
  }

  int32  numOfExamples = imagesToUseFoXSpace->QueueSize ();
  int32  elements      = numOfExamples * xSpaceNeededPerImage;

  prob.l     = numOfExamples;
  prob.y     = (double*)malloc  (prob.l * sizeof (double));
  prob.x     = (struct svm_node **) malloc (prob.l * sizeof (struct svm_node*));
  prob.index = new int32[prob.l];
  prob.exampleNames.clear ();

  int32  numNeededXspaceNodes = DetermineNumberOfNeededXspaceNodes (imagesToUseFoXSpace);

  int32  totalBytesForxSpaceNeeded = (numNeededXspaceNodes + 10) * sizeof (struct svm_node);  // I added '10' to elements because I am paranoid

  xSpace = (struct svm_node*) malloc (totalBytesForxSpaceNeeded);
  if  (xSpace == NULL)
  {
    log.Level (-1) << endl << endl << endl
                   << " FeatureEncoder::Compress   *** Failed to allocates space for 'xSpace' ****" << endl
                   << endl
                   << "     Space needed          [" << totalBytesForxSpaceNeeded << "]" << endl
                   << "     Num of Examples       [" << numOfExamples             << "]" << endl
                   << "     Num XSpaceNodesNeeded [" << numNeededXspaceNodes      << "]" << endl
                   << endl;
    // we sill have to allocate space for each individule training example seperatly.
    //throw "FeatureEncoder::Compress     Allocation of memory for xSpace Failed.";
  }

  if  (svmParam.WeightBitReduction ())
    prob.W = new double[prob.l];
  else
    prob.W = NULL;

  //prob.W = new double[prob.l];

  int32 i = 0;

  FeatureVectorPtr    image          = NULL;
  MLClassConstPtr  lastMLClass = NULL;
  short               lastClassNum   = -1;

  int32  bytesOfxSpacePerImage = xSpaceNeededPerImage * sizeof (struct svm_node);

  for (i = 0;  i < prob.l;  i++)
  {
    if  (totalxSpaceUsed > numNeededXspaceNodes)
    {
      log.Level (-1) << endl << endl
        << "FeatureEncoder::Compress   ***ERROR***   We have exceedd the numer of XSpace nodes allocated." << endl
        << endl;
    }

    image = imagesToUseFoXSpace->IdxToPtr (i);

    if  (image->MLClass () != lastMLClass)
    {
      lastMLClass = image->MLClass ();
      lastClassNum   = assignments.GetNumForClass (lastMLClass);
    }

    prob.y[i]     = lastClassNum;
    prob.index[i] = i;
    prob.exampleNames.push_back (osGetRootName (image->ImageFileName ()));

    if  (prob.W)
    {
      prob.W[i] = image->TrainWeight () * svmParam.C_Param ();
      if  (image->TrainWeight () <= 0.0f)
      {
        log.Level (-1) << endl << "FeatureEncoder::Compress    ***ERROR***   Example[" << image->ImageFileName () << "]"
                               << "                                          has a TrainWeigh value of 0 or less.  defaulting to 1.0" << endl
                               << endl;
        prob.W[i] = 1.0 * svmParam.C_Param ();
      }
    }

    if  (xSpace == NULL)
    {
      struct svm_node*  xSpaceThisImage = (struct svm_node*) malloc (bytesOfxSpacePerImage);
      prob.x[i] = xSpaceThisImage;
      EncodeAImage (image, prob.x[i], xSpaceUsed);
      if  (xSpaceUsed < xSpaceNeededPerImage)
      {
        int32  bytesNeededForThisExample = xSpaceUsed * sizeof (struct svm_node);
        struct svm_node*  smallerXSpaceThisImage = (struct svm_node*) malloc (bytesNeededForThisExample);
        memcpy (smallerXSpaceThisImage, xSpaceThisImage, bytesNeededForThisExample);
        free  (xSpaceThisImage);
        prob.x[i] = smallerXSpaceThisImage;
      }
    }
    else
    {
      prob.x[i] = &xSpace[totalxSpaceUsed];
      EncodeAImage (image, prob.x[i], xSpaceUsed);
    }
    totalxSpaceUsed += xSpaceUsed;
  }

  delete  compressedImages;
  return  stats;
}  /* Compress */





/**
 * @brief Attempts to reduce the number of images in the images_list via compression
 *        if no compression specified will just copy refrences to imahges to compressed
 *        list, and flag as Opwner(false).
 * @param[in] images_list The list of images you want to attempt to reduce
 * @param[out] compressed_images_list The reduced list of images
 * @return a CompressionStats struct containing the details of the compression
 */
CompressionStats  FeatureEncoder::CompressImages (FeatureVectorListPtr    srcImages,
                                                  FeatureVectorListPtr    compressedImages,
                                                  ClassAssignments&       assignments
                                                 )
{
  CompressionStats  stats;

  switch  (compressionMethod)
  {
  case BRcompressionPost:
    {
      BitReduction  br (svmParam, fileDesc, selectedFeatures, log);
      stats = br.compress (*srcImages, compressedImages, assignments);
      compressedImages->Owner (true);
    }
    break;

  case  BRunBalancedVariance:
    {
      BitReduction  br (svmParam, fileDesc, selectedFeatures, log);
      stats = br.compress (*srcImages, compressedImages, assignments);
      compressedImages->Owner (true);
    }
    break;


  default:
    double time_before, time_after;


    time_before = osGetSystemTimeUsed ();
    compressedImages->AddQueue (*srcImages);
    time_after = osGetSystemTimeUsed ();
    compressedImages->Owner (false);

    stats.num_images_before = (long)srcImages->size();
    stats.num_images_after  = (long)compressedImages->size();
    stats.compression_ratio = 0.0f;
    stats.elapsed_cpu_time = time_after-time_before;
    break;
  }

  log.Level (20) << "COMPRESSION: # images before was "   << stats.num_images_before  << endl;
  log.Level (20) << "COMPRESSION: # images after was "    << stats.num_images_after   << endl;
  log.Level (20) << "COMPRESSION: Compression ratio was " << stats.compression_ratio  << endl;
  log.Level (20) << "COMPRESSION: Reduction Time was "    << stats.elapsed_cpu_time   << endl;

  return stats;
}  /* CompressImages */




FeatureVectorListPtr  FeatureEncoder::CreateEncodedFeatureVector (FeatureVectorList&  srcData)
{
  if  (srcData.AllFieldsAreNumeric ())
    return  srcData.DuplicateListAndContents ();

  FeatureVectorListPtr  encodedFeatureVectorList = new FeatureVectorList (destFileDesc, true, log, srcData.QueueSize ());

  FeatureVectorList::iterator  idx;
  for  (idx = srcData.begin ();   idx != srcData.end ();  idx++)
  {
    FeatureVectorPtr  srcExample = *idx;
    XSpacePtr  encodedData = EncodeAImage (srcExample);

    int32  zed = 0;
    FeatureVectorPtr  encodedFeatureVector = new FeatureVector (codedNumOfFeatures);
    while  (encodedData[zed].index != -1)
    {
      encodedFeatureVector->AddFeatureData (encodedData[zed].index, (float)encodedData[zed].value);
      zed++;
    }

    encodedFeatureVector->MLClass (srcExample->MLClass ());
    encodedFeatureVectorList->PushOnBack (encodedFeatureVector);

    delete  encodedData;
  }

  return  encodedFeatureVectorList;
}  /* CreateEncodedFeatureVector */
