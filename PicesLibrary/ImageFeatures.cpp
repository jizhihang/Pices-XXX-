#include  "FirstIncludes.h"

#include  <ctype.h>
#include  <time.h>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <map>
#include  <vector>

#include  "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "BMPImage.h"
#include "ContourFollower.h"
#include "ConvexHull.h"
#include "DateTime.h"
#include "ImageIO.h"
#include "OSservices.h"
#include "RasterSipper.h"
#include "RunLog.h"
#include "Str.h"
using namespace  KKU;


#include "InstrumentData.h"
#include "InstrumentDataList.h"
#include "InstrumentDataFileManager.h"
using namespace SipperHardware;


#include "ImageFeatures.h"
#include "FeatureNumList.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "MLClass.h"
#include "ImageDirTree.h"
using namespace  MLL;



const int32  ImageFeatures::SizeThreshold = 100000;  // Size of image in num of pixels before we decide to reduce the
                                                     // image to improve feature calculation.

int32  ImageFeatures::FirstInstrumentDataField = 84;  // see 'NumSeperateInstrumentDataFields' below.

/**
 @code
 *********************************************************************************************
 **                             NumSeperateInstrumentDataFields                              * 
 **                                                                                          *
 ** This is the number of fields that are not going to be stored in the regular feature file *
 ** but in seperate insrument data files that will be accessed by SiperFile name and scan    *
 ** line number.                                                                             *
 **                                                                                          *
 *********************************************************************************************
 */
int32  ImageFeatures::NumSeperateInstrumentDataFields = 4;  

              
short  ImageFeatures::SizeIndex             = 0;
short  ImageFeatures::Moment1Index          = 1;
short  ImageFeatures::Moment2Index          = 2;
short  ImageFeatures::Moment3Index          = 3;
short  ImageFeatures::Moment4Index          = 4;
short  ImageFeatures::Moment5Index          = 5;
short  ImageFeatures::Moment6Index          = 6;
short  ImageFeatures::Moment7Index          = 7;

short  ImageFeatures::EdgeSizeIndex         = 8;
short  ImageFeatures::EdgeMoment1Index      = 9;
short  ImageFeatures::EdgeMoment2Index      = 10;  
short  ImageFeatures::EdgeMoment3Index      = 11;
short  ImageFeatures::EdgeMoment4Index      = 12;
short  ImageFeatures::EdgeMoment5Index      = 13;
short  ImageFeatures::EdgeMoment6Index      = 14;
short  ImageFeatures::EdgeMoment7Index      = 15;



short  ImageFeatures::TransparancyConvexHullIndex = 16;
short  ImageFeatures::TransparancyPixelCountIndex = 17;
short  ImageFeatures::TransparancyOpen3Index      = 18;
short  ImageFeatures::TransparancyOpen5Index      = 19;
short  ImageFeatures::TransparancyOpen7Index      = 20;
short  ImageFeatures::TransparancyOpen9Index      = 21;
short  ImageFeatures::TransparancyClose3Index     = 22;
short  ImageFeatures::TransparancyClose5Index     = 23;
short  ImageFeatures::TransparancyClose7Index     = 24;



short  ImageFeatures::EigenRatioIndex       = 25;
short  ImageFeatures::EigenHeadIndex        = 26;
short  ImageFeatures::ConvexAreaIndex       = 27;
short  ImageFeatures::TransparancySizeIndex = 28;
short  ImageFeatures::TransparancyWtdIndex  = 29;
short  ImageFeatures::WeighedMoment0Index   = 30;
short  ImageFeatures::WeighedMoment1Index   = 31;
short  ImageFeatures::WeighedMoment2Index   = 32;
short  ImageFeatures::WeighedMoment3Index   = 33;
short  ImageFeatures::WeighedMoment4Index   = 34;
short  ImageFeatures::WeighedMoment5Index   = 35;
short  ImageFeatures::WeighedMoment6Index   = 36;
short  ImageFeatures::WeighedMoment7Index   = 37;


short  ImageFeatures::Fourier0Index         = 38;
short  ImageFeatures::Fourier1Index         = 39;
short  ImageFeatures::Fourier2Index         = 40;
short  ImageFeatures::Fourier3Index         = 41;
short  ImageFeatures::Fourier4Index         = 42;


short  ImageFeatures::FdRight1Index          = 43;
short  ImageFeatures::FdLeft2Index           = 44;
short  ImageFeatures::FdRight2Index          = 45;
short  ImageFeatures::FdLeft3Index           = 46;
short  ImageFeatures::FdRight3Index          = 47;
short  ImageFeatures::FdLeft4Index           = 48;
short  ImageFeatures::FdRight4Index          = 49;
short  ImageFeatures::FdLeft5Index           = 50;
short  ImageFeatures::FdRight5Index          = 51;
short  ImageFeatures::FdLeft6Index           = 52;
short  ImageFeatures::FdRight6Index          = 53;
short  ImageFeatures::FdLeft7Index           = 54;
short  ImageFeatures::FdRight7Index          = 55;
short  ImageFeatures::FdLeft8Index           = 56;
short  ImageFeatures::FdRight8Index          = 57;



short  ImageFeatures::ContourFourierDescriptor0 = 58;
short  ImageFeatures::ContourFourierDescriptor1 = 59;
short  ImageFeatures::ContourFourierDescriptor2 = 60;
short  ImageFeatures::ContourFourierDescriptor3 = 61;
short  ImageFeatures::ContourFourierDescriptor4 = 62;


short  ImageFeatures::IntensityHist1Index   = 63;
short  ImageFeatures::IntensityHist2Index   = 64;
short  ImageFeatures::IntensityHist3Index   = 65;
short  ImageFeatures::IntensityHist4Index   = 66;
short  ImageFeatures::IntensityHist5Index   = 67;
short  ImageFeatures::IntensityHist6Index   = 68;
short  ImageFeatures::IntensityHist7Index   = 69;

short  ImageFeatures::LengthVsWidthIndex    = 70;
short  ImageFeatures::LengthIndex           = 71;
short  ImageFeatures::WidthIndex            = 72;
short  ImageFeatures::FilledAreaIndex       = 73;

short  ImageFeatures::FlowRate1Index        = 74;
short  ImageFeatures::FlowRate2Index        = 75;

short  ImageFeatures::IntensityHistWS0Index = 76;
short  ImageFeatures::IntensityHistWS1Index = 77;
short  ImageFeatures::IntensityHistWS2Index = 78;
short  ImageFeatures::IntensityHistWS3Index = 79;
short  ImageFeatures::IntensityHistWS4Index = 80;
short  ImageFeatures::IntensityHistWS5Index = 81;
short  ImageFeatures::IntensityHistWS6Index = 82;
short  ImageFeatures::IntensityHistWS7Index = 83;

short  ImageFeatures::DepthIndex            = 84;
short  ImageFeatures::SalinityIndex         = 85;
short  ImageFeatures::OxygenIndex           = 86;
short  ImageFeatures::FluorescenceIndex     = 87;



ImageFeatures::ImageFeatures (int32  _numOfFeatures):
       FeatureVector (_numOfFeatures),

        centroidCol      (-1),
        centroidRow      (-1),
        ctdDateTime      (),
        latitude         (0.0),
        longitude        (0.0),
        numOfEdgePixels  (-1),
        sfCentroidCol    (-1.0f),
        sfCentroidRow    (-1.0),
        version          (0)
{
}



ImageFeatures::ImageFeatures (const ImageFeatures&  _image):
  FeatureVector (_image),

  centroidCol      (_image.centroidCol),
  centroidRow      (_image.centroidRow),
  ctdDateTime      (_image.ctdDateTime),
  latitude         (_image.latitude),
  longitude        (_image.longitude),
  numOfEdgePixels  (_image.numOfEdgePixels),
  sfCentroidCol    (_image.sfCentroidCol),
  sfCentroidRow    (_image.sfCentroidRow),
  version          (_image.version)

{
}




ImageFeatures::ImageFeatures (const BmpImage&      _image,
                              MLClassConstPtr   _mlClass,
                              RasterSipperListPtr  _saveImages
                             ):
  FeatureVector (FeatureFileIOPices::PlanktonMaxNumOfFields ()),
  centroidCol      (-1),
  centroidRow      (-1),
  ctdDateTime      (),
  latitude         (0.0),
  longitude        (0.0),
  numOfEdgePixels  (-1),
  sfCentroidCol    (-1.0f),
  sfCentroidRow    (-1.0),
  version          (0)

{
  MLClass       (_mlClass);
  ImageFileName    (_image.FileName ());

  RasterSipperPtr raster = new RasterSipper (_image);
  CalcFeatures (*raster, _saveImages);
  delete  raster;
}




