/* RasterSipper.cpp -- Class for one raster image.
 * Copyright (C) 1994-2011 Kurt Kramer
 * For conditions of distribution and use, see copyright notice in KKU.h
 */
#include "FirstIncludes.h"

#include <memory>
#include <math.h>
#include <limits.h>
#include <fstream>
#include <map>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>

#include "MemoryDebug.h"

using namespace std;


#include "RasterSipper.h"

#include "BasicTypes.h"
#include "Blob.h"
#include "BMPImage.h"
#include "Compressor.h"
#include "ConvexHull.h"
#include "Histogram.h"
#include "ImageIO.h"
#include "Matrix.h"
#include "OSservices.h"
#include "SimpleCompressor.h"
#include "Sobel.h"
using namespace KKU;


RasterSipper::RasterSipper (): Raster ()
{
}


RasterSipper::RasterSipper (int32  _height,
                            int32  _width
                           ):
  Raster (_height, _width)
{
}



RasterSipper::RasterSipper (int32 _height,
                            int32 _width,
                            bool  _color
                           ):
  Raster (_height, _width, _color)

{
}



/**
 *@brief  Constructs a RasterSipper from a BMP image loadded from disk.
 *@detais If BMP Image is a grayscale value pixel values will be reveresed.  See description of grayscale constructor.
 */
RasterSipper::RasterSipper (const BmpImage&  _bmpImage):
  Raster (_bmpImage)

{
}




RasterSipper::RasterSipper (const RasterSipper&  _raster):
  Raster (_raster)
{
}



RasterSipper::RasterSipper (const Raster&  _raster):
  Raster (_raster)
{
}



RasterSipper::RasterSipper (const RasterSipper&  _raster,
                            int32                _row,
                            int32                _col,
                            int32                _height,
                            int32                _width
                           ):
  Raster (_raster, _row, _col, _height, _width)
{
}



RasterSipper::RasterSipper (const RasterSipper& _raster,
                            MaskTypes           _mask,
                            int32               _row,
                            int32               _col
                           ):
  Raster (_raster, _mask, _row, _col)
{
}



/**
 *@brief Constructs a raster object from a Image File.
 *@details If a color image the pixel values will be reveresed, see constructor
 *         for grayscale images.
 */
RasterSipper::RasterSipper (const KKStr&  _fileName,
                            bool&          validFile
                           ):
  Raster (_fileName, validFile)

{
}




RasterSipper::RasterSipper (int32    _height,
                            int32    _width,
                            uchar*   _grayScaleData,
                            uchar**  _grayScaleRows
                           ):
  Raster (_height, _width, _grayScaleData, _grayScaleRows)

{
}



RasterSipper::RasterSipper (int32         _height,
                            int32         _width,
                            const uchar*  _grayScaleData
                           ):
  Raster (_height, _width, _grayScaleData)
{
}


RasterSipper::RasterSipper (int32         _height,
                            int32         _width,
                            const uchar*  _redChannel,
                            const uchar*  _greenChannel,
                            const uchar*  _blueChannel
                           ):
  Raster (_height, _width, _redChannel, _greenChannel, _blueChannel)
{
}




RasterSipper::~RasterSipper ()
{
}



int32  RasterSipper::MemoryConsumedEstimated ()  const
{
  return  Raster::MemoryConsumedEstimated ();
}



RasterSipperPtr  RasterSipper::TurnIntoSipperRasterPtr (RasterPtr& r)
{
  RasterSipperPtr  sr = new RasterSipper ();

  if  (r->Color ())
  {
    sr->Initialize (r->Height (),
                    r->Width  (),
                    r->RedArea   (),  r->Red   (),
                    r->GreenArea (),  r->Green (),
                    r->BlueArea  (),  r->Blue  (),
                    true
                   );
  }
  else
  {
    sr->Initialize (r->Height (),
                    r->Width  (),
                    r->GreenArea (),  r->Green (),
                    true
                   );
  }

  r->WeOwnRasterData (false);
  delete  r;
  r = NULL;

  return sr;
}




