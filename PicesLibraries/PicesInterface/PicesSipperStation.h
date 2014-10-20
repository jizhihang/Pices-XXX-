#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;


#include  "SipperStation.h"
using namespace  SipperHardware;

#include  "PicesRunLog.h"
#include  "PicesKKStr.h"
#include  "PicesSipperCruise.h"

namespace PicesInterface 
{
  public  ref class PicesSipperStation
  {
  public:
    PicesSipperStation (SipperStationPtr  _station);

    PicesSipperStation (PicesSipperCruise^  _cruise);  /// Creatuing a new station  that will belong to '_cruise'


  private:
    ~PicesSipperStation ();

  protected:
    !PicesSipperStation ();

  public:
    property  String^  StationName  {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (station->StationName ());}
                                     void     set (String^ _stationName)  {station->StationName (PicesKKStr::SystemStringToKKStr (_stationName));}
                                    }

    property  String^  CruiseName   {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (station->CruiseName  ());}
                                     void     set (String^ _cruiseName)   {station->CruiseName (PicesKKStr::SystemStringToKKStr (_cruiseName));}
                                    }

    property  String^  Description  {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (station->Description  ());}
                                     void     set (String^ _description)   {station->Description (PicesKKStr::SystemStringToKKStr (_description));}
                                    }

    property  double   Latitude  {double  get ()  {return  station->Latitude   ();}  void  set (double _latitude)   {station->Latitude  (_latitude );}}
    property  double   Longitude {double  get ()  {return  station->Longitude  ();}  void  set (double _longitude)  {station->Longitude (_longitude);}}

    property  System::DateTime  DateTimeStart  {System::DateTime  get ();  void  set (System::DateTime  _dateTimeStart);}
    
    property  String^  GPSString  {String^  get ();}

    SipperStationPtr  UnmanagedClass ()  {return  station;}

  private:
    SipperStationPtr  station;
  };




  public ref class  PicesSipperStationList:  public  List<PicesSipperStation^>
  {
  public:
    // Will take ownership of contents of 'stations'  but not the list itself.
    // the caler will still be responsable for deleting the list('stations').
    PicesSipperStationList (SipperStationListPtr  stations);

  private:
    ~PicesSipperStationList ();

  protected:
    !PicesSipperStationList ();
  };
};
