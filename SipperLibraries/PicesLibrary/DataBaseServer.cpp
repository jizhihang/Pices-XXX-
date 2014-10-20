#include  "FirstIncludes.h"

#include  <ctype.h>
#include  <time.h>

#include  <fstream>
#include  <iostream>
#include  <map>
#include  <string>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"


using namespace std;

#include "OSservices.h"
using namespace KKU;

#include "SipperVariables.h"
using namespace SipperHardware;


#include "DataBaseServer.h"
using  namespace  MLL;


DataBaseServer::DataBaseServer ():
  description   ("Default"),
  embedded      (false),
  hostName      ("localhost"),
  mySqlDataDir  (""),
  userName      ("root"),
  passWord      ("dasani20"),
  dataBaseName  ("pices")
{
}



DataBaseServer::DataBaseServer (const DataBaseServer&  dataBaseServer):
  description   (dataBaseServer.description),
  embedded      (dataBaseServer.embedded),
  hostName      (dataBaseServer.hostName),
  mySqlDataDir  (dataBaseServer.mySqlDataDir),
  userName      (dataBaseServer.userName),
  passWord      (dataBaseServer.passWord),
  dataBaseName  (dataBaseServer.dataBaseName)
{
}



DataBaseServer::DataBaseServer (const KKStr&  parameterStr):
  description   ("Default"),
  embedded      (false),
  hostName      ("localhost"),
  mySqlDataDir  (""),
  userName      ("root"),
  passWord      ("dasani20"),
  dataBaseName  ("pices")
{
  ParseParameterStr (parameterStr);
}

  

DataBaseServer::~DataBaseServer ()
{

}



KKStr  DataBaseServer::ServerDescription ()  const  // Get description of server for info display
{
  if  (embedded)
    return  "Embedded " + description + "  MySqlDataDir[" + mySqlDataDir + "]  Database[" + dataBaseName + "]";
  else
    return  description + "  Host[" + hostName + "]  User[" + userName + "]  Database[" + dataBaseName + "]";
}



void  DataBaseServer::ParseParameterStr (const  KKStr&  parameterStr)
{
  VectorKKStr  parameterPairs = parameterStr.Split ("\t");

  VectorKKStr::iterator  idx;
  for  (idx = parameterPairs.begin ();  idx != parameterPairs.end ();  idx++)
  {
    VectorKKStr  fields = (*idx).Split (":=");   // Split by either ':'  or  '='
    if  (fields.size () < 2)
    {
      // Should be two fields;  line must be malformed.
      continue;
    }

    KKStr  parameterName = fields[0].ToUpper ();

    if      ((parameterName == "EMBEDDED")      ||  (parameterName == "EMB")       ||  (parameterName == "E"))   embedded     = fields[1].ToBool ();
    else if ((parameterName == "MYSQLDATADIR")  ||  (parameterName == "MYSQL")     ||  (parameterName == "MDD")) mySqlDataDir = fields[1];
    else if ((parameterName == "DESCRIPTION")   ||  (parameterName == "DESC")      ||  (parameterName == "D"))   description  = fields[1];
    else if ((parameterName == "HOSTNAME")      ||  (parameterName == "HOST")      ||  (parameterName == "H"))   hostName     = fields[1];
    else if ((parameterName == "USERNAME")      ||  (parameterName == "USER")      ||  (parameterName == "U"))   userName     = fields[1];
    else if ((parameterName == "PASSWORD")      ||  (parameterName == "PW")        ||  (parameterName == "P"))   passWord     = fields[1];
    else if ((parameterName == "DATABASENAME")  ||  (parameterName == "DATABASE")  ||  (parameterName == "DB"))  dataBaseName = fields[1];
  }

  if  (description.EqualIgnoreCase ("Embedded"))
    embedded = true;
}  /* ParseParameterStr */





KKStr  DataBaseServer::ToParameterStr ()  const
{
  KKStr   parameterStr (1024);

  parameterStr = "Description:"   + description                + "\t"  +
                 "Embedded:"      + (embedded ? "Yes" : "No")  + "\t"  +
                 "HostName:"      + hostName                   + "\t"  +
                 "UserName:"      + userName                   + "\t"  +
                 "PassWord:"      + passWord                   + "\t"  +
                 "MySqlDataDir:"  + mySqlDataDir               + "\t"  +
                 "DataBaseName:"  + dataBaseName;

  return  parameterStr;

}  /* ToParameterStr */





DataBaseServerList::DataBaseServerList (RunLog&  _log,
                                        bool     _loadFromConfigFile
                                       ):  
  KKQueue<DataBaseServer> (true, 10),   // true = We will own contents,  10 = Initial capacity set to 10
  defaultServerDescription ("Default"),
  log                      (_log)
{
  if  (_loadFromConfigFile)
    ReadConfigFile ();
}