ImageFeatures::ImageFeatures (      RasterSipper&  _raster,
                              MLClassConstPtr   _mlClass,
                              RasterSipperListPtr  _saveImages
                             ):
  FeatureVector (FeatureFileIOPices::PlanktonMaxNumOfFields ()),

  centroidCol      (-1),
  centroidRow      (-1),
  ctdDateTime      (),
  latitude         (0.0),
  longitude        (0.0),
  numOfEdgePixels  (-1),
  sfCentroidCol    (-1.0f),
  sfCentroidRow    (-1.0),
  version          (0)

{
  MLClass    (_mlClass);
  ImageFileName (_raster.FileName ());

  KKStr  imageRootName = osGetRootName (_raster.FileName ()).ToUpper ();
  if  (imageRootName == "STATION1C_04_00474734_2979")
  {
    cout << "We are here." << imageRootName << endl;
  }


  CalcFeatures  (_raster, _saveImages);
}



ImageFeatures::ImageFeatures (KKStr                _fileName,
                              MLClassConstPtr   _mlClass,
                              bool&                _successfull,
                              RasterSipperListPtr  _saveImages
                             ):

FeatureVector (FeatureFileIOPices::PlanktonMaxNumOfFields ()),

  centroidCol      (-1),
  centroidRow      (-1),
  ctdDateTime      (),
  latitude         (0.0),
  longitude        (0.0),
  numOfEdgePixels  (-1),
  sfCentroidCol    (-1.0f),
  sfCentroidRow    (-1.0),
  version          (0)

{
  MLClass       (_mlClass);
  ImageFileName    (_fileName);

  KKStr  ext = osGetFileExtension (_fileName);
  ext.Upper ();

  RasterSipperPtr  raster = NULL;

  if  (ext == "BMP")
  {
    BmpImagePtr image = new BmpImage (_fileName, _successfull);

    if  (_successfull)
    {
      if  (image->AreThereEdgePixels ())
      {
        // There are pixels along the edge of the image,  To help avoid 
        // problems with feature calculation, we will pad the image with 
        // a few extra row's and collumns.
        image->ReAllocateForBiggerScreen ();
        image->Save (_fileName);
      }

      raster = new RasterSipper (*image);
    }

    delete  image;
  }
  else
  {
    RasterPtr  workRaster = ReadImage (_fileName);
    if  (workRaster)
    {
      RasterPtr  redChannel   = workRaster->ExtractChannel (RedChannel);
      RasterPtr  greenChannel = workRaster->ExtractChannel (GreenChannel);
      RasterPtr  blueChannel  = workRaster->ExtractChannel (BlueChannel);

      RasterPtr  segmentedRedRaster   = redChannel->SegmentImage ();
      RasterPtr  segmentedGreenRaster = greenChannel->SegmentImage ();
      RasterPtr  segmentedBlueRaster  = blueChannel->SegmentImage ();

      RasterSipperPtr  segmentedRaster = new RasterSipper (workRaster->Height (), workRaster->Width (), false);

      for  (int32 r = 0;  r < raster->Height ();  r++)
      {
        for  (int32 c = 0;  c < raster->Width ();  c++)
        {
          if  ((segmentedRedRaster->ForegroundPixel   (r, c))  ||
               (segmentedGreenRaster->ForegroundPixel (r, c))  ||
               (segmentedBlueRaster->ForegroundPixel  (r, c))
              )
            segmentedRaster->SetPixelValue (r, c, raster->GetPixelValue (r, c));
          else
            segmentedRaster->SetPixelValue (r, c, 255);
        }
      }

      KKStr rootName = osGetRootName (_fileName);

      //KKStr  dirName = "C:\\Temp\\PolutionImages\\" + rootName;
      //osCreateDirectoryPath (dirName);
      //SaveImage (*segmentedRaster, dirName + DS + rootName + "_Segmented.bmp");

      delete  redChannel;    redChannel = NULL;
      delete  greenChannel;  greenChannel = NULL;
      delete  blueChannel;   blueChannel  = NULL;

      delete  segmentedRedRaster;    segmentedRedRaster   = NULL;
      delete  segmentedGreenRaster;  segmentedGreenRaster = NULL;
      delete  segmentedBlueRaster;   segmentedBlueRaster  = NULL;

      // RasterSipperPtr  segmentedRaster = raster->SegmentImage ();
      delete  workRaster;
      workRaster = NULL;
      raster = segmentedRaster;
    }
  }

  if  (raster)
  {
    raster->ConnectedComponent (3);
    CalcFeatures (*raster, _saveImages);
    delete  raster;
  }
  else
  {
    int32  x;
    _successfull = false;

    for  (x = 0; x < this->NumOfFeatures (); x++)
      featureData[x] = 0;

    cerr  << "ImageFeatures::ImageFeatures  ***ERROR***, Opening File[" << _fileName << "]." << endl;
  }
}



ImageFeatures::ImageFeatures (const FeatureVector&  featureVector):
   FeatureVector    (featureVector),
   centroidCol      (-1),
   centroidRow      (-1),
   ctdDateTime      (),
   latitude         (0.0),
   longitude        (0.0),
   numOfEdgePixels  (-1),
   sfCentroidCol    (-1.0f),
   sfCentroidRow    (-1.0),
   version          (0)

{
  if  (strcmp (featureVector.UnderlyingClass (), "ImageFeatures") == 0)
  {
    // The underlying class is anothr ImageFeatures object.
    const ImageFeatures&  image = dynamic_cast<const ImageFeatures&>(featureVector);

    centroidCol      = image.CentroidCol     ();
    centroidRow      = image.CentroidRow     ();
    ctdDateTime      = image.CtdDateTime     ();
    latitude         = image.Latitude        ();
    longitude        = image.Longitude       ();
    numOfEdgePixels  = image.NumOfEdgePixels ();
    sfCentroidCol    = image.SfCentroidCol   ();
    sfCentroidRow    = image.SfCentroidRow   ();
    version          = image.Version         ();
  }
}



ImageFeatures::~ImageFeatures ()
{
}


int32  ImageFeatures::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = FeatureVector::MemoryConsumedEstimated () 
    +  sizeof (float)   * 3
    +  sizeof (double)  * 3
    +  sizeof (int32)   * 1
    +  sizeof (short)   * 1;

  return  memoryConsumedEstimated;
}



void  ImageFeatures::ResetVersion (short  newVersion)
{
  Version (newVersion);
}  /* ResetVersionMum */



bool   ImageFeatures::atExitDefined   = false;


void  ImageFeatures::FinalCleanUp ()
{
}  /* FinalCleanUp */




void  ImageFeatures::Save (RasterSipper&        raster, 
                           const KKStr&         desc,
                           RasterSipperListPtr  saveImages
                          )
{
  if  (!saveImages)
    return;

  RasterSipperPtr  newRaster = NULL;

  int32  largestDim = Max (raster.Height (), raster.Width ());
  if  (largestDim < 300)
  {
    newRaster = new RasterSipper (raster);
  }
  else
  {
    int32  reductionMultiple = 2;
    while  ((largestDim / reductionMultiple) > 300)
     reductionMultiple++;
    newRaster = raster.ReduceByEvenMultiple (reductionMultiple);
  }

  newRaster->FileName (desc);
  saveImages->PushOnBack (newRaster);

  //KKStr newName (baseName);
  //newName << "_" << desc << ".bmp";
  //
  //BmpImagePtr  bmpImage = new BmpImage (raster);
  //bmpImage->Save (newName);
  //delete  bmpImage;
}




void  ImageFeatures::ParseImageFileName (const KKStr&  fullFileName, 
                                         KKStr&        sipperFileName,
                                         uint32&       scanLineNum,
                                         uint32&       scanCol
                                        )
{
  sipperFileName = "";
  scanLineNum    = 0;
  scanCol        = 0;
  
  KKStr  rootName = osGetRootName (fullFileName);
  if  (rootName.Empty ())
    return;
  
  int32  x = rootName.LocateLastOccurrence ('_');
  if  (x > 0)
  {
    KKStr  colStr = rootName.SubStrPart (x + 1);
    KKStr  temp = rootName.SubStrPart (0, x - 1);
    x = temp.LocateLastOccurrence ('_');
    if  (x > 0)
    {
      sipperFileName = temp.SubStrPart (0, x - 1);
      KKStr  rowStr = temp.SubStrPart (x + 1);
      scanCol     = atoi (colStr.Str ());
      scanLineNum = atoi (rowStr.Str ());
    }
  }
}  /* ParseImageFileName */






//#define  DEBUB_CalcFeatures

