#include "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <istream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;


using namespace System;
using namespace System::Threading;
using namespace System::Windows::Forms;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"
#include  "..\\BaseLibrary\\GoalKeeper.h"
#include  "OSservices.h"

using namespace KKU;


#include  "Blob.h"
#include  "ContourFollower.h"
#include  "ConvexHull.h"
#include  "ImageIO.h"
#include  "OSservices.h"

#include  "InstrumentDataFileManager.h"
#include  "RasterSipper.h"

#include  "FeatureFileIO.h"
#include  "FeatureFileIOPices.h"
#include  "ImageFeatures.h"

#include  "PicesKKStr.h"
#include  "PicesRunLog.h"
#include  "PicesRaster.h"
#include  "TrainingModel2.h"


using namespace  PicesInterface;

namespace  PicesInterface
{

  PicesRaster::PicesRaster  (String^  _fileName):
      memoryPressure (0)
  {
    System::Drawing::Bitmap^ i = nullptr;
    raster = NULL;
    try
    {
      i = gcnew System::Drawing::Bitmap (_fileName);
    }
    catch (Exception^ e)
    {
      throw gcnew Exception ("PicesRaster::PicesRaster", e);
    }

    System::Drawing::Color   color;

    int  r, c;

    int h = i->Height;
    int w = i->Width;

    raster = new RasterSipper (h, w);

    for  (r = 0;  r < h;  r++)
    {
      for  (c = 0;  c < w;  c++)
      {
        color = i->GetPixel (c, r);
        raster->SetPixelValue (r, c, 255 - color.G);
      }
    }
    raster->FileName (PicesKKStr::SystemStringToKKStr (_fileName));

    memoryPressure = raster->Width () * raster->Height () * (raster->Color () ? 3 : 1);
    GC::AddMemoryPressure (memoryPressure);
  }



  PicesRaster::PicesRaster (int  height,
                            int  width
                           ):
      memoryPressure (0)
  {
    raster = new RasterSipper (height, width);
    memoryPressure = raster->Width () * raster->Height () * (raster->Color () ? 3 : 1);
    GC::AddMemoryPressure (memoryPressure);
  }




  PicesRaster::PicesRaster (System::Array^ _raster):
      memoryPressure (0)
  {
    raster = BuildRasterObj (_raster);
    memoryPressure = raster->Width () * raster->Height () * (raster->Color () ? 3 : 1);
    GC::AddMemoryPressure (memoryPressure);
  }



  PicesRaster::PicesRaster (RasterSipperPtr  _raster):
     memoryPressure (0),
     raster         (_raster)
  {
    memoryPressure = raster->MemoryConsumedEstimated ();
    GC::AddMemoryPressure (memoryPressure);
  }



  PicesRaster::!PicesRaster ()
  {
    delete  raster;
    raster = NULL;

    if  (memoryPressure > 0)
      GC::RemoveMemoryPressure (memoryPressure);
  }



  PicesRaster::~PicesRaster ()
  {
    this->!PicesRaster ();
  }



  String^  PicesRaster::FileName::get ()  
  {
    return  PicesKKStr::KKStrToSystenStr (raster->FileName ());
  }



  void  PicesRaster::FileName::set (String^ fileName)  
  {
    raster->FileName (PicesKKStr::SystemStringToKKStr (fileName));
  }





  System::String^  PicesRaster::MaskTypeToStr (MaskType  mt)
  {
    System::String^  result = nullptr;
    switch  (mt)
    {
    case  MaskType::CROSS3:   result = "Cross-3";     break;
    case  MaskType::CROSS5:   result = "Cross-5";     break;
    case  MaskType::SQUARE3:  result = "Square-3";    break;
    case  MaskType::SQUARE5:  result = "Square-5";    break;
    case  MaskType::SQUARE7:  result = "Square-7";    break;
    case  MaskType::SQUARE9:  result = "Square-9";    break;
    case  MaskType::SQUARE11: result = "Square-11";   break;
    default:                  result = "UnKnown";     break;
    }  /* end of switch (ot) */

    return  result;
  }  /* MaskTypeToStr */



