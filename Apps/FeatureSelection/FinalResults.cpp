#include "FirstIncludes.h"

#ifdef  WIN32
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
#else
  #include <stdlib.h>
#endif

#include <stdio.h>

#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>



#ifdef  WIN32
#include <ostream>
#endif

#include "MemoryDebug.h"
#include <sys/types.h>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif

using namespace std;


#include "BasicTypes.h"
#include "HTMLReport.h"
#include "OSservices.h"
#include "RunLog.h"
#include "StatisticalFunctions.h"
using namespace KKU;


#include "BinaryClassParms.h"
#include "ConfusionMatrix2.h"
using namespace MLL;

#include "FinalResults.h"
#include "RandomSplitResults.h"



FinalResults::FinalResults (const KKStr&  _summaryResultsFileName,
                            double        _totalSearchTime,
                            double        _totalProcessingTime,
                            double        _elapsedClockTime,
                            double        _longestPath,
                            int           _jobCount,
                            RunLog&       _log
                           ):

  configFileName       (),
  elapsedClockTime     (_elapsedClockTime),
  jobCount             (_jobCount),
  longestPath          (_longestPath),
  randomSplitResults   (NULL),
  randomSplitsAvg      (NULL),
  svmParameters        (),
  totalSearchTime      (_totalSearchTime),
  totalProcessingTime  (_totalProcessingTime),
  validationResults    (NULL),
  log                  (_log)

{
  try
  {
  ReadInResultsFile (_summaryResultsFileName);
  }
  catch (exception&  e)
  {
    log.Level (-1) << endl << endl 
      << "FinalResults::FinalResults   ***ERROR***   Exception Thrown Reading in results File[" << _summaryResultsFileName << "]" << endl
      << "                             Exception: " << e.what () << endl
      << endl;
    configFileName = "***ERROR***";
    randomSplitResults = NULL;
    randomSplitsAvg    = NULL;
    svmParameters      = "";
    validationResults  = NULL;
    return;
  }

  if  (randomSplitResults)
  {
    randomSplitsAvg    = randomSplitResults->Average ();
    configFileName     = randomSplitsAvg->ConfigFileName ();
  }

  if  (validationResults)
    configFileName = validationResults->ConfigFileName ();
}



FinalResults::~FinalResults ()
{
  delete  randomSplitResults; randomSplitResults = NULL;
  delete  randomSplitsAvg;    randomSplitsAvg    = NULL;
  delete  validationResults;  validationResults  = NULL;
}



ConfusionMatrix2Ptr  FinalResults::ConfusionMatrix ()
{
  if  (!randomSplitsAvg)
    return NULL;

  return  randomSplitsAvg->ConfusionMatrix ();
}




ConfusionMatrix2Ptr  FinalResults::ValidationConfusionMatrix ()
{
  if  (!validationResults)
    return NULL;

  return  validationResults->ConfusionMatrix ();
}



MLClassConstListPtr  FinalResults::MLClasses ()  const
{
  if  (validationResults)
    return  validationResults->MLClasses ();

  if  (randomSplitsAvg)
    return  randomSplitsAvg->MLClasses ();

  return NULL;
}




KKStr   FinalResults::Description ()  const
{
  if  (validationResults)
    return  validationResults->Description ();

  else if  (randomSplitsAvg)
    return randomSplitsAvg->Description ();

  else
    return KKStr::EmptyStr ();
}




KKStr  FinalResults::LinkName ()  const
{
  KKStr selMethodStr = "";
  if  (validationResults)
  {
    selMethodStr = validationResults->SelectionMethodStr ();
    return  osGetRootName (validationResults->ConfigFileName ()) + selMethodStr;
  }

  else if  (randomSplitsAvg)
  {
    selMethodStr = randomSplitsAvg->SelectionMethodStr ();
    return  osGetRootName (randomSplitsAvg->ConfigFileName ()) + selMethodStr;
  }

  else
    return  KKStr::EmptyStr ();
}




VectorFloat  FinalResults::RandomSplitAccuracies ()  const
{
  VectorFloat  splitAccuracies;
  if  (randomSplitResults)
  {
    RandomSplitResultsList::const_iterator  idx;
    for  (idx = randomSplitResults->begin ();  idx != randomSplitResults->end ();  idx++)
    {
      RandomSplitResultsPtr r = *idx;
      splitAccuracies.push_back (r->Accuracy ());
    }
  }

  return  splitAccuracies;
}  /* RandmSplitAccuracies */




