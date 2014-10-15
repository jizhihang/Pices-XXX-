#if  !defined(_EXTRACTIONMANAGER_)
#define  _EXTRACTIONMANAGER_

#include <fstream>
#include <iostream>

#include "MsgQueue.h"
#include "RunLog.h"
using namespace  KKU;

#include "SipperFile.h"
using namespace  SipperHardware;

#include "DataBase.h"
#include "DataBaseLogEntry.h"
#include "DataBaseImageValidatedEntry.h"
#include "DuplicateImages.h"
#include "FileDesc.h"
#include "SipperExtractionImageManager.h"
using namespace MLL;

#include "PreDefinedClasses.h"
#include "ExtractionParms.h"


namespace  ImageExtractionManager
{ 

  /**
   *@brief  Manages the extraction of images from a single Sipper File utilizing multiple threads.
   */
  class ExtractionManager
  {
  public:
    ExtractionManager (const KKStr&      _applicationName,
                       ExtractionParms&  _parms,
                       uint32            _maxNumThreads,
                       RunLog&           _log
                      );

    ~ExtractionManager ();

    const bool&  CancelFlag    ()  const  {return cancelFlag;}
    const bool&  Crashed       ()  const  {return crashed;}
    const bool&  DoneExecuting ()  const  {return doneExecuting;}
    RunLog&      Log           ()  const  {return log;}
    uint32       LogEntryId    ()  const  {return logEntryId;}

    bool  AllProcessorsTerminated (ImageExtractionThreadListPtr  threads);

    bool  AnyProcessorsCrashed ();

    void  CancelProcessing (int32 miliSecsToWait);

    void  GetRunTimeStats (ExtractionManagerStats&  stats);

    void  GetRunTimeStats (uint64&  _bytesRead,
                           uint32&  _framesRead,
                           uint32&  _framesProcessed,
                           uint32&  _scanLinesProcessed,
                           uint32&  _imagesFound,
                           uint32&  _imagesClassified,
                           uint32&  _imagesUpdated,
                           uint32&  _duplicatesDetected,
                           uint32&  _updateFailures,
                           uint32&  _framesOnQueue,
                           uint32&  _imagesOnQueue
                          );

    /** 
     *@brief Returns a snapshot of the current class count statistics collected.
     *@details  Caller will get ownewship of the instance of 'ClassStatisticList' so wil be responsable 
     *         for deleting when done.
     */
    ClassStatisticListPtr  GetClassStatistics ()  const;

    void  ManageTheExtraction (bool&  successful);

    void  TerminateProcessing (ImageExtractionThreadListPtr  threads);

  private:
    void  DisplayRunTimeStats (uint32  loopCount);

    void  GenerateReport ();

    void  Initialize (bool&  _successful);

    void  MakeSureSipperCruiseExists ();

    void  MakeSureSipperStationExists ();

    void  MakeSureSipperDeploymentExists ();

    SipperFilePtr  MakeSureSipperSipperFileExists (const KKStr&  sipperFileName);

    void  MonitorUntilDone (ImageExtractionThreadListPtr  threadsToMonitor,
                            uint32&                       loopCount,
                            bool&                         successful
                           );

    void  ReportHeader (ostream&  r);

    void  StartThreads (bool&  threadsStartedSuccessfully);

    void  StartThread (ImageExtractionThreadPtr  threadInstance);
    void  StartFrameProcessor (bool&  threadStartedSuccessfully);
    void  StartFrameExtractor (bool&  threadStartedSuccessfully);

    void  ReUpdateValidationData ();


    KKStr                               applicationName;
    DataBasePtr                         dbConn;
    bool                                cancelFlag;
    KKStr                               completionStatus;
    bool                                crashed;                /**< Indicates if any one of the threads crasehd.         */
    bool                                doneExecuting;          /**< The last thing this instance will do in 'ManageTheExtraction'is setthis falg to true. */
    FileDescPtr                         fileDesc;
    LogicalFrameQueuePtr                framePool;              /**< Frames that will be used by 'FrameExtractorThread' and 'FrameProcessorThread'. */
    uint32                              frameWidth;
    ExtractedImageQueuePtr              imagesAwaitingUpdate;   /**< These are images that are queued to tb inserted into database.                 */
    SipperExtractionImageManagerPtr     imageManager;
    DataBaseLogEntryPtr                 logEntry;
    uint32                              logEntryId;
    uint32                              maxNumThreads;
    MsgQueuePtr                         msgQueue;               /**< Messages from all the different threads will be sent through this instance.    */
    ExtractionParms&                    parms;
    std::ofstream                       reportFile;
    SipperFilePtr                       sipperFileRec;
    KKStr                               sipperRootName;
    bool                                terminateFlag;
    DataBaseImageValidatedEntryListPtr  validationInfo;         /**< Holds Images Validation info to be reapplied to images after extraction.       */
    KKStr                               validationInfoFileName;
    KKStr                               validationInfoFileNameHistory;
    uint32                              veryLargeImageSize;     /**< Threshhold for putting Images into Very Large Directory.                       */

    // Variables used for Report
    DateTime                            endTime;
    double                              endCPUsecs;
    DateTime                            startTime;
    double                              startCPUsecs;


    //  Variables used to manage underlying threads.

    FrameExtractorThreadPtr             frameExtractorThread;

    ImageExtractionThreadListPtr        frameProcessors;   /**< List of threads that are specificacly instances of 'FrameProcessorThread'.     */

    DataBaseUpdateThreadPtr             dataBaseUpdaterThread;

    ImageExtractionThreadListPtr        allThreads;        /**<  List of all threads created under this instance of 'ExtractionManager'.  It
                                                            * will own them.
                                                            */

    RunLog&                       log;

  };  /* ExtractionManager */

  typedef  ExtractionManager*  ExtractionManagerPtr;


}  /* ImageExtractionManager */


#endif

