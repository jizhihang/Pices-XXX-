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
//*    2   <Num of Classes>List of all Classes                                  *
//*    3   next class idx's to perform  <class0 idx>  <class1 idx>              *
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
//*       <Class 0 IDX>  These two fields are only used when searching for      *
//*       <Class 1 IDX>  optimal parameters by two class combos.                *
//*                                                                             *
//*                                                                             *
//*                                                                             *
//*                                                                             *
//*                                                                             *
//*                                                                             *
//*******************************************************************************





#include <stdlib.h>
#include <stdio.h>

#include  <ctype.h>
#include  <time.h>

#include  <fcntl.h>

#include  <string>
#include  <iomanip>
#include  <iostream>
#include  <fstream>
//#include  <ostream>
#include  <vector>


using namespace std;


#include <sys/types.h>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
#include  <sys/loadavg.h>
#include <unistd.h>
#endif


#include  "ParameterSearchBinaryCombo.h"

#include  "OSservices.h"
#include  "ParameterJob.h"
#include  "SVMparam.h"




// -X  5,6,10,14,15,17,21,22,24,27,28  -L 1  -a 1  -b 100  -f 10  -s 1  -t 2  -d 3.0  -g 0.010  -r 0.0  -n 0.20  -m 40.0  -c 1.0  -e 0.0010  -p 0.10  -h 1  -v 10  -z BestCaseOut.txt  TEST_OCT_24_80p.data

// -I 0,1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,19,20,22,23,24,25,26,28   -L 1  -a 1  -b 100  -f 10  -s 1  -t 2  -d 3.0  -g 0.010  -r 0.0  -n 0.20  -m 40.0  -c 1.0  -e 0.0010  -p 0.10  -h 1  -v 5  -z K:\Plankton\StatusReports\2003_Mar_18\NextBestBack_26.txt  TEST_OCT_24_80p.data  



// ParameterSearchBinaryCombo   -SEARCHTYPE BESTFIRST  -el 150  -s 0 -n 0.11 -t 2 -g 0.032  -c 300 -v 5 -l 1 -z  GrayScaleCpp_April_16_03.txt K:\Plankton\TempBeamSearch\GrayScaleCpp_April_16_03.data
//                                                 -s 0 -n 0.11 -t 2 -g 0.032  -c 300


//  -SEARCHTYPE BESTFIRST  -ALGORITHM  c45  -el 150  -v 5 -l 1 -z  TEST_OCT_24_80p_NeuroC45.txt  K:\Plankton\TempBeamSearch\BestCase\TEST_OCT_24_80p.data


// ParameterSearchBinaryCombo  -el 150  -SEARCHTYPE BEAM  -L 5  -a 1  -b 100  -f 10  -s 0  -t 2  -d 3  -g 0.032  -r 0  -n 0.11  -m 40  -c 300  -e 0.001  -p 0.1  -h 1  -v 5     -z GrayScaleCpp_New_Report.txt  K:\\Plankton\\TempBeamSearch\\GrayScale_May_13_03\\GrayScaleCpp_2003-May-13.data




// ParameterSearchBinaryCombo  -v 5   -a 1  -b 100  -f 10  -s 0  -t 2  -d 3  -g 0.032  -r 0  -n 0.11  -m 40  -c 300  -e 0.001  -p 0.1  -h 1  -i 1,9,17,18,19,21,28,29,32,35,40,41,42,45,46 -z For_15_Features.txt  GrayScaleCpp_April_19_03.data



// ParameterSearchBinaryCombo -v 5  -s 0 -n 0.11 -t 2 -g 0.032  -c 300  -u 1500  2004_03_02_TestData.data


// -v 5  -s 0   -n 0.11   -t 2   -g 0.032    -c 300    -u 1500  -mt OneVsAll  K:\\Plankton\\StatusReports\\2004-02-10\\allnorm_Sorted.data

//  -v 5 -s 0 -n 0.11 -t 2 -g 0.032 -c 300 -u 1500 -mt OneVsAll   K:\v1\Plankton\TempBeamSearch\2004_March_02\2004_03_02_TestData.data

int   lockFile = 0;

bool  lockFileOpened = false;

const char*  lockFileName   = "ParameterSearchBinaryCombo.lock";

