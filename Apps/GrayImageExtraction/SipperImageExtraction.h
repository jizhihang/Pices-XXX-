#ifndef  _SipperImageExtraction_
#define  _SipperImageExtraction_

//**************************************************************************
//*                          SipperImageExtraction                         *
//*                                                                        *
//*                                                                        *
//*                                                                        *
//* <p>Copyright: Copyright (c) 2003</p>                                   *
//* <p>author     Kurt Kramer                                              * 
//*                                                                        *
//*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
//*                                                                        *
//*  January 2003   Initial Development as GrayImageExtraction as a stand- *   
//*  Kurt Kramer    alone application.                                     *
//*                                                                        *
//*  July 2003      RE-organized such that the actual image Extraction is  *
//*  Kurt Kramer    an independent object thatcan be used by other apps.   *
//*                                                                        *
//*                                                                        *
//*  2005-Feb-22    Added duplicate image detection.  Because sometimes    *
//*  Kurt Kramer    SIPPER will write its last buffer in a file twice theer*
//*                 can be duplicate images extracted.  We detect these    *
//*                 through there feature data.  Duplicactes will be       *
//*                 placed in a seperate directory called                  *
//*                 "c:\Temp\DuplicateImages\<SipperFileName>"             *
//*                                                                        *
//*  2005-08-12     Adding support for SIPPER 3 Instrument data.  Did this *
//*                 with the creation of two new Objects called            *
//*                 'InstrumentDataManager' & 'InstrumentDataReport'.  We  *
//*                 will create an instance of 'InstrumentDataManager' and *
//*                 pass it to teh SipperBuff object created.  That Sipper-*
//*                 Buff object will use it to report and Instrument data  *
//*                 that is read from the SIPPER file.                     *
//*                                                                        *
//**************************************************************************


/**
 * <p>Title: Image Management Syetem</p>
 * <p>Description: Application for the Classification and Management of Plankton Images</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: USF</p>
 * @author Kurt Kramer
 * @version 1.0
 */


#include  "BasicTypes.h"
#include  "RunLog.h"
using namespace KKU;


#include  "SipperBuff.h"
#include  "InstrumentDataManager.h"
using namespace SipperHardware;

#include  "Classifier2.h"
#include  "DataBase.h"
#include  "DuplicateImages.h"
#include  "FileDesc.h"
#include  "MLClass.h"
#include  "ImageFeatures.h"
#include  "SipperBlob.h"
#include  "SipperExtractionImageManager.h"
#include  "TrainingProcess2.h"
using namespace MLL;

#include  "SipperImage.h"
#include  "SipperImageExtractionParms.h"



#define  VERSION           "4.01"
#define  MaxArtifactLineWidth  8



/**
 @class  SipperImageExtraction
 @brief  Sipper Image Extraction module.  Shared by both GrayImageExtraction and ImageExtractionWindows.
 @details Module was originally developed back in 2003 with the advent of SIPPER 2.   It had one 
          through many enhanacements since then.
 @author  Kurt Kramer
 */


class  SipperImageExtraction
{
public:
  SipperImageExtraction (SipperImageExtractionParms&  parms,
                         bool&                        _successful,
                         RunLog&                      _log
                        );
  


  ~SipperImageExtraction ();


  void  ExtractFrames ();


private:
  void    AllocateFrame ();

  void    BuildColCount (uint  colCount[]);

  void    BuildColCount ();

  VectorSipperImages*  BuildListOfSipperImages (int&  imagesInFrame);

  void    CreateAllSipperFileRecs (DataBasePtr  dbConn,
                                   RunLog&      log
                                  );

  void    EliminatePosibleLines (uint  colCount[]);

  int     GetMaxBlobIdInUpperLeft (int row,
                                   int col
                                  );

  int     GetMaxBlobIdInUpperRight (int row,
                                    int col
                                   );

  void    InitailzieBlobIds ();

  void    LocateBlobsUsingConnectedDistanceOf3 ();

  void    LocateBlobsUsingConnectedDistanceNotOf3 ();

