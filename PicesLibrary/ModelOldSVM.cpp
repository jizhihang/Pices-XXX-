#include  "FirstIncludes.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <set>
#include <vector>


#include "MemoryDebug.h"
#include "BasicTypes.h"

using namespace  std;


#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace  KKU;


#include "ModelOldSVM.h"

#include "BinaryClassParms.h"
#include "ClassAssignments.h"
#include "ClassProb.h"
#include "FeatureEncoder2.h"
#include "FeatureNumList.h"
#include "FeatureVector.h"
#include "MLLTypes.h"
#include "ModelParamOldSVM.h"
#include "NormalizationParms.h"
#include "SVMparam.h"
using namespace MLL;




ModelOldSVM::ModelOldSVM (FileDescPtr           _fileDesc,
                          volatile const bool&  _cancelFlag,
                          RunLog&               _log
                         ):
  Model (_fileDesc, _cancelFlag, _log),
  assignments (NULL),
  svmModel    (NULL)
{
  Model::param = new ModelParamOldSVM (_fileDesc, _log);
}





/**
 @brief Creates a new svm model from the provided example (example) data
 @param[in] _param The paramaters for the svm, and for creating the model.
 @param[in] _fileDesc A description of the data file.
 @param[in] _cancelFlag  If you want this instance to stop processing set this field to true in another thread.
 @param[in] _log A logfile stream. All important events will be ouput to this stream
 */
ModelOldSVM::ModelOldSVM (const KKStr&            _name,
                          const ModelParamOldSVM& _param,         // Create new model from
                          FileDescPtr             _fileDesc,
                          volatile const bool&    _cancelFlag,
                          RunLog&                 _log
                         )
:
  Model   (_name, _param, _fileDesc, _cancelFlag, _log),
  assignments (NULL),
  svmModel    (NULL)

{
}



ModelOldSVM::ModelOldSVM (const ModelOldSVM& _model)
:
  Model (_model),
  assignments (NULL),
  svmModel    (NULL)

{
}



/**
 *@brief Frees any memory allocated by, and owned by the ModelOldSVM
 */
ModelOldSVM::~ModelOldSVM ()
{
  log.Level (20) << "ModelOldSVM::~ModelOldSVM   Strating Destructor for Model[" << rootFileName << "]" << endl;

  delete  svmModel;
  svmModel = NULL;

  delete  assignments;
  assignments = NULL;
}


KKU::int32  ModelOldSVM::MemoryConsumedEstimated ()  const
{
  KKU::int32  memoryConsumedEstimated = Model::MemoryConsumedEstimated () +
                                        sizeof (ClassAssignmentsPtr) +
                                        sizeof (SVMModelPtr);

  if  (assignments)   memoryConsumedEstimated += assignments->MemoryConsumedEstimated ();
  if  (svmModel)      memoryConsumedEstimated += svmModel->MemoryConsumedEstimated ();
  return  memoryConsumedEstimated;
}


ModelOldSVMPtr  ModelOldSVM::Duplicate ()  const
{
  return new ModelOldSVM (*this);
}


KKStr  ModelOldSVM::Description ()  const
{
  KKStr  result = "SVM(" + Name () + ")";

  if  (svmModel)
  {
    const SVMparam&  p = svmModel->SVMParameters ();
    result << " " << MachineTypeToStr (p.MachineType ())
           << " " << SelectionMethodToStr (p.SelectionMethod ());
  }
  return  result;
}



const ClassAssignments&  ModelOldSVM::Assignments ()  const
{
  return svmModel->Assignments ();
}



FeatureNumList  ModelOldSVM::GetFeatureNums (MLClassConstPtr  class1,
                                             MLClassConstPtr  class2
                                            )
{
  return svmModel->GetFeatureNums (class1, class2);
}  /* GetFeatureNums */




const FeatureNumList&   ModelOldSVM::GetFeatureNums ()  const
{
  return svmModel->GetFeatureNums ();
}



int32  ModelOldSVM::NumOfSupportVectors () const
{
  return svmModel->NumOfSupportVectors ();
}



void  ModelOldSVM::SupportVectorStatistics (int32&  numSVs,
                                            int32&  totalNumSVs
                                           )
{
  return  svmModel->SupportVectorStatistics (numSVs, totalNumSVs);
}