void  ImageFeatures::CalcFeatures (RasterSipper&        srcRaster,
                                   RasterSipperListPtr  saveImages
                                  )
{
  #if defined (DEBUB_CalcFeatures)
  cout << std::endl
       << std::endl
       << "ImageFeatures::CalcFeatures" << std::endl;
  #endif


  RunLog  log;

  if  (NumOfFeatures () != FeatureFileIOPices::PlanktonMaxNumOfFields ())
  {
    numOfFeatures = FeatureFileIOPices::PlanktonMaxNumOfFields ();
    AllocateFeatureDataArray ();
  }


  #if defined (DEBUB_CalcFeatures)
  cout << "CalcFeatures   After 'AllocateFeatureDataArray'" << std::endl;
  #endif

  version = CurrentFeatureFileVersionNum;
  uint32  intensityHistBuckets[8];
  uint32  intensityHistBucketsB[8];  // These buckets will include white space

  bool  weOwnRaster = false;
  RasterSipperPtr  reversedImage = NULL;

  if  (srcRaster.BackgroundPixelValue () > srcRaster.ForegroundPixelValue ())
  {
    weOwnRaster = true;
    reversedImage = srcRaster.ReversedImage ();
  }

  RasterSipperPtr raster = (weOwnRaster ? reversedImage : &srcRaster);

  int32 areaBeforeReduction;
  int32 areaWithWhiteSpace = 0;
  FFLOAT  weighedSizeBeforeReduction    = 0.0f;
 
  raster->CalcAreaAndIntensityFeatures (areaBeforeReduction, 
                                        weighedSizeBeforeReduction,
                                        intensityHistBuckets,
                                        areaWithWhiteSpace, 
                                        intensityHistBucketsB
                                       );


  #if defined (DEBUB_CalcFeatures)
  cout << "CalcFeatures   After 'CalcAreaAndIntensityFeatures'" << std::endl;
  #endif

  int32  reductionFactor = 1;
  int32  reductionFactorSquared = 1;

  if  (areaBeforeReduction > SizeThreshold)
  {
    int32  reducedMinDim = Min (raster->Height (), raster->Width ());

    int32  reducedSize = areaBeforeReduction;
    reductionFactor = 1;
    while  ((reducedSize > SizeThreshold)  &&  (reducedMinDim > 20))
    {
      reductionFactor = reductionFactor + 1;
      reductionFactorSquared = reductionFactor * reductionFactor;
      reducedSize = areaBeforeReduction / reductionFactorSquared;
      reducedMinDim = Min (raster->Height (), raster->Width ()) / reductionFactor;
    }

    RasterSipperPtr  reducedRaster = raster->ReduceByEvenMultiple (reductionFactor);

    if  (saveImages)
      Save (*reducedRaster, "Reduced_" + StrFormatInt (reductionFactor, "ZZZZZZ0"), saveImages);

    if  (weOwnRaster)
    {
      delete  raster;
      raster = NULL;
    }

    raster = reducedRaster;
    weOwnRaster = true;
  }


  if  (areaBeforeReduction < 20)
  {
    for  (int32 tp = 0; tp < numOfFeatures; tp++)
      featureData[tp] = 9999999;
    
    if  (weOwnRaster)
    {
      delete  raster;
      raster = NULL;
    }
    return;
  }
  
  KKStr  driveLetter, path, root, extension;

  FFLOAT  fourierFeatures[5];

  FFLOAT  convexf = 0.0;

  osParseFileSpec (ImageFileName (), driveLetter, path, root, extension);
  KKStr  baseName = "c:\\Temp\\TestImages2\\" + root + "\\" + root;

  if  (saveImages)
  {
    //osCreateDirectoryPath ("c:\\Temp\\TestImages2\\" + root);
    Save (*raster, "Original_" + StrFormatInt (areaBeforeReduction, "ZZZZZZ0"), saveImages);
  }

  //raster->ConnectedComponent ();
  //Save (*raster, baseName, "ConnectedComponent", saveImage);


  // Baishali added ecentricity moments calc's
  float  momentf[9];
  raster->CentralMoments (momentf);

  FFLOAT  momentfWeighted[9];
  raster->CentralMomentsWeighted (momentfWeighted);

  #if defined (DEBUB_CalcFeatures)
  cout << "CalcFeatures   After Moments and Weighted Moments Calculated." << std::endl;
  #endif



  RasterSipperPtr  edgeImage = new RasterSipper (*raster);

  edgeImage->Dialation ();
  //if  (saveImages)
  //{
  //  int32  a = (int32)edgeImage->CalcArea ();
  //  Save (*edgeImage, "Edge_Dialation_1_" + StrFormatInt (a, "ZZZZZZ0"), saveImages);
  //}

  edgeImage->Dialation ();
  //if  (saveImages)
  //{
  //  int32  a = (int32)edgeImage->CalcArea ();
  //  Save (*edgeImage, "Edge_Dialation_2_" + StrFormatInt (a, "ZZZZZZ0"), saveImages);
  //}

  edgeImage->FillHole ();
  //if  (saveImages)
  //{
  //  int32  a = (int32)edgeImage->CalcArea ();
  //  Save (*edgeImage, "Edge_FillHole_" + StrFormatInt (a, "ZZZZZZ0"), saveImages);
  //}

  edgeImage->Erosion ();
  //if  (saveImages)
  //{
  //  int32  a = (int32)edgeImage->CalcArea ();
  //  Save (*edgeImage, "Edge_Erosion_1_" + StrFormatInt (a, "ZZZZZZ0"), saveImages);
  //}

  edgeImage->Erosion ();
  if  (saveImages)
  {
    int32  a = (int32)edgeImage->CalcArea ();
    Save (*edgeImage, "Edge_JustBefore" + StrFormatInt (a, "ZZZZZZ0"), saveImages);
  }

  edgeImage->Edge ();
  int32  numEdgePixelsFound = (int32)edgeImage->CalcArea ();
  Save (*edgeImage, "Edge_Image_" + StrFormatInt (numEdgePixelsFound, "ZZZZ0"), saveImages);

  float  edgeMomentf[9];
  edgeImage->CentralMoments (edgeMomentf);
  delete  edgeImage;  edgeImage = NULL;

  int32 area = (int32)(momentf[0] + 0.5f);  // Moment-0 is the same as the number of forground pixels in image.
  {
    ConvexHullPtr  ch = new ConvexHull();
    RasterPtr  convexImage = ch->Filter (*raster);
    RasterSipperPtr  convexImageSipper = RasterSipper::TurnIntoSipperRasterPtr (convexImage);
    convexImage = NULL;

    convexf = (FFLOAT)ch->ConvexArea ();

    if  (saveImages)
    {
      KKStr convexImageFileName = "ConvexHull_" +
                                   StrFormatInt ((int32)convexf, "ZZZZZ0");
      Save (*convexImageSipper, convexImageFileName, saveImages);
    }

    delete  convexImageSipper;
    convexImageSipper = NULL;
    delete  ch;
    ch = NULL;
  }

  RasterSipperPtr open3Raster = new RasterSipper (*raster);
  open3Raster->Opening ();
  FFLOAT  areaOpen3 = FFLOAT (open3Raster->ForegroundPixelCount ());
  if  (saveImages)
    Save (*open3Raster, "Opening3_" + StrFormatInt ((int32)areaOpen3, "ZZZZZZ0"), saveImages);
  delete  open3Raster;
  open3Raster = NULL;
  

  RasterSipperPtr open5Raster = new RasterSipper (*raster);
  open5Raster->Opening (SQUARE5);
  FFLOAT  areaOpen5 = FFLOAT (open5Raster->ForegroundPixelCount ());
  if  (saveImages)
    Save (*open5Raster, "Opening5_" + StrFormatInt ((int32)areaOpen5, "ZZZZZZ0"), saveImages);
  delete  open5Raster;
  open5Raster = NULL;
  

  RasterSipperPtr open7Raster = new RasterSipper (*raster);
  open7Raster->Opening (SQUARE7);
  FFLOAT  areaOpen7 = FFLOAT (open7Raster->ForegroundPixelCount ());
  if  (saveImages)
    Save (*open7Raster, "Opening7_" + StrFormatInt ((int32)areaOpen7, "ZZZZZZ0"), saveImages);
  delete  open7Raster;
  open7Raster = NULL;
  
  RasterSipperPtr open9Raster = new RasterSipper (*raster);
  open9Raster->Opening (SQUARE9);
  FFLOAT  areaOpen9 = (FFLOAT)open9Raster->ForegroundPixelCount ();
  if  (saveImages)
    Save (*open9Raster, "Opening9_" + StrFormatInt ((int32)areaOpen9, "ZZZZZZ0"), saveImages);
  delete  open9Raster;
  open9Raster = NULL;

  RasterSipperPtr close3Raster = new RasterSipper (*raster);
  close3Raster->Closing ();
  FFLOAT  areaClose3 = FFLOAT (close3Raster->ForegroundPixelCount ());
  if  (saveImages)
    Save (*close3Raster, "Close3_" + StrFormatInt ((int32)areaClose3, "ZZZZZZ0"), saveImages);

  close3Raster->FillHole ();
  FFLOAT  tranf = (FFLOAT)close3Raster->CalcArea ();
  if  (saveImages)
    Save (*close3Raster, "FillHole_" + StrFormatInt ((int32)tranf, "ZZZZZZ0"), saveImages);
  delete  close3Raster;
  close3Raster = NULL;
  
  RasterSipperPtr close5Raster = new RasterSipper (*raster);
  close5Raster->Closing (SQUARE5);
  FFLOAT  areaClose5 = FFLOAT (close5Raster->ForegroundPixelCount ());
  if  (saveImages)
    Save (*close5Raster, "Close5_" + StrFormatInt ((int32)areaClose5, "ZZZZZZ0"), saveImages);
  delete  close5Raster;
  close5Raster = NULL;
  
  RasterSipperPtr close7Raster = new RasterSipper (*raster);
  close7Raster->Closing (SQUARE7);
  FFLOAT  areaClose7 = FFLOAT (close7Raster->ForegroundPixelCount ());
  if  (saveImages)
    Save (*close7Raster, "Close7_" + StrFormatInt ((int32)areaClose7, "ZZZZZZ0"), saveImages);
  delete  close7Raster;
  close7Raster = NULL;


  #if defined (DEBUB_CalcFeatures)
  cout << "CalcFeatures   After Morphalogical Opertations." << std::endl;
  #endif


  {
    featureData[SizeIndex]    = FFLOAT (momentf[0]);
    featureData[Moment1Index] = FFLOAT (momentf[1]);
    featureData[Moment2Index] = FFLOAT (momentf[2]);
    featureData[Moment3Index] = FFLOAT (momentf[3]);
    featureData[Moment4Index] = FFLOAT (momentf[4]);
    featureData[Moment5Index] = FFLOAT (momentf[5]);
    featureData[Moment6Index] = FFLOAT (momentf[6]);
    featureData[Moment7Index] = FFLOAT (momentf[7]);

    featureData[WeighedMoment0Index] = momentfWeighted[0];
    featureData[WeighedMoment1Index] = momentfWeighted[1];
    featureData[WeighedMoment2Index] = momentfWeighted[2];
    featureData[WeighedMoment3Index] = momentfWeighted[3];
    featureData[WeighedMoment4Index] = momentfWeighted[4];
    featureData[WeighedMoment5Index] = momentfWeighted[5];
    featureData[WeighedMoment6Index] = momentfWeighted[6];
    featureData[WeighedMoment7Index] = momentfWeighted[7];

    featureData[EdgeSizeIndex]    = (float)edgeMomentf[0];
    featureData[EdgeMoment1Index] = (float)edgeMomentf[1];
    featureData[EdgeMoment2Index] = (float)edgeMomentf[2];
    featureData[EdgeMoment3Index] = (float)edgeMomentf[3];
    featureData[EdgeMoment4Index] = (float)edgeMomentf[4];
    featureData[EdgeMoment5Index] = (float)edgeMomentf[5];
    featureData[EdgeMoment6Index] = (float)edgeMomentf[6];
    featureData[EdgeMoment7Index] = (float)edgeMomentf[7];

    // Need to adjust for any reduction in Image Size
    featureData[SizeIndex]           = FFLOAT (areaBeforeReduction);
    featureData[EdgeSizeIndex]       = FFLOAT (edgeMomentf[0] * (FFLOAT)reductionFactor);
    featureData[WeighedMoment0Index] = weighedSizeBeforeReduction;
  }


  if ((area > convexf)  &&  (convexf > 0))
     featureData[TransparancyConvexHullIndex] = 1.0;
  else 
     featureData[TransparancyConvexHullIndex] = (FFLOAT)area / (FFLOAT)convexf;

  featureData[TransparancyPixelCountIndex] = (FFLOAT)area / (FFLOAT)tranf;
  featureData[TransparancyOpen3Index]      = (FFLOAT)(area - areaOpen3)  / (FFLOAT)area;
  featureData[TransparancyOpen5Index]      = (FFLOAT)(area - areaOpen5)  / (FFLOAT)area;
  featureData[TransparancyOpen7Index]      = (FFLOAT)(area - areaOpen7)  / (FFLOAT)area;                
  featureData[TransparancyOpen9Index]      = (FFLOAT)(area - areaOpen9)  / (FFLOAT)area;
  featureData[TransparancyClose3Index]     = (FFLOAT)(area - areaClose3) / (FFLOAT)area;
  featureData[TransparancyClose5Index]     = (FFLOAT)(area - areaClose5) / (FFLOAT)area;
  featureData[TransparancyClose7Index]     = (FFLOAT)(area - areaClose7) / (FFLOAT)area;
       
  {
    float  eigenRatio;
    float  orientationAngle;

    featureData[EigenRatioIndex] = 0.0;
    featureData[EigenHeadIndex]  = 0.0;

    raster->CalcOrientationAndEigerRatio (eigenRatio,
                                          orientationAngle
                                         );
    if  ((orientationAngle > TwoPie)  ||  (orientationAngle < -TwoPie))
    {
      orientationAngle = 0.0;
      eigenRatio = 1.0;
    }
    
    featureData[EigenRatioIndex] = eigenRatio;

    // Image will be rotated such that it slongest dimension will be paralell to the x-axis

    RasterSipperPtr rotatedImage = raster->Rotate (orientationAngle);

    int32  tlRow;
    int32  tlCol;
    int32  brRow;
    int32  brCol;

     rotatedImage->FindBoundingBox (tlRow, tlCol, brRow, brCol);
    if  (tlRow < 0)
    {
      featureData[EigenHeadIndex] = 9999;  
      // featureData[59]  = 9999;      // Prior 2 Format 18
      // featureData[151] = 9999;      // Format 18
    }
    else
    {
      int32 firstQtrWeight = 0;
      int32 forthQtrWeight = 0;

      int32 boundingLength = brCol - tlCol + 1;
      int32 boundingWidth  = brRow - tlRow + 1;

      featureData[LengthVsWidthIndex] = (FFLOAT)boundingWidth / (FFLOAT)boundingLength;  // Prior 2 Format 18
      featureData[LengthIndex]        = (FFLOAT)boundingLength * reductionFactor;
      featureData[WidthIndex]         = (FFLOAT)boundingWidth  * reductionFactor;
      featureData[FilledAreaIndex]    = (FFLOAT)areaWithWhiteSpace;  // Same as holeFilledArea
      
      int32 firstQtr = tlCol +  (boundingLength / 4);
      int32 thirdQtr = tlCol + ((boundingLength * 3) / 4);

      int32  row;
      int32  col;
      
      uchar** grey = rotatedImage->Green ();

      for  (row = tlRow;  row <= brRow;  row++)
      {
        for (col = tlCol;  col <= firstQtr;  col++)
        {
          if  (grey[row][col] > 0)
          {
            firstQtrWeight++;
          }
        }

        for (col = thirdQtr;  col <= brCol;  col++)
        {
          if  (grey[row][col] > 0)
          {
            forthQtrWeight++;
          }
        }
      }

      if  (saveImages)  
        Save (*rotatedImage, 
              "Rotated_" + StrFormatDouble (orientationAngle * 57.2958f, "zz0") + "_" 
              + StrFormatInt (firstQtrWeight, "####0") + "-" 
              + StrFormatInt (forthQtrWeight, "####0"), 
              saveImages
             );

      grey = NULL;

      if  ((firstQtrWeight < 1)  || (forthQtrWeight < 1))
      {
        // This should not happen;  but if it does will assign '0' to EigenHead feature.
        featureData[EigenHeadIndex] = 0;
      }
      else
      {
        if  (firstQtrWeight > forthQtrWeight)
        {
          featureData[EigenHeadIndex] = (FFLOAT)firstQtrWeight / (FFLOAT)forthQtrWeight;
        }
        else
        {
          featureData[EigenHeadIndex] = (FFLOAT)forthQtrWeight / (FFLOAT)firstQtrWeight;
        }
      }
    }

    delete  rotatedImage;
  }


  {
    // This part has to be done after 'CalcOrientationAndEigerRatio' is called.  That is where the image centroid is calced.
    centroidCol = raster->CentroidCol () * reductionFactor;
    centroidRow = raster->CentroidRow () * reductionFactor;

    // sfCentroid,  will use ImageFileName to calc Centroid with respect to sipper file.
    //  FileName format is 
    sfCentroidCol = -1.0f;
    sfCentroidRow = -1.0;

    if  (!ImageFileName ().Empty ())
    {
      KKStr  sipperFileName (30);
      uint32 scanLineNum = 0;
      uint32 scanColNum  = 0;

      ParseImageFileName (ImageFileName (), sipperFileName, scanLineNum, scanColNum);

      if  (!sipperFileName.Empty ())
      {
        sfCentroidCol = float  (centroidCol) + float  (scanColNum);
        sfCentroidRow = double (centroidRow) + double (scanLineNum);
      }
    }
  }


  #if defined (DEBUB_CalcFeatures)
    cout << "CalcFeatures   After Image Rotation." << std::endl;
  #endif


  featureData[ConvexAreaIndex]       = convexf * reductionFactorSquared;
  featureData[TransparancySizeIndex] = (float)(momentf[0] / convexf);
  featureData[TransparancyWtdIndex]  = (float)(momentfWeighted[0] / convexf);

  {
    //RasterSipperPtr  fourier  = raster->FastFourierKK ();
    RasterSipperPtr  fourier  = raster->FastFourier ();

    Save (*fourier, "Fourier", saveImages);
   
    fourier->FourierExtractFeatures (fourierFeatures);
    featureData[Fourier0Index] = fourierFeatures[0];
    featureData[Fourier1Index] = fourierFeatures[1];
    featureData[Fourier2Index] = fourierFeatures[2];
    featureData[Fourier3Index] = fourierFeatures[3];
    featureData[Fourier4Index] = fourierFeatures[4];

    delete  fourier;
  }


  {
    bool  successful;
    FFLOAT  countourFreq[5];
    FFLOAT  fourierDescriptors[15];

    {
      RasterSipper  countourImage (*raster);
      //countourImage.Dialation (SQUARE5);  


      int32  minExpectedLen = (int32)(float (numEdgePixelsFound) * 0.5f);

      successful = false;
      int32  numOfTries = 0;
      while  (!successful)
      {
        countourImage.Dialation (); // We want the image tob completly connected.
        {
          Save  (countourImage, "_JustBeforeContour", saveImages);
          ContourFollower contourFollower (countourImage, log);
          numOfEdgePixels = contourFollower.FollowContour (countourFreq, fourierDescriptors, (int32)featureData[SizeIndex], successful) * reductionFactor;

          if  (successful)
          {
            if  (numOfEdgePixels < minExpectedLen)
            {
              // We probably found a detached piece;  so we need to do another dialation  and try again.
              successful = false;
            }
          }
          if  (numOfTries > 3)
          {
            // We have already tried 3 times;  we willjust go with what we have.
            successful = true;
          }
          numOfTries++;
        }
      }

      if  (saveImages)
      {
        ContourFollower contourFollower2 (countourImage, log);
        PointListPtr  contourEdgePixels = contourFollower2.GenerateContourList ();
        RasterSipperPtr  r = new RasterSipper (countourImage.Height (), countourImage.Width (), false);
        r->DrawPointList (*contourEdgePixels, PixelValue (255, 255, 255));
        Save  (*r, "_JustBeforeContour_Edge", saveImages);
        delete  r;
        delete  contourEdgePixels;
      }
    }


    {
      featureData[ContourFourierDescriptor0] = countourFreq[0];
      featureData[ContourFourierDescriptor1] = countourFreq[1];
      featureData[ContourFourierDescriptor2] = countourFreq[2];
      featureData[ContourFourierDescriptor3] = countourFreq[3];
      featureData[ContourFourierDescriptor4] = countourFreq[4];

      featureData[FdRight1Index] = fourierDescriptors[0];

      featureData[FdLeft2Index]  = fourierDescriptors[1];
      featureData[FdRight2Index] = fourierDescriptors[2];

      featureData[FdLeft3Index]  = fourierDescriptors[3];
      featureData[FdRight3Index] = fourierDescriptors[4];

      featureData[FdLeft4Index]  = fourierDescriptors[5];
      featureData[FdRight4Index] = fourierDescriptors[6];

      featureData[FdLeft5Index]  = fourierDescriptors[7];
      featureData[FdRight5Index] = fourierDescriptors[8];

      featureData[FdLeft6Index]  = fourierDescriptors[9];
      featureData[FdRight6Index] = fourierDescriptors[10];

      featureData[FdLeft7Index]  = fourierDescriptors[11];
      featureData[FdRight7Index] = fourierDescriptors[12];

      featureData[FdLeft8Index]  = fourierDescriptors[13];
      featureData[FdRight8Index] = fourierDescriptors[14];
    }
  }


  #if defined (DEBUB_CalcFeatures)
    cout << "CalcFeatures   After   Fourier Descriptors." << std::endl;
  #endif


  FFLOAT  areaD = FFLOAT (areaBeforeReduction);
  FFLOAT  areaWithWhiteSpaceD = FFLOAT (areaWithWhiteSpace);

  featureData[IntensityHist1Index] = ((FFLOAT)intensityHistBuckets[1] / areaD);
  featureData[IntensityHist2Index] = ((FFLOAT)intensityHistBuckets[2] / areaD);
  featureData[IntensityHist3Index] = ((FFLOAT)intensityHistBuckets[3] / areaD);
  featureData[IntensityHist4Index] = ((FFLOAT)intensityHistBuckets[4] / areaD);
  featureData[IntensityHist5Index] = ((FFLOAT)intensityHistBuckets[5] / areaD);
  featureData[IntensityHist6Index] = ((FFLOAT)intensityHistBuckets[6] / areaD);
  featureData[IntensityHist7Index] = ((FFLOAT)intensityHistBuckets[7] / areaD);

  featureData[IntensityHistWS0Index] = ((FFLOAT)intensityHistBucketsB[0] / areaWithWhiteSpaceD);
  featureData[IntensityHistWS1Index] = ((FFLOAT)intensityHistBucketsB[1] / areaWithWhiteSpaceD);
  featureData[IntensityHistWS2Index] = ((FFLOAT)intensityHistBucketsB[2] / areaWithWhiteSpaceD);
  featureData[IntensityHistWS3Index] = ((FFLOAT)intensityHistBucketsB[3] / areaWithWhiteSpaceD);
  featureData[IntensityHistWS4Index] = ((FFLOAT)intensityHistBucketsB[4] / areaWithWhiteSpaceD);
  featureData[IntensityHistWS5Index] = ((FFLOAT)intensityHistBucketsB[5] / areaWithWhiteSpaceD);
  featureData[IntensityHistWS6Index] = ((FFLOAT)intensityHistBucketsB[6] / areaWithWhiteSpaceD);
  featureData[IntensityHistWS7Index] = ((FFLOAT)intensityHistBucketsB[7] / areaWithWhiteSpaceD);

  if  (saveImages)
  {
    RasterSipperPtr  thinnedImage = raster->ThinContour ();
    Save  (*thinnedImage, "_Thinned", saveImages);
    delete  thinnedImage;  
    thinnedImage = NULL;
  }


  featureData[0] = (float)areaBeforeReduction;  // Incase the image was reduced.
  OrigSize ((float)areaBeforeReduction);


  #if defined (DEBUB_CalcFeatures)
    cout << "CalcFeatures   After   Thinned Images." << std::endl;
    cout.flush ();
  #endif

  Probability (-1.0f);

  if  (weOwnRaster)
  {
    delete  raster;
    raster = NULL;
  }


  #if defined (DEBUB_CalcFeatures)
    cout << "CalcFeatures   Exiting Calcs." << std::endl;
  #endif

}  /* CalcFeatures */






