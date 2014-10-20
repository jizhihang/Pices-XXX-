#include  "FirstIncludes.h"

//**********************************************************************************
//*                                                                                *
//*                                                                                *
//*  Provides an interface to the svm.cpp functions.  This was copyied from a ver- *
//*  sion written by Tong Luo.  Received from Tong June-2-2003.                    *
//*                                                                                *
//*                                                                                *
//*  Prog           Date         Description                                       *
//*                                                                                *
//*  Kurt Kramer  June-02-2003   Initial development.  Started with svm-train as   *
//*                              given to me by Tong Luo on 6/2/2003.  Supports    *
//*                              functionality that Tong added to the svm.cpp      *
//*                              routines.                                         *
//*                                                                                *
//*  Kurt Kramer  2004-12-22     Adding probabilities[] array to SvmPredictClass   *
//*                              parameter list.  This will allow us to pass the   *
//*                              probabilities assined to each class back to the   *
//*                              caller, which should be SvmModel.                 *
//*                                                                                *
//*                                                                                *
//**********************************************************************************

 

#include <stdio.h>
#include <time.h>

#include <string>

#include <ctype.h>

#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

#ifdef  WIN32
#include <ostream>
#endif


#include <fstream>
#include <set>
#include <math.h>
#include <string>
#include <sstream>
#include <map>
using namespace std;



#include  "MemoryDebug.h"
#include  "BasicTypes.h"
#include  "OSservices.h"
using namespace KKU;



#include  "SvmWrapper.h"


#include  "MLLTypes.h"
#include  "svm.h"



using namespace MLL;





#ifdef  WIN32

#include  "float.h"

//inline
//long  max  (long  x, long y)
//{
//  if  (x > y)
//    return  x;
//  else
//    return  y;
//}
#endif





#define Malloc (type,n) (type *)malloc((n)*sizeof(type))




void do_cross_validation ();
//luo add



void saveData (svm_problem  ds, 
               int32          begin, 
               int32          end, 
               std::string  name
              );



template<class T>
int32  GetMaxIndex (vector<T>&   vote, 
                    int32        voteLength,
                    int32&       maxIndex2  // second highest indx
                   )
{
  T max=vote[0];
  int32 maxIndex=0;

  T  max2 = 0;
  maxIndex2 = -1;

  for  (int32 i = 1;  i < voteLength;  i++)
  {
    if  (vote[i]> max)
    {
      max2      = max;
      maxIndex2 = maxIndex;
      max       = vote[i];
      maxIndex  = i;
    }

    else if  (maxIndex2 < 0)
    {
      max2      = vote[i];
      maxIndex2 = i;
    }

    else if  (vote[i] > max2)
    {
      max2 = vote[i];
      maxIndex2 = i;
    }
  }

  return maxIndex;
}  /* GetMaxIndex */




template<class T>
int32  GetMaxIndex (T*       vote, 
                    int32    voteLength,
                    int32&   maxIndex2  // second highest indx
                   )
{
  T max=vote[0];
  int32 maxIndex=0;

  T  max2 = 0;
  maxIndex2 = -1;

  for  (int32 i = 1;  i < voteLength;  i++)
  {
    if  (vote[i]> max)
    {
      max2      = max;
      maxIndex2 = maxIndex;
      max       = vote[i];
      maxIndex  = i;
    }

    else if  (maxIndex2 < 0)
    {
      max2      = vote[i];
      maxIndex2 = i;
    }

    else if  (vote[i] > max2)
    {
      max2 = vote[i];
      maxIndex2 = i;
    }
  }

  return maxIndex;
}  /* GetMaxIndex */







bool  GreaterThan (int32     leftVotes,
                   double  leftProb,
                   int32     rightVotes,
                   double  rightProb
                  )
{
  if  (leftVotes < rightVotes)
    return false;

  else if  (leftVotes > rightVotes)
    return true;

  else if  (leftProb < rightProb)
    return false;
  
  else if  (leftProb > rightProb)
    return  true;
  
  else
    return  false;
}  /* GreaterThan */



