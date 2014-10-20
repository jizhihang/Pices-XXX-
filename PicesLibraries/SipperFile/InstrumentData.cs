using   System;
using   System.IO;
using   System.Collections;

using  PicesInterface;


namespace SipperFile
{
  /// <summary>
  /// Summary description for DataRow.
  /// </summary>
  public class InstrumentData
  {
    static  string[] fieldNames
      = {"CTD_Battery",            //  0
         "Conductivity",           //  1
         "Density",                //  2
         "Depth",                  //  3
         "FlowRate1",              //  4
         "FlowRate2",              //  5
         "Fluorescence",           //  6
         "FluorescenceSensor",     //  7
         "CdomFluorescence",       //  8
         "CdomFluorescenceSensor", //  9
         "Illumination",           // 10
         "Oxygen",                 // 11
         "OxygenSensor",           // 12
         "Pressure",               // 13
         "Record Rate",            // 14
         "Salinity",               // 15
         "SoundVelocity",          // 16
         "Temperature",            // 17

         "Transmisivity",          // 18
         "TransmisivitySensor",    // 19
         "Turbidity",              // 20
         "TurbiditySensor",        // 21

         "Pitch",                  // 22
         "Roll",                   // 23

         "Bat1Level",              // 24
         "Bat2Level",              // 25
         "Bat3Level",              // 26
         "Bat4Level",              // 27
         
         "ScanLine",               // 28
         "ByteOffset",             // 29        
         "CTD_Date",               // 30
         "ActiveBattery",          // 31
         "Latitude",               // 32
         "Longitude",              // 33
         "BatteryStatus",          // 34
         "Date"                    // 35
        };




        
    static  int  numDataFields = 28;
    
    public static  int     NumDataFields ()  {return  numDataFields;}

    static public  int     NumOfFields   ()  {return  fieldNames.Length;}
    
    static public  string  FieldName (int idx) 
    {
      if  ((idx < 0)  ||  (idx >= fieldNames.Length))
        return "";
      return fieldNames[idx];  
    }


    // Indexes used to find correct field in 'data'.
    private const  int CTDBatteryIndex             =  0;
    private const  int ConductivityIndex           =  1;
    private const  int DensityIndex                =  2;
    private const  int DepthIndex                  =  3;

    private const  int FlowRate1Index              =  4;
    private const  int FlowRate2Index              =  5;
    private const  int FluorescenceIndex           =  6;
    private const  int FluorescenceSensorIndex     =  7;
    private const  int CdomFluorescenceIndex       =  8;
    private const  int CdomFluorescenceSensorIndex =  9;

    private const  int IlluminationIndex           = 10;
    private const  int OxygenIndex                 = 11;
    private const  int OxygenSensorIndex           = 12;
    private const  int PressureIndex               = 13;
    private const  int RecordingRateIndex          = 14;
    private const  int SalinityIndex               = 15;
    private const  int SoundVelocityIndex          = 16;
    private const  int TemperatureIndex            = 17;
    private const  int TransmisivityIndex          = 18;
    private const  int TransmisivitySensorIndex    = 19;
    private const  int TurbidityIndex              = 20;
    private const  int TurbiditySensorIndex        = 21;
    public  const  int PitchIndex                  = 22;
    public  const  int RollIndex                   = 23;

    private const  int Bat1LevelIndex              = 24;
    private const  int Bat2LevelIndex              = 25;
    private const  int Bat3LevelIndex              = 26;
    private const  int Bat4LevelIndex              = 27;
    
    // Indexes of fields that are not in 'data'
    private const  int  ScanLineIndex              = 28;
    private const  int  ByteOffseIndex             = 29;
    private const  int  CTD_DateIndex              = 30;
    private const  int  ActiveBatteryIndex         = 31;
    private const  int  LatitudeIndex              = 32;
    private const  int  LongitudeIndex             = 33;
    private const  int  BatteryStatusesIndex       = 34;
    private const  int  DateIndex                  = 35;

    private const  int  UnKnownIndex               = 36;

    static string[] fieldNamesLower = null;




