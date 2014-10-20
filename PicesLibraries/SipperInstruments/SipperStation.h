#ifndef  _SIPPERSTATION_
#define  _SIPPERSTATION_

#include  "DateTime.h"
#include  "KKQueue.h"
#include  "RunLog.h"

namespace  SipperHardware
{
  class SipperStation
  {
  public:
    typedef  SipperStation*  SipperStationPtr;

    SipperStation ();
    ~SipperStation ();

    const   KKStr&           CruiseName    () const  {return cruiseName;}
    const   KKStr&           StationName   () const  {return stationName;}
    const   KKStr&           Description   () const  {return description;}
    double                   Latitude      () const  {return latitude;}
    double                   Longitude     () const  {return longitude;}
    const   KKU::DateTime&   DateTimeStart () const  {return dateTimeStart;}

    void  CruiseName     (const KKStr&          _cruiseName)     {cruiseName    = _cruiseName;}
    void  StationName    (const KKStr&          _stationName)    {stationName   = _stationName;}
    void  Description    (const KKStr&          _description)    {description   = _description;}
    void  Latitude       (double                _latitude)       {latitude      = _latitude;}
    void  Longitude      (double                _longitude)      {longitude     = _longitude;}
    void  DateTimeStart  (const KKU::DateTime&  _dateTimeStart)  {dateTimeStart = _dateTimeStart;}

  private:
    KKStr          cruiseName;
    KKStr          stationName;
    KKStr          description;
    double         latitude;
    double         longitude;
    KKU::DateTime  dateTimeStart;
  };  /* SipperStation */


  typedef  SipperStation::SipperStationPtr   SipperStationPtr;



  class  SipperStationList:  public  KKQueue<SipperStation>
  {
  public:
    typedef  SipperStationList*  SipperStationListPtr;
  
    SipperStationList (bool  _owner = true);
    ~SipperStationList ();
  
  };
  

  typedef  SipperStationList::SipperStationListPtr   SipperStationListPtr;
}  /* SipperHardware;  */

#endif
