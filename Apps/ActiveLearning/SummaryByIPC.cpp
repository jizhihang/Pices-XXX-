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
#include  "RunLog.h"
#include  "Str.h"



#include  "SummaryByIPC.h"

#include  "SummaryByIPCandIPR.h"

#include  "ImageClass.h"
#include  "ImageFeatures.h"
#include  "RunLog.h"





SummaryByIPC::SummaryByIPC (RunLog&          _log,
                            const KKStr     _rootDir,
                            ImageClassList&  _imageClasses,
                            bool&            _successful
                           ):
  ipc          (-1),
  log          (_log),
  rootDir      (_rootDir),
  imageClasses (_imageClasses),
  summaries    (true)
{
  LoadSummaries (_successful);
}



void  SummaryByIPC::GetImageCountsByIncrements (int            imagesPerRetraining,
                                                SortOrderType  sortOrder,
                                                int            increment,
                                                int&           count,
                                                int*&          imageCounts
                                               )
{
  int  x;

  for  (x = 0;  x <   summaries.QueueSize ();  x++)
  {
    if  (summaries[x].ImagesPerRetraining () == imagesPerRetraining)
    {
      summaries[x].GetImageCountsByIncrements (sortOrder, increment, count, imageCounts);
      return;
    }
  }

  count = 0;
  imageCounts = NULL;
  return;
}  /* GetImageCountsByIncrements */