  PicesRaster::MaskType  PicesRaster::MaskTypeFromStr (System::String^  mtStr)
  {
    mtStr = mtStr->ToUpper ();

    if  (mtStr == "CROSS-3")
      return  MaskType::CROSS3;

    if  (mtStr == "CROSS-5")
      return  MaskType::CROSS5;

    if  (mtStr == "SQUARE-3")
      return  MaskType::SQUARE3;

    if  (mtStr == "SQUARE-5")
      return  MaskType::SQUARE5;

    if  (mtStr == "SQUARE-7")
      return  MaskType::SQUARE7;

    if  (mtStr == "SQUARE-9")
      return  MaskType::SQUARE9;

    if  (mtStr == "SQUARE-11")
      return  MaskType::SQUARE11;

    return  MaskType::SQUARE3;
  }  /* MaskTypeFromStr */





  System::String^  PicesRaster::OperationTypeToStr (OperationType  ot)
  {
    System::String^  result = nullptr;
    switch  (ot)
    {
    case  OperationType::BandPass:            result = "BandPass";            break;
    case  OperationType::Binarize:            result = "Binarize";            break;
    case  OperationType::BinarizeTH:          result = "BinarizeTH";          break;
    case  OperationType::Closing:             result = "Closing";             break;
    case  OperationType::ConnectedComponent:  result = "ConnectedComponent";  break;
    case  OperationType::ConvexHull:          result = "ConvexHull";          break;
    case  OperationType::Dialation:           result = "Dialation";           break;
    case  OperationType::Edge:                result = "Edge";                break;
    case  OperationType::Erosion:             result = "Erosion";             break;
    case  OperationType::FillHoles:           result = "FillHoles";           break;
    case  OperationType::FourierTransform:    result = "FourierTransform";    break;
    case  OperationType::Opening:             result = "Opening";             break;
    case  OperationType::SmoothAveraging:     result = "SmoothAveraging";     break;
    case  OperationType::SmoothMedium:        result = "SmoothMedium";        break;
    case  OperationType::Thinning:            result = "Thinning";            break;
    default:                                  result = "UnKnown";             break;
    }  /* end of switch (ot) */

    return  result;
  }  /* OperationTypeToStr */




  PicesRaster::OperationType  PicesRaster::OperationTypeFromStr (System::String^  otStr)
  {
    otStr = otStr->ToUpper ();

    if  (otStr == "BANDPASS")
      return  OperationType::BandPass;
    
    if  (otStr == "BINARIZE")
      return  OperationType::Binarize;
    
    if  (otStr == "BINARIZETH")
      return  OperationType::BinarizeTH;
    
    if  (otStr == "CLOSING")
      return  OperationType::Closing;

    if  (otStr == "CONNECTEDCOMPONENT")
      return  OperationType::ConnectedComponent;

    if  (otStr == "CONVEXHULL")
      return  OperationType::ConvexHull;

    if  (otStr == "DIALATION")
      return  OperationType::Dialation;

    if  (otStr == "EDGE")
      return  OperationType::Edge;

    if  (otStr == "EROSION")
      return  OperationType::Erosion;
    
    if  (otStr == "FILLHOLES")
      return  OperationType::FillHoles;

    if  (otStr == "FOURIERTRANSFORM")
      return  OperationType::FourierTransform;

    if  (otStr == "OPENING")
      return  OperationType::Opening;

    if  (otStr == "SMOOTHAVERAGING")
      return  OperationType::SmoothAveraging;

    if  (otStr == "SMOOTHMEDIUM")
      return  OperationType::SmoothMedium;

    if  (otStr == "THINNING")
      return  OperationType::Thinning;

    return  OperationType::Null;
  }  /* OperationTypeFromStr */




