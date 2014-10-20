#if  !defined(_IEM_EXTRACTIONPARMS_)
#define  _IEM_EXTRACTIONPARMS_

#include  "BasicTypes.h"
#include  "CmdLineExpander.h"
#include  "RunLog.h"
#include  "Str.h"
using namespace  KKU;

#include  "SipperBuff.h"
using namespace  SipperHardware;


#include  "ClassStatistic.h"
#include  "DataBaseServer.h"
#include  "MLClass.h"
#include  "SipperExtractionImageManager.h"
using namespace MLL;


#include "PreDefinedClasses.h"
using namespace ImageExtractionManager;


#define   DefaultConnectedPixelDist  3


namespace  ImageExtractionManager
{ 
  class  ExtractionParms
  {
  public:
    typedef  SipperHardware::SipperFileFormat  SipperFileFormat;

    ExtractionParms (int32    argc,
                     char**   argv,
                     bool     _runningAsAWindowsApp,
                     KKStr&   errorMessage,
                     bool&    formatGood,
                     RunLog&  log
                    );

    ExtractionParms (KKStr    progName,
                     KKStr    cmdLine,
                     bool     _runningAsAWindowsApp,
                     KKStr&   errorMessage,
                     bool&    formatGood,
                     RunLog&  log
                    );

    ~ExtractionParms ();


    KKStr    CmdLine ()  const;

    void     DisplayExampleCmdLine ();

    void     DoneExecuting   (bool  _doneExecuting)   {doneExecuting = _doneExecuting;}

    bool     RunningAsAWindowsApp ()  {return  runningAsAWindowsApp;}

    void     ParseCmdLine (const CmdLineExpander&  cmdLineExpander,
                           KKStr&                  errorMessage,
                           bool&                   formatGood,
                           RunLog&                 log
                          );

    // Parameter Values
    uchar              CameraNum                () const  {return  cameraNum;}
    bool               Colorize                 () const  {return  colorize;}
    const KKStr&       ConfigFileName           () const  {return  configFileName;}
    const KKStr&       ConfigFileNameSpecified  () const  {return  configFileNameSpecified;}
    uint32             ConnectedPixelDist       () const  {return  connectedPixelDist;}
    bool               CountOnly                () const  {return  countOnly;}
    DataBaseServerPtr  DataBaseServer           () const  {return  dataBaseServer;}
    KKStr              DataBaseServerDesc       () const;
    bool               DoneExecuting            () const  {return  doneExecuting;}
    bool               ExtractFeatureData       () const  {return  extractFeatureData;}
    SipperFileFormat   FileFormat               () const  {return  fileFormat;}
    const KKStr&       FlowRateFileName         () const  {return  flowRateFileName;}
    bool               FramesOnly               () const  {return  framesOnly;}
    uint32             ImagesPerDirectory       () const  {return  imagesPerDirectory;}
    uint32             MinImageSize             () const  {return  minImageSize;}
    uint32             MaxImageSize             () const  {return  maxImageSize;}
    const KKStr&       MorphOperations          () const  {return  morphOperations;}
    bool               MultiThreaded            () const  {return  multiThreaded;}
    const KKStr&       OutputRootDir            () const  {return  outputRootDir;}
    bool               PreProcess               () const  {return  preProcess;}
    const KKStr&       ReportFileName           () const  {return  reportFileName;}
    bool               RefreshDataBaseImages    () const  {return  refreshDataBaseImages;}
    bool               SaveFrames               () const  {return  saveFrames;}
    bool               SaveFramesAfter          () const  {return  saveFramesAfter;}
    uint32             ScanLineEnd              () const  {return  scanLineEnd;}
    uint32             ScanLineStart            () const  {return  scanLineStart;}
    const KKStr&       SipperFileName           () const  {return  sipperFileName;}
    bool               UpdateDataBase           () const  {return  updateDataBase;}


    // Runtime Stats
    ClassStatisticListPtr  ClassStats      () const  {return  classStats;     }

