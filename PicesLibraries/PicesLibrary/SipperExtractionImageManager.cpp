#include  "FirstIncludes.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

#include "MemoryDebug.h"
#include "BasicTypes.h"
#include "BMPImage.h"
#include "ImageIO.h"
#include "OSservices.h"
using namespace KKU;


#include "SipperExtractionImageManager.h"

#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "MLClass.h"
#include "ImageFeatures.h"
using namespace MLL;
//#include  "SipperImageExtractionParms.h"


#define  BucketSizeSmall    10
#define  BucketSizeMed     100
#define  BucketSizeLarge  1000
#define  BucketCount        50


namespace  MLL
{

class  SipperExtractionImageManager::ImageEntry
{
public:
  ImageEntry (const KKStr&        _fileName,
              int32               _size,
              MLClassConstPtr  _predClass
             ):
       fileName  (_fileName),
       predClass (_predClass),
       size      (_size)
   {}

  KKStr               fileName;
  MLClassConstPtr  predClass;
  int32               size;
};  /* ImageEntry */




class  SipperExtractionImageManager::ManagedClass
{
public:
  ManagedClass (FileDescPtr         _fileDesc,
                MLClassConstPtr  _mlClass,
                const KKStr&        _rootDir,
                bool                _saveFeatureData,
                bool                _countOnly,
                uint32              _imagesPerDirectory,
                RunLog&             _log
               );

  ~ManagedClass ();

  const KKStr&        ClassName      () const;
  const KKStr&        ClassNameUpper () const;
  MLClassConstPtr  MLClass     () const  {return mlClass;}
  int32               ImageCount     () const  {return imageCount;}

  void  AddImage (const KKStr&        fileName,
                  MLClassConstPtr  predClass,
                  int32               size,
                  ImageFeaturesPtr    example,
                  RasterPtr           raster,
                  bool                colorize
                 );

  void  CloseOutCurDirectory ();

  void  Flush ();

  void  SaveFrame (const KKStr&    fileName, 
                         RasterPtr  raster,
                         bool       colorize
                  );