  int   PicesRaster::MaskSize (MaskType  mt)
  {
    int  size = 3;
    switch  (mt)
    {
    case  MaskType::CROSS3:   size = 3;   break;
    case  MaskType::CROSS5:   size = 5;   break;
    case  MaskType::SQUARE3:  size = 3;   break;
    case  MaskType::SQUARE5:  size = 5;   break;
    case  MaskType::SQUARE7:  size = 7;   break;
    case  MaskType::SQUARE9:  size = 9;   break;
    case  MaskType::SQUARE11: size = 11;  break;
    }
    return  size;
  }



  void  PicesRaster::DrawGrid (float                   pixelsPerMinor,
                               int                     minorsPerMajor,
                               System::Drawing::Color  hashColor,
                               System::Drawing::Color  gridColor
                              )
  {
    PixelValue hashPv (hashColor.R, hashColor.G, hashColor.B);
    PixelValue gridPv (gridColor.R, gridColor.G, gridColor.B);
    raster->DrawGrid (pixelsPerMinor, minorsPerMajor, hashPv, gridPv);
  }




  RasterSipperPtr  PicesRaster::BuildRasterObj (System::Array^  _raster)
  {
    if  (_raster == nullptr)
      return  NULL;

    int  h = _raster->Length;
    if  (h < 1)
      return  NULL;

    int w = ((System::Array^)(_raster->GetValue (0)))->Length;

    RasterSipperPtr  r = new RasterSipper (h, w, false);

    uchar*  greenArea = r->GreenArea ();

    int  row, col;

    int  z = 0;

    for (row = 0;  row < h;  row++)
    {
      System::Array^  rowData = (System::Array^)_raster->GetValue (row);
      for  (col = 0;  col < w;  col++)
      {
        greenArea[z] = (uchar)rowData->GetValue (col);
        z++;
      }
    }
   
    return  r;
  }  /* BuildRasterObj */




  System::Array^  PicesRaster::BuildRasterArray ()
  {
    if  (!raster)
      return  nullptr;

    int  width  = raster->Width ();
    int  height = raster->Height ();

    uchar**  srcRows = raster->Green ();
    uchar*   srcRow  = NULL;

    typedef  cli::array<Byte > ^   RowType;

    cli::array<RowType> ^  destRows  = gcnew array<RowType> (height);
    RowType   destRow = nullptr;
    
    for  (int row = 0;  row < height;  row++)
    {
      destRow = gcnew cli::array<Byte > (width);
        
      destRows[row] = destRow;
      srcRow        = srcRows[row];
      for  (int  col = 0;  col < width;  col++)
        destRow[col] = srcRow[col];
    }

    return  destRows;
  }  /* BuildRasterArray */



  array<array<System::Byte>^ >^  PicesRaster::BuildRasterArray2 ()
  {
    if  (!raster)
      return  nullptr;

    int  row, col;

    int  width  = raster->Width ();
    int  height = raster->Height ();

    uchar**  srcRows = raster->Green ();
    if  (srcRows == NULL)
      throw gcnew Exception ("PicesRaster::BuildRasterArray2     GreenArea == NULL");

    uchar*   srcRow  = NULL;

    array<array<System::Byte>^ >^  destRows = gcnew array<array<System::Byte>^ > (height);
    
    array<System::Byte>^   destRow = nullptr;

    for  (row = 0;  row < height;  row++)
    {
      destRow = gcnew array<System::Byte> (width);

      destRows->SetValue (destRow, row);
      srcRow        = srcRows[row];
      for  (col = 0;  col < width;  col++)
      { 
        try
        {
          destRow[col] = srcRow[col];
        }
        catch  (Exception^ e3)
        {
          throw gcnew Exception ("PicesRaster::BuildRasterArray2   Error Processing Row[" + row.ToString () + "]  Col[" + col.ToString () + "]", e3);
        }
      }
    }

    return  destRows;
  }



  bool  PicesRaster::Color ()
  {
    return  raster->Color ();
  }