VectorFloat  FinalResults::RandomSplitAccuracyByClass  ()  const
{
  if  (!randomSplitsAvg)
    return VectorFloat ();
  else
    return randomSplitsAvg->AccuracyByClass ();
}




const bool*   FinalResults::ValidationClassedCorrectly () const
{
  if  (!validationResults)
    return NULL;

  return  validationResults->ClassedCorrectly ();
}  /* ValidationClassedCorrectly */



int   FinalResults::ValidationClassedCorrectlySize () const
{
  if  (!validationResults)
    return 0;

  return  validationResults->ClassedCorrectlySize ();
}  /* ValidationClassedCorrectlySize */




VectorFloat  FinalResults::ValidationAccuracyByClass  ()  const
{
  if  (!validationResults)
    return VectorFloat ();
  else
    return validationResults->AccuracyByClass ();
}




void  FinalResults::PrintConfigParametersHTML (HTMLReport&   r,
                                                ResultsPtr    results
                                               )
{
  if  (!results)
  {
     r << endl << endl << "<c>" << "*** NO RESULTS ***" << "</c>"  << endl << endl << endl;
     return;
  }

  KKStr  trainingTimeStr = StrFormatDouble (results->TrainingTime (), "ZZ,ZZZ,ZZ0.0");
  KKStr  testTimeStr     = StrFormatDouble (results->TestTime     (), "ZZ,ZZZ,ZZ0.0");

  r << "<table align=\"center\" border=\"2\" cellpadding=\"3\" cellspacing=\"0\" frame=\"box\"  summary=\"Confusion \" >" << endl
    << "  <tr>" << "<td>" << "Description"         << "</td>" << "<td>" << results->Description ()                                  << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Config File Name"    << "</td>" << "<td>" << configFileName                                           << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Machine Type"        << "</td>" << "<td>" << results->MachineTypeStr ()                               << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Selection Method"    << "</td>" << "<td>" << results->SelectionMethodStr ()                           << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Config Parms"        << "</td>" << "<td>" << svmParameters                                            << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Training Time"       << "</td>" << "<td>" << trainingTimeStr                                          << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Test Time"           << "</td>" << "<td>" << testTimeStr                                              << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Support Vectors's"   << "</td>" << "<td>" << results->NumSupportVectors ()                            << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Number of Features"  << "</td>" << "<td>" << results->NumFeatures ()                                  << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "CPU Search Time"     << "</td>" << "<td>" << StrFormatDouble (totalSearchTime,     "ZZ,ZZZ,ZZ0")      << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "CPU Classifier Time" << "</td>" << "<td>" << StrFormatDouble (totalProcessingTime, "ZZ,ZZZ,ZZ0")      << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Longest Path Time"   << "</td>" << "<td>" << StrFormatDouble (longestPath,         "ZZ,ZZZ,Z00")      << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Job Count"           << "</td>" << "<td>" << StrFormatInt    (jobCount,            "ZZ,ZZZ,ZZ0")      << "</td>" << "</tr>" << endl
    << "  <tr>" << "<td>" << "Elapsed Clock Time"  << "</td>" << "<td>" << StrFormatDouble (elapsedClockTime,    "ZZ,ZZZ,ZZ0")      << "</td>" << "</tr>" << endl
    << "</table>" << endl
    << endl;
}


void  FinalResults::PrintConfigParametersValidationHTML (HTMLReport& r)
{
  PrintConfigParametersHTML (r, validationResults);
}


void  FinalResults::PrintConfigParametersRandomSplitsHTML (HTMLReport& r)
{
  PrintConfigParametersHTML (r, randomSplitsAvg);
}



KKStr  FinalResults::HeaderLineHTML1 ()
{
  KKStr s (132);

  s << "<th colspan=\"4\">" << "Configuration"          << "</th>"
    << "<th colspan=\"7\">" << "Test Results"           << "</th>"
    << "<th colspan=\"7\">" << "Random Splits Results"  << "</th>"
    << "<th colspan=\"7\">" << "Search Statistics"      << "</th>";
  return  s;
}



