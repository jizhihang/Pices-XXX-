#include  "FirstIncludes.h"

#include  <stdio.h>

#include  <ctype.h>
#include  <time.h>

#include  <fcntl.h>

#include  <string>
#include  <iomanip>
#include  <iostream>
#include  <fstream>
#include  <vector>

#ifdef  WIN32
#include  <ostream>
#endif

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;


#include <sys/types.h>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif

#include  "MemoryDebug.h"

#include  "OSservices.h"

#include  "BeamSearchSVM.h"
#include  "FeatureFileIO.h"

#include  "RunLog.h"
#include  "NormalizationParms.h"
#include  "BeamSearchSVM.h"



// 2004-10-03
// Searcgh Nine Class Model, for Thesis,  
// -el 150  -SEARCHTYPE BESTFIRST -algorithm SVM  -L 1  -c 12  -g 0.01507   -s 0  -t 2  -u 100 -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56  -up  -sm P  -MT ONEVSONE  -v 5 NineClasses_ValidationData.data



// 2004-10-01
// Searcgh Nine Class Model, for Thesism Using New 16 Contour features
// -el 150  -SEARCHTYPE BESTFIRST -algorithm SVM  -L 1  -c 12  -g 0.01507   -s 0  -t 2  -u 100 -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72  -up  -sm P  -MT ONEVSONE  -v 5 NineClasses_ValidationData.data


//  -el 150  -SEARCHTYPE BESTFIRST  -L 5  -a 1  -b 100  -f 10  -s 0  -t 2  -d 3  -g 0.032  -r 0  -n 0.5  -m 40  -c 300  -e 0.001  -p 0.1 -h 1  -v 5  -z October24BestCaseOut.txt  TEST_OCT_24_80p.data




// -X  5,6,10,14,15,17,21,22,24,27,28  -L 1  -a 1  -b 100  -f 10  -s 1  -t 2  -d 3.0  -g 0.010  -r 0.0  -n 0.20  -m 40.0  -c 1.0  -e 0.0010  -p 0.10  -h 1  -v 10  -z BestCaseOut.txt  TEST_OCT_24_80p.data

// -I 0,1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,19,20,22,23,24,25,26,28   -L 1  -a 1  -b 100  -f 10  -s 1  -t 2  -d 3.0  -g 0.010  -r 0.0  -n 0.20  -m 40.0  -c 1.0  -e 0.0010  -p 0.10  -h 1  -v 5  -z K:\Plankton\StatusReports\2003_Mar_18\NextBestBack_26.txt  TEST_OCT_24_80p.data  



// BeamSearchSVM   -SEARCHTYPE BESTFIRST  -el 150  -s 0 -n 0.11 -t 2 -g 0.04096 -c 16 -u 100  -v 5 -l 1 -z  K:\Plankton\TempBeamSearch\Jan_18_2004\All5Classes.txt    K:\Plankton\TempBeamSearch\Jan_18_2004\BeamSearch_TestData.data


//  -SEARCHTYPE BESTFIRST  -ALGORITHM  c45  -el 150  -v 5 -l 1 -z  TEST_OCT_24_80p_NeuroC45.txt  K:\Plankton\TempBeamSearch\BestCase\TEST_OCT_24_80p.data


// BeamSearchSVM  -el 150  -SEARCHTYPE BEAM  -L 5  -a 1  -b 100  -f 10  -s 0  -t 2  -d 3  -g 0.032  -r 0  -n 0.11  -m 40  -c 300  -e 0.001  -p 0.1  -h 1  -v 5     -z GrayScaleCpp_New_Report.txt  K:\\Plankton\\TempBeamSearch\\GrayScale_May_13_03\\GrayScaleCpp_2003-May-13.data


// BeamSearchSVM  -v 10  -L 5  -a 1  -b 100  -f 10  -s 0  -t 2  -d 3  -g 0.032  -r 0  -n 0.11  -m 40  -c 300  -e 0.001  -p 0.1  -h 1  -i 1,9,17,18,19,21,28,29,32,35,40,41,42,45,46 -z For_15_Features.txt  GrayScaleCpp_April_19_03.data
// BeamSearchSVM  -v 10  -L 5  -a 1  -b 100  -s 0  -t 2  -d 3  -g 0.032  -r 0  -n 0.5  -m 40  -c 200  -e 0.001  -p 0.1  -h 1   -i 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28 -z For_15_Features.txt  K:\v1\Plankton\TempBeamSearch\Test_Oct_24.data

//  -SEARCHTYPE BESTFIRST -BINARY -el 150  -s 0 -n 0.11 -t 2 -g 0.04096 -c 16 -u 100  -v 5 -l 1   2004_03_02_TestData.data


//  Parameters 4 Grayscale Paper,  July-28-2004
//  -SEARCHTYPE BESTFIRST  -el 150 -s 1 -n 0.11 -t 2 -g 0.0496 -c 16 -u 100 -v 5  -l 1 -z  GrayscaleFeatureSelection_49.txt ActiveLearning_ValidationImages.data


//  July-29-2004
//  Created cmd str below because thw one from the 28th was not working
//  Modified  2004-07-30,  removed features 25,26
//  -SEARCHTYPE BESTFIRST -el 150  -s 0 -n 0.11 -t 2 -g 0.04096 -c 16 -v 5 -l 1 -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48  ActiveLearning_ValidationImages.data
//  from  FiveBigest  "Parameters=-s 0 -n 0.11 -t 2 -g 0.04096  -c 16  -u 100  -up"


// Copied in from old beam search run
//  BeamSearchSVM  -el 150  -SEARCHTYPE BESTFIRST                 -L 1  -a 1  -b 100  -f 10  -s 0  -t 2  -d 3  -g 0.032     -r 0  -n 0.11  -m 40  -c 300  -e 0.001  -p 0.1  -h 1  -v 5     -z GrayScaleCpp_New_Report.txt  K:\\Plankton\\TempBeamSearch\\GrayScale_May_13_03\\GrayScaleCpp_2003-May-13.data

//  from the Status file we generated where the runs do not work very well.
//                 -el 150  -SEARCHTYPE BESTFIRST -algorithm SVM  -L 1  -a 1  -b 100         -s 0  -t 2  -d 3  -g 0.04096   -r 0  -n 0.11  -m 40  -c  16  -e 0.001  -p 0.1  -h 1  -v 5     -j 0  -k 0   -q 0  -u 100  -MT ONEVSONE  ActiveLearning_ValidationImages.data




//  2005-01-04
// -binary  -el 150  -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56  -SEARCHTYPE BESTFIRST -algorithm SVM  -L 1  -a 1  -b 100  -c 12  -d 3  -e 0.001  -g 0.01507  -h 1  -j 0  -k 0  -l 1  -m 40  -n 0.5  -p 0.1  -q 0  -r 0  -s 0  -t 2  -u 100  -MT ONEVSONE  -SM Probability  -UseProbability  -v 5  NineClasses_TestData.data

//  2005-03-16
// -el 150  -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56  -SEARCHTYPE BESTFIRST -algorithm SVM  -L 1  -a 1  -b 100  -c 16  -d 3  -e 0.001  -g 0.02048  -h 1  -j 0  -k 0  -l 1  -m 40  -n 0.5  -p 0.1  -q 0  -r 0  -s 0  -t 2  -u 100  -MT ONEVSONE  -SM Probability  -UseProbability  -v 5 OnePercentModelTestData.data

//  2005-03-17   Start off with beam from 
// -el 150  -InitialFeatures 0,9,16,19,20,21,22,23,24,26,28,29,30,31,32,39,40,41,42,43,44,45,46,48,50,51,52,53,54,55,56  -SEARCHTYPE Beam -algorithm SVM  -WIDTH 5  -a 1  -b 100  -c 16  -d 3  -e 0.001  -g 0.02048  -h 1  -j 0  -k 0  -m 40  -n 0.5  -p 0.1  -q 0  -r 0  -s 0  -t 2  -u 100  -MT ONEVSONE  -SM Probability  -UseProbability  -v 5 OnePercentModelTestData.data

//  2005-03-20   Start off with beam from 
// -el 150  -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56  -SEARCHTYPE BESTFIRST -algorithm SVM  -L 1  -a 1  -b 100  -c 16  -d 3  -e 0.001  -g 0.02048  -h 1  -j 0  -k 0  -l 1  -m 40  -n 0.5  -p 0.1  -q 0  -r 0  -s 0  -t 2  -u 100  -MT ONEVSONE  -SM Probability  -UseProbability  -v 5 OnePercentModelTestData_TrainingData.data

//  2005-03-24   Start off with beam from 
// K:\v1\Plankton\BeamSearches\OnePercentModel_500\Binary
//  -BINARY   -el 30  -SEARCHTYPE BESTFIRST -algorithm SVM  -L 1  -a 1  -b 100  -c 16  -d 3  -e 0.001  -g 0.02048  -h 1  -j 0  -k 0  -l 1  -m 40  -n 0.5  -p 0.1  -q 0  -r 0  -s 0  -t 2  -u 100  -MT ONEVSONE  -SM Probability  -UseProbability  -v 5  OnePercentModelTestData_TrainingData.data  


// 2005-05-13  
// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures_NewSearch
// -el 150  -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56  -SEARCHTYPE BESTFIRST -algorithm SVM  -L 1  -a 1    -c 12  -d 3  -e 0.001  -g 0.01507  -h 1  -j 0  -k 0  -l 1  -m 40  -n 0.5  -p 0.1  -q 0  -r 0  -s 0  -t 2  -u 100  -MT ONEVSONE  -SM Probability  -UseProbability  -v 5  NineClasses_ValidationData.data