  PicesRaster^   PicesRaster::BandPass (float  lowerFreqBound,
                                        float  upperFreqBound
                                       )
  {
    return  gcnew PicesRaster (raster->BandPass (lowerFreqBound, upperFreqBound));
  }  /* BandPass */



  PicesRaster^   PicesRaster::Binarize ()
  {
    RasterSipperPtr  result = raster->BinarizeByThreshold (2, 255);
    return  gcnew PicesRaster (result);
  }  /* Binarize */




  PicesRaster^   PicesRaster::BinarizeByThreshold (uint  min,
                                                   uint  max
                                                  )
  {
    RasterSipperPtr  result = raster->BinarizeByThreshold (min, max);
    return  gcnew PicesRaster (result);
  }  /* BinarizeByThreshold */




  PicesRaster^   PicesRaster::ErodeImage (MaskType  mt)
  {
    RasterSipperPtr  erodedRaster = raster->CreateErodedImage ((KKU::MaskTypes)mt);

    return  gcnew PicesRaster (erodedRaster);
  }  /* ErodeImage */



  PicesPointList^  PicesRaster::DeriveImageLength ()
  {
    KKU::PointListPtr  points = raster->DeriveImageLength ();
    if  (points == NULL)
      return nullptr;

    PicesPointList^  results = gcnew PicesPointList (points);
    return  results;
  }


  PicesRaster^   PicesRaster::DialateImage (MaskType  mt)
  {
    RasterSipperPtr  dialatedRaster = raster->CreateDialatedRaster ((KKU::MaskTypes)mt);

    return  gcnew PicesRaster (dialatedRaster);
  }   /* DialateImage */



  PicesRaster^   PicesRaster::OpenImage (MaskType  mt)
  {
    RasterSipperPtr  erodedImage = raster->CreateErodedImage         ((KKU::MaskTypes)mt);
    RasterSipperPtr  opendImage  = erodedImage->CreateDialatedRaster ((KKU::MaskTypes)mt);
    delete  erodedImage;  erodedImage = NULL;
    return  gcnew PicesRaster (opendImage);
  }  /* OpenImage */



  PicesRaster^   PicesRaster::ReversedImage ()
  {
    return gcnew PicesRaster (raster->ReversedImage ());
  }


  void   PicesRaster::Save (String^  fileName)
  {
    if  (!raster)
       throw gcnew Exception ("PicesRaster::Save  'raster' is not defined.");

    String^  extension = OSservices::GetFileExtension (fileName)->ToLower ();

    if  ((extension == "bmp")  &&  (!raster->Color ()))
    {
      // We want to use the routine in ImageIO from "BaseLibrary" to save this image because it is much more efficient in space ussage.
      if  (raster->Color ())
        SaveImage (*raster, PicesKKStr::SystemStringToKKStr (fileName));
      else
        KKU::SaveImageGrayscaleInverted4Bit (*raster, PicesKKStr::SystemStringToKKStr (fileName));
    }
    else
    {
      Bitmap^  bm = BuildBitmap ();
      bm->Save (fileName);
    }
  }  /* Save */





  PicesRaster^   PicesRaster::CloseImage (MaskType  mt)
  {
    RasterSipperPtr  dialatedImage = raster->CreateDialatedRaster     ((KKU::MaskTypes)mt);
    RasterSipperPtr  closedImage   = dialatedImage->CreateErodedImage ((KKU::MaskTypes)mt);
    delete  dialatedImage;  dialatedImage = NULL;
    return  gcnew PicesRaster (closedImage);
  }  /* CloseImage */




  PicesRaster^   PicesRaster::ConvexHull ()
  {
    KKU::ConvexHullPtr  ch = new KKU::ConvexHull ();
    RasterPtr  convexImage = ch->Filter (*raster);
    delete  ch; ch = NULL;
    return  gcnew PicesRaster (RasterSipper::TurnIntoSipperRasterPtr (convexImage));
  }  /* ConvexHull */



