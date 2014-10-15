#ifndef  _CTD_PLUS_DATA_
#define  _CTD_PLUS_DATA_

#include  "DateTime.h"
#include  "Str.h"
using  namespace  KKU;


namespace SipperHardware
{
  class CTD_Plus_Data
  {
  public:
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;
    typedef  KKU::int64   int64;
    typedef  KKU::uint64  uint64;

    CTD_Plus_Data (bool  _checkCDOM);
   
    CTD_Plus_Data (const KKStr&   _txt,
                   double         _latitude,
                   bool&          valid,
                   const KKStr&   ext0CCode,
                   const KKStr&   ext1CCode,
                   const KKStr&   ext2CCode,
                   const KKStr&   ext3CCode
                  );
  
    ~CTD_Plus_Data ();
  
  
    float            BatteryVoltage ()  const  {return  batteryVoltage;}
    float            Conductivity   ()  const  {return  conductivity;}
    const 
      KKU::DateTime& Date                   ()  const  {return  date;}
    float            Density                ()  const  {return  density;}
    float            Depth                  ()  const  {return  depthMeters;}
    float            Fluorescence           ()  const  {return  fluorescence;}
    float            FluorescenceSensor     ()  const  {return  fluorescenceSensor;}
    float            CdomFluorescence       ()  const  {return  cdomFluorescence;}
    float            CdomFluorescenceSensor ()  const  {return  cdomFluorescenceSensor;}
    float            Oxygen                 ()  const  {return  oxygen;}
    float            OxygenSensor           ()  const  {return  oxygenSensor;}
    float            Pressure               ()  const  {return  pressure;}
    float            Salinity               ()  const  {return  salinity;}
    float            SoundVelocity          ()  const  {return  soundVelocity;}
    float            Temperature            ()  const  {return  temperature;}
    float            Transmisivity          ()  const  {return  transmisivity;}
    float            TransmisivitySensor    ()  const  {return  transmisivitySensor;}
    float            Turbidity              ()  const  {return  turbidity;}
    float            TurbiditySensor        ()  const  {return  turbiditySensor;}
  
    static
    KKStr  Header1Str ();
  
    static
    KKStr  Header2Str ();
  
    KKStr  ToStrNoLabels ()  const;

    bool   ValidData ()  const;   /*!< Returns 'true'  if the CTD data fields appear to be valid. */
  
    CTD_Plus_Data&  operator=  (const CTD_Plus_Data&  right);
  
  private:
    void  CalcFluorescence ();
    void  CalcCdomFluorescence ();
    void  CalcTransmisivity ();
    void  CalcTurbidity ();
  
    void  UpdateExternalVoldtageField (const KKStr&  extCode,
                                       const KKStr&  amtField,
                                       int32&        numExtenalVoltsProcessed,
                                       int32&        fieldNum,
                                       bool&         valid
                                      );
  
    float           batteryVoltage;
    bool            checkCDOM;    /**< Indicates if CDOM is one of the external instruments. */
    float           conductivity;
    KKU::DateTime   date;
    float           density;
    float           depthMeters;
    float           fluorescence;
    float           fluorescenceSensor;
    float           cdomFluorescence;
    float           cdomFluorescenceSensor;
    float           oxygen;
    float           oxygenSensor;
    float           pressure;        // psia
    float           salinity;
    float           soundVelocity;
    float           temperature;
    float           transmisivity;
    float           transmisivitySensor;
    float           turbidity;
    float           turbiditySensor;

  
  //"Transmisivity",   // 17
  //"Turbidity",     // 15
  //"Flourences",      // 16
  //"OxygenSensor",    // 18
  
  
  
    float  DissolvedOxygenConcentration (float  voltage,
                                         float  temperature,
                                         float  salinity,
                                         float  pressure
                                        );
  
  
    
  };
}  /* SipperHardware */  
  
#endif
