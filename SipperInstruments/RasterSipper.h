/* RasterSipper.h -- A specialized RasterSipper object for Sipper Images.
 * Copyright (C) 1994-2011 Kurt Kramer
 * For conditions of distribution and use, see copyright notice in KKU.h
 */
#ifndef _RASTERSIPPER_
#define _RASTERSIPPER_

#include <map>

#include "KKQueue.h"
#include "Raster.h"
#include "RunLog.h"


namespace KKU
{
  /**
   *@file  RasterSipper.h
   *@author  Kurt Kramer
   *@details
   *@code
   ***************************************************************************
   **                             RasterSipper                               *
   **                                                                        *
   **  A specialized RasterSipper object for managing Sipper Images.               *
   ***************************************************************************
   *@endcode
   *@sa RasterSipper
   */



  class  RasterSipperList;
  typedef  RasterSipperList*  RasterSipperListPtr;


  /**
   *@class RasterSipper 
   *@brief  A class that is used by to represent a single Sipper Image in memory.  
   *@details  A specialized RasterSipper object for managing Sipper Images.  
   *@see Blob
   *@see ContourFollower
   *@see ConvexHull
   *@see PixelValue
   *@see Point
   *@see PointList
   *@see RasterSipperList()
   *@see Sobel
   */
  class  RasterSipper: public Raster
  {
  public:
    typedef  RasterSipper*  RasterSipperPtr;

    RasterSipper ();

    RasterSipper (const RasterSipper&  _raster);  /**< @brief Copy Constructor */

    RasterSipper (const Raster&  _raster);        /**< @brief Copy Constructor */


    /**
     *@brief Constructs a blank GrayScale image with given dimensions; all pixels will be initialized to 0.  
     *@details  When working with Grayscale images pixel value of '0' = Background and '255'= foreground.
     *          The green channel will be used to represent the grayscale value.  When these raster images
     *          are saved to a image file such as a BMP file the pixel value of 0 will point to the color
     *          value of (255, 255, 255) and  pixel value 255 will point to the color value of (0, 0, 0).
     *          This way when displaying the image background will appear as white.
     */
    RasterSipper (int32  _height,
                  int32  _width
                 );


    /**
     *@brief  Constructs a blank image with given dimensions.  
     *@details  The third parameter determines whether it will be a color or grayscale image,  If a Color
     *          image then all three color channel will be set to = 255 which stands for white.  If
     *          Grayscale the green channel will be set to 0.
     */
    RasterSipper (int32 _height,
                  int32 _width,
                  bool  _color
                 );

    /**
     *@brief  Constructs a RasterSipper from a BMP image loaded from disk.
     *@details If BMP Image is a grayscale value pixel values will be reversed.  See description of
     *        grayscale constructor.
     */
    RasterSipper (const BmpImage&  _bmpImage);


    /**
     *@brief Constructs a new RasterSipper using a subset of the specified RasterSipper as its source.  The
     *      dimensions of the resultant raster will be '_height', and '_width'
     */
    RasterSipper (const RasterSipper& _raster,  /**<  Source RasterSipper                             */
                  int32               _row,     /**<  Starting Row in '_raster' to copy from.             */
                  int32               _col,     /**<  Starting Col in '_raster' to copy from.             */
                  int32               _height,  /**<  Height of resultant raster. Will start from '_row'  */
                  int32               _width    /**<  Width of resultant raster.                          */
                 );

    /**
     * Constructs a RasterSipper that will be as large as the specified '_mask'  at the point
     * specified by '_row' and '_col'.
     * \see MaskTypes
     */
    RasterSipper (const RasterSipper&   _raster,
                  MaskTypes             _mask,    /*!< \see MaskTyps  Generated 'RasterSipper' with be the of the mask.   */
                  int32                 _row,
                  int32                 _col
                 );

    /**
     *@brief  Constructs a RasterSipper image from by reading an existing image File such as a BMP file.
     *@details  Will read from the specified file (fileName) the existing image.  If the load fails then 
     *          the contents of this object will be undefined.
     *@param[in]  fileName  Name of Image file to read.
     *@param[out] validFile  If image successfully loaded will be set to 'True' otherwise 'False'.
     */
    RasterSipper (const KKStr&  fileName,    /*!<  \param  fileName  name of image file to load/                    */
                  bool&         validFile    /*!<  \param  validFile will return true if image successfully loaded. */
                 );