ImageFeaturesList::ImageFeaturesList (FileDescPtr  _fileDesc,
                                      bool         _owner,
                                      RunLog&      _log,
                                      int32        _size
                                     ):

    FeatureVectorList (_fileDesc, _owner, _log, _size),
    version           (0)
{

}




ImageFeaturesList::ImageFeaturesList (RunLog&             _log,
                                      MLClassConstPtr  _mlClass,
                                      const KKStr&        _dirName,
                                      const KKStr&        _fileName
                                     ):

  FeatureVectorList (FeatureFileIOPices::NewPlanktonFile (_log), true, _log, 1000),
  version       (0)

{
  FeatureExtraction (_dirName, _fileName, _mlClass);
}




ImageFeaturesList::ImageFeaturesList (const ImageFeaturesList&  images):

   FeatureVectorList (images.FileDesc (), images.Owner (), images.log, images.QueueSize ()),
   version           (images.version)
{
  const_iterator  idx;
  for  (idx = images.begin ();  idx != images.end ();  idx++)
  {
    const ImageFeaturesPtr  imageExample = *idx;
    if  (Owner ())
      PushOnBack (new ImageFeatures (*imageExample));
    else
      PushOnBack (imageExample);
  }
}




ImageFeaturesList::ImageFeaturesList (const ImageFeaturesList&  images,
                                      bool                      _owner
                                     ):

   FeatureVectorList (images.FileDesc (), _owner, images.log, images.QueueSize ()),
   version           (images.version)
{
  const_iterator  idx;
  for  (idx = images.begin ();  idx != images.end ();  idx++)
  {
    const ImageFeaturesPtr  imageExample = *idx;
    if  (Owner ())
      PushOnBack (new ImageFeatures (*imageExample));
    else
      PushOnBack (imageExample);
  }
}


