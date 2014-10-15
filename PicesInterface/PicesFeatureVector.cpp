#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


#include  <ctype.h>
#include  <time.h>

#include  <fstream>
#include  <iostream>
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


#include "FileDesc.h"
#include "MLClass.h"
#include "FeatureFileIOPices.h"
#include "Raster.h"

#include "PicesKKStr.h"
#include "PicesRaster.h"
#include "PicesRunLog.h"
#include "PicesMethods.h"

#include "PicesFeatureVector.h"

using namespace  PicesInterface;


#define  MemPreasPerFV  430

namespace  PicesInterface
{
  PicesFeatureVector::PicesFeatureVector (PicesFeatureVector^  picesFeatureVector):
      features (NULL),
      mlClass (nullptr)
  {
    features    = new ImageFeatures (*(picesFeatureVector->Features ()));
    mlClass  = PicesClassList::GetUniqueClass (features->MLClass ());

    GC::AddMemoryPressure (MemPreasPerFV);
  }



  PicesFeatureVector::PicesFeatureVector (PicesRaster^      raster,
                                          System::String^   imageFileName,
                                          PicesRasterList^  intermediateImages,
                                          PicesRunLog^      log
                                         ):
     mlClass (nullptr)
  {
    mlClass = PicesClassList::GetUnKnownClassStatic ();
    RasterSipperPtr r = raster->UnmanagedClass ();
    if  (r == NULL)
    {
      features = new ImageFeatures (FeatureFileIOPices::PlanktonMaxNumOfFields ());
      return;
    }

    RasterSipperListPtr  unMangedIntermediateImages = NULL;
    if  (intermediateImages != nullptr)
      unMangedIntermediateImages = new RasterSipperList (true);

    if  (imageFileName != nullptr)
      r->FileName (PicesKKStr::SystemStringToKKStr (imageFileName));

    features = new ImageFeatures (*r, mlClass->UnmanagedMLClass (), unMangedIntermediateImages);

    if  (unMangedIntermediateImages)
    {
      intermediateImages->AddList (unMangedIntermediateImages);
      unMangedIntermediateImages = NULL;
    }

    GC::AddMemoryPressure (MemPreasPerFV);
  }




  PicesFeatureVector::PicesFeatureVector (System::Array^   raster,
                                          System::String^  imageFileName,
                                          PicesRunLog^     log
                                         ):
     mlClass (nullptr)
  {
    mlClass = PicesClassList::GetUnKnownClassStatic ();
    RasterPtr r = PicesRaster::BuildRasterObj (raster);
    if  (r == NULL)
    {
      features = new ImageFeatures (FeatureFileIOPices::PlanktonMaxNumOfFields ());
      return;
    }

    if  (imageFileName != nullptr)
      r->FileName (PicesKKStr::SystemStringToKKStr (imageFileName));

    features = new ImageFeatures (*r, mlClass->UnmanagedMLClass ());

    delete  r;  r = NULL;
    GC::AddMemoryPressure (MemPreasPerFV);
  }



  PicesFeatureVector::PicesFeatureVector (ImageFeaturesPtr  _features):
     mlClass (nullptr)
  {
    if  (_features == NULL)
      throw gcnew Exception ("PicesFeatureVector   NULL passed into constructor.");

    features = _features;
    mlClass = PicesClassList::GetUniqueClass (PicesKKStr::KKStrToSystenStr (features->MLClass ()->Name ()), String::Empty);
    GC::AddMemoryPressure (MemPreasPerFV);
  }



  PicesFeatureVector::PicesFeatureVector (String^           _imageFileName,
                                          PicesClass^       _mlClass,
                                          PicesRasterList^  _intermediateImages,
                                          PicesRunLog^      log
                                         )
  {
    bool  successful = false;
    if  (mlClass == nullptr)
      mlClass = PicesClassList::GetUnKnownClassStatic ();


    RasterSipperListPtr  unManagedIntermediateImages = NULL;
    if  (_intermediateImages != nullptr)
      unManagedIntermediateImages = new RasterSipperList (true);

    features = new ImageFeatures (PicesKKStr::SystemStringToKKStr (_imageFileName),
                                  mlClass->UnmanagedMLClass (), 
                                  successful, 
                                  unManagedIntermediateImages
                                 );
    GC::AddMemoryPressure (MemPreasPerFV);

    if  (unManagedIntermediateImages)
    {
      _intermediateImages->AddList (unManagedIntermediateImages);  // Will take owneship of 'unManagedIntermediateImages' contents and delete the list.
      unManagedIntermediateImages = NULL;
    }
  }



  PicesFeatureVector::~PicesFeatureVector ()
  {
    mlClass = nullptr;
    this->!PicesFeatureVector ();
  }



  PicesFeatureVector::!PicesFeatureVector ()
  {
    CleanUpUnmanagedResources ();
  }



  void  PicesFeatureVector::CleanUpUnmanagedResources ()
  {    
    delete  features;  features = NULL;
    GC::RemoveMemoryPressure (MemPreasPerFV);
  }



  String^  PicesFeatureVector::ClassName::get ()  
  {
    if  (mlClass != nullptr)
      return  mlClass->Name;

    if  (features == NULL)
      return  "";

    return  PicesKKStr::KKStrToSystenStr (features->ClassName ());
  }



