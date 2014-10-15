#ifndef  _CLASSGROUPTOTALS_
#define  _CLASSGROUPTOTALS_
//********************************************************************************************
//*                                ClassGroupTotals                                          *
//*                                                                                          *
//* Used by all the active learning programs.  Is responsable for keeping track of stats     *
//* while performining the active learning expirements and saving them to disk.  Can later   *
//* read these results and report them.                                                      *
//*                                                                                          *
//* Each instance of this objects can hold all the results for all Random Passes for a       *
//* given IIPC & IPR combinatio.  Each ActiveLearning job will only process one RandomPass.  *
//* The report apps will load all passes for a IIPC & IPR combination into one instance of   *
//* this object.                                                                             *
//*                                                                                          *
//*                                                                                          *
//* The idea is that each ActiveLearning job(JobDesc) will create one instance of this       *
//* object and save the results of the active learning in them.  When the job is done it     *
//* will save the results in a file for later recall.  Each ActiveLearning job(Jobdesc)      *
//* will process one random randomPass for one sort method for one IIPC for one IPR.         *
//*                                                                                          *
//* Later when all active learning jobs have been completed a seperate app can retrieve the  *
//* results and report them.                                                                 *
//*                                                                                          *
//*                                                                                          *
//*                                                                                          *
//*------------------------------------------------------------------------------------------*
//*    Date     Programer    Description                                                     *
//*                                                                                          *
//* 2004-12-08  Kurt Kramer  Fixed it so that when stats are calculated the actual number    *
//*                          of result file will be used for calcing stats.  This way we can *
//*                          run the reports before all the jobsb are done to see how things *
//*                          are doing.                                                      *
//*                                                                                          *
//*                                                                                          *
//* 2005-01-15  Kurt Kramer  Added support for Stream Block size                             *
//*                                                                                          *
//* 2005-01-18  Kurt Kramer  Removed all references to 2 class Totals.                       *
//*                                                                                          *
//* 2005-01-24  Kurt Kramer  replaced references to Group to retrainig,  and pass to         *
//*                          randomPass to reflect tere real purpose.                        *
//*                                                                                          *
//*                                                                                          *
//*                                                                                          *
//********************************************************************************************

#ifndef  _CLASSGROUPHISTORY_
class  HistoryBit;
class  HistoryBitList;

typedef  HistoryBit*      HistoryBitPtr;
typedef  HistoryBitList*  HistoryBitListPtr;
#endif



#include "MLClass.h"


#include "BasicTypes.h"
#include "Str.h"
#include "SortOrderType.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "TrainingImage.h"

using namespace MLL;


#ifndef  _CLASSFOCUS_
class  ClassFocus;
typedef  ClassFocus*  ClassFocusPtr;
#endif



#ifndef  _RUNLOG_
class  RunLog;
typedef  RunLog*  RunLogPtr;
#endif



typedef  std::vector<VectorDouble>      VectorDouble2D;
typedef  std::vector<VectorDouble2D>    VectorDouble3D;
typedef  std::vector<VectorDouble3D>    VectorDouble4D;

typedef  std::vector<SortOrderType>     VectorSortOrder;
typedef  std::vector<VectorSortOrder>   VectorSortOrder2D;
typedef  std::vector<VectorSortOrder2D> VectorSortOrder3D;





class  ClassGroupTotals
{
public:
  ClassGroupTotals (KKStr         _fileName,   // use this constructor to load results from
                    MLClassList&  _mlClasses,  // a file.  You would call this constructor
                    int           _iipc,       // for the 1st results file representing one
                    int           _ipr,        // random randomPass.  Then you would call ReadInDataFile
                    RunLog&       _log         // for each of the other results file involved in
                   );                          // the same IIPC & IPR & SortOrder combination.
                                               // Used by the  SummaryByIPCandIPR object to load
                                               // existing results.


  ClassGroupTotals (SortOrderType   _sortOrder,        // This constructor is used to by ActiveLearning
                    int             _numOfRetrainings, // when running a Job that only has twoo classes.
                    int             _numOfPasses,      // Active learning will use the WriteOutHistory
                    int             _iipc,             // the WriteOutHistory method to save the 
                    int             _ipr,              // results.  
                    int             _streamBlockSize,
                    RunLog&         _log,
                    MLClassList&    _mlClasses,
                    bool            _collectHistory
                   );



