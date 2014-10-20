//  because <my_global.h> needs to define some Macro's before windows.h  and stdlib.h  we need t include
// before  "FirstIncludes.h"
#include <errmsg.h>
#include <my_global.h>
#include <my_sys.h>
#include <mysql.h>

#undef min
#undef max


#include "FirstIncludes.h"

#include <ctype.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <windows.h>
#include <wingdi.h>
#include "MemoryDebug.h"
using namespace std;


// Base Library
#include "BasicTypes.h"
#include "OSservices.h"
#include "RasterSipper.h"
using namespace KKU;

// SipperInstruments Library
#include "InstrumentDataFileManager.h"
#include "SipperFile.h"
#include "SipperStation.h"
#include "SipperVariables.h"
#include "SipperCruise.h"
#include "VolumeSampledStat.h"
using namespace SipperHardware;


#include "FileDesc.h"
#include "FeatureFileIOPices.h"
#include "ImageFeatures.h"

#include "DataBase.h"
#include "DataBaseServer.h"
using  namespace  MLL;




KKU::int32   CharStarToInt32 (const char* s)
{
  if  (s == NULL)
    return 0;
  return (KKU::int32)atoi (s);
}  /* CharStarToInt32 */



KKU::uint32   CharStarToUint32 (const char* s)
{
  if  (s == NULL)  return 0;
  return (KKU::uint32)atol (s);
}  /* CharStarToUint32 */



KKU::ulong  CharStarToUlong (const char*  s)
{
  if  (s == NULL)  return 0;
  KKU::ulong  l = (KKU::ulong)atol (s);
  return l;
}  /* CharStarToUlong */



KKU::int64  CharStarToInt64 (const char*  s)
{
  if  (s == NULL)  return 0;
  #if  defined(__GNUC__)
    KKU::int64  l = (KKU::int64)atoll (s);
  #else
    KKU::int64  l = (KKU::int64)_atoi64 (s);
  #endif
  return l;
}  /* CharStarToInt64 */



KKU::uint64  CharStarToUint64 (const char*  s)
{
  if  (s == NULL)  return 0;

  #if  defined(__GNUC__)
    KKU::uint64  l = (KKU::uint64)atoll (s);
  #else
    KKU::uint64  l = (KKU::uint64)_atoi64 (s);
  #endif
  return l;
}  /* CharStarToUint64 */



float  CharStarToFloat (const char*  s)
{
  if  (s == NULL)  return 0.0f;
  return  (float)atof (s);
}  /* CharStarToFloat */



double  CharStarToDouble (const char*  s)
{
  if  (s == NULL)  return 0.0;
  return  atof (s);
}  /* CharStarToDouble */




void  DataBase::WriteBuff (char*        dest,
                           KKU::int32   destSize,
                           const char*  src
                          )
{
  memset (dest, 0, destSize);
  if  (strlen (src) < (size_t)destSize)
  {
    STRCOPY (dest, destSize, src);
  }
  else
  {
    KKU::STRCOPY (dest, destSize - 1, src);
    dest[destSize - 1] = 0;
  }
  return;
}



KKStr  DataBase::FloatToStr (float f)
{
  KKStr  s (12);
  switch  (_fpclass (f))
  {
  case  _FPCLASS_PINF:
  case  _FPCLASS_NINF:
  case  _FPCLASS_SNAN:
  case  _FPCLASS_QNAN:
    s = "0.0";
    break;

  default:
    s << f; 
  }
  return  s;  
}  /* FloatToStr */



GoalKeeperPtr            DataBase::blocker                  = NULL;
volatile KKU::int32      DataBase::numDataBaseInstances     = 0;
volatile bool            DataBase::mySqlEmbeddedServerInitailzied = false;
KKU::KKStr               DataBase::mySqlHomeDir             = "";
KKU::KKStr               DataBase::mySqlDataDir             = "";
KKU::KKStr               DataBase::mySqlErroLogFileName     = "";
volatile bool            DataBase::staticVariablesInialized = false;
DataBase::DataBaseIndex  DataBase::existingInstances;



void  DataBase::Initialization ()
{
  CreateBlocker ();
  blocker->StartBlock ();
  InitializeDataBaseStaticVariables ();
  blocker->EndBlock ();
}


bool  DataBase::EmbeddedServerRunning ()
{
  return  mySqlEmbeddedServerInitailzied;
}



// Will initiate an instance of "GoalKeeper" if "blocker" does not already
// point one.
void  DataBase::CreateBlocker ()
{
  if  (!blocker)
    GoalKeeper::Create ("DataBaseBlocker", blocker);  // Will handle Race condition.
}


void   DataBase::ErrorLogMsg (const KKStr&  msg)
{
  if  (blocker)
    blocker->StartBlock ();

  ofstream  o (mySqlErroLogFileName.Str (), ios::app);
  o << endl << osGetThreadId () << ": " << osGetLocalDateTime () << ": " << msg << endl;
  o.close ();

  if  (blocker)
    blocker->EndBlock ();
}  /* WriteErrorLogMsg */



void   DataBase::UpdatesNotAllowed (const KKStr&  methodName)
{
  ErrorLogMsg ("Attemp to perform Update:" + methodName);
}




void  DataBase::InitializeDataBaseStaticVariables ()
{
  if  (!staticVariablesInialized)
  {
    CreateBlocker ();
    blocker->StartBlock ();

    if  (!staticVariablesInialized)
    {
      KKStr  picesHomeDir = SipperVariables::PicesHomeDir ();
      if  (picesHomeDir.Empty ())
      {
        KKStr  errMsg = "DataBase::InitializeDataBaseStaticVariables    ***ERROR***       Can not locate the PICES home directory.    Please make sure that the Environment variable \"PicesHomeDir\" is set to the correct location.";
        cerr << endl << errMsg << endl << endl;
        osDisplayWarning (errMsg);
        exit (-1);
      }

      mySqlHomeDir = osAddSlash (osAddSlash (picesHomeDir) + "MySql");
      mySqlDataDir = osAddSlash (mySqlHomeDir) + "data";
      mySqlErroLogFileName = osAddSlash (mySqlHomeDir) + "DataBase_ErrorLog.txt";
      osCreateDirectoryPath (mySqlHomeDir);
      staticVariablesInialized = true;
      InitializeMySqlLibrary ();
      atexit (DataBase::FinalCleanUp);
    }

    blocker->EndBlock ();
  }
}



void  DataBase::InitializeMySqlLibrary ()
{
#if  defined(EMBEDDED_MYSQL)
  if  (osValidDirectory (mySqlDataDir))
    InitializeMySqlLibraryEmbedded ();
  else
    InitializeMySqlLibraryServer ();
#else
  InitializeMySqlLibraryServer ();
#endif
}



void  DataBase::InitializeMySqlLibraryServer ()
{
  cout << "DataBase::InitializeMySqlLibraryServer" << endl;
  int32  retVal = mysql_library_init (-1, NULL, NULL);
  if  (retVal == 0)
  {
    bool  threadSafe = mysql_thread_safe () != 0;
    if  (!threadSafe)
    {
      ErrorLogMsg ("DataBase::InitializeMySqlLibraryServer     ***NOT THREAD SAFE ***");
    }
  }
}  /* InitializeMySqlLibraryServer */


KKU::int32  initializeMySqlLibraryEmbeddedThreadId = 0;

void  DataBase::InitializeMySqlLibraryEmbedded ()
{
  KKStr  startDefaultDir = osGetCurrentDirectory ();

  cout << "DataBase::InitializeMySqlLibraryEmbedded" << endl;
  KKStr dataDir   = "--datadir="  + mySqlDataDir;
  KKStr baseDir   = "--basedir="  + mySqlHomeDir;
  KKStr language  = "--language=" + osAddSlash (osAddSlash (mySqlHomeDir) + "share") + "english";
  KKStr defaultsFile = osAddSlash (mySqlDataDir) + "my.ini";

  char**  server_options = new char*[10];
  server_options[0] = STRDUP ("mysql_test");
  server_options[1] = STRDUP (baseDir.Str ());
  server_options[2] = STRDUP (dataDir.Str ());
  server_options[3] = STRDUP (language.Str ());
  server_options[4] = STRDUP ("--tmp-table-size=256M");
  server_options[5] = STRDUP ("--innodb-flush-log-at-trx-commit=1");
  server_options[6] = STRDUP ("--innodb-buffer-pool-size=512M");
  server_options[7] = STRDUP ("--innodb_flush_method=normal");    /**<  This setting allowed me to run on 3TB external USB drive
                                                                   * see http://www.apachefriends.org/f/search.php?st=0&sk=t&sd=d&sr=posts&sid=24b3f455e3ba3c7ebe038d06957fe9d3&keywords=InnoDB&fid%5B%5D=16&ch=-1&start=75
                                                                   */
  server_options[8] = NULL;
  server_options[9] = NULL;
  int32  num_elements = 8;

  char*  server_groups[] = {"libmysqld_server", "client", "mysqld",  NULL};

  int32  retval = 0;

  // We want to make sure that when MySQl is shutdown (call to mysql_library_end()) it is done by the same thread.
  initializeMySqlLibraryEmbeddedThreadId = osGetThreadId ();

  retval = mysql_library_init (num_elements, 
                               server_options, 
                               (char **)server_groups
                              );
  if  (retval != 0)
  {
    KKStr  errMsg = mysql_error (NULL);
    KKStr  errMsgStr = "Call to \"mysql_library_init\"  Failed with retval[" + StrFormatInt (retval, "##0") + "]";
    ErrorLogMsg (errMsgStr);
    ErrorLogMsg ("Error Description = " + errMsg);
  }

  cout  << " DataBase::InitializeMySqlLibraryEmbedded   num_elements" << "\t" << num_elements << endl
        << "                                            retval"       << "\t" << retval       << endl;


  if  (retval == 0)
  {
    mySqlEmbeddedServerInitailzied = true;
    bool  threadSafe = mysql_thread_safe () != 0;
    if  (!threadSafe)
      ErrorLogMsg ("DataBase::InitializeMySqlLibraryEmbedded     ***NOT THREAD SAFE ***");
  }

  delete  server_options[0];
  delete  server_options[1];
  delete  server_options[2];
  delete  server_options;
  server_options = NULL;


  // The following code is attempting to duplicate a bug with "mysql_library_end" where it hangs under normal operation.
  /*
  RunLogPtr  runLog = new RunLog ();

  DataBasePtr dbConnFirstOne = new DataBase (*runLog);

  DataBasePtr dbConn = new DataBase (*runLog);

  SipperCruiseListPtr cruises = dbConn->SipperCruiseLoadAllCruises ();

  MLClassListPtr  classes = dbConn->MLClassLoadList ();

  DataBaseImageGroupListPtr  imageGroups = dbConn->ImageGroupLoad ();   // returns a list of all Group Assignments

  SipperStationListPtr  stations = dbConn->SipperStationLoad ("SD01");

  SipperDeploymentListPtr  deployments = dbConn->SipperDeploymentLoad ("SD01",
                                                                       "06day"
                                                                      );

  delete  dbConnFirstOne;  dbConnFirstOne = NULL;

  delete  dbConn;  dbConn = NULL;


  ThreadEnd ();


  try  {mysql_library_end ();}
  catch  (std::exception  e1)
  {
    cerr << endl << "DataBase::FinalCleanUp    ***EXCEPTION***    " << e1.what () << endl << endl;
  }
  catch  (...)
  {
    cerr << endl << "DataBase::FinalCleanUp    ***EXCEPTION***    " << endl << endl;
  }

  delete  runLog;  runLog = NULL;
*/


}  /* InitializeMySqlLibraryEmbedded */




/**
 *@details  When using Embeded server oinside a ".net" application you need to call the "FinalCleanUp" method at the same level
 *  that the Initialization method gets called.  If you wait until the "atexitt functon in "DateBase" gets called al the threads 
 *  that the MySQL sderver create will be terminated and "mysql_library_end"  will hang.
 */
void  DataBase::FinalCleanUp ()
{
  if  (!staticVariablesInialized)
  {
    // No need to run 'FinalCleanUp';  Either static variabls were never initialized or 'FinalCleanUp'
    // was not ran yet.
    return;
  }

  if  (!blocker)
  {
    // If the GoalKeeper instance 'blocker' is not defined then eithe the DataBase inistialization code never 
    // completed or some other thread had already deleted 'blocker'.  In either case we can not perform
    // the clean up code safely.
    return;
  }

  blocker->StartBlock ();
  if  (!staticVariablesInialized)
  {
    blocker->EndBlock ();
    return;
  }

  // If you made it this far;  then you are thhe thread that has to clean up the 'DataBase' static variables.
  staticVariablesInialized = false;
  DataBaseIndex::iterator  idx;
  for  (idx = existingInstances.begin ();  idx != existingInstances.end ();  ++idx)
  {
    DataBasePtr  db = idx->first;
    db->Close ();
  }

  existingInstances.clear ();

  if  (mySqlEmbeddedServerInitailzied)
  {
    // We only want to call 'mysql_library_end' if we initialized embedded server.

    int32  curThreadId = osGetThreadId ();
    if  (curThreadId != initializeMySqlLibraryEmbeddedThreadId)
    {
      KKStr errMsg (128);
      errMsg << "DataBase::FinalCleanUp    Wrong thread is calling 'mysql_library_end'" << endl
             << "   Initializatiob ThreadId[" << initializeMySqlLibraryEmbeddedThreadId << "]  CurrentThreadId[" << curThreadId << "]" << endl;

      ErrorLogMsg (errMsg);
    }


    try  {mysql_library_end ();}
    catch  (std::exception  e1)
    {
      cerr << endl << "DataBase::FinalCleanUp    ***EXCEPTION***    " << e1.what () << endl << endl;
    }
    catch  (...)
    {
      cerr << endl << "DataBase::FinalCleanUp    ***EXCEPTION***    " << endl << endl;
    }
  }

  if  (featureDataFieldNames)
  {
    int x = 0;
    while  (featureDataFieldNames[x] != NULL)
    {
      delete  featureDataFieldNames[x];  featureDataFieldNames[x] = NULL;
      ++x;
    }
    delete  featureDataFieldNames;
    featureDataFieldNames = NULL;
  }

  staticVariablesInialized = false;

  blocker->EndBlock ();
  delete  blocker;
  blocker = NULL;
}  /* FinalCleanUp */



void   DataBase::DataBaseAddInstance (DataBasePtr  _instance)
{
  existingInstances.insert (DataBaseIndexPair (_instance, _instance));
}



void   DataBase::DataBaseDeleteInstance (DataBasePtr  _instance)
{
  DataBaseIndex::iterator  idx;
  idx = existingInstances.find (_instance);
  if  (idx != existingInstances.end ())
    existingInstances.erase (idx);
}




DataBase::DataBase (RunLog&  _log):
  allowUpdates      (false),
  conn              (NULL),
  useEmbeddedServer (false),
  featureFileDesc   (NULL),
  resultSet         (NULL),
  server            (NULL),
  log               (_log)

{
  CreateBlocker ();
  blocker->StartBlock ();
  allowUpdates = SipperVariables::AllowUpdates ();
  InitializeDataBaseStaticVariables ();
  server = GetDefaultMySqlParameters ();
  int  returnCd = Connect ();
  valid  = (returnCd == 0);
  numDataBaseInstances++;
  DataBaseAddInstance (this);
  blocker->EndBlock ();
}



DataBase::DataBase (const DataBaseServerPtr  _server,
                    RunLog&                  _log
                   ):

  allowUpdates      (false),
  conn              (NULL),
  featureFileDesc   (NULL),
  resultSet         (NULL),
  server            (NULL),
  useEmbeddedServer (false),
  log               (_log)
  
{
  CreateBlocker ();
  blocker->StartBlock ();

  allowUpdates = SipperVariables::AllowUpdates ();

  InitializeDataBaseStaticVariables ();
  
  if  (_server)
  {
    server = new DataBaseServer (*_server);
  }
  else
  {
    log.Level (-1) << endl << endl << endl
                   << "DataBase::DataBase  ***ERROR***       No Server Parameters provided;  will use default ones." << endl
                   << endl;
    server = new DataBaseServer ();
  }

  int  returnCd = Connect ();
  valid  = (returnCd == 0);
  numDataBaseInstances++;
  DataBaseAddInstance (this);
  blocker->EndBlock ();
}




DataBase::~DataBase ()
{
  CreateBlocker ();
  blocker->StartBlock ();

  Close ();

  delete  server;
  server = NULL;
  numDataBaseInstances--;
  DataBaseDeleteInstance (this);
  blocker->EndBlock ();
}




void  DataBase::Close ()
{
  CreateBlocker ();
  blocker->StartBlock ();

  if  (conn)
  {
    mysql_close (conn);
    conn = NULL;
  }

  blocker->EndBlock ();
}  /* Close */




bool  DataBase::ThreadInit ()
{
  // http://bugs.mysql.com/bug.php?id=59082

  MYSQL*  tempPtr = NULL;
  tempPtr = mysql_init (tempPtr);
  //my_init ();
  if (!mysql_thread_safe ())
  {
    ErrorLogMsg ("DataBase::ThreadInit   \"mysql_thread_safe\"  returned 'false';        Major problem: libmysqlcompiled ThreadSafe?");
    return  false;
  }

  bool  successful = (bool)(mysql_thread_init () == 0);

  mysql_close(tempPtr);

  return  successful;
}  /* ThreadInit */



void  DataBase::ThreadEnd ()
{
  mysql_thread_end ();
}  /* ThreadEnd */



DataBaseServerListPtr  DataBase::GetListOfServers (RunLog&  _log)
{
  CreateBlocker ();
  blocker->StartBlock ();

  DataBaseServerListPtr  list = new DataBaseServerList (_log);

  blocker->EndBlock ();
  return  list;
}



DataBaseServerPtr  DataBase::GetDefaultServer (RunLog&  _log)
{
  DataBaseServerPtr  defaultSerever = NULL;
  CreateBlocker ();
  blocker->StartBlock ();

  DataBaseServerListPtr  list = new DataBaseServerList (_log);
  if  (list)
  {
    defaultSerever = list->GetDefaultServer ();
    if  (defaultSerever)
      defaultSerever = new DataBaseServer (*defaultSerever);  //  Caller will own this instance.
  }

  delete  list;  list = NULL;
  blocker->EndBlock ();
  return  defaultSerever;
}  /* GetDefaultServer */




DataBaseServerPtr  DataBase::GetServer (const KKStr&  description,
                                        RunLog&       _log
                                       )
{
  DataBaseServerPtr  selServer = NULL;
  CreateBlocker ();
  blocker->StartBlock ();

  DataBaseServerListPtr  list = new DataBaseServerList (_log);
  if  (list)
  {
    selServer = list->LookUpByDescription (description);
    if  (selServer)
      selServer = new DataBaseServer (*selServer);
  }
  delete  list;
  list = NULL;

  blocker->EndBlock ();
  return  selServer;
}




KKStr  DataBase::ServerDescription ()  const
{
  if  (server == NULL)
    return  "*** no server paranmeters selected ***";

  KKStr  description = server->ServerDescription ();
  if  (!Valid ())
    description << "  ***ERROR***";

  return  description;
}  /* ServerDescription */




KKU::int32  DataBase::Connect ()
{
  int32  returnCd = 0;
  if  (server->Description ().EqualIgnoreCase ("Embedded"))
  {
    useEmbeddedServer = true;
    returnCd = ConnectEmbedded ();
  }
  else
  {
    useEmbeddedServer = false;
    returnCd = ConnectServer ();
  }

  return  returnCd;
}  /* Connect */




KKU::int32  DataBase::ConnectEmbedded ()
{
  if  (conn)
  {
    mysql_close (conn);
    conn = NULL;
  }

  conn = mysql_init (NULL);

  if  (conn == NULL)
  {
    log.Level (-1) << endl << endl
                   << "DataBase     ***ERROR***" << endl
                   << endl
                   << "       mysql_init ()    Failed    No database activity can work without it" << endl
                   << endl;
    valid = false;
    return - 1;
  }

  // mysql_options (conn, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);

  //uint32  timeOut = 120;
  //int32 returnCd = mysql_options (conn, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)(&timeOut)); 
  int32 returnCd = mysql_options (conn, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL); 

  MYSQL*  returnMYSQLstruct = 
        mysql_real_connect (conn,                /* pointer to connection handler */
                            server->HostName     ().Str (),
                            server->UserName     ().Str (),
                            server->PassWord     ().Str (),
                            server->DataBaseName ().Str (), /* Database Name                 */
                            0,                        /* port (use default)            */
                            NULL,                     /* socket (use default)          */
                            CLIENT_MULTI_STATEMENTS   /* flags (none)                  */
                           );

  if  (returnMYSQLstruct == NULL)
  {
    // The connection failed.
    valid = false;
    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);
    log.Level (-1) << endl 
                   << endl
                   << "DataBase     ***ERROR***" << endl
                   << endl
                   << "       ErrorCd      [" << lastMySqlErrorNo        << "]" << endl
                   << "       Error        [" << lastMySqlErrorDesc      << "]" << endl
                   << "       HostName     [" << server->HostName     () << "]" << endl
                   << "       UserName     [" << server->UserName     () << "]" << endl
                   << "       DataBaseName [" << server->DataBaseName () << "]" << endl
                   << endl;
    return  lastMySqlErrorNo;
  }
  else
  {
    valid = true;
    lastMySqlErrorNo = 0;
    lastMySqlErrorDesc = "";
    return 0;
  }

}  /* ConnectEmbedded */



KKU::int32  DataBase::ConnectServer ()
{
  if  (conn)
  {
    mysql_close (conn);
    conn = NULL;
  }

  conn = mysql_init (NULL);

  if  (conn == NULL)
  {
    KKStr  msg (255);
    msg << "DataBase::ConnectServer     ***ERROR***   'mysql_init' Failed  No database activity can work without it.";
    log.Level (-1) << endl << endl << msg << endl << endl;
    valid = false;
    return - 1;
  }


  //uint32  timeOut = 120;
  //int32 returnCd = mysql_options (conn, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)(&timeOut)); 
  //http://www.linuxtopia.org/online_books/database_guides/mysql_5.1_database_reference_guide/mysql-options.html
  int32 returnCd = mysql_options (conn, MYSQL_OPT_USE_REMOTE_CONNECTION, NULL); 

  MYSQL*  returnMYSQLstruct = NULL;

  {
    char*  hostName     = STRDUP (server->HostName     ().Str ());
    char*  userName     = STRDUP (server->UserName     ().Str ());
    char*  passWord     = STRDUP (server->PassWord     ().Str ());
    char*  dataBaseName = STRDUP (server->DataBaseName ().Str ());
   
    returnMYSQLstruct = 
        mysql_real_connect (conn,                           /* pointer to connection handler */
                            hostName,
                            userName,
                            passWord,
                            dataBaseName,                   /* Database Name        */
                            0,                              /* port (use default)   */
                            NULL,                           /* socket (use default) */
                            CLIENT_MULTI_STATEMENTS         /* flags (none)         */
                           );

    delete hostName;     hostName = NULL;
    delete userName;     userName = NULL;
    delete passWord;     passWord = NULL;
    delete dataBaseName; dataBaseName = NULL;

  }

  if  (returnMYSQLstruct == NULL)
  {
    // The connection failed.
    valid = false;
    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    KKStr  logMsg (256);
    logMsg << "DataBase::ConnectServer  ***ERROR***  returnMYSQLstruct == NULL"  << endl
           << "mysql_errno"     << "\t" << lastMySqlErrorNo        << "\t" 
           << "mysql_error"     << "\t" << lastMySqlErrorDesc      << endl
           << "HostName"        << "\t" << server->HostName ()     << "\t" 
           << "UserName"        << "\t" << server->UserName ()     << "\t" 
           << "DataBaseName"    << "\t" << server->DataBaseName () << endl;

    ErrorLogMsg (logMsg);
    
    log.Level (-1) << endl << endl << logMsg << endl << endl;
    return  lastMySqlErrorNo;
  }
  else
  {
    valid = true;
    lastMySqlErrorNo = 0;
    lastMySqlErrorDesc = "";
    return 0;
  }
}  /* ConnectServer */




DataBaseServerPtr  DataBase::GetDefaultMySqlParameters ()
{
  DataBaseServerPtr  defaultParameters = NULL;
  {
    DataBaseServerListPtr servers = this->GetListOfServers (log);
    if  (servers)
    {
      DataBaseServerPtr entryInList = servers->GetDefaultServer ();
      if  (entryInList)
        defaultParameters = new DataBaseServer (*entryInList);
      delete  servers;
    }
  }

  if  (!defaultParameters)
  {
    defaultParameters = new DataBaseServer ();
  }

  return  defaultParameters;
}  /* GetDefaultMySqlParameters */




bool  DataBase::ResultSetLoad (ConstCharStarArray    fieldNames)
{
  resultSetFieldDefs  = NULL;
  resultSetNumRows    = 0;
  resultSetNumFields  = 0;
  resultSetMore       = false;
  resultSet           = NULL;
  resultSetNextRow    = NULL;
  resultSetLengths    = NULL;
  resultSetFieldIndex.clear ();
 
  resultSet = mysql_use_result (conn); /* generate result set */
  if  (resultSet == NULL)
  {
    resultSetMore = false;
    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    log.Level (-1) << endl << endl 
                   << "DataBase::ResultSetLoad    ***ERROR***     occured when retrieving result set." << endl
                   << endl
                   << "ErrorDesc[" << lastMySqlErrorDesc << "]"  << endl
                   << endl;
    return  false;
  }

  resultSetMore = true;
  resultSetNumRows    = (uint32)mysql_num_rows   (resultSet);
  resultSetNumFields  = (uint32)mysql_num_fields (resultSet);
  resultSetFieldDefs  = mysql_fetch_fields (resultSet);

  if  (fieldNames)
  {
    ResultSetBuildFieldIndexTable (fieldNames);
  }
  else
  {
    for (uint32 x = 0; x < resultSetNumFields;  x++)
      resultSetFieldIndex.push_back (x);
  }

  return  resultSetMore;
}  /* ResultSetLoad */