    static public  int[]  CreateCurrentFieldIndexList ()
    {
      int[] fieldIndexes = new int[fieldNames.Length];
      for (int x = 0; x < fieldNames.Length; x++)
        fieldIndexes[x] = x;
        
      return fieldIndexes;
    }  /* CreateCurrentFieldIndexList */




    static public   int  GetFieldNameIndex (string  fieldName)
    {
      if  (fieldNamesLower == null)
      {
        int  x;
        fieldNamesLower = new string[fieldNames.Length];
        for  (x = 0;  x < fieldNames.Length;  x++)
          fieldNamesLower[x] = fieldNames[x].ToLower ();
      }

      string fieldNameLower = fieldName.ToLower ();

      int  idx = 0;
      while  (idx < fieldNamesLower.Length)
      {
        if  (fieldNameLower == fieldNamesLower[idx])
          return  idx;
        idx++;
      }

      return -1;
    }  /* GetFieldNameIndex */




    static  public  void  WriteFieldList (StreamWriter  w)
    {
      int  x;
      w.WriteLine ("FieldsNameList");
      for  (x = 0;  x < fieldNames.Length;  x++)
      {
        if  (x > 0)
          w.Write ("\t");
        w.Write (fieldNames[x]);
      }
      w.WriteLine ("");
    }  /* WriteFieldList */




    static  public  int[]  BuildFieldNameList (string[]  fields)
    {
      ArrayList indexes = new ArrayList ();
      int    x = 0;
      
      if  (fields[0].ToLower () == "fieldsnamelist")
      {
        // Skip this field.
        x++;
      }
      
      while  (x < fields.Length)
      {
        int  idx = GetFieldNameIndex (fields[x]);
        if  (idx < 0)
          idx = UnKnownIndex;
        indexes.Add (idx);
        x++;
      }

      int[] fieldIndexes = new int[indexes.Count];
      for  (x = 0;  x < indexes.Count;  x++)
      {
        fieldIndexes[x] = (int)indexes[x];
      }
      
      return  fieldIndexes;

    }  /* BuildFieldNameList */


    private String    batteryStatuses = "";
    private DateTime  ctdDate         = DateTime.Now;
    private float[]   data            = null;  // Will consist of 13 fields (0-12) as described below.
    private double    latitude        = 0.0;
    private double    longitude       = 0.0;
    private int       activeBattery   = 1;
    private DateTime  time            = DateTime.Now;
    bool              updatedFlag     = false;
    int               scanLine        = 0;
    System.UInt64     byteOffset      = 0;

    public int        ActiveBattery          () {return activeBattery;                  }
    public string     BatteryStatuses        () {return batteryStatuses;                }
    public DateTime   CTDdate                () {return ctdDate;                        }
    public float[]    Data                   () {return data;                           }
    public float      Density                () {return data[DensityIndex            ]; }
    public float      Depth                  () {return data[DepthIndex              ]; }
    public float      Fluorescence           () {return data[FluorescenceIndex         ]; }
    public float      FluorescenceSensor     () {return data[FluorescenceSensorIndex   ]; }
    public float      CdomFluorescence       () {return data[CdomFluorescenceIndex         ]; }
    public float      CdomFluorescenceSensor () {return data[CdomFluorescenceSensorIndex   ]; }
    public double     Latitude               () {return latitude;                       }
    public double     Longitude              () {return longitude;                      }
    public float      Oxygen                 () {return data[OxygenIndex             ]; }
    public float      OxygenSensor           () {return data[OxygenSensorIndex       ]; }
    public float      Pitch                  () {return data[PitchIndex              ]; }
    public float      Roll                   () {return data[RollIndex               ]; }
    public float      Salinity               () {return data[SalinityIndex           ]; }
    public float      Temperature            () {return data[TemperatureIndex        ]; }
    public float      Transmissivity         () {return data[TransmisivityIndex      ]; }
    public float      TransmissivitySensor   () {return data[TransmisivitySensorIndex]; }
    public float      Turbidity              () {return data[TurbidityIndex          ]; }
    public float      TurbiditySensor        () {return data[TurbiditySensorIndex    ]; }

    


    public UInt64  ByteOffset
    {
      get  {return  byteOffset;}
      set  {byteOffset = value;}
    }

