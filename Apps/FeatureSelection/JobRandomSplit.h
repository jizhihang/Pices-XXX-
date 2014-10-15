#if  !defined(_JOBRANDOMSPLIT_)  
#define  _JOBRANDOMSPLIT_

#include "BinaryJob.h"


class  JobRandomSplit:  public  BinaryJob
{
public:
  typedef  JobRandomSplit*  JobRandomSplitPtr;

  JobRandomSplit (const JobRandomSplit&  j);


  JobRandomSplit (ProcessorPtr           _processor,
                  int                    _parentId,
                  const FeatureNumList&  _features,
                  int                    _chgFeatureNum,
                  int32                  _numOfRounds,
                  double                 _cParm,
                  double                 _gammaParm,
                  float                  _aParm,
                  int                    _splitNum
                 );

  JobRandomSplit (ProcessorPtr  _processor,
                  KKStrParser&  _statusLine
                 );

  virtual
    ~JobRandomSplit ();



  virtual  JobTypes  JobType ()  const;

  int  SplitNum ()  const  {return  splitNum;}

  void  SplitNum (int _splitNum)   {splitNum = _splitNum;}



  virtual  void     EvaluateNode ();

  virtual
  void     ReFresh (BinaryJob&  j);
 
  virtual
  void  ReFresh (JobRandomSplit&  j);


  virtual  KKStr    ToStatusStr ();



private:

  // Caller will take ownership of both 'randSplitTrainData' and 'randSplitTestData' which 
  // meens it will be responable for deleting them when it is done with them.
  void  CreateRandomSplitTrainAndTestExamples (FeatureVectorListPtr&  randSplitTrainData,
                                               FeatureVectorListPtr&  randSplitTestData
                                              );



  void  ProcessStatusField (const KKStr&  fieldName,
                            const KKStr&  fieldValue
                           );


  void  ProcessStatusStr (KKStrParser&  statusStr);




  int   splitNum;

};  /* JobRandomSplit */


typedef  JobRandomSplit::JobRandomSplitPtr  JobRandomSplitPtr;


#endif