  ~ClassGroupTotals ();




  float             Accuracy (int  retraining,
                              int  classIDX
                             )  const;

  float             Accuracy (int         retraining,
                              MLClassPtr  mlClass
                             )  const;

  float             Accuracy (int  retraining)  const;
 
  float             AccuracyForRetrainingAndPass (int  retraining,
                                                  int  randomPass
                                                 )  const;

  //*******************************************************************************
  //*                   AccuracyDeltaForRetrainingAndPass                         *
  //*                                                                             *
  //*  Calcs the change in accuracy occuring at a given retraining and randomPass *
  //*  by subtracting the accuracy from a prev retraining from the one for the    *
  //*  next retraining.                                                           *
  //*******************************************************************************
  float             AccuracyDeltaForRetrainingAndPass (int  retraining,
                                                       int  randomPass
                                                      )  const;


  float             AvgNumOfProbs (int retraining,
                                   int classIDX
                                  )  const;

  float             AvgNumOfProbs (int            retraining,
                                   MLClassPtr  mlClass
                                  )  const;

  float             AvgProb (int retraining,  
                             int classIDX,   
                             uint idx
                            )  const;

  float             AvgProb (int            retraining,  
                             MLClassPtr  mlClass, 
                             uint           idx
                            )  const;
  
  VectorDouble      AvgProbVector (int retraining,  int classIDX)  const;

  VectorDouble      AvgProbVector (int retraining,  MLClassPtr mlClass)  const;

  float            Count  (int  retraining,
                           int  classIDX
                          )  const;
  
  float            Count  (int         retraining,
                           MLClassPtr  mlClass
                          )  const;
  
  float            Correct  (int  retraining,
                             int  classIDX
                            )  const;

  float            Correct  (int            retraining,
                             MLClassPtr  mlClass
                            )  const;

  int               IIPC () const  {return iipc;}

  int               IPR  () const  {return ipr;}

  const
  MLClassList&      MLClasses () const  {return  mlClasses;}

  float             RetrainingCounts (int retraining)  const;
 
  uint              MaxNumOfProbs  (int  retraining,  int  classIDX)  const;

  uint              MaxNumOfProbs  (int  retraining,  MLClassPtr  mlClass)  const;

  int               NumOfRandomPasses ()  const  {return  numOfRandomPasses;}

  int               NumOfRetrainings  ()  const  {return  numOfRetrainings;}

  float             NumOfSupportVectors (int retraining)  const;

  void              NumOfSupportVectors (int     retraining, 
                                         int     randomPass,  
                                         int     _numOfSupportVectors,
                                         float   _trainingTime,
                                         float   _testingTime
                                        )  const;

  SortOrderType     SortOrder       ()  const {return  sortOrder;}

  int               StreamBlockSize ()  const  {return streamBlockSize;}

  float             TrainingTimeMean (int retraining)  const;
  
  float             TestingTimeMean  (int retraining)  const;


  //  About Images added to Training Libray for a given Retrainig and randomPass
  const 
  TrainingImageList&    TrainingImages (int  retraining,
                                        int  randomPass
                                       );

  TrainingImageListPtr  TrainingImages (int  retraining);

  VectorFloat           MeanNonStreamPlaces (int  retraining);





  void   AddAProbability (int            retraining,
                          int            randomPass,
                          MLClassPtr     mlClass,
                          float          probability,
                          const KKStr&   imageFileName,
                          int            position,
                          float          percentile
                         );

  void   SetSortMethodUsed (int            retraining,
                            MLClassPtr     mlClass, 
                            int            randomPass,
                            SortOrderType  sortMethodUsed
                           );

  KKStr   SortMethodsUsed (int  retraining)  const;

  KKStr   SortMethodsUsed (int         retraining,
                           MLClassPtr  mlClass
                          )  const;

  void  AddIn (ClassGroupTotals&  ai);


  VectorFloat  CalcAccuraciesByClass (int  retraining)  const;

  void  CalcRetrainingAccuracyStats (int      retraining,
                                     float&   mean,
                                     float&   variance,
                                     float&   stdDev,
                                     float&   min,
                                     float&   max,
                                     float&   accuracyDelta
                                    )  const;

