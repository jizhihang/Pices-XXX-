#include  "FirstIncludes.h"


#include  <string>
#include  <ctype.h>
#include  <stdio.h> 
#include  <time.h>

#include  <cmath>

#ifndef  WIN32
#include  <unistd.h>
#endif

#include  <iomanip>
#include  <iostream>
#include  <fstream>
//#include  <ostream>
#include  <iostream>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;


#ifdef WIN32
#include  <windows.h>
#else
#endif


#include  "BitReductionPaper.h"

#include  "BinaryClassParms.h"
#include  "BitReductionJobList.h"
#include  "Classifier.h"
#include  "CrossValidation.h"
#include  "CrossValidationVoting.h"
#include  "CrossValidationThreeWay.h"
#include  "CrossValidationMxN.h"
#include  "FeatureFileIO.h"

#include  "FeatureFileIOC45.h"
#include  "FeatureFileIOPices.h"
#include  "FeatureFileIOSparse.h"
#include  "FeatureFileIOUCI.h"



#include  "FeatureNumList.h"
#include  "Orderings.h"
#include  "OSservices.h"
#include  "NormalizationParms.h"
#include  "RandomSampleJob.h"
#include  "StatisticalFunctions.h"
#include  "TrainingProcess.h"



// 2006-06-06
// C:\bigpine\Plankton\Papers\BitReduction\DataSets\ForestCover
// -C covtype.cfg -DF CovType_TwoClass.data -Validation CovType_TwoClass.test -format C45  -Report Forest_UnBal_3rdTry_UserTimeOnly.txt -DataSet Forest


// 2006-06-23
// -RootDir C:\bigpine\Plankton\Papers\BitReduction\DataSets  -RANDOMSAMPLEALL
// -RootDir  C:\bigpine\Plankton\Papers\BitReduction\DataSets -DataSet Phoneme

// -dataSet Plankton  -RandomSplits -RootDir  C:\users\kkramer\GradSchool\Plankton\Papers\BitReduction\DataSets
// -dataSet Page  -RandomSplits -RootDir  F:\Users\kkramer\GradSchool\Plankton\Papers\BitReduction\DataSets

// -dataset  Forest  -SmallSample 0.01 -RootDir  C:\bigpine\Plankton\Papers\BitReduction\DataSets

// -RootDir  C:\bigpine\Plankton\Papers\BitReduction\DataSets  -Dataset Banana2  -ALT 0.25


DataSetParametersPtr  dataSetParms = NULL;


