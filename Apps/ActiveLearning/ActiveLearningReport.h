#ifndef  _ACTIVELEARNINGREPORT_
#define  _ACTIVELEARNINGREPORT_

//********************************************************************************************
//*                                ActiveLearningReport                                      *
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


typedef  KKQueue<SortOrderResults>  SortOrderResultsList;

class  RetrainingResultsList;
typedef  RetrainingResultsList*  RetrainingResultsListPtr;


class  ActiveLearningReport
{
public:
  ActiveLearningReport (RunLog&          _log,
                        ImageClassList&  _imageClasses,
                        KKStr           _subDirName
                       );

  ~ActiveLearningReport ();


  void  GetImageCountsByIncrements (SortOrderType  sortOrder,
                                    int            increment,
                                    int&           count,
                                    int*&          imageCounts
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

  void  Save (KKStr  fileName);

  void  GetStatsForNumOfImages (SortOrderType  sortOrder, 
                                int            numOfImages,
                                float&         numOfSupportVectors,
                                float&         accuracy,
                                bool&          found
                               );


private:
  void  TryLoadingResultsForASpecificSortOrder (SortOrderType     sortOrder,
                                                KKStr            qualifier,
                                                ImageClassList&   classes
                                               );

  RetrainingResultsListPtr  GetRetrainingsForASortOrder (SortOrderType  sortOrder);


  KKStr                 baseResultsFileName;
  ImageClassList&       imageClasses;
  int                   imagesPerRetraining;
  int                   initialImagesPerClass;
  RunLog&               log;
  SortOrderResultsList  results;
  KKStr                 subDirName;
};



typedef  ActiveLearningReport*  ActiveLearningReportPtr;



class  ActiveLearningReportList: public  KKQueue<ActiveLearningReport>
{
public:
  ActiveLearningReportList (bool  _owner);
};


#endif