    /**
     *@brief  Construct a raster object that will utilize a image already in memory.
     *@details  This instance will not own the raster data.  It will only point to it.
     */
    RasterSipper (int32    _height,
                  int32    _width,
                  uchar*   _grayScaleData,
                  uchar**  _grayScaleRows
                 );


    /**
     *@brief  Construct a Grayscale RasterSipper object using provided raw data,
     */
    RasterSipper (int32         _height,
                  int32         _width,
                  const uchar*  _grayScaleData
                 );

    /**
     *@brief  Construct a Color RasterSipper object using provided raw data,
     */
    RasterSipper (int32         _height,
                  int32         _width,
                  const uchar*  _redChannel,
                  const uchar*  _greenChannel,
                  const uchar*  _blueChannel
                 );


    virtual
    ~RasterSipper ();


    int32   MemoryConsumedEstimated ()  const;

    /**
     *@brief  Turn an instance of 'Raster' into 'RasterSipper'.
     *@details Will utilize the dynamically allocated raster data of the 'Raster' instance 'r' to build a new instance of a 'SipperRaster'.
     *@param[in,out] r The raster data of 'r' will be given to the new instance of 'RasterSipper' and 'r' will then be deleted and set to NULL.
     */
    static
      RasterSipperPtr  TurnIntoSipperRasterPtr (RasterPtr& r);



    /**
     *@brief Returns a image that is the result of a BandPass using Fourier Transforms.
     *@details A 2D Fourier transform is performed.  The range specified is from 0.0 to 1.0 where range is
     * determined from the center of the image to the farthest corner where the center is 0.0 and the farthest
     * corner is 1.0.  Pixels in the resultant 2D Transform that are "NOT" in the specified range are set to
     * 0.0.  A reverse transform is then performed and the resultant image is returned.
     *@param[in] lowerFreqBound  Lower range of frequencies to retain; between 0.0 and 1.0.
     *@param[in] upperFreqBound  Upper range of frequencies to retain; between 0.0 and 1.0.
     *@return The result image.
     */
    RasterSipperPtr     BandPass (float  lowerFreqBound,    /**< Number's between 0.0 and 1.0  */
                                  float  upperFreqBound     /**< Represent fraction.           */
                                 );


    RasterSipperPtr     BinarizeByThreshold (uchar  min,
                                             uchar  max
                                            );

    static
    float               ComputeSegmentLens (const PointListPtr  points,
                                            float               heightFactor,
                                            float               widthFactor
                                           );

    RasterSipperPtr     CreateDialatedRaster ()  const;


    RasterSipperPtr     CreateDialatedRaster (MaskTypes  mask)  const;

    RasterSipperPtr     CreateErodedImage (MaskTypes  mask)  const;


    RasterSipperPtr     CreateGrayScale ()  const;

    /**
     *@brief Creates a grayscale image using a KLT Transform with the goal of weighting in favor the color
     * channels with greatest amount of variance.
     *@details The idea is to weight each color channel by the amount of variance.  This is accomplished by
     *  producing a covariance matrix of the three color channels and then taking the Eigen vector with the
     *  largest Eigen value and using its components to derive weights for each channel for the conversion
     *  from RGB to grayscale.
     */
    RasterSipperPtr     CreateGrayScaleKLT ()  const;

    /**
     *@brief  Same as 'CreateGrayScaleKLT' except it will only take into account 
     *        pixels specified by the 'mask' image.
     *@param[in]  mask  RasterSipper object where pixels that are greater than 'backgroundPixelTH' are to be considered.
     */
    RasterSipperPtr     CreateGrayScaleKLTOnMaskedArea (const RasterSipper&  mask)  const;

    static
    RasterSipperPtr     CreatePaddedRaster (BmpImage&  image,
                                            int32      padding
                                           );

    RasterSipperPtr     CreateSmoothImage (int32  maskSize = 3)  const;
    
