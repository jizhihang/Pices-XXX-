#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <memory>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;

#include  "BasicTypes.h"
#include  "DateTime.h"
using namespace KKU;



#include "InstrumentData.h"
#include "SipperVariables.h"
using namespace SipperHardware;




InstrumentData::InstrumentData ():
  activeBattery (0),
  byteOffset    (0),
  ctdDate       (),
  data          (NULL),
  latitude      (0.0),
  longitude     (0.0),
  scanLine      (0)
  
{
  InitializeDataVariables ();
}



InstrumentData::InstrumentData (const InstrumentData&   id):
  activeBattery (0),
  byteOffset    (0),
  ctdDate       (),
  data          (NULL),
  latitude      (0.0),
  longitude     (0.0),
  scanLine      (0)
    
{
  InitializeDataVariables ();

  activeBattery  = id.activeBattery;
  ctdDate        = id.ctdDate;

  for  (int32 x = 0;  x < numDataFields;  x++)
    data[x] = id.data[x];

  latitude   = id.latitude;
  longitude  = id.longitude;
  scanLine   = id.scanLine;
  byteOffset = id.byteOffset;
}





InstrumentData::InstrumentData (      KKStrParser&  line, 
                                const VectorInt&    fieldIndirections
                               ):
  activeBattery (0),
  byteOffset    (0),
  ctdDate       (),
  data          (NULL),
  latitude      (0.0),
  longitude     (0.0),
  scanLine      (0)
  
{
  InitializeDataVariables ();

  uint32  n = (uint32)fieldIndirections.size ();

  KKStr  field (64);
  uint32  x = 0;
  for  (x = 0;  x < n;  x++)
  {
    field = line.GetNextToken ("\t");
    if  (fieldIndirections[x] >= 0)
    {
      UpdateFromStr (fieldIndirections[x], field);
    }
  }
}



InstrumentData::InstrumentData (uint32           _scanLine,
                                uint32            _byteOffset,
                                const DateTime&  _ctdDate,
                                char             _activeBattery,
                                double           _latitude,
                                double           _longitude,
                                float            _ctdBattery,
                                float            _conductivity,
                                float            _density,
                                float            _depth,
                                float            _flowRate1,
                                float            _flowRate2,
                                float            _fluorescence,
                                float            _fluorescenceSensor,
                                float            _cdomFluorescence,
                                float            _cdomFluorescenceSensor,
                                float            _illumination,
                                float            _oxygen,
                                float            _oxygenSensor,
                                float            _pressure,
                                float            _recordRate,
                                float            _salinity,
                                float            _soundVelocity,
                                float            _temperature,
                                float            _transmisivity,
                                float            _transmisivitySensor,
                                float            _turbidity,
                                float            _turbiditySensor,
                                float            _pitch,
                                float            _roll,
                                float            _bat1Level,
                                float            _bat2Level,
                                float            _bat3Level,
                                float            _bat4Level
                               ):

  activeBattery (_activeBattery),
  byteOffset    (_byteOffset),
  ctdDate       (_ctdDate),
  data          (NULL),
  latitude      (_latitude),
  longitude     (_longitude),
  scanLine      (_scanLine)

{
  InitializeDataVariables ();
    
  data[ctdBatteryIndex]             = _ctdBattery;
  data[ConductivityIndex]           = _conductivity;
  data[densityIndex]                = _density;
  data[depthIndex]                  = _depth;
  data[flowRate1Index]              = _flowRate1;
  data[flowRate2Index]              = _flowRate2;
  data[fluorescenceIndex]           = _fluorescence;
  data[fluorescenceSensorIndex]     = _fluorescenceSensor;
  data[cdomFluorescenceIndex]       = _cdomFluorescence;
  data[cdomFluorescenceSensorIndex] = _cdomFluorescenceSensor;
  data[illuminationIndex]           = _illumination;
  data[oxygenIndex]                 = _oxygen;
  data[oxygenSensorIndex]           = _oxygenSensor;
  data[pressureIndex]               = _pressure;
  data[recordRateIndex]             = _recordRate;
  data[salinityIndex]               = _salinity;
  data[soundVelocityIndex]          = _soundVelocity;
  data[temperatureIndex]            = _temperature;
  data[transmisivityIndex]          = _transmisivity;
  data[transmisivitySensorIndex]    = _transmisivitySensor;
  data[turbidityIndex]              = _turbidity;
  data[turbiditySensorIndex]        = _turbiditySensor;
  data[pitchIndex]                  = _pitch;
  data[rollIndex]                   = _roll;
  data[bat1LevelIndex]              = _bat1Level;
  data[bat2LevelIndex]              = _bat2Level;
  data[bat3LevelIndex]              = _bat3Level;
  data[bat4LevelIndex]              = _bat4Level;
}