KKStr  FinalResults::HeaderLineHTML2 ()
{
  KKStr s (132);

    // Configuration Parameters.
  s << "<th>" << "Config"                          << "</th>"
    << "<th>" << "Description"                     << "</th>"
    << "<th>" << "Machine<br />Type"               << "</th>"
    << "<th>" << "Selection<br />Method"           << "</th>"

    // Validation Results
    << "<th>" << "Accuracy"                        << "</th>"
    << "<th>" << "Accuracy<br />Normalized"        << "</th>"
    << "<th>" << "Pred Prob"                       << "</th>"
    << "<th>" << "Training<br />Time"              << "</th>"
    << "<th>" << "Test<br />Time"                  << "</th>"
    << "<th>" << "Number<br />Tot SV's"            << "</th>"
    << "<th>" << "Number<br />Net SV's"            << "</th>"

    // Random Splits Results
    << "<th>" << "Accuracy"                        << "</th>"
    << "<th>" << "Accuracy<br />Normalized"        << "</th>"
    << "<th>" << "Average<br />Pred Prob"          << "</th>"
    << "<th>" << "Training<br />Time"              << "</th>"
    << "<th>" << "Test<br />Time"                  << "</th>"
    << "<th>" << "Number<br />Tot SV's"            << "</th>"
    << "<th>" << "Number<br />Net SV's"            << "</th>"

    << "<th>" << "Number<br />Features"            << "</th>"
    << "<th>" << "Number<br />Features<br />Wtd"   << "</th>"
    << "<th>" << "CPU<br />Search<br />Time"       << "</th>"
    << "<th>" << "CPU<br />Classifier<br />Time"   << "</th>"
    << "<th>" << "Longest<br />Path<br />Time"     << "</th>"
    << "<th>" << "Job<br />Count"                  << "</th>"
    << "<th>" << "Elapsed<br />Clock<br />Time"    << "</th>";
  return s;
}



