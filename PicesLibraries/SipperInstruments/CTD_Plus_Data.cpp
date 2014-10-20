#include  "FirstIncludes.h"

#include  <ctype.h>
#include  <math.h>
#include  <stdio.h>

#include  <iostream>
#include  <string>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include  "BasicTypes.h"
using namespace KKU;



#include "CTD_Plus_Data.h"
using namespace SipperHardware;

//"Transmisivity",   // 17
//"Turbidicity",     // 15
//"Flourences",      // 16
//"OxygenSensor",    // 18




CTD_Plus_Data::CTD_Plus_Data (bool  _checkCDOM):

    batteryVoltage         (0.0f),
    checkCDOM              (false),
    conductivity           (0.0f),
    date                   (_checkCDOM),
    density                (0.0f),
    depthMeters            (0.0f),
    fluorescence           (0.0f),
    fluorescenceSensor     (0.0f),
    cdomFluorescence       (0.0f),
    cdomFluorescenceSensor (0.0f),
    oxygen                 (0.0f),
    oxygenSensor           (0.0f),
    pressure               (0.0f),
    salinity               (0.0f),
    soundVelocity          (0.0f),
    temperature            (0.0f),
    transmisivity          (0.0f),
    transmisivitySensor    (0.0f),
    turbidity              (0.0f),
    turbiditySensor        (0.0f)
{
}



float  ToFloat (KKStr  s,
                bool&  valid
               )
{
  s.TrimLeft ();
  s.TrimRight ();

  valid = false;

  bool  negNum = false;
  bool  decimalDetected = false;

  int32  intPart   = 0;
  int32  fractPart = 0;
  int32  fractPartDivisor = 1;
  
  int32  x = 0;
  if  ((s[x] == '-')  &&  (s.Len () > 0))
  {
    negNum = true;
    x++;
  }

  if  (x >= s.Len ())
  {
    return 0.0f;
  }

  while  (x < s.Len ())
  {
    uchar ch = s[x];  
    x++;

    if  (ch == '.')
    {
      if  (decimalDetected)
      {
        // more than one decimal place
        return 0.0f;
      }
      decimalDetected = true;
    }

    else if  ((ch < '0')  ||  (ch > '9'))
    {
      // Invalid character encountered.
      return 0.0f;

    }
   
    else if  (decimalDetected)
    {
      // We are in the fractional part of the number.
      fractPart = fractPart * 10 + (ch - '0');
      fractPartDivisor *= 10;
    }

    else
    {
      intPart = intPart * 10 + (ch - '0');
    }
  }

  float  result = (float)intPart + ((float)fractPart / (float)fractPartDivisor);

  if  (negNum)
    result = result * -1.0f;
  valid = true;

  return  result;
}  /* ToFloat */




CTD_Plus_Data::CTD_Plus_Data (const KKStr&  _txt,
                              double        _latitude,
                              bool&         valid,
                              const KKStr&  ext0CCode,
                              const KKStr&  ext1CCode,
                              const KKStr&  ext2CCode,
                              const KKStr&  ext3CCode
                             ):
    batteryVoltage         (0.0f),
    checkCDOM              (false),
    conductivity           (0.0f),
    date                   (),
    density                (0.0f),
    depthMeters            (0.0f),
    fluorescence           (0.0f),
    fluorescenceSensor     (0.0f),
    cdomFluorescence       (0.0f),
    cdomFluorescenceSensor (0.0f),
    oxygen                 (0.0f),
    oxygenSensor           (0.0f),
    pressure               (0.0f),
    salinity               (0.0f),
    soundVelocity          (0.0f),
    temperature            (0.0f),
    transmisivity          (0.0f),
    transmisivitySensor    (0.0f),
    turbidity              (0.0f),
    turbiditySensor        (0.0f)