/** @brief   Use instead of 'ResultSetLoad'; returns all fields plust their names. */
bool  DataBase::ResultSetLoadFieldNames (VectorKKStr&  fieldNames)
{
  resultSetFieldDefs  = NULL;
  resultSetNumRows    = 0;
  resultSetNumFields  = 0;
  resultSetMore       = false;
  resultSet           = NULL;
  resultSetNextRow    = NULL;
  resultSetLengths    = NULL;
  resultSetFieldIndex.clear ();
 
  resultSet = mysql_use_result (conn); /* generate result set */
  if  (resultSet == NULL)
  {
    resultSetMore = false;
    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    log.Level (-1) << endl << endl 
                   << "DataBase::ResultSetLoad    ***ERROR***     occured when retrieving result set." << endl
                   << endl
                   << "ErrorDesc[" << lastMySqlErrorDesc << "]"  << endl
                   << endl;
    return  false;
  }

  resultSetMore = true;
  resultSetNumRows    = (uint32)mysql_num_rows   (resultSet);
  resultSetNumFields  = (uint32)mysql_num_fields (resultSet);
  resultSetFieldDefs  = mysql_fetch_fields (resultSet);

  fieldNames.clear ();

  if  (resultSet == NULL)
    return  false;

  resultSetFieldIndex.clear ();
  for  (uint32 fieldNum = 0;   fieldNum < resultSetNumFields;  ++fieldNum)
  {
    fieldNames.push_back (resultSetFieldDefs[fieldNum].name);
    resultSetFieldIndex.push_back (fieldNum);
  }

  return  resultSetMore;
}  /* ResultSetLoadFieldNames */



bool   DataBase::ResultSetGetNext (const char**  fieldNames)
{
  int32  returnCd = mysql_next_result (conn);
  if  (returnCd == 0)
  {
    resultSetMore = true;
    ResultSetLoad (fieldNames);
  }
  else
  {
    resultSetMore= false;
  }

  return  resultSetMore;
}  /* ResultSetGetNext */




void DataBase::ResulSetFree ()
{
  if  (resultSet)
    mysql_free_result (resultSet);
  resultSet = NULL;
  resultSetFieldDefs = NULL;
  resultSetNumFields = 0;
  resultSetNumRows   = 0;
  resultSetNextRow   = NULL;
  resultSetLengths   = NULL;
  resultSetFieldIndex.clear ();
}


/**
 *@brief  Call this method after you are done proessing the results of the last query.  If you do not 
 *      do this the next call may fail.
 */
void  DataBase::ResultSetsClear ()
{
  if  (resultSet)
    ResulSetFree ();

  int32 returnCd = mysql_next_result (conn);
  while  (returnCd == 0)
  {
    resultSet = mysql_use_result (conn); /* generate result set */
    if  (resultSet)
    {
      do  
      {
        resultSetNextRow = mysql_fetch_row (resultSet);
      } while  (resultSetNextRow);
      ResulSetFree ();
    }
    returnCd = mysql_next_result (conn);
  }
}  /* ResultSetsClear*/




/**
 *@brief  Returns an array with the indirection index for each field listed in fieldNames.
 *@details
 * Will return an array that has the indirection index for each field in fieldNames.
 * ex   results[2]  specifies which entry in the results row contains the value for 
 * the field name in 'fieldNames[2]'.
 *@param[in]  fieldNames  List of field names to get indirection matrix.  The last enry should be NULL to mark end of list.
 */
void  DataBase::ResultSetBuildFieldIndexTable (ConstCharStarArray   fieldNames)
{
  if  ((resultSet == NULL)  ||  (fieldNames == NULL))
    return;

  resultSetFieldIndex.clear ();

  if  (resultSetNumFields < 1)
    return;

  int32  fieldNum = 0;

  while  (fieldNames[fieldNum])
  {
    const char* fieldName = fieldNames[fieldNum];
    int32 idx = -1;
    for  (uint32 x = 0;  x < resultSetNumFields;  x++)
    {
      if  (KKU::STRICMP (resultSetFieldDefs[x].name, fieldName) == 0)
      {
        idx = x;
        break;
      }
    }
    resultSetFieldIndex.push_back (idx);
    fieldNum++;
  }

  return;
}  /* ResultSetBuildFieldIndexTable */



  
bool  DataBase::ResultSetFetchNextRow ()
{
  if  (resultSet == NULL)
  {
    KKStr  errMsg (256);
    errMsg << "DataBase::ResultSetFetchNextRow   'resultSet' == NULL   ***ERROR***" << endl
           << "             Prev SQL Cmd :" << prevQueryStatement << endl
           << "           LastMySQLError :" << lastMySqlErrorNo   << "\t" << lastMySqlErrorDesc << endl;

    log.Level (-1) << endl << endl << errMsg << endl << endl;
    ErrorLogMsg (errMsg);
    return false;
  }

  resultSetLengths = NULL;
  resultSetNextRow = mysql_fetch_row (resultSet);
  return  resultSetNextRow != NULL;
}



const char*  DataBase::ResultSetGetField (uint32 fieldIdx)
{
  if  ((fieldIdx < 0)  ||  (fieldIdx >= (uint32)resultSetFieldIndex.size ()))
    return  "";

  uint32  rowFieldIndex = resultSetFieldIndex[fieldIdx];
  if  ((rowFieldIndex < 0)  ||  (rowFieldIndex >= resultSetNumFields))
    return  "";

  if  (!resultSetNextRow)
  {
    log.Level (-1) << endl << endl
                   << "DataBase::ResultSetGetField     ***ERROR***" << endl
                   << "                             There is no active ResultSetRow." << endl
                   << endl;
    return  NULL;
  }

  //if  (resultSetNextRow[rowFieldIndex] == NULL)
  //  return  "";
  
  return  resultSetNextRow[rowFieldIndex];
}



KKU::ulong  DataBase::ResultSetGetFieldLen (uint32  fieldIdx)
{
  if  (!resultSetLengths)
    resultSetLengths = mysql_fetch_lengths (resultSet);

  if  (!resultSetLengths)
    return  0;

  if  ((fieldIdx < 0)  ||  (fieldIdx >= (int32)resultSetFieldIndex.size ()))
    return  0;

  uint32  rowFieldIndex = resultSetFieldIndex[fieldIdx];
  if  ((rowFieldIndex < 0)  ||  (rowFieldIndex >= resultSetNumFields))
    return  0;

  return  resultSetLengths[rowFieldIndex];
}



RasterSipperPtr  DataBase::ResultSetGetRasterField (uint32 fieldIdx)
{
  const char*  buffer = ResultSetGetField (fieldIdx);
  if  (buffer == NULL)
    return NULL;
  ulong  bufferLen = ResultSetGetFieldLen (fieldIdx);
  
  RasterSipperPtr r = NULL;
  try
  {
    r = RasterSipper::FromCompressor ((uchar*)buffer, bufferLen);
  }
  catch  (const KKStrException&  e1)
  {
    ErrorLogMsg ("DataBase::ResultSetGetRasterField      Exception Thrown calling   'RasterSipper::FromCompressor' \n" + e1.ToString ());
    r = NULL;
  }
  catch  (...)
  {
    ErrorLogMsg ("DataBase::ResultSetGetRasterField      Exception Thrown calling   'RasterSipper::FromCompressor'" );
    r = NULL;
  }

  return  r;
}



KKStr  DataBase::ResultSetGetKKStrField (uint32 fieldIdx)
{
  return  ResultSetGetField (fieldIdx);
}


char  DataBase::ResultSetGetCharField (uint32 fieldIdx)
{
  KKStr  s = ResultSetGetField (fieldIdx);
  return  s[0];
}




bool  DataBase::ResultSetGetBool (uint32 fieldIdx)
{
  KKStr s = ResultSetGetField (fieldIdx);
  s.Upper ();
  return  ((s == "1")  ||  (s == "TRUE")  ||  (s == "YES")  ||  (s == "T")  ||  (s == "Y"));
}




float  DataBase::ResultSetGetFloatField (uint32 fieldIdx)
{
  return  CharStarToFloat (ResultSetGetField (fieldIdx));
}



DateTime  DataBase::ResultSetGetDateTimeField (uint32 fieldIdx)
{
  return  DateTimeFromMySqlDateTimeField (ResultSetGetField (fieldIdx));
}


TimeType  DataBase::ResultSetGetTimeField (uint32 fieldIdx)
{
  return  TimeFromMySqlDateTimeField (ResultSetGetField (fieldIdx));
}


double  DataBase::ResultSetGetDoubleField (uint32 fieldIdx)
{
  return  CharStarToDouble (ResultSetGetField (fieldIdx));
}


KKU::int32   DataBase::ResultSetGetIntField (uint32 fieldIdx)
{
  return  CharStarToInt32 (ResultSetGetField (fieldIdx));
}


KKU::int64  DataBase::ResultSetGetInt64Field    (uint32 fieldIdx)
{
  return  CharStarToInt64 (ResultSetGetField (fieldIdx));
}


KKU::uint32   DataBase::ResultSetGetUintField (uint32 fieldIdx)
{
  return  CharStarToUint32 (ResultSetGetField (fieldIdx));
}


KKU::ulong   DataBase::ResultSetGetUlongField (uint32 fieldIdx)
{
  return  CharStarToUlong (ResultSetGetField (fieldIdx));
}


KKU::uint64   DataBase::ResultSetGetUint64Field (uint32 fieldIdx)
{
  return  CharStarToUint64 (ResultSetGetField (fieldIdx));
}



KKStr  DataBase::LastErrorDesc ()  const
{
  return  lastMySqlErrorDesc;
}



KKU::DateType  DataBase::DateFromMySqlDateTimeField (KKStr  field)
{
  int32  year  = field.ExtractTokenInt ("-");
  int32  month = field.ExtractTokenInt ("-");
  int32  day   = field.ToInt ();

  if  ((year < 1900)  || (year > 2199)  ||  (month < 1)  ||  (month > 12)  || (day < 1)  || (day > 31))
    return DateType (1900, 1, 1);

  return  DateType (year, month, day);
}  /* DateFromMySqlDateTimeField */





DateTime  DataBase::DateTimeFromMySqlDateTimeField (KKStr  field)
{
  KKStr  yearStr   = field.ExtractToken ("-");
  KKStr  monthStr  = field.ExtractToken ("-");
  KKStr  dayStr    = field.ExtractToken (" ");
  KKStr  hourStr   = field.ExtractToken (":");
  KKStr  minStr    = field.ExtractToken (":");
  
  DateTime result ((uint32) yearStr.ToInt (),  (uchar)monthStr.ToInt (), (uchar)dayStr.ToInt (), 
                   (uchar)hourStr.ToInt (),  (uchar)minStr.ToInt (),   (uchar)field.ToInt ());
  return  result;
}  /* DateTimeFromMySqlField */





KKU::TimeType  DataBase::TimeFromMySqlDateTimeField (KKStr field)
{
  KKStr  hourStr = field.ExtractToken (":");
  KKStr  minStr  = field.ExtractToken (":");
  TimeType  result ((uchar)hourStr.ToInt16 (), (uchar)minStr.ToInt16 (), (uchar)field.ToInt16 ());
  return  result;
}



KKStr  DataBase::DateTimeToQuotedStr (const DateTime&  dt)
{
  KKStr  result = "\"" + dt.Date ().YYYY_MM_DD () + " " + dt.Time ().HH_MM_SS () + "\"";
  return  result;
}




KKStr  DateToQuotedStr (const DateType&  d)
{
  KKStr  result = "\"" + d.YYYY_MM_DD () + "\"";
  return  result;
}



KKU::int32  DataBase::ReConnect (const KKStr&  funcCall,
                                 const KKStr&  queryStrMsg,
                                 int32         numReTries
                                )
{
  // We will attempt to reconnect to the server 
  KKStr  l (256);
  l << "DataBase::ReConnect   Server Connection Lost    FuncCall[" << funcCall << "]" << endl
    << lastMySqlErrorNo << "\t" << lastMySqlErrorDesc << endl
    << "LastQueryStr"   << "\t" <<queryStrMsg << endl;
  log.Level (-1) << endl << endl << l << endl << endl;
  ErrorLogMsg (l);

  int32  attempt = 1;

  int32  returnCd = Connect ();

  while  (returnCd != 0)
  {
    if  ((attempt > numReTries)  &&  (numReTries > 0))
    {
      KKStr  errMsg(256);
      errMsg << "DataBase::ReConnect  " << numReTries << " Attempts at reconnection have failed.";
      log.Level (-1) << endl << errMsg << endl << endl;
      ErrorLogMsg (errMsg);
      return  CR_SERVER_LOST;
    }

    if  (attempt > 10)
      osSleep (10);
    else if  (attempt > 3)
      osSleep (5);
    else
      osSleep (2.0f);

    attempt++;
    {
      KKStr  msg (256);
      msg << "DataBase::ReConnect    Attempt[" << attempt << "]";
      log.Level (-1) << endl << msg << endl << endl;
      ErrorLogMsg (msg);
    }

    returnCd = Connect ();
  }

  {
    KKStr  msg (256);
    msg << "DataBase::ReConnect    Connection reestablished after [" << attempt << "] attempts.";
    log.Level (-1) << endl << msg << endl << endl;
    ErrorLogMsg (msg);
  }

  return  0;
}  /* Reconnect */



void  DataBase::ValidateConnection (const char*  funcCall)
{
  if  (resultSet != NULL)
  {
    // This should not happen,  We will add warning to Sql ERROR File.

    KKStr  msg (256);
    msg << "DataBase::ValidateConnection   ***ERROR*** Func[" << funcCall << "]  'resultSet != NULL'   This should never happen." << endl
        << "     Prev Statement[" << prevQueryStatement << "]";

    log.Level (-1) << endl << msg << endl << endl;

    ErrorLogMsg (msg);

    // Since we do not know the status of the Prebvious ResultSet we will set it to NULL.
    resultSet = NULL;
  }

  if  (!conn)
  {
    KKStr  msg (256);
    msg << "DataBase::ValidateConnection   Func[" << funcCall << "]  Connection was closed.";
    log.Level (-1) << endl << msg << endl << endl;
    ErrorLogMsg (msg);
    Connect ();
  }
}  /* ValidateConnectiuon */



KKU::int32  DataBase::QueryStatement (const char* queryStr,
                                      int32       queryStrLen
                                     )
{
  ValidateConnection ("QueryStatement");

  int32  outOfSeqLoopCount = 0;

  int32  attempt = 1;

  while  (true)
  {
    int32 returnCd = mysql_real_query (conn, queryStr, queryStrLen);
    if  (returnCd == 0)
      break;

    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    if  ((lastMySqlErrorNo == CR_SERVER_GONE_ERROR)  ||  (lastMySqlErrorNo == CR_SERVER_LOST))
    {
      // We will attempt to reconnect to the server 
      KKStr  queryStrMsg;
      if  (queryStrLen > 256)  queryStrMsg.Append (queryStr, 255);
      else  queryStrMsg = queryStr;

      returnCd = ReConnect ("QueryStatement", queryStrMsg, 30);
      if  (returnCd != 0)
        return returnCd;

      continue;
    }

    else if  (lastMySqlErrorNo == 2014)
    {
      // Calls are out of sequence;  

      KKStr  curStatement;
      if  (queryStrLen > 512)  curStatement.Append (queryStr, 512);
      else  curStatement.Append (queryStr, queryStrLen);

      KKStr  msg (512);
      msg << "DataBase::QueryStatement    MySQL calls out of sequence." << endl
          << "          lastMySqlErrorDesc[" << lastMySqlErrorDesc << "]" << endl
          << "          lastMySqlErrorNo  [" << lastMySqlErrorNo   << "]" << endl
          << endl;

      log.Level (-1) << endl << endl << endl  << msg << endl << endl;
      ErrorLogMsg (msg);

      if  (outOfSeqLoopCount == 0)
      {
        ResultSetsClear ();
      }

      else if  (outOfSeqLoopCount == 1)
      {
        // Clearing result sets did not work so lets try closing and opening connection
        returnCd = Connect ();
      }

      else if  (outOfSeqLoopCount > 1)
      {
        return  lastMySqlErrorNo;
      }

      outOfSeqLoopCount++;
    }

    duplicateKey = ((lastMySqlErrorNo == 1022)  ||  (lastMySqlErrorNo == 1062));

    if  (returnCd != 0)
    {
      valid = false;

      KKStr  msg (255);
      msg << osGetLocalDateTime () << " : " << lastMySqlErrorNo << "\t" << lastMySqlErrorDesc << endl << queryStr;
      ErrorLogMsg (msg);
      return  lastMySqlErrorNo;
    }
  }  /* End of while(true)  loop */

  valid = true;

  prevQueryStatement = "";
  if  (queryStrLen > 512)
    prevQueryStatement.Append (queryStr, 512);
  else
    prevQueryStatement.Append (queryStr, queryStrLen);

  return 0;
}  /* QueryStatement */




KKU::int32  DataBase::QueryStatement2 (const char*  queryStr,
                                       int32        queryStrLen
                                      )
{
  ValidateConnection ("QueryStatement2");

  int32  outOfSeqLoopCount = 0;

  int32 returnCd = mysql_real_query (conn, queryStr, queryStrLen);
  if  (returnCd != 0)
  {
    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    if  ((lastMySqlErrorNo == CR_SERVER_GONE_ERROR)  ||  (lastMySqlErrorNo == CR_SERVER_LOST))
    {
      KKStr  msg (255);
      msg << osGetLocalDateTime () << " : " << lastMySqlErrorNo << "\t" << lastMySqlErrorDesc << endl << queryStr << endl;
      ErrorLogMsg (msg);
      return  returnCd;
    }
    else
    {
      duplicateKey = ((lastMySqlErrorNo == 1022)  ||  (lastMySqlErrorNo == 1062));
      if  (returnCd != 0)
      {
        valid = false;

        KKStr  msg (255);
        msg << osGetLocalDateTime () << " : " << lastMySqlErrorNo << "\t" << lastMySqlErrorDesc << endl << queryStr << endl;
        ErrorLogMsg (msg);

        return  lastMySqlErrorNo;
      }
    }
  }  /* End of while(true)  loop */

  valid = true;

  prevQueryStatement = "";
  if  (queryStrLen > 512)
    prevQueryStatement.Append (queryStr, 512);
  else
    prevQueryStatement.Append (queryStr, queryStrLen);

  return 0;
}  /* QueryStatement2 */



KKU::int32  DataBase::QueryStatement (const KKStr& statement)
{
  ValidateConnection ("QueryStatement");

  int32  outOfSeqLoopCount = 0;
  while  (true)
  {
    int32 returnCd = mysql_real_query (conn, statement.Str (), statement.Len ());
    if  (returnCd == 0)
      break;

    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    if  ((lastMySqlErrorNo == CR_SERVER_GONE_ERROR)  ||  (lastMySqlErrorNo == CR_SERVER_LOST))
    {
      returnCd = ReConnect ("QueryStatement", "", 30);
      if  (returnCd != 0)
        return returnCd;
      continue;
    }

    if  (lastMySqlErrorNo == 2014)
    {
      // Calls are out of sequence;  
      KKStr l(512);
      l << "MySQL Calls out of sequience" << "\t" << lastMySqlErrorNo << "\t" << lastMySqlErrorDesc << endl
        << "Prev Statement" << "\t" << prevQueryStatement << endl 
        << "Cur  Statement" << "\t" << statement          << endl 
        << "Loop Count"     << "\t" << outOfSeqLoopCount  << endl;
      log.Level (-1) << endl << l << endl;
      ErrorLogMsg (l);

      if  (outOfSeqLoopCount == 0)
      {
        ResultSetsClear ();
      }

      else if  (outOfSeqLoopCount == 1)
      {
        // Clearing result sets did not work so lets try closing and opening connection
        returnCd = Connect ();
      }

      else if  (outOfSeqLoopCount > 1)
      {
        return  lastMySqlErrorNo;
      }

      outOfSeqLoopCount++;
    }

    duplicateKey = ((lastMySqlErrorNo == 1022)  ||  (lastMySqlErrorNo == 1062));

    if  (returnCd != 0)
    {
      valid = false;
      KKStr  l (512);
      l << "DataBase::QueryStatement" << "\t"
        << "lastMySqlErrorNo" << "\t" << lastMySqlErrorNo << "\t" 
        << "mysql_error"      << "\t" << lastMySqlErrorDesc << endl
        << "Cur  Statement"   << "\t" << statement          << endl
        << "Prev Statement"   << "\t" << prevQueryStatement << endl;
      ErrorLogMsg (l);

      return  lastMySqlErrorNo;
    }
  }

  valid = true;

  prevQueryStatement = statement;

  return 0;
}  /* QueryStatement */





KKStrMatrixPtr  DataBase::QueryStatementKKStr (const KKStr&        statement,
                                               ConstCharStarArray  colsToReturn
                                              )
{
  int32  returnCd = QueryStatement (statement.Str (), statement.Len ());
  if  (returnCd != 0)
    return NULL;

  if  (!ResultSetLoad (colsToReturn))
    return NULL;

  uint32 numCols = resultSetNumFields;
  uint32 col = 0;

  KKStrMatrixPtr  results = new KKStrMatrix (numCols);
  while  (ResultSetFetchNextRow ())
  {
    KKStrListPtr  newResultRow = new KKStrList (true);
    for  (col = 0;  col < numCols;  col++)
    {
      newResultRow->PushOnBack (new KKStr (ResultSetGetField (col)));
    }
    results->AddRow (newResultRow);
  }

  ResulSetFree ();
  ResultSetsClear ();

  return  results;
}  /* QueryStatement */




KKStrMatrixPtr  DataBase::QueryStatementReturnAllColumns (const char*   statement,
                                                          int32         statementLen,
                                                          VectorKKStr&  columnsReturned
                                                         )
{
  int32  returnCd = QueryStatement (statement, statementLen);
  if  (returnCd != 0)
    return NULL;

  columnsReturned.clear ();
  if  (!ResultSetLoadFieldNames (columnsReturned))
    return NULL;

  uint32 numCols = resultSetNumFields;
  uint32 col = 0;

  KKStrMatrixPtr  results = new KKStrMatrix (numCols);
  while  (ResultSetFetchNextRow ())
  {
    KKStrListPtr  newResultRow = new KKStrList (true);
    for  (col = 0;  col < numCols;  col++)
    {
      newResultRow->PushOnBack (new KKStr (ResultSetGetField (col)));
    }
    results->AddRow (newResultRow);
  }

  ResulSetFree ();
  ResultSetsClear ();

  return  results;
}  /* QueryStatementReturnAllColumns */





//********************************************************************************************
//*                                 FeatureData   Routines                                   *
//********************************************************************************************

void  DataBase::FeatureDataCreateTable ()
{
  KKStr  createStatement;

  createStatement << "create table  FeatureData"         << endl
                  << "("                                 << endl
                  << "  ImageFileName    varchar(32)   not null,"   << endl
                  << "  SipperFileName   char (14)     not null,"   << endl;

  int32  fieldNum = 0;
  for  (fieldNum = 0;  fieldNum < FeatureFileIOPices::PlanktonMaxNumOfFields ();  fieldNum++)
  {
    KKStr  fieldName = FeatureFileIOPices::PlanktonFieldName (fieldNum);
    if  (!fieldName.Empty ())
      createStatement << "  " << FeatureFileIOPices::PlanktonFieldName (fieldNum) << "  real  null," << endl;
  }

  createStatement << endl
                  << "  Primary Key       (ImageFileName   ASC),"  << endl
                  << "  Key SipperFileKey (SipperFileName  ASC)"  << endl
                  << ")";

  int32  returnCd = QueryStatement (createStatement);

  if  (returnCd != 0)
  {
    log.Level (-1) << endl << endl
                   << "DataBase::CreateFeatureDataTable    ***ERROR***" << endl
                   << endl
                   << "         Error[" << lastMySqlErrorDesc << "] occured trying to create FeatureData table." << endl
                   << endl;
    valid = false;
    return;
  }

  ResultSetsClear ();
}  /* CreateFeatureDataTable */




void  DataBase::FeatureDataInsertRow (const KKStr&          _sipperFileName,
                                      const ImageFeatures&  example
                                     )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("FeatureDataInsertRow");
    return;
  }

  KKStr  sipperFileName = osGetRootName (_sipperFileName);
  KKStr  imageFileName  = osGetRootName (example.ImageFileName ());

  KKStr  insertStatement (3000);
  insertStatement << "call  FeatureDataInsert(" << imageFileName.QuotedStr () << ", " << sipperFileName.QuotedStr ();
  for  (int32 fieldNum = 0;  fieldNum < example.NumOfFeatures ();  fieldNum++)
  {
    double f = example.FeatureData (fieldNum);
    if  ((IsNaN (f))  ||  (!IsFinite (f)))
      insertStatement << ", " << "NULL";
    else
      insertStatement << ", " << example.FeatureData (fieldNum);
  }
  insertStatement << ")";

  int32  returnCd = QueryStatement (insertStatement);
  ResultSetsClear ();
}  /* FeatureDataInsertRow */




KKU::int32  LookUpFeatureDataField (const KKStr& fieldName)
{
  KKU::int32  fieldNum = 0;
  for  (fieldNum = 0;  fieldNum < FeatureFileIOPices::PlanktonMaxNumOfFields ();  fieldNum++)
  {
    if  (fieldName == FeatureFileIOPices::PlanktonFieldName (fieldNum))
      return  fieldNum;
  }
  return -1;
}  /* LookUpFeatureDataField */




char**   DataBase::featureDataFieldNames = NULL;



