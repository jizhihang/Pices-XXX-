#include "FirstIncludes.h"
#include <stdio.h>
#include <vector>
#include "MemoryDebug.h"
using namespace  std;


#include "BasicTypes.h"
#include "RasterSipper.h"
#include "Str.h"
using namespace  KKU;


#include  "ExtractedImage.h"
using namespace  ImageExtractionManager;


ExtractedImage::ExtractedImage (RasterSipperPtr _image,
                                uint64          _byteOffsetFirstScanRow,
                                uint32          _sipperFileScanRow,
                                uint32          _sipperFileScanCol,
                                uint32          _pixelCount,
                                float           _depth,
                                MLClassPtr   _predClass1,
                                float           _predClass1Prob,
                                MLClassPtr   _predClass2,
                                float           _predClass2Prob
                               ):
   image                  (_image),
   byteOffsetFirstScanRow (_byteOffsetFirstScanRow),
   featureVector          (NULL),
   sipperFileScanRow      (_sipperFileScanRow),
   sipperFileScanCol      (_sipperFileScanCol),
   pixelCount             (_pixelCount),
   depth                  (_depth),
   predClass1             (_predClass1),
   predClass1Prob         (_predClass1Prob),
   predClass2             (_predClass2),
   predClass2Prob         (_predClass2Prob)
{
}




ExtractedImage::~ExtractedImage ()
{
  delete  featureVector;  featureVector = NULL;
  delete  image;          image         = NULL;
}



int32   ExtractedImage::MemoryConsumedEstimated ()  const
{
  int32  m = sizeof (*this);
  if  (image)
    m += image->MemoryConsumedEstimated ();

  if  (featureVector)
    m += featureVector->MemoryConsumedEstimated ();

  return  m;
}



void  ExtractedImage::FeatureVector (ImageFeaturesPtr  _featureVector)
{
  if  (featureVector)
    delete  featureVector;
  featureVector = _featureVector;
}


void  ExtractedImage::Prediction (MLClassConstPtr _predClass1,
                                  float              _predClass1Prob,
                                  MLClassConstPtr _predClass2,
                                  float              _predClass2Prob
                                 )
{
  predClass1      = _predClass1;
  predClass1Prob  = _predClass1Prob;
  predClass2      = _predClass2;
  predClass2Prob  = _predClass2Prob;
}



ImageFeaturesPtr  ExtractedImage::FeatureVectorGiveOwnership ()
{
  ImageFeaturesPtr  fv = featureVector;
  featureVector = NULL;
  return  fv;
}  /* FeatureVectorGiveOwnership */


RasterSipperPtr  ExtractedImage::ImageGiveOwnership ()
{
  RasterSipperPtr  i = image;
  image = NULL;
  return  i;
}


ExtractedImageQueue::ExtractedImageQueue ():
   goalie       (NULL),
   maxQueueSize (200)
{
  goalie = new GoalKeeper ("ExtractedImageQueue");
}



ExtractedImageQueue::~ExtractedImageQueue ()
{
  goalie->StartBlock ();
  while  (size () > 0)
  {
    ExtractedImagePtr  i = front ();
    pop_front ();

    delete  i;
    i = NULL;
  }
  goalie->EndBlock ();
  delete  goalie;
  goalie = NULL;
}



int32  ExtractedImageQueue::MemoryConsumedEstimated ()  const
{
  goalie->StartBlock ();
  int32  memoryConsumedEstimated = sizeof (*this);

  std::deque<ExtractedImagePtr>:: const_iterator idx;
  for  (idx = begin ();  idx != end ();  ++idx)
  {
    ExtractedImagePtr  i = *idx;
    memoryConsumedEstimated += i->MemoryConsumedEstimated ();
  }
  goalie->EndBlock ();
  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */



void  ExtractedImageQueue::AddExtractedImage (ExtractedImagePtr  extractedImage)
{
  bool  added = false;
  while  (!added)
  {
    goalie->StartBlock ();
    if  (size () < maxQueueSize)
    {
      push_back (extractedImage);
      added = true;
    }
    goalie->EndBlock ();
    if  (!added)
      osSleep (0.01f);
  }
}  /* AddExtractedImage */



void  ExtractedImageQueue::AddExtractedImages (VectorExtractedImage&  extractedImages)
{
  goalie->StartBlock ();

  VectorExtractedImage::const_iterator  idx;
  for  (idx = extractedImages.begin ();  idx != extractedImages.end ();  ++idx)
  {
    ExtractedImagePtr  i = *idx;
    push_back (i);
  }

  extractedImages.clear ();

  goalie->EndBlock ();
}  /* AddExtractedImages */



ExtractedImagePtr  ExtractedImageQueue::GetNextExtractedImage ()
{
  ExtractedImagePtr  nextImage = NULL;
  goalie->StartBlock ();
  if  (size () > 0)
  {
    nextImage = front ();
    pop_front ();
  }
  goalie->EndBlock ();
  return  nextImage;
}  /* GetNextExtractedImage */



uint32  ExtractedImageQueue::ImagesOnQueue ()
{
  uint32    imagesOnQueue = 0;
  goalie->StartBlock ();
  imagesOnQueue = size ();
  goalie->EndBlock ();
  return  imagesOnQueue;
}
