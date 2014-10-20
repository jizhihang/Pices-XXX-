using System;
using System.IO;
using System.Windows.Forms;

namespace SipperFile
{
	/// <summary>
	/// Summary description for SipperConfigRec.
  /// Used for the SipperFileHeader record.  It will have the ability to code and decode the 
  /// information thatwill be stored at the very beginning of a SipperFile.
	/// </summary>
	public class SipperHeaderRec
	{


    public const int MaxHeaderLen = 455;         // The first 455 bytes of a sipper file are supposd to be header
                                                 // info.  After that teh avctual SIPPER data starts.

    public const int MaxSipperFileNameLen = 30;  // Longest name thatthe SIPPER data controller can support

    private string   sipperFileName;
    private string   cruiseLocation;
    private string   objective;
    private string   principal;
    private string   researchOrg;
    private DateTime cruiseDateStart;
    private DateTime cruiseDateEnd;
    private string   deploymentVehicle;
    private string   seaConditions;
    private string   waveHeight;
    private string   weatherConditions;
    private string   airTemp;
    private double   latitude;
    private double   longitude;
    private int      scanRate;
    PortAssignment[] portAssignments = new PortAssignment[6];
    private string   ctdExt0Code = "";
    private string   ctdExt1Code = "";
    private string   ctdExt2Code = "";
    private string   ctdExt3Code = "";

	
		public SipperHeaderRec ()
		{
      InitFields ();
		}


    private  void  InitFields ()
    {
      int x = 0;
      for  (x = 0;  x < PortAssignment.MaxNumOfPorts;  x++)
      {
        portAssignments[x] = null;
      }

      sipperFileName    = "";
      cruiseLocation    = "";
      objective         = "";
      principal         = "";
      researchOrg       = "";
      cruiseDateStart   = DateTime.Today;
      cruiseDateEnd     = DateTime.Today;
      deploymentVehicle = "";
      seaConditions     = "";
      waveHeight        = "";
      weatherConditions = "";
      airTemp           = "";
      scanRate          = 0;
      latitude          = 0.0;
      longitude         = 0.0;
      ctdExt0Code       = "";
      ctdExt1Code       = "";
      ctdExt2Code       = "";
      ctdExt3Code       = "";

    }   /* InitFields */
    

    public  PortAssignment[]  PortAssignments ()  {return  portAssignments;}


    
    // Builds Header string that will be placed at beginning of SIPPER file
    // with ethe sfd command.
    public  string  HeaderStr ()
    {
      int  x = 0;
      string  s = "fn"   + "\t" + sipperFileName       + "\t" 
                + "sr"   + "\t" + scanRate.ToString () + "\t";

      for  (x = 0;  x < 6;  x++)
      {
        if  (portAssignments[x] != null)
        {
          s += "sp" + x.ToString () + "," + portAssignments[x].HeaderStr () + "\t";
        }
      }

      s += "cl"   + "\t" + cruiseLocation + "\t"
        +  "ob"   + "\t" + objective      + "\t"
        +  "pr"   + "\t" + principal      + "\t"
        +  "ro"   + "\t" + researchOrg    + "\t"
        +  "cd"   + "\t" + cruiseDateStart.ToString ("yyyy/mm/dd") + "," + cruiseDateEnd.ToString ("yyyy/mm/dd") + "\t"
        +  "dv"   + "\t" + deploymentVehicle     + "\t"
        +  "sc"   + "\t" + seaConditions         + "\t"
        +  "wh"   + "\t" + waveHeight            + "\t"
        +  "wc"   + "\t" + weatherConditions     + "\t"
        +  "at"   + "\t" + airTemp               + "\t"
        +  "lat"  + "\t" + latitude.ToString ()  + "\t"
        +  "log"  + "\t" + longitude.ToString () + "\t"
        +  "ext0" + "\t" + ctdExt0Code           + "\t"
        +  "ext1" + "\t" + ctdExt1Code           + "\t"
        +  "ext2" + "\t" + ctdExt2Code           + "\t"
        +  "ext3" + "\t" + ctdExt3Code;

      return  s;
    }

   
    public  void  ParseHeaderStr (string  s)
    {
      InitFields ();
      
      string[] fields = s.Trim ().Split ('\t');

      int  x = 0;
      bool  found = true;

      while  (x < fields.Length)
      {
        string  fieldName  = fields[x].ToLower (); 
        x++;

        string  fieldValue = "";
        if  (x < fields.Length)
        {
          fieldValue = fields[x]; 
          x++;
        }

        ProcessNameAndDataStrings (fieldName, fieldValue, ref found);
      }
    }  /* ParseDataFileLine */




