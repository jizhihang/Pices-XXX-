#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>
#include  <memory.h>
#include  <stdio.h>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;


#include  "OSservices.h"
#include  "Str.h"



#include  "ActiveLearning.h"
#include  "ImageClass.h"
#include  "ImageFeatures.h"
#include  "RunLog.h"
#include  "SummaryByIPC.h"


ActiveLearningPtr  activeLearning = NULL;  
FileFormat         inputFormat = RawFormat;
RunLog             runLog;
KKStr              logFileName;
KKStr              rootDir;
KKStr              summaryFileName;
KKStr              testDataFileName;


// 2004-10-17
// -R C:\users\kkramer\GradSchool\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning\010-IPC  -TD C:\users\kkramer\GradSchool\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning\NineClasses_TestData.data


// 2004-10-17
// -R K:\v1\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning\010-IPC  -td K:\v1\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning\NineClasses_TestData.data

// 2005-05-18
// K:\v1\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning
// -d NineClasses_TestData.data -R K:\v1\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning -F Raw

// 2006-06-28
// -r K:\v1\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning_New_BT3  -d NineClasses_TestData.data  -f RAW
// 


void  DisplayCmdLine ()
{
  cout << endl
       << endl
       << "AcriveLearningSummarizeResults   -R <Root Directory>  -L <Log File>" << endl
       << endl;
}  /* DisplayCmdLine */
 



void  ProcessCmdLine (int    argc,
                      char** argv,
                      bool&  validCmdLine
                     )
{
  validCmdLine = false;
  
  int  x = 1;
  while  (x < argc)
  {
    KKStr  switchStr (argv[x]);  x++;
    KKStr  switchStrUpper (switchStr);
    switchStrUpper.Upper ();

    KKStr  parmStr;
    KKStr  parmStrUpper;
    
    if  (switchStr[0] != '-')
    {
      cerr << endl
           << "Command Switch Expected,  found[" << switchStr << "]."  << endl;
      DisplayCmdLine ();
      return;
    }

    if  (x < argc)
    {
      if  (argv[x][0] != '-')
      {
        parmStr = argv[x];  x++;
        parmStrUpper = parmStr;
        parmStrUpper.Upper ();
      }
    }


    if  ((switchStrUpper == "-R")  ||  (switchStrUpper == "-ROOTDIR"))
    {
      if  (parmStr.Empty ())
      {
        cerr << endl
             << switchStr << " Requires a Parameter" << endl
             << endl;
        DisplayCmdLine ();
        return;
      }

      rootDir = parmStr;
      if  (!osValidDirectory (rootDir))
      {
        cerr << endl
             << "RootDir[" << rootDir << "]  Specified is not a Valid Directory" << endl
             << endl;

        DisplayCmdLine ();
        return;
      }
    }

    else if  ((switchStrUpper == "-D")  ||  (switchStrUpper == "-DATAFILE"))
    {    
      testDataFileName = parmStr;
    }

    else if  ((switchStrUpper == "-F")  ||  (switchStrUpper == "-FORMAT"))
    {
      parmStr.Upper ();
      inputFormat = FileFormatFromStr (parmStr);
      if  (inputFormat == NULL_Format)
      {
        cerr << endl
             << endl
             << "*** ERROR *** Invalid File Format[" << parmStr << "]" << endl
             << "              Supported Formats are " << FileFormatsReadOptionsStr () << endl
             << endl;

        DisplayCmdLine ();
        osWaitForEnter ();
        exit (-1);
      } 
    }


    else if  ((switchStrUpper == "-L")  ||  (switchStrUpper == "-LOG"))
    {
      if  (parmStr.Empty ())
      {
        cerr << endl
             << switchStr << " Requires a Parameter" << endl
             << endl;
        DisplayCmdLine ();
        return;
      }

      logFileName = parmStr;
    }

    else
    {
      cerr << endl
           << "Invalid Switch [" << switchStr << "]." << endl
           << endl;

      DisplayCmdLine ();
      return;
    }
  }

  validCmdLine = true;
}  /* ProcessCmdLine */