// 2005-06-05      Beam search on data set going out
// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures_BestFirstOut
// -el 150  -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56  -SEARCHTYPE BESTFIRSTOUT -algorithm SVM  -L 1  -a 1    -c 12  -d 3  -e 0.001  -g 0.01507  -h 1  -j 0  -k 0  -l 1  -m 40  -n 0.5  -p 0.1  -q 0  -r 0  -s 0  -t 2  -u 100  -MT ONEVSONE  -SM Probability  -UseProbability  -v 5  NineClasses_ValidationData.data

// 2006-03-14
// Y:\FeatureSelection\2006-03-14
// -el 150  -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48  -SEARCHTYPE BestFirst  -algorithm SVM   -v 5  -dataFile ActiveLearning_ValidationImages_shuffled.data  -config FiveBigest.cfg

// 2006-03-28
// Y:\FeatureSelection\2006-03-28
// -el 150  -InitialFeatures 0-56  -SEARCHTYPE BestFirst  -algorithm SVM   -v 5  -dataFile NineClasses_ValidationData.data  -config Thesis9Classes.cfg -validate NineClasses_TestData.data

// 2006-03-30
// C:\bigpine\Plankton\Papers\FastFeatureSelAlgorithm\WrappersFeatureSearch
// -el 150  -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48  -SEARCHTYPE BestFirst  -algorithm SVM   -v 5  -dataFile FiveClassPaper_Train.data  -config FiveBigest.cfg

// 2006-05-25
// C:\Documents and Settings\kkramer\My Documents\Plankton\Papers\FastFeatureSelAlgorithm\Wrapper1
// -el 150  -InitialFeatures 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48  -SEARCHTYPE BestFirst  -algorithm SVM   -v 5  -dataFile ..\FiveClassPaper_FeatureSelection1.data  -config ..\FiveBigest.cfg

// C:\bigpine\LI\Data\Fold2
// -el 2000 -SEARCHTYPE BestFirst  -v 10 -FileFormat Col -dataFile ..\traindata2  -config ..\LI.cfg

// C:\bigpine\LI\Data\TestData
// -el 20 -SEARCHTYPE BestFirst  -v 10 -FileFormat Sparse -dataFile ..\colon.train_TestData.data  -config ..\LI.cfg  -batch  -InitialFeatures 1-10,1990-1999

// -el 200 -SEARCHTYPE BestFirst  -v 10 -FileFormat Sparse -dataFile ..\colon.train_TestData.data  -config ..\LI.cfg  -batch 

// C:\bigpine\LI\Data\TestData
// -el 20 -SEARCHTYPE BestFirst  -v 10 -FileFormat Col  -dataFile ..\traindata1  -config ..\LI.cfg  -batch 



// 
// -el 150  -InitialFeatures 0-59,62-73 -SEARCHTYPE BestFirst  -algorithm SVM   -v 5  -dataFile C:\Temp\FeatureSearch_BinaryClasses\etpB_training.data    -config ..\FiveBigest.cfg

const char*  lockFileName       = "BeamSearchSVM.lock";
const char*  statusFileNameOpen = "BeamSearchSVM_Open.status";
const char*  statusFileNameDone = "BeamSearchSVM_Done.status";


SearchMethod  searchMethod;


BeamSearchSVM::BeamSearchSVM ():
  algorithm              (SVM),
  batchSize              (50),
  beamSize               (5),
  bestResultThisNode     (0),
  cancelFlag             (false),
  config                 (NULL),
  configFileName         (),
  doneJobs               (NULL),
  expansionsSinceHighestAccuracy (0),
  expansionLimit         (100),
  fileDesc               (NULL),
  fileFormat             (RawFormat),
  highestAccuracySoFar   (0.0), 
  images                 (NULL),
  initialFeaturesStr     (),
  inputFileName          (),
  lockFile               (-1),
  lockFileOpened         (false),
  log                    (),
  imageClasses           (),
  initialFeatures        (NULL),
  nextJobId              (0),
  numOfFolds             (5),
  numPrevResults         (0),
  numOfExpansions        (0),
  performARestart        (false),
  resultFileName         (),
  schedulerStarted       (false),
  schedulingEnabled      (true),
  startCpuTime           (osGetSystemTimeUsed ()),
  testTimeTotal          (0.0),
  totalCpuTime           (0.0),
  trainTimeTotal         (0.0),
  validationData         (NULL),
  validationDataFileName (),
  weAreStillSearching    (true)

{
  log.Level (10) << "BeamSearchSVM::BeamSearchSVM" 
                 << endl;

  //time_t  curTime;
  //srand (time (&curTime));

  #ifndef  WIN32
  weWereSleeping = true;
  #endif;

  int  x;

  for  (x = 0; x < MaxNumPrevResults; x++)
    prevResults[x] = 0.0;

  doneJobs = new SingleRunList (KKStr (statusFileNameDone));
}





BeamSearchSVM::~BeamSearchSVM ()
{
  delete  validationData;

  delete  config;
  delete  doneJobs;
  delete  initialFeatures;
  delete  images;
}




void  BeamSearchSVM::Block ()
{
  log.Level (20) << "BeamSearchSVM::Block - Entering." << endl;
  
  int  count = 0;

  do  {
    lockFile = open (lockFileName, O_WRONLY | O_CREAT | O_EXCL);

    if  (lockFile < 0)
    {
      count++;
      log.Level (20) << "BeamSearchSVM::Block - We are locked out[" << count << "]."  << endl;
      #ifdef  WIN32
      Sleep (5000);
      #else
      sleep (5);
      #endif
    }
  }  while  (lockFile < 0);

  lockFileOpened = true;

  log.Level (20) << "BeamSearchSVM::Block - Lock is Established." << endl;
}



void  BeamSearchSVM::EndBlock ()
{              
  #ifndef  WIN32
  int  returnCd;
  #endif
  
  log.Level (20) << "BeamSearchSVM::EndBlock - Ending Block" << endl;

  if  (!lockFileOpened)
  {
    log.Level (-1) << "BeamSearchSVM::EndBlock - Lock file is not opened." << endl;
    return;
  }

  close (lockFile);
  lockFileOpened = false;
  

  #ifdef  WIN32
  if  (!DeleteFile (lockFileName))
  {
     DWORD fileAttributes = GetFileAttributes (lockFileName);
     fileAttributes = FILE_ATTRIBUTE_NORMAL;
     if  (!SetFileAttributes (lockFileName, fileAttributes))
     {
       DWORD  lastErrorNum = GetLastError ();
       log.Level (-1) << "BeamSearchSVM::EndBlock - *** ERROR *** Could not set Lock File  to  Normal." << endl;
     }
     else
     {
       if  (!DeleteFile (lockFileName))
       {
         DWORD  lastErrorNum = GetLastError ();
         log.Level (-1) << "BeamSearchSVM::EndBlock - Error["  << lastErrorNum << "] deleting Lock File." << endl;
       }
     }
  }
  #else
  returnCd = unlink (lockFileName);
  #endif  

  log.Level (20) << "EndBlock - Unlocking" << endl;
  return;
}  /* EndBlock */





void  DisplayCommandUssage ()
{
	printf (
           "Usage: svm-train [options] training_set_file [model_file]\n"
           "options:\n"
           "-l          beam size\n"
           "-v          n: n-fold cross validation mode\n"
           "-exclude    Features List to Exclude\n"
           "-SEARCHTYPE <BEAM  |  BESTFIRST  |  BESTFIRSTOUT>\n"
           "-ALGORITHM  <svm  | C45>     'svm' is Default\n"
           "-InitialFeatures  <Initial Features>\n"
           "-Config           <Config File Name>\n"
           "-FileFormat\n"
          );
}  /* DisplayCommandUssage */



void  BeamSearchSVM::exit_with_help ()
{
  DisplayCommandUssage ();
  osWaitForEnter ();
  exit(-1);
}



KKStr  BeamSearchSVM::ExcludedFeaturesStr (SingleRunPtr  j)  const
{
  FeatureNumList  excudedFeatures = (*initialFeatures) - FeatureNumList (fileDesc, j->FeatureNums ());
  return  excudedFeatures.ToString ();
}  /* ExcludedFeaturesStr */