DataSetParameters   dataSetParamaters[] =
{
  {DataSetAdult,     "Adult",
                     "Adult",
                     "adult.cfg",
                     "adult.data",       // Training data File
                     "adult.test",       // Test data file
                     FeatureFileIOC45::Driver (),
                     {KT_Linear, KT_NULL},            
                     {BinaryEncoding, ScaledEncoding, Encoding_NULL},
                     true,  
                     {1, -1}, 
                     {0,1,2,3,4,5,6,7,8,9,10,11,-1},
                     1,                 // rsBrBits
                     12                 // rsUBR
  },

  {DataSetBanana,    "banana",
                     "Banana",
                     "banana.cfg",
                     "banana.all_80p.data",
                     "banana.all_20p.data",
                     FeatureFileIOSparse::Driver (),
                     {KT_RBF, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},
                     false,
                     {1, -1},
                     {0,1,2,3,4,5,6,7,8,9,10,-1},
                     -1,                 // rsBrBits
                     -1                  // rsUBR
  },

  //{DataSetBanana2,   "Banana2",
  //                   {KT_RBF, KT_Linear, KT_NULL}, 
  //                   {NoEncoding, Encoding_NULL},
  //                   false,
  //                   {1, 3, 6, 9, 12, 15, -1},
  //                   {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,-1}
  //},
  {DataSetBanana2,   "Banana2",
                     "Banana",
                     "banana.cfg",
                     "banana.all_80p.data",
                     "banana.all_20p.data",
                     FeatureFileIOSparse::Driver (),
                     {KT_RBF, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},
                     false,
                     {1, -1},
                     {0,1,2,3,4,5,6,7,8,9,10,-1},
                     7,
                     2
  },

  {DataSetForest,    "Forest",
                     "ForestCover",
                     "covtype.cfg",
                     "CovType_TwoClass.data",
                     "CovType_TwoClass.test",
                     FeatureFileIOC45::Driver (),
                     {KT_Linear, KT_NULL}, 
                     {ScaledEncoding, Encoding_NULL},
                     false,
                     {1, -1},
                     {0,9,10,11,-1},
                     7,                  // rsBrBits
                     2                   // rsUBR
  },

  {DataSetLetter,    "Letter",
                     "Letter",
                     "Letter.cfg",
                     "letter-c45_80p.data",
                     "letter-c45_20p.data",
                     FeatureFileIOC45::Driver (),
                     {KT_RBF, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},   // Continuous data only
                     false,
                     {1, -1},
                     {0,9,10,11,12,-1},
                     9,                 // rsBrBits
                     12                 // rsUBR
  },


  {DataSetPage,      "Page",
                     "Page-Blocks",
                     "page-blocks.cfg",
                     "page-blocks_80p.data",
                     "page-blocks_20p.data",
                     FeatureFileIOC45::Driver (),
                     {KT_RBF, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},
                     false,
                     {1, -1},
                     {0,8,9,10,11,-1},
                     9,                 // rsBrBits
                     8                  // rsUBR
  },


  {DataSetPenDigits, "Pen",
                     "Pendigits",
                     "pendigits.cfg",
                     "pendigits_train_sparse.txt",
                     "pendigits_Test_sparse.txt",
                     FeatureFileIOSparse::Driver (),
                     {KT_RBF, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},   // All attributes are int's [0..100]
                     false,                         // Wonderg if scaled might be better.
                     {1, -1},
                     {0,8,9,10,11,-1},
                     9,                 // rsBrBits
                     2                  // rsUBR

  },


  {DataSetPlankton,  "Plankton",
                     "Plankton",
                     "Plankton.cfg",
                     "PlanktonTrain_17.data",
                     "PlanktonTest_17.data",
                     FeatureFileIOPices::Driver (),
                     {KT_RBF, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},   
                     false,
                     {16, -1},
                     {0,1,2,3,4,5,6,7,8,9,10,11,-1},
                     9,                 // rsBrBits
                     10                 // rsUBR
  },


  //{DataSetPhoneMe,   "PhoneMe",
  //                   {KT_RBF, KT_NULL}, 
  //                   {NoEncoding, Encoding_NULL},   
  //                   false,
  //                   {1, 3, 6, 9, 12, 15, -1},
  //                   {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,-1}
  //},



  {DataSetPhoneMe,   "PhoneMe",
                     "Phoneme",
                     "phoneme.cfg",
                     "phoneme_80p.data",
                     "phoneme_20p.data",
                     FeatureFileIOC45::Driver (),
                     {KT_RBF, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},   
                     false,
                     {1, -1},
                     {0,1,2,3,4,5,6,7,8,9,10,-1},
                     9,
                     0
  },



  {DataSetSatImage,  "SatImage",
                     "SatImage",
                     "Sat.cfg",
                     "sat.trn",
                     "sat.tst",
                     FeatureFileIOUCI::Driver (),
                     {KT_RBF, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},   
                     false,
                     {1, -1},
                     {0,1,2,3,4,5,6,7,8,9,10,11,-1},
                     9, 
                     33
  },

                     
  {DataSetShuttle,   "Shuttle",
                     "Shuttle",
                     "Shuttle.cfg",
                     "Shuttle_Train_c45.data",
                     "Shuttle_Test_c45.data",
                     FeatureFileIOC45::Driver (),
                     {KT_RBF, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},   
                     false,
                     {1, -1},
                     {0,1,2,3,4,5,6,7,8,9,10,11,-1},
                     4,
                     7
  },


  {DataSetWaveForm,  "Wave",
                     "WaveForm",
                     "waveform.cfg",
                     "waveform-+noise_80p.data",
                     "waveform-+noise_20p.data",
                     FeatureFileIOC45::Driver (),
                     {KT_RBF, KT_Linear, KT_NULL}, 
                     {NoEncoding, Encoding_NULL},   
                     false,
                     {1, -1},
                     {0,1,2,3,4,5,6,7,8,9,10,11,-1},
                     11,
                     22
  },


  {DataSetWeb,       "Web",
                     "WEB_Data",
                     "anonymous-msweb.cfg",
                     "anonymous-msweb_Data-C45.data",
                     "anonymous-msweb_Test-C45.data",
                     FeatureFileIOC45::Driver (),
                     {KT_Linear, KT_NULL}, 
                     {ScaledEncoding, Encoding_NULL},
                     false,  
                     {1, -1}, 
                     {0, 1, -1},
                     0,
                     0
  },

  {DataSetNULL,      "",
                     "",
                     "",
                     "",
                     "",
                     FeatureFileIOC45::Driver (),
                     {KT_NULL},
                     {Encoding_NULL}, 
                     false, 
                     {-1},
                     {-1},
                     0,
                     0
  }
};




const DataSetParametersPtr  DataSetParametersFromStr (const KKStr&  name)
{
  int  idx = 0;

  while  (dataSetParamaters[idx].dataSet != DataSetNULL)
  {
    DataSetParameters&  dsp = dataSetParamaters[idx];

    if  (name.CompareIgnoreCase (dsp.name) == 0)
      return  &dsp;

    idx++;
  }

  return  NULL;
}  /* DataSetParametersFromStr */



