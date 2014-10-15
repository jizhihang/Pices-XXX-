#include  "FirstIncludes.h"

//*******************************************************************
//*                           Classifier2                            *
//*******************************************************************

#include <ctype.h>
#include <stdio.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "MemoryDebug.h"


#ifdef WIN32
#include <windows.h>
#else
#endif

using namespace  std;


#include "BasicTypes.h"
#include "RunLog.h"
using namespace  KKU;


#include "Classifier2.h"
#include "MLClass.h"
#include "MLClassConstList.h"
#include "NormalizationParms.h"
#include "TrainingProcess2.h"
using namespace  MLL;



Classifier2::Classifier2 (TrainingProcess2Ptr  _trainer,
                          RunLog&              _log
                         ):

  abort                     (false),
  featuresAlreadyNormalized (false),
  mlClasses              (NULL),
  log                       (_log),
  trainedModel              (NULL),
  trainedModelOldSVM        (NULL),
  trainedModelSVMModel      (NULL),
  noiseMLClass           (NULL),
  trainingProcess           (_trainer),
  unKnownMLClass         (NULL)
{
  if  (!_trainer)
  {
    log.Level (-1) << endl
      << "Classifier2::Classifier2    ***ERROR***     (_trainer == NULL)" << endl
      << endl;
    throw KKStrException ("Classifier2::Classifier2    ***ERROR***     (_trainer == NULL)");
  }


  if  (_trainer->Abort ())
  {
    log.Level (-1) << endl
      << "Classifier2::Classifier2    ***ERROR***     '_trainer' is invalid." << endl
      << endl;
    throw KKStrException ("Classifier2::Classifier2    ***ERROR***     '_trainer' is invalid.");
  }

  featuresAlreadyNormalized = trainingProcess->FeaturesAlreadyNormalized ();
  mlClasses = new MLClassConstList (*(_trainer->MLClasses ()));
  trainedModel = _trainer->TrainedModel ();
  if  (trainedModel == NULL)
  {
    KKStr errMsg = "Classifier2::Classifier2    ***ERROR***     (trainedModel == NULL).";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  if  (!trainedModel->ValidModel ())
  {
    KKStr errMsg = "Classifier2::Classifier2    ***ERROR***     trainedModel is not valid.";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  log.Level (20) << "Classifier2::Classifier2" << endl;
  noiseMLClass   = mlClasses->GetNoiseClass ();
  unKnownMLClass = mlClasses->GetUnKnownClass ();

  if  (trainedModel->ModelType () == Model::mtOldSVM)
  {
    trainedModelOldSVM = dynamic_cast<ModelOldSVMPtr> (trainedModel);
    if  (trainedModelOldSVM)
      trainedModelSVMModel = trainedModelOldSVM->SvmModel ();
  }
}



Classifier2::~Classifier2 ()
{
  delete  mlClasses;
}


int32  Classifier2::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (*this);
  if  (mlClasses)  memoryConsumedEstimated += mlClasses->MemoryConsumedEstimated ();
  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */



SVM_SelectionMethod   Classifier2::SelectionMethod ()  const
{
  if  (!trainedModelOldSVM)
    return   SelectionMethod_NULL;
  else
    return  trainedModelOldSVM->SelectionMethod ();
}





MLClassConstPtr  Classifier2::ClassifyAImageOneLevel (FeatureVector&  example,
                                                         double&         probability,
                                                         int32&          numOfWinners, 
                                                         bool&           knownClassOneOfTheWinners,
                                                         double&         breakTie
                                                        )

{
  double  compact;

  probability       = 0.0;

  double  probOfKnownClass = 0.0f;

  MLClassConstPtr  origClass       = example.MLClass ();
  MLClassConstPtr  predictedClass  = NULL;
  MLClassConstPtr  predictedClass2 = NULL;

  int32            class1Votes = -1;
  int32            class2Votes = -1;

  double         predictedClass2Prob = 0.0f;

  trainedModel->Predict (&example, 
                         origClass,
                         predictedClass,
                         predictedClass2,
                         class1Votes,
                         class2Votes,
                         probOfKnownClass,
                         probability,
                         predictedClass2Prob,
                         compact,
                         numOfWinners,
                         knownClassOneOfTheWinners,
                         breakTie
                        );

  if  (predictedClass == NULL)
  {
    log.Level (-1) << endl << endl 
                   << "Classifier2::ClassifyAImageOneLevel   The trainedModel returned back a NULL ptr for predicted class" << endl
                   << endl;
    predictedClass = unKnownMLClass;
  }


  if  (predictedClass->UnDefined ())
  {
    predictedClass = noiseMLClass;
    example.MLClass (noiseMLClass);
  }
  else
  {
    example.MLClass (predictedClass);
  }

  return  predictedClass;
}  /* ClassifyAImageOneLevel */




MLClassConstPtr  Classifier2::ClassifyAImageOneLevel (FeatureVector&  example)
{
  double  probability;
  bool    knownClassOneOfTheWinners;
  int32     numOfWinners;
  double  breakTie;

  return  ClassifyAImageOneLevel (example, 
                                  probability, 
                                  numOfWinners,
                                  knownClassOneOfTheWinners,
                                  breakTie
                                );
}  /* ClassifyAImageOneLevel */




MLClassConstPtr  Classifier2::ClassifyAImageOneLevel (FeatureVector&  example,
                                                         int32&          numOfWinners,
                                                         bool&           knownClassOneOfTheWinners
                                                        )
{
  double   probability;
  double   breakTie;

  return  ClassifyAImageOneLevel (example, 
                                  probability, 
                                  numOfWinners, 
                                  knownClassOneOfTheWinners,
                                  breakTie
                                );
}  /* ClassifyAImageOneLevel */







void  Classifier2::ClassifyAImage (FeatureVector&       example,
                                   MLClassConstPtr&  predClass1,
                                   MLClassConstPtr&  predClass2,
                                   int32&               predClass1Votes,
                                   int32&               predClass2Votes,
                                   double&              knownClassProb,
                                   double&              predClass1Prob,
                                   double&              predClass2Prob,
                                   int32&               numOfWinners,
                                   double&              breakTie,
                                   double&              compact
                                  )
{
  bool   knownClassOneOfTheWiners = false;

  compact        = -1.0;
  predClass1     = NULL;
  predClass2     = NULL;
  knownClassProb = -1.0f;
  predClass1Prob = -1.0f;
  predClass2Prob = -1.0f;

  MLClassConstPtr origClass  = example.MLClass ();


  trainedModel->Predict (&example,
                         origClass,
                         predClass1,
                         predClass2,
                         predClass1Votes,
                         predClass2Votes,
                         knownClassProb,
                         predClass1Prob,
                         predClass2Prob,
                         compact,
                         numOfWinners,
                         knownClassOneOfTheWiners,
                         breakTie
                        );

  if  (!predClass1)
  {
    predClass1 = noiseMLClass;
    example.MLClass (noiseMLClass);
  }
  else
  {
    example.MLClass (predClass1);
  }

  return;
}  /* ClassifyAImage */





MLClassConstPtr  Classifier2::ClassifyAImage (FeatureVector&  example,
                                                 double&         probability,
                                                 int32&          numOfWinners,
                                                 bool&           knownClassOneOfTheWinners,
                                                 double&         breakTie
                                                )
{
  MLClassConstPtr  predictedClass;

  probability       = 0.0;

  predictedClass = ClassifyAImageOneLevel (example, 
                                           probability, 
                                           numOfWinners, 
                                           knownClassOneOfTheWinners,
                                           breakTie
                                          );
  return  predictedClass;
}  /* ClassifyAImage */




MLClassConstPtr  Classifier2::ClassifyAImage (FeatureVector&  example,
                                                 int32&          numOfWinners,
                                                 bool&           knownClassOneOfTheWinners
                                                )
{
  MLClassConstPtr  predictedClass = NULL;

  // Lets first Normalize Feature Data.

  predictedClass = ClassifyAImageOneLevel (example, numOfWinners, knownClassOneOfTheWinners);

  return  predictedClass;
} /* ClassifyAImage */




MLClassConstPtr  Classifier2::ClassifyAImage (FeatureVector&  example)
{
  int32   numOfWinners;
  bool  knownClassOneOfTheWinners;
  return  ClassifyAImage (example, numOfWinners, knownClassOneOfTheWinners);
}




vector<KKStr>  Classifier2::SupportVectorNames (MLClassConstPtr  c1,
                                                MLClassConstPtr  c2
                                               )
{
  if  (!trainedModelSVMModel)
  {
    vector<KKStr>  results;
    return  results;
  }
  else
  {
    return  trainedModelSVMModel->SupportVectorNames (c1, c2);
  }
}





vector<ProbNamePair>  Classifier2::FindWorstSupportVectors (FeatureVectorPtr    example,
                                                            int32               numToFind,
                                                            MLClassConstPtr  c1,
                                                            MLClassConstPtr  c2
                                                           )
{
  if  (!trainedModelSVMModel)
  {
    vector<ProbNamePair>  results;
    return  results;
  }

  return  trainedModelSVMModel->FindWorstSupportVectors (example, numToFind, c1, c2);
}

 


vector<ProbNamePair>  Classifier2::FindWorstSupportVectors2 (FeatureVectorPtr   example,
                                                             int32               numToFind,
                                                             MLClassConstPtr  c1,
                                                             MLClassConstPtr  c2
                                                            )
{
  if  (!trainedModelSVMModel)
  {
    vector<ProbNamePair>  results;
    return  results;
  }

  return  trainedModelSVMModel->FindWorstSupportVectors2 (example, numToFind, c1, c2);
}

 


void  Classifier2::ProbabilitiesByClass (const MLClassConstList& classes,
                                         FeatureVectorPtr           example,
                                         int32*                     votes,
                                         double*                    probabilities
                                        )
{
  if  (trainedModel)
  {
    trainedModel->ProbabilitiesByClass (example, classes, votes, probabilities);
  }
}  /* ProbabilitiesByClass */




ClassProbListPtr  Classifier2::ProbabilitiesByClass (FeatureVectorPtr  example)
{
  if  (trainedModel)
    return trainedModel->ProbabilitiesByClass (example);
  else
    return NULL;
}  /* ProbabilitiesByClass */




void  Classifier2::RetrieveCrossProbTable (MLClassConstList&  classes,
                                           double**              crossProbTable  // two dimension matrix that needs to be classes.QueueSize ()  squared.
                                          )
{
  if  (trainedModel)
    trainedModel->RetrieveCrossProbTable (classes, crossProbTable);
}




void  Classifier2::ProbabilitiesByClassDual (FeatureVectorPtr   example,
                                             KKStr&             classifier1Desc,
                                             KKStr&             classifier2Desc,
                                             ClassProbListPtr&  classifier1Results,
                                             ClassProbListPtr&  classifier2Results
                                            )
{
  if  (trainedModel)
    trainedModel->ProbabilitiesByClassDual (example, classifier1Desc, classifier2Desc, classifier1Results, classifier2Results);
}
