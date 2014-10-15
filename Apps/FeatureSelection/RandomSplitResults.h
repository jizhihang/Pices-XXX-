#if  !defined(_RANDOMSPLITRESULTS_)  
#define  _RANDOMSPLITRESULTS_

#include  "FeatureSelectionTypes.h"
#include  "JobRandomSplit.h"
#include  "ConfusionMatrix2.h"

#include  "Results.h"

/**
 @class  RandomSplitResults
 @brief Where results for a JobRandomSplits are stored.
 @details
 @code
  ***************************************************************************
  *  Used to store rsults from a single RandomSplit job (JobRandomSplit).   *
  *  Will hold both ConfusionMatrix and CrossValiodation Results(such as)   *
  *  TrainingTime, TestTime, Number Support Vectors, etc.                   *
  *  These results will be written to a file by 'JobRandomSplit' as each    *
  *  node is evaluated.                                                     *
  ***************************************************************************
 @endcode
 */
class  RandomSplitResults: public  Results
{
public:
  typedef  RandomSplitResults*  RandomSplitResultsPtr;

  RandomSplitResults (RunLog&  _log);

  RandomSplitResults (FinalResultType          _resultType,
                      TrainingConfiguration2Ptr _config,
                      CrossValidationPtr        _crossValidation,
                      FeatureVectorListPtr      _trainData,
                      const KKStr&              _hostName,
                      JobRandomSplitPtr         _job,
                      RunLog&                   _log
                     );
  
  virtual
    ~RandomSplitResults ();

  static
  RandomSplitResultsPtr  ConstructFromStream (istream&  i,
                                              RunLog&   log
                                             );

  int                   SplitNum           ()  const  {return splitNum;}


  void  Write (ostream&  o);



private:
  void  Read (istream&  i);
  int                  splitNum;
};  /* RandomSplitResults*/


typedef  RandomSplitResults::RandomSplitResultsPtr  RandomSplitResultsPtr;




class  RandomSplitResultsList: public  KKQueue<RandomSplitResults>
{
public:
  typedef  RandomSplitResultsList*  RandomSplitResultsListPtr;

  RandomSplitResultsList (bool    _owner,
                          RunLog& _log
                         );

  RandomSplitResultsList (const RandomSplitResultsList&  results);

  RandomSplitResultsList (const KKStr&  resultsFileName,
                          RunLog&       _log
                         );

  virtual
    ~RandomSplitResultsList ();


  RandomSplitResultsPtr  Average ()  const ;

  static
  void  WriteFileHeader (ostream&             o,
                         const KKStr&         configFileName,
                         const KKStr&         description,
                         KKStr                fileName,
                         SVM_MachineType      machineType,
                         SVM_SelectionMethod  selectionMethod
                        );

private:
  void  Read (istream&  i);

  RunLog&  log;
};  /* RandomSplitResultsList */

typedef  RandomSplitResultsList::RandomSplitResultsListPtr  RandomSplitResultsListPtr;


#endif