void  InstrumentData::InitializeDataVariables ()
{
  activeBattery = 0;
  data = new float[numDataFields];
  for  (int32 x = 0;  x < numDataFields;  x++)
    data[x] = 0.0;

  scanLine   = 0;
  byteOffset = 0;
}  /* InitializeDataVariables */





InstrumentData::~InstrumentData ()
{
  delete  data;
  data = NULL;
}


/*
    class  FieldDesc
    {
    public:
      KKStr  name;
      KKStr  shortDesc;
      KKStr  unitOfMeasure;
      float  minVal;
      KKStr  formatStr;
    };
*/


InstrumentData::FieldDesc  InstrumentData::fieldDescriptions[] =
{
 // Name                      Short   UnitOfMeasure     Min   format 
   {"CTD_Battery",            "CB",   "#",              0.0f, "#0"},      // 0
   {"Conductivity",           "CD",   "Siemens/Meter",  0.0f, "#0.000"},  //  1
   {"Density",                "DN",   "kg/m^3",         0.0f, "#0.00"},   //  2
   {"Depth",                  "DP",   "Meters",         0.0f, "#0.00"},   //  3
   {"FlowRate1",              "FR1",  "Meters/Sec",     0.0f, "#0.00"},   //  4
   {"FlowRate2",              "FR2",  "Meters/Sec",     0.0f, "#0.00"},   //  5
   {"Fluorescence",           "FL",   "� chl/liter",    0.0f, "#0.00"},   //  6
   {"FluorescenceSensor",     "FLS",  "Volts",          0.0f, "#0.00"},   //  7
   {"CdomFluorescence",       "CDM",  "ppb QSD",        0.0f, "#0.00"},   //  8
   {"CdomFluorescenceSensor", "CDMS", "Volts",          0.0f, "#0.00"},   //  9
   {"Illumination",           "IL",   "",               0.0f, "#0.00"},   // 10
   {"Oxygen",                 "OXG",  "ml/L",           0.0f, "#0.00"},   // 11
   {"OxygenSensor",           "OXGS", "Volts",          0.0f, "#0.00"},   // 12
   {"Pressure",               "PR",   "PSI",            0.0f, "#0.00"},   // 13
   {"Record Rate",            "RR",   "MB/Sec",         0.0f, "#0.00"},   // 14
   {"Salinity",               "SAL",  "",               0.0f, "#0.00"},   // 15
   {"SoundVelocity",          "SV",   "M/S",         1200.0f, "#0"},      // 16
   {"Temperature",            "TMP",  "Celsius",       15.0f, "#0"},      // 17
   {"Transmissivity",         "TRM",  "%Light",         0.0f, "#0.00"},   // 18
   {"TransmissivitySensor",   "TRMS", "Volts",          0.0f, "#0.00"},   // 19
   {"Turbidity",              "TRB",  "NTU",            0.0f, "#0.00"},   // 20
   {"TurbiditySensor",        "TRBS", "Volts",          0.0f, "#0.00"},   // 21
   {"Pitch",                  "PTCH", "Degrees",      -35.0f, "##0"},    // 22
   {"Roll",                   "ROL",  "Degrees",      -35.0f, "##0"},    // 23 

   {"Bat1Level",              "BT1",  "Volts",          0.0f, "#0.0"},    // 24
   {"Bat2Level",              "BT2",  "Volts",          0.0f, "#0.0"},    // 25
   {"Bat3Level",              "BT3",  "Volts",          0.0f, "#0.0"},    // 26
   {"Bat4Level",              "BT4",  "Volts",          0.0f, "#0.0"},    // 27

   {"ScanLine",               "SL",   "",               0.0f, "##,###,##0"},   // 28
   {"ByteOffset",             "BO",   "",               0.0f, "##,###,###,##0"},   // 29 
   {"CTD_Date",               "CD",   "",               0.0f, "#0.00"},   // 30
   {"ActiveBattery",          "AB",   "",               0.0f, "#0.00"},   // 31
   {"Latitude",               "LAT",  "",               0.0f, "#0.00"},   // 32
   {"Longitude",              "LOG",  "",               0.0f, "#0.00"},   // 33

   {"",                       "",     "",               0.0f, "#0.00"}
};



