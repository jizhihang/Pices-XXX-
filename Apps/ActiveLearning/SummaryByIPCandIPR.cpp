#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>

#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <iomanip>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;


#include <sys/types.h>

#include  "KKQueue.h"
#include  "Str.h"



#include  "SummaryByIPCandIPR.h"

#include  "ClassGroupHistory.h"
#include  "ImageClass.h"
#include  "OSservices.h"
#include  "RunLog.h"
#include  "SortOrderType.h"



//********************************************************************************************
//*                                SummaryByIPCandIPR                                        *
//*                                                                                          *
//*  Purpose:  Combine results from several different ClassGroupToalts where they all have   *
//*            the same IPC(Image Per Class)  and  IPR(Images Per Retraining). These results *
//*            can then be saved into a file, for later use with out having to extract them  *
//*            again.                                                                        *
//*                                                                                          *
//*  This object will scan the contents of a given directory where the results of an         *
//*  ActiveLearning run are stored and create the appropriate instances of ClassGroupTotals. *
//*  Using the totals stored in the objects  Various summary reports can be generated.       *
//*  And a summary file can also be generated.                                               *
//*                                                                                          *
//********************************************************************************************


class  RetrainingResults
{
public:
  RetrainingResults ();

  RetrainingResults (int           _group,
                     int           _numOfPasses,
                     float*        _accuracies,
                     float         _numOfTrainingImages,
                     float         _accuracyMean,
                     float         _accuracyMin,
                     float         _accuracyMax,
                     float         _accuracyStdDev,
                     float         _accuracyDelta,     // kk 2005-06-29  Added with Version 6
                     float         _supportPointsMean,
                     float         _supportPointsMin,
                     float         _supportPointsMax,
                     float         _supportPointsStdDev,
                     float         _trainingTimeMean,
                     float         _testingTimeMean,
                     VectorFloat&  _trainLibClassCounts,
                     VectorFloat&  _classAccuracies
                    );

  ~RetrainingResults ();

  int           Group               () {return group;}
  int           NumOfPasses         () {return numOfPasses;}
  float*        Accuracies          () {return accuracies;}
  float         NumOfTrainingImages () {return numOfTrainingImages;}
  float         AccuracyMean        () {return accuracyMean;}
  float         AccuracyMin         () {return accuracyMin;}
  float         AccuracyMax         () {return accuracyMax;}
  float         AccuracyStdDev      () {return accuracyStdDev;}
  float         AccuracyDelta       () {return accuracyDelta;}
  float         SupportPointsMean   () {return supportPointsMean;}
  float         SupportPointsMin    () {return supportPointsMin;}
  float         SupportPointsMax    () {return supportPointsMax;}
  float         SupportPointsStdDev () {return supportPointsStdDev;}
  float         TrainingTimeMean    () {return trainingTimeMean;}
  float         TestingTimeMean     () {return testingTimeMean;}

  const
  VectorFloat&  ClassAccuracies     ()  {return  classAccuracies;}

  const
  VectorFloat&  TrainLibClassCounts () {return  trainLibClassCounts;}



  KKStr  ToString ();

  float   AccurcayByClass    (int  classIDX)  {return  classAccuracies[classIDX];}

  float   TrainLibClassCount (int  classIDX)  {return  trainLibClassCounts[classIDX];}


private:
  int          group;
  int          numOfPasses;
  float*       accuracies;           // Will be array with numOfPasses  elements,  one for each random pass
  float        numOfTrainingImages;
  float        accuracyMean;
  float        accuracyMin;
  float        accuracyMax;
  float        accuracyStdDev;
  float        accuracyDelta;        // kk 2005-06-29  Added with Version 6
  VectorFloat  classAccuracies;
  VectorFloat  trainLibClassCounts;
  float        supportPointsMean;
  float        supportPointsMin;
  float        supportPointsMax;
  float        supportPointsStdDev;
  float        trainingTimeMean;
  float        testingTimeMean;
};


typedef  RetrainingResults*  RetrainingResultsPtr;



class  RetrainingResultsList: public KKQueue<RetrainingResults>
{
public:
  RetrainingResultsList (bool _owner,  int _size):
    KKQueue<RetrainingResults> (_owner, _size)  {}
};


typedef  RetrainingResultsList*    RetrainingResultsListPtr;

typedef  QueueIterator<RetrainingResults>  RetrainingResultsListIterator;



class  SortOrderResults: public  RetrainingResultsList
{
public:
  SortOrderResults (SortOrderType  _sortOrder);
  SortOrderType    SortOrder ()  {return  sortOrder;}

private:
  SortOrderType  sortOrder;
};



typedef  SortOrderResults*  SortOrderResultsPtr;


typedef  KKQueue<SortOrderResults>  SortOrderResultsList;

typedef  QueueIterator<SortOrderResults>  SortOrderResultsListIterator;







RetrainingResults::RetrainingResults ():
  group               (0),
  numOfPasses         (0),
  accuracies          (NULL),
  numOfTrainingImages (0.0f),
  accuracyMean        (0.0f),
  accuracyMin         (0.0f),
  accuracyMax         (0.0f),
  accuracyStdDev      (0.0f),
  accuracyDelta       (0.0f),
  supportPointsMean   (0.0f),
  supportPointsMin    (0.0f),
  supportPointsMax    (0.0f),
  supportPointsStdDev (0.0f),
  trainingTimeMean    (0.0f),
  testingTimeMean     (0.0f),
  trainLibClassCounts (),
  classAccuracies     ()
{
}  



RetrainingResults::RetrainingResults (int           _group,
                                      int           _numOfPasses,
                                      float*        _accuracies,
                                      float         _numOfTrainingImages,
                                      float         _accuracyMean,
                                      float         _accuracyMin,
                                      float         _accuracyMax,
                                      float         _accuracyStdDev,
                                      float         _accuracyDelta,
                                      float         _supportPointsMean,
                                      float         _supportPointsMin,
                                      float         _supportPointsMax,
                                      float         _supportPointsStdDev,
                                      float         _trainingTimeMean,
                                      float         _testingTimeMean,
                                      VectorFloat&  _trainLibClassCounts,
                                      VectorFloat&  _classAccuracies
                                     ):

  group                (_group),
  numOfPasses          (_numOfPasses),
  accuracies           (_accuracies),
  numOfTrainingImages  (_numOfTrainingImages),
  accuracyMean         (_accuracyMean),
  accuracyMin          (_accuracyMin),
  accuracyMax          (_accuracyMax),
  accuracyStdDev       (_accuracyStdDev),
  accuracyDelta        (_accuracyDelta),
  supportPointsMean    (_supportPointsMean),
  supportPointsMin     (_supportPointsMin),
  supportPointsMax     (_supportPointsMax),
  supportPointsStdDev  (_supportPointsStdDev),
  trainingTimeMean     (_trainingTimeMean),
  testingTimeMean      (_testingTimeMean),
  trainLibClassCounts  (_trainLibClassCounts),
  classAccuracies      (_classAccuracies)
{
}



RetrainingResults::~RetrainingResults ()
{
  delete  accuracies;
}



KKStr  RetrainingResults::ToString ()
{
 KKStr  result (500);

 result << numOfTrainingImages  << "\t"     // Earlier versions only wrote out the 1st three
        << accuracyMean         << "\t"     // fields,  so to maintain compatability, we add
        << accuracyMin          << "\t"
        << accuracyMax          << "\t"
        << accuracyStdDev       << "\t"
        << accuracyDelta        << "\t"
        << supportPointsMean    << "\t"
        << supportPointsMin     << "\t"
        << supportPointsMax     << "\t"
        << supportPointsStdDev  << "\t"
        << trainingTimeMean     << "\t"
        << testingTimeMean      << "\t"
        << group                << "\t"
        << numOfPasses;

 if  (accuracies)
 {
   for  (int pass = 0;  pass < numOfPasses;  pass++)
   {
     result << "\t" << accuracies[pass];
   }
 }


 for  (uint x = 0;  x < classAccuracies.size ();  x++)
   result << "\t" << trainLibClassCounts[x];

 for  (uint x = 0;  x < classAccuracies.size ();  x++)
   result << "\t" << classAccuracies[x];


  return  result;
}  /* ToString */