ImageFeaturesList::ImageFeaturesList (const FeatureVectorList&  featureVectorList,
                                      bool                      _owner
                                     ):

  FeatureVectorList (featureVectorList.FileDesc (),
                     _owner, 
                     featureVectorList.log, 
                     featureVectorList.QueueSize ()
                    ),
  version (0)

{
  if  (strcmp (featureVectorList.UnderlyingClass (), "ImageFeaturesList") == 0)
  {
    const ImageFeaturesList&  images = dynamic_cast<const ImageFeaturesList&> (featureVectorList);
    version = images.Version ();
  }


  if  (_owner)
  {
    FeatureVectorList::const_iterator  idx;
    for  (idx = featureVectorList.begin ();  idx != featureVectorList.end ();  idx++)
    {
      FeatureVectorPtr featureVector = *idx;
      
      // The constructor below will detect what the underlying type of 'featureVector' is.  
      // If (underlying type is a 'ImageFeatures' object)  then
      //   | Information that is particular to a 'ImageFeatures' object will be extracted
      //   | from the 'FeatureVector' object being passed in.
      // else
      //   | info that is particular to a 'ImageFeatures' object will be set to
      //   | default values.
      ImageFeaturesPtr  image = new ImageFeatures (*featureVector);
      PushOnBack (image);
    }
  }
  else
  {
    // Since we will not own the contents but just point to an existing instances we will 
    // have to make sure that the existing instances of 'FeatureVector' objects have a 
    // underlying type of 'ImageFeatures'.
    FeatureVectorList::const_iterator  idx;
    for  (idx = featureVectorList.begin ();  idx != featureVectorList.end ();  idx++)
    {
      FeatureVectorPtr featureVector = *idx;
      if  (strcmp (featureVector->UnderlyingClass (), "ImageFeatures") == 0)
      {
        ImageFeaturesPtr image = dynamic_cast<ImageFeaturesPtr>(featureVector);
        PushOnBack (image);
      }
      else
      {
        // ****    ERROR  ****
        KKStr  msg (200);
        msg << "ImageFeaturesList Copy Constructor for FeatureVectorList  Example[" <<  featureVector->ImageFileName () 
            << "] does not have a underlying type of 'ImageFeatures'.";
        log.Level (-1) << endl << endl << msg << endl << endl;
        throw KKStrException (msg);
      }
    }
  }
}









