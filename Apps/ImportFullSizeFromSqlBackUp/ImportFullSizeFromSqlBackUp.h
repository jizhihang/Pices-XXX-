#ifndef  _IMPORTFULLSIZEFROMSQLBACKUP_
#define  _IMPORTFULLSIZEFROMSQLBACKUP_

#include "Application.h"
#include "DataBase.h"
#include "RunLog.h"


class  ImportFullSizeFromSqlBackUp: public Application
{
public:

  ImportFullSizeFromSqlBackUp (int     argc, 
                     char**  argv
                    );

  ~ImportFullSizeFromSqlBackUp ();

  void  Main ();

private:
  void   DisplayCommandLineParameters ();

  bool   ProcessCmdLineParameter (char    parmSwitchCode, 
                                  KKStr   parmSwitch, 
                                  KKStr   parmValue
                                 );

  KKStr  srcFileName;

};
#endif