SortOrderResults::SortOrderResults (SortOrderType  _sortOrder):
  RetrainingResultsList (true, 100),
  sortOrder             (_sortOrder)
{
}

  



SummaryByIPCandIPR::SummaryByIPCandIPR (RunLog&          _log,
                                        ImageClassList&  _imageClasses,
                                        KKStr           _subDirName
                                       ):
  allTotals           (),
  imageClasses        (_imageClasses),
  log                 (_log),
  subDirName          (_subDirName),
  results             (true, 10),
  baseResultsFileName ("ActiveLearningResults"),
  versionNum          (-1)
{
  if  (!subDirName.Empty ())
  {
    KKStr  w (subDirName);
    osAddLastSlash (w);
    w << baseResultsFileName;
    baseResultsFileName = w;
  }


// C:\users\kkramer\GradSchool\Plankton\ActiveLearning\Results\010-IPC\2003-12-03_AllOrders_010-IPC_50-IPR

  int x = subDirName.LocateLastOccurrence ('_');

  if  (x > 5)
  {
    KKStr leftSide = subDirName.SubStrPart (0, x - 1);
    KKStr rightSide = subDirName.SubStrPart (x + 1);
    x = rightSide.LocateCharacter ('-');
    KKStr  IPR = rightSide.SubStrPart (0, x - 1);
    imagesPerRetraining = atoi (IPR.Str ());

    // Now lets determine initialImagesPerClass
    x = leftSide.Len () - 1;
    //  We will scan from the end of the string until we find a Ubderscore(_)
    //  or path seperator (\).

    while  (x >= 0)
    {
      if  ((leftSide[x] == '_')  ||  (leftSide[x] == DSchar))
        break;
      x--;
    }

    if (x >= 0)
    {   
      rightSide = leftSide.SubStrPart (x + 1);
      leftSide  = leftSide.SubStrPart (0, x - 1);
    }
    else
    {
      rightSide = leftSide;
      leftSide = "";
    }
    x = rightSide.LocateCharacter ('-');
    if  (x < 0)
    {
      initialImagesPerClass = 0;
    }
    else
    {
      KKStr  IPC = rightSide.SubStrPart (0, x - 1);
      initialImagesPerClass = atoi (IPC.Str ());
    }
  }

  KKStr  summaryFileName (subDirName);
  osAddLastSlash (summaryFileName);
  summaryFileName << "ActiveLearningSummary_IPC-"
                  << StrFormatInt (InitialImagesPerClass (), "000")
                  << "_IPR-"
                  << StrFormatInt (ImagesPerRetraining   (), "000")
                  << ".data";
  
  bool validFormat = false;
  Load (summaryFileName, validFormat);
  if  (!validFormat)
  {
    LoadResultsFiles ();

    Save (summaryFileName);
  }
}  





SummaryByIPCandIPR::~SummaryByIPCandIPR ()
{
}






SortOrderResultsPtr    SummaryByIPCandIPR::GetSortOrderResults (SortOrderType  sortOrder)
{
  SortOrderResultsPtr  r = NULL;

  for  (int x = 0;  x < results.QueueSize ();  x++)
  {
    r = results.IdxToPtr (x);
    if  (r->SortOrder () == sortOrder)
      return  r;
  }

  return  NULL;
}  /* GetSortOrderResults */






TrainingImageListPtr  SummaryByIPCandIPR::GetImagesAddedToTrainingLibrary (SortOrderType  sortOrder,
                                                                           int            retraining
                                                                          )
{
  ClassGroupTotalsPtr  t = allTotals.LocateBySortOrder (sortOrder);
  return  t->TrainingImages (retraining);
}  /* GetImagesAddedToTrainingLibrary */




TrainingImageListPtr  SummaryByIPCandIPR::GetImagesAddedToTrainingLibrary (SortOrderType  sortOrder,
                                                                           int            retraining,
                                                                           int            randomPass
                                                                          )
{
  SortOrderResultsPtr  r = GetSortOrderResults (sortOrder);
  if  (!r)
  {
    return NULL;
  }

  TrainingImageListPtr  trainingImages = new TrainingImageList (false, 20);

  for  (ClassGroupTotalsList::iterator i = allTotals.begin (); i != allTotals.end ();  i++)
  {
    ClassGroupTotalsPtr  t = *i;

    const
    TrainingImageList&  trainingImagesThisClassGroup = t->TrainingImages (retraining, randomPass);

    int  x;
    for  (x = 0;  x < trainingImagesThisClassGroup.QueueSize ();  x++)
    {
      TrainingImagePtr  tip = trainingImagesThisClassGroup.IdxToPtr (x);
      trainingImages->PushOnBack (tip);
    }
  }
  return  trainingImages;
}  /* GetImagesAddedToTrainingLibrary */








void  SummaryByIPCandIPR::TryLoadingResultsForASpecificSortOrder (SortOrderType  sortOrder)
{
  log.Level (10) << "SummaryByIPCandIPR::TryLoadingResultsForASpecificSortOrder["
                 << SortOrderDescr (sortOrder) 
                 << "]"  
                 << endl;

  ClassGroupTotalsPtr  totals =
    ClassGroupTotals::ConsolidateOneDirectoryOneSortOrder (subDirName, 
                                                           imageClasses,
                                                           sortOrder,
                                                           log
                                                          );
 
  if  (!totals)
  {
    log.Level (10) << "TryLoadingResultsForASpecificSortOrder, no Totals found for [" 
                   <<  SortOrderDescr (sortOrder) 
                   << "]." 
                   << endl;
  }
  else
  {
    log.Level (10) << "Done loading all Files into 'ClassGroupTotals' object." << endl;

    SortOrderResultsPtr  sortOrderResults = new SortOrderResults (sortOrder);

    int  imagesInTrainingLibrary = initialImagesPerClass * imageClasses.QueueSize ();

    int  numOfPasses = totals->NumOfRandomPasses ();

    VectorFloat  trainLibSizeByClasses (imageClasses.QueueSize (), 0);

    for  (int retraining = 0;  retraining < totals->NumOfRetrainings ();  retraining++)
    {
      float  mean, variance, stdDev, min, max, accuracyDelta;

      float  spMean, spVariance, spStdDev, spMin, spMax;

      float  trainingTimeMean;
      float  testingTimeMean;

      totals->CalcRetrainingAccuracyStats (retraining, mean, variance, stdDev, min, max, accuracyDelta);

      totals->CalcRetrainingSupportPointStats (retraining, spMean, spVariance, spStdDev, spMin, spMax, 
                                               trainingTimeMean,
                                               testingTimeMean
                                              );

      float*  accuracies = NULL;
      if  (numOfPasses > 0)
      {
        accuracies = new float[numOfPasses];   // RetrainingResults will take ownership of this memory.
        for  (int pass = 0;  pass < numOfPasses;  pass++)
          accuracies[pass] = totals->AccuracyForRetrainingAndPass (retraining, pass);
      }

      VectorFloat  newTrainLibImagesClassCounts  = totals->GetNewTrainingImagesThisRetraining (retraining);
      for  (int classIDX = 0;  classIDX < imageClasses.QueueSize ();  classIDX++)
        trainLibSizeByClasses[classIDX] += newTrainLibImagesClassCounts[classIDX];

      VectorFloat  classAccuracies = totals->CalcAccuraciesByClass (retraining);

      RetrainingResultsPtr  r = new RetrainingResults (retraining,
                                                       numOfPasses,
                                                       accuracies,
                                                       (float)imagesInTrainingLibrary, 
                                                       mean,   min,   max,    stdDev, accuracyDelta,
                                                       spMean, spMin, spMax,  spStdDev,
                                                       trainingTimeMean,
                                                       testingTimeMean,
                                                       trainLibSizeByClasses,
                                                       classAccuracies
                                                      );
      sortOrderResults->PushOnBack (r);

      imagesInTrainingLibrary += imagesPerRetraining;
    }

    results.PushOnBack (sortOrderResults);
  	
    allTotals.PushOnBack (totals);
  }
}  /* TryLoadingResultsForASpecificSortOrder */





