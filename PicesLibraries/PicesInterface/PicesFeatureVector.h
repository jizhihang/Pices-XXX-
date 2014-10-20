#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;


#include  "ImageFeatures.h"

#include  "PicesClass.h"
#include  "PicesClassList.h"
#include  "PicesInstrumentData.h"
#include  "PicesRaster.h"

#include  "PicesRunLog.h"


namespace PicesInterface 
{
  public  ref class  ParsedImageFileName
  {
  public:
    ParsedImageFileName (System::String^  _sipperFileName, 
                         uint             _scanLineNum,  
                         uint             _scanColNum
                        )
    {
      sipperFileName = _sipperFileName;
      scanLineNum    = _scanLineNum;
      scanColNum     = _scanColNum;
    }

     System::String^  sipperFileName;
     uint             scanLineNum; 
     uint             scanColNum;
  };



  public  ref class PicesFeatureVector
  {
  public:
    //**************************************************************************
    //* Will create a new instance of "features" that will be owned by the new *
    //* instance of "PicesFeatureVector" that is created.                      *
    //**************************************************************************
    PicesFeatureVector (PicesFeatureVector^  picesFeatureVector);  


    
    /**
     *@brief  Calculates features off the image that is in "raster".
     *@details Features that are based off instrumentation data will come from the IntrumentDataManager
     * the "imageFileName" will be parsed to get SipperFileName and ScanLine.
     *@param[in]  raster  Raster in the form of a 2 dmensional byte array.
     *@param[in]  imageFileName  Name of image that is stored in 'raster'; should follow Sipper file name conventions.
     *@param[in,out]  logging file.
     *@see  PicesLibrary::ImageFeatures::CalcFeatures
     */
    PicesFeatureVector (System::Array^   raster,
                        System::String^  imageFileName,
                        PicesRunLog^     log                        
                       );


    //**************************************************************************
    //* Will calculate features off the image that is in "raster"; see         *
    //* PicesLibrary::ImageFeatures::CalcFeatures.  Features that are          *
    //* based off instrumentation data will come from the IntrumentDataManager *
    //* the "imageFileName" will be parsed to get SipperFileName and ScanLine. *
    //**************************************************************************
    PicesFeatureVector (PicesRaster^     raster,
                        System::String^  imageFileName,
                        PicesRasterList^ intermediateImages,
                        PicesRunLog^     log
                       );

    PicesFeatureVector (ImageFeaturesPtr  _features);  // will take ownership of '_features'  from caller

    
    PicesFeatureVector (String^           _imageFileName,
                        PicesClass^       _mlClass,
                        PicesRasterList^  _intermediateImages,
                        PicesRunLog^      log
                       );

  private:
    ~PicesFeatureVector ();

  protected:
    !PicesFeatureVector ();


  public:
    property String^           ClassName          {String^     get ();}
    property System::DateTime  CtdDateTime        {System::DateTime    get ();}
    property float             Depth              {float       get ();  void  set (float        _depth);}
    property bool              FeatureDataMissing {bool        get ();}  // If feature values are 0.0f then will return 'true' else 'false'.
    property PicesClass^       MLClass         {PicesClass^ get ();  void  set (PicesClass^  _mlClass);}
    property String^           ImageFileName      {String^     get ();  void  set (String^      _imageFileName);}
    property int               NumFeatures        {int         get ();}
    property float             OrigSize           {float       get ();}
    property float             Probability        {float       get ();  void  set (float        _probability);}
    property bool              Validated          {bool        get ();  void  set (bool         _validated);}



    void  AddInstrumentData (System::Single^  depth,
                             System::Single^  salinity, 
                             System::Single^  oxygen, 
                             System::Single^  fluorescence
                            );

    void  AddInstrumentData (PicesInstrumentData^  pid);
    
    
    ImageFeaturesPtr  Features ()  {return  features;}


    String^  FeatureName (int featureNum);


    double  FeatureValue (int featureNum);


    static  ParsedImageFileName^  ParseImageFileName (System::String^  fullFileName);


    static  void  ParseImageFileName (String^   _fullFileName,
                                      String^%  _sipperFileName,
                                      uint%     _scanLine,
                                      uint%     _scanCol
                                     );

    ImageFeaturesPtr  UnManagedClass ()  {return  features;}



  private:
    void  CleanUpUnmanagedResources ();


    ImageFeaturesPtr  features;
    PicesClass^       mlClass;
  };  /* PicesFeatureVector */
}  /* PicesInterface */