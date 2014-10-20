#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <math.h>
#include  <vector>
#include  <sstream>
#include  <iomanip>

#include  <set>
#include  <vector>


#include  "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace  KKU;


#include "Model.h"
#include "ClassProb.h"
#include "FeatureNumList.h"
#include "FeatureVector.h"


#include "ModelKnn.h"


using namespace  MLL;



ModelKnn::ModelKnn (FileDescPtr           _fileDesc,
                    volatile const bool&  _cancelFlag,
                    RunLog&               _log
                   ):
  Model (_fileDesc, _cancelFlag, _log),
  param (NULL)
{
}




ModelKnn::ModelKnn (const KKStr&          _name,
                    const ModelParamKnn&  _param,         // Create new model from
                    FileDescPtr           _fileDesc,
                    volatile const bool&  _cancelFlag,
                    RunLog&               _log
                   ):
  Model (_name, _param, _fileDesc, _cancelFlag, _log),
  param (NULL)
{
  param = dynamic_cast<ModelParamKnnPtr> (Model::param);
}




ModelKnn::ModelKnn (const ModelKnn&   _model):
  Model (_model),
  param (NULL)
{
  param = dynamic_cast<ModelParamKnnPtr> (Model::param);
}


ModelKnn::~ModelKnn ()
{

}



ModelKnnPtr  ModelKnn::Duplicate ()  const
{
  return  new ModelKnn (*this);
}


ModelParamKnnPtr  ModelKnn::Param ()
{
  return  param;
}


MLClassConstPtr  ModelKnn::Predict (FeatureVectorPtr  image)
{
  return NULL;
}



void  ModelKnn::Predict (FeatureVectorPtr     example,
                         MLClassConstPtr   knownClass,
                         MLClassConstPtr&  predClass1,
                         MLClassConstPtr&  predClass2,
                         int32&               predClass1Votes,
                         int32&               predClass2Votes,
                         double&              probOfKnownClass,
                         double&              predClass1Prob,
                         double&              predClass2Prob,
                         double&              compact,
                         int32&               numOfWinners,
                         bool&                knownClassOneOfTheWinners,
                         double&              breakTie
                        )
{
}  /* Predict */
                        



ClassProbListPtr  ModelKnn::ProbabilitiesByClass (FeatureVectorPtr  example)
{
  if  (!classesIndex)
  {
    KKStr errMsg = "ModelKnn::ProbabilitiesByClass   ***ERROR***      (classLabelAssigments == NULL)";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  bool  newExampleCreated = false;
  FeatureVectorPtr  encodedExample = PrepExampleForPrediction (example, newExampleCreated);

  //  double  y = SVM289::svm_predict_probability (svmModel,  *encodedExample, classProbs, votes);

  if  (newExampleCreated)
  {
    delete encodedExample;
    encodedExample = NULL;
  }

  ClassProbListPtr  results = new ClassProbList ();
  int32  idx;
  for  (idx = 0;  idx < numOfClasses;  idx++)
  {
    MLClassConstPtr  ic = classesIndex->GetMLClass (idx);
    results->PushOnBack (new ClassProb (ic, 0.0, 0.0f));
  }

  return  results;
}  /* ProbabilitiesByClass */




void  ModelKnn::ProbabilitiesByClass (FeatureVectorPtr            example,
                                      const MLClassConstList&  _mlClasses,
                                      int32*                      _votes,
                                      double*                     _probabilities
                                     )
{
}


void  ModelKnn::ProbabilitiesByClass (FeatureVectorPtr            _example,
                                      const MLClassConstList&  _mlClasses,
                                      double*                     _probabilities
                                     )
{
}  /* ProbabilitiesByClass */

  


void  ModelKnn::ReadSpecificImplementationXML (istream&  i,
                                               bool&     _successful
                                              )
{
  char  buff[20480];
  KKStr  field;

  KKStr  modelFileName;

  int32  numOfModels = 0;

  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);
    field = ln.ExtractQuotedStr ("\n\r\t", true);
    field.Upper ();

    if  (field.EqualIgnoreCase ("</ModelKnn>"))
    {
      break;
    }

    else if  (field.EqualIgnoreCase ("<Model>"))
    {
      Model::ReadXML (i, _successful);
    }

    else
    {
      // Add code tro deal with items that are specific to 'ModelSvmBase'
    }
  }

  if  (!_successful)
    validModel = false;

  return;
}  /* ReadSpecificImplementationXML */



void  ModelKnn::TrainModel (FeatureVectorListPtr  _trainExamples,
                            bool                  _alreadyNormalized,
                            bool                  _takeOwnership  /*!< Model will take ownership of these examples */
                           )
{
  log.Level (10) << "ModelKnn::TrainModel  Model[" << param->FileName () << "]" << endl;

  try
  {
    Model::TrainModel (_trainExamples, _alreadyNormalized, _takeOwnership);
  }
  catch  (const KKStrException& e)
  {
    validModel = false;
    throw e;
  }
}  /* TrainModel */



void  ModelKnn::WriteSpecificImplementationXML (ostream&  o)
{
  log.Level (20) << "ModelKnn::WriteSpecificImplementationXML  Saving Model in File." << endl;

  o << "<ModelKnn>" << endl;

  o << "</ModelKnn>" << endl;
}  /* WriteSpecificImplementationXML */