void   SummaryByIPCandIPR::LoadResultsFiles ()
{
  TryLoadingResultsForASpecificSortOrder (MixedRandomProbability);
  TryLoadingResultsForASpecificSortOrder (ProbabilityOrder);
  TryLoadingResultsForASpecificSortOrder (BreakTieOrder);
  TryLoadingResultsForASpecificSortOrder (ProbabilityLittleRandom);
  TryLoadingResultsForASpecificSortOrder (RandomOrder);
}  /* LoadResultsFiles */





void  SummaryByIPCandIPR::PrintReport (ostream&  report)
{
  log.Level (10) << "SummaryByIPCandIPR::PrintReport  Start" << endl;

  
  // Lets Load Results File

  report << endl
         << endl
         << "****      Multi Class Results     ****" << endl
         << endl;

  SortOrderResultsListIterator  soIDX (results);

  SortOrderResultsPtr  sortOrderResults = NULL;

  int  numOfRetrainings = 0;

  for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();    ++soIDX)
  {
    report << SortOrderDescr (sortOrderResults->SortOrder ())  << "\t\t\t\t\t\t\t\t\t";
    numOfRetrainings = Max (numOfRetrainings, sortOrderResults->QueueSize ());
  }
  report << endl;
  
  for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();    ++soIDX)
  {
    report << "Image_Count"  << "\t" 
           << "AccMean"      << "\t" 
           << "AccMin"       << "\t" 
           << "AccMax"       << "\t" 
           << "AccStdDev"    << "\t" 
           << "AccDelta"     << "\t" 
           << "SPmean"       << "\t"
           << "SPmin"        << "\t"
           << "SPmax"        << "\t"
           << "SPstdDev"     << "\t"
           << "TrainTime"    << "\t"
           << "TestTime";
  }
  report << endl;


  for  (int retraining = 0;  retraining < numOfRetrainings;  retraining++)
  {
    for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();    ++soIDX)
    {
      if  (retraining >= sortOrderResults->QueueSize ())
      {
        report << "\t\t\t\t\t\t\t\t\t\t\t\t";
      }
      else
      {
        RetrainingResultsPtr  retrainingResults = sortOrderResults->IdxToPtr (retraining);
        report << retrainingResults->NumOfTrainingImages ()                                         << "\t" 
               << StrFormatDouble (retrainingResults->AccuracyMean        (), "zzz,zz0.00")  << "%" << "\t"
               << StrFormatDouble (retrainingResults->AccuracyMin         (), "zzz,zz0.00")  << "%" << "\t"
               << StrFormatDouble (retrainingResults->AccuracyMax         (), "zzz,zz0.00")  << "%" << "\t"
               << StrFormatDouble (retrainingResults->AccuracyStdDev      (), "zzz,zz0.00")  << "%" << "\t"
               << StrFormatDouble (retrainingResults->AccuracyDelta       (), "z,zz0.0000")  << "%" << "\t"
               << StrFormatDouble (retrainingResults->SupportPointsMean   (), "zzz,zz0.00")         << "\t"
               << StrFormatDouble (retrainingResults->SupportPointsMin    (), "zzz,zz0.00")         << "\t"
               << StrFormatDouble (retrainingResults->SupportPointsMax    (), "zzz,zz0.00")         << "\t"
               << StrFormatDouble (retrainingResults->SupportPointsStdDev (), "zzz,zz0.00")         << "\t"
               << StrFormatDouble (retrainingResults->TrainingTimeMean    (), "zzz,zz0.00")         << "\t"
               << StrFormatDouble (retrainingResults->TestingTimeMean     (), "zzz,zz0.00")         << "\t";
      }
    }

    report << endl;
  }


  log.Level (10) << "SummaryByIPCandIPR::PrintReport  Done" << endl;
} /* PrintReport */





void  SummaryByIPCandIPR::PrintReportWithIncrements (ostream&  report,
                                                     int       increment
                                                    )
{
  log.Level (10) << "SummaryByIPCandIPR::PrintReportWithIncrements  Start" << endl;

  
  // Lets Load Results File

  report << endl
         << endl
         << "****      Multi Class Results   Imcrement[" << increment << "]  ****" << endl
         << endl;



  int**  imageCounts    = new int*[results.QueueSize ()];
  int*   numImageCounts = new int[results.QueueSize ()];
  int    maxImageCount = 0;

  SortOrderResultsListIterator  soIDX (results);
  SortOrderResultsPtr  sortOrderResults = NULL;
  int  numOfRetrainings = 0;
  int  soCount          = 0;
  for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();    ++soIDX)
  {
    report << SortOrderDescr (sortOrderResults->SortOrder ())  << "\t\t\t\t\t\t\t\t\t\t\t\t\t";
    numOfRetrainings = Max (numOfRetrainings, sortOrderResults->QueueSize ());
    GetImageCountsByIncrements (sortOrderResults->SortOrder (), increment, numImageCounts[soCount], imageCounts[soCount]);
    maxImageCount = Max (maxImageCount, numImageCounts[soCount]);
    soCount++;
  }
  report << endl;
  
  for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();    ++soIDX)
  {
    report << "Count"        << "\t" 
           << "AccMean"      << "\t"
           << "AccMin"       << "\t"
           << "AccMax"       << "\t"
           << "AccStdDev"    << "\t"
           << "AccDelta"     << "\t"
           << "SPsMean"      << "\t"
           << "SPsMin"       << "\t"
           << "SPsMax"       << "\t"
           << "SPsStfDev"    << "\t"
           << "TrainTime"    << "\t"
           << "TestTime"     << "\t"
           <<                   "\t";
  }
  report << endl;

  for  (int count = 0;  count < maxImageCount;  count++)
  {
    soCount = 0;
    for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();    ++soIDX)
    {
      if  (count >= numImageCounts[soCount])
      {
        report << "\t\t\t\t\t\t\t\t\t\t\t";
      }
      else
      {
        float  supportPointsMean;
        float  supportPointsMin;
        float  supportPointsMax;
        float  supportPointsStdDev;
        float  supportPointsStdErr;
        float  accuracyMean;
        float  accuracyMin;
        float  accuracyMax;
        float  accuracyStdDev;
        float  accuracyStdErr;
        float  accurcayDelta;
        bool   found;

        float  trainingTimeMean;
        float  testingTimeMean;

        GetStatsForNumOfImages (sortOrderResults->SortOrder (), 
                                imageCounts[soCount][count], 
                                accuracyMean, 
                                accuracyMin, 
                                accuracyMax,
                                accuracyStdDev,
                                accuracyStdErr,
                                accurcayDelta,
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
          report << "\t\t\t\t\t\t\t\t\t\t\t\t\t";
        }
        else
        {
          report << imageCounts[soCount][count]                                 << "\t" 
                 << StrFormatDouble (100 * accuracyMean,   "zzz,zz0.00") << "%" << "\t"
                 << StrFormatDouble (100 * accuracyMin,    "zzz,zz0.00") << "%" << "\t"
                 << StrFormatDouble (100 * accuracyMax,    "zzz,zz0.00") << "%" << "\t"
                 << StrFormatDouble (100 * accuracyStdDev, "zzz,zz0.00") << "%" << "\t"
                 << StrFormatDouble (100 * accurcayDelta,  "zz,zz0.000") << "%" << "\t"
                 << StrFormatDouble (supportPointsMean,    "zzz,zz0.0")         << "\t"
                 << StrFormatDouble (supportPointsMin,     "zzz,zz0.0")         << "\t"
                 << StrFormatDouble (supportPointsMax,     "zzz,zz0.0")         << "\t"
                 << StrFormatDouble (supportPointsStdDev,  "zzz,zz0.00")        << "\t"
                 << StrFormatDouble (trainingTimeMean,     "zzz,zz0.00")        << "\t"
                 << StrFormatDouble (testingTimeMean,      "zzz,zz0.00")        << "\t"
                 << "\t";
        }
      }

      soCount++;
    }
    report << endl;
  }

  {
    // Lets clean up dynamicly allocated memory.
    for  (soCount = 0;  soCount < results.QueueSize ();  soCount++)
      delete  imageCounts[soCount];

    delete  imageCounts;
    delete  numImageCounts;
  }

  log.Level (10) << "SummaryByIPCandIPR::PrintReportWithIncrements  Done" << endl;
} /* PrintReportWithIncrements */








