#include  "FirstIncludes.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#include <iomanip>


#include  <fstream>

#ifdef  WIN32
#include  <ostream>
#endif

#include  <iostream>
#include  <vector>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;



#include  "BasicTypes.h"
#include  "OSservices.h"


#include  "ResultLine.h"
#include  "SingleRun.h"
#include  "SingleRunList.h"
#include  "SummaryLine.h"
#include  "TotalLine.h"


#include  "CrossValidation.h"
#include  "FeatureFileIO.h"
#include  "FeatureNumList.h"
#include  "ImageClass.h"
#include  "RunLog.h"
#include  "StatisticalFunctions.h"
#include  "TrainingConfiguration.h"


//  2004-10-06   
//  K:\Thesis\TestSet9Classes\300ImagesPerClass\Using5OldContourFfeatures\NineClasses_ValidationData.data.result

// 2004-10-03   Process feature Selection that uses 16 new Contour features
// C:\users\kkramer\GradSchool\Thesis\TestSet9Classes\Using16NewContourfeatures\NineClasses_ValidationData.data.result

//  K:\v1\Plankton\TempBeamSearch\2004_March_02\BinaryRuns\2004_03_02_TestData.data.result

//  C:\users\kkramer\GradSchool\Plankton\BeamSearches\GrayScalePaperFeatureSearch\ActiveLearning_ValidationImages.data.result


//  2005-01-06     tesis 9 Class feature Search Results
//  -R NineClasses_ValidationData.data.result  -C Thesis9Classes.cfg  -T NineClasses_TestData.data 

//  2005-01-07     tesis 9 Class feature Search Results
//  -R NineClasses_ValidationData.data.result  -C Thesis9Classes.cfg  -T NineClasses_ValidationData.data -V NineClasses_TestData.data 

// 2005-01-10      Tesis 9 Classes  Binary results
//  -R NineClasses_TestData.data.result  -C Thesis9Classes.cfg  -V NineClasses_ValidationData.data -T NineClasses_TestData.data 
// K:\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinaryFeatureSearch


// 2005-02-09
//  -R NineClasses_TestData.data.result  -C Thesis9Classes.cfg  -V NineClasses_ValidationData.data -T NineClasses_TestData.data 
//  C:\users\kkramer\GradSchool\Thesis\TestSet9Classes\300_ImagesPerClass\UsingOld5ContourFeatures\BinaryFeatureSearch

// 2005-03-17
// K:\v1\Plankton\BeamSearches\OnePercentModel
// -r OnePercentModelTestData.data.result -c "1% and greater model.cfg"  -T OnePercentModelTestData.data   -v OnePercentModelValidationData.data


// 2005-03-24      Tesis 9 Classes  Binary results
//  -R NineClasses_TestData.data.result  -C Thesis9Classes.cfg  -V NineClasses_ValidationData.data -T NineClasses_TestData.data 
// C:\users\kkramer\GradSchool\Thesis\TestSet9Classes\300_ImagesPerClass\UsingOld5ContourFeatures\BinaryFeatureSearch


// 2005-03-26      Tesis 9 Classes  Binary results
//  -R OnePercentModelTestData_TrainingData.data.result  -C "1% and greater model.cfg"  -V OnePercentModelTestData_ValidationData.data -T OnePercentModelTestData_TrainingData.data 
// K:\v1\Plankton\BeamSearches\OnePercentModel_500


// 2005-03-27      Binary Class feature Selection,   Meant to work wioth andrews latest Training Library
//  -R OnePercentModelTestData_TrainingData.data.result  -C "1% and greater model.cfg"  -V OnePercentModelTestData_ValidationData.data  -T OnePercentModelTestData_TrainingData.data
// K:\v1\Plankton\BeamSearches\OnePercentModel_500\Binary


// 2005-04-19      Binary Class feature Selection,   part of Data mining using Punishment Factor
//  -R K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinaryFeatureSearch_10-PunishmentFactor -C "Thesis9Classes.cfg"  -V NineClasses_ValidationData.data  -T NineClasses_TestData.data  -P 0.10 -u 12
//  K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinaryFeatureSearch_10-PunishmentFactor

// 2005-04-21      Binary Class feature Selection,   part of Data mining using Punishment Factor
// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinaryFeatureSearch_01-PunishmentFactor
//-R K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinaryFeatureSearch_01-PunishmentFactor -C "Thesis9Classes.cfg"  -V NineClasses_ValidationData.data  -T NineClasses_TestData.data  -P 0.01 -u 12
//-R K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinaryFeatureSearch_01-PunishmentFactor -C "Thesis9Classes.cfg"  -V NineClasses_ValidationData.data  -T NineClasses_TestData.data  -P 0.01 -u 12

// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinaryFeatureSearch_00-PunishMentFactor
//-R K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinaryFeatureSearch_00-PunishMentFactor  -C "Thesis9Classes.cfg"  -V NineClasses_ValidationData.data  -T NineClasses_TestData.data  -P 0.00 -u 9

// 2005-05-27      results of Feature Selection  of Thesis Data,  using new improved serach, where we every 10 we search out.
// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures_NewSearch
// -r K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures_NewSearch\NineClasses_ValidationData.data.result   -C "Thesis9Classes.cfg"  -V NineClasses_ValidationData.data  -T NineClasses_TestData.data

// KNS C:\Temp
// -r NineClasses_ValidationData.data.result   -C "Thesis9Classes.cfg"  -V NineClasses_ValidationData.data  -T NineClasses_TestData.data

// 2005-06-06
//  K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures_BestFirstOut
// -r NineClasses_ValidationData.data.result   -C "Thesis9Classes.cfg"  -V NineClasses_ValidationData.data  -T NineClasses_TestData.data

// 2005-10-19 
// Want to add number of Support Points
//  K:\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures
// -r NineClasses_ValidationData.data.result   -C "Thesis9Classes.cfg"  -V NineClasses_ValidationData.data  -T NineClasses_TestData.data

// C:\users\kkramer\GradSchool\Thesis\TestSet9Classes\300_ImagesPerClass\BinaryFeatureSearch
// -c AllTrainingImages_Trimmed.cfg -t AllTrainingImages_Trimmed.data

// 2005-12-14
// K:\v1\Plankton\FeatureSelections\2005-12-09
// -c Spain.cfg  -T TrainingImages_2005-12-09.data

// -c Thesis9Classes.cfg  -DataFile  NineClasses_TestData.data  -R NineClasses_TestData_AllFeatures.txt


//K:\v1\Plankton\BeamSearches\2006-03-14
// -c FiveBigest.cfg  -ResultFile ActiveLearning_MasterTestImages_shuffled.data.result -TestFile ActiveLearning_MasterTestImages_shuffled.data  -ValidationFile ActiveLearning_ValidationImages.data

// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures_2006-03-28
// -c Thesis9Classes.cfg    -ResultFile NineClasses_ValidationData.data.result -TestFile NineClasses_ValidationData.data  -ValidationFile NineClasses_TestData.data

// K:\Plankton\Papers\FastFeatureSelAlgorithm\Wrapper1
// -c ..\FiveBigest.cfg   -ResultFile FiveClassPaper_FeatureSelection1.result -TestFile ..\FiveClassPaper_FeatureSelection1.data  -TestFile2 ..\FiveClassPaper_TrainingSubset1.data   -ValidationFile ..\FiveClassPaper_TestSet1.data
// -c ..\FiveBigest.cfg   -ResultFile FiveClassPaper_FeatureSelection1.result -TestFile ..\FiveClassPaper_FeatureSelection1.data  -TestFile2 ..\FiveClassPaper_TrainingSubset1.data   -ValidationFile ..\FiveClassPaper_TestSet1.data

// C:\Documents and Settings\djgarcia\Desktop\FastFeatureSelectionAlgorithm\Wrapper1
// -c ..\FiveBigest.cfg   -ResultFile FiveClassPaper_FeatureSelection1.result -TestFile ..\FiveClassPaper_FeatureSelection1.data  -TestFile2 ..\FiveClassPaper_TrainingSubset1.data   -ValidationFile ..\FiveClassPaper_TestSet1.data

