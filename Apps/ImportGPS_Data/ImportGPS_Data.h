#if  !defined(_IMPORTGPS_DATA_)
#define  _IMPORTGPS_DATA_

#include "Application.h"
#include "DataBase.h"
#include "RunLog.h"
#include "Str.h"


namespace  ImportGPS_DataNameSpace
{
  class  SipperFileEntryList;
  typedef  SipperFileEntryList*  SipperFileEntryListPtr;

  class  ImportGPS_Data: public Application
  {
  public:
    typedef  enum  {GDF_NULL, GDF_WeatherBird, GDF_GPGGA, GDF_SpecialtyDiverI}  GpsDataFormat;

    ImportGPS_Data (int     argc, 
                    char**  argv
                   );

    ~ImportGPS_Data ();

    void  Main ();

  private:
    GpsDataFormat  GpsDataFormatFromStr (const KKStr& str);
    KKStr          GpsDataFormatToStr (GpsDataFormat  format);

    void   DisplayCommandLineParameters ();

    void   ImportGPSDataWeatherBird (const KKStr&  fileName);

    void   ImportGPSDataGPGGA (const KKStr&  fileName);

    void   ImportGPSDataSpecialtyDiverI (const KKStr&  fileName);

    bool   ProcessCmdLineParameter (char    parmSwitchCode, 
                                    KKStr   parmSwitch, 
                                    KKStr   parmValue
                                   );

    void  UpdateInstrumentData (const DateTime&  gpsTimeStamp,
                                double           lattitude,
                                double           longitude
                               );


    SipperCruisePtr          cruise;
    KKStr                    cruiseStr;
    DataBasePtr              dbConn;
    double                   deltaHours;
    SipperDeploymentListPtr  deployments;
    GpsDataFormat            fileFormat;
    SipperFileListPtr        sipperFiles;
    SipperFileEntryListPtr   sipperFileEntries;
    KKStr                    srcDirectory;
  };  /* ImportGPS_Data */
}  /* ImportGPS_DataNameSpace */

#endif
