#ifndef  _ACTIVELEARNING_
#define  _ACTIVELEARNING_
//*************************************************************************************************
//*                                        Activelearning                                         *
//*                                                                                               *
//*                                                                                               *
//*                                                                                               *
//*                                                                                               *
//*                                                                                               *
//*                                                                                               *
//*-----------------------------------------------------------------------------------------------*
//*                                                                                               *
//*                                                                                               *
//*                                                                                               *
//*                                                                                               *
//* 2004-12-11  Kurt Kramer  Modify constructor that was used for report file that does not       *
//*                          appear to be in use to load parameters from the status file.         *
//*                          This way the two report apps ActiveLearningCombinedresults  and      *
//*                          ActiveLearningSuarizeResults can get the ActiveLlearning parameters  *
//*                          from the status file.                                                *
//*                                                                                               *
//* 2005-01-18  Kurt Kramer  Record Image File Name when selcting an image for addition to the    *
//*                          training library.                                                    *
//*                                                                                               *
//* 2005-02-05  Kurt Kramer  Added UsePriorForIIPC,   When this flad is usd meens that the IIPC   *
//*                          for each class should be weighted by the prior info in the pool of   *
//*                          samples.                                                             *
//*************************************************************************************************
#include "Str.h"
#include "Application.h"
using namespace KKU;

#ifndef  _CLASSGROUPTOTALS_
class  ClassGroupTotals;
typedef  ClassGroupTotals*  ClassGroupTotalsPtr;
#endif

#include "Classifier2.h"
#include "ConfusionMatrix2.h"
#include "FeatureFileIO.h"
#include "FileDesc.h"
#include "ImageFeatures.h"
#include "MLClass.h"
#include "TrainingConfiguration2.h"


using namespace MLL;


#include "ClassGroupTotals.h"
#include "SortOrderType.h"
#include "PassAssignments.h"

#ifndef  _JOBDESC_
class  JobDesc;
typedef  JobDesc*  JobDescPtr;
class  JobDescList;
typedef  JobDescList*  JobDescListPtr;
#endif



class  ImageGroupList;
typedef  ImageGroupList*  ImageGroupListPtr;





class  ActiveLearning: public  Application
{
public:
  ActiveLearning (int     argc,
                  char**  argv
                 );
  
                                       // 2004-12-11   KK
  ActiveLearning (KKStr  _rootDir);   // When you want to only load parameters 
                                       // from the status file.

  ~ActiveLearning ();



  virtual const 
  char*          ApplicationName ()  {return  "ActiveLearning";}

  void           ClassifyImage (ImageFeaturesPtr  image);

  void           DisplayCommandLineParameters ();

  void           ProcessAllJobs ();

  virtual 
  bool           ProcessCmdLineParameter (char    parmSwitchCode, 
                                          KKStr  parmSwitch, 
                                          KKStr  parmValue
                                         );

  FeatureFileIOPtr       FileFormats       ()  {return  fileFormat;}

  ImageClassListPtr      ImageClasses      ()  {return  imageClasses;}

  vector<int>&           IIPCTable         ()  {return  iipcTable;}

  vector<int>&           IPRTable          ()  {return  iprTable;}

  FeatureVectorListPtr   MasterTestImages  ()  {return  masterTestImages;}

  int                    NumOfRandomPasses ()  {return  numOfRandomPasses;}

  const
  KKStr&                 RootDir           ()  {return  rootDir;}

  FeatureVectorListPtr   ValidationImages  ()  {return  validationImages;}



private:
  KKStr              DeriveIIPCsubDirectory     (int  iipc);

  KKStr              DeriveIPRsubSirectory (int  iipc,
                                             int  ipr
                                            );

  KKStr              DeriveResultsFileName (SortOrderType  sortOrder,
                                             int            pass
                                            );

  KKStr              DeriveFullResultsFileName (SortOrderType  sortOrder, 
                                                 int            iipc, 
                                                 int            ipr, 
                                                 int            pass
                                                );

  void  ExtractValidationImages (MLClassList&     classes,
                                 FeatureVectorList&  srcImages,
                                 FeatureVectorList&  destImages,
                                 FeatureVectorList&  validationImages
                                );

  PassAssignmentsPtr    GetAssignments (int  iipc);



  FeatureVectorListPtr  LoadImages (KKStr                dirPath,
                                    FeatureVectorListPtr trainingImages
                                   );