void  SummaryByIPCandIPR::PrintReportByIncrementsAndPaiedTTest (ostream&  report,
                                                                int       increment,
                                                                int       countLimit
                                                               )
{
  log.Level (10) << "SummaryByIPCandIPR::PrintReportByIncrementsAndPaiedTTest  Start" << endl;

  report << setprecision (7);

  report << "IIPC" << "\t" << InitialImagesPerClass ()     << endl;
  report << "IPR"  << "\t" << ImagesPerRetraining ()       << endl; 
  report << endl;

  int**  imageCounts    = new int*[results.QueueSize ()];
  int*   numImageCounts = new int[results.QueueSize ()];
  int    maxImageCount = 0;

  int  x,y;
  // Build Header Lines.

  int  numOfRetrainings = 0;
  int  soCount          = 0;

  KKStr h1 (300);
  KKStr h2 (300);

  h1 << "Image";
  h2 << "Count";

  SortOrderResultsListIterator  soIDX (results);
  SortOrderResultsPtr  sortOrderResults = NULL;

  for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();  ++soIDX)
  {
    h1 << "\t" << SortOrderDescr (sortOrderResults->SortOrder ())
       << "\t"
       << "\t"
       << "\t"
       << "\t";

    h2 << "\t" << "AccMean" 
       << "\t" << "AcStdDev" 
       << "\t" << "AccStdErr"
       << "\t" << "AccDelta"
       << "\t";

    numOfRetrainings = Max (numOfRetrainings, sortOrderResults->QueueSize ());
    GetImageCountsByIncrements (sortOrderResults->SortOrder (), increment, numImageCounts[soCount], imageCounts[soCount]);
    maxImageCount = Max (maxImageCount, numImageCounts[soCount]);
    soCount++;
  }

  // Lets Add Paired Test Titles to header.
  for  (x = 0;  x < (results.QueueSize () - 1); x++)
  {
    SortOrderResultsPtr s1 = results.IdxToPtr (x);
    for  (y = x + 1;  y < results.QueueSize ();  y++)
    {
      SortOrderResultsPtr s2 = results.IdxToPtr (y);
      h1 << "\t" << SortOrderDescr (s1->SortOrder ());
      h2 << "\t" << SortOrderDescr (s2->SortOrder ());
    }
  }

  report << h1 << endl;
  report << h2 << endl;

  for  (int count = 0;  count < maxImageCount;  count++)
  {
    soCount = 0;
    int  consistantImageCount = imageCounts[soCount][count];


    RetrainingResultsPtr*  retranings = new RetrainingResultsPtr[results.QueueSize ()];

    report << consistantImageCount;

    for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();    ++soIDX)
    {
      if  (count >= numImageCounts[soCount])
      {
        consistantImageCount = -1;
        report << "\t\t\t\t\t";
      }
      else
      {
        if  (imageCounts[soCount][count] != consistantImageCount)
          consistantImageCount = -1;

        RetrainingResultsPtr  r = GetRetrainingResults (sortOrderResults->SortOrder (), imageCounts[soCount][count]);
        retranings[soCount] = r;

        if  (!r)
        {
          report << "\t\t\t\t\t";
          consistantImageCount = -1;
        }
        else
        {
          float  stdErr = 0.0f;
          if  (r->NumOfPasses () > 0)
          {
            stdErr = r->AccuracyStdDev () / sqrt ((float)(r->NumOfPasses ()));
          }

          report << "\t" << r->AccuracyMean ()
                 << "\t" << r->AccuracyStdDev ()
                 << "\t" << stdErr
                 << "\t" << r->AccuracyDelta ()
                 << "\t";
        }
      }
      soCount++;
    }

    if  (consistantImageCount < 1)
    {
      // The image Counts were not consistant accross the line so no point printing a paired test.
    }
    else
    {
      // We cam now add the T-Test results.
      soCount = 0;
      for  (x = 0;  x < (results.QueueSize () - 1); x++)
      {
        for  (y = x + 1;  y < results.QueueSize ();  y++)
        {
          float  tValue = 0.0f;
          CalcPairedTTest (retranings[x], retranings[y], consistantImageCount, tValue);
          report << "\t" << tValue;
        }
      }
    }

    report << endl;

    delete  retranings;

    if  (consistantImageCount >= countLimit)
    {
      // We have printed as many rows as thecaller has asked for.
      break;
    }
  }

  for  (soCount = 0;  soCount < results.QueueSize ();  soCount++)
    delete  imageCounts[soCount];

  delete  imageCounts;
  delete  numImageCounts;

  log.Level (10) << "SummaryByIPCandIPR::PrintReportByIncrementsAndPaiedTTest  Done" << endl;
} /* PrintReportByIncrementsAndPaiedTTest */







