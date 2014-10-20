#ifndef  _INSTRUMENTDATAUSERMSGS_
#define  _INSTRUMENTDATAUSERMSGS_

#include  "Str.h"
using namespace  KKU;


#include  "InstrumentDataReport.h"

namespace  SipperHardware
{
  class InstrumentDataUserMsgs: public  InstrumentDataReport
  {
  public:
    typedef  InstrumentDataUserMsgs*  InstrumentDataUserMsgsPtr;


    InstrumentDataUserMsgs (InstrumentDataManagerPtr _manager,
                                const KKStr&            _reportDir,
                                bool                     _text,
                                int32                    _instrumentId
                               );

    virtual
      ~InstrumentDataUserMsgs ();


    virtual  
      void  ReportInstrumentData (uint32 curScanLine,
                                  uchar  sensorData
                                 );

  private:
    KKStr  curTextLine;
    int32   curTextLineStartScanLine;
    KKStr  lastTextLine;
    char    prevEolChar;
  };

  typedef  InstrumentDataUserMsgs::InstrumentDataUserMsgsPtr  InstrumentDataUserMsgsPtr;
}  /* SipperHardware */

#endif
