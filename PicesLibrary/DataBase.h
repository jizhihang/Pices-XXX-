#if  !defined(_DATABASE_)
#define  _DATABASE_



#include "DateTime.h"
#include "GoalKeeper.h"
#include "RasterSipper.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;


#include "GPSDataPoint.h"
#include "InstrumentData.h"
#include "InstrumentDataList.h"
#include "InstrumentDataMeans.h"
#include "SipperCruise.h"
#include "SipperDeployment.h"
#include "SipperFile.h"
#include "SipperStation.h"
#include "VolumeSampledStat.h"
using namespace SipperHardware;


#include "ClassStatistic.h"
#include "DataBaseGpsData.h"
#include "DataBaseImage.h"
#include "DataBaseImageGroup.h"
#include "DataBaseImageGroupEntry.h"
#include "DataBaseImageValidatedEntry.h"
#include "DataBaseLogEntry.h"
#include "FileDesc.h"
#include "MLClass.h"


#ifndef  _mysql_h
struct  MYSQL;
struct  MYSQL_RES;
struct  MSQL_STRUCT; 
typedef  MSQL_STRUCT* MYSQL_ROW;
struct  MYSQL_FIELD;
#endif

namespace MLL 
{

  #ifndef  _IMAGEFEATURES_
  class  ImageFeatures;
  typedef  ImageFeatures*  ImageFeaturesPtr;

  class  ImageFeaturesList;
  typedef  ImageFeaturesList*  ImageFeaturesListPtr;
  #endif


  #if  !defined(_DATABASESERVER_)
  class  DataBaseServer;
  typedef  DataBaseServer*  DataBaseServerPtr;

  class  DataBaseServerList;
  typedef  DataBaseServerList*  DataBaseServerListPtr;
  #endif


  /**
   @class  DataBase
   @brief DataBase connection management object.  All database communications will go through this class.
   @author Kurt Kramer
   @date Dec/01/2008
   */
  class  DataBase
  {
  public:
    typedef  DataBase*  DataBasePtr;

    typedef  KKU::uchar   uchar;
    typedef  KKU::ulong   ulong;
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;
    typedef  KKU::int64   int64;
    typedef  KKU::uint64  uint64;

    static  void  Initialization ();
    static  void  FinalCleanUp ();

    static  bool  EmbeddedServerRunning ();  /**< Indicates that the Embeded server has been initialize and cosumming resources */

    static  DataBaseServerListPtr  GetListOfServers (RunLog&  _log);  /**< Returns a list of previosly defined MySQL servers that we can connect to. */


   
    static  DataBaseServerPtr  GetDefaultServer (RunLog&  _log);   /**< Returns an instance of the current server;  caller will own it. */


    /** @brief  returns an instance of the server with 'description';  caller will own it. */
    static  DataBaseServerPtr  GetServer (const KKStr&  description,
                                          RunLog&       _log
                                         );

    static  void   ErrorLogMsg (const KKStr&  msg);


    DataBase (RunLog&  _log);

    DataBase (const DataBaseServerPtr  _server,   /**< Will make own local copy of "_server" */
              RunLog&                  _log
             );

    ~DataBase ();

    void  Close ();

    KKStr  ServerDescription ()  const;   /**< return description of server that this instance is connected to.  */

    KKStr  LastErrorDesc ()  const;

    bool                     DuplicateKey () const {return duplicateKey;}
    const DataBaseServerPtr  Server       () const {return server;}
    bool                     Valid        () const {return valid;}



    int32  QueryStatement (const char* queryStr,
                           int32       queryStrLen
                          );

    int32  QueryStatement (const KKStr&  statement);



    ///  Will make MySQL call contained in 'statement'.  and return back results as a Vector List of KKStr objects.
    ///  colsToReturn  - A list of columns to return from the result set.
    KKStrMatrixPtr  QueryStatementKKStr (const KKStr&        statement,
                                         ConstCharStarArray  colsToReturn
                                        );

    /// Wil make SQL Query call; if there is a disconnect from the database;  it will not try to
    /// recconnect like the other 'QueryStatement2' calls.
    int32  QueryStatement2 (const char*  queryStr,
                            int32        queryStrLen
                           );

    KKStrMatrixPtr  QueryStatementReturnAllColumns (const char*   statement,
                                                    int32         statementLen,
                                                    VectorKKStr&  columnsReturned
                                                   );

    static bool  ThreadInit ();  /**< Call at start of new thread.        */

    static void  ThreadEnd ();   /**< Call just before thread terminates. */



    //***********************************************************************************
    void  FeatureDataCreateTable ();