{
  valid = true;

  int32  expNumExtVoltages = (ext0CCode.Empty () ? 0:1) + (ext1CCode.Empty () ? 0:1) + (ext2CCode.Empty () ? 0:1) + (ext3CCode.Empty () ? 0:1);

  VectorKKStr  fields = _txt.Split (",");
  int32  numExternalVoltages = (int32)fields.size () - 9;  // Ther are 9 fixed fields and then up to four external voltages.
  if  (numExternalVoltages != expNumExtVoltages)
  {
    valid = false;
    return;
  }

  int32  fieldNum = 0;

  temperature  = ToFloat (fields[fieldNum], valid);  fieldNum++;
  if  (!valid)
    return;

  conductivity = ToFloat (fields[fieldNum], valid);  fieldNum++;
  if  (!valid)
    return;

  pressure = ToFloat (fields[fieldNum], valid);  fieldNum++;
  if  (!valid)
    return;


  int32 numExtenalVoltsProcessed = 0;
  if  (numExtenalVoltsProcessed < numExternalVoltages)
  {
    UpdateExternalVoldtageField (ext0CCode, fields[fieldNum], numExtenalVoltsProcessed, fieldNum, valid);
    if  (!valid)
      return;
  }

  if  (numExtenalVoltsProcessed < numExternalVoltages)
  {
    UpdateExternalVoldtageField (ext1CCode, fields[fieldNum], numExtenalVoltsProcessed, fieldNum, valid);
    if  (!valid)
      return;
  }
    
  if  (numExtenalVoltsProcessed < numExternalVoltages)
  {
    UpdateExternalVoldtageField (ext2CCode, fields[fieldNum], numExtenalVoltsProcessed, fieldNum, valid);
    if  (!valid)
      return;
  }
    
  if  (numExtenalVoltsProcessed < numExternalVoltages)
  {
    UpdateExternalVoldtageField (ext3CCode, fields[fieldNum], numExtenalVoltsProcessed, fieldNum, valid);
    if  (!valid)
      return;
  }
    
  if  (numExtenalVoltsProcessed <  numExternalVoltages)
  {
    fieldNum += (numExternalVoltages - numExtenalVoltsProcessed);
    if  (!valid)
      return;
  }

  salinity = ToFloat (fields[fieldNum], valid);  fieldNum++;
  if  (!valid)
    return;

  soundVelocity = ToFloat (fields[fieldNum], valid);  fieldNum++;
  if  (!valid)
    return;

  DateType  d (fields[fieldNum]);    fieldNum++;
  TimeType  t (fields[fieldNum]);    fieldNum++;

  if  ((d.Year () < 1997)  ||  (d.Year () > 2030)  ||  (d.Month () > 12)  ||  (d.Day () > 31))
  {
    valid = false;
    return;
  }

  date = DateTime (d, t);

  density = ToFloat (fields[fieldNum], valid);  fieldNum++;
  if  (!valid)
    return;

  batteryVoltage = ToFloat (fields[fieldNum], valid);  fieldNum++;
  if  (!valid)
    return;

  if  ((batteryVoltage < 6.0f)  ||  (batteryVoltage > 14.9f)     ||
       (temperature    < 1.0f)  ||  (temperature    > 50.0f)     ||
       (pressure       < 0.0f)  ||  (pressure       > 10000.0f)
      )
  {
    valid = false;
    return;
  }
  else
  {
    oxygen = DissolvedOxygenConcentration (oxygenSensor, temperature, salinity, pressure);
  }
      
   
  // The folowing calc for deapth comes from:
  // SBE  Application Note No. 69,  Page 1.  July 2002
  // references UNESCO Technical Papers in Marine Science No. 44.
      
  // gravity = (m/sec^2)

 
  if  (_latitude < 1.0)
  {
    // Latitude of panama cruses first Station
    _latitude = 11.0 + (59.7 / 60.0);
  }


  double  degToRad = 1.0 / 57.29578;
     
  double  x = pow (sin (_latitude * degToRad), 2.0);
  double  gravity = 9.780318 * (1.0 + (5.2788e-3 + 2.36e-5 * x) * x) + 1.092e-6 * pressure;
      
  double p = (double)pressure;

  depthMeters  = (float)(((((-1.82e-15 * p + 2.279e-10) * p - 2.2512e-5) * p + 9.72659) * p) / gravity);


  CalcFluorescence ();
  CalcCdomFluorescence ();
  CalcTransmisivity ();
  CalcTurbidity ();
}


 //23.6075,  5.33506,    9.587, 3.1297, 0.0861, 0.9381,  36.3187, 1532.581, 07 Jan 2007, 07:43:25,  24.7565, 11.3
 //23.6074,  5.33505,    9.579, 3.1280, 0.0884, 0.7192,  36.3187, 1532.581, 07 Jan 2007, 07:43:25,  24.7565, 11.2
 //23.6072,  5.33502,    9.515, 3.1262, 0.0897, 0.6206,  36.3186, 1532.579, 07 Jan 2007, 07:43:25,  24.7565, 11.3
 //23.6071,  5.33503,    9.476, 3.1240, 0.0888, 0.4750,  36.3188, 1532.579, 07 Jan 2007, 07:43:26,  24.7566, 11.3
 //23.6074,  5.33505,    9.512, 3.1215, 0.0855, 0.3661,  36.3187, 1532.580, 07 Jan 2007, 07:43:26,  24.7565, 11.2
 //23.6073,  5.33502,    9.616, 3.1188, 0.0848, 0.3231,  36.3185, 1532.581, 07 Jan 2007, 07:43:26,  24.7563, 11.2
 //23.6074,  5.32387,    9.724, 3.1185, 0.0848, 0.3351,  36.2329, 1532.490, 07 Jan 2007, 07:43:27,  24.6914, 11.3
 //23.6076,  5.33512,    9.774, 3.1200, 0.0851, 0.2735,  36.3190, 1532.585, 07 Jan 2007, 07:43:27,  24.7566, 11.3
 //23.6078,  5.33512,    9.787, 3.1233, 0.0840, 0.2161,  36.3188, 1532.585, 07 Jan 2007, 07:43:27,  24.7565, 11.2
 

  void   CTD_Plus_Data::UpdateExternalVoldtageField (const KKStr&  extCode,
                                                     const KKStr&  amtField,
                                                     int32&        numExtenalVoltsProcessed,
                                                     int32&        fieldNum,
                                                     bool&         valid
                                                    )
  {
    if  (extCode.Empty ())
      return;

    numExtenalVoltsProcessed++;
    fieldNum++;
    float  amt = ToFloat (amtField, valid);
    if  (!valid)
      return;

    if  (extCode == "CDM")
    {
      checkCDOM = true;
      cdomFluorescenceSensor = amt;
      if  ((cdomFluorescenceSensor < -3.0) ||  (cdomFluorescenceSensor > 10))
        valid = false;
    }

    if  (extCode == "FLO")
    {
      fluorescenceSensor = amt;
      if  ((fluorescenceSensor < -3.0) ||  (fluorescenceSensor > 10))
        valid = false;
    }

    else if  (extCode == "OXG")
    {
      oxygenSensor = amt;
    }

    else if  (extCode == "TRN")
      transmisivitySensor = amt;

    else if  (extCode == "TUR")
      turbiditySensor  = amt; 
  }  /* UpdateExternalVoldtageField */




  void   CTD_Plus_Data::CalcFluorescence ()
  {
    // Using calibration sheet from WETLabs
    // dated July-16-2008
    // S/N: FLNTURTD-1112
    // Phone # 541-929-5650
    // www.wetlabs.com
    //
    //  dark Counts       = 0.081 (V)          56      counts
    //  ScaleFactor       = 10    (ug/l/V)     0.0120  ug/l/count
    //  Maximum Output    = 4.97  (V)          4119    counts
    //  Resolution        = 0.1   (mV)         0.7     counts


    // dated November-16-2010
    // S/N: FLNTURTD-1112
    // Phone # 541-929-5650
    // www.wetlabs.com
    //
    //  dark Counts       = 0.079 (V)          57      counts
    //  ScaleFactor       = 10    (ug/l/V)     0.0122  ug/l/count
    //  Maximum Output    = 4.94  (V)          4119    counts
    //  Resolution        = 1.0   (mV)         1.0     counts

    //fluorescence = (float)(10.0 * (fluorescenceSensor - 0.081));
    fluorescence = (float)(10.0 * (fluorescenceSensor - 0.079));
  }  /* CalcFluorescence */



  /**
   @brief  Initially cloned from 'CalcFluorescence'.
   */
  void   CTD_Plus_Data::CalcCdomFluorescence ()
  {
    // Received the parameters below from Andrew July-01-2013
    // Using calibration sheet from WETLabs
    // dated July-24-2008
    // S/N: FLCDRTD-1111
    // Phone # 541-929-5650
    // www.wetlabs.com
    //                      Analog              (Default)
    //                      Range1    Range2    Range4 
    //  dark Counts       = 0.083     0.053      0.038   (V)          41      counts
    //  ScaleFactor       = 24           47        96    (ug/l/V)     0.0291  ppb/count
    //  Maximum Output    = 4.96        4.96      4.96   (V)          16315   counts
    //  Resolution        = 4.2          4,2      4.2    (mV)         4.1     counts
    //  
    // As per Skype conversation with Andrew (July-1-2013) we were at Default Setting
    cdomFluorescence = (float)(96.0 * (cdomFluorescenceSensor - 0.038));
  }  /* CalcCdomFluorescence */


  void   CTD_Plus_Data::CalcTurbidity ()
  {
    // KAK 2013-07-01  
    //There are two sets of calibrartions below,  The 1st one is what we have been using;  probably
    // up to the 1st BP cruise,  but th esecond set are the ones we should be using as of the BP
    // oil spill cruises.

    // Using calibration sheet from WETLabs
    // dated July-16-2008
    // S/N: FLNTURTD-1112
    // Phone # 541-929-5650
    // www.wetlabs.com
    //
    //  dark Counts        = 0.075 (V)          50      counts
    //  NTU Solution Value = 2.41  (V)          1976    counts
    //  ScaleFactor        = 5     (NTU/V)      0.0060  NTU/count
    //  Maximum Output     = 4.97  (V)          4119    counts
    //  Resolution         = 0.1   (mV)         0.6     counts
    // NTU = ScaleFactor (5 NTU/V) * (turbiditySensor - DarkCounts(0.075 V) )


    // KAK 2013-07-01  Received the below calibratin parameters from andrew to be used for BP cruises.
    // Using calibration sheet from WETLabs
    // dated November-16-2010
    // S/N: FLNTURTD-1112
    // Phone # 541-929-5650
    // www.wetlabs.com
    //
    //  dark Counts        = 0.073 (V)          50      counts
    //  NTU Solution Value = 2.94  (V)          2427    counts
    //  ScaleFactor        = 5     (NTU/V)      0.0061  NTU/count
    //  Maximum Output     = 4.93  (V)          4119    counts
    //  Resolution         = 0.8   (mV)         1.0     counts

    //turbidity = (float)(2.41 * (turbiditySensor - 0.075));
    turbidity = (float)(2.94 * (turbiditySensor - 0.073));
  }  /* CalcTurbidity */



  void   CTD_Plus_Data::CalcTransmisivity ()
  {
    // Using calibration sheet from WETLabs
    // dated July-16-2008
    // S/N: FLNTURTD-1112
    // Phone # 541-929-5650
    // www.wetlabs.com
    //
    //  Vdark = 0.061V
    //  Vair  = 4.815V
    //  Vref  = 4.466V
    //
    // Tr = (Vsig - Vdark) / (Vref - Vdark)

    transmisivity = (float)(((double)transmisivitySensor - 0.061) / (4.466 - 0.061));
  }  /* CalcTurbidity */



    
  float  OxygenSaturation (float waterTemp,   // Celcius
                           float salinity     // psu
                          )
  {
    // From page Appendix A, page 6of SBE 43  Dissolved Oxygen Sensor
    // Application Note No. 64.


    static  double  A1 = -173.4292;
    static  double  A2 =  249.6339;
    static  double  A3 =  143.3483;
    static  double  A4 =  -21.8429;
    static  double  B1 =   -0.033096;
    static  double  B2 =    0.014259;
    static  double  B3 =   -0.00170;

    static  float   lastWaterTemp = -9999.99f;
    static  float   lastSalinity  = -9999.99f;
    static  double  lastOxSat     = -9999.99;
    
    
    if  ((lastWaterTemp == waterTemp)  &&  (lastSalinity == salinity))
        return (float)lastOxSat;

    double S = salinity;

    double Ta = 273.15 + (double)waterTemp;

    double Ta100 = Ta / 100.0;

    double  A = A1 + A2 * (100.0 / Ta) + A3 * log (Ta100) + A4 * Ta100;
    double  B = B1 + B2 * Ta100 + B3 * Ta100 * Ta100;

    lastOxSat = exp (A + S * B);

    return  (float)lastOxSat;

  }  /* OxygenSaturation */
    
    


