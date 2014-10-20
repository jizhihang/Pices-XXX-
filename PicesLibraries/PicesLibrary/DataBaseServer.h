#if  !defined(_DATABASESERVER_)
#define  _DATABASESERVER_


/**
 *@class MLL::DataBaseServer
 *@brief  Represents the parameters for a single Database Server.  
 *@details  Things like username, hostname, etc.   The "DataBase"  class will use one instance
 *          of this object to use as connection parameters.
 */


#include  "KKQueue.h"
#include  "RunLog.h"
#include  "Str.h"
using namespace KKU;

namespace MLL 
{
  class  DataBaseServer
  {
  public:
    typedef  DataBaseServer*    DataBaseServerPtr;

    DataBaseServer ();
    DataBaseServer (const DataBaseServer&  dataBaseServer);
    DataBaseServer (const KKStr&           parameterStr);

    ~DataBaseServer ();

    const KKStr&  DataBaseName ()  const  {return dataBaseName;}
    const KKStr&  Description  ()  const  {return description;}
    bool          Embedded     ()  const  {return embedded;}
    const KKStr&  HostName     ()  const  {return hostName;}
    const KKStr&  MySqlDataDir ()  const  {return mySqlDataDir;}
    const KKStr&  PassWord     ()  const  {return passWord;}
    const KKStr&  UserName     ()  const  {return userName;}

    void  DataBaseName (const KKStr&  _dataBaseName)  { dataBaseName = _dataBaseName;}
    void  Description  (const KKStr&  _description)   { description  = _description;}
    void  Embedded     (bool          _embedded)      { embedded     = _embedded;}
    void  HostName     (const KKStr&  _hostName)      { hostName     = _hostName;}
    void  MySqlDataDir (const KKStr&  _mySqlDataDir)  { mySqlDataDir = _mySqlDataDir;}
    void  PassWord     (const KKStr&  _passWord)      { passWord     = _passWord;}
    void  UserName     (const KKStr&  _userName)      { userName     = _userName;}


    KKStr  ServerDescription ()  const;  // Get description of server for info display

    KKStr  ToParameterStr ()  const;

  private:
    void  ParseParameterStr (const  KKStr&  parameterStr);

    KKStr     dataBaseName;
    KKStr     description;
    bool      embedded;
    KKStr     hostName;
    KKStr     mySqlDataDir;   /**< Used with Embedded server. */
    KKStr     userName;
    KKStr     passWord;
  };  /* DataBaseServer */


  typedef  DataBaseServer::DataBaseServerPtr  DataBaseServerPtr;



  class  DataBaseServerList: public  KKQueue<DataBaseServer>
  {
  public:
    typedef  DataBaseServerList*  DataBaseServerListPtr;

    DataBaseServerList (RunLog&  _log,                         // Will load lost from "MySql.cfg" file in "%PicesHomeDir%\Configurations" directory
                        bool     _loadFromConfigFile = true    // if set to false will create an empty list.
                       );

    DataBaseServerList (const DataBaseServerList&  serverList);

   ~DataBaseServerList ();


    const KKStr&        DefaultServerDescription ()  const  {return defaultServerDescription;}
    void                DefaultServerDescription (const KKStr&   _defaultServerDescription)  {defaultServerDescription = _defaultServerDescription;}


    DataBaseServerPtr   LookUpByDescription (const KKStr&  _description);

    DataBaseServerPtr   GetDefaultServer ();   // Will return a pointer to the DataBaseServer instance that
                                               // has the same decription as "defaultServerDescription".
  


    RunLog&   Log ()   {return  log;}

    void  WriteConfigFile ()  const;

  private:
    void  ReadConfigFile ();

    KKStr  defaultServerDescription;

    RunLog&  log;
  };


  typedef  DataBaseServerList::DataBaseServerListPtr  DataBaseServerListPtr;

}  /* namespace MLL */

#endif