void  SummaryByIPCandIPR::PrintReportSupportPointsUsingSampling (ostream&  report,
                                                                 int       sampleRate,
                                                                 int       countLimit
                                                                )
{
  log.Level (10) << "SummaryByIPCandIPR::PrintReportSupportPointsUsingSampling  Start" << endl;

  report << setprecision (7);

  report << "IIPC" << "\t" << InitialImagesPerClass ()     << endl;
  report << "IPR"  << "\t" << this->ImagesPerRetraining () << endl; 
  report << endl;

  int**  imageCounts    = new int*[results.QueueSize ()];
  int*   numImageCounts = new int[results.QueueSize ()];
  int    maxImageCount = 0;

  int  x,y;
  // Build Header Lines.

  int  numOfRetrainings = 0;
  int  soCount          = 0;

  KKStr h1 (300);
  KKStr h2 (300);

  h1 << "Image";
  h2 << "Count";

  SortOrderResultsListIterator  soIDX (results);
  SortOrderResultsPtr  sortOrderResults = NULL;

  for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();  ++soIDX)
  {
    h1 << "\t" << SortOrderDescr (sortOrderResults->SortOrder ())
       << "\t"
       << "\t"
       << "\t";

    h2 << "\t" << "SupportMean" 
       << "\t" << "SupportStdDev" 
       << "\t" << "SupportStdErr"
       << "\t";

    numOfRetrainings = Max (numOfRetrainings, sortOrderResults->QueueSize ());
    GetImageCountsByIncrements (sortOrderResults->SortOrder (), sampleRate, numImageCounts[soCount], imageCounts[soCount]);
    maxImageCount = Max (maxImageCount, numImageCounts[soCount]);
    soCount++;
  }

  // Lets Add Paired Test Titles to header.
  for  (x = 0;  x < (results.QueueSize () - 1); x++)
  {
    SortOrderResultsPtr s1 = results.IdxToPtr (x);
    for  (y = x + 1;  y < results.QueueSize ();  y++)
    {
      SortOrderResultsPtr s2 = results.IdxToPtr (y);
      h1 << "\t" << SortOrderDescr (s1->SortOrder ());
      h2 << "\t" << SortOrderDescr (s2->SortOrder ());
    }
  }

  report << h1 << endl;
  report << h2 << endl;

  for  (int count = 0;  count < maxImageCount;  count++)
  {
    soCount = 0;
    int  consistantImageCount = imageCounts[soCount][count];


    RetrainingResultsPtr*  retranings = new RetrainingResultsPtr[results.QueueSize ()];

    report << consistantImageCount;

    for  (soIDX.Reset ();  sortOrderResults = soIDX.CurPtr ();    ++soIDX)
    {
      if  (count >= numImageCounts[soCount])
      {
        consistantImageCount = -1;
        report << "\t\t\t\t";
      }
      else
      {
        if  (imageCounts[soCount][count] != consistantImageCount)
          consistantImageCount = -1;

        RetrainingResultsPtr  r = GetRetrainingResults (sortOrderResults->SortOrder (), imageCounts[soCount][count]);
        retranings[soCount] = r;

        if  (!r)
        {
          report << "\t\t\t\t";
          consistantImageCount = -1;
        }
        else
        {
          float  stdErr = 0.0f;
          if  (r->NumOfPasses () > 0)
          {
            stdErr = r->SupportPointsStdDev () / sqrt ((float)(r->NumOfPasses ()));
          }

          report << "\t" << r->SupportPointsMean ()
                 << "\t" << r->SupportPointsStdDev ()
                 << "\t" << stdErr
                 << "\t";
        }
      }
      soCount++;
    }

    if  (consistantImageCount < 1)
    {
      // The image Counts were not consistant accross the line so no point printing a paired test.
    }
    else
    {
      // We cam now add the T-Test results.
      soCount = 0;
      for  (x = 0;  x < (results.QueueSize () - 1); x++)
      {
        for  (y = x + 1;  y < results.QueueSize ();  y++)
        {
          float  tValue = 0.0f;
          CalcPairedTTest (retranings[x], retranings[y], consistantImageCount, tValue);
          report << "\t" << tValue;
        }
      }
    }

    report << endl;

    delete  retranings;

    if  (consistantImageCount >= countLimit)
    {
      // We have printed as many rows as thecaller has asked for.
      break;
    }
  }

  for  (soCount = 0;  soCount < results.QueueSize ();  soCount++)
    delete  imageCounts[soCount];

  delete  imageCounts;
  delete  numImageCounts;

  log.Level (10) << "SummaryByIPCandIPR::PrintReportSupportPointsUsingSampling  Done" << endl;
} /* PrintReportSupportPointsUsingSampling */







void  SummaryByIPCandIPR::PrintReportByClass (ostream&       report,
                                              SortOrderType  sortOrder,
                                              int            increment
                                             )
{
  log.Level (10) << endl;
  log.Level (10) << "SummaryByIPCandIPR::PrintReportByClass  Start" << endl;
  log.Level (10) << "SortOrder[" << SortOrderDescr (sortOrder) << "]  Increment[" << increment << "]" << endl;

  
  // Lets Load Results File

  report << endl
         << endl
         << "****      Multi Class Results By Class,   Sort Order["  << SortOrderDescr (sortOrder) 
         << "]   Imcrement[" << increment << "]    ****" << endl
         << endl;

  SortOrderResultsPtr  soResults = GetSortOrderResults (sortOrder);
  
  if  (soResults == NULL)
  {
    report << endl
           << "****  NO RESULTS FOR SORT ORDER[" << SortOrderDescr (sortOrder) << "]  ****" << endl
           << endl;
    return;
  }

  int*  imageCounts   = NULL;
  int   numImageCounts;

  // Build Header Lines.

  int  numOfRetrainings = 0;
  int  soCount          = 0;

  KKStr h1 (300);
  KKStr h2 (300);

  h1 << "Image";
  h2 << "Count"  << "\t" << "Accuracy";


  ImageClassListIterator  cIDX (imageClasses);
  ImageClassPtr  imageClass = NULL;

  numOfRetrainings = soResults->QueueSize ();
  GetImageCountsByIncrements (sortOrder, increment, numImageCounts, imageCounts);
  int  maxImageCount = numImageCounts;


  for  (cIDX.Reset ();  imageClass = cIDX.CurPtr ();  ++cIDX)
  {
    h2 << "\t" << imageClass->Name () << "\t" << "Count";
  }

  report << h1 << endl;
  report << h2 << endl;

  for  (int count = 0;  count < maxImageCount;  count++)
  {
    int  consistantImageCount = imageCounts[count];
    RetrainingResultsPtr  r = GetRetrainingResults (sortOrder, imageCounts[count]);

    report << imageCounts[count] << "\t"
           << r->AccuracyMean ();
    

    int  classIDX = 0;
    for  (classIDX = 0;  classIDX <  imageClasses.QueueSize ();  classIDX++)
    {
      report << "\t" << r->AccurcayByClass (classIDX)
             << "\t" << r->TrainLibClassCount (classIDX);
    }

    report << endl;
  }

  delete  imageCounts;

  log.Level (10) << "PrintReportByClass::PrintReportByIncrementsAndPaiedTTest  Done" << endl;

}  /* PrintReportByClass */












