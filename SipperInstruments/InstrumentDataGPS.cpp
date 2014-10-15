#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <stdio.h>
#include  <fstream>
#include  <iostream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;
#include  "BasicTypes.h"
#include  "Str.h"

using namespace KKU;



#include  "InstrumentDataGPS.h"
#include  "InstrumentDataManager.h"

using namespace  SipperHardware;



InstrumentDataGPS::InstrumentDataGPS (InstrumentDataManagerPtr _manager,
                                      const KKStr&            _reportDir,
                                      bool                     _text,
                                      int32                    _instrumentId
                                     ):

    InstrumentDataReport (_manager, _reportDir, _text, _instrumentId, false),
    curTextLine              (),
    curTextLineStartScanLine (-1),
    lastTextLine             ()
{
}



InstrumentDataGPS::~InstrumentDataGPS ()
{
  report << curTextLine;
}





void  InstrumentDataGPS::ParseDegreeMinutesStr (const KKStr&  str,
                                                double&        degrees,
                                                double&        minutes
                                               ) const
{
  degrees = 0.0;
  minutes = 0.0;

  int32 x = str.LocateCharacter ('.');
  if  (x < 0)
    return;

  KKStr  degStr = str.SubStrPart (0, x - 3);
  KKStr  minStr = str.SubStrPart (x - 2);
  
  degrees = atof (degStr.Str ());
  minutes = atof (minStr.Str ());
}  /* ParseDegreeMinutesStr */




/**
 * @brief Processes a GPGLL strig that contains Laditude and Longitudal info.
 * @param[in] _str '$GPGLL' string with '$GPGLL' stripped off front.
 */
void  InstrumentDataGPS::ProcessGPGLL (const KKStr&  _str)
{
  KKStr str (_str);

  KKStr  field = str.ExtractToken2 (",");
  if  (field == "$GPGLL")
    field = str.ExtractToken2 (",");

  double  latitudeDegrees;
  double  latitudeMinutes;
  ParseDegreeMinutesStr (field, latitudeDegrees, latitudeMinutes);
  double  latitude = latitudeDegrees + (latitudeMinutes / 60.0);

  field = str.ExtractToken2 (",");
  if  (field == "S")
    latitude = -latitude;

  field = str.ExtractToken2 (",");
  double  longitudeDegrees;
  double  longitudeMinutes;
  ParseDegreeMinutesStr (field, longitudeDegrees, longitudeMinutes);
  double  longitude = longitudeDegrees + (longitudeMinutes / 60.0);
  field = str.ExtractToken2 (",");
  if  (field == "E")
    longitude = -longitude;

  field = str.ExtractToken2 (",");
  TimeType  gpsTime (field);
  manager->Location (curTextLineStartScanLine, latitude, longitude, gpsTime);
}  /* ProcessGPGLL */



void  InstrumentDataGPS::ProcessNMEAInfo (const KKStr& _str)
{
  if  (_str.SubStrPart (0, 5) == "$GPGLL")
    ProcessGPGLL (_str);
}  /* ProcessNMEAInfo */



void  InstrumentDataGPS::ReportInstrumentData (uint32 curScanLine,
                                               uchar  sensorData
                                              )
{
  if  (sensorData == '$')
  {
    ProcessNMEAInfo (curTextLine);
    report << curTextLineStartScanLine << "\t" << curTextLine << endl;
    lastTextLine = curTextLine;
    curTextLine = "";
    curTextLineStartScanLine = curScanLine;
  }


  if  (curTextLine.Len () > 5000)
  {
    // This line is way too int32;  Something has gone wrong.
    // we will break it hear for now.
    report << curTextLineStartScanLine << "\t" << curTextLine << endl;
    lastTextLine = curTextLine;
    curTextLine = "";
    curTextLineStartScanLine = curScanLine;
  }


  if  (curTextLine.Empty ())
    curTextLineStartScanLine = curScanLine;

  curTextLine.Append ((char)sensorData);
}  /* ReportInstrumentData */
