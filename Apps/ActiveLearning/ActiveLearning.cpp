#include  "FirstIncludes.h"

#include  <fcntl.h>
#include  <stdio.h>
#include  <time.h>
#include  <math.h>

#include  <iomanip>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <algorithm>
#include  <functional>

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



#include  "Classifier2.h"
#include  "ConfusionMatrix2.h"
#include  "FeatureFileIO.h"
#include  "FeatureFileIOPices.h"
#include  "HashTable.h"
#include  "FeatureVector.h"
#include  "OSservices.h"
#include  "TrainingConfiguration2.h"
#include  "TrainingProcess2.h"
using namespace MLL;


#include  "JobDesc.h"
#include  "PassAssignments.h"
#include  "ActiveLearning.h"





// -C FiveBigest.cfg  -G FiveBigest_AllOrders_200-IPC_01-IPR.txt  -I 10 -J 50 -q 30  -M ALL -R Test.txt 


//  -C FiveBigest.cfg  -G FiveBigest_Retraining050_Passes030.txt  -I 5  -J 0  -K 10 -M ALL -R Report_10-IPC-ITL_1-IPR.txt  -S  C:\ActiveLearningTest\TestImages -T C:\ActiveLearningTest\InitialTrainingSet


// -c FourBigest.cfg  -o  Probability  -g FiveBigest -M All -i 5 -k 10  -T C:\ActiveLearningTest\InitialTraningSetBinary

// -c ThreeBigest.cfg  -o  Probability  -g ThreeBigest -M All -i 5 -k 3  



// -C FiveBigest.cfg  -E ActiveLearningResults  -I 200  -R AllOrders_200-IPC_01-IPR.txt  
//  -C FiveBigest.cfg  -E ActiveLearningResults  -I 10  -J 1200  k 2   -R ActiveLearningReport.txt  -Q 30  -S  C:\ActiveLearningTest\TestImages -T /home/vision2/student/kkramer/v1/Plankton/ActiveLearning/ActiveLearningTest/InitialTrainingSet

//  2004-09-22
//   -C FiveBigest.cfg  -I 10  -J 1200 -k 10  -q 30  -M ALL


//  2004-10-09
//  -c  Thesis9Classes.cfg -J 1700  -q 30 

//  2004-10-10,   For Test Purposes
//  -c  Thesis9Classes.cfg -J 100  -q 30 


//  2004-11-08,   Letter DataBase
//  -c  26Letters.cfg -J 1600  -q 30 -F UCI

//  2004-12-06,   Letter DataBase,  IPR = 10, 20, 50
//  -c  26Letters.cfg -J 6000  -q 30 -F UCI


//  2005-01-16    Testing Streaming Active Learning
//  C:\users\kkramer\GradSchool\Plankton\ActiveLearning\Results\2004_Sep_Expirements_Streaming
//  -MasterFile ActiveLearning_MasterTestImages.data  -ValidationFile ActiveLearning_ValidationImages.data  -ConfigFile FiveBigest.cfg  -IPR 1,5,10,20  -IIPC 10,50,100  -Q 30  -Stream 100

// 2005-01-17
// K:\v1\Plankton\ActiveLearning\Results\2004_Sep_Expirements_Streaming
//  -MasterFile ActiveLearning_MasterTestImages.data  -ValidationFile ActiveLearning_ValidationImages.data  -ConfigFile FiveBigest.cfg  -IPR 1,5,10,20  -IIPC 10,50,100  -Q 30  -Stream 100


// 2005-01-18
// K:\v1\Plankton\ActiveLearning\Results\2004_Sep_Expirements_Streaming
//  -MasterFile ActiveLearning_MasterTestImages.data  -ValidationFile ActiveLearning_ValidationImages.data  -ConfigFile FiveBigest.cfg  -IPR 1,5,10,20  -IIPC 10,50,100,200  -Q 30  -Stream 100

// 2005-01-24
// K:\v1\Plankton\ActiveLearning\Results\2004_Sep_Expirements_Diff_Prior
//  -MasterFile ActiveLearning_MasterData_Diff_Prior.data  -ValidationFile ActiveLearning_MasterTest_Diff_Prior.data  -ConfigFile FiveBigest.cfg  -IPR 1,5,10,20  -IIPC 10,50,100,200  -Q 30   -j 1200

// 2005-01-27
// K:\v1\Plankton\ActiveLearning\Results\3_Class_ForJDR_Paper\Equal_Prior
//  -MasterFile Data_Equal_Prior.data  -ValidationFile Test_Equal_Prior.data  -ConfigFile FiveBigest.cfg  -IPR 1,5,10,20  -IIPC 10,50,100,200  -Q 30   -j 1200

// 2005-02-06   Different Priors 4 Test and Ijitial Training Set
// K:\v1\Plankton\ActiveLearning\Results\3_Class_ForJDR_Paper\Diff_Prior
// -MasterFile ActiveLearning_MasterData_Diff_Prior.data  -ValidationFile ActiveLearning_MasterTest_Diff_Prior.data  -C FiveBigest.cfg    -IPR 1,5,10,20  -IIPC 10,50,100,200  -J 1000 -Q 30  -S  C:\ActiveLearningTest\TestImages  -F RAW  -USEPRIORFORIIPC



// 2005-02-24
// K:\v1\Plankton\ActiveLearning\Results\2004_Sep_Expirements_1_RandomPass
// -MasterFile ActiveLearning_MasterTestImages.data   -ValidationFile ActiveLearning_ValidationImages.data  -C FiveBigest.cfg    -IPR 1,5,10,20  -IIPC 10,50,100,200  -J 1800 -Q 1  -S  C:\ActiveLearningTest\TestImages  -F RAW  


// 2005-06-08
// C:\users\kkramer\GradSchool\Thesis\TestSet9Classes\1000_ImagesPerClass\NewActiveLearning
// -MasterFile NineClasses_TestData.data  -ValidationFile NineClasses_ValidationData.data  -C Thesis9Classes.cfg    -IPR 1,5,10,20  -IIPC 10,50,100,200  -J 1800 -Q 1  -S  C:\ActiveLearningTest\TestImages  -F RAW  


// 2005-06-25
// I removed the RandomOrder,  ProbabilityOrder, and ProbabilityOrder5 passes for now,  just want
// to get ProbabilityOrder6 pass
// K:\v1\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning_New_BT3
// -MasterFile NineClasses_TestData.data  -ValidationFile NineClasses_ValidationData.data  -C Thesis9Classes.cfg    -IPR 1,5,10,20  -IIPC 10,50,100  -J 1800 -Q 30  -S   -F RAW  


// 2005-10-13
// Want to try   Number of ties  and  (Best - Worst)
// -MasterFile NineClasses_TestData.data  -ValidationFile NineClasses_ValidationData.data  -C Thesis9Classes.cfg  -E ActiveLearningResults  -IPR 1,5,10  -IIPC 10,50,100   -J 1800 -R ActiveLearningReport.txt  -Q 30  -S  C:\ActiveLearningTest\TestImages -T C:\ActiveLearningTest\InitialTrainingSet -F RAW



const char*  lockFileName   = "ActiveLearning.lock";

const char*  statusFileName = "ActiveLearning.status";





KKStr  IntListToStr (vector<int>&  intList)
{
  KKStr  intListStr ((ushort)intList.size () * 5);
  for  (uint x = 0;  x < intList.size ();  x++)
  {
    if  (x > 0)
      intListStr << ",";
    intListStr << intList[x];
  }

  return  intListStr;
}  /* IntListToStr */



vector<int>   ParseIntList (KKStr  intListStr)
{
  vector<int>   intList;
  
  KKStr  intStr = intListStr.ExtractToken (", \n\r\t");
  while  (!intStr.Empty ())
  {
    intList.push_back (atoi (intStr.Str ()));
    intStr = intListStr.ExtractToken (", \n\r\t");
  }
  
  return  intList;
}  /* ParseIntList */




ActiveLearning::ActiveLearning (int     argc,
                                char**  argv
                               ):
  Application                 (argc, argv),
  classProbReport             (NULL),
  fileDesc                    (NULL),
  fileFormat                  (FeatureFileIOPices::Driver ()),
  finalNumOfImagesInTrainingLibrary  (1800),
  mlClasses                (new MLClassList ()),
  jobs                        (NULL),
  lockFileOpened              (false),
  masterTestImages            (NULL),
  masterTrainingConfiguration (NULL),
  numOfProcesses              (-1),
  numOfRandomPasses           (30),
  ourProcessNum               (-1),
  performStreaming            (false),
  report                      (NULL),
  retrainingsPerStreamBlock   (1),
  rootDir                     (),
  streamingBlockSize          (100),
  testImagesRootDir           (),
  trainingImagesRootDir       (),
  testRelativeNonStreamingPosition (true),
  usePriorForIIPC             (false),
  validationImages            (NULL),
  validationImagesPerClass    (200),
  weAreUsingANetworkDrive     (false)
{
  iipcTable.push_back (5);
  iipcTable.push_back (10);
  iipcTable.push_back (20);
  iipcTable.push_back (50);
    
  iprTable.push_back (1);
  iprTable.push_back (5);
  iprTable.push_back (10);
  iprTable.push_back (20);

  // By uncommenting the line below, statustics pertaining to probabilities of genreal pool of data will be recorded,
  // usefull in getting to understand what is happening with different selction methods
  //classProbReport = new ofstream ("ClassProbReport.txt");


  rootDir = osGetCurrentDirectory ();
  DetermineIfOnANetworkDrive ();
  ProcessStatusFile (argc, argv);
}  /* ActiveLearning */




