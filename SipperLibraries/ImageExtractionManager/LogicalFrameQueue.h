#if  !defined(_IEM_LOGICALFRAMEQUEUE_)
#define  _IEM_LOGICALFRAMEQUEUE_

#include  <queue>
#include  "GoalKeeper.h"
#include  "ExtractionParms.h"


#if  !defined(_IEM_LOGICALFRAME_)
namespace  ImageExtractionManager
{
  class  LogicalFrame;
  typedef  LogicalFrame*  LogicalFramePtr;
}
#endif



namespace  ImageExtractionManager
{
  class  LogicalFrameQueue
  {
  public:
    LogicalFrameQueue (const ExtractionParms&  _parms,
                       uint32                  _maxSize,
                       uint32                  _frameHeightMax,
                       uint32                  _frameWidth,
                       const bool&             _cancelFlag
                      );

    ~LogicalFrameQueue ();

    int32  MemoryConsumedEstimated ()  const;

    void  AddBackToAvailableFrames (LogicalFramePtr   availableFrame);

    void  AddBackToFramesAwaitingProcessing (LogicalFramePtr   frameAwaitingProcessing);

    /**
     *@brief 'ImageProcessorThread; calls this methd when it is done processing a frame.
     *@details  This lets the queue know that the frame is avaialble for use and can be 
     * used again by the 'FrameExtractorThread'.
     */
    void  FrameProcessed (LogicalFramePtr   processedFrame);

    /**
     *@brief  Returns the a Frame that is not in use;  if none are avaialble will return NULL.
     */
    LogicalFramePtr  GetNextAvailaleFrame ();

    /**
     *@brief Returns the next logical frame that needs to be proecssed. 
     *@details  If there are no frames that are readu to be proecssed will return NULL.
     */
    LogicalFramePtr  GetNextFrameToProcess ();

    /**
     *@brief  The 'FrameExtractorThread' calls this method to add a frame that needs to be processed.
     */
    void  QueueFrameToProcess (LogicalFramePtr   processedFrame);


    /** @brief Returns the number frames awaiting processing. */
    uint32  FramesOnQueue ();


  private:
    void  RemoveFromFramesBeingProcessed (LogicalFramePtr  frame);


    const ExtractionParms&  parms;
    uint32                  frameHeightMax;
    uint32                  frameWidth;
    uint32                  maxSize;
    uint32                  totalFramesAllocated;

    std::vector<LogicalFramePtr>   allFrames;                  /**<  Owns all 'LogicalFrame' instances.                                     */
    std::queue<LogicalFramePtr>    availableFrames;            /**<  Frames not in use.                                                     */
    std::queue<LogicalFramePtr>    framesAwaitingProcessing;   /**<  Frames waiting to be proecssed.                                        */
    std::vector<LogicalFramePtr>   framesBeingProcessed;       /**<  Frames that are currently being processed by a 'ImageProcessingThread. */

    GoalKeeperPtr   goalie;

    const bool&     cancelFlag;
  };  /* LogicalFrameQueue */




}  /* ImageExtractionManager */


#endif
