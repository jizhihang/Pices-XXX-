#ifndef  _INSTRUMENTDATAREPORT_
#define  _INSTRUMENTDATAREPORT_

#include  "Str.h"
using namespace KKU;

namespace SipperHardware
{
  #ifndef _INSTRUMENTDATAMANAGER_
  class InstrumentDataManager;
  typedef  InstrumentDataManager*  InstrumentDataManagerPtr;
  #endif


  class InstrumentDataReport
  {
  public:
    typedef  InstrumentDataReport*  InstrumentDataReportPtr;
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    InstrumentDataReport (InstrumentDataManagerPtr  _manager,
                          const KKStr&              _reportDir,
                          bool                      _text,
                          int32                     _instrumentId,
                          bool                      _useReportWithLineNumbers
                         );
    virtual
      ~InstrumentDataReport ();


    const KKStr&   CurTextLine  ()  {return curTextLine;}
    const KKStr&   LastTextLine ()  {return lastTextLine;}



    /**
     * @brief A virtual Constructor for InstrumentDataReport
     * @param[in]  _reportDir    Directory where report file is to be placed.  
     * @param[in]  text,         Set to true = text data,  false = binary data.
     * @param[in]  InstrumentID, Sensor/Instrument ID
     *
     *    In the future as different types of instruments are introduced we can make
     *    specialized versions of InstrumentDataReport.
    */
    static  
      InstrumentDataReportPtr  CreateInstrumentDataReport (InstrumentDataManagerPtr _manager,
                                                           const KKStr&             _reportDir,
                                                           bool                     _text,
                                                           int32                    _instrumentId
                                                          );

    virtual  
      void  ReportInstrumentData (uint32 curScanLine,
                                  uchar  sensorData
                                 );

  protected:
    KKStr  InstrumentName ();
    
    static  const char*  InstrumentNames[];
  
    KKStr    curTextLine;      /**< Used where "text == true",  Every time a character
                                * is read via 'ReportInstrumentData' will append to
                                * end of string.  When a '\n' is processed will move
                                * to 'lastTextLine' and then set to empty.
                                */
    int32    instrumentId;
    
    uint32   lastScanLineReported;
  
    KKStr    lastTextLine;    /**< Used where "text == true",  will keep an instance of last complete text line 
                               * read.  Will come from curText line when 
                               * ever a '\n' character is read.
                               */
  
    InstrumentDataManagerPtr  manager;
    KKStr                     reportFileName;
    KKStr                     reportFileNameWLN;
    ofstream                  report;
    ofstream                  reportWLN;
    bool                      text;
    bool                      useReportWithLineNumbers;
  };
  
  typedef  InstrumentDataReport::InstrumentDataReportPtr  InstrumentDataReportPtr;
}  /* SipperHardware */

#endif
