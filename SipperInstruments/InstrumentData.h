#if  !defined(_INSTRUMENTDATA_)
#define  _INSTRUMENTDATA_

#include <map>

#include "BasicTypes.h"
#include "DateTime.h"
#include "GoalKeeper.h"
#include "KKStrParser.h"
using namespace KKU;

#include "CTD_Plus_Data.h"


namespace SipperHardware
{
  class InstrumentData
  {
  public:
    typedef  KKU::int32  int32;
    typedef  KKU::uint32 uint32;
    typedef  KKU::int64  int64;
    typedef  KKU::uint64 uint64;


    InstrumentData ();
  
    InstrumentData (const InstrumentData&   id);

    InstrumentData (      KKStrParser&  line, 
                    const VectorInt&    fieldIndirections
                   );

    InstrumentData (uint32                _scanLine,
                    uint32                _byteOffset,
                    const KKU::DateTime&  _ctdDate,
                    char                  _activeBattery,
                    double                _latitude,
                    double                _longitude,
                    float                 _ctdBattery,
                    float                 _conductivity,
                    float                 _density,
                    float                 _depth,
                    float                 _flowRate1,
                    float                 _flowRate2,
                    float                 _fluorescence,
                    float                 _fluorescenceSensor,
                    float                 _cdomFluorescence,
                    float                 _cdomFluorescenceSensor,
                    float                 _illumination,
                    float                 _oxygen,
                    float                 _oxygenSensor,
                    float                 _pressure,
                    float                 _recordRate,
                    float                 _salinity,
                    float                 _soundVelocity,
                    float                 _temperature,
                    float                 _transmisivity,
                    float                 _transmisivitySensor,
                    float                 _turbidity,
                    float                 _turbiditySensor,
                    float                 _pitch,
                    float                 _roll,
                    float                 _bat1Level,
                    float                 _bat2Level,
                    float                 _bat3Level,
                    float                 _bat4Level
                   );

    ~InstrumentData ();

    static  VectorIntPtr  CreateFieldIndirectionVectorFromTabDelStr (KKStrParser& s);
    static  VectorIntPtr  CreateDefaultFieldIndirectionList ();
    static  const KKStr&  FieldName                (int32 fieldIndex);
    static  const KKStr&  ShortFieldName           (int32 fieldIndex);
    static  const KKStr&  FieldUnitOfMeasure       (int32 fieldIndex);
    static  float         FieldMinValue            (int32 fieldIndex);
    static  const KKStr&  FieldFormatStr           (int32 fieldIndex);

    static  int32         GetFieldNum              (const KKStr& s);
    static  int32         GetFieldNumFromShortName (const KKStr&  s);
    static  int32         NumFields   ();



    static  int32   CurrentFileVersion ()  {return 13;}

    KKStr   FieldToStr (int32  fieldIndex)  const;  // 'fieldIndex' = Same number returned by 'GetFieldNum'

    KKU::uint64  ByteOffset  ()  const  {return  byteOffset;}

    const KKU::DateTime&  CtdDate ()  const  {return  ctdDate;}
 