const DataSetParametersPtr  DataSetParameterFromDataSet (DataSet dataSet)
{
  int  idx = 0;

  while  (dataSetParamaters[idx].dataSet != DataSetNULL)
  {
    DataSetParameters&  dsp = dataSetParamaters[idx];

    if  (dsp.dataSet == dataSet)
      return  &dsp;

    idx++;
  }

  return  NULL;
}  /* DataSetParameterFromDataSet */





DataSet  DataSetFromString (const KKStr&  s)
{
  
  DataSetParametersPtr  dsp = DataSetParametersFromStr (s);
  if  (!dsp)
    return  DataSetNULL;

  return  dsp->dataSet;
}  /* DataSetFromString */




const KKStr&  DataSetParametersToStr (const DataSetParametersPtr  _dataSetParms)
{
  if  (_dataSetParms)
    return _dataSetParms->name;
  else
    return KKStr::EmptyStr ();
}  /* DataSetParametersToStr */

 

BitReductionPaper::BitReductionPaper ()
  :
    accLossTolerance                 (0.5f),
    config                           (NULL),
    featuresAreNormalizedAlready     (false),
    fileDesc                         (NULL),
    runLog                           (),
    images                           (NULL),
    mlClasses                     (NULL),
    inputFormat                      (FeatureFileIOPices::Driver ()),
    learningAlgorithm                (laSupportVectorMachine),
    numOfFolds                       (10),
    performRandomSampleAll           (false),
    //performRandomSampling            (false),
    jobTypes                         (brjNormalJob),
    samplePercentage                 (100.0f),
    reportFile                       (NULL),
    reportFileStream                 (NULL),
    testData                         (NULL)

{
  mlClasses = new MLClassList ();
}  /* BitReductionPaper */




BitReductionPaper::~BitReductionPaper ()
{
  delete  testData;           testData   = NULL;
  delete  images;             images           = NULL;
  delete  mlClasses;       mlClasses     = NULL;
  delete  config;             config           = NULL;
  delete  reportFileStream;   reportFileStream = NULL;
}


void  BitReductionPaper::SetDataSetRootDirName ()
{
  dataSetRootDir = osAddSlash (osAddSlash (rootDirName) + dataSetParms->dataSetSubDir);
}


void  BitReductionPaper::CmdLineHelp ()
{
  cout  << endl;
  cout  <<  "BitReductionPaper -DataSet      <Adul | Forest | PhoneMe | Web, etc >"                         << endl;
  cout  <<  "                  -Log          <Log File Name>"                                               << endl;
  cout  <<  "                  -Report       <Report File Name>"                                            << endl;
  cout  <<  "                  -RootDir      <Root Directory for datasets>  // Used with Random"            << endl;
  cout  <<  "                  -LearningAlgorithm  <SVM, CASCOR>"                                           << endl;
  cout  <<  ""                                                                                              << endl;
  cout  <<  "Other Options:"                                                                                << endl;
  cout  <<  "                  -Tune              Want to test Tuning Procedure."                           << endl;
  cout  <<  "                  -RandomSampling"                                                             << endl;
  cout  <<  "                  -RandomSplite"                                                               << endl;
  cout  <<  "                  -SmallSample"                                                                << endl;
  cout  <<  "                  -AccLossTolerance  <acceptable Accuracy Loss>"                               << endl;
  cout  << endl;
  cout  << endl;
}



