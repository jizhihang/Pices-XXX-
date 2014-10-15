#ifndef  _MERGEFEATUREFILES_
#define  _MERGEFEATUREFILES_

#include "Application.h"
#include "DataBase.h"
#include "FeatureFileIO.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "RandomNumGenerator.h"
#include "RunLog.h"
#include "SipperFile.h"
#include "Str.h"




class  MergeFeatureFiles: public Application
{
public:
  typedef  KKU::uchar  uchar;
  typedef  KKU::uint   uint;
  typedef  KKU::ulong  ulong;
  typedef  KKU::ushort ushort;

  MergeFeatureFiles (int     argc, 
                     char**  argv
                    );

  ~MergeFeatureFiles ();

  void  Main ();

private:
  void   DisplayCommandLineParameters ();

  bool   ProcessCmdLineParameter (char    parmSwitchCode, 
                                  KKStr   parmSwitch, 
                                  KKStr   parmValue
                                 );

  FeatureFileIOPtr           currentDefaultFormat;    // Will change as each '-format' option is processed.

  KKStr                      destFileName;
  FeatureFileIOPtr           destFormat;

  int                        numOfFolds;

  bool                       randomize;

  VectorKKStr                srcFileNames;
  vector<FeatureFileIOPtr>   srcFormats;

  FeatureVectorListPtr       srcData;

  bool                       stratify;


};
#endif