// 2004-12-11   KK
// When you want to only load parameters from the status file.
ActiveLearning::ActiveLearning (KKStr  _rootDir):
  Application                   (0, NULL),
  classProbReport               (NULL),
  fileDesc                      (NULL),
  fileFormat                    (FeatureFileIOPices::Driver ()),
  finalNumOfImagesInTrainingLibrary (1800),
  mlClasses                  (new MLClassList ()),
  jobs                          (NULL),
  lockFileOpened                (false),
  masterTestImages              (NULL),
  masterTrainingConfiguration   (NULL),
  numOfProcesses                (-1),
  numOfRandomPasses             (30),
  ourProcessNum                 (-1),
  performStreaming              (false),
  report                        (NULL),
  retrainingsPerStreamBlock     (1),
  rootDir                       (_rootDir),
  streamingBlockSize            (100),
  testImagesRootDir             (),
  testRelativeNonStreamingPosition (false),
  trainingImagesRootDir         (),
  usePriorForIIPC               (false),
  validationImages              (NULL),
  validationImagesPerClass      (200),
  weAreUsingANetworkDrive       (false)

{
  if  (rootDir.Empty ())
    rootDir = osGetCurrentDirectory ();

  //classProbReport = new ofstream ("ClassProbReport.txt");

  DetermineIfOnANetworkDrive ();
  ProcessStatusFile ();
}  /* ActiveLearning */






ActiveLearning::~ActiveLearning ()
{
  if  (report)
  {
    report->close ();
    delete  report;
    report = NULL;
  }

  delete  classProbReport;

  delete  jobs;  jobs = NULL;

  delete  masterTestImages;  masterTestImages = NULL;
  delete  validationImages;  validationImages = NULL;

  delete  masterTrainingConfiguration;
  delete  mlClasses;
}


KKStr  ActiveLearning::DeriveIIPCsubDirectory (int  iipc)
{
  KKStr  iipcSubDirName (rootDir);
  osAddLastSlash (iipcSubDirName);
  iipcSubDirName << StrFormatInt (iipc, "000") << "-IPC";
  return  iipcSubDirName;
}  /* DeriveIIPCsubDirectory */




PassAssignmentsPtr  ActiveLearning::GetAssignments (int  iipc)
{
  KKStr  fullAssignmentsName (DeriveIIPCsubDirectory (iipc));

  osAddLastSlash (fullAssignmentsName);
  fullAssignmentsName << initTrainImagesAssignmentFileName;

  PassAssignmentsPtr assignments = NULL;

  if  (!osFileExists (fullAssignmentsName))
  {
    assignments = new PassAssignments (fileDesc,
                                       *masterTestImages,              
                                       *mlClasses,
                                       numOfRandomPasses, 
                                       iipc,
                                       usePriorForIIPC,
                                       log
                                      );
    assignments->Save (fullAssignmentsName);
  }
  else
  {
    bool  successful;
    assignments = new PassAssignments (fileDesc,
                                       fullAssignmentsName, 
                                       *masterTestImages,
                                       log, 
                                       successful
                                      );
    if  (!successful)
    {
      delete  assignments;
      assignments = NULL;
    }
  }

  return  assignments;
}  /* GetAssignments */




KKStr  ActiveLearning::DeriveIPRsubSirectory (int  iipc,
                                               int  ipr
                                              )
{
  KKStr   iprSubDirName (DeriveIIPCsubDirectory (iipc));
  osAddLastSlash (iprSubDirName);

  iprSubDirName << StrFormatInt (iipc, "000") << "-IPC"
                << "_"
                << StrFormatInt (ipr,  "00")  << "-IPR";
  return  iprSubDirName;
}  /* DeriveIPRsubSirectory */



KKStr  ActiveLearning::DeriveResultsFileName (SortOrderType  sortOrder,
                                               int            pass
                                              )
{

  KKStr  resultsFileName = baseResultsFileName 
                            + "_" 
                            + "Combined"                 + "_"
                            + SortOrderDescr (sortOrder) + "_" 
                            + StrFormatInt (pass, "000") 
                            + ".data";
  return  resultsFileName;
}





KKStr  ActiveLearning::DeriveFullResultsFileName (SortOrderType  sortOrder, 
                                                   int            iipc, 
                                                   int            ipr, 
                                                   int            pass
                                                  )
{
   KKStr  fullResultsFileName (DeriveIPRsubSirectory (iipc, ipr));
   osAddLastSlash (fullResultsFileName);
   fullResultsFileName << DeriveResultsFileName (sortOrder, pass);
   return  fullResultsFileName;
}  /* DeriveFullResultsFileName */







void  ActiveLearning::DetermineIfOnANetworkDrive ()
{
  KKStr  localDir = osGetCurrentDirectory ();
  char  localDrive = osGetDriveLetter (localDir);
  weAreUsingANetworkDrive = (toupper (localDrive)  > 'D');
}  /* DetermineIfOnANetworkDrive */





void  ActiveLearning::Block ()
{
  log.Level (20) << "ActiveLearning::Block - Entering." << endl;
  
  int  count = 0;

  do  {
    lockFile = open (lockFileName, O_WRONLY | O_CREAT | O_EXCL);

    if  (lockFile < 0)
    {
      count++;
      log.Level (20) << "ActiveLearning::Block - We are locked out[" << count << "]."  << endl;
      #ifdef  WIN32
      Sleep (5000);
      #else
      sleep (5);
      #endif
    }
  }  while  (lockFile < 0);

  lockFileOpened = true;

  log.Level (20) << "ActiveLearning::Block - Lock is Established." << endl;
}




void  ActiveLearning::EndBlock ()
{              
  #ifndef  WIN32
  int  returnCd;
  #endif
  
  log.Level (20) << "ActiveLearning::EndBlock - Ending Block" << endl;

  if  (!lockFileOpened)
  {
    log.Level (-1) << "ActiveLearning::EndBlock - Lock file is not opened." << endl;
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
       log.Level (-1) << "ActiveLearning::EndBlock - *** ERROR *** Could not set Lock File  to  Normal." << endl;
     }
     else
     {
       if  (!DeleteFile (lockFileName))
       {
         DWORD  lastErrorNum = GetLastError ();
         log.Level (-1) << "ActiveLearning::EndBlock - Error["  << (int64)lastErrorNum << "] deleting Lock File." << endl;
       }
     }
  }
  #else
  returnCd = unlink (lockFileName);
  #endif  

  log.Level (20) << "EndBlock - Unlocking" << endl;
  return;
}  /* EndBlock */



FILE*  ActiveLearning::OpenStatusFile ()
{
  KKStr  fullStatusFileName;

  if  (rootDir.Empty ())
  {
    fullStatusFileName = statusFileName;
  }
  else
  {
    fullStatusFileName = rootDir;
    osAddLastSlash (fullStatusFileName);
    fullStatusFileName << statusFileName;
  }

  FILE*  statusFile = osFOPEN (fullStatusFileName.Str (), "r");
  return  statusFile;
}  /*  OpenStatusFile */