  float  PicesRaster::ComputeSegmentLens (PicesPointList^  points,
                                          float            heightFactor,
                                          float            widthFactor
                                         )
  {
    if  ((points == nullptr)  ||  (points->Count < 1))
      return 0.0f;
    else
      return points->ComputeSegmentLens (heightFactor, widthFactor);
  }



  PicesRaster^   PicesRaster::ConnectedComponent (uchar connectedComponentDist)
  {
    RasterSipperPtr  connectedRaster = new RasterSipper (*raster);
    connectedRaster->ConnectedComponent (connectedComponentDist);
    return  gcnew PicesRaster (connectedRaster);
  }  /* ConnectedComponent */




  PicesRaster^  PicesRaster::FillHoles ()
  {
    RasterSipperPtr  filledImage = new RasterSipper (*raster);
    filledImage->FillHole ();
    return  gcnew PicesRaster (filledImage);
  }  /* FillHoles */



  PicesRaster^  PicesRaster::EdgeImage ()
  {
    RasterSipperPtr  edgeImage = new RasterSipper (*raster);
    edgeImage->Edge ();
    return  gcnew PicesRaster (edgeImage);
  }  /* FillHoles */



  PicesRaster^  PicesRaster::FourierTransform ()
  {
    RasterSipperPtr  result = raster->FastFourier ();
    return  gcnew PicesRaster (result);
  }  /* FourierTransform */



  PicesRaster^   PicesRaster::SmoothAveraging (int  maskSize)
  {
    RasterSipperPtr  smoothedImage = raster->CreateSmoothImage (maskSize);
    return  gcnew PicesRaster (smoothedImage);
  }  /* SmoothAveraging  */
     


  PicesRaster^   PicesRaster::SmoothMedium (int  maskSize)
  {
    RasterSipperPtr   smoothedImage = raster->CreateSmoothedMediumImage (maskSize);
    return  gcnew PicesRaster (smoothedImage);
  }  /* SmoothMedium  */



  PicesRaster^   PicesRaster::Thinning ()
  {
    RasterSipperPtr  thinnedImage = NULL;
    try
    {
      thinnedImage = raster->ThinContour ();
    }
    catch  (Exception^)
    {
      thinnedImage = NULL;
    }

    if  (thinnedImage == NULL)
      return  nullptr;

    return  gcnew PicesRaster (thinnedImage);
  }  /* Thinning */



  PicesRaster^   PicesRaster::ToColor ()
  {
    return gcnew PicesRaster (raster->ToColor ());
  }



  RasterSipperPtr  ApplyMask (RasterSipperPtr  origImage,
                              RasterSipperPtr  maskImage
                             )
  {
    int  row, col;
    RasterSipperPtr  result = new RasterSipper (origImage->Height (), origImage->Width ());
    for  (row = 0;  row < origImage->Height ();  row++)
    {
      for  (col = 0;  col < origImage->Width ();  col++)
      {
        if  (maskImage->ForegroundPixel (row, col))
          result->SetPixelValue (row, col, origImage->GetPixelValue (row, col));
      }
    }

    return  result;
  }  /* ApplyMask*/




