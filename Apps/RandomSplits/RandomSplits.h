#if  !defined(_RANDOMSPLITS_)
#define  _RANDOMSPLITS_


#include  "Application.h"
#include  "FeatureVector.h"
#include  "JobManager.h"
#include  "Orderings.h"


using namespace  KKU;

class RandomSplits: public Application
{
public:
  typedef  RandomSplits*  RandomSplitsPtr;

  RandomSplits (int      argc, 
                char**   argv,
                RunLog&  _log
               );


  ~RandomSplits ();


  void  Run ();


private:
  // Will be classed by base class  'Application'  
  bool  ProcessCmdLineParameter (char    parmSwitchCode, 
                                 KKStr   parmSwitch, 
                                 KKStr   parmValue
                                );

  void  DisplayCommandLineParameters ();


  KKStr              configFileName;
  KKStr              dataFileName;
  FeatureFileIOPtr   format;
  int                numSplits;
  int                numFolds;
  bool               restart;
  float              splitFraction;
};

typedef  RandomSplits::RandomSplitsPtr  RandomSplitsPtr;

#define  _RandomSplitsDefined_

#endif