KKStr  FinalResults::ToHTMLStr () const
{
  
  ConfusionMatrix2Ptr  valCM = NULL;

  KKStr  description;
  KKStr  machineTypeStr;
  KKStr  selectionMethodStr;

  KKStr  valAccuracyStr;

  KKStr  valTrainingTimeStr;
  KKStr  valTestTimeStr;
  KKStr  valTotalNumSVsStr;
  KKStr  valNumSVsStr;
  KKStr  valAccuracyNormStr;
  KKStr  valAvgPredProbStr;

  KKStr  numFeaturesStr;
  KKStr  numFeaturesWtdStr;

  if  (validationResults)
  {
    valCM = validationResults->ConfusionMatrix ();
    description         = validationResults->Description ();
    machineTypeStr      = validationResults->MachineTypeStr ();
    selectionMethodStr  = validationResults->SelectionMethodStr ();

    valAccuracyStr      = StrFormatDouble (valCM->Accuracy (), "ZZ0.0000") + "%";
    valAccuracyNormStr  = StrFormatDouble (valCM->AccuracyNorm (), "ZZ0.0000") + "%";
    valAvgPredProbStr   = StrFormatDouble (100.0 * valCM->AvgPredProb (),  "ZZ0.0000") + "%";

    valTrainingTimeStr  = StrFormatDouble (validationResults->TrainingTime        (), "ZZ,ZZZ,ZZ0.000");
    valTestTimeStr      = StrFormatDouble (validationResults->TestTime            (), "ZZ,ZZZ,ZZ0.000");
    valTotalNumSVsStr   = StrFormatDouble (validationResults->TotalNumSVs         (), "ZZ,ZZ0.000");
    valNumSVsStr        = StrFormatDouble (validationResults->NumSVs              (), "ZZ,ZZ0.000");
    numFeaturesStr      = StrFormatDouble (validationResults->NumFeatures         (), "ZZ,ZZ0.00");
    numFeaturesWtdStr   = StrFormatDouble (validationResults->NumFeaturesWeighted (), "ZZ,ZZ0.00");
  }
  else
  {
    description         = randomSplitsAvg->Description ();
    machineTypeStr      = randomSplitsAvg->MachineTypeStr ();
    selectionMethodStr  = randomSplitsAvg->SelectionMethodStr ();
    numFeaturesStr      = StrFormatDouble (randomSplitsAvg->NumFeatures         (), "ZZ,ZZ0.00");
    numFeaturesWtdStr   = StrFormatDouble (randomSplitsAvg->NumFeaturesWeighted (), "ZZ,ZZ0.00");
  }

  ConfusionMatrix2Ptr  rsCM  = randomSplitsAvg->ConfusionMatrix ();
  KKStr  rsTrainingTimeStr   = StrFormatDouble (randomSplitsAvg->TrainingTime      (), "ZZ,ZZZ,ZZ0.000");
  KKStr  rsTestTimeStr       = StrFormatDouble (randomSplitsAvg->TestTime          (), "ZZ,ZZZ,ZZ0.000");
  KKStr  rsNumSVsStr         = StrFormatDouble (randomSplitsAvg->NumSVs            (), "ZZ,ZZ0.000");
  KKStr  rsTotalNumSVsStr    = StrFormatDouble (randomSplitsAvg->TotalNumSVs       (), "ZZ,ZZ0.000");

  double  rsAccuracy     = 0.0f;
  float   rsAccuracyNorm = 0.0f;
  double  rsAvgPredProb  = 0.0f;
  if  (rsCM)
  {
    rsAccuracy     = rsCM->Accuracy ();
    rsAccuracyNorm = rsCM->AccuracyNorm ();
    rsAvgPredProb  = rsCM->AvgPredProb ();
  }


  KKStr s (132);

    // Configuration Parameters.
  s << "<td  style=\"text-align:left\"><a href=\"#" << LinkName () << "-VAL" << "\">" << configFileName  << "</a>" << "</td>"
    << "<td  style=\"text-align:left\">" << description                              << "</td>"
    << "<td>" << machineTypeStr                                                      << "</td>"
    << "<td>" << selectionMethodStr                                                  << "</td>"

    // Validation Results
    << "<td>" << "<a href=\"#" << LinkName () << "-VAL" << "\">"
              << valAccuracyStr
              << "</a>"
    << "</td>"

    << "<td>" << valAccuracyNormStr                  << "</td>"
    << "<td>" << valAvgPredProbStr                   << "</td>"
    << "<td>" << valTrainingTimeStr                                                  << "</td>"
    << "<td>" << valTestTimeStr                                                      << "</td>"
    << "<td>" << valTotalNumSVsStr                                                   << "</td>"
    << "<td>" << valNumSVsStr                                                        << "</td>"

    << "<td>" << "<a href=\"#" << LinkName () << "-RS" << "\">"
              << StrFormatDouble (rsAccuracy, "ZZ0.0000") << "%" 
              << "</a>"
    << "</td>"

    << "<td>" << StrFormatDouble (rsAccuracyNorm,                "ZZ0.0000") << "%" << "</td>"
    << "<td>" << StrFormatDouble (100.0 * rsAvgPredProb,         "ZZ0.0000") << "%" << "</td>"
    << "<td>" << rsTrainingTimeStr                                                  << "</td>"
    << "<td>" << rsTestTimeStr                                                      << "</td>"
    << "<td>" << rsTotalNumSVsStr                                                   << "</td>"
    << "<td>" << rsNumSVsStr                                                        << "</td>"

    // Search Statistics
    << "<td>" << numFeaturesStr                                                   << "</td>"
    << "<td>" << numFeaturesWtdStr                                                << "</td>"
    << "<td>" << StrFormatDouble (totalSearchTime,     "ZZ,ZZZ,ZZ0.000")          << "</td>"
    << "<td>" << StrFormatDouble (totalProcessingTime, "ZZ,ZZZ,ZZ0.000")          << "</td>"
    << "<td>" << StrFormatDouble (longestPath,         "ZZ,ZZZ,ZZ0.0")            << "</td>"
    << "<td>" << StrFormatInt    (jobCount,            "ZZ,ZZZ,ZZ0")              << "</td>"
    << "<td>" << StrFormatDouble (elapsedClockTime,    "ZZ,ZZZ,ZZ0.000")          << "</td>";
  return s;
}  /* ToHTMLStr */




void  FinalResults::ReadInResultsFile (const KKStr&  fileName)
{
  ifstream i (fileName.Str ());
  if  (!i.is_open ())
  {
    KKStr  errMsg = "FinalResults::ReadInResultsFile  Error opening [" + fileName + "]";
    log.Level (-1) << errMsg << endl;
    throw errMsg;
  }

  delete  randomSplitResults;  randomSplitResults = NULL;
  delete  validationResults;   validationResults  = NULL;
  randomSplitResults = new RandomSplitResultsList (true, log);

  char buff[50000];
  i.getline (buff, sizeof (buff));
  while  (!i.eof ())
  {
    KKStr  ln (buff);
    ln.TrimLeft ();
    ln.TrimRight ();

    KKStr  lineName = ln.ExtractToken2 ("\t");

    if  (lineName.CompareIgnoreCase ("<RandomSplitResults>") == 0)
    {
      RandomSplitResultsPtr r = RandomSplitResults::ConstructFromStream (i, log);
      if  (r)
        randomSplitResults->PushOnBack (r);
    }

    else if  (lineName.CompareIgnoreCase ("<ValidationResults>") == 0)
    {
      delete  validationResults;
      validationResults = ValidationResults::ConstructFromStream (i, log);
    }

    i.getline (buff, sizeof (buff));
  }


  i.close ();
}  /* ReadInResultsFile */







