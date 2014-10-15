#if  !defined(_IMPORTGPS_DATA_)
#define  _IMPORTGPS_DATA_

#include "Application.h"
#include "DataBase.h"
#include "RunLog.h"
#include "Str.h"


namespace  FullSizeImagesInstall_DataNameSpace
{
  /**
   *@brief  Application that wil scan through all images in the MySQL Images table and verify that a FullSize image 
   * exists for those that are larger than their thumbnail version.  The data will be proecssed by SipperFile entries.
   */
  class  FullSizeImagesInstall: public Application
  {
  public:
    FullSizeImagesInstall (int     argc, 
                           char**  argv
                          );

    ~FullSizeImagesInstall ();

    void  Main ();

  private:
   class  SipperFileResults
   {
   public:
     SipperFileResults (KKStr  _sipperFileName,
                        int32  _sipperFileImageCount,
                        int32  _sipperFileThumbNail,
                        int32  _sipperFileAlreadyInFullSizeTable,
                        int32  _sipperFileMissingFromFullSize,
                        int32  _sipperFileImagesLost,
                        int32  _sipperFileImagesRecoverd
                       ):
         sipperFileName                   (_sipperFileName),
         sipperFileImageCount             (_sipperFileImageCount),
         sipperFileThumbNail              (_sipperFileThumbNail),
         sipperFileAlreadyInFullSizeTable (_sipperFileAlreadyInFullSizeTable),
         sipperFileMissingFromFullSize    (_sipperFileMissingFromFullSize),
         sipperFileImagesLost             (_sipperFileImagesLost),
         sipperFileImagesRecoverd         (_sipperFileImagesRecoverd)
     {}


     static  void  PrintHeader (ostream& o);
     void  Print (ostream&  o);

     SipperFileResults&  operator+= (const SipperFileResults& right);


     KKStr  sipperFileName;
     int32  sipperFileImageCount;
     int32  sipperFileThumbNail;              // number of images where the ThumbNail is the true extracted image.
     int32  sipperFileAlreadyInFullSizeTable;
     int32  sipperFileMissingFromFullSize;
     int32  sipperFileImagesLost;
     int32  sipperFileImagesRecoverd;
   };



    void   DisplayCommandLineParameters ();

    bool   ProcessCmdLineParameter (char    parmSwitchCode, 
                                    KKStr   parmSwitch, 
                                    KKStr   parmValue
                                   );

    void   ProcessSipperFile (SipperFilePtr  sipperFile);

    void   DiagnoseImage (DataBaseImagePtr  image, 
                          RasterSipperPtr   fullSizeImage
                         );


    bool                        cancelFlag;
    DataBasePtr                 dbConn;
    vector<SipperFileResults>   results;
    KKStr                       reportFileName;
    ostream*                    report;
    SipperFileListPtr           sipperFiles;
    int32                       sipperFilesProcessed;
  };  /* FullSizeImagesInstall */
}  /* FullSizeImagesInstall_DataNameSpace */

#endif
