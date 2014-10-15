#ifndef  _DUMPSIPPER3INSTRUMENTDATA_
#define  _DUMPSIPPER3INSTRUMENTDATA_


#include "Application.h"



class DumpSipper3InstrumentData:  public Application
{
public:
  DumpSipper3InstrumentData (int     argc,
                             char**  argv
                            );

  ~DumpSipper3InstrumentData ();


  virtual const 
  char*   ApplicationName ()  {return  "DumpSipper3InstrumentData";}



  void  ProcessSipperFile ();


private:

  void  DisplayCmdLine ();



  uchar  GetNextSipperRec (bool&  moreRecs);

  void  OpenSipperFile ();


  virtual 
  bool   ProcessCmdLineParameter (char    parmSwitchCode, 
                                  KKStr  parmSwitch, 
                                  KKStr  parmValue
                                 );


  int        byteOffset;
  KKStr     reportFileName;
  ofstream*  report;
  FILE*      sipperFile;
  KKStr     sipperFileName;
};


typedef  DumpSipper3InstrumentData*  DumpSipper3InstrumentDataPtr;


#endif