ConstCharStarArray  DataBase::GetFeatureDataFieldNames ()
{
  if  (featureDataFieldNames != NULL)
    return  featureDataFieldNames;

  CreateBlocker ();
  blocker->StartBlock ();

  if  (featureDataFieldNames != NULL)
  {
    // Looks like another thread must have been in the middle of creating this list.
    blocker->EndBlock ();
    return  featureDataFieldNames;
  }

  char**  fieldNames = NULL;
  int32  fieldNamesSize = 16 + FeatureFileIOPices::PlanktonMaxNumOfFields ();
  fieldNames = new char*[fieldNamesSize];
  fieldNames[ 0] = STRDUP ("ImageId");
  fieldNames[ 1] = STRDUP ("ImageFileName");
  fieldNames[ 2] = STRDUP ("TopLeftRow");
  fieldNames[ 3] = STRDUP ("TopLeftCol");
  fieldNames[ 4] = STRDUP ("PixelCount");
  fieldNames[ 5] = STRDUP ("CentroidRow");
  fieldNames[ 6] = STRDUP ("CentroidCol");
  fieldNames[ 7] = STRDUP ("Class1Id");
  fieldNames[ 8] = STRDUP ("Class1Name");
  fieldNames[ 9] = STRDUP ("Class1Prob");
  fieldNames[10] = STRDUP ("Class2Prob");
  fieldNames[11] = STRDUP ("ClassValidatedId");
  fieldNames[12] = STRDUP ("ClassNameValidated");
  fieldNames[13] = STRDUP ("ImagesDepth");
  fieldNames[14] = STRDUP ("CtdDateTime");
   
  int32  fieldNum = 0;
  for  (fieldNum = 0;  fieldNum < FeatureFileIOPices::PlanktonMaxNumOfFields ();  fieldNum++)
    fieldNames[15 + fieldNum] = STRDUP (FeatureFileIOPices::PlanktonFieldName (fieldNum).Str ());
  fieldNames[15 + fieldNum] = NULL;

  featureDataFieldNames = fieldNames;
  blocker->EndBlock ();
  return  featureDataFieldNames;
}  /* GetFeatureDataFieldNames */





ImageFeaturesListPtr   DataBase::FeatureDataProcessResults ()
{
  ConstCharStarArray   fieldNames = GetFeatureDataFieldNames ();

  if  (!featureFileDesc)
    featureFileDesc = FeatureFileIOPices::NewPlanktonFile (log);

  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
    return  NULL;

  ImageFeaturesListPtr  results = new ImageFeaturesList (featureFileDesc, true, log, resultSetNumRows);

  uint32  topLeftRow  = 0;
  uint32  topLeftCol  = 0;

  KKStr  class1Name         (64);
  int32  class1Id = -1;
  KKStr  classNameValidated (64);
  int32  classValidatedId = -1;
  float  class1Prob = 0.0f;
  float  class2Prob = 0.0f;

  float  imagesDepth = 0.0f;

  KKU::DateTime  ctdDateTime;

  while  (ResultSetFetchNextRow ())
  {
    ImageFeaturesPtr  i = new ImageFeatures (featureFileDesc->NumOfFields ());

    i->ImageFileName (ResultSetGetField       (1));
    topLeftRow = ResultSetGetUintField        (2);
    topLeftCol = ResultSetGetUintField        (3);
    i->OrigSize (ResultSetGetFloatField       (4));
    i->CentroidRow (ResultSetGetFloatField    (5));
    i->CentroidCol (ResultSetGetFloatField    (6));
    class1Id   = ResultSetGetIntField         (7);
    class1Name = ResultSetGetField            (8);
    class1Prob = ResultSetGetFloatField       (9);
    class2Prob = ResultSetGetFloatField       (10);
    classValidatedId = ResultSetGetIntField   (11);
    classNameValidated = ResultSetGetField    (12);
    imagesDepth = ResultSetGetFloatField      (13);
    i->CtdDateTime (ResultSetGetDateTimeField (14));

    for  (int32 fieldNum = 0;  fieldNum < FeatureFileIOPices::PlanktonMaxNumOfFields ();  fieldNum++)
      i->FeatureData (fieldNum, ResultSetGetFloatField (15 + fieldNum));

    if  (class1Id == 0)
      class1Id = -1;

    if  (classValidatedId == 0)
      classValidatedId = -1;

    if  (!class1Name.Empty ())
      i->PredictedClass (MLClass::CreateNewMLClass (class1Name, class1Id));

    if  (!classNameValidated.Empty ())
    {
      i->MLClass (MLClass::CreateNewMLClass (classNameValidated, classValidatedId));
      i->Validated (true);
    }
    else
    {
      if  (!class1Name.Empty ())
        i->MLClass (MLClass::CreateNewMLClass (class1Name, class1Id));
      else
        i->MLClass (MLClass::GetUnKnownClassStatic ());
    }

    i->Probability (class1Prob);
    i->BreakTie (fabs (class1Prob - class2Prob));
    i->TrainWeight (1.0f);

    i->SfCentroidCol (topLeftCol + i->CentroidCol ());
    i->SfCentroidRow (topLeftRow + i->CentroidRow ());

    results->PushOnBack (i);
  }

  ResulSetFree ();

  return  results;
}  /* FeatureDataProcessResults */




ImageFeaturesPtr    DataBase::FeatureDataRecLoad (const KKStr&  imageFileName)
{
  KKStr  rootName = osGetRootName (imageFileName);

  KKStr  selectStr (256);

  selectStr << "call  FeatureDataLoadByImageFileName2(" << rootName.QuotedStr () << ")";
  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
  {
    log.Level (-1) << endl << endl 
                   << "DataBase::FeatureDataRecLoad     ***ERROR***" << endl
                   << endl
                   << "Error[" << lastMySqlErrorDesc << "]" << endl
                   << endl;
    return NULL;
  }

  ImageFeaturesListPtr  results = FeatureDataProcessResults ();  // true = Refresh Insrument Data from Sipper Files.
  ResultSetsClear ();
  if  (results == NULL)  
    return NULL;

  if  (results->QueueSize () < 1)
  {
    delete  results;  results = NULL;
    return NULL;
  }

  ImageFeaturesPtr  result = results->PopFromBack ();
  delete results;  results = NULL;

  if  (result)
  {
    // To make sure that we have valid CTD data the search must start no sooner than 30K scan 
    // lines from the beginning of the sipper file.
    KKStr  SipperFileName = InstrumentDataFileManager::SipperFileRootNameFromSipperImageFileName (result->ImageFileName ());
    InstrumentDataPtr  id = this->InstrumentDataGetByScanLine (SipperFileName, Max ((ulong)30000, (ulong)(result->SfCentroidRow ())));
    if  (id  &&  id->Depth () > 0.0)
    {
      if  (result->Depth () == 0.0)
      {
        result->Depth         (id->Depth        ());
        result->Fluorescence  (id->Fluorescence ());       
        result->Oxygen        (id->Oxygen       ());
        result->Salinity      (id->Salinity     ());
      }

      if  (result->FlowRate1 () == 0.0)
      {
        result->FlowRate1     (id->FlowRate1    ());
        result->FlowRate2     (id->FlowRate2    ());
      }

      if  (result->Latitude () == 0.0)
      {
        result->Latitude      (id->Latitude     ());
        result->Longitude     (id->Longitude    ());
      }

      delete  id;  id = NULL;
    }
  }

  ResultSetsClear ();
  return  result;
}  /* FeatureDataRecLoad */




/// Retrieves FeatureData for a given 'DataBaseImage' object.  It will also refresh the Instrument data fields
/// of the 'ImageFeatures' objects from the 'InstruentData' table
ImageFeaturesPtr   DataBase::FeatureDataRecLoad (DataBaseImagePtr  image)
{
  KKStr  selectStr (256);
                     
  selectStr << "call  FeatureDataLoadByImageFileName2(" <<image->ImageFileName ().QuotedStr () << ")";
  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
  {
    log.Level (-1) << endl << endl 
                   << "DataBase::FeatureDataRecLoad     ***ERROR***" << endl
                   << endl
                   << "Error[" << lastMySqlErrorDesc << "]" << endl
                   << endl;
    return NULL;
  }

  ImageFeaturesListPtr  results = FeatureDataProcessResults ();  // true = Refresh Insrument Data from Sipper Files.
  ResultSetsClear ();
  if  (results == NULL)  
    return NULL;

  if  (results->QueueSize () < 1)
  {
    delete  results;  results = NULL;
    return NULL;
  }

  ImageFeaturesPtr  result = results->PopFromBack ();
  delete results;  results = NULL;

  if  (result)
  {
    // To make sure that we have valid CTD data the search must start no sooner than 30K scan 
    // lines from the beginning of the sipper file.
    KKStr  SipperFileName = InstrumentDataFileManager::SipperFileRootNameFromSipperImageFileName (result->ImageFileName ());
    InstrumentDataPtr  id = this->InstrumentDataGetByScanLine (SipperFileName, Max ((ulong)30000, (ulong)(result->SfCentroidRow ())));
    if  (id  &&  id->Depth () > 0.0)
    {
      if  (result->Depth () == 0.0)
      {
        result->Depth         (id->Depth        ());
        result->Fluorescence  (id->Fluorescence ());       
        result->Oxygen        (id->Oxygen       ());
        result->Salinity      (id->Salinity     ());
      }

      if  (result->FlowRate1 () == 0.0)
      {
        result->FlowRate1     (id->FlowRate1    ());
        result->FlowRate2     (id->FlowRate2    ());
      }

      if  (result->Latitude () == 0.0)
      {
        result->Latitude      (id->Latitude     ());
        result->Longitude     (id->Longitude    ());
      }

      delete  id;  id = NULL;
    }
  }

  ResultSetsClear ();
  return  result;
}  /* FeatureDataRecLoad */





/**
 *@brief  Loads all Images with related Feature data for the specified 'SipperFile'
 *@param[in] sipperFileName            Sipper file to retrieve feature data for.
 *@param[in] mlClass                You are only interested in feature data for a specific class;  whether predicted or va;idated class depends on 'classKeyToUse'.
 *@param[in] classKeyToUse             Indicates which ClassId field to use for selecting feature data by class;  'V' = 'Images.ClassValidatedId' otherwise 'Images.Class1Id'.
 *@param[in] reExtractInstrumentData:  True indicates to reextract feature data from the original SipperFile and update InmstrumentData tables.
 */
ImageFeaturesListPtr  DataBase::FeatureDataGetOneSipperFile (const KKStr&        sipperFileName,
                                                             MLClassConstPtr  mlClass,
                                                             char                classKeyToUse,
                                                             bool                reExtractInstrumentData,
                                                             bool&               cancelFlag
                                                            )
{
  SipperFilePtr  sipperFile = NULL;
  InstrumentDataListPtr  instrumentData = NULL;

  KKStr  classKeyToUseStr = "";
  classKeyToUseStr.Append (classKeyToUse);

  if  (reExtractInstrumentData)
  {
    // We are making sure that the InstrumentData for this sipper file is loaded into memory
    SipperFilePtr  sipperFile = SipperFileRecLoad (osGetRootName (sipperFileName));
    instrumentData = InstrumentDataFileManager::ReExtractInstrumentDataForSipperFile    // We will not own 'instrumentData' it 
                         (sipperFileName, sipperFile, cancelFlag, log);                 // is maintained by 'InsrumentDataFileManager'
    if  (instrumentData)
      InstrumentDataSaveListForOneSipperFile (sipperFileName, *instrumentData, cancelFlag);
  }

  KKStr  selectStr (256);
  if  (mlClass)
    selectStr << "call FeatureDataGetOneSipperFileClassName(" << osGetRootName (sipperFileName).QuotedStr () << "," 
                                                              << mlClass->Name ().QuotedStr ()            << ","
                                                              << classKeyToUseStr.QuotedStr ()
                                                              << ")";
  else
    selectStr << "call FeatureDataGetOneSipperFile(" << osGetRootName (sipperFileName).QuotedStr () << ", " 
                                                     << classKeyToUseStr.QuotedStr () 
                                                     << ")";

  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  ImageFeaturesListPtr results = FeatureDataProcessResults ();
  if  (results  &&  instrumentData)
  {
    ImageFeaturesList::iterator  idx;
    for  (idx = results->begin ();  (idx != results->end ())  &&  (!cancelFlag);  idx++)
    {
      ImageFeaturesPtr  i = *idx;
      if  ((i->Depth () == 0.0)  ||  (i->FlowRate1 () == 0.0)  ||  (i->Latitude () == 0.0))
      {
        InstrumentDataPtr  id = instrumentData->SearchForNearestScanLine (Max ((ulong)i->SfCentroidRow (), (ulong)30000));
        if  (id)
        {
          if  (i->Depth () == 0.0)
          {
            i->Depth         (id->Depth        ());
            i->Fluorescence  (id->Fluorescence ());       
            i->Oxygen        (id->Oxygen       ());
            i->Salinity      (id->Salinity     ());
          }

          if  (i->FlowRate1 () == 0.0)
          {
            i->FlowRate1     (id->FlowRate1    ());
            i->FlowRate2     (id->FlowRate2    ());
          }

          if  (i->Latitude () == 0.0)
          {
            i->Latitude      (id->Latitude     ());
            i->Longitude     (id->Longitude    ());
          }
        }
      }
    }
  }
  
  ResultSetsClear ();

  if  (cancelFlag)
  {
    delete  results;
    results = NULL;
  }

  return  results;
}  /* FeatureDataGetOneSipperFile */






ImageFeaturesListPtr  DataBase::FeatureDataForImageGroup (const DataBaseImageGroupPtr  imageGroup,
                                                          MLClassConstPtr           mlClass,
                                                          char                         classKeyToUse,
                                                          const bool&                  cancelFlag
                                                         )
{
  if  (imageGroup == NULL)
  {
    log.Level (10) << endl << endl
                   << "DataBase::FeatureDataForImageGroup   ***WARNING***    (imageGroup == NULL)"  << endl
                   << endl;
    return NULL;
  }

  log.Level (20) << "DataBase::FeatureDataForImageGroup   ImageGroup[" << imageGroup->Name () << "]" << endl;

  KKStr  classKeyToUseStr = "";
  classKeyToUseStr.Append (classKeyToUse);

  InstrumentDataListPtr  instrumentData = NULL;

  KKStr  selectStr (256);

  if  (mlClass)
    selectStr << "call FeatureDataGetOneImageGroupClassName(" << imageGroup->ImageGroupId ()      << "," 
                                                              << mlClass->Name ().QuotedStr () << "," 
                                                              << classKeyToUseStr.QuotedStr ()
                                                              << ")";
  else
    selectStr << "call FeatureDataGetOneImageGroup(" << imageGroup->ImageGroupId ()   << ","
                                                     << classKeyToUseStr.QuotedStr ()
                                                     << ")";

  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  ImageFeaturesListPtr results = FeatureDataProcessResults ();
  ResultSetsClear ();

  return  results;
}  /* FeatureDataForImageGroup */





void   DataBase::FeatureDataUpdate (DataBaseImagePtr  dataBaseImage,
                                    ImageFeaturesPtr  example
                                   )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("FeatureDataUpdate");
    return;
  }

  KKStr  updateStr (3072);

  updateStr << "update FeatureData set ";
  for  (int32 featureNum = 0;  featureNum < example->NumOfFeatures ();  featureNum++)
  {
    if  (featureNum > 0)
      updateStr << ", ";
    updateStr << FeatureFileIOPices::PlanktonFieldName (featureNum) << " = " << example->FeatureData (featureNum);
  }

  if  (dataBaseImage != NULL)
  {
    updateStr << "  where ImageId = " << dataBaseImage->ImageId ();
  }
  else
  {
    KKStr  rootImageName =  KKU::osGetRootName (example->ImageFileName ());
    updateStr << "  where ImageId = (select  ImageId from Images where ImageFileName = " << rootImageName.QuotedStr () << ")";
  }

  int32  returnCd = QueryStatement (updateStr);
}  /* FeatureDataUpdate */




void   DataBase::FeatureDataUpdateInstrumentDataFields (const KKStr&       imageFileName,
                                                        InstrumentDataPtr  instumentData
                                                       )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("FeatureDataUpdateInstrumentDataFields");
    return;
  }

  KKStr  updateStr (1024);

  updateStr << "call FeatureDataUpdateInstrumentDataFields("
            << imageFileName.QuotedStr  () << ", "
            << instumentData->FlowRate1 () << ", "
            << instumentData->FlowRate2 () << ", "
            << instumentData->Depth     () << ", "  
            << instumentData->Salinity  () << ", "
            << instumentData->Oxygen    () << ", "
            << instumentData->Fluorescence ()
            << ")";
  int32  returnCd = QueryStatement (updateStr);

  ResultSetsClear ();
}  /* FeatureDataUpdateInstrumentDataFields */



vector<KKU::ulong>*  DataBase::FeatureDataGetScanLinesPerMeterProfile (const KKStr&  sipperFileName)
{
  KKStr  selectStr = "Call FeatureDataGetScanLinesPerMeterProfile (" + osGetRootName (sipperFileName).QuotedStr () + ")";
  int32  returnCd = QueryStatement (selectStr);

  char*  fieldNames[] = {"StartDepth", "ScanLineCount", NULL};
  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
    return NULL;

  vector<ulong>* results = new vector<ulong> (0);

  while  (ResultSetFetchNextRow ())
  {
    uint32 startDepth    = ResultSetGetUintField (0);
    ulong  scanLineCount = ResultSetGetUlongField (1);

    while  (results->size () < startDepth)
      results->push_back (0);


    (*results)[startDepth] += scanLineCount;
  }
    
  return  results;
}  /* FeatureDataGetScanLinesPerMeterProfile*/







//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//*                                   GpsData Routines                                      *
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************

DataBaseGpsDataListPtr   DataBase::GpsDataProcessResults ()
{
  ResultSetLoad (DataBaseGpsData::FieldList ());
  if  (!resultSetMore)
    return  NULL;

  DataBaseGpsDataListPtr  results = new DataBaseGpsDataList ();

  KKStr     cruiseName;
  DateTime  utcDateTime;
  double    latitude;
  double    longitude;
  float     courseOverGround;
  float     speedOverGround;

  while  (ResultSetFetchNextRow ())
  {
    cruiseName       = ResultSetGetField         (0);
    utcDateTime      = ResultSetGetDateTimeField (1);
    latitude         = ResultSetGetDoubleField   (2);
    longitude        = ResultSetGetDoubleField   (3);
    courseOverGround = ResultSetGetFloatField    (4);
    speedOverGround  = ResultSetGetFloatField    (5);

    DataBaseGpsDataPtr  gpsData = new DataBaseGpsData (cruiseName, utcDateTime, latitude, longitude, courseOverGround, speedOverGround);
    results->PushOnBack (gpsData);
  }

  ResulSetFree ();

  return  results;
}  /* GpsDataProcessResults */




void    DataBase::GpsDataInsert (const DataBaseGpsData&  gpsData)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("GpsDataInsert");
    return;
  }

  KKStr  insertStr = "call GpsDataInsert(" + gpsData.CruiseName ().QuotedStr ( )                        + ", " +
                                             gpsData.UtcDateTime ().YYYY_MM_DD_HH_MM_SS ().QuotedStr () + ", " +
                                             StrFormatDouble (gpsData.Latitude  (), "###0.0000000")     + ", " + 
                                             StrFormatDouble (gpsData.Longitude (), "###0.0000000")     + ", " +
                                             StrFormatDouble (gpsData.CourseOverGround (), "##0.0000")  + ", " +
                                             StrFormatDouble (gpsData.SpeedOverGround (), "##0.0000")   +
                                        ")";
  
  int32  returnCd = QueryStatement (insertStr);
  bool  successful = (returnCd == 0);

  ResultSetsClear ();
}  /* GpsDataInsert */




DataBaseGpsDataListPtr   DataBase::GpsDataQuery (const KKStr&     cruiseName,
                                                 const DateTime&  utcDateTimeStart,
                                                 const DateTime&  utcDateTimeEnd
                                                )
{
  DataBaseGpsDataListPtr   results = NULL;

  KKStr  queryStr = "call GpsDataQuery(" + cruiseName.QuotedStr ( )                        + ", " +
                                           utcDateTimeStart.YYYY_MM_DD_HH_MM_SS ().QuotedStr () + ", " +
                                           utcDateTimeEnd.YYYY_MM_DD_HH_MM_SS ().QuotedStr () + ", " +
                                        ")";
  
  int32  returnCd = QueryStatement (queryStr);
  if  (returnCd == 0)
    results = GpsDataProcessResults ();

  return results;
}  /* GpsDataQuery*/



void  DataBase::GpsDataDelete (const KKStr&     cruiseName,
                               const DateTime&  utcDateTimeStart,
                               const DateTime&  utcDateTimeEnd
                              )
{
  KKStr  queryStr = "call GpsDataDelete(" + cruiseName.QuotedStr ( )                             + ", " +
                                            utcDateTimeStart.YYYY_MM_DD_HH_MM_SS ().QuotedStr () + ", " +
                                            utcDateTimeEnd.YYYY_MM_DD_HH_MM_SS ().QuotedStr   () + ", " +
                                         ")";
  
  int32  returnCd = QueryStatement (queryStr);
  ResultSetsClear ();
  return;
}  /* GpsDataDelete */





//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//*                                 MLClass Routines                                     *
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************

MLClassListPtr   DataBase::MLClassProcessResults ()
{
  char const * const  fieldNames[] = {"ClassId", "ClassName", "ParentId", "Description", "ParentName", NULL};

  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
    return  NULL;

  MLClassListPtr  results = new MLClassList ();

  int32  classId = -1;
  KKStr  className (64);
  int32  parentId = -1;
  KKStr  description = "";
  KKStr  parentName = "";

  while  (ResultSetFetchNextRow ())
  {
    classId     = ResultSetGetIntField (0);
    className   = ResultSetGetField    (1);
    parentId    = ResultSetGetIntField (2);
    description = ResultSetGetField    (3);
    parentName  = ResultSetGetField    (4);

    MLClassPtr  ic = MLClass::CreateNewMLClass (className, classId);
    ic->Description (description);
    if  (!parentName.Empty ())
      ic->Parent (MLClass::CreateNewMLClass (parentName, parentId));
    else if  (parentId >= 0)
      ic->Parent (MLClass::GetByClassId (parentId));
      
    results->PushOnBack (ic);
  }

  ResulSetFree ();

  return  results;
}  /* MLClassProcessResults */




MLClassConstPtr  DataBase::MLClassLoad (const KKStr&  className)
{
  KKStr  selectStr (128);

  selectStr << "call ClassesRetrieveByName(" << className.QuotedStr () << ")";
  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
  {
    log.Level (-1) << endl << endl << "DataBase::MLClassLoad     ***ERROR***" << endl << endl
                   << "Error[" << lastMySqlErrorDesc << "]" << endl << endl;
    return NULL;
  }

  ResultSetLoad (NULL);
  if  (!resultSetMore)
    return  NULL;

  MLClassPtr  mlClass = NULL;
  while  (ResultSetFetchNextRow ())
  {
    //delete  mlClass;   //We can never delete an instance of MLClass because there is only 
                            //one global instance of each class that everone has to share.
    mlClass = NULL;
    
    int32  classId     = CharStarToInt32 (resultSetNextRow[0]);
    KKStr  className   = resultSetNextRow[1];
    int32  parentId    = CharStarToInt32 (resultSetNextRow[2]);
    KKStr  parentName  = resultSetNextRow[3];
    KKStr  description = resultSetNextRow[4];

    if  (!className.Empty ())
    {
      mlClass = MLClass::CreateNewMLClass (className, classId);
      mlClass->Description (description);
      mlClass->ClassId (classId);

      parentName.TrimLeft ();
      parentName.TrimRight ();
      if  (!parentName.Empty ())
        mlClass->Parent (MLClass::CreateNewMLClass (parentName, parentId));
    }
  }

  ResulSetFree ();

  ResultSetsClear ();
  return  mlClass;
}  /* MLClassLoad */




MLClassConstListPtr  DataBase::MLClassLoadList ()
{
  KKStr  selectStr = "call  ClassesRetrieveAll()";
  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
  {
    log.Level (-1) << endl << endl << "DataBase::MLClassLoadList     ***ERROR***" << endl << endl
                   << "Error[" << lastMySqlErrorDesc << "]" << endl << endl;
    return NULL;
  }

  ResultSetLoad (NULL);
  if  (!resultSetMore)
    return  NULL;

  // Since we are reloading ALL classes we will re-intialize the paremts of all classes to 'NULL'.  The
  // relationship will be re-established as they are reloaded from the database. This will help prevent
  // coruptions when we switch between MySQL servers.

  
  MLClass::ResetAllParentsToAllClasses ();

  MLClassConstListPtr  classes = new MLClassConstList ();

  vector<MLClassPtr>  classesWithParents;
  vector<KKStr>          classesWithParentsNames;

  while  (ResultSetFetchNextRow ())
  {
    int32  classId     = CharStarToInt32 (resultSetNextRow[0]);
    KKStr  className   = resultSetNextRow[1];
    int32  parentId    = CharStarToInt32 (resultSetNextRow[2]);
    KKStr  parentName  = resultSetNextRow[3];
    KKStr  description = resultSetNextRow[4];

    if  (!className.Empty ())
    {
      MLClassPtr  c = MLClass::CreateNewMLClass (className, classId);
      c->Description (description);
      classes->PushOnBack (c);
      c->StoredOnDataBase (true);

      if  (!parentName.Empty ())
      {
        classesWithParents.push_back (c);
        classesWithParentsNames.push_back (parentName);
      }
    }
  }

  ResulSetFree ();

  for  (uint32 x = 0;  x < classesWithParents.size ();  x++)
  {
    MLClassConstPtr parent = classes->LookUpByName (classesWithParentsNames[x]);
    if  (parent)
    {
      if  (parent->IsAnAncestor (classesWithParents[x]))
      {
        log.Level (-1) << endl << endl << "DataBase::LoadSipperFileRec    ***ERROR***     occured when retrieving result set." << endl << endl
                       << "    There is a Circular Parent Assignment" << endl 
                       << "    Class[" << classesWithParents[x]->Name () << "]   Parent[" << parent->Name () << "]" << endl
                       << endl;
        // We can not make this parent assignment;  otherwise we will end up crating a circular data structure.
      }
      else
      {
        classesWithParents[x]->Parent (parent);
      }
    }
  }

  ResultSetsClear ();

  return  classes;
}  /* MLClassLoadList */