    public float  FlowRate1
    {
      get  {return  data[FlowRate1Index];}
      set  {data[FlowRate1Index] = value;}
    }

    public float FlowRate2
    {
      get  {return  data[FlowRate2Index]; }
      set  {data[FlowRate2Index] = value; }
    }

   
    public float Illumination
    {
      get  {return  data[IlluminationIndex];}
      set  {data[IlluminationIndex] = value;}
    }


    public float RecordingRate 
    {
      get  {return data[RecordingRateIndex];}
      set  {data[RecordingRateIndex] = value;}
    }

    
    public int  ScanLine
    {
      get {return  scanLine;}
      set {scanLine = value;}
    }

    
    public DateTime  Time
    {
      get {return time; }
      set {time = value;}
    }



    public bool  UpdatedFlag
    {
      get {return  updatedFlag; }
      set {updatedFlag = value;}
    }

    

    public  InstrumentData ()
    {
      data = new float[numDataFields];
    }



    public InstrumentData 
      (float[]  _data,
       double   _latitude,
       double   _longitude,
       DateTime _time
      )
    {
      int  x;
      data = new float[_data.Length];
      for  (x = 0;  x < _data.Length;  x++)
        data[x] = _data[x];

      latitude    = _latitude;
      longitude   = _longitude;
      time        = _time;
      updatedFlag = true;
      scanLine    = 0;
      byteOffset  = 0;

      batteryStatuses = "";
      ctdDate = new DateTime (1, 1, 1, 0, 0, 0);
    }



    public InstrumentData (InstrumentData  r)
    {
      int  x;
      data = new float[r.data.Length];
      for  (x = 0;  x < r.data.Length;  x++)
        data[x] = r.data[x];

      activeBattery   = r.activeBattery;
      byteOffset      = r.byteOffset;
      latitude        = r.latitude;
      longitude       = r.longitude;
      scanLine        = r.scanLine;
      time            = r.time;
      updatedFlag     = r.updatedFlag;
      batteryStatuses = r.batteryStatuses;
      ctdDate         = r.ctdDate;
    }



    public InstrumentData (string    s,
                           int[]     fieldIndexes,
                           ref bool  valid
                          )
    {
      data = new float[numDataFields];
      ParseTabDelStr (s, fieldIndexes, ref valid);
      updatedFlag = false;
    }




    public  InstrumentData  (PicesInstrumentData  picesData)
    {
      data = new float[numDataFields];
      UpdateCTDDataFields  (picesData.CTDBattery,                          // _batteryVolt,
                            picesData.Conductivity,
                            picesData.Density,
                            picesData.Depth,
                            picesData.Fluorescence,
                            picesData.FluorescenceSensor,
                            picesData.Oxygen,
                            picesData.CdomFluorescence,
                            picesData.CdomFluorescenceSensor,
                            picesData.OxygenSensor,
                            picesData.Pressure,
                            picesData.Salinity,
                            picesData.SoundVelocity,
                            picesData.Temperature,
                            picesData.Transmissivity,
                            picesData.TransmissivitySensor,
                            picesData.Turbidity,
                            picesData.TurbiditySensor,
                            picesData.CtdDate
                           );


      byteOffset    = picesData.ByteOffset;
      FlowRate1     = picesData.FlowRate1;
      FlowRate2     = picesData.FlowRate2;
      longitude     = picesData.Longitude;
      latitude      = picesData.Latitude;
      RecordingRate = picesData.RecordRate;
      scanLine      = (int)picesData.ScanLine;

      data[PitchIndex        ] = picesData.Pitch;
      data[RollIndex         ] = picesData.Roll;
      data[IlluminationIndex ] = picesData.Illumination;
      data[RecordingRateIndex] = picesData.RecordRate;
      data[Bat1LevelIndex    ] = picesData.Bat1Level;
      data[Bat2LevelIndex    ] = picesData.Bat2Level;
      data[Bat3LevelIndex    ] = picesData.Bat3Level;
      data[Bat4LevelIndex    ] = picesData.Bat4Level;

      this.latitude       = picesData.Latitude;
      this.longitude      = picesData.Longitude;
      this.activeBattery  = picesData.ActiveBattery;
    
    }  /* InstrumentData */




