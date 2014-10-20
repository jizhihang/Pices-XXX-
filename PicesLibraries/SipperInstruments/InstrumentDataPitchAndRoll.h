#ifndef  _INSTRUMENTDATAPITCHANDROLL_
#define  _INSTRUMENTDATAPITCHANDROLL_

#include  "Str.h"
using namespace KKU;

#include  "InstrumentDataReport.h"


namespace SipperHardware
{
  class InstrumentDataPitchAndRoll: public  InstrumentDataReport
  {
  public:
    typedef  InstrumentDataPitchAndRoll*  InstrumentDataPitchAndRollPtr;

  
    InstrumentDataPitchAndRoll (InstrumentDataManagerPtr _manager,
                                const KKStr&            _reportDir,
                                bool                     _text,
                                int32                    _instrumentId
                               );
  
    virtual
      ~InstrumentDataPitchAndRoll ();
  
  
  
    virtual  
      void  ReportInstrumentData (uint32 curScanLine,
                                  uchar  sensorData
                                 );
  
  private:
    virtual void  ProcessData (const KKStr&  txt);
  
  
    KKStr  curTextLine;
    int32   curTextLineStartScanLine;
    KKStr  lastTextLine;
    char    prevEolChar;
  };
  
  
  typedef  InstrumentDataPitchAndRoll::InstrumentDataPitchAndRollPtr  InstrumentDataPitchAndRollPtr;
}  /* namespace SipperHardware */

#endif