void  DataBase::MLClassInsert (MLClass&  mlClass,
                                  bool&        successful
                                 )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("MLClassInsert");
    return;
  }


  KKStr  insertStr = "call MLClassInsert(" + 
                     mlClass.Name ().QuotedStr ()        + ", " + 
                     mlClass.ParentName  ().QuotedStr () + ", " +
                     mlClass.Description ().QuotedStr () +
                     ")";
  
  int32  returnCd = QueryStatement (insertStr);
  successful = (returnCd == 0);
  if  (successful)
  {
    // We perform the 'MLClassLoad' so that the classId field is populated with correct value.  Also note
    // that there is only one instance in memory of any goven class(See MLClass.h).
    KKStr  className = mlClass.Name ();
    MLClassLoad (className);
  }

  ResultSetsClear ();
}  /* MLClassInsert */



void  DataBase::MLClassInsertReturn (MLClass&  mlClass,
                                        bool&        successful
                                       )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("MLClassInsertReturn");
    return;
  }

  KKStr  insertStr = "call MLClassInsertReturn(" + 
                     mlClass.Name ().QuotedStr ()        + ", " + 
                     mlClass.ParentName  ().QuotedStr () +
                     ")";
  
  int32  returnCd = QueryStatement (insertStr);
  successful = (returnCd == 0);
  if  (successful)
  {
    MLClassListPtr  classes = MLClassProcessResults ();
    if  (!classes)
      successful = false;
    else if  (classes->QueueSize () < 1)
      successful = false;

    delete  classes;
    classes = NULL;
  }
}  /* MLClassInsertReturn */





void  DataBase::MLClassUpdate (const KKStr&       oldClassName,
                                  const MLClass&  mlClass,
                                  bool&              successful
                                 )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("MLClassUpdate");
    return;
  }

  KKStr updateStr = "Call MLClassUpdate(" +
                          oldClassName.QuotedStr ()              + ", " +
                          mlClass.Name ().QuotedStr ()        + ", " +
                          mlClass.ParentName  ().QuotedStr () + ", " +
                          mlClass.Description ().QuotedStr () +
                          ")";

  int32  returnCd = QueryStatement (updateStr);
  successful = (returnCd == 0);
  ResultSetsClear ();
}  /* MLClassUpdate */




void  DataBase::MLClassDelete (const KKStr&  mlClassName)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("MLClassDelete");
    return;
  }

  KKStr  deleteStr = "Call MLClassDelete(" + mlClassName.QuotedStr () + ")";
  int32  returnCd = QueryStatement (deleteStr);
  ResultSetsClear ();
}






//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*                                  Image  Routines                                      *
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************


KKU::uchar*  DataBase::EncodeARasterImageIntoAThumbNail (const RasterSipper&  image,
                                                         uint32               maxDimSizeAllowed,
                                                         uint32&              buffLen
                                                        )
{
  RasterSipperPtr  thumbNail = NULL;
  uint32  maxDim = Max (image.Height (), image.Width ());

  if  (maxDim > maxDimSizeAllowed)
  {
    float  reductionFactor = (float)maxDimSizeAllowed / (float)maxDim;
    //thumbNail = image.ReduceByEvenMultiple (multiple);
    thumbNail = image.ReduceByFactor (reductionFactor);
  }

  uchar*  compressedBuff = NULL;
  if  (thumbNail)
  {
    compressedBuff = thumbNail->SimpleCompression (buffLen);
    delete  thumbNail; 
    thumbNail = NULL;
  }
  else
  {
    compressedBuff = image.SimpleCompression (buffLen);
  }
  
  return  compressedBuff;
}  /* EncodeARasterImageIntoAThumbNail */





void  DataBase::ImageInsert (const RasterSipper&       image,
                             const KKStr&              imageFileName,
                             const KKStr&              sipperFileName,
                                   uint64              byteOffset,     // byteOffset of SipperRow containing TopLeftRow
                                   uint32              topLeftRow,
                                   uint32              topLeftCol,
                                   uint32              height,
                                   uint32              width,
                                   uint32              pixelCount,
                                   uchar               connectedPixelDist,
                                   uint32              extractionLogEntryId,
                                   uint32              classLogEntryId,
                                   uint32              centroidRow,
                                   uint32              centroidCol,
                                   MLClassConstPtr  class1,
                                   float               class1Prob,
                                   MLClassConstPtr  class2,
                                   float               class2Prob,
                                   MLClassConstPtr  validatedClass,
                                   float               depth,
                                   float               imageSize,
                                   PointListPtr        sizeCoordinates,
                                   int32&              imageId,
                                   bool&               successful
                            )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImageInsert");
    return;
  }

  KKStr  insertStatement (3000);
  KKStr  class1Name = "";
  KKStr  class2Name = "";
  KKStr  validatedClassName = "";

  if  (class1)
    class1Name = class1->Name ();

  if  (class2)
    class2Name = class2->Name ();

  if  (validatedClass)
    validatedClassName = validatedClass->Name ();

  uint32         imageCompressedBuffLen      = 0;
  uchar*         mySqlImageCompressedBuff    = NULL;
  unsigned long  mySqlImageCompressedBuffLen = 0;

  uchar* imageCompressedBuff = EncodeARasterImageIntoAThumbNail (image, 100, imageCompressedBuffLen);
  if  (imageCompressedBuff == NULL)
  {
    mySqlImageCompressedBuff = NULL;
    mySqlImageCompressedBuffLen = 0;
  }
  else
  {
    mySqlImageCompressedBuff = new uchar[imageCompressedBuffLen * 2 + 5];
    mySqlImageCompressedBuffLen = mysql_real_escape_string (conn, (char*)mySqlImageCompressedBuff, (char*)imageCompressedBuff, imageCompressedBuffLen);
  }

  //_fpclass

  KKStr  sizeCoordinatesStr (33);
  if  (sizeCoordinates)
  {
    PointList::const_iterator idx;
    int  x = 0;
    for  (idx = sizeCoordinates->begin ();  idx != sizeCoordinates->end ();  ++idx, ++x)
    {
      if  (x > 0)  sizeCoordinatesStr << ",";
      const PointPtr p = *idx;
      sizeCoordinatesStr << p->Row () << "," << p->Col ();
    }
  }

  insertStatement << "call ImagesInsert("
                  << imageFileName.QuotedStr ()       << ", "
                  << sipperFileName.QuotedStr ()      << ", "
                  << byteOffset                       << ", "
                  << topLeftRow                       << ", "
                  << topLeftCol                       << ", "
                  << height                           << ", "
                  << width                            << ", "
                  << pixelCount                       << ", "
                  << connectedPixelDist               << ", "
                  << extractionLogEntryId             << ", "
                  << classLogEntryId                  << ", "
                  << centroidRow                      << ", "
                  << centroidCol                      << ", "
                  << class1Name.QuotedStr ()          << ", "
                  << FloatToStr (class1Prob)          << ", "
                  << class2Name.QuotedStr ()          << ", "
                  << FloatToStr (class2Prob)          << ", "
                  << validatedClassName.QuotedStr ()  << ", "
                  << depth                            << ", "
                  << imageSize                        << ", "
                  << sizeCoordinatesStr.QuotedStr ()  << ", ";

  if  (mySqlImageCompressedBuff == NULL)
  {
    insertStatement << "NULL";
  }
  else
  {
    insertStatement.Append ('"');
    insertStatement.Append ((char*)mySqlImageCompressedBuff, mySqlImageCompressedBuffLen);
    insertStatement.Append ('"');
  }

  insertStatement << ")";

  int32  returnCd = QueryStatement (insertStatement);
  successful = (returnCd == 0);
  delete  mySqlImageCompressedBuff;
  mySqlImageCompressedBuff = NULL;
  
  delete  imageCompressedBuff;
  imageCompressedBuff = NULL;

  if  (successful)
  {
    imageId = (int32)mysql_insert_id (conn);

    ResultSetsClear ();

    int32  maxDim = Max (image.Height (), image.Width ());
    if  (maxDim > 100)
    {
      // The image had to be reduced to a thumbnail size so we want to store a copy of the original in the ImagesFullSize table.
      ImageFullSizeSave (imageFileName, image);
    }
  }
}  /* ImageInsert */





void  DataBase::ImagesEraseSipperFile (const KKStr&  _sipperFileName)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImagesEraseSipperFile");
    return;
  }

  KKStr  sipperFileName = osGetRootName (_sipperFileName);
  int32  returnCd = 0;

  KKStr  imageDeleteStatement = "call ImagesEraseSipperFile(" + sipperFileName.QuotedStr () + ")";
  returnCd = QueryStatement (imageDeleteStatement);
  ResultSetsClear ();
}  /* ImagesEraseSipperFile */




/**
 *@brief  Will remove all images and associated data for a specified SipperFile that are within the specified size range from the database.
 *@details Will also delete all Instrument data for the specified SIPPER file on the assumption that the calling procedure 
 * is going to re-extract instrument data.
 */
void  DataBase::ImagesEraseSipperFileSizeRange (const   KKStr&  _sipperFileName,
                                                uint32          _minSize,
                                                uint32          _maxSize
                                               )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImagesEraseSipperFileSizeRange");
    return;
  }

  KKStr  sipperFileName = osGetRootName (_sipperFileName);
  int32  returnCd = 0;

  if  (_maxSize == 0)
    _maxSize = INT_MAX;

  KKStr  queryStr = "call ImagesEraseSipperFileSizeRange(" + 
                                                         sipperFileName.QuotedStr ()  + ", "  +
                                                         StrFromUint32 (_minSize)     + ", "  +
                                                         StrFromUint32 (_maxSize)     +
                                                         ")";
  returnCd = QueryStatement (queryStr);
  ResultSetsClear ();
}  /* ImagesEraseSipperFileSizeRange */




ClassStatisticListPtr  DataBase::ImageProcessClassStaticsResults ()
{
  ResultSetLoad (NULL);
  if  (!resultSetMore)
    return  NULL;

  ClassStatisticListPtr  stats = new ClassStatisticList (true);

  int32  classId;
  KKStr  className;
  uint32 count;

  while  (ResultSetFetchNextRow ())
  {
    classId   = CharStarToInt32 (resultSetNextRow[0]);
    className = resultSetNextRow[1];
    count     = CharStarToInt32 (resultSetNextRow[2]);
    if  (count > 0)
    {
      ClassStatisticPtr  stat = new ClassStatistic (MLClass::CreateNewMLClass (className, classId), count);
      stats->PushOnBack (stat);
    }
  }

  ResulSetFree ();

  ResultSetsClear ();

  return  stats;
}  /* ImageProcessClassStaticsResults */




ClassStatisticListPtr  DataBase::ImageGetClassStatistics (DataBaseImageGroupPtr  imageGroup,
                                                          const KKStr&           sipperFileName,
                                                          MLClassConstPtr     mlClass,
                                                          char                   classKeyToUse,
                                                          float                  minProb,
                                                          float                  maxProb,
                                                          int32                  minSize,
                                                          int32                  maxSize,
                                                          float                  minDepth,
                                                          float                  maxDepth
                                                         )
{
  KKStr  selectStr (1024);

  KKStr  groupNameStr = ((imageGroup == NULL) ? "" : imageGroup->Name ());

  KKStr  classKeyToUseStr = "";
  classKeyToUseStr.Append (classKeyToUse);


  KKStr  className = "";
  if  (mlClass)
    className = mlClass->Name ();

  selectStr << "call ImagesGetClassStatistics("
            << groupNameStr.QuotedStr      () << ", "
            << sipperFileName.QuotedStr    () << ", "
            << className.QuotedStr         () << ", "
            << classKeyToUseStr.QuotedStr  () << ", "
            << minProb                        << ", "
            << maxProb                        << ", "
            << minSize                        << ", "
            << maxSize                        << ", "
            << minDepth                       << ", "
            << maxDepth
            << ")";

  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  ClassStatisticListPtr  stats = ImageProcessClassStaticsResults ();
  ResultSetsClear ();
 
  return  stats;
}  /* ImageGetClassStatistics */




ClassStatisticListPtr  DataBase::ImageGetClassStatistics (DataBaseImageGroupPtr  imageGroup,
                                                          const KKStr&           cruiseName,
                                                          const KKStr&           stationName,
                                                          const KKStr&           deploymentNum,
                                                          MLClassConstPtr     mlClass,
                                                          char                   classKeyToUse,
                                                          float                  minProb,
                                                          float                  maxProb,
                                                          int32                  minSize,
                                                          int32                  maxSize,
                                                          float                  minDepth,
                                                          float                  maxDepth
                                                         )
{
  if  (cruiseName.Empty ()  &&  stationName.Empty ()  &&  deploymentNum.Empty ())
  {
    // We do not care about individule SipperFiles
    return  ImageGetClassStatistics (imageGroup, "", mlClass, classKeyToUse, minProb, maxProb, minSize, maxSize, minDepth, maxDepth);
  }

  VectorKKStr*  sipperFiles = SipperFileGetList (cruiseName, stationName, deploymentNum);

  if  (!sipperFiles)
    return NULL;

  ClassStatisticListPtr  stats = new ClassStatisticList (true);

  VectorKKStr::iterator  idx;
  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  idx++)
  {
    ClassStatisticListPtr  statsOneFile = ImageGetClassStatistics (imageGroup, *idx, mlClass, classKeyToUse, minProb, maxProb, minSize, maxSize, minDepth, maxDepth);
    if  (statsOneFile)
    {
      (*stats) += (*statsOneFile);
      delete  statsOneFile;
      statsOneFile = NULL;
    }
  }

  delete  sipperFiles;

  return  stats;
}  /* ImageGetClassStatistics */




VectorUint*  DataBase::ImageGetDepthStatistics (DataBaseImageGroupPtr  imageGroup,
                                                const KKStr&           sipperFileName,
                                                float                  depthIncrements,
                                                MLClassConstPtr     mlClass,
                                                char                   classKeyToUse,
                                                float                  minProb,
                                                float                  maxProb,
                                                int32                  minSize,
                                                int32                  maxSize
                                               )
{
  KKStr  selectStr (1024);

  KKStr  groupNameStr = ((imageGroup == NULL) ? "" : imageGroup->Name ());
  KKStr  classNameStr = ((mlClass == NULL) ? "" : mlClass->Name ());

  KKStr  classKeyToUseStr = "";
  classKeyToUseStr.Append (classKeyToUse);

  if  ((depthIncrements <= 0.0f)  ||  (depthIncrements > 1000.0f))
  {
    log.Level (-1) << endl << endl << "DataBase::ImageGetDepthStatistics    ***ERROR***    depthIncrements[" << depthIncrements << "] is unrealistic,  defaulting to 10.0 meters." << endl << endl;
    depthIncrements = 10.0f;
  }
  
  selectStr << "call ImagesGetDepthStatistics("
            << groupNameStr.QuotedStr      () << ", "
            << sipperFileName.QuotedStr   ()  << ", "
            << depthIncrements                << ", "
            << classNameStr.QuotedStr     ()  << ", "
            << classKeyToUseStr.QuotedStr ()  << ", "
            << minProb                        << ", "
            << maxProb                        << ", "
            << minSize                        << ", "
            << maxSize
            << ")";

  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  VectorUint*  counts = new VectorUint;
  ResultSetLoad (NULL);
  if  (!resultSetMore)
    return  counts;

  int32   incIndex;
  uint32  count;

  while  (ResultSetFetchNextRow ())
  {
    incIndex = atoi (resultSetNextRow[0]);
    if  (incIndex < 0)
      incIndex = 0;

    count = CharStarToUint32 (resultSetNextRow[1]);

    while  (counts->size () <= (uint32)incIndex)
      counts->push_back (0);

    (*counts)[incIndex] = count;
  }

  ResulSetFree ();

  ResultSetsClear ();

  return  counts;
}  /* ImageGetDepthStatistics */




VectorUint*  DataBase::ImageGetDepthStatistics (DataBaseImageGroupPtr  imageGroup,
                                                const KKStr&           cruiseName,
                                                const KKStr&           stationName,
                                                const KKStr&           deploymentNum,
                                                float                  depthIncrements,
                                                MLClassConstPtr     mlClass,
                                                char                   classKeyToUse,
                                                float                  minProb,
                                                float                  maxProb,
                                                int32                  minSize,
                                                int32                  maxSize,
                                                bool&                  cancelFlag
                                               )
{
  if  ((depthIncrements <= 0.0f)  ||  (depthIncrements > 1000.0f))
  {
    log.Level (-1) << endl
                   << endl
                   << "DataBase::ImageGetDepthStatistics    ***ERROR***    depthIncrements[" << depthIncrements << "] is unrealistic,  defaulting to 10.0 meters." << endl
                   << endl;
    depthIncrements = 10.0f;
  }


  bool  allSipperFiles = cruiseName.Empty ()  &&  stationName.Empty ()  &&  deploymentNum.Empty ();
  if  (allSipperFiles)
  {
    return ImageGetDepthStatistics (imageGroup, "", depthIncrements, mlClass, classKeyToUse, minProb, maxProb, minSize, maxSize);
  }

  VectorKKStr*  sipperFiles = SipperFileGetList (cruiseName, stationName, deploymentNum);
  if  (!sipperFiles)
    return  NULL;

  VectorUint* counts = new VectorUint ();

  VectorKKStr::iterator  idx;
  for  (idx = sipperFiles->begin ();  ((idx != sipperFiles->end ())  &&  (!cancelFlag));  idx++)
  {
    VectorUint*  countsThisSipperFile = ImageGetDepthStatistics (imageGroup, *idx, depthIncrements, mlClass, classKeyToUse, minProb, maxProb, minSize, maxSize);
    if  (countsThisSipperFile)
    {
      while  (counts->size () <= countsThisSipperFile->size ())
        counts->push_back (0);
      for  (uint32 x = 0;  x < countsThisSipperFile->size ();  x++)
        (*counts)[x] += (*countsThisSipperFile)[x];

      delete  countsThisSipperFile;
      countsThisSipperFile = NULL;
    }
  }

  delete  sipperFiles;

  return  counts;
}  /* ImageGetDepthStatistics */



DataBaseImageListPtr  DataBase::ImageQueryProcessResults ()
{
  ResultSetLoad (NULL);
  if  (!resultSetMore)
    return  NULL;

  DataBaseImageListPtr  results = new DataBaseImageList (true);
 
  int32  imageIdIdx              = -1;
  int32  sipperFileNameIdx       = -1;
  int32  class1IdIdx             = -1;
  int32  class1NameIdx           = -1; 
  int32  class1ProbIdx           = -1;
  int32  class2IdIdx             = -1;
  int32  class2NameIdx           = -1;
  int32  class2ProbIdx           = -1;
  int32  ClassValidatedIdIdx     = -1;
  int32  classNameValidatedIdx   = -1;
  int32  topLeftRowIdx           = -1;
  int32  topLeftColIdx           = -1;
  int32  pixelCountIdx           = -1;
  int32  connectedPixelDistIdx   = -1;
  int32  extractionLogEntryIdIdx = -1;
  int32  classLogEntryIdIdx      = -1;
  int32  centroidRowIdx          = -1;
  int32  centroidColIdx          = -1;
  int32  heightIdx               = -1;
  int32  widthIdx                = -1;
  int32  byteOffsetIdx           = -1;
  int32  depthIdx                = -1;
  int32  imageSizeIdx            = -1;
  int32  sizeCoordinatesIdx      = -1;
  int32  thumbnailIdx            = -1;

  {
    // Build Field Index
    MYSQL_FIELD *field;
    int32  idx = 0;
    while  ((field = mysql_fetch_field (resultSet)))
    {
      KKStr  fieldName = field->name;
      if  (fieldName.CompareIgnoreCase ("ImageId") == 0)
        imageIdIdx = idx;
      
      else if  (fieldName.CompareIgnoreCase ("sipperFileName") == 0)
        sipperFileNameIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("class1Id") == 0)
        class1IdIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("class1Name") == 0)
        class1NameIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("class1Prob") == 0)
        class1ProbIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("class2Id") == 0)
        class2IdIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("class2Name") == 0)
        class2NameIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("class2Prob") == 0)
        class2ProbIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("ClassValidatedId") == 0)
        ClassValidatedIdIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("ClassNameValidated") == 0)
        classNameValidatedIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("topLeftRow") == 0)
        topLeftRowIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("topLeftCol") == 0)
        topLeftColIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("pixelCount") == 0)
        pixelCountIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("ConnectedPixelDist") == 0)
        connectedPixelDistIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("ExtractionLogEntryId") == 0)
        extractionLogEntryIdIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("ClassLogEntryId") == 0)
        classLogEntryIdIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("CentroidRow") == 0)
        centroidRowIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("CentroidCol") == 0)
        centroidColIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("height") == 0)
        heightIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("width") == 0)
        widthIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("byteOffset") == 0)
        byteOffsetIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("depth") == 0)
        depthIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("ImageSize") == 0)
        imageSizeIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("SizeCoordinates") == 0)
        sizeCoordinatesIdx = idx;

      else if  (fieldName.CompareIgnoreCase ("thumbnail") == 0)
        thumbnailIdx = idx;

      idx++;
    }
  }

  uint32 numFields = resultSetNumFields;

  uint32 imageId            = 0;
  int32  class1Id           = -1;
  KKStr  class1Name         = "";
  int32  class2Id           = -1;
  KKStr  class2Name         = "";
  int32  classValidatedId   = -1;
  KKStr  classNameValidated = "";

  while  (ResultSetFetchNextRow ())
  {
    ulong*  lengths    = mysql_fetch_lengths (resultSet);
    DataBaseImagePtr  i = new DataBaseImage ();

    class1Id           = -1;
    class1Name         = "";
    class2Id           = -1;
    class2Name         = "";
    classValidatedId   = -1;
    classNameValidated = "";
    imageId            = 0;

    if  (imageIdIdx >= 0)
      i->ImageId (CharStarToUint32 (resultSetNextRow[imageIdIdx]));

    if  (sipperFileNameIdx >= 0)
      i->SipperFileName (resultSetNextRow[sipperFileNameIdx]);

    if  (class1IdIdx >= 0)
    {
      if  (resultSetNextRow[class1IdIdx])
        class1Id = CharStarToInt32 (resultSetNextRow[class1IdIdx]);
    }

    if  (class1NameIdx >= 0)
    {
      KKStr  class1Name = resultSetNextRow[class1NameIdx];
      if  (class1Name.Empty ())
        class1Name = "ClassId_" + StrFormatInt (class1Id, "0000");
      i->Class1 (MLClass::CreateNewMLClass (class1Name, class1Id));
    }

    if  (class1ProbIdx >= 0)
      i->Class1Prob (CharStarToFloat (resultSetNextRow[class1ProbIdx]));

    if  (class2IdIdx >= 0)
    {
      if  (resultSetNextRow[class2IdIdx])
        class2Id = CharStarToInt32 (resultSetNextRow[class2IdIdx]);
    }

    if  (class2NameIdx >= 0)
    {
      KKStr  class2Name = resultSetNextRow[class2NameIdx];
      if  (!class2Name.Empty ())
        i->Class2 (MLClass::CreateNewMLClass (resultSetNextRow[class2NameIdx], class2Id));
    }

    if  (class2ProbIdx >= 0)
      i->Class2Prob (CharStarToFloat (resultSetNextRow[class2ProbIdx]));

    if  (ClassValidatedIdIdx >= 0)
    {
      if  (resultSetNextRow[ClassValidatedIdIdx])
        classValidatedId = CharStarToInt32 (resultSetNextRow[ClassValidatedIdIdx]);
    }

    if  (classNameValidatedIdx >= 0)
    {
      KKStr classNameValidated = resultSetNextRow[classNameValidatedIdx];
      if  (!classNameValidated.Empty ())
        i->ValidatedClass (MLClass::CreateNewMLClass (classNameValidated, classValidatedId));
    }

    if  (topLeftRowIdx >= 0)
      i->TopLeftRow (CharStarToUint32  (resultSetNextRow[topLeftRowIdx]));

    if  (topLeftColIdx >= 0)
      i->TopLeftCol (CharStarToUint32  (resultSetNextRow[topLeftColIdx]));

    if  (pixelCountIdx >= 0)
      i->PixelCount (CharStarToUint32  (resultSetNextRow[pixelCountIdx]));

    if  (connectedPixelDistIdx >= 0)
      i->ConnectedPixelDist ((uchar)CharStarToUint32 (resultSetNextRow[connectedPixelDistIdx]));

    if  (extractionLogEntryIdIdx >= 0)
      i->ExtractionLogEntryId (CharStarToUint32 (resultSetNextRow[extractionLogEntryIdIdx]));

    if  (classLogEntryIdIdx >= 0)
      i->ClassLogEntryId (CharStarToUint32 (resultSetNextRow[classLogEntryIdIdx]));

    if  (centroidRowIdx >= 0)
      i->CentroidRow (CharStarToUint32 (resultSetNextRow[centroidRowIdx]));

    if  (centroidColIdx >= 0)
      i->CentroidCol (CharStarToUint32 (resultSetNextRow[centroidColIdx]));

    if  (heightIdx >= 0)
      i->Height (CharStarToUint32  (resultSetNextRow[heightIdx]));

    if  (widthIdx >= 0)
      i->Width (CharStarToUint32  (resultSetNextRow[widthIdx]));

    if  (byteOffsetIdx >= 0)
      i->ByteOffset (CharStarToInt64 (resultSetNextRow[byteOffsetIdx]));

    if  (depthIdx >= 0)
      i->Depth (CharStarToFloat (resultSetNextRow[depthIdx]));

    if  (imageSizeIdx >= 0)
      i->ImageSize (CharStarToFloat (resultSetNextRow[imageSizeIdx]));

    if  (sizeCoordinatesIdx >= 0)
    {
      i->SizeCoordinates (resultSetNextRow[sizeCoordinatesIdx]);
    }

    if  (thumbnailIdx >= 0)
    {
      int32 blockLen = lengths[thumbnailIdx];

      uchar*  encodedThumbNail = new uchar[blockLen];
      memcpy (encodedThumbNail, resultSetNextRow[thumbnailIdx], blockLen);

      int32 encBlockLen = encodedThumbNail[0] * 256 + encodedThumbNail[1];  // This should be the same as 'blockLen'

      if  (encBlockLen != blockLen)
      {
        log.Level (-1)  << "DataBase::ImageQueryProcessResults     ***ERROR***"  << endl
                        << endl
                        << "DataBase::ImageQueryProcessResults      There is an error in Compress Block Length data." << endl
                        << endl;
      }

      i->EncodedThumbnail    (encodedThumbNail);
      i->EncodedThumbnailLen (blockLen);
    }

    results->PushOnBack (i);
  }

  ResulSetFree ();

  return  results;
}  /* ImageQueryProcessResults */





