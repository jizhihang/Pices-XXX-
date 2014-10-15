using  System;
using  System.IO;

using  PicesInterface;


namespace SipperFile
{
	/// <summary>
	/// Summary description for InstrumentDataReportCTD.
	/// </summary>
	public class InstrumentDataReportCTD:  InstrumentDataReport
	{
    public  enum  OutPutFormat {RawFreqAndVoltsInHex,
                                EngineeringUnitsInHex,
                                RawFreqAndVoltsInDec,
                                EngineeringUnitsInDec,
                                PressureAndScanNumInHex
                               };

    private  OutPutFormat  outFormat = OutPutFormat.EngineeringUnitsInDec;

    private  float    batteryVoltage      = 0.0f;
    private  float    conductivity        = 0.0f;  // (S/m)
    private  DateTime dateTime            = DateTime.Now;
    private  float    density             = 0.0f;  // (kg/m^3)
    private  float    depth               = 0.0f;  // meters
    private  float    oxygen              = 0.0f;
    private  float    oxygenSensor        = 0.0f;  // Oxygen will be calculated from this field.  
    private  float    fluorescence        = 0.0f;
    private  float    fluorescenceSensor  = 0.0f;

    private  float    cdomFluorescence       = 0.0f;
    private  float    cdomFluorescenceSensor = 0.0f;

    private  float    transmisivity       = 0.0f;
    private  float    transmisivitySensor = 0.0f;
    private  float    turbidity           = 0.0f;
    private  float    turbiditySensor     = 0.0f;
    private  float    pressure            = 0.0f;  // Decibars
    private  float    salinity            = 0.0f;  // (psu)
    private  float    soundVelocity       = 0.0f;  // (meters/sec)
    private  float    temperature         = 0.0f;  // celcius

    private  String   ext0Code         = "";
    private  String   ext1Code         = "";  // "OXG" - Oxygen,       "FLO" - Flourameter,  "CDM" - CDOM fluorometer
    private  String   ext2Code         = "";  // "TRN" - Transameter,  "TUR" - Turbidity"
    private  String   ext3Code         = "";
    

    private  StreamWriter     ctdStream  = null;
    private  PicesSipperFile  sipperFile = null;

    private  bool  processSoundVelocity = true;


    IFormatProvider intFormat = new System.Globalization.CultureInfo ("", true);


		public InstrumentDataReportCTD (InstrumentDataManager  _manager,
		                                byte                   _deviceId,
                                    OutPutFormat           _outFormat
                                   ):
                                   base(_manager, _deviceId)
		{
      sipperFile = _manager.SipperFile ();
      SipperConfigRec  configRec  = _manager.ConmfigRec ();

      if  (sipperFile != null)
      {
        ext0Code  = sipperFile.CtdExt0;
        ext1Code  = sipperFile.CtdExt1;
        ext2Code  = sipperFile.CtdExt2;
        ext3Code  = sipperFile.CtdExt3;
      }

      else if (configRec != null)
      {
        ext0Code  = configRec.HeaderRec ().CtdExt0Code ();
        ext1Code  = configRec.HeaderRec ().CtdExt1Code ();
        ext2Code  = configRec.HeaderRec ().CtdExt2Code ();
        ext3Code  = configRec.HeaderRec ().CtdExt3Code ();
      }

      String  siperFileName = "";
      if  (sipperFile != null)
        siperFileName = OSservices.GetRootName (sipperFile.SipperFileName);

      //if  (!String.IsNullOrEmpty (siperFileName))
      //  ctdStream = new StreamWriter ("C:\\Temp\\" + siperFileName + ".txt");
		}
 


