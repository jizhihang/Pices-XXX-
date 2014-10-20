#ifndef  _SIPPERHEADERREC_
#define  _SIPPERHEADERREC_

#include  "DateTime.h"
#include  "Str.h"

#include  "Instrument.h"

//********************************************************************************************
//*  This 'class' is meant to deal with the header info at the start of each Sipper File.    *
//*  This header info is in the form of a User Text message encoded as instrument data. The  *
//*  'InstrumentDataUserMsgs' object is responsible for extracting this text.  This text can *
//*  then be processed by the 'ParseHeaderStr' method.  There is a corresponding c# class in *
//*  the 'SipperFile' library.                                                               *
//********************************************************************************************


namespace  SipperHardware
{
  class PortAssignment
  {
  public: 
    typedef  PortAssignment*  PortAssignmentPtr;
  
    PortAssignment (InstrumentPtr  _instrument,
                    const KKStr&  _desc
                   );
  
    PortAssignment (const KKStr&  _shortName,
                    const KKStr&  _desc
                   );
  
    PortAssignment (const PortAssignment&  _portAssignment);
  
    const KKStr&         Desc       ()  const  {return  desc;}
    KKStr                HeaderStr  ()  const;
    const InstrumentPtr  Instrument ()  const  {return  instrument;}
    const KKStr&         ShortName  ()  const;
  
  private:
    InstrumentPtr  instrument;
    KKStr          desc;
  };  /* PortAssignment */




  typedef  PortAssignment::PortAssignmentPtr  PortAssignmentPtr;


  typedef  vector<PortAssignmentPtr>  PortAssignmentList;



  class SipperHeaderRec
  {
  public:
    typedef  SipperHeaderRec*  SipperHeaderRecPtr;
    typedef  KKU::uint32  uint32;

    static const int32 MaxHeaderLen = 456;
    static const int32 MaxSipperFileNameLen = 30;
  
  
	  SipperHeaderRec ();
  
    SipperHeaderRec (const SipperHeaderRec&  _headerRec);
  
	  ~SipperHeaderRec ();
  
  
    const PortAssignmentList&  PortAssignments () const  {return  portAssignments;}
      
    // Builds Header string that will be placed at beginning of SIPPER file
    // with the 'sfd' command.
    KKStr  HeaderStr ();
     
    void  ParseHeaderStr (const KKStr&  s);
  
  
    void  ProcessNameAndDataStrings (const KKStr&  fieldName,
                                     const KKStr&  fieldValue,
                                     bool&         found
                                    );
  
    void  Save (ostream& w);
  
    void  CruiseLocation     (KKStr    _cruiseLocation)     {cruiseLocation    = _cruiseLocation;}
    void  Objective          (KKStr    _objective)          {objective         = _objective;}
    void  Principal          (KKStr    _principal)          {principal         = _principal;}
    void  ResearchOrg        (KKStr    _researchOrg)        {researchOrg       = _researchOrg;}
    void  CruiseDateStart    (DateType _cruiseDateStart)    {cruiseDateStart   = _cruiseDateStart;}
    void  CruiseDateEnd      (DateType _cruiseDateEnd)      {cruiseDateEnd     = _cruiseDateEnd;}
    void  DeploymentVehicle  (KKStr    _deploymentVehicle)  {deploymentVehicle = _deploymentVehicle;}
    void  SeaConditions      (KKStr    _seaConditions)      {seaConditions     = _seaConditions;}
    void  WaveHeight         (KKStr    _waveHeight)         {waveHeight        = _waveHeight;}
    void  WeatherConditions  (KKStr    _weatherConditions)  {weatherConditions = _weatherConditions;}
    void  AirTemp            (KKStr    _airTemp)            {airTemp           = _airTemp;}
    void  Latitude           (double   _latitude)           {latitude          = _latitude; }
    void  Longitude          (double   _longitude)          {longitude         = _longitude; }
    void  ScanRate           (int32    _scanRate)           {scanRate          = _scanRate; }
    void  SipperFileName     (KKStr    _sipperFileName)     {sipperFileName    = _sipperFileName;}
  
    void  CtdExt0Code        (const KKStr&  _ctdExt0Code)   {ctdExt0Code       = _ctdExt0Code;}
    void  CtdExt1Code        (const KKStr&  _ctdExt1Code)   {ctdExt1Code       = _ctdExt1Code;}
    void  CtdExt2Code        (const KKStr&  _ctdExt2Code)   {ctdExt2Code       = _ctdExt2Code;}
    void  CtdExt3Code        (const KKStr&  _ctdExt3Code)   {ctdExt3Code       = _ctdExt3Code;}
  
  
    KKStr      CruiseLocation     () const  {return cruiseLocation;}
    KKStr      Objective          () const  {return objective;}
    KKStr      Principal          () const  {return principal;}
    KKStr      ResearchOrg        () const  {return researchOrg;}
    DateType   CruiseDateStart    () const  {return cruiseDateStart;}
    DateType   CruiseDateEnd      () const  {return cruiseDateEnd;}
    KKStr      DeploymentVehicle  () const  {return deploymentVehicle;}
    KKStr      SeaConditions      () const  {return seaConditions;}
    KKStr      WaveHeight         () const  {return waveHeight;}
    KKStr      WeatherConditions  () const  {return weatherConditions;}
    KKStr      AirTemp            () const  {return airTemp;}
    double     Latitude           () const  {return latitude;}
    double     Longitude          () const  {return longitude; }
    int32      ScanRate           () const  {return scanRate; }
    KKStr      SipperFileName     () const  {return sipperFileName;}
  
    KKStr      CtdExt0Code        () const  {return ctdExt0Code;}
    KKStr      CtdExt1Code        () const  {return ctdExt1Code;}
    KKStr      CtdExt2Code        () const  {return ctdExt2Code;}
    KKStr      CtdExt3Code        () const  {return ctdExt3Code;}
  
    InstrumentPtr   InclinationMeter     (uint32 inclinationSerailPort)  const;
    InstrumentPtr   SerialPortAssignment (uint32 serialPort)             const;
    const KKStr&    SerialPortShortName  (uint32 serialPortNum)          const;
    const KKStr&    SerialPortDesc       (uint32 serialPortNum)          const;
   
  
    SipperHeaderRec&  operator= (const SipperHeaderRec&  right);
  
  private:
    void  ClearOutExistingPortAssignments ();
    void  InitFields ();
  
    KKStr              sipperFileName;
    KKStr              cruiseLocation;
    KKStr              objective;
    KKStr              principal;
    KKStr              researchOrg;
    DateType           cruiseDateStart;
    DateType           cruiseDateEnd;
    KKStr              deploymentVehicle;
    KKStr              seaConditions;
    KKStr              waveHeight;
    KKStr              weatherConditions;
    KKStr              airTemp;
    double             latitude;
    double             longitude;
    int32              scanRate;
    PortAssignmentList portAssignments;
  
    KKStr              ctdExt0Code;
    KKStr              ctdExt1Code;
    KKStr              ctdExt2Code;
    KKStr              ctdExt3Code;
  };  /* SipperHeaderRec */


  typedef  SipperHeaderRec::SipperHeaderRecPtr  SipperHeaderRecPtr;
}  /* SipperHardware */

#endif