SummaryByIPCListPtr  LoadAllIPCSummaries (ImageClassListPtr  imageClasses)
{
  runLog.Level (10) << endl;
  runLog.Level (10) << endl;
  runLog.Level (10) << endl;
  runLog.Level (10) << "LoadAllIPCSummaries" << endl;
  runLog.Level (10) << endl;
  StringListPtr  dirList = osGetListOfDirectories (rootDir);
  if  (!dirList)
    return NULL;

  if  (dirList->QueueSize () < 1)
    return NULL;

  StringListIterator dIDX (*dirList);
  KKStrPtr          dirPath = NULL;

  SummaryByIPCListPtr  iipcSummaris = new SummaryByIPCList (true);

  for  (dIDX.Reset ();  dirPath = dIDX.CurPtr ();  ++dIDX)
  {
    runLog.Level (10) << "LoadAllIPCSummaries  Loading SummaryByIPCPtr for[" << *dirPath << "]." << endl;

    bool  successful;
    SummaryByIPCPtr  summary = new SummaryByIPC (runLog, osAddSlash (rootDir) + (*dirPath), *imageClasses, successful);
    if  (successful)
       iipcSummaris->PushOnBack (summary);
    else
       delete  iipcSummaris;
  }

  runLog.Level (10) << endl;
  runLog.Level (10) << "exiting       LoadAllIPCSummaries" << endl;
  runLog.Level (10) << endl;

  return  iipcSummaris;
}  /* LoadAllIPCSummaries */



void  GenerateReports (SummaryByIPCListPtr  summaries)
{
  runLog.Level (10) << endl;
  runLog.Level (10) << endl;
  runLog.Level (10) << endl;
  runLog.Level (10) << "GenerateReports" << endl;
  runLog.Level (10) << endl;

  SummaryByIPCListIterator  sIDX (*summaries);
  SummaryByIPCPtr  summary = NULL;

  for  (sIDX.Reset ();  summary = sIDX.CurPtr ();  ++sIDX)
  {
    runLog.Level (10) << endl;
    runLog.Level (10) << "GenerateReports IIPC[" << summary->IPC ()    << "]." << endl;
    runLog.Level (10) << "                Dir [" << summary->RootDir () << "]." << endl;
    runLog.Level (10) << endl;

    summary->GenerateGNUplotData  (BreakTieOrder);
    summary->GenerateReportByIPRForSampleRate (BreakTieOrder, 40);
    summary->GenerateReportsForJMLR ();
    summary->GenerateReportAccuracyByClass (BreakTieOrder, 40);
    summary->GenerateReportAccuracyByClass (RandomOrder,       40);
  }

  runLog.Level (10) << endl;
  runLog.Level (10) << "exiting     GenerateReports" << endl;
  runLog.Level (10) << endl;
}  /* GenerateReports */