  KKStr   SipperFileFormatStr ();

  void    GetNextFrame (bool&  moreFrames);

  void    MergeBlobIds (uint idOfBlobToMergeTo,
                        uint idOfBlobToMergeFrom
                       );

  SipperBlobPtr NewBlob (uint  rowTop,
                         uint  colLeft);


  void    PerformClosing ();

  void    PerformDialation ();

  void    PerformErosion ();

  void    PerformMorphOperations ();

  void    PerformOpening ();

  void    ProcessFrame ();

  void    ReFreshDataBaseImage (const KKStr&      imageFileName, 
                                const KKStr&      sipperFileName, 
                                RasterSipperPtr   raster, 
                                ImageFeaturesPtr  featureVector,
                                uint64            byteOffset,
                                uint              sipperTopRow,
                                uint              sipperTopCol
                               );

  void    SaveFrame (const KKStr& suffix);

  void    SearchForCleanBreak ();



  uint        connectedPixelDist;

  SipperImageExtractionParms&  parms;

  FileDescPtr fileDesc;

  uint        veryLargeImageSize;   // Threshhold for putting Images into Very Large Directory.

  uint        imagesWrittenInFrame;

  //  Variables used for processing a single frame.
  uchar**     frame;
  uchar*      frameArea;
  uint64*     frameRowByteOffset;

  uchar**     origFrame;       // 'origFrame' and 'origFrameArea' are only used when
  uchar*      origFrameArea;   // morphalogical operations are performed.

  uchar**     workFrame;       // 'workFrame' & 'workFrameArea' are used to perform 
  uchar*      workFrameArea;   // morphalogical operations with.

  uint        totPixels;

  uint        frameNum;
  uint        frameSipperRow;  // Row within the Sipper File That Frame starts at.
  bool        endOfFileReachedInLastCall;

  
  SipperBlobPtr*  blobs;
  uint            numOfBlobsInFrame;
  uint            maxBlobsPerFrame;


  int**       blobIds;         // Two dimensional array that will work with frame.
                               // for each pixel that is determined to belong to 
                               // an image, the ID of that image that it is part of
                               // will be kept here.

  int*        blobIdsArea;
   

  TrainingProcess2Ptr  trainer;
  Classifier2Ptr       classifier;

  uint        lastRowInFrame;  // Indicates the last row in frame that is being
                               // used for the current frame.  This value may
                               // be less than lastRowUsed.  It should be pointing
                               // to a row that has no pixels that belong to any 
                               // image. 

  uint        lastRowUsed;     // Last row in frame that is being used.  If the prev
                               // had lastRowInFrame that was less than lastRowUsed
                               // then the extra rows will be moved to beginning of
                               // frame when the next frame is retreived.

  uint        maxFrameHeight;  // Max number of rows in a frame.

  uint        nextFrameStartRow;  // If lastRowInFrame is less than lastRowUsed 
                                  // then this variable will point to the start of
                                  // the next frame.


  uint        numImagesWritten;

  uint*       pixsInRows;         // Keeps a count of number of pixels in the core-
                                  // sponding rows in frame.  Assists in speeding 
                                  // up processing of frames.
 
  std::ofstream             reportFile;
  SipperBuffPtr             sipperBuff;
  KKStr                     sipperRootName;
  InstrumentDataManagerPtr  instrumentDataManager;

  MLClassPtr             unKnownMLClass;
  
  DuplicateImagesPtr        dupImageDetector;
  KKStr                     duplicateImageDir;
  ImageFeaturesListPtr      duplicateImages;   // We keep track of these images, otherwise they
                                               // will not get deleted. and hence cause memory
                                               // leaks.

  SipperExtractionImageManagerPtr  imageManager;

  RunLog&     log;

  uint32         scanLineEnd;     /**< Will cme from command line or database if not specified in command line. */
  uint32         scanLineStart;   /**< Will cme from command line or database if not specified in command line. */

  DataBasePtr    dbConn;
  SipperFilePtr  sipperFileRec;
};



#endif