  void  ProcessASingleRetraining (SortOrderType        sortOrder,
                                  double               probabilityTest,
                                  MLClassList&      classesInGroup,
                                  Classifier2Ptr       classifier, 
                                  FeatureVectorList&   trainingImages,
                                  FeatureVectorList&   testImages,
                                  FeatureVectorList&   validationImages,
                                  int                  retrainingPass,
                                  ClassGroupTotals&    stats,
                                  int                  pass,
                                  int                  iipc,
                                  int                  ipr,
                                  float&               testingTime,
                                  FeatureVectorListPtr nonStreamingTestImages
                                 );


  void  ProcessOneImageAtAtimeOneSpecificPass (SortOrderType       sortOrder,
                                               double              probabilityTest,
                                               MLClassList&     groupClasses,           // List of Classes that we want to processs for.
                                               ClassGroupTotals&   retrainingStats,
                                               int                 pass,
                                               int                 iipc,
                                               int                 ipr,
                                               int                 numOfRetrainings
                                              );


  void  SortImageFeatures (FeatureVectorList&  images,
                           SortOrderType       sortOrder
                          );


  void  UpdateNonStreamingTestImages (SortOrderType       sortOrder,
                                      Classifier2Ptr      classifier, 
                                      FeatureVectorList&  nonStreamingTestImages
                                     );
  


  
  //***************************************************/
  //*          Job Management Routines                */
  //***************************************************/

  KKStr      CmdLineStr ();

  void        Block ();

  void        CloseReportFile ();

  void        CreateInitialImages ();

  void        DetermineIfOnANetworkDrive ();

  void        EndBlock ();

  void        FinishJob (JobDescPtr  j);

  JobDescPtr  GetNextJob ();

  float       GetPertinentProbability (ImageFeaturesPtr  image,
                                       SortOrderType     sortOrder
                                      );


  void        InitializeResultFile (SortOrderType  sortOrder,
                                    int            pass,
                                    int            iipc,
                                    int            ipr
                                   );

  void        LoadCurrentStatusFile ();

  void        LoadInitialImages ();

  void        OpenReportFile ();

  FILE*       OpenStatusFile ();

  void        ProcessCmdLineStr  (KKStr  cmdLine);

  void        ProcessCombinedClassJob (SortOrderType  sortOrder,
                                       int            pass,
                                       int            iipc,
                                       int            ipr
                                      );

  void        ProcessStatusFile (int     argc,
                                 char**  argv
                                );

                                      // 2004-12-11  KK
  void        ProcessStatusFile ();   // When no parameters provided,  caller just wants to load
                                      // the run time parms from an existing StatusFile.


  void        UpdateCurrentStatusFile ();


  void        WriteResultsFileViaLocalDrive (ClassGroupTotals&  results, 
                                             KKStr             resultsFileName,
                                             KKStr             fullResultsFileName
                                            );



  FileDescPtr               fileDesc;

  int                       ourProcessNum;

  int                       numOfProcesses;

  int                       numOfJobs;

  JobDescListPtr            jobs; 

  int                       lockFile;
  bool                      lockFileOpened;

  bool                      weAreUsingANetworkDrive; //  if True then we want to write result files
                                                     //  to a local drive,  then copy then to the
                                                     //  network.

  KKStr                    baseResultsFileName;
  KKStr                    configFileName;
  KKStr                    dataFileSaveDir;
  KKStr                    testImagesRootDir;
  KKStr                    trainingImagesRootDir;

  KKStr                    masterTestImagesFileName;
  KKStr                    validationImagesFileName;
  FeatureFileIOPtr         fileFormat;
  KKStr                    initTrainImagesAssignmentFileName;

  ImageClassListPtr         imageClasses;

  FeatureVectorListPtr      masterTestImages; 
  FeatureVectorListPtr      validationImages; 

  int                       finalNumOfImagesInTrainingLibrary;
  int                       numOfRandomPasses;
  int                       validationImagesPerClass;

  vector<int>               iipcTable;
  vector<int>               iprTable;

  bool                      usePriorForIIPC;      // If true,  means that we want the initial traning 
                                                  // libraries to reflect the prior clasa info in 
                                                  // the over all pool of data.

  bool                      performStreaming;
  int                       streamingBlockSize;
  int                       retrainingsPerStreamBlock;
  bool                      testRelativeNonStreamingPosition;

  TrainingConfiguration2Ptr masterTrainingConfiguration;

  ofstream*                 classProbReport;
  ofstream*                 report;
  KKStr                     reportFileName;

  KKStr                     rootDir;  
};


typedef  ActiveLearning*  ActiveLearningPtr;


#endif
