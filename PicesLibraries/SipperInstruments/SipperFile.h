#if  !defined(_SIPPERFILE_)
#define  _SIPPERFILE_

#include  "DateTime.h"
#include  "Instrument.h"
#include  "KKStrParser.h"
#include  "RunLog.h"


namespace  SipperHardware
{
  class  SipperFile
  {
  public:
    typedef  SipperFile*  SipperFilePtr;

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;
    typedef  KKU::int64   int64;
    typedef  KKU::uint64  uint64;

    SipperFile (const KKStr&  _sipperFileName);

    SipperFile (KKStrParser&  str);
  
    ~SipperFile ();

    void   AssignCtdExternalInstruments (const KKStr&   rootName);
  
    // Access Methods
    int32             SipperFileId   () const  {return sipperFileId;}
  
    const KKStr&      SipperFileName () const  {return  sipperFileName;}
    const KKStr&      CruiseName     () const  {return  cruiseName;}
    const KKStr&      StationName    () const  {return  stationName;}
    const KKStr&      DeploymentNum  () const  {return  deploymentNum;}
    const KKStr&      Description    () const  {return  description;}
    double            Latitude       () const  {return  latitude;}
    double            Longitude      () const  {return  longitude;}
  
    const KKU::DateTime&   DateTimeStart  () const  {return  dateTimeStart;}
     
    const InstrumentPtr  Sp0         () const {return  sp0;}
    const InstrumentPtr  Sp1         () const {return  sp1;}
    const InstrumentPtr  Sp2         () const {return  sp2;}
    const InstrumentPtr  Sp3         () const {return  sp3;}
  
    const KKStr&      CtdExt0        () const  {return  ctdExt0;}
    const KKStr&      CtdExt1        () const  {return  ctdExt1;}
    const KKStr&      CtdExt2        () const  {return  ctdExt2;}
    const KKStr&      CtdExt3        () const  {return  ctdExt3;}
     
    int64             SizeInBytes    () const  {return  sizeInBytes;}
    uint32            NumScanLines   () const  {return  numScanLines;}
  
    float             ScanRate       () const  {return  scanRate;}
  
    float             Depth          () const  {return  depth;}
  
    char    ExtractionStatus        ()  const  {return  extractionStatus;}
    uint32  ExtractionScanLineStart ()  const  {return  extractionScanLineStart;}
    uint32  ExtractionScanLineEnd   ()  const  {return  extractionScanLineEnd;}

    KKStr  Sp0ShortName  () const;
    KKStr  Sp1ShortName  () const;
    KKStr  Sp2ShortName  () const;
    KKStr  Sp3ShortName  () const;
  
    void  SipperFileId   (int32            _sipperFileId)       {sipperFileId   = _sipperFileId;}
    void  SipperFileName (const KKStr&     _sipperFileName)     {sipperFileName = _sipperFileName;}
    void  CruiseName     (const KKStr&     _cruiseName)         {cruiseName     = _cruiseName;}
    void  StationName    (const KKStr&     _stationName)        {stationName    = _stationName;}
    void  DeploymentNum  (const KKStr&     _deploymentNum)      {deploymentNum  = _deploymentNum;}
    void  Description    (const KKStr&     _description)        {description    = _description;}
    void  Latitude       (double           _latitude)           {latitude       = _latitude;}
    void  Longitude      (double           _longitde)          {longitude       = _longitde;}
    void  DateTimeStart  (const KKU::DateTime&  _dateTimeStart) {dateTimeStart  = _dateTimeStart;}
     
    void  Sp0            (const InstrumentPtr  _sp0)          {sp0            = _sp0;}
    void  Sp1            (const InstrumentPtr  _sp1)          {sp1            = _sp1;}
    void  Sp2            (const InstrumentPtr  _sp2)          {sp2            = _sp2;}
    void  Sp3            (const InstrumentPtr  _sp3)          {sp3            = _sp3;}
  