  Bitmap^  PicesRaster::BuildBitmap ()
  {
    int  rowFirst = 99999;
    int  rowLast  = -1;
    int  colFirst = 99999;
    int  colLast  = -1;

    int  width = raster->Width ();
    int  height = raster->Height ();

    bool  color = raster->Color ();

    uchar**  red   = NULL;
    uchar**  green = raster->Green ();
    uchar**  blue  = NULL;

    if  (color)
    {
      red   = raster->Red   ();
      blue  = raster->Blue  ();
    }
    else
    {
      red  = green;
      blue = green;
    }

    Bitmap^  image = gcnew Bitmap (width, height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
      
    // GDI+ return format is BGR, NOT RGB. 
    System::Drawing::Imaging::BitmapData^ bmData 
      = image->LockBits (System::Drawing::Rectangle (0, 0, width, height),
                         System::Drawing::Imaging::ImageLockMode::ReadWrite,
                         image->PixelFormat
                        );
    int stride = bmData->Stride;
    System::IntPtr  Scan0 = bmData->Scan0;

    {
      int  nOffset = stride - width * 3;
      int  bytesPerRow = width * 3 + nOffset;
      int  col = 0;
      int  row = 0;
      byte redValue   = 0;
      byte greenValue = 0;
      byte blueValue  = 0;

      byte* ptr = (byte*)(void*)Scan0;

      for  (row = 0;  row < height;  row++)
      {
        uchar*  redDataRow   = red  [row];
        uchar*  greenDataRow = green[row];
        uchar*  blueDataRow  = blue [row];
   
        for (col = 0;  col < width;  col++)
        {
          if  (color)
          {
            redValue   = redDataRow   [col];
            greenValue = greenDataRow [col];
            blueValue  = blueDataRow  [col];

            ptr[0] = blueValue;    ptr++;    // kak
            ptr[0] = greenValue;   ptr++;
            ptr[0] = redValue;     ptr++;
           }
          else
          {
            greenValue = 255 - greenDataRow[col];
            ptr[0] = greenValue;  ptr++;
            ptr[0] = greenValue;  ptr++;
            ptr[0] = greenValue;  ptr++;
          }
        }

        ptr += nOffset;
      }  /* Unsafe */

      image->UnlockBits (bmData);
      return  image;
    }   /* Unsafe */
  }  /* BuildBitmap */



  PicesRaster^  PicesRaster::GetOrigSipperImage (String^       sipperFileRootName,
                                                 uint64        byteOffset,
                                                 uint          topLeftRow,
                                                 uint          topLeftCol,
                                                 uint          height,
                                                 uint          width,
                                                 uchar         connectedComponentDist,
                                                 PicesRunLog^  log
                                                )
  {
    if  (String::IsNullOrEmpty (sipperFileRootName))
      return  nullptr;

    RasterSipperPtr  r = InstrumentDataFileManager::GetOrigSipperImage 
                        (PicesKKStr::SystemStringToKKStr (sipperFileRootName),
                          byteOffset, 
                          topLeftRow, 
                          topLeftCol, 
                          height, 
                          width,
                          connectedComponentDist,
                          log->Log ()
                         );

    if  (r == NULL)
      return nullptr;

    return  gcnew  PicesRaster (r);
  }  /* GetOrigSipperImage */








  PicesRasterList::PicesRasterList ()
  {
  }


  PicesRasterList::PicesRasterList (RasterSipperListPtr&  unmanagedImages)
  {
    if  (unmanagedImages == NULL)
      return;

    RasterSipperList::iterator  idx;
    for  (idx = unmanagedImages->begin ();  idx != unmanagedImages->end ();  idx++)
    {
      RasterSipperPtr  i = *idx;
      PicesRaster^ pi = gcnew PicesRaster (i);
      Add (pi);
    }
    unmanagedImages->Owner (false);
    delete  unmanagedImages;
    unmanagedImages = NULL;
  }


  PicesRasterList::~PicesRasterList ()
  {
    CleanUpMemory ();
  }


  PicesRasterList::!PicesRasterList ()
  {
    CleanUpMemory ();
  }


  void  PicesRasterList::CleanUpMemory ()
  {
    Clear ();
  }


  void  PicesRasterList::AddList (PicesRasterList^ list)
  {
    for each  (PicesRaster^ i in list)
    {
      Add (i);
    }
  }



  void  PicesRasterList::AddList (RasterSipperListPtr&  list)
  {
    if  (!list)  return;
    RasterSipperList::iterator  idx;
    for  (idx = list->begin ();  idx != list->end ();  ++idx)
    {
      RasterSipperPtr  r = *idx;
      Add (gcnew PicesRaster (r));
    }

    list->Owner (false);
    delete  list;
    list = NULL;
  }




}  /* PicesInterface */