void  ActiveLearning::LoadCurrentStatusFile ()
{
  log.Level (10) << "ActiveLearning::LoadCurrentStatusFile - Loading current Status File." << endl;

  char  buff[10240];

  FILE*  statusFile = OpenStatusFile ();

  if  (!statusFile)
  {
     log.Level (-1) << endl
                    << "ActiveLearning::LoadCurrentStatusFile - *** ERROR ***  Can not open Status File[" 
                    << statusFileName << "]." 
                    << endl;
     EndBlock ();
     osWaitForEnter ();
     exit (-1);
  }

  delete  jobs;  jobs = NULL;

  jobs = new JobDescList (true);

  while  (fgets (buff, sizeof (buff), statusFile)) 
  {
    KKStr  l (buff);

    KKStr  fieldName = l.ExtractToken (" \n\t\r");
    fieldName.Upper ();

    l.TrimLeft  ();
    l.TrimRight ();

    if  (fieldName == "CMDLINE")
    {
      ProcessCmdLineStr  (l);
    }

    else if  (fieldName == "NUMOFPROCESSES")
    {
      numOfProcesses = atoi (l.Str ());
    }

    else if  (fieldName == "REPORTFILENAME")
    {
      reportFileName = l;
    }

    else if  (fieldName == "TESTIMAGESROOTDIR")
    {
      testImagesRootDir = l;
    }

    else if  (fieldName == "TRAININGIMAGESROOTDIR")
    {
      trainingImagesRootDir = l;
    }

    else if  (fieldName == "MASTERTESTIMAGESFILENAME")
    {  
      masterTestImagesFileName = l;
    }

    else if  (fieldName == "VALIDATIONIMAGESFILENAME")
    {
      validationImagesFileName = l;
    }

    else if  (fieldName == "FILEFORMAT")
    {
      fileFormat = FeatureFileIO::FileFormatFromStr (l, true, false);
      if  (fileFormat == NULL)
      {
        // This should never be able to happen, but if it does then
        // something has gone seriously wrong.
        cerr << endl
             << endl
             << "LoadCurrentStatusFile   ***** ERROR *****      Invalid File Format[" << l << "]" << endl
             << endl
             << "                        Formats Supported " << FeatureFileIO::FileFormatsReadOptionsStr ()   << endl
             << endl
             << endl;
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (fieldName == "INITTRAINIMAGESASSIGNMENTFILENAME")
    {
      initTrainImagesAssignmentFileName = l;
    }

    else if  (fieldName == "FINALNUMOFIMAGESINTRAININGLIBRARY")
    {
      finalNumOfImagesInTrainingLibrary = atoi (l.Str ());
    }

    else if  (fieldName == "NUMOFRANDOMPASSES")
    {
      numOfRandomPasses = atoi (l.Str ());
    }

    else if  (fieldName == "NUMOFJOBS")
    {
      // We don't use this anymore.
      int numOfJobs = atoi (l.Str ());
    }

    else if  (fieldName == "JOB")
    {
      jobs->PushOnBack (new JobDesc (*mlClasses, l));
    }
  }


  fclose (statusFile);
  statusFile = NULL;
  return;
}  /* LoadCurrentStatusFile */






void  ActiveLearning::ProcessCmdLineStr  (KKStr  cmdLine)
{
  char*  arguments[60];
  int    argCount = 0;

  arguments[0] = STRDUP ("ActiveLearning");
  argCount++;

  while  (!cmdLine.Empty ())
  {
    KKStr  parmField = cmdLine.ExtractToken (" ");
    arguments[argCount] = STRDUP (parmField.Str ());
    argCount++;
  }

  ProcessCmdLineParameters (argCount, arguments);

  for  (int x = 0;  x < argCount; x++)
  {
    delete  arguments[x];
    arguments[x] = NULL;
  }
}  /* ProcessCmdLineStr */




void  ActiveLearning::UpdateCurrentStatusFile ()
{
  log.Level (10) << "ActiveLearning::UpdateCurrentStatusFile - Updating  Current Status File." << endl;

  KKStr  fullStatusFileName;
  {
    // We will first rename the existing Status File to a new name

    // KKStr  dirPath, rootName, extension;
    //osParseFileName (statusFileName, dirPath, rootName, extension);


    if  (rootDir.Empty ())
    {
      fullStatusFileName = statusFileName;
    }
    else
    {
      fullStatusFileName = rootDir;
      osAddLastSlash (fullStatusFileName);
      fullStatusFileName << statusFileName;
    }


    KKStr  prevName1 (fullStatusFileName);
    prevName1 << ".Prev1";

    KKStr  prevName2 (fullStatusFileName);
    prevName2 << ".Prev2";


    if  (osFileExists (prevName2))
    {
       osDeleteFile (prevName2);
    }

    osRenameFile (prevName1, prevName2);

    if  (osFileExists (fullStatusFileName))
    {
      osRenameFile (fullStatusFileName, prevName1);
    }
  }

  int  openAttempts = 0;


  FILE*  statusFile = NULL;

  while  ((!statusFile)  &&  (openAttempts < 3))
  {
    openAttempts++;
    log.Level (50) << "UpdateCurrentStatusFile Oppen Attempt[" << openAttempts << "]." << endl;
    statusFile = osFOPEN (fullStatusFileName.Str (), "w");
    if  (!statusFile)
    {
      log.Level (0) << "UpdateCurrentStatusFile - *** WARNING ***  Can Not Open Status File[" 
                    << fullStatusFileName << "]   -Will Retry-." 
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
                    << fullStatusFileName << "]." 
                    << endl;
     EndBlock ();
     exit (1);
     osWaitForEnter ();
  }

  
  log.Level (50) << "UpdateCurrentStatusFile - Status File Opened Successfully." << endl;


  KKStr  s;
  s = CmdLineStr ();  
  fprintf (statusFile, "%s %s\n",  "CmdLine",                             s.Str ());
  fprintf (statusFile, "%s %d\n",  "NumOfProcesses",                      numOfProcesses);
  fprintf (statusFile, "%s %s\n",  "ReportFileName",                      reportFileName.Str ());
  fprintf (statusFile, "%s %s\n",  "TestImagesRootDir",                   testImagesRootDir.Str ());
  fprintf (statusFile, "%s %s\n",  "TrainingImagesRootDir",               trainingImagesRootDir.Str ());
  fprintf (statusFile, "%s %s\n",  "MasterTestImagesFileName",            masterTestImagesFileName.Str ());
  fprintf (statusFile, "%s %s\n",  "ValidationImagesFileName",            validationImagesFileName.Str ());
  fprintf (statusFile, "%s %s\n",  "FileFormat",                          fileFormat->DriverName ().Str ());
  fprintf (statusFile, "%s %s\n",  "InitTrainImagesAssignmentFileName",   initTrainImagesAssignmentFileName.Str ());
  fprintf (statusFile, "%s %d\n",  "numOfRandomPasses",                   numOfRandomPasses);
  fprintf (statusFile, "%s %d\n",  "finalNumOfImagesInTrainingLibrary",   finalNumOfImagesInTrainingLibrary);
  fprintf (statusFile, "%s %d\n",  "NumOfJobs",  jobs->QueueSize ());

  for  (int jobIDX = 0;  jobIDX < jobs->QueueSize ();  jobIDX++)
  {
    fprintf (statusFile, "%s %s\n", "Job", jobs->IdxToPtr (jobIDX)->ToString ().Str ());
  }

  //fprintf (statusFile, "EndJobDescriptions\n");

  fclose  (statusFile);
  statusFile = NULL;

  log.Level (20) << "UpdateCurrentStatusFile - Exiting." << endl;
}  /* UpdateCurrentStatusFile */





JobDescPtr  ActiveLearning::GetNextJob ()
{
  log.Level (10) << "ActiveLearning::GetNextJob" << endl;

  bool  exitLoop = false;
  bool  jobFound = false;
  bool  updateStatusFile = false;
  bool  thereAreUnstartedJobs = false;

  JobDescPtr nextJob = NULL;

  while  (!exitLoop)
  {
    Block ();
  
    bool  thereAreUnstartedJobs = false;
      
    LoadCurrentStatusFile ();

    int  jobIDX = 0;

    while  ((!jobFound)  &&  (jobIDX < jobs->QueueSize ()))
    {
      JobDescPtr  j = jobs->IdxToPtr (jobIDX);

      if  (j->CurStatus () == '0')
      {
        thereAreUnstartedJobs = true;

        if  (j->JobType () != 'F')
        {
          jobFound = true;
          nextJob = j;
          nextJob->CurStatus ('1');
          updateStatusFile = true;
          exitLoop = true;
        }
        else
        {
          // Before we can start the Final Job we must make sure all other
          // jobs are done.

          bool  allOtherJobsAreDone = true;

          for  (int k = 0;  k < jobs->QueueSize ();  k++)
          {
            if  (jobs->IdxToPtr (k)->JobID () != j->JobID ())
            {
              if  (jobs->IdxToPtr (k)->CurStatus () != '3')
                allOtherJobsAreDone = false;
            }
          }

          if  (allOtherJobsAreDone)
          {
            jobFound = true;
            nextJob = j;
            nextJob->CurStatus ('1');
            updateStatusFile = true;
            exitLoop = true;
          }
        }
      }

      jobIDX++;
    }
  
    if  (updateStatusFile)
    {
      UpdateCurrentStatusFile ();
      updateStatusFile = false;
    }

    EndBlock ();

    if  (!nextJob)
    {  
      if  (thereAreUnstartedJobs)
      {
        log.Level (10) << "GetNextJob - No Available Jobs to Run,  will wait 2 minites before Trying Again." << endl;

        #ifdef  WIN32
        Sleep (120000);
        #else
        sleep (120);
        // weWereSleeping = true;
        #endif
      }
      else  
      {
        // We Are Done,  Can Terminate Program.
        exitLoop = true;
      }
    }
  }

  if  (nextJob)
    nextJob =  new JobDesc (*nextJob);

  delete  jobs;  jobs = NULL;

  if  (nextJob)
  {
    log.Level (10) << "GetNextJob - Selected Job[" << nextJob->JobID () << "] as the Next Job." << endl;
    return  nextJob;
  }
  else
  {
    log.Level (10) << "GetNextJob -  No Nore Jobs To Run,  Returning NULL." << endl;
    return NULL;
  }
}  /* GetNextJob */




void  ActiveLearning::FinishJob (JobDescPtr  j)
{
  log.Level (10) << " ActiveLearning::FinishJob  Flagging Job[" << j->JobID () << "]  as completed." << endl;

  Block ();

  LoadCurrentStatusFile ();

  for  (int i = 0;  i < jobs->QueueSize ();  i++)
  {
    if  (jobs->IdxToPtr (i)->JobID () == j->JobID ())
    {
      jobs->IdxToPtr (i)->CurStatus ('3');
      break;
    }
  }

  UpdateCurrentStatusFile ();

  EndBlock ();

  delete  jobs;  jobs = NULL;
}  /* FinishJob */





bool  ActiveLearning::ProcessCmdLineParameter (char    parmSwitchCode, 
                                               KKStr  parmSwitch, 
                                               KKStr  parmValue
                                              )
{
  parmSwitch.Upper ();

  if  ((parmSwitch == "-MASTERFILE")  ||  (parmSwitch == "-MF"))
     masterTestImagesFileName = parmValue;

  else if  ((parmSwitch == "-VALIDATIONFILE")  ||  (parmSwitch == "-VF"))
     validationImagesFileName = parmValue;

  else if  ((parmSwitch == "USEPRIORFORIIPC")  ||  
            (parmSwitch == "-USEPRIOR")        ||
            (parmSwitch == "-UPFI")
           )
     usePriorForIIPC = true;

  else if  ((parmSwitch == "-C") || (parmSwitch == "-CONFIGFILE"))
  { // Configuration File Specification.
    if  (parmValue.Empty ())
    {
      log.Level (-1) << "ProcessCmdLineParameter *** ERROR Missing Configuration Parameter ***" 
                     << endl;
      Abort (true);
      return  false;
    }

    configFileName = parmValue;
  }

  else if  (parmSwitch == "-E")
  {
    if  (parmValue.Empty ())
    {
      log.Level (-1) << "ProcessCmdLineParameter *** ERROR Missing BaseResultsFileName ***" 
                     << endl;
      Abort (true);
      return  false;
    }
         
    baseResultsFileName = parmValue;
  }

  else if  ((parmSwitch == "-F")   ||  
            (parmSwitch == "-FF")  ||  
            (parmSwitch == "-FILEFORMAT")
           )
  {
    if  (parmValue.Empty ())
    {
      log.Level (-1) << "ProcessCmdLineParameter *** ERROR Missing FileFormat ***" 
                     << endl;
      Abort (true);
      return  false;
    }

    fileFormat = FeatureFileIO::FileFormatFromStr (parmValue, true, false);
    if  (fileFormat == NULL)
    {
      log.Level (-1) << endl
                     << endl
                     << "ProcessCmdLineParameter   *** Invalid File Format[" << parmValue << "] ***"   << endl
                     << "                          Formats Supported " << FeatureFileIO::FileFormatsReadOptionsStr () << endl
                     << endl;
      Abort (true);
      return  false;
    }
  }

  else if  (parmSwitch == "-IPR")
  {
    iprTable = ParseIntList (parmValue);
  }
  
  else if  (parmSwitch == "-IIPC")
  {
    iipcTable = ParseIntList (parmValue);
  }
  
  else if  ((parmSwitch == "-J")  ||  (parmSwitch == "-ENDINGLIBRARYSIZE"))
  {
    finalNumOfImagesInTrainingLibrary = atoi (parmValue.Str ());
    if  (finalNumOfImagesInTrainingLibrary < 10)
    {
      log.Level (-1) << endl
		                 << "ProcessCmdLineParameter   ******** FinalNumOfImagesInTrainingLibrary[" 
                     << finalNumOfImagesInTrainingLibrary 
                     << "]  Is too small ********"
                     << endl
					           << endl;
      Abort (true);
      return  false;
    }
  }


  else if  ((parmSwitch == "-Q") || (parmSwitch == "-RANDOMPASSES"))
  {
    numOfRandomPasses = atoi (parmValue.Str ());

    if  (numOfRandomPasses < 1)
    {
      log.Level (-1) << "ProcessCmdLineParameter *** NumOfRandomPasses[" 
                     << numOfRandomPasses 
                     << "]  must be greater than zero('0')."
                     << endl;
      Abort (true);
      return  false;
    }
  }


  else if  (parmSwitch == "-P")
  {
    testImagesRootDir = parmValue;
  }

  else if  ((parmSwitch == "-STREAM")  ||  (parmSwitch == "-STREAMING"))
  {
    performStreaming = true;
    streamingBlockSize = atoi (parmValue.Str ());
    if  (streamingBlockSize < 2)
    {
      log.Level (-1) << endl
		                 << "ProcessCmdLineParameter   ******** Streaming[" 
                     << streamingBlockSize 
                     << "]  Must be greater than 2 and IPR ********"
                     << endl
					           << endl;
      Abort (true);
      return  false;
    }
  }

  else if  (parmSwitch == "-T")
  {
    trainingImagesRootDir = parmValue;
  }


  return  !Abort ();
}  /* ProcessCmdLineParameter */





KKStr  ActiveLearning::CmdLineStr ()
{
  KKStr  c (200);

  c << "-MasterFile "       << masterTestImagesFileName;
  c << "  -ValidationFile " << validationImagesFileName;
  c << "  -C " << configFileName;

  if  (!baseResultsFileName.Empty ())
    c << "  -E " << baseResultsFileName;

  c << "  -IPR "  << IntListToStr (iprTable);

  c << "  -IIPC " << IntListToStr (iipcTable);

  if  (usePriorForIIPC)
    c << "  -UsePriorForIIPC ";

  c << "  -J " << finalNumOfImagesInTrainingLibrary;

  if  (!reportFileName.Empty ())
    c << " -R " << reportFileName;


  c << "  -Q " << numOfRandomPasses;


  if  (!testImagesRootDir.Empty ())
  {
    c << "  -S  "  << testImagesRootDir;
  }

  if  (performStreaming)
  {
    c << " -Streaming " << streamingBlockSize;
  }
   
  if  (!trainingImagesRootDir.Empty ())
  {
    c << " -T " << trainingImagesRootDir;
  }

  c << " -F " << fileFormat->DriverName ();

  return  c;
}  /* CmdLineStr */







void   ActiveLearning::DisplayCommandLineParameters ()
{
  log.Level (0) << "ActiveLearning  -a <xxx>  -c <xxx> -e <xxx> -f <xxx>  -j <xxx> -r <xxx>  -s <xxx>" << endl;
  log.Level (0) << endl;
  log.Level (0) << "    -a  Save File Name"                                                  << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -c  Specify the Configuration file to use if you want"               << endl; 
  log.Level (0) << "        to classify images."                                             << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -e  Base Results File Name"                                          << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -r  Report File,  Defaults to Command Line."                         << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -s  Source Directory Tree to Search for images."                     << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -j  Total Number of Training Images in Final Libray"                 << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -IIPC  nn,nn,nnn"                                                    << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -IPR   nn,nn,nn"                                                     << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -USEPRIORFORIIPC"                                                    << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -FileFormat <" << FeatureFileIO::FileFormatsReadOptionsStr () << ">" << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -Stream  <Stream Block Size>"                                        << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -MasterFile  <Data for Active Learning>"                             << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -ValidationFile  <Data to test with>"                                << endl;
}  /* DisplayCommandLineParameters */




void  ActiveLearning::SortImageFeatures (FeatureVectorList&  images,
                                         SortOrderType       sortOrder
                                        )

{
  log.Level (10) << "ActiveLearning::SortImageFeatures  SortOrder[" << SortOrderDescr (sortOrder) << "]." << endl;

  switch  (sortOrder)
  {
    case  ProbabilityOrder:
                images.SortByProbability (false);
                break;

    case  BreakTieOrder: 
                images.SortByBreakTie (false);
                break;

    case  RandomOrder:
                images.RandomizeOrder ();
                break;
  }  /* end of case */
}  /* SortImageFeatures */








FeatureVectorListPtr  ActiveLearning::LoadImages (KKStr                dirPath,
                                                  FeatureVectorListPtr  trainingImages
                                                 )
{
  log.Level (10) << "LoadImages"                              << endl;
  log.Level (10) << "    dirPath        [" << dirPath << "]." << endl;


  osAddLastSlash (dirPath);

  KKStr  localDirFeatureFileName (dirPath);

  KKStr  driveLetter;
  KKStr  path;
  KKStr  root;
  KKStr  extension;

  if  (!dirPath.Empty ())
  {
    KKStr  temp (dirPath);

    temp.ChopLastChar ();

    osParseFileSpec (temp, driveLetter, path, root, extension);
    localDirFeatureFileName << root << ".data";
  }
  else
  {
    localDirFeatureFileName << "Root.data";
    root = "Root";
  }


  FeatureVectorListPtr  images = NULL;


  TrainingClassPtr trainingClass = masterTrainingConfiguration->LocateByMLClassName (root);

  if  (!trainingClass)
  {
    // Since there is no Training Library for the the class in this directory
    // we will create an empty FeatureVectorList.

    images = new FeatureVectorList (fileDesc, true, log, 2000);
  }

  else
  {
    MLClassPtr  mlClass = mlClasses->GetMLClassPtr (root);

    KKStr  featureFileName (root);
    featureFileName << ".data";

    bool  changesMade = false;
    bool  cancelFlag  = false;

    DateTime  timeStamp;

    images = FeatureFileIOPices::FeatureDataReSink (dirPath, 
                                                    featureFileName, 
                                                    mlClass,
                                                    false,     // Don't override Class name in feature file.
                                                    *mlClasses,
                                                    cancelFlag,
                                                    changesMade,
                                                    timeStamp,
                                                    log
                                                   );
    if  (images->QueueSize () <= 0)
    {
      mlClasses->DeleteEntry (mlClass);
      //delete  mlClass;
    }
    else
    {
      FeatureVectorListPtr  filteredList = new FeatureVectorList (fileDesc, false, log, 1000);
      {
        // Remove images that are being used as test Images.
        ImageFeaturesListIterator  iIDX (*images);
        for  (iIDX.Reset ();  iIDX.CurPtr ();  ++iIDX)
        {
          if  (trainingImages->LookUpByImageFileName (iIDX->ImageFileName ()) == NULL)
          {
             filteredList->PushOnBack (iIDX.CurPtr ());
          }
          else
          {
            cout << "*";
          }
        }
      }

      images->Owner (false);
      delete  images;
      images = filteredList;
    }
  }



  // We can now process any Sub-Directories.

  KKStr  dirSearchPath (dirPath);

  dirSearchPath << "*.*";

  KKStrListPtr  subDirectories = osGetListOfDirectories (dirSearchPath);
  if  (subDirectories)
  {
    StringListIterator  sdIDX (*subDirectories);
  
    for  (sdIDX.Reset ();  sdIDX.CurPtr (); ++sdIDX)
    {
      KKStr  newDirPath (dirPath);
      newDirPath << *(sdIDX.CurPtr ());

      FeatureVectorListPtr subDirImages = LoadImages (newDirPath, trainingImages);

      images->AddQueue (*subDirImages);
      subDirImages->Owner (false);
      delete  subDirImages;
    }
  }

  log.Level (10) << "LoadImages  Done" << endl;

  return  images;
}  /* LoadImages */





void  ActiveLearning::ExtractValidationImages (MLClassList&     classes,
                                               FeatureVectorList&  srcImages,
                                               FeatureVectorList&  destImages,
                                               FeatureVectorList&  validationImages
                                              )

{
  log.Level (10) << "ExtractValidationImages" << endl;


  int  numOfClasses = classes.QueueSize ();
  int  classNum;

  srcImages.RandomizeOrder ();

  int  smallestNumOfImagesInAClass = 9999999;

  // First Seperate Images into there respective Classes

  MLClassPtr  mlClass = NULL;
  //MLClassListIterator  clIDX (classes);
  
  FeatureVectorListPtr*  classImages = new FeatureVectorListPtr[numOfClasses];

  for  (classNum = 0;  classNum < numOfClasses;  classNum++)
  {
    mlClass = classes.IdxToPtr (classNum);

    classImages[classNum] = srcImages.ExtractImagesForAGivenClass (mlClass);

    if  (classImages[classNum]->QueueSize () < smallestNumOfImagesInAClass)
       smallestNumOfImagesInAClass = classImages[classNum]->QueueSize ();
 
    classImages[classNum]->RandomizeOrder ();
  }



  // Now that we know the number of images in the Smallest Class.
  // We will divide the images in each class evenly amongst the
  // different groups. With all classes having the same number of 
  // images 
  for  (classNum = 0;  classNum < numOfClasses;  classNum++)
  {
    mlClass = classes.IdxToPtr (classNum);

    classImages[classNum]->RandomizeOrder ();

    ImageFeaturesPtr  image = NULL;
    ImageFeaturesListIterator  ciIDX (*classImages[classNum]);

    int  imageCount = 0;
    int  numValidationImages4Class = 0;


    for  (ciIDX.Reset ();  (image = ciIDX.CurPtr ())  &&  (imageCount < smallestNumOfImagesInAClass);  ++ciIDX)
    {
      if  (numValidationImages4Class < validationImagesPerClass)  
      {
        validationImages.PushOnBack (image);
        numValidationImages4Class++;
      }

      else
      {
        if  (imageCount < smallestNumOfImagesInAClass)
        {
          destImages.PushOnBack (image);
        }
      }

      imageCount++;
    }
  }

  for  (classNum = 0;  classNum < numOfClasses;  classNum++)
  {
    delete  classImages[classNum];
  }

  delete  classImages;

}  /* ExtractValidationImages */







void  ActiveLearning::UpdateNonStreamingTestImages (SortOrderType       sortOrder,
                                                    Classifier2Ptr      classifier, 
                                                    FeatureVectorList&  nonStreamingTestImages
                                                   )
{
  ImageFeaturesListIterator iIDX (nonStreamingTestImages);
  ImageFeaturesPtr  image = NULL;

  for  (iIDX.Reset ();  image = iIDX.CurPtr ();  ++iIDX)
  {
    ImageFeaturesPtr  testImage = new ImageFeatures (*image);

    double  probability;
    double  breakTie;
    int     numOfWinners;
    bool    knownClassOneOfTheWinners;
    

    MLClassPtr  predictedClass =  classifier->ClassifyAImage (*testImage, 
                                                                 probability, 
                                                                 numOfWinners,
                                                                 knownClassOneOfTheWinners,
                                                                 breakTie
                                                                );

    image->Probability      ((FFLOAT)probability);
    image->PredictedClass   (predictedClass);
    image->BreakTie         ((float)breakTie);

    delete  testImage;
  }
  
  SortImageFeatures (nonStreamingTestImages, sortOrder);
}  /* UpdateNonStreamingTestImages */






float  ActiveLearning::GetPertinentProbability (ImageFeaturesPtr  image,
                                                SortOrderType     sortOrder
                                               )
{
  float  probability = 0.0f;

  switch  (sortOrder)
  {
    case  ProbabilityOrder:      probability = image->Probability      ();  break;
    case  BreakTieOrder:         probability = image->BreakTie         ();  break;
    case  RandomOrder:           probability = image->Probability      ();  break;
    default:                     probability = image->Probability      ();  break;
  }

  return  probability;
}  /* GetPertinentProbability */





void  ActiveLearning::ProcessASingleRetraining (SortOrderType        sortOrder,
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
                                               )

{
  log.Level (10) << "ProcessASingleRetraining  SortOrder[" << SortOrderDescr (sortOrder) << "], iipc[" << iipc << "], ipr[" << ipr << "], Pass[" << retrainingPass << "]." << endl;

  {
    // Calculate Probabilities of each test image.

    ImageFeaturesListIterator  imagesIDX (testImages);

    for  (imagesIDX.Reset ();  imagesIDX.CurPtr (); ++imagesIDX)
    {
      ImageFeaturesPtr  origImage = imagesIDX.CurPtr ();
      MLClassPtr     origClass = origImage->MLClass ();

      ImageFeaturesPtr  testImage = new ImageFeatures (*origImage);

      double   probability;
      double   breakTie;
      int      numOfWinners;
      bool     knownClassOneOfTheWinners;
    
//    MLClassPtr  predictedClass 
//               =  classifier->ClassifyAImage (*testImage, probability, distanceToBoarder);


      MLClassPtr  predictedClass =  classifier->ClassifyAImage (*testImage, 
                                                                   probability, 
                                                                   numOfWinners,
                                                                   knownClassOneOfTheWinners,
                                                                   breakTie
                                                                 );


      predictedClass = classesInGroup.LookUpByName (predictedClass->Name ());

      origImage->Probability      ((FFLOAT)probability);
      origImage->PredictedClass   (predictedClass);
      origImage->BreakTie         ((float)breakTie);

      if  (classProbReport)
      {
        int  numOfClasses = classesInGroup.QueueSize ();  
        double*  probs = new double[numOfClasses];
        int*    votes = new int[numOfClasses];
        classifier->ProbabilitiesByClass (classesInGroup, testImage, votes, probs);
        *classProbReport <<         retrainingPass
                         << "\t" << testImage->ImageFileName ()
                         << "\t" << predictedClass->Name ()
                         << "\t" << probability
                         << "\t" << breakTie;

        for  (int x = 0;  x < numOfClasses; x++)
        {
          *classProbReport << "\t" << probs[x];
        }
        *classProbReport << endl;

        delete  probs;
        delete  votes;
      }

      delete  testImage;
    }
  }

  log.Level (20) << "ProcessASingleRetraining  All Images in Test Set Classified." << endl;



  {
    // Process Validation Images

    double  testingTimeStart = osGetSystemTimeUsed ();

    ImageFeaturesListIterator  imagesIDX (validationImages);

    for  (imagesIDX.Reset ();  imagesIDX.CurPtr (); ++imagesIDX)
    {
      ImageFeaturesPtr  origImage = imagesIDX.CurPtr ();
      MLClassPtr     origClass = origImage->MLClass ();

      ImageFeaturesPtr  testImage = new ImageFeatures (*origImage);

      double  probability;
      double  breakTie;
      int     numOfWinners;
      bool    knownClassOneOfTheWinners;
    
      MLClassPtr  predictedClass =  classifier->ClassifyAImage (*testImage, 
                                                                   probability, 
                                                                   numOfWinners,
                                                                   knownClassOneOfTheWinners,
                                                                   breakTie
                                                                  );


      predictedClass = classesInGroup.LookUpByName (predictedClass->Name ());

      stats.Prediction (retrainingPass, pass, origClass, predictedClass, (FFLOAT)probability);
  
      origImage->Probability    ((FFLOAT)probability);
      origImage->PredictedClass (predictedClass);
      origImage->BreakTie       ((float)breakTie);

      delete  testImage;
    }

    double  testingTimeDone = osGetSystemTimeUsed ();

    testingTime = (float)(testingTimeDone - testingTimeStart);
  }

  log.Level (20) << "ProcessASingleRetraining  All Images in Validation Set classified." << endl;

  SortImageFeatures (testImages, sortOrder);
  log.Level (20) << "ProcessASingleRetraining    Test Images Have Been Sorted." << endl;

  ImageFeaturesPtr  image;
  ImageFeaturesListIterator tiIDX (testImages);
  FeatureVectorList  borderImages (fileDesc, false, log, 1000);

  int  imagesCopied    = 0;

  double  probability;
  int     numNonStreamingTestImages = 0;


  if  (testRelativeNonStreamingPosition  &&  nonStreamingTestImages)
  {
    numNonStreamingTestImages = nonStreamingTestImages->QueueSize ();
    UpdateNonStreamingTestImages (sortOrder, 
                                  classifier, 
                                  *nonStreamingTestImages
                                 );
  }


  for  (tiIDX.Reset (); ((image = tiIDX.CurPtr ()) &&  (imagesCopied < ipr)); ++tiIDX)
  {
    probability = GetPertinentProbability (image, sortOrder);
    borderImages.PushOnBack (image);

    float percentile = 0.0f;
    int   position   = 0;

    if  (testRelativeNonStreamingPosition  &&  nonStreamingTestImages)
    {
      position = nonStreamingTestImages->LocateEntry (image);
      percentile = ((float)position / (float)numNonStreamingTestImages);
      nonStreamingTestImages->DeleteEntry (image);
    }

    stats.AddAProbability (retrainingPass, 
                           pass, 
                           image->MLClass (), 
                           (FFLOAT)probability, 
                           image->ImageFileName (),
                           position,
                           percentile
                          );
    imagesCopied++;
  }

  trainingImages.AddQueue (borderImages);

  ImageFeaturesListIterator borderIDX (borderImages);
  for  (borderIDX.Reset (); (image = borderIDX.CurPtr ()); ++borderIDX)
  {
    testImages.DeleteEntry (image);
  }

  log.Level (10) << "ProcessASingleRetraining  Exiting." << endl;
}  /* ProcessASingleRetraining */






void  ActiveLearning::ProcessOneImageAtAtimeOneSpecificPass 
                             (SortOrderType       sortOrder,
                              double              probabilityTest,
                              MLClassList&     groupClasses,           // List of Classes that we want to processs for.
                              ClassGroupTotals&   retrainingStats,
                              int                 pass,
                              int                 iipc,
                              int                 ipr,
                              int                 numOfRetrainings
                             )
{
  log.Level (10) << "ProcessOneImageAtAtimeOneSpecificPass" << endl;

  KKStr  classNames;
  classNames << groupClasses;

  /************************************************/
  /*  Create Initial trainingConfiguration Object */
  /************************************************/

  TrainingConfiguration2Ptr  trainingConfiguration 
        = new TrainingConfiguration2 (fileDesc, configFileName, log, false);

  MLClassListPtr  classesInGroup = trainingConfiguration->ExtractClassList ();

  // Now that we built a trainingConfiguration object we need to remove all the training Libraries 
  // that are not a member of groupClasses.  We do this by using the  "classesInGroup"  object that will
  // contain a list of all classes in trainingConfiguration.  For each member in the list that does 
  // not exist in  "groupClasses"  we will remove from  "classesInGroup".
  MLClassPtr  mlClass;
  for  (int idx = classesInGroup->QueueSize () - 1; idx >= 0;  idx--)
  {
    mlClass = classesInGroup->IdxToPtr (idx);
    if  (groupClasses.PtrToIdx (mlClass) < 0)
    {
      classesInGroup->DeleteEntry (mlClass);
    }
  }


  FeatureVectorList  twoClassValidationImages (groupClasses, *validationImages, log);


  trainingConfiguration->EmptyTrainingClasses ();  // remove all trainingClasses,
                                                   // then in following cose we will create
                                                   // New TrainingClasses  for the classes that
                                                   // we want to process for.
  TrainingClassPtr  trainingClass = NULL;

  // We are now going to add to the trainingConfiguration object Training Classes 
  // for the classes that we are going to process for as indicated by 
  // classesInGroup.
  //MLClassListIterator  icIDX (*classesInGroup);

  MLClassList::iterator  icIDX;
  for  (icIDX = classesInGroup->begin ();   icIDX != classesInGroup->end ();  ++icIDX)
  {
    mlClass = *icIDX;
    float  weight = 1.0f;
    trainingClass = new TrainingClass (KKStr (""),
                                       mlClass->Name (),
                                       KKStr ("B"),
                                       weight,
                                       *classesInGroup
                                      );
    trainingConfiguration->AddATrainingClass (trainingClass);
  }

  FeatureVectorListPtr  trainingImages = NULL;
  FeatureVectorListPtr  testImages     = NULL;

  { 
    PassAssignmentsPtr  assignments = GetAssignments (iipc);

    assignments->ExtractInitialTrainingAndTestImages (pass, 
                                                      *masterTestImages, 
                                                      groupClasses,
                                                      trainingImages,
                                                      testImages
                                                     );
    testImages->RandomizeOrder (pass * 9753);
    testImages->RandomizeOrder ();
    delete  assignments;
  }


  TrainingProcess2Ptr  trainer    = NULL;
  Classifier2Ptr       classifier = NULL;

  FeatureVectorListPtr  nonStreamingTestImages = NULL;

  if  (performStreaming  &&  (sortOrder != RandomOrder))
    testRelativeNonStreamingPosition = true;
  else
    testRelativeNonStreamingPosition = false;


  if  (testRelativeNonStreamingPosition)
  {
    // We must be doing Streaming,  
    // This list will be used todetermine where a selected image would have ended up if we 
    // were doing Non Streaming.
    nonStreamingTestImages = new FeatureVectorList (*testImages, false);
  }

  int  numOfStreamBlocks = 1;
  int  streamBlockCount = 0;
  if  (performStreaming)
  {
    numOfStreamBlocks = (int)ceil ((double)testImages->QueueSize () / (double)streamingBlockSize);
  }
  else
  {
    numOfStreamBlocks = 1;
    retrainingsPerStreamBlock = numOfRetrainings;
    streamingBlockSize = testImages->QueueSize ();
  }

  int  retrainingPass = 0;
  int  streamBlockStartIDX = 0;
  int  streamBlockEndIDX   = 0;

  while  (streamBlockCount < numOfStreamBlocks)
  {
    streamBlockStartIDX = streamBlockEndIDX;
    FeatureVectorListPtr streamBlockTestImages = new FeatureVectorList (fileDesc, false, log, streamingBlockSize);

    int  x = 0;

    if  (performStreaming)
    {
      while  ((x < streamingBlockSize)  &&  (streamBlockEndIDX < testImages->QueueSize ()))
      {
        streamBlockTestImages->PushOnBack (testImages->IdxToPtr (streamBlockEndIDX));
        x++;
        streamBlockEndIDX++;
      }
    }
    else
    {
      streamBlockTestImages->AddQueue (*testImages);
    }

    log.Level (10) << "Processing Stream Block[" << streamBlockCount    << "]  " 
                   << "Start Idx["               << streamBlockStartIDX << "]  "  
                   << "End Idx["                 << streamBlockEndIDX   << "]"  
                   << endl;

    for  (int retrainingInStreamBlock = 0;  retrainingInStreamBlock < retrainingsPerStreamBlock;  retrainingInStreamBlock++)
    {
      // We make a duplicate of the current contents of trainingImages and use that to generate the 
      // training model.  This is done because the trainingModel will normalize their contents.

      FeatureVectorListPtr  trainingImagesCopy = trainingImages->DuplicateListAndContents ();
 
      float  testingTime  = 0.0;
      float  trainingTime = 0.0;

      bool    cancelFlag  = false;
      KKStr  statusMessage;

     
      double  trainingTimeStart = osGetSystemTimeUsed ();

      trainer = new TrainingProcess2 (trainingConfiguration, 
                                      trainingImagesCopy, 
                                      /** classesInGroup,  */
                                      NULL, 
                                      fileDesc,
                                      log, 
                                      false,
                                      cancelFlag,
                                      statusMessage
                                     );

      trainer->CreateModelsFromTrainingData ();

      double  trainingTimeEnd = osGetSystemTimeUsed ();


      classifier = new Classifier2 (trainer, log);


      ProcessASingleRetraining (sortOrder,
                                probabilityTest,
                                *classesInGroup,
                                classifier, 
                                *trainingImages, 
                                *streamBlockTestImages,
                                twoClassValidationImages,
                                retrainingPass,
                                retrainingStats,
                                pass,
                                iipc,
                                ipr,
                                testingTime,
                                nonStreamingTestImages
                               );

      trainingTime = (float)(trainingTimeEnd - trainingTimeStart);

      retrainingStats.NumOfSupportVectors (retrainingPass,
                                           pass,
                                           trainer->NumOfSupportVectors (),
                                           trainingTime,
                                           testingTime
                                          );
      delete classifier;
      delete trainer;
      delete trainingImagesCopy;

      retrainingPass++;
    }

    delete  streamBlockTestImages;
    streamBlockStartIDX = streamBlockEndIDX;
    streamBlockCount++;
  }


  delete  nonStreamingTestImages;

  delete  trainingImages;
  delete  testImages;

  delete  trainingConfiguration;   trainingConfiguration = NULL;
  delete  classesInGroup;          classesInGroup        = NULL;
}  /* ProcessOneImageAtAtimeOneSpecificPass */




void  ActiveLearning::ProcessCombinedClassJob (SortOrderType  sortOrder,
                                               int            pass,
                                               int            iipc,
                                               int            ipr
                                              )
{
  log.Level (10) << "ActiveLearning::ProcessCombinedClassJob  Pass[" << pass << "]  iipc[" << iipc << "]  ipr[" << ipr << "]." << endl;

  int  numOfClasses = mlClasses->QueueSize ();

  int  startingNumOfImagesInTrainingLibrary = numOfClasses * iipc;
  int  imagesNeededToBeAdded = finalNumOfImagesInTrainingLibrary - startingNumOfImagesInTrainingLibrary;
  
  int  numOfRetrainings = 0;
 
  if  (performStreaming)
  {
    int  numOfImagesToWorkWith = masterTestImages->QueueSize () - startingNumOfImagesInTrainingLibrary;
    int  numOfStreamBlocks = (int)ceil ((double)numOfImagesToWorkWith / (double)streamingBlockSize);
    numOfRetrainings = numOfStreamBlocks * retrainingsPerStreamBlock;
  }
  else
  {
    numOfRetrainings = (int)ceil ((double)imagesNeededToBeAdded / (double)ipr) + 1;
  }

  imagesNeededToBeAdded = numOfRetrainings * ipr;

  if  (imagesNeededToBeAdded < 0)
  {
    log.Level (-1) << endl
                   << endl
                   << endl
                   << "ProcessCombinedClassJob         *** ERROR ***     The parameters don't make sense" << endl
                   << endl
                   << "startingNumOfImagesInTrainingLibrary [" << startingNumOfImagesInTrainingLibrary << "]" << endl
                   << "imagesNeededToBeAdded                [" << imagesNeededToBeAdded                << "]" << endl
                   << "numOfRetrainings                     [" << numOfRetrainings                     << "]" << endl
                   << endl
                   << "   Check your parameters"  << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }


  ClassGroupTotals  results  (sortOrder, 
                              numOfRetrainings, 
                              numOfRandomPasses,
                              iipc,
                              ipr,
                              streamingBlockSize,
                              log, 
                              *mlClasses, 
                              true
                             );

  ProcessOneImageAtAtimeOneSpecificPass (sortOrder,
                                         99999.9,
                                         *mlClasses,
                                         results,
                                         pass,
                                         iipc,
                                         ipr,
                                         numOfRetrainings
                                        );
  
  // Lets Save Results To Combined Results File

  KKStr  resultsFileName = DeriveResultsFileName (sortOrder, pass);
  KKStr  fullResultsFileName = DeriveFullResultsFileName (sortOrder, iipc, ipr, pass);


  if  ((weAreUsingANetworkDrive)  && (resultsFileName.LocateCharacter (DSchar) < 0))
  {
    // We are running on a windows PC that is about to write to a Network drive
    // This tends to and exceptionaly long time,  not sure why,  but something to do 
    // with the networ.  So we will write to a local drive 1st,  then perform a 
    // windows copy function.

    log.Level (10) << "ProcessCombinedClassJob    Writting Out Results to Local Drive,   Pass[" << pass << "]." << endl;

    WriteResultsFileViaLocalDrive (results, resultsFileName, fullResultsFileName);
  }

  else
  {
    log.Level (10) << "ProcessCombinedClassJob    Writting Out Results,   Pass[" << pass << "]." << endl;
    results.WriteOutHistory (fullResultsFileName);
  }

  log.Level (10) << "ActiveLearning::ProcessCombinedClassJob,   Exiting,   SortOrder[" << SortOrderDescr (sortOrder) 
                 << "   Pass[" << pass << "]." 
                 << endl;
}  /* ProcessCombinedClassJob */




void  ActiveLearning::WriteResultsFileViaLocalDrive (ClassGroupTotals&  results, 
                                                     KKStr             resultsFileName,
                                                     KKStr             fullResultsFileName
                                                    ) 
{
  KKStr  localDrivePath = "C:\\Temp\\ActiveLearning";

  if  (!osValidDirectory (localDrivePath))
  {
    log.Level (-1) << endl
                   << "WriteResultsFileViaLocalDrive  Directory[" << localDrivePath << "]  Does not exist,  will create." << endl
                   << endl;
    bool  dirrectoryCreatedOK = osCreateDirectoryPath (localDrivePath);

    if  (!dirrectoryCreatedOK)
    {
      log.Level (-1) << endl
                     << "WriteResultsFileViaLocalDrive  Directory[" << localDrivePath << "]  Could not be created." << endl
                     << "                               Will write directly to network drive."  << endl
                     << endl;

      results.WriteOutHistory (fullResultsFileName);
      return;
    }
  }

  osAddLastSlash (localDrivePath);

  KKStr  localDriveName = localDrivePath + resultsFileName;
 
  results.WriteOutHistory (localDriveName);

  KKStr  networkPath = osGetCurrentDirectory ();

  log.Level (10) << "ProcessCombinedClassJob    Copying Results File from["  << localDrivePath << "]  "
                 << "  to[" << networkPath << "]." << endl;

  bool  copyOK = osCopyFile (localDriveName, fullResultsFileName);

  if  (copyOK)
  {
    osDeleteFile (localDriveName);
  }
  else
  {
    log.Level (-1) << endl
                   << "WriteResultsFileViaLocalDrive    **** ERROR ****"  << endl
                   << endl
                   << "Error copying Results file[" << resultsFileName << "]" << endl
                   << "From[" << localDrivePath << "]  To[" << networkPath << "]" << endl
                   << endl;

    results.WriteOutHistory (fullResultsFileName);

    osWaitForEnter ();
    exit (-1);
  }

} /* WriteResultsFileViaLocalDrive */






void  ActiveLearning::ProcessAllJobs ()
{
  log.Level (10) << "ActiveLearning::ProcessAllJobs" << endl;

  JobDescPtr  curJob = NULL;
  
  curJob =  GetNextJob ();
  while  (curJob)
  {
    switch  (curJob->JobType ())
    {
      case 'C': ProcessCombinedClassJob (curJob->SortOrder (), 
                                         curJob->Pass (),
                                         curJob->InitialTrainingImagesPerClass (),
                                         curJob->NewImagesPerRetraining ()
                                        );
                break;
    }

    FinishJob (curJob);
    delete  curJob;
    curJob = NULL;


//    bool  memoryLeakOccured = _CrtDumpMemoryLeaks ();
//    if  (memoryLeakOccured)
//    {
//      cout << "Memory Leak Occured." << endl;
//    }

    curJob =  GetNextJob ();
  }

}  /* ProcessAllJobs */








void  ActiveLearning::OpenReportFile ()
{
  report = new ofstream (reportFileName.Str (), ios::app);
}



void  ActiveLearning::CloseReportFile ()
{
  delete  report;
  report = NULL;
}




void  ActiveLearning::CreateInitialImages ()
{
  // Since we are going to be creating Data from images, then we know that the
  // FileDesc is the Plankton data set

  fileDesc = FeatureFileIOPices::NewPlanktonFile (log);

  FeatureVectorListPtr  emptyTrainingImagesList = new FeatureVectorList (fileDesc, true, log, 100);

  FeatureVectorListPtr  testImages = LoadImages (testImagesRootDir,  emptyTrainingImagesList);

  delete  emptyTrainingImagesList;
  emptyTrainingImagesList = NULL;

  if  (masterTestImages)
  {
    delete  masterTestImages;  
    masterTestImages = NULL;
  }

  if  (validationImages)
  {
    delete validationImages;
    validationImages = NULL;
  }

  masterTestImages = new FeatureVectorList (fileDesc, true, log, testImages->QueueSize ());
  validationImages = new FeatureVectorList (fileDesc, true, log, mlClasses->QueueSize () * validationImagesPerClass + 5);

  ExtractValidationImages (*mlClasses, *testImages, *masterTestImages, *validationImages);

  delete  testImages;
  testImages = NULL;
}  /* CreateInitialImages */







void  ActiveLearning::LoadInitialImages ()
{
  bool successful;
  bool changesMade = false;;
  bool cancelFlag  = false;

  if  (masterTestImages)
  {
    delete masterTestImages; 
    masterTestImages = NULL;
  }

  masterTestImages =  fileFormat->LoadFeatureFile (masterTestImagesFileName, 
                                                   *mlClasses,
                                                   -1,
                                                   cancelFlag,
                                                   successful,
                                                   changesMade,
                                                   log
                                                  );
    
  if  (!successful)
  {
    cerr << endl
         << "**** ERROR ****    LoadInitialImages,  Error Reading In Master Test Images"            << endl
         << "                                       from File[" << masterTestImagesFileName << "]"  << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  fileDesc = masterTestImages->FileDesc ();

  if  (validationImages)
  {
    delete validationImages;  
    validationImages = NULL;
  }

  validationImages = fileFormat->LoadFeatureFile (validationImagesFileName, 
                                                  *mlClasses,
                                                  -1,
                                                  cancelFlag,
                                                  successful,
                                                  changesMade,
                                                  log
                                                 );
   
   if  (!successful)
  {
    cerr << endl
         << "**** ERROR ****    LoadInitialImages,  Error Reading In Validation Images"             << endl
         << "                                       from File[" << validationImagesFileName << "]"  << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  if  (validationImages->FileDesc () != fileDesc)
  {
    // The two feature files are not of the same format.
    log.Level (-1) << endl
                   << " *****  Error Loading Validation Data[" << validationImagesFileName << "]  *****"
                   << "     Training Data and Validation data do not have same FileDsc" << endl
                   << endl
                   << "     masterTestImages->FileDesc->NumOfFields [" << masterTestImages->FileDesc ()->NumOfFields () << "]" << endl
                   << "     validationImages->FileDesc->NumOfFields [" << validationImages->FileDesc ()->NumOfFields () << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }
}  /* LoadInitialImages */





void  ActiveLearning::InitializeResultFile (SortOrderType  sortOrder,
                                             int            pass,
                                             int            iipc,
                                             int            ipr
                                            )
{
  KKStr  oldFileName = this->DeriveFullResultsFileName (sortOrder, iipc, ipr, pass);
  osDeleteFile (oldFileName);
}  /* InitializeResultFile */





void  ActiveLearning::ProcessStatusFile (int     argc,
                                         char**  argv
                                        )
{
  Block ();
  
  FILE* statusFile = OpenStatusFile ();
  if  (!statusFile)
  {
    //  We are doing our initial StartUp.  So lets set up the Status file.

    ProcessCmdLineParameters (argc, argv);
 
    if  (baseResultsFileName.Empty ())
    {
      baseResultsFileName = "ActiveLearningResults";
    }

    ourProcessNum  = 0;
    numOfProcesses = 1;


    if  (reportFileName.Empty ())
    {
      reportFileName = "ActiveLearningReport.txt";
    }

    if  (testImagesRootDir.Empty ())
    {
      testImagesRootDir =  "C:\\ActiveLearningTest\\TestImages";
    }

    if  (trainingImagesRootDir.Empty ())
    {
      trainingImagesRootDir = "C:\\ActiveLearningTest\\InitialTrainingSet";
    }

    initTrainImagesAssignmentFileName = "ActiveLearning_InitTrainImagesAssignments.data";

    if  ((osFileExists (masterTestImagesFileName))  &&  (osFileExists (validationImagesFileName)))
    {
      // We already had data files set up with there assignment to there
      // respective random passes(See class  PassAssignments)
      LoadInitialImages ();
    }
    else
    {
      CreateInitialImages ();

      bool  cancelFlag = false;
      bool  successful = false;
      uint  numExamplesWritten = 0;

      FeatureFileIOPices::Driver ()->SaveFeatureFile (masterTestImagesFileName, 
                                                     masterTestImages->AllFeatures (),
                                                     *masterTestImages,
                                                     numExamplesWritten,
                                                     cancelFlag,
                                                     successful,
                                                     log
                                                    );

      FeatureFileIOPices::Driver ()->SaveFeatureFile (validationImagesFileName, 
                                                     validationImages->AllFeatures (),
                                                     *validationImages,
                                                     numExamplesWritten, 
                                                     cancelFlag,
                                                     successful,
                                                     log
                                                    );
    }

    masterTrainingConfiguration = new TrainingConfiguration2 (fileDesc, 
                                                             configFileName, 
                                                             log
                                                            );

    delete  mlClasses;
    mlClasses = masterTrainingConfiguration->ExtractClassList ();

    if  (!performStreaming)
    {
      streamingBlockSize = masterTestImages->QueueSize ();
    }

    int  numOfClasses = mlClasses->QueueSize ();

    // Create Initial Job List
    JobDesc::InitializeNextJobID ();

    delete  jobs;  jobs = NULL;

    jobs = new JobDescList (true);

    uint  iipcIDX = 0;

    for  (iipcIDX = 0;  iipcIDX < iipcTable.size ();  iipcIDX++)
    {
      int  iipc = iipcTable[iipcIDX];
       
      KKStr  iipcSubDirName = DeriveIIPCsubDirectory (iipc);
      osCreateDirectoryPath (iipcSubDirName);


      // Lets set up Intial Assignments File 
      PassAssignmentsPtr  assignments = GetAssignments (iipc);
      delete  assignments;

      uint  iprIDX = 0;

      for  (iprIDX = 0;  iprIDX < iprTable.size ();  iprIDX++)
      {
        int  ipr = iprTable[iprIDX];

        KKStr  iprSubDirName = this->DeriveIPRsubSirectory (iipc, ipr);
        osCreateDirectoryPath (iprSubDirName);

        for  (int pass = 0;  pass < numOfRandomPasses;  pass++)
        {
          jobs->PushOnBack (new JobDesc (ProbabilityOrder, pass, iipc, ipr)); 
          InitializeResultFile (ProbabilityOrder, pass, iipc, ipr);

          jobs->PushOnBack (new JobDesc (BreakTieOrder,  pass, iipc, ipr));
          InitializeResultFile (BreakTieOrder, pass, iipc, ipr);
      
          if  (ipr == 10)
          {
            jobs->PushOnBack (new JobDesc (RandomOrder,  pass, iipc, ipr));
            InitializeResultFile (RandomOrder, pass, iipc, ipr);
          }
        }
      }
    }
    jobs->PushOnBack (new JobDesc ());  // Final Report Job


    UpdateCurrentStatusFile ();

    OpenReportFile ();

    *report << "Configuration File                [" << configFileName                      << "]."  << endl;
    *report << "Report File Name                  [" << reportFileName                      << "]."  << endl;
    *report << "Test Images Root Directory        [" << testImagesRootDir                   << "]."  << endl;
    *report << "Trainig Root Directory            [" << trainingImagesRootDir               << "]."  << endl;
    *report << "Base Results File Name            [" << baseResultsFileName                 << "]."  << endl;

    *report << "FinalNumOfImagesInTrainingLibrary [" << finalNumOfImagesInTrainingLibrary   << "]."  << endl;
    *report << "NumOfRandomPasses                 [" << numOfRandomPasses                   << "]."  << endl;

    *report << "MasterTestImagesFileName          [" << masterTestImagesFileName            << "]."  << endl;
    *report << "ValidationImagesFileName          [" << validationImagesFileName            << "]."  << endl;
    *report << "FileFormat                        [" << fileFormat->DriverName ()           << "]."  << endl;

    if  (performStreaming)
      *report << "Perform Streamin      Block Size  [" << streamingBlockSize                << "]." << endl;

    *report << endl;
    *report << "Validation Image Stats:"  << endl;
    *report << validationImages->ClassStatisticsStr () << endl;
    *report << endl;
    *report << "Test Image Stats:"  << endl;
    *report << masterTestImages->ClassStatisticsStr () << endl;
    *report << endl;

    CloseReportFile ();

    delete jobs;  jobs = NULL;
  }

  else
  {
    fclose (statusFile);
    LoadCurrentStatusFile ();
    LoadInitialImages ();
    masterTrainingConfiguration = new TrainingConfiguration2 (fileDesc, configFileName, log);
    delete  mlClasses;
    mlClasses = masterTrainingConfiguration->ExtractClassList ();
    ourProcessNum = numOfProcesses;
    numOfProcesses++;

    UpdateCurrentStatusFile ();

    delete jobs;  jobs = NULL;
  }  

  EndBlock ();
}  /* ProcessStatusFile */





// 2004-12-11  kk
// See Header file for descrotion of this constructor.
void  ActiveLearning::ProcessStatusFile ()   
{
  FILE* statusFile = OpenStatusFile ();
  if  (!statusFile)
  {
    log.Level (-1) << endl
                   << "ActiveLearning::ProcessStatusFile   **** ERROR ****"  << endl
                   << "                                    No Status File Exists."
                   << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  else
  {
    fclose (statusFile);
    LoadCurrentStatusFile ();
    LoadInitialImages ();
    masterTrainingConfiguration = new TrainingConfiguration2 (fileDesc, configFileName, log);
    delete  mlClasses;
    mlClasses = masterTrainingConfiguration->ExtractClassList ();
  }  
}  /* ProcessStatusFile */
