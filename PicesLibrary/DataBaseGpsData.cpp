#include "FirstIncludes.h"
#include <stdlib.h>
#include <memory>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "MemoryDebug.h"
using namespace std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "Str.h"
using namespace KKU;


#include "DataBaseGpsData.h"
using namespace MLL;




DataBaseGpsData::DataBaseGpsData ():
    cruiseName        (),
    utcDateTime       (),
    latitude          (0.0),
    longitude         (0.0),
    courseOverGround  (0.0f),
    speedOverGround   (0.0f)
{
}



DataBaseGpsData::DataBaseGpsData (const DataBaseGpsData&  gpsData):
    cruiseName       (gpsData.cruiseName),
    utcDateTime      (gpsData.utcDateTime),
    latitude         (gpsData.latitude),
    longitude        (gpsData.longitude),
    courseOverGround (gpsData.courseOverGround),
    speedOverGround  (gpsData.speedOverGround)
{
}



DataBaseGpsData::DataBaseGpsData (const KKStr&     _cruiseName,
                                  const DateTime&  _utcDateTime,
                                  double           _latitude,
                                  double           _longitude,
                                  float            _courseOverGround,
                                  float            _speedOverGround
                                 ):
    cruiseName       (_cruiseName),
    utcDateTime      (_utcDateTime),
    latitude         (_latitude),
    longitude        (_longitude),
    courseOverGround (_courseOverGround),
    speedOverGround  (_speedOverGround)
{
}



DataBaseGpsData::~DataBaseGpsData ()
{
}




char const *const   DataBaseGpsData::fieldList[] = {"CruiseName", "UtcDateTime", "Latitude", "Longitude", "CourseOverGround", "SpeedOverGround", NULL};



char const *const *const  DataBaseGpsData::FieldList ()
{
  return  fieldList;
}



DataBaseGpsDataList::DataBaseGpsDataList (bool _owner):
      KKQueue<DataBaseGpsData> (_owner)
{
}



DataBaseGpsDataList::~DataBaseGpsDataList ()
{
}



