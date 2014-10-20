#ifndef  _INSTRUMENTDATAFLOWMETER1_
#define  _INSTRUMENTDATAFLOWMETER1_

#include  "Str.h"
using namespace KKU;

#include  "InstrumentDataReport.h"

namespace SipperHardware
{
  class InstrumentDataFlowMeter1;
  typedef  InstrumentDataFlowMeter1*  InstrumentDataFlowMeter1Ptr;

  #define  NumTurnSignalsToTrack 4
  #define  ScanLinesPerSecond    25000


  class InstrumentDataFlowMeter1: public  InstrumentDataReport
  {
  public:
    InstrumentDataFlowMeter1 (InstrumentDataManagerPtr _manager,
                              const KKStr&            _reportDir,
                              bool                     _text,
                              int32                    _instrumentId
                             );
  
    virtual
      ~InstrumentDataFlowMeter1 ();
  
    virtual  
      void  ReportInstrumentData (uint32 curScanLine,
                                  uchar  sensorData
                                 );
  
  private:
    int32  flowMeterCurSlot;        // Indicates the slot last used.
  
    int32  flowMeterTrailingSlot;   // Indicates the slot that is behind current slot.
  
    uint32*  flowMeterTurnScanLines;  // A circular buffer that will keep track of scan lines.
  
   };
  
  
  typedef  InstrumentDataFlowMeter1*  InstrumentDataFlowMeter1Ptr;
}  /* SipperHardware */

#endif