ModelParamOldSVMPtr   ModelOldSVM::Param () const
{
  if  (param == NULL)
  {
    KKStr errMsg = "ModelOldSVM::Param   ***ERROR***  param not defined (param == NULL).";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  if  (param->ModelParamType () != ModelParam::mptOldSVM)
  {
    KKStr errMsg = "ModelOldSVM::Param   ***ERROR***  param variable of wrong type.";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  return  dynamic_cast<ModelParamOldSVMPtr> (param);
}



SVM_SelectionMethod  ModelOldSVM::SelectionMethod () const
{
  return  svmModel->SelectionMethod ();
}




void  ModelOldSVM::WriteSpecificImplementationXML (ostream& o)
{
  log.Level (20) << "ModelOldSVM::WriteSpecificImplementationXML  Saving Model in File." << endl;

  bool successful = true;
  o << "<ModelOldSVM>" << endl;
  svmModel->Write (o, rootFileName, successful);
  o << "</ModelOldSVM>" << endl;

} /* WriteSpecificImplementationXML */





void  ModelOldSVM::ReadSpecificImplementationXML (istream&  i,
                                                  bool&     _successful
                                                 )
{
  char  buff[20480];

  delete  svmModel;  svmModel = NULL;

  while (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);
    if  (ln.Len () < 1)
      continue;

    if  ((ln[0] == '/')  &&  (ln[1] == '/'))
      continue;

    KKStr  lineName = ln.ExtractToken2 ("\t\n\r");
    if  (lineName.EqualIgnoreCase ("</ModelOldSVM>"))
      break;

    if  (lineName.EqualIgnoreCase ("<SVMModel>"))
    {
      delete  svmModel;
      svmModel = NULL;

      try
      {
        svmModel = new SVMModel (i, _successful, fileDesc, log, cancelFlag);
      }
      catch (...)
      {
        log.Level (-1) << endl << endl << "ModelOldSVM::ReadSpecificImplementationXML   ***ERROR***  Exception occured calling 'new SVMModel'." << endl << endl;
        validModel = false;
        _successful = false;
      }

      if  (!_successful)
      {
        validModel = false;
        log.Level (-1) << endl << endl << "ModelOldSVM::ReadSpecificImplementationXML   ***ERROR***  Could not loadf model in SVMModel::SVMModel." << endl << endl;
      }
    }
  }

  if  (_successful  ||  validModel)
  {
    if  (svmModel == NULL)
    {
      _successful = false;
      validModel  = false;

      log.Level (-1) << endl << endl
        << "ModelOldSVM::ReadSpecificImplementationXML  ***ERROR***   'svmModel' was not defined." << endl
        << endl;
    }
  }

}  /* ReadSpecificImplementationXML */




void   ModelOldSVM::Predict (FeatureVectorPtr     example,
                             MLClassConstPtr   knownClass,
                             MLClassConstPtr&  predClass,
                             MLClassConstPtr&  predClass2,
                             int32&               predClass1Votes,
                             int32&               predClass2Votes,
                             double&              probOfKnownClass,
                             double&              predClassProb,
                             double&              predClass2Prob,
                             double&              compact,
                             int32&               numOfWinners,
                             bool&                knownClassOneOfTheWinners,
                             double&              breakTie
                            )
{
  bool  newExampleCreated = false;
  FeatureVectorPtr  encodedExample = PrepExampleForPrediction (example, newExampleCreated);
  svmModel->Predict (encodedExample, knownClass, predClass, predClass2,
                     predClass1Votes,  predClass2Votes,
                     probOfKnownClass, 
                     predClassProb,  predClass2Prob,
                     compact,
                     numOfWinners,
                     knownClassOneOfTheWinners,
                     breakTie
                    );
  if  (newExampleCreated)
  {
    delete encodedExample;
    encodedExample = NULL;
  }
  
  return;
}  /* Predict */





MLClassConstPtr  ModelOldSVM::Predict (FeatureVectorPtr  example)
{
  bool  newExampleCreated = false;
  FeatureVectorPtr  encodedExample = PrepExampleForPrediction (example, newExampleCreated);
  MLClassConstPtr  c = svmModel->Predict (example);

  if  (newExampleCreated)
  {
    delete encodedExample;
    encodedExample = NULL;
  }

  return c;
}  /* Predict */




ClassProbListPtr  ModelOldSVM::ProbabilitiesByClass (FeatureVectorPtr  example)
{
  if  (!svmModel)
  {
    KKStr errMsg = "ModelOldSVM::ProbabilitiesByClass   ***ERROR***      (svmModel == NULL)";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  bool  newExampleCreated = false;
  FeatureVectorPtr  encodedExample = PrepExampleForPrediction (example, newExampleCreated);
  svmModel->ProbabilitiesByClass (encodedExample, *classes, votes, classProbs);

  if  (newExampleCreated)
  {
    delete encodedExample;
    encodedExample = NULL;
  }
  
  ClassProbListPtr  results = new ClassProbList ();
  int32  idx = 0;
  for  (idx = 0;  idx < numOfClasses;  idx++)
  {
    MLClassConstPtr  ic = classes->IdxToPtr (idx);
    results->PushOnBack (new ClassProb (ic, classProbs[idx], (float)votes[idx]));
  }

  if  (svmModel->SVMParameters ().SelectionMethod () == SelectByVoting)
    results->SortByVotes (true);
  else
    results->SortByProbability (true);

  return  results;
}  /* ProbabilitiesByClass */






 /**
  ******************************************************************************************************************
  *@brief  Will get the probabilities assigned to each class.
  *@param[in]  example unknown example that we want to get predicted probabilities for. 
  *@param[in]  _mlClasses  List classes that caller is aware of.  This should be the same list that 
  *                        was used when consttructing this ModelOldSVM object.  The list must be the 
  *                        same but not nessasarily in the same order as when ModelOldSVM was 1st 
  *                        constructed.
  *@param[out] _probabilities  An array that must be as big as the number of classes in _mlClasses.  
  *                            The probability of class in _mlClasses[x] will be returned in _probabilities[x].
  */
void  ModelOldSVM::ProbabilitiesByClass (FeatureVectorPtr         _example,
                                         const MLClassConstList&  _mlClasses,
                                         double*                  _probabilities
                                        )
{
  bool  newExampleCreated = false;
  FeatureVectorPtr  encodedExample = PrepExampleForPrediction (_example, newExampleCreated);

  svmModel->ProbabilitiesByClass (encodedExample, _mlClasses, votes, _probabilities);
  if  (newExampleCreated)
  {
    delete encodedExample;
    encodedExample = NULL;
  }

  return;
}



 /**
  ******************************************************************************************************************
  *@brief  Will get the probabilities assigned to each class.
  *@param[in]  example unknown example that we want to get predicted probabilities for. 
  *@param[in]  _mlClasses  List classes that caller is aware of.  This should be the same list that 
  *                        was used when consttructing this ModelOldSVM object.  The list must be the 
  *                        same but not nessasarily in the same order as when ModelOldSVM was 1st 
  *                        constructed.
  *@param[out] _votes        Number of votes for eacgh class.
  *@param[out] _probabilities  An array that must be as big as the number of classes in _mlClasses.  
  *                            The probability of class in _mlClasses[x] will be returned in _probabilities[x].
  */
 void  ModelOldSVM::ProbabilitiesByClass (FeatureVectorPtr         example,
                                          const MLClassConstList&  _mlClasses,
                                          int32*                   _votes,
                                          double*                  _probabilities
                                         )
{
  bool  newExampleCreated = false;
  FeatureVectorPtr  encodedExample = PrepExampleForPrediction (example, newExampleCreated);
  svmModel->ProbabilitiesByClass (encodedExample, _mlClasses, _votes, _probabilities);
  if  (newExampleCreated)
  {
    delete encodedExample;
    encodedExample = NULL;
  }

  return;
}  /* ProbabilitiesByClass */







vector<KKStr>  ModelOldSVM::SupportVectorNames (MLClassConstPtr  c1,
                                                MLClassConstPtr  c2
                                               )  const
{
  return  svmModel->SupportVectorNames (c1, c2);
}  /* SupportVectorNames */







vector<KKStr>  ModelOldSVM::SupportVectorNames () const
{
  return  svmModel->SupportVectorNames ();
}  /* SupportVectorNames */






/**
 *@brief  For a given two class pair return the names of the 'numToFind' worst S/V's.
 *@details  This method will iterate through all the S/V's removing them one at a 
 *           time and recompute the decision boundary and probability.  It will then
 *           return the S/V's that when removed improve the probability in 'c1's 
 *           the most.
 *@param[in]  example  Example that was classified incorrectly.
 *@param[in]  numToFind  The number of teh worst examples you are looking for.
 *@param[in]  c1  Class that the 'example; parameter should have been classed as.
 *@param[in]  c2  Class that it was classified as.
 */
vector<ProbNamePair>  ModelOldSVM::FindWorstSupportVectors (FeatureVectorPtr    example,
                                                            int32               numToFind,
                                                            MLClassConstPtr  c1,
                                                            MLClassConstPtr  c2
                                                           )
{
  return  svmModel->FindWorstSupportVectors (example, numToFind, c1, c2);
}  /* FindWorstSupportVectors */




/**
 *@brief  For a given two class pair return the names of the 'numToFind' worst S/V's.
 *@details  This method will iterate through all the S/V's removing them one at a 
 *          time and retraining a new SVM and then comparing with the new prediction results.
 *@param[in]  example  Example that was classified incorrectly.
 *@param[in]  numToFind  The number of teh worst examples you are looking for.
 *@param[in]  c1  Class that the 'example; parameter should have been classed as.
 *@param[in]  c2  Class that it was classified as.
 */
vector<ProbNamePair>  ModelOldSVM::FindWorstSupportVectors2 (FeatureVectorPtr    example,
                                                             int32               numToFind,
                                                             MLClassConstPtr  c1,
                                                             MLClassConstPtr  c2
                                                            )
{
  return  svmModel->FindWorstSupportVectors2 (example, numToFind, c1, c2);
}  /* FindWorstSupportVectors2 */





bool  ModelOldSVM::NormalizeNominalAttributes ()  const
{
  return  svmModel->NormalizeNominalAttributes ();
}  /* NormalizeNominalAttributes */




void  ModelOldSVM::RetrieveCrossProbTable (MLClassConstList&  classes,
                                           double**              crossProbTable  // two dimension matrix that needs to be classes.QueueSize ()  squared.
                                          )
{
  svmModel->RetrieveCrossProbTable (classes, crossProbTable);
  return;
}  /* RetrieveCrossProbTable */



/**
 *@brief Use given traiing data to create a trained Model that can be used for classifying examples.
 *@param[in] _trainExamples      The example data we will be building the model from.
 *@param[in] _alreadyNormalized  Specifies weather the training data has already been normalized.
 *@param[in] _takeOwnership      If true this instance will take ownership of '_trainExamples' and delete it when done with it.
 */
void  ModelOldSVM::TrainModel (FeatureVectorListPtr  _trainExamples,
                               bool                  _alreadyNormalized,
                               bool                  _takeOwnership  /*!< Model will take ownership of these examples */
                              )
{
  log.Level (10) << "ModelOldSVM::TrainModel  Model[" << rootFileName << "]" << endl;
  // We do not bother with the base class 'TrainModel' like we do with other models.
  // 'ModelOldSVM' is a special case.  All we are trying to do is create a pass through
  // for 'svmModel'.

  delete  svmModel;
  svmModel = NULL;

  Model::TrainModel (_trainExamples, _alreadyNormalized, _takeOwnership);
  // The "Model::TrainModel" may have manipulated the '_trainExamples'.  It will have also 
  // updated 'Model::trainExamples.  So from this point forward we use 'trainExamples'.
  _trainExamples = NULL;


  SVMparamPtr svmParam = Param ()->SvmParameters ();
  if  (!svmParam)
  {
    validModel = false;
    KKStr errMsg = " ModelOldSVM::TrainModel  ***ERROR***    (svmParam == NULL).";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  {
    delete  classes;
    classes = trainExamples->ExtractMLClassConstList ();
    classes->SortByName ();
    numOfClasses = classes->QueueSize ();
    delete  assignments;
    assignments = new ClassAssignments (*classes, log);
  }

  try
  {
    TrainingTimeStart ();
    svmModel = new SVMModel (*svmParam, *trainExamples, *assignments, fileDesc, log, cancelFlag);
    TrainingTimeEnd ();
    compression_stats = svmModel->Compression_Stats ();
  }
  catch (...)
  {
    log.Level (-1) << endl << endl << "ModelOldSVM::TrainModel  Exception occured building training model." << endl << endl;
    validModel = false;
    delete  svmModel;
    svmModel = NULL;
  }

  if  (weOwnTrainExamples)
  {
    // We are done with the training examples and since we were to take ownership,  we can delete them.
    delete trainExamples;
    trainExamples = NULL;
  }
}  /* TrainModel */





/**
 *@brief  ModelOldSVM Specific 'PrepExampleForPrediction'  will only normalize data.
 *@param[in]  fv  Feature vector of example that needs to be prepared.
 *@param[out]  newExampleCreated  Indicates if either Feature Encoding and/or Normalization needed
 *             to be done.  If neither then the original instance is returned.  If Yes then 
 *             a new instance which the caller will have to be delete will be returned.
 */
FeatureVectorPtr  ModelOldSVM::PrepExampleForPrediction (FeatureVectorPtr  fv,
                                                         bool&             newExampleCreated
                                                        )
{
  FeatureVectorPtr  oldFV = NULL;
  newExampleCreated = false;
  if  ((!alreadyNormalized)  &&  (normParms))
  {
    oldFV = fv;
    fv = normParms->ToNormalized (fv);
    if  (newExampleCreated)
    {
      delete  oldFV;
      oldFV = NULL;
    }
    newExampleCreated = true;
  }

  // Since 'SvmModel' will be doing the encoding we do not need to do it here.

  return  fv;
}  /* PrepExampleForPrediction */