float  CTD_Plus_Data::DissolvedOxygenConcentration (float  voltage,
                                                    float  temperature,
                                                    float  salinity,
                                                    float  pressure
                                                   )
{
  static double  Soc     =  0.3634;
  static double  Boc     =  0.0000;
  static double  Voffset = -0.5098;
  static double  tcor    =  0.0016;
  static double  pcor    =  1.350e-04;
  static double  tau     =  0.0; 
  static double  deltaV  =  0.0;
    
  double  P = pressure;
  double  T = temperature;
  double  V = voltage;

  double  disolvedOxygen_a = (Soc * (V + Voffset + tau * deltaV) + Boc * exp (-0.03 * T));

  double  Oxsat = OxygenSaturation (temperature, salinity);
  double  disolvedOxygen_b = Oxsat * exp (tcor * T) * exp (pcor * P);

  double  dissolvedOxygenConcentration = disolvedOxygen_a * disolvedOxygen_b;

  return  (float)dissolvedOxygenConcentration;
}  /* DissolvedOxygenConcentration */





CTD_Plus_Data::~CTD_Plus_Data()
{
}



CTD_Plus_Data&  CTD_Plus_Data::operator=  (const CTD_Plus_Data&  right)
{
  batteryVoltage         = right.batteryVoltage;
  conductivity           = right.conductivity;
  date                   = right.date;
  density                = right.density;
  depthMeters            = right.depthMeters;
  fluorescence           = right.fluorescence;
  fluorescenceSensor     = right.fluorescenceSensor;
  cdomFluorescence       = right.cdomFluorescence;
  cdomFluorescenceSensor = right.cdomFluorescenceSensor;
  oxygen                 = right.oxygen;
  oxygenSensor           = right.oxygenSensor;
  pressure               = right.pressure;
  salinity               = right.salinity;
  soundVelocity          = right.soundVelocity;
  temperature            = right.temperature;
  transmisivity          = right.transmisivity;
  transmisivitySensor    = right.transmisivitySensor;
  turbidity              = right.turbidity;
  turbiditySensor        = right.turbiditySensor;

  return  *this;
}  /* operator= */







