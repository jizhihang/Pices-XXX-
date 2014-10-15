#ifndef  _SIPPEREXTRACTIONIMAGEMANAGER_
#define  _SIPPEREXTRACTIONIMAGEMANAGER_

//******************************************************************************
//*                         SipperExtractionImageManager                       *
//*                                                                            *
//* <p>Copyright: Copyright (c) 2006</p>                                       *
//* <p>author     Kurt Kramer                                                  * 
//*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
//* To be used by SipperImageExtraction to manage the disposition of extracted *
//* images.                                                                    *
//*                                                                            *
//*                                                                            *
//*                                                                            *
//*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
//*                                                                            *
//*  2006-July-08 Original development.                                        *   
//******************************************************************************

#include "Raster.h"
#include "Str.h"

#include "MLLTypes.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "SizeDistribution.h"


namespace  MLL
{
  class  SipperExtractionImageManager
  {
  public:
    class  ManagedClass;
    typedef  ManagedClass*  ManagedClassPtr;
    typedef  map<MLClassConstPtr,ManagedClassPtr>  ManagedClassList;
    typedef  ManagedClassList*  ManagedClassListPtr;


    SipperExtractionImageManager (FileDescPtr    _fileDesc,
                                  const KKStr&   _rootDir,
                                  bool           _saveFeatureData,
                                  int32          _veryLargeImageSize,
                                  bool           _imagesAreClassified,
                                  bool           _countOnly,
                                  uint32         _imagesPerDirectory,
                                  RunLog&        _log
                                 );

    ~SipperExtractionImageManager ();

    void  AddImage (const KKStr&        fileName,
                    MLClassConstPtr  predClass,
                    int32               size,
                    float               depth,
                    ImageFeaturesPtr    example,
                    RasterPtr           raster,
                    bool                colorize
                   );


    ClassStatisticListPtr  ClassStats ()  const;

    void  Flush ();

    void  ReportStatistics (ostream&  r)  const;

    void  ReportStatsByDepth (ostream&      r,
                              VectorUlong*  scanLinesPerMeterDepth
                             )  const;

    void  SaveFrame (const KKStr&    _fileName,
                           RasterPtr  _raster,
                           bool       colorize
                    );

    vector<ManagedClassPtr>  SortByClassName ()  const;


  private:
    ManagedClassPtr  ManagedClassLookUp (MLClassConstPtr  mlClass);

    ImageFeaturesListPtr  allExamples;
    bool                  countOnly;
    SizeDistribution      depthDistribution_1;     // using a SizeDistribution structure to implemet counts by depth
    SizeDistribution      depthDistribution_10;    // using a SizeDistribution structure to implemet counts by depth
    FileDescPtr           fileDesc;
    bool                  imagesAreClassified;
    uint32                imagesPerDirectory;
    RunLog&               log;
    ManagedClassPtr       largeImages;
    ManagedClassPtr       lastManagedClassUsed;
    ManagedClassList      managedClasses;
    KKStr                 rootDir;
    bool                  saveFeatureData;
    SizeDistribution      sizeDistributionLarge;
    SizeDistribution      sizeDistributionMed;
    SizeDistribution      sizeDistributionSmall;

    int32                   veryLargeImageSize;

    class  ImageEntry;
    class  ManagedClasssesSortByClassName;
    typedef  ImageEntry*      ImageEntryPtr;
    typedef  KKQueue<ImageEntry>  ImageEntryList;
    typedef  ImageEntryList*  ImageEntryListPtr;
  };  /* SipperExtractionImageManager */

  typedef  SipperExtractionImageManager* SipperExtractionImageManagerPtr;
} /* MLL */

#endif
