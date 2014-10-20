#ifndef  _DATABASELOGENTRY_
#define  _DATABASELOGENTRY_

/**
 @class  MLL::DataBaseLogEntry
 @details 
 @code
 ************************************************************************************************
 **  Used for Maintaing LogEntries; specificaly is is used to indicate when Processes started   *
 **  and ended along with the command line used.  Each entry will be assigned a unique ID that  *
 **  other elements can refer to.  For example each entry in the Images table can refer to the  *
 **  log entry that generated the image.  Programs such as "GrayImageExtraction" and            *
 **  "ImageClassification" wil generate entries into this table.                                *
 ************************************************************************************************
 @endcode
 */

#ifndef  _mysql_h
struct  MYSQL;
struct  MYSQL_RES;
struct  MSQL_STRUCT; 
typedef  MSQL_STRUCT* MYSQL_ROW;
#endif

#include  "OSservices.h"
#include  "RunLog.h"
using namespace KKU;

namespace MLL 
{

  class  DataBaseLogEntry
  {
  public:
    typedef  DataBaseLogEntry*  DataBaseLogEntryPtr;

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;
    typedef  KKU::ulong    ulong;
    typedef  KKU::uchar    uchar;

    DataBaseLogEntry ();
    
    DataBaseLogEntry (const KKStr&          _progCode,
                      const KKStr&          _progName,
                      const KKU::DateTime&  _dateTimeCompiled,
                      const KKStr&          _cmdLine,
                      const KKStr&          _compName,
                      const KKStr&          _compUserName,
                      const KKStr&          _sipperFileName
                     );

    ~DataBaseLogEntry ();

    uint32         LogEntryId        () const  {return logEntryId;};
    const KKStr&   ProgCode          () const  {return progCode;} 
    const KKStr&   ProgName          () const  {return progName;} 
    KKU::DateTime  DateTimeCompiled  () const  {return dateTimeCompiled;} 
    const KKStr&   CmdLine           () const  {return cmdLine;} 
    const KKStr&   CompName          () const  {return compName;} 
    const KKStr&   DataBaseUserName  () const  {return dataBaseUserName;} 
    const KKStr&   CompUserName      () const  {return compUserName;} 
    KKU::DateTime  DateTimeStart     () const  {return dateTimeStart;} 
    KKU::DateTime  DateTimeStartUtc  () const  {return dateTimeStartUtc;} 
    KKU::DateTime  DateTimeEnd       () const  {return dateTimeEnd;};
    float          CpuTimeUsed       () const  {return cpuTimeUsed;} 
    const KKStr&   CompletionStatus  () const  {return completionStatus;}
    const KKStr&   SipperFileName    () const  {return sipperFileName;}

    void   LogEntryId        (uint32                _logEntryId)       {logEntryId       = _logEntryId;}
    void   ProgCode          (const KKStr&          _progCode)         {progCode         = _progCode;}
    void   ProgName          (const KKStr&          _progName)         {progName         = _progName;}
    void   DateTimeCompiled  (const KKU::DateTime&  _dateTimeCompiled) {dateTimeCompiled = _dateTimeCompiled;}
    void   CmdLine           (const KKStr&          _cmdLine)          {cmdLine          = _cmdLine;} 
    void   CompName          (const KKStr&          _compName)         {compName         = _compName;} 
    void   DataBaseUserName  (const KKStr&          _dataBaseUserName) {dataBaseUserName = _dataBaseUserName;} 
    void   CompUserName      (const KKStr&          _compUserName)     {compUserName     = _compUserName;} 
    void   DateTimeStart     (const KKU::DateTime&  _dateTimeStart)    {dateTimeStart    = _dateTimeStart;}
    void   DateTimeStartUtc  (const KKU::DateTime&  _dateTimeStartUtc) {dateTimeStartUtc = _dateTimeStartUtc;}
    void   DateTimeEnd       (const KKU::DateTime&  _dateTimeEnd)      {dateTimeEnd      = _dateTimeEnd;}
    void   CpuTimeUsed       (float                 _cpuTimeUsed)      {cpuTimeUsed      = _cpuTimeUsed;}
    void   CompletionStatus  (const KKStr&          _completionStatus) {completionStatus = _completionStatus;}
    void   SipperFileName    (const KKStr&          _sipperFileName)   {sipperFileName   = _sipperFileName;}

  private:
    uint32            logEntryId;
    KKStr             progCode;          /**< "IE"=Image Extraction, "CL"=Classification.    */
    KKStr             progName;          /**< Name of applicationperforming process.         */
    KKU::DateTime     dateTimeCompiled;  /**< DateTime program was compiled.                 */
    KKStr             cmdLine;           /**< Command line used to run program.              */
    KKStr             compName;          /**< Name of computer that program ran on.          */
    KKStr             dataBaseUserName;  /**< UserName used to connect to the database with. */
    KKStr             compUserName;      /**< UserNane used to log into computer with.       */
    KKU::DateTime     dateTimeStart;     /**< DateTime process started.                      */
    KKU::DateTime     dateTimeStartUtc;  /**< UTC DateTime process started.                  */
    KKU::DateTime     dateTimeEnd;
    float             cpuTimeUsed;       /**< Number of CPU seconds used surimng process.    */
    KKStr             completionStatus;  /**< Indicates status of process when completed.    */
    KKStr             sipperFileName;    /**< Root Name of Sipper file; if processed against one SIPPER file. */
  };  /* DataBaseLogEntry */



  typedef  DataBaseLogEntry::DataBaseLogEntryPtr  DataBaseLogEntryPtr;



  class  DataBaseLogEntryList: public  KKQueue<DataBaseLogEntry>
  {
  public:
    typedef  DataBaseLogEntryList*  DataBaseLogEntryListPtr;

    DataBaseLogEntryList (bool  _owner);

    ~DataBaseLogEntryList ();

    void  SortByProgCode ();

  private:
    class  ProgCodeComparison;
  };  /* DataBaseLogEntryList */


  typedef  DataBaseLogEntryList::DataBaseLogEntryListPtr  DataBaseLogEntryListPtr;


}  /* namespace MLL */


#endif