    void  CtdExt0        (const KKStr&     _ctdExt0)          {ctdExt0        = _ctdExt0;}
    void  CtdExt1        (const KKStr&     _ctdExt1)          {ctdExt1        = _ctdExt1;}
    void  CtdExt2        (const KKStr&     _ctdExt2)          {ctdExt2        = _ctdExt2;}
    void  CtdExt3        (const KKStr&     _ctdExt3)          {ctdExt3        = _ctdExt3;}
     
    void  SizeInBytes    (int64            _sizeInBytes)      {sizeInBytes    = _sizeInBytes;}
    void  NumScanLines   (uint32           _numScanLines)     {numScanLines   = _numScanLines;}
  
    void  ScanRate       (float            _scanRate)         {scanRate       = _scanRate;}
  
    void  Depth          (float            _depth)            {depth          = _depth;}
  
    void  ExtractionStatus        (char   _extractionStatus)        {extractionStatus        = _extractionStatus;}
    void  ExtractionScanLineStart (uint32 _extractionScanLineStart) {extractionScanLineStart = _extractionScanLineStart;}
    void  ExtractionScanLineEnd   (uint32 _extractionScanLineEnd)   {extractionScanLineEnd   = _extractionScanLineEnd;}

    KKStr   ToTabDelStr ()  const;
    void    ParseTabDelStr (KKStrParser&  parser);
    void    ReFresh (const SipperFile&  sf);
  
  private:
     void   AssignCtdExternalInstrumentsETP2007 (const KKStr&   rootName);
     void   AssignCtdExternalInstrumentsETP2008 (const KKStr&   rootName);

     int32          sipperFileId;
     KKStr          sipperFileName;
     KKStr          cruiseName;
     KKStr          stationName;
     KKStr          deploymentNum;
     KKStr          description;
     double         latitude;
     double         longitude;
     KKU::DateTime  dateTimeStart;
   
     InstrumentPtr  sp0;
     InstrumentPtr  sp1;
     InstrumentPtr  sp2;
     InstrumentPtr  sp3;
  
     KKStr          ctdExt0;
     KKStr          ctdExt1;
     KKStr          ctdExt2;
     KKStr          ctdExt3;
     
     int64          sizeInBytes;
     int32          numScanLines;
  
     float          scanRate;                //  Cammera Scan Rate  Lines/Sec
     float          depth;                   //  Meters
     char           extractionStatus;        //  '0=Not Extracted, 1=Extract, 2=Extracting, 3=Extracted'
     uint32         extractionScanLineStart;
     uint32         extractionScanLineEnd;
    };


  typedef  SipperFile::SipperFilePtr  SipperFilePtr;



  class  SipperFileList: public KKQueue<SipperFile>
  {
  public:
    typedef  SipperFileList*  SipperFileListPtr;
  
    SipperFileList (bool  _owner = true);
    ~SipperFileList ();

    SipperFilePtr  LookUpBySipperFileName (const KKStr&  sipperFileName);

    void  Save (const KKStr&  fileName);

    static
      SipperFileListPtr  GetListOfSipperFilesWeCanFind (RunLog&  runLog);
  
    static
      SipperFileListPtr  Load (const KKStr&  fileName);

  private:
  };

  typedef  SipperFileList::SipperFileListPtr  SipperFileListPtr;





  /**
   *@brief  Will maintain a global list of all sipper files encountered.  
   *@details  Should be initially loaded by controlling application from Database by calling 'AddSipperFileEntries'.
   */
  class  SipperFileOracle
  {
  public:
    static  SipperFilePtr  GetSipperFileEntry (const KKStr& sipperFileRootName);

    static  void  AddSipperFileEntry (SipperFilePtr  sipperFile);

    /**
     *@details  Giving the contents of a list of 'SipperFile' entries to the oracle including the list. Upon 
     * return sipperFiles will no longer own its contents.  If 'sipperFiles' did not does not own its contents 
     * to start with then duplicates of the, will be made.
     */
    static  void  AddSipperFileEntries (SipperFileListPtr  sipperFiles);

  private:
    static  map<KKStr,SipperFilePtr>  allSipperFiles;  /**<  A list of every sipper File ever created;  will own its contents. */
  };



}  /* SipperHardware */

#endif