void  CombinedSummaryReport (SummaryByIPCListPtr  summaries,
                             SortOrderType        sortOrder,
                             int                  ipr,
                             int                  increment
                            )
{
  float    accuracyMean;
  float    accuracyMin;
  float    accuracyMax;
  float    accuracyStdDev;
  float    accuracyStdErr;
  float    accuracyDelta;
  float    supportPointsMean;
  float    supportPointsMin;
  float    supportPointsMax;
  float    supportPointsStdDev;
  float    supportPointsStdErr;
  float    trainingTimeMean;
  float    testingTimeMean;
  bool     found;
  int      imageCount;
  int      x = 0;

  KKStr  reportFileName = osAddSlash (rootDir) + 
                           "CombinedSummaryReport_" +
                           SortOrderDescrShort (sortOrder) + 
                           "_IPR-" + StrFormatInt (ipr, "ZZ00") + 
                           "_ICR-" + StrFormatInt (increment, "ZZ00") + 
                           ".txt";

  ofstream  r (reportFileName.Str ());

  int  numOfSummaries = summaries->QueueSize ();

  int*   counts      = new int [numOfSummaries];
  int**  imageCounts = new int*[numOfSummaries];
  int    imageCountMin = INT_MAX;
  int    imageCountMax = INT_MIN;

  r << "Count" << "\t" << "Random";
  for  (x = 0;  x < numOfSummaries;  x++)
  {
    SummaryByIPCPtr sum = summaries->IdxToPtr (x);
    r << "\t" << sum->IPC ();
    sum->GetImageCountsByIncrements (ipr, sortOrder, increment, counts[x], imageCounts[x]);
    if  (counts[x] > 0)
    {
      imageCountMin = Min (imageCountMin, imageCounts[x][0]);
      imageCountMax = Max (imageCountMax, imageCounts[x][counts[x] - 1]);
    }
  }
  r << endl;

  imageCount = imageCountMin;

  while  (imageCount <= imageCountMax)
  {
    r << imageCount;
    SummaryByIPCPtr sum = summaries->IdxToPtr (0);
    sum->GetStatsForNumOfImages (1, RandomOrder, imageCount,
                                 accuracyMean,
                                 accuracyMin,
                                 accuracyMax,
                                 accuracyStdDev,
                                 accuracyStdErr,
                                 accuracyDelta,
                                 supportPointsMean,
                                 supportPointsMin,
                                 supportPointsMax,
                                 supportPointsStdDev,
                                 supportPointsStdErr,
                                 trainingTimeMean,
                                 testingTimeMean,
                                 found
                                );

    r << "\t";
    if  (found)
      r << accuracyMean;
    
    for  (x = 0; x < numOfSummaries; x++)
    {
      sum = summaries->IdxToPtr (x);
      r << "\t";
      sum->GetStatsForNumOfImages (ipr, sortOrder, imageCount,
                                   accuracyMean,
                                   accuracyMin,
                                   accuracyMax,
                                   accuracyStdDev,
                                   accuracyStdErr,
                                   accuracyDelta,
                                   supportPointsMean,
                                   supportPointsMin,
                                   supportPointsMax,
                                   supportPointsStdDev,
                                   supportPointsStdErr,
                                   trainingTimeMean,
                                   testingTimeMean,
                                   found
                                  );
      if  (found)
        r << accuracyMean;
    }
    r << endl;

    if  ((imageCount % 20) == 0)
      imageCount += increment;
    else
      imageCount += 10;

  }

  r.close (); 

  for  (x = 0;  x < numOfSummaries;  x++)
    delete  imageCounts[x];

  delete  imageCounts;
  delete  counts;
}  /* CombinedSummaryReport */



void  main (int argc,  char** argv)
{
  bool  validParms;

  ProcessCmdLine (argc, argv, validParms);

  if  (!validParms)
    exit (-1);

  if  (!logFileName.Empty ())
  {
    runLog.AttachFile (logFileName);
  }

  if  (rootDir.Empty ())
  {
    rootDir = osGetCurrentDirectory ();
  }

  KKStr  activeLearningStatusFileName = osAddSlash (rootDir) + "ActiveLearning.status";
  if  (!osFileExists (activeLearningStatusFileName))
  {
    cerr << endl
         << endl
         << "*** ERROR ***        No Active leaning Status fikle Present" << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }


  activeLearning = new ActiveLearning (rootDir);
  activeLearning->AssignLog (runLog);

  ImageClassList&  imageClasses (*activeLearning->ImageClasses ());
    
  // We now need to load all the iipc summary files.

  SummaryByIPCListPtr  summaries = LoadAllIPCSummaries (activeLearning->ImageClasses ());

  if  (!summaries)
  {
    cerr << endl
         << endl
         << "*** ERROR ***    Could not load any summaries" << endl
         << endl;
    exit (-1);
  }

  GenerateReports (summaries);
  CombinedSummaryReport (summaries, BreakTieOrder, 1, 20);

  delete summaries;
  delete activeLearning;
}  /* main */