//****************************************************************************
//*  Will Create a list of images that are a subset of the ones in _images.  *
//* The subset will consist of the images who's mlClass is one of the     *
//* ones in mlClasses.                                                    *
//****************************************************************************
ImageFeaturesList::ImageFeaturesList (MLClassConstList&  _mlClasses,
                                      ImageFeaturesList&    _images,
                                      RunLog&               _log
                                     ):
  FeatureVectorList (_mlClasses, _images, _log),
  version           (_images.version)

{
}




ImageFeaturesList::ImageFeaturesList (const FeatureVectorList&  featureVectorList):
  FeatureVectorList (featureVectorList.FileDesc (),
                     featureVectorList.Owner (), 
                     featureVectorList.log, 
                     featureVectorList.QueueSize ()
                    ),
  version (0)

{
  if  (strcmp (featureVectorList.UnderlyingClass (), "ImageFeaturesList") == 0)
  {
    const ImageFeaturesList&  images = dynamic_cast<const ImageFeaturesList&> (featureVectorList);
    version = images.Version ();
  }


  if  (featureVectorList.Owner ())
  {
    FeatureVectorList::const_iterator  idx;
    for  (idx = featureVectorList.begin ();  idx != featureVectorList.end ();  idx++)
    {
      FeatureVectorPtr featureVector = *idx;
      
      // The constructor below will detect what the underlying type of 'featureVector' is.  
      // If (underlying type is a 'ImageFeatures' object)  then
      //   | Information that is particular to a 'ImageFeatures' object will be extracted
      //   | from the 'FeatureVector' object being passed in.
      // else
      //   | info that is particular to a 'ImageFeatures' object will be set to
      //   | default values.
      ImageFeaturesPtr  image = new ImageFeatures (*featureVector);
      PushOnBack (image);
    }
  }
  else
  {
    // Since we will not own the contents but just point to existing instances we will 
    // have to make sure that the existing instances of 'FeatureVector' objects have a 
    // underlying type of 'ImageFeatures'.
    FeatureVectorList::const_iterator  idx;
    for  (idx = featureVectorList.begin ();  idx != featureVectorList.end ();  idx++)
    {
      FeatureVectorPtr featureVector = *idx;
      if  (strcmp (featureVector->UnderlyingClass (), "ImageFeatures") == 0)
      {
        ImageFeaturesPtr image = dynamic_cast<ImageFeaturesPtr>(featureVector);
        PushOnBack (image);
      }
      else
      {
        // ****    ERROR  ****
        log.Level (-1) << endl << endl << endl
             << "ImageFeaturesList ::ImageFeaturesList (const FeatureVectorList&  featureVectorList)              ***ERROR***" << endl
             << endl
             << "One of the elements in 'featureVectorList' is not of 'ImageFeatures'  type.  We can not  recast this element"
             << "FileName[" << featureVector->ImageFileName () << "]"  << endl
             << endl;
        osWaitForEnter ();
        exit (-1);
      }
    }
  }
}





ImageFeaturesList::~ImageFeaturesList ()
{
}




ImageFeaturesPtr  ImageFeaturesList::IdxToPtr (int32 idx)  const
{
  return  (ImageFeaturesPtr)FeatureVectorList::IdxToPtr (idx);
}  /* IdxToPtr */




void  ImageFeaturesList::AddSingleImageFeatures (ImageFeaturesPtr  _imageFeatures)
{
  PushOnBack (_imageFeatures);
}



ImageFeaturesPtr  ImageFeaturesList::BackOfQueue ()
{
  if  (size () < 1)
    return NULL;

  FeatureVectorPtr  fv = back ();
  if  (strcmp (fv->UnderlyingClass (), "ImageFeatures") == 0)
    return  dynamic_cast<ImageFeaturesPtr> (fv);

  
  log.Level (-1) << endl << endl 
                 << "ImageFeaturesList::BackOfQueue ()    ***ERROR***        Entry at back of Queue is not a 'ImageFeatures' object." << endl
                 << endl;

  return NULL;
}  /* BackOfQueue */




ImageFeaturesPtr  ImageFeaturesList::PopFromBack ()
{
  if  (size () < 1)  return NULL;

  FeatureVectorPtr  fv = back ();
  if  (strcmp (fv->UnderlyingClass (), "ImageFeatures") != 0)
  {
    log.Level (-1)  << endl << endl 
                    << "ImageFeaturesList::BackOfQueue ()    ***ERROR***        Entry poped from back of Queue is not a 'ImageFeatures' object." << endl
                    << endl;
    return NULL;
  }

  return  dynamic_cast<ImageFeaturesPtr> (FeatureVectorList::PopFromBack ());
}  /* PopFromBack */






void  ImageFeaturesList::AddQueue (ImageFeaturesList&  imagesToAdd)
{
  if  (QueueSize () < 1)
  {
    // Since we don't have any images in the existing queue then the version number
    // of the queue that we are adding will be our version number.
    Version (imagesToAdd.Version ());
  }
  else
  {
    // Since there are images in both queues,  then they should have the same version number
    // otherwise the version number of the resulting KKQueue will be undefined (-1).
    if  (imagesToAdd.Version () != Version ())
      Version (0);
  }

  FeatureVectorList::AddQueue (imagesToAdd);
}  /* AddQueue */






ImageFeaturesPtr  ImageFeaturesList::BinarySearchByName (const KKStr&  _imageFileName)  const
{
  return  (ImageFeaturesPtr)FeatureVectorList::BinarySearchByName (_imageFileName);
}  /* BinarySearchByName */





ImageFeaturesPtr  ImageFeaturesList::LookUpByRootName (const KKStr&  _rootName)
{
  return  (ImageFeaturesPtr)FeatureVectorList::LookUpByRootName (_rootName);
}  /* LookUpByRootName */






ImageFeaturesPtr  ImageFeaturesList::LookUpByImageFileName (const KKStr&  _imageFileName)  const
{
  return  (ImageFeaturesPtr)FeatureVectorList::LookUpByImageFileName (_imageFileName);
}  /* LookUpByImageFileName */





ImageFeaturesListPtr  ImageFeaturesList::OrderUsingNamesFromAFile (const KKStr&  fileName)
{
  FeatureVectorListPtr  examples = FeatureVectorList::OrderUsingNamesFromAFile (fileName);
  examples->Owner (false);
  ImageFeaturesListPtr  orderedImages = new ImageFeaturesList (*examples);
  delete  examples;
  return  orderedImages;
}  /* OrderUsingNamesFromAFile */






