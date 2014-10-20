#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <memory>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <map>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;

#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;


#include "SipperStation.h"
using  namespace  SipperHardware;


SipperStation::SipperStation ():
    cruiseName     (),
    stationName    (),
    description    (),
    latitude       (0),
    longitude      (0),
    dateTimeStart  (osGetLocalDateTime ())
{

}



SipperStation::~SipperStation ()
{
}





SipperStationList::SipperStationList (bool _owner):
   KKQueue<SipperStation> (_owner)
{
}



SipperStationList::~SipperStationList ()
{
}