void  SummaryByIPC::GetStatsForNumOfImages (int            _imagesPerRetraining,
                                            SortOrderType  sortOrder, 
                                            int            numOfImages,
                                            float&         accuracyMean,
                                            float&         accuracyMin,
                                            float&         accuracyMax,
                                            float&         accuracyStdDev,
                                            float&         accuracyStdErr,
                                            float&         accuracyDelta,
                                            float&         supportPointsMean,
                                            float&         supportPointsMin,
                                            float&         supportPointsMax,
                                            float&         supportPointsStdDev,
                                            float&         supportPointsStdErr,
                                            float&         trainingTimeMean,
                                            float&         testingTimeMean,
                                            bool&          found
                                           )
{
  int  x;

  for  (x = 0;  x <   summaries.QueueSize ();  x++)
  {
    if  (summaries[x].ImagesPerRetraining () == _imagesPerRetraining)
    {
      summaries[x].GetStatsForNumOfImages (sortOrder, 
                                           numOfImages, 
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
      return;
    }
  }

  accuracyMean        = 0.0f;
  accuracyMin         = 0.0f;
  accuracyMax         = 0.0f;
  accuracyStdDev      = 0.0f;
  accuracyStdErr      = 0.0f;
  accuracyDelta       = 0.0f;
  supportPointsMean   = 0.0f;
  supportPointsMin    = 0.0f;
  supportPointsMax    = 0.0f;
  supportPointsStdDev = 0.0f;
  supportPointsStdErr = 0.0f;
  trainingTimeMean    = 0.0f;
  testingTimeMean     = 0.0f;
  found = false;
}  /* GetStatsForNumOfImages */




void   SummaryByIPC::GenerateSummaryReports ()
{
  int x;

  for  (x = 0; x < summaries.QueueSize ();  x++)
  {
    SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

    KKStr  reportFileName (rootDir);
    osAddLastSlash (reportFileName);
    reportFileName << "SummaryReport_" + 
                      StrFormatInt (summary->InitialImagesPerClass (), "000") + "-IPC_" +
                      StrFormatInt (summary->ImagesPerRetraining   (), "000") + "-IPR"  +
                      ".txt";

    ofstream  r(reportFileName.Str ());
    if  (!r.is_open ())
    {
      log.Level (-1) << endl << endl
                     << "***** ERROR *****"  
                     << endl 
                     << endl
                     << "SummaryByIPC::GenerateSummaryReports    Error Opening File[" << reportFileName << "]." << endl
                     << endl;
    }
    else
    {
      r << endl << endl << endl;
      //summary->PrintReportWithIncrements (r, 1);
      summary->PrintReportByIncrementsAndPaiedTTest (r, 1, 99999);

      r << endl << endl << endl;
      //summary->PrintReportWithIncrements (r, 5);
      summary->PrintReportByIncrementsAndPaiedTTest (r, 5, 99999);

      r << endl << endl << endl;
      //summary->PrintReportWithIncrements (r, 10);
      summary->PrintReportByIncrementsAndPaiedTTest (r, 10, 99999);

      r << endl << endl << endl;
      //summary->PrintReportWithIncrements (r, 20);
      summary->PrintReportByIncrementsAndPaiedTTest (r, 20, 99999);

      r.close ();
    }
  }
}  /* GenerateSummaryReports */









void   SummaryByIPC::GenerateReportsForJMLR ()
{
  log.Level (10) << endl
                 << endl
                 << "SummaryByIPC::GenerateReportsForJMLR" << endl
                 << endl;

  int x;

  int  librarySizeToReportTo = imageClasses.QueueSize () * IPC () + 750;

  int  sampleRates[] = {10, 20, 30, 40, -1};

  for  (x = 0; x < summaries.QueueSize ();  x++)
  {
    SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

    KKStr  baseReportFileName (rootDir);
    osAddLastSlash (baseReportFileName);
    baseReportFileName << "JMLR_CompareMethodsAccuracy"
                       << "_IIPC-" << StrFormatInt (summary->InitialImagesPerClass (), "000")
                       << "_IPR-"  << StrFormatInt (summary->ImagesPerRetraining   (), "00") 
                       << "_SR-";

    int  sampleRate = 0;
    int  sampleRateIDX = 0;
    for  (sampleRateIDX = 0;  (sampleRate = sampleRates[sampleRateIDX]) > 0;  sampleRateIDX++)
    {
      {
        KKStr  accuracyReportFileName (rootDir);
        osAddLastSlash (accuracyReportFileName);
        accuracyReportFileName << "JMLR_CompareMethodsAccuracy"
                               << "_IIPC-" << StrFormatInt (summary->InitialImagesPerClass (), "000")
                               << "_IPR-"  << StrFormatInt (summary->ImagesPerRetraining   (), "00") 
                               << "_SR-";

        accuracyReportFileName << StrFormatInt (sampleRate, "00") << ".txt";
        ofstream  r(accuracyReportFileName.Str ());
        if  (!r.is_open ())
        {
          log.Level (-1) << endl << endl
                         << "***** ERROR *****"  
                         << endl 
                         << endl
                         << "SummaryByIPC::GenerateSummaryReports    Error Opening File[" << accuracyReportFileName << "]." << endl
                         << endl;
          osWaitForEnter ();
          exit (-1);
        }
        else
        {
          summary->PrintReportByIncrementsAndPaiedTTest (r, sampleRate, librarySizeToReportTo);
          r.close ();
        }
      }

      {
        KKStr  spReportFileName (rootDir);
        osAddLastSlash (spReportFileName);
        spReportFileName << "JMLR_CompareMethodsSupPoints"
                         << "_IIPC-" << StrFormatInt (summary->InitialImagesPerClass (), "000")
                         << "_IPR-"  << StrFormatInt (summary->ImagesPerRetraining   (), "00") 
                         << "_SR-";

        spReportFileName << StrFormatInt (sampleRate, "00") << ".txt";

        ofstream  r(spReportFileName.Str ());
        if  (!r.is_open ())
        {
          log.Level (-1) << endl << endl
                         << "***** ERROR *****"  
                         << endl 
                         << endl
                         << "SummaryByIPC::GenerateSummaryReports    Error Opening File[" << spReportFileName << "]." << endl
                         << endl;
          osWaitForEnter ();
          exit (-1);
        }
        else
        {
          summary->PrintReportSupportPointsUsingSampling (r, sampleRate, librarySizeToReportTo);
          r.close ();
        }
      }
    }
  }
}  /* GenerateReportsForJMLR */








void   SummaryByIPC::GenerateReportAccuracyByClass (SortOrderType sortOrder,
                                                    int           sampleRate
                                                   )
{
  int x;

  for  (x = 0; x < summaries.QueueSize ();  x++)
  {
    SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

    KKStr  reportFileName (rootDir);
    osAddLastSlash (reportFileName);
    reportFileName << "AccuracyByClass_" 
                   << SortOrderDescr (sortOrder) << "_"
                   << StrFormatInt (summary->InitialImagesPerClass (), "000") + "-IPC_"
                   << StrFormatInt (summary->ImagesPerRetraining   (), "000") + "-IPR"  
                   << ".txt";

    ofstream  r(reportFileName.Str ());
    if  (!r.is_open ())
    {
      log.Level (-1) << endl << endl
                     << "***** ERROR *****"  
                     << endl 
                     << endl
                     << "SummaryByIPC::GenerateSummaryReports    Error Opening File[" << reportFileName << "]." << endl
                     << endl;
    }
    else
    {
      r << endl << endl << endl;
      summary->PrintReportByClass (r, sortOrder, sampleRate);
      r.close ();
    }
  }
}  /* GenerateReportAccuracyByClass */





void   SummaryByIPC::GenerateReportByIPR_ByTens (ostream&      r,
                                                 SortOrderType sortOrder
                                                )
{
  log.Level (10) << "SummaryByIPC::GenerateReportByIPR_ByTens  sortOrder[" << SortOrderDescr (sortOrder) << "]." << endl;

  KKStr  l;

  if  (summaries.QueueSize () < 1)
  {
    log.Level (-1) << endl
                   << endl
                   << "GenerateReportByIPR_ByTens      *** ERROR ***      There are no Summaries to report." << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }


  int**  imageCounts    = new int*[summaries.QueueSize ()];
  int*   numImageCounts = new int[summaries.QueueSize ()];

  int    largestCount = 0;

  int*   randomImageCounts = NULL;
  int    randomNumImageCount;

  float  accuracyMean;
  float  accuracyMin;
  float  accuracyMax;
  float  accuracyStdDev;
  float  accuracyStdErr;
  float  accuracyDelta;
  float  supportPointsMean;
  float  supportPointsMin;
  float  supportPointsMax;
  float  supportPointsStdDev;
  float  supportPointsStdErr;
  float  trainingTimeMean;
  float  testingTimeMean;
  bool   found;

  int x;

  SummaryByIPCandIPRPtr  firstSummary = summaries.IdxToPtr (0);
  firstSummary->GetImageCountsByIncrements (RandomOrder, 10, randomNumImageCount, randomImageCounts);


  //********************************
  //* Write out first header line. *
  //********************************
  r << "Random"  << "\t\t\t\t\t\t\t\t\t\t\t";
  for  (x = 0; x < summaries.QueueSize ();  x++)
  {
    SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

    r << "\t"
      << summary->ImagesPerRetraining () << "_IPR"
      << "\t\t\t\t\t\t\t\t\t\t\t\t";
  }
  r << endl;



  //*********************************
  //* Write out Second header line. *
  //*********************************
  r << "Count"        << "\t" 
    << "AccMean"      << "\t" 
    << "AccMin"       << "\t" 
    << "AccMax"       << "\t" 
    << "AccStdDev"    << "\t" 
    << "SVsMean"      << "\t"
    << "SVsMin"       << "\t"
    << "SVsMax"       << "\t"
    << "SVsStdDev"    << "\t"
    << "TrainingTime" << "\t"
    << "TestingTime"  << "\t";

  for  (x = 0; x < summaries.QueueSize ();  x++)
  {
    SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

    r <<                    "\t"  // Blank Collum inbetweeen IPR  
      << "Count"         << "\t"        
      << "AccMean"       << "\t" 
      << "AccMin"        << "\t" 
      << "AccMax"        << "\t" 
      << "AccStdDev"     << "\t" 
      << "AccDelta"      << "\t"
      << "SVsMean"       << "\t"
      << "SVsMin"        << "\t"
      << "SVsMax"        << "\t"
      << "SVsStdDev"     << "\t"
      << "TrainingTime"  << "\t"
      << "TestingTime"   << "\t";

    summary->GetImageCountsByIncrements (sortOrder, 10, numImageCounts[x], imageCounts[x]);

    largestCount = Max (largestCount, numImageCounts[x]);
  }
  r << endl;




  log.Level (10) << "GenerateReportByIPR_ByTens  Done Printing Header,  will now print Detail Lines." << endl;
  
  
  //**************************
  //* Print out detail lines *   
  //**************************
  int  row = 0;
  
  for  (row = 0;  row < largestCount;  row++)
  {
    cout << "Printing row[" << row << "]." << endl;

    if  (row < randomNumImageCount)
    {
      cout << "Random Row" << endl;
      firstSummary->GetStatsForNumOfImages (RandomOrder, 
                                            randomImageCounts[row],
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
      if  (!found)
      {
        r << "\t\t\t\t\t\t\t\t\t\t\t\t";
      }
      else
      { 
        r << randomImageCounts[row]                               << "\t"
          << StrFormatDouble (accuracyMean,         "zzz,zz0.00") << "\t"
          << StrFormatDouble (accuracyMin,          "zzz,zz0.00") << "\t"
          << StrFormatDouble (accuracyMax,          "zzz,zz0.00") << "\t"
          << StrFormatDouble (accuracyStdDev,       "zzz,zz0.00") << "\t"
          << StrFormatDouble (accuracyDelta,        "z,zz0.0000") << "\t"
          << StrFormatDouble (supportPointsMean,    "zzz,zz0.0")  << "\t"
          << StrFormatDouble (supportPointsMin,     "zzz,zz0.0")  << "\t"
          << StrFormatDouble (supportPointsMax,     "zzz,zz0.0")  << "\t"
          << StrFormatDouble (supportPointsStdDev,  "zz,zz0.00")  << "\t"
          << StrFormatDouble (trainingTimeMean,     "zz,zz0.00")  << "\t"
          << StrFormatDouble (testingTimeMean,      "zz,zz0.00")  << "\t"
          ;
      }
    }
    else
    {
      r << "\t\t\t\t\t\t\t\t\t\t\t\t";
    }


    for  (x = 0; x < summaries.QueueSize ();  x++)
    {
      SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

      if  (!summary)
      {
        log.Level (-1) << endl
                       << endl
                       << "GenerateReportByIPR_ByTens    *** ERROR ***,  Could not locate Summary[" << x << "]" << endl
                       << endl;
        osWaitForEnter ();
        exit (-1);
      }
       
      if  (row >= numImageCounts[x])
      {
        r << "\t\t\t\t\t\t\t\t\t\t\t\t\t";
      }
      else
      {
        cout << "Going to get stats for Row[" << row << "]  Summary[" << x << "]" << endl;

        summary->GetStatsForNumOfImages (sortOrder, 
                                         imageCounts[x][row], 
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
        {
          r << "\t" << imageCounts[x][row]
            << "\t" << StrFormatDouble (100.0f * accuracyMean,   "zzz,zz0.00") << "%"
            << "\t" << StrFormatDouble (100.0f * accuracyMin,    "zzz,zz0.00") << "%"
            << "\t" << StrFormatDouble (100.0f * accuracyMax,    "zzz,zz0.00") << "%"
            << "\t" << StrFormatDouble (100.0f * accuracyStdDev, "zzz,zz0.00") << "%"
            << "\t" << StrFormatDouble (100.0f * accuracyDelta,  "z,zz0.0000") << "%"
            << "\t" << StrFormatDouble (supportPointsMean,       "zzz,zz0.00") 
            << "\t" << StrFormatDouble (supportPointsMin,        "zzz,zz0.00") 
            << "\t" << StrFormatDouble (supportPointsMax,        "zzz,zz0.00") 
            << "\t" << StrFormatDouble (supportPointsStdDev,     "zzz,zz0.00")
            << "\t" << StrFormatDouble (trainingTimeMean,        "zzz,zz0.00")
            << "\t" << StrFormatDouble (testingTimeMean,         "zzz,zz0.00")
            << "\t";
        }
        else
        {
          r << "\t\t\t\t\t\t\t\t\t\t\t\t\t";
        }
      }
    }

    r << endl;
  }


  delete  randomImageCounts;

  for  (x = 0; x < summaries.QueueSize ();  x++)
    delete  imageCounts[x];

  delete  imageCounts;
  delete  numImageCounts;

  log.Level (10) << "SummaryByIPC::GenerateReportByIPR_ByTens     Exiting." << endl;
  log.Level (10) << endl;
}  /* GenerateReportByIPR_ByTens */














void   SummaryByIPC::GenerateReportByIPRForSampleRate (ostream&      r,
                                                       SortOrderType sortOrder,
                                                       int           sampleRate
                                                     )
{
  log.Level (10) << "SummaryByIPC::GenerateReportByIPRForSampleRate  sortOrder[" << SortOrderDescr (sortOrder) << "]  " 
                 << "Increment[" << sampleRate << "]."
                 << endl;

  KKStr  l;

  if  (summaries.QueueSize () < 1)
  {
    log.Level (-1) << endl
                   << endl
                   << "GenerateReportByIPRForSampleRate      *** ERROR ***      There are no Summaries to report." << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }


  int**  imageCounts    = new int*[summaries.QueueSize ()];
  int*   numImageCounts = new int[summaries.QueueSize ()];

  int    largestCount = 0;

  int*   randomImageCounts = NULL;
  int    randomNumImageCount;

  float  accuracyMean;
  float  accuracyMin;
  float  accuracyMax;
  float  accuracyStdDev;
  float  accuracyStdErr;
  float  accuracyDelta;
  float  supportPointsMean;
  float  supportPointsMin;
  float  supportPointsMax;
  float  supportPointsStdDev;
  float  supportPointsStdErr;
  float  trainingTimeMean;
  float  testingTimeMean;
  bool   found;

  int x;

  SummaryByIPCandIPRPtr  firstSummary = summaries.IdxToPtr (0);
  firstSummary->GetImageCountsByIncrements (RandomOrder, 
                                            sampleRate, 
                                            randomNumImageCount, 
                                            randomImageCounts
                                           );


  //********************************
  //* Write out first header line. *
  //********************************
  r << "Random"  << "\t\t\t\t\t";
  for  (x = 0; x < summaries.QueueSize ();  x++)
  {
    SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

    r << "\t"
      << summary->ImagesPerRetraining () << "_IPR"
      << "\t\t\t\t\t";
  }
  r << endl;



  //*********************************
  //* Write out Second header line. * 
  //*********************************
  r << "Count"        << "\t"   // Random Part 
    << "AccMean"      << "\t" 
    << "AccStdDev"    << "\t" 
    << "AccStdErr"    << "\t"
    << "AccDelta"     << "\t";

  for  (x = 0; x < summaries.QueueSize ();  x++)
  {
    SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

    r <<                    "\t"  // Blank Collum inbetweeen IPR  
      << "Count"         << "\t"        
      << "AccMean"       << "\t" 
      << "AccStdDev"     << "\t" 
      << "AccStdErr"     << "\t"
      << "AccDelta"      << "\t";

    summary->GetImageCountsByIncrements (sortOrder, 
                                         sampleRate, 
                                         numImageCounts[x], 
                                         imageCounts[x]
                                        );

    largestCount = Max (largestCount, numImageCounts[x]);
  }
  r << endl;




  log.Level (10) << "GenerateReportByIPRForSampleRate  Done Printing Header,  will now print Detail Lines." << endl;
  
  
  //**************************
  //* Print out detail lines *   
  //**************************
  int  row = 0;
  
  for  (row = 0;  row < largestCount;  row++)
  {
    cout << "Printing row[" << row << "]." << endl;

    if  (row < randomNumImageCount)
    {
      cout << "Random Row" << endl;
      firstSummary->GetStatsForNumOfImages (RandomOrder,
                                            randomImageCounts[row],
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
      if  (!found)
      {
        r << "\t\t\t\t\t";
      }
      else
      { 
        r << randomImageCounts[row]  << "\t"
          << accuracyMean            << "\t"
          << accuracyStdDev          << "\t"
          << accuracyStdErr          << "\t"
          << accuracyDelta           << "\t";
      }
    }
    else
    {
      r << "\t\t\t\t\t";
    }


    for  (x = 0; x < summaries.QueueSize ();  x++)
    {
      SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

      if  (!summary)
      {
        log.Level (-1) << endl
                       << endl
                       << "GenerateReportByIPRForSampleRate    *** ERROR ***,  Could not locate Summary[" << x << "]" << endl
                       << endl;
        osWaitForEnter ();
        exit (-1);
      }
       
      if  (row >= numImageCounts[x])
      {
        r << "\t\t\t\t\t\t";
      }
      else
      {
        cout << "Going to get stats for Row[" << row << "]  Summary[" << x << "]" << endl;

        summary->GetStatsForNumOfImages (sortOrder, 
                                         imageCounts[x][row], 
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
        {
          r << "\t" << imageCounts[x][row]
            << "\t" << accuracyMean
            << "\t" << accuracyStdDev
            << "\t" << accuracyStdErr
            << "\t" << accuracyDelta
            << "\t";
        }
        else
        {
          r << "\t\t\t\t\t\t";
        }
      }
    }

    r << endl;
  }


  delete  randomImageCounts;

  for  (x = 0; x < summaries.QueueSize ();  x++)
    delete  imageCounts[x];

  delete  imageCounts;
  delete  numImageCounts;

  log.Level (10) << "SummaryByIPC::GenerateReportByIPRForSampleRate     Exiting." << endl;
  log.Level (10) << endl;
}  /* GenerateReportByIPRForSampleRate */







void   SummaryByIPC::GenerateReportByIPRForSampleRate (SortOrderType sortOrder,
                                                       int           sampleRate
                                                      )

{
  log.Level (10) << "SummaryByIPC::GenerateReportByIPRForSampleRate  sortOrder[" << SortOrderDescr (sortOrder) << "]." << endl;

  KKStr  l;

  KKStr  reportFileName (rootDir);
  osAddLastSlash (reportFileName);
  reportFileName << "JMLR_Report_By-IPR_For-" << SortOrderDescrShort (sortOrder) 
                 << "_SampleRate-" << StrFormatInt (sampleRate, "00")
                 << ".txt";
  log.Level (20) << "GenerateReportByIPRForSampleRate  for file[" << reportFileName << "]." << endl;

  ofstream r (reportFileName.Str ());

  if  (!r.is_open ())
  {
    log.Level (-1) << endl << endl
                   << "***** ERROR *****"  
                   << endl 
                   << endl
                   << "SummaryByIPC::GenerateReportByIPRForIncrement    Error Opening File[" << reportFileName << "]." << endl
                   << endl;
  }
  else
  {
    GenerateReportByIPRForSampleRate (r, sortOrder, sampleRate);
    r.close ();
  }

  log.Level (10) << "SummaryByIPC::GenerateReportByIPRForSampleRate     Exiting." << endl;
  log.Level (10) << endl;
}  /* GenerateReportByIPRForSampleRate */







void   SummaryByIPC::GenerateReportByIPR_ByTens (SortOrderType sortOrder)
{
  log.Level (10) << "SummaryByIPC::GenerateReportByIPR_ByTens  sortOrder[" << SortOrderDescr (sortOrder) << "]." << endl;

  KKStr  l;

  KKStr  reportFileName (rootDir);
  osAddLastSlash (reportFileName);
  reportFileName << "SummaryReport_By-IPR_By-" + SortOrderDescr (sortOrder) + ".txt";
  log.Level (20) << "GenerateReportByIPR_ByTens  for file[" << reportFileName << "]." << endl;

  ofstream r (reportFileName.Str ());

  if  (!r.is_open ())
  {
    log.Level (-1) << endl << endl
                   << "***** ERROR *****"  
                   << endl 
                   << endl
                   << "SummaryByIPC::GenerateReportByIPR_ByTens    Error Opening File[" << reportFileName << "]." << endl
                   << endl;
  }
  else
  {
    GenerateReportByIPR_ByTens (r, sortOrder);
    r.close ();
  }

  log.Level (10) << "SummaryByIPC::GenerateReportByIPR_ByTens     Exiting." << endl;
  log.Level (10) << endl;
}  /* GenerateReportByIPR_ByTens */










void   SummaryByIPC::GenerateGNUplotData (ostream&      r,
                                          SortOrderType sortOrder
                                         )
{
  log.Level (10) << "SummaryByIPC::GenerateGNUplotData  sortOrder[" << SortOrderDescr (sortOrder) << "]." << endl;

  if  (summaries.QueueSize () < 1)
  {
    log.Level (-1) << endl
                   << endl
                   << "GenerateGNUplotData      *** ERROR ***      There are no Summaries to report." << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }


  float  accuracyMean;
  float  accuracyMin;
  float  accuracyMax;
  float  accuracyStdDev;
  float  accuracyStdErr;
  float  accuracyDelta;
  float  supportPointsMean;
  float  supportPointsMin;
  float  supportPointsMax;
  float  supportPointsStdDev;
  float  supportPointsStdErr;
  float  trainingTimeMean;
  float  testingTimeMean;

  bool   found;

  int x;

  int  row = 0;
  

  {
    SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (0);

    int   numOfRows = 0;
    int*  imageCounts = NULL;

    summary->GetImageCountsByIncrements (sortOrder, 10, numOfRows, imageCounts);

    for  (row = 0;  row < numOfRows;  row++)
    {
      summary->GetStatsForNumOfImages (RandomOrder, 
                                       imageCounts[row], 
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
      {
        r << imageCounts[row] << "  " << accuracyMean << endl;
      }
    }

    r << endl 
      << endl;
  }

  for  (x = 0; x < summaries.QueueSize ();  x++)
  {
    SummaryByIPCandIPRPtr  summary = summaries.IdxToPtr (x);

    int   numOfRows = 0;
    int*  imageCounts = NULL;

    summary->GetImageCountsByIncrements (sortOrder, 10, numOfRows, imageCounts);

    for  (row = 0;  row < numOfRows;  row++)
    {
      summary->GetStatsForNumOfImages (sortOrder, 
                                       imageCounts[row], 
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
      {
        r << imageCounts[row] << "  " << accuracyMean << endl;
      }
    }

    r << endl 
      << endl;

    delete  imageCounts;
  }


  log.Level (10) << "SummaryByIPC::GenerateGNUplotData     Exiting." << endl;
  log.Level (10) << endl;
}  /* GenerateGNUplotData */





void   SummaryByIPC::GenerateGNUplotData (SortOrderType sortOrder)
{
  log.Level (10) << "SummaryByIPC::GenerateGNUplotData  sortOrder[" << SortOrderDescr (sortOrder) << "]." << endl;

  KKStr  l;

  KKStr  reportFileName (rootDir);
  osAddLastSlash (reportFileName);
  reportFileName << "GNUplotAccuracy_By-IPR_By-" + SortOrderDescr (sortOrder) + ".txt";
  log.Level (20) << "GenerateGNUplotData  for file[" << reportFileName << "]." << endl;

  ofstream r (reportFileName.Str ());

  if  (!r.is_open ())
  {
    log.Level (-1) << endl << endl
                   << "***** ERROR *****"  
                   << endl 
                   << endl
                   << "SummaryByIPC::GenerateGNUplotData    Error Opening File[" << reportFileName << "]." << endl
                   << endl;
  }
  else
  {
    GenerateGNUplotData (r, sortOrder);
    r.close ();
  }

  log.Level (10) << "SummaryByIPC::GenerateGNUplotData     Exiting." << endl;
  log.Level (10) << endl;
}  /* GenerateGNUplotData */




void  SummaryByIPC::LoadSummaries (bool&  _successful)
{
  log.Level (10) << "SummaryByIPC::LoadSummaries" << endl;

  _successful = false;

  KKStr  homeDir (rootDir);
  osAddLastSlash (homeDir);

  StringListPtr  dirList = osGetListOfDirectories (homeDir);

  if  (!dirList)
  {
    _successful = false;
    log.Level (-1) << endl << endl
                   << "****  ERROR  ****" << endl
                   << endl
                   << "SummaryByIPC::LoadSummaries    There were no SubDirectories found in Directory[" << homeDir << "]." << endl
                   << endl;
  }

  else if  (dirList->QueueSize () < 1)
  {
    _successful = false;
    log.Level (-1) << endl << endl
                   << "****  ERROR  ****" << endl
                   << endl
                   << "SummaryByIPC::LoadSummaries    There were no SubDirectories found in Directory[" << homeDir << "]." << endl
                   << endl;
  }

  else
  {
    dirList->Sort (false);
    KKStrPtr  subDirName = NULL;

    StringListIterator  dIDX (*dirList);
   
    for  (dIDX.Reset ();  subDirName = dIDX.CurPtr ();  ++  dIDX)
    {
      log.Level (20) << "LoadSummaries  Looking at SubDirectory[" << *subDirName << "]." << endl;

      KKStr  fullSubDirName (homeDir);
      osAddLastSlash (fullSubDirName);
      fullSubDirName << *subDirName;

      SummaryByIPCandIPRPtr summary = new SummaryByIPCandIPR (log,
                                                              imageClasses,
                                                              fullSubDirName
                                                             );

      if  (ipc <= 0)
      {
        // We don't know how many Images Per Class there will be in the initial 
        // Training Libraries
        ipc = summary->InitialImagesPerClass ();
      }
      else
      {
        if  (ipc != summary->InitialImagesPerClass ())
        {
          // It appears that we are loading summary files that are starting with
          // different number of training images per class.  This should not be 
          // happening.  There is something very very wrong at this time.
          log.Level (-1) << endl
                         << "      *** ERROR ***" << endl
                         << endl
                         << "SummaryByIPC::LoadSummaries   We are encountering summary files with different IIPC" << endl
                         << "                              This should not be happening in this object"           << endl
                         << endl
                         << "IIPC Expected[" << ipc << "]  IIPC Found[" << summary->InitialImagesPerClass () << endl
                         << "Summary File[" << fullSubDirName << "]" << endl
                         << endl;
          osWaitForEnter ();
          exit (-1);
        }
      }

      summaries.PushOnBack (summary);
    }
  }

  _successful = true;

  summaries.SortByImagesPerRetraining ();

  log.Level (10) << "SummaryByIPC::LoadSummaries    Exiting" << endl;
}  /* LoadSummaries */
 


void   SummaryByIPC::RecoverNewTrainingImages (SortOrderType  sortOrder, 
                                               int            numImagesToRecover,
                                               const KKStr&  rootDir
                                              )
{
  for  (SummaryByIPCandIPRList::iterator si = summaries.begin ();  si != summaries.end ();  si++)
  {
    SummaryByIPCandIPRPtr sum = *si;
    int numRetrainingsToKeep = Max (1, numImagesToRecover / sum->IPR ());
    (*(*si)).RecoverNewTrainingImages (sortOrder, numRetrainingsToKeep, rootDir);
  }
}  /* RecoverNewTrainingImages */




SummaryByIPCList::SummaryByIPCList (bool  _owner):
  KKQueue<SummaryByIPC> (_owner)
{
}





SummaryByIPCListIterator::SummaryByIPCListIterator (SummaryByIPCList&  _ipcSummaries):
  QueueIterator<SummaryByIPC> (_ipcSummaries)
{
}