void   ImageFeaturesList::FeatureExtraction (const KKStr&        _dirName, 
                                             const KKStr&        _fileName, 
                                             MLClassConstPtr  _mlClass
                                            )
{
  KKStr  className = _mlClass->Name ();
  log.Level (10) << "FeatureExtraction,  dirName   [" << _dirName    << "]." << endl;
  log.Level (10) << "                    fileName  [" << _fileName   << "]." << endl;
  log.Level (10) << "                    className [" << className   << "]." << endl;

  bool  cancelFlag  = false;
  bool  successful  = false;

  KKStr  fullFeatureFileName = osAddSlash (_dirName) + _fileName;

  KKStrListPtr   fileNameList;
  
  KKStr  fileSpec = osAddSlash (_dirName) + "*.*";
  fileNameList = osGetListOfFiles (fileSpec);
  if  (!fileNameList)
    return;

  bool  successfull;

  int32  numOfImages = fileNameList->QueueSize ();
  int32  count = 0;

  KKStrList::const_iterator  fnIDX;
  for  (fnIDX = fileNameList->begin ();  fnIDX != fileNameList->end ();  ++fnIDX)
  {
    if  ((count % 100) == 0)
      cout << className << " " << count << " of " << numOfImages << endl;

    KKStrPtr imageFileName = *fnIDX;
    
    bool validImageFileFormat = SupportedImageFileFormat (*imageFileName);
    if  (!validImageFileFormat)
      continue;

    KKStr  fullFileName = osAddSlash (_dirName) + (*imageFileName);
    ImageFeaturesPtr image = new ImageFeatures (fullFileName, _mlClass, successfull);

    if  (!image)
    {
      KKStr  msg (100);
      msg  << "ImageFeaturesList::FeatureExtraction  ***ERROR***, FeatureExtraction Could not Allocate Space for ImageFeatures object.";
      cerr << endl << endl << msg << endl << endl;
      osDisplayWarning (msg);
    }

    if  (!successfull)
    {
      log.Level (-1) << "ImageFeaturesList::FeatureExtraction  ***ERROR***, Processing file[" << imageFileName << "]." << endl;
      delete  image;
    }

    else
    {
      image->ImageFileName (imageFileName);
      log.Level (30) << image->ImageFileName () << "  " << image->OrigSize () << endl;
      PushOnBack (image);
    }

    count++;
  }

  Version (CurrentFeatureFileVersionNum);

  uint32  numExamplesWritten = 0;

  // WriteImageFeaturesToFile (fullFeatureFileName, RawFormat, FeatureNumList::AllFeatures (fileDesc));
  FeatureFileIOPices::Driver ()->SaveFeatureFile (fullFeatureFileName, 
                                                  FeatureNumList::AllFeatures (FileDesc ()), 
                                                  *this, 
                                                  numExamplesWritten,
                                                  cancelFlag,
                                                  successful,
                                                  log
                                                 );

  delete  fileNameList;  fileNameList = NULL;
}  /* FeatureExtraction */




VectorDouble  ImageFeaturesList::ExtractPositionsByMeter (InstrumentDataListPtr  instrumentData,
                                                          float                  defaultScanRate,
                                                          float                  defaultFlowRate
                                                         )
{
  SortBySpatialDistance ();

  ImageFeaturesList::iterator  idx;

  if  (defaultScanRate <= 0.0f)
    defaultScanRate = 24950.0f;

  if  (defaultFlowRate <= 0.0f)
    defaultFlowRate = 1.0f;

  uint32  frameSize = 4096;

  VectorDouble  frameOffsets;
  if  (instrumentData != NULL)
  {
    frameOffsets = instrumentData->FrameOffsetsInMeters (frameSize, defaultScanRate, defaultFlowRate);
  }

  VectorDouble  positions;
  double        position = 0.0;

  double  flowRate = defaultFlowRate;
  double  scanRate = defaultScanRate;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    ImageFeaturesPtr  example = *idx;
    position = 0.0;
    uint32  frameIdx = (uint32)(example->SfCentroidRow () / frameSize);
    uint32  scanLineToCalcFrom = 0;
    float lastFlowRate = defaultFlowRate;
    if  (frameIdx < frameOffsets.size ())
    {
      position = frameOffsets[frameIdx];
      scanLineToCalcFrom = frameIdx * frameSize;

      InstrumentDataPtr  id = instrumentData->SearchForNearestScanLine ((uint32)example->SfCentroidRow ());
      if  (!id)
        flowRate = defaultFlowRate;
      else
      {
        flowRate = id->FlowRate1 ();
        if  (flowRate <= 0.0)
          flowRate = defaultFlowRate;
      }
    }
    {
      // Add in the diference from the start of the frame.
      double  deltaScanLines = example->SfCentroidRow () - scanLineToCalcFrom;
      double  deltaTime = deltaScanLines / defaultScanRate;
      double  deltaDist = deltaTime * flowRate;
      position += deltaDist;
    }

    positions.push_back (position);
  }

  return  positions;
}  /* ExtractPositionsByMeter */






VectorInt   ImageFeaturesList::CreateSpatialDistributionHistogram (InstrumentDataListPtr  instrumentData,
                                                                   float                  defaultScanRate,   // Scan rate to use if it can not be located from 
                                                                   float                  defaultFlowRate,   // Flow rate to use if no instrumentation data.
                                                                   float                  bucketSize         // Size of each histogram bucket.
                                                                  )
{
  VectorDouble  positions = ExtractPositionsByMeter (instrumentData, defaultScanRate, defaultFlowRate);

  VectorInt     histogram;
  uint32        bucketIdx = 0;

  VectorDouble::iterator  idx;
  for  (idx = positions.begin ();  idx != positions.end ();  idx++)
  {
    bucketIdx = (int32)floor (*idx / bucketSize);

    if  (bucketIdx < 0)
      bucketIdx= 0;

    while  (histogram.size () <= bucketIdx)
      histogram.push_back (0);

    histogram[bucketIdx]++;
  }

  return  histogram;
}  /* CreateSpatialDistributionHistogram */





void  ImageFeaturesList::PrintSpatialHistogramReport (ostream&               r,
                                                      InstrumentDataListPtr  instrumentData,
                                                      float                  defaultScanRate,   // Scan rate to use if it can not be located from 
                                                      float                  defaultFlowRate,   // Flow rate to use if no instrumentation data.
                                                      float                  bucketSize
                                                     )
{
  VectorInt  allClassesHistogram = CreateSpatialDistributionHistogram (instrumentData, defaultScanRate, defaultFlowRate, bucketSize);

  vector<VectorInt>  histogramsByClass;

  MLClassConstListPtr  classes = ExtractMLClassConstList ();
  classes->SortByName ();

  uint32  maxHistogramSize = (uint32)allClassesHistogram.size ();


  MLClassConstList::iterator  idx;
  for  (idx = classes->begin ();  idx != classes->end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;

    ImageFeaturesListPtr  imagesThisClass = ExtractImagesForAGivenClass (ic);

    VectorInt  histogramThisClass = imagesThisClass->CreateSpatialDistributionHistogram (instrumentData, defaultScanRate, defaultFlowRate, bucketSize);
    maxHistogramSize = Max (maxHistogramSize, (uint32)histogramThisClass.size ());

    histogramsByClass.push_back (histogramThisClass);

    delete  imagesThisClass;   imagesThisClass = NULL;
  }

  KKStr  h1, h2, h3;
  classes->ExtractThreeTitleLines (h1, h2, h3);

  r << ""        << "\t" << ""        << "\t" << h1 << endl;
  r << ""        << "\t" << "All"     << "\t" << h2 << endl;
  r << "Bucket"  << "\t" << "Classes" << "\t" << h3 << endl;

  uint32  bucketIdx = 0;
  for  (bucketIdx = 0;  bucketIdx < maxHistogramSize;  bucketIdx++)
  {
    r << bucketSize * (float)bucketIdx;

    r << "\t";
    if  (bucketIdx < (uint32)allClassesHistogram.size ())
      r << allClassesHistogram[bucketIdx];
    else
      r << "";

    uint32  classIdx = 0;
    for  (classIdx = 0;  classIdx < (uint32)histogramsByClass.size ();  classIdx++)
    {
      r << "\t";
      if  ((bucketIdx < (uint32)histogramsByClass[classIdx].size ()))
        r << histogramsByClass[classIdx][bucketIdx];
      else
        r << "";
    }

    r << endl;
  }
   
  delete  classes; classes = NULL;
}  /* PrintSpatialHistogramReport */






/**
 * @brief  Creates a duplicate of list and also dupliactes it contents.
 * @return Duplicated list with hardcopy of its contents.
 */
ImageFeaturesListPtr  ImageFeaturesList::DuplicateListAndContents ()  const
{
  ImageFeaturesListPtr  copyiedList = new ImageFeaturesList (FileDesc (), true, log, QueueSize ());

  for  (int32 idx = 0;  idx < QueueSize ();  idx++)
  {
    ImageFeaturesPtr  curImage = (ImageFeaturesPtr)IdxToPtr (idx);
    copyiedList->AddSingleImageFeatures (new ImageFeatures (*curImage));
  }
  
  copyiedList->Version (Version ());

  return  copyiedList;
}  /* DuplicateListAndContents */






