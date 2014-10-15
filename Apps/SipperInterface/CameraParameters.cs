using System;
using System.Collections.Generic;
using System.Text;

using PicesInterface;
using SipperFile;


namespace SipperInterface
{
  public  class  CameraParameters
  {
    private string  modelNum = "";
    private string  serialNum       = "";
    private string  sensorSerialNum = "";
    private string  firmWareRev     = "";
    private string  dspDesignRev    = "";
    private float   scanRate        = 0.0f;
    private float   exposureTime    = 0.0f;
    


    public  float  ExposureTime  {get {return exposureTime;}  set {exposureTime = value;}}
    public  float  ScanRate      {get {return scanRate;}      set {scanRate     = value;}}


    
    public  CameraParameters (string parameterStr)
    {
      // parameterStr = String that is returned from a 'gcp' command to the camaera.
      ParseParameterStr (parameterStr);
    }




    private  void  ParseParameterStr (string parameterStr)
    {
      string[]  lines  = parameterStr.Split ('\n');
      
      int  lineNum = 0;
      for  (lineNum = 0;  lineNum < lines.Length;  lineNum++)
      {
        ParseLine (lines[lineNum]);
      }
    }  /* ParseParameterStr */
     
     
    private  void  ParseLine (string  ln)
    {
      string[]  fields = ln.Split (':');
       
      if  (fields.Length < 2)
        return;
         
      string  fieldName  = fields[0].Trim ().ToLower ();
      string  fieldValue = fields[1].Trim ();
       
      switch  (fieldName)
      {
        case  "camera model no.":     modelNum        = fieldValue;   break;
        case  "camera serial no.":    serialNum       = fieldValue;   break;
        case  "sensor serial no.":    sensorSerialNum = fieldValue;   break;
        case  "firmware design rev.": firmWareRev     = fieldValue;   break;
        case  "dsp design rev.":      dspDesignRev    = fieldValue;   break;
        case  "sync frequency":       scanRate        = ParseSyncFreqField (fieldValue);  break;
        case  "exposure time:":       exposureTime    = ParseExposureField (fieldValue);  break;
       }
     
     }  /* ParseLine */
     
     
     private  float  ParseSyncFreqField (string  s)
     {
       float  scanRate = 0.0f;
       
       int  x = s.IndexOf ('(');
       
       if  (x > 0)
       {
         s = s.Substring (x + 1);
         x = s.IndexOf (')');
         
         if  (x > 0)
         {
           s = s.Substring (0, x);
           scanRate = PicesKKStr.StrToFloat (s);
         }
       }

       return scanRate;           
     }  /* ParseSyncFreqField */
     
     
     
     private  float  ParseExposureField (string  s)
     {
       float  exposureTime = 0.0f;
       
       int  x = s.IndexOf ("uSec");
       if  (x > 0)
       {
         s = s.Substring (0, x).Trim ();
         exposureTime = PicesKKStr.StrToFloat (s);
       }  
       
       return  exposureTime;
     }  /* ParseExposureField */
     
       
  }  /* CameraParameters */
  
}
