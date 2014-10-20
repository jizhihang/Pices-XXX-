#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"
#include  "..\BaseLibrary\GoalKeeper.h"

#include  "PicesKKStr.h"
#include  "PicesSipperFile.h"

#include  "PicesDataBaseServer.h"

using namespace  PicesInterface;

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace KKU;



PicesDataBaseServer::PicesDataBaseServer ():
  server (NULL)
{
  server = new DataBaseServer ();
}




PicesDataBaseServer::PicesDataBaseServer (PicesDataBaseServer^  picesServer):
  server (NULL)
{
  server = new DataBaseServer (*(picesServer->server));
}



PicesDataBaseServer::PicesDataBaseServer (const DataBaseServerPtr  _server):
  server (NULL)
{
  server = new DataBaseServer (*_server);
}




PicesDataBaseServer::~PicesDataBaseServer ()
{
  this->!PicesDataBaseServer ();
}


PicesDataBaseServer::!PicesDataBaseServer ()
{
  delete  server;
  server = NULL;
}



String^  PicesDataBaseServer::Description::get () 
{
  if  (!server)
    return  String::Empty;

  return  PicesKKStr::KKStrToSystenStr (server->Description ());
}


void  PicesDataBaseServer::Description::set (String^  _description)
{
  if  (!server)
    server = new DataBaseServer ();
  server->Description (PicesKKStr::SystemStringToKKStr (_description));
}





String^  PicesDataBaseServer::HostName::get () 
{
  if  (!server)
    return  String::Empty;

  return  PicesKKStr::KKStrToSystenStr (server->HostName ());
}


void  PicesDataBaseServer::HostName::set (String^ _hostName)
{
  if  (!server)
    server = new DataBaseServer ();
  server->HostName (PicesKKStr::SystemStringToKKStr (_hostName));
}



String^  PicesDataBaseServer::UserName::get () 
{
  if  (!server)
    return  String::Empty;

  return  PicesKKStr::KKStrToSystenStr (server->UserName ());
}


void  PicesDataBaseServer::UserName::set (String^  _userName)
{
  if  (!server)
    server = new DataBaseServer ();
  server->UserName (PicesKKStr::SystemStringToKKStr (_userName));
}




String^  PicesDataBaseServer::PassWord::get () 
{
  if  (!server)
    return  String::Empty;

  return  PicesKKStr::KKStrToSystenStr (server->PassWord ());
}


void  PicesDataBaseServer::PassWord::set (String^  _passWord)
{
  if  (!server)
    server = new DataBaseServer ();
  server->PassWord (PicesKKStr::SystemStringToKKStr (_passWord));
}



String^  PicesDataBaseServer::DataBaseName::get () 
{
  if  (!server)
    return  String::Empty;

  return  PicesKKStr::KKStrToSystenStr (server->DataBaseName ());
}




void  PicesDataBaseServer::DataBaseName::set (String^  _dataBaseName)
{
  if  (!server)
    server = new DataBaseServer ();
  server->DataBaseName (PicesKKStr::SystemStringToKKStr (_dataBaseName));
}




void  PicesDataBaseServer::CleanUpMemory ()
{
}




PicesDataBaseServerList::PicesDataBaseServerList (PicesRunLog^  _log)
{
  log = _log;
  DataBaseServerListPtr  servers = new DataBaseServerList (log->Log ());

  DataBaseServerList::const_iterator  idx;

  for  (idx = servers->begin ();  idx != servers->end ();  idx++)
  {
    DataBaseServerPtr  s = *idx;
    Add (gcnew PicesDataBaseServer (s));
  }

  defaultServerDescription = PicesKKStr::KKStrToSystenStr (servers->DefaultServerDescription ());

  // Since 'PicesDataBaseServer' takes ownership of the 'DataBaseServer' object being passed in
  // we don't want the 'servers' list to own them;  this way when we delete 'servers' we do
  // not delete the instances of 'DataBaseServer' that it references.
  servers->Owner (false);
  delete  servers;
  servers = NULL;
}





PicesDataBaseServerList::PicesDataBaseServerList (const DataBaseServerListPtr  _servers,
                                                  PicesRunLog^                 _log
                                                 )
{
  log = _log;

  DataBaseServerList::const_iterator  idx;

  for  (idx = _servers->begin ();  idx != _servers->end ();  idx++)
  {
    DataBaseServerPtr  s = *idx;
    Add (gcnew PicesDataBaseServer (new DataBaseServer (*s)));
  }

  defaultServerDescription = PicesKKStr::KKStrToSystenStr (_servers->DefaultServerDescription ());
}



PicesDataBaseServerList::~PicesDataBaseServerList ()
{
  CleanUpMemory ();
}



void  PicesDataBaseServerList::CleanUpMemory ()
{
}  /* CleanUpMemory */




PicesDataBaseServer^   PicesDataBaseServerList::GetDefaultServer ()
{
  PicesDataBaseServer^  defaultServer = LookUpByDescription (defaultServerDescription);
  if  (defaultServer == nullptr) 
  {
    defaultServer = LookUpByDescription ("Default");
    if  (defaultServer == nullptr)
    {
      if  (Count > 0)
        defaultServer = (*this)[0];
    }
  }
  return  defaultServer;
}  /* GetDefaultServer*/




PicesDataBaseServer^  PicesDataBaseServerList::LookUpByDescription (String^ _description)
{
  PicesDataBaseServer^ s = nullptr;

  for each  (PicesDataBaseServer^  server in (*this))
  {
    if  (String::Compare (_description, server->Description, true) == 0)
    {
      s = server;
      break;
    }
  }

  return  s;
}  /* LookUpByDescription */




void  PicesDataBaseServerList::WriteConfigFile ()
{
  DataBaseServerListPtr  servers = new DataBaseServerList (log->Log (), 
                                                           false        // false = Do not load from config file
                                                          );

  for each  (PicesDataBaseServer^  pdbs  in  (*this))
  {
    DataBaseServerPtr dbs = new DataBaseServer (*(pdbs->UnManagedClass ()));
    servers->PushOnBack (dbs);
  }

  servers->DefaultServerDescription (PicesKKStr::SystemStringToKKStr (defaultServerDescription));
  servers->WriteConfigFile ();
  delete  servers;
  servers = NULL;
}  /* WriteConfigFile */