void  InstrumentData::CreateBlocker ()
{
  GoalKeeper::Create ("InstrumentData", blocker);
}

      
map<KKStr,uint32>*  InstrumentData::fieldNamesUpperIdx     = NULL;
map<KKStr,uint32>*  InstrumentData::shortFieldNamesIdx     = NULL;
GoalKeeperPtr       InstrumentData::blocker                = NULL;
bool                InstrumentData::needToRunFinalCleanUp = false;


int32  InstrumentData::numFields     = -1;
int32  InstrumentData::numDataFields = 28;

int32  InstrumentData::ctdBatteryIndex          =   0;
int32  InstrumentData::ConductivityIndex        =   1;
int32  InstrumentData::densityIndex             =   2;
int32  InstrumentData::depthIndex               =   3;
int32  InstrumentData::flowRate1Index           =   4;
int32  InstrumentData::flowRate2Index           =   5;
int32  InstrumentData::fluorescenceIndex        =   6;
int32  InstrumentData::fluorescenceSensorIndex  =   7;
int32  InstrumentData::cdomFluorescenceIndex        =   8;
int32  InstrumentData::cdomFluorescenceSensorIndex  =   9;
int32  InstrumentData::illuminationIndex        =  10; 
int32  InstrumentData::oxygenIndex              =  11;
int32  InstrumentData::oxygenSensorIndex        =  12;
int32  InstrumentData::pressureIndex            =  13;
int32  InstrumentData::recordRateIndex          =  14;
int32  InstrumentData::salinityIndex            =  15;
int32  InstrumentData::soundVelocityIndex       =  16;
int32  InstrumentData::temperatureIndex         =  17;
int32  InstrumentData::transmisivityIndex       =  18;
int32  InstrumentData::transmisivitySensorIndex =  19;
int32  InstrumentData::turbidityIndex           =  20;
int32  InstrumentData::turbiditySensorIndex     =  21;

int32  InstrumentData::pitchIndex               =  22;
int32  InstrumentData::rollIndex                =  23;

int32  InstrumentData::bat1LevelIndex           =  24;
int32  InstrumentData::bat2LevelIndex           =  25;
int32  InstrumentData::bat3LevelIndex           =  26;
int32  InstrumentData::bat4LevelIndex           =  27;

int32  InstrumentData::scanLineIndex            =  28;
int32  InstrumentData::byteOffsetIndex          =  29;
int32  InstrumentData::ctdDateIndex             =  30;
int32  InstrumentData::activeBatteryIndex       =  31;
int32  InstrumentData::latitudeIndex            =  32;
int32  InstrumentData::longitudeIndex           =  33;



const KKStr&  InstrumentData::FieldName (int32 fieldIndex)
{
  if  ((fieldIndex < 0)  ||  (fieldIndex >= NumFields ()))
    return KKStr::EmptyStr ();

  return fieldDescriptions[fieldIndex].name;
}  /* FieldName */



const KKStr&  InstrumentData::ShortFieldName (int32 fieldIndex)
{
  if  ((fieldIndex < 0)  ||  (fieldIndex >= NumFields ()))
    return KKStr::EmptyStr ();

  return fieldDescriptions[fieldIndex].shortDesc;
}  /* FieldName */



const KKStr&  InstrumentData::FieldUnitOfMeasure (int32 fieldIndex)
{
  if  ((fieldIndex < 0)  ||  (fieldIndex >= NumFields ()))
    return KKStr::EmptyStr ();

  return fieldDescriptions[fieldIndex].unitOfMeasure;
}


float  InstrumentData::FieldMinValue (int32 fieldIndex)
{
  if  ((fieldIndex < 0)  ||  (fieldIndex >= NumFields ()))
    return 0.0f;

  return fieldDescriptions[fieldIndex].minVal;
}



const KKStr&  InstrumentData::FieldFormatStr (int32 fieldIndex)
{
  if  ((fieldIndex < 0)  ||  (fieldIndex >= NumFields ()))
    return KKStr::EmptyStr ();

  return fieldDescriptions[fieldIndex].formatStr;
}