// C:\bigpine\LI\Data
// -c LI.cfg   -ResultFile traindata1.result -Format Col -TestFile traindata1    -ValidationFile testdata1
 

// C:\bigpine\LI\Data\Data_200F\Fold1
// -c ..\LI.cfg   -ResultFile traindata1.result -Format Col -TestFile ..\traindata1    -ValidationFile ..\testdata1

// C:\bigpine\LI\Data\Data_100F-2\Fold1
// -c ..\LI.cfg   -ResultFile traindata1.result -Format Col -TestFile ..\traindata1    -ValidationFile ..\testdata1

// -c ..\LI.cfg  -USEDONE -ResultFile colon.train_TestData.result -Format Sparse -TestFile ..\colon.train_TestData.data    -ValidationFile ..\colon.train_ValidationData.data  -USEDONE


typedef  vector<FFLOAT>  FFLOATVector;

bool                      beamSearchResults  = false;
bool                      cancelFlag         = false;
TrainingConfigurationPtr  config             = NULL;
KKStr                     configFileName     = "";
FileDescPtr               fileDesc           = NULL;
ImageClassListPtr         imageClasses       = NULL;
FileFormat                fileFormat         = RawFormat;
float                     punishmentFactor   = 0.0f;
KKStr                     resultFileName     = "";
KKStr                     modelFileName      = "";
int                       maxNumOfFeatures   = -1;
bool                      processMultipleWrappers  = false;
bool                      processDoneStatusFile    = false;
RunLog                    runLog;
KKStr                     testFileName;
KKStr                     testFileName2;
ImageFeaturesListPtr      testData           = NULL;
ImageFeaturesListPtr      testData2          = NULL;
int                       upOneCount         = 0;
KKStr                     validationFileName = "";
ImageFeaturesListPtr      validationData     = NULL;




void  PrintOutSummaryOfHighestAccuracies (ostream&        o, 
                                          TotalLineList&  totals
                                         )
{
  int  x;

  // 1st Title Line
  o                                << "\t"
                                   << "\t"
    << "Highest Accuracy Result"   << "\t"
                                   << "\t"
                                   << "\t"
    << "Statistcally Same" 
    << endl;

  // 2nd Title Line
  o << "Classes"        << "\t"
    << "NumNodes"       << "\t"
    << "FeatureCount"   << "\t"
    << "Accuracy"       << "\t"
    << "Features"       << "\t"
    << "FeatureCount"   << "\t"
    << "Accuracy"       << "\t"
    << "Features"
    << endl;

  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    TotalLinePtr  t = totals.IdxToPtr (x);

    ResultLinePtr  highestAccuracy      = t->ResultWithHighestAccuracy ();
    ResultLinePtr  statisticallySameJob = t->StatisticallySameJob ();

    o << t->Classes ()->ToString () << "\t"
      << t->NumOfResults ()         << "\t";

    if  (!highestAccuracy)
    {
      o << "\t\t\t";
    }

    else
    {
      o << highestAccuracy->NumOfFeatures ()               << "\t"
        << highestAccuracy->Accuracy () << "%"             << "\t"
        << highestAccuracy->GetFeatureNums ().ToString ()  << "\t";
    }

    if  (!statisticallySameJob)
    {
      o << "\t\t";
    }

    else
    {
      o << statisticallySameJob->NumOfFeatures () << "\t"
        << statisticallySameJob->Accuracy () << "%"               << "\t"
        << statisticallySameJob->GetFeatureNums ().ToString ();
    }

    o << endl;
  }

  o << endl;

}  /* PrintOutSummaryOfHighestAccuracies */





void  PrintOutHighestAccuracyByNumOfFeatures (ostream&        o, 
                                              TotalLineList&  totals
                                             )
{
  int   numOfFeatures;
  int  x;

  // Print Title Line

  o << "NumOf";

  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    o << "\t" << totals[x].Class1Name ();
  }
  o << endl;

  o << "Features";
  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    o << "\t" << totals[x].Class2Name ();
  }
  o << endl;

  for  (numOfFeatures = 1;  numOfFeatures <= maxNumOfFeatures;  numOfFeatures++)
  {
    o << numOfFeatures;

    for  (x = 0;  x < totals.QueueSize ();  x++)
    {
      o << "\t" << StrFormatDouble (totals[x].AccuracyByNumOfFeatuers (numOfFeatures), "ZZ0.000") << "%"; 
    }
    o << endl;
  }
}  /* PrintOutHighestAccuracyByNumOfFeatures */








void  PrintOutFeatureImpact (ostream&        o, 
                             TotalLineList&  totals
                            )

{
  int   featureNum;
  int   x;


  o << "Feature"  << "\t" << "Feature";

  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    o << "\t\t" << totals[x].Class1Name ();
  }
  o << endl;



  o << "Num"  << "\t"  << "Description";

  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    o << "\t\t" << totals[x].Class2Name ();
  }
  o << endl;



  for  (featureNum = 0;   featureNum < maxNumOfFeatures;   featureNum++)
  {
    o << featureNum << "\t" << fileDesc->FieldName (featureNum); 
    for  (x = 0;  x < totals.QueueSize ();  x++)
    {
      FFLOAT  mean;
      FFLOAT  stdDev;

      totals[x].CalcFeatureChangeStats (featureNum, mean, stdDev);

      o << "\t" << StrFormatDouble (mean,   "ZZZ0.000") << "%"
        << "\t" << StrFormatDouble (stdDev, "ZZZ0.0000") << "%";
    }

    o << endl;
  }
  
}  /* PrintOutFeatureImpact */









void  PrintOutFeatureUssageForTop100 (ostream&        o, 
                                      TotalLineList&  totals
                                     )

{
  int  featureNum;
  int  x;



  o << "Feature"  << "\t" << "Feature";

  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    o << "\t" << totals[x].Class1Name ();
  }
  o << endl;



  o << "Num"  << "\t"  << "Description";

  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    o << "\t" << totals[x].Class2Name ();
  }
  o << endl;



  for  (featureNum = 0;   featureNum < maxNumOfFeatures;   featureNum++)
  {
    o << featureNum << "\t" << fileDesc->FieldName (featureNum);

    for  (x = 0;  x < totals.QueueSize ();  x++)
    {
      o << "\t" << totals[x].UssageInTop100 (featureNum);
    }

    o << endl;
  }
}  /* PrintOutFeatureUssageForTop100  */





void  PrintOutFeatureUssageForTop200 (ostream&        o, 
                                      TotalLineList&  totals
                                     )

{
  int  featureNum;
  int  x;

  o << "Feature"  << "\t" << "Feature";

  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    o << "\t" << totals[x].Class1Name ();
  }
  o << endl;

  o << "Num"  << "\t"  << "Description";
  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    o << "\t" << totals[x].Class2Name ();
  }
  o << endl;


  for  (featureNum = 0;   featureNum < maxNumOfFeatures;   featureNum++)
  {
    o << featureNum << "\t" << fileDesc->FieldName (featureNum);

    for  (x = 0;  x < totals.QueueSize ();  x++)
    {
      o << "\t" << totals[x].UssageInTop200 (featureNum);
    }

    o << endl;
  }
  
}  /* PrintOutFeatureUssageForTop200  */






KKStr  beamSearchLine  = "OVER TO BEAM SEARCH";
KKStr  newBinarySearchLine = "Switching To Next Binary Class";




ResultLinePtr  GetNextRecord (FILE*              in,
                              ImageClassListPtr  imageClasses,
                              bool               newFormat,     // If using result file from new
                              ResultLineList&    results        // (Binary)FeatureSelection app.
                             )