    int32   ActiveBattery         ()  const  {return  activeBattery;                 }
    float   Bat1Level             ()  const  {return  data[bat1LevelIndex          ];}
    float   Bat2Level             ()  const  {return  data[bat2LevelIndex          ];}
    float   Bat3Level             ()  const  {return  data[bat3LevelIndex          ];}
    float   Bat4Level             ()  const  {return  data[bat4LevelIndex          ];}
    float   CTDBattery            ()  const  {return  data[ctdBatteryIndex         ];}
    float   Conductivity          ()  const  {return  data[ConductivityIndex       ];}
    float   Depth                 ()  const  {return  data[depthIndex              ];}
    float   Density               ()  const  {return  data[densityIndex            ];}
    float   FlowRate1             ()  const  {return  data[flowRate1Index          ];}
    float   FlowRate2             ()  const  {return  data[flowRate2Index          ];}
    float   Fluorescence          ()  const  {return  data[fluorescenceIndex       ];}
    float   FluorescenceSensor    ()  const  {return  data[fluorescenceSensorIndex ];}
    float   CdomFluorescence      ()  const  {return  data[cdomFluorescenceIndex   ];}
    float   CdomFluorescenceSensor()  const  {return  data[cdomFluorescenceSensorIndex ];}
    float   Illumination          ()  const  {return  data[illuminationIndex       ];}
    float   Oxygen                ()  const  {return  data[oxygenIndex             ];}
    float   OxygenSensor          ()  const  {return  data[oxygenSensorIndex       ];}
    double  Latitude              ()  const  {return  latitude;                      }
    double  Longitude             ()  const  {return  longitude;                     }
    float   Pressure              ()  const  {return  data[pressureIndex           ];}
    float   RecordRate            ()  const  {return  data[recordRateIndex         ];}
    float   Salinity              ()  const  {return  data[salinityIndex           ];}
    uint32  ScanLine              ()  const  {return  scanLine;                      }
    float   SoundVelocity         ()  const  {return  data[soundVelocityIndex      ];}
    float   Temperature           ()  const  {return  data[temperatureIndex        ];}
    float   Transmisivity         ()  const  {return  data[transmisivityIndex      ];}
    float   TransmisivitySensor   ()  const  {return  data[transmisivitySensorIndex];}
    float   Turbidity             ()  const  {return  data[turbidityIndex          ];}
    float   TurbiditySensor       ()  const  {return  data[turbiditySensorIndex    ];}

    float   Pitch                 ()  const  {return  data[pitchIndex              ];}
    float   Roll                  ()  const  {return  data[rollIndex               ];}

    static  KKStr  DensityUnit          ()  {return  FieldUnitOfMeasure (densityIndex);}
    static  KKStr  FluorescenceUnit     ()  {return  FieldUnitOfMeasure (fluorescenceIndex);}
    static  KKStr  OxygenUnit           ()  {return  FieldUnitOfMeasure (oxygenIndex);}
    static  KKStr  SalinityUnit         ()  {return  FieldUnitOfMeasure (salinityIndex);}
    static  KKStr  TemperatureUnit      ()  {return  FieldUnitOfMeasure (temperatureIndex);}
    static  KKStr  TransmisivityUnit    ()  {return  FieldUnitOfMeasure (transmisivityIndex);}
    static  KKStr  TurbidityUnit        ()  {return  FieldUnitOfMeasure (turbidityIndex);}
    static  KKStr  CdomFluorescenceUnit ()  {return  FieldUnitOfMeasure (cdomFluorescenceIndex);}

    KKStr   ToTabDelStr   ()  const;


    void   CtdDate  (const KKU::DateTime&  _ctdDate)  {ctdDate = _ctdDate;}

    void   ByteOffset    (uint64 _byteOffset)  {byteOffset = _byteOffset;}
    void   Latitude      (double _latitude)    {latitude   = _latitude;}
    void   Longitude     (double _longitude)   {longitude  = _longitude;}
    void   ScanLine      (uint32 _scanLine)    {scanLine   = _scanLine;}


    void   CTDBattery             (float _ctdBattery)             {data [ctdBatteryIndex            ] = _ctdBattery;}
    void   Conductivity           (float _conductivity)           {data [ConductivityIndex          ] = _conductivity;}
    void   Depth                  (float _depth)                  {data [depthIndex                 ] = _depth;}
    void   Density                (float _density)                {data [densityIndex               ] = _density;}
    void   FlowRate1              (float _flowRate1)              {data [flowRate1Index             ] = _flowRate1;}
    void   FlowRate2              (float _flowRate2)              {data [flowRate2Index             ] = _flowRate2;}
    void   Fluorescence           (float _fluorescence)           {data [fluorescenceIndex          ] = _fluorescence;}
    void   FluorescenceSensor     (float _fluorescenceSensor)     {data [fluorescenceSensorIndex    ] = _fluorescenceSensor;}
    void   CdomFluorescence       (float _cdomFluorescence)       {data [cdomFluorescenceIndex      ] = _cdomFluorescence;}
    void   CdomFluorescenceSensor (float _cdomFluorescenceSensor) {data [cdomFluorescenceSensorIndex] = _cdomFluorescenceSensor;}
    void   OxygenSensor           (float _oxygenSensor)           {data [oxygenSensorIndex          ] = _oxygenSensor;}
    void   Oxygen                 (float _oxygen)                 {data [oxygenIndex                ] = _oxygen;}
    void   Pressure               (float _pressure)               {data [pressureIndex              ] = _pressure;}
    void   RecordRate             (float _recordRate)             {data [recordRateIndex            ] = _recordRate;}
    void   Salinity               (float _salinity)               {data [salinityIndex              ] = _salinity;}
    void   SoundVelocity          (float _soundVelocity)          {data [soundVelocityIndex         ] = _soundVelocity;}
    void   Temperature            (float _temperature)            {data [temperatureIndex           ] = _temperature;}
    void   Transmisivity          (float _transmisivity)          {data [transmisivityIndex         ] = _transmisivity;}
    void   TransmisivitySensor    (float _transmisivitySensor)    {data [transmisivitySensorIndex   ] = _transmisivitySensor;}
    void   Turbidity              (float _turbidity)              {data [turbidityIndex             ] = _turbidity;}
    void   TurbiditySensor        (float _turbiditySensor)        {data [turbiditySensorIndex       ] = _turbiditySensor;}