    int   LocateAMonthNameStr (String str)
    {
      // Meant to locate the Month in a String produced by CTD.  Which is always
      // started by a upper case character followed by two lower case chracaters.


      int idx = 0;
      int lastIdx = str.Length - 6;

      char  c0, c1, c2;

      while  (idx < lastIdx)
      {
        c0 = str[idx];

        if  ((c0 >= 'A')  &&  (c0 < 'Z'))
        {
          c1 = str[idx + 1];
          c2 = str[idx + 2];

          if  ((c0 == 'J')  && (c1 == 'a')  &&  (c2 =='n'))
            return idx;

          if  ((c0 == 'F')  && (c1 == 'e')  &&  (c2 =='b'))
            return idx;

          if  ((c0 == 'M')  && (c1 == 'a')  &&  (c2 =='r'))
            return idx;

      
          if  ((c0 == 'A')  && (c1 == 'p')  &&  (c2 =='r'))
            return idx;

          if  ((c0 == 'M')  && (c1 == 'a')  &&  (c2 =='y'))
            return idx;

          if  ((c0 == 'J')  && (c1 == 'u')  &&  (c2 =='n'))
            return idx;

          if  ((c0 == 'J')  && (c1 == 'u')  &&  (c2 =='l'))
            return idx;

          if  ((c0 == 'A')  && (c1 == 'u')  &&  (c2 =='g'))
            return idx;

          if  (((c0 == 'S')  && (c1 == 'p')  &&  (c2 =='t'))  ||  ((c0 == 'S')  && (c1 == 'e')  &&  (c2 =='p')))
            return idx;

      
          if  ((c0 == 'O')  && (c1 == 'c')  &&  (c2 =='t'))
            return idx;

          if  ((c0 == 'N')  && (c1 == 'o')  &&  (c2 =='v'))
            return idx;

          if  ((c0 == 'D')  && (c1 == 'e')  &&  (c2 =='c'))
            return idx;
        }

        idx++;
      }

      return -1;
    }  /* LocateAMonthNameStr */



    override  public  void   ParseTxtBlock (ref String  block)
    {
      // We have no idea what to do with this data so we will ignore it !!!

      // There is at least 118 characters in a full line of CTD data with all 4 sensors on

      while  (block.Length > 117)
      {
        int  monthLoc = LocateAMonthNameStr (block);

        if  (monthLoc < 0)
        {
          if  (block.Length > 120)
          {
            // We could not find the Month field yet the block is more than long enough to have 
            // included it.  We will assume that we are loking at garbage and throw it away.
            block = "";
            return;
          }
          else
          {
            // We do not appear to have enough data to incluse a cmplete CTD string.  So we will just
            // return bac to the caller and wait for some nore data to show up.
            return;
          }
        }

        int  endofCtdStrIdx   = monthLoc + 34;
        int  startOfCtdStrIdx = monthLoc - 84;

        if  (startOfCtdStrIdx < 0)
        {
          // We do not have enough of the beginning part of the string to process.  
          // So we need to throw it out
          if  (endofCtdStrIdx > block.Length)
            block = "";
          else
            block = block.Substring (endofCtdStrIdx + 1);
        }

        else
        {
          if  (endofCtdStrIdx >= block.Length)
          {
            // We need some more data in block to complete a CtdData line.
            return;
          }
          else
          {
            String  curLine = block.Substring (startOfCtdStrIdx, 1 + endofCtdStrIdx - startOfCtdStrIdx);
            block = block.Substring (endofCtdStrIdx + 1);
            try
            {
              ParseTxtLine (curLine);
            }
            catch  (Exception)
            {
              // For now we will be happy that the program does not crash.
              
            }
          }
        }
      }
    }  /* ParseTxtBlock*/

    
/*
28.4069,  5.48627,   17.249, 4.2988, 3.3123, 0.0967, 0.0816,  33.7479, 1541.195, 30 Oct 2007, 21:52:42,  21.3204,  9.9 
28.4070,  5.48617,   17.247, 4.2993, 3.3116, 0.0972, 0.0797,  33.7472, 1541.194, 30 Oct 2007, 21:52:42,  21.3198,  9.9 
28.4072,  5.48616,   17.244, 4.2986, 3.3089, 0.0996, 0.0795,  33.7470, 1541.194, 30 Oct 2007, 21:52:43,  21.3196, 10.0 
28.4071,  5.48620,   17.249, 4.2993, 3.3093, 0.0995, 0.0788,  33.7473, 1541.194, 30 Oct 2007, 21:52:43,  21.3198,  9.9 
28.4072,  5.48620,   17.257, 4.3007, 3.3099, 0.0994, 0.0796,  33.7472, 1541.195, 30 Oct 2007, 21:52:43,  21.3198, 10.0
28.4075,  5.48617,   17.268, 4.2999, 3.3098, 0.1021, 0.0823,  33.7469, 1541.195, 30 Oct 2007, 21:52:44,  21.3194,  9.9

18.4906,  4.58677,   39.889, 0.8271, 0.2241, 4.3278,  34.5427, 1517.357, 30 Oct 2007, 17:55:35,  24.8002, 11.6
18.4828,  4.58623,   39.874, 0.8261, 0.2327, 4.3310,  34.5446, 1517.337, 30 Oct 2007, 17:55:36,  24.8036, 11.6
18.4751,  4.58572,   39.869, 0.8255, 0.2352, 4.3305,  34.5468, 1517.317, 30 Oct 2007, 17:55:36,  24.8071, 11.5
18.4683,  4.58524,   39.879, 0.8242, 0.2290, 4.3316,  34.5485, 1517.299, 30 Oct 2007, 17:55:36,  24.8102, 11.5
18.4623,  4.58499,   39.911, 0.8234, 0.2366, 4.3254,  34.5514, 1517.286, 30 Oct 2007, 17:55:37,  24.8138, 11.5
*/    
  