    uint64            BytesExtracted       () const  {return  bytesExtracted; }
    uint32            LinesExtracted       () const  {return  linesExtracted; }
    uint32            ImagesExtracted      () const  {return  imagesExtracted;}
    uint32            ImagesWritten        () const  {return  imagesWritten;  }
    uint32            PixelsRead           () const  {return  pixelsRead;     }
    uint32            PixelsWritten        () const  {return  pixelsWritten;  }
    uint64            SipperFileSize       () const  {return  sipperFileSize; }
    const KKStr&      StatusMessage        () const  {return  statusMessage;}
    KKStr&            StatusMessageField   ()        {return  statusMessage;}



    //  Update RunTime Stats
    void             ClassStats      (ClassStatisticListPtr  _classStats);  // Will take ownership of '_classStats'
    uint32           IncrementImagesExtracted () {return  (++imagesExtracted);}
    uint32           IncrementImagesWritten   () {return  (++imagesWritten);}
    uint32           IncrementLinesExtracted  () {return  (++linesExtracted);}

    void             ImagesPerDirectory       (uint32 _imagesPerDirectory)     {imagesPerDirectory = _imagesPerDirectory;}
    void             IncrementBytesExtracted  (uint32 _bytesExtracted)         {bytesExtracted  += _bytesExtracted;}
    void             IncrementPixelsRead      (uint32 _pixelsRead)             {pixelsRead      += _pixelsRead;}
    void             IncrementPixelsWritten   (uint32 _pixelsWritten)          {pixelsWritten   += _pixelsWritten;}
    void             BytesExtracted           (uint64 _bytesExtracted)         {bytesExtracted  = _bytesExtracted;}
    void             LinesExtracted           (uint32 _linesExtracted)         {linesExtracted  = _linesExtracted;}
    void             ImagesExtracted          (uint32 _imagesExtracted)        {imagesExtracted = _imagesExtracted;}
    void             ImagesWritten            (uint32 _imagesWritten)          {imagesWritten   = _imagesWritten;}
    void             PixelsRead               (uint32 _pixelsRead)             {pixelsRead      = _pixelsRead;}
    void             PixelsWritten            (uint32 _pixelsWritten)          {pixelsWritten   = _pixelsWritten;}
    void             StatusMessage            (const KKStr& _statusMessage)    {statusMessage   = _statusMessage;}


  private:
    void  SetupOtherFileNames ();

    uchar              cameraNum;
    bool               colorize;
    KKStr              configFileName;
    KKStr              configFileNameSpecified;
    uint32             connectedPixelDist;
    bool               countOnly;
    DataBaseServerPtr  dataBaseServer;
    bool               doneExecuting;
    bool               extractFeatureData;
    SipperFileFormat   fileFormat;
    KKStr              flowRateFileName;
    bool               framesOnly;
    uint32             imagesPerDirectory;
    uint32             maxImageSize;
    uint32             minImageSize;
    KKStr              morphOperations;
    bool               multiThreaded;
    KKStr              outputRootDir;
    bool               preProcess;
    bool               refreshDataBaseImages;
    KKStr              reportFileName;
    bool               runningAsAWindowsApp;
    bool               saveFrames;
    bool               saveFramesAfter;
    KKStr              sipperFileName;
    uint32             scanLineEnd;
    uint32             scanLineStart;
    KKStr              statusMessage;
    bool               updateDataBase;


    // Runtime Stats
    uint64                 bytesExtracted;
    ClassStatisticListPtr  classStats;
    ClassStatisticListPtr  classStatsPrev;   // Used to help prevent issues with thread conflicts.
    uint32                 imagesExtracted;
    uint32                 imagesWritten;
    uint32                 linesExtracted;
    uint32                 pixelsRead;
    uint32                 pixelsWritten;
    uint64                 sipperFileSize;
  };  /* ExtractionParms*/
 
  typedef  ExtractionParms*  ExtractionParmsPtr;

}  /* ImageExtractionManager */

#endif
