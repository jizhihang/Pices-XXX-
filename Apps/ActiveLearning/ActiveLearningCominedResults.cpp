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


#include  "SummaryByIPC.h"

#include  "ImageClass.h"
#include  "ImageFeatures.h"
#include  "RunLog.h"



RunLog    log;
KKStr    logFileName;
KKStr    rootDir;
KKStr    summaryFileName;
//KKStr    testDataFileName = "K:\\v1\\Plankton\\ActiveLearning\\Results\\LetterRecognition\\letter-recognition_TestData.data";

//  2004-12-07
//KKStr    testDataFileName = "C:\\users\\kkramer\\GradSchool\\Plankton\\ActiveLearning\\Results\\LetterRecognition6000\\letter-recognition_TestData.data";

//  2005-01-12      To get results with Standard Deveiation for paper
//KKStr    testDataFileName = "K:\\v1\\Plankton\\ActiveLearning\\Results\\2004_Sep_Expirements\\ActiveLearning_MasterTestImages.data";

// 2005-Jan-26
//KKStr    testDataFileName = "K:\\v1\\Plankton\\ActiveLearning\\Results\\2004_Sep_Expirements_Diff_Prior\\ActiveLearning_MasterData_Diff_Prior.data";


// 2005-01-17
// -D  C:\users\kkramer\GradSchool\Plankton\ActiveLearning\Results\2004_Sep_Expirements_Streaming\ActiveLearning_MasterTestImages.data

//FileFormat  testDataFileFormat = UCIFormat;

// 2005-01-28
// K:\v1\Plankton\ActiveLearning\Results\3_Class_ForJDR_Paper\Equal_Prior\010-IPC


// 2005-05-18
// C:\users\kkramer\GradSchool\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning
// 

FileFormat   inputFormat = RawFormat;

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

//    else if  ((switchStrUpper == "-D")  ||  (switchStrUpper == "-DATAFILE"))
//    {    
//      testDataFileName = parmStr;
//    }

    else if  ((switchStrUpper == "-F")  ||  (switchStrUpper == "-FORMAT"))
    {
      parmStr.Upper ();
      inputFormat = FileFormatFromStr (parmStr);
      if  (inputFormat == NULL_Format)
      {
        cerr << "*** ERROR *** Invalid File Format[" << parmStr << "]" << endl;
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







void  main (int argc,  char** argv)
{
  bool  validParms;

  ProcessCmdLine (argc, argv, validParms);

  if  (!validParms)
    exit (-1);


  if  (rootDir.Empty ())
  {
    rootDir = osGetCurrentDirectory ();
  }
    
  if  (!logFileName.Empty ())
  {
    log.AttachFile (logFileName);
  }


  bool  successful;

  ImageClassList  imageClasses (log);

//  ImageFeaturesList  masterTestImages (testDataFileName, 
//                                       inputFormat,
//                                       imageClasses,
//                                       log,
//                                       successful,
//                                       true
//                                      );
//if  (!successful)
//  {
//    cerr << endl
//         << "*** ERROR ***,  Could not load MasterTest feature file[" << testDataFileName << "]" << endl
//         << endl;
//    exit (-1);
//  }

  SummaryByIPC  ipcSummaries (log, rootDir, imageClasses, successful);

  if  (!successful)
  {
    cerr << endl
         << "*** ERROR ***,  SummaryByIPC  could not be constructed" << endl
         << endl;
    exit (-1);
  }
  else
  {
    ipcSummaries.GenerateReportAccuracyByClass (BreakTieOrder, 40);
    ipcSummaries.GenerateReportByIPR_ByTens    (BreakTieOrder);
    //ipcSummaries.GenerateSummaryReports ();

    ipcSummaries.GenerateGNUplotData  (BreakTieOrder);
    ipcSummaries.GenerateReportByIPRForSampleRate (BreakTieOrder, 40);
    ipcSummaries.GenerateReportsForJMLR ();

    
//    ipcSummaries.RecoverNewTrainingImages (ProbabilityOrder5, 
//                                           50, 
//                                           "c:\\Temp\\ActiveLearningNewTrainingImages"
//                                          );
  }
}  /* main */