DataBaseServerList::DataBaseServerList (const DataBaseServerList&  serverList):
    KKQueue<DataBaseServer> (true, QueueSize ()),
    defaultServerDescription  (serverList.defaultServerDescription),
    log                       (serverList.log)
{
  DataBaseServerList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    DataBaseServerPtr  dbs = *idx;
    PushOnBack (new DataBaseServer (*dbs));
  }
}


DataBaseServerList::~DataBaseServerList ()
{
}






void  DataBaseServerList::WriteConfigFile ()  const
{
  osCreateDirectoryPath (SipperVariables::PicesConfigurationDirectory ());
  KKStr  configFileName = osAddSlash (SipperVariables::PicesConfigurationDirectory ()) + "MySql.cfg";
 
  log.Level (10) << "DataBaseServerList::WriteConfigFile    Writing ConfigFileName[" << configFileName << "]" << endl;
  
  ofstream  o (configFileName.Str ());

  if  (!o.is_open ())
  {
    log.Level (-1) << endl << endl
                   << "DataBaseServerList::WriteConfigFile     ***ERROR***   Opening ConfigFile["  <<  configFileName << "]" << endl
                   << endl;
    return;
  }

  o << "//  PICES  MySQL  configuration file.  There will be one entry for each known MySQL database server." << std::endl
    << "//"  << std::endl
    << "//   Date Writen: " << osGetLocalDateTime () << std::endl
    << "//"  << std::endl
    << "//  Num Entries: " << QueueSize () << std::endl
    << "//" << std::endl;

  DataBaseServerList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
    o << "Server" << "\t" << (*idx)->ToParameterStr () << endl;

  o << "//"  << std::endl
    << "DefaultServer" << "\t" << defaultServerDescription << std::endl;

  o.close ();
}  /* WriteConfigFile */




void  DataBaseServerList::ReadConfigFile ()
{
  KKStr  configFileName = osAddSlash (SipperVariables::PicesConfigurationDirectory ()) + "MySql.cfg";

  log.Level (10) << "DataBaseServerList::ReadConfigFile      ConfigFileName[" <<  configFileName << "]" << endl;
  FILE* in = osFOPEN (configFileName.Str (), "r");
  if  (!in)
  {
    log.Level (-1) << "DataBaseServerList::ReadConfigFile    Error loading ConfigFileName[" <<  configFileName << "]" << endl;
    return;
  }

  defaultServerDescription = "";

  bool  defaultServerFound = false;

  char  buff[1024];
  while  (fgets (buff, sizeof (buff), in))
  {
    buff[sizeof(buff) - 1] = 0;  // Just to make sure that bufer not comepletely full
    KKStr  ln (buff);

    if  (ln.Len () < 3)
      continue;

    if  ((ln[0] == '/')  &&  (ln[1] == '/'))
      continue;

    KKStr  lineName = ln.ExtractToken2 ("\n\r\t");
    if  (lineName.EqualIgnoreCase ("Server"))
    {
      DataBaseServerPtr  server = new DataBaseServer (ln);
      PushOnBack (server);
    }

    else if  (lineName.EqualIgnoreCase ("DefaultServer"))
    {
      defaultServerDescription = ln.ExtractToken2 ("\n\r\t");
      defaultServerFound = true;
    }
  }

  if  (!defaultServerFound)
  {
    DataBaseServerPtr  defaultServer = LookUpByDescription ("Local");
    if  (!defaultServer)
      defaultServer = LookUpByDescription ("localhost"); 

    if  (!defaultServer)
    {
      defaultServer = new DataBaseServer ();
      defaultServer->UserName      ("Root");
      defaultServer->Description   ("localhost");
      defaultServer->Embedded      (false);
      defaultServer->HostName      ("localhost");
      defaultServer->MySqlDataDir  ("");
      defaultServer->UserName      ("root");
      defaultServer->PassWord      ("dasani30");
      defaultServer->DataBaseName  ("pices_new");
      defaultServerDescription = defaultServer->Description ();
      PushOnBack (defaultServer);
    }

    defaultServerDescription = defaultServer->Description ();
  }

  fclose (in);
}  /* ReadConfigFile */




DataBaseServerPtr  DataBaseServerList::LookUpByDescription (const KKStr&  _description)
{
  DataBaseServerList::iterator  idx;

  DataBaseServerPtr  dabaseServer = NULL;

  for  (idx = begin ();  idx != end ();  idx++)
  {
     DataBaseServerPtr  dbs = *idx;
     if  (dbs->Description ().EqualIgnoreCase (_description))
     {
       dabaseServer = dbs;
       break;
     }
  }

  return  dabaseServer;
}  /* LookUpByDescription */




DataBaseServerPtr   DataBaseServerList::GetDefaultServer ()
{
  DataBaseServerPtr  defaultServer = LookUpByDescription (defaultServerDescription);
  if  (!defaultServer)
  {
    defaultServer = LookUpByDescription ("Default");
    if  (!defaultServer)
    {
      if  (QueueSize () > 0)
        defaultServer = IdxToPtr (0);   // Just return the first one in the list.

    }
  }
  return  defaultServer;
}  /* GetDefaultServer */



