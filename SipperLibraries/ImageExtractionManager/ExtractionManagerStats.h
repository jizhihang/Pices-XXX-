#if  !defined(_IEM_EXTRACTIONMANAGERSTATS_)
#define  _IEM_EXTRACTIONMANAGERSTATS_


namespace  ImageExtractionManager
{ 
  class  ExtractionManagerStats
  {
  public:
    friend class ExtractionManager;
    ExtractionManagerStats ();
    ExtractionManagerStats (const ExtractionManagerStats&  stats);
    ~ExtractionManagerStats ();

    uint64  BytesRead          () const  {return bytesRead;}
    uint32  FramesRead         () const  {return framesRead;}
    uint32  FramesProcessed    () const  {return framesProcessed;}
    uint32  ScanLinesProcessed () const  {return scanLinesProcessed;}
    uint32  ImagesFound        () const  {return imagesFound;}
    uint32  ImagesClassified   () const  {return imagesClassified;}
    uint32  ImagesUpdated      () const  {return imagesUpdated;}
    uint32  DuplicatesDetected () const  {return duplicatesDetected;}
    uint32  UpdateFailures     () const  {return updateFailures;}
    uint32  FramesOnQueue      () const  {return framesOnQueue;}
    uint32  ImagesOnQueue      () const  {return imagesOnQueue;}

  private:
    uint64  bytesRead;
    uint32  framesRead;
    uint32  framesProcessed;
    uint32  scanLinesProcessed;
    uint32  imagesFound;
    uint32  imagesClassified;
    uint32  imagesUpdated;
    uint32  duplicatesDetected;
    uint32  updateFailures;
    uint32  framesOnQueue;
    uint32  imagesOnQueue;
  };  /* ExtractionManagerStats */
}

#endif