RasterPtr  RasterSipper::AllocateARasterInstance (int32  height,
                                                  int32  width,
                                                  bool   color
                                                 )  const
{
  return new RasterSipper (height, width, color);
}



RasterPtr  RasterSipper::AllocateARasterInstance (const Raster& r)  const
{
  return new RasterSipper (r);
}




RasterPtr  RasterSipper::AllocateARasterInstance (const Raster& _raster,  /**<  Source Raster                                       */
                                                  int32         _row,     /**<  Starting Row in '_raster' to copy from.             */
                                                  int32         _col,     /**<  Starting Col in '_raster' to copy from.             */
                                                  int32         _height,  /**<  Height of resultant raster. Will start from '_row'  */
                                                  int32         _width    /**<  Width of resultant raster.                          */
                                                 )
{
  return new RasterSipper (dynamic_cast<const RasterSipper&>(_raster), _row, _col, _height, _width);
}







RasterSipperPtr  RasterSipper::CreatePaddedRaster (BmpImage&  image,
                                                   int32      padding
                                                  )
{
   return dynamic_cast<RasterSipperPtr>(Raster::CreatePaddedRaster (image, padding));
}  /* CreatePaddedRaster */



RasterSipperPtr   RasterSipper::ReversedImage ()
{
  return dynamic_cast<RasterSipperPtr>(Raster::ReversedImage ());
}  /* ReversedImage */




RasterSipperPtr  RasterSipper::CreateDialatedRaster ()  const
{
  return dynamic_cast<RasterSipperPtr>(Raster::CreateDialatedRaster ());
}  /* CreateDialatedRaster */




RasterSipperPtr  RasterSipper::CreateDialatedRaster (MaskTypes  mask)  const
{
  return  dynamic_cast<RasterSipperPtr> (Raster::CreateDialatedRaster (mask));
}  /* CreateDialatedRaster */





RasterSipperPtr  RasterSipper::CreateErodedImage (MaskTypes  mask)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateErodedImage (mask));
}  /* Erosion  */




/**
 @brief Returns image where each blob is labeled with a different color.
 @details Only useful if 'ExtractBlobs' was performed on this instance.
 */
RasterSipperPtr  RasterSipper::CreateColorWithBlobsLabeldByColor (BlobListPtr  blobs)
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateColorWithBlobsLabeldByColor (blobs));
}  /* CreateColorWithBlobsLabeldByColor*/



RasterSipperPtr  RasterSipper::CreateFromOrginalImageWithSpecifidBlobsOnly (RasterSipperPtr  origImage,
                                                                            BlobListPtr      blobs
                                                                           )
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateFromOrginalImageWithSpecifidBlobsOnly (origImage, blobs));
}  /* CreateFromOrginalImageWithSpecifidBlobsOnly*/





RasterSipperPtr  RasterSipper::ExtractABlobTightly (const BlobPtr  blob,
                                                    int32          padding
                                                   )  const
{
  if  (blob == NULL)
    return NULL;

  return dynamic_cast<RasterSipperPtr> (Raster::ExtractABlobTightly (blob, padding));
}  /* ExtractABlobTightly */




RasterSipperPtr  RasterSipper::ExtractABlob (const BlobPtr  blob)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::ExtractABlob (blob));
}  /* ExtractABlob */





RasterSipperPtr  RasterSipper::FastFourierKK ()  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::FastFourierKK ());
}  /* FastFourierKK */





RasterSipperPtr  RasterSipper::FastFourier ()  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::FastFourier ());
}  /* FastFourier */




RasterSipperPtr  RasterSipper::SwapQuadrants ()  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::SwapQuadrants ());
}  /* SwapQuadrants  */






RasterSipperPtr  RasterSipper::Rotate (float  turnAngle)
{
  RasterPtr  rotatedRaster = Raster::Rotate (turnAngle);
  return RasterSipper::TurnIntoSipperRasterPtr (rotatedRaster);
}  /* Rotate */