DataBaseImagePtr  DataBase::ImagesLocateClosestImage (const KKStr&  imageFileName)
{
  KKStr   sipperFileName (32);
  uint32  scanLineNum = 0;
  uint32  scanCol     = 0;
  ImageFeatures::ParseImageFileName (imageFileName, sipperFileName, scanLineNum, scanCol);
  return  ImagesLocateClosestImage (sipperFileName, scanLineNum, scanCol);
}  /* ImagesLocateClosestImage */





DataBaseImagePtr  DataBase::ImagesLocateClosestImage (const KKStr&  sipperFileName,
                                                      uint32        scanLine,
                                                      uint32        scanCol
                                                     )
{
  KKStr  sqlStr = "call  ImagesLocateClosestImage(" + sipperFileName.QuotedStr ()         + ", " + 
                                                      StrFormatInt (scanLine, "ZZZZZZZ0") + ", " +
                                                      StrFormatInt (scanCol,  "ZZZZZZZ0") + 
                                                 ")";
  

  KKStrMatrixPtr  results = QueryStatementKKStr (sqlStr, NULL);
  if  (results == NULL)
    return NULL;

  if  ((results->NumRows () < 1)  ||  (results->NumCols () < 2))
  {
    delete  results;
    return NULL;
  }

  KKStr  imageFileNameInDataBase = (*results)[0][1];
  delete  results;  results = NULL;

  return  ImageLoad (imageFileNameInDataBase);
}  /* ImagesLocateClosestImage */




DataBaseImagePtr  DataBase::ImageLoad (uint32  imageId)
{
  KKStr selectStr (100);
  selectStr << "call ImagesLoadByImageId(" << imageId << ")";
  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  DataBaseImageListPtr  images =  ImageQueryProcessResults ();

  ResultSetsClear ();
 
  if  ((images == NULL)  ||  (images->QueueSize () < 1))
  {
    delete  images;  images = NULL;
    return NULL;
  }
  DataBaseImagePtr  dbImage = images->PopFromBack ();
  delete  images; images = NULL;

  return  dbImage;
}  /* ImageLoad */




DataBaseImagePtr  DataBase::ImageLoad (const KKStr&   imageFileName)
{
  if  (this == NULL)
  {
    cerr  << endl << "DataBase::ImageLoad     this == NULL" << endl << endl;
  }

  KKStr selectStr = "call ImagesLoadByImageFileName(" + imageFileName.QuotedStr () + ")";
  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  DataBaseImageListPtr  images =  ImageQueryProcessResults ();

  ResultSetsClear ();
 
  if  ((images == NULL)  ||  (images->QueueSize () < 1))
  {
    delete  images;  images = NULL;
    return NULL;
  }
  DataBaseImagePtr  dbImage = images->PopFromBack ();
  delete  images; images = NULL;

  return  dbImage;
}  /* ImageLoad */




DataBaseImageListPtr  DataBase::ImagesQuery (DataBaseImageGroupPtr  group,
                                             bool                   includeThumbnail,
                                             const bool&            cancelFlag
                                            )
{
  if  (!group)
    return NULL;

  KKStr  selectStr = "call  ImagesQueryByImageGroup(" + 
                     group->Name ().QuotedStr () + ", " +
                     (includeThumbnail ? "true":"false") +
                     ")";

  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  DataBaseImageListPtr results = ImageQueryProcessResults ();

  ResultSetsClear ();

  return  results;
}  /* ImagesQuery */
    



DataBaseImageListPtr  DataBase::ImagesQuery (DataBaseImageGroupPtr  imageGroup,
                                             const KKStr&           sipperFileName,
                                             MLClassConstPtr     mlClass,
                                             char                   classKeyToUse,
                                             float                  probMin,
                                             float                  probMax,
                                             int32                  sizeMin,
                                             int32                  sizeMax,
                                             float                  depthMin,
                                             float                  depthMax,
                                             uint32                 restartImageId,
                                             int32                  limit,            // Max # of rows 2 return.  -1 idicates no limit.
                                             bool                   includeThumbnail,
                                             const bool&            cancelFlag
                                            )
{
  KKStr  selectStr(512);

  KKStr  emptyStr = "\"\"";

  KKStr  groupNameStr = ((imageGroup == NULL) ? "" : imageGroup->Name ());
  KKStr  classNameStr = ((mlClass == NULL) ? "" : mlClass->Name ());
  KKStr  classKeyToUseStr = "";
  classKeyToUseStr.Append (classKeyToUse);


  selectStr << "call  ImagesQuery("  
            << groupNameStr.QuotedStr     ()  << ", "
            << sipperFileName.QuotedStr   ()  << ", " 
            << classNameStr.QuotedStr     ()  << ", "
            << classKeyToUseStr.QuotedStr ()  << ", "
            << probMin  << ", " << probMax    << ", "
            << sizeMin  << ", " << sizeMax    << ", "
            << depthMin << ", " << depthMax   << ", "
            << restartImageId                 << ", "
            << limit                          << ", "  
            << (includeThumbnail ? "true" : "false")
            << ")";
   
  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  DataBaseImageListPtr results = ImageQueryProcessResults ();

  ResultSetsClear ();

  return  results;
}  /* ImagesQuery */




DataBaseImageListPtr  DataBase::ImagesQuery (DataBaseImageGroupPtr  imageGroup,
                                             const KKStr&           cruiseName,
                                             const KKStr&           stationName,
                                             const KKStr&           deploymentNum,
                                             MLClassConstPtr     mlClass,
                                             char                   classKeyToUse,
                                             float                  probMin,
                                             float                  probMax,
                                             int32                  sizeMin,
                                             int32                  sizeMax,
                                             float                  depthMin,
                                             float                  depthMax,
                                             uint32                 restartImageId,
                                             int32                  limit,            // Max # of rows to return.  -1 indicates no limit.,
                                             bool                   includeThumbnail,
                                             const bool&            cancelFlag
                                            )
{
  KKStr  selectStr (4096);
  VectorKKStr*  sipperFiles = SipperFileGetList (cruiseName, stationName, deploymentNum);

  if  (!sipperFiles)
    return NULL;

  DataBaseImageListPtr  images = new DataBaseImageList (true);

  VectorKKStr::iterator  idx;
  for  (idx = sipperFiles->begin ();  (idx != sipperFiles->end ())  &&  (!cancelFlag);  idx++)
  {
    DataBaseImageListPtr  imagesForOneSipperFile = ImagesQuery (imageGroup,
                                                                *idx,           mlClass,  classKeyToUse,
                                                                probMin,        probMax, 
                                                                sizeMin,        sizeMax, 
                                                                depthMin,       depthMax, 
                                                                restartImageId, limit,
                                                                includeThumbnail,        //  true = include thumbNail
                                                                cancelFlag
                                                               );
    if  (imagesForOneSipperFile)
    {
      imagesForOneSipperFile->Owner (false);
      images->AddQueue (*imagesForOneSipperFile);
      delete  imagesForOneSipperFile;
      imagesForOneSipperFile = NULL;
    }
  }

  if  (cancelFlag)
  {
    delete  images;
    images = NULL;
  }

  delete  sipperFiles;

  return  images;
}  /* ImagesQuery */



DataBaseImageListPtr  DataBase::ImagesQueryByGrouop 
                                  (DataBaseImageGroupPtr  imageGroup,
                                   const KKStr&           cruiseName,
                                   const KKStr&           stationName,
                                   const KKStr&           deploymentNum,
                                   const KKStr&           sipperFileName,
                                   MLClassConstPtr        mlClass,
                                   char                   classKeyToUse,
                                   float                  probMin,
                                   float                  probMax,
                                   int32                  sizeMin,
                                   int32                  sizeMax,
                                   float                  depthMin,
                                   float                  depthMax,
                                   uint32                 restartImageId,
                                   int32                  limit,            // Max # of rows to return.  -1 indicates no limit.,
                                   bool                   includeThumbnail
                                  )
{
  KKStr  sqlStr(512);

  KKStr  imageGroupName (32);
  if  (imageGroup)  imageGroupName = imageGroup->Name ();

  KKStr  className (32);
  if  (mlClass)  className = mlClass->Name ();

  KKStr  classKeyToUseStr = "";
  classKeyToUseStr.Append (classKeyToUse);

  sqlStr    << "call  ImagesQueryByGrouop("  
            << imageGroupName.QuotedStr   ()         << ", "
            << cruiseName.QuotedStr       ()         << ", "
            << stationName.QuotedStr      ()         << ", "
            << deploymentNum.QuotedStr    ()         << ", "
            << sipperFileName.QuotedStr   ()         << ", "
            << className.QuotedStr        ()         << ", "
            << classKeyToUseStr.QuotedStr ()         << ", "
            << probMin  << ", " << probMax           << ", "
            << sizeMin  << ", " << sizeMax           << ", "
            << depthMin << ", " << depthMax          << ", "
            << restartImageId                        << ", "
            << limit                                 << ", "  
            << (includeThumbnail ? "true" : "false")
            << ")";
   
  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return NULL;

  DataBaseImageListPtr results = ImageQueryProcessResults ();

  ResultSetsClear ();

  return  results;
}  /* ImagesQuery */






DataBaseImageListPtr  DataBase::ImagesQueryForScanLineRange (const KKStr&   sipperFileName,
                                                             uint32         scanLineStart,
                                                             uint32         scanLineEnd
                                                            )
{
  KKStr selectStr = "call ImagesQueryForScanLineRange(" + sipperFileName.QuotedStr ()            + ", "
                                                        + StrFormatInt (scanLineStart, "ZZZZZ0") + ", "
                                                        + StrFormatInt (scanLineStart, "ZZZZZ0") +
                                                      ")";

  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  DataBaseImageListPtr  results = ImageQueryProcessResults ();
  ResultSetsClear ();
  return  results;
}  /* ImagesQueryForScanLineRange */




VectorKKStr*   DataBase::ImageListOfImageFileNamesByScanLineRange (const KKStr&   sipperFileName,
                                                                   uint32         scanLineStart,
                                                                   uint32         scanLineEnd
                                                                  )
{
  KKStr selectStr = "call ImagesImageFileNamesByScanLineRange(" + sipperFileName.QuotedStr ()            + ", "
                                                                + StrFormatInt (scanLineStart, "ZZZZZ0") + ", "
                                                                + StrFormatInt (scanLineStart, "ZZZZZ0") +
                                                             ")";

  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return NULL;

  char*  fieldNames[] = {"ImageId", "ImageFileName", NULL};
  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
    return  NULL;

  VectorKKStr*  results = new VectorKKStr ();

  MYSQL_ROW  row = NULL;
  while  (ResultSetFetchNextRow ())
    results->push_back (ResultSetGetField (1));

  ResulSetFree ();

  ResultSetsClear ();

  return  results;
}  /* ImageListOfImageFileNamesByScanLineRange */





void  DataBase::ImagesUpdatePredictions (const KKStr&        imageFileName,
                                         MLClassConstPtr  class1Pred,
                                         float               class1Prob,
                                         MLClassConstPtr  class2Pred,
                                         float               class2Prob,
                                         uint32              logEntryId
                                       )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImagesUpdatePredictions");
    return;
  }

  KKStr  updateStr (512);
  if  ((class1Pred == NULL)  &&  (class2Pred == NULL))
    return;

  KKStr  emptyStr = "";
  KKStr  c1Name = ((class1Pred) ?  class1Pred->Name () : emptyStr);
  KKStr  c2Name = ((class2Pred) ?  class2Pred->Name () : emptyStr);

  updateStr << "call ImagesUpdatePredictions("  << imageFileName.QuotedStr () << ", "
                                                << c1Name.QuotedStr ()        << ", "
                                                << class1Prob                 << ", "
                                                << c2Name.QuotedStr ()        << ", "
                                                << class2Prob                 << ", "
                                                << logEntryId
                                                << ")";
  int32  returnCd = QueryStatement (updateStr);

  ResultSetsClear ();

}  /* ImagesUpdatePredictions */




void  DataBase::ImagesUpdatePredictionsList (uint32        _logEntryId,
                                             const KKStr&  _predictionList
                                            )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImagesUpdatePredictionsList");
    return;
  }

  KKStr  sqlStr (64 + _predictionList.Len ());
  sqlStr << "call  ImagesUpdatePredictionsList(" << _logEntryId << "," << _predictionList.QuotedStr () << ")";
  int32  returnCd = QueryStatement (sqlStr);
  ResultSetsClear ();
}  /* ImagesUpdatePredictionsList */
 





void  DataBase::ImagesUpdateValidatedClass (const KKStr&        imageFileName, 
                                            MLClassConstPtr  mlClass
                                           )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImagesUpdateValidatedClass");
    return;
  }

  if  (!mlClass)
    return;

  KKStr  updateStr (512);

  updateStr << "call  ImagesUpdateValidatedClass(" << imageFileName.QuotedStr ()       << ", " 
            <<                                        mlClass->Name ().QuotedStr () << ", "
            <<                                        "1.0"                                                   
            <<                                 ")";
  int32  returnCd = QueryStatement (updateStr);
  ResultSetsClear ();
}  /* ImagesUpdateValidatedClass */




void  DataBase::ImagesUpdateImageSize (const KKStr&        imageFileName,
                                       const PointListPtr  sizeCoordinates,
                                       float               imageSize
                                      )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImagesUpdateImageSize");
    return;
  }

  KKStr  sizeCoordinatesStr = "";
  if  (sizeCoordinates)
    sizeCoordinatesStr = sizeCoordinates->ToDelStr (',');

  KKStr  updateStr (256);

  updateStr << "call  ImagesUpdateImageSize(" << imageFileName.QuotedStr ()      << ", " 
            <<                                   sizeCoordinatesStr.QuotedStr () << ", "
            <<                                   imageSize
            <<                             ")";
  int32  returnCd = QueryStatement (updateStr);
  ResultSetsClear ();
}





void  DataBase::ImageRemoveValidation (const KKStr&   imageFileName)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImageRemoveValidation");
    return;
  }

  KKStr  updateStr (512);

  updateStr << "call  ImagesRemoveValidation(" << imageFileName.QuotedStr () << ")";
  int32  returnCd = QueryStatement (updateStr);
  ResultSetsClear ();
}  /* ImageRemoveValidation */




DataBaseImageValidatedEntryListPtr
      DataBase::ImagesRetrieveValidatedClass (const KKStr&  sipperFileName)
{
  KKStr  sqlStr (128);
  sqlStr << "call  ImagesRetrieveValidatedClass ("
         << osGetRootName (sipperFileName).QuotedStr ()
         << ")";
  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)  
    return NULL;

  char*  fieldNames[] = {"ImageId",  "ImageFileName", "ClassValidatedId", "ClassNameValidated", "SizeCoordinates", NULL};
  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
    return  NULL;

  DataBaseImageValidatedEntryListPtr  results = new DataBaseImageValidatedEntryList (true);
  MYSQL_ROW  row = NULL;
  while  (ResultSetFetchNextRow ())
  {
    KKStr  imageFileName = ResultSetGetField (1);
    int32  validatedClassId = ResultSetGetIntField (2);
    KKStr  validateClassName = ResultSetGetField (3);
    KKStr  sizeCoordinatesStr = ResultSetGetField (4);

    MLClassConstPtr  validatedClass = NULL;
    if  (!validateClassName.Empty ())
      validatedClass = MLClass::CreateNewMLClass (validateClassName, validatedClassId);
    results->push_back (new DataBaseImageValidatedEntry (imageFileName, sizeCoordinatesStr, validatedClass));
  }

  ResulSetFree ();

  ResultSetsClear ();

  return  results;
}  /* ImagesRetrieveValidatedClass */





void  DataBase::ImagesDeleteOneImage (const KKStr&   imageFileName)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImagesDeleteOneImage");
    return;
  }

  KKStr  updateStr (512);

  updateStr << "call  ImagesDeleteOneImage(" << imageFileName.QuotedStr () << ")";
  int32  returnCd = QueryStatement (updateStr);
  ResultSetsClear ();
}  /* ImagesDeleteOneImage */





void  DataBase::ImageUpdate (DataBaseImage&  dbImage,
                             RasterSipper&   image
                            )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImageUpdate");
    return;
  }

  KKStr  sqlStr (3000);
  KKStr  class1Name = "";
  KKStr  class2Name = "";
  KKStr  validatedClassName = "";

  class1Name = dbImage.Class1Name ();
  class2Name = dbImage.Class2Name ();
  validatedClassName = dbImage.ValidatedClassName ();

  uint32         imageCompressedBuffLen      = 0;
  uchar*         mySqlImageCompressedBuff    = NULL;
  unsigned long  mySqlImageCompressedBuffLen = 0;

  uchar* imageCompressedBuff = EncodeARasterImageIntoAThumbNail (image, 100, imageCompressedBuffLen);
  if  (imageCompressedBuff == NULL)
  {
    mySqlImageCompressedBuff = NULL;
    mySqlImageCompressedBuffLen = 0;
  }
  else
  {
    // In a worst case situation the esape string could be twoce as long as the source string plus 5 bytes overhead.
    mySqlImageCompressedBuff = new uchar[imageCompressedBuffLen * 2 + 5];
    mySqlImageCompressedBuffLen = mysql_real_escape_string (conn, (char*)mySqlImageCompressedBuff, (char*)imageCompressedBuff, imageCompressedBuffLen);
  }

  sqlStr << "call ImagesUpdate("
         << dbImage.ImageId        ()               << ", "
         << dbImage.ImageFileName  ().QuotedStr ()  << ", "
         << dbImage.SipperFileName ().QuotedStr ()  << ", "
         << (int64)dbImage.ByteOffset ()            << ", "
         << dbImage.TopLeftRow           ()         << ", "
         << dbImage.TopLeftCol           ()         << ", "
         << dbImage.Height               ()         << ", "
         << dbImage.Width                ()         << ", "
         << dbImage.PixelCount           ()         << ", "
         << dbImage.ConnectedPixelDist   ()         << ", "
         << dbImage.ClassLogEntryId      ()         << ", "
         << dbImage.ExtractionLogEntryId ()         << ", "
         << dbImage.CentroidRow          ()         << ", "
         << dbImage.CentroidCol          ()         << ", "
         << class1Name.QuotedStr         ()         << ", "
         << dbImage.Class1Prob           ()         << ", "
         << class2Name.QuotedStr         ()         << ", "
         << dbImage.Class2Prob           ()         << ", "
         << validatedClassName.QuotedStr ()         << ", "
         << dbImage.Depth                ()         << ", ";

  if  (mySqlImageCompressedBuff == NULL)
  {
    sqlStr << "NULL";
  }
  else
  {
    sqlStr.Append ('"');
    sqlStr.Append ((char*)mySqlImageCompressedBuff, mySqlImageCompressedBuffLen);
    sqlStr.Append ('"');
  }

  sqlStr << ")";

  int32 returnCd = QueryStatement (sqlStr);
  bool  successful = (returnCd == 0);
  delete  mySqlImageCompressedBuff;
  mySqlImageCompressedBuff = NULL;
  
  delete  imageCompressedBuff;
  imageCompressedBuff = NULL;

  if  (successful)
  {
    ResultSetsClear ();
    sqlStr = "delete  from  ImagesFullSize  where  ImageId = " + StrFormatInt (dbImage.ImageId (), "ZZZZZZZZ0");
    int32  returnCd = QueryStatement (sqlStr);
    ResultSetsClear ();

    int32  maxDim = Max (dbImage.Height (), dbImage.Width ());
    if  (maxDim > 100)
    {
      // The image had to be reduced to a thumbnail size so we want to store a copy of the original in the ImagesFullSize table.
      ImageFullSizeSave (dbImage.ImageFileName (), image);
    }
  }
}  /* ImageUpdate */





void  DataBase::ImagesUpdateValidatedAndPredictClass (const KKStr&        imageFileName, 
                                                      MLClassConstPtr  mlClass, 
                                                      float               class1Prob
                                                     )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImagesUpdateValidatedAndPredictClass");
    return;
  }

  if  (!mlClass)
    return;

  KKStr  updateStr (512);
  updateStr << "call  ImagesUpdateValidatedClass(" << imageFileName.QuotedStr ()       << ", " 
            <<                                        mlClass->Name ().QuotedStr () << ", "
            <<                                        class1Prob                                                   
            <<                                 ")";
  int32  returnCd = QueryStatement (updateStr);
  ResultSetsClear ();
}  /* ImagesUpdateValidatedAndPredictClass*/




void  DataBase::ImageUpdateInstrumentDataFields (InstrumentDataPtr  instumentData,
                                                 const KKStr&       sipperFileName,
                                                 uint32             scanLineStart,
                                                 uint32             scanLineEnd
                                                )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImageUpdateInstrumentDataFields");
    return;
  }

  KKStr  updateStr (512);

  updateStr << "call ImagesUpdateInstrumentDataFields("
    <<      sipperFileName.QuotedStr ()    << ", "
    <<      scanLineStart                  << ", "
    <<      scanLineEnd                    << ", "
    <<      instumentData->FlowRate1    () << ", "
    <<      instumentData->FlowRate2    () << ", "
    <<      instumentData->Depth        () << ", "
    <<      instumentData->Salinity     () << ", "
    <<      instumentData->Oxygen       () << ", "
    <<      instumentData->Fluorescence () 
    <<    ")";
    
  int32  returnCd = QueryStatement (updateStr);
  if  (returnCd != 0)
  {
    log.Level (-1) << endl << endl 
                   << "DataBase::ImageUpdateInstrumentDataFields    ***ERROR***     occured performing update." << endl
                   << endl
                   << "ErrorDesc[" << lastMySqlErrorDesc << "]"  << endl
                   << endl;
  }

  ResultSetsClear ();
}  /* ImageUpdateInstrumentDataFields */





//*************************************************************************************
//*                             Image Full SizeRoutines                               *
//*************************************************************************************
RasterSipperPtr  DataBase::ImageFullSizeLoad (const KKStr&  imageFileName)
{
  RasterSipperPtr  fullSizeImage = NULL;

  char*  fieldNames[] = {"ImageId", "ImageFileName", "FullSizeImage", NULL};

  KKStr  selectStr = "call ImagesFullSizeLoad(" + osGetRootName (imageFileName).QuotedStr () + ")";
  int32  returnCd = QueryStatement (selectStr);
  if (returnCd == 0)
  {
    ResultSetLoad (fieldNames);
    if  (ResultSetFetchNextRow ())
      fullSizeImage = ResultSetGetRasterField (2);
  }

  ResulSetFree ();
  ResultSetsClear ();

  return  fullSizeImage;
}  /* ImageFullSizeLoad */



void  DataBase::ImageFullSizeDelete (const KKStr&  imageFileName)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImageFullSizeDelete");
    return;
  }

  KKStr  deleteStr = "call  ImagesFullSizeDelete(" + imageFileName.QuotedStr () + ")";
  int32  returnCd = QueryStatement (deleteStr);
  if  (returnCd == 0)
  {
    ResultSetLoad (NULL);
  }

  ResulSetFree ();
  ResultSetsClear ();
}  /* ImageFullSizeDelete */




void   DataBase::ImageFullSizeSave (const KKStr&         imageFileName,
                                    const RasterSipper&  raster
                                   )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImageFullSizeSave");
    return;
  }

  uint32  compressedBuffLen = 0;
  uchar*  compressedBuff = raster.ToCompressor (compressedBuffLen);
  if  (!compressedBuff)
  {
    log.Level (-1) << endl << endl << "DataBase::ImageFullSizeSave   ***ERROR***    Compressing RasterSipper  ImageFileName[" << imageFileName << "]" << endl << endl;
    return;
  }

  char*          queryStr  = NULL;
  int32          queryStrLen = 0;
  unsigned long  mySqlImageCompressedBuffLen = 0;

  int32  queryStrSize = compressedBuffLen * 2 + 300;
  int32  queryStrLeft = queryStrSize;
  queryStr = new char[queryStrSize];
  if  (!queryStr)
  {
    log.Level (-1) << endl << endl << "DataBase::ImageFullSizeSave   ***ERROR***    ImageFileName[" << imageFileName << "]  Could not allocate 'QueryStr'.  Length[" << queryStrSize << "]" << endl << endl;
    delete  compressedBuff;
    compressedBuff = NULL;
    return;
  }

  char*  queryStrPtr = queryStr;
  STRCOPY (queryStrPtr, queryStrLeft, "call ImagesFullSizeSave(");
  uint32  x = strlen (queryStrPtr);  queryStrPtr += x;   queryStrLeft -= x;

  STRCOPY (queryStrPtr, queryStrLeft, osGetRootName (imageFileName).QuotedStr ().Str ());
  x = strlen (queryStrPtr);  queryStrPtr += x;   queryStrLeft -= x;

  STRCOPY (queryStrPtr, queryStrLeft, ", \"");
  x = strlen (queryStrPtr);  queryStrPtr += x;   queryStrLeft -= x;
  queryStrLen = (int32)strlen (queryStr); 

  mySqlImageCompressedBuffLen = mysql_real_escape_string (conn, (char*)queryStrPtr, (char*)compressedBuff, compressedBuffLen);
  queryStrLen  += mySqlImageCompressedBuffLen;
  queryStrPtr  += mySqlImageCompressedBuffLen;
  queryStrLeft -= mySqlImageCompressedBuffLen;

  // We no longer need 'compressedBuff' so lets delete it.
  delete  compressedBuff;  compressedBuff = NULL;
  
  KKU::STRCAT (queryStrPtr, queryStrLeft, "\")");
  x = strlen (queryStrPtr);
  queryStrLen  += x;
  queryStrPtr  += x;
  queryStrLeft -= x;

  int32  returnCd = QueryStatement (queryStr, queryStrLen);
  if  (returnCd == 0)
  {
    int32  imageId = -1;
    ResultSetLoad (NULL);
    if  (resultSetMore)
    {
      if  (ResultSetFetchNextRow ())
        imageId = ResultSetGetIntField (0);
      ResulSetFree ();
    }
    ResultSetsClear ();
  }

  delete  queryStr; 
  queryStr = NULL;

  delete  compressedBuff;
  compressedBuff = NULL;
}  /* ImageFullSizeSave*/






