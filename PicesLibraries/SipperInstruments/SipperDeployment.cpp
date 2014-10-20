#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <memory>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <map>

#include  "MemoryDebug.h"
using namespace std;

#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;


#include "SipperDeployment.h"
using  namespace  SipperHardware;



SipperDeployment::SipperDeployment ():
    cruiseName          (),
    stationName         (),
    deploymentNum       (),
    description         (),
    latitude            (0),
    longitude           (0),
    dateTimeStart       (),
    dateTimeEnd         (),
    syncTimeStampActual (),
    syncTimeStampCTD    (),
    syncTimeStampGPS    (),
    cropLeft            (0),
    cropRight           (4095)
{
  DateTime  currentTime = osGetLocalDateTime ();
  dateTimeStart       = currentTime;
  dateTimeEnd         = currentTime;
  syncTimeStampActual = currentTime;
  syncTimeStampCTD    = currentTime;
  syncTimeStampGPS    = currentTime;
}



SipperDeployment::~SipperDeployment ()
{
}





SipperDeploymentList::SipperDeploymentList (bool _owner):
   KKQueue<SipperDeployment> (_owner)
{
}



SipperDeploymentList::~SipperDeploymentList ()
{
}

