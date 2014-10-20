#if  !defined(_IEM_DATABASEUPDATETHREAD_)
#define  _IEM_DATABASEUPDATETHREAD_

#include "BasicTypes.h"
#include "RunLog.h"
using namespace KKU;

#include "DataBase.h"
#include "DuplicateImages.h"
using namespace  MLL;

#include "PreDefinedClasses.h"
#include "ExtractionParms.h"
#include "ImageExtractionThread.h"
using namespace  ImageExtractionManager;


namespace  ImageExtractionManager
{
  class  DataBaseUpdateThread:  public  ImageExtractionThread
  {
  public:
    DataBaseUpdateThread (ExtractionParms&                 _parms,
                          ExtractionManagerPtr             _extractionManager,
                          const KKStr&                     _threadName,
                          MsgQueuePtr                      _msgQueue,
                          ExtractedImageQueuePtr           _imagesAwaitingUpdate,
                          SipperExtractionImageManagerPtr  _imageManager,
                          FileDescPtr                      _fileDesc
                         );

    virtual ~DataBaseUpdateThread ();

    virtual  ThreadTypes   ThreadType ()  {return ttDataBAseUpdater;}

    void  GetRunTimeStats (uint32&  _imagesUpdated,
                           uint32&  _duplicatesDetected,
                           uint32&  _updateFailures
                          );
    
    virtual  void  Run ();

    uint32  DbUpdateFailures ()  const  {return updateFailures;}

  private:
    ExtractedImagePtr  GetNextImageToUpdate ();

    void  ReFreshDataBaseImage (const KKStr&      imageFileName, 
                                const KKStr&      sipperFileName, 
                                RasterSipperPtr   raster, 
                                ImageFeaturesPtr  featureVector,
                                uint64            byteOffset,
                                uint32            sipperTopRow,
                                uint32            sipperTopCol
                               );

    DataBasePtr                      dbConn;
    DuplicateImagesPtr               dupImageDetector;
    KKStr                            duplicateImageDir;
    ImageFeaturesListPtr             duplicateImages;        /**< We keep track of these images, otherwise they will not get deleted 
                                                              * and hence cause memory leaks.
                                                              */

    FileDescPtr                      fileDesc;

    SipperExtractionImageManagerPtr  imageManager;
    ExtractedImageQueuePtr           imagesAwaitingUpdate;
    KKStr                            sipperRootName;

    uint32                           duplicatesDetected;
    uint32                           imagesUpdated;
    uint32                           updateFailures;  /**< The number of updates to the database that failed. */

    RunLog                           log;

  };  /* DataBaseUpdateThread */

  typedef  DataBaseUpdateThread*  DataBaseUpdateThreadPtr;

}  /* ImageExtractionManager */

#endif


