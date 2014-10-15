#if  !defined(_JOBVALIDATION_)  
#define  _JOBVALIDATION_

#include "BinaryJob.h"


class  JobValidation:  public  BinaryJob
{
public:
  typedef  JobValidation*  JobValidationPtr;
  typedef  KKU::uint  uint;

  JobValidation (const JobValidation&  j);


  JobValidation (ProcessorPtr           _processor,
                 int                    _parentId,
                 const FeatureNumList&  _features,
                 int                    _chgFeatureNum,
                 int                    _numOfRounds,
                 double                 _cParm,
                 double                 _gammaParm,
                 float                  _aParm,
                 const KKStr&           _configFileName
                );

  JobValidation (ProcessorPtr  _processor,
                 KKStrParser&  _statusLine
                );

  virtual
    ~JobValidation ();



  virtual  JobTypes  JobType ()  const;


  virtual  void     EvaluateNode ();

  virtual
  void     ReFresh (BinaryJob&  j);
 
  virtual
  void  ReFresh (JobValidation&  j);


  virtual  KKStr    ToStatusStr ();



private:

  void  ProcessStatusField (const KKStr&  fieldName,
                            const KKStr&  fieldValue
                           );

  void  ProcessStatusStr (KKStrParser&  statusStr);

  bool*  classedCorrectly;
  int    classedCorrectlySize;
  KKStr  configFileName;
};  /* JobValidation */


typedef  JobValidation::JobValidationPtr  JobValidationPtr;


#endif