    public  void  Dispose ()
    {
      data = null;
    }



    public  string  GetField (int idx)
    {
      if  (idx < numDataFields)
        return data[idx].ToString ();
        
        
      string  field = "";
        
      switch  (idx)
      {
        case  LatitudeIndex:      
           field = latitude.ToString ();
           break;
           
        case  LongitudeIndex:     
           field = longitude.ToString ();     
           break;
        
        case  DateIndex:          
           field = time.ToString ("G");  
           break;
        
        case  ActiveBatteryIndex: 
           field = activeBattery.ToString ();
           break;
           
        case  BatteryStatusesIndex:
           field = batteryStatuses;
           break;
           
        case  CTD_DateIndex:
           field = ctdDate.ToString ("yy/MM/dd HH:mm:ss");
           break;
           
        case  ScanLineIndex:
           field = scanLine.ToString ();
           break;
           
        case  ByteOffseIndex:
           field = byteOffset.ToString ();
           break;
      }
      
      return field;
    }  /* GetField */



    public  void  UpdateBatteryPack (ChartControls.BatteryPack  bp)
    {
      bp.PopulateFields (activeBattery, data[Bat1LevelIndex], data[Bat2LevelIndex], data[Bat3LevelIndex], data[Bat4LevelIndex], batteryStatuses);
    }  /* UpdateBatteryPack */



   
    public  void  WriteLine (StreamWriter  w,
                             int[]         fieldIndexes
                            )
    {
      int x;
      for (x = 0; x < fieldIndexes.Length; x++)
      {
        if  (x > 0)
          w.Write ("\t");
        w.Write (GetField (fieldIndexes[x]));
      }
      
      w.WriteLine ();
    } /* WriteLine */


    private  void  ParseTabDelStr (string    s,
                                   int[]     fieldIndexes,
                                   ref bool  valid
                                  )
    {
      valid = true;

      ctdDate = new DateTime (1, 1, 1, 0, 0, 0);

      string[] fields = s.Split ('\t');
      
      if  (fields.Length != fieldIndexes.Length)
      {
        valid = false;
        return;
      }
      
      int  x = 0;

      for  (x = 0;  x < fields.Length;  x++)
      {
        string  field = fields[x];
        float   floatValue = PicesKKStr.StrToFloat (field);
        int  fieldIndex = fieldIndexes[x];

        if  (fieldIndex < numDataFields)
        {
          data[fieldIndex] = floatValue;
          continue;
        }
        
        switch  (fieldIndex)
        {
           case  LatitudeIndex:        latitude        = PicesKKStr.StrToDouble   (field); break;
           case  LongitudeIndex:       longitude       = PicesKKStr.StrToDouble   (field); break;
           case  DateIndex:            time            = PicesKKStr.StrToDateTime (field); break;
           case  ActiveBatteryIndex:   activeBattery   = PicesKKStr.StrToInt      (field); break;
           case  BatteryStatusesIndex: batteryStatuses = field.Trim ();                    break;
           case  CTD_DateIndex:        ctdDate         = PicesKKStr.StrToDateTime (field); break;
           case  ScanLineIndex:        scanLine        = PicesKKStr.StrToInt      (field); break;
           case  ByteOffseIndex:       byteOffset      = PicesKKStr.StrToUInt64   (field); break;
        }
      }
    }  /* ParseTabDelStr */



    public bool Equals (InstrumentData o)
    {
      if  (o == null)
        return false;
        
      int  x;
      for  (x = 0;  x < data.Length;  x++)
      {
        if  (data[x] != o.data[x])
          return false;
      }
      
      if  ((longitude     == o.longitude)      &&
           (latitude      == o.latitude)       &&
           (time          == o.time)           &&
           (activeBattery == o.activeBattery)  &&
           (batteryStatuses.Equals (o.batteryStatuses))  &&
           (ctdDate       == o.ctdDate)
          )
        return true;
      else
        return false;
    }  /* Equals */