SingleRunListPtr  BeamSearchSVM::LoadStatusFileOpen (bool  forceLoadDone)
{
  log.Level (10) << "BeamSearchSVM::LoadStatusFileOpen - Loading current Status File." << endl;

  char  buff[40960];

  FILE*  statusFile;

  statusFile = osFOPEN (statusFileNameOpen, "r");
  if  (!statusFile)
  {
     log.Level (-1) << endl
                    << "BeamSearchSVM::LoadCurrentStatusFile - *** ERROR ***  Can not open Status File[" 
                    << statusFileNameOpen << "]." 
                    << endl;
     EndBlock ();
     osWaitForEnter ();
     exit (-1);
  }

  weAreStillSearching = true;

  bool  anyJobsNotStarted = false;

  SingleRunListPtr  queue = new SingleRunList (true);

  while  (fgets (buff, sizeof (buff), statusFile))
  {
    KKStr  buffStr (buff);
    
    if  (buffStr.SubStrPart (0, 1) == "//")
    {
      // We have a comment line,  will ignore
      continue;
    }


    KKStr  fieldName = buffStr.ExtractToken ("\t\n\r ");
    fieldName.TrimLeft ();
    fieldName.TrimRight ();
    fieldName.Upper ();

    if  (fieldName == "CMDLINE")
    {
      // We no longer need to read cmdLine here.  It is done when processimg the staus file at the start of the program.
      //ProcessCmdLineStr  (buffStr);
    }

    else if  (fieldName == "NEXTJOBID")
    {
      nextJobId = buffStr.ExtractTokenInt (" \t\n\r");
    }

    else if  (fieldName == "NUMOFEXPANSIONS")
    {
      numOfExpansions = buffStr.ExtractTokenInt (" \t\n\r");
    }

    else if  (fieldName == "HIGHESTACCURACYSOFAR")
    {
      highestAccuracySoFar = buffStr.ExtractTokenDouble (" \n\r\t");
    }

    else if  (fieldName == "EXPANSIONSSINCEHIGHESTACCURACY")
    {
      expansionsSinceHighestAccuracy = buffStr.ExtractTokenInt (" \n\r\t");
    }

    else if  (fieldName == "PREVRESULTSSTR")
    {
      ProcessPrevResultsStr (buffStr);
    }

    else if  (fieldName == "SCHEDULERSTARTED")
    {
      schedulerStarted = buffStr.ExtractTokenBool (" \n\r\t");
    }

    else if  (fieldName == "STARTTIME")
    {
      startTime = DateTime (buffStr);
    }

    else if  ((fieldName == "TESTTIMETOTAL")  || (fieldName == "TESTTIME"))
    {
      testTimeTotal = buffStr.ExtractTokenDouble (" \n\r\t");
    }

    else if  (fieldName == "TOTALCPUTIME")
    {
      totalCpuTime = buffStr.ExtractTokenDouble (" \n\r\t");
    }

    else if  ((fieldName == "TRAINTIMETOTAL")  ||  (fieldName == "TRAINTIME"))
    {
      trainTimeTotal = buffStr.ExtractTokenDouble (" \n\r\t");
    }

    else if  (fieldName == "WEARESTILLSEARCHING")
    {
      weAreStillSearching = buffStr.ExtractTokenBool (" \n\r\t");
    }

    else if  (fieldName == "JOBQENTRY")
    {
      SingleRunPtr  newRun = new SingleRun (fileDesc, log, buffStr, algorithm, *queue);
      if  (newRun->CurStatus () < '2')
         anyJobsNotStarted = true;

      SingleRunPtr  existingRun = queue->LookUpByJobId (newRun->JobId ());
      if  (existingRun)
      {
        // Seems this job was in the file earlier, so we alreadt have created an 
        // existence of it.  In this case we will just update the previous one 
        // with the data from the one that was juist read into memory.
        existingRun->UpdateFromJob (*newRun);
        delete  newRun;  newRun = existingRun;
      } 
      else
      {
        queue->PushOnBack (newRun);
      }

      if  (newRun->CurStatus () <= '2')
      {
        // If a jobs was loaded from the OpenStatus file and its status is '2' or less
        // then it has not been added to the DoneStatus file.  In this case we want to 
        // flag it as updated so that it gets added to the DoneStatus file.
        newRun->Updated (true);
      }
    }
  }

  fclose (statusFile);

  if  (forceLoadDone)
  {
    // This meens that there are no more jobs ready to run,  
    // in this case we then want to load all completed jobs


    if  (!doneJobs)
      doneJobs = new SingleRunList (KKStr (statusFileNameDone));

    doneJobs->LoadStatusFile (algorithm, fileDesc, log);
  }


  return  queue;
}  /* LoadStatusFileOpen */







void  BeamSearchSVM::AddJobAndParentsToQueue (SingleRunList&  masterList,
                                              SingleRunList&  list, 
                                              SingleRunPtr    job
                                             )
{
  // if job is dependednt on parent,  make sure that it is
  // part of the list

  if  ((job->ChgFeatureAction () == faAddFeature)  ||
       (job->ChgFeatureAction () == faDeleteFeature)
      )
  {
    // Since this job depends on existance of parent, make sure it is also loaded
    SingleRunPtr parent = list.LookUpByJobId (job->ParentJobId ());
    if (!parent)
    {
      parent = masterList.LookUpByJobId (job->ParentJobId ());
      if  (parent)
      {
        AddJobAndParentsToQueue (masterList, list, parent);
      }
      else
      {
        parent = doneJobs->LookUpByJobId (job->ParentJobId ());
        if  (parent)
        {
          AddJobAndParentsToQueue (masterList, list, parent);
        }
        else
        {
          job->ChgFeatureAction (faSpecifyFeatures);
        }
      }
    }
  }

  list.PushOnBack (job);
}  /* AddJobAndParentsToQueue */





SingleRunListPtr  BeamSearchSVM::BuildQueueList (SingleRunList&  openJobs)
{ 
  // This method will be used when we are not updating the done file.
  // in thst case we will want to add jobs that are flagged as both 
  // open and done.

  openJobs.SortByJobId ();
  SingleRunListPtr  jobsToWriteToStatusFile = new SingleRunList (false);
  SingleRunList::const_iterator  idx;

  for (idx = openJobs.begin ();  idx != openJobs.end ();  idx++)
  {
    SingleRunPtr  singleRun = *idx;
    AddJobAndParentsToQueue (openJobs, *jobsToWriteToStatusFile, singleRun);
  }

  jobsToWriteToStatusFile->SortByJobId ();

  return  jobsToWriteToStatusFile;
}  /* BuildQueueList */




SingleRunListPtr  BeamSearchSVM::BuildOpenQueueList (SingleRunList&  openJobs)
{ 
  openJobs.SortByJobId ();
  SingleRunListPtr  jobsToWriteToStatusFile = new SingleRunList (false);
  SingleRunList::const_iterator  idx;

  for (idx = openJobs.begin ();  idx != openJobs.end ();  idx++)
  {
    SingleRunPtr  singleRun = *idx;
    AddJobAndParentsToQueue (openJobs, *jobsToWriteToStatusFile, singleRun);
  }

  jobsToWriteToStatusFile->SortByJobId ();

  return  jobsToWriteToStatusFile;
}  /* BuildOpenQueueList */






ofstream*  BeamSearchSVM::OpenDoneStatusFile ()
{
  ofstream*  statusFileDone = NULL;

  int  openAttempts = 0;
  
  statusFileDone = new ofstream (statusFileNameDone, ios::app);

  while  ((!statusFileDone->is_open ()) &&  (openAttempts < 3))
  {
    openAttempts++;
    log.Level (50) << "OpenDoneStatusFile Oppen Attempt[" << openAttempts << "]." << endl;
     statusFileDone->open (statusFileNameDone, ios::app);
     if  (!statusFileDone->is_open ())
     {
       log.Level (0) << "UpdateStatusFileBoth - *** WARNING ***  Can Not Open Status File[" 
                     << statusFileNameDone << "]   -Will Retry-." 
                     << endl;
       #ifdef  WIN32
       Sleep (2000);
       #else
       sleep (2);
       #endif
     }
  }

  if  (!statusFileDone->is_open ())
  {
    log.Level (-1) << "OpenDoneStatusFile - *** ERROR ***  Can not Create Status File[" 
                   << statusFileNameDone << "]." 
                   << endl;
    EndBlock ();
    osWaitForEnter ();
    exit (-1);
  }

  return  statusFileDone;
}  /* OpenDoneStatusFile */