//==============================================================================================
//    Date     Time  Temp SpCond    Sal DOsat     DO   Press   Depth    pH Turbid+   Chl Battery
//   m/d/y hh:mm:ss     C  mS/cm    ppt     %   mg/L    psia  meters           NTU  ug/L   volts
//----------------------------------------------------------------------------------------------
//*** 1-LOG last sample   2-LOG ON/OFF, 3-Clean optics ***
//06/16/06 12:45:35 28.39  53.23  35.03  98.6   6.31  15.102   0.220  8.15     1.2  56.2    10.6
//06/16/06 12:45:36 28.39  53.23  35.03  98.6   6.31  15.097   0.217  8.15     1.3  56.4    10.5
//06/16/06 12:45:36 28.39  53.24  35.03  98.6   6.31  15.100   0.219  8.15     1.4  63.8    10.5
//06/16/06 12:45:37 28.39  53.24  35.03  98.6   6.31  15.098   0.217  8.15     1.5  67.9    10.5


KKStr  CTD_Plus_Data::Header1Str ()
{
  KKStr  result;
  result << "    "           << "\t"
         << "Battery"        << "\t"
         << "Conductivity"   << "\t"
         << "Density"        << "\t"
         << "Depth"          << "\t"
         << "Flourences"     << "\t"
         << "Flourences"     << "\t"
         << "Cdom"           << "\t"
         << "Cdom"           << "\t"
         << "Oxygen"         << "\t"
         << "OxygenSensor"   << "\t"
         << "Pressure"       << "\t"
         << "Salinity"       << "\t"
         << "Sound"          << "\t"
         << "Temperature"    << "\t"
         << "Transmisivity"  << "\t"
         << "Transmisivity"  << "\t"
         << "Turbidicity"    << "\t"
         << "Turbidicity";

  return  result;
}  /* Header */




