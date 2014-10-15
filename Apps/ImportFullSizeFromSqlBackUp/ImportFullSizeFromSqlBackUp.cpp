#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <memory>
#include  <math.h>

#include  <map>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;

#include  "Compressor.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;

#include  "DataBase.h"
using namespace MLL;


#include "ImportFullSizeFromSqlBackUp.h"


ImportFullSizeFromSqlBackUp::ImportFullSizeFromSqlBackUp (int     argc, 
                                                          char**  argv
                                                         ):

   Application (argc, argv),
   srcFileName ()
{
  if  (argc < 2)
  {
    log.Level (-1) << endl << endl 
                   << "No Command Line Parameters were provided." << endl
                   << endl;

    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  ProcessCmdLineParameters (argc, argv);
  if  (Abort ())
  {
    DisplayCommandLineParameters ();
    return;
  }

  if  (srcFileName.Len () < 1)
  {
    log.Level (-1) << endl << endl 
                   << "ImportFullSizeFromSqlBackUp    ***ERROR***   you must provide at least the name of the SQL backup file." << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }
}





ImportFullSizeFromSqlBackUp::~ImportFullSizeFromSqlBackUp ()
{
}




bool  ImportFullSizeFromSqlBackUp::ProcessCmdLineParameter (
                                                            char    parmSwitchCode, 
                                                            KKStr   parmSwitch, 
                                                            KKStr   parmValue
                                                           )
{
  KKStr  parmValueUpper (parmValue);
  parmValueUpper.Upper ();

  parmSwitch.Upper ();

  if  ((parmSwitch == "-S")    ||  
       (parmSwitch == "-SRC")  ||  
       (parmSwitch == "-SOURCE")  ||
       (parmSwitch.EqualIgnoreCase ("-SrcFile"))
       )
  {
    srcFileName = parmValue;
    if  (!osFileExists (srcFileName))
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    Invalid '-SrcFile' [" << srcFileName << "] file." << endl;
      Abort (true);
    }
  }

	return  !Abort ();
}  /* ProcessCmdLineParameter */




/** ****************************************************************************
 * DisplayCommandLineParameters()
 * DESC: Displays a help message to the user
 ******************************************************************************/
void   ImportFullSizeFromSqlBackUp::DisplayCommandLineParameters ()
{
  log.Level (0) << "ImportFullSizeFromSqlBackUp"                                                     << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "    -SrcFile  <Source File>  Specify the name of the file where the Full Size "  << endl;
  log.Level (0) << "              Images are to be restored from.  This is the SQL file that you "   << endl;
  log.Level (0) << "              have backed up to."                                                << endl;
  log.Level (0)                                                                                      << endl;
}  /* DisplayCommandLineParameters */





void  ImportFullSizeFromSqlBackUp::Main ()
{
  DataBasePtr  dbConn = new DataBase (log);
  ifstream  i (srcFileName.Str ());

  int buffLen = 20 * 1024 * 1024;
  char*  buff = new char[buffLen];

  for  (int y = 0;  y < 28;  y++)
    i.getline (buff, buffLen);

  while  (i.getline (buff, buffLen))
  {
    char buffShort[32];
    for  (int x = 0;  x < sizeof (buffShort);  x++)
      buffShort[x] = buff[x];
    buffShort[sizeof (buffShort) - 1] = 0;
    cout << buffShort << endl;

    if  ((buff[0] == 'I')  &&  
         (buff[1] == 'N')  &&
         (buff[2] == 'S')  &&
         (buff[3] == 'E')  &&
         (buff[4] == 'R')
        )
    {
      int  buffStrLen = strlen (buff);
      dbConn->QueryStatement2 (buff, buffStrLen);
    }
  }

}  /* ImportFullSizeImages */



int  main (int     argc,
           char**  argv
          )
{

  ImportFullSizeFromSqlBackUp  mergeFeatureFiles (argc, argv);
  if  (mergeFeatureFiles.Abort ())
    return 1;

  mergeFeatureFiles.Main ();
  if  (mergeFeatureFiles.Abort ())
    return 1;
  else
    return 0;
}
