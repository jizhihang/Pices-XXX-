#pragma once


#include "Application.h"

#include "DataBase.h"


class ExtractAllSipperFiles: public Application
{
public:
  typedef  ExtractAllSipperFiles*  ExtractAllSipperFilesPtr;

  ExtractAllSipperFiles (int      argc, 
                         char**   argv,
                         RunLog&  _log
                        );


  ~ExtractAllSipperFiles ();


  void  Run ();


private:
  // Will be classed by base class  'Application'  
  bool  ProcessCmdLineParameter (char    parmSwitchCode, 
                                 KKStr   parmSwitch, 
                                 KKStr   parmValue
                                );

  void  DisplayCommandLineParameters ();



  void  ProcessADir (VectorKKStr&  list,
                     KKStr         dirName
                    );

  void  RunOneImageExtraction (const KKStr&  sfn);


  KKStr             configFileName;
  MLL::DataBasePtr  dbConn;
  bool              firstOneFound;
  int               minSize;
  int               maxSize;
  KKStr             rootDir;
  KKStr             startSipperFileName;
};


typedef  ExtractAllSipperFiles::ExtractAllSipperFilesPtr  ExtractAllSipperFilesPtr;