    void  FeatureDataInsertRow (const KKStr&          _sipperFileName,
                                const ImageFeatures&  example
                               );
    

    /// Retrieves FeatureData for a given 'DataBaseImage' object.  It will also refresh the Instrument data fields
    /// of the 'ImageFeatures' objects from the 'InstruentData' table
    ImageFeaturesPtr      FeatureDataRecLoad (DataBaseImagePtr  image);


    /// Retrieves FeatureData for a given ImageFileName It will also refresh the Instrument data fields
    /// of the 'ImageFeatures' objects from the 'InstruentData' table
    ImageFeaturesPtr      FeatureDataRecLoad (const KKStr&  imageFileName);



    /**
     *@brief Returns a List of FeatureVector's for specified SipperFile and if specified for Specified ImaegClass.
     *@details The retrieval is driven by the Images table and a Left-Join is donewith the FeatureData table; so if 
     * there is no coresponding FeatureData table enry for a image the FeaterData will be filled with 0's.
     *@param[in] classKeyToUse  If equal to 'V' will only return Validated examples.
     *@param[in] reExtractInstrumentData If set to true will Re-Extract Instrument data from the SipperFile.
     */
    ImageFeaturesListPtr  FeatureDataGetOneSipperFile (const KKStr&        sipperFileRootName,
                                                       MLClassConstPtr  mlClass,
                                                       char                classKeyToUse,
                                                       bool                reExtractInstrumentData,
                                                       bool&               cancelFlag
                                                      );

    ImageFeaturesListPtr  FeatureDataForImageGroup (const DataBaseImageGroupPtr  imageGroup,
                                                    MLClassConstPtr           mlClass,
                                                    char                         classKeyToUse,
                                                    const bool&                  cancelFlag
                                                   );

    void   FeatureDataUpdate (DataBaseImagePtr  dataBaseImage,
                              ImageFeaturesPtr  example
                             );

    void   FeatureDataUpdateInstrumentDataFields (const KKStr&       imageFileName,
                                                  InstrumentDataPtr  instumentData
                                                 );

    std::vector<ulong>*  FeatureDataGetScanLinesPerMeterProfile (const KKStr&  sipperFileName);





    //***********************************************************************************
    void    GpsDataInsert (const DataBaseGpsData&  gpsData);

    DataBaseGpsDataListPtr   GpsDataQuery (const KKStr&          cruiseName,
                                           const KKU::DateTime&  utcDateTimeStart,
                                           const KKU::DateTime&  utcDateTimeEnd
                                          );

    void  GpsDataDelete (const KKStr&          cruiseName,
                         const KKU::DateTime&  utcDateTimeStart,
                         const KKU::DateTime&  utcDateTimeEnd
                        );




    //***********************************************************************************



    ///  Creates and entry in the Images table for 'image',
    ///  if either dimension of 'image' exceeds 100 it will save a reduced version of it in the thumbNail field of 'Images' 
    ///  and a full size version in the ImagesFullSize table.
    void  ImageInsert (const RasterSipper&       image,
                       const KKStr&              imageFileName,
                       const KKStr&              sipperFileName,
                             uint64              byteOffset,     // byteOffset of SipperRow containing TopLeftRow
                             uint32              topLeftRow,
                             uint32              topLeftCol,
                             uint32              height,
                             uint32              width,
                             uint32              pixelCount,
                             uchar               connectedPixelDist,
                             uint32              extractionLogEntryId,
                             uint32              classLogEntryId,
                             uint32              centroidRow,
                             uint32              centroidCol,
                             MLClassConstPtr  class1,
                             float               class1Prob,
                             MLClassConstPtr  class2,
                             float               class2Prob,
                             MLClassConstPtr  validatedClass,
                             float               depth,
                             float               imageSize,
                             PointListPtr        sizeCoordinates,
                             int32&              imageId,
                             bool&               successful
                       );


    /**
     *@brief Erases all data related to a single SipperFile except the entry in the SipperFiles table.
     *@details  Calls stored procedure 'ImagesEraseSipperFile' which will delete all data related to 
     * sipper file '_sipperFileName' from the tables 'ImagesFullSize', 'ImageGroupEntries', 
     * 'InstrumentData', 'FeatureData', and 'Images'.
     */
    void  ImagesEraseSipperFile (const KKStr&  _sipperFileName);

    void  ImagesEraseSipperFileSizeRange (const   KKStr&  _sipperFileName,
                                          uint32          _minSize,
                                          uint32          _maxSize
                                         );

    DataBaseImagePtr      ImageLoad (uint32  imageId);  

    DataBaseImagePtr      ImageLoad (const KKStr&   imageFileName);  // Root name of image.

