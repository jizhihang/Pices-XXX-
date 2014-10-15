#if  !defined(_RESULTS_)  
#define  _RESULTS_

#include  "FeatureSelectionTypes.h"
#include  "BinaryJob.h"
#include  "ConfusionMatrix2.h"
#include  "FeatureVector.h"


//***************************************************************************
//*  Used to store rsults from a single RandomSplit job (JobRandomSplit).   *
//*  Will hold both ConfusionMatrix and CrossValiodation Results(such as)   *
//*  TrainingTime, TestTime, Number Support Vectors, etc.                   *
//*  These results will be written to a file by 'JobRandomSplit' as each    *
//*  node is evaluated.                                                     *
//***************************************************************************

class  Results
{
public:
  typedef  Results*  ResultsPtr;

  Results (RunLog&  _log);

  Results (FinalResultType          _resultType,
           TrainingConfiguration2Ptr _config,
           CrossValidationPtr        _crossValidation,
           FeatureVectorListPtr      _trainData,
           const KKStr&              _hostName,
           BinaryJobPtr              _job,
           RunLog&                   _log
          );
  
  virtual
    ~Results ();

  static
  ResultsPtr  ConstructFromStream (istream&  i,
                                   RunLog&   log
                                  );

  float                   Accuracy            ()  const;
  VectorFloat             AccuracyByClass     ()  const;
  float                   AccuracyNorm        ()  const;
  const KKStr&            ConfigFileName      ()  const  {return configFileName;}
  ConfusionMatrix2Ptr     ConfusionMatrix     ()  const  {return confusionMatrix;}
  KKStr                   Description         ()  const;
  const KKStr&            HostName            ()  const  {return hostName;}
  MLClassConstListPtr  MLClasses        ()  const;
  int                     JobId               ()  const  {return jobId;}
  SVM_MachineType         MachineType         ()  const  {return machineType;}
  KKStr                   MachineTypeStr      ()  const;
  double                  NumFeatures         ()  const  {return numFeatures;}
  float                   NumFeaturesWeighted ()  const  {return numFeaturesWeighted;}
  double                  NumSupportVectors   ()  const  {return numSVs;}
  double                  NumSVs              ()  const  {return numSVs;}
  FinalResultType         ResultType          ()  const  {return resultType;}
  KKStr                   ResultTypeStr       ()  const;
  SVM_SelectionMethod     SelectionMethod     ()  const  {return selectionMethod;}
  KKStr                   SelectionMethodStr  ()  const;
  KKStr                   SvmParameters       ()  const  {return svmParameters;}
  double                  TestTime            ()  const  {return testTime;}
  double                  TotalNumSVs         ()  const  {return totalNumSVs;}
  double                  TrainingTime        ()  const  {return trainingTime;}


  virtual
  void  Write (ostream&  o);

  Results&   operator+= (const Results&  right);
  Results&   operator/= (float divisor);
  Results&   operator/= (int   divisor);

protected:
  virtual
  void  Read (istream&  i);


private:
  ConfusionMatrix2Ptr  confusionMatrix;
  KKStr                hostName;
  int                  jobId;                // Id of Job that prodced these results
  RunLog&              log;
  double               numFeatures;
  float                numFeaturesWeighted;
  double               numSVs;
  FinalResultType      resultType;
  double               testTime;
  double               totalNumSVs;
  double               trainingTime;

  // Configuration Parameters Used
  KKStr                configFileName;
  SVM_MachineType      machineType;
  SVM_SelectionMethod  selectionMethod;
  KKStr                svmParameters;
};  /* Results*/


typedef  Results::ResultsPtr  ResultsPtr;



#endif
