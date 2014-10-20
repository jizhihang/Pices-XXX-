#if  !defined(_IEM_EXTRACTEDIMAGE_)
#define  _IEM_EXTRACTEDIMAGE_


#include <deque>
using namespace  std;

#include "KKQueue.h"
#include "GoalKeeper.h"
#include "RasterSipper.h"
using namespace  KKU;


#include "MLClass.h"
#include "ImageFeatures.h"
using namespace  MLL;


namespace  ImageExtractionManager
{
  /**
   *@brief   Represets particle that was extracted by 'LogicalFrame' with its related data.
   */
  class  ExtractedImage
  {
  public:
    ExtractedImage (RasterSipperPtr _image,
                    uint64          _byteOffsetFirstScanRow,
                    uint32          _sipperFileScanRow,
                    uint32          _sipperFileScanCol,
                    uint32          _pixelCount,
                    float           _depth,
                    MLClassPtr   _predClass1,
                    float           _predClass1Prob,
                    MLClassPtr   _predClass2,
                    float           _predClass2Prob
                   );

    ~ExtractedImage ();

    int32   MemoryConsumedEstimated ()  const;

    uint64              ByteOffsetFirstScanRow () const  {return byteOffsetFirstScanRow;}
    float               Depth                  () const  {return depth;}
    ImageFeaturesPtr    FeatureVector          () const  {return featureVector;}
    RasterSipperPtr     Image                  () const  {return image;}
    uint32              PixelCount             () const  {return pixelCount;}
    MLClassConstPtr  PredClass1             () const  {return predClass1;}
    float               PredClass1Prob         () const  {return predClass1Prob;}
    MLClassConstPtr  PredClass2             () const  {return predClass2;}
    float               PredClass2Prob         () const  {return predClass2Prob;}
    uint32              SipperFileScanRow      () const  {return sipperFileScanRow;}
    uint32              SipperFileScanCol      () const  {return sipperFileScanCol;}

    void  FeatureVector (ImageFeaturesPtr  _featureVector);
    void  Depth         (float             _depth)           {depth     = _depth;}

    void  Prediction (MLClassConstPtr _predClass1,
                      float              _predClass1Prob,
                      MLClassConstPtr _predClass2,
                      float              _predClass2Prob
                     );
    

    /**@brief  Turns over ownership of FeatureVector to the caller.  */
    ImageFeaturesPtr  FeatureVectorGiveOwnership ();
    RasterSipperPtr   ImageGiveOwnership ();


  private:
    uint64              byteOffsetFirstScanRow;
    float               depth;
    ImageFeaturesPtr    featureVector;
    RasterSipperPtr     image;
    uint32              pixelCount;
    MLClassConstPtr  predClass1;
    float               predClass1Prob;
    MLClassConstPtr  predClass2;
    float               predClass2Prob;
    uint32              sipperFileScanRow;
    uint32              sipperFileScanCol;
  };  /* ExtractedImage */

  typedef  ExtractedImage*  ExtractedImagePtr;

  typedef  vector<ExtractedImagePtr>  VectorExtractedImage;

  typedef  VectorExtractedImage*  VectorExtractedImagePtr;


  typedef  KKQueue<ExtractedImage>  ExtractedImageList;
  typedef  ExtractedImageList*  ExtractedImageListPtr;


  class  ExtractedImageQueue: public  deque<ExtractedImagePtr>
  {
  public:
    ExtractedImageQueue ();

    ~ExtractedImageQueue ();

    int32  MemoryConsumedEstimated ()  const;

    void  AddExtractedImage (ExtractedImagePtr  extractedImage);

    void  AddExtractedImages (VectorExtractedImage&  extractedImages);

    uint32  ImagesOnQueue ();

    ExtractedImagePtr  GetNextExtractedImage ();

  private:
    GoalKeeperPtr   goalie;
    uint32          maxQueueSize;
  };  /* ExtractedImageQueue */


  typedef  ExtractedImageQueue*  ExtractedImageQueuePtr;

}  /* ImageExtractionManager */


#endif

