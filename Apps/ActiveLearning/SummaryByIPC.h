#ifndef  _SUMMARYBYIPC_
#define  _SUMMARYBYIPC_


#include  "RunLog.h"
#include  "ImageClass.h"
#include  "SortOrderType.h"
#include  "SummaryByIPCandIPR.h"

class  SummaryByIPC
{
public:
  SummaryByIPC (RunLog&          _log,
                const KKStr      _rootDir,
                ImageClassList&  _imageClasses,
                bool&            _successful
               );


  int             IPC                   ()  {return  ipc;}
  int             InitialImagesPerClass ()  {return  ipc;}
  const  KKStr&   RootDir               ()  {return rootDir;}

  void   GetImageCountsByIncrements (int            _imagesPerRetraining,
                                     SortOrderType  sortOrder,
                                     int            increment,
                                     int&           count,
                                     int*&          imageCounts
                                    );

  void   GetStatsForNumOfImages (int            _imagesPerRetraining,
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
                                );


  void   GenerateSummaryReports ();

  void   GenerateReportAccuracyByClass (SortOrderType sortOrder,
                                        int           sampleRate    
                                       );

  void   GenerateReportByIPR_ByTens (SortOrderType sortOrder);

  void   GenerateReportByIPR_ByTens (ostream&      r,
                                     SortOrderType sortOrder
                                    );

  void   GenerateGNUplotData (ostream&      r,
                              SortOrderType sortOrder
                             );

  void   GenerateGNUplotData (SortOrderType sortOrder);

  void   GenerateReportsForJMLR ();


  void   GenerateReportByIPRForSampleRate (SortOrderType sortOrder,
                                           int           increment
                                          );

  void   GenerateReportByIPRForSampleRate (ostream&      r,
                                           SortOrderType sortOrder,
                                           int           increments
                                          );


  void   RecoverNewTrainingImages (SortOrderType  sortOrder, 
                                   int            numImagesToRecocer,
                                   const KKStr&  rootDir
                                  );


private:
  void  LoadSummaries (bool&  _successful);


  int                      ipc;
  ImageClassList&          imageClasses;
  RunLog&                  log;
  KKStr                    rootDir;
  SummaryByIPCandIPRList   summaries;
};



typedef  SummaryByIPC*  SummaryByIPCPtr;


class  SummaryByIPCList: public  KKQueue<SummaryByIPC>
{
public:
  SummaryByIPCList (bool  _owner);
private:
};


typedef  SummaryByIPCList*  SummaryByIPCListPtr;



class  SummaryByIPCListIterator: public  QueueIterator<SummaryByIPC>
{
public:
  SummaryByIPCListIterator (SummaryByIPCList&  _ipcSummaries);
};


#endif