  void  StartNewDirectory ();

private:
  bool                  countOnly;
  int32                 curSubDirNum;
  KKStr                 curSubDirName;
  ImageEntryListPtr     entries;
  ImageFeaturesListPtr  examplesCurDir;
  FileDescPtr           fileDesc;
  KKStr                 fullSubDirName;
  MLClassConstPtr    mlClass;
  int32                 imageCount;
  uint32                imagesPerDirectory;
  RunLog&               log;
  KKStr                 rootDir;
  bool                  saveFeatureData;
};  /* ManagedClass */




SipperExtractionImageManager::SipperExtractionImageManager (FileDescPtr    _fileDesc,
                                                            const KKStr&   _rootDir,
                                                            bool           _saveFeatureData,
                                                            int32            _veryLargeImageSize,
                                                            bool           _imagesAreClassified,
                                                            bool           _countOnly,
                                                            uint32         _imagesPerDirectory,
                                                            RunLog&        _log
                                                           ):
     allExamples           (NULL),
     countOnly             (_countOnly),
     depthDistribution_1   (500,  1, _log),    // A distribution by class and depth
     depthDistribution_10  (50,  10, _log),    // A distribution by class and depth
     fileDesc              (_fileDesc),
     imagesAreClassified   (_imagesAreClassified),
     imagesPerDirectory    (_imagesPerDirectory),
     log                   (_log),
     largeImages           (NULL),
     managedClasses        (),
     rootDir               (_rootDir),
     saveFeatureData       (_saveFeatureData),
     sizeDistributionLarge (BucketCount, BucketSizeLarge, _log),
     sizeDistributionMed   (BucketCount, BucketSizeMed,   _log), 
     sizeDistributionSmall (BucketCount, BucketSizeSmall, _log),
     veryLargeImageSize    (_veryLargeImageSize)

{
  if  ((imagesPerDirectory < 10)  ||  (imagesPerDirectory > 1000000))
  {
    log.Level (-1) << endl << endl << endl
                   << "SipperExtractionImageManager::SipperExtractionImageManager" << endl
                   << "                              ImagesPerDirectory [" << imagesPerDirectory << "]  is not valid,  resetting to 1000."  << endl 
                   << endl;
    imagesPerDirectory = 1000;
  }


  allExamples = new ImageFeaturesList (fileDesc, 
                                       true,     // will own images/examples 
                                       log
                                      );

  MLClassConstPtr  largeMLClass = MLClass::CreateNewMLClass ("VeryLargeImages");
  largeImages = new ManagedClass (fileDesc,
                                  largeMLClass,
                                  rootDir,
                                  saveFeatureData,
                                  countOnly,
                                  imagesPerDirectory,
                                  log
                                 );

  managedClasses.insert (pair<MLClassConstPtr, ManagedClassPtr> (largeMLClass, largeImages));
}



SipperExtractionImageManager::~SipperExtractionImageManager ()
{
  {
    ManagedClassList::iterator   idx;
    for  (idx = managedClasses.begin ();  idx != managedClasses.end ();  idx++)
    {
      delete  idx->second;
    }
  }

  delete  allExamples;  allExamples = NULL;
  // delete  largeImages;  largeImages = NULL;  Will be deleted when we delete managedClasses.
}



void  SipperExtractionImageManager::ReportStatistics (ostream&  r)  const
{
  log.Level (10) << "SipperExtractionImageManager::ReportStatistics" << endl;

  r << endl << endl << endl
    << "Large Size Distribution" << endl
    << endl;

  sizeDistributionLarge.PrintFormatedDistributionMatrix (r);

  r << endl << endl << endl
    << "Medium Size Distribution" << endl
    << endl;

  sizeDistributionMed.PrintFormatedDistributionMatrix (r);

  r << endl << endl << endl
    << "Small Size Distribution" << endl
    << endl;

  sizeDistributionSmall.PrintFormatedDistributionMatrix (r);

  r << endl << endl << endl
    << "       Distribution by depth" << endl
    << "===================================" << endl
    << endl;

  depthDistribution_10.PrintFormatedDistributionMatrix (r);

  r << endl << endl << endl << endl
    << "Tab Deleimited Reports"  << endl;

  r << endl << endl << endl
    << "Large Size Distribution" << endl
    << endl;

  sizeDistributionLarge.PrintTabDelDistributionMatrix (r);

  r << endl << endl << endl
    << "Medium Size Distribution" << endl
    << endl;

  sizeDistributionMed.PrintTabDelDistributionMatrix (r);

  r << endl << endl << endl
    << "Small Size Distribution" << endl
    << endl;

  sizeDistributionSmall.PrintTabDelDistributionMatrix (r);


  r << endl << endl << endl
    << "       Distribution by depth" << endl
    << "===================================" << endl
    << endl;

  depthDistribution_10.PrintTabDelDistributionMatrix (r);
}  /* ReportStatistics */








void  SipperExtractionImageManager::ReportStatsByDepth (ostream&      r,
                                                        VectorUlong*  scanLinesPerMeterDepth
                                                       )  const
{
  log.Level (10) << "SipperExtractionImageManager::ReportStatsByDepth" << endl;
  
  r << endl << endl << endl
    << " Distribution by depth 1 Meter Increments" << endl
    << "==========================================" << endl
    << endl;

  depthDistribution_1.PrintByClassCollumns (r, scanLinesPerMeterDepth);

  r << endl << endl << endl
    << " Distribution by depth 10 Meter Increments" << endl
    << "==========================================" << endl
    << endl;

  depthDistribution_10.PrintByClassCollumns (r, scanLinesPerMeterDepth);
  r << endl;
}  /* ReportStatsByDepth */




SipperExtractionImageManager::ManagedClassPtr  SipperExtractionImageManager::ManagedClassLookUp (MLClassConstPtr  mlClass)
{
  ManagedClassList::iterator p;
  p = managedClasses.find (mlClass);
  if (p != managedClasses.end ())
    return p->second;

  ManagedClassPtr  managedClass = new ManagedClass (fileDesc,
                                                    mlClass,
                                                    rootDir,
                                                    saveFeatureData,
                                                    countOnly,
                                                    imagesPerDirectory,
                                                    log
                                                   );


  managedClasses.insert (pair<MLClassConstPtr, ManagedClassPtr> (mlClass, managedClass));

  return managedClass;
}  /* ManagedClassLookUp */




void  SipperExtractionImageManager::AddImage (const KKStr&        fileName,
                                              MLClassConstPtr  predClass,
                                              int32               size,
                                              float               depth,
                                              ImageFeaturesPtr    example,
                                              RasterPtr           raster,
                                              bool                colorize
                                             )
{
  sizeDistributionLarge.Increment (predClass, size);
  sizeDistributionMed.Increment   (predClass, size);
  sizeDistributionSmall.Increment (predClass, size);
  depthDistribution_10.Increment  (predClass, int32 (floor (depth)));
  depthDistribution_1.Increment   (predClass, int32 (floor (depth)));

  if  (size > veryLargeImageSize)
  {
    largeImages->AddImage (fileName, predClass, size, example, raster, colorize);
  }
  else
  {
    lastManagedClassUsed = ManagedClassLookUp (predClass);
    lastManagedClassUsed->AddImage (fileName, predClass, size, example, raster, colorize);
  }

  if  (saveFeatureData)
  {
    allExamples->PushOnBack (example);
  }
}  /* AddImage */





void  SipperExtractionImageManager::Flush ()
{
  log.Level (10) << "SipperExtractionImageManager::Flush" << endl;

  ManagedClassList::iterator   idx;
  for  (idx = managedClasses.begin ();  idx != managedClasses.end ();  idx++)
    idx->second->Flush ();

  if  (saveFeatureData)
  {
    KKStr  rootFeatureFileName = osAddSlash (rootDir) + osGetRootNameOfDirectory (rootDir) + ".data";

    bool  cancelFlag = false;
    bool  successful = true;
    FeatureFileIOPices::Driver ()->SaveFeatureFileMultipleParts 
                                 (rootFeatureFileName,
                                  allExamples->AllFeatures (),
                                  *allExamples,
                                  cancelFlag,
                                  successful,
                                  log
                                 );
  }

  log.Level (10) << "SipperExtractionImageManager::Flush    Done Flushing." << endl;
}  /* Flush */




void  SipperExtractionImageManager::SaveFrame (const KKStr&    _fileName,
                                                     RasterPtr  _raster,
                                                     bool       _colorize
                                              )
{
  if  (imagesAreClassified)
  {
    MLClassConstPtr  frameClass = MLClass::CreateNewMLClass ("Frames");
    ManagedClassPtr  frameManager = ManagedClassLookUp (frameClass);
    frameManager->SaveFrame (_fileName, _raster, _colorize);
  }
  else
  {
    MLClassConstPtr  unknowClass = MLClass::GetUnKnownClassStatic ();
    ManagedClassPtr  unknownManagedClass = ManagedClassLookUp (unknowClass);
    unknownManagedClass->SaveFrame (_fileName, _raster, _colorize);
  }
}  /* SaveFrame */





class  SipperExtractionImageManager::ManagedClasssesSortByClassName
{
public:
  ManagedClasssesSortByClassName () {}

