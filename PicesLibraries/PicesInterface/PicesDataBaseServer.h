#pragma once

#include  "DateTime.h"
#include  "Str.h"
using namespace  KKU;

#include  "DataBaseServer.h"
using namespace  MLL;


#include  "PicesRunLog.h"

//  Job in Life:   This module is the Managed version of "DataBaseServer".


namespace PicesInterface 
{

  public  ref  class PicesDataBaseServer
  {
  public:
    PicesDataBaseServer ();
    
    PicesDataBaseServer (const DataBaseServerPtr  server);  // Will take ownership of 'server' 
    
    PicesDataBaseServer (PicesDataBaseServer^  picesServer);

  protected:
    ~PicesDataBaseServer ();

  private:
    !PicesDataBaseServer ();

  public:
    DataBaseServerPtr   Server ()  {return  server;}

    property  String^  Description  {String^   get ();   void set (String^ _description)  ;}
    property  String^  HostName     {String^   get ();   void set (String^ _hostName)     ;}
    property  String^  UserName     {String^   get ();   void set (String^ _userName)     ;}
    property  String^  PassWord     {String^   get ();   void set (String^ _passWord)     ;}
    property  String^  DataBaseName {String^   get ();   void set (String^ _dataBaseName) ;}

    void  CleanUpMemory ();

    DataBaseServerPtr   UnManagedClass ()   {return  server;}

  private:
    DataBaseServerPtr   server;
  };



  public ref class PicesDataBaseServerList:  List<PicesDataBaseServer^>
  {
  public:
    PicesDataBaseServerList (PicesRunLog^  _log);


    PicesDataBaseServerList (const DataBaseServerListPtr  _servers,
                             PicesRunLog^                 _log
                            );

    ~PicesDataBaseServerList ();

    property  String^  DefaultServerDescription  
                          {
                           String^  get ()  {return  defaultServerDescription;}  
                           void set (String^ _defaultServerDescription)  {defaultServerDescription = _defaultServerDescription;}
                          }

    property  PicesRunLog^  Log  {PicesRunLog^   get () {return  log;};}


    void  CleanUpMemory ();


    PicesDataBaseServer^  GetDefaultServer ();
    PicesDataBaseServer^  LookUpByDescription (String^ _description);

    void  WriteConfigFile ();


  private:
    String^        defaultServerDescription;
    PicesRunLog^   log;
  };
}