    override public void  ParseTxtLine (string  txtLine)
    {
      switch  (outFormat)
      {
        case OutPutFormat.RawFreqAndVoltsInHex:     ParseRawFreqAndVoltsInHex    (txtLine);  break;
        case OutPutFormat.EngineeringUnitsInHex:    ParseEngineeringUnitsInHex   (txtLine);  break;
        case OutPutFormat.RawFreqAndVoltsInDec:     ParseRawFreqAndVoltsInDec    (txtLine);  break;
        case OutPutFormat.EngineeringUnitsInDec:    ParseEngineeringUnitsInDec   (txtLine);  break;
        case OutPutFormat.PressureAndScanNumInHex:  ParsePressureAndScanNumInHex (txtLine);  break;
      }

      if  (ctdStream != null)
        ctdStream.WriteLine (fluorescence.ToString ("#,##0.000") + "\t" + txtLine);
      
      if  ((temperature    <= 0.0f)    ||  (temperature    >= 50.0f)   ||
           (density        <  -6.0f)   ||  
           (soundVelocity  <  100.0f)  ||  (soundVelocity  >  2000.0f) ||  
           (depth          <  -10)     ||  (depth          > 600)      ||
           (batteryVoltage <  6.0f)    ||  (batteryVoltage > 14.9f)
          )
      {
        return;
      }     
      

      // Now time to update CTD data fields in CurrentDataRow in InstrumentManager object.
      Manager ().CurrentDataRow ().UpdateCTDDataFields  
        (batteryVoltage,
         conductivity,
         density,
         depth,
         fluorescence, 
         fluorescenceSensor,
         cdomFluorescence,
         cdomFluorescenceSensor,
         oxygen,
         oxygenSensor,
         pressure,
         salinity,
         soundVelocity,
         temperature,
         transmisivity,
         transmisivitySensor,
         turbidity,
         turbiditySensor,
         dateTime
        );
    }

 

    private  void  ParseRawFreqAndVoltsInHex (string  txtLine)
    {
      string[] fields = txtLine.Split (',');
    }



    private  void  ParseEngineeringUnitsInHex (string  txtLine)
    {
      string[] fields = txtLine.Split (',');
      if  (fields.Length < 10)
      {
        // We have an uncomplete text line; no point trying to decode it.
        return;
      }
    }



    private  void  ParseRawFreqAndVoltsInDec (string  txtLine)
    {
      string[] fields = txtLine.Split (',');
      if  (fields.Length < 10)
      {
        // We have an uncomplete text line; no point trying to decode it.
        return;
      }
    }  /* ParseRawFreqAndVoltsInDec */





    private void  UpdateExternalVoldtageField (String  extCode,
                                               String  amtField
                                              )
    {
      float  amt = PicesKKStr.StrToFloat (amtField);
      switch  (extCode)
      {
        case  "CDM": cdomFluorescenceSensor = amt;  break;
        case  "FLO": fluorescenceSensor     = amt;  break;
        case  "OXG": oxygenSensor           = amt;  break;
        case  "TRN": transmisivitySensor    = amt;  break;
        case  "TUR": turbiditySensor        = amt;  break;
      }
    }  /* UpdateExternalVoldtageField */




