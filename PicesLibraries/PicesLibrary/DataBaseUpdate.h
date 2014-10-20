#if  !defined(_DATABASEUPDATE_)
#define  _DATABASEUPDATE_

/**
 *@class  MLL::DataBaseUpdate
 *@brief  Routines used to update the MySql database to the curent version.
 *
 */



#include  "DateTime.h"
#include  "GoalKeeper.h"
#include  "Raster.h"
#include  "RunLog.h"
#include  "Str.h"
using namespace KKU;



#include  "DataBaseUpdate.h"


namespace MLL 
{
  #if  !defined(_DATABASESERVER_)
  class  DataBaseServer;
  typedef  DataBaseServer*  DataBaseServerPtr;

  class  DataBaseServerList;
  typedef  DataBaseServerList*  DataBaseServerListPtr;
  #endif


  #if  !defined(_DATABASE_)
  class  DataBase;
  typedef  DataBase*  DataBasePtr;
  #endif





  /**
   @class  DataBaseUpdate
   @brief DataBaseUpdate connection management object.  All database communications will go through this class.
   @author Kurt Kramer
   @date Dec/01/2008
   */
  class  DataBaseUpdate
  {
  public:
    typedef  DataBaseUpdate*  DataBaseUpdatePtr;

    typedef  KKU::uchar   uchar;
    typedef  KKU::ulong   ulong;
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;
    typedef  KKU::int64   int64;
    typedef  KKU::uint64  uint64;


    DataBaseUpdate (RunLog&  _log);

    ~DataBaseUpdate ();


    void  LoadSourceFile (const KKStr&  fileName,
                          DataBasePtr   dbConn,
                          bool&         successful,
                          KKStrListPtr  errorMsgs,
                          RunLog&       log
                         );


  };  /* DataBaseUpdate */


  typedef  DataBaseUpdate::DataBaseUpdatePtr  DataBaseUpdatePtr;

}  /* namespace MLL */


#endif
