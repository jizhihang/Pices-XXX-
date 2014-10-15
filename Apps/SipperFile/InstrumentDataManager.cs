using System;
using System.IO;

using  PicesInterface;

namespace SipperFile
{
	/// <summary>
	/// Summary description for InstrumentDataManager.
  /// Instrument data from sipper files will be directed to an instance of this class.
  /// then based off the DeviceId the data will be sent to the appropriate Instrtument 
  /// derived class.
	/// </summary>
	public class InstrumentDataManager
	{
    InstrumentData            currentDataRow  = null;
    InstrumentDataLogger      dataLogger      = null;

    double                    latitude        =0.0;
    PortAssignment[]          portAssignments = null;

    InstrumentDataReport[]    reporters = null;  // There will one potential Reporter for each of
                                               // the 64 posible devices.


    const byte                MaxNumOfDevices     = 64;
    const byte                SerialPort0DeviceID = 16;

    private PicesSipperFile   sipperFile = null;
    private SipperConfigRec   configRec = null;


    public  double            Latitude   ()  {return  latitude;}
    public  PicesSipperFile   SipperFile ()  {return  sipperFile;}
    public  SipperConfigRec   ConmfigRec ()  {return  configRec;}

    public InstrumentData     CurrentDataRow ()  { return currentDataRow; }



    /// <summary>
    /// Initializes a new instance of the <see cref="InstrumentDataManager"/> class.
    /// </summary>
    /// <param name="_dataLogger">The _data logger.</param>
    /// <param name="_sipperFile">The _sipper file.</param>
    /// <param name="_configRec">The _config rec.</param>
    /// <param name="_currentDataRow">The _current data row.</param>
		public InstrumentDataManager (InstrumentDataLogger  _dataLogger,   // Will take ownershop of '_dataLogger'
		                              PicesSipperFile       _sipperFile,
                                  SipperConfigRec       _configRec,
                                  InstrumentData        _currentDataRow
                                 )
		{
      dataLogger = _dataLogger;
      configRec  = _configRec;
      sipperFile = _sipperFile;

      portAssignments = null;

      if  (sipperFile != null)
      {
        portAssignments = new PortAssignment[4];

        if  (String.IsNullOrEmpty (sipperFile.Sp0))
          portAssignments[0] = null;
        else
          portAssignments[0] = new PortAssignment (sipperFile.Sp0, "");

        if  (String.IsNullOrEmpty (sipperFile.Sp1))
          portAssignments[1] = null;
        else
          portAssignments[1] = new PortAssignment (sipperFile.Sp1, "");

        if  (String.IsNullOrEmpty (sipperFile.Sp2))
          portAssignments[2] = null;
        else
          portAssignments[2] = new PortAssignment (sipperFile.Sp2, "");

        if  (String.IsNullOrEmpty (sipperFile.Sp3))
          portAssignments[3] = null;
        else
          portAssignments[3] = new PortAssignment (sipperFile.Sp3, "");
      }
      else
      {
        portAssignments = configRec.HeaderRec ().PortAssignments ();
      }

      reporters = new InstrumentDataReport[MaxNumOfDevices];

      currentDataRow = _currentDataRow;

      byte  deviceId = 0;
      for  (deviceId = 0;  deviceId < MaxNumOfDevices;  deviceId++)
      {
        reporters[deviceId] = null;
        
        if  (deviceId >= SerialPort0DeviceID)
        {
          byte serialPort = (byte)(deviceId - SerialPort0DeviceID);
          if  (serialPort < portAssignments.Length)
          {
            if  (portAssignments[serialPort] != null)
              reporters[deviceId] = portAssignments[serialPort].Instrument().CreateInstrumentDataReport(this, deviceId);
          }
        }
      }

      InitializeLatitude ();
		}


    public  void  Dispose ()
    {
      if  (reporters != null)
      {
        for  (int x = 0;  x < reporters.Length;  x++)
        {
          if  (reporters[x] != null)
          {
            reporters[x].Dispose ();
            reporters[x] = null;
          }
        }
      }

      currentDataRow = null;
      reporters = null;

      if  (dataLogger != null)
      {
        dataLogger.Dispose ();
        dataLogger = null;
      }

    } /* Dispose */



    private  void  InitializeLatitude ()
    {
      // because we do not always have access to the SipperFile, or the SIPPER file does
      // not have the latitude info in it we need to deduce the approximate lattitude from 
      // the sipper file name.
      if  (sipperFile != null)
      {
        if  (sipperFile.Latitude > 0.1)
        {
          latitude = sipperFile.Latitude;
        }

        else if  ((sipperFile.CruiseName == "ETP2007")  ||  (sipperFile.CruiseName == "ETP2008"))
        {
          // it is for a fact a panama cruise
          latitude = 11.0 + (59.7 / 60.0);
        }
        else
        {
          // We will assume it must be one of the GULF cruises.
          latitude = 29 + (30.0 / 60.0);
        }
      }
      else
      {
        latitude = 29 + (30.0 / 60.0);
      }
      return;
    }  /* InitializeLatitude */





    /// <summary>
    /// Assigns the reporter.
    /// </summary>
    /// <param name="deviceID">The device ID ccomming from the SIPPER file.  See Sipper3 File Format.
    ///                        see Sipper3File.cs
    /// </param>
    /// <param name="reporter">The reporter.</param>
    public void  AssignReporter (byte                 deviceID,
                                 InstrumentDataReport reporter
                                )
    {
      if  ((deviceID < 0)   ||  (deviceID >= MaxNumOfDevices))
      {
        throw new Exception ("DeviceID[" + deviceID.ToString () + "] out of range.");
      }
      
      reporters[deviceID] = reporter;
    }  /* AssignReporter */
    
    
    

    public void  ReportSerialPortData (byte    portNum,
                                       string  txt
                                      )
    {
      ReportData ((byte)(portNum + SerialPort0DeviceID), txt);
    }  /* ReportSerialPortData */                                     



   
    public void  ReportData (byte    deviceId,
                             string  txt
                            )
    {
      if  ((deviceId < 0)  ||  (deviceId >= reporters.Length))
        return;

      if (reporters[deviceId] == null)
        reporters[deviceId] = new InstrumentDataReport (this, deviceId);

      reporters[deviceId].ReportData (txt);
        
    }  /* ReportData */




    public void ReportData (byte  deviceId,
                            char  ch
                           )
    {
      if ((deviceId < 0) || (deviceId >= reporters.Length))
        return;


      if (reporters[deviceId] == null)
        reporters[deviceId] = new InstrumentDataReport (this, deviceId);

      reporters[deviceId].ReportData (ch);
    }  /* ReportData */




    public void  ReportData (Sipper3DecodeRec  dataRec,
                             int               scanLine
                            )
    {
      byte  deviceId = dataRec.SensorNum ();
      if  ((deviceId < 0)  ||  (deviceId >= reporters.Length))
        return;

      if  (reporters[deviceId] == null)
        reporters[deviceId] = (new Instrument ("Temp","Temp")).CreateInstrumentDataReport (this, deviceId);

      reporters[deviceId].ReportData (dataRec.Text (),  dataRec.SensorData (), scanLine);
    }  /* ReportData */


   
    public void  LogTxtLine (byte    deviceId,
                             string  txtLine
                            )
    {
      if  (dataLogger != null)
        dataLogger.LogInstrumentData (deviceId, txtLine);
    }  /* LogTxtLine */

  }  /* DepthPlot2 */
}