RasterSipperPtr  RasterSipper::CreateGrayScale ()  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateGrayScale ());
}  /* CreateGrayScale */





RasterSipperPtr  RasterSipper::HistogramEqualizedImage ()  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::HistogramEqualizedImage ());
}  /* HistogramEqualizedImage */





RasterSipperPtr  RasterSipper::HistogramEqualizedImage (HistogramPtr  equalizedHistogram)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::HistogramEqualizedImage (equalizedHistogram));
}  /* HistogramEqualizedImage */





RasterSipperPtr  RasterSipper::HistogramGrayscaleImage ()  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::HistogramGrayscaleImage ());
}  /* HistogramGrayscaleImage */



RasterSipperPtr  RasterSipper::HistogramImage (ColorChannels  channel)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::HistogramImage (channel));
}  /* HistogramImage */





RasterSipperPtr  RasterSipper::CreateSmoothImage (int32 maskSize)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateSmoothImage (maskSize));
} /* CreateSmoothImage */





RasterSipperPtr  RasterSipper::CreateSmoothedMediumImage (int32 maskSize)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateSmoothedMediumImage (maskSize));
} /* CreateSmoothedMediumImage */




RasterSipperPtr  RasterSipper::HalfSize ()
{
  return dynamic_cast<RasterSipperPtr> (Raster::HalfSize ());
}  /* HalfSize */





RasterSipperPtr  RasterSipper::ReduceByEvenMultiple (int32  multiple)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::ReduceByEvenMultiple (multiple));
}  /* ReduceByEvenMultiple */



RasterSipperPtr  RasterSipper::ReduceByFactor (float factor)  const  //  0 < factor <= 1.0
{
  return dynamic_cast<RasterSipperPtr> (Raster::ReduceByFactor (factor));
}  /* ReduceByFactor */




RasterSipperPtr  RasterSipper::SobelEdgeDetector ()
{
  return dynamic_cast<RasterSipperPtr> (Raster::SobelEdgeDetector ());
}  /* SobelEdgeDetector */




RasterSipperPtr  RasterSipper::BinarizeByThreshold (uchar  min,
                                                    uchar  max
                                                   )
{
  return dynamic_cast<RasterSipperPtr> (Raster::BinarizeByThreshold (min, max));
}  /* BinarizeByThreshold */



float  RasterSipper::ComputeSegmentLens (const PointListPtr  points,
                                         float               heightFactor,
                                         float               widthFactor
                                        )
{
  if  (points)
    return points->ComputeSegmentLens (heightFactor, widthFactor);
  else
    return 0.0f;
}




RasterSipperPtr  RasterSipper::ExtractChannel (ColorChannels  channel)
{
  return dynamic_cast<RasterSipperPtr> (Raster::ExtractChannel (channel));
}  /* ExtractChannel */




//******************************************************************************
//*  Wrote this method to deel with polution sample particles, meeded to help  *
//* segment out particles from a verynoisy background.                         *
//******************************************************************************
RasterSipperPtr   RasterSipper::SegmentImage (bool  save)
{
  return dynamic_cast<RasterSipperPtr> (Raster::SegmentImage (save));
}  /* SegmentImage */




RasterSipperListPtr  RasterSipper::SplitImageIntoEqualParts (int32 numColSplits,
                                                             int32 numRowSplits
                                                            )  const
{
  RasterListPtr  rasterParts = Raster::SplitImageIntoEqualParts (numColSplits, numRowSplits);
  if  (!rasterParts)
    return NULL;

  RasterSipperListPtr  rasterSipperParts = new RasterSipperList (*rasterParts);
  rasterParts->Owner (false);
  delete  rasterParts;
  rasterParts = NULL;
  return rasterSipperParts;
}  /* SplitImageIntoEqualParts */



RasterSipperPtr  RasterSipper::ThinContour ()
{
  return dynamic_cast<RasterSipperPtr> (Raster::ThinContour ());
}  /* ThinContour */




