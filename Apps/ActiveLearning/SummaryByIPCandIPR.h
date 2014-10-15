#ifndef  _ACTIVELEARNINGREPORT_
#define  _ACTIVELEARNINGREPORT_

//********************************************************************************************
//*                                SummaryByIPCandIPR                                      *
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



class    SortOrderResults;

#include  "ClassGroupTotals.h"
#include  "ImageClass.h"
#include  "TrainingImage.h"

typedef  KKQueue<SortOrderResults>  SortOrderResultsList;

class  RetrainingResults;
typedef  RetrainingResults*  RetrainingResultsPtr;

class  RetrainingResultsList;
typedef  RetrainingResultsList*  RetrainingResultsListPtr;


class  SortOrderResults;
typedef  SortOrderResults*  SortOrderResultsPtr;

//  VersionNum is used to make sure that the data stored in the Sumary Files kept inm 
//             the subdirectories  is of the current format.  If not then the program 
//             will re-read the ClassGroupTotals files and save new Summary files.

// 2005-06-29   
// Moved to version 6,  Added a field that represents the change in accurcay.  It will be calculated by getting the average
// over all random passes such that   accDelta[x] = (sum(i=0 to numRandomPasses) (accuracy[i][x-1])) / numRandomPasses
#define  VersionNum  6


class  SummaryByIPCandIPR
{
public:
  SummaryByIPCandIPR (RunLog&          _log,
                      ImageClassList&  _imageClasses,
                      KKStr           _subDirName
                     );

  ~SummaryByIPCandIPR ();


  int   IPR ()  {return imagesPerRetraining;}


  void  CalcPairedTTest (RetrainingResultsPtr  so1RetrainingResults, 
                         RetrainingResultsPtr  so2RetrainingResults, 
                         int                   numOfImages,
                         float&                tValue
                        );


  void  GetImageCountsByIncrements (SortOrderType  sortOrder,
                                    int            increment,
                                    int&           count,
                                    int*&          imageCounts
                                   );

  RetrainingResultsPtr  GetRetrainingResults (SortOrderType  sortOrder, 
                                              int            numOfImages
                                             );


  SortOrderResultsPtr   GetSortOrderResults (SortOrderType  sortOrder);


  TrainingImageListPtr  GetImagesAddedToTrainingLibrary (SortOrderType  sortOrder,
                                                         int            retraining
                                                        );

  TrainingImageListPtr  GetImagesAddedToTrainingLibrary (SortOrderType  sortOrder,
                                                         int            retraining,
                                                         int            randomPass
                                                        );

  int   ImagesPerRetraining   ()  {return  imagesPerRetraining;}

  int   InitialImagesPerClass ()  {return  initialImagesPerClass;}

  void  Load (KKStr   fileName,
              bool&    validFormat
             );

  void  LoadResultsFiles ();

  void  PrintReport (ostream&  report);

  void  PrintReportWithIncrements (ostream&  report,
                                   int       increment
                                  );

  void  PrintReportByIncrementsAndPaiedTTest (ostream&  report,
                                              int       increment,
                                              int       countLimit
                                             );


  void  PrintReportSupportPointsUsingSampling (ostream&  report,
                                               int       sampleRate,
                                               int       countLimit
                                              );


  void  PrintReportByClass (ostream&       report,
                            SortOrderType  sortOrder,
                            int            increment
                           );


  void  Save (KKStr  fileName);

  void  GetStatsForNumOfImages (SortOrderType  sortOrder, 
                                int            numOfImages,
                                float&         accuracyMean,
                                float&         accuracyMin,
                                float&         accuracyMax,
                                float&         accuracyStdDev,
                                float&         accuracyStdErr,
                                float&         accuracyDelta,     // kk 2006-06-29
                                float&         supportPointsMean,
                                float&         supportPointsMin,
                                float&         supportPointsMax,
                                float&         supportPointsStdDev,
                                float&         supportPointsStdErr,
                                float&         trainingTimeMean,
                                float&         testingTimeMean,
                                bool&          found
                               );

   
  //***************************************************************
  //*    Will copy images added to training library for a given   *
  //*   retraining to specified directory  'destDir'.             *
  //***************************************************************
  void  RecoverNewTrainingImages (SortOrderType  sortOrder, 
                                  int            retraining,
                                  const KKStr&  rootDir
                                 );

private:
  void  TryLoadingResultsForASpecificSortOrder (SortOrderType sortOrder);

  RetrainingResultsListPtr  GetRetrainingsForASortOrder (SortOrderType  sortOrder);


  ClassGroupTotalsList  allTotals;
  KKStr                baseResultsFileName;
  ImageClassList&       imageClasses;
  int                   imagesPerRetraining;
  int                   initialImagesPerClass;
  RunLog&               log;
  SortOrderResultsList  results;
  KKStr                subDirName;
  int                   versionNum;
};



typedef  SummaryByIPCandIPR*  SummaryByIPCandIPRPtr;



class  SummaryByIPCandIPRList: public  KKQueue<SummaryByIPCandIPR>
{
public:
  SummaryByIPCandIPRList (bool  _owner);
  void  SortByImagesPerRetraining ();
};




#endif
