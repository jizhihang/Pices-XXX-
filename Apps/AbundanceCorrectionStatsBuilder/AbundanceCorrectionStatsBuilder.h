#if  !defined(_ABUNDANCECORRECTIONSTATSBUILDER_)
#define  _ABUNDANCECORRECTIONSTATSBUILDER_

#include "Application.h"
#include "MsgQueue.h"
#include "RunLog.h"
#include "Str.h"

#include "AbundanceCorrectionMatrix.h"
#include "FeatureFileIO.h"
#include "FeatureVector.h"
#include "FileDesc.h"
#include "ImageFeatures.h"
#include "MLClass.h"
#include "NormalizationParms.h"
#include "TrainingConfiguration2.h"

namespace  AbundanceCorrectionApplication
{

  #if  !defined(_TRAINTESTTHREAD_)
  class  TrainTestThread;
  typedef  TrainTestThread*          TrainTestThreadPtr;
  typedef  KKQueue<TrainTestThread>  TrainTestThreadList;
  typedef  TrainTestThreadList*      TrainTestThreadListPtr;
  #endif

  class  AbundanceCorrectionStatsBuilder: public Application
  {
  public:
    typedef  KKU::uchar  uchar;
    typedef  KKU::uint   uint;
    typedef  KKU::ulong  ulong;
    typedef  KKU::ushort ushort;

    AbundanceCorrectionStatsBuilder (int     argc, 
                                     char**  argv
                                    );

    ~AbundanceCorrectionStatsBuilder ();

    void  Main ();

  private:
    void   CreateInitialThreadInstaces ();

    void   DisplayCommandLineParameters ();

    void   GenerateCrashReport ();

    void   GenerateReportAndStats ();

    void   ManageThreads ();

    void   PrintComandLineParameters ();

    void   PrintStartStatistics ();

    bool   ProcessCmdLineParameter (char    parmSwitchCode, 
                                    KKStr   parmSwitch, 
                                    KKStr   parmValue
                                   );

    void   RemoveDuplicateImages ();

    void   TerminateThreads ();


    MLClassConstListPtr        allClasses;           /**< Contains  trainLibDataClasses + otherClass               */

    TrainingConfiguration2Ptr     config;

    MLClassConstListPtr        configClasses;

    KKStr                         configFileName;

    KKStr                         configFileFullPath;   /**< Will include the full path to config file that is used.  */

    FileDescPtr                   fileDesc;

    int                           maxNumActiveThreads;

    MsgQueuePtr                   msgQueue;

    NormalizationParmsPtr         normalizationParms;

    int                           numOfFolds;

    bool                          oneOrMoreThreadsCrashed;

    MLClassConstPtr            otherClass;

    FeatureVectorListPtr          otherClassData;

    TrainTestThreadListPtr        queueDone;

    TrainTestThreadListPtr        queueReady;

    TrainTestThreadListPtr        queueRunning;

    KKStr                         reportFileName;

    ostream*                      report;

    FeatureVectorListPtr          trainLibData;         /**< All the examples that were retrived from the configuration file. */

    MLClassConstListPtr        trainLibDataClasses;
  };  /* AbundanceCorrectionStatsBuilder */

  typedef  AbundanceCorrectionStatsBuilder*  AbundanceCorrectionStatsBuilderPtr;
}
#endif