void  BeamSearchSVM::UpdateStatusFile (SingleRunListPtr  queue)
{
  log.Level (20) << "BeamSearchSVM::UpdateStatusFile - Updating  Current Status File." << endl;

  int  openAttempts = 0;

  {
    // Lets rename Old Status files.
    KKStr  ext = osGetFileExtension (statusFileNameOpen);
    KKStr  baseName = osRemoveExtension (statusFileNameOpen) + "_BackUp";

    KKStr toName1 = baseName + "1." + ext;
    KKStr toName2 = baseName + "2." + ext;
    KKStr toName3 = baseName + "3." + ext;

    osDeleteFile (toName3);
    osRenameFile (toName2, toName3);
    osRenameFile (toName1, toName2);
    osRenameFile (statusFileNameOpen, toName1);
  }


  ofstream  statusFileOpen  (statusFileNameOpen, ios::out);

  while  ((!statusFileOpen.is_open ()) &&  (openAttempts < 3))
  {
    openAttempts++;
    log.Level (50) << "UpdateStatusFile Oppen Attempt[" << openAttempts << "]." << endl;
    statusFileOpen.open (statusFileNameOpen, ios::out);
    if  (!statusFileOpen.is_open ())
    {
      log.Level (0) << "UpdateStatusFile - *** WARNING ***  Can Not Open Status File[" 
                    << statusFileNameOpen << "]   -Will Retry-." 
                    << endl;
      #ifdef  WIN32
      Sleep (2000);
      #else
      sleep (2);
      #endif

    }
  }

  if  (!statusFileOpen.is_open ())
  {
     log.Level (-1) << "UpdateStatusFile - *** ERROR ***  Can not Create Status File[" 
                    << statusFileNameOpen << "]." 
                    << endl;
     EndBlock ();
     osWaitForEnter ();
     exit (-1);
  }

  ofstream*  statusFileDone = NULL;

  log.Level (50) << "UpdateStatusFileBoth - Status File Opened Successfully." << endl;


  statusFileOpen << "CmdLine" << "\t" << CommandLineStr () << endl;

  {
    const SVMparam&  svmParam = config->SVMparamREF ();
    statusFileOpen << "//" << svmParam.ToString () << endl;
  }

  {
    // Update totalCpuTime
    double  endCpuTime = osGetSystemTimeUsed ();
	  double  cpuTimeUsedSinceLastUpdate = endCpuTime - startCpuTime;
  	if  (cpuTimeUsedSinceLastUpdate < 0.0)
    {
      // We wrapped arround the 24 hour clock,  need to add 1 day worth of seconds.
      cpuTimeUsedSinceLastUpdate = cpuTimeUsedSinceLastUpdate + 86400;
    }
    totalCpuTime += cpuTimeUsedSinceLastUpdate;
    startCpuTime = endCpuTime;
  }


  statusFileOpen << "NextJobId"                       << "\t" << nextJobId                          << endl;
  statusFileOpen << "NumOfExpansions"                 << "\t" << numOfExpansions                    << endl;
  statusFileOpen << "HighestAccuracySoFar"            << "\t" << highestAccuracySoFar               << endl;
  statusFileOpen << "ExpansionsSinceHighestAccuracy"  << "\t" << expansionsSinceHighestAccuracy     << endl;
  statusFileOpen << "PrevResultsStr"                  << "\t" << PrevResultsStr ()                  << endl;
  statusFileOpen << "SchedulerStarted"                << "\t" << (schedulerStarted ? "Yes":"No")    << endl;
  statusFileOpen << "StartTime"                       << "\t" << startTime                          << endl;
  statusFileOpen << "TotalCpuTime"                    << "\t" << totalCpuTime                       << endl;
  statusFileOpen << "TrainTimeTotal"                  << "\t" << trainTimeTotal                     << endl;
  statusFileOpen << "TestTimeTotal"                   << "\t" << testTimeTotal                      << endl;
  statusFileOpen << "WeAreStillSearching"             << "\t" << (weAreStillSearching ? "Yes":"No") << endl;

  {
    log.Level (20) << "UpdateStatusFile  Starting to write entries into OpenStatusFile" << endl;


    SingleRunListPtr openJobs = NULL;

    if  (searchMethod == Beam)
      openJobs = BuildQueueList (*queue);
    else
      openJobs = BuildOpenQueueList (*queue);

    SingleRunList::const_iterator  idx;
    for  (idx = openJobs->begin ();  idx != openJobs->end ();  idx++)
    {
      SingleRunPtr  j = *idx;

      SingleRunPtr  parent = openJobs->LookUpByJobId (j->ParentJobId ());
      if  (!parent)
      {
        // Since the parent is not in the list then we want all the
        // features to be written out to the status file.  Not just the
        // ones that changed.
        j->ChgFeatureAction (faSpecifyFeatures);
      }

      statusFileOpen << "JobQEntry" << "\t" << j->RunStatusStr () << endl;
    }

    log.Level (20) << "UpdateStatusFile  Done writing entries into OpenStatusFile" << endl;

    delete  openJobs;
  }


  statusFileOpen.close ();


  log.Level (50) << "UpdateCurrentStatusFile - Exiting." << endl;
}  /* UpdateStatusFile */





bool  BeamSearchSVM::TerminationConditionReached ()
{
  log.Level (10) << "TerminationConditionReached" << endl;

  if  (numPrevResults < MaxNumPrevResults)
    return  false;

  if  (expansionsSinceHighestAccuracy > expansionLimit)
  {
    log.Level (10) << "TerminationConditionReached   True (expansionsSinceHighestAccuracy > expansionLimit)." << endl;
  
    return true;
  }

  return  false;
}  /*  TerminationConditionReached  */








void  BeamSearchSVM::ScheduleAJobRandomly (SingleRunListPtr  queue,
                                           ostream&          resultFile
                                          )
{
  log.Level (20) << "BeamSearchSVM::ScheduleAJobRandomly" << endl;

  ofstream*  doneStatusFile = OpenDoneStatusFile ();

  SingleRunListPtr  elidgableJobs = new SingleRunList (false);
  SingleRunListIterator  queueIDX (*doneJobs);
  for  (queueIDX.Reset (); queueIDX.CurPtr (); ++queueIDX)
  {
    if  (queueIDX->CurStatus () == '2')
      elidgableJobs->PushOnBack (queueIDX.CurPtr ());
  }

  if  (elidgableJobs->QueueSize () < 1)
  {
    log.Level (20) << "ScheduleAJobRandomly, Did not find any elidgable Jobs." << endl;
  }

  else
  {
    log.Level (30) << "ScheduleAJobRandomly,  ElidgableJobs[" << elidgableJobs->QueueSize () << "]." << endl;

    int   numJobsCreated = 0;

    do
    {
      int  x = rand () % elidgableJobs->QueueSize ();
 
      SingleRunPtr elidgableJob = elidgableJobs->IdxToPtr (x);

      log.Level (30) << "ScheduleAJobRandomly,  Job[" << elidgableJob->JobId () << "] was selected at random." << endl;

      if  (searchMethod == BestFirstOut)
      {
        elidgableJob->BuildSubSetsOut (queue, nextJobId, numJobsCreated, initialFeatures, doneJobs);
      }
      else
      {
        elidgableJob->BuildSubSets (queue, nextJobId, numJobsCreated, doneJobs);
      }

      elidgableJob->CurStatus ('3');
      *doneStatusFile <<  "JobQEntry" << "\t" << elidgableJob->RunStatusStr () << endl;
      doneStatusFile->flush ();

      if  (numJobsCreated > 0)
      {
        resultFile << "Included_Features(" << elidgableJob->FeaturesIncluded () << ") " 
                   << elidgableJob->Accuracy () << "%    **** Randomly Selected ****  Job["
                   << elidgableJob->JobId () << "]."
                   << endl
                   << endl;
      }
      else
      {
        log.Level (30) << "ScheduleAJobRandomly,  No jobs were Created" << endl;
        elidgableJobs->DeleteEntry (elidgableJob);
      }

    }  while  ((numJobsCreated < 1)  &&  (elidgableJobs->QueueSize () > 0));
  }

  delete elidgableJobs; 


  doneStatusFile->close ();
  delete  doneStatusFile;
  doneStatusFile = NULL;

}  /* ScheduleAJobRandomly */






SingleRunListPtr  BeamSearchSVM::ScheduleNextLevelOfJobs (SingleRunListPtr  queue)
{
  log.Level (20) << "BeamSearchSVM::ScheduleNextLevelOfJobs - Building Next Level of Jobs." << endl;
 
  ofstream* statusFileDone = OpenDoneStatusFile ();

  queue->SortByAccuracy ();

  // Lets build the next queue that we are going to process.
  SingleRunListIterator  rli2 (*queue);

  SingleRunListPtr nextQueue = new SingleRunList (true);

  int  numJobsSelected = 0;
  SingleRunPtr  s = NULL;
  for  (rli2.Reset (); ((s = rli2.CurPtr ())  &&  (numJobsSelected < beamSize)); ++rli2)
  {
    if  (s->CurStatus () != '2')
      continue;
    int  numJobsCreated = 0;
    s->BuildSubSets (nextQueue, nextJobId, numJobsCreated, doneJobs);
    s->CurStatus ('3');
    *statusFileDone <<  "JobQEntry" << "\t" << rli2->RunStatusStr () << endl;
    statusFileDone->flush ();
    numJobsSelected++;
 
    SingleRunPtr  existingJob = nextQueue->LookUpByJobId (s->JobId ());
    if  (!existingJob)
      nextQueue->PushOnBack (new SingleRun (*s)); 
  }

  statusFileDone->close ();
  delete  statusFileDone;
  statusFileDone = NULL;

  return  nextQueue;
}  /* ScheduleNextLevelOfJobs */






void  BeamSearchSVM::SwitchOverToBeamSeach (SingleRunListPtr&  queue)
{
  // Look for jobs we\ith the leasdt num of features left.

  if  (beamSize < 5)
    beamSize = 5;


  delete  queue;
  queue = new SingleRunList (true);

  while  (queue->size () < uint (beamSize))
  {
    SingleRunListPtr  topJobs = doneJobs->GetNextBestJobs ();
    if  (!topJobs)
      break;

    SingleRunList::iterator  idx;
    for  (idx = topJobs->begin ();  idx != topJobs->end ();  idx++)
    {
      SingleRunPtr  j = *idx;
      queue->PushOnBack (new SingleRun (*j));
      j->CurStatus ('3');
    }
  }

  searchMethod = Beam;

}  /* SwitchOverToBeamSeach */








