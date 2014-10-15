#if  !defined(_SIPPERDEPLOYMENT_)
#define  _SIPPERDEPLOYMENT_

#include  "DateTime.h"
#include  "KKQueue.h"
#include  "RunLog.h"
using  namespace  KKU;

namespace  SipperHardware
{
  class SipperDeployment
  {
  public:
    typedef  SipperDeployment*  SipperDeploymentPtr;
  
    SipperDeployment ();
    ~SipperDeployment ();
  
    const   KKStr&           CruiseName          () const  {return cruiseName;}
    const   KKStr&           StationName         () const  {return stationName;}
    const   KKStr&           DeploymentNum       () const  {return deploymentNum;}
    const   KKStr&           Description         () const  {return description;}
    double                   Latitude            () const  {return latitude;}
    double                   Longitude           () const  {return longitude;}
    const   KKU::DateTime&   DateTimeStart       () const  {return dateTimeStart;}
    const   KKU::DateTime&   DateTimeEnd         () const  {return dateTimeEnd;}
    const   KKU::DateTime&   SyncTimeStampActual () const  {return syncTimeStampActual;}
    const   KKU::DateTime&   SyncTimeStampCTD    () const  {return syncTimeStampCTD;}
    const   KKU::DateTime&   SyncTimeStampGPS    () const  {return syncTimeStampGPS;}
    KKU::uint32              CropLeft            () const  {return cropLeft;}
    KKU::uint32              CropRight           () const  {return cropRight;}
  
    void   CruiseName          (const KKStr&          _cruiseName)          {cruiseName          = _cruiseName;}
    void   StationName         (const KKStr&          _stationName)         {stationName         = _stationName;}
    void   DeploymentNum       (const KKStr&          _deploymentNum)       {deploymentNum       = _deploymentNum;}
    void   Description         (const KKStr&          _description)         {description         = _description;}
    void   Latitude            (double                _latitude)            {latitude            = _latitude;}
    void   Longitude           (double                _longitude)           {longitude           = _longitude;}
    void   DateTimeStart       (const KKU::DateTime&  _dateTimeStart)       {dateTimeStart       = _dateTimeStart;}
    void   DateTimeEnd         (const KKU::DateTime&  _dateTimeEnd)         {dateTimeEnd         = _dateTimeEnd;}
    void   SyncTimeStampActual (const KKU::DateTime&  _syncTimeStampActual) {syncTimeStampActual = _syncTimeStampActual;}
    void   SyncTimeStampCTD    (const KKU::DateTime&  _syncTimeStampCTD)    {syncTimeStampCTD    = _syncTimeStampCTD;}
    void   SyncTimeStampGPS    (const KKU::DateTime&  _syncTimeStampGPS)    {syncTimeStampGPS    = _syncTimeStampGPS;}
    void   CropLeft            (KKU::uint32           _cropLeft)            {cropLeft            = _cropLeft;}
    void   CropRight           (KKU::uint32           _cropRight)           {cropRight           = _cropRight;}

  private:
    KKStr          cruiseName;
    KKStr          stationName;
    KKStr          deploymentNum;
    KKStr          description;
    KKU::DateTime  dateTimeStart;
    KKU::DateTime  dateTimeEnd;
    double         latitude;
    double         longitude;
    KKU::DateTime  syncTimeStampActual;
    KKU::DateTime  syncTimeStampCTD;
    KKU::DateTime  syncTimeStampGPS;
    KKU::uint32    cropLeft;
    KKU::uint32    cropRight;
  };  /* SipperDeployment */



  typedef  SipperDeployment::SipperDeploymentPtr   SipperDeploymentPtr;



  class  SipperDeploymentList:  public  KKQueue<SipperDeployment>
  {
  public:
    typedef  SipperDeploymentList*  SipperDeploymentListPtr;
  
    SipperDeploymentList (bool  _owner = true);
    ~SipperDeploymentList ();
  
  };


  typedef  SipperDeploymentList::SipperDeploymentListPtr   SipperDeploymentListPtr;


}  /* SipperHardware */

#endif
