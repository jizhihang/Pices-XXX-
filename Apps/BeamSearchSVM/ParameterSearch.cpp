#include  "FirstIncludes.h"
//*******************************************************************************
//*                                                                             *
//*                                                                             *
//*-----------------------------------------------------------------------------*
//*                                                                             *
//*  Status File Format                                                         *
//*                                                                             *
//*  Line#                                                                      *
//*    0   <Command Line Including Deature Selection>                           *
//*    1   <Num Of Processes>  <Next Job ID>  <Highest Accuracy So Far>         *
//*    2   <Next C Parameter>  <Next Gamma Paramer>   <Next A Parameter>        *
//*    3 ...                                                                    *
//*      ...\                                                                   *
//*    n .....One Line Per Running Process   (Process Line Description)         *
//*    n..../                                                                   *
//*                                                                             *
//*                                                                             *
//*  Process Line Description:                                                  *
//*       <Process Num>                                                         *
//*       <Job Id>                                                              *
//*       <c Parameter>                                                         *
//*       <gamma Parameter>                                                     *
//*       <a Parameter>                                                         *
//*       <Accuracy>                                                            *
//*       <1st Class>  These two fields are only used when searching for        *
//*       <2nd Class>  optimal parameters by two class combos.                  *
//*                                                                             *
//*                                                                             *
//*                                                                             *
//*                                                                             *
//*                                                                             *
//*                                                                             *
//*******************************************************************************




#include <stdlib.h>
#include <stdio.h>
#include <cmath>



#include  <ctype.h>
#include  <time.h>

#include  <fcntl.h>

#include  <string>
#include  <iomanip>
#include  <iostream>
#include  <fstream>
//#include  <ostream>
#include  <vector>
#include  <algorithm>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;



#include <sys/types.h>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
#include  <sys/loadavg.h>
#include <unistd.h>
#endif


#include  "OSservices.h"
#include  "RunLog.h"

#include  "FeatureFileIO.h"
#include  "NormalizationParms.h"
#include  "ParameterSearchReportLine.h"
#include  "TrainingConfiguration.h"

#include  "ParameterSearch.h"




// -X  5,6,10,14,15,17,21,22,24,27,28  -L 1  -a 1  -b 100  -f 10  -s 1  -t 2  -d 3.0  -g 0.010  -r 0.0  -n 0.20  -m 40.0  -c 1.0  -e 0.0010  -p 0.10  -h 1  -v 10  -z BestCaseOut.txt  TEST_OCT_24_80p.data

// -I 0,1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,19,20,22,23,24,25,26,28   -L 1  -a 1  -b 100  -f 10  -s 1  -t 2  -d 3.0  -g 0.010  -r 0.0  -n 0.20  -m 40.0  -c 1.0  -e 0.0010  -p 0.10  -h 1  -v 5  -z K:\Plankton\StatusReports\2003_Mar_18\NextBestBack_26.txt  TEST_OCT_24_80p.data  



// ParameterSearch   -SEARCHTYPE BESTFIRST  -el 150  -s 0 -n 0.11 -t 2 -g 0.032  -c 300 -v 5 -l 1 -z  GrayScaleCpp_April_16_03.txt K:\Plankton\TempBeamSearch\GrayScaleCpp_April_16_03.data
//                                                 -s 0 -n 0.11 -t 2 -g 0.032  -c 300


//  -SEARCHTYPE BESTFIRST  -ALGORITHM  c45  -el 150  -v 5 -l 1 -z  TEST_OCT_24_80p_NeuroC45.txt  K:\Plankton\TempBeamSearch\BestCase\TEST_OCT_24_80p.data


// ParameterSearch  -el 150  -SEARCHTYPE BEAM  -L 5  -a 1  -b 100  -f 10  -s 0  -t 2  -d 3  -g 0.032  -r 0  -n 0.11  -m 40  -c 300  -e 0.001  -p 0.1  -h 1  -v 5     -z GrayScaleCpp_New_Report.txt  K:\\Plankton\\TempBeamSearch\\GrayScale_May_13_03\\GrayScaleCpp_2003-May-13.data




// ParameterSearch  -v 5   -a 1  -b 100  -f 10  -s 0  -t 2  -d 3  -g 0.032  -r 0  -n 0.11  -m 40  -c 300  -e 0.001  -p 0.1  -h 1  -i 1,9,17,18,19,21,28,29,32,35,40,41,42,45,46 -z For_15_Features.txt  GrayScaleCpp_April_19_03.data



// ParameterSearch -v 5  -s 0 -n 0.11 -t 2 -g 0.032  -c 300  -u 1500  ActiveLearningValidationImages

// -v 5  -s 0   -n 0.11   -t 2   -g 0.032    -c 300    -u 1500  -mt OneVsAll  K:\\Plankton\\StatusReports\\2004-02-10\\allnorm_Sorted.data

//  -v 5 -s 0 -n 0.11 -t 2 -g 0.032 -c 300 -u 1500 -mt OneVsAll   K:\v1\Plankton\TempBeamSearch\2004_March_02\2004_03_02_TestData.data

// 2004-09-30
//  -v 5  -up  -sm P  NineClasses_ValidationData.data

// 2004-10-05
//  -v 5  -up  -sm P  NineClasses_TestData.data

// 2005-01-24
//  K:\v1\Plankton\ActiveLearning\Results\2004_Sep_Expirements_Diff_Prior
//  -v 5 -s 0 -n 0.11 -t 2 -up  -sm P   ActiveLearning_MasterTest_Diff_Prior_GridSearch.data

// 2005-03-15   Paramter Search for Andrews "1% model"
//  -v 5 -s 0 -n 0.11 -t 2 -up  -sm P   OnePercentModelTestData.data


// 2005-05-18   
// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures\ParameterSearch
//  -a 1  -e 0.001  -g 0  -h 1  -j 0  -k 0  -l 1  -m 40  -n 0.5  -p 0.1  -q 0  -r 0  -s 0  -t 2  -u 1  -MT ONEVSONE  -SM Probability  -UseProbability  -v 5  NineClasses_ValidationData.data

// -c covtype.cfg -df covtype.data -validation covtype.test -report covtype_with_compression-and-encoding.txt

// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures\ParameterSearch
// -c Thesis9Classes.cfg   NineClasses_TestData.data

// 2005-11-16
// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures\ParameterSearch\Binary_05
// -BinaryClasses -df NineClasses_ValidationData.data -Folds 5  -Config Thesis9Classes_05.cfg

// 2006-02-09
// K:\Plankton\Papers\BitReduction\DataSets\WEB_Data\Tune
// -df anonymous-msweb_Data-C45_30p.data -Folds 5  -Config anonymous-msweb.cfg

// 2006-02-10
// K:\Plankton\Papers\BitReduction\DataSets\Banana\Tune
// -Config  Banana.cfg  -df banana.all.txt  -Folds 5
// -Config  Banana.cfg  -df banana.all.txt   -Format Sparse  -Folds 5

// C:\bigpine\Plankton\Papers\BitReduction\DataSets\Banana\Tuning
// -Config  Banana.cfg  -df banana.all.txt  -Folds 5

// K:\Plankton\Papers\BitReduction\DataSets\WaveForm\Tunning
// -Config  WaveForm.cfg  -df waveform-+noise_80p.data   -Validation waveform-+noise_20p.data   -Format c45 -Report WaveForm_80-20_218PC.txt 

// C:\bigpine\Plankton\Papers\BitReduction\DataSets\SatImage\Tuning
// -Config  Sat.cfg   -Format c45 -df SatAll_shuffled.data  -Folds 5

// K:\Plankton\Papers\BitReduction\DataSets\Adult\Tunning
// -Config Adult.cfg  -df Adult_10p.data -Format c45 -folds 5


// 2006-02-20
// K:\Plankton\Papers\BitReduction\DataSets\Phoneme\Tunning
// -config phoneme.cfg -df phoneme_80p.data -format c45 -folds 5

// K:\Plankton\Papers\BitReduction\DataSets\Letter\Tunned
// C:\bigpine\Plankton\Papers\BitReduction\DataSets\WEB_Data\Tuning
// -config Letter.cfg -df letter-c45_80p.data -format c45 -folds 5

// K:\Plankton\Papers\BitReduction\DataSets\WEB_Data\Tunned
// C:\bigpine\Plankton\Papers\BitReduction\DataSets\WEB_Data\Tuning
// -config anonymous-msweb.cfg -df anonymous-msweb_All-C45_10p.data -format c45 -folds 5
// -config anonymous-msweb.cfg -df anonymous-msweb_Data-C45.data -format c45 -folds 5


// 2006-02-23
// C:\bigpine\Plankton\Papers\BitReduction\DataSets\Page-Blocks\Tuning
// -config ..\page-blocks.cfg -df ..\page-blocks.data -format UCI -folds 5

// C:\bigpine\Plankton\Papers\BitReduction\DataSets\Shuttle\Tuning
// -config Shuttle.cfg -df Shuttle_Train_c45_shuffled.data   -format C45 -folds 5

// C:\bigpine\Plankton\Papers\BitReduction\DataSets\Pendigits\Tuning
// -config pendigits.cfg -df pendigits_train_sparse_shuffled.data  -format Sparse  -folds 5

// C:\bigpine\Plankton\Papers\BitReduction\DataSets\Letter\Tuning
// -config Letter.cfg  -df letter-c45_80p_shuffled.data  -format c45 -folds 5

// C:\bigpine\Plankton\Papers\BitReduction\DataSets\Phoneme\Tuning
//  -config phoneme.cfg  -df phoneme_80p.data   -format c45 -folds 5

//C:\bigpine\Plankton\Papers\BitReduction\DataSets\Adult\Tuning
// -Config Adult.cfg  -df Adult_10p.data -Format c45 -folds 5

// 2006-06-16
// C:\bigpine\Plankton\Papers\BitReduction\DataSets\ForestCover\Tuning
// -config covtype.cfg -df ..\CovType_TwoClass.data  -format c45  -folds 5


int   lockFile = 0;

bool  lockFileOpened = false;

const char*  lockFileName   = "ParameterSearch.lock";

const char*  statusFileName = "ParameterSearch.status";


#define  GammaMin   0.01000
//#define  GammaMax  20.00000
#define  GammaMax   0.01010

//#define  GammaMin  0.0001
//#define  GammaMin  0.00100
//#define  GammaMax  0.99999



ParameterSearch::ParameterSearch ():
  cancelFlag     (false),
  fileFormat     (RawFormat),
  log            (),
  images         (NULL),
  imageClasses   (),
  svmParamaters  (NULL),
  testExamples   (NULL),

  nextJobId             (0),
  highestAccuracySoFar  (0.0),

  ourProcessNum         (-1),
  multiClassClassifiers (true),

  numOfProcesses        (-1),

  nextC                 (1),
  nextGamma             (GammaMin),
  nextA                 (100.0f),

/*  
  nextC                 (1),
  nextGamma             (0.01507),
  nextA                 (100),
*/

/*
  nextC                 (12),
  nextGamma             (GammaMin),
  nextA                 (100),
*/
  curJob                (NULL),
  config                (NULL),
  processes             (NULL)

{
  log.Level (10) << "ParameterSearch::ParameterSearch" 
                 << endl;
  time_t  curTime;
  srand (int (time (&curTime)));

  #ifndef  WIN32
  weWereSleeping = true;
  #endif;

}



ParameterSearch::~ParameterSearch ()
{
  if  (svmParamaters)
    delete svmParamaters;

  if  (curJob)
    delete  curJob;

  if  (processes)
    delete  processes;

  if  (images)
    delete  images;

  delete  testExamples;  testExamples = NULL;
}





#ifndef  WIN32
double  ParameterSearch::LoadAverage ()
{
  log.Level (50) << "ParameterSearch::LoadAverage" << endl;

  int  numExamples;
  double  loadAvg[3];

  numExamples = getloadavg (loadAvg, 3);
  if  (numExamples < 1)
    return -1;

  else if  (weWereSleeping)
    return loadAvg[0];

  else 
    return loadAvg[0] - 1.0;
}
#endif




void  ParameterSearch::Block ()
{
  log.Level (20) << "ParameterSearch::Block - Entering." << endl;
  
  int  count = 0;

  do  {
    lockFile = open (lockFileName, O_WRONLY | O_CREAT | O_EXCL);
    if  (lockFile < 0)
    {
      count++;
      log.Level (20) << "ParameterSearch::Block - We are locked out[" << count << "]."  << endl;
      #ifdef  WIN32
      Sleep (5000);
      #else
      sleep (5);
      #endif
    }
  }  while  (lockFile < 0);

  lockFileOpened = true;

  log.Level (20) << "ParameterSearch::Block - Lock is Established." << endl;
}




void  ParameterSearch::EndBlock ()
{              
  #ifndef  WIN32
  int  returnCd;
  #endif
  
  log.Level (20) << "ParameterSearch::EndBlock - Ending Block" << endl;

  if  (!lockFileOpened)
  {
    log.Level (-1) << "ParameterSearch::EndBlock - Lock file is not opened." << endl;
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
       log.Level (-1) << "ParameterSearch::EndBlock - *** ERROR *** Could not set Lock File  to  Normal." << endl;
     }
     else
     {
       if  (!DeleteFile (lockFileName))
       {
         DWORD  lastErrorNum = GetLastError ();
         log.Level (-1) << "ParameterSearch::EndBlock - Error["  << lastErrorNum << "] deleting Lock File." << endl;
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
	printf(
           "Usage: svm-train [options] training_set_file [model_file]\n"
           "options:\n"
           "-s svm_type : set type of SVM (default 0)\n"
           "     0 -- C-SVC\n"
           "     1 -- nu-SVC\n"
           "     2 -- one-class SVM\n"
           "     3 -- epsilon-SVR\n"
           "     4 -- nu-SVR\n"
           "-t kernel_type : set type of kernel function (default 2)\n"
           "     0 -- linear: u'*v\n"
           "     1 -- polynomial: (gamma*u'*v + coef0)^degree\n"
           "     2 -- radial basis function: exp(-gamma*|u-v|^2)\n"
           "     3 -- sigmoid: tanh(gamma*u'*v + coef0)\n"
           "-d  degree : set degree in kernel function (default 3)\n"
           "-g  gamma : set gamma in kernel function (default 1/k)\n"
           "-r  coef0 : set coef0 in kernel function (default 0)\n"
           "-c  cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)\n"
           "-n  nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)\n"
           "-p  epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)\n"
           "-m  cachesize : set cache memory size in MB (default 40)\n"
           "-e  epsilon : set tolerance of termination criterion (default 0.001)\n"
           "-h  shrinking: whether to use the shrinking heuristics, 0 or 1 (default 1)\n"
           "-wi weight: set the parameter C of class i to weight*C, for C-SVC (default 1)\n"
           "-v n: n-fold cross validation mode\n"
          );
}  /* DisplayCommandUssage */



void  ParameterSearch::exit_with_help ()
{
  DisplayCommandUssage ();
  exit(1);
}






void  ParameterSearch::LoadCurrentStatusFile ()
{
  log.Level (10) << "ParameterSearch::LoadCurrentStatusFile - Loading current Status File." << endl;

  char  buff[10240];

  statusFile = osFOPEN (statusFileName, "r");
  if  (!statusFile)
  {
     log.Level (-1) << endl
                    << "ParameterSearch::LoadCurrentStatusFile - *** ERROR ***  Can not open Status File[" 
                    << statusFileName << "]." 
                    << endl;
     EndBlock ();
     exit (1);

     osWaitForEnter ();
  }


  {
    fgets (buff, sizeof (buff), statusFile);
    KKStr  cmdLine (buff);
    cmdLine.TrimRight ();
    ProcessCmdLineStr  (cmdLine);
  }


  {
    fgets (buff, sizeof (buff), statusFile);
    KKStr  buffStr (buff);

    numOfProcesses       = buffStr.ExtractTokenInt    (" ,\n\t");
    nextJobId            = buffStr.ExtractTokenInt    (" ,\n\t");
    highestAccuracySoFar = buffStr.ExtractTokenDouble (" ,\n\t");
  }

  {
    fgets (buff, sizeof (buff), statusFile);
    KKStr  buffStr (buff);

    nextC     = buffStr.ExtractTokenInt    (" ,\n\t");
    nextGamma = buffStr.ExtractTokenDouble (" ,\n\t");
    nextA     = (float)buffStr.ExtractTokenDouble (" ,\n\t");
  }

  
  if  (processes)
  {
    delete  processes;
    processes = NULL;
  }

  processes = new ParameterJobList (true);

  for  (int  processLineNum = 0;  processLineNum < numOfProcesses;  processLineNum++)
  {
    fgets (buff, sizeof (buff), statusFile);
    KKStr  buffStr (buff);
    processes->PushOnBack (new ParameterJob (log, buffStr));
  }

  fclose (statusFile);
  statusFile = NULL;
  return;
}  /* LoadCurrentStatusFile */






void  ParameterSearch::UpdateCurrentStatusFile ()
{
  log.Level (20) << "ParameterSearch::UpdateCurrentStatusFile - Updating  Current Status File." << endl;

  int  openAttempts = 0;

  statusFile = NULL;

  while  ((!statusFile)  &&  (openAttempts < 3))
  {
    openAttempts++;
    log.Level (50) << "UpdateCurrentStatusFile Oppen Attempt[" << openAttempts << "]." << endl;
    statusFile = osFOPEN (statusFileName, "w");
    if  (!statusFile)
    {
      log.Level (0) << "UpdateCurrentStatusFile - *** WARNING ***  Can Not Open Status File[" 
                    << statusFileName << "]   -Will Retry-." 
                    << endl;
      #ifdef  WIN32
      Sleep (2000);
      #else
      sleep (2);
      #endif

    }
  }

  if  (!statusFile)
  {
     log.Level (-1) << "UpdateCurrentStatusFile - *** ERROR ***  Can not Create Status File[" 
                    << statusFileName << "]." 
                    << endl;
     EndBlock ();
     exit (1);
     osWaitForEnter ();
  }

  
  log.Level (50) << "UpdateCurrentStatusFile - Status File Opened Successfully." << endl;


  KKStr  s;
  s = CommandLineStr ();
  fprintf (statusFile, "%s\n", s.Str ());

  fprintf (statusFile, "%d\t %d\t %f\n", 
                        numOfProcesses,
                        nextJobId, 
                        highestAccuracySoFar
          );


  fprintf (statusFile, "%d\t %f\t %f\n", 
                        nextC,
                        nextGamma, 
                        nextA
          );


  ParameterJobListIterator  jIDX (*processes);
  ParameterJobPtr           process;

  for  (jIDX.Reset ();  process = jIDX.CurPtr ();  ++ jIDX)
  {
    fprintf (statusFile, "%s\n", process->ToString ().Str ());
  }


  fclose  (statusFile);
  statusFile = NULL;

  log.Level (50) << "UpdateCurrentStatusFile - Exiting." << endl;
}  /* UpdateCurrentStatusFile */




void  ParameterSearch::ProcessJobBinaryClasses ()
{
  log.Level (10) << "ParameterSearch::ProcessJobBinaryClasses" << endl;

  double  elaspedTime;
  double  endTime;
  double  startTime;

  int  numOfClasses = imageClasses.QueueSize ();

  vector<KKStr>  resultLines;

  for  (int  class1Idx = 0;  class1Idx < (numOfClasses - 1);  class1Idx++)
  {
    ImageClassPtr  class1 = imageClasses.IdxToPtr (class1Idx);
    for  (int  class2Idx = class1Idx + 1;  class2Idx < numOfClasses;  class2Idx++)
    {
      ImageClassPtr  class2 = imageClasses.IdxToPtr (class2Idx);

      ImageClassList  twoClassList;
      twoClassList.PushOnBack (class1);
      twoClassList.PushOnBack (class2);

      ImageFeaturesListPtr  twoClassExamples =   new ImageFeaturesList (twoClassList, *images, log);

      FeatureNumList  selectedFeatures = config->GetFeatureNums (class1, class2);

      ConfussionMatrix2  results (twoClassList, log);

      double  numOfSupportVectors;
      float   avgProbability;
      float   avgKnowClassProb;
      double  avgCompact;
      double  compactDev;

      startTime = osGetSystemTimeUsed ();

      curJob->CrossValidation (*twoClassExamples,
                               twoClassList,
                               selectedFeatures,
                               svmParamaters->ToString (),
                               nr_fold,
                               results,
                               numOfSupportVectors,
                               avgProbability,
                               avgKnowClassProb,
                               avgCompact,
                               compactDev
                              );

      endTime = osGetSystemTimeUsed ();

      elaspedTime = endTime - startTime;

      KKStr  resultLine (255);
      resultLine <<           curJob->JobId ()
                 << "\t"  << class1->Name ()
                 << "\t"  << class2->Name ()
                 << "\t"  << curJob->CParm ()
                 << "\t"  << StrFormatDouble (curJob->GammaParm (), "-zz0.000000")
                 << "\t"  << curJob->AParm ()
                 << "\t"  << StrFormatDouble (curJob->Accuracy (), "-zz0.00") << "%"
                 << "\t"  << numOfSupportVectors
                 << "\t"  << StrFormatDouble (100.0 * avgProbability,   "-zz0.00") << "%"
                 << "\t"  << StrFormatDouble (100.0 * avgKnowClassProb, "-zz0.00") << "%"
                 << "\t"  << avgCompact
                 << "\t"  << compactDev
                 << "\t"  << StrFormatDouble (elaspedTime, "ZZZ,ZZ0.000")
                 << "\t"  << StrFormatDouble (curJob->TrainTime (), "ZZZ,ZZ0.000")
                 << "\t"  << StrFormatDouble (curJob->ClassTime (), "ZZZ,ZZ0.000")
                 << "\t"  << results.AccuracyStr ();

      resultLines.push_back (resultLine);

      log.Level (10) << endl
                     << endl
                     << resultLine << endl
                     << endl;

      delete  twoClassExamples;
    }
  }



  Block ();

  ofstream  resultFile (resultFileName.Str (), ios::app);
  for  (uint x = 0;  x < resultLines.size ();  x++)
    resultFile << resultLines[x] << endl;
  resultFile.close ();

  LoadCurrentStatusFile ();

  ParameterJobPtr  j = processes->LocateByProcessNum (ourProcessNum);
  if  (!j)
  {
    log.Level (-1) << endl
                   << "*** ERROR ***"  << endl
                   << "ProcessNextJob,  Could not locate job for ProcessNum["
                   << ourProcessNum
                   << "]."
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }
  
  double  accuracy = curJob->Accuracy ();

  bool  newHigh = false;

  if  (accuracy > highestAccuracySoFar)
  {
    highestAccuracySoFar = accuracy;
    newHigh = true;
  }

  j->CurStatus ('2');
  j->Accuracy  (accuracy);

  UpdateCurrentStatusFile ();

  EndBlock ();


  log.Level (20) << "ParameterSearch::ProcessNextJob - Job[" << curJob->JobId () 
                 << "]  is done."
                 << endl;
}  /* ProcessJobBinaryClasses */



void  ParameterSearch::ProcessNextJobAllClassesCombined ()
{
  double  elaspedTime;
  double  endTime;
  double  startTime;

  log.Level (20) << "ProcessNextJob - Processing Job[" << curJob->JobId () << "]."
                 << endl;

  int  numOfClasses = imageClasses.QueueSize ();

  FeatureNumList  selectedFeatures = config->GetFeatureNums ();

  ConfussionMatrix2  results (imageClasses, log);

  double  numOfSupportVectors;
  float   avgProbability;
  float   avgKnowClassProb;
  double  avgCompact;
  double  compactDev;

  startTime = osGetSystemTimeUsed ();

  curJob->CrossValidation (*images,
                           imageClasses,
                           selectedFeatures,
                           svmParamaters->ToString (),
                           nr_fold,
                           results,
                           numOfSupportVectors,
                           avgProbability,
                           avgKnowClassProb,
                           avgCompact,
                           compactDev
                          );

  endTime = osGetSystemTimeUsed ();

  elaspedTime = endTime - startTime;

  KKStr  resultLine (255);
  resultLine <<           curJob->JobId ()
             << "\t"  << "AllClasses"
             << "\t"  << curJob->CParm ()
             << "\t"  << StrFormatDouble (curJob->GammaParm (), "-zz0.000000")
             << "\t"  << curJob->AParm ()
             << "\t"  << StrFormatDouble (curJob->Accuracy (), "-zz0.00") << "%"
             << "\t"  << numOfSupportVectors
             << "\t"  << StrFormatDouble (100.0 * avgProbability,   "-zz0.00") << "%"
             << "\t"  << StrFormatDouble (100.0 * avgKnowClassProb, "-zz0.00") << "%"
             << "\t"  << avgCompact
             << "\t"  << compactDev
             << "\t"  << StrFormatDouble (elaspedTime, "ZZZ,ZZ0.000")
             << "\t"  << StrFormatDouble (curJob->TrainTime (), "ZZZ,ZZ0.000")
             << "\t"  << StrFormatDouble (curJob->ClassTime (), "ZZZ,ZZ0.000")
             << "\t"  << results.AccuracyStr ();

  
   log.Level (10) << endl
                  << endl
                  << resultLine << endl
                  << endl;


  log.Level (20) << "                 Will flag as completed." << endl;

  Block ();

  ofstream  resultFile (resultFileName.Str (), ios::app);
  resultFile << resultLine << endl;
  resultFile.close ();

  LoadCurrentStatusFile ();


  ParameterJobPtr  j = processes->LocateByProcessNum (ourProcessNum);
  if  (!j)
  {
    log.Level (-1) << endl
                   << "*** ERROR ***"  << endl
                   << "ProcessNextJob,  Could not locate job for ProcessNum["
                   << ourProcessNum
                   << "]."
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }
  
  double  accuracy = curJob->Accuracy ();

  bool  newHigh = false;

  if  (accuracy > highestAccuracySoFar)
  {
    highestAccuracySoFar = accuracy;
    newHigh = true;
  }

  j->CurStatus ('2');
  j->Accuracy  (accuracy);

  UpdateCurrentStatusFile ();

  EndBlock ();
}  /* ProcessNextJobAllClassesCombined */






void  ParameterSearch::LetsDoIt ()
{
  log.Level (10)  << "ParameterSearch::LetsDoIt - Started" << endl;



  if  (images->MissingData ())
  {
    log.Level (-1) << endl << endl
                   << "ParameterSearch::LetsDoIt   *** WARNING ***"
                   << endl
                   << "    DataFile[" << inputFileName << "]  has missing data." << endl
                   << endl;
    images->RemoveEntriesWithMissingFeatures ();

    ofstream  resultFile (resultFileName.Str (), ios::app);
    resultFile << endl << endl 
               << "   DataFile[" << inputFileName << "]  has missing data." << endl
               << endl;
    resultFile.close ();

  }

  if  (testExamples)
  {
    if  (testExamples->MissingData ())
    {
      log.Level (-1) << endl << endl
                     << "ParameterSearch::LetsDoIt   *** WARNING ***"
                     << endl
                     << "    TestFile[" << testFileName << "]  has missing data." << endl
                     << endl;
      testExamples->RemoveEntriesWithMissingFeatures ();
      ofstream  resultFile (resultFileName.Str (), ios::app);
      resultFile << endl << endl 
                 << "   TestFile[" << testFileName << "]  has missing data." << endl
                 << endl;
      resultFile.close ();
    }
  }
  


  bool  weAreAllDone = false;

  while  (!weAreAllDone)
  {

    if  (curJob)
    {
      delete  curJob;
      curJob = NULL;
    }


    #ifndef  WIN32
    double  la;

    if  (LoadAverage () > 7.2)
    {
      while  ((la = LoadAverage ()) > 6.6)
      {
        // The system is pretty Busy so to be Fair to everyone else lets go to sleep 
        // for a while.
        log.Level (20) << "ParameterSearch::LetsDoIt - Load Average[" << la << "] is greater than 4.8." << endl;
        log.Level (20) << "                          Will go to sleep for one minute and try again."  << endl;
        weWereSleeping = true;

        int  randSleepTime = rand () % 20;

        sleep (20 + randSleepTime);
      }
    }

    weWereSleeping = false;
    #endif


    Block ();

    LoadCurrentStatusFile ();

    if  (nextC > 17000)
    {
      weAreAllDone = true;
      EndBlock ();
      break;
    }

    ParameterJobPtr  j = processes->LocateByProcessNum (ourProcessNum);
    if  (j)
    {
      processes->DeleteEntry (j);
      j = NULL;
    }

    curJob = new ParameterJob (log, ourProcessNum, nextJobId, nextC, nextGamma, nextA);
    curJob->CurStatus ('1');
    processes->PushOnBack (new ParameterJob (*curJob));

    cout << endl
         << "nextC[" <<nextC << "]  nextGamma[" << nextGamma << "]  NextA[" << nextA << "]" << endl
         << endl;

/*  
    nextA = 100;
    nextGamma = GammaMin;
    //nextGamma = 0.01507;
    if  (nextC < 20)
      nextC++;
    else
      nextC = (int)((float)nextC * 1.1f + 0.5);

    if  (nextC > 17000)
    {
      weAreAllDone = true;
      delete  curJob;
      curJob = NULL;
    }
*/


    nextA = 100.0f;
    nextGamma = nextGamma * 1.1;
    if  (nextGamma > GammaMax)
    {
      nextGamma = GammaMin;

      if  (nextC < 20)
        nextC++;
      else
        nextC = (int)(((FFLOAT)nextC * 1.2) + (FFLOAT)0.5);

      if  (nextC > 30)
      {
        weAreAllDone = true;
        delete  curJob;
        curJob = NULL;
      }
    }


/*
    nextA = int((double)nextA * 1.2);
    if  (nextA > 200)
    {
      nextA = 100;
      nextGamma = nextGamma * 1.2;

      if  (nextGamma > GammaMax)
      {
        nextGamma = GammaMin;

        if  (nextC < 20)
          nextC++;
        else
          nextC = (int)(((FFLOAT)nextC * 1.2) + (FFLOAT)0.5);

        if  (nextC > 17000)
        {
          weAreAllDone = true;
          delete  curJob;
          curJob = NULL;
        }
      }
    }
*/



    if  (!weAreAllDone)
    {
      currentJobId = nextJobId;
      nextJobId++;
    }

    UpdateCurrentStatusFile ();
    EndBlock ();

    if  (curJob)
    {
      if  (multiClassClassifiers)
        ProcessNextJobAllClassesCombined ();
       else
        ProcessJobBinaryClasses ();
    }
  }
} /* LetsDoIt */







void  ParameterSearch::ParseCommandLine (int    argCount, 
                                         char **arguments
                                        )
{
  int i;

  KKStr  svmCmdLineStr = "";

  nr_fold    = 5;

  KKStr switchStr;
  KKStr switchParm;


  // parse options
  i = 1;
  while  (i < argCount)
  {
    bool addToSvmCmdLineStr = false;

    if  (arguments[i][0] != '-')  
    {
      break;
    }

    switchStr = arguments[i];
    switchStr.Upper ();
    switchParm = "";
    i++;

    if  (i < argCount)
    {
      if  (arguments[i][0] != '-') 
      {
        switchParm = arguments[i];
        i++;
      }
    }

    if  (switchStr == "-CONFIG") 
    {
      configFileName = switchParm;
    }

    else if  ((switchStr == "-DF")   ||  (switchStr == "-DATAFILE"))
    {
      inputFileName = switchParm;
    }

    else if  ((switchStr == "-VALIDATION")   ||  
              (switchStr == "-VALIDATE")     ||
              (switchStr == "-VF")           ||
              (switchStr == "-TESTFILENAME") ||
              (switchStr == "-TESTFILE")     ||
              (switchStr == "-TF") 
             )
    {
      testFileName = switchParm;
    }

    else if  (switchStr == "-FORMAT")
    {
      fileFormat = FileFormatFromStr (switchParm);
      if  (fileFormat == NULL_Format)
      {
        log.Level (-1) << endl << endl << endl
                       << "*** ERROR ***    Invalid Forat Specified[" << switchParm << "]." << endl 
                       << endl
                       << "                 Valid Read Formats [" << FileFormatsReadOptionsStr () << "]" << endl
                       << endl;
        exit_with_help ();
      }

      if  (!FileFormatReadSupported (fileFormat))
      {
        log.Level (-1) << endl << endl << endl
                       << "ParseCommandLine   *** ERROR ***" << endl 
                       << endl
                       << "Reading feature file format[" << switchParm << "] is not supported." << endl
                       << endl;
        exit_with_help ();
      }
    }

    else if  (switchStr == "-FOLDS") 
    {
      nr_fold = atoi (switchParm.Str ());

      if  (nr_fold < 2)
      {
        cerr << "n-fold cross validation: n must >= 2"  << endl;
        exit_with_help ();
      }
    }

    else if  (switchStr == "-BINARYCLASSES")
    {
      multiClassClassifiers = false;
    }

    else
    {
      addToSvmCmdLineStr = true;
    }


    if  (addToSvmCmdLineStr)
    {
      if  (!svmCmdLineStr.Empty ())
           svmCmdLineStr << "  ";

      svmCmdLineStr << switchStr << " " << switchParm;
    }
  }


  if  (svmParamaters)
  {
    delete  svmParamaters;
    svmParamaters = NULL;
  }


  if  (!images)
  {
    bool  successful;  
    bool  changesMade = false;
    images = LoadFeatureFile (inputFileName, fileFormat,   imageClasses, -1, log, cancelFlag, successful, changesMade);
    if  ((!successful)  ||  (!images))
    {
      log.Level (-1) << endl
                     << endl
                     << "ParameterSearch::ParseCommandLine   *** ERROR ***"
                     << "                            Can not load DataFile[" << inputFileName << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

    imageClasses.SortByName ();
  }


  if  ((!testExamples)  &&  (!testFileName.Empty ()))
  {
    bool  successful;  
    bool  changesMade = false;
    testExamples = LoadFeatureFile (testFileName, fileFormat, imageClasses, -1, log, cancelFlag, successful, changesMade);
    if  ((!successful)  ||  (!testExamples))
    {
      log.Level (-1) << endl
                     << endl
                     << "ParameterSearch::ParseCommandLine   *** ERROR ***"
                     << "                            Can not load TestFile[" << testFileName << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

    //imageClasses.SortByName ();
  }


  if  (!config)
  {
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

    imageClasses.SortByName ();
  }

  if  (!svmParamaters)
  {
    FeatureNumList selectedFeatures = images->AllFeatures ();

    svmParamaters = new SVMparam (*(config->Model3Parameters ()));

    if  (!svmParamaters)
    {
      cerr << endl;
      cerr << "parse_command_line  **** ERROR ****" << endl;
      cerr << "                    Could not Allocate SVMparam Object." << endl;
      cerr << endl;
      cerr << endl;
      DisplayCommandUssage ();
      osWaitForEnter ();
      exit (-1);
    }
  }

  // inputFileName = "allnorm_Sorted.data";
}  /* ParseCommandLine */




KKStr  ParameterSearch::CommandLineStr  ()
{
  log.Level (50) << "ParameterSearch::CommandLineStr - Entered." << endl;

  if  (!svmParamaters)
  {
    log.Level (-1) << "ParameterSearch::CommandLineStr - *** ERROR***,  svmParamaters == NULL." << endl;
    osWaitForEnter ();
    exit (-1);
  }

  KKStr  cmdStr (255);
  KKStr  svmParmStr;

  svmParmStr = svmParamaters->ToString ();

  cmdStr << "-Config"    << " " << configFileName                     << "  "
         << "-Format"    << " " << FileFormatDescription (fileFormat) << "  "
         << "-DataFile"  << " " << inputFileName                      << "  ";

  if  (!testFileName.Empty ())
    cmdStr << "-TestFile" << " " << testFileName   << "  ";

  cmdStr << "-Folds "    << " " << nr_fold        << "  ";

  if  (!multiClassClassifiers)
    cmdStr << " -BinaryClasses  ";

  cmdStr << svmParmStr;

  log.Level (60) << "ParameterSearch::CommandLineStr - Exiting." << endl;

  return  cmdStr;
}  /* CommandLineStr */








void  ParameterSearch::ProcessCmdLineStr  (KKStr  cmdLine)
{
  char*  arguments[60];
  int    argCount = 0;

# ifdef  USE_SECURE_FUNCS
  arguments[0] = _strdup ("ParameterSearch");
# else
  arguments[0] = strdup ("ParameterSearch");
# endif

  argCount++;

  while  (!cmdLine.Empty ())
  {
    KKStr  parmField = cmdLine.ExtractToken (" ");

#   ifdef USE_SECURE_FUNCS
      arguments[argCount] = _strdup (parmField.Str ());
#   else
      arguments[argCount] = strdup (parmField.Str ());
#   endif


    argCount++;
  }

  ParseCommandLine (argCount, arguments);
}  /* CommandLineStr */







void ParameterSearch::ReadProblem ()
{
  bool  successfull;
  bool  changesMade = false;

  if  (!images)
  {
    images = LoadFeatureFile (inputFileName, fileFormat, imageClasses, -1, log, cancelFlag, successfull, changesMade);
    if (!successfull)
    {
      log.Level (-1) << endl
                     << " *****  Can not open input file[" << inputFileName << "]  *****" 
                     << endl;
      EndBlock ();
      exit (1);
      osWaitForEnter ();
    }

    imageClasses.SortByName ();
  }



  if  ((!testExamples)  &&  (!testFileName.Empty ()))
  {
    testExamples = LoadFeatureFile (testFileName, fileFormat, imageClasses, -1, log, cancelFlag, successfull, changesMade);
    if  (!successfull)
    {
      log.Level (-1) << endl
                     << " *****  Can not TestFileName[" << testFileName << "]  *****" 
                     << endl;
      EndBlock ();
      exit (1);
      osWaitForEnter ();
    }

    //imageClasses.SortByName ();
  }



  if  (!config)
  {
    if  (configFileName.Empty ())
    {
      log.Level (-1) << endl
                     << "ParameterSearch::ReadProblem  ConfigFile not specified." << endl
                     << "                              -Config  <file name>"     << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

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
  }

  imageClasses.SortByName ();

  // Lets Normalize the Data.
  NormalizationParms normParmns (config, *images, log);
  normParmns.NormalizeImages (images);
  if  (testExamples)
    normParmns.NormalizeImages (testExamples);
}  /* ReadProblem */






void  ParameterSearch::ProcessStatusFile (int     argc,  
                                          char**  argv
                                         )
{
  Block ();
  
  statusFile = osFOPEN (statusFileName, "r");
  if  (!statusFile)
  {
    //  We are doing our initial StartUp.  So lets set up the Status file.
    ParseCommandLine (argc, argv);

    ReadProblem ();

    nextJobId      = 0;
    ourProcessNum  = 0;
    numOfProcesses = 1;

    nextC          = 1;
    nextGamma      = GammaMin;
    nextA          = 100.0f;

    curJob         = NULL;

    processes = new ParameterJobList (true);
    processes->PushOnBack (new ParameterJob (log, ourProcessNum, -1, -1, -1.0, -1));
    UpdateCurrentStatusFile ();

    resultFileName = osGetRootNameWithExtension (inputFileName) + ".result";

    // Initialize Result File  
    ofstream  resultFile (resultFileName.Str (), ios::app);
    resultFile << "JobId\t";

    if  (multiClassClassifiers)
      resultFile << "ALLCLASSES"  << "\t";
    else
      resultFile << "Class1"      << "\t"
                 << "Class2"      << "\t";

    resultFile << "C_Parm"         << "\t"
               << "Gamma_Parm"     << "\t"
               << "A_Parm"         << "\t"
               << "Accuracy"       << "\t" 
               << "Num_SV"         << "\t"
               << "Avg_PredProb"   << "\t"
               << "Avg_KnownProb"  << "\t"
               << "Compact"        << "\t" 
               << "CompactDev"     << "\t"
               << "Time"           << "\t"
               << "TrainTime"      << "\t"
               << "ClassTime"
               << endl;

    resultFile.close ();
  }

  else
  {
    LoadCurrentStatusFile ();

    ourProcessNum = numOfProcesses;
    numOfProcesses++;
    processes->PushOnBack (new ParameterJob (log, ourProcessNum, -1, -1, -1.0, -1));

    UpdateCurrentStatusFile ();

    ReadProblem ();
  }  

  EndBlock ();

  resultFileName = inputFileName;
  resultFileName << ".result";
}  /* ProcessStatusFile */







uint  GetCostRow (vector<int>&  costNums,
                  int           cParm
                 )
{
  uint  x;

  for  (x = 0;  x < costNums.size ();  x++)
  {
    if  (costNums[x] == cParm)
      return  x;
  }

  costNums.push_back (cParm);
  return  costNums.size () - 1;

}  /* GetCostRow */



uint  GetGammaRow (vector<float>&  gammaNums,
                   float           gamma
                  )
{
  uint  x;

  for  (x = 0;  x < gammaNums.size ();  x++)
  {
    if  (gammaNums[x] == gamma)
      return  x;
  }

  gammaNums.push_back (gamma);
  return  gammaNums.size () - 1;

}  /* GetGammaRow */






void  ParameterSearch::MakeGammaCostReport (KKStr  resultFileName,
                                            KKStr  reportFileName
                                           )
{
  vector<int>    costNums;
  vector<float>  gammaNums;

  ImageClassList  classes;

  ParameterSearchReportLineListPtr results = new  ParameterSearchReportLineList (resultFileName, &classes, log);

  results->SortByGammaCost ();

  uint  x, y, z;

  for  (x = 0;  x < (uint)results->QueueSize ();  x++)
  {
    GetCostRow  (costNums,  results->IdxToPtr (x)->CParm ());
    GetGammaRow (gammaNums, results->IdxToPtr (x)->Gamma ());
  }

  sort (costNums.begin (),  costNums.end ());
  sort (gammaNums.begin (), gammaNums.end ());
  
  float**  accuracies = new float*[costNums.size ()];
  float**  times      = new float*[costNums.size ()];
  float**  svs        = new float*[costNums.size ()];

  int  stepSize = 3;

  for  (x = 0;  x < costNums.size (); x++)
  {
    accuracies[x] = new float[gammaNums.size ()];
    times     [x] = new float[gammaNums.size ()];
    svs       [x] = new float[gammaNums.size ()];

    for  (y = 0; y < gammaNums.size ();  y++)
    {
      accuracies[x][y] = 0.0;
      times     [x][y] = 0.0f;
      svs       [x][y] = 0.0f;
    }
  }

  for  (z = 0;  z < (uint)results->QueueSize ();  z++)
  {
    ParameterSearchReportLinePtr  rl = results->IdxToPtr (z);
    int  costRow  = GetCostRow  (costNums,  rl->CParm ());
    int  gammaRow = GetGammaRow (gammaNums, rl->Gamma ());
    
    accuracies[costRow][gammaRow] = rl->Accuracy ();
    times     [costRow][gammaRow] = rl->Time ();
    svs       [costRow][gammaRow] = rl->NumSVs ();
  }


  ofstream r (reportFileName.Str ());

  for  (y = 0;  y < gammaNums.size ();  y += stepSize)
    r << "\t" << gammaNums[y];
  r << endl;

  for  (x = 0;  x < costNums.size ();  x++)
  {
    r << costNums[x];
    for  (y = 0;  y < gammaNums.size ();  y += stepSize)
    {
      r << "\t";
      if  (accuracies[x][y] > 0.0f)
        r << accuracies[x][y] << "%";
    }
    r << endl;
  }


  r << endl << endl << endl;
  r << "CPU Time for 5 Fold Cross Vvalidation" << endl;
  for  (x = 0;  x < costNums.size ();  x++)
  {
    r << costNums[x];
    for  (y = 0;  y < gammaNums.size ();  y += stepSize)
    {
      r << "\t";
      if  (times[x][y] > 0.0f)
        r << times[x][y];
    }
    r << endl;
  }


  r << endl << endl << endl;
  r << "Support Vectors" << endl;
  for  (x = 0;  x < costNums.size ();  x++)
  {
    r << costNums[x];
    for  (y = 0;  y < gammaNums.size ();  y += stepSize)
    {
      r << "\t";
      if  (svs[x][y] > 0.0f)
        r << svs[x][y];
    }
    r << endl;
  }



  r.close ();


  for  (x = 0;  x < costNums.size (); x++)
  {
    delete  accuracies[x];
    delete  times     [x];
  }
  
  delete  accuracies;
  delete  times;

  delete  results;
}  /* MakeGammaCostReport */





void  ParameterSearch::GetGoodParametersForTwoClasses (ImageClassPtr                     class1,
                                                       ImageClassPtr                     class2,
                                                       ParameterSearchReportLineListPtr  results,
                                                       ostream&                          r,
                                                       ostream&                          c
                                                      )
{
  r << class1->Name () << "\t" << class2->Name () << endl;

  results->SortByAccuracy ();

  ParameterSearchReportLineList::iterator  idx;

  float  highestAccuracy = 0.0f;
  ParameterSearchReportLinePtr  oneWithGoodParameters = NULL;

  for  (idx = results->begin ();  idx != results->end ();  idx++)
  {
    ParameterSearchReportLinePtr rl = *idx;
    r << (*rl) << endl;

    if  (rl->Accuracy () > highestAccuracy)
    {
      highestAccuracy = rl->Accuracy ();
      oneWithGoodParameters = rl;
    }
  }

  float  minAcceptableAcc = highestAccuracy - 1.01f;

  float  shortestTime = FLT_MAX;
  for  (idx = results->begin ();  idx != results->end ();  idx++)
  {
    ParameterSearchReportLinePtr rl = *idx;
    if  (rl->Accuracy () > minAcceptableAcc)
    {
      if  (rl->Time () < shortestTime)
        shortestTime = rl->Time ();
    }
  }
  float  maxAcceptableTime = shortestTime * 1.3f;
  float  smallestNumSVs = FLT_MAX;

  // Now locate best time with in minAcceptableAcc
  for  (idx = results->begin ();  idx != results->end ();  idx++)
  {
    ParameterSearchReportLinePtr rl = *idx;

    if  ((rl->Accuracy () > minAcceptableAcc)  &&  (rl->Time () < maxAcceptableTime))
    {
      if  (rl->NumSVs () < smallestNumSVs)
      {
         smallestNumSVs = rl->NumSVs ();
         oneWithGoodParameters = rl;
      }
    }
  }




  r << endl
    << "Best Parameters Found   JobId[" << oneWithGoodParameters->JobId () << "]"
    << "  C[" << oneWithGoodParameters->CParm ()     << "]"
    << "  Gamma[" << oneWithGoodParameters->Gamma () << "]"
    << endl
    << endl
    << endl;

  // We now have job with good parameters
  FeatureNumList  features = config->GetFeatureNums (class1, class2);
  c << "[TWO_CLASSPARAMETERS]"        << endl;
  c << "// JobId[" << oneWithGoodParameters->JobId () << "]" << endl;
  c << "CLASS1="             << class1->Name ()       << endl;
  c << "CLASS2="             << class2->Name ()       << endl;
  c << "FEATURES_INCLUDED="  << features.ToString ()  << endl;
  c << "Parameters=-s 0 -n 0.11 -t 2 "
    <<            " -g " << StrFormatDouble (oneWithGoodParameters->Gamma (), "zz0.00000")
    <<            " -C " << oneWithGoodParameters->CParm ()
    <<            " -u 100  -up  -mt OneVsOne  -sm P"
    << endl
    << endl;


}  /* GetGoodParametersForTwoClasses */




void  ParameterSearch::ExtractGoodParamters (KKStr  resultFileName,
                                             KKStr  reportFileName,
                                             KKStr  configFileName
                                            )
{
  ImageClassList  classes;

  ParameterSearchReportLineListPtr results = new  ParameterSearchReportLineList (resultFileName, &classes, log);

  ofstream r (reportFileName.Str ());
  ofstream c (configFileName.Str ());

  int  numOfClasses = classes.QueueSize ();

  for  (int  class1Idx = 0;  class1Idx < (numOfClasses - 1);  class1Idx++)
  {
    ImageClassPtr  class1 = classes.IdxToPtr (class1Idx);

    for  (int class2Idx = class1Idx + 1;  class2Idx < numOfClasses;  class2Idx++)
    {
      ImageClassPtr  class2 = classes.IdxToPtr (class2Idx);

      ParameterSearchReportLineListPtr  twoClassResults = results->ExtractTwoClasses (class1, class2);
       
      GetGoodParametersForTwoClasses (class1, class2, twoClassResults, r, c);

      delete  twoClassResults;
    }
  }

  c.close ();
  r.close ();
  delete  results;
}  /* ExtractGoodParamters */




int  main (int argc, char **argv)
{
  KKStr  sFN = "NineClasses_ValidationData.data.result";
  KKStr  rFN = "NineClasses_ValidationData.data.report.txt";
  KKStr  cFN = "NineClasses_ValidationData.Config.txt";

//  ParameterSearch*  parmSearch2 = new ParameterSearch ();
//  parmSearch2->ParseCommandLine (argc, argv);
//  //parmSearch2->MakeGammaCostReport (sFN, rFN, cFN);
//  parmSearch2->ExtractGoodParamters (sFN, rFN, cFN);
//  exit (0);


  ParameterSearch*  parmSearch = new ParameterSearch ();
  {
    parmSearch->ProcessStatusFile (argc, argv);
    parmSearch->LetsDoIt ();
  }

  delete  parmSearch;

  return 0;
}
