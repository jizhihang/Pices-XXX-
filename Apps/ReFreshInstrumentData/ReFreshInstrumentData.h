#ifndef  _REFRESHINSTRUMENTDATA_
#define  _REFRESHINSTRUMENTDATA_

#include "Application.h"
#include "DataBase.h"
#include "FeatureFileIO.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "RandomNumGenerator.h"
#include "RunLog.h"
#include "SipperFile.h"
#include "Str.h"




class  ReFreshInstrumentData: public Application
{
public:
  ReFreshInstrumentData (int     argc, 
                         char**  argv
                        );

  ~ReFreshInstrumentData ();

  void  Main ();

private:
  void   DisplayCommandLineParameters ();

  bool   ProcessCmdLineParameter (char    parmSwitchCode, 
                                  KKStr   parmSwitch, 
                                  KKStr   parmValue
                                 );

  DataBasePtr  dbConn;

  KKStr    cruiseName;
  KKStr    deploymentNum;
  KKStr    stationName;

  KKStr    reportFileName;
};
#endif
