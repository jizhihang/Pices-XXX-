#ifndef  _DATABASEIMAGE_
#define  _DATABASEIMAGE_

/**
 @class  MLL::DataBaseImage
 @details 
 @code
 ************************************************************************************************
 **   Used for returning results of Queries against the Images table in the Pices database (See *
 **   DataBase::ImagesQuery).                                                                    *
 ************************************************************************************************
 @endcode
 */

#ifndef  _mysql_h
struct  MYSQL;
struct  MYSQL_RES;
struct  MSQL_STRUCT; 
typedef  MSQL_STRUCT* MYSQL_ROW;
#endif

#include  "OSservices.h"
#include  "RasterSipper.h"
#include  "RunLog.h"
using namespace KKU;

namespace MLL 
{


  #ifndef  _MLCLASS_
  class  MLClass;
  typedef  MLClass*  MLClassPtr;
  class  MLClassList;
  typedef  MLClassList*  MLClassListPtr;
  #endif




  class  DataBaseImage
  {
  public:
    typedef  DataBaseImage*  DataBaseImagePtr;

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;
    typedef  KKU::ulong    ulong;
    typedef  KKU::uchar    uchar;

    DataBaseImage ();
    ~DataBaseImage ();

    float           BreakTie             () const;
    osFilePos       ByteOffset           () const  {return  byteOffset;}
    uint32          CentroidCol          () const  {return  centroidCol;}
    uint32          CentroidRow          () const  {return  centroidRow;}
    MLClassPtr   Class1               () const  {return  class1;}
    const KKStr&    Class1Name           () const;
    float           Class1Prob           () const  {return  class1Prob;}
    MLClassPtr   Class2               () const  {return  class2;}
    const KKStr&    Class2Name           () const;
    float           Class2Prob           () const  {return  class2Prob;}
    uint32          ClassLogEntryId      () const  {return  classLogEntryId;}
    uchar           ConnectedPixelDist   () const  {return  connectedPixelDist;}
    float           Depth                () const  {return  depth;}
    const uchar*    EncodedThumbnail     () const  {return  encodedThumbnail;}
    uint32          EncodedThumbnailLen  () const  {return  encodedThumbnailLen;}
    uint32          ExtractionLogEntryId () const  {return  extractionLogEntryId;}
    uint32          Height               () const  {return  height;}
    KKStr           ImageFileName        () const;
    int32           ImageId              () const  {return  imageId;}
    float           ImageSize            () const  {return  imageSize;}
    uint32          PixelCount           () const  {return  pixelCount;}
    int32           SipperFileId         () const  {return  sipperFileId;}
    const KKStr&    SipperFileName       () const  {return  sipperFileName;}
    PointListPtr    SizeCoordinates      () const  {return  sizeCoordinates;}
    KKU::RasterSipperPtr  ThumbNail      (RunLog&  log) const;
    uint32          TopLeftRow           () const  {return  topLeftRow;}
    uint32          TopLeftCol           () const  {return  topLeftCol;}
    MLClassPtr   ValidatedClass       () const  {return  validatedClass;}
    const KKStr&    ValidatedClassName   () const;
    uint32          Width                () const  {return  width;}


    RasterSipperPtr       GetOrigImageFromSipperFile (RunLog&  log) const;  /**< Will go to the original Sipper file and extract the image. */

