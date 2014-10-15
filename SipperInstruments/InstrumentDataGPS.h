#ifndef  _INSTRUMENTDATAGPS_
#define  _INSTRUMENTDATAGPS_

#include  "Str.h"
using namespace  KKU;


#include  "InstrumentDataReport.h"


namespace  SipperHardware
{
  class InstrumentDataGPS;
  typedef  InstrumentDataGPS*  InstrumentDataSerialPort0Ptr;


  class InstrumentDataGPS: public  InstrumentDataReport
  {
  public:
    InstrumentDataGPS (InstrumentDataManagerPtr _manager,
                       const KKStr&             _reportDir,
                       bool                     _text,
                       int32                    _instrumentId
                      );

    virtual
      ~InstrumentDataGPS ();
  


    virtual  
      void  ReportInstrumentData (uint32 curScanLine,
                                  uchar  sensorData
                                 );

  private:
    void  ParseDegreeMinutesStr (const KKStr&  str,
                                 double&        degrees,
                                 double&        minutes
                                )  const;

    void  ProcessGPGLL    (const KKStr& _str);
    void  ProcessNMEAInfo (const KKStr& _str);

    KKStr  curTextLine;
    int32  curTextLineStartScanLine;
    KKStr  lastTextLine;
  };
  

  typedef  InstrumentDataGPS*  InstrumentDataSerialPort0Ptr;
}  /* SipperHardware */

#endif