    DataBaseImagePtr      ImagesLocateClosestImage (const KKStr&  imageFileName);

    DataBaseImagePtr      ImagesLocateClosestImage (const KKStr&  sipperFileName,
                                                    uint32        scanLine,
                                                    uint32        scanCol
                                                   );


    DataBaseImageListPtr  ImagesQuery (DataBaseImageGroupPtr  group,
                                       bool                   includeThumbnail,
                                       const bool&            cancelFlag
                                      );
      
   
    DataBaseImageListPtr  ImagesQuery (DataBaseImageGroupPtr  imageGroup,
                                       const KKStr&           sipperFileName,
                                       MLClassConstPtr     mlClass,
                                       char                   classKeyToUse,   /**< 'P' - Use Prediced Class Key,  'V' - Validated Class */
                                       float                  probMin,
                                       float                  probMax,
                                       int32                  sizeMin,
                                       int32                  sizeMax,
                                       float                  depthMin,
                                       float                  depthMax,
                                       uint32                 restartImageId,
                                       int32                  limit,            // Max # of rows to return.  -1 indicates no limit.
                                       bool                   includeThumbnail,
                                       const bool&            cancelFlag
                                      );



    DataBaseImageListPtr  ImagesQuery (DataBaseImageGroupPtr imageGroup,
                                       const KKStr&          cruiseName,
                                       const KKStr&          stationName,
                                       const KKStr&          deploymentNum,
                                       MLClassConstPtr    mlClass,
                                       char                  classKeyToUse,
                                       float                 probMin,
                                       float                 probMax,
                                       int32                 sizeMin,
                                       int32                 sizeMax,
                                       float                 depthMin,
                                       float                 depthMax,
                                       uint32                restartImageId,
                                       int32                 limit,            // Max # of rows to return.  -1 indicates no limit.
                                       bool                  includeThumbnail,
                                       const bool&           cancelFlag
                                      );


    DataBaseImageListPtr  ImagesQuery (const KKStr&        cruiseName,
                                       const KKStr&        stationName,
                                       const KKStr&        deploymentNum,
                                       MLClassConstPtr  mlClass,
                                       char                classKeyToUse,
                                       float               minProb,
                                       float               minSize,
                                       const KKStr&        dataField1Name,
                                       float               dataField1Min,
                                       float               dataField1Max,
                                       const KKStr&        dataField2Name,
                                       float               dataField2Min,
                                       float               dataField2Max,
                                       const KKStr&        dataField3Name,
                                       float               dataField3Min,
                                       float               dataField3Max,
                                       uint32              restartImageId,
                                       int32               limit            // Max # of rows 2 return.  -1 idicates no limit.
                                      );


    DataBaseImageListPtr  ImagesQueryByGrouop 
                                    (DataBaseImageGroupPtr  imageGroup,
                                     const KKStr&           cruiseName,
                                     const KKStr&           stationName,
                                     const KKStr&           deploymentNum,
                                     const KKStr&           sipperFileName,
                                     MLClassConstPtr     mlClass,
                                     char                   classKeyToUse,
                                     float                  probMin,
                                     float                  probMax,
                                     int32                  sizeMin,
                                     int32                  sizeMax,
                                     float                  depthMin,
                                     float                  depthMax,
                                     uint32                 restartImageId,
                                     int32                  limit,            // Max # of rows to return.  -1 indicates no limit.,
                                     bool                   includeThumbnail
                                    );
    

    DataBaseImageListPtr  ImagesQueryForScanLineRange (const KKStr&   sipperFileName,
                                                       uint32         scanLineStart,
                                                       uint32         scanLineEnd
                                                      );


    VectorKKStr*          ImageListOfImageFileNamesByScanLineRange (const KKStr&   sipperFileName,
                                                                    uint32         scanLineStart,
                                                                    uint32         scanLineEnd
                                                                   );




    // Will update the instrument data fields in both  'Images' and 'FeatureData' tables for 
    // sipper files and scan range specified.
    void  ImageUpdateInstrumentDataFields (InstrumentDataPtr  instumentData,
                                           const KKStr&       sipperFileName,
                                           uint32             scanLineStart,
                                           uint32             scanLineEnd
                                          );


    void  ImagesUpdatePredictions (const KKStr&        imageFileName,
                                   MLClassConstPtr  class1Pred,
                                   float               class1Prob,
                                   MLClassConstPtr  class2Pred,
                                   float               class2Prob,
                                   uint32              logEntryId
                                  );