    /*
        Temp      Conduc      Pres   Ext0    Ext1    Ext2    Ext3      Salin   Sound        Date        Time     Density  Volts
        27.2807,  0.00004,    0.161, 0.0000, 0.0000, 0.0000, 0.0000,   0.0127, 1502.622, 01 Jan 2009, 21:49:00,  -3.5531, 11.8 
        27.2806,  0.00004,    0.161, 0.0000, 0.0000, 0.0000, 0.0000,   0.0127, 1502.622, 01 Jan 2009, 21:49:00,  -3.5531, 11.7 
        27.2804,  0.00005,    0.163, 0.0000, 0.0000, 0.0000, 0.0000, 
        27.2673,  0.00004,    0.168, 0.0000, 0.0000, 0.0000, 0.0000,   0.0127, 1502.589, 01 Jan 2009, 21:50:15,  -3.5494, 11.8   
        22.6653,  0.00004,    0.186, 4.8936, 0.0001, 3.6778, 4.9422,   0.0110, 1490.267, 31 Dec 2008, 08:48:56,  -2.3730, 12.3
        22.6650,  0.00004,    0.186, 4.8956, 0.0000, 3.6778, 4.9447,   0.0110, 1490.267, 31 Dec 2008, 08:48:57,  -2.3729, 12.3
        21.2358,  0.00005,    0.127, 4.4267, 3.0283, 2.9500, 4.9417,   0.0104, 1486.104, 02 Jan 2011, 13:12:44,  -2.0497, 11.8

        21.2359,  0.00006,    0.135, 4.4265, 3.0284, 2.9513, 4.9428,   0.0104, 1486.104, 02 Jan 2011, 13:12:44,  -2.0497, 11.9
        21.2359,  0.00006,    0.130, 4.4263, 3.0285, 2.9508, 4.9432,   0.0104, 1486.104, 02 Jan 2011, 13:12:44,  -2.0497, 11.9
        21.2361,  0.00006,    0.133, 4.4265, 3.0287, 2.9514, 4.9451,   0.0104, 1486.105, 02 Jan 2011, 13:12:45,  -2.0497, 11.9
        21.2363,  0.00005,    0.129, 4.4262, 3.0286, 2.9511, 4.9443,   0.0104, 1486.105, 02 Jan 2011, 13:12:45,  -2.0498, 11.8
        21.2363,  0.00006,    0.131, 4.4261, 3.0290, 2.9508, 4.9437,   0.0104, 1486.105, 02 Jan 2011, 13:12:45,  -2.0498, 11.8

         * Raw data from Panama Cruise. Dec-2008 thru Jan-2009; 
         * We got a surprise,  When all 4 external inputs are enabled in CTD,  there is not cr/lf at end of line.
         * 
        28.4069,  5.48627,   17.249, 4.2988, 3.3123, 0.0967, 0.0816,  33.7479, 1541.195, 30 Oct 2007, 21:52:42,  21.3204,  9.9 

    */


    
    private  void  ParseEngineeringUnitsInDec (string  txtLine)
    {
      string[] fields = txtLine.Split (',');
      if  (fields.Length < 10)
      {
        // We have an uncomplete text line; no point trying to decode it.
        return;
      }
      else if  (fields.Length > 13)
        return;

      int numExternalVoltages = 0;
      if  (processSoundVelocity)
        numExternalVoltages = fields.Length - 9;
      else
        numExternalVoltages = fields.Length - 8;

      if  (numExternalVoltages > 4)
        numExternalVoltages = 4;

      int  fieldNum = 0;

      temperature  = PicesKKStr.StrToFloat (fields[fieldNum++]);
      conductivity = PicesKKStr.StrToFloat (fields[fieldNum++]);
      pressure     = PicesKKStr.StrToFloat (fields[fieldNum++]);

      int  numExternalProcessed = 0;

      if  ((!String.IsNullOrEmpty (ext0Code))  &&  (numExternalProcessed < numExternalVoltages))
      {
        UpdateExternalVoldtageField (ext0Code, fields[fieldNum++]);
        numExternalProcessed++;
      }

      if  ((!String.IsNullOrEmpty (ext1Code))  &&  (numExternalProcessed < numExternalVoltages))
      {
        UpdateExternalVoldtageField (ext1Code, fields[fieldNum++]);
        numExternalProcessed++;
      }

      if  ((!String.IsNullOrEmpty (ext2Code))  &&  (numExternalProcessed < numExternalVoltages))
      {
        UpdateExternalVoldtageField (ext2Code, fields[fieldNum++]);
        numExternalProcessed++;
      }

      if  ((!String.IsNullOrEmpty (ext3Code))  &&  (numExternalProcessed < numExternalVoltages))
      {
        UpdateExternalVoldtageField (ext3Code, fields[fieldNum++]);
        numExternalProcessed++;
      }

      while  (numExternalProcessed < numExternalVoltages)
      {
        fieldNum++;
        numExternalProcessed++;
      }

      salinity       = PicesKKStr.StrToFloat (fields[fieldNum++]);
      soundVelocity  = PicesKKStr.StrToFloat (fields[fieldNum++]);
      dateTime       = StrToDateTime (fields[fieldNum++] + fields[fieldNum++], "dd MMM yyyy HH':'mm':'ss");
      density        = PicesKKStr.StrToFloat (fields[fieldNum++]);
      batteryVoltage = PicesKKStr.StrToFloat (fields[fieldNum++]);

       if  ((temperature < -1.0f)  ||  (temperature > 50.0f)  ||
           (pressure < -0.5f)     ||  (pressure > 10000.0f)  || 
           (oxygenSensor <= 0.0f)                                   // kak 2010-05-05  Sine we no longer have Oxygen Sensor need to set to zero 
          )
      {
        oxygen = 0.0f;
      }
      else
      {
        oxygen = DissolvedOxygenConcentration (oxygenSensor, temperature, salinity, pressure);
      }
      
      CalcFluorescence  ();
      CalcCdomFluorescence  ();
      CalcTurbidity     ();
      CalcTransmisivity ();

      double  latitude = Manager ().Latitude ();
      if  (latitude < 1.0)
      {
        // Set to latitude of Panama Cruises fisrt station.
        latitude = 29 + (30.0 / 60.0);     // 2010-05-05    For gulf trip will use 29.5  degrees.
        //  latitude = 11 + (59.7 /60.0);  // 11 degrees 59.7 minutes were used for Panama Trip.
      }
   
      // The folowing calc for deapth comes from:
      // SBE  Application Note No. 69,  Page 1.  July 2002
      // references UNESCO Technical Papers in Marine Science No. 44.
      
      // gravity = (m/sec^2)

      double  degToRad = 1.0 / 57.29578;
     
      double  x = Math.Pow (Math.Sin (latitude * degToRad), 2.0);
      double  gravity = 9.780318 * (1.0 + (5.2788e-3 + 2.36e-5 * x) * x) + 1.092e-6 * pressure;
      
      double p = (double)pressure;

      depth  = (float)(((((-1.82e-15 * p + 2.279e-10) * p - 2.2512e-5) * p + 9.72659) * p) / gravity);
    }  /* ParseEngineeringUnitsInDec */