int32  InstrumentData::NumFields ()
{
  if  (numFields < 0)
  {
    numFields = 0;

    while  (!fieldDescriptions[numFields].name.Empty ())
      numFields++;
  }
  return  numFields;
}  /* NumFields */




int32  InstrumentData::GetFieldNum (const KKStr&  s)
{
  if  (fieldNamesUpperIdx == NULL)
  {
    CreateBlocker ();
    blocker->StartBlock ();
    if  (fieldNamesUpperIdx == NULL)
    {
      int32  n = NumFields ();

      fieldNamesUpperIdx = new map<KKStr,uint32> ();
      for  (int32 x = 0;  x < n;  ++x)
      {
        KKStr  upperName = fieldDescriptions[x].name.ToUpper ();
        fieldNamesUpperIdx->insert (pair<KKStr,uint32> (upperName, x));
      }
      needToRunFinalCleanUp = true;
      atexit (InstrumentData::FinalCleanUp);
    }
    blocker->EndBlock ();
  }
  
  map<KKStr,uint32>::iterator  idx;

  idx = fieldNamesUpperIdx->find (s.ToUpper ());
  if  (idx == fieldNamesUpperIdx->end ())
    return -1;
  else
    return idx->second;
}  /* GetFieldNum */





int32  InstrumentData::GetFieldNumFromShortName (const KKStr&  s)
{
  int32  n = NumFields ();

  if  (shortFieldNamesIdx == NULL)
  {
    int32  n = NumFields ();
    shortFieldNamesIdx = new map<KKStr,uint32> ();

    for  (int32 x = 0;  x < n;  x++)
    {
      KKStr  uperName = fieldDescriptions[x].shortDesc.ToUpper ();
      shortFieldNamesIdx->insert (pair<KKStr,uint32> (uperName, x));
    }
  }

  map<KKStr,uint32>::iterator  idx;

  idx = shortFieldNamesIdx->find (s.ToUpper ());
  if  (idx == shortFieldNamesIdx->end ())
    return -1;
  else
    return idx->second;
}  /* GetFieldNumFromShortName */




VectorIntPtr  InstrumentData::CreateFieldIndirectionVectorFromTabDelStr (KKStrParser& s)
{
  int32  x;
  uint32  n = NumFields ();
  VectorIntPtr  indirectionVector = new VectorInt ();

  VectorKKStr  names = s.Split ("\t");

  if  (names.size () > n)
  {
    cerr << endl << endl
       << "InstrumentData::CreateFieldIndirectionVectorFromTabDelStr   ***ERROR***   Number of fields is greater than what we allow for." << endl
       << endl;
  }

  for  (x = 0;  x < (int32)names.size ();  x++)
  {
    int32  fieldNum = GetFieldNum (names[x]);
    indirectionVector->push_back (fieldNum);
  }

  return  indirectionVector;
 }  /* CreateFieldIndirectionVectorFromTabDelStr */




VectorIntPtr  InstrumentData::CreateDefaultFieldIndirectionList ()
{
  int32  x;
  int32  n = NumFields ();
  VectorIntPtr  indirectionVector = new VectorInt ();
  for  (x = 0;  x < n;  x++)
    indirectionVector->push_back (x);
  return  indirectionVector;
}  /* CreateDefaultFieldIndirectionList */




KKStr  InstrumentData::FieldToStr (int32  fieldIndex)  const
{
  if  (fieldIndex == scanLineIndex)
    return  StrFromUint32 (scanLine);

  if  (fieldIndex == byteOffsetIndex)
    return  StrFromUint64 (byteOffset);

  if  (fieldIndex == ctdDateIndex)
  {
    KKStr  ctdDateStr = ctdDate.Date ().YYYY_MM_DD () + " " + ctdDate.Time ().HH_MM_SS ();
    return  ctdDateStr;
  }

  if  (fieldIndex == activeBatteryIndex)
    return  StrFromInt32 (activeBattery);

  if  (fieldIndex == latitudeIndex)
  {
    return  StrFormatDouble (latitude, "-#0.000000");
  }

  if  (fieldIndex == longitudeIndex)
  {
    return StrFormatDouble (longitude, "-##0.000000");
  }

  if  ((fieldIndex < ctdBatteryIndex)  ||  (fieldIndex >= numFields))
  {
    cerr << std::endl << std::endl << "InstrumentData::FieldToStr     **** ERROR ****       Invalid FieldIndex[" << fieldIndex << "]" << std::endl << std::endl;
    return  "";
  }

  char buff[50];
  SPRINTF (buff, sizeof (buff), "%g", data[fieldIndex - ctdBatteryIndex]);
  return  KKStr (buff);
}  /* FieldToStr */