   bool  operator () (ManagedClassPtr  p1,
                      ManagedClassPtr  p2
                     )
   {
     return  (p1->ClassNameUpper () < p2->ClassNameUpper ());
   }
};  /* ManagedClasssesSortByClassName */




vector<SipperExtractionImageManager::ManagedClassPtr>  SipperExtractionImageManager::SortByClassName ()  const
{
  ManagedClassList::const_iterator idx;

  vector<ManagedClassPtr>  sortedList;
  
  for  (idx = managedClasses.begin ();  idx != managedClasses.end ();  idx++)
  {
    sortedList.push_back (idx->second);
  }

  ManagedClasssesSortByClassName  sortObj;

  sort (sortedList.begin (), sortedList.end (), sortObj);

  return  sortedList;
}  /* SortByClassName */




ClassStatisticListPtr  SipperExtractionImageManager::ClassStats ()  const
{
  ClassStatisticListPtr classStats = new ClassStatisticList (true);

  ManagedClassList::const_iterator idx;
  for  (idx = managedClasses.begin ();  idx != managedClasses.end ();  idx++)
  {
    ManagedClassPtr  managedClass = idx->second;
    classStats->PushOnBack (new ClassStatistic (managedClass->MLClass (), managedClass->ImageCount ()));
  }

  classStats->SortByCount ();

  return  classStats;
}  /* ClassStats */




SipperExtractionImageManager::ManagedClass::ManagedClass 
                           (FileDescPtr         _fileDesc,
                            MLClassConstPtr  _mlClass,
                            const KKStr&        _rootDir,
                            bool                _saveFeatureData,
                            bool                _countOnly,
                            uint32              _imagesPerDirectory,
                            RunLog&             _log
                           ):
    
