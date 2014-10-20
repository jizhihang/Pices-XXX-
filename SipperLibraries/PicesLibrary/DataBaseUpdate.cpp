//  because <my_global.h> needs to define some Macro's before windows.h  and stdlib.h  we need t include
// before  "FirstIncludes.h"
#include  <errmsg.h>
#include  <my_global.h>
#include  <mysql.h>

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
using namespace KKU;

#include "DataBase.h"
#include "DataBaseUpdate.h"
using  namespace  MLL;







DataBaseUpdate::DataBaseUpdate (RunLog&  _log)
{
}




DataBaseUpdate::~DataBaseUpdate ()
{

}




void  DataBaseUpdate::LoadSourceFile (const KKStr&  fileName,
                                      DataBasePtr   dbConn,
                                      bool&         successful,
                                      KKStrListPtr  errorMsgs,
                                      RunLog&       log
                                     )
{
  successful = true;
  int32  fileSize = (int32)osGetFileSize (fileName);
  if  (fileSize < 0)
  {
    KKStr errorMsg = "File[" + fileName + "] Not Found";
    log.Level (-1) << "DataBaseUpdate::LoadSourceFile   ***ERROR***   " << errorMsg << endl;
    successful = false;
    if  (errorMsgs)
      errorMsgs->PushOnBack (new KKStr (errorMsg));
    return;
  }

  int32  maxLen = fileSize + 500;

  char*  sqlStr = new char[maxLen + 1];   // "+ 1" added to account for terminating 0 if whil estring is used.

  FILE*  f = osFOPEN (fileName.Str (), "r");
  if  (!f)
  {
    KKStr errorMsg = "File[" + fileName + "] could not be opened.";
    log.Level (-1) << "DataBaseUpdate::LoadSourceFile   ***ERROR***   " << errorMsg << endl;
    successful = false;
    if  (errorMsgs)
      errorMsgs->PushOnBack (new KKStr (errorMsg));
    return;
  }


  






  VectorKKStr  columnNames;
  int32  sqlStrLen = fread (sqlStr, 1, maxLen, f);
  sqlStr[sqlStrLen] = 0;
  KKStrMatrixPtr results = dbConn->QueryStatementReturnAllColumns (sqlStr, sqlStrLen, columnNames);
  if  (!results)
  {
    log.Level (-1) << endl 
      << "DataBaseUpdate::LoadSourceFile  ***ERROR***   Error occured executing sql string." << endl
      << dbConn->LastErrorDesc () << endl
      << endl;

    if  (errorMsgs)
    {
      errorMsgs->PushOnBack (new KKStr ("Error executing MySql String."));
      errorMsgs->PushOnBack (new KKStr (dbConn->LastErrorDesc ()));
    }
    successful = false;
    return;
  }

  delete  results;  results = NULL;
  delete  sqlStr;   sqlStr = NULL;
  fclose (f);       f = NULL;
}  /* LoadSourceFile */

