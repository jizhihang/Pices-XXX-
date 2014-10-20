#if  !defined(_IEM_FRAMEEXTRACTORTHREAD_)
#define  _IEM_FRAMEEXTRACTORTHREAD_

#include "InstrumentDataManager.h"
#include "SipperBuff.h"
#include "SipperFile.h"

#include "PreDefinedClasses.h"
#include "ExtractionParms.h"
#include "ImageExtractionThread.h"


namespace  ImageExtractionManager
{
  class  FrameExtractorThread:  public  ImageExtractionThread
  {
  public:
    FrameExtractorThread (ExtractionParms&                 _parms,
                          ExtractionManagerPtr             _extractionManager,
                          const KKStr&                     _threadName,
                          MsgQueuePtr                      _msgQueue,
                          LogicalFrameQueuePtr             _framePool,
                          SipperFilePtr                    _sipperFileRec,
                          SipperExtractionImageManagerPtr  _imageManager,
                          bool&                            _successful
                         );

    virtual ~FrameExtractorThread ();

    virtual  ThreadTypes   ThreadType ()  {return ttFrameExtractor;}

    uint32        FrameWidth         ()  const  {return frameWidth;}
    const KKStr&  SipperBuffFileName ()  const  {return sipperBuffFileName;}

    void     GetRunTimeStats (uint64&  _bytesRead,
                              uint32&  _framesRead
                             );

    InstrumentDataManagerPtr  DataManager ()  const  {return instrumentDataManager;}

    virtual  void  Run ();


  private:
    void    AllocateFrame ();

    void    BuildColCount (uint  colCount[]);

    void    BuildColCount ();

    void    EliminatePosibleLines (uint  colCount[]);

    void    GetNextFrame (bool&  moreFrames);

    void    ProcessFrame ();

    void    SaveFrame (const KKStr& suffix);

    void    SearchForCleanBreak ();


    LogicalFrameQueuePtr  framePool;  /**<  Extracted logical frames will be placed here. */


    //  Variables used for processing a single frame.
    uchar**     frame;
    uchar*      frameArea;
    uint64*     frameRowByteOffset;

    uint32      totPixels;

    uint32      frameNum;
    uint32      frameSipperRow;     /**< Row within the Sipper File That Frame starts at.  */
    bool        endOfFileReachedInLastCall;
   

    uint32      lastRowInFrame;     /**< Indicates the last row in frame that is being used for the current 
                                     * frame.  This value may be less than lastRowUsed.  It should be pointing
                                     * to a row that has no pixels that belong to any image. 
                                     */
                               
    uint32      lastRowUsed;        /**< Last row in frame that is being used.  If the prev had lastRowInFrame 
                                     * that was less than lastRowUsed then the extra rows will be moved to 
                                     * beginning of frame when the next frame is retreived.
                                     */
                               
    uint32      frameHeightMax;     /**< Max number of rows in a frame.  */
    uint32      frameWidth;

    uint32      nextFrameStartRow;  /**< If lastRowInFrame is less than lastRowUsed then this variable will 
                                     * point to the start of the next frame.
                                     */
  
    uint32*     colCount;

    uint32*     pixelsPerRow;       /**< Keeps a count of number of pixels in the coresponding rows in frame.  
                                     * Assists in speeding up processing of frames.
                                     */

    uint32      scanLineEnd;        /**< Last scanline in Sipper file that we will process; comes from command line. 
                                     * If = 0, will read all lines in sipper file bye setting to max-uint.
                                     */
 
    SipperBuffPtr                    sipperBuff;
    KKStr                            sipperBuffFileName;  /** Name that the SipperBuff ultimately opened. */
    KKStr                            sipperRootName;
    InstrumentDataManagerPtr         instrumentDataManager;

    SipperExtractionImageManagerPtr  imageManager;
    double                           systemTimeUsed;   /**<  When thread is done will represent total number of CPU secs used. */
    DateTime                         elaspedTime;
    DateTime                         endTime;
    DateTime                         startTime;

    int64                            bytesRead;
    uint32                           framesRead;

    SipperFilePtr  sipperFileRec;

    RunLog         log;  /**< Logging file to be used only by this thread. */

  };  /* FrameExtractorThread */

  typedef  FrameExtractorThread*  FrameExtractorThreadPtr;

}  /* ImageExtractionManager */


#define  MaxArtifactLineWidth  8


#endif