RasterSipperPtr  RasterSipper::TightlyBounded (uint32 borderPixels)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::TightlyBounded (borderPixels));
}  /* TightlyBounded */



RasterSipperPtr  RasterSipper::ToColor ()  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::ToColor ());
}  /* ToColor */



RasterSipperPtr  RasterSipper::BandPass (float  lowerFreqBound,    // Number's between 0.0 and 1.0
                                         float  upperFreqBound     // Represent percentage.
                                        )
{
  return dynamic_cast<RasterSipperPtr> (Raster::BandPass (lowerFreqBound, upperFreqBound));
}  /* BandPass */





uchar*   RasterSipper::SimpleCompression (uint32&  buffLen) const // Will create a compress image using 'SimpleCompression'
{
  int32  totalPixs = Height () * Width ();

  SimpleCompressor  compressor (totalPixs);

  compressor.Add16BitInt (Height ());
  compressor.Add16BitInt (Width ());

  int32  x;
  for  (x = 0;  x < totalPixs;  x++)
    compressor.AddByte (greenArea[x]);

  return  compressor.CreateCompressedBuffer (buffLen);
}  /* SimpleCompression */
  





// Creates a raster from a compressedBuff created by 'SimpleCompression'
RasterSipperPtr  RasterSipper::FromSimpleCompression (const uchar*  compressedBuff,
                                                      uint32        compressedBuffLen
                                                     )  
{
  // I expect simple run length compressed data to be passed in.  The data was  orginally 
  // compressed by SimpleCompressor and we will use the same class to decompress.

  // The format of the uncompressed data is very simple.
  //  Bytes    Description
  //  0 thru 1:    Height   2 byte integer.
  //  2 thru 3:    Width    2 byte integer.
  //
  //  4 thru (Hight * Width) - 1:   Raster Data
  //          Row 0          : (consists of 'Width' bytes.)
  //          Row 1          : (    ""    ""    ""   ""   )
  //                   --     ---     ---     ----   
  //          Row Height - 1 : (Last row of image) 
  //
  //          Each byte inthe raster represents one pixel 0 - 255 grayscale;  where 0=background.

  uint32  unCompressedSize = 0;

  if  (compressedBuff == NULL)
  {
    cerr << endl
         << "RasterSipper::FromSimpleCompression   ***ERROR***  (compressedBuff==NULL)" << endl
         << endl;
    return new RasterSipper (20, 20, false);
  }

  uchar*  uncompressedBuff = SimpleCompressor::Decompress (compressedBuff, compressedBuffLen, unCompressedSize);
  if  (uncompressedBuff == NULL)
  {
    cerr << endl
         << "RasterSipper::FromSimpleCompression   ***ERROR***  (uncompressedBuff==NULL)" << endl
         << endl;
    return new RasterSipper (20, 20, false);
  }

  // The first Four Bytes have the image Height, and Width.

  int32  height = uncompressedBuff[0] * 256 +  uncompressedBuff[1];
  int32  width  = uncompressedBuff[2] * 256 +  uncompressedBuff[3];

  if  ((height < 1)  ||  (height > 1000)  ||  (width < 1)  ||  (width > 1000))
  {
    cerr << endl
         << "RasterSipper::FromSimpleCompression   ***ERROR***   Height[" << height << "]  Width[" << width << "]  are out of bounds." << endl
         << endl;
    return new RasterSipper (20, 20, false);
  }

  uint32  totalPixels = height * width;
  if  (unCompressedSize > (totalPixels + 4))
  {
    cerr << endl
         << "RasterSipper::FromSimpleCompression   ***ERROR***   unCompressedSize[" << unCompressedSize << "] is greater than 'totalPixels + 4'[" << totalPixels + 4 << "]." << endl
         << endl;
  }

  RasterSipperPtr  result = new RasterSipper (height, width, false);
  uchar* greenArea = result->GreenArea ();

  uint32  nextIdx = 4;
  uint32  greanAreaIdx = 0;

  while  ((nextIdx < (uint32)unCompressedSize)  &&  (greanAreaIdx < totalPixels))
  {
    greenArea[greanAreaIdx] = uncompressedBuff[nextIdx];
    nextIdx++;
    greanAreaIdx++;
  }

  delete  uncompressedBuff;
  uncompressedBuff = NULL;

  return  result;
}  /* FromSimpleCompression */




