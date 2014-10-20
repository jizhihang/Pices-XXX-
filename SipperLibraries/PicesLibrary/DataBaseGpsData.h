#if  !defined(_DATABASEGPSDATA_)
#define  _DATABASEGPSDATA_

#include  "Str.h"



namespace MLL
{
  class DataBaseGpsData
  {
  public:
    typedef  DataBaseGpsData*  DataBaseGpsDataPtr;

    DataBaseGpsData ();

    DataBaseGpsData (const DataBaseGpsData&  _gpsData);


    DataBaseGpsData (const KKStr&          _cruiseName,
                     const KKU::DateTime&  _utcDateTime,
                     double                _latitude,
                     double                _longitude,
                     float                 _courseOverGround,
                     float                 _speedOverGround
                    );

    ~DataBaseGpsData ();

    const KKStr&   CruiseName        ()  const  {return cruiseName;}
    KKU::DateTime  UtcDateTime       ()  const  {return utcDateTime;}
    double         Latitude          ()  const  {return latitude;}
    double         Longitude         ()  const  {return longitude;}
    float          CourseOverGround  ()  const  {return courseOverGround;}
    float          SpeedOverGround   ()  const  {return speedOverGround;}

    KKStr          ToTabStr ()  const;   /**< tab separated list of class contents; order specified by 'DataBaseGpsData::FieldList'. */

    static  char const *const *const   FieldList ();

  private:
    KKStr          cruiseName;
    KKU::DateTime  utcDateTime;
    double         latitude;
    double         longitude;
    float          courseOverGround;
    float          speedOverGround;

    static  char const *const   fieldList[];
  };

  typedef  DataBaseGpsData::DataBaseGpsDataPtr  DataBaseGpsDataPtr;




  class  DataBaseGpsDataList: public  KKQueue<DataBaseGpsData>
  {
  public:
    typedef  DataBaseGpsDataList*  DataBaseGpsDataListPtr;

    DataBaseGpsDataList (bool  _owner = true);
    ~DataBaseGpsDataList ();
  };  /* DataBaseImageList */


  typedef  DataBaseGpsDataList::DataBaseGpsDataListPtr  DataBaseGpsDataListPtr;
}  /* namespace MLL */


#endif