    /**
     *@brief  Updates prediction on multiple images at one time.
     *@param[in]  _logEntryId Log id of process that is doing th eclassification.
     *@param[in]  _predictionList  A tab delimited string that will contain the list of predictions.  Each
     *   image will be separated by line feed ('\n') charaters while each field for each uiage will be
     *   separated by 'tabs'('\t').  So if there are two images with predictions the string will look like
     *   "<ImageFileRootName><\t><Class1Name><\t><Class1Prob><\t><Class2Name><\t><Class2Prob><\n><ImageFileRootName><\t><Class1Name><\t><Class1Prob><\t><Class2Name><\t><Class2Prob>"
     */
    void  ImagesUpdatePredictionsList (uint32        _logEntryId,
                                       const KKStr&  _predictionList
                                      );


    void  ImagesUpdateValidatedClass (const KKStr&        imageFileName, 
                                      MLClassConstPtr  mlClass
                                     );

    void  ImagesUpdateImageSize (const KKStr&        imageFileName,
                                 const PointListPtr  sizeCoordinates,
                                 float               imageSize
                                );

    void  ImageRemoveValidation (const KKStr&  imageFileName);

    DataBaseImageValidatedEntryListPtr
          ImagesRetrieveValidatedClass (const KKStr&  sipperFileName);


    void  ImagesDeleteOneImage (const KKStr&  imageFileName);


    /// Update both the Validates and Class1Name
    void  ImagesUpdateValidatedAndPredictClass (const KKStr&        imageFileName, 
                                                MLClassConstPtr  mlClass, 
                                                float               class1Prob
                                               );

    void  ImageUpdate (DataBaseImage&  dbImage,
                       RasterSipper&   image
                      );


    ClassStatisticListPtr  ImageGetClassStatistics (DataBaseImageGroupPtr  imageGroup,
                                                    const KKStr&           sipperFileName,
                                                    MLClassConstPtr     mlClass,
                                                    char                   classKeyToUse,
                                                    float                  minProb,
                                                    float                  maxProb,
                                                    int32                  minSize,
                                                    int32                  maxSize,
                                                    float                  minDepth,
                                                    float                  maxDepth
                                                   );


    ClassStatisticListPtr  ImageGetClassStatistics (DataBaseImageGroupPtr  imageGroup,
                                                    const KKStr&           cruiseName,
                                                    const KKStr&           stationName,
                                                    const KKStr&           deploymentNum,
                                                    MLClassConstPtr     mlClass,
                                                    char                   classKeyToUse,
                                                    float                  minProb,
                                                    float                  maxProb,
                                                    int32                  minSize,
                                                    int32                  maxSize,
                                                    float                  minDepth,
                                                    float                  maxDepth
                                                   );



    /// Will return a pointer to a vector<int32> instance that will contain counts of images
    /// by depth.  Each element in the vector will represent a depth range such that
    /// index 0 will be for depth range (0 <= images.Depth < depthIncrements)
    /// index 1 will ""     ""      ""  (depthIncrements <= images.Depth < (2 * depthIncrements))
    VectorUint*  ImageGetDepthStatistics (DataBaseImageGroupPtr  imageGroup,
                                          const KKStr&           sipperFileName,
                                          float                  depthIncrements,
                                          MLClassConstPtr     mlClass,
                                          char                   classKeyToUse,
                                          float                  minProb,
                                          float                  maxProb,
                                          int32                  minSize,
                                          int32                  maxSize
                                         );


    VectorUint*  ImageGetDepthStatistics (DataBaseImageGroupPtr  imageGroup,
                                          const KKStr&           cruiseName,
                                          const KKStr&           stationName,
                                          const KKStr&           deploymentNum,
                                          float                  depthIncrements,
                                          MLClassConstPtr     mlClass,
                                          char                   classKeyToUse,
                                          float                  minProb,
                                          float                  maxProb,
                                          int32                  minSize,
                                          int32                  maxSize,
                                          bool&                  cancelFlag
                                         );


    
    //***********************************************************************************
    RasterSipperPtr  ImageFullSizeLoad (const KKStr&  imageFileName);


    void       ImageFullSizeDelete (const KKStr&  imageFileName);


    void       ImageFullSizeSave (const KKStr&         imageFileName,
                                  const RasterSipper&  raster
                                 );

    /// Will locate a original size version of the image; it will try in order 3 different sources.
    /// 1) ThumbNail in Images Table.
    /// 2) FullSizeImage in ImagesFullSize table
    /// 3) will go to SipperFile
    RasterSipperPtr  ImageFullSizeFind (const KKStr&  imageFileName);



    //***********************************************************************************
    MLClassConstPtr  MLClassLoad (const KKStr&  className);