RasterSipperPtr  DataBase::ImageFullSizeFind (const KKStr&  imageFileName)
{
  DataBaseImagePtr i = this->ImageLoad (imageFileName);
  if  (!i)
  {
    // Without this entry there is no way to locate the image.
    return NULL;
  }

  RasterSipperPtr  r = NULL;
  uint32  maxDim = Max (i->Height (), i->Width ());
  if  (maxDim <= 100)
  {
    try
    {r = i->ThumbNail (log);}
    catch (KKStrException& e1)  {throw KKStrException ("DataBase::ImageFullSizeFind  Exception while calling 'ThumbNail'  " + e1.ToString ());}
    catch (std::exception& e2)  {throw KKStrException ("DataBase::ImageFullSizeFind  std::exception while calling 'ThumbNail'.", e2.what ());}
    catch (...)                 {throw KKStrException ("DataBase::ImageFullSizeFind  Exception while calling 'ThumbNail'.");}
  }

  if  (!r)
  {
    // Will now try the ImagesFullSize table
    try  {r = ImageFullSizeLoad (imageFileName);}
    catch (KKStrException& e1)  {throw KKStrException ("DataBase::ImageFullSizeFind  Exception while calling 'ImageFullSizeLoad'  " + e1.ToString ());}
    catch (std::exception& e2)  {throw KKStrException ("DataBase::ImageFullSizeFind  std::exception while calling 'ImageFullSizeLoad'.", e2.what ());}
    catch (...)                 {throw KKStrException ("DataBase::ImageFullSizeFind  Exception while calling 'ImageFullSizeLoad'.");}
  }

  if  (!r)
  {
    // Will now try to go to original sipper file.
    try  {r = i->GetOrigImageFromSipperFile (log);}
    catch (KKStrException& e1)  {throw KKStrException ("DataBase::ImageFullSizeFind  Exception while calling 'GetOrigImageFromSipperFile'  " + e1.ToString ());}
    catch (std::exception& e2)  {throw KKStrException ("DataBase::ImageFullSizeFind  std::exception while calling 'GetOrigImageFromSipperFile'.", e2.what ());}
    catch (...)                 {throw KKStrException ("DataBase::ImageFullSizeFind  Exception while calling 'GetOrigImageFromSipperFile'.");}

    if  (r != NULL)
    {
      // Sine we just went to the trouble to load construct the Full Size version of the image from the 
      // SIPPER file we should update the database so that next time we can retrieve without resorting to 
      // the SIPPER file.
      ImageFullSizeSave (imageFileName, *r);
    }
  }

  if  (r)
    r->FileName (imageFileName);

  delete  i;
  i = NULL;

  return  r;
}  /* ImageFullSizeFind */




//*************************************************************************************
//*                             Image Group Routines                                  *
//*************************************************************************************
void  DataBase::ImageGroupInsert (DataBaseImageGroup&  imageGroup)
{
  KKStr  insertStr (256);

  insertStr << "CALL ImageGroupInsert(" 
            <<                         imageGroup.Name       ().QuotedStr () << ", "
            <<                         imageGroup.Description().QuotedStr ()
            << ")";
    
  int32  returnCd = QueryStatement (insertStr.Str ());
  if  (returnCd != 0)
    return;

  ResultSetLoad (NULL);
  if  (!resultSetMore)
    return;

  if  (resultSetNumFields < 1)
  {
    lastMySqlErrorDesc = "ImageGroupId was not returnd.";
    lastMySqlErrorNo   = -1;
  }
  else
  {
    while  (ResultSetFetchNextRow ())
      imageGroup.ImageGroupId (ResultSetGetIntField (0));
  }

  ResulSetFree ();
  ResultSetsClear ();

  return;
}  /* ImageGroupInsert */




DataBaseImageGroupPtr  DataBase::ImageGroupLoad (const KKStr&  imageGroupName)   // returns a list of all Group Assignments
{
  DataBaseImageGroupPtr  group = NULL;

  KKStr  selectCmd = "call ImageGroupLoad(" + imageGroupName.QuotedStr () + ")";
  int32  returnCd = QueryStatement (selectCmd);
  if  (returnCd != 0)
    return  NULL;

  char*  fieldNames[] = {"ImageGroupId", "ImageGroupName", "Description", "GroupCount", NULL};
  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
    return  NULL;

  int32  groupId = 0;
  KKStr  name    = "";
  KKStr  desc    = "";
  int32  count   = 0;
  while  (ResultSetFetchNextRow ())
  {
    if  (group)
    {
      delete  group;  group = NULL;
    }
    groupId = ResultSetGetIntField (0);
    name    = ResultSetGetField    (1);
    desc    = ResultSetGetField    (2);
    count   = ResultSetGetIntField (3);
    group = new DataBaseImageGroup (groupId, name, desc, count);
  }

  ResulSetFree ();
  ResultSetsClear ();

  return  group;
}  /* ImageGroupLoad */






DataBaseImageGroupListPtr  DataBase::ImageGroupLoad ()   // returns a list of all Group Assignments
{
  DataBaseImageGroupListPtr  groups = NULL;

  KKStr  selectCmd = "call ImageGroupLoadAll()";
  int32  returnCd = QueryStatement (selectCmd);
  if  (returnCd != 0)
    return  groups;

  ResultSetLoad (NULL);
  if  (!resultSetMore)
    return  NULL;

  if  (resultSetNumFields < 4)
  {
    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    log.Level (-1) << endl << endl 
                   << "DataBase::ImageGroupLoad    ***ERROR***     Not enough fields returned." << endl
                   << endl
                   << "ErrorDesc[" << lastMySqlErrorDesc << "]"  << endl
                   << endl;
    return  NULL;
  }

  groups = new DataBaseImageGroupList (true);

  while  (ResultSetFetchNextRow ())
  {
    int32  groupId = ResultSetGetIntField (0);
    KKStr  name    = ResultSetGetField (1);
    KKStr  desc    = ResultSetGetField (2);
    uint32 count   = (uint32)ResultSetGetUlongField (3);
    groups->PushOnBack (new DataBaseImageGroup (groupId, name, desc, count));
  }

  ResulSetFree ();
  ResultSetsClear  ();
  return  groups;
}  /* ImageGroupLoad */




void  DataBase::ImageGroupDelete (int32 imageGroupId)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("ImageGroupDelete");
    return;
  }

  KKStr  deleteStr (1000);
  deleteStr << "CALL ImageGroupDelete(" << imageGroupId << ")";
    
  int32  returnCd = QueryStatement (deleteStr.Str ());
  if  (returnCd != 0)
    return;

  ResultSetsClear  ();

  return;
}  /* ImageGroupDelete */




//*************************************************************************************
//*                               ImageGroupEntries                                   *
//*************************************************************************************

void  DataBase::ImageGroupEntriesInsert (const DataBaseImageGroupEntry&  imageGroupEntry)
{
  KKStr  sqlStr (256);
  sqlStr << "call ImageGroupEntryInsert(" <<  imageGroupEntry.ImageGroupId () << ", " << imageGroupEntry.ImageFileName ().QuotedStr () << ")";
  int32  returnCd = QueryStatement (sqlStr.Str ());

  return;
}  /* ImageGroupEntriesInsert */





void  DataBase::ImageGroupEntriesInsert (const DataBaseImageGroupEntryList&  imageGroupList)
{
  DataBaseImageGroupEntryList::const_iterator idx;
  for  (idx = imageGroupList.begin ();   idx != imageGroupList.end ();  idx++)
  {
    DataBaseImageGroupEntryPtr ie = *idx;
    ImageGroupEntriesInsert (*ie);
  }
}  /* ImageGroupEntriesInsert */





KKStrMatrixPtr  DataBase::ImageGroupEntriesInsert (int32               groupId,
                                                   const VectorKKStr&  imageFileNames
                                                  )
{
  int32  x = 0;
  int32  maxParmLen = 60000;  // The stored procedure parameter is limited to 60,000 characters.
  int32  sqlStrLen = Min (maxParmLen, (int32)(49 * imageFileNames.size ()));  
  sqlStrLen = Max (sqlStrLen, 100);
  
  KKStr  sqlStr (sqlStrLen);

  sqlStr = "";
  sqlStr << "call  ImageGroupEntryInsertList(" << groupId << ", \"";

  uint32  zed = 0;

  while  (zed < imageFileNames.size ())
  {
    if  (zed > 0)
      sqlStr << "\t";
    sqlStr << imageFileNames[zed];
    zed++;
  }
  sqlStr << "\")";

  if  (zed < imageFileNames.size ())
  {
    log.Level (-1) << endl
                   << "DataBase::ImageGroupEntriesInsert    ***ERROR***" << endl
                   << endl
                   << "      Parameter for ImageFileNames was to large." << endl
                   << endl;
    return  NULL;
  }

  KKStrMatrixPtr results = new KKStrMatrix (2);  // 2 Columns

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
  {
    // Since we failed to process anything we will return 'NULL' indicating total failure.
    delete  results;
    return  NULL;
  }
  
  // Process results of procedure call.
  char*  fieldNames[] = {"ImageId", "ImageFileName", "Successful", "ErrorDesc", NULL};
  ResultSetLoad (fieldNames);
  while  (ResultSetFetchNextRow ())
  {
    bool  successful    = ResultSetGetBool (2);
    if  (!successful)
    {
      KKStrPtr  imageFileName = new KKStr (ResultSetGetKKStrField (1));
      KKStrPtr  errorDesc     = new KKStr (ResultSetGetKKStrField (3));
      KKStrListPtr  row = new KKStrList (true, 2);
      row->push_back (imageFileName);
      row->push_back (errorDesc);
      results->AddRow (row);
    }
  }

  ResulSetFree ();
  ResultSetsClear  ();

  return  results;
}  /* ImageGroupEntriesInsert */





DataBaseImageGroupEntryListPtr  DataBase::ImageGroupEntriesLoad (int32 groupId)
{
  KKStr  selectCmd (200);

  selectCmd << "select  ImageFileName  from ImageGroupEntries  where  ImageGroupId = " << groupId << endl;

  int32  returnCd = QueryStatement (selectCmd);
  if  (returnCd != 0)
    return  NULL;

  MYSQL_RES*  resSet;
  resSet = mysql_use_result (conn); /* generate result set */
  if  (resSet == NULL)
  {
    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    log.Level (-1) << endl << endl 
                   << "DataBase::ImageGroupEntriesLoad    ***ERROR***     occured when retrieving result set." << endl
                   << endl
                   << "ErrorDesc[" << lastMySqlErrorDesc << "]"  << endl
                   << endl;
    return  NULL;
  }

  DataBaseImageGroupEntryListPtr  entries = new DataBaseImageGroupEntryList (true);
  int32  numFields  = mysql_num_fields (resSet);
  if  (numFields < 1)
  {
    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    log.Level (-1) << endl << endl 
                   << "DataBase::ImageGroupEntriesLoad    ***ERROR***     Not enough cllumns ." << endl
                   << endl
                   << "ErrorDesc[" << lastMySqlErrorDesc << "]"  << endl
                   << endl;
    return  NULL;
  }

  MYSQL_ROW  row;
  while  (row = mysql_fetch_row (resSet))
  {
    KKStr  imageFileName = row[0];
    DataBaseImageGroupEntryPtr  entry = new DataBaseImageGroupEntry (groupId, imageFileName);
    entries->PushOnBack (entry);
  }

  mysql_free_result (resSet);

  return  entries;
}  /* ImageGroupEntriesLoad */






//*************************************************************************************
//*                           Sipper Deloyment Routines                               *
//*************************************************************************************





SipperDeploymentListPtr  DataBase::SipperDeploymentProcessResults ()
{
  char*  fieldNames[] = {"CruiseName",        "StationName",       "DeploymentNum",
                         "Description",       "DateTimeStart",     "DateTimeEnd",
                         "Latitude",          "Longitude",         "SyncTimeStampActual",
                         "SyncTimeStampCTD",  "SyncTimeStampGPS",  "CropLeft",
                         "CropRight",
                         NULL
                        };
  
  if  (!ResultSetLoad (fieldNames))
    return NULL;

  SipperDeploymentListPtr  results = new SipperDeploymentList (true);

  while  (ResultSetFetchNextRow ())
  {
    SipperDeploymentPtr  d = new SipperDeployment ();
    d->CruiseName          (ResultSetGetField (0));
    d->StationName         (ResultSetGetField (1));
    d->DeploymentNum       (ResultSetGetField (2));
    d->Description         (ResultSetGetField (3));
    d->DateTimeStart       (ResultSetGetDateTimeField  (4));
    d->DateTimeEnd         (ResultSetGetDateTimeField  (5));
    d->Latitude            (ResultSetGetDoubleField    (6));
    d->Longitude           (ResultSetGetDoubleField    (7));
    d->SyncTimeStampActual (ResultSetGetDateTimeField  (8));
    d->SyncTimeStampCTD    (ResultSetGetDateTimeField  (9));
    d->SyncTimeStampGPS    (ResultSetGetDateTimeField (10));
    d->CropLeft            (ResultSetGetUintField     (11));
    d->CropRight           (ResultSetGetUintField     (12));

    if  ((d->SyncTimeStampActual ().Year () < 1)  &&
         (d->SyncTimeStampCTD    ().Year () < 1)  &&
         (d->SyncTimeStampGPS    ().Year () < 1)
        )
    {
      d->SyncTimeStampActual (d->DateTimeStart ());
      d->SyncTimeStampCTD    (d->DateTimeStart ());
      d->SyncTimeStampGPS    (d->DateTimeStart ());
    }

    results->PushOnBack (d);
  }

  ResulSetFree ();
  ResultSetsClear ();

  return  results;
}  /* SipperDeploymentProcessResults */





SipperDeploymentListPtr  DataBase::SipperDeploymentLoad (const KKStr& cruiseName,
                                                         const KKStr& stationName
                                                        )
{
  KKStr  sqlStr = "call DeploymentLoadList(" + cruiseName.QuotedStr () + ", " + stationName.QuotedStr () + ")";

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return  NULL;

  return  SipperDeploymentProcessResults ();
}  /* SipperDeploymentLoad */




SipperDeploymentPtr    DataBase::SipperDeploymentLoad (const KKStr&  cruiseName,
                                                       const KKStr&  stationName,
                                                       const KKStr&  deploymentNum
                                                      )
{
  KKStr  sqlStr = "call DeploymentLoad(" + cruiseName.QuotedStr () + ", " + stationName.QuotedStr () + ", " + deploymentNum.QuotedStr () + ")";
  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return  NULL;

  SipperDeploymentListPtr  results = SipperDeploymentProcessResults ();
  if  (results == NULL)
    return NULL;


  SipperDeploymentPtr  result = results->PopFromBack ();
  delete  results;
  results = NULL;
  return result;
}  /* SipperDeploymentLoad*/




void   DataBase::SipperDeploymentInsert (const SipperDeployment&  deployment)
{
  KKStr  sqlStr = "call DeploymentInsert(" + 
                        deployment.CruiseName    ().QuotedStr ()                + ", " +
                        deployment.StationName   ().QuotedStr ()                + ", " +
                        deployment.DeploymentNum ().QuotedStr ()                + ", " +
                        deployment.Description   ().QuotedStr ()                + ", " +
                        DateTimeToQuotedStr (deployment.DateTimeStart ())       + ", " + 
                        DateTimeToQuotedStr (deployment.DateTimeEnd   ())       + ", " + 
                        StrFormatDouble (deployment.Latitude  (), "##0.000000") + ", " +
                        StrFormatDouble (deployment.Longitude (), "##0.000000") + ", " +
                        DateTimeToQuotedStr (deployment.SyncTimeStampActual ()) + ", " +
                        DateTimeToQuotedStr (deployment.SyncTimeStampCTD    ()) + ", " +
                        DateTimeToQuotedStr (deployment.SyncTimeStampGPS    ()) + ", " +
                        StrFormatInt (deployment.CropLeft  (), "####0")         + ", " +
                        StrFormatInt (deployment.CropRight (), "####0")         +
                        ")";
  int32  returnCd = QueryStatement (sqlStr);
  ResultSetsClear ();
}  /* SipperDeploymentInsert */



void   DataBase::SipperDeploymentUpdate (const SipperDeployment&  deployment)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperDeploymentUpdate");
    return;
  }

  KKStr  sqlStr = "call DeploymentUpdate(" + 
                        deployment.CruiseName    ().QuotedStr ()                + ", " +
                        deployment.StationName   ().QuotedStr ()                + ", " +
                        deployment.DeploymentNum ().QuotedStr ()                + ", " +
                        deployment.Description   ().QuotedStr ()                + ", " +
                        DateTimeToQuotedStr (deployment.DateTimeStart ())       + ", " + 
                        DateTimeToQuotedStr (deployment.DateTimeEnd   ())       + ", " + 
                        StrFormatDouble (deployment.Latitude  (), "##0.000000") + ", " +
                        StrFormatDouble (deployment.Longitude (), "##0.000000") + ", " +
                        DateTimeToQuotedStr (deployment.SyncTimeStampActual ()) + ", " +
                        DateTimeToQuotedStr (deployment.SyncTimeStampCTD    ()) + ", " +
                        DateTimeToQuotedStr (deployment.SyncTimeStampGPS    ()) + ", " +
                        StrFormatInt (deployment.CropLeft  (), "####0")         + ", " +
                        StrFormatInt (deployment.CropRight (), "####0")         +
                       ")";
  int32  returnCd = QueryStatement (sqlStr);
  ResultSetsClear ();
}  /* SipperDeploymentUpdate */




void   DataBase::SipperDeploymentDelete (const KKStr&  cruiseName,
                                         const KKStr&  stationName,
                                         const KKStr&  deploymentNum
                                        )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperDeploymentDelete");
    return;
  }

  KKStr  sqlStr = "call DeploymentDelete(" + 
                        cruiseName.QuotedStr    ()  + ", " +
                        stationName.QuotedStr   ()  + ", " +
                        deploymentNum.QuotedStr ()  +
                       ")";
  int32  returnCd = QueryStatement (sqlStr);
  ResultSetsClear ();
}  /* SipperDeploymentDelete */







//*******************************************************************************************
//*                                SipperFile Routines                                      *
//*******************************************************************************************


char*  DataBase::sipperFileFieldNames[] = 
                       {"SipperFileId",
                        "SipperFileName", "CruiseName",     "StationName",    "DeploymentNum",
                        "Description",    "Latitude",       "Longitude",      "DateTimeStart",
                        "SP0Instrument",  "SP1Instrument",  "SP2Instrument",  "SP3Instrument",
                        "CtdExt0",        "CtdExt1",        "CtdExt2",        "CtdExt3",
                        "SizeInBytes",    "NumScanLines",   "ScanRate",       "Depth",
                        "ExtractionStatus",        // 21
                        "ExtractionScanLineStart", // 22
                        "ExtractionScanLineEnd",   // 23
                        NULL
                       };


SipperFileListPtr  DataBase::SipperFileProcessResults ()
{
  ResultSetLoad (sipperFileFieldNames);
  if  (!resultSetMore)
    return NULL;

  SipperFileListPtr  results = new SipperFileList (true);
  while (ResultSetFetchNextRow ())
  {
    SipperFilePtr  sf = new SipperFile (ResultSetGetField (1));
    sf->SipperFileId            (ResultSetGetIntField (0));
    sf->CruiseName              (ResultSetGetField (2));
    sf->StationName             (ResultSetGetField (3));
    sf->DeploymentNum           (ResultSetGetField (4));
    sf->Description             (ResultSetGetField (5));
    sf->Latitude                (ResultSetGetDoubleField (6));
    sf->Longitude               (ResultSetGetDoubleField (7));
    sf->DateTimeStart           (DateTimeFromMySqlDateTimeField (ResultSetGetField (8))); 
    sf->Sp0                     (Instrument::LookUpByShortName  (ResultSetGetField (9))); 
    sf->Sp1                     (Instrument::LookUpByShortName  (ResultSetGetField (10))); 
    sf->Sp2                     (Instrument::LookUpByShortName  (ResultSetGetField (11)));
    sf->Sp3                     (Instrument::LookUpByShortName  (ResultSetGetField (12)));
    sf->CtdExt0                 (ResultSetGetField       (13));
    sf->CtdExt1                 (ResultSetGetField       (14));
    sf->CtdExt2                 (ResultSetGetField       (15));
    sf->CtdExt3                 (ResultSetGetField       (16));
    sf->SizeInBytes             (ResultSetGetInt64Field  (17));
    sf->NumScanLines            (ResultSetGetIntField    (18));
    sf->ScanRate                (ResultSetGetFloatField  (19));
    sf->Depth                   (ResultSetGetFloatField  (20));
    sf->ExtractionStatus        (ResultSetGetCharField   (21));
    sf->ExtractionScanLineStart (ResultSetGetUintField   (22));
    sf->ExtractionScanLineEnd   (ResultSetGetUintField   (23));
    
    results->PushOnBack (sf);
  }

  ResulSetFree ();
  return  results;
}  /* SipperFileProcessResults */



void  DataBase::SipperFileLoadOracle ()
{
  SipperFileListPtr  sipperFiles = SipperFileLoad ("", "", "");
  SipperFileOracle::AddSipperFileEntries (sipperFiles);
  sipperFiles->Owner (false);
  delete  sipperFiles;
}



SipperFilePtr  DataBase::SipperFileRecLoad (const KKStr&  _sipperFileName)
{
  KKStr sipperFileName = osGetRootName (_sipperFileName);

  KKStr  queryStr = "select *  from  SipperFiles  " 
                    "where SipperFileName = " + sipperFileName.QuotedStr ();

  int32  returnCd = QueryStatement (queryStr);
  if  (returnCd != 0)
    return  NULL;

  SipperFileListPtr  results = SipperFileProcessResults ();
  if  (results == NULL)
    return NULL;

  SipperFilePtr  result = results->PopFromBack ();
  delete  results;
  results = NULL;
  ResultSetsClear ();
  return  result;
}  /* SiperFileRecLoad */



void  DataBase::SipperFilesUpdateExtractionStatus (const KKStr&  sipperFileName,
                                                   char          extractionStatus
                                                  )
{
  KKStr sqlStr (128);

  KKStr rootName = osGetRootName (sipperFileName);

  sqlStr << "call SipperFilesUpdateExtractionStatus(" << rootName.QuotedStr () << ", \"" << extractionStatus << "\")";
  int32  returnCd = QueryStatement (sqlStr);

}  /* SipperFilesUpdateExtractionStatus */





VectorKKStr*  DataBase::SipperFileGetList (const KKStr& cruiseName,
                                           const KKStr& stationName,
                                           const KKStr& deploymentNum
                                          )
{
  KKStr  selectCmd (4096);

  selectCmd << "select  sf.SipperFileName  from  SipperFiles  sf ";

  VectorKKStr  conditions;

  if  (!cruiseName.Empty ())
    conditions.push_back ("CruiseName = " + cruiseName.QuotedStr ());

  if  (!stationName.Empty ())
    conditions.push_back ("StationName = " + stationName.QuotedStr ());

  if  (!deploymentNum.Empty ())
    conditions.push_back ("DeploymentNum = " + deploymentNum.QuotedStr ());

  if  (conditions.size () > 0)
  {
    selectCmd << "  where  ";
    for  (uint32 x = 0;  x < conditions.size ();  x++)
    {
      if  (x > 0)
        selectCmd << "  and  ";
      selectCmd << conditions[x];
    }
  }

  selectCmd << "    order by CruiseName, StationName, DeploymentNum, DateTimeStart";

  int32  returnCd = QueryStatement (selectCmd);
  if  (returnCd != 0)
    return  NULL;


  MYSQL_RES*  resSet;
  resSet = mysql_use_result (conn);
  if  (resSet == NULL)
  {
    lastMySqlErrorDesc = mysql_error (conn);
    lastMySqlErrorNo   = mysql_errno (conn);

    log.Level (-1) << endl << endl 
                   << "DataBase::SipperFileGetList    ***ERROR***     occured when retrieving result set." << endl
                   << endl
                   << "ErrorDesc[" << lastMySqlErrorDesc << "]"  << endl
                   << endl;
    return  NULL;
  }


  VectorKKStr*  sipperFiles = new VectorKKStr ();

  MYSQL_ROW  row;
  while  (row = mysql_fetch_row (resSet))
  {
    KKStr  sipperFileName (row[0]);
    sipperFiles->push_back (sipperFileName);
  }

  mysql_free_result (resSet);

  return  sipperFiles;
}  /* SipperFileGetList */





