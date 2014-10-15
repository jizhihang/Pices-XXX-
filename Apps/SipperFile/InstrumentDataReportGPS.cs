using System;
using System.Collections.Generic;
using System.Text;
using PicesInterface;


namespace SipperFile
{
	/// <summary>
	/// Summary description for InstrumentDataReportCTD.
	/// </summary>
	public class  InstrumentDataReportGPS:  InstrumentDataReport
	{


    IFormatProvider intFormat = new System.Globalization.CultureInfo ("", true);


    public InstrumentDataReportGPS (InstrumentDataManager _manager,
		                                byte                  _deviceId
                                   ):
                                   base(_manager, _deviceId)
		{
		}
 




    override public void  ParseTxtLine (string  txtLine)
    {
      if  (txtLine.Length < 5)
        return;
        
      string[]  fields = txtLine.Split (',');
      if  (fields[0] == "$GPGLL")
        ProcessGPGLL (fields);
    }


    private  void  ParseDegreeMinutesStr (string       str,
                                          ref double   degrees,
                                          ref double   minutes
                                         )
    {
      degrees = 0.0;
      minutes = 0.0;

      int x = str.IndexOf ('.');
      if  (x < 1)
        return;

      string  degStr = str.Substring (0, x - 2);

      string  minStr = str.Substring (x - 2);
  
      degrees = PicesKKStr.StrToDouble (degStr);
      minutes = PicesKKStr.StrToDouble (minStr);
    }  /* ParseDegreeMinutesStr */



    /**
     * @brief Processes a GPGLL strig that contains Laditude and Longitudal info.
     * @param[in] _str '$GPGLL' string with '$GPGLL' stripped off front.
     */
    private void  ProcessGPGLL (string[] fields)
    {
      if  ((fields[0] != "$GPGLL")  ||  (fields.Length < 6))
        return;
        
      double  latitudeDegrees = 0.0;
      double  latitudeMinutes = 0.0;
      ParseDegreeMinutesStr (fields[1], ref latitudeDegrees, ref latitudeMinutes);
      double  latitude = latitudeDegrees + (latitudeMinutes / 60.0);

      if  (fields[2] == "S")
        latitude = -latitude;

      double  longitudeDegrees = 0.0;
      double  longitudeMinutes = 0.0;
      ParseDegreeMinutesStr (fields[3], ref longitudeDegrees, ref longitudeMinutes);
      double  longitude = longitudeDegrees + (longitudeMinutes / 60.0);
      if  (fields[4] == "E")
        longitude = -longitude;

      DateTime  gpsTime = DateTime.ParseExact (fields[5], "hh:mm:ss", null);

      Manager ().CurrentDataRow ().UpdateGPS (latitude, longitude, gpsTime);
    }  /* ProcessGPGLL */
  }  /* InstrumentDataReportGPS */
}