RasterSipperPtr  RasterSipper::FromCompressor (const uchar*  compressedBuff,    // Creates a raster from a compressedBuff created by 'Compressor'(zlib)
                                               uint32        compressedBuffLen
                                              )
{
  uint32  unCompressedBuffLen = 0;
  uchar*  unCompressedBuff = NULL;
  try
  {
    unCompressedBuff = (uchar*)Compressor::Decompress (compressedBuff, compressedBuffLen, unCompressedBuffLen);
  }
  catch  (...)
  {
    KKStr errMsg = "RasterSipper::FromCompressor  Exception thrown calling 'Compressor::Decompress'.";
    cerr << errMsg << endl;
    throw KKStrException (errMsg);
  }

  if  (!unCompressedBuff)
    return NULL;

  if  (unCompressedBuffLen < 10)
  {
    cerr << std::endl << std::endl << "Raster::FromCompressor  Compressor did not return any data."  << std::endl;
    return NULL;
  }

  // 0 - 3:    Height:  high order to low order
  // 4 - 7:    Width:   high order to low order
  // 8 - 8:    Color    0 = Grayscale,  1 = Color
  // 9 - 8 + (Height * Width) Green Channel
  // xxxxx                    Red  Channel
  // xxxxx                    Blue Channel

  uint32  height = 0;
  uint32  width  = 0;
  height = unCompressedBuff[0] + unCompressedBuff[1] * 256 + unCompressedBuff[2] * 256 * 256 + unCompressedBuff[3] * 256 * 256 * 256;
  width  = unCompressedBuff[4] + unCompressedBuff[5] * 256 + unCompressedBuff[6] * 256 * 256 + unCompressedBuff[7] * 256 * 256 * 256;

  bool color = (unCompressedBuff[8] == 1);

  uint32  totalPixels = height * width;
  if  ((totalPixels > (100 * 1024 * 1024))  ||  (height < 1)  ||  (width < 1))
  {
    cerr << std::endl << std::endl << "Raster::FromCompressor  Height[" << height << "]  Width[" << width << "]  is not valid." << std::endl << std::endl;
    delete  unCompressedBuff;
    unCompressedBuff = NULL;
    return NULL;
  }

  uint32  totalDataNeeded = totalPixels + (color ? (2 * totalPixels) : 0) + 9;
  if  (totalDataNeeded > unCompressedBuffLen)
  {
    cerr << std::endl << std::endl 
         << "Raster::FromCompressor  Height[" << height << "]  Width[" << width << "]  Color[" << (color ? "Yes" : "No") << "]" << std::endl
         << "              requires TotalDataNeeded[" << totalDataNeeded << "]  but UnCompressedBuffLen[" << unCompressedBuffLen << "]" << std::endl
         << std::endl;
    delete  unCompressedBuff;
    unCompressedBuff = NULL;
    return NULL;
  }

  RasterSipperPtr  r = new RasterSipper ((int32)height, (int32)width, color);

  uint32 nextIdx = 9;
  uint32 x;

  uchar*  greenArea = r->GreenArea ();
  for  (x = 0;  x < totalPixels;  x++, nextIdx++)
    greenArea[x] = unCompressedBuff[nextIdx];

  if  (color)
  {
    uchar*  redArea = r->RedArea ();
    for  (x = 0;  x < totalPixels;  x++, nextIdx++)
      redArea[x] = unCompressedBuff[nextIdx];

    uchar*  blueArea = r->BlueArea ();
    for  (x = 0;  x < totalPixels;  x++, nextIdx++)
      blueArea[x] = unCompressedBuff[nextIdx];
  }
 
  delete  unCompressedBuff;
  unCompressedBuff = NULL;

  return  r;
}  /* FromCompressor */