    MLClassConstListPtr  MLClassLoadList ();

    void  MLClassInsert (MLClass&  mlClass,
                            bool&        successful
                           );

    void  MLClassUpdate (const KKStr&       oldClassName,
                            const MLClass&  mlClass,
                            bool&              successful
                           );

    /**
     *@brief  Inserts 'mlClass' into database if it is not already in it and then updates all the fields
     * in 'mlClass' with the data in the database.
     */
    void  MLClassInsertReturn (MLClass&  mlClass,
                               bool&     successful
                              );

    void  MLClassDelete (const KKStr&  mlClassName);




    //***********************************************************************************
    void  ImageGroupInsert (DataBaseImageGroup&  imageGroup);

    DataBaseImageGroupListPtr  ImageGroupLoad ();   // returns a list of all Group Assignments

    DataBaseImageGroupPtr      ImageGroupLoad (const KKStr&  name);  

    void  ImageGroupDelete (int32 imageGroupId);



    //***********************************************************************************
    void  ImageGroupEntriesInsert (const DataBaseImageGroupEntry&  imageGroupEntry);

    void  ImageGroupEntriesInsert (const DataBaseImageGroupEntryList&  imageGroupList);

    /**
     *@brief  
     *@details
     * Will insert a list of images into the same group in ImageGroupEntries.  A list of ImageFileNames
     * that failed to be inserted with a description of the error will be returned.  If NULL is returned
     * the the insertion failed completely.   A 2 x n matrix will be returned, One row for each image that
     * failed insertion.  
     *  - [0,0] = ImageFileName for row '0' that failed insertion
     *  - [0,1] = Description of error that caused insertion to fail.
     *
     * Since there is a limitation on the length ogf the MySQL parameter of 64K  we need to make sure that the
     * length of all the imageFileNames will be less than 64K  that is  sum_i (ImageFileNamess[i].Len ()) + imageFileNams.size () < 64000.
     */
    KKStrMatrixPtr  ImageGroupEntriesInsert (int32                 groupId,         // Will insert a list of images into the same 
                                             const VectorKKStr&  imageFileNames   // group
                                            );

    DataBaseImageGroupEntryListPtr  ImageGroupEntriesLoad (int32 groupId);





    //***********************************************************************************

    /**
     *@brief Retrieves the closest Instrument Data that preceeds or equals 'ScanLine' but
     * not more that 8192 scan-lines before.
     */
    InstrumentDataPtr  InstrumentDataGetByScanLine (const KKStr&  sipperFileName,
                                                    uint32        scanLine
                                                   );


    void  InstrumentDataInsert (const KKStr&           _sipperFileName,
                                const InstrumentData&  id
                               );


    void  InstrumentDataSaveListForOneSipperFile (const KKStr&               _sipperFileName,
                                                  const InstrumentDataList&  instrumentData,
                                                  const bool&                _cancelFlag
                                                 );



    /**
     *@brief  Will extract Instrument Data from "ALL" known Sipper files and update the database tables.
     *@details  For each known SIPPER file;  will load instrument data from from Sipper file and save
     * into 'InstrumentData' table. useful in populating 'InstrumentData' table.  Make sure that
     * the Instrument assignments are correct in 'SipperFiles'.<br />
     *
     * Also make sure that all the Sipper Files are in one of the directory trees specified by
     * 'SipperDirectoryList.txt' in "%PicesHomeDir%\DataFiles"
     */
    void  InstrumentDataLoadAllSipperFiles (const bool&  cancelFlag);


    /**
     *@brief Returns a list of all the field names in the Instrument Data table.
     */
    VectorKKStr  InstrumentDataGetFieldNames ();


    InstrumentDataListPtr  InstrumentDataLoad (const KKStr&  sipperFileName,
                                               const bool&   cancelFlag
                                              );


    InstrumentDataListPtr  InstrumentDataLoad (const KKStr&  cruiseName,
                                               const KKStr&  stationName,
                                               const KKStr&  deploymentNum,
                                               const bool&   cancelFlag
                                              );



    /**
     *@brief  Re-Extracts Instrument data from SIPPER file and updates DataBase tables to make sure they are correct.
     *@details
     * This method re-extracts instrument data for the given SipperFile and updates the
     * appropriate tables.  This way all tables are in agreement with each other.
     * For the Specified SIPPER File "siperFileName".
     *@code
     *  - 1) Extract Instrument Data from original SipperFile.
     *  - 2) Replaces all entries in IntrumentData table for 'sipperFile' with what was just extracted. 
     *  - 3) Updates the InstrumentData fields in Images with data just extracted.
     *@endcode
     *@param[in]  sipperFileName  Name of Sipper files that insrument data is to be extracted from; the root name wil be used.
     *@param[in]  cancelFlag      The method will monitor this flag; if it goes true it will exit gracefully and return to caller.
     *@param[in]  msgBuff         Pointer to message buffer that the method will write NULL terminated strings to this field indicating progress;
     *@param[in]  msgBuffLen      Length in characters of 'msgBuff'.
     */
    void  InstrumentDataReFreshSipperFile (const KKStr&   sipperFileName,
                                           const bool&    cancelFlag,
                                           char*          msgBuff,
                                           int32          msgBuffLen
                                          );

