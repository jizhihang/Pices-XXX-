#if  !defined(_IEM_PREDEFINEDCLASSES_)
#define  _IEM_PREDEFINEDCLASSES_

namespace  ImageExtractionManager
{
#if  !defined(_IEM_LOGICALFRAME_)
  class  LogicalFrame;
  typedef  LogicalFrame*  LogicalFramePtr;
#endif


#if  !defined(_IEM_LOGICALFRAMEQUEUE_)
  class  LogicalFrameQueue;
  typedef  LogicalFrameQueue*  LogicalFrameQueuePtr;
#endif


#if  !defined(_IEM_LOGICALFRAMEBLOB_)
  class  LogicalFrameBlob;
  typedef  LogicalFrameBlob*            LogicalFrameBlobPtr;
  typedef  vector<LogicalFrameBlobPtr>  VectorLogicalFrameBlob;
#endif


#if  !defined(_IEM_EXTRACTEDIMAGE_)
  class  ExtractedImage;
  typedef  ExtractedImage*  ExtractedImagePtr;
  typedef  vector<ExtractedImagePtr>  VectorExtractedImage;
  typedef  VectorExtractedImage*  VectorExtractedImagePtr;

  typedef  KKQueue<ExtractedImage>  ExtractedImageList;
  typedef  ExtractedImageList*  ExtractedImageListPtr;

  class  ExtractedImageQueue;
  typedef  ExtractedImageQueue*  ExtractedImageQueuePtr;
#endif


#if  !defined(_IEM_EXTRACTIONPARMS_)
  class  ExtractionParms;
  typedef  ExtractionParms*  ExtractionParmsPtr;
#endif


#if  !defined(_EXTRACTIONMANAGER_)
  class  ExtractionManager;
  typedef  ExtractionManager*  ExtractionManagerPtr;
#endif

#if  !defined(_IEM_EXTRACTIONMANAGERSTATS_)
  class  ExtractionManagerStats;
  typedef   ExtractionManagerStats*  ExtractionManagerStatsPtr;
#endif


#if  !defined(_IEM_SIPPERIMAGE_)
  class  SipperImage;
  typedef  SipperImage*            SipperImagePtr;
  typedef  vector<SipperImagePtr>  VectorSipperImage;
  typedef  VectorSipperImage*      VectorSipperImagePtr;
#endif



#if  !defined(_IEM_IMAGEEXTRACTIONTHREAD_)
  class  ImageExtractionThread;
  typedef  ImageExtractionThread*          ImageExtractionThreadPtr;
  typedef  KKQueue<ImageExtractionThread>  ImageExtractionThreadList;
  typedef  ImageExtractionThreadList*      ImageExtractionThreadListPtr;
#endif


#if  !defined(_IEM_FRAMEEXTRACTORTHREAD_)
  class  FrameExtractorThread;
  typedef  FrameExtractorThread*  FrameExtractorThreadPtr;
#endif



#if  !defined(_IEM_FRAMEPROCESSORTHREAD_)
  class  FrameProcessorThread;
  typedef  FrameProcessorThread*          FrameProcessorThreadPtr;
  typedef  KKQueue<FrameProcessorThread>  FrameProcessorThreadList;
  typedef  FrameProcessorThreadList*      FrameProcessorThreadListPtr;
#endif


#if  !defined(_IEM_DATABASEUPDATETHREAD_)
  class  DataBaseUpdateThread;
  typedef  DataBaseUpdateThread*  DataBaseUpdateThreadPtr;
#endif

}

#endif