RasterSipperPtr   RasterSipper::Padded (int32 padding)
{
  return dynamic_cast<RasterSipperPtr> (Raster::Padded (padding));
}



RasterSipperPtr  RasterSipper::CreateGaussianSmoothedImage (float sigma)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateGaussianSmoothedImage (sigma));
}  /* CreateGaussianSmoothedImage */




RasterSipperPtr  RasterSipper::ThresholdInHSI (float              thresholdH,
                                               float              thresholdS, 
                                               float              thresholdI, 
                                               float              distance,
                                               const PixelValue&  flagValue
                                              )
{
  return dynamic_cast<RasterSipperPtr> (Raster::ThresholdInHSI (thresholdH, thresholdS, thresholdI, distance, flagValue));
}  /* ThresholdingHSI */




RasterSipperPtr  RasterSipper::CreateGrayScaleKLT ()  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateGrayScaleKLT ());
}  /* CreateGrayScaleKLT */




RasterSipperPtr  RasterSipper::CreateGrayScaleKLTOnMaskedArea (const RasterSipper&  mask)  const
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateGrayScaleKLTOnMaskedArea (mask));
}  /* CreateGrayScaleKLTOnMaskedArea */



RasterSipperPtr  RasterSipper::CreateColorImageFromLabels ()
{
  return dynamic_cast<RasterSipperPtr> (Raster::CreateColorImageFromLabels ());
}  /* CreateColorImageFromLabels */







Point  DerivePreRotatedPoint (int32   height,
                              int32   width,
                              Point&  rotatedPoint, 
                              float   turnAngle
                             )
{
  int32  diag = (int32)sqrt ((float)(height * height + width * width)) + 10;

  float  a11 = (float)(cos (-turnAngle));
  float  a12 = (float)(sin (-turnAngle));
  float  b1  = width  * 0.5f;

  float  a21 = -a12;
  float  a22 = a11;
  float  b2  = height * 0.5f;

  int32  halfDiag = (diag + 1) / 2;

  int32  centDestRow = rotatedPoint.Row () - halfDiag;
  int32  centDestCol = rotatedPoint.Col () - halfDiag;

  int32  srcY = (int32)((float)(a21 * centDestCol) + (float)(a22 * centDestRow) + b2 + 0.5);
  int32  srcX = (int32)((float)(a11 * centDestCol) + (float)(a12 * centDestRow) + b1 + 0.5);

  return  Point (srcY, srcX);
}