    RasterSipperPtr     CreateSmoothedMediumImage (int32 maskSize)  const;

    RasterSipperPtr     CreateGaussianSmoothedImage (float sigma)  const;

    RasterSipperPtr     CreateColorImageFromLabels ();

    RasterSipperPtr     CreateColorWithBlobsLabeldByColor (BlobListPtr  blobs);  /**< Only useful if 'ExtractBlobs' was performed on this instance, the returned image will be color with each blob labeled a different color. */


    /**
     *@brief Returns a copy of 'origImage' where only the blobs specified in 'blobs' are copied over.
     *@details 
     *@code
     *  Example:
     *      origImage = image that we want to segment and get list of discrete blobs from.
     *      RasterSipperPtr  segmentedImage = origImage->SegmentImage ();
     *      BlobListPtr  blobs = segmentedImage->ExtractBlobs (1);
     *      RasterSipperPtr  imageWithBlobOnly = segmentedImage->CreateFromOrginalImageWithSpecifidBlobsOnly (blobs);
     *@endcode
     *@param[in]  origImage  Image that this instance was derived for, must have same dimensions.
     *@param[in]  blobs      Blobs that you want copied over from.
     *@returns Image consisting of specified blobs only.
     */
    RasterSipperPtr     CreateFromOrginalImageWithSpecifidBlobsOnly (RasterSipperPtr    origImage,
                                                                     BlobListPtr  blobs
                                                                    );


    PointListPtr        DeriveImageLength () const;

    /**
     *@brief  Extracts a specified blob from this image;  useful to extract individual detected blobs.
     *@details  The 'ExtractBlobs' method needs to have been performed on this instance first.  You
     * would use this method after calling 'ExtractBlobs'. The extracted image will be of the same 
     * dimensions as the original image except it will extract the pixels that belong to the specified
     * blob only.
     *@code
     *   // Example of processing extracted blobs
     *   void  ProcessIndividulConectedComponents (RasterSipperPtr  image)
     *   {
     *     BlobListPtr blobs = image->ExtractBlobs (3);
     *     BlobList::iterator  idx;
     *     for  (idx = blobs->begin ();  idx != end ();  ++idx)
     *     {
     *       RasterSipperPtr  individuleBlob = image->ExtractABlob (*idx);
     *       DoSomethingWithIndividuleBlob (individuleBlob);
     *       delete  individuleBlob;
     *       individuleBlob = NULL;
     *     )
     *     delete  blobs;
     *     blobs = NULL;
     *   }
     *@endcode
     */
    RasterSipperPtr     ExtractABlob (const BlobPtr  blob)  const;


    /**
     *@brief  Extracts a specified blob from this image into a tightly bounded image.
     *@details
     *        Similar to 'ExtractABlob' except that the returned image will have the dimension necessary
     *        to contain the specified blob with the specified number of padded row and columns.
     */
    RasterSipperPtr     ExtractABlobTightly (const BlobPtr  blob,
                                             int32          padding
                                            ) const;


    /**
     *@brief Will return a grayscale image consisting of the specified color channel only.
     */
    RasterSipperPtr     ExtractChannel (ColorChannels  channel);

    RasterSipperPtr     FastFourier ()  const;
    RasterSipperPtr     FastFourierKK ()  const;


    /**
     * @brief Creates a raster from a compressedBuff created by 'SimpleCompression'.
     */
    static
    RasterSipperPtr     FromSimpleCompression (const uchar*  compressedBuff,
                                               uint32        compressedBuffLen
                                              ); 

    /**
     *@brief  Creates a new instance of RasterSipper object from zLib compressed data.
     *@details Performs the inverse operation of RasterSipper::ToCompressor.
     *@param[in] compressedBuff  Pointer to buffer area containing compressed data originally created by 'ToCompressor'.
     *@param[in] compressedBuffLen  Length in bytes of 'compressedBuff'.
     *@returns If successful a pointer to a new instance of 'RasterSipper'; if there is an error will return NULL.
     *@see  ToCompressor
     */
    static
    RasterSipperPtr     FromCompressor (const uchar*  compressedBuff,
                                        uint32        compressedBuffLen
                                       ); 