void  BeamSearchSVM::ScheduleMoreJobsBestFirst (SingleRunListPtr  queue)
{
  log.Level (20) << "BeamSearchSVM::ScheduleMoreJobsBestFirst - Building Next Level of Jobs." << endl;
 
  ofstream  resultFile (resultFileName.Str (), ios::app);
  ofstream* statusFileDone = OpenDoneStatusFile ();


  int  numSelected = 0;

  while  (numSelected < beamSize)
  {
    SingleRunListPtr  topJobs = doneJobs->GetNextBestJobs ();
    if  (!topJobs)
    {
      log.Level (20) << "ScheduleMoreJobsBestFirst - There are no unexpanded jobs." << endl;
      // There are no unexpanded jobs,  there is nothing else we can do then
      break;
    }

    topJobs->RandomizeOrder ();

    log.Level (30) << "ScheduleMoreJobsBestFirst - topJobs.size[" << topJobs->QueueSize () << "." << endl;

    SingleRunListIterator  tjIDX (*topJobs);
    SingleRunPtr  nextTopJob = tjIDX.CurPtr ();
    while  ((nextTopJob)  &&  (numSelected < beamSize))
    {
      int  numJobsCreated = 0;

      log.Level (30) << "ScheduleMoreJobsBestFirst - Expanding Job[" << nextTopJob->JobId () << "]." << endl;

      nextTopJob->BuildSubSets (queue, nextJobId, numJobsCreated, doneJobs);
      nextTopJob->CurStatus ('3');

      *statusFileDone <<  "JobQEntry" << "\t" << nextTopJob->RunStatusStr () << endl;
      statusFileDone->flush ();

      SingleRunPtr  existingJob = queue->LookUpByJobId (nextTopJob->JobId ());
      if  (existingJob)
        existingJob->CurStatus ('3');
      else
        queue->PushOnBack (new SingleRun (*nextTopJob));

      if  (numJobsCreated > 0)
      {
        if  ((numOfExpansions % 10) == 0)
        {
          // We will not onky Build in,  we will buld out, on the hope of 
          // picking up a some features that were lost.
          nextTopJob->BuildSubSetsOut (queue, nextJobId, numJobsCreated, initialFeatures, doneJobs);
        }

        numSelected++;

        resultFile << endl
                   << "Included_Features(" << nextTopJob->FeaturesIncluded () << ") " 
                   << "Job Id("            << nextTopJob->JobId ()            << ") "
                   << nextTopJob->Accuracy () << "%"
                   << "   Expansion[" << numOfExpansions << "]."
                   << "    ****  NEW BEST FIRST ****   Job["
                   << nextTopJob->JobId () << "]."
                   << endl
                   << endl;
      }

      ++tjIDX;
      nextTopJob = tjIDX.CurPtr ();
    }

    log.Level (30) << "ScheduleMoreJobsBestFirst - Done exapnding topJobs." << endl;

    delete  topJobs;  topJobs = NULL;
  }

  statusFileDone->close ();
  delete  statusFileDone;
  statusFileDone = NULL;



  //if  ((numOfExpansions % 10) == 0)
  //{
  //  ScheduleAJobRandomly (queue, resultFile);
  //}

  resultFile.close ();

  return;
}  /* ScheduleMoreJobsBestFirst */




void  BeamSearchSVM::ScheduleMoreJobsBestFirstOut (SingleRunListPtr  queue)
{
  log.Level (20) << "BeamSearchSVM::ScheduleMoreJobsBestFirstOut - Building Next Level of Jobs." << endl;
 
  int  featureCount = 0;
  int  numSelected  = 0;

  int  numJobsInQueue = queue->QueueSize ();

  ofstream  resultFile (resultFileName.Str (), ios::app);

  ofstream*  statusFileDone = OpenDoneStatusFile ();

  doneJobs->SortByAccuracy ();
  SingleRunListIterator  queueIDX (*doneJobs);
  int  iteration = 0;

  SingleRunPtr  nextTopJob = NULL;

  for  (queueIDX.Reset (); (nextTopJob = queueIDX.CurPtr ()) &&  (numSelected < beamSize); ++queueIDX)
  {
    log.Level (30) << "ScheduleMoreJobsBestFirstOut - Iteration[" << iteration << "]." << endl
                   << "                               Loking for Highest Accuracy."
                   << endl;

    if  (nextTopJob->CurStatus () == '2')
    {
      int  numJobsCreated = 0;

      log.Level (30) << "ScheduleMoreJobsBestFirstOut - Found New Best First[" 
                     << nextTopJob->FeaturesIncluded () << "]." 
                     << endl;

      nextTopJob->BuildSubSetsOut (queue, nextJobId, numJobsCreated, initialFeatures, doneJobs);
      nextTopJob->CurStatus ('3');

      SingleRunPtr  existingJob = queue->LookUpByJobId (nextTopJob->JobId ());
      if  (existingJob)
        existingJob->CurStatus ('3');
      else
        queue->PushOnBack (new SingleRun (*nextTopJob));

      *statusFileDone <<  "JobQEntry" << "\t" << queueIDX->RunStatusStr () << endl;
      statusFileDone->flush ();

      if  (numJobsCreated > 0)
      {
        numSelected++;

        featureCount = queueIDX->FeatureCount ();

        resultFile << endl
                   << "Included_Features("   << queueIDX->FeaturesIncluded () << ") " 
                   << "Job Id("              << queueIDX->JobId ()            << ") "
                   << queueIDX->Accuracy ()  << "%"
                   << "   Expansion[" << numOfExpansions << "]."
                   << "    ****  NEW BEST FIRST ****"
                   << endl
                   << endl;
      }
    }
  }

  statusFileDone->close ();
  delete  statusFileDone;
  statusFileDone = NULL;

  if  ((numOfExpansions % 10) == 0)
  {
    ScheduleAJobRandomly (queue, resultFile);
  }

  resultFile.close ();

  return;
}  /* ScheduleMoreJobsBestFirstOut */





void   BeamSearchSVM::ProcessTerminationSituation (SingleRunListPtr&  queue)
{
  if  (searchMethod == BestFirst)
  {
    ofstream  resultFile (resultFileName.Str (), ios::app);
    resultFile << "Termination Condition Reached." << endl << endl;

    resultFile << endl 
               << endl;
    resultFile << "*** SWITCHING OVER TO BEAM SEARCH ***" << endl;
    resultFile << endl;
    resultFile.close ();
            
    SwitchOverToBeamSeach (queue);
    SingleRunListPtr nextQueue = ScheduleNextLevelOfJobs (queue);
    delete queue;
    queue = nextQueue;
    nextQueue = NULL;
  }
  else
  {
    delete  queue;
    queue = new SingleRunList (true);
        
    ofstream  resultFile (resultFileName.Str (), ios::app);
    resultFile << "Termination Condition Reached." << endl << endl;
    resultFile << "Result History :";
          
    for  (int x = 0; x < numPrevResults; x++)
    {
      if  (x > 0)
        resultFile << ", ";
      resultFile << prevResults[x];
    }
          
    resultFile << endl;
    resultFile << "Expansions Since Last Highest Accuracy: " << expansionsSinceHighestAccuracy << endl;
    resultFile << "Highest Accuracy So Far               : " << highestAccuracySoFar           << endl;
    resultFile << "Total Number of Expansions            : " << numOfExpansions                << endl;
    resultFile << endl;

    resultFile.close ();
  }
}  /* ProcessTerminationSituation */







void  BeamSearchSVM::LetsDoItInBatches ()
{
  log.Level (10)  << "BeamSearchSVM::LetsDoItInBatches - Starting" << endl;

  bool  moreJobs = true;
  while  (moreJobs)
  {
    ProcessAllJobsInOpenStatusFile ();
    if  (schedulingEnabled)
      TrySchedulingMoreJobs (moreJobs);
    else
      DetermineIfMoreJobs (moreJobs);
  }
} /* LetsDoItInBatches */





void  BeamSearchSVM::ProcessAllJobsInOpenStatusFile ()
{
  log.Level (10)  << "BeamSearchSVM::ProcessAllJobsInOpenStatusFile - Starting" << endl;

  bool  allJobsAreDone = false;
  SingleRunListPtr  openJobs = UpdateDoneJobsAndGetNextBatch (allJobsAreDone, NULL);

  while  (openJobs  ||  (!allJobsAreDone))
  {
    if  (openJobs)
      ProcessAllOpenJobsAtOnce (openJobs);

    SingleRunListPtr  nextOpenJobs = UpdateDoneJobsAndGetNextBatch (allJobsAreDone, openJobs);
    delete  openJobs;  openJobs = NULL;
    openJobs = nextOpenJobs;
   
    if  ((!openJobs)  &&  (!allJobsAreDone))
    {
      int  secsToSleep = 30;
      if  (schedulingEnabled)
      {
        // Since we are a scheduler we want to wake up quicker than 30 sec's
        secsToSleep = 10;
      }


      log.Level (10) << "ProcessAllJobsInOpenStatusFile    Will sleep for 30 seconds."  << endl;
      #ifdef  WIN32
      Sleep (secsToSleep * 1000);
      #else
      sleep (secsToSleep);
      #endif
    }
  }

  delete  openJobs;  openJobs = NULL;

} /* ProcessAllJobsInOpenStatusFile */






void  BeamSearchSVM::DoneJobsReportResults (SingleRunListPtr  jobsJustDone,
                                            SingleRunListPtr  jobsOnFile
                                           )

