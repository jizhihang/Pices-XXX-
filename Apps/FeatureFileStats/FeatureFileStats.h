#ifndef  _FEATUREFILESTATS_
#define  _FEATUREFILESTATS_

#include  "Application.h"
#include  "RunLog.h"
#include  "Str.h"

#include  "FileDesc.h"
#include  "MLClass.h"

#ifndef  _IMAGEFEATURES_
class    ImageFeatures;
typedef  ImageFeatures*  ImageFeaturesPtr;
class    FeatureVectorList;
typedef  FeatureVectorList*  FeatureVectorListPtr;
#endif



class  FeatureFileStats: public Application
{
public:

	FeatureFileStats (int     argc, 
                    char**  argv
                   );

	~FeatureFileStats();

	virtual const char* ApplicationName ()  {return  "FeatureFileStats";}

  void  ReportStats ();

	void	DisplayCommandLineParameters ();

	virtual bool ProcessCmdLineParameter (
                                        char    parmSwitchCode, 
                                        KKStr  parmSwitch, 
                                        KKStr  parmValue
                                       );
private:
  bool                  cancelFlag;
  FeatureVectorListPtr  data;

  FileDescPtr           fileDesc;

  MLClassConstList   mlClasses;

	ostream*              report;
	ofstream*             reportFile;
	KKStr                 reportFileName;

  KKStr                 srcFileName;
  FeatureFileIOPtr      srcFileFormat;
};

#endif