void  GreaterVotes (bool    useProbability,
                    int32   numClasses,
                    int32*  votes,
                    double* probabilities,
                    int32&  pred1Idx,
                    int32&  pred2Idx
                   )
{
  if  (useProbability)
  {
    pred1Idx = GetMaxIndex (probabilities, numClasses, pred2Idx);
    return;
  }

  int32     max1Votes = votes[0];
  double  max1Prob  = probabilities[0];
  pred1Idx = 0;

  int32     max2Votes = -1;
  double  max2Prob  = -1.0f;
  pred2Idx = -1;

  for  (int32 x = 1;  x < numClasses;  x++)
  {
    if  (GreaterThan (votes[x], probabilities[x], max1Votes, max1Prob))
    {
      max2Votes = max1Votes;
      max2Prob  = max1Prob;
      pred2Idx  = pred1Idx;
      max1Votes = votes[x];
      max1Prob  = probabilities[x];
      pred1Idx = x;
    }
    else if  ((pred2Idx < 0)  ||  GreaterThan (votes[x], probabilities[x], max2Votes, max2Prob))
    {
      max2Votes = votes[x];
      max2Prob  = probabilities[x];
      pred2Idx = x;
    }
  }
} /* GreaterVotes*/







void saveData (svm_problem  ds, 
               int32          begin, 
               int32          end, 
               string  name
              )
{
  ofstream out(name.c_str());
  if(!out.good())
  {
    cout << " cannot open " << name << endl;
    exit(-1);
  }
  for(int32 i=begin; i<end; i++)
  {
    svm_node* temp=ds.x[i];
    while(temp->index!=-1)
    {
      out << temp->value << ",";
      temp++;
    }
    out << ds.y[i] << endl;
  }
  out.close();
}






void  ComputeProb  (int32                numClasses,               // Number of Classes
                    float              A,                      // probability parameter
                    vector<double>&    dist,                   // Distances for each binary classifier from decision boundary.
                    double**           crossClassProbTable,    // A 'numClass' x 'numClass' matrix;  will get the probabilities between classes.
                    int32*               votes,                  // votes by class
                    double*            probabilities,          // Probabilities for Each Class
                    int32                knownClassNum,          // -1 = Don't know the class otherwise the Number of the Class.
                    double             confidence,             // Used for calculating 'compact'  probability must exceed this 
                    double&            compact                 // 'knownClassNum'  and  'confidence'  need to be provided
                   )
{
  compact = 0.0;

  int32  i;
  for  (i = 0;  i < numClasses;  i++)
    votes[i] = 0;

   int32 distIdx = 0;
   for  (i = 0;  i < (numClasses - 1); i++)
   {
     for  (int32 j = i + 1;  j < numClasses;  j++)
     {
       if  (dist[distIdx] > 0)
         votes[i]++;
       else
         votes[j]++;

       double tempProb = (double)(1.0 / (1.0 + exp (-1.0 * double (A) * dist[distIdx])));
       crossClassProbTable[i][j] = tempProb;
       crossClassProbTable[j][i] = (1.0 - tempProb);
       distIdx++;
     }
   }

   double  totalProb = 0.0;
   for  (i = 0;  i < numClasses;  i++)
   {
     double  probThisClass = 1.0;
     for  (int32 j = 0;  j < numClasses;  j++)
     {
       if  (i != j)
         probThisClass *= crossClassProbTable [i][j];
     }

     probabilities[i] = probThisClass;
     totalProb += probThisClass;
   }

   if  (totalProb == 0.0)
   {
     // I think this happens because we are using float pfor probTable and double for dist[]
     // For now we will give each class an equal probability.
     for  (i = 0; i < numClasses; i++)
       probabilities[i] = (1.0 / double(numClasses));
   }
   else
   {
     for  (i = 0;  i < numClasses;  i++)
       probabilities[i] = probabilities[i] / totalProb;
   }

   if  ((knownClassNum >= 0) &&  (knownClassNum < numClasses))
   {
     int32 maxIndex1 = -1;
     int32 maxIndex2 = -1;
     maxIndex1 = GetMaxIndex (probabilities, numClasses, maxIndex2);

     if  (probabilities[maxIndex1] >= confidence)
     {
       if  ((probabilities[knownClassNum] < 1.0f)  &&  (probabilities[knownClassNum] > 0.0f))
       {
         compact = -log ((double)probabilities[knownClassNum]);
       }
     }
   }
}  /* ComputeProb */






struct svm_model**  MLL::SvmTrainModel (const struct svm_parameter&  param,
                                        struct       svm_problem&    subprob
                                       )
{ 
  struct svm_model **submodel;

  int32 numSVM = param.numSVM;
  int32 sample = (int32) (param.sample);

  //int32 numClass=param.numClass;
  int32    sampleSV   = param.sampleSV;
  int32    boosting   = param.boosting;
  int32    dimSelect  = param.dimSelect;

  Learn_Type learnType;

  if  ((numSVM == 1)  &&  (sample == 100))
    learnType= NORMAL;

  else if  (dimSelect > 0)
    learnType = SUBSPACE;

  else if (boosting != 0)
    learnType=BOOSTING;

  else if(sampleSV!=0)
    learnType=SAMPLESV;

  else
    learnType=BAGGING;

  submodel = new svm_model* [numSVM];
  submodel[0] = svm_train (&subprob,  &param);

  return  submodel;
}  /* SvmTrainModel */