    RasterSipperPtr     HalfSize ();

    RasterSipperPtr     HistogramEqualizedImage ()  const;

    RasterSipperPtr     HistogramEqualizedImage (HistogramPtr  equalizedHistogram) const;

    RasterSipperPtr     HistogramImage (ColorChannels  channel)  const;

    RasterSipperPtr     HistogramGrayscaleImage ()  const;

    RasterSipperPtr     Padded (int32 padding);  // Creates a Padded raster object.

    RasterSipperPtr     ReversedImage ();

    RasterSipperPtr     ReduceByEvenMultiple (int32  multiple)  const;

    RasterSipperPtr     ReduceByFactor (float factor)  const;  //  0 < factor <= 1.0  ex: 0.5 = Make raster half size

    RasterSipperPtr     Rotate (float  turnAngle);

    RasterSipperPtr     SegmentImage (bool  save = false);

    uchar*              SimpleCompression (uint32&  buffLen) const; // Will create a compress image using 'SimpleCompression'

    RasterSipperPtr     SobelEdgeDetector ();

    RasterSipperListPtr SplitImageIntoEqualParts (int32 numColSplits,
                                                  int32 numRowSplits
                                                 )  const;

    RasterSipperPtr     SwapQuadrants ()  const;

    /**
     *@brief Thresholds image in HSI space.
     *@details
     *        Returns an image with only the pixels that are within a specified distance in HSI space to the
     *        supplied HSI parameters.  All pixels that are not within the specified distance will be set to
     *        'flagValue'.
     *@param[in] thresholdH  Hue in radians(0.0 <-> 2Pie).
     *@param[in] thresholdS  Saturation (0.0 <-> 1.0).
     *@param[in] thresholdI  Intensity (0.0 <-> 1.0).
     *@param[in] distance    Threshold Distance (0.0 <-> 1.0) that a pixel must be within in HSI space to be included.
     *@param[in] flagValue   PixelValue to set for pixels that are NOT within 'distance' of threshold.
     *@returns A image where pixels that are within the threshold will retain their original pixel values and 
     * the ones that are not will be set to 'flagValue'.
     */
    RasterSipperPtr     ThresholdInHSI (float              thresholdH,
                                        float              thresholdS, 
                                        float              thresholdI, 
                                        float              distance,
                                        const PixelValue&  flagValue
                                       );

    RasterSipperPtr     ThinContour ();

    RasterSipperPtr     TightlyBounded (uint32 borderPixels)  const;

    RasterSipperPtr     ToColor ()  const;

    virtual
    RasterPtr  AllocateARasterInstance (int32  height,
                                        int32  width,
                                        bool   color
                                       )  const;

    virtual
    RasterPtr  AllocateARasterInstance (const Raster& r)  const;


    virtual
      RasterPtr  AllocateARasterInstance (const Raster& _raster,  /**<  Source Raster                             */
                                          int32         _row,     /**<  Starting Row in '_raster' to copy from.             */
                                          int32         _col,     /**<  Starting Col in '_raster' to copy from.             */
                                          int32         _height,  /**<  Height of resultant raster. Will start from '_row'  */
                                          int32         _width    /**<  Width of resultant raster.                          */
                                         );  private:

  };  /* RasterSipper */


  typedef  RasterSipper::RasterSipperPtr  RasterSipperPtr;





  class  RasterSipperList:  public  KKQueue<RasterSipper>
  {
  public:
    RasterSipperList (bool  owner):
        KKQueue<RasterSipper> (owner)
        {}

  private:
    RasterSipperList (const RasterSipperList&  rasterList):
        KKQueue<RasterSipper> (rasterList)
        {}

  public:
    RasterSipperList (const RasterSipperList&  rasterList,
                      bool                     _owner
                     ):
        KKQueue<RasterSipper> (rasterList, _owner)
        {}

    RasterSipperList (const RasterList&  rasterList);

    RasterSipperList (const RasterList&  rasterList,
                      bool               _owner
                     );
  };  /* RasterSipperList */


  typedef  RasterSipperList*  RasterSipperListPtr;


}  /* namespace KKU; */
#endif