    public void  ProcessNameAndDataStrings (string   fieldName,
                                            string   fieldValue,
                                            ref bool found
                                           )
    {

      found = true;
      fieldName = fieldName.ToLower ();

      if  ((fieldName == "at")  ||  (fieldName == "airtemp"))
      {
        airTemp = fieldValue;
      }

      else if  ((fieldName == "cd")  ||  (fieldName == "cruisedates"))
      {
        string[]  subFields = fieldValue.Split (',');
        IFormatProvider format = new System.Globalization.CultureInfo ("", true);
        cruiseDateStart = DateTime.ParseExact (subFields[0], "yyyy/mm/dd", format);
        if  (subFields.Length < 2)
          cruiseDateEnd = cruiseDateStart;
        else
          cruiseDateEnd = DateTime.ParseExact (subFields[1], "yyyy/mm/dd", format);
      }

      else if  ((fieldName == "cl")  ||  (fieldName == "cruiselocation"))
      {
        cruiseLocation = fieldValue;
      }

      else if  ((fieldName == "dv")  ||  (fieldName == "deploymentvehicle"))
      {
        deploymentVehicle = fieldValue;
      }

      else if  ((fieldName == "ext0") || (fieldName == "ctdext0code"))
      {
        ctdExt0Code = fieldValue;
      }

      else if  ((fieldName == "ext1") || (fieldName == "ctdext1code"))
      {
        ctdExt1Code = fieldValue;
      }

      else if  ((fieldName == "ext2") || (fieldName == "ctdext2code"))
      {
        ctdExt2Code = fieldValue;
      }

      else if  ((fieldName == "ext3") || (fieldName == "ctdext3code"))
      {
        ctdExt3Code = fieldValue;
      }

      else if  ((fieldName == "fn")  ||  (fieldName == "sipperfilename"))
      {
        sipperFileName = fieldValue;
      }
      
      else if  ((fieldName == "lat")  ||  (fieldName == "latitude"))
      {
        latitude = double.Parse (fieldValue);
      }

      else if ((fieldName == "log") || (fieldName == "longitude"))
      {
        longitude = double.Parse(fieldValue);
      }

      else if ((fieldName == "ob") || (fieldName == "objective"))
      {
        objective = fieldValue;
      }

      else if  ((fieldName == "pr")  ||  (fieldName == "principal"))
      {
        principal = fieldValue;
      }

      else if  ((fieldName == "ro")  ||  (fieldName == "researchorganization"))
      {
        researchOrg = fieldValue;
      }

      else if  ((fieldName == "sc")  ||  (fieldName == "seaconditions"))
      {
        seaConditions = fieldValue;
      }

      else if  ((fieldName == "sr")  ||  (fieldName == "scanrate"))
      {
        scanRate = int.Parse (fieldValue);
      }

      else if  ((fieldName == "sp")  ||  (fieldName == "serialport"))
      {
        // We are looking at a serial port definition
        string[]  subFields = fieldValue.Split (',');
        int  serialPortNum = int.Parse (subFields[0]);
        if  ((serialPortNum >= 0) && (serialPortNum < 6))
        {
          PortAssignment  portAssignment = null;

          if  (subFields.Length < 2)
            portAssignment = new PortAssignment ("", "");

          else if  (subFields.Length < 3)
            portAssignment = new PortAssignment (subFields[1], "");

          else
            portAssignment = new PortAssignment (subFields[1], subFields[2]);

          portAssignments[serialPortNum] = portAssignment;
        }
      }
 

      else if  ((fieldName == "wc")  ||  (fieldName == "weatherconditions"))
      {
        weatherConditions = fieldValue;
      }

      else if  ((fieldName == "wh")  ||  (fieldName == "waveheight"))
      {
        waveHeight = fieldValue;
      }

      else
      {
        found = false;
      }
    }  /* ParseDataFileLine */



    public  void  Save (StreamWriter w)
    {
      w.WriteLine ("AirTemp              = " + airTemp);
      w.WriteLine ("CruiseDates          = " + cruiseDateStart.ToString ("yyyy/mm/dd") + "," + cruiseDateEnd.ToString ("yyyy/mm/dd"));
      w.WriteLine ("CruiseLocation       = " + cruiseLocation);
      w.WriteLine ("deploymentVehicle    = " + deploymentVehicle);
      w.WriteLine ("SipperFilename       = " + sipperFileName);
      w.WriteLine ("Latitude             = " + latitude.ToString ());
      w.WriteLine ("Longitude            = " + longitude.ToString());
      w.WriteLine ("Objective            = " + objective);
      w.WriteLine ("Principal            = " + principal);
      w.WriteLine ("ResearchOrganization = " + researchOrg);
      w.WriteLine ("SeaConditions        = " + seaConditions);
      w.WriteLine ("ScanRate             = " + scanRate.ToString ());
      w.WriteLine ("CtdExt0Code          = " + ctdExt0Code);
      w.WriteLine ("CtdExt1Code          = " + ctdExt1Code);
      w.WriteLine ("CtdExt2Code          = " + ctdExt2Code);
      w.WriteLine ("CtdExt3Code          = " + ctdExt3Code);

      if  (portAssignments != null)
      {
        for  (int x = 0;  x < 6;  x++)
        {
          PortAssignment  portAssignment = portAssignments[x];

          if  (portAssignment != null)
          {
            w.WriteLine 
                  ("SerialPort           = " + x.ToString () + "," + portAssignment.HeaderStr ());
          }
        }
      }
           
      w.WriteLine ("WeatherConditions    = " + weatherConditions);
      w.WriteLine ("WaveHeight           = " + waveHeight);
    }  /* Save */


