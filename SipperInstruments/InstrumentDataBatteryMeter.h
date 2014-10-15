#ifndef  _INSTRUMENTDATABATTERYMETER_
#define  _INSTRUMENTDATABATTERYMETER_

#include  "Str.h"
using namespace  KKU;


#include  "InstrumentDataReport.h"

namespace SipperHardware
{
class InstrumentDataBatteryMeter: public InstrumentDataReport
{
public:
  typedef  InstrumentDataBatteryMeter*  InstrumentDataBatteryMeterPtr;

  InstrumentDataBatteryMeter (InstrumentDataManagerPtr _manager,
                              const KKStr&            _reportDir,
                              bool                     _text,
                              int32                    _instrumentId
                             );
  virtual
    ~InstrumentDataBatteryMeter ();


  virtual  
    void  ReportInstrumentData (uint32 curScanLine,
                                uchar  sensorData
                               );

protected:
  void  ProcessBatteryData (const KKStr&  txt);

  VectorFloat  batteryLevels;
  KKStr        curTextLine;
  int32        curTextLineStartScanLine;
  KKStr        lastTextLine;
  uint32       numOfBatteries;
  char         prevEolChar;
};


typedef  InstrumentDataBatteryMeter::InstrumentDataBatteryMeterPtr  InstrumentDataBatteryMeterPtr;
}  /* namespace SipperHardware */

#endif