    /**
     *@brief Returns volume of water sampled, in cubic meters, by depth for a given sipper file.
     */
    VolumeSampledStatListPtr  InstrumentDataGetVolumePerMeterDepth (const KKStr&  siperFileName,
                                                                    float         depthBinSize
                                                                   );


    /**
     *@brief Returns volume of water sampled, in cubic meters, by depth for a complete deployment.
     */
    VolumeSampledStatListPtr  InstrumentDataGetVolumePerMeterDepth (const KKStr&  cruiseName,
                                                                    const KKStr&  stationName,
                                                                    const KKStr&  deploymentNum,
                                                                    float         depthBinSize,
                                                                    const bool&   cancelFlag
                                                                   );


    /**
     *@brief  Update all InstrumentData entries tht fall within the dateTime range with the specified Latitude and Longitude.
     */
    void  InstrumentDataUpdateLatitudeAndLongitude (const KKU::DateTime&  dateTimeStart,
                                                    const KKU::DateTime&  dateTimeEnd,
                                                    double                latitude,
                                                    double                longitude
                                                   );


    /**
     *@brief Returns 2d array consisting of the specific data fields from the Instrument data file 
     * for the specified SipperFile.
     *@returns 2D float array where each row will represent a Instrument row and each column will 
     *  represent a field specified in 'fieldNames'.
     */
    vector<vector<float> >*  InstrumentDataGetSpecificFields (const KKStr&          sipperFileName,
                                                              const KKStrListPtr    fieldNames,
                                                              const KKU::DateTime&  dateTimeStart,
                                                              const KKU::DateTime&  dateTimeEnd
                                                             );


     vector<vector<float> >*  InstrumentDataGetSpecificFields (const KKStr&          cruiseName,
                                                               const KKStr&          stationName,
                                                               const KKStr&          deploymentNum,
                                                               const KKStrListPtr    fieldNames,
                                                               const KKU::DateTime&  dateTimeStart,
                                                               const KKU::DateTime&  dateTimeEnd
                                                              );


    GPSDataPointListPtr InstrumentDataRetrieveGPSInfo (const KKStr&  cruiseName,
                                                       const KKStr&  stationName,
                                                       const KKStr&  deploymentNum,
                                                       int32         timeInterval   /**< Interval in seconds */
                                                      );



    InstrumentDataMeansListPtr  InstrumentDataBinByMeterDepth (const KKStr&  cruiseName,
                                                               const KKStr&  stationName,
                                                               const KKStr&  deploymentNum,
                                                               float         depthBinSize,
                                                               const bool&   cancelFlag
                                                              );


    //***********************************************************************************
    DataBaseLogEntryPtr  LogEntriesProcessStart (const KKStr&          progCode,
                                                 const KKStr&          progName,
                                                 const KKU::DateTime&  dateTimeCompiled,
                                                 const KKStr&          cmdLine,
                                                 const KKStr&          sipperFileName   /**< Root name of Sipper file; only if specific to a single Sipper File */
                                                );
  

    void  LogEntriesProcessEnd (DataBaseLogEntryPtr   logEntry,
                                float                 cpuTimeUsed,
                                const KKStr&          completionResult
                               );


    DataBaseLogEntryPtr  LogEntriesSelect (uint32 _logEntryId);






    //***********************************************************************************
    void  InsertCruise (const KKStr&     shipName, 
                        const KKStr&     description,
                        const DateType&  dateStart,
                        const DateType&  dateEnd,
                        const KKStr&     location,
                        const KKStr&     objective,
                        const KKStr&     principal,
                        const KKStr&     researchOrg
                       );


    void                  SipperCruiseDelete (const KKStr&  cruiseName);

    void                  SipperCruiseInsert (SipperCruisePtr  cruise,
                                              bool&            successful
                                             );

    SipperCruisePtr       SipperCruiseLoad (const KKStr&  cruiseName);

    SipperCruiseListPtr   SipperCruiseLoadAllCruises ();