PointListPtr  RasterSipper::DeriveImageLength () const
{
  PointListPtr  results = NULL;

  float  eigenRatio;
  float  orientationAngle;

  RasterSipperPtr  workRaster = CreateErodedImage (SQUARE3);
  workRaster->FillHole ();
  workRaster->Erosion (SQUARE7);
  workRaster->ConnectedComponent (3);
  workRaster->CalcOrientationAndEigerRatio (eigenRatio,
                                            orientationAngle
                                           );
  if  ((orientationAngle > TwoPie)  ||  (orientationAngle < -TwoPie))
  {
    orientationAngle = 0.0;
    eigenRatio = 1.0;
  }
 
  RasterSipperPtr rotatedImage = workRaster->Rotate (orientationAngle);
  int32  tlRow;
  int32  tlCol;
  int32  brRow;
  int32  brCol;

  rotatedImage->FindBoundingBox (tlRow, tlCol, brRow, brCol);
  if  (tlRow >= 0)
  {
    uchar**  imageData = rotatedImage->Green ();

    int32  boxWidth  = brCol - tlCol;
    int32  boxHeight = brRow - tlRow;

    int32  mark1Col = (int32)((float)boxWidth * 0.05f + 0.5f) + tlCol;
    int32  mark2Col = (int32)((float)boxWidth * 0.95f + 0.5f) + tlCol;

    int32  a1RowTot   = 0;
    int32  a1ColTot   = 0;
    int32  a1PixCount = 0l;

    int32  a2RowTot   = 0;
    int32  a2ColTot   = 0;
    int32  a2PixCount = 0l;

    int32  a3RowTot   = 0;
    int32  a3ColTot   = 0;
    int32  a3PixCount = 0;

    for  (int32 row = tlRow;  row <= brRow;  ++row)
    {
      int32 col = 0;

      uchar*  rowData = imageData[row];

      for  (col = tlCol;  col <= mark1Col;  ++col)
      {
        if  (rowData[col] > 2)
        {
          ++a1PixCount;
          a1RowTot += row;
          a1ColTot += col;
        }
      }

      for  (col = mark1Col + 1;  col <= mark2Col;  ++col)
      {
        if  (rowData[col] > 2)
        {
          ++a2PixCount;
          a2RowTot += row;
          a2ColTot += col;
        }
      }

      for  (col = mark2Col + 1;  col <= brCol;  ++col)
      {
        if  (rowData[col] > 2)
        {
          ++a3PixCount;
          a3RowTot += row;
          a3ColTot += col;
        }
      }
    }

    a1PixCount = Max (1, a1PixCount);
    a2PixCount = Max (1, a2PixCount);
    a3PixCount = Max (1, a3PixCount);

    Point p1 ((int)(0.5f + a1RowTot / a1PixCount), (int)(0.5f + a1ColTot / a1PixCount));
    Point p2 ((int)(0.5f + a2RowTot / a2PixCount), (int)(0.5f + a2ColTot / a2PixCount));
    Point p3 ((int)(0.5f + a3RowTot / a3PixCount), (int)(0.5f + a3ColTot / a3PixCount));

   
    Point p1Orig = DerivePreRotatedPoint (height, width, p1, orientationAngle);
    Point p2Orig = DerivePreRotatedPoint (height, width, p2, orientationAngle);
    Point p3Orig = DerivePreRotatedPoint (height, width, p3, orientationAngle);

    results = new PointList (true);
    results->PushOnBack (new Point (p1Orig));
    results->PushOnBack (new Point (p2Orig));
    results->PushOnBack (new Point (p3Orig));
  }


  delete rotatedImage;  rotatedImage = NULL;
  delete workRaster;    workRaster   = NULL;

  return  results;

}  /* DeriveImageLength */





RasterSipperList::RasterSipperList (const RasterList&  rasterList,
                                    bool               _owner
                                   ):
     KKQueue<RasterSipper> (_owner)
{

  RasterList::const_iterator  idx;
  for  (idx = rasterList.begin ();  idx != rasterList.end ();  ++idx)
  {
    RasterPtr  r = *idx;
    RasterSipperPtr sp = dynamic_cast<RasterSipperPtr> (r);
    if  (sp == NULL)
    {
      if  (!_owner)
        throw KKStrException ("RasterSipperList::RasterSipperList  instance in container is not a 'RasterSipperPtr'");
      else
        PushOnBack (new RasterSipper (*r));
    }
    else
    {
      if  (!_owner)
        PushOnBack (sp);
      else
        PushOnBack (new RasterSipper (*sp));
    }
  }
}



RasterSipperList::RasterSipperList (const RasterList&  rasterList):
    KKQueue<RasterSipper> (rasterList.Owner ())
{
  RasterList::const_iterator  idx;
  for  (idx = rasterList.begin ();  idx != rasterList.end ();  ++idx)
  {
    RasterPtr  r = *idx;
    RasterSipperPtr sp = dynamic_cast<RasterSipperPtr> (r);
    if  (sp == NULL)
    {
      if  (!Owner ())
        throw KKStrException ("RasterSipperList::RasterSipperList  instance in container is not a 'RasterSipperPtr'");
      else
        PushOnBack (new RasterSipper (*r));
    }
    else
    {
      if  (!Owner ())
        PushOnBack (sp);
      else
        PushOnBack (new RasterSipper (*sp));
    }
  }
}