{
  char  buff[40960];

  bool  nextLineFound = false;

  ResultLinePtr  resultLine = NULL;

  while  (!nextLineFound)
  {
    if  (!fgets (buff, sizeof (buff), in))
    {
      return NULL;
    }

    KKStr  line (buff);

    line.TrimRight ();

    if  (line.Empty ())
    {
      continue;
    }

    else if  (line.SubStrPart (0, 1) == "//")
      continue;
    
    else if  (line.SubStrPart (0, 16) == "Included_Features")
    {
      continue;
    }

    else if  (line.StrInStr (beamSearchLine))
    {
      beamSearchResults = true;
    }

    else  if  (line.StrInStr (newBinarySearchLine))
    {
      beamSearchResults = false;
    }
    
    else
    {
      resultLine = new ResultLine (fileDesc,
                                   line, 
                                   nextLineFound, 
                                   imageClasses, 
                                   runLog,
                                   beamSearchResults,
                                   newFormat,
                                   results
                                  );
      if  (!nextLineFound)
      {
        delete  resultLine;
        resultLine = NULL;
      }
    }
  }

  if  (!nextLineFound)
  {
    delete  resultLine;
    resultLine = NULL;
  }

  return  resultLine;
}  /* GetNextRecord */








TotalLinePtr  ProcessResults (ImageClassListPtr     classes,
                              ResultLineList&       results,
                              ostream&              o,
                              bool                  sortByGrade,
                              ImageFeaturesListPtr  testData,
                              ImageFeaturesListPtr  validationData
                             )
{

  runLog.Level (10) << "ProcessResults." << endl;

  ResultLinePtr  rl;
  int            x;


  // Lets first check for Missing Jjob Id's
  {
    results.SortByJobId ();
    
    ResultLineList::iterator rlIDX = results.begin ();
    int  lastJobId = (*rlIDX)->JobId ();
    rlIDX++;
    for  (;  rlIDX != results.end ();  rlIDX++)
    {
      int nextJobId = (*rlIDX)->JobId ();
      if  ((lastJobId +1 ) != nextJobId)
      {
        KKStr  errMsg;
        errMsg << "Missing Job Id's[" << (lastJobId + 1) << " - " << (nextJobId - 1) << "]";
        cerr << errMsg << endl;
        o  << endl 
           << errMsg << endl
           << endl;
      }

      lastJobId = nextJobId;
    }
  }


  TotalLinePtr  totalLine = new TotalLine (fileDesc,
                                           classes,
                                           runLog,
                                           config,
                                           testData,
                                           validationData,
                                           sortByGrade
                                          );

  if  (sortByGrade)
  {
    results.SortByGrade ();
    for  (x = 0;  x < results.QueueSize ();  x++)
    {
      rl = results.IdxToPtr (x);
      totalLine->UpdateTotalsByGrades (x, rl);
    }
  }


  results.SortByAccuracy ();

  for  (x = 0;  x < results.QueueSize ();  x++)
  {
    rl = results.IdxToPtr (x);
    totalLine->UpdateTotals (x, rl);
  }

  runLog.Level (10) << "ProcessResults   Returning." << endl;

  return  totalLine;
}  /* ProcessResults */






#define  MAXFIELDS 100




double  Accuracy (TotalLineList&  totals,
                  ImageClassPtr   class1, 
                  ImageClassPtr   class2,
                  int             featureCount
                 )
{
  int  x;

  double  accuracy = 0.0;

  for  (x = 0;  x < totals.QueueSize ();  x++)
  {
    if  (((totals[x].Class1 () == class1)  &&
          (totals[x].Class2 () == class2))
        ||
         ((totals[x].Class1 () == class2)  &&
          (totals[x].Class2 () == class1))
        )
    {
      accuracy = totals[x].AccuracyByNumOfFeatuers (featureCount);
      break;
    }
  }

  return  accuracy;
}  /* Acuracy */




void  DisplayCmdLine ()
{
  cout << endl
       << endl
       << "ParsebeamSearchresults  -ConfigFile      <Configuration File Name>"      << endl
       << "                        -FileFormat      <Raw | Raw29 | Sparse | UCI>"   << endl
       << "                        -Punishment      <Punishment Factor, 0.0<->1.0>" << endl
       << "                        -ResultFile      <Result File Name>"             << endl
       << "                        -TestFile        <Test File Data>"               << endl
       << "                        -UpOneCount      <Number obove same>"            << endl
       << "                        -ValidationFile  <Validation File Name>"         << endl
       << endl;
}  /* DisplayCmdLine */





KKStr  CmdLineStr ()
{
  KKStr  cmdLine;

  cmdLine << "-ResultFile "  << resultFileName;

  if  (testData  ||  validationData)
  {
    cmdLine << "  FileFormat " << FileFormatDescription (fileFormat);
  }
  
  if  (config)
    cmdLine << "  -ConfigFile " << configFileName;

  if  (testData)
    cmdLine << "  -TestFile "   << testFileName;

  if  (!testFileName2.Empty ())
    cmdLine << "  -TestFile2 "  << testFileName2;

  if  (validationData)
    cmdLine << "  -ValidationFile "   << validationFileName;

  cmdLine << "-Punishment " << punishmentFactor;

  return  cmdLine;
}  /* CmdLineStr */





