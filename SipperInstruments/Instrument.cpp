#include  "FirstIncludes.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include  "BasicTypes.h"
using namespace KKU;


#include  "Instrument.h"

#include  "InstrumentDataManager.h"
#include  "InstrumentDataReport.h"
#include  "InstrumentDataCTDplus.h"
#include  "InstrumentDataBatteryMeter.h"
#include  "InstrumentDataPitchAndRoll.h"

using  namespace  SipperHardware;


// The FinalCleanUp  method will be registered with atexit in the 'InstrumentOther' constructor.
bool  Instrument::needToRunFinalCleanUp = true;


InstrumentPtr  Instrument::instruments[] = {new InstrumentCTD (), 
                                            new InstrumentGPS (),
                                            new InstrumentInclinationMeter (),
                                            new InstrumentBatteryMeters (),
                                            new InstrumentOther (),
                                            NULL
                                           };


Instrument::Instrument (const KKStr&  _shortName,
                        const KKStr&  _description
                       ):
      shortName   (_shortName),
      description (_description)
    {}




Instrument::~Instrument ()
{
}



bool  Instrument::Other ()  const
{
  return false;
}



InstrumentPtr  Instrument::LookUpByShortName (const KKStr&  _shortName)
{
  KKStr  shortNameLower = _shortName.ToUpper ();

  //if  (instruments == NULL)
  //   BuildInstrumentList ();

  int32  x = 0;

  while  (instruments[x] != NULL)
  {
    if  (instruments[x]->ShortName () == shortNameLower)
      return  instruments[x];
    x++;
  }  /* LookUpByShortName */

  return NULL;
}  /* LookUpByShortName */




InstrumentPtr  Instrument::OtherInstrument ()
{
  return  LookUpByShortName ("Other");
}



void  Instrument::FinalCleanUp ()
{
  if  (!needToRunFinalCleanUp)
    return;

  int32 x = 0;
  while (instruments[x] != NULL)
  {
    delete  instruments[x];  instruments[x] = NULL;
    x++;
  }
}
 


InstrumentCTD::~InstrumentCTD ()
{
}


InstrumentDataReportPtr  InstrumentCTD::CreateInstrumentDataReporter 
                                        (InstrumentDataManagerPtr _manager,
                                         const KKStr&             _reportDir,
                                         int32                    _instrumentId
                                        )
{
  return  new InstrumentDataCTDplus (_manager, _reportDir, false, _instrumentId);
}



InstrumentGPS::~InstrumentGPS ()
{
}


InstrumentDataReportPtr  InstrumentGPS::CreateInstrumentDataReporter 
                                        (InstrumentDataManagerPtr _manager,
                                         const KKStr&             _reportDir,
                                         int32                    _instrumentId
                                        )
{
  return  new InstrumentDataReport (_manager, _reportDir, false, _instrumentId, true);
}




InstrumentInclinationMeter::~InstrumentInclinationMeter ()
{
}


InstrumentDataReportPtr  InstrumentInclinationMeter::CreateInstrumentDataReporter 
                                        (InstrumentDataManagerPtr _manager,
                                         const KKStr&             _reportDir,
                                         int32                    _instrumentId
                                        )
{
  return  new InstrumentDataPitchAndRoll (_manager, _reportDir, false, _instrumentId);
}





InstrumentBatteryMeters::~InstrumentBatteryMeters ()
{
}


InstrumentDataReportPtr  InstrumentBatteryMeters::CreateInstrumentDataReporter 
                                        (InstrumentDataManagerPtr _manager,
                                         const KKStr&             _reportDir,
                                         int32                    _instrumentId
                                        )
{
  return  new InstrumentDataBatteryMeter (_manager, _reportDir, false, _instrumentId);
}



InstrumentOther::InstrumentOther (): 
   Instrument ("OTH", "Other")
{
  atexit (Instrument::FinalCleanUp);
}



InstrumentOther::~InstrumentOther ()
{
}


InstrumentDataReportPtr  InstrumentOther::CreateInstrumentDataReporter 
                                        (InstrumentDataManagerPtr _manager,
                                         const KKStr&             _reportDir,
                                         int32                    _instrumentId
                                        )
{
  return  new InstrumentDataBatteryMeter (_manager, _reportDir, false, _instrumentId);
}