  System::DateTime  PicesFeatureVector::CtdDateTime::get ()
  {
    return  PicesMethods::DateTimeKKUtoSystem (features->CtdDateTime ());
  }
 



  float  PicesFeatureVector::Depth::get ()
  {
    if  (!features)
      return  0.0f;
    return  features->Depth ();
  }



  PicesClass^   PicesFeatureVector::MLClass::get ()
  {
    if  (mlClass != nullptr)
      return mlClass;

    if  (!features)
      mlClass = PicesClassList::GetUnKnownClassStatic ();
    else
      mlClass = PicesClassList::GetUniqueClass (features->MLClass ());

    return  mlClass;
  }



  String^  PicesFeatureVector::ImageFileName::get ()  
  {
    if  (!features)
      return "";

    return  PicesKKStr::KKStrToSystenStr (features->ImageFileName ());
  }



  int  PicesFeatureVector::NumFeatures::get ()
  {
    if  (!features)
      return 0;

    return  features->NumOfFeatures ();
  }



  float  PicesFeatureVector::OrigSize::get ()
  {
    if  (!features)
      return 0.0f;
    return  features->OrigSize ();
  }



  float  PicesFeatureVector::Probability::get ()
  {
    if  (!features)
      return 0.0f;
    return  features->Probability ();
  }



  bool   PicesFeatureVector::Validated::get ()
  {
    if  (!features)
      return false;

    return features->Validated ();
  }




  void  PicesFeatureVector::MLClass::set (PicesClass^  picesClass)
  {
    if  (picesClass == nullptr)
      picesClass = PicesClassList::GetUnKnownClassStatic ();

    mlClass = picesClass;

    if  (!features)
      return;

     features->MLClass (picesClass->UnmanagedMLClass ());
  }





  void  PicesFeatureVector::ImageFileName::set (String^ _imageFileName)
  {
    if  (!features)
      return;

    features->ImageFileName (PicesKKStr::SystemStringToKKStr (_imageFileName));
  }




  void  PicesFeatureVector::Probability::set (float _probability)
  {
    if  (features)
    {
      features->Probability (_probability);
    }
  }





  void  PicesFeatureVector::Depth::set (float _depth)
  {
    if  (features)
    {
      features->Depth (_depth);
    }
  }





  void   PicesFeatureVector::Validated::set (bool  _validated)
  {
    if  (features)
      features->Validated (_validated);
  }



  bool   PicesFeatureVector::FeatureDataMissing::get ()
  {
    return  (features->FeatureData (0) == 0.0f);   // Feature 0 is the Size feature;  this feature has to have a value greater than 0.0
  }



  String^  PicesFeatureVector::FeatureName (int featureNum)
  {
    return  PicesKKStr::KKStrToSystenStr (FeatureFileIOPices::PlanktonFieldName (featureNum));
  }



  double  PicesFeatureVector::FeatureValue (int featureNum)
  {
    if  (!features)
      return 0.0;

    if  ((featureNum < 0)  ||  (featureNum >= this->features->NumOfFeatures ()))
      return 0.0;

    return  features->FeatureData (featureNum);
  }



  void  PicesFeatureVector::AddInstrumentData (System::Single^  depth,
                                               System::Single^  salinity, 
                                               System::Single^  oxygen, 
                                               System::Single^  fluorescence
                                              )
  {
    features->Depth        (*depth);
    features->Salinity     (*salinity);
    features->Oxygen       (*oxygen);
    features->Fluorescence (*fluorescence);
  }  /* AddInstrumentData */



  void  PicesFeatureVector::AddInstrumentData (PicesInstrumentData^  pid)
  {
    if  (features->Depth () == 0.0)
    {
      features->Depth        (pid->Depth);
      features->Salinity     (pid->Salinity);
      features->Oxygen       (pid->Oxygen);
      features->Fluorescence (pid->Fluorescence);
    }

    if  (features->FlowRate1 () == 0.0)
    {
      features->FlowRate1 (pid->FlowRate1);
      features->FlowRate2 (pid->FlowRate2);
    }
  }  /* AddInstrumentData */




  ParsedImageFileName^  PicesFeatureVector::ParseImageFileName (System::String^  _fullFileName)
  {
    KKStr  fullFileName = PicesKKStr::SystemStringToKKStr (_fullFileName);

    KKStr  sipperFileName (30);
    uint   scanLineNum;
    uint   scanColNum;

    ImageFeatures::ParseImageFileName (fullFileName, sipperFileName, scanLineNum, scanColNum);


    System::String^ _sipperFileName = PicesKKStr::KKStrToSystenStr (sipperFileName);

    return  gcnew ParsedImageFileName (_sipperFileName, scanLineNum, scanColNum);

  }  /* ParseImageFileName */




  void  PicesFeatureVector::ParseImageFileName (String^   _fullFileName,
                                                String^%  _sipperFileName,
                                                uint%     _scanLine,
                                                uint%     _scanCol
                                               )
  {
    KKStr  fullFileName = PicesKKStr::SystemStringToKKStr (_fullFileName);

    KKStr  sipperFileName (64);
    uint   scanLine;
    uint   scanCol;

    ImageFeatures::ParseImageFileName (fullFileName, sipperFileName, scanLine, scanCol);

    _sipperFileName = PicesKKStr::KKStrToSystenStr (sipperFileName);
    _scanLine = scanLine;
    _scanCol  = scanCol;
  }  /* ParseImageFileName */

} /* PicesInterface */