KKStr  CTD_Plus_Data::Header2Str ()
{
  KKStr  result;
  result << "Date"     << "\t"
         << "Voltage"  << "\t"
         << "mS/cm"    << "\t"   // Conductivity
         << "%"        << "\t"   // Density
         << "meters"   << "\t"   // Depth
         << "Units"    << "\t"   // Flourences
         << "Volts"    << "\t"   // Flourences
         << "Units"    << "\t"   // CdomFlourences
         << "Volts"    << "\t"   // CdomFlourences
         << "ml/l"     << "\t"   // Oxygen
         << "Volts"    << "\t"   // Oxygen
         << "psi"      << "\t"   // Pressure
         << "ppt"      << "\t"   // Salinity
         << "m/s"      << "\t"   // SoundVelocity
         << "c"        << "\t"   // Temperature
         << "Units"    << "\t"   // Transmisivity
         << "Volts"    << "\t"   // Transmisivity
         << "Units"    << "\t"   // Turbidicity
         << "Volts";

  return  result;
}  /* Header */



KKStr  CTD_Plus_Data::ToStrNoLabels ()  const
{
  KKStr  result (100);

  result << date                                                << "\t"
         << StrFormatDouble (batteryVoltage,         "##0.000") << "\t"
         << StrFormatDouble (conductivity,           "##0.00")  << "\t"
         << StrFormatDouble (density,                "##0.00")  << "\t"
         << StrFormatDouble (depthMeters,            "##0.00")  << "\t"
         << StrFormatDouble (fluorescence,           "##0.000") << "\t"
         << StrFormatDouble (fluorescenceSensor,     "##0.000") << "\t"
         << StrFormatDouble (cdomFluorescence,       "##0.000") << "\t"
         << StrFormatDouble (cdomFluorescenceSensor, "##0.000") << "\t"
         << StrFormatDouble (oxygenSensor,           "##0.000") << "\t"
         << StrFormatDouble (oxygen,                 "##0.000") << "\t"
         << StrFormatDouble (pressure,               "##0.00")  << "\t"
         << StrFormatDouble (salinity,               "##0.00")  << "\t"
         << StrFormatDouble (soundVelocity,          "##0.0")   << "\t"
         << StrFormatDouble (temperature,            "##0.00")  << "\t"
         << StrFormatDouble (transmisivity,          "##0.000") << "\t"
         << StrFormatDouble (transmisivitySensor,    "##0.000") << "\t"
         << StrFormatDouble (turbidity,              "##0.000") << "\t"
         << StrFormatDouble (turbiditySensor,        "##0.000");

  return  result;
}  /* ToStrNoLabels */



bool  CTD_Plus_Data::ValidData ()  const
{
  if  (date.Year () < 2005)
    return  false;

  if  ((temperature <= 0.0f)  ||  (temperature > 40.0f))
    return  false;

  if  ((depthMeters < 0.0)  ||  (depthMeters > 1000.0f))
    return  false;

  if  ((salinity < -2.0f)  ||  (salinity > 40.0f))
    return  false;

  // 2010-11-17 
  // Andrew was having problem with a lot of instruiment data data not updating for ETP2007-1-E.
  // Seems that teh Florencses was out of bounds.  Wil change to let it go through.
  //if  ((fluorescence < -2.0f)  ||  (fluorescence >= 7.0f))
  //  return  false;
  if  (fluorescence < -2.0f)
    return  false;

  //if  ((cdomFluorescence < -2.0f)  ||  (cdomFluorescence >= 7.0f))
  //  return  false;

  if  ((oxygen < -3.0f)  ||  (oxygen >= 10.0f))
    return  false;

  return  true;
}  /* ValidData */