SipperFileListPtr  DataBase::SipperFileLoad (const KKStr& cruiseName,
                                             const KKStr& stationName,
                                             const KKStr& deploymentNum
                                            )
{
  KKStr  selectCmd (200);

  selectCmd << "select * from  SipperFiles  "  << endl;

  if  (!cruiseName.Empty ())
  {
    selectCmd << "  where  CruiseName = " << cruiseName.QuotedStr ();

    if  (!stationName.Empty ())
    {
      selectCmd  << "  and  StationName = " << stationName.QuotedStr ();

      if  (!deploymentNum.Empty ())
      {
        selectCmd  << "  and  DeploymentNum = " << deploymentNum.QuotedStr ();
      }
    }
  }

  selectCmd << "  order by CruiseName, StationName, DeploymentNum, DateTimeStart";

  int32  returnCd = QueryStatement (selectCmd);
  if  (returnCd != 0)
    return  NULL;

  return  SipperFileProcessResults ();
}  /* SipperFileLoad */






void  DataBase::SipperFileInsert (SipperFile&  sipperFile)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperFileInsert");
    return;
  }

  KKStr  insertStr (1000);
  KKStr  extractionStatusStr;
  extractionStatusStr.Append (sipperFile.ExtractionStatus ());

  insertStr <<  "insert into  SipperFiles (SipperFileName, CruiseName, StationName, DeploymentNum, Description, Latitude, "      
                                           "Longitude, DateTimeStart, SP0Instrument, SP1Instrument, SP2Instrument, SP3Instrument, "
                                           "CtdExt0, CtdExt1, CtdExt2, CtdExt3, SizeInBytes, NumScanLines, Scanrate, Depth, "
                                           "ExtractionStatus, ExtractionScanLineStart, ExtractionScanLineEnd"
                                         ")" << endl
            <<      "values(" 
            <<         sipperFile.SipperFileName ().QuotedStr ()          << ", "
            <<         sipperFile.CruiseName     ().QuotedStr ()          << ", "
            <<         sipperFile.StationName    ().QuotedStr ()          << ", "
            <<         sipperFile.DeploymentNum  ().QuotedStr ()          << ", "
            <<         sipperFile.Description    ().QuotedStr ()          << ", "
            <<         sipperFile.Latitude       ()                       << ", "
            <<         sipperFile.Longitude      ()                       << ", "
            <<         DateTimeToQuotedStr (sipperFile.DateTimeStart  ()) << ", "
  
            <<         sipperFile.Sp0ShortName   ().QuotedStr ()          << ", "
            <<         sipperFile.Sp1ShortName   ().QuotedStr ()          << ", "
            <<         sipperFile.Sp2ShortName   ().QuotedStr ()          << ", "
            <<         sipperFile.Sp3ShortName   ().QuotedStr ()          << ", "

            <<         sipperFile.CtdExt0        ().QuotedStr ()          << ", "
            <<         sipperFile.CtdExt1        ().QuotedStr ()          << ", "
            <<         sipperFile.CtdExt2        ().QuotedStr ()          << ", "
            <<         sipperFile.CtdExt3        ().QuotedStr ()          << ", "
   
            <<         sipperFile.SizeInBytes    ()                       << ", "
            <<         sipperFile.NumScanLines   ()                       << ", "
            <<         sipperFile.ScanRate       ()                       << ", "
            <<         sipperFile.Depth          ()                       << ", "

            <<         extractionStatusStr.QuotedStr ()                   << ", "
            <<         sipperFile.ExtractionScanLineStart ()              << ", "
            <<         sipperFile.ExtractionScanLineEnd   ()
            <<       ")";

  int32  returnCd = QueryStatement (insertStr.Str ());
  if  (returnCd == 0)
     sipperFile.SipperFileId ((int32)mysql_insert_id (conn));

  return;
}  /* SiperFileInsert */



void   DataBase::SipperFileUpdate (SipperFile&   sipperFile)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperFileUpdate");
    return;
  }

  KKStr  updateStr(1024);

  KKStr  extractionStatusStr;
  extractionStatusStr.Append (sipperFile.ExtractionStatus ());

  updateStr << "update  SipperFiles sf"  << endl
            << "  set  CruiseName = "     << sipperFile.CruiseName    ().QuotedStr ()          << ", " << endl
            << "       StationName = "    << sipperFile.StationName   ().QuotedStr ()          << ", " << endl
            << "       DeploymentNum = "  << sipperFile.DeploymentNum ().QuotedStr ()          << ", " << endl
            << "       Description = "    << sipperFile.Description   ().QuotedStr ()          << ", " << endl
            << "       Latitude = "       << sipperFile.Latitude      ()                       << ", " << endl
            << "       Longitude = "      << sipperFile.Longitude     ()                       << ", " << endl
            << "       DateTimeStart = "  << DateTimeToQuotedStr (sipperFile.DateTimeStart ()) << ", " << endl
            << "       SP0Instrument = "  << sipperFile.Sp0ShortName  ().QuotedStr ()          << ", " << endl
            << "       SP1Instrument = "  << sipperFile.Sp1ShortName  ().QuotedStr ()          << ", " << endl
            << "       SP2Instrument = "  << sipperFile.Sp2ShortName  ().QuotedStr ()          << ", " << endl
            << "       SP3Instrument = "  << sipperFile.Sp3ShortName  ().QuotedStr ()          << ", " << endl
            << "       CtdExt0 = "        << sipperFile.CtdExt0       ().QuotedStr ()          << ", " << endl
            << "       CtdExt1 = "        << sipperFile.CtdExt1       ().QuotedStr ()          << ", " << endl
            << "       CtdExt2 = "        << sipperFile.CtdExt2       ().QuotedStr ()          << ", " << endl
            << "       CtdExt3 = "        << sipperFile.CtdExt3       ().QuotedStr ()          << ", " << endl
            << "       SizeInBytes = "    << sipperFile.SizeInBytes   ()                       << ", " << endl
            << "       NumScanLines = "   << sipperFile.NumScanLines  ()                       << ", " << endl
            << "       ScanRate = "       << sipperFile.ScanRate      ()                       << ", " << endl
            << "       Depth = "          << sipperFile.Depth         ()                       << ", " << endl
            << "       ExtractionStatus = "        << extractionStatusStr.QuotedStr ()         << ", " << endl
            << "       ExtractionScanLineStart = " << sipperFile.ExtractionScanLineStart ()    << ", " << endl
            << "       ExtractionScanLineEnd   = " << sipperFile.ExtractionScanLineEnd   ()
            << "  where  sf.SipperFileName = " << sipperFile.SipperFileName ().QuotedStr () << endl;

  int32  returnCd = QueryStatement (updateStr.Str ());
}  /* SipperFileUpdate */





void   DataBase::SipperFileUpdateFileSizeStats (const KKStr&  _sipperFileName, 
                                                int64         _sizeInBytes,
                                                uint32        _numScanLines
                                               )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperFileUpdateFileSizeStats");
    return;
  }

  KKStr  updateStr (512);
  
  updateStr << "update  SipperFiles  set SizeInBytes = " << _sizeInBytes << ", NumScanLines = " << _numScanLines << endl 
            << "        where SipperFileName = " << _sipperFileName.QuotedStr ();

  int32  returnCd = QueryStatement (updateStr.Str ());
}  /* SipperFileUpdateFileSizeStats */





void  DataBase::SipperFilesGetCTDDateTime (const KKStr&  _sipperFileName,
                                           int32&        _sipperFileId, 
                                           uint32&       _numScanLines, 
                                           DateTime&     _CTDDateTimeStart, 
                                           DateTime&     _CTDDateTimeEnd
                                          )
{
  _sipperFileId = -1;

  char*  fieldNames[] =
  {"sipperFileId", "sipperFileName", "numScanLines", "CTDDateTimeStart", "CTDDateTimeEnd", NULL};

  KKStr  sqlStr (256);
  sqlStr << "call  SipperFilesGetCTDDateTime("  << _sipperFileName.QuotedStr () << ")";
  int32  returnCd = QueryStatement (sqlStr.Str ());

  if  (returnCd == 0)
  {
    ResultSetLoad (fieldNames);
    if  (ResultSetFetchNextRow ())   
    {
      _sipperFileId     = ResultSetGetIntField (0);
      _numScanLines     = ResultSetGetIntField (2);
      _CTDDateTimeStart = DateTimeFromMySqlDateTimeField (ResultSetGetField (3));
      _CTDDateTimeEnd   = DateTimeFromMySqlDateTimeField (ResultSetGetField (4));
    }
    ResulSetFree ();
    ResultSetsClear ();
  }

  return;
}  /* SipperFilesGetCTDDateTime */




/**
 *@brief  Delete the contents of a single SIPPER file from the database from all tables such as Images, FeatureData, InstrumentData, etc.
 *@param[in]  _sipperFileName  Name of SIPPER file to delete.
 */
void   DataBase::SipperFilesDelete (const KKStr&  _sipperFileName)
{
  KKStr  updateStr (512);
  updateStr << "call  SipperFilesDelete (" << _sipperFileName.QuotedStr () << ")";
  int32  returnCd = QueryStatement (updateStr.Str ());
}



//*******************************************************************************************
//*                              SipperStation  Rotines                                     *
//*******************************************************************************************
SipperStationListPtr  DataBase::SipperStationProcessResults ()
{
  char*  fieldNames[] = {"CruiseName", "StationName", "Description", "Latitude", "Longitude", "DateTimeStart", NULL};
  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
    return NULL;

  SipperStationListPtr  results = new SipperStationList (true);
  while  (ResultSetFetchNextRow ())
  {
    SipperStationPtr s = new SipperStation ();
    s->CruiseName    (ResultSetGetField         (0));
    s->StationName   (ResultSetGetField         (1));
    s->Description   (ResultSetGetField         (2));
    s->Latitude      (ResultSetGetDoubleField   (3));
    s->Longitude     (ResultSetGetDoubleField   (4));
    s->DateTimeStart (ResultSetGetDateTimeField (5));

    results->PushOnBack (s);
  }

  ResulSetFree ();
  ResultSetsClear ();

  return  results;
}  /* SipperStationProcessResults */


 


SipperStationListPtr  DataBase::SipperStationLoad (const KKStr&  cruiseName)
{
  KKStr  selectCmd (200);

  selectCmd << "select   * from Stations" << endl;
  if  (!cruiseName.Empty ())
    selectCmd << "     where CruiseName = " << cruiseName.QuotedStr () << endl;

  int32  returnCd = QueryStatement (selectCmd);
  if  (returnCd != 0)
    return  NULL;

  return  SipperStationProcessResults ();
}  /* SipperStationLoad */





SipperStationPtr  DataBase::SipperStationLoad (const KKStr&  cruiseName,
                                               const KKStr&  stationName
                                              )
{
  KKStr  sqlStr (256);

  sqlStr << "select * from Stations s" << endl
         << "      where s.CruiseName  = " << cruiseName.QuotedStr  () << "  and  " << endl
         << "            s.StationName = " << stationName.QuotedStr () << endl;

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return  NULL;

  SipperStationListPtr  results = SipperStationProcessResults ();
  if  (results == NULL)
    return NULL;

  SipperStationPtr  sipperStation = NULL;

  if  (results->QueueSize () > 0)
    sipperStation = results->PopFromBack ();

  delete  results;
 
  return  sipperStation;
}  /* SipperStationLoad */




void   DataBase::SipperStationInsert (const SipperStation&  station)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperStationInsert");
    return;
  }

  KKStr  sqlStr (256);
  sqlStr << "call StationsInsert(" + 
                  station.CruiseName    ().QuotedStr ()  << ", " <<
                  station.StationName   ().QuotedStr ()  << ", " <<
                  station.Description   ().QuotedStr ()  << ", " <<
                  station.Latitude      ()               << ", " <<
                  station.Longitude     ()               << ", " <<
                  DateTimeToQuotedStr (station.DateTimeStart ()) << 
                ")";

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd == 0)
    ResultSetsClear ();
}  /* SipperStationInsert */




void  DataBase::SipperStationUpdate (const SipperStation&  station)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperStationUpdate");
    return;
  }

  KKStr sqlStr(255);
  sqlStr << "call StationsUpdate(" + 
                  station.CruiseName    ().QuotedStr ()  << ", " <<
                  station.StationName   ().QuotedStr ()  << ", " <<
                  station.Description   ().QuotedStr ()  << ", " <<
                  station.Latitude      ()               << ", " <<
                  station.Longitude     ()               << ", " <<
                  DateTimeToQuotedStr (station.DateTimeStart ()) << 
                ")";
  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd == 0)
    ResultSetsClear ();
}  /* SipperStationUpdate */



void  DataBase::SipperStationDelete (const KKStr&  cruiseName,
                                     const KKStr&  stationName
                                   )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperStationDelete");
    return;
  }

  KKStr sqlStr = "call StationsDelete(" + cruiseName.QuotedStr  () + ", " + stationName.QuotedStr () + ")";
  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd == 0)
    ResultSetsClear ();
}




InstrumentDataListPtr  DataBase::InstrumentDataProcessResults (const bool&  cancelFlag)

{
  char*  instrumentDataFieldNames[] = {"SipperFileId",        // 0
                                       "SipperFileName",      // 1
                                       "ScanLine",            // 2
                                       "ByteOffset",          // 3
                                       "FlowRate1",           // 4
                                       "FlowRate2",           // 5
                                       "Latitde",             // 6
                                       "Longitude",           // 7
                                       "CTDBattery",          // 8
                                       "Conductivity",        // 9
                                       "Density",             // 10
                                       "Depth",               // 11
                                       "CTDDateTime",         // 12
                                       "Fluorescence",        // 13
                                       "FluorescenceSensor",  // 14
                                       "Oxygen",              // 15
                                       "OxygenSensor",        // 16
                                       "Pressure",            // 17
                                       "RecordRate",          // 18
                                       "Salinity",            // 19
                                       "SoundVelocity",       // 20
                                       "Temperature",         // 21
                                       "Transmisivity",       // 22
                                       "TransmisivitySensor", // 23
                                       "Turbidity",           // 24
                                       "TurbiditySensor",     // 25
                                       "Pitch",               // 26
                                       "Roll",                // 27
                                       NULL,
                                      };

  ResultSetLoad (instrumentDataFieldNames);
  if  (!resultSetMore)
    return NULL;

  InstrumentDataListPtr  results = new InstrumentDataList (true);

  while  (ResultSetFetchNextRow ()  &&  (!cancelFlag))
  {
    InstrumentDataPtr  id = new InstrumentData ();
    id->ScanLine            (ResultSetGetUintField     ( 2));
    id->ByteOffset          (ResultSetGetUint64Field   ( 3));
    id->FlowRate1           (ResultSetGetFloatField    ( 4));
    id->FlowRate2           (ResultSetGetFloatField    ( 5));
    id->Latitude            (ResultSetGetDoubleField   ( 6));
    id->Longitude           (ResultSetGetDoubleField   ( 7));
    id->CTDBattery          (ResultSetGetFloatField    ( 8));
    id->Conductivity        (ResultSetGetFloatField    ( 9));
    id->Density             (ResultSetGetFloatField    (10));
    id->Depth               (ResultSetGetFloatField    (11));
    id->CtdDate             (ResultSetGetDateTimeField (12));
    id->Fluorescence        (ResultSetGetFloatField    (13));
    id->FluorescenceSensor  (ResultSetGetFloatField    (14));
    id->Oxygen              (ResultSetGetFloatField    (15));
    id->OxygenSensor        (ResultSetGetFloatField    (16));
    id->Pressure            (ResultSetGetFloatField    (17));
    id->RecordRate          (ResultSetGetFloatField    (18));
    id->Salinity            (ResultSetGetFloatField    (19));
    id->SoundVelocity       (ResultSetGetFloatField    (20));
    id->Temperature         (ResultSetGetFloatField    (21));
    id->Transmisivity       (ResultSetGetFloatField    (22));
    id->TransmisivitySensor (ResultSetGetFloatField    (23));
    id->Turbidity           (ResultSetGetFloatField    (24));
    id->TurbiditySensor     (ResultSetGetFloatField    (25));
    id->Pitch               (ResultSetGetFloatField    (26));
    id->Roll                (ResultSetGetFloatField    (27));

    results->PushOnBack (id);
  }

  ResulSetFree ();

  return  results;
}  /* InstrumentDataProcessResults */





InstrumentDataPtr  DataBase::InstrumentDataGetByScanLine (const KKStr&  sipperFileName,
                                                          uint32        scanLine
                                                         )
{
  KKStr   slqStr = "call  InstrumentDataGetByScanLine(" + sipperFileName.QuotedStr () + ", " + StrFormatInt (scanLine, "########0") + ")";

  int32  returnCd = QueryStatement (slqStr);
  if  (returnCd != 0)
    return  NULL;

  bool  cancelFlag = false;
  InstrumentDataListPtr  results = InstrumentDataProcessResults (cancelFlag);
  if  (results == NULL)
    return NULL;

  ResultSetsClear ();

  InstrumentDataPtr result = results->PopFromBack ();
  delete  results;
  results = NULL;

  return  result;
}  /* InstrumentDataGetByScanLine */





void  DataBase::InstrumentDataInsert (const KKStr&            _sipperFileName,
                                      const InstrumentData&   id
                                     )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("InstrumentDataInsert");
    return;
  }

  KKStr  sipperFileName = osGetRootName (_sipperFileName);

  KKStr  insertStr (2500);
  insertStr << "call  InstrumentDataInsert2("
            << sipperFileName.QuotedStr  ()            << ", "
            << id.ScanLine               ()            << ", "
            << id.ByteOffset             ()            << ", "
            << DateTimeToQuotedStr (id.CtdDate ())     << ", "
            << id.Latitude               ()            << ", "
            << id.Longitude              ()            << ", "
            << id.CTDBattery             ()            << ", "
            << id.Conductivity           ()            << ", "
            << id.Density                ()            << ", "
            << id.Depth                  ()            << ", "
            << id.FlowRate1              ()            << ", "
            << id.FlowRate2              ()            << ", "
            << id.Fluorescence           ()            << ", "
            << id.FluorescenceSensor     ()            << ", "
            << id.CdomFluorescence       ()            << ", "
            << id.CdomFluorescenceSensor ()            << ", "
            << id.Oxygen                 ()            << ", "
            << id.OxygenSensor           ()            << ", "
            << id.Pressure               ()            << ", "
            << id.RecordRate             ()            << ", "
            << id.Salinity               ()            << ", "
            << id.SoundVelocity          ()            << ", "
            << id.Temperature            ()            << ", "
            << id.Transmisivity          ()            << ", "
            << id.TransmisivitySensor    ()            << ", "
            << id.Turbidity              ()            << ", "
            << id.TurbiditySensor        ()            << ", "
            << id.Pitch                  ()            << ", "
            << id.Roll                   ()
            << ")";

  int32  returnCd = QueryStatement (insertStr);

  return;
}  /* InstrumentDataInsert */





void  DataBase::InstrumentDataSaveListForOneSipperFile (const KKStr&               _sipperFileName,
                                                        const InstrumentDataList&  instrumentData,
                                                        const bool&                _cancelFlag
                                                       )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("InstrumentDataSaveListForOneSipperFile");
    return;
  }

  KKStr  sipperFileName = osGetRootName (_sipperFileName);

  // Lets first make sure thata record exists for this SipperFile

  SipperFilePtr  sf = SipperFileRecLoad (sipperFileName);
  if  (sf == NULL)
  {
    sf = new SipperFile (sipperFileName);
    sf->AssignCtdExternalInstruments (osGetRootName (sipperFileName));
    SipperFileInsert (*sf);
  }


  KKStr  deleteStr = "call  InstrumentDataDeleteOneSipperFile(" + sipperFileName.QuotedStr () + ")";
  int32  returnCd = QueryStatement (deleteStr);
  ResultSetsClear ();

  InstrumentDataList::const_iterator  idx;
  for  (idx = instrumentData.begin ();  (idx != instrumentData.end ())  &&  (!_cancelFlag);  idx++)
    InstrumentDataInsert (sipperFileName, **idx);

  delete  sf;
  sf = NULL;
}  /* InstrumentDataSaveListForOneSipperFile */





void  DataBase::InstrumentDataLoadAllSipperFiles (const bool&  cancelFlag)
{
  VectorKKStr  allSipperFiles = InstrumentDataFileManager::GetListOfSipperFiles (log);

  VectorKKStr::iterator  idx;
  for  (idx = allSipperFiles.begin ();  (idx != allSipperFiles.end ())  &&  (!cancelFlag);   idx++)
  {
    KKStr  sipperFileRootName = osGetRootName (*idx);

    cout << sipperFileRootName << std::endl;

    SipperFilePtr  sipperRec = SipperFileRecLoad (sipperFileRootName);

    if  (sipperRec)
    {
      if  (sipperFileRootName.Len () <= 14)
      {
        InstrumentDataListPtr   oneSipperFile = InstrumentDataFileManager::ReExtractInstrumentDataForSipperFile (*idx, sipperRec, cancelFlag, log);
        if  ((oneSipperFile != NULL)  &&  (!cancelFlag))
          InstrumentDataSaveListForOneSipperFile (*idx, *oneSipperFile, cancelFlag);
        delete  oneSipperFile;
        oneSipperFile = NULL;
      }
    }
  }
}  /* InstrumentDataLoadAllSipperFiles */



VectorKKStr  DataBase::InstrumentDataGetFieldNames ()
{
  VectorKKStr  results;
  KKStr  selectStr = "select Column_Name, Data_Type  from information_schema.Columns  c where c.Table_Schema= \"pices\"  and c.Table_Name = \"InstrumentData\"";
  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return  results;

  MYSQL_RES*  resSet;
  resSet = mysql_store_result (conn); /* generate result set */
  if  (resSet == NULL)
    return  results;


  MYSQL_ROW  row = NULL;
  while  (row = mysql_fetch_row (resSet))
  {
    KKStr fieldName   = row[0];
    results.push_back (fieldName);
  }


  mysql_free_result (resSet);
  resSet = NULL;

  return  results;
}   /* InstrumentDataGetFieldNames */







InstrumentDataListPtr  DataBase::InstrumentDataLoad (const KKStr&  sipperFileName,
                                                     const bool&   cancelFlag
                                                    )
{
  KKStr   sqlStr = "call  InstrumentDataLoadOneSipperFile(" + sipperFileName.QuotedStr () + ")";

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return  NULL;

  InstrumentDataListPtr  results = InstrumentDataProcessResults (cancelFlag);
  if  (results == NULL)
    return NULL;

  ResultSetsClear ();
  if  (!cancelFlag)
  {
    SipperFilePtr sf = SipperFileRecLoad (sipperFileName);
    if  (sf)
      results->ScanRate (sf->ScanRate ());
    delete  sf;
    sf = NULL;
  }

  return  results;
}  /* InstrumentDataLoad */






InstrumentDataListPtr  DataBase::InstrumentDataLoad (const KKStr&  cruiseName,
                                                     const KKStr&  stationName,
                                                     const KKStr&  deploymentNum,
                                                     const bool&   cancelFlag
                                                    )
{
  // Will acomplish in two parts.
  // 1) Get list of sipper files that meet the criteria passed in,
  // 2) For each sipper file we will call "InstrumentDataLoad" by SipperFileName as defined in prev method.

  VectorKKStr*  sipperFileNames = SipperFileGetList (cruiseName, stationName, deploymentNum);
  if  (sipperFileNames == NULL)
    return NULL;

  InstrumentDataListPtr  data = new InstrumentDataList (true);

  VectorKKStr::iterator  idx;
  for  (idx = sipperFileNames->begin ();  (idx != sipperFileNames->end ())  &&  (!cancelFlag);  idx++)
  {
    InstrumentDataListPtr  dataThisSipperFile = InstrumentDataLoad (*idx, cancelFlag);
    data->AddQueue (*dataThisSipperFile);
    dataThisSipperFile->Owner (false);
    delete  dataThisSipperFile;
    dataThisSipperFile = NULL;
  }

  delete  sipperFileNames;
  sipperFileNames = NULL;

  return  data;
}  /* InstrumentDataLoad */







/**
 @brief  Will update all InstrumentData entries tht fall within the dateTime range with the Latitude and longitude.
 */
void  DataBase::InstrumentDataUpdateLatitudeAndLongitude (const DateTime&  dateTimeStart,
                                                          const DateTime&  dateTimeEnd,
                                                          double           latitude,
                                                          double           longitude
                                                         )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("InstrumentDataUpdateLatitudeAndLongitude");
    return;
  }

  KKStr   sqlStr (1024);
  
  sqlStr << "call  InstrumentDataUpdateLatitudeAndLongitude("
         << DateTimeToQuotedStr (dateTimeStart)         << ", "
         << DateTimeToQuotedStr (dateTimeEnd)           << ", " 
         << StrFormatDouble (latitude,  "---0.0000000")  << ", " 
         << StrFormatDouble (longitude, "---0.0000000")
         << ")";

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd == 0)
    ResultSetsClear ();
    
}  /* InstrumentDataUpdateLatitudeAndLongitude */