void  SummaryByIPCandIPR::Save (KKStr  fileName)
{
  ofstream f (fileName.Str ());

  if  (!f.is_open ())
  {
    log.Level (-1) << endl
                   << "SummaryByIPCandIPR::Save        *** ERROR ***" << endl
                   << endl
                   << "                      Could noty open File[" << fileName << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  int x;

  f << setprecision (7);
  f << "\\SummaryByIPCandIPR"            << "\t" << "Version"  << "\t" << VersionNum << endl;
  f << "\\InitialTrainingImagesPerClass" << "\t" << initialImagesPerClass << endl;
  f << "\\ImagesPerRetraining"           << "\t" << imagesPerRetraining   << endl;
  
  f << "\\ImageClassList";
  for  (x = 0;  x < imageClasses.QueueSize ();  x++)
    f << "\t" << imageClasses.IdxToPtr (x)->Name ();
  f << endl;


  SortOrderResultsListIterator  soIDX (results);
  SortOrderResultsPtr  sortOrderResults = NULL;
  for  (soIDX.Reset ();   sortOrderResults = soIDX.CurPtr ();    ++soIDX)
  {
    f << "\\SortOrder"         << "\t" << SortOrderDescr (sortOrderResults->SortOrder ()) << endl;
    f << "\\NumOfRetrainings"  << "\t" << sortOrderResults->QueueSize ()                  << endl;
    f << "\\Fields"            << "\t" << "NumOfTrainingImages" 
                               << "\t" << "AccuracyMean"
                               << "\t" << "AccuracyMin"
                               << "\t" << "AccuracyMax"
                               << "\t" << "AccuracyStdDev"
                               << "\t" << "AccuracyDelta"
                               << "\t" << "SupportPointsMean"
                               << "\t" << "SupportPointsMin"
                               << "\t" << "SupportPointsMax"
                               << "\t" << "SupportPointsStdDev"
                               << "\t" << "TrainingTimeMean"
                               << "\t" << "TestingTimeMean"
                               << endl;

    RetrainingResultsListIterator rIDX (*sortOrderResults);
    RetrainingResultsPtr  retraining = NULL;

    for  (rIDX.Reset ();  retraining = rIDX.CurPtr ();  ++rIDX)
    {
      f << retraining->NumOfTrainingImages  () << "\t" 
        << retraining->AccuracyMean         () << "\t"
        << retraining->AccuracyMin          () << "\t"
        << retraining->AccuracyMax          () << "\t"
        << retraining->AccuracyStdDev       () << "\t"
        << retraining->AccuracyDelta        () << "\t"
        << retraining->SupportPointsMean    () << "\t"
        << retraining->SupportPointsMin     () << "\t"
        << retraining->SupportPointsMax     () << "\t"
        << retraining->SupportPointsStdDev  () << "\t"
        << retraining->TrainingTimeMean     () << "\t"
        << retraining->TestingTimeMean      () << "\t"
        << retraining->Group                () << "\t"
        << retraining->NumOfPasses          ();

      const float*  accuracies = retraining->Accuracies ();
      if  ((accuracies)  &&  (retraining->NumOfPasses () > 0))
      {
        for  (int pass = 0;  pass < retraining->NumOfPasses ();  pass++)
          f << "\t" << accuracies[pass];
      }

      const VectorFloat  trainLibClassCounts = retraining->TrainLibClassCounts ();
      for  (uint x = 0;  x < trainLibClassCounts.size ();  x++)
        f << "\t" << trainLibClassCounts[x];

      const VectorFloat&  classAccuracies = retraining->ClassAccuracies ();
      for  (uint x = 0;  x < classAccuracies.size ();  x++)
        f << "\t" << classAccuracies[x];

      f << endl;
    }
  }

  f.close ();
}  /* Save */




void  SummaryByIPCandIPR::Load (KKStr   fileName,
                                bool&    validFormat
                               )
{
  versionNum = -1;

  log.Level (10) << "SummaryByIPCandIPR::Load  fileName[" << fileName << "]." << endl;

  validFormat = false;

  FILE*  f = osFOPEN (fileName.Str (), "r");

  if  (!f)
  {
    log.Level (-1) << endl 
                   << endl;
    log.Level (-1) << endl
                   << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                   << endl
                   << "                      Could not open File[" << fileName << "]" << endl
                   << endl;
    return;
  }

  char  buff[10240];

  bool  fileParmsLoaded = false;

  {
    // Get Frst Row wich should be InitialTrainingImagesPerClass 
    if  (!fgets (buff, sizeof (buff), f))
    {
      log.Level (-1) << endl
                     << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "                      *** No Data ***" << endl
                     << endl;
      fclose (f);
      return;
    }

    KKStr l (buff);
    l.Upper ();

    KKStr  field = l.ExtractToken ("\n\t\r ");
    if  (field != "\\SUMMARYBYIPCANDIPR")
    {
      log.Level (-1) << endl 
                     << endl;
      log.Level (-1) << endl
                     << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "  First Liine Not Valid[" << l << "]" << endl
                     << endl;
      fclose (f);
      return;
    }


    KKStr  fieldName = l.ExtractToken ("\n\r\t ");
    while  (!fieldName.Empty ())
    {
      fieldName.Upper ();

      if  (fieldName == "VERSION")
      {
        versionNum = l.ExtractTokenInt ("\n\r\t ");
      }

      fieldName = l.ExtractToken ("\n\r\t ");
    }

    if  (versionNum  <  VersionNum)
    {
      log.Level (-1) << endl 
                     << endl;
      log.Level (-1) << endl
                     << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "  VersionNum["  << versionNum << "] has beem Replaced." << endl
                     << endl;
      fclose (f);
      return;
    }
    
    // Later on we can go after Version Info
  }



  {
    // Get Second Row which should be InitialTrainingImagesPerClass 
    if  (!fgets (buff, sizeof (buff), f))
    {
      log.Level (-1) << endl
                     << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "                      *** No Data ***" << endl
                     << endl;
      fclose (f);
      return;
    }

    KKStr l (buff);
    l.Upper ();

    KKStr  field = l.ExtractToken ("\n\t\r ");
    if  (field != "\\INITIALTRAININGIMAGESPERCLASS")
    {
      log.Level (-1) << endl 
                     << endl;
      log.Level (-1) << endl
                     << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "  First Liine Not Valid[" << l << "]" << endl
                     << endl;
      fclose (f);
      return;
    }
    
    initialImagesPerClass = l.ExtractTokenInt ("\n\t\r ");
  }

  {
    // Get Third Row which should be ImagesPerRetraining
    if  (!fgets (buff, sizeof (buff), f))
    {
      log.Level (-1) << endl 
                     << endl;
      log.Level (-1) << endl
                     << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "              *** Missing Second Line ***" << endl
                     << endl;
      fclose (f);
      return;
    }
    
    KKStr l (buff);
    l.Upper ();

    KKStr  field = l.ExtractToken ("\n\t\r ");
    if  (field != "\\IMAGESPERRETRAINING")
    {
      log.Level (-1) << endl
                     << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "  Second Liine Not Valid[" << l << "]" << endl
                     << endl;
      fclose (f);
      return;
    }
    
    imagesPerRetraining = l.ExtractTokenInt ("\n\t\r ");
  }




  {
    // Get Forth Row,  Class List

    if  (!fgets (buff, sizeof (buff), f))
    {
      log.Level (-1) << endl
                     << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "                      *** No Data ***" << endl
                     << endl;
      fclose (f);
      return;
    }

    KKStr l (buff);

    KKStr  field = l.ExtractToken ("\n\t\r ");
    field.Upper ();
    if  (field != "\\IMAGECLASSLIST")
    {
      log.Level (-1) << endl 
                     << endl;
      log.Level (-1) << endl
                     << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "  Fifth Line Not Valid[" << l << "]   Should be Class List" << endl
                     << endl;
      fclose (f);
      return;
    }
    
    // Build Class List

    ImageClassList  newClassList (log);

    KKStr  className = l.ExtractToken ("\n\r\t");
    while  (!className.Empty ())
    {
      ImageClassPtr  imageClass = imageClasses.LookUpByName (className);
      if  (imageClass)
      {
        imageClasses.DeleteEntry (imageClass);
      }
      else
      {
        imageClass = ImageClass::CreateNewImageClass (className, log);
      }

      newClassList.PushOnBack (imageClass);
      className = l.ExtractToken ("\n\r\t");
    }


    // we now have Image Class List in Correct Order,  but in wrong varabble.

    while  (imageClasses.QueueSize ())
    {
      ImageClassPtr  imageClass = imageClasses.PopFromBack ();
    }

    for  (int x = 0;  x < newClassList.QueueSize ();  x++)
    {
      ImageClassPtr  imageClass = newClassList.IdxToPtr (x);
      imageClasses.PushOnBack (imageClass);
    }
  }



  SortOrderResultsPtr  curSortOrderResults = NULL;

  while  (fgets (buff, sizeof (buff), f))
  {
    KKStr l (buff);

    if  (l.SubStrPart (0, 0) == "\\")
    {
      if  (curSortOrderResults)
      {
        results.PushOnBack (curSortOrderResults);
        curSortOrderResults = NULL;
      }

      SortOrderType  nextSortOrder = NULL_SortOrder;
      int            numOfRetrainings = 0;

      while  (l.SubStrPart (0, 0) == "\\")
      {
        l = l.SubStrPart (1);
        l.Upper ();

        KKStr  fieldName = l.ExtractToken ("\n\t\r ");
        if  (fieldName == "SORTORDER")
        {
          KKStr  sortOrderDesc = l.ExtractToken ("\n\t\r ");
          nextSortOrder = SortOrderTypeFromStr (sortOrderDesc);
        }
        else if  (fieldName == "NUMOFRETRAININGS")
        {
          numOfRetrainings = l.ExtractTokenInt ("\n\t\r ");
        }

        if  (fgets (buff, sizeof (buff), f))
          l = buff;
        else
          l = "";
      }
      
      if  (nextSortOrder == NULL_SortOrder)
      {
        log.Level (-1) << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                       << endl
                       << "   Invalid Sort Order Defined." << endl
                       << endl;
        fclose (f);
        return;
      }

      curSortOrderResults = new SortOrderResults (nextSortOrder);
    }

    if  (!curSortOrderResults)
    {
      log.Level (-1) << endl 
                     << endl;
      log.Level (-1) << "SummaryByIPCandIPR::Load        *** ERROR ***" << endl
                     << endl
                     << "   Invalid Sort Order Defined." << endl
                     << endl;
      fclose (f);
      return;
    }

    float numOfTrainingImages  = (float)l.ExtractTokenDouble ("\n\t\r ");
    float accuracyMean         = (float)l.ExtractTokenDouble ("\n\t\r ");
    float accuracyMin          = (float)l.ExtractTokenDouble ("\n\t\r ");
    float accuracyMax          = (float)l.ExtractTokenDouble ("\n\t\r ");
    float accuracyStdDev       = (float)l.ExtractTokenDouble ("\n\t\r ");
    float accuracyDelta        = (float)l.ExtractTokenDouble ("\n\t\r ");  // Added With Vversion 
    float supportPointsMean    = (float)l.ExtractTokenDouble ("\n\t\r ");
    float supportPointsMin     = (float)l.ExtractTokenDouble ("\n\t\r ");
    float supportPointsMax     = (float)l.ExtractTokenDouble ("\n\t\r ");
    float supportPointsStdDev  = (float)l.ExtractTokenDouble ("\n\t\r ");
    float trainingTimeMean     = (float)l.ExtractTokenDouble ("\n\t\r ");
    float testingTimeMean      = (float)l.ExtractTokenDouble ("\n\t\r ");
    int   group                = l.ExtractTokenInt ("\n\t\r ");
    int   numOfPasses          = l.ExtractTokenInt ("\n\t\r ");

    float* accuracies = NULL;
    if  (numOfPasses > 0)
    {
      accuracies = new float[numOfPasses];
      for  (int pass = 0;  pass < numOfPasses;  pass++)
        accuracies[pass] = (float)l.ExtractTokenDouble ("\n\t\r ");
    }


    VectorFloat  trainLibClassCounts;
    for  (int classIDX = 0;  classIDX < imageClasses.QueueSize ();  classIDX++)
    {
      float   c = (float)l.ExtractTokenDouble ("\n\t\r ");
      trainLibClassCounts.push_back (c);
    }


    VectorFloat  classAccuracies;
    for  (int classIDX = 0;  classIDX < imageClasses.QueueSize ();  classIDX++)
    {
      float   a = (float)l.ExtractTokenDouble ("\n\t\r ");
      classAccuracies.push_back (a);
    }

    RetrainingResultsPtr  retrainingResults = new RetrainingResults (group,
                                                                     numOfPasses, 
                                                                     accuracies,
                                                                     numOfTrainingImages,
                                                                     accuracyMean,
                                                                     accuracyMin,
                                                                     accuracyMax,
                                                                     accuracyStdDev,
                                                                     accuracyDelta,
                                                                     supportPointsMean,
                                                                     supportPointsMin,
                                                                     supportPointsMax,
                                                                     supportPointsStdDev,
                                                                     trainingTimeMean,
                                                                     testingTimeMean,
                                                                     trainLibClassCounts,
                                                                     classAccuracies
                                                                    );
    curSortOrderResults->PushOnBack (retrainingResults);
  }

  if  (curSortOrderResults)
  {
    results.PushOnBack (curSortOrderResults);
    curSortOrderResults = NULL;
  }


  fclose (f);
  validFormat = true;

  log.Level (10) << "SummaryByIPCandIPR::Load  Exiting   validFormat[" << (validFormat ? "T" : "F") << "]." << endl;

  return;
}  /* Load */




RetrainingResultsListPtr  SummaryByIPCandIPR::GetRetrainingsForASortOrder (SortOrderType  sortOrder)
{
  SortOrderResultsListIterator  rIDX (results);
  SortOrderResultsPtr  sortOrderResults = NULL;
  for (rIDX.Reset ();  sortOrderResults = rIDX.CurPtr ();  ++rIDX)
  {
    if  (sortOrderResults->SortOrder () == sortOrder)
    {
      break;
    }
  }

  if  (!sortOrderResults)
    return NULL;

  return  sortOrderResults;
}  /* GetRetrainingsByImageIncrements */





void  SummaryByIPCandIPR::GetImageCountsByIncrements (SortOrderType  sortOrder,
                                                      int            increment,
                                                      int&           count,
                                                      int*&          imageCounts
                                                      )
{
  int x;

  RetrainingResultsListPtr retrainings = GetRetrainingsForASortOrder (sortOrder);

  if  (!retrainings)
  {
    count = 0;
    imageCounts = NULL;
    return;
  }

  if  (retrainings->QueueSize () < 1)
  {
    count = 0;
    imageCounts = NULL;
    return;
  }


  int*  counts = new int[retrainings->QueueSize ()];
  int   nextAllowableCount = 0;
  count = 0;

  RetrainingResultsPtr retraining = NULL;
  RetrainingResultsListIterator  rIDX (*retrainings);
  for  (rIDX.Reset ();  retraining = rIDX.CurPtr ();  ++rIDX)
  {
    if  (retraining->NumOfTrainingImages () >= nextAllowableCount)
    {
      counts[count] = (int)retraining->NumOfTrainingImages ();
      count++;
      nextAllowableCount = (int)retraining->NumOfTrainingImages () + increment;
    }
  }

  imageCounts = new int[count + 1];
  for (x = 0;  x < count;  x++)
    imageCounts[x] = counts[x];
  imageCounts[count] = -1;

  delete  counts;

  return;
}  /* GetImageCountsByIncrements */





void  SummaryByIPCandIPR::GetStatsForNumOfImages (SortOrderType  sortOrder, 
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
  found = false;

  SortOrderResultsListIterator  rIDX (results);
  SortOrderResultsPtr  sortOrderResults = NULL;
  for (rIDX.Reset ();  sortOrderResults = rIDX.CurPtr (); ++rIDX)
  {
    if  (sortOrderResults->SortOrder () == sortOrder)
    {
      break;
    }
  }

  if  (!sortOrderResults)
    return;

  RetrainingResultsPtr  retrainingResults = NULL;

  RetrainingResultsListIterator  retIDX (*sortOrderResults);

  for  (retIDX.Reset ();  retrainingResults = retIDX.CurPtr ();  ++retIDX)
  {
    if  (retrainingResults->NumOfTrainingImages () > numOfImages)
    {
      // There are no results for this numOfImages, so we will return with found = false;     
      return;
    }

    if  (retrainingResults->NumOfTrainingImages () == numOfImages)
      break;
  }

  if  (!retrainingResults)
    return;

  accuracyMean        = retrainingResults->AccuracyMean   ();
  accuracyMin         = retrainingResults->AccuracyMin    ();
  accuracyMax         = retrainingResults->AccuracyMax    ();
  accuracyStdDev      = retrainingResults->AccuracyStdDev ();
  accuracyDelta       = retrainingResults->AccuracyDelta  ();

  supportPointsMean   = retrainingResults->SupportPointsMean   ();
  supportPointsMin    = retrainingResults->SupportPointsMin    ();
  supportPointsMax    = retrainingResults->SupportPointsMax    ();
  supportPointsStdDev = retrainingResults->SupportPointsStdDev ();

  trainingTimeMean   = retrainingResults->TrainingTimeMean ();
  testingTimeMean    = retrainingResults->TestingTimeMean ();

  if  (retrainingResults->NumOfPasses () < 1)
  {
    accuracyStdErr      = 0.0f;
    supportPointsStdErr = 0.0f;
  }
  else
  {
    float numOfPassesSQRT = (float)sqrt ((float)retrainingResults->NumOfPasses ());
    accuracyStdErr      = accuracyStdDev      / numOfPassesSQRT;
    supportPointsStdErr = supportPointsStdDev / numOfPassesSQRT;
  }


  found = true;
  return;
}  /* GetStatsForNumOfImages */





void  SummaryByIPCandIPR::RecoverNewTrainingImages (SortOrderType  sortOrder, 
                                                    int            numOfRetraings,
                                                    const KKStr&  rootDir
                                                   )
{
  KKStr  dirDesc = "IPC-"  + StrFormatInt (InitialImagesPerClass (), "000")  +
                    "_IPR-" + StrFormatInt (ImagesPerRetraining   (), "000")  +
                    "_"     + SortOrderDescr (sortOrder);

  KKStr  destDir = osAddSlash (rootDir) + dirDesc;

  osCreateDirectoryPath (destDir);

  KKStr  reportFileName = osAddSlash (destDir) + "NewTrainingImages_" + dirDesc + ".txt";

  ofstream  r (reportFileName.Str ());
  r << "New Training Images added to Training Libraries" << endl 
    << endl;

  r << "IPC="            << InitialImagesPerClass () << endl;
  r << "IPR="            << ImagesPerRetraining   () << endl;
  r << "SortOrder="      << SortOrderDescr (sortOrder) << endl;
  r << endl;

  for  (int retraining = 0;  retraining < numOfRetraings;  retraining++)
  {
    TrainingImageListPtr addedImages = GetImagesAddedToTrainingLibrary (sortOrder, retraining);

    if  (addedImages)
    {
      for  (TrainingImageList::iterator  i = addedImages->begin ();  i != addedImages->end ();  i++)
      {
        TrainingImagePtr  ti = *i;

        KKStr  dirWhereFileIs = osLookForFile (ti->ImageFileName (), "C:\\TrainingApp\\TrainingImages");
        if  (dirWhereFileIs.Empty ())
        {
          dirWhereFileIs = osLookForFile (ti->ImageFileName (), "K:\\v1\\Plankton\\GrayLevelTrainingImages\\AllTrainingImages");
        }

        if  (dirWhereFileIs.Empty ())
        {
          cerr << "RecoverNewTrainingImages   *** ERROR ***   Could not find image[" << ti->ImageFileName () << "]" << endl;
          KKStr  flagFileName = osAddSlash (destDir) + osGetRootName (ti->ImageFileName ()) + ".txt";
          ofstream flagFile (flagFileName.Str ());
          flagFile << "*** ERROR ***   Could not find image[" << ti->ImageFileName () << "]" << endl;
          flagFile.close ();
          r << "*** ERROR ***   Could not find image[" << ti->ImageFileName () << "]" << endl;
        }
        else
        {
          KKStr  sourceFileName = osAddSlash (dirWhereFileIs) + ti->ImageFileName ();
          KKStr  destFileName   = osAddSlash (destDir) + 
                                   "Retraining" + StrFormatInt (retraining, "000") +
                                   "_" + osGetRootName (ti->ImageFileName ()) + ".bmp";

          bool copyGood = osCopyFile (sourceFileName, destFileName);
          r << "Retraining="          << retraining                 << "\t"
            << "Image="               << ti->ImageFileName ()       << "\t"
            << "Class="               << ti->ImageClass ()->Name () << "\t"
            << "Probability="         << ti->Probability ()         << "\t"
            << "NonStreamPlace="      << ti->NonStreamPlace ()      << "\t"
            << "NonStreamPercentile=" << ti->NonStreamPercentile () << endl;
        }
      }

      delete addedImages;
    }
  }

  r.close ();
}  /* RecoverNewTrainingImages */







RetrainingResultsPtr  SummaryByIPCandIPR::GetRetrainingResults (SortOrderType  sortOrder, 
                                                                int            numOfImages
                                                               )
{
  SortOrderResultsListIterator  rIDX (results);
  SortOrderResultsPtr  sortOrderResults = NULL;
  for (rIDX.Reset ();  sortOrderResults = rIDX.CurPtr (); ++rIDX)
  {
    if  (sortOrderResults->SortOrder () == sortOrder)
    {
      break;
    }
  }

  if  (!sortOrderResults)
    return NULL;

  RetrainingResultsPtr  retrainingResults = NULL;
  RetrainingResultsListIterator  retIDX (*sortOrderResults);
  for  (retIDX.Reset ();  retrainingResults = retIDX.CurPtr ();  ++retIDX)
  {
    if  (retrainingResults->NumOfTrainingImages () > numOfImages)
    {
      // Therer are no results for this numOfImages, so we will return with found = false;     
      return NULL;
    }

    if  (retrainingResults->NumOfTrainingImages () == numOfImages)
      break;
  }

  if  (!retrainingResults)
    return NULL;

  return  retrainingResults;
}  /* GetRetrainingResults */




float  Round (float  f)
{
  long  l = (long)(f * 10000 + 0.5f);
  f = (float)l / 10000;
  return  f;
}  /* Round */




void  SummaryByIPCandIPR::CalcPairedTTest (RetrainingResultsPtr  so1RetrainingResults, 
                                           RetrainingResultsPtr  so2RetrainingResults, 
                                           int                   numOfImages,
                                           float&                tValue
                                          )
{
  // see  web site  "http://mathworld.wolfram.com/Pairedt-Test.html"
  // for procedure to calc paired t-test

  tValue = 0.0f;
 
  if  ((!so1RetrainingResults)  ||  (!so2RetrainingResults))
    return;

  float  so1Mean = Round (so1RetrainingResults->AccuracyMean ());
  float  so2Mean = Round (so2RetrainingResults->AccuracyMean ());

  int  N = Min (so1RetrainingResults->NumOfPasses (), so2RetrainingResults->NumOfPasses ());

  float  totDenominator = 0.0f;

  const float*  so1 = so1RetrainingResults->Accuracies ();
  const float*  so2 = so2RetrainingResults->Accuracies ();

  if  ((!so1)  ||  (!so2))
    return;


  for  (int i = 0; i < N;  i++)
  {
    float  so1_i = Round (so1[i]) - so1Mean;
    float  so2_i = Round (so2[i]) - so2Mean;
    float delta = so1_i - so2_i;
    totDenominator += (delta * delta);
  }

  if  (totDenominator == 0.0)
    return;

  float  numerator = (float)(N * (N - 1));

  float  deltaMean = so1Mean - so2Mean;

  tValue = deltaMean * sqrt (numerator / totDenominator);

  return;
}  /* CalcPairedTTest */










SummaryByIPCandIPRList::SummaryByIPCandIPRList (bool _owner):
     KKQueue<SummaryByIPCandIPR> (_owner, 10)
{
}




class  IPRComparrison: public  QueueComparison<SummaryByIPCandIPR>
{
public:
   IPRComparrison ():  QueueComparison<SummaryByIPCandIPR> () {}

   bool  operator () (SummaryByIPCandIPRPtr  p1,  
                      SummaryByIPCandIPRPtr  p2
                     )
   {
     return  (p1->ImagesPerRetraining () < p2->ImagesPerRetraining ());
   }
};




void  SummaryByIPCandIPRList::SortByImagesPerRetraining ()
{
  IPRComparrison*  c = new IPRComparrison ();
  sort (begin (), end (), *c);
  delete  c;
}  /* SortByImagesPerRetraining */