    countOnly          (_countOnly),
    curSubDirNum       (-1),
    curSubDirName      (),
    entries            (NULL),
    examplesCurDir     (NULL),
    fileDesc           (_fileDesc),
    fullSubDirName     (),
    mlClass         (_mlClass),
    imageCount         (0),
    imagesPerDirectory (_imagesPerDirectory),
    log                (_log),
    rootDir            (_rootDir),
    saveFeatureData    (_saveFeatureData)

{
  StartNewDirectory ();
}



SipperExtractionImageManager::ManagedClass::~ManagedClass ()
{
  if  (entries)
    CloseOutCurDirectory ();

  delete  entries;         entries        = NULL;
  delete  examplesCurDir;  examplesCurDir = NULL;
}




const  KKStr&  SipperExtractionImageManager::ManagedClass::ClassName () const
{
  if  (mlClass)
    return  mlClass->Name ();
  else
    return  KKStr::EmptyStr ();
}  /* ClassName */



const  KKStr&  SipperExtractionImageManager::ManagedClass::ClassNameUpper () const
{
  if  (mlClass)
    return  mlClass->UpperName ();
  else
    return  KKStr::EmptyStr ();
}  /* ClassName */





void  SipperExtractionImageManager::ManagedClass::StartNewDirectory ()
{
  curSubDirNum++;
  curSubDirName = mlClass->Name () + "_" + StrFormatInt (curSubDirNum, "00");
  fullSubDirName = osAddSlash (rootDir) + curSubDirName;

  
  if  (!countOnly)
    osCreateDirectoryPath (fullSubDirName);

  delete  entries;  entries = NULL;
  entries = new ImageEntryList (true);

  if  (saveFeatureData)
  {
    delete  examplesCurDir;  examplesCurDir = NULL;
    examplesCurDir = new ImageFeaturesList (fileDesc, 
                                            false,     // false = We will not own these examples
                                            log,
                                            1000
                                           );
  }
}  /* StartNewDirectory */





void  SipperExtractionImageManager::ManagedClass::AddImage 
                             (const KKStr&         fileName,
                              MLClassConstPtr   predClass,
                              int32                size,
                              ImageFeaturesPtr     example,
                              RasterPtr            raster,
                              bool                 colorize
                             )
{
  if  (!entries)
    StartNewDirectory ();

  if  (entries->size () >= imagesPerDirectory)
  {
    CloseOutCurDirectory ();
    StartNewDirectory ();
  }

  KKStr  fullFileName = osAddSlash (fullSubDirName) + fileName;

  if  (!countOnly)
  {
    BmpImage  bmpImage (*raster);

    if  (colorize)
    {
      //                                          R    G    B
      bmpImage.SetPaletteEntry (  0, PixelValue (255, 255, 255));
      bmpImage.SetPaletteEntry ( 36, PixelValue (255, 255,   0));
      bmpImage.SetPaletteEntry ( 73, PixelValue (255,   0, 255));
      bmpImage.SetPaletteEntry (109, PixelValue (255,   0,   0));
      bmpImage.SetPaletteEntry (146, PixelValue (  0, 255, 255));
      bmpImage.SetPaletteEntry (182, PixelValue (  0, 255,   0));
      bmpImage.SetPaletteEntry (219, PixelValue (  0,   0, 255));
      bmpImage.SetPaletteEntry (255, PixelValue (  0,   0,   0));
      bmpImage.Save (fullFileName);
    }
    else
    {
      bmpImage.SaveGrayscaleInverted4Bit (fullFileName);
    }
  }


  //{
  //  KKStr  thinFileName = osRemoveExtension (fullFileName)  + "_BandPass.bmp";
  //  RasterPtr  thinnedImage = raster->BandPass (0.0f, 0.3f);
  //  SaveImage  (*thinnedImage, thinFileName);
  //  delete  thinnedImage;  
  //  thinnedImage = NULL;
  //}


  ImageEntryPtr  imageEntry = new ImageEntry (fileName, size, predClass);
  entries->PushOnBack (imageEntry);

  if  (saveFeatureData)
  {
    example->ImageFileName (fileName);
    examplesCurDir->PushOnBack (example);
  }

  imageCount++;
}  /* AddImage */




void  SipperExtractionImageManager::ManagedClass::CloseOutCurDirectory ()
{
  delete  entries; entries = NULL;

  if  ((saveFeatureData)  &&  (!countOnly))
  {
    KKStr  fullFeatureFileName = osAddSlash (fullSubDirName) + curSubDirName + ".data";

    bool    cancelFlag  = false;
    bool    successful  = false;
    uint32  numExamplesWritten = 0;

    FeatureFileIOPices::Driver ()->SaveFeatureFile (fullFeatureFileName, 
                                                    examplesCurDir->AllFeatures (),
                                                    *examplesCurDir,
                                                    numExamplesWritten,
                                                    cancelFlag,
                                                    successful,
                                                    log
                                                   );

    // Now that we saved the local features for the local directory,
    // we need to add the subDir info that is needed for 'allExamples' 
    // in 'SipperExtractionImageManager'.  
    //
    // 'examplesCurDir' only has pointers to teh imageFeatures objects, 
    // 'allExamples' owns them, so when we change the ones pointed to by 
    // 'examplesCurDir' we will be changing the ones pointed to by
    // 'allExamples'.

    ImageFeaturesList::const_iterator  idx;
    for  (idx = (ImageFeaturesList::const_iterator)examplesCurDir->begin ();  idx != examplesCurDir->end ();  idx++)
    {
      ImageFeaturesPtr  example = *idx;
      KKStr  fullFileName = osAddSlash (curSubDirName) + example->ImageFileName ();
      example->ImageFileName (fullFileName);
    }

    delete  examplesCurDir;  examplesCurDir = NULL;
  }
}  /* CloseOutCurDirectory */




void  SipperExtractionImageManager::ManagedClass::SaveFrame 
                             (const KKStr&    fileName, 
                                     RasterPtr  raster,
                                     bool       colorize
                              )
{
  if  (!countOnly)
  {
    KKStr  fullFileName = osAddSlash (fullSubDirName) + fileName;
    BmpImage  bmpImage (*raster);
    if  (colorize)
    {
      //                                          R    G    B
      bmpImage.SetPaletteEntry (  0, PixelValue (255, 255, 255));
      bmpImage.SetPaletteEntry ( 36, PixelValue (255, 255,   0));
      bmpImage.SetPaletteEntry ( 73, PixelValue (255,   0, 255));
      bmpImage.SetPaletteEntry (109, PixelValue (255,   0,   0));
      bmpImage.SetPaletteEntry (146, PixelValue (  0, 255, 255));
      bmpImage.SetPaletteEntry (182, PixelValue (  0, 255,   0));
      bmpImage.SetPaletteEntry (219, PixelValue (  0,   0, 255));
      bmpImage.SetPaletteEntry (255, PixelValue (  0,   0,   0));
    }
    bmpImage.Save (fullFileName);
  }
}  /* SaveFrame */




void  SipperExtractionImageManager::ManagedClass::Flush ()
{
  CloseOutCurDirectory ();
}  /* Flush */

}  /* MLL */