    private  void  ParsePressureAndScanNumInHex (string  txtLine)
    {
      string[] fields = txtLine.Split (',');
      if  (fields.Length < 10)
      {
        // We have an uncomplete text line; no point trying to decode it.
        return;
      }
    }


    private  DateTime  StrToDateTime (string s,  string formatStr)
    {
      DateTime d;

      try
      {
        d = DateTime.ParseExact(s, formatStr, null, System.Globalization.DateTimeStyles.AllowWhiteSpaces);
      }
      catch (Exception)
      {
        d = DateTime.MinValue;
      }

      return d;
    }





    private  void  CalcFluorescence ()
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


      // CHL(ug/l) = ScaleFactor(10) * (Output - DarkCounts(0.081))
      fluorescence = (float)(10.0 * (fluorescenceSensor - 0.081));
    }  /* CalcFluorescence */





    private  void  CalcCdomFluorescence ()
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


      // CHL(ug/l) = ScaleFactor(10) * (Output - DarkCounts(0.081))
      cdomFluorescence = (float)(10.0 * (cdomFluorescenceSensor - 0.081));
    }  /* CalcCdmFluorescence */



    
    private  void  CalcTurbidity ()
    {
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
      turbidity = (float)(2.41 * (turbiditySensor - 0.075));
    }  /* CalcTurbidity */



    private  void  CalcTransmisivity ()
    {
      // Using calibration sheet from WETLabs
      // dated July-23-2008
      // S/N: CST-1149
      // Phone # 541-929-5650
      // www.wetlabs.com
      //
      //  Vdark = 0.061V
      //  Vair  = 4.815V
      //  Vref  = 4.466V
      //
      // Tr = (Vsig - Vdark) / (Vref - Vdark)

      double  numerator   = (transmisivitySensor - 0.061);

      transmisivity = (float)(((double)transmisivitySensor - 0.075) / (4.466 - 0.061));
    }  /* CalcTransmisivity */



    // Calibration Fields.
    private  double  Soc     =  0.3634;
    private  double  Boc     =  0.0000;
    private  double  Voffset = -0.5098;
    private  double  tcor    =  0.0016;
    private  double  pcor    =  1.350e-04;
    private  double  tau     =  0.0; 
    private  double  deltaV  =  0.0;
    
    private  double  A1 = -173.4292;
    private  double  A2 =  249.6339;
    private  double  A3 =  143.3483;
    private  double  A4 =  -21.8429;
    private  double  B1 =   -0.033096;
    private  double  B2 =    0.014259;
    private  double  B3 =   -0.00170;

    private  float   lastWaterTemp = -9999.99f;
    private  float   lastSalinity  = -9999.99f;
    private  double  lastOxSat     = -9999.99;
    
    
    private  float  OxygenSaturation 
                   (float waterTemp,   // Celcius
                    float salinity     // psu
                   )
    {
      // Froma page Appendix A, page 6of SBE 43  Dissolved Oxygen Sensor
      // Application Note No. 64.

      if  ((lastWaterTemp == waterTemp)  &&  (lastSalinity == salinity))
        return (float)lastOxSat;

      double T = waterTemp;
      double S = salinity;

      double Ta = 273.15 + (double)waterTemp;

      double Ta100 = Ta / 100.0;

      double  A = A1 + A2 * (100.0 / Ta) + A3 * Math.Log (Ta100) + A4 * Ta100;
      double  B = B1 + B2 * Ta100 + B3 * Ta100 * Ta100;

      lastOxSat = Math.Exp (A + S * B);

      return  (float)lastOxSat;

    }  /* OxygenSaturation */
    
    




    private float  DissolvedOxygenConcentration 
                  (float  voltage,
                   float  temperature,
                   float  salinity,
                   float  pressure
                  )
    {
      double  P = pressure;
      double  T = temperature;
      double  V = voltage;

      double  disolvedOxygen_a = (Soc * (V + Voffset + tau * deltaV) + Boc * Math.Exp (-0.03 * T));

      double  Oxsat = OxygenSaturation (temperature, salinity);
      double  disolvedOxygen_b = Oxsat * Math.Exp (tcor * T) * Math.Exp (pcor * P);


      double  dissolvedOxygenConcentration = disolvedOxygen_a * disolvedOxygen_b;

      return  (float)dissolvedOxygenConcentration;
    }  /* DissolvedOxygenConcentration */







  }  /* InstrumentDataReportCTD */
}


/*
<<<<<<< InstrumentDataReportCTD.cs


21.2476,  0.04720,    0.058, 1.1153,   0.2468, 1486.411, 20 Nov 2006, 14:21:42,  -1.8719, 12.7
21.2476,  0.04720,    0.062, 1.1151,   0.2468, 1486.411, 20 Nov 2006, 14:21:42,  -1.8719, 12.7
21.2477,  0.04721,    0.056, 1.1154,   0.2469, 1486.411, 20 Nov 2006, 14:21:42,  -1.8718, 12.7
21.2477,  0.04720,    0.057, 1.1156,   0.2468, 1486.411, 20 Nov 2006, 14:21:43,  -1.8719, 12.8
21.2478,  0.04720,    0.055, 1.1155,   0.2468, 1486.411, 20 Nov 2006, 14:21:43,  -1.8719, 12.7
21.2477,  0.04721,    0.057, 1.1153,   0.2469, 1486.411, 20 Nov 2006, 14:21:43,  -1.8718, 12.7
21.2479,  0.04721,    0.058, 1.1152,   0.2469, 1486.412, 20 Nov 2006, 14:21:44,  -1.8719, 12.7
>>>>>>> 1.2
*/