    void    ByteOffset           (osFilePos         _byteOffset)           {byteOffset           = _byteOffset;}
    void    CentroidCol          (uint32            _centroidCol)          {centroidCol          = _centroidCol;}
    void    CentroidRow          (uint32            _centroidRow)          {centroidRow          = _centroidRow;}
    void    Class1               (MLClassPtr     _class1)               {class1               = _class1;}
    void    Class1Name           (const char*       _class1Name);
    void    Class1Prob           (float             _class1Prob)           {class1Prob           = _class1Prob;}
    void    Class2               (MLClassPtr     _class2)               {class2               = _class2;}
    void    Class2Name           (const char*       _class2Name);
    void    Class2Prob           (float             _class2Prob)           {class2Prob           = _class2Prob;}
    void    ClassLogEntryId      (uint32            _classLogEntryId)      {classLogEntryId      = _classLogEntryId;}
    void    ConnectedPixelDist   (uchar             _connectedPixelDist)   {connectedPixelDist   = _connectedPixelDist;}
    void    Depth                (float             _depth)                {depth                = _depth;}
    void    EncodedThumbnail     (uchar*            _encodedThumbnail)     {encodedThumbnail     = _encodedThumbnail;}
    void    EncodedThumbnailLen  (uint32            _encodedThumbnailLen)  {encodedThumbnailLen  = _encodedThumbnailLen;}
    void    ExtractionLogEntryId (uint32            _extractionLogEntryId) {extractionLogEntryId = _extractionLogEntryId;}
    void    Height               (uint32            _height)               {height               = _height;}
    void    ImageId              (int32             _imageId)              {imageId              = _imageId;}
    void    ImageSize            (float             _imageSize)            {imageSize            = _imageSize;}
    void    PixelCount           (uint32            _pixelCount)           {pixelCount           = _pixelCount;}
    void    SipperFileId         (int32             _sipperFileId)         {sipperFileId         = _sipperFileId;}
    void    SipperFileName       (const KKStr&      _sipperFileName)       {sipperFileName       = _sipperFileName;}  
    void    SizeCoordinates      (PointListPtr      _sizeCoordinates);                                                    /**< Giving ownership of '_sizeCoordinates' to this instance of 'DataBaseImage'. */
    void    SizeCoordinates      (const KKStr       _sizeCoordinatesStr);                                                 /**< Giving ownership of '_sizeCoordinates' to this instance of 'DataBaseImage'. */
    void    SizeCoordinatesAdd   (const KKU::Point& _point);                                                              /**< Adds additional point to end of 'SizeCoordinates' list. */
    void    TopLeftRow           (uint32            _topLeftRow)           {topLeftRow           = _topLeftRow;}
    void    TopLeftCol           (uint32            _topLeftCol)           {topLeftCol           = _topLeftCol;}
    void    ValidatedClass       (MLClassPtr     _validatedClass)       {validatedClass       = _validatedClass;}
    void    ValidatedClassName   (const char*       _validatedClassName);
    void    Width                (uint32            _width)                {width                = _width;}

  private:
    osFilePos       byteOffset;
    uint32          centroidRow;
    uint32          centroidCol;
    MLClassPtr   class1;
    float           class1Prob;
    MLClassPtr   class2;
    float           class2Prob;
    uint32          classLogEntryId;
    uchar           connectedPixelDist;
    float           depth;
    uchar*          encodedThumbnail; 
    uint32          encodedThumbnailLen;
    uint32          extractionLogEntryId;
    uint32          height;
    int32           imageId;
    float           imageSize;
    uint32          pixelCount;
    int32           sipperFileId;
    KKStr           sipperFileName;
    PointListPtr    sizeCoordinates;
    uint32          topLeftRow;
    uint32          topLeftCol;
    MLClassPtr   validatedClass;
    uint32          width;
  };  /* DataBaseImage */



  typedef  DataBaseImage::DataBaseImagePtr  DataBaseImagePtr;



  class  DataBaseImageList: public  KKQueue<DataBaseImage>
  {
  public:
    typedef  DataBaseImageList*  DataBaseImageListPtr;

    DataBaseImageList (bool  _owner = true);
    ~DataBaseImageList ();

    void  SortBySpatialDistance ();


    MLClassListPtr  ExtractListOfClasses ();

    DataBaseImageListPtr  ExtractImagesForAGivenClass (MLClassPtr  _mlClass);


    VectorFloat  CalculateDensitesByQuadrat (float        scanRate,     // Scan Lines per Sec.
                                             float        quadratSize,  // Meters.
                                             float        flowRate,     // Meters per Sec
                                             const bool&  cancelFlag,
                                             RunLog&      log
                                            );

  private:
    class  SpatialComparison;
  };  /* DataBaseImageList */



  typedef  DataBaseImageList::DataBaseImageListPtr  DataBaseImageListPtr;


}  /* namespace MLL */


#endif