    void                  SipperCruiseUpdate (SipperCruisePtr  cruise,
                                              bool&            successful
                                             );


    //***********************************************************************************
    SipperDeploymentListPtr  SipperDeploymentLoad (const KKStr& cruiseName,
                                                   const KKStr& stationName
                                                  );
    
    SipperDeploymentPtr      SipperDeploymentLoad (const KKStr&  cruiseName,
                                                   const KKStr&  stationName,
                                                   const KKStr&  deploymentNum
                                                  );
    
    void                     SipperDeploymentInsert (const SipperDeployment&  deployment);

    void                     SipperDeploymentUpdate (const SipperDeployment&  deployment);

    void                     SipperDeploymentDelete (const KKStr&  cruiseName,
                                                     const KKStr&  stationName,
                                                     const KKStr&  deploymentNum
                                                    );



    //***********************************************************************************
    VectorKKStr*          SipperFileGetList (const KKStr& cruiseName,
                                             const KKStr& stationName,
                                             const KKStr& deploymentNum
                                            );

    SipperFileListPtr     SipperFileLoad      (const KKStr& cruiseName,
                                               const KKStr& stationName,
                                               const KKStr& deploymentNum
                                              );

    void                  SipperFileLoadOracle ();

    SipperFilePtr         SipperFileRecLoad   (const KKStr& _sipperFileName);


    void                  SipperFileInsert (SipperFile&   sipperFile);

    void                  SipperFileUpdate (SipperFile&   sipperFile);

    void                  SipperFilesUpdateExtractionStatus (const KKStr&  sipperFileName,
                                                             char          extractionStatus
                                                            );

    void                  SipperFileUpdateFileSizeStats (const KKStr&  _sipperFileName, 
                                                         int64         _sizeInBytes,
                                                         uint32        _numScanLines
                                                        );

    void                  SipperFilesDelete (const KKStr&  _sipperFileName);  /**< Delete a Sipper File and all its related data from all tables in database. */


    void                  SipperFilesGetCTDDateTime (const KKStr&    _sipperFileName,
                                                     int32&          _sipperFileId, 
                                                     uint32&         _numScanLines, 
                                                     KKU::DateTime&  _CTDDateTimeStart, 
                                                     KKU::DateTime&  _CTDDateTimeEnd
                                                    );



    //***********************************************************************************
    SipperStationListPtr  SipperStationLoad (const KKStr&  cruiseName);
    
    SipperStationPtr      SipperStationLoad (const KKStr&  cruiseName,
                                             const KKStr&  stationName
                                            );
    
    void                  SipperStationInsert (const SipperStation&  station);

    void                  SipperStationUpdate (const SipperStation&  station);

    void                  SipperStationDelete (const KKStr&  cruiseName,
                                               const KKStr&  stationName
                                              );
    void   ResultSetsClear ();

  private:
    static  GoalKeeperPtr  blocker;   /*!< used to handle multi-threading issues. */

    static volatile int32  numDataBaseInstances;  /**< Will be incremented when the constructor is called and
                                                   * decremented when the destructor is called.  This way the
                                                   * Constructor and Destructors will know if they should call
                                                   * "mysql_library_init" and "mysql_library_end" respectively.
                                                   */

    static KKStr           mySqlDataDir;

    static volatile bool   mySqlEmbeddedServerInitailzied;  /**< true indicates that embedded server is available. */

    static KKStr           mySqlErroLogFileName;

    static KKStr           mySqlHomeDir;

    static volatile bool   staticVariablesInialized;

    typedef  std::map<DataBasePtr,DataBasePtr>   DataBaseIndex;
    typedef  std::pair<DataBasePtr,DataBasePtr>  DataBaseIndexPair;
    static DataBaseIndex   existingInstances;


    /** Call this methid when a update is called that is not allowed when 'allowUpdates == false'. */
    static  void   UpdatesNotAllowed (const KKStr&  methodName);


    static  void   CreateBlocker ();  /*!< Call this to make sure that Blocker exists.  Will handle race conditions. */
    static  void   InitializeDataBaseStaticVariables ();
    static  void   InitializeMySqlLibrary ();
    static  void   InitializeMySqlLibraryEmbedded ();
    static  void   InitializeMySqlLibraryServer ();
    static  void   DataBaseAddInstance (DataBasePtr  _instance);
    static  void   DataBaseDeleteInstance (DataBasePtr  _instance);



    DataBaseServerPtr  GetDefaultMySqlParameters ();


    int32  Connect ();

    int32  ConnectEmbedded ();

    int32  ConnectServer ();

    int32  ReConnect (const KKStr&  funcCall,
                      const KKStr&  queryStrMsg,
                      int32         numReTries
                     );