vector<vector<float> >*  DataBase::InstrumentDataGetSpecificFields (const KKStr&        sipperFileName,
                                                                    const KKStrListPtr  fieldNames,
                                                                    const DateTime&     dateTimeStart,
                                                                    const DateTime&     dateTimeEnd
                                                                   )
{
  if  (fieldNames == NULL)
    return NULL;

  int numFields = fieldNames->QueueSize ();

  KKStr  fieldNameList (256);

  KKStrList::iterator idx;
  for  (idx = fieldNames->begin ();  idx != fieldNames->end ();  ++idx)
  {
    if  (!fieldNameList.Empty ())
      fieldNameList << ", ";
    fieldNameList << "id." << **idx;
  }

  KKStr  dateTimeStartStr = dateTimeStart.YYYY_MM_DD_HH_MM_SS ().QuotedStr ();
  KKStr  dateTimeEndStr   = dateTimeEnd.YYYY_MM_DD_HH_MM_SS ().QuotedStr ();

  KKStr  sqlStr (256);
  sqlStr << "select " << fieldNameList << "\n"
         << "     from InstrumentData id"                                                   << "\n"
         << "     join SipperFiles sf on(sf.SipperFileId = id.SipperFileId)"                << "\n"
         << "     where (sf.SipperFileName = " << sipperFileName.QuotedStr () << ")   and"  << "\n"
         << "           (id.CTDDateTime >= " << dateTimeStartStr << ") and"                 << "\n"
         << "           (id.CTDDateTime <  " << dateTimeEndStr   << ")"                     << "\n";

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return NULL;

  ResultSetLoad (NULL);
  if  (!resultSetMore)
    return NULL;

  vector<vector<float> >* results = new vector<vector<float> >();

  while  (ResultSetFetchNextRow ())
  {
    vector<float>  row (numFields, 0.0f);
    for  (int xxx = 0;  xxx < numFields;  ++xxx)
      row[xxx] = (float)this->ResultSetGetDoubleField (xxx);
    results->push_back (row);
  }

  ResulSetFree ();
  ResultSetsClear ();

  return  results;
}  /* InstrumentDataGetSpecificFields */




vector<vector<float> >*  DataBase::InstrumentDataGetSpecificFields (const KKStr&        cruiseName,
                                                                    const KKStr&        stationName,
                                                                    const KKStr&        deploymentNum,
                                                                    const KKStrListPtr  fieldNames,
                                                                    const DateTime&     dateTimeStart,
                                                                    const DateTime&     dateTimeEnd
                                                                   )
{
  if  (fieldNames == NULL)
    return NULL;

  int numFields = fieldNames->QueueSize ();

  KKStr  fieldNameList (256);

  KKStrList::iterator idx;
  for  (idx = fieldNames->begin ();  idx != fieldNames->end ();  ++idx)
  {
    if  (!fieldNameList.Empty ())
      fieldNameList << ", ";
    fieldNameList << "id." << **idx;
  }

  KKStr  dateTimeStartStr = dateTimeStart.YYYY_MM_DD_HH_MM_SS ().QuotedStr ();
  KKStr  dateTimeEndStr   = dateTimeEnd.YYYY_MM_DD_HH_MM_SS ().QuotedStr ();

  KKStr  sqlStr (256);
  sqlStr << "select " << fieldNameList << "\n"
         << "     from InstrumentData id"                                     << "\n"
         << "     join SipperFiles sf on(sf.SipperFileId = id.SipperFileId)"  << "\n"
         << "     where ";

  if  (!cruiseName.Empty ())
    sqlStr << "(sf.CruiseName    = " << cruiseName.QuotedStr    () << ")   and"    << "\n";

  if  (!stationName.Empty ())
    sqlStr << "           (sf.stationName   = " << stationName.QuotedStr   () << ")   and"    << "\n";

  if  (!deploymentNum.Empty ())
    sqlStr <<                               "           (sf.deploymentNum = " << deploymentNum.QuotedStr () << ")   and"    << "\n";

  sqlStr << "           (id.CTDDateTime  >= " << dateTimeStartStr           << ")   and"    << "\n"
         << "           (id.CTDDateTime  <  " << dateTimeEndStr             << ")"          << "\n";

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return NULL;
    
  ResultSetLoad (NULL);
  if  (!resultSetMore)
    return NULL;

  vector<vector<float> >* results = new vector<vector<float> >();

  while  (ResultSetFetchNextRow ())
  {
    vector<float>  row (numFields, 0.0f);
    for  (int xxx = 0;  xxx < numFields;  ++xxx)
      row[xxx] = (float)this->ResultSetGetDoubleField (xxx);
    results->push_back (row);
  }

  ResulSetFree ();
  ResultSetsClear ();

  return  results;
}  /* InstrumentDataGetSpecificFields */






/**
 *@brief  Re-Extracts Instrument data from SIPPER file and updates DataBase tables to make sure they are correct.
 *@details This method re-extracts instrument data for the given SipperFile and updates the
 *        appropriate tables.  This way all tables are in agreement with each other.  For the Specified 
 *        SIPPER File "siperFileName".
 * 1) Extract Instrument Data from original SipperFile.
 * 2) Replaces all entries in IntrumentData table for 'sipperFile' with what was just extracted.
 * 3) Updates the InstrumentData fields in Images with data just extracted.
 */
void  DataBase::InstrumentDataReFreshSipperFile (const KKStr&   sipperFileName,
                                                 const bool&    cancelFlag,
                                                 char*          msgBuff,
                                                 int32          msgBuffLen
                                                )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("InstrumentDataReFreshSipperFile");
    return;
  }

  SipperFilePtr  sf = SipperFileRecLoad (sipperFileName);
  if  (!sf) 
  {
    WriteBuff (msgBuff, msgBuffLen, "No sunch SIPPER file.");
    return;
  }

  WriteBuff (msgBuff, msgBuffLen, "Retrieving Instrument Data.");
  InstrumentDataListPtr  data = InstrumentDataFileManager::ReExtractInstrumentDataForSipperFile (sipperFileName, sf, cancelFlag, log);
  if  (!data)
  {
    WriteBuff (msgBuff, msgBuffLen, "Instrument data not found.");
    return;
  }

  if   (cancelFlag)
    return;

  WriteBuff (msgBuff, msgBuffLen, "Updating Instrument Data File.");
  InstrumentDataSaveListForOneSipperFile (sipperFileName, *data, cancelFlag);

  InstrumentDataList::iterator  idx;
  idx = data->begin ();
  InstrumentDataPtr id = *idx;    idx++;

  while  ((idx != data->end ())  &&  (id->ScanLine () < 30000))
  {
    id = *idx;
    idx++;
  }

  if  ((idx == data->end ())  ||  (id == NULL))
  {
    // There is just not enough instrument data
    return;
  }

  InstrumentDataPtr lastId = id;
  uint32  scanLineStart = 0;

  while  ((idx != data->end ())  &&  (!cancelFlag))
  {
    id = *idx;
    uint32  scanLineEnd = id->ScanLine ();

    KKStr  msg (128);
    msg << "Updating[" << sipperFileName << "]  ScanLines[" << scanLineStart << " - " << scanLineEnd << "]";
    WriteBuff (msgBuff, msgBuffLen, msg.Str ());

    cout << msg << endl;

    ImageUpdateInstrumentDataFields (lastId, sipperFileName, scanLineStart, scanLineEnd);

    lastId = id;
    scanLineStart = scanLineEnd;
    idx++;
  }
}  /* InstrumentDataReFreshSipperFile */




VolumeSampledStatListPtr  DataBase::InstrumentDataGetVolumePerMeterDepth (const KKStr&  siperFileName,
                                                                          float         depthBinSize
                                                                         )
{
  KKStr  sipperFileRootName = osGetRootName (siperFileName);
  KKStr  sqlStr (128);
  sqlStr << "call  InstrumentDataGetVolumePerMeterDepth (" << sipperFileRootName.QuotedStr () << ", " << depthBinSize << ")";
  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return  NULL;

  char*  fieldNames[] = {"DownCast", "binId", "binDepth", "volumeSampled", NULL};
  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
  {
    log.Level (-1) << endl << "DataBase::InstrumentDataGetVolumePerMeterDepth   ResultSets not returned." << endl;
    return NULL;
  }

  VolumeSampledStatListPtr  results = new VolumeSampledStatList ();
  while  (ResultSetFetchNextRow ())
  {
    VolumeSampledStatPtr  stat = new VolumeSampledStat ();
    stat->BinId         (ResultSetGetIntField   (1));
    stat->BinDepth      (ResultSetGetFloatField (2));
    stat->VolumeSampled (ResultSetGetFloatField (3));
    results->Give (stat);
  }

  ResulSetFree ();
  ResultSetsClear ();
  return  results;
}  /* InstrumentDataGetVolumePerMeterDepth */





VolumeSampledStatListPtr  DataBase::InstrumentDataGetVolumePerMeterDepth (const KKStr&  cruiseName,
                                                                          const KKStr&  stationName,
                                                                          const KKStr&  deploymentNum,
                                                                          float         depthBinSize,
                                                                          const bool&   cancelFlag
                                                                         )
{
  VectorKKStr*  sipperFileNames = SipperFileGetList (cruiseName, stationName, deploymentNum);
  if  (sipperFileNames == NULL)
    return NULL;

  VolumeSampledStatListPtr  results = new VolumeSampledStatList ();

  VectorKKStr::iterator  idx;
  for  (idx = sipperFileNames->begin ();  (idx != sipperFileNames->end ())  &&  (!cancelFlag);  idx++)
  {
    VolumeSampledStatListPtr  profileThisSiperFile = InstrumentDataGetVolumePerMeterDepth (*idx, depthBinSize);
    if  (profileThisSiperFile)
      (*results) += (*profileThisSiperFile);

    delete  profileThisSiperFile;
    profileThisSiperFile = NULL;
  }

  delete  sipperFileNames;
  sipperFileNames = NULL;

  return  results;
}  /* InstrumentDataGetVolumePerMeterDepth */




InstrumentDataMeansListPtr  DataBase::InstrumentDataBinByMeterDepth (const KKStr&  cruiseName,
                                                                     const KKStr&  stationName,
                                                                     const KKStr&  deploymentNum,
                                                                     float         depthBinSize,
                                                                     const bool&   cancelFlag
                                                                    )
{
  KKStr  depthBinSizeStr = StrFormatDouble (depthBinSize, "##0.0");
  KKStr  sqlStr = "Call  InstrumentDataBinByMeterDepth(" + cruiseName.QuotedStr () + "," + stationName.QuotedStr () + "," + deploymentNum.QuotedStr () + "," + depthBinSizeStr + ")";

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return  NULL;

  char*  fieldNames[] = {"DownCast", "BinId", "BinDepth", "ScanLines", "VolumeSampled",  
                         "TemperatureMean", "SalinityMean", "DenisityMean",
                         "FluorescenceMean", "FluorescenceSensorMean", "OxygenMean", 
                         "DepthMean",  "TransmisivityMean", "TurbidityMean", "CdomFluorescenceMean",
                         NULL
                        };




  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
  {
    log.Level (-1) << endl << "DataBase::InstrumentDataGetVolumePerMeterDepth   ResultSets not returned." << endl;
    return NULL;
  }

  InstrumentDataMeansListPtr  results = new InstrumentDataMeansList (true);
  while  (ResultSetFetchNextRow ())
  {
    bool  downCast               = ResultSetGetBool        (0);
    int32 binId                  = ResultSetGetIntField    (1);
    float binDepth               = ResultSetGetFloatField  (2);
    int32 scanLines              = ResultSetGetIntField    (3);
    float volumeSampled          = ResultSetGetFloatField  (4);
    float temperatureMean        = ResultSetGetFloatField  (5);
    float salinityMean           = ResultSetGetFloatField  (6);
    float denisityMean           = ResultSetGetFloatField  (7);
    float fluorescenceMean       = ResultSetGetFloatField  (8);
    float fluorescenceSensorMean = ResultSetGetFloatField  (9);
    float oxygenMean             = ResultSetGetFloatField  (10);
    float depthMean              = ResultSetGetFloatField  (11);
    float transmisivityMean      = ResultSetGetFloatField  (12);
    float turbidityMean          = ResultSetGetFloatField  (13);
    float cdomFluorescenceMean   = ResultSetGetFloatField  (14);

    results->PushOnBack (new InstrumentDataMeans (downCast,
                                                  binId,
                                                  binDepth,
                                                  scanLines,
                                                  volumeSampled,
                                                  temperatureMean,
                                                  salinityMean,
                                                  denisityMean,
                                                  fluorescenceMean,
                                                  fluorescenceSensorMean,
                                                  oxygenMean,
                                                  depthMean,
                                                  transmisivityMean,
                                                  turbidityMean,
                                                  cdomFluorescenceMean
                                                 )
                        );
  }

  ResulSetFree ();
  ResultSetsClear ();
  return  results;
}  /* InstrumentDataBinByMeterDepth */










GPSDataPointListPtr DataBase::InstrumentDataRetrieveGPSInfo (const KKStr&  cruiseName,
                                                             const KKStr&  stationName,
                                                             const KKStr&  deploymentNum,
                                                             int32         timeInterval   /**< Interval in seconds */
                                                            )
{
  KKStr  sqlStr (128);
  sqlStr << "call  InstrumentDataRetrieveGpsInfo (" 
         << cruiseName.QuotedStr () << ", " 
         << stationName.QuotedStr () << ", " 
         << deploymentNum.QuotedStr () << ", " 
         << timeInterval
         << ")";

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd != 0)
    return  NULL;

  char*  fieldNames[] = {"CTDDateTime", "GpsStartTime", "SipperFileId", "AvgScanLine", "AvgLatitude", "AvgLongitude", "AvgFlowRate", NULL};
  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
  {
    log.Level (-1) << endl << "DataBase::InstrumentDataRetrieveGPSInfo   ResultSets not returned." << endl;
    return NULL;
  }

  GPSDataPointListPtr  results = new GPSDataPointList (true);
  while  (ResultSetFetchNextRow ())
  {
    GPSDataPointPtr  stat 
      = new GPSDataPoint (ResultSetGetDateTimeField      (0),   /**< CTDDateTime  */
                          ResultSetGetTimeField          (1),   /**< GpsStartTime */
                          ResultSetGetIntField           (2),   /**< SipperFileId */
                          (uint32)ResultSetGetFloatField (3),   /**< AvgScanLine  */
                          ResultSetGetDoubleField        (4),   /**< AvgLatitude  */
                          ResultSetGetDoubleField        (5),   /**< AvgLongitude */
                          ResultSetGetFloatField         (6)    /**< AvgFlowRate  */
                         );
    results->PushOnBack (stat);
  }

  ResulSetFree ();
  ResultSetsClear ();
  return  results;
}   /* InstrumentDataRetrieveGpsInfo */







//*********************************************************************************************
//*                                  LogEntries  Rotines                                      *
//*********************************************************************************************
DataBaseLogEntryPtr  DataBase::LogEntriesProcessStart 
                                       (const KKStr&     progCode,
                                        const KKStr&     progName,
                                        const DateTime&  dateTimeCompiled,
                                        const KKStr&     cmdLine,
                                        const KKStr&     sipperFileName
                                        )
{
  KKStr  sqlStr (256);

  KKStr  compName = osGetHostName ();
  KKStr  userName = osGetUserName ();
  KKStr  sipperFileNameRoot = osGetRootName (sipperFileName);

  sqlStr << "call LogEntriesProcessStart("
         << progCode.MaxLen (4).QuotedStr ()  << ","
         << progName.MaxLen (64).QuotedStr () << ","
         << dateTimeCompiled.YYYY_MM_DD_HH_MM_SS ().QuotedStr () << ","
         << cmdLine.MaxLen (512).QuotedStr () << ","
         << compName.QuotedStr ()             << ","
         << userName.QuotedStr ()             << ","
         << sipperFileNameRoot.QuotedStr ()   
         << ")";

  DataBaseLogEntryPtr  result = new DataBaseLogEntry (progCode, progName, dateTimeCompiled, cmdLine, compName, userName, sipperFileName);

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd == 0)
  {
    char*  fieldNames[] = {"LogEntryId", "DataBaseUserName", "DateTimeStart", "DateTimeStartUtc", NULL};
    ResultSetLoad (fieldNames);
    if  (!resultSetMore)
      return  NULL;

    MYSQL_ROW  row = NULL;
    while  (ResultSetFetchNextRow ())
    {
      result->LogEntryId       (ResultSetGetUintField     (0));
      result->DataBaseUserName (ResultSetGetField         (1));
      result->DateTimeStart    (ResultSetGetDateTimeField (2));
      result->DateTimeStartUtc (ResultSetGetDateTimeField (3));
    }
  }

  ResulSetFree ();
  ResultSetsClear ();
  return  result;
}  /* LogEntriesProcessStart */



  

void  DataBase::LogEntriesProcessEnd (DataBaseLogEntryPtr   logEntry,
                                      float                 cpuTimeUsed,
                                      const KKStr&          completionResult
                                     )
{
  if  (!logEntry)
  {
    log.Level (-1) << endl << "DataBase::LogEntriesProcessEnd   ***ERROR***      logEntry == NULL" << endl << endl;
    return;
  }

  KKStr  sqlStr (128);

  sqlStr << "call LogEntriesProcessEnd("
         << logEntry->LogEntryId ()        << ","
         << cpuTimeUsed                    << ","
         << completionResult.QuotedStr ()
         << ")";

  int32  returnCd = QueryStatement (sqlStr);
  if  (returnCd == 0)
  {
    char*  fieldNames[] = {"DateTimeEnd", NULL};
    ResultSetLoad (fieldNames);
    if  (!resultSetMore)
      return;

    MYSQL_ROW  row = NULL;
    while  (ResultSetFetchNextRow ())
    {
      logEntry->DateTimeEnd (ResultSetGetDateTimeField (0));
    }
  }

  ResulSetFree ();
  ResultSetsClear ();
  return;
}  /* LogEntriesProcessEnd */




DataBaseLogEntryPtr  DataBase::LogEntriesSelect (uint32 _logEntryId)
{
  DataBaseLogEntryPtr  result = NULL;
  KKStr  sqlCmd (100);
  sqlCmd << "call LogEntriesSelect(" << _logEntryId << ")";
  int32  returnCd = QueryStatement (sqlCmd);
  if  (returnCd == 0)
  {
    char*  fieldNames[] = {"LogEntryId",         //  0
                           "ProgCode",           //  1
                           "ProgName",           //  2
                           "DateTimeCompiled",   //  3
                           "CmdLine",            //  4
                           "CompName",           //  5
                           "DataBaseUserName",   //  6
                           "CompUserName",       //  7
                           "DateTimeStart",      //  8
                           "DateTimeStartUtc",   //  9
                           "DateTimeEnd",        // 10
                           "CpuTimeUsed",        // 11
                           "CompletionStatus",   // 12
                           "SipperFileName",     // 13
                           NULL
                          };

    ResultSetLoad (fieldNames);
    if  (!resultSetMore)
      return  NULL;

    MYSQL_ROW  row = NULL;
    while  (ResultSetFetchNextRow ())
    {
      delete result;
      result = NULL;
      result = new DataBaseLogEntry ();
      result->LogEntryId       (ResultSetGetUintField      (0));
      result->ProgCode         (ResultSetGetField          (1));
      result->ProgName         (ResultSetGetField          (2));
      result->DateTimeCompiled (ResultSetGetDateTimeField  (3));
      result->CmdLine          (ResultSetGetField          (4));
      result->CompName         (ResultSetGetField          (5));
      result->DataBaseUserName (ResultSetGetField          (6));
      result->CompUserName     (ResultSetGetField          (7));
      result->DateTimeStart    (ResultSetGetDateTimeField  (8));
      result->DateTimeStartUtc (ResultSetGetDateTimeField  (9));
      result->DateTimeEnd      (ResultSetGetDateTimeField (10));
      result->CpuTimeUsed      (ResultSetGetFloatField    (11));
      result->CompletionStatus (ResultSetGetField         (12));
      result->SipperFileName   (ResultSetGetField         (13));
    }
  }

  ResulSetFree ();
  ResultSetsClear ();
  return  result;
}





//*********************************************************************************************
//*                                 SipperCruise  Rotines                                     *
//*********************************************************************************************

void  DataBase::InsertCruise (const KKStr&     shipName, 
                              const KKStr&     description,
                              const DateType&  dateStart,
                              const DateType&  dateEnd,
                              const KKStr&     location,
                              const KKStr&     objective,
                              const KKStr&     principal,
                              const KKStr&     researchOrg
                             )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("InsertCruise");
    return;
  }

  KKStr  shipNameAdj = shipName;
  if  (shipNameAdj.Len () > 10)
    shipNameAdj = shipNameAdj.SubStrPart (0, 9);

  KKStr  insertStatement;
  insertStatement << "Insert into Cruises (CruiseName, ShipName, Description, DateStart, DateEnd, Location, Objective, Principal, ResearchOrg)" << endl
                  << "             values ("
                  << shipNameAdj.QuotedStr ()    << ", "
                  << description.QuotedStr ()    << ", "
                  << DateToQuotedStr (dateStart) << ", "
                  << DateToQuotedStr (dateEnd)   << ", "
                  << location.QuotedStr ()       << ", "
                  << objective.QuotedStr ()      << ", "
                  << principal.QuotedStr ()      << ", "
                  << researchOrg.QuotedStr ()
                  << ")" << endl;

  int32  returnCd = QueryStatement (insertStatement.Str ());
}  /* InsertCruise  */



                            
void  DataBase::SipperCruiseDelete (const KKStr&  cruiseName)
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperCruiseDelete");
    return;
  }

  KKStr  sqlStr = "call  CruisesDelete(" + cruiseName.QuotedStr () + ")";
  int32  returnCd = QueryStatement (sqlStr.Str ());
  ResultSetsClear ();
}  /* SipperCruiseDelete */




void  DataBase::SipperCruiseInsert (SipperCruisePtr  cruise,
                                    bool&            successful
                                   )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperCruiseInsert");
    return;
  }

  KKStr  insertStr (1000);

  insertStr <<  "insert into  Cruises (CruiseName, ShipName, Description, DateStart, DateEnd, Location, Objective, Principal, ResearchOrg)" << endl
            <<  "values(" 
            <<  cruise->CruiseName  ().QuotedStr () << ", "
            <<  cruise->ShipName    ().QuotedStr () << ", "
            <<  cruise->Description ().QuotedStr () << ", "
            <<  cruise->DateStart   ().YYYY_MM_DD ().QuotedStr () << ", "
            <<  cruise->DateEnd     ().YYYY_MM_DD ().QuotedStr () << ", "
            <<  cruise->Location    ().QuotedStr () << ", "
            <<  cruise->Objective   ().QuotedStr () << ", "
            <<  cruise->Principal   ().QuotedStr () << ", "
            <<  cruise->ResearchOrg ().QuotedStr ()
            <<  ")"
            << endl;

  int32  returnCd = QueryStatement (insertStr.Str ());

  successful   = (returnCd == 0);
  return;
}  /* SipperCruiseInsert */




void  DataBase::SipperCruiseUpdate (SipperCruisePtr  cruise,
                                    bool&            successful
                                   )
{
  if  (!allowUpdates)
  {
    UpdatesNotAllowed ("SipperCruiseUpdate");
    return;
  }

  KKStr  updateStr (2000);

  updateStr << "update Cruises " << endl
            << "  set  ShipName     = " << cruise->ShipName     ().QuotedStr () << ", "
            << "       Description  = " << cruise->Description  ().QuotedStr () << ", "
            << "       DateStart    = " << cruise->DateStart    ().YYYY_MM_DD ().QuotedStr () << ", "
            << "       DateEnd      = " << cruise->DateEnd      ().YYYY_MM_DD ().QuotedStr () << ", "
            << "       Location     = " << cruise->Location     ().QuotedStr () << ", "
            << "       Objective    = " << cruise->Objective    ().QuotedStr () << ", "
            << "       Principal    = " << cruise->Principal    ().QuotedStr () << ", "
            << "       ResearchOrg  = " << cruise->ResearchOrg  ().QuotedStr ()
            << endl
            << "    where CruiseName = " << cruise-> CruiseName ().QuotedStr () << endl;

  int32  returnCd = QueryStatement (updateStr.Str ());

  successful   = (returnCd == 0);
  return;
}  /* SipperCruiseUpdate */



SipperCruiseListPtr  DataBase::SipperCruiseProcessResults ()
{
  char*  fieldNames[] = {"CruiseName", "ShipName", "Description", "DateStart", "DateEnd", "Location", "Objective", "Principal", "ResearchOrg", NULL};
  ResultSetLoad (fieldNames);
  if  (!resultSetMore)
  {
    log.Level (-1) << endl << "DataBase::SipperCruiseProcessResults   ResultSets not returned." << endl;
    return NULL;
  }

  SipperCruiseListPtr  results = new SipperCruiseList (true);
  while  (ResultSetFetchNextRow ())
  {
    SipperCruisePtr sipperCruise = new SipperCruise ();
    sipperCruise->CruiseName   (ResultSetGetField (0));
    sipperCruise->ShipName     (ResultSetGetField (1));
    sipperCruise->Description  (ResultSetGetField (2));
    sipperCruise->DateStart    (ResultSetGetDateTimeField (3).Date ());
    sipperCruise->DateEnd      (ResultSetGetDateTimeField (4).Date ());
    sipperCruise->Location     (ResultSetGetField (5)); 
    sipperCruise->Objective    (ResultSetGetField (6));
    sipperCruise->Principal    (ResultSetGetField (7));
    sipperCruise->ResearchOrg  (ResultSetGetField (8));
    results->PushOnBack (sipperCruise);
  }

  ResulSetFree ();
  return  results;
}



SipperCruisePtr  DataBase::SipperCruiseLoad (const KKStr&  cruiseName)
{
  KKStr queryStr = "select * from Cruises  where cruiseName = " + cruiseName.QuotedStr ();
  int32  returnCd = QueryStatement (queryStr);
  if  (returnCd != 0)
    return  NULL;

  SipperCruiseListPtr  results = SipperCruiseProcessResults ();
  ResultSetsClear ();

  if  (!results)
    return NULL;

  SipperCruisePtr  cruise = results->PopFromBack ();

  delete  results;
  return  cruise;
}  /* SipperCruiseLoad */




SipperCruiseListPtr  DataBase::SipperCruiseLoadAllCruises ()
{
  KKStr  selectStr (1000);
  selectStr << "select * from Cruises c  order by c.DateStart";
  int32  returnCd = QueryStatement (selectStr);
  if  (returnCd != 0)
    return  NULL;

  SipperCruiseListPtr  results = SipperCruiseProcessResults ();
  ResultSetsClear ();

  if  (results)
    results->SortByCruiseName ();

  return  results;
}  /* SipperCruiseLoadAllCruises */




