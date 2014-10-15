#ifndef  _GRAYIMAGEEXTRACTION_
#define  _GRAYIMAGEEXTRACTION_

#include  "SipperBuff.h"


typedef  unsigned char uchar;
typedef  unsigned int  uint;

#define  VERSION       "1.66"
#define  MAXFLOWRATES  20
#define  SIZEBUCKETS   40
#define  BUCKETSIZE    50
#define  AllowBlanks    3



class  GrayImageExtraction
{
public:
  GrayImageExtraction (FileFormat  _fileFormat);

  GrayImageExtraction (KKStr     _sipperFileName,
                       FileFormat _fileFormat,
                       uchar      _cameraNum,
                       KKStr     _outputRootDir,
                       ostream&   _reportFile,
                       uint       _minImageSize,
                       uint       _startScanLine,
                       bool       _saveFrames);

  ~GrayImageExtraction ();


  void  ExtractFrames ();


private:
  void  AllocateFrame ();

  void  CreateSubdirectory ();

  void  EliminatePosibleLines (uint  colCount[]);

  KKStr  FileFormatStr ();

  double  GrayImageExtraction::FlowRate ();

  void  GetNextFrame (bool&  moreFrames);

  void  ProcessFrame ();

  void  SaveFrame ();

  void  SearchForCleanBreak ();



  FileFormat  fileFormat;
  uchar       cameraNum;
  bool        saveFrames;

  uint        startScanLine;
  KKStr       outputRootDir;
  KKStr       outputSubdir;
  KKStr       subDirectoryName;
  uint        numOfDirectories;
  uint        imagesWrittenInSubdirectory;
  uint        imagesInFrame;
  uint        imagesWrittenInFrame;

  uint       totalImages;
  uint       totalImagesWritten;
  uint       totalPixels;
  uint       totalPixelsWritten;
  uint       totalFlowCount;
  uint       totalLines;

  uint       sizeCount[SIZEBUCKETS];


  //  Variables used for processing a single frame.
  uchar**    frame;
  uint       frameNum;
  uint       frameSipperRow;  // Row with In Sipper File That Frame starts at.

  int**      imageIds;        // Two dimensional array that will work with frame.
                              // for each pixel that is determined to belong to
                              // an image, the ID of that image that it is part of
                              // will be kept here.

  uint       lastRowInFrame;  // Indicates the last row in frame that is being
                              // used for the current frame.  This value may
                              // be less than lastRowUsed.  It should be pointing
                              // to a row that has no pixels that belong to any
                              // image.

  uint       lastRowUsed;     // Last row in frame that is being used.  If the prev
                              // had lastRowInFrame that was less than lastRowUsed
                              // then the extra rows will be moved to beginning of
                              // frame when the next frame is retreived.

  uint       maxFrameHeight;  // Max number of rows in a frame.

  uint       nextFrameStartRow;  // If lastRowInFrame is less than lastRowUsed
                              // then this variable will point to the start of
                              // the next frame.


  uint       numImagesWritten;

  uint*      pixsInRows;      // Keeps a count of number of pixels in the core-
                              // sponding rows in frame.  Assists in speeding
                              // up processing of frames.

  uint          minImageSize;
  ostream&      reportFile;
  SipperBuffPtr sipperBuff;
  KKStr         sipperRootName;


  bool       lastFlow;
  uint       flowRates[MAXFLOWRATES];
  uint       linesSinceLastFlowRate;
  int        nextFlowRate;
};



#endif