    void  ValidateConnection (const char*  funcCall);



    // Routines to format MySql dtatat types.
    KKU::DateTime  DateTimeFromMySqlDateTimeField (KKStr  field);

    KKU::TimeType  TimeFromMySqlDateTimeField (KKStr field);

    KKU::DateType  DateFromMySqlDateTimeField (KKStr  field);

    KKStr   DateTimeToQuotedStr (const KKU::DateTime&  dt);

    KKStr    FloatToStr (float f);

    SipperDeploymentListPtr  SipperDeploymentProcessResults ();

    DataBaseGpsDataListPtr   GpsDataProcessResults ();


    static char**         featureDataFieldNames;
    static ConstCharStarArray     GetFeatureDataFieldNames ();

    ImageFeaturesListPtr  FeatureDataProcessResults ();  // Used by the FeatureData retrieval routines.


    uchar*  EncodeARasterImageIntoAThumbNail (const RasterSipper&  image,
                                              uint32               maxDimSizeAllowed,
                                              uint32&              buffLen
                                             );

    MLClassListPtr  MLClassProcessResults ();

    DataBaseImageListPtr   ImageQueryProcessResults ();

    ClassStatisticListPtr  ImageProcessClassStaticsResults ();

    InstrumentDataListPtr  InstrumentDataProcessResults (const bool&  cancelFlag);

    SipperCruiseListPtr  SipperCruiseProcessResults ();

    SipperFileListPtr  SipperFileProcessResults ();
    static char*  sipperFileFieldNames[];

    SipperStationListPtr  SipperStationProcessResults ();


    /** Copies String from src to dest making sure that it does not exceed 'destSize'. */
    void  WriteBuff (char*        dest,
                     KKU::int32   destSize,
                     const char*  src
                    );


    RunLog&  log;

    bool               allowUpdates;        /**< Indicates that no updates are allowed; controlled by 
                                             * 'Permissions.cfg' file in ${PicesHomeDir}\Configurations        */

    bool               useEmbeddedServer;   /**< When set to 'true' we are to operate as an embeded server.    */
    MYSQL*             conn;
    DataBaseServerPtr  server;              /**< We will own this instance and be responsible for deleting.    */
    bool               duplicateKey;        /**< Is set to true during insert if a duplicate key error occurs. */
    bool               valid;               /**< Will be set to false if connection is bad.                    */
    uint32             lastMySqlErrorNo;
    KKStr              lastMySqlErrorDesc;
    KKStr              prevQueryStatement;

    FileDescPtr        featureFileDesc;


    MYSQL_FIELD*       resultSetFieldDefs;
    MYSQL_RES*         resultSet;
    uint32             resultSetNumFields;
    uint32             resultSetNumRows;
    bool               resultSetMore;   
    MYSQL_ROW          resultSetNextRow;
    VectorInt          resultSetFieldIndex;
    ulong*             resultSetLengths;


    void   ResultSetBuildFieldIndexTable (ConstCharStarArray    fieldNames);
    bool   ResultSetLoad    (ConstCharStarArray   fieldNames);
    bool   ResultSetLoadFieldNames (VectorKKStr&  fieldNames);
    bool   ResultSetGetNext (const char**  fieldNames);
    bool   ResultSetFetchNextRow ();
    void   ResulSetFree ();

    
    bool             ResultSetGetBool          (uint32 fieldIdx);
    char             ResultSetGetCharField     (uint32 fieldIdx);
    const char*      ResultSetGetField         (uint32 fieldIdx);
    ulong            ResultSetGetFieldLen      (uint32 fieldIdx);
    KKStr            ResultSetGetKKStrField    (uint32 fieldIdx);
    KKU::DateTime    ResultSetGetDateTimeField (uint32 fieldIdx);
    KKU::TimeType    ResultSetGetTimeField     (uint32 fieldIdx);
    double           ResultSetGetDoubleField   (uint32 fieldIdx);
    float            ResultSetGetFloatField    (uint32 fieldIdx);
    int32            ResultSetGetIntField      (uint32 fieldIdx);
    KKU::int64       ResultSetGetInt64Field    (uint32 fieldIdx);
    RasterSipperPtr  ResultSetGetRasterField   (uint32 fieldIdx);
    uint32           ResultSetGetUintField     (uint32 fieldIdx);
    KKU::ulong       ResultSetGetUlongField    (uint32 fieldIdx);
    KKU::uint64      ResultSetGetUint64Field   (uint32 fieldIdx);
  };


  typedef  DataBase::DataBasePtr  DataBasePtr;

}  /* namespace MLL */





#endif