void  ImageFeaturesList::RecalcFeatureValuesFromImagesInDirTree (KKStr    rootDir,
                                                                 bool&    successful
                                                                )
{
  log.Level (20) << "RecalcFeatureValuesFromImagesInDirTree   RootDir[" << rootDir << "]." << endl;

  successful = false;

  ImageDirTree   fileDirectory (rootDir);

  if  (QueueSize () < 1)
  {
    successful = true;
    return;
  }

  if  (fileDirectory.Size () < 1)
  {
    log.Level (10) << "RecalcFeatureValuesFromImagesInDirTree  No Image Files in[" << rootDir << "]" << endl;
    return;
  }

  KKStrConstPtr  dirPath = NULL;

  iterator  idx;
  ImageFeaturesPtr  image = NULL;

  for  (idx = begin ();   idx != end ();  idx++)
  {
    image = *idx;
    dirPath = fileDirectory.LocateImage (image->ImageFileName ());
    if  (!dirPath)
    {
      log.Level (10) << "RecalcFeatureValuesFromImagesInDirTree  Could not locate Image[" << image->ImageFileName () << "] in Subdirectory Tree." << endl;
      return;
    }

    KKStr  fullFileName (dirPath);
    osAddLastSlash (fullFileName);
    fullFileName << image->ImageFileName ();

    bool   validFile;
    RasterSipperPtr  raster = new RasterSipper (fullFileName, validFile);
    if  (!validFile)
    {
      delete  raster;
      return;
    }
    image->CalcFeatures (*raster);
    delete  raster;  raster = NULL;
  }
}  /* RecalcFeatureValuesFromImagesInDirTree */





ImageFeaturesListPtr  ImageFeaturesList::ExtractDuplicatesByRootImageFileName ()
{
  FeatureVectorListPtr  duplicateFeatureVectorObjects = FeatureVectorList::ExtractDuplicatesByRootImageFileName ();
  ImageFeaturesListPtr  duplicateImageFeaturesObjects = new ImageFeaturesList (*duplicateFeatureVectorObjects);
  duplicateFeatureVectorObjects->Owner (false);
  delete  duplicateFeatureVectorObjects;  duplicateFeatureVectorObjects = NULL;
  return  duplicateImageFeaturesObjects;
}  /* ExtractDuplicatesByRootImageFileName */






ImageFeaturesListPtr   ImageFeaturesList::ExtractImagesForAGivenClass (MLClassConstPtr  _mlClass,
                                                                       int32               _maxToExtract,
                                                                       FFLOAT              _minSize
                                                                      )  const
{
  FeatureVectorListPtr  featureVectorList = FeatureVectorList::ExtractImagesForAGivenClass (_mlClass, _maxToExtract, _minSize);
  ImageFeaturesListPtr  imageFeaturesList = new ImageFeaturesList (*featureVectorList);
  featureVectorList->Owner (false);
  delete  featureVectorList;  featureVectorList = NULL;
  return  imageFeaturesList;
}  /*  ExtractImagesForAGivenClass  */





ImageFeaturesListPtr  ImageFeaturesList::StratifyAmoungstClasses (MLClassConstListPtr  mlClasses,
                                                                  int32                   maxImagesPerClass,
                                                                  int32                   numOfFolds
                                                                 )
{
  FeatureVectorListPtr  stratifiedFeatureVectors = FeatureVectorList::StratifyAmoungstClasses (mlClasses, maxImagesPerClass, numOfFolds);
  ImageFeaturesListPtr  stratifiedImagefeatures  = new ImageFeaturesList (*stratifiedFeatureVectors);
  stratifiedFeatureVectors->Owner (false);
  delete stratifiedFeatureVectors;  stratifiedFeatureVectors = NULL;
  return  stratifiedImagefeatures;
}  /* StratifyAmoungstClasses */




ImageFeaturesListPtr  ImageFeaturesList::StratifyAmoungstClasses (int32  numOfFolds)
{
  MLClassConstListPtr  classes = ExtractMLClassConstList ();

  FeatureVectorListPtr  stratifiedFeatureVectors = FeatureVectorList::StratifyAmoungstClasses (classes, -1, numOfFolds);
  ImageFeaturesListPtr  stratifiedImagefeatures  = new ImageFeaturesList (*stratifiedFeatureVectors);
  
  stratifiedFeatureVectors->Owner (false);

  delete stratifiedFeatureVectors;  stratifiedFeatureVectors = NULL;
  delete  classes;                  classes                  = NULL;

  return  stratifiedImagefeatures;
}  /* StratifyAmoungstClasses */







/**
 * @brief Fixed the two fields  sfCentroidCol & sfCentroidRow.
 *        This has to be done because teh original calcs for them assumed that  there
 *        would be no '_' characters in the sipper file name.
 */
void  ImageFeaturesList::FixSipperFileScanLineAndColFields ()
{
  iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    ImageFeaturesPtr  i = *idx;

    KKStr  rootName = osGetRootName (i->ImageFileName ());
    int32 x = rootName.LocateLastOccurrence ('_');
    if  (x > 0)
    {
      KKStr  colStr = rootName.SubStrPart (x + 1);
      KKStr  temp = rootName.SubStrPart (0, x - 1);
      x = temp.LocateLastOccurrence ('_');
      if  (x > 0)
      {
        KKStr  rowStr = temp.SubStrPart (x + 1);
        float  sfCentroidCol = float  (i->CentroidCol ()) + float  (atoi (colStr.Str ()));
        double sfCentroidRow = double (i->CentroidRow ()) + double (atoi (rowStr.Str ()));
        i->SfCentroidCol (sfCentroidCol);
        i->SfCentroidRow (sfCentroidRow);
      }
    }
  }
}  /* FixSipperFileScanLineAndColFields */




void  ImageFeaturesList::SortBySpatialDistance ()
{
  ImageFeaturesList::SpatialComparison  c;
  sort (begin (), end (), c);
}  /* SortBySpatialDistance */




float  ImageFeaturesList::FlowRate (InstrumentDataPtr  id,
                                    float              defaultFlowRate
                                   )
{
  if  ((!id)  ||  (id->FlowRate1 () <= 0.0f))
    return  defaultFlowRate;

  return  id->FlowRate1 ();
}



VectorFloat   ImageFeaturesList::CalculateDensitesByQuadrat (float        scanRate,         // Scan Lines per Sec.
                                                             float        quadratSize,      // Meters.
                                                             float        defaultFlowRate,  // Meters per Sec
                                                             const bool&  cancelFlag,
                                                             RunLog&      log
                                                            )
{
  InstrumentDataFileManager::InitializePush ();

  SortBySpatialDistance ();

  iterator  idx;
  idx = begin ();
  ImageFeaturesPtr  firstOne = *idx;

  float  curFlowRate      = defaultFlowRate;
  float  distLastImage    = 0.0f;
  double   scanLineLastImage = 0;

  VectorInt  imagesPerQuadrat;
  imagesPerQuadrat.push_back (0);

  InstrumentDataPtr  id = InstrumentDataFileManager::GetClosestInstrumentData (firstOne->ImageFileName (), cancelFlag, log);
  curFlowRate = FlowRate (id, defaultFlowRate);

  for  (idx = begin ();  idx != end ();  idx++)
  {
    ImageFeaturesPtr  i = *idx;

    if  (id  &&  ((i->SfCentroidRow () - id->ScanLine ()) > 4096))
    {
      // Time to get a more UpToDate Instrument Data Item
      id = InstrumentDataFileManager::GetClosestInstrumentData (i->ImageFileName (), cancelFlag, log);
      curFlowRate = FlowRate (id, defaultFlowRate);
    }

    double  deltaScanLines = i->SfCentroidRow () - scanLineLastImage;

    float  deltaDist = ((float)deltaScanLines / scanRate) * curFlowRate;

    float  newDist = distLastImage + deltaDist;

    uint32  quadratIdx = (int32)(newDist / quadratSize);
    while  (imagesPerQuadrat.size () <= quadratIdx)
      imagesPerQuadrat.push_back (0);
    imagesPerQuadrat[quadratIdx]++;

    distLastImage = newDist;
    scanLineLastImage = i->SfCentroidRow ();
  }


  //float  areaPerQuatdrat = quadratSize * 0.098f;
  float  volPerQuatdrat = quadratSize * 0.098f * 0.098f;

  VectorFloat  density (imagesPerQuadrat.size (), 0.0f);
  for  (uint32 x = 0;  x < imagesPerQuadrat.size ();  x++)
    density[x] = imagesPerQuadrat[x] / volPerQuatdrat;

  {
    uint32  i = 0;
    ofstream r ("c:\\Temp\\CalculateDensitesByQuadrat.txt");
    for  (i = 0; i < imagesPerQuadrat.size ();  i++)
      r << "i = " << i << "\t" << imagesPerQuadrat[i] << "\t" << "Density = " << "\t" << density[i] << endl;
    r << endl;

    r.close ();
  }
  

  InstrumentDataFileManager::InitializePop ();

  return  density;
}  /* CalculateDensitesByQuadrat */