    void   Pitch                  (float _pitch)                  {data [pitchIndex              ] = _pitch;}
    void   Roll                   (float _roll)                   {data [rollIndex               ] = _roll;}

    /*!
     /brief  refreshes data fields from another 'InstrumentData' instance.
     */
    void   RefreshDataFields (const  InstrumentData&  id);

    void   UpdateFromCtdData (const CTD_Plus_Data&  ctdData);

    static  void  FinalCleanUp ();


  private:
    class  FieldDesc
    {
    public:
      KKStr  name;
      KKStr  shortDesc;
      KKStr  unitOfMeasure;
      float  minVal;
      KKStr  formatStr;
    };

    static  FieldDesc  fieldDescriptions[];
  
    void    InitializeDataVariables ();

    void    ParseTabDelString (const KKStr& s);

    void    UpdateFromStr (int32         fieldIndex,
                           const KKStr&  s
                          );
  
    //static const char*   fieldNames[][3];

    static void  CreateBlocker ();

    static bool  needToRunFinalCleanUp;

    static GoalKeeperPtr       blocker;
    static map<KKStr,uint32>*  fieldNamesUpperIdx;
    static map<KKStr,uint32>*  shortFieldNamesIdx;

    static int32         numFields;
    static int32         numDataFields;    /**< The first fields will be float fields that will be in a array.
                                            * the rest will have there own individule fields.
                                            */

    static int32  scanLineIndex;
    static int32  byteOffsetIndex;
    static int32  ctdDateIndex;;
    static int32  activeBatteryIndex;
    static int32  latitudeIndex;
    static int32  longitudeIndex;

    static int32  ctdBatteryIndex;    // This is the first field that will be stored in the array 'data'
    static int32  ConductivityIndex;
    static int32  densityIndex;
    static int32  depthIndex;
    static int32  flowRate1Index;
    static int32  flowRate2Index;
    static int32  fluorescenceIndex;
    static int32  fluorescenceSensorIndex;
    static int32  cdomFluorescenceIndex;
    static int32  cdomFluorescenceSensorIndex;
    static int32  illuminationIndex;
    static int32  oxygenIndex;
    static int32  oxygenSensorIndex;
    static int32  pressureIndex;
    static int32  recordRateIndex;
    static int32  salinityIndex;
    static int32  soundVelocityIndex;
    static int32  temperatureIndex;
    static int32  transmisivityIndex;
    static int32  transmisivitySensorIndex;
    static int32  turbidityIndex;
    static int32  turbiditySensorIndex;
    static int32  pitchIndex;
    static int32  rollIndex;
    static int32  bat1LevelIndex;
    static int32  bat2LevelIndex;
    static int32  bat3LevelIndex;
    static int32  bat4LevelIndex;
  
  
    char             activeBattery;
    uint64           byteOffset;
    KKU::DateTime    ctdDate;
    float*           data;
    double           latitude;
    double           longitude;
    uint32           scanLine;
  };  /* InstrumentData */
  
  
  typedef  InstrumentData*  InstrumentDataPtr;
}  /* SipperHardware */
  
  
#endif