void  BitReductionPaper::ProcessCmdLineParameter (KKStr  parmSwitch, 
                                                  KKStr  parmValue 
                                                 )
{

  parmSwitch.Upper ();

  if  ((parmSwitch == "-ACCLOSSTOLERANCE")  ||  (parmSwitch == "-ALT"))
  {
    accLossTolerance = (float)atof (parmValue.Str ());
    if  ((accLossTolerance < 0.0f)  ||  (accLossTolerance >= 100.0f))
    {
      runLog.Level (-1) << endl
                        << endl
                        << "*** ERROR ***   Invalid Accuracy Losss Tolerance Specified.[" << accLossTolerance << "]" << endl
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }
  }

  else if (parmSwitch == "-DATASET")
  {
    dataSetParms = DataSetParametersFromStr (parmValue);
    if  (!dataSetParms)
    {
      runLog.Level (-1) << endl
                        << endl
                        << "*** ERROR ***   Invalid DataSet Parameter[" << parmValue << "]" << endl
                        << endl;
      exit (-1);
    }

    SetDataSetRootDirName ();
  }

  else if  ((parmSwitch == "-L")  ||  (parmSwitch == "-LOG"))
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameter - Must provide File Name with -L option." 
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    runLog.AttachFile (parmValue);
  }

  else if  ((parmSwitch.EqualIgnoreCase ("-LearningAlgorithm"))  ||  (parmSwitch == "-LA"))
  {
    learningAlgorithm = LearningAlgorithmFromStr (parmValue);
    if  (learningAlgorithm == laNULL)
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameter - Invalid Learning Algorithm Specified[" << parmValue << "]" << endl
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }
  }

  else if  (parmSwitch == "-RANDOMSAMPLING")
  {
    jobTypes = brjRandomSampling;
    //performRandomSampling = true;
  }


  else if  (parmSwitch == "-RANDOMSPLITS")
  {
    jobTypes = brjRandomSplits;
  }


  else if  ((parmSwitch == "-TUNE")  ||  (parmSwitch == "-TUNEBITREDUCTION"))
  {
    jobTypes = brjTuningTest;
    samplePercentage = (float)atof (parmValue.Str ());
    if  ((samplePercentage <= 0.0f)  ||  (samplePercentage > 100.0f))
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameter     ***ERROR***" << endl
                        << endl
                        << "                       Invalid Percentage[" << samplePercentage << "]" << endl
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }
  }


  else if  (parmSwitch == "-SMALLSAMPLE")
  {
    jobTypes = brjSmallSample;
    samplePercentage = (float)atof (parmValue.Str ());
    if  ((samplePercentage <= 0.0f)  ||  (samplePercentage > 100.0f))
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameter     ***ERROR***" << endl
                        << endl
                        << "                       Invalid Percentage[" << samplePercentage << "]" << endl
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }
  }


  else if  (parmSwitch == "-RANDOMSAMPLEALL")
  {
    performRandomSampleAll = true;
  }


  else if  ((parmSwitch == "-R")  ||  (parmSwitch == "-REPORT"))
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) << "ProcessCmdLineParameter - Must provide File Name with -R option." << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }
    else
    {
      reportFileName = parmValue;
    }
  }


  else if  (parmSwitch == "-ROOTDIR")
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) << "ProcessCmdLineParameter - Must provide Directory Name with -RootDir option." << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }
    else
    {  
      rootDirName = parmValue;

      if  (!osValidDirectory (rootDirName))
      {
        runLog.Level (-1) << endl << endl
                          << "ProcessCmdLineParameter    *** ERROR ***" << endl
                          << endl
                          << "   RootDir[" << rootDirName << "] is not a valid directory." << endl
                          << endl;
        CmdLineHelp ();
        osWaitForEnter ();
        exit (-1);
      }
    }
  }

  else
  {
    runLog.Level (-1) << endl
                      << "ProcessCmdLineParameter   Invalid Switch["  <<  parmSwitch << "]." 
                      << endl;
    CmdLineHelp ();
    osWaitForEnter ();
    exit (-1);
  }

  return;
}  /* ProcessCmdLineParameter */





void  BitReductionPaper::ReportCmdLineParameters ()
{
  *reportFile << endl;
  if  (performRandomSampleAll)
  {
    *reportFile << "Random Sampling All     [" << "Yes"                                         << "]" << endl;
  }
  else
  {
    *reportFile << "DataSet                 [" << dataSetParms->name                            << "]" << endl;
    *reportFile << "DataSetRootDir          [" << dataSetRootDir                                << "]" << endl;
    //*reportFile << "RandomSampling          [" << (performRandomSampling ? "Yes" : "No")        << "]" << endl;
    *reportFile << "JobTypes                [" << BitReductionJob::JobTypeToStr (jobTypes)      << "]" << endl;
  }

  *reportFile << "Log File                [" << runLog.FileName ()                            << "]" << endl;
  *reportFile << "ReportFileName          [" << reportFileName                                << "]" << endl;

  *reportFile << "LearningAlgorithm       [" << LearningAlgorithmToStr (learningAlgorithm)    << "]" << endl;
  *reportFile << "Job Types               [" << BitReductionJob::JobTypeToStr (jobTypes)      << "]" << endl;
  *reportFile << "Sample Percentag  e     [" << samplePercentage << "%"                       << "]" << endl;
  *reportFile << "Accuracy Loss Tolerance [" << accLossTolerance << "%"                       << "]" << endl;

  *reportFile << endl;
}  /* ReportCmdLineParameters */





void  BitReductionPaper::ProcessCmdLineParameters (int     argc, 
                                                   char**  argv
                                                  )
{
  int  x;

  for  (x = 1; x < argc;  x++)
  {
    bool    parmGood = true;
    KKStr  parmValue ("");
    KKStr  parmSwitch ("");

    if  (argv[x][0] == '-')  
    {
      parmSwitch = argv[x];
      if  (argc > (x + 1))  
      {
        if  (argv[x + 1][0] != '-')
        {
          x++;
          parmValue = argv[x];
        }
      }
      ProcessCmdLineParameter (parmSwitch, parmValue);
    }
  }  /*  end of for loop  */


  bool  conflictFound = false;

  if  (rootDirName.Empty ())
  {
    runLog.Level (-1) << endl
                      << "You must specify [-RoodDir] when specifying [-RandomSampleAll]" << endl
                      << endl;
    conflictFound = true;
  }

  if  (performRandomSampleAll)
  {

    if  (jobTypes == brjRandomSampling)
    {
      runLog.Level (-1) << endl
                        << "Can not specify both [-RandomSampling] and [-RandomSampleAll]" << endl
                        << endl;
      conflictFound = true;
    }

    if  (dataSetParms)
    {
      runLog.Level (-1) << endl
                        << "Can not specify a Data Set when specifying [-RandomSampleAll]" << endl
                        << endl;
      conflictFound = true;
    }

    if  (conflictFound)
    {
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    if  (reportFileName.Empty ())
    {
      reportFileName = osAddSlash (rootDirName) + "RandomSampleAll.txt";
    }
    else
    {
      KKStr  pathPart = osGetPathPartOfFile (reportFileName);
      if  (pathPart.Empty ())
      {
        reportFileName = osAddSlash (rootDirName) + reportFileName;
      }
    }

    reportFileName = osCreateUniqueFileName (reportFileName);
  }
  else
  {
    // We are not doing -RandomSamplingAll
    if  (!dataSetParms)
    {
      runLog.Level (-1) << endl
                        << "You must specify a DataSet to process [-DataSet]" << endl
                        << endl;
      conflictFound = true;
    }

    if  (conflictFound)
    {
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    if  (reportFileName.Empty ())
    {
      SetDataSetRootDirName ();


      KKStr  baseName = dataSetRootDir + "BitRed_" + dataSetParms->name + "_" + LearningAlgorithmToShortStr (learningAlgorithm);

      if  (jobTypes == brjRandomSampling)
        reportFileName = baseName + "_RandSampling.txt";

      else if  (jobTypes == brjNormalJob)
        reportFileName = baseName + ".txt";

      else if  (jobTypes == brjSmallSample)
        reportFileName = baseName + "_SmallSampling_" + StrFormatDouble (samplePercentage, "000") + ".txt";

      else if  (jobTypes == brjTuningTest)
        reportFileName = baseName + "_Tuning3_"       
                         + StrFormatDouble (samplePercentage,       "000") 
                         + "-"
                         + StrFormatDouble (accLossTolerance * 100, "000") 
                         + ".txt";

      else
        reportFileName = baseName + "_RandomSplits.txt";
    }
    else
    {
      KKStr  pathPart = osGetPathPartOfFile (reportFileName);
      if  (pathPart.Empty ())
      {
        reportFileName = osAddSlash (rootDirName) + reportFileName;
      }
    }

    reportFileName = osCreateUniqueFileName (reportFileName);
  }
}  /* ProcessCmdLineParameters */







void  BitReductionPaper::RunRandomSamplingPasses (TrainingConfigurationPtr  config, 
                                                  SVM_KernalType            kernalType,
                                                  SVM_EncodingMethod        encodingMethiod,
                                                  int                       C,
                                                  int                       bitReduction,
                                                  int                       unBalBits,
                                                  int                       numCompressedImages, 
                                                  OrderingsPtr              orderings,
                                                  double&                   accuracyMean,
                                                  double&                   accuracyStdDev,
                                                  double&                   trainTimeMean,
                                                  double&                   testTimeMean
                                                 )
{
  KKStr  kernmalShortName;
  KKStr  encShortName;

  config->CompressionMethod (BRnoCompression);
  config->KernalType        (kernalType);
  config->EncodingMethod    (encodingMethiod);
  config->C_Param           (C);
  config->BitsToReduceBy    (bitReduction);
  config->UnBalancedBits    (unBalBits);

  switch  (encodingMethiod)
  {
  case  NoEncoding:
  case  Encoding_NULL:
     encShortName = "None";
     break;

  case  BinaryEncoding:
     encShortName = "Binary";
     break;

  case  ScaledEncoding:
     encShortName = "Scale";
     break;
  }


  switch  (kernalType)
  {
  case  KT_NULL:
  case  KT_Linear:
     kernmalShortName = "Linear";
     break;

  case  KT_Polynomial:
     kernmalShortName = "Poly";
     break;

  case  KT_RBF:
     kernmalShortName = "RBF";
     break;
  }

  KKStr  jobFileName = osRemoveExtension (trainDataFileName) + 
                        "_" + "RandomSampling"                +
                        "_" + kernmalShortName                +
                        "_" + encShortName;

  if  (C != 1)
    jobFileName << "_C-" + StrFormatInt (C, "00");

  jobFileName <<  "_" <<  StrFormatInt (numCompressedImages, "000000")
              <<  ".status";
 
  RandomSampleJobListPtr  jobs = RandomSampleJobList::CreateNewList 
                (runLog,
                 config,
                 orderings,
                 numCompressedImages,
                 jobFileName
                );

  RandomSampleJob::ReportResultsHeader (*reportFile);
  {
    // Report results from jobs already completed.
    RandomSampleJobList::const_iterator idx;
    for  (idx = jobs->begin ();  idx != jobs->end ();  idx++)
    {
      RandomSampleJobPtr j = *idx;
      if  (j->Status () == rjDone)
        j->ReportResults (*reportFile);
    }
  }

  RandomSampleJobPtr  job = jobs->LocateOpenJob ();
  while  (job)
  {
    job->EvaluteNode (testData, mlClasses);
    jobs->Save ();
    job->ReportResults (*reportFile);
    job = jobs->LocateOpenJob ();
  }

  double  supportVectorsMean;

  jobs->CalcAccuracyStats (accuracyMean, accuracyStdDev, trainTimeMean, testTimeMean, supportVectorsMean);

  delete  jobs;
}  /* RunRandomSamplingPasses */







int   BitReductionPaper::DetermineCompressedImageCount ()
{
  FeatureVectorListPtr  srcImages = images->DuplicateListAndContents ();
  FeatureVectorListPtr  imagesToTrain = new ImageFeaturesList (fileDesc, false, runLog, 10000);

  MLClassListPtr mlClasses = srcImages->ExtractListOfClasses ();

  {
    MLClassList::const_iterator  idx;

    for  (idx = mlClasses->begin ();  idx != mlClasses->end ();  idx++)
    {
      MLClassPtr  mlClass = *idx;
      FeatureVectorListPtr  imagesThisClass = srcImages->ExtractImagesForAGivenClass (mlClass);
      imagesToTrain->AddQueue (*imagesThisClass);
      delete  imagesThisClass;
    }
  }

  NormalizationParms  normParms (config, *imagesToTrain, runLog);
  normParms.NormalizeImages (imagesToTrain);

  ClassAssignments  classAssignments (*mlClasses, runLog);
  FeatureVectorListPtr  compressedImageList = new ImageFeaturesList (fileDesc, true, runLog, 10000);

  BitReduction br (config->SVMparamREF (), fileDesc, images->AllFeatures ());

  CompressionStats compressionStats 
              = br.compress (*imagesToTrain, 
                             compressedImageList, 
                             classAssignments
                            );

  delete  compressedImageList;  compressedImageList = NULL;
  delete  mlClasses;         mlClasses        = NULL;
  delete  imagesToTrain;        imagesToTrain       = NULL;
  delete  srcImages;            srcImages           = NULL;

  return compressionStats.num_images_after;
}  /* DetermineCompressedImageCount */





void  BitReductionPaper::RunRandomSamplingOnOneDataSet (DataSetParameters&  dsp,
                                                        double&             accuracyMean,
                                                        double&             accuracyStdDev,
                                                        double&             trainTimeMean,
                                                        double&             testTimeMean,
                                                        float&              compRatio,
                                                        float&              majarityClassFrac
                                                       )
{
  *reportFile << endl << endl << endl
              << "***************************************************************************************************" << endl
              << "   Starting Dataset[" << dsp.name << "]" << endl
              << endl;

  bool  cancelFlag = false;

  accuracyMean   = -1.0;
  accuracyStdDev = -1.0;
  trainTimeMean  = 0.0;
  testTimeMean   = 0.0;
  compRatio      = 0.0;

  delete  images;    images   = NULL;
  delete  testData;  testData = NULL;
  delete  config;    config   = NULL;

  delete  mlClasses;  mlClasses = NULL;

  mlClasses = new MLClassList ();

  bool successful = false;  // will be used by the feature file load routines.

  inputFormat = dsp.fileFormat;

  KKStr  dataSetDirName = osAddSlash (osAddSlash (rootDirName) + dsp.dataSetSubDir);
  
  if  (!osValidDirectory (dataSetDirName))
  {
    *reportFile << endl
                << "**** Invalid Sub-directory[" << dataSetDirName << "] ****" << endl
                << endl;

    runLog.Level (-1) << endl << endl
                      << "RunRandomSamplingOnOneDataSet    *** SubDir Invalid ***" << endl
                      << endl
                      << "SubDir[" << dataSetDirName << "]" << endl
                      << endl;
    return;
  }

  osChangeDir (dataSetDirName, successful);
  if  (!successful)
  {
    *reportFile << endl
                << "**** Cound not make SubDir[" << dataSetDirName << "] Current Directory." << endl
                << endl;

    runLog.Level (-1) << endl << endl
                      << "RunRandomSamplingOnOneDataSet    *** Could not make SubDir[" << dataSetDirName << "] current directory." << endl
                      << endl;
    return;
  }


  trainDataFileName = dataSetDirName + dsp.trainDataFileName;

  delete  images;  images = NULL;

  bool  changesMade = false;
  images = inputFormat->LoadFeatureFile (trainDataFileName, *mlClasses, -1, cancelFlag, successful, changesMade, runLog);

  if  (!successful)
  {
    *reportFile << endl
                << "**** Invalid Training Data File [" << trainDataFileName << "] ****" << endl
                << endl;

    runLog.Level (-1) << endl
                      << "RunRandomSamplingOnOneDataSet    Error loading training data[" << dsp.trainDataFileName << "]" << endl
                      << endl;
    return;
  }

  images->RemoveEntriesWithMissingFeatures ();
  fileDesc = images->FileDesc ();


  testDataFileName = dataSetDirName + dsp.testDataFileName;

  testData = inputFormat->LoadFeatureFile (testDataFileName, 
                                           *mlClasses,
                                           -1,
                                           cancelFlag,
                                           successful,
                                           changesMade,
                                           runLog
                                          );

  if  ((!successful)  ||  (!testData))
  {
    *reportFile << endl
                << "**** Invalid Test Data File [" << testDataFileName << "] ****" << endl
                << endl;

    runLog.Level (-1) << endl
                      << "RunRandomSamplingOnOneDataSet    Error loading training data[" << dsp.testDataFileName << "]" << endl
                      << endl;
    return;
  }


  // Since for right now we do not know how to handle missing data, 
  // will remove from data loaded from file;
  testData->RemoveEntriesWithMissingFeatures ();


  configFileName = dataSetDirName + dsp.configFileName;
  config = new TrainingConfiguration (fileDesc, configFileName, runLog, false);
  if  (!config->FormatGood ())
  {
    *reportFile << endl
                << "**** Invalid Configuration File [" << configFileName << "] ****" << endl
                << endl;

    runLog.Level (-1) << endl
                        << "RunRandomSamplingOnOneDataSet **** Invalid Configuration File[" << configFileName << "] ****" << endl
                        << endl;
    return;
  }


  OrderingsPtr  orderings = NULL;
  orderings = Orderings::CreateOrderingsObjFromFileIfAvaliable (images, 50, 1);

  majarityClassFrac = images->MajorityClassFraction ();

  config->KernalType         (dsp.kernalTypes[0]);
  config->EncodingMethod     (dsp.encodingMethods[0]);
  config->WeightBitReduction (true);
  config->CompressionMethod  (BRunBalancedVariance);
  config->C_Param            (dsp.c_params[0]);
  config->BitsToReduceBy     (dsp.rsBrBits);
  config->UnBalancedBits     (dsp.rsUBR);


  int  compressedImageCount = DetermineCompressedImageCount ();
  compRatio = float (double (compressedImageCount) / double (images->QueueSize ()));

  RunRandomSamplingPasses (config, 
                           config->KernalType (),
                           config->EncodingMethod (),
                           dsp.c_params[0],
                           dsp.rsBrBits,
                           dsp.rsUBR,
                           compressedImageCount,
                           orderings, 
                           accuracyMean, 
                           accuracyStdDev,
                           trainTimeMean,
                           testTimeMean
                          );

  delete  orderings;  orderings = NULL;

} /* RunRandomSamplingOnOneDataSet */





void  BitReductionPaper::RunRandomSamplingOnAllDataSets ()
{
  int  idx = 0;

  VectorInt                     bits;
  VectorInt                     unBalBits;
  VectorDouble                  accuracyMeans;
  VectorDouble                  accuracyStdDevs;
  VectorFloat                   compRatios;
  VectorDouble                  trainTimeMeans;
  VectorDouble                  testTimeMeans;
  VectorFloat                   majorityFractions;
  vector<DataSetParametersPtr>  dataSets;


  while  (dataSetParamaters[idx].dataSet != DataSetNULL)
  {
    DataSetParameters&  dsp = dataSetParamaters[idx];

    if  (dsp.rsBrBits >= 0)
    {
      double  accuracyMean;
      double  accuracyStdDev;
      double  trainTimeMean;
      double  testTimeMean;
      float   compRatio;
      float   majorityFrac;
    
      RunRandomSamplingOnOneDataSet (dsp,
                                     accuracyMean,
                                     accuracyStdDev,
                                     trainTimeMean,
                                     testTimeMean,
                                     compRatio,
                                     majorityFrac
                                    );

      dataSets.push_back          (&dsp);
      bits.push_back              (dsp.rsBrBits);
      unBalBits.push_back         (dsp.rsUBR);
      accuracyMeans.push_back     (accuracyMean);
      accuracyStdDevs.push_back   (accuracyStdDev);
      trainTimeMeans.push_back    (trainTimeMean);
      testTimeMeans.push_back     (testTimeMean);
      compRatios.push_back        (compRatio);
      majorityFractions.push_back (majorityFrac);
    }

    idx++;
  }



  *reportFile << endl << endl << endl;

  *reportFile << "Data"     << "\t"
              << "SVM"      << "\t"
              << ""         << "\t"
              << ""         << "\t"
              << "Bit"      << "\t"
              << "Un"       << "\t"
              << "Comp"     << "\t"
              << "Majority" << "\t"
              << "Mean"     << "\t"
              << "Acc"      << "\t"
              << "Train"    << "\t"
              << "Test"    
              << endl;

  *reportFile << "Set"      << "\t"
              << "Kernal"   << "\t"
              << "Encoding" << "\t"
              << "C"        << "\t"
              << "Red"      << "\t"
              << "Bits"     << "\t"
              << "Ratios"   << "\t"
              << "Perc"     << "\t"
              << "Accuracy" << "\t"
              << "StdDev"   << "\t"
              << "Time"     << "\t"
              << "Time"
              << endl;


  idx = 0;
  for  (idx = 0;  idx < int (dataSets.size ());  idx++)
  {
    DataSetParameters&  dsp = *(dataSets[idx]);

    *reportFile << dsp.name                                       << "\t"
                << KernalTypeToStr     (dsp.kernalTypes     [0])  << "\t"
                << EncodingMethodToStr (dsp.encodingMethods [0])  << "\t"
                << dsp.c_params[0]                                << "\t"
                << bits               [idx]                       << "\t"
                << unBalBits          [idx]                       << "\t"
                << compRatios         [idx]                       << "\t"
                << (majorityFractions [idx] * 100.0)              << "\t"
                << accuracyMeans      [idx]                       << "\t"
                << accuracyStdDevs    [idx]                       << "\t"
                << trainTimeMeans     [idx]                       << "\t"
                << testTimeMeans      [idx]
                << endl;
  }

}  /* RunRandomSamplingOnAllDataSets */





void  BitReductionPaper::ProcessAllJobsForASingleDataSet (DataSetParameters&  dsp)
{
   BitReductionJobList  jobs (runLog, 
                              rootDirName,
                              config,
                              dsp,
                              jobTypes,
                              *reportFile,
                              samplePercentage,
                              learningAlgorithm,
                              accLossTolerance
                             );

   if  (jobTypes == brjRandomSplits)
     jobs.ProcessRandomSplitsJobQueue (*reportFile);

   else if  (jobTypes == brjTuningTest)
     jobs.ProcessTuningJobQueue (*reportFile);

   else
     jobs.ProcessJobQueue (*reportFile);

}  /* ProcessAllJobsForASingleDataSet */




//**************************************************************************
//*                           The Main Module                              *
//*                                                                        *
//**************************************************************************

int  BitReductionPaper::Main (int  argc,  char** argv)
{
  ProcessCmdLineParameters (argc, argv);

  if  (reportFileName.Empty ())
  {
    reportFile = &cout;
  }
  else
  {
    reportFileStream = new ofstream (reportFileName.Str ());
    if  (!reportFileStream->is_open ())
    {
      runLog.Level (-1) << "CrossValidation - Error Opening Report File["
        << reportFileName << "]."
        << endl;
      osWaitForEnter ();
      return  -1;
    }

    reportFile = reportFileStream;
  }

  if  (learningAlgorithm == laUSFcascor)
  {
    // Lets make sure that teh Cascading Correlation program can be found.
    if  (!BitReductionJob::DoseUsfCasCorExist ())
    {
      runLog.Level (-1) << endl << endl << endl
                        << "BitReductionPaper::Main      ***ERROR***     Can not find 'usfcascor' program." << endl
                        << "                                             This program needs to be in the default directory." << endl
                        << endl;
      osWaitForEnter ();
      return -1;
    }
  }


  ReportCmdLineParameters ();

  startTime = osGetLocalDateTime ();
  
  *reportFile << "Start Time[" << startTime << "]." << endl << endl;

  if  (performRandomSampleAll)
  {
    RunRandomSamplingOnAllDataSets ();
  }
  else
  {
    ProcessAllJobsForASingleDataSet (*dataSetParms);
  }

  return 0;
} /* Main */



int  main (int    argc, 
           char** argv
          )
{
  #if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
  //  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
  #endif

  TimeType t = osGetLocalDateTime ().Time ();

  
  long  processorId = osGetProcessId ();



  long  z = t.Hour () * 3600 + t.Minute () * 60 + t.Seconds () + (long)osGetSystemTimeUsed () + processorId;
  srand48  (z);


  BitReductionPaperPtr  bitreductionPaper = new BitReductionPaper ();
  bitreductionPaper->Main (argc, argv);
  delete  bitreductionPaper;
  bitreductionPaper = NULL;

  MLClass::FinalCleanUp ();
  FileDesc::FinalCleanUp ();

  return 0;
}