    public void  CruiseLocation     (string    _cruiseLocation)     {cruiseLocation    = _cruiseLocation;}
    public void  Objective          (string    _objective)          {objective         = _objective;}
    public void  Principal          (string    _principal)          {principal         = _principal;}
    public void  ResearchOrg        (string    _researchOrg)        {researchOrg       = _researchOrg;}
    public void  CruiseDateStart    (DateTime  _cruiseDateStart)    {cruiseDateStart   = _cruiseDateStart;}
    public void  CruiseDateEnd      (DateTime  _cruiseDateEnd)      {cruiseDateEnd     = _cruiseDateEnd;}
    public void  DeploymentVehicle  (string    _deploymentVehicle)  {deploymentVehicle = _deploymentVehicle;}
    public void  SeaConditions      (string    _seaConditions)      {seaConditions     = _seaConditions;}
    public void  WaveHeight         (string    _waveHeight)         {waveHeight        = _waveHeight;}
    public void  WeatherConditions  (string    _weatherConditions)  {weatherConditions = _weatherConditions;}
    public void  AirTemp            (string    _airTemp)            {airTemp           = _airTemp;}
    public void  Latitude           (double    _latitude)           {latitude          = _latitude; }
    public void  Longitude          (double    _longitude)          {longitude         = _longitude; }
    public void  ScanRate           (int       _scanRate)           {scanRate          = _scanRate; }
    public void  SipperFileName     (string    _sipperFileName)     {sipperFileName    = _sipperFileName;}
    public void  CtdExt0Code        (string    _ctdExt0Code)        {ctdExt0Code       = _ctdExt0Code;}
    public void  CtdExt1Code        (string    _ctdExt1Code)        {ctdExt1Code       = _ctdExt1Code;}
    public void  CtdExt2Code        (string    _ctdExt2Code)        {ctdExt2Code       = _ctdExt2Code;}
    public void  CtdExt3Code        (string    _ctdExt3Code)        {ctdExt3Code       = _ctdExt3Code;}

    public void  SerialPort         (int  serialPortNum, string shortName, string description)
    {
      if  ((serialPortNum < 0)  ||  (serialPortNum >= 6))
      {
        MessageBox.Show ("Invalid Serial Port Number[" + serialPortNum.ToString () + "].");
        return;
      }

      if  (shortName.ToLower () == "none")
        portAssignments[serialPortNum] = null;
      else
        portAssignments[serialPortNum] = new PortAssignment (shortName, description);
    }


    public string    CruiseLocation     ()  {return cruiseLocation;}
    public string    Objective          ()  {return objective;}
    public string    Principal          ()  {return principal;}
    public string    ResearchOrg        ()  {return researchOrg;}
    public DateTime  CruiseDateStart    ()  {return cruiseDateStart;}
    public DateTime  CruiseDateEnd      ()  {return cruiseDateEnd;}
    public string    DeploymentVehicle  ()  {return deploymentVehicle;}
    public string    SeaConditions      ()  {return seaConditions;}
    public string    WaveHeight         ()  {return waveHeight;}
    public string    WeatherConditions  ()  {return weatherConditions;}
    public string    AirTemp            ()  {return airTemp;}
    public double    Latitude           ()  {return latitude;}
    public double    Longitude          ()  {return longitude; }
    public int       ScanRate           ()  {return scanRate; }
    public string    SipperFileName     ()  {return sipperFileName;}

    public string    CtdExt0Code        ()  {return ctdExt0Code;}
    public string    CtdExt1Code        ()  {return ctdExt1Code;}
    public string    CtdExt2Code        ()  {return ctdExt2Code;}
    public string    CtdExt3Code        ()  {return ctdExt3Code;}
    


    public  string  SerialPortShortName (int  serialPortNum)
    {
      if  ((serialPortNum < 0)  ||  (serialPortNum >= 6))
        return "";

      if  (portAssignments[serialPortNum] == null)
        return "";

      return  portAssignments[serialPortNum].ShortName ();
    }


    public  string  SerialPortDesc (int  serialPortNum)
    {
      if  ((serialPortNum < 0)  ||  (serialPortNum >= 6))
        return "";

      if  (portAssignments[serialPortNum] == null)
        return "";

      return  portAssignments[serialPortNum].Desc ();
    }


    public Instrument InclinationMeter (ref int inclinationSerailPort)
    {
      inclinationSerailPort = -1;
      int  x;
      for  (x = 0;  x < portAssignments.Length;  x++)
      {
        if  (portAssignments[x] != null)
        {
          if  (portAssignments[x].Instrument ().ShortName () == "Inclination")
          {
            inclinationSerailPort = x;
            return  portAssignments[x].Instrument ();
          }
        }
      }
      
      return  null;
    }  /* InclinationMeter */


  }  /* SipperHeaderRec */
}