void  FinalResults::PrintAccuraciesByClassSummary (vector<FinalResultsPtr>&  results,
                                                    HTMLReport&                o,
                                                    bool                       validationData
                                                   )
{
  uint  x;

  if  (results.size () < 1)
  {
    o << endl << endl
      << "FinalResults::PrintAccuraciesByClassSummary      *** No Summary Records Provided ***"  << endl
      << endl;
    return;
  }

  ConfusionMatrix2Ptr  cm = NULL;
  if  (validationData)
    cm = results[0]->ValidationConfusionMatrix ();
  else
    cm = results[0]->ConfusionMatrix ();

  if  (!cm)
  {
    o << endl << endl
      << "FinalResults::PrintAccuraciesByClassSummary      *** No Confusion Matrix Provided ***"  << endl
      << endl;
    return;
  }

  MLClassConstListPtr  classes = results[0]->MLClasses ();
  if  (!classes)
  {
    o << endl << endl
      << "FinalResults::PrintAccuraciesByClassSummary      *** NO Class List Defined ***"  << endl
      << endl;
    return;
  }

  MLClassConstList::iterator  idx;
  
  o << "<table cellpadding=\"2\" cellspacing=\"0\" border=\"2\">" << endl;
	o << "<tr>" << endl;

	// output the first row (which is class names)
	o << "<th align=\"center\" bgcolor=\"#CCCCCC\"><b>Description</b></th>" << endl;

  for  (idx = classes->begin ();  idx != classes->end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;
    KKStr  className = ic->Name ();
    VectorKKStr classNameParsed = className.Parse ("_");

    o << "<th align=\"center\" bgcolor=\"#CCCCCC\"><b>";
    for  (x = 0;  x < classNameParsed.size ();  x++)
    {
      if  (x > 0)
        o << "<br />";
      o << classNameParsed[x];
    }
    o << "</b></th>";
  }
  o << "<th align=\"center\"><b>" << "Avg" << "<br />" << "Accuracy" << "</b></th>";
	o << "</tr>" << endl;

  VectorDouble classCounts = cm->CountsByKnownClass ();

  double  totalCount = 0;

  o << "<tr>";
  o << "<td align=\"left\">"  << "Counts" << "</th>";
  for  (x = 0;  x < classCounts.size ();  x++)
  {
    o << "<td align=\"center\">" << classCounts[x] << "</th>";
    totalCount += classCounts[x];
  }
  o << "<td align=\"center\">" << totalCount << "</th>";
  o << "</tr>" << endl;

  o << "<tr>";
  o << "<td align=\"left\">"  << "Percent" << "</th>";
  for  (x = 0;  x < classCounts.size ();  x++)
  {
    double  perc = 100.0 * (double)classCounts[x] / totalCount;
    o << "<td align=\"center\">" << StrFormatDouble (perc, "##0.00") << "%" << "</th>";
  }
  o << "</tr>" << endl;

  uint  frIDX = 0;
  for  (frIDX = 0;  frIDX < results.size ();  frIDX++)
  {
    FinalResultsPtr  fr = results[frIDX];
    if  (validationData)
      cm = fr->ValidationConfusionMatrix ();
    else
      cm = fr->ConfusionMatrix ();

    VectorFloat  accuraciesByClass = cm->AccuracyByClass ();

    float  toatalAcc = 0.0f;
    o << "<tr>";
    o << "<td align=\"left\">"  << fr->Description () << "</th>";
    for  (x = 0;  x < accuraciesByClass.size ();  x++)
    {
      o << "<td align=\"center\">" << StrFormatDouble (accuraciesByClass[x], "##0.000") << "%" << "</th>";
      toatalAcc += accuraciesByClass[x];
    }

    float  avgAcc = toatalAcc / (float)accuraciesByClass.size ();
    o << "<td align=\"center\">" << StrFormatDouble (avgAcc, "##0.000") << "%" << "</th>";

    o << "</tr>" << endl;
  }
  o << "</table>" << endl;

  delete  classes;  classes = NULL;

}  /* PrintAccuraciesByClassSummary */

