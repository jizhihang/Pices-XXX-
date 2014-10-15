#ifndef  _RIPOUTCLASS_
#define  _RIPOUTCLASS_

#include "Application.h"
#include "FeatureVector.h"
#include "FileDesc.h"
#include "MLClass.h"


class  RipOutClass: public Application
{
public:
	RipOutClass (int argc, char**  argv);
	~RipOutClass ();

	virtual const char* ApplicationName ()  {return  "RipOutClass";}

	void	DisplayCommandLineParameters ();

  void  ExtractSpecifiedClass ();
  
  virtual bool ProcessCmdLineParameter (
											char    parmSwitchCode, 
											KKStr   parmSwitch, 
											KKStr   parmValue
										  );

private:
  bool                  cancelFlag;
  MLClassConstList   classesToRipOut;
	KKStr                 destFileName;
  FeatureVectorListPtr  destImages;
  FileDescPtr           fileDesc;
	MLClassConstList   mlClasses;
  FeatureFileIOPtr      inFileFormat;
  int                   numOfFolds;
  FeatureFileIOPtr      outFileFormat;
  bool                  stratify;
	KKStr                 srcFileName;
  FeatureVectorListPtr  srcImages;
  bool                  successful;
};

#endif