const char*  statusFileName = "ParameterSearchBinaryCombo.status";


int     initC      = 1;
FFLOAT  initGamma  = (FFLOAT)0.00001;
int     initA      = 100;


ParameterSearchBinaryCombo::ParameterSearchBinaryCombo ():
  log            (),
  images         (true, log, 1000),
  imageClasses   (true, log),
  svmParamaters  (NULL),

  highestAccuracySoFar (0.0),

  ourProcessNum        (-1),

  numOfProcesses       (-1),

  processes            (NULL),
  numOfClasses         (0),
  masterClassList      (NULL),

  nextClass0IDX        (-1),
  nextClass1IDX        (-1)

{
  log.Level (10) << "ParameterSearchBinaryCombo::ParameterSearchBinaryCombo" 
                 << endl;
  time_t  curTime;
  srand (time (&curTime));

  #ifndef  WIN32
  weWereSleeping = true;
  #endif;

}



ParameterSearchBinaryCombo::~ParameterSearchBinaryCombo ()
{
  if  (svmParamaters)
    delete svmParamaters;

  if  (processes)
    delete  processes;

  delete  masterClassList;
}





#ifndef  WIN32
double  ParameterSearchBinaryCombo::LoadAverage ()
{
  log.Level (50) << "ParameterSearchBinaryCombo::LoadAverage" << endl;

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




void  ParameterSearchBinaryCombo::Block ()
{
  log.Level (20) << "ParameterSearchBinaryCombo::Block - Entering." << endl;
  
  int  count = 0;

  do  {
    lockFile = open (lockFileName, O_WRONLY | O_CREAT | O_EXCL);

    if  (lockFile < 0)
    {
      count++;
      log.Level (20) << "ParameterSearchBinaryCombo::Block - We are locked out[" << count << "]."  << endl;
      #ifdef  WIN32
      Sleep (5000);
      #else
      sleep (5);
      #endif
    }
  }  while  (lockFile < 0);

  lockFileOpened = true;

  log.Level (20) << "ParameterSearchBinaryCombo::Block - Lock is Established." << endl;
}



void  ParameterSearchBinaryCombo::EndBlock ()
{              
  #ifndef  WIN32
  int  returnCd;
  #endif
  
  log.Level (20) << "ParameterSearchBinaryCombo::EndBlock - Ending Block" << endl;

  if  (!lockFileOpened)
  {
    log.Level (-1) << "ParameterSearchBinaryCombo::EndBlock - Lock file is not opened." << endl;
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
       log.Level (-1) << "ParameterSearchBinaryCombo::EndBlock - *** ERROR *** Could not set Lock File  to  Normal." << endl;
     }
     else
     {
       if  (!DeleteFile (lockFileName))
       {
         DWORD  lastErrorNum = GetLastError ();
         log.Level (-1) << "ParameterSearchBinaryCombo::EndBlock - Error["  << lastErrorNum << "] deleting Lock File." << endl;
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



void  ParameterSearchBinaryCombo::exit_with_help ()
{
  DisplayCommandUssage ();
  exit(1);
}






void  ParameterSearchBinaryCombo::LoadCurrentStatusFile ()
{
  log.Level (10) << "ParameterSearchBinaryCombo::LoadCurrentStatusFile - Loading current Status File." << endl;

  char  buff[10240];

  statusFile = fopen (statusFileName, "r");
  if  (!statusFile)
  {
     log.Level (-1) << endl
                    << "ParameterSearchBinaryCombo::LoadCurrentStatusFile - *** ERROR ***  Can not open Status File[" 
                    << statusFileName << "]." 
                    << endl;
     EndBlock ();
     exit (1);

     osWaitForEnter ();
  }

  {
    fgets (buff, sizeof (buff), statusFile);
    String  cmdLine (buff);
    cmdLine.TrimRight ();
    ProcessCmdLineStr  (cmdLine);
  }

  {
    // If images are not loadted then we need 2 load trhem now.

    if  (images.QueueSize () < 1)
    {
      ReadProblem ();
    }
  }


  {
    fgets (buff, sizeof (buff), statusFile);
    String  buffStr (buff);

    numOfProcesses       = buffStr.ExtractTokenInt    (" ,\n\t");
    highestAccuracySoFar = buffStr.ExtractTokenDouble (" ,\n\t");
  }


  {
    // Get Master Class List
    delete  masterClassList;

    fgets (buff, sizeof (buff), statusFile);
    String  buffStr (buff);

    numOfClasses = buffStr.ExtractTokenInt    (" ,\n\t");
    masterClassList = new ImageClassPtr[numOfClasses];
    for  (int idx = 0;  idx < numOfClasses;  idx++)
    {
      String  className = buffStr.ExtractToken (" ,\n\t");
      masterClassList[idx] = imageClasses.LookUpByName (className);
      if  (!masterClassList[idx])
      {
        cerr << endl
             << "*** ERROR ***,   Class[" << className << "]  is not one of the classes in the data file." << endl
             << endl;
        exit (-1);
      }
    }
  }


  {
    // Lets get next Class IDX's to perform

    fgets (buff, sizeof (buff), statusFile);
    String  buffStr (buff);

    nextClass0IDX = buffStr.ExtractTokenInt    (" ,\n\t");
    nextClass1IDX = buffStr.ExtractTokenInt    (" ,\n\t");
  }

  
  if  (processes)
  {
    delete  processes;
    processes = NULL;
  }

  processes = new ParameterProcessList (true);

  for  (int  processLineNum = 0;  processLineNum < numOfProcesses;  processLineNum++)
  {
    fgets (buff, sizeof (buff), statusFile);
    String  buffStr (buff);
    processes->PushOnBack (new ParameterProcess (log, imageClasses, buffStr));
  }

  fclose (statusFile);
  statusFile = NULL;
  return;
}  /* LoadCurrentStatusFile */






void  ParameterSearchBinaryCombo::UpdateCurrentStatusFile ()
{
  log.Level (20) << "ParameterSearchBinaryCombo::UpdateCurrentStatusFile - Updating  Current Status File." << endl;

  int  openAttempts = 0;

  statusFile = NULL;

  while  ((!statusFile)  &&  (openAttempts < 3))
  {
    openAttempts++;
    log.Level (50) << "UpdateCurrentStatusFile Oppen Attempt[" << openAttempts << "]." << endl;
    statusFile = fopen (statusFileName, "w");
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


  String  s;
  s = CommandLineStr ();
  fprintf (statusFile, "%s\n", s.Str ());

  fprintf (statusFile, "%d\t %f\n", 
                        numOfProcesses,
                        highestAccuracySoFar
          );


  {
    // Generate Master Class List Line.

    String  masterClassListLine;
    masterClassListLine << numOfClasses;

    for  (int idx = 0;   idx < numOfClasses;  idx++)
    {
      masterClassListLine << "\t" << masterClassList[idx]->Name ();
    }

    fprintf (statusFile, "%s\n", masterClassListLine.Str ());
  }


  {
    // Write Out Next Class IDX's
    fprintf (statusFile, "%d\t%d\n", nextClass0IDX, nextClass1IDX);
  }





  ParameterProcessListIterator  jIDX (*processes);
  ParameterProcessPtr           process;

  for  (jIDX.Reset ();  process = jIDX.CurPtr ();  ++ jIDX)
  {
    fprintf (statusFile, "%s\n", process->ToString ().Str ());
  }


  fclose  (statusFile);
  statusFile = NULL;

  log.Level (50) << "UpdateCurrentStatusFile - Exiting." << endl;
}  /* UpdateCurrentStatusFile */






String  CurTime ()
{
  time_t  curTime;

  curTime = time (&curTime);

  return (String (ctime (&curTime)));
}






void  ParameterSearchBinaryCombo::ProcessNextJob (ParameterProcessPtr   curProcess,
                                                  ParameterJobPtr       curJob,
                                                  ImageFeaturesListPtr  twoClassImages,
                                                  ImageClassListPtr     twoClasses
                                                 )
{
  time_t  elaspedTime;
  time_t  endTime;
  time_t  startTime;

  log.Level (20) << "ProcessNextJob - Processing Job[" << curJob->JobId () << "]."<< endl;

  ConfussionMatrix2 results (*twoClasses, log);

  double  numOfSupportVectors;
  double  avgProbability;
  double  avgKnowClassProb;
  double  avgCompact;
  double  compactDev;

  startTime = time (&startTime);

  curJob->CrossValidation (*twoClassImages,
                           *twoClasses,
                           svmParamaters->ToString (),
                           nr_fold,
                           results,
                           numOfSupportVectors,
                           avgProbability,
                           avgKnowClassProb,
                           avgCompact,
                           compactDev
                          );

  endTime = time (&endTime);

  elaspedTime = endTime - startTime;

  log.Level (20) << CurTime () << ": "
                 << "ParameterSearchBinaryCombo::ProcessNextJob - Job[" << curJob->JobId () 
                 << "]  is done."
                 << endl;

  Block ();
  LoadCurrentStatusFile ();

  curProcess->ImcrementParameters (curJob);

  {
    // Update Process Table
    ParameterProcessPtr  curProcessOnFile = processes->LocateByProcessNum (ourProcessNum);
    if  (!curProcessOnFile)
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
  
    processes->DeleteEntry (curProcessOnFile);
    delete  curProcessOnFile;
    curProcessOnFile = NULL;
    processes->PushOnBack (new ParameterProcess (*curProcess));
  }


  double  accuracy = curJob->Accuracy ();

  bool  newHigh = false;

  if  (accuracy > highestAccuracySoFar)
  {
    highestAccuracySoFar = accuracy;
    newHigh = true;
  }


  ofstream  resultFile (resultFileName.Str (), ios::app);

  resultFile <<          "JobId\t"            << curJob->JobId ();
  resultFile << "\t"  << "Class0\t"           << curProcess->Class0 ()->Name ();
  resultFile << "\t"  << "Class1\t"           << curProcess->Class1 ()->Name ();
  resultFile << "\t"  << "C_Parm\t"           << curJob->CParm ();
  resultFile << "\t"  << "Gamma_Parm\t"       << StrFormatDouble (curJob->GammaParm (), "-zz0.00000");
  resultFile << "\t"  << "A_Parm\t"           << curJob->AParm ();
  resultFile << "\t"  << "Accuracy\t"         << StrFormatDouble (curJob->Accuracy (), "-zz0.00") << "%";
  resultFile << "\t"  << "Num_SV\t"           << numOfSupportVectors;
  resultFile << "\t"  << "Avg_PredProb\t"     << StrFormatDouble (100.0 * avgProbability,   "-zz0.00") << "%";
  resultFile << "\t"  << "Avg_KnownProb\t"    << StrFormatDouble (100.0 * avgKnowClassProb, "-zz0.00") << "%";
  resultFile << "\t"  << "Compact\t"          << avgCompact;
  resultFile << "\t"  << "CompactDev\t"       << compactDev;
  resultFile << "\t"  << "Time\t"             << elaspedTime;
  resultFile << "\t"                          << results.AccuracyStr ();

  if  (newHigh)
  {
    resultFile << "\t***";
  }

  resultFile << endl;
  resultFile.close ();

  UpdateCurrentStatusFile ();

  EndBlock ();
}  /* ProcessNextJob */






void  ParameterSearchBinaryCombo::ProcessTwoClassCombo (ParameterProcessPtr  curProcess)
{
  log.Level (10)  << "ParameterSearchBinaryCombo::ProcessClassCombo - Started" << endl;

  ImageFeaturesListPtr  twoClassImages = new ImageFeaturesList (false, log, 5000);

  {
    ImageFeaturesPtr  image = NULL;

    ImageFeaturesListIterator  idx (images);

    for  (idx.Reset ();  image = idx.CurPtr ();  ++idx)
    {
      if  ((image->ImageClass () == curProcess->Class0 ())  ||
           (image->ImageClass () == curProcess->Class1 ())
          )
      {
        twoClassImages->PushOnBack (image);
      }

    }
  }


  ImageClassListPtr  twoClasses = new ImageClassList (false, log);
  twoClasses->PushOnBack (curProcess->Class0 ());
  twoClasses->PushOnBack (curProcess->Class1 ());

  bool  weAreAllDone = false;

  while  (!weAreAllDone)
  {
    #ifndef  WIN32
    double  la;

    if  (LoadAverage () > 7.2)
    {
      while  ((la = LoadAverage ()) > 6.6)
      {
        // The system is pretty Busy so to be Fair to everyone else lets go to sleep 
        // for a while.
        log.Level (20) << "ParameterSearchBinaryCombo::LetsDoIt - Load Average[" << la << "] is greater than 4.8." << endl;
        log.Level (20) << "                          Will go to sleep for one minute and try again."  << endl;
        weWereSleeping = true;

        int  randSleepTime = rand () % 20;

        sleep (20 + randSleepTime);
      }
    }

    weWereSleeping = false;
    #endif

    ParameterJobPtr  curJob = new ParameterJob (log, 
                                                ourProcessNum, 
                                                curProcess->JobId     (),
                                                curProcess->CParm     (),
                                                curProcess->GammaParm (), 
                                                curProcess->AParm     ()
                                               );

    ProcessNextJob (curProcess, curJob, twoClassImages, twoClasses);

    delete  curJob;
    curJob = NULL;

    if  (curProcess->CParm () > 17000)
    {
      weAreAllDone = true;
    }
  }


  delete  twoClasses;
  delete  twoClassImages;
} /* ProcessTwoClassCombo */









void  ParameterSearchBinaryCombo::LetsDoIt ()
{
  log.Level (10)  << "ParameterSearchBinaryCombo::LetsDoIt - Started" << endl;

  bool  weAreAllDone = false;

  while  (!weAreAllDone)
  {
    Block ();

    LoadCurrentStatusFile ();

    if  (nextClass0IDX >= numOfClasses)
    {
      weAreAllDone = true;
      EndBlock ();
      break;
    }

    int  jobId = 0;

    ParameterProcessPtr  j = processes->LocateByProcessNum (ourProcessNum);
    if  (j)
    {
      processes->DeleteEntry (j);
      jobId = j->JobId ();
      delete  j;
      j = NULL;

    }

    ParameterProcessPtr  curProcess = new ParameterProcess (log, 
                                                            ourProcessNum, 
                                                            jobId, 
                                                            masterClassList[nextClass0IDX],
                                                            masterClassList[nextClass1IDX],
                                                            initC,
                                                            initGamma,
                                                            initA
                                                           );

    curProcess->CurStatus ('0');

    processes->PushOnBack (new ParameterProcess (*curProcess));

    nextClass1IDX++;
    if  (nextClass1IDX >= numOfClasses)
    {
      nextClass0IDX++;
      nextClass1IDX = nextClass0IDX + 1;
    }


    UpdateCurrentStatusFile ();
    EndBlock ();

    ProcessTwoClassCombo (curProcess);

    delete  curProcess;
    curProcess = NULL;
  }
} /* LetsDoIt */







void  ParameterSearchBinaryCombo::ParseCommandLine (int    argCount, 
                                                    char **arguments
                                                   )
{
  int i;

  String  svmCmdLineStr = "";

  nr_fold    = 2;

  // parse options
  for  (i = 1; i < argCount;  i++)
  {
    bool addToSvmCmdLineStr = false;

    if  (arguments[i][0] != '-')  
        break;

    String switchStr = arguments[i];
    switchStr.Upper ();

    ++i;

    if  (switchStr == "-V") 
    {
      nr_fold = atoi (arguments[i]);

      if  (nr_fold < 2)
      {
        cerr << "n-fold cross validation: n must >= 2"  << endl;
        exit_with_help ();
      }
    }

    else
    {
      addToSvmCmdLineStr = true;
    }


    if  (addToSvmCmdLineStr)
    {
      if  (!svmCmdLineStr.Empty ())
           svmCmdLineStr << "  ";

      svmCmdLineStr << arguments[i - 1] << " " << arguments[i];
    }
  } /* End of For */



  if  (svmParamaters)
    delete  svmParamaters;

  bool validFormat;

  FeatureNumList selectedFeatures (IncludeFeatureNums, String ("8,9,11,15,16,17,18,19,20,21,22,24,25,26,28,31,32,35,39,40,41,42,43,45,46,49,50,51,52,53,54,56"));
  svmParamaters = new SVMparam (svmCmdLineStr, selectedFeatures, log, validFormat);

  if  ((!svmParamaters)  ||  (!validFormat))
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

  if  (i >= argCount)
    exit_with_help ();

  inputFileName = arguments[i];

  // inputFileName = "allnorm_Sorted.data";

}  /* ParseCommandLine */






String  ParameterSearchBinaryCombo::CommandLineStr  ()
{
  log.Level (50) << "ParameterSearchBinaryCombo::CommandLineStr - Entered." << endl;

  if  (!svmParamaters)
  {
    log.Level (-1) << "ParameterSearchBinaryCombo::CommandLineStr - *** ERROR***,  svmParamaters == NULL." << endl;
    osWaitForEnter ();
    exit (-1);
  }

  String  cmdStr;
  String  svmParmStr;

  svmParmStr = svmParamaters->ToString ();

  cmdStr << svmParmStr        << "  ";
  cmdStr << "-v " << nr_fold  << "  ";
  cmdStr << inputFileName;

  log.Level (60) << "ParameterSearchBinaryCombo::CommandLineStr - Exiting." << endl;

  return  cmdStr;
}  /* CommandLineStr */








void  ParameterSearchBinaryCombo::ProcessCmdLineStr  (String  cmdLine)
{
  char*  arguments[60];
  int    argCount = 0;

  arguments[0] = strdup ("ParameterSearchBinaryCombo");
  argCount++;

  while  (!cmdLine.Empty ())
  {
    String  parmField = cmdLine.ExtractToken (" ");
    arguments[argCount] = strdup (parmField.Str ());
    argCount++;
  }

  ParseCommandLine (argCount, arguments);
}  /* CommandLineStr */







void ParameterSearchBinaryCombo::ReadProblem ()
{
  bool  successfull;

  images.LoadImageFeatures (inputFileName, RawFormat, imageClasses, successfull);
  if (!successfull)
  {
    log.Level (-1) << endl
                   << " *****  Can not open input file[" << inputFileName << "]  *****" 
                   << endl;
    EndBlock ();
    exit (1);
    osWaitForEnter ();
  }



  // Lets Normalize the Data.

  /*
  NormalizationParms normParmns (images, log);

  {
    ImageFeaturesListIterator  imagesIDX (images);

    for (imagesIDX.Reset (); imagesIDX.CurPtr (); ++imagesIDX)
    {
      imagesIDX->Normalize (normParmns, log, successfull);
    }
  }
  */
}  /* ReadProblem */






void  ParameterSearchBinaryCombo::ProcessStatusFile (int     argc,  
                                                     char**  argv
                                                    )
{
  Block ();
  
  statusFile = fopen (statusFileName, "r");
  if  (!statusFile)
  {
    //  We are doing our initial StartUp.  So lets set up the Status file.
    ParseCommandLine (argc, argv);

    ReadProblem ();

    ourProcessNum  = 0;
    numOfProcesses = 1;

    nextClass0IDX  = 0;
    nextClass1IDX  = 1;

    // Since the status file did not exist before we must be starting 
    // scratch,  because of this we can setup the master class list 
    // now.

    {
      imageClasses.SortByName ();
      numOfClasses = imageClasses.QueueSize ();

      masterClassList = new ImageClassPtr[numOfClasses];
      for  (int idx = 0;  idx < numOfClasses;  idx++)
        masterClassList[idx] = imageClasses.IdxToPtr (idx);
    }

    processes = new ParameterProcessList (true);
    processes->PushOnBack (new ParameterProcess (log, ourProcessNum, -1, NULL, NULL, -1, -1.0, -1));

    UpdateCurrentStatusFile ();
  }

  else
  {
    fclose (statusFile);

    LoadCurrentStatusFile ();

    ourProcessNum = numOfProcesses;
    numOfProcesses++;
    processes->PushOnBack (new ParameterProcess (log, ourProcessNum, 0, NULL, NULL, -1, -1.0, -1));

    UpdateCurrentStatusFile ();
  }  

  EndBlock ();

  resultFileName = inputFileName;
  resultFileName << ".result";
}  /* ProcessStatusFile */






int  main (int argc, char **argv)
{
  ParameterSearchBinaryCombo*  parmSearch = new ParameterSearchBinaryCombo ();
  {
    parmSearch->ProcessStatusFile (argc, argv);
    parmSearch->LetsDoIt ();
  }

  delete  parmSearch;

  return 0;
}