void  InstrumentData::UpdateFromStr (int32         fieldIndex,
                                     const KKStr&  s
                                    )
{
  if  (fieldIndex == scanLineIndex)
  {
    scanLine = s.ToLong ();
    return;
  }

  if  (fieldIndex == byteOffsetIndex)
  {
    byteOffset = s.ToUint64 ();
    return;
  }

  if  (fieldIndex == ctdDateIndex)
  {
    ctdDate  = DateTime (s);
    return;
  }

  if  (fieldIndex == activeBatteryIndex)
  {
    activeBattery = (char)(s.ToInt ());
    return;
  }

  if  (fieldIndex == latitudeIndex)
  {
    latitude = s.ToDouble ();
    return;
  }

  if  (fieldIndex == longitudeIndex)
  {
    longitude = s.ToDouble ();
    return;
  }

  float d = s.ToFloat ();
  data[fieldIndex - ctdBatteryIndex] = d;
}  /* UpdateFromStr */





KKStr  InstrumentData::ToTabDelStr ()  const
{
  int32  x = 0;

  KKStr  ln (200);

  for  (x = 0;  x < NumFields ();  x++)
  {
    if  (x > 0)
      ln << "\t";
    ln << FieldToStr (x);
  }

  return  ln;  
}  /* ToTabDelStr */

   

void   InstrumentData::RefreshDataFields (const InstrumentData&  id)
{
  if  (!data)
    data = new float[numDataFields];

  for  (int32 x = 0;  x  < numDataFields;  x++)
    data[x] = id.data[x];

  activeBattery = id.ActiveBattery ();
  ctdDate       = id.CtdDate       ();
  latitude      = id.Latitude      ();
  longitude     = id.Longitude     ();
}  /* RefreshDataFields */




void   InstrumentData::UpdateFromCtdData (const CTD_Plus_Data&  ctdData)
{
  data[ctdBatteryIndex            ]  = ctdData.BatteryVoltage         ();
  data[ConductivityIndex          ]  = ctdData.Conductivity           ();
  data[densityIndex               ]  = ctdData.Density                ();
  data[depthIndex                 ]  = ctdData.Depth                  ();
  data[fluorescenceIndex          ]  = ctdData.Fluorescence           ();
  data[fluorescenceSensorIndex    ]  = ctdData.FluorescenceSensor     ();
  data[cdomFluorescenceIndex      ]  = ctdData.CdomFluorescence       ();
  data[cdomFluorescenceSensorIndex]  = ctdData.CdomFluorescenceSensor ();
  data[oxygenIndex                ]  = ctdData.Oxygen                 ();
  data[oxygenSensorIndex          ]  = ctdData.OxygenSensor           ();
  data[pressureIndex              ]  = ctdData.Pressure               ();
  data[salinityIndex              ]  = ctdData.Salinity               ();
  data[soundVelocityIndex         ]  = ctdData.SoundVelocity          ();
  data[temperatureIndex           ]  = ctdData.Temperature            ();
  data[transmisivityIndex         ]  = ctdData.Transmisivity          ();
  data[transmisivitySensorIndex   ]  = ctdData.TransmisivitySensor    ();
  data[turbidityIndex             ]  = ctdData.Turbidity              ();
  data[turbiditySensorIndex       ]  = ctdData.TurbiditySensor        ();
}  /* UpdateFromCtdData */



void  InstrumentData::FinalCleanUp ()
{
  if  (!needToRunFinalCleanUp)
    return;

  CreateBlocker ();
  blocker->StartBlock ();

  delete  fieldNamesUpperIdx;  fieldNamesUpperIdx = NULL;
  delete  shortFieldNamesIdx;  shortFieldNamesIdx = NULL;

  needToRunFinalCleanUp = false;

  blocker->EndBlock ();
  delete  blocker;
  blocker = NULL;
}  /* FinalCleanUp */
