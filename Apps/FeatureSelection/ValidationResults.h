#if  !defined(_VALIDATIONRESULTS_)  
#define  _VALIDATIONRESULTS_

#include  "FeatureSelectionTypes.h"
#include  "JobValidation.h"
#include  "ConfusionMatrix2.h"
#include  "Results.h"

/**
 @class  ValidationResults
 @brief  Where inale Results for a validation run for a single configration are stored.
 @details
 @code
 ***************************************************************************
 *  Used to store rsults from a single validation job (JobValidtaion).     *
 *  Will hold both ConfusionMatrix and CrossValiodation Results(such as)   *
 *  TrainingTime, TestTime, Number Support Vectors, etc.                   *
 *  These results will be written to a file by 'JobRandomSplit' as each    *
 *  node is evaluated.                                                     *
 *                                                                         *
 *  this class was cloned from ValidationResults.  It will be used by     *
 *  FinalResults to track the Validation results for a Training Model.    *
 ***************************************************************************
 @endcode
 */
class  ValidationResults: public  Results
{
public:
  typedef  ValidationResults*  ValidationResultsPtr;

  ValidationResults (RunLog&  _log);

  ValidationResults (FinalResultType          _resultType,
                     TrainingConfiguration2Ptr  _config,
                     CrossValidationPtr        _crossValidation,
                     FeatureVectorListPtr      _trainData,
                     const KKStr&              _hostName,
                     int                       _classedCorrectlySize,
                     bool*                     _classedCorrectly,
                     JobValidationPtr          _job,
                     RunLog&                   _log
                    );
  
  virtual
    ~ValidationResults ();

  static
  ValidationResultsPtr  ConstructFromStream (istream&  i,
                                             RunLog&   log
                                            );

  const bool*           ClassedCorrectly     ()  const  {return classedCorrectly;}
  int                   ClassedCorrectlySize ()  const  {return classedCorrectlySize;}

  void  Write (ostream&  o);


private:
  void  Read (istream&  i);

  bool*                classedCorrectly;
  int                  classedCorrectlySize;

};  /* ValidationResults*/


typedef  ValidationResults::ValidationResultsPtr  ValidationResultsPtr;



#endif