{
  log.Level (10) << "DoneJobsReportResults  Start of Method" << endl;

  ofstream  resultFile (resultFileName.Str (), ios::app);

  SingleRunList::iterator  cjIDX;
  for  (cjIDX = jobsJustDone->begin ();  cjIDX != jobsJustDone->end ();  cjIDX++)
  {
    SingleRunPtr j = *cjIDX;

    if  (j->CurStatus () < '2')
      continue;

    log.Level (50) << "DoneJobsReportResults - Record result for JobId[" << j->JobId () <<"]" << endl;

    if  (j->Accuracy () > bestResultThisNode)
      bestResultThisNode = j->Accuracy ();

    KKStr  newHighStr;
    if  (j->Accuracy () > (highestAccuracySoFar + 0.0001))
    {
      highestAccuracySoFar = j->Accuracy ();
      expansionsSinceHighestAccuracy = 0;
      newHighStr = "***NewHigh***(";
      newHighStr << expansionsSinceHighestAccuracy << ")";
    }

    resultFile << "JobId"      << "\t" << j->JobId ()         << "\t"
               << "Parent"     << "\t" << j->ParentJobId ()   << "\t"
               << "FeatureChg" << "\t" << j->ChgFeatureNum () << "\t";

    {
      KKStr  includedFeatursStr  = j->FeaturesIncluded ();
      KKStr  excludedFeaturesStr = ExcludedFeaturesStr (j);
      KKStr  hexStr              = j->FeatureNums ().HexStr ();

      int  minLen = Min (includedFeatursStr.Len (), excludedFeaturesStr.Len ());

      if  ((minLen > 256) &&  (hexStr.Len () < minLen))
        resultFile << "FtrsHex" << "\t" << hexStr <<  "\t";

      else if  (includedFeatursStr.Len () <= excludedFeaturesStr.Len ())
        resultFile << "FtrsIncl" << "\t" << includedFeatursStr <<  "\t";

      else
        resultFile << "FtrsExcl" << "\t" << excludedFeaturesStr << "\t";
    }

    double  accuracyChg = j->Accuracy () - j->AccuracyHistory (0);

    resultFile << "Acc"            << "\t"  << StrFormatDouble (j->Accuracy (), "zz0.0000") << "%"  << "\t"
               << "AccChg"         << "\t"  << StrFormatDouble (accuracyChg,    "zz0.0000") << "%"  << "\t"
               << "TrainTimeTotal" << "\t"  << StrFormatDouble (j->TrainTimeTotal (), "zzz0.000")   << "\t"
               << "TestTimeTotal"  << "\t"  << StrFormatDouble (j->TestTimeTotal (), "zzz0.000")    << "\t"
               << "SupPts"         << "\t"  << j->SupportPoints ()                                  << "\t"
               << "ClassAcc"       << "\t"  << j->Results ()->AccuracyStr ()                        << "\t"
               << "RunTime"        << "\t"  << osGetLocalDateTime ();

    if  (!newHighStr.Empty ())
      resultFile << "\t" << newHighStr;

    j->DeleteResults ();

    resultFile << endl;
  }

  resultFile.close ();

  {
    // Update Status Flag and jobsOnFile

    ofstream* statusFileDone = OpenDoneStatusFile ();

    SingleRunList::const_iterator  idx;
    for  (idx = jobsJustDone->begin ();  idx != jobsJustDone->end ();  idx++)
    {
      SingleRunPtr j = *idx;

      SingleRunPtr jobOnFile = jobsOnFile->LookUpByJobId (j->JobId ());
      if  (!jobOnFile)
      {
        log.Level (-1) << endl << endl << endl
                       << "DoneJobsReportResults   *** ERROR ***      Missing JobId[" << j->JobId () << "]" << endl
                       << endl;
        osWaitForEnter ();
        exit (-1);
      }

      jobOnFile->UpdateFromJob (*j);

      *statusFileDone <<  "JobQEntry" << "\t" << j->RunStatusStr () << endl;
      statusFileDone->flush ();
    }

    statusFileDone->close ();
    delete  statusFileDone;  
    statusFileDone = NULL;
  }
}  /* DoneJobsReportResults */







SingleRunListPtr  BeamSearchSVM::GetNextBatchOfOpenJobs (bool&             allJobsAreDone,
                                                         SingleRunListPtr  jobsOnFile
                                                        )
{
  allJobsAreDone = true;


  // Lets select open Jobs from this list
  SingleRunListPtr  jobsToRun = new SingleRunList (true);

  SingleRunList::iterator  idx;
  for  (idx = jobsOnFile->begin ();  idx != jobsOnFile->end ();  idx++)
  {
    SingleRunPtr j = *idx;
    if  (j->CurStatus () == '1')
    {
      allJobsAreDone = false;
    }

    else if  (j->CurStatus () == '0')
    {
      allJobsAreDone = false;

      j->CurStatus ('1');
      jobsToRun->PushOnBack (new SingleRun (*j));

      if  (jobsToRun->QueueSize () > batchSize)
        break;
    }
  }

  if  (jobsToRun->QueueSize () < 1)
  {
    delete  jobsToRun;  jobsToRun = NULL;
  }

  return  jobsToRun;
}  /* GetNextBatchOfOpenJobs */





SingleRunListPtr  BeamSearchSVM::UpdateDoneJobsAndGetNextBatch (bool&             allJobsAreDone,
                                                                SingleRunListPtr  jobsJustDone
                                                               )

{
  log.Level (10) << "UpdateDoneJobsAndGetNextBatch  Start of Method" << endl;

  Block ();

  // We will now:
  //   1) load openQueue file.
  //   2) Report results and flag jobs as done
  //   3) Get another batch of open jobs
  //   4) Update OpenQueue file.

  SingleRunListPtr  jobsOnFile = LoadStatusFileOpen (false);
  if  (jobsJustDone)
    DoneJobsReportResults (jobsJustDone, jobsOnFile);

  SingleRunListPtr nextBatchOfJobs = GetNextBatchOfOpenJobs (allJobsAreDone, jobsOnFile);

  UpdateStatusFile (jobsOnFile);

  delete  jobsOnFile;  jobsOnFile = NULL;

  EndBlock ();

  return  nextBatchOfJobs;
}  /* UpdateDoneJobsAndGetNextBatch */





void  BeamSearchSVM::DetermineIfMoreJobs (bool&  moreJobs)
{
  log.Level (10) << "DetermineIfMoreJobs   Starting." << endl;
  moreJobs = false;
  
  while  ((!moreJobs) && weAreStillSearching)
  {
    Block ();
    SingleRunListPtr  jobsOnFile = LoadStatusFileOpen (false);
    EndBlock ();
    if  (!jobsOnFile->AllJobsEvaluated ())
    {
      moreJobs = true;
    }
    else
    {
      log.Level (10) << "DetermineIfMoreJobs  Will Sleep for 30 seconds." << endl;
      #ifdef  WIN32
      Sleep (30000);
      #else
      sleep (30);
      #endif
    }
    delete  jobsOnFile;
  }
}  /* DetermineIfMoreJobs */




void  BeamSearchSVM::TrySchedulingMoreJobs (bool&  moreJobs)

{
  log.Level (10) << "TrySchedulingMoreJobs   Starting." << endl;

  Block ();
  
  bool  loadDoneJobsToo = true;
  //if  (searchMethod == Beam)
  //  loadDoneJobsToo = false;

  SingleRunListPtr  jobsOnFile = LoadStatusFileOpen (loadDoneJobsToo);

  // Make sure qanother process did not already schedule some jobs.
  if  (!jobsOnFile->AllJobsEvaluated ())
  {
    // Some other process has managed to already create some new jobs
    // We will not need to schedule any more jobs.
  }
  else
  {
    numOfExpansions++;
    expansionsSinceHighestAccuracy++;

    // We need to schedule more Jobs.

    UpdatePrevResults (bestResultThisNode);

    moreJobs = true;

    if  (searchMethod == Beam)
    {
      log.Level (20) << "TrySchedulingMoreJobs - Beam Search Method." << endl;

      SingleRunListPtr  nextLevelOfJobs = ScheduleNextLevelOfJobs (jobsOnFile);
      delete  jobsOnFile;
      jobsOnFile = nextLevelOfJobs;
      nextLevelOfJobs = NULL;

      if  (jobsOnFile->QueueSize () < 1)
      {
        moreJobs = false;
        weAreStillSearching = false;
      }
    }
    else 
    {
      if  (TerminationConditionReached ())
      {
        log.Level (10) << "TrySchedulingMoreJobs - TerminationConditionReached." << endl;

        ProcessTerminationSituation (jobsOnFile);
        moreJobs = (jobsOnFile->QueueSize () > 0);
      }
      else
      {
        // Since we will be scheduling a whol enew level of jobs we are
        // free to delete currecnt contents of OpenQueue  aka 'jobsOnFile'
        delete  jobsOnFile;
        jobsOnFile = new SingleRunList (true);

        if  (searchMethod == BestFirst)
        {
          ScheduleMoreJobsBestFirst (jobsOnFile);
        }

        else  // BestFirstOut
        {
          ScheduleMoreJobsBestFirstOut (jobsOnFile);
        }
      }
    }

    log.Level (10) << "TrySchedulingMoreJobs - At end of function, just before UpdateStatusFile." << endl;
    UpdateStatusFile (jobsOnFile);
  }
  
  delete  jobsOnFile; jobsOnFile = NULL;

  EndBlock ();
} /* TrySchedulingMoreJobs */







void  BeamSearchSVM::ProcessAllOpenJobsAtOnce (SingleRunListPtr  jobList)
{
  // The idea of this method is to process all open jobs in queue in one pass,  
  // with only updating the sattus files once at the very very end.  This way
  // we will minimize the i/o on the status files.  This method would not be able 
  // to run in a multi processing environment.   But would be good when the time
  // it takes to process a node is very very short.
  log.Level (10)  << "BeamSearchSVM::ProcessAllOpenJobsAtOnce - Started" << endl;

  ImageClassListPtr  nextJobImageClasses = NULL;

  nextJobImageClasses = new ImageClassList (imageClasses);


  int  qSize = jobList->QueueSize ();

  SingleRunPtr  nextJob = NULL;
  SingleRunList::iterator  jIDX = jobList->begin ();

  for  (jIDX = jobList->begin ();  jIDX != jobList->end ();  jIDX++)
  {
    nextJob = *jIDX;

    if  (nextJob->CurStatus () > '1')
      continue;

    log.Level (20) << "ProcessAllOpenJobsAtOnce - Job[" << nextJob->FeaturesIncluded () << "] Selected for processing." << endl;

    // Set next Job Status to Started.
    nextJob->CurStatus ('1');

    DateTime  startTime = osGetLocalDateTime ();
    nextJob->RunCrossValidation (*images,
                                 *nextJobImageClasses,
                                 config,
                                 numOfFolds,
                                 &cout
                                );

    DateTime  endTime = osGetLocalDateTime ();
    DateTime  elaspedTime = endTime - startTime;

    log.Level (20) << "ProcessAllOpenJobsAtOnce - Job[" << nextJob->FeaturesIncluded () << "]  is done." << endl;
    log.Level (20) << "                 Will flag as completed." << endl;

    nextJob->CurStatus ('2');
  }


  delete  nextJobImageClasses;
} /* ProcessAllOpenJobsAtOnce */