void   MLL::SvmPredictClass (SVMparam&               svmParam,
                             struct svm_model**      subModel,
                             const struct svm_node*  unknownClassFeatureData, 
                             int32*                  votes,
                             double*                 probabilities,
                             int32                   knownClass,
                             int32&                  predClass1,
                             int32&                  predClass2,
                             int32&                  predClass1Votes,
                             int32&                  predClass2Votes,
                             double&                 predClass1Prob,
                             double&                 predClass2Prob,
                             double&                 probOfKnownClass,
                             double&                 compact,
                             Ivector&                winners,
                             double**                crossClassProbTable,
                             double&                 breakTie
                            )
{
  const struct svm_parameter&  param = svmParam.Param ();

  compact = 0.0;

  int32  NUMCLASS = subModel[0][0].nr_class;

  int32 numBinary = (NUMCLASS * (NUMCLASS - 1)) / 2;
  Dvector dist (numBinary, 0);


  int32  maxIndex = int32 (svm_predict (subModel[0], unknownClassFeatureData, dist, winners, -1));
  ComputeProb  (NUMCLASS,
                param.A,
                dist,                   // Distances for each binary classifier from decision boundary.
                crossClassProbTable,    // Will get Probabilities between classes.
                votes,
                probabilities,          // Probabilities for Each Class
                knownClass,             // -1 = Don't know the class otherwise the Number of the Class.
                0.9,                    // Used for calculating 'compact'  probability of predicted class must exceed this 
                compact             
               );


  GreaterVotes ((svmParam.SelectionMethod () == SelectByProbability),
                NUMCLASS,
                votes,
                probabilities,
                predClass1,
                predClass2
               );
  if  (predClass1 >= 0)
  {
    predClass1Votes    = votes[predClass1];
    predClass1Prob   = probabilities[predClass1];
  }

  if  (predClass2 >= 0)
  {
    predClass2Votes    = votes[predClass2];
    predClass2Prob   = probabilities[predClass2];
  }

  if  (knownClass >= 0)
    probOfKnownClass   = probabilities[knownClass];

  breakTie = (predClass1Prob - predClass2Prob);
}  /* SvmPredictClass */






int32  MLL::SvmPredictTwoClass (const struct svm_parameter&   param,
                              svm_model**                   submodel, 
                              const svm_node*               unKnownData, 
                              int32                           desired, 
                              double&                       dist,
                              double&                       probability,
                              int32                           excludeSupportVectorIDX
                             )
{
  if  (submodel[0]->nr_class != 2)
  {
    cerr << endl
         << endl
         << "SvmPredictTwoClass    *** ERROR ***" << endl
         << endl
         << "Number of classes should be equal to two." << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }


  int32  v = int32 (svm_predictTwoClasses (submodel[0], unKnownData, dist, excludeSupportVectorIDX));

  probability = (1.0 / (1.0 + exp (-1.0 * param.A * dist)));

  return  v;
}  /* SvmPredictTwoClass */






void  MLL::SvmSaveModel (struct svm_model**  subModel,
                         const char*         fileName,
                         bool&               successfull
                        )
{
  successfull = true;

  int32 x = svm_save_model (fileName, subModel[0]);
  successfull = (x == 0);
}



void  MLL::SvmSaveModel (ostream&             o,
                         struct  svm_model**  model
                        )
{
   Svm_Save_Model (o, model[0]);
}





struct svm_model**   MLL::SvmLoadModel (const char* fileName)
{
  svm_model**  models = new svm_model*[1];
  models[0] = svm_load_model(fileName);

  if  (models[0] == NULL)
  {
    delete  models;
    models = NULL;
  }
  return models;
}



struct svm_model**   MLL::SvmLoadModel (istream&  f,
                                        RunLog&   log
                                       )
{
  svm_model**  models = new svm_model*[1];
  models[0] = Svm_Load_Model (f, log);

  if  (models[0] == NULL)
  {
    delete  models;
    models = NULL;
  }

  return models;
}





void   MLL::SvmDestroyModel (struct svm_model**  subModel)
{
  svm_destroy_model (subModel[0]);
}