    public bool  BatteryDataChanged (InstrumentData o)
    {
      if (o == null)
        return false;

      if ((activeBattery == o.activeBattery)  &&  
          (batteryStatuses.Equals (o.batteryStatuses))  &&
          (data[Bat1LevelIndex] == o.data[Bat1LevelIndex]) &&
          (data[Bat2LevelIndex] == o.data[Bat2LevelIndex]) &&
          (data[Bat3LevelIndex] == o.data[Bat3LevelIndex]) &&
          (data[Bat4LevelIndex] == o.data[Bat4LevelIndex])
         )
        return false;
      else
        return true;
    } /* BatteryDataChanged */



    public  void  UpdateCTDDataFields  (
      float    _batteryVolt,
      float    _conductivity,
      float    _density,
      float    _depth,
      float    _fluorescence,
      float    _fluorescenceSensor,
      float    _cdomFluorescence,
      float    _cdomFluorescenceSensor,
      float    _oxygen,
      float    _oxygenSensor,
      float    _pressure,
      float    _salinity,
      float    _soundVelocity,
      float    _temperature,
      float    _transmisivity,
      float    _transmisivitySensor,
      float    _turbidity,
      float    _turbiditySensor,
      DateTime _ctdDate
      )
    {
      data[CTDBatteryIndex          ] = _batteryVolt;
      data[ConductivityIndex        ] = _conductivity;
      data[DensityIndex             ] = _density;
      data[DepthIndex               ] = _depth;
      data[FluorescenceIndex        ] = _fluorescence;
      data[CdomFluorescenceSensorIndex] = _cdomFluorescenceSensor;
      data[CdomFluorescenceIndex      ] = _cdomFluorescence;
      data[FluorescenceSensorIndex  ] = _fluorescenceSensor;
      data[OxygenIndex              ] = _oxygen;
      data[OxygenSensorIndex        ] = _oxygenSensor;
      data[PressureIndex            ] = _pressure;
      data[SalinityIndex            ] = _salinity;
      data[SoundVelocityIndex       ] = _soundVelocity;
      data[TemperatureIndex         ] = _temperature;
      data[TransmisivityIndex       ] = _transmisivity;
      data[TransmisivitySensorIndex ] = _transmisivitySensor;
      data[TurbidityIndex           ] = _turbidity;
      data[TurbiditySensorIndex     ] = _turbiditySensor;
      ctdDate                   = _ctdDate;
    }


    public  void  UpdatePitchAndRoll (float _pitch, float _roll)
    {
      data[PitchIndex] = _pitch;
      data[RollIndex]  = _roll;
      updatedFlag = true;
    }



    public  void  UpdateLatitudeAndLongitude (double _latitude, double _longitude)
    {
      latitude  = _latitude;
      longitude = _longitude;
      updatedFlag = true;
    }
  


    public  void  UpdateRecordingRate (float  _recordingRate)
    {
      data[RecordingRateIndex] = _recordingRate;
      updatedFlag = true;
    }



    public  void  UpdateGPS (double    _latitude, 
                             double    _longitude, 
                             DateTime  _gpsTime
                            )
    {
      this.longitude = _longitude;
      this.latitude  = _latitude;
    }                           




    public void UpdateSipperBatteries (int     _activeBattery,
                                       float[] _batteryLevels,   // only concerned with indexes '1' -> '4'
                                       string  _batteryStatuses
                                      )
    {
      _batteryStatuses =  _batteryStatuses.Trim ();
    
      if  ((_batteryStatuses != "") && (_batteryStatuses.Length != 4))
        return;
    
      if  (_batteryLevels.Length != 5)
        return;
    
      if  ((_activeBattery < 1)  ||  (_activeBattery > 4))
        return;
    
      activeBattery = _activeBattery;
      data[Bat1LevelIndex] = _batteryLevels[1];
      data[Bat2LevelIndex] = _batteryLevels[2];
      data[Bat3LevelIndex] = _batteryLevels[3];
      data[Bat4LevelIndex] = _batteryLevels[4];

      updatedFlag = true;
      
      batteryStatuses = _batteryStatuses.Trim ();
    }

  }  /* InstrumentData */
}