FeatureNumList  BeamSearchSVM::ExtractFeatureNumbers (KKStr&  featureListStr)
{
  bool valid;
  return  FeatureNumList (fileDesc, IncludeFeatureNums, featureListStr, valid);
} /* ExtractFeatureNumbers */






void  BeamSearchSVM::parse_command_line (int    argCount, 
                                         char **arguments
                                        )
{
  int i;

  numOfFolds    = 2;
  beamSize   = 1;

  initialFeaturesStr = "";

  i = 1;
  while  (i < argCount)
  {
    if  (arguments[i][0] != '-')  
        break;

    KKStr switchStr = arguments[i];
    switchStr.Upper ();
    i++;

    KKStr  parmValue;
    KKStr  parmValueUpper;
    if  (arguments[i])
    {
      if  (arguments[i][0] != '-')
      {
        parmValue = arguments[i];
        parmValueUpper = parmValue;
        parmValueUpper.Upper ();
        i++;
      }
    }

    if  (switchStr == "-BATCH")
    {
      batchSize = atoi (parmValue.Str ());
      if  (batchSize == 0)
      {
        batchSize = 100;
      }

      else if  (batchSize < 0)
      {
        log.Level (-1) << "parse_command_line   **** ERROR ****"  << endl
                       << "               '-BatchSize " << batchSize << "' Must be greater than 0." 
                       << endl
                       << endl;

        DisplayCommandUssage ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  ((switchStr == "-DATAFILE")  ||  (switchStr == "-DF"))
    {
      inputFileName = parmValue;
    }

    else if  (switchStr == "-INITIALFEATURES")
    {

      if  (parmValue.Empty ())
      {
        log.Level (-1) << "parse_command_line   **** ERROR ****"  << endl
                       << "                     -InitialFeatures requires a parameter." 
                       << endl
                       << endl;

        DisplayCommandUssage ();

        osWaitForEnter ();
        exit (-1);
      }


      initialFeaturesStr = parmValue;
    }

    else if  (switchStr == "-WIDTH") 
    {
      beamSize = atoi (parmValue.Str ());
    }

    else if  ((switchStr == "-V")  ||  (switchStr == "-FOLDS"))
    {
      numOfFolds = atoi (parmValue.Str ());

      if  (numOfFolds < 2)
      {
        cerr << "n-fold cross validation: n must >= 2"  << endl;
        exit_with_help ();
      }
    }

    else if  (switchStr == "-CONFIG") 
    {
      configFileName = parmValue;
    }


    else if  (switchStr == "-RESTART") 
    {
      performARestart = true;
    }

    else if  (switchStr == "-SEARCHTYPE") 
    {
      KKStr  searchTypeStr (parmValue);

      searchTypeStr.Upper ();

      if  (searchTypeStr == "BEAM")
        searchMethod = Beam;

      else if  (searchTypeStr == "BESTFIRST")
        searchMethod = BestFirst;

      else if  (searchTypeStr == "BESTFIRSTOUT")
        searchMethod = BestFirstOut;

      else
      {
        log.Level (-1) << "parse_command_line   **** ERROR ****"  << endl
                       << "                     Invalid SEARCHTYPE Parameter[" 
                       <<                       searchTypeStr 
                       << "]."
                       << endl
                       << endl;

        DisplayCommandUssage ();

        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (switchStr == "-ALGORITHM") 
    {
      KKStr  algorithmStr (parmValue);
      algorithmStr.Upper ();

      if  (algorithmStr == "C45")
      {
        algorithm = C45;
      }

      else if  (algorithmStr == "SVM")
      {
        algorithm = SVM;
      }

      else
      {
        log.Level (-1) << "parse_command_line   **** ERROR ****"  << endl
                       << "                     Invalid ALGORITHM Parameter[" 
                       <<                       algorithmStr 
                       << "]."
                       << endl;

        DisplayCommandUssage ();

        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (switchStr == "-EL")
    {
      expansionLimit = atoi (parmValue.Str ());
    }

    else if  ((switchStr == "-FILEFORMAT")  ||  (switchStr == "-FORMAT"))
    {
      fileFormat = FileFormatFromStr (parmValue);
      if  (fileFormat == NULL_Format)
      {
        log.Level (-1) << endl << endl << endl
                       << "parse_command_line   **** ERROR ****"  << endl
                       << endl
                       << "             Invalid File Format[" << parmValue << "]" << endl
                       << endl;

        DisplayCommandUssage ();
        osWaitForEnter ();
        exit (-1);
      }
    }


    else if  ((switchStr == "-VALIDATION")  ||  (switchStr == "-VALIDATIONFILE")  ||  (switchStr == "-VF"))
    {
      validationDataFileName = parmValue;
    }

    else
    {
      log.Level (-1) << endl << endl << endl
                     << "parse_command_line     **** ERROR ****" << endl
                     << endl
                     << "               Invalid Parameter[" << switchStr << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

  } /* End of For */


  if  (!images)
    read_problem ();

  delete  initialFeatures;
  initialFeatures = NULL;

  {
    bool  validInitialFeaturs = false;
    if  (initialFeaturesStr.Empty ())
    {
      initialFeatures = new FeatureNumList (images->AllFeatures ());
      validInitialFeaturs = true;
    }
    else
    {
      initialFeatures = new FeatureNumList (fileDesc, initialFeaturesStr, validInitialFeaturs);
    }

    if  (!validInitialFeaturs)
    {
      log.Level (-1) << endl << endl << endl
                     << "parse_command_line   *** ERROR ***" << endl
                     << "                     Invalid Initial Features[" << initialFeatures->ToString () << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }
  }
}  /* parse_command_line */







KKStr  BeamSearchSVM::CommandLineStr  ()
{
  log.Level (50) << "BeamSearchSVM::CommandLineStr - Entered." << endl;

  KKStr  cmdStr;

  cmdStr << "-el " << expansionLimit << "  ";

  if  (initialFeatures)
  {
    if  (!initialFeatures->AllFeaturesSelected ())
      cmdStr << "-InitialFeatures "  << initialFeatures->ToString () << "  ";
  }


  cmdStr << "-SEARCHTYPE ";

  if  (searchMethod == Beam)
    cmdStr << "BEAM";

  else if  (searchMethod == BestFirst)
    cmdStr << "BESTFIRST";

  else if  (searchMethod == BestFirstOut)
    cmdStr << "BESTFIRSTOUT";

  else
    cmdStr << "BEAM";


  cmdStr << " -algorithm ";
  if  (algorithm == SVM)
    cmdStr << "SVM";

  else if  (algorithm == C45)
    cmdStr << "C45";    

  else
    cmdStr << "*Undefined*";

  cmdStr << "  -Batch "  << batchSize << "  ";

  cmdStr << "  ";
  cmdStr << "-WIDTH " << beamSize           << "  ";

  cmdStr << "-v " << numOfFolds            << "  ";

  cmdStr << "-Config " << configFileName << "  ";

  cmdStr << "-FileFormat " << FileFormatDescription (fileFormat) << "  ";

  cmdStr << "-DataFile " << inputFileName << " ";

  if  (!validationDataFileName.Empty ())
    cmdStr << "-Validation" << validationDataFileName;

  return  cmdStr;
}  /* CommandLineStr */




void  BeamSearchSVM::ProcessCmdLineStr  (KKStr  cmdLine)
{
  char*  arguments[200];
  int    argCount = 0;

  arguments[0] = KKU::STRDUP ("BeamSearchSVM");
  argCount++;

  while  (!cmdLine.Empty ())
  {
    KKStr  parmField = cmdLine.ExtractToken (" \n\r\t");
    arguments[argCount] = KKU::STRDUP (parmField.Str ());
    argCount++;
  }

  parse_command_line (argCount, arguments);

  for  (int  x = 0;  x < argCount;  x++)
    delete  arguments[x];

}  /* CommandLineStr */






KKStr  BeamSearchSVM::PrevResultsStr ()
{
  log.Level (50) << "BeamSearchSVM::PrevResultsStr - Entered,  NumPrevResults[" 
                 << numPrevResults 
                 << "]  BestResultThisNode[" << bestResultThisNode << "]." 
                 << endl;

  int  x;

  KKStr  prevResultsStr;

  prevResultsStr << bestResultThisNode;
  prevResultsStr << ", " << numPrevResults;

  for  (x = 0; x < numPrevResults; x++)
  {
    prevResultsStr.Append (", ");
    prevResultsStr << prevResults[x];
  }

  log.Level (50) << "BeamSearchSVM::PrevResultsStr - Exiting." << endl;

  return  prevResultsStr;
}  /* PrevResultsStr */






void   BeamSearchSVM::ProcessPrevResultsStr (KKStr  prevResultsStr)
{
  log.Level (20) << "BeamSearchSVM::ProcessPrevResultsStr[" << prevResultsStr << "]." << endl;

  int  x;
  for  (x = 0; x < MaxNumPrevResults; x++)
    prevResults[x] = 0.0;

  if  (prevResultsStr.Empty ())
  {
    bestResultThisNode = 0;
    numPrevResults = 0;
    return;
  }
  
  KKStr  bestResultsThisNodeStr;
  bestResultsThisNodeStr =  prevResultsStr.ExtractToken (", \t");
  bestResultThisNode = atof (bestResultsThisNodeStr.Str ());

  KKStr  numPrevResultsStr = prevResultsStr.ExtractToken (", \t");
  numPrevResults = atoi (numPrevResultsStr.Str ());

  if  ((numPrevResults < 0)  ||  (numPrevResults > MaxNumPrevResults))
  {
    log.Level (-1) << endl
                   << "ProcessPrevResultsStr   *** ERROR ***   Invalid NumPrevResults[" 
                   << numPrevResults << "]." 
                   << endl;
    numPrevResults = 0;
    return;
  }

  for (x = 0; x < numPrevResults; x++)
  {
    KKStr  prefResultStr = prevResultsStr.ExtractToken (", \t");
    prevResults[x] = atof (prefResultStr.Str ());
  }
}  /* ProcessPrevResultsStr */






void  BeamSearchSVM::UpdatePrevResults (double  result)
{
  int  x;

  for  (x = MaxNumPrevResults - 1; x > 0; x--)
     prevResults[x] = prevResults[x - 1];

  prevResults[0] = result;

  numPrevResults++;

  if  (numPrevResults > MaxNumPrevResults)
    numPrevResults = MaxNumPrevResults;
}  /* UpdatePrevResults */





// read in a problem (in svmlight format)
void BeamSearchSVM::read_problem ()
{
  bool  successfull;
  bool  changesMade = false;

  log.Level (10) << "BeamSearchSVM::read_problem  Load Data Files" << endl;

  delete  images;
  images = LoadFeatureFile (inputFileName, fileFormat, imageClasses, -1, log, cancelFlag, successfull, changesMade);
  if (!successfull)
  {
    log.Level (-1) << endl
                   << " *****  Can not open input file[" << inputFileName << "]  *****" 
                   << endl;
    EndBlock ();
  	osWaitForEnter ();
    exit (-1);
  }

  fileDesc = images->FileDesc ();

  if  (!validationDataFileName.Empty ())
  {
    delete  validationData;
    validationData = LoadFeatureFile (validationDataFileName, fileFormat, imageClasses, -1, log, cancelFlag, successfull, changesMade);

    if (!successfull)
    {
      log.Level (-1) << endl
                     << " *****  Error Loading Validation Data[" << validationDataFileName << "]  *****" << endl;
      EndBlock ();
      osWaitForEnter ();
      exit (-1);
    }

    if  (validationData->FileDesc () != fileDesc)
    {
      log.Level (-1) << endl
                     << " *****  Error Loading Validation Data[" << validationDataFileName << "]  *****"
                     << "     Training Data and Validation data do not have same FileDsc" << endl
                     << endl
                     << "     images->FileDesc->NumOfFields            [" << images->FileDesc ()->NumOfFields ()            << "]" << endl
                     << "     validationData->FileDesc ()->NumOfFields [" << validationData->FileDesc ()->NumOfFields () << "]" << endl
                     << endl;
      EndBlock ();
      osWaitForEnter ();
      exit (-1);
    }
  }

  delete config;  
  config = new TrainingConfiguration (images->FileDesc (),
                                      configFileName, 
                                      log,
                                      false  // Don't validate Training Directotries,
                                     );
  if  (!config->FormatGood ())
  {
    log.Level (-1) << endl
                   << "ParameterSearch::ReadProblem  ConfigFile Invalid[" << configFileName << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  // Lets Normalize the Data.
  NormalizationParms normParmns (config, *images, log);
  normParmns.NormalizeImages (images);
  if  (validationData)
    normParmns.NormalizeImages (validationData);

}  /* read_problem */





void  BeamSearchSVM::PerformARestart ()
{
  KKStr  answer;
  do
  {
    cout << endl << endl << endl << endl << endl
         << "****************************************************************************"  << endl
         << endl
         << endl
         << "*        Are You Sure You Want To Restart (Yes/No) ?";

    cin >> answer;
    answer.TrimLeft ();
    answer.TrimRight ();
    answer.Upper ();
  }
  while  ((answer != "YES")  &&  (answer != "NO"));

  if  (answer == "NO")
  {
    EndBlock ();
    exit (-1);
  }

  SingleRunListPtr  openJobs = LoadStatusFileOpen (false);

  {
    resultFileName =  osGetRootName (inputFileName) + ".result";

    ofstream  resultFile (resultFileName.Str (), ios::app);
    resultFile  << endl << endl << endl
                << "*********************************"  << endl
                << "*     Performing a Restart      *"  << endl
                << "*********************************"  << endl
                << endl
                << endl
                << "Date Time[" << osGetLocalDateTime () << "]" << endl
                << endl;

    resultFile.close ();
  }

  SingleRunList::iterator  idx;
  for  (idx = openJobs->begin ();  idx != openJobs->end ();  idx++)
  {
    SingleRunPtr  j = *idx;

    if  (j->CurStatus () < '2')
      j->CurStatus ('0');
  }

  weAreStillSearching = true;
  schedulerStarted    = false;
  schedulingEnabled   = false;

  UpdateStatusFile (openJobs);

}  /* PerformARestart */




void  BeamSearchSVM::ProcessStatusFile (int     argc,  
                                        char**  argv
                                       )
{
  Block ();

  if  (argc > 1)
  {
    KKStr  parm1 (argv[1]);
    parm1.Upper ();
    if  (parm1 == "-LOCK")
    {
      cout << endl
           << endl
           << endl
           << "   *** Lock File Created. ***" << endl
           << endl;
      exit (0);
    }

    else if  (parm1 == "-RESTART")
    {
      performARestart = true;
    }
  }
  
  FILE*  statusFile = osFOPEN (statusFileNameOpen, "r");
  if  (!statusFile)
  {
    //  We are doing our initial StartUp.  So lets set up the Status file.
    parse_command_line (argc, argv);

    cout << endl
         << CommandLineStr  ()  << endl
         << endl;

    nextJobId = 0;
    
    // Since we are the first processor to start we will be the scheduler.
    schedulerStarted  = true;
    schedulingEnabled = true;

    startTime = osGetLocalDateTime ();

    SingleRunListPtr  queue = new SingleRunList (true);

    if  (searchMethod == BestFirstOut)
    {
      int  numInitialFeatures = initialFeatures->NumOfFeatures ();

      for  (int z = 0; z < (numInitialFeatures - 1); z++)
      {
        for  (int w = z + 1; w < numInitialFeatures; w++)
        {
          SingleRunPtr  root = new SingleRun (fileDesc, log, nextJobId, -1, algorithm);

          if  (!root)
          {
            cerr << endl;
            cerr << "ProcessStatusFile  **** ERROR ****" << endl;
            cerr << "                   Could not Allocate SingleRun Object." << endl;
            cerr << endl;
            osWaitForEnter ();
            exit (-1);
          }

          nextJobId++;
          root->AddFeatureNum ((*initialFeatures)[z]);
          root->AddFeatureNum ((*initialFeatures)[w]);
          queue->PushOnBack (root);
        }
      }
    }
    else
    {
      SingleRunPtr  root = new SingleRun (fileDesc, log, nextJobId, -1, algorithm);

      nextJobId++;

      if  (initialFeatures)
      {
        root->SetFeatureNums (*initialFeatures);
      }
      else
      {
        // Since Initial Feature nums not specified,  we want to start with all features.
        for  (int x = 0; x < fileDesc->NumOfFields (); x++)
        {
          root->AddFeatureNum (x);
        }
      }

      queue->PushOnBack (root);
    }

    UpdateStatusFile (queue);
    delete  queue;  queue = NULL;
  }
  else
  {
    // Status file already exists.


    // Since the status file already exists we will assume that
    // a scheduler is already running
    schedulingEnabled = false;

    bool  cmdLineRead = false;

    char  buff[40960];

    while  ((fgets (buff, sizeof (buff), statusFile))  &&  (!cmdLineRead))
    {
      KKStr  buffStr (buff);
      KKStr  fieldName = buffStr.ExtractToken ("\t\n\r ");
      fieldName.Upper ();

      if  (fieldName == "CMDLINE")
      {
        ProcessCmdLineStr  (buffStr);
        cmdLineRead = true;
      }
    }

    fclose (statusFile);

    if  (performARestart)
    {
      PerformARestart ();
      EndBlock ();
      exit (-1);
    }

    if  (!cmdLineRead)
    {
      log.Level (-1) << endl << endl << endl
                     << "ProcessStatusFile    **** ERROR ****" << endl
                     << endl
                     << "          CmdLine field is not in the status file." << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

    SingleRunListPtr  queue = LoadStatusFileOpen ();

    if  (!schedulerStarted)
    {
      // The status file indicates that a scheduler is not started.  
      // We will have to perform that function.
      schedulerStarted  = true;
      schedulingEnabled = true;
    }

    UpdateStatusFile (queue);

    delete  queue;
  }  


  {
    resultFileName =  osGetRootName (inputFileName) + ".result";

    ofstream  resultFile (resultFileName.Str (), ios::app);


    resultFile << "// Date         [" << osGetLocalDateTime ()  << "]" << endl
               << "// StartTime    [" << startTime              << "]" << endl
               << "// Command Line [" << CommandLineStr ()      << "]" << endl
               << "// Data File    [" << inputFileName          << "]" << endl
               << "// Validation   [" << validationDataFileName << "]" << endl
               << "// NumOfFolds   [" << numOfFolds             << "]" << endl
               << "// ConfigFile   [" << configFileName         << "]" << endl
               << "//" << endl;

    resultFile.close ();
  }


  EndBlock ();
}  /* ProcessStatusFile */






int  main (int argc, char **argv)
{
  #if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
  #endif


  searchMethod = Beam;

  BeamSearchSVM*  beamSearch = new BeamSearchSVM ();

  beamSearch->ProcessStatusFile (argc, argv);

  beamSearch->LetsDoItInBatches ();

  delete  beamSearch;

  ImageClass::FinalCleanUp ();
  FileDesc::FinalCleanUp ();

  #if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
  _CrtDumpMemoryLeaks();
  #endif



  return 0;
}
