#ifndef  _FEATUREFILECONVERTER_
#define  _FEATUREFILECONVERTER_

#include  "Application.h"
#include  "RunLog.h"
#include  "Str.h"

#include "FeatureNumList.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "SVMparam.h"


class  FeatureFileConverter: public Application
{
public:
  typedef  KKU::uint  uint;

	FeatureFileConverter (int     argc, 
                        char**  argv
                       );

	~FeatureFileConverter();

	virtual const char* ApplicationName ()  {return  "FeatureFileConverter";}

  void  ConvertData ();

	void	DisplayCommandLineParameters ();

	virtual bool ProcessCmdLineParameter (char    parmSwitchCode, 
                                        KKStr  parmSwitch, 
                                        KKStr  parmValue
                                       );
private:
  void  EncodeFeatureData ();

  void  NormalizeExamples (ModelParam&         param,
                           FeatureVectorList&  examples
                          );

  bool                  cancelFlag;

  FeatureVectorListPtr  data;

  FileDescPtr           destFileDesc;

  FeatureFileIOPtr      destFileFormat;
  KKStr                 destFileName;

  bool                             encodeFeatureData;
  ModelParam::EncodingMethodType   encodingMethod;

  bool                             enumerateClasses;

  FeatureNumListPtr     features;    // When equial NULL means All features.
  KKStr                 featureStr;  

  MLClassConstList   mlClasses;

  bool                  normalizeData;
  KKStr                 nornParmsFileName;

  ostream*              report;
  ofstream*             reportFile;
  KKStr                 reportFileName;

  FileDescPtr           srcFileDesc;

  KKStr                 srcFileName;
  FeatureFileIOPtr      srcFileFormat;

  bool                  statistics;
};

#endif
