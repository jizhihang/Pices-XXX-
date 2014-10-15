#if  !defined(_IEM_FRAMEPROCESSORTHREAD_)
#define  _IEM_FRAMEPROCESSORTHREAD_

#include "BasicTypes.h"
#include "RunLog.h"
using namespace KKU;


#include "InstrumentDataManager.h"
using namespace SipperHardware;


#include "Classifier2.h"
#include "DataBase.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "SipperBlob.h"
#include "SipperExtractionImageManager.h"
#include "TrainingProcess2.h"
using namespace MLL;


#include "PreDefinedClasses.h"
#include "ExtractionParms.h"
#include "ImageExtractionThread.h"


namespace  ImageExtractionManager
{
  class  FrameProcessorThread:  public  ImageExtractionThread
  {
  public:
    FrameProcessorThread (ExtractionParms&        _parms,
                          ExtractionManagerPtr    _extractionManager,
                          const KKStr&            _threadName,
                          MsgQueuePtr             _msgQueue,
                          LogicalFrameQueuePtr    _framePool,
                          ExtractedImageQueuePtr  _imagesAwaitingUpdate,
                          bool&                   _successful
                         );

    virtual  ~FrameProcessorThread ();

    virtual  ThreadTypes   ThreadType ()  {return ttFrameProcessor;}

    void  GetRunTimeStats (uint32&  _framesProcessed,
                           uint32&  _scanLinesProcessed,
                           uint32&  _imagesFound,
                           uint32&  _imagesClassified
                          );

    virtual  void  Run ();

  private:
    void              LoadClassifier (bool&  _successful);
    void              ProcessFrame (LogicalFramePtr  frame);
    KKStr             SipperFileFormatStr ();
    LogicalFramePtr   GetNextFrameToProcess ();

    Classifier2Ptr            classifier;
    FileDescPtr               fileDesc;
    LogicalFrameQueuePtr      framePool;
    ExtractedImageQueuePtr    imagesAwaitingUpdate;
    KKStr                     sipperRootName;
    TrainingProcess2Ptr       trainer;
    MLClassPtr             unKnownMLClass;

    uint32                    framesProcessed;
    uint32                    scanLinesProcessed;
    uint32                    imagesFound;
    uint32                    imagesClassified;

    RunLog                    log;
  };  /* FrameProcessorThread */

  typedef  FrameProcessorThread*  FrameProcessorThreadPtr;

  typedef  KKQueue<FrameProcessorThread>  FrameProcessorThreadList;
  typedef  FrameProcessorThreadList*      FrameProcessorThreadListPtr;

}  /* ImageExtractionManager */



#endif