void  ProcessCmdLine (int     argc,
                      char**  argv
                     )
{
  int  x = 1;

  while  (x < argc)
  {
    KKStr   switchStr (argv[x]);
    switchStr.Upper ();

    if  (switchStr[0] != '-')
    {
      cerr << endl
           << "Invalid Switch[" << switchStr << "]" << endl
           << endl;
      DisplayCmdLine ();
      exit (-1);
    }

    x++;

    KKStr  parm;
    KKStr  parmUpper;
    if  (x < argc)
    {
      if  (argv[x][0] != '-')
      {
        parm = argv[x];
        x++;

        parmUpper = parm;
        parmUpper.Upper ();
      }
    }

    if  ((switchStr == "-C")  ||  (switchStr == "-CONFIG") ||  (switchStr == "-CONFIGFILE"))
    {
      configFileName = parm;
    }

    else if  ((switchStr == "-F")  ||  (switchStr == "-FORMAT") ||  (switchStr == "-FILEFORMAT"))
    {
      fileFormat = FileFormatFromStr (parm);
      if  (fileFormat == NULL_Format)
      {
        runLog.Level (-1) << endl
                          << "        *** ERROR ***      Invalid File Format["  << parm << "]."  << endl
                          << endl;
        DisplayCmdLine ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (switchStr == "-USEDONE")
      processDoneStatusFile = true;

    else if  ((switchStr == "-P")  ||  (switchStr == "-PUNSIHMENT"))
    {
      punishmentFactor = (float)atof (parm.Str ());

      if  ((punishmentFactor < 0.0f)  ||  (punishmentFactor > 1.0f))
      {
        runLog.Level (-1) << endl
                          << "        *** ERROR ***      PunishmentFactor[" << punishmentFactor << "]" << endl
                          << "                          must be between 0.0 and 1.0"                   << endl
                          << endl;
        DisplayCmdLine ();
        osWaitForEnter ();
        exit (-1);
      }
    }


    else if  ((switchStr == "-R")  ||  (switchStr == "-RESULT") ||  (switchStr == "-RESULTFILE"))
    {
      resultFileName = parm;
    }


    else if  ((switchStr == "-T")  ||  (switchStr == "-TEST") ||  (switchStr == "-TESTFILE"))
    {
      testFileName = parm;
    }


    else if  (switchStr == "-TESTFILE2")
    {
      testFileName2 = parm;
    }



    else if  ((switchStr == "-UPONECOUNT")  ||  (switchStr == "-UO") ||  (switchStr == "-U"))
    {
      upOneCount = atoi (parm.Str ());
    }

    else if  ((switchStr == "-V")  ||  (switchStr == "-VALIDATION") ||  (switchStr == "-VALIDATIONFILE"))
    {
      validationFileName = parm;
    }
  }


  if  (!testFileName.Empty ())
  {
    bool  successful;
    bool  changesMade = false;
    
     
    testData = LoadFeatureFile (testFileName, fileFormat, *imageClasses, -1, runLog, cancelFlag, successful, changesMade);
    if  (!successful)
    {
      runLog.Level (-1) << endl
                        << "      *** ERROR ***    Invalid Test Data File[" << testFileName << "]." << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }

    fileDesc = testData->FileDesc ();
  }


  if  (!testFileName2.Empty ())
  {
    bool  successful;
    bool  changesMade = false;
     
    testData2 = LoadFeatureFile (testFileName2, 
                                 fileFormat, 
                                 *imageClasses, 
                                 -1, 
                                 runLog, 
                                 cancelFlag,
                                 successful,
                                 changesMade
                                );
    if  (!successful)
    {
      runLog.Level (-1) << endl
                        << "      *** ERROR ***    Invalid Test Data File 2[" << testFileName2 << "]." << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }

    if  (testData)
    {
      if  (fileDesc != testData2->FileDesc ())
      {
        runLog.Level (-1) << endl
                          << "      *** ERROR ***   [" << testFileName << "]  and  [" << testFileName2 << "]" << " File descriptions don't match." << endl
                          << endl;
        osWaitForEnter ();
        exit (-1);
      }

      testData->AddQueue (*testData2);
      testData2->Owner (false);
      delete  testData2;
      testData2 = NULL;
    }
    else
    {
      fileDesc = testData2->FileDesc ();
      testData = testData2;
      testData2 = NULL;
    }
  }



  if  (!validationFileName.Empty ())
  {
    bool  successful;
    bool  changesMade = false;
    validationData = LoadFeatureFile   (validationFileName, fileFormat, *imageClasses, -1, runLog, cancelFlag, successful, changesMade);
    if  (!successful)
    {
      runLog.Level (-1) << endl
                        << "      *** ERROR ***    Invalid Validation File[" << validationFileName << "]." << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }

    if  (!fileDesc)
      fileDesc = validationData->FileDesc ();
  }


  if  (testData  &&  configFileName.Empty ())
  {
    runLog.Level (-1) << endl
                      << "      *** ERROR ***   You specified TestData["  << testFileName << "]  but no configuration file." << endl
                      << endl;
    osWaitForEnter ();
    exit (-1);
  }


  if  (validationData  &&  !testData)
  {
    runLog.Level (-1) << endl
                      << "      *** ERROR ***   You specified  Validation Data["  << validationFileName << "]  but no Test Data to use as training data." << endl
                      << endl;
    osWaitForEnter ();
    exit (-1);
  }


  if  (!configFileName.Empty ())
  {
    if  (!fileDesc)
    {
      runLog.Level (-1) << endl
                        << endl
                        << " You specified a Config file but nit testData file." << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }


    config = new TrainingConfiguration (fileDesc, configFileName, runLog, false);
    if  (!config->FormatGood ())
    {
      runLog.Level (-1) << endl
                        << "           *** ERROR ***        Invalid Configuration File[" << configFileName << "]." << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }
  }
}  /* ProcessCmdLine */





void  PrintRunTimeParameters (ostream&  o)
{
  DateTime  curTime = osGetLocalDateTime ();

  o << "Current Date Time    ["  << osGetLocalDateTime () << "]"     << endl;
  o << "Curent Directory     ["  << osGetCurrentDirectory () << "]"  << endl;
  o << "Result File          ["  << resultFileName        << "]    File Time[" << osGetFileDateTime (resultFileName) << "]" << endl;

  if  (configFileName.Empty ())
    o << "Config File          [" << configFileName << "]    File Time["  << osGetFileDateTime (configFileName) << "]" << endl;

  o << "Up One Count         [" << upOneCount << "]" << endl;

  if  (testData  ||  validationData)
    o << "File Format          [" << FileFormatDescription (fileFormat) << "]" << endl;

  if  (testData)
    o << "Test File Name       [" << testFileName << "]    File Time[" << osGetFileDateTime (testFileName) << "]" << endl;

  if  (!testFileName.Empty ())
    o << "Test File Name 2     [" << testFileName2 << "]   File Time[" << osGetFileDateTime (testFileName2) << "]" << endl;

  if  (validationData)
    o << "Validation File Name [" << validationFileName << "]    File Time[" << osGetFileDateTime (validationFileName) << "]" << endl;

  o << endl;
  o << "Image Classes        [" << (*imageClasses)  << "]" << endl;
  o << "Punishment Factor    [" << punishmentFactor << "]" << endl;
  
  if  (testData)
  {
    o << endl
      << endl
      << "Test Data Class Statistics" << endl
      << testData->ClassStatisticsStr ()
      << endl;
  }

  if  (validationData)
  {
    o << endl 
      << endl
      << "Validation Data Class Statistics" << endl
      << validationData->ClassStatisticsStr ()
      << endl;
  }
}  /* PrintRunTimeParameters */






ResultLineListPtr  LoadBinaryClassResultLines (KKStr         rootDir,
                                               ImageClassPtr  class1,
                                               ImageClassPtr  class2,
                                               bool           newFormat
                                              )
{
  KKStr  resultFileName = osAddSlash (rootDir)  +
                           class1->Name () + "_" + 
                           class2->Name ()       + 
                           ".Feature_Results";

  if  (resultFileName == "artifact_lines_Chaetognath.Feature_Results")
  {
    cout << "Write Name" << endl;
  }

  FILE*  in = osFOPEN (resultFileName.Str (), "r");
  if  (!in)
    return NULL;

  ResultLineListPtr  results = new ResultLineList (10000);


  ResultLinePtr  resultLine = GetNextRecord (in, imageClasses, newFormat, *results);
  while  (resultLine)
  {
    results->PushOnBack (resultLine);
    resultLine = GetNextRecord (in, imageClasses, newFormat, *results);
  }

  fclose (in);

  return  results;
}  /* LoadBinaryClassResultLines */





void  ProcessResultsForTwoClasses (ostream&           report, 
                                   ostream&           binConfigBest,
                                   ofstream**         binConfigSame,
                                   ImageClassPtr      class1, 
                                   ImageClassPtr      class2, 
                                   ResultLineListPtr  results,
                                   TotalLineList&     totals,
                                   ostream&           sumResults
                                  )
{
  ImageClassList  ourClasses;
  ourClasses.PushOnBack (class1);
  ourClasses.PushOnBack (class2);

  beamSearchResults  = false;
  
  TotalLinePtr  totalLine = ProcessResults (&ourClasses,
                                            *results, 
                                            report,
                                            true,    // Sort by Grade,  NOT accuracy,
                                            testData,
                                            validationData
                                           );

  totals.PushOnBack (totalLine);

  report << endl
         << endl
         << "Classes [" << ourClasses << "]" << endl;

  totalLine->PrintHighestAccuraciesByNumOfFeatures (report, upOneCount, sumResults);

/*
  totalLine->PrintHighestGradesByNumOfFeatures (report, 
                                                class1, 
                                                class2, 
                                                punishmentFactor,
                                                upOneCount,
                                                sumResults
                                               );
*/

  report << endl;
  //totalLine->CrossValidationsForTop200 (report);
  report << endl;

  binConfigBest << endl
                << "[TWO_CLASS_PARAMETERS]"                         << endl
                << "CLASS1"             << "="  <<  class1->Name () << endl
                << "CLASS2"             << "="  <<  class2->Name () << endl
                << "FEATURES_INCLUDED"  << "="  <<  totalLine->BestFeatureSetFound ().ToString () << endl
                << endl;


  int  bestFeatureCount = totalLine->BestFeatureSetFoundFeatureCount ();
  int  x;
  for  (x = 0;  x <= upOneCount;  x++)
  {
    FeatureNumList f = totalLine->FeatureForBestAccurcayFoundFeatureCount (x + bestFeatureCount);

    *(binConfigSame[x]) << endl
                        << "[TWO_CLASS_PARAMETERS]"                         << endl
                        << "CLASS1"             << "="  <<  class1->Name () << endl
                        << "CLASS2"             << "="  <<  class2->Name () << endl
                        << "FEATURES_INCLUDED"  << "="  <<  f.ToString ()   << endl
                        << endl;
  }

}  /* ProcessResultsForTwoClasses */



void  CreateBaseClassNames (ostream&           config,
                            ImageClassListPtr  imageClasses
                           )
{
  ImageClassList::iterator  idx;
  for  (idx = imageClasses->begin ();  idx != imageClasses->end ();  idx++)
  {
    ImageClassPtr  ic = *idx;

    config << "[Training_Class]" << endl
           << "dir=c:\\TrainingApp\\TraningImages\\" << ic->Name () << endl
           << "class_name=" << ic->Name () << endl
           << endl;
  }
}  /* CreateBaseClassNames */



void  ProcessBinaryFeatureSelectionSummaryResults (KKStr   rootDir)
{
  KKStr  tunnedConfigFileName = osAddSlash    (rootDir)        +
                                 osGetRootName (configFileName) +
                                 "_Tunned.cfg";

  KKStr  summaryResultsFileName = osAddSlash (rootDir)         +
                                   osGetRootName (testFileName) +
                                   "_SummaryResults.txt";

  bool  sucessfull = true;

  SummaryLineListPtr summary 
       = new SummaryLineList (runLog,
                              summaryResultsFileName,
                              testData->FileDesc (),
                              imageClasses,
                              sucessfull
                              );
  if  (!sucessfull)
  {
    runLog.Level (-1) << endl
                      << endl

                      << "ProcessBinaryFeatureSelectionSummaryResults    *** ERROR ***" << endl
                      << endl
                      << "                    Opening File[" << summaryResultsFileName << "]" << endl
                      << endl;
    osWaitForEnter ();
    exit (-1);
  }


  TrainingConfigurationPtr  newConfig = new TrainingConfiguration (*config);
  ImageClassListPtr  classes = summary->GetListOfClasses ();

  classes->SortByName ();

  int  numOfClasses = classes->QueueSize ();

  for  (int  clIDX = 0;  clIDX < (numOfClasses - 1);  clIDX++)
  {
    ImageClassPtr  class1 = classes->IdxToPtr (clIDX);

    for  (int  c2IDX = clIDX + 1;  c2IDX < numOfClasses;  c2IDX++)
    {
      ImageClassPtr  class2 = classes->IdxToPtr (c2IDX);
      
      SummaryLineListPtr  twoClassList = summary->GetSummaryLinesForTheseClasses (class1, class2);

      //twoClassList->SortHighAccuracyLowSupportPoints ();
      twoClassList->SortLowSuportPoints ();

      SummaryLineList::iterator  idx   = twoClassList->begin ();
      SummaryLinePtr             bestAccuracy = *idx;
      SummaryLinePtr             bestSet      = *idx;
      int                        idxOfBest    = 0;


      for  (idx = twoClassList->begin ();  idx != twoClassList->end ();  idx++)
      {
        SummaryLinePtr s = *idx;

        if  (bestSet->FeatureCount () < 4)
        {
          if  (s->FeatureCount () > bestSet->FeatureCount ())
            bestSet = s;
        }



        float tTest = PairedTTest (bestAccuracy->FoldAccuracies (), s->FoldAccuracies ());

        if  (tTest < 1.0f)
        {
          if  (s->SupportPoints () < bestSet->SupportPoints ())
          {
            // We have a better set to work with, since it has less support points.
            bestSet = s;
          }
        }

        cout << s->TestAccuracy ()  << "\t" 
             << tTest               << "\t"
             << s->SupportPoints () << "\t"
             << s->FeatureCount ();

        if  (s == bestSet)
        {
          cout << "\t" << "BestSoFar";
        }

        cout << endl;
      }

      newConfig->SetFeatureNums (class1, class2, bestSet->Features (), (bestSet->TestAccuracy () / 100.0f));
 
      delete  twoClassList;
    }
  }
  

  cout << endl
       << endl
       << endl
       << "Saving Config File[" << tunnedConfigFileName << "]" << endl
       << endl;

  osWaitForEnter ();

  newConfig->Save (tunnedConfigFileName);


  delete  summary;  summary = NULL;
  delete  classes;  classes = NULL;
}  /* ProcessBinaryFeatureSelectionSummaryResults */






void  ProcessBinaryFeatureSelectionResults (KKStr   rootDir,
                                            bool     newFormat
                                           )
{
  KKStr  binaryModelNameBest;
  KKStr  binaryModelNameSame;
  KKStr  reportFileName;

  int  totalNodesSearhed = 0;

  KKStr  summaryResultsFileName = osAddSlash (rootDir)         +
                                   osGetRootName (testFileName) +
                                   "_SummaryResults.txt";

  ofstream  sumResults (summaryResultsFileName.Str ());

  ofstream** binConfigSame = new ofstream*[upOneCount + 1];
  int  x;
  for  (x = 0;  x <= upOneCount;  x++)
  {
    binaryModelNameSame = osAddSlash (rootDir) + osGetDirNameFromPath (rootDir) +"_BinaryModelSame-" + StrFormatInt (x, "z00") + ".cfg";
    binConfigSame[x] = new ofstream (binaryModelNameSame.Str ());
  }

  {
    reportFileName = osAddSlash (rootDir) + 
                     osGetDirNameFromPath (rootDir) +
                     ".parsed";

    binaryModelNameBest = osAddSlash (rootDir) + 
                          osGetDirNameFromPath (rootDir) +
                          "_BinaryModelBest-" + StrFormatInt (upOneCount, "zz0") +
                          ".cfg";

  }


  ofstream  report (reportFileName.Str ());
  ofstream  binConfigBest (binaryModelNameBest.Str ());

  CreateBaseClassNames (binConfigBest, imageClasses);

  PrintRunTimeParameters (report);

  int  idx1, idx2;
  int  numOfClasses = imageClasses->QueueSize ();

  TotalLineList  totals (true);

  imageClasses->SortByName ();

  ImageClassPtr  class1 = NULL;
  ImageClassPtr  class2 = NULL;
  for  (idx1 = 0;  idx1 < (numOfClasses - 1);  idx1++)
  {
    class1 = imageClasses->IdxToPtr (idx1);

    for  (idx2 = idx1 + 1;  idx2 < numOfClasses;  idx2++)
    {
      class2 = imageClasses->IdxToPtr (idx2);
      ResultLineListPtr  results = LoadBinaryClassResultLines (rootDir, class1, class2, newFormat);
      if  (results)
      {
        totalNodesSearhed += results->QueueSize ();
        ProcessResultsForTwoClasses (report, binConfigBest, binConfigSame, class1, class2, results, totals, sumResults);
        delete  results;
      }
    }
  }


  imageClasses->SortByName ();
  ImageClassListIterator  cIdx1 (*imageClasses);

  class1 = NULL;
  for  (cIdx1.Reset ();  class1 = cIdx1.CurPtr ();  ++cIdx1)
  {
    report << endl
           << endl
           << class1->Name ()  << endl;
            
    ImageClassListIterator  cIdx2 (*imageClasses);
    ImageClassPtr  class2 = NULL;

    // Create Header Line 
    report << "FeatureCount";
    for  (cIdx2.Reset ();  class2 = cIdx2.CurPtr ();  ++cIdx2)
    {
      if  (class1 != class2)
        report << "\t" << class2->Name ();
    }
    report << endl;


    int  featureCount;
    for  (featureCount = 1;  featureCount <= maxNumOfFeatures;  featureCount++)
    {
      report << featureCount;

      for  (cIdx2.Reset ();  class2 = cIdx2.CurPtr ();  ++cIdx2)
      {
        if  (class1 == class2)
          continue;

        report << "\t" << Accuracy (totals, class1, class2, featureCount) << "%";
      }

      report << endl;
    }
  }

  binConfigBest.close ();

  for  (x = 0;  x <= upOneCount;  x++)
  {
    binConfigSame[x]->close ();
    delete  binConfigSame[x];
  }
  delete  binConfigSame;

  report << endl
         << endl
         << endl
         << "Total Nodes Searched[" << totalNodesSearhed << "]" << endl
         << endl;

  report.close ();
}  /* ProcessBinaryFeatureSelectionResults */



VectorKKStr  MultipleWrappersDetermineResultFiles ()
{
  KKStr  rootDir = osGetPathPartOfFile (resultFileName);
  KKStr  subDirSpec;

  if  (rootDir.Empty ())
    subDirSpec = "*.*";
  else
    subDirSpec = osAddSlash (rootDir) + "*.*";

  StringListPtr  dirs = osGetListOfDirectories (subDirSpec);

  if  (dirs == NULL)
  {
    runLog.Level (-1) << endl << endl
                      << "ProcessMutipleWrapperResults    *** No Sub Directories Found ***"
                      << endl
                      << "Dir Spec [" << subDirSpec << "]" << endl
                      << endl;
    osWaitForEnter ();
    exit (-1);
  }

  KKStrList::const_iterator dirIDX;

  for  (dirIDX = dirs->begin ();  dirIDX != dirs->end ();  dirIDX++)
  {
    const KKStr&  subDirName = *(*dirIDX);

    KKStr  dirName;
    if  (rootDir.Empty ())
      dirName = subDirName;
    else
      dirName = osAddSlash (rootDir) + subDirName;

  }

  return VectorKKStr ();
}  /* MultipleWrappersDetermineResultFiles */





TotalLinePtr  LoadResultFile (const KKStr&         resultFileName,
                              ostream&              sumResults,
                              ImageFeaturesListPtr  testData,
                              ImageFeaturesListPtr  validationData
                             )
{
  FILE*  in = osFOPEN (resultFileName.Str (), "r");
  if  (!in)
  {
    cerr << endl << "**** ERROR **** File[" << resultFileName << "]  not found." << endl;
    osWaitForEnter ();
    exit (1);
  }

  KKStr  parsedFileName = osRemoveExtension (resultFileName) + ".parsed"; 
  ofstream outFile (parsedFileName.Str ());
  if  (!outFile.is_open ())
  {
    cerr << endl 
         << "Error Opening Output File["  << parsedFileName << "]." 
         << endl;

    osWaitForEnter ();
    exit (1);
  }

  PrintRunTimeParameters (outFile);
 
  ResultLineList  results (10000);

  ResultLinePtr  resultLine = GetNextRecord (in, imageClasses, false, results);

  int  lastJobId = resultLine->JobId ();

  ImageClassListPtr  curClasses = new ImageClassList (*(resultLine->Classes ()));

  cout << *curClasses << endl;

  int  count = 0;

  while  ((resultLine)  &&  (*(resultLine->Classes ()) == *curClasses))
  {
    results.PushOnBack (resultLine);
    lastJobId = resultLine->JobId ();
    count++;

    if  ((count % 500) == 0)
      cout << count << endl;
    resultLine = GetNextRecord (in, imageClasses, false, results);
  }

  cout << count << endl;

  TotalLinePtr  totalLine = ProcessResults (curClasses, 
                                            results, 
                                            outFile, 
                                            false,
                                            testData,
                                            validationData
                                           );
  outFile << endl
          << endl
          << "Classes [" << *curClasses << "]" << endl;

  totalLine->PrintHighestAccuraciesByNumOfFeatures (outFile, upOneCount, sumResults);
  outFile << endl;
  //totalLine->CrossValidationsForTop200 (outFile);
  outFile << endl;

  delete  curClasses;


  imageClasses->SortByName ();
  ImageClassListIterator  idx1 (*imageClasses);
  ImageClassPtr  class1 = NULL;

  for  (idx1.Reset ();  class1 = idx1.CurPtr ();  ++idx1)
  {
    outFile << endl
            << endl
            << class1->Name ()  
            << endl;
            
    ImageClassListIterator  idx2 (*imageClasses);
    ImageClassPtr  class2 = NULL;

    // Create Header Line 
    outFile << "FeatureCount";
    for  (idx2.Reset ();  class2 = idx2.CurPtr ();  ++idx2)
      if  (class1 != class2)
        outFile << "\t" << class2->Name ();
    outFile << endl;
  }

  outFile.close ();

  return  totalLine;
}  /* LoadResultFile */













void  ProcessMutipleWrapperResultsIntersection (const FeatureNumList&         featureNums,
                                                vector<ImageFeaturesListPtr>  testDataList,
                                                vector<ImageFeaturesListPtr>  validationDataList,
                                                VectorFloat&                  accuracies
                                               )
{
  config->SetFeatureNums (featureNums);

  accuracies.erase (accuracies.begin (), accuracies.end ());

  uint  idx;

  bool  cancelFlag = false;

  for  (idx = 0;  idx < testDataList.size ();  idx++)
  {
    ImageFeaturesListPtr  testData       = testDataList[idx];
    ImageFeaturesListPtr  validationData = validationDataList[idx];

    CrossValidationPtr cv = NULL;
    if  (validationData)
    {
      cv = new CrossValidation (config, 
                                testData, 
                                imageClasses, 
                                1, 
                                false, 
                                fileDesc, 
                                runLog,
                                cancelFlag
                               );

      cv->RunValidationOnly (validationData, NULL);
    }
    else   
    {
      cv = new CrossValidation (config, 
                                testData, 
                                imageClasses, 
                                NumOfValidationFolds, 
                                false, 
                                fileDesc, 
                                runLog,
                                cancelFlag
                               );

      cv->RunCrossValidation ();
    }

    accuracies.push_back (cv->Accuracy ());
    delete  cv;
    cv = NULL;
  }
}  /* ProcessMutipleWrapperResultsIntersection */





void  ProcessMutipleWrapperLoadDataFiles (vector<ImageFeaturesListPtr>&  testDataList,
                                          vector<ImageFeaturesListPtr>&  validationDataList
                                         )
{
  KKStr  dirName = "K:\\Plankton\\Papers\\FastFeatureSelAlgorithm";
  int  seqNum = 0;
  for  (seqNum = 1;  seqNum <= 5;  seqNum++)
  {

    KKStr  test1FileName  = osAddSlash (dirName) + 
                             "FiveClassPaper_FeatureSelection" +
                             StrFormatInt (seqNum, "0") +
                             ".data";

    KKStr  test2FileName  = osAddSlash (dirName) + 
                             "FiveClassPaper_TrainingSubset" +
                             StrFormatInt (seqNum, "0") +
                             ".data";

    KKStr  validationName = osAddSlash (dirName) + 
                             "FiveClassPaper_TestSet" +
                             StrFormatInt (seqNum, "0") +
                             ".data";

    bool  successful;
    bool  changesMade = false;

    ImageFeaturesListPtr  test1Data = LoadFeatureFile (test1FileName, 
                                                       fileFormat,
                                                       *imageClasses,
                                                       -1,
                                                       runLog,
                                                       cancelFlag,
                                                       successful,
                                                       changesMade
                                                      );

    ImageFeaturesListPtr  test2Data = LoadFeatureFile (test2FileName, 
                                                       fileFormat,
                                                       *imageClasses,
                                                       -1,
                                                       runLog,
                                                       cancelFlag,
                                                       successful,
                                                       changesMade
                                                      );


    test1Data->AddQueue (*test2Data);
    test2Data->Owner (false);
    delete  test2Data;
    test2Data = NULL;

    ImageFeaturesListPtr  validationData = LoadFeatureFile (validationName, 
                                                            fileFormat,
                                                            *imageClasses,
                                                            -1,
                                                            runLog,
                                                            cancelFlag,
                                                            successful,
                                                            changesMade
                                                           );

    testDataList.push_back (test1Data);
    validationDataList.push_back (validationData);
  }
}  /* ProcessMutipleWrapperLoadDataFiles */




void  ProcessMutipleWrapperResults (const VectorKKStr&  resultFiles)
{
  vector<ImageFeaturesListPtr>  testDataList;
  vector<ImageFeaturesListPtr>  validationDataList;

  ProcessMutipleWrapperLoadDataFiles (testDataList, validationDataList);

  ofstream  sumResults ("MutipleWrapperResults.txt");

  VectorKKStr::const_iterator rfIDX;

  TotalLineList  totals (true);

  int  resultFileNum = 0;
  for  (rfIDX = resultFiles.begin ();  rfIDX != resultFiles.end ();  rfIDX++)
  {
    const KKStr&  resultFileName = *rfIDX;

    sumResults << "Results File [" << resultFileName << "]" << endl << endl;

    TotalLinePtr  totalLine = LoadResultFile (resultFileName, 
                                              sumResults,
                                              testDataList[resultFileNum],
                                              validationDataList[resultFileNum]
                                             );

    sumResults << endl << endl << endl << endl;

    totals.PushOnBack (totalLine);
    resultFileNum++;
  }


  sumResults << endl;
             

  uint  featureCount = 0;

  sumResults << "Feature";
  for  (featureCount = 1;  featureCount <= testDataList.size ();  featureCount++)
    sumResults << "\t" << "Search";
 
  sumResults << "\t" 
             << "\t"
             << "\t" << "Num"
             << "\t" << "Inter-"
             << "\t"
             << endl;


  sumResults << "Count";
  for  (featureCount = 1;  featureCount <= testDataList.size ();  featureCount++)
    sumResults << "\t" << featureCount;

  sumResults << "\t" << "Mean" 
             << "\t" << "StdDev"
             << "\t" << "Features"
             << "\t" << "section"
             << "\t" << "Accuracy"
             << "\t" << "StdDev"
             << endl;


  for  (featureCount = 1;  featureCount <= uint (fileDesc->NumOfFields ());  featureCount++)
  {
    sumResults << featureCount;

    FeatureNumList  itersectionOfFeatures (fileDesc);
    int  tlIDX;

    VectorFloat testAccuracies;

    for  (tlIDX = 0;  tlIDX < totals.QueueSize ();  tlIDX++)
    {
      const TotalLine&  tl = totals[tlIDX];
      ResultLinePtr  r = tl.JobWithHighestAccuracy (featureCount);
      if  (!r)
      {
        sumResults << "\t";
        continue;
      }

      sumResults << "\t" << r->TestAccuracy ();
      testAccuracies.push_back (r->TestAccuracy ());

      if  (tlIDX == 0)
        itersectionOfFeatures = r->GetFeatureNums ();
      else
        itersectionOfFeatures = itersectionOfFeatures * (r->GetFeatureNums ());
    }

    float  mean, stdDev;

    CalcMeanAndStdDev (testAccuracies, mean, stdDev);

    sumResults << "\t" << mean << "\t" << stdDev;

    sumResults << "\t"  << itersectionOfFeatures.NumOfFeatures ()
               << "\t"  << itersectionOfFeatures.ToString ();

    if  (itersectionOfFeatures.NumOfFeatures () > 0)
    {
      VectorFloat  accuracies;
      ProcessMutipleWrapperResultsIntersection (itersectionOfFeatures,
                                                testDataList,
                                                validationDataList,
                                                accuracies
                                               );

      CalcMeanAndStdDev (accuracies, mean, stdDev);

      sumResults << "\t" << mean
                 << "\t" << stdDev;
    }
    
    sumResults << endl;
  }
}  /* ProcessMutipleWrapperResults */




ResultLineListPtr  LoadFromDoneStatusFile (const KKStr&  fileName)
{
  SingleRunList  doneJobs (fileName);
  doneJobs.LoadStatusFile (C45, fileDesc, runLog);

  int  totalRecs = doneJobs.QueueSize ();

  runLog.Level (10) << "LoadFromDoneStatusFile  doneJobs loaded,  [" << totalRecs << "]  Recs loaded." << endl;

  ResultLineListPtr  results = new ResultLineList (doneJobs.QueueSize ());

  int  c = 0;

  SingleRunList::const_iterator  idx;
  for  (idx = doneJobs.begin ();  idx !=  doneJobs.end ();  idx++)
  {
    SingleRunPtr  r = *idx;
    results->PushOnBack (new ResultLine (*r, *imageClasses, fileDesc, *results));
    c++;

    if  ((c % 10000) == 0)
    {
      cout << c << " out of " << totalRecs << endl;
    }
  }

  runLog.Level (10) << "LoadFromDoneStatusFile  returning." << endl;

  return  results;
}  /* LoadFromDoneStatusFile */





void  ProcessDoneStatusFile (const KKStr&  parsedFileName,
                             const KKStr&  summaryResultsFileName
                            )
{
  runLog.Level (10) << "ProcessDoneStatusFile" << endl;

  KKStr  doneStatusFileName = "BeamSearchSVM_Done.status";

  ResultLineListPtr  results = LoadFromDoneStatusFile (doneStatusFileName);



  ofstream  outFile (parsedFileName.Str ());
  if  (!outFile.is_open ())
  {
    cerr << endl 
         << "Error Opening Output File["  << parsedFileName << "]." 
         << endl;
    osWaitForEnter ();
    exit (1);
  }


  ofstream  sumResults (summaryResultsFileName.Str ());
  if  (!sumResults.is_open ())
  {
    cerr << endl 
         << "Error Opening Output File["  << sumResults << "]." 
         << endl;
    osWaitForEnter ();
    exit (1);
  }



  PrintRunTimeParameters (outFile);

  if  (fileDesc->NumOfFields () < 100)
  {
    outFile << endl << endl << endl 
            << "Test Data Statistics" << endl
            << "====================" << endl;
    testData->PrintFeatureStatisticsByClass (outFile);

    outFile << endl << endl << endl 
            << "Validation Data Statistics" << endl
            << "==========================" << endl;
    validationData->PrintFeatureStatisticsByClass (outFile);
  }


  TotalLineList  totals (true);

  TotalLinePtr  totalLine = ProcessResults (imageClasses, 
                                            *results, 
                                            outFile, 
                                            false,
                                            testData,
                                            validationData
                                           );

  totals.PushOnBack (totalLine);

  totalLine->PrintHighestAccuraciesByNumOfFeatures (outFile, upOneCount, sumResults);
  outFile << endl << endl << endl;

  totalLine->CrossValidationsForTop200 (outFile);
  outFile << endl << endl << endl;

  outFile << endl  << endl  << endl;
  outFile << "Summary of Highest Accuracies"  << endl;
  outFile << endl;
  PrintOutSummaryOfHighestAccuracies (outFile, totals);

  outFile << endl  << endl  << endl;
  outFile << "Accuracies by Number of Features"  << endl;
  outFile << endl;
  PrintOutHighestAccuracyByNumOfFeatures (outFile, totals);

  outFile << endl  << endl  << endl;
  outFile << "Feature Ussage for Top 100"  << endl;
  outFile << endl;
  PrintOutFeatureUssageForTop100 (outFile, totals);

  outFile << endl  << endl  << endl;
  outFile << "Feature Ussage for Top 200"  << endl;
  outFile << endl;
  PrintOutFeatureUssageForTop200 (outFile, totals);

  outFile << endl  << endl  << endl;
  outFile << "Feature Impact"  << endl;
  outFile << endl;
  PrintOutFeatureImpact (outFile, totals);

  sumResults.close ();
  outFile.close ();

  delete  results;  results = NULL;

}  /* ProcessDoneStatusFile */




void  ProcessResultFile (const KKStr&  parsedFileName,
                         const KKStr&  summaryResultsFileName,
                         const KKStr&  binaryModelName
                        )
{
  //FILE*  in = fopen ("K:\\v1\\Plankton\\TempBeamSearch\\Jan_18_2004\\GrayScaleCpp_2003-May-13.data.result", "r");
  FILE*  in = osFOPEN (resultFileName.Str (), "r");
  if  (!in)
  {
    cerr << endl << "**** ERROR **** File[" << resultFileName << "]  not found." << endl;
    osWaitForEnter ();
    exit (1);
  }

  ofstream  binConfig (binaryModelName.Str ());

  ofstream  outFile (parsedFileName.Str ());
  if  (!outFile.is_open ())
  {
    cerr << endl 
         << "Error Opening Output File["  << parsedFileName << "]." 
         << endl;
    osWaitForEnter ();
    exit (1);
  }


  ofstream  sumResults (summaryResultsFileName.Str ());
  if  (!sumResults.is_open ())
  {
    cerr << endl 
         << "Error Opening Output File["  << summaryResultsFileName << "]." 
         << endl;
    osWaitForEnter ();
    exit (1);
  }



  PrintRunTimeParameters (outFile);
 

  {
    outFile << endl << endl << endl 
            << "Test Data Statistics" << endl
            << "====================" << endl;
    testData->PrintFeatureStatisticsByClass (outFile);

    outFile << endl << endl << endl 
            << "Validation Data Statistics" << endl
            << "==========================" << endl;
    validationData->PrintFeatureStatisticsByClass (outFile);
  }


  TotalLineList  totals (true);

  ResultLineList  emptyResultsList (1);

  ResultLinePtr  resultLine = GetNextRecord (in, imageClasses, false, emptyResultsList);

  int  lastJobId = resultLine->JobId ();

  while  (resultLine)
  {
    ResultLineList  results (10000);

    ImageClassPtr  curClass1 = resultLine->Class1 ();
    ImageClassListPtr  curClasses = new ImageClassList (*(resultLine->Classes ()));

    cout << *curClasses << endl;

    int  count = 0;

    while  ((resultLine)  &&
            (*(resultLine->Classes ()) == *curClasses)
           )
    {
      results.PushOnBack (resultLine);
      lastJobId = resultLine->JobId ();
      count++;

      if  ((count % 500) == 0)
        cout << count << endl;
      resultLine = GetNextRecord (in, imageClasses, false, results);
    }

    cout << count << endl;

    TotalLinePtr  totalLine = ProcessResults (curClasses, 
                                              results, 
                                              outFile, 
                                              false,
                                              testData,
                                              validationData
                                             );
    totals.PushOnBack (totalLine);

    outFile << endl
            << endl
            << "Classes [" << *curClasses << "]" << endl;

    totalLine->PrintHighestAccuraciesByNumOfFeatures (outFile, upOneCount, sumResults);
    outFile << endl;
    //totalLine->CrossValidationsForTop200 (outFile);
    outFile << endl;

    if  (curClasses->QueueSize () == 2)
    {
      binConfig << endl
                << "[TWO_CLASS_PARAMETERS]" << endl;
      binConfig << "CLASS1"             << "\t"  <<  curClasses->IdxToPtr (0)->Name () << endl;
      binConfig << "CLASS2"             << "\t"  <<  curClasses->IdxToPtr (1)->Name () << endl;
      binConfig << "FEATURES_INCLUDED"  << "\t"  <<  totalLine->BestFeatureSetFound ().ToString () << endl;
      binConfig << endl;
    }

    delete  curClasses;
  }

  imageClasses->SortByName ();
  ImageClassListIterator  idx1 (*imageClasses);
  ImageClassPtr  class1 = NULL;

  for  (idx1.Reset ();  class1 = idx1.CurPtr ();  ++idx1)
  {
    outFile << endl
            << endl
            << class1->Name ()  
            << endl;
            
    ImageClassListIterator  idx2 (*imageClasses);
    ImageClassPtr  class2 = NULL;

    // Create Header Line 
    outFile << "FeatureCount";
    for  (idx2.Reset ();  class2 = idx2.CurPtr ();  ++idx2)
      if  (class1 != class2)
        outFile << "\t" << class2->Name ();
    outFile << endl;


    int  featureCount;
    for  (featureCount = 1;  featureCount <= maxNumOfFeatures;  featureCount++)
    {
      outFile << featureCount;

      for  (idx2.Reset ();  class2 = idx2.CurPtr ();  ++idx2)
      {
        if  (class1 == class2)
          continue;

        outFile << "\t" << Accuracy (totals, class1, class2, featureCount) << "%";
      }

      outFile << endl;
    }
  }


  outFile << endl  << endl  << endl;
  outFile << "Summary of Highest Accuracies"  << endl;
  outFile << endl;
  PrintOutSummaryOfHighestAccuracies (outFile, totals);

  outFile << endl  << endl  << endl;
  outFile << "Accuracies by Number of Features"  << endl;
  outFile << endl;
  PrintOutHighestAccuracyByNumOfFeatures (outFile, totals);

  outFile << endl  << endl  << endl;
  outFile << "Feature Ussage for Top 100"  << endl;
  outFile << endl;
  PrintOutFeatureUssageForTop100 (outFile, totals);

  outFile << endl  << endl  << endl;
  outFile << "Feature Ussage for Top 200"  << endl;
  outFile << endl;
  PrintOutFeatureUssageForTop200 (outFile, totals);

  outFile << endl  << endl  << endl;
  outFile << "Feature Impact"  << endl;
  outFile << endl;
  PrintOutFeatureImpact (outFile, totals);


  delete  imageClasses;    imageClasses   = NULL;
  delete  testData;        testData       = NULL;
  delete  validationData;  validationData = NULL;

  fclose (in);
  binConfig.close ();
  sumResults.close ();
  outFile.close ();
}  /* ProcessResultFile */










int  main (int     argc, 
           char**  argv
          )

{
  imageClasses = new ImageClassList ();

  ProcessCmdLine (argc, argv);

  if  (!fileDesc)
  {
    runLog.Level (-1) << endl
                      << endl
                      << "main   *** ERROR ***,   No FileDesc provided." << endl
                      << endl;
    osWaitForEnter ();
    exit (-1);
  }

  maxNumOfFeatures = fileDesc->NumOfFields ();

  if  (processMultipleWrappers)
  {
    VectorKKStr  resultFiles;
    resultFiles.push_back  ("K:\\Plankton\\Papers\\FastFeatureSelAlgorithm\\Wrapper1\\FiveClassPaper_FeatureSelection1.result");
    resultFiles.push_back  ("K:\\Plankton\\Papers\\FastFeatureSelAlgorithm\\Wrapper2\\FiveClassPaper_FeatureSelection2.result");
    resultFiles.push_back  ("K:\\Plankton\\Papers\\FastFeatureSelAlgorithm\\Wrapper3\\FiveClassPaper_FeatureSelection3.result");
    resultFiles.push_back  ("K:\\Plankton\\Papers\\FastFeatureSelAlgorithm\\Wrapper4\\FiveClassPaper_FeatureSelection4.result");
    resultFiles.push_back  ("K:\\Plankton\\Papers\\FastFeatureSelAlgorithm\\Wrapper5\\FiveClassPaper_FeatureSelection5.result");

    ProcessMutipleWrapperResults (resultFiles);
    exit (0);
  }



//  {
//    //const char*  srcFileName = "K:\\v1\\Plankton\\TempBeamSearch\\2004_March_02\\2004_03_02_TestData.data.result";
//    //const char*  srcFileName = "K:\\v1\\Plankton\\TempBeamSearch\\Jan_18_2004\\BeamSearch_TestData.data.result";
//    //const char*  srcFileName = "K:\\Plankton\\StatusReports\\2004-03-16\\Binary_Marine_Tricho.result";
//    const char*  rootDir = "K:\\v1\\Plankton\\FeatureSelections\\2005-12-09";
//    ProcessBinaryFeatureSelectionResults (rootDir, true);
//    exit (0);
//  }

//  {
//    //const char*  rootDir = "C:\\users\\kkramer\\GradSchool\\Plankton\\FeatureSelections\\2005-12-09";
//    const char*  rootDir = "K:\\v1\\Plankton\\FeatureSelections\\2005-12-09";
//    ProcessBinaryFeatureSelectionSummaryResults (rootDir);
//    exit (0);
//  }


  KKStr  parsedFileName = osRemoveExtension (resultFileName) + ".parsed";
  KKStr  baseDirectory = osGetPathPartOfFile (resultFileName);
  if  (baseDirectory.Empty ())
    baseDirectory = osGetCurrentDirectory ();

  KKStr  binaryModelName = osAddSlash (baseDirectory) + "BinaryModel.txt";

  KKStr  summaryResultsFileName = osAddSlash (baseDirectory)   +
                                   osGetRootName (testFileName) + 
                                   "_SummaryResults.txt";


  if  (processDoneStatusFile)
  {
    ProcessDoneStatusFile (parsedFileName, summaryResultsFileName);
  }
  else
  {
    ProcessResultFile (parsedFileName, summaryResultsFileName, binaryModelName);
  }


  delete  imageClasses;    imageClasses   = NULL;
  delete  testData;        testData       = NULL;
  delete  validationData;  validationData = NULL;

  return 0;
}  /* main */