  void  CalcRetrainingSupportPointStats (int     retraining,
                                         float&  mean,
                                         float&  variance,
                                         float&  stdDev,
                                         float&  min,
                                         float&  max,
                                         float&  trainingTimeMean,
                                         float&  testingTimeMean
                                        )  const;

  void  ConfusionMatrix (ostream&  report,
                         int       retraining
                        )  const;

  VectorFloat  GetNewTrainingImagesThisRetraining (int  retraining);


  void  Prediction (int             retraining,
                    int             randomPass,
                    MLClassPtr   knownClass,
                    MLClassPtr   predictedClass,
                    float          probability
                   );


  KKStr  ProbabilityStr (int retraining,  int classIDX)  const;

  KKStr  ProbabilityStr (int retraining,  MLClassPtr mlClass)  const;

  KKStr  FormatSummaryHeader ()  const;

  KKStr  FormatSummaryLine (int  retraining)  const;

  KKStr  OneImageFormatSummaryLine (int  retraining)  const;

  KKStr  OneImageFormatHeaderLine1 ()  const;

  KKStr  OneImageFormatHeaderLine2 ()  const;


  void   PrintGroupBreakDown (ostream&  report)  const;





  //**************************************************************
  //*                   File I/O Routines.                       *
  //**************************************************************
  static
  ClassGroupTotals*  ConsolidateOneDirectoryOneSortOrder (const KKStr&   dirName,
                                                          MLClassList&   globalClassList,
                                                          SortOrderType  sortOrder,
                                                          RunLog&        log
                                                         );

  static
  void   GetClassGroupTotalsFileStats (const KKStr&     fileName,
                                       MLClassList&     globalClassList,
                                       RunLog&          log,
                                       MLClassListPtr&  classesInFile,
                                       SortOrderType&   sortOrder,
                                       int&             numOfRetrainings,
                                       int&             numOfRandomPasses,
                                       int&             streamBlockSize
                                      );



  void    ReadInDataFile (KKStr  _fileName,
                          bool&   _successful
                         );

  void    ReadInHistory (FILE*  in);

  void    WriteOutHistory (KKStr  fileName);




private:
  static
  void  Extract_IIPC_IPR_FromDirName (const KKStr&    dirName,
                                      int&             iipc,
                                      int&             ipr
                                     );


  inline
  int     MLClassIDX (const char*  funcName,
                      MLClassPtr   mlClass
                     )   const;


  void    InitializeVectors ();

  float   Predicted (int  retraining,
                     int  knownIDX,
                     int  predIDX
                    )  const;

  KKStr  ProbabilityStr (VectorDouble&  v)  const;


  void    ValidateClassIDX (const char*  funcName, 
                            int          classIDX
                           )   const;

  void    ValidateRetraining (const char*  funcName, 
                              int          retraining
                             )   const;

  void    ValidatePass (const char*  funcName, 
                        int          randomPass
                       )   const;

  
  void               CalcAvgProb (int         retraining, 
                                  MLClassPtr  mlClass,
                                  float&      numOfProbabilities,
                                  float&      avgProbability
                                 )  const;


  int                iipc;
  int                ipr;
  int                streamBlockSize;

  bool               collectHistory;

  KKStr              fileName;

  SortOrderType      sortOrder;

  MLClassList        mlClasses;

  RunLog&            log;

  uint               maxTrainingImagesPerRetraining;
 
  int                numOfRetrainings;

  int                numOfRandomPasses;

  float              totalCounts;

  float              totalCorrect;


  ClassFocusPtr**    totals;


  //  int**              trainLibSizeByClass;  // [

  HistoryBitList***  predictions;
  HistoryBitList***  probabilities;   //  [retraining][randomPass][]
  HistoryBitList***  sortMethods;
};  /* ClassGroupTotals */




typedef  ClassGroupTotals*  ClassGroupTotalsPtr;


typedef  vector<ClassGroupTotalsPtr>  ClassGroupTotalsVector;


class  ClassGroupTotalsList:  public  KKQueue<ClassGroupTotals>
{
public:
  ClassGroupTotalsList ();

  ClassGroupTotalsPtr  LocateBySortOrder (SortOrderType  sortOrder)  const;
};

typedef  ClassGroupTotalsList*  ClassGroupTotalsListPtr;



#endif
