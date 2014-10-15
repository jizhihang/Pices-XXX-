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


#include "SVMModel.h"
#include "MLLTypes.h"
#include "BitReduction.h"
#include "BinaryClassParms.h"
#include "FeatureEncoder.h"
#include "FeatureNumList.h"
#include "FeatureVector.h"
#include "SvmWrapper.h"
using namespace MLL;



template<class T>
void  GetMaxIndex (T*       vote, 
                   int32    voteLength,
                   int32&   maxIndex1,
                   int32&   maxIndex2
                  )
{
  T max1     = vote[0];
  maxIndex1  = 0;

  T  max2    = 0;
  maxIndex2  = -1;

  for  (int32 i = 1;  i < voteLength;  i++)
  {
    if  (vote[i] > max1)
    {
      max2      = max1;
      maxIndex2 = maxIndex1;
      max1      = vote[i];
      maxIndex1 = i;
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

  return;
}  /* GetMaxIndex */





bool  SVMModel::GreaterThan (int32     leftVotes,
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



double  AdjProb (double  prob)
{
  if  (prob < 0.001)
    prob = 0.001;
  else if  (prob > 0.999)
    prob = 0.999;
  return  prob;
}


void  SVMModel::GreaterVotes (bool     useProbability,
                              int32    numClasses,
                              int32*   votes,
                              int32&   numOfWinners,
                              double*  probabilities,
                              int32&   pred1Idx,
                              int32&   pred2Idx
                             )
{
  if  (useProbability)
  {
    GetMaxIndex (probabilities, numClasses, pred1Idx , pred2Idx);
    numOfWinners = 1;
    return;
  }

  int32     max1Votes = votes[0];
  double  max1Prob  = probabilities[0];
  pred1Idx = 0;
  numOfWinners = 1;

  int32     max2Votes = -1;
  double  max2Prob  = -1.0f;
  pred2Idx = -1;

  for  (int32 x = 1;  x < numClasses;  x++)
  {
    if  (votes[x] > max1Votes)
      numOfWinners = 1;

    else if  (votes[x] == max1Votes)
      numOfWinners++;

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



SVMModel::SVMModel (const KKStr&          _rootFileName,   // Create from existing Model on Disk.
                    bool&                 _successful,
                    FileDescPtr           _fileDesc,
                    RunLog&               _log,
                    volatile const bool&  _cancelFlag
                   )
:  
  assignments              (_log),
  binaryFeatureEncoders    (NULL),
  binaryParameters         (NULL),
  cardinality_table        (),
  cancelFlag               (_cancelFlag),
  classIdxTable            (NULL),
  compression_stats        (),
  crossClassProbTable      (NULL),
  crossClassProbTableSize  (0),
  featureEncoder           (NULL),
  fileDesc                 (_fileDesc),
  log                      (_log),
  models                   (NULL),
  numOfClasses             (0),
  numOfModels              (0),
  oneVsAllAssignment       (),
  oneVsAllClassAssignments (NULL),
  predictXSpace            (NULL),
  predictXSpaceWorstCase   (0),
  probabilities            (NULL),
  rootFileName             (_rootFileName),
  selectedFeatures         (_fileDesc),
  svmParam                 (_fileDesc, _log),
  trainingTime             (0.0),
  type_table               (),
  validModel               (true),
  votes                    (NULL),
  xSpaces                  (NULL),
  xSpacesTotalAllocated    (0)

{
  log.Level (20) << "SVMModel::SVMModel - Construction from existing Model File[" 
                 << _rootFileName  << "]."
                 << endl;

  _successful = true;
  validModel = true;

  type_table        = fileDesc->CreateAttributeTypeTable ( );
  cardinality_table = fileDesc->CreateCardinalityTable ( );

  ifstream  in (_rootFileName.Str ());
  if  (!in.is_open ())
  {
    validModel = false;
    log.Level (-1) << endl << endl << "SVMModel::SVMModel   ***ERROR*** Could not open file[" << _rootFileName << "]" << endl << endl;
  }
  else
  {
    Read (in);
  }

  if  ((!validModel)  ||   cancelFlag)
  {
    _successful = false;
  }

  numOfClasses = (int32)assignments.size ();

  BuildClassIdxTable ();
  BuildCrossClassProbTable ();
}


SVMModel::SVMModel (istream&              _in,   // Create from existing Model on Disk.
                    bool&                 _successful,
                    FileDescPtr           _fileDesc,
                    RunLog&               _log,
                    volatile const bool&  _cancelFlag
                   )
:  
  assignments              (_log),
  binaryFeatureEncoders    (NULL),
  binaryParameters         (NULL),
  cancelFlag               (_cancelFlag),
  cardinality_table        (),
  classIdxTable            (NULL),
  compression_stats        (),
  crossClassProbTable      (NULL),
  crossClassProbTableSize  (0),
  featureEncoder           (NULL),
  fileDesc                 (_fileDesc),
  log                      (_log),
  models                   (NULL),
  numOfClasses             (0),
  numOfModels              (0),
  oneVsAllAssignment       (),
  oneVsAllClassAssignments (NULL),
  predictXSpace            (NULL),
  predictXSpaceWorstCase   (0),
  probabilities            (NULL),
  rootFileName             (),
  selectedFeatures         (_fileDesc),
  svmParam                 (_fileDesc, _log),
  trainingTime             (0.0),
  type_table               (),
  validModel               (true),
  votes                    (NULL),
  xSpaces                  (NULL),
  xSpacesTotalAllocated    (0)

{
  log.Level (20) << "SVMModel::SVMModel - Construction from File Stream." << endl;

  _successful = true;

  type_table        = fileDesc->CreateAttributeTypeTable ( );
  cardinality_table = fileDesc->CreateCardinalityTable ( );

  Read (_in);

  if  ((!validModel)  ||   cancelFlag)
  {
    _successful = false;
  }
  else
  {
    numOfClasses = (int32)assignments.size ();
    BuildClassIdxTable ();
    BuildCrossClassProbTable ();
  }
}


SVMModel::SVMModel (SVMparam&             _svmParam,      // Create new model from
                    FeatureVectorList&    _examples,        // Training data.
                    ClassAssignments&     _assignmnets,
                    FileDescPtr           _fileDesc,
                    RunLog&               _log,
                    volatile const bool&  _cancelFlag
                   )
:
  assignments              (_assignmnets),
  binaryFeatureEncoders    (NULL),
  binaryParameters         (NULL),
  cancelFlag               (_cancelFlag),
  cardinality_table        (),
  classIdxTable            (NULL),
  compression_stats        (),
  crossClassProbTable      (NULL),
  crossClassProbTableSize  (0),
  featureEncoder           (NULL),
  fileDesc                 (_fileDesc),
  log                      (_log),
  models                   (NULL),
  numOfClasses             (0),
  numOfModels              (0),
  oneVsAllAssignment       (),
  oneVsAllClassAssignments (NULL),
  predictXSpace            (NULL),
  predictXSpaceWorstCase   (0),
  probabilities            (NULL),
  rootFileName             (),
  selectedFeatures         (_svmParam.SelectedFeatures ()),
  svmParam                 (_svmParam),
  trainingTime             (0.0),
  type_table               (),
  validModel               (true),
  votes                    (NULL),
  xSpaces                  (NULL),
  xSpacesTotalAllocated    (0)

{
  if  (_examples.QueueSize () < 2)
  {
    log.Level (-1) << endl << endl 
                   << "SVMModel       **** ERROR ****      NO EXAMPLES TO TRAIN WITH." << endl
                   << endl;
    validModel = false;
    return;
  }


  type_table        = fileDesc->CreateAttributeTypeTable ( );
  cardinality_table = fileDesc->CreateCardinalityTable ( );
  
  struct svm_problem  prob;
  //  memset(&prob, 0, sizeof(svm_problem));

  numOfClasses = (int32)assignments.size ();

  log.Level (20) << "SVMModel::SVMModel - Constructing From Training Data." << endl;

  // figure out how many features the svm_problem struct will have
  SetSelectedFeatures (svmParam.SelectedFeatures ());

  try
  {
    if  (svmParam.MachineType () == OneVsOne)
    {
      ConstructOneVsOneModel (&_examples, prob);
    }
    else if  (svmParam.MachineType () == OneVsAll)
    {
      ConstructOneVsAllModel (&_examples, prob);
    }
    else if  (svmParam.MachineType () == BinaryCombos) 
    {
      ConstructBinaryCombosModel (&_examples);
    }
  }
  catch  (const exception& e)
  {
    log.Level (-1) << endl
      << "SVMModel  ***ERROR***   Exception occured constructing model." << endl
      << e.what () << endl
      << endl;
    validModel = false;
  }
  catch  (...)
  {
    log.Level (-1) << endl
      << "SVMModel  ***ERROR***   Exception occured constructing model." << endl
      << endl;
    validModel = false;
  }

  if  (cancelFlag)
    validModel = false;

  if  (!validModel)
    return;

  BuildClassIdxTable ();
  BuildCrossClassProbTable ();
}



SVMModel::~SVMModel ()
{
  log.Level (20) << "SVMModel::~SVMModel   Strating Destructor for Model[" << rootFileName << "]" << endl;

  int32  x;

  if  (models)
  {
    for  (x = 0;  x < numOfModels;  x++)
    {
      if  (models[x] != NULL)
      {
        SvmDestroyModel (models[x]);
   	    delete  [] models[x];
        models[x] = NULL;
      }
    }

    delete[]  models;
    models = NULL;
  }

  if  (xSpaces)
  {
    for  (int32 x = 0;  x < numOfModels;  x++)
    {
      if  (xSpaces[x] != NULL)
      {
        free (xSpaces[x]);
        xSpaces[x] = NULL;
      }
    }

    delete[]  xSpaces;
    xSpaces = NULL;
    xSpacesTotalAllocated = 0;
  }


  if  (oneVsAllClassAssignments)
  {
    for  (int32 x = 0;  x < numOfModels;  x++)
    {
      if  (oneVsAllClassAssignments[x])
      {
        delete  oneVsAllClassAssignments[x];
        oneVsAllClassAssignments[x] = NULL;
      }
    }
    delete  oneVsAllClassAssignments;
    oneVsAllClassAssignments = NULL;
  }


  delete  featureEncoder;  featureEncoder = NULL;

  if  (binaryParameters)
  {
    // We don't own the binaryParametres them self, only the 
    // array pointing to them,  they are owned by svmParam.
    // so we don't want to delete the contents.
    delete[]  binaryParameters;
    binaryParameters = NULL;
  }

  if  (binaryFeatureEncoders)
  {
    for  (x = 0;  x < numOfModels;  x++)
    {
      delete  binaryFeatureEncoders[x];
      binaryFeatureEncoders[x] = NULL;
    }
    delete[]  binaryFeatureEncoders;
    binaryFeatureEncoders = NULL;
  }

  if  (crossClassProbTable)
  {
    for  (x = 0;  x < crossClassProbTableSize;  x++)
    {
      delete  [] crossClassProbTable[x];
      crossClassProbTable[x] = NULL;
    }
    delete[]  crossClassProbTable;
    crossClassProbTable = NULL;
  }

  delete [] classIdxTable;   classIdxTable        = NULL;
  delete [] predictXSpace;   predictXSpace        = NULL;
  delete [] probabilities;   probabilities        = NULL;
  delete [] votes;           votes                = NULL;
}



int32  SVMModel::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (SVMModel)
       + assignments.MemoryConsumedEstimated ()
       + sizeof (short) * oneVsAllAssignment.size ()
       + sizeof (int32) * cardinality_table.size ()
       + rootFileName.MemoryConsumedEstimated ()
       + svmParam.MemoryConsumedEstimated ()
       + sizeof (AttributeType) * type_table.size ();

  if  (binaryFeatureEncoders)
  {
    memoryConsumedEstimated += sizeof (FeatureEncoderPtr) * numOfModels;
    for  (int32 x = 0;  x < numOfModels;  x++)
    {
      if  (binaryFeatureEncoders[x])
        memoryConsumedEstimated += binaryFeatureEncoders[x]->MemoryConsumedEstimated ();
    }
  }

  if  (binaryParameters)
    memoryConsumedEstimated += numOfModels * sizeof (BinaryClassParmsPtr);

  if  (classIdxTable)
    memoryConsumedEstimated += numOfClasses * sizeof (MLClassConstPtr);

  if  (crossClassProbTable)
    memoryConsumedEstimated += crossClassProbTableSize * crossClassProbTableSize * sizeof (double);

  if  (featureEncoder)
    memoryConsumedEstimated += featureEncoder->MemoryConsumedEstimated ();

  // fileDesc  We do not own 'filedesc'.

  if  (models)
  {
    memoryConsumedEstimated +=  numOfModels * sizeof (ModelPtr);
    for  (int32 x = 0;  x < numOfModels;  ++x)
    {
      if  (models[x])
        if  (models[x][0])
          memoryConsumedEstimated += models[x][0]->MemoryConsumedEstimated ();
    }
  }

  if  (oneVsAllClassAssignments)
  {
    memoryConsumedEstimated += numOfModels * sizeof (ClassAssignmentsPtr);
    for  (int32 x = 0;  x < numOfModels;  ++x)
    {
      if  (oneVsAllClassAssignments[x])
        memoryConsumedEstimated += oneVsAllClassAssignments[x]->MemoryConsumedEstimated ();
    }
  }

  if  (predictXSpace)
     memoryConsumedEstimated += sizeof (svm_node) * predictXSpaceWorstCase;

  if  (probabilities)
     memoryConsumedEstimated += sizeof (double) * numOfClasses;

  if  (votes)
     memoryConsumedEstimated += sizeof (int32) * numOfClasses;

  if  (xSpaces)
    memoryConsumedEstimated += xSpacesTotalAllocated * sizeof (svm_node) + numOfModels * sizeof (XSpacePtr);

  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */






void  SVMModel::BuildClassIdxTable ()
{
  delete[]  classIdxTable;

  classIdxTable = new MLClassConstPtr[numOfClasses];
  for  (int32 classIdx = 0;  classIdx < numOfClasses;  classIdx++)
    classIdxTable[classIdx] = assignments.GetMLClassByIndex (classIdx);

  probabilities = new double[numOfClasses + 2]; // I am add 2 as a deperqate move to deal with some kind ofg memory corruption  kak
  votes         = new int32[numOfClasses + 2];    // 
  
  for  (int32 x = 0;  x < numOfClasses;  x++)
  {
    probabilities[x] = 0.0;
    votes[x]         = 0;
  }

}  /* BuildClassIdxTable */



CompressionStats  SVMModel::BuildProblemOneVsAll (FeatureVectorList&          examples,
                                                  struct svm_problem&         prob,
                                                  XSpacePtr&                  xSpace,
                                                  const MLClassConstList&  classesThisAssignment,
                                                  FeatureEncoderPtr           featureEncoder,
                                                  MLClassConstList&        allClasses,
                                                  ClassAssignmentsPtr&        classAssignments
                                                 )
{ 
  log.Level (20) << "SVMModel::BuildProblemOneVsAll" << endl;

  int32  numOfFeaturesSelected = selectedFeatures.NumOfFeatures ();

  MLClassConstPtr  mlClass = NULL;
  delete  classAssignments;
  classAssignments = new ClassAssignments (log);

  MLClassConstList::const_iterator  idx;

  for  (idx = allClasses.begin ();  idx != allClasses.end ();  idx++)
  {
    mlClass = *idx;
    if  (classesThisAssignment.PtrToIdx (mlClass) >= 0)
    {
      // We are looking at a class that is to be treated logically as the 'one' class in 'One-vs-All'
      classAssignments->AddMLClass (mlClass, 0);
    }
    else
    {
      // We are looking at a class that is to be treated logically as one of the 'all' classes in 'One-vs-All'
      classAssignments->AddMLClass (mlClass, 1);
    }
  }
  
  int32  totalxSpaceUsed;

  CompressionStats  oneModelCompStats  
           = featureEncoder->Compress (&examples,
                                       *classAssignments,
                                       xSpace, 
                                       totalxSpaceUsed,
                                       prob
                                      );

  xSpacesTotalAllocated += totalxSpaceUsed;

  if  (svmParam.Param ().gamma == 0)
    svmParam.Gamma_Param (1.0 / numOfFeaturesSelected);

  return  oneModelCompStats;
}  /* BuildProblemOneVsAll */



void  SVMModel::BuildProblemBinaryCombos (FeatureVectorListPtr  class1Examples, 
                                          FeatureVectorListPtr  class2Examples, 
                                          SVMparam&             _svmParam,
                                          BinaryClassParmsPtr&  _twoClassParms,
                                          FeatureEncoderPtr&    _encoder,
                                          struct svm_problem&   prob, 
                                          XSpacePtr&            xSpace, 
                                          MLClassConstPtr    class1, 
                                          MLClassConstPtr    class2
                                         )
{ 
  log.Level (10) << "SVMModel::BuildProblemBinaryCombos   Class1[" << class1->Name () << "]  Class2[" << class2->Name () << "]" << endl;

  int32  totalxSpaceUsed = 0;

  ClassAssignments  binaryAssignments (log);
  binaryAssignments.AddMLClass (class1, 0);
  binaryAssignments.AddMLClass (class2, 1);

  _twoClassParms = _svmParam.GetParamtersToUseFor2ClassCombo (class1, class2);

  FeatureVectorListPtr  twoClassImages 
          = new FeatureVectorList (fileDesc, 
                                   false, 
                                   log, 
                                   class1Examples->QueueSize () + class2Examples->QueueSize ()
                                  );
  twoClassImages->AddQueue (*class1Examples);
  twoClassImages->AddQueue (*class2Examples);

  _encoder = new FeatureEncoder (_svmParam, 
                                 fileDesc,
                                 type_table,
                                 cardinality_table,
                                 class1,
                                 class2,
                                 log
                                );
  
  CompressionStats statsThisBinaryCombo 
                    = _encoder->Compress (twoClassImages,
                                          binaryAssignments,
                                          xSpace,
                                          totalxSpaceUsed,
                                          prob
                                         );

  xSpacesTotalAllocated += totalxSpaceUsed;

  if  (svmParam.CompressionMethod () == BRcompressionPost)
    compression_stats += statsThisBinaryCombo;

  if  (_twoClassParms->Param ().gamma == 0)
  {
    _twoClassParms->Gamma (1.0f / (float)_encoder->CodedNumOfFeatures ());
    cout << endl << endl
         << "Gamma was set to ZERO" << endl
         << endl;
  }

  delete  twoClassImages;
  twoClassImages = NULL;
}  /* BuildProblemBinaryCombos */





void  SVMModel::BuildCrossClassProbTable ()
{
  int32  x, y;

  if  (crossClassProbTable)
  {
    for  (x = 0;  x < crossClassProbTableSize;  x++)
    {
      delete  crossClassProbTable[x];
      crossClassProbTable[x] = NULL;
    }
    delete[]  crossClassProbTable;
    crossClassProbTable = NULL;
  }

  crossClassProbTable = NULL;

  if  (numOfClasses > 0)
  {
    crossClassProbTableSize = numOfClasses;
    crossClassProbTable = new double*[crossClassProbTableSize + 2];   // I am adding 2 to deal with a memory corruption problem.   kak
    for  (x = 0;  x < crossClassProbTableSize;  x++)
    {
      crossClassProbTable[x] = new double[crossClassProbTableSize + 2];
      for  (y = 0;  y < crossClassProbTableSize;  y++)
        crossClassProbTable[x][y] = 0.0;
    }
  }
}  /* BuildCrossClassProbTable */





const FeatureNumList&  SVMModel::GetFeatureNums (MLClassConstPtr  class1,
                                                 MLClassConstPtr  class2
                                                )  const
{
  return  svmParam.GetFeatureNums (class1, class2);
}  /* GetFeatureNums */





const FeatureNumList&   SVMModel::GetFeatureNums ()  const
{
  return  svmParam.GetFeatureNums ();
}



void  SVMModel::Save (const KKStr&  _rootFileName,
                      bool&          _successful
                     )
{
  log.Level (20) << "SVMModel::Save  Saving Model in File["
                 << _rootFileName << "]."
                 << endl;

  rootFileName = _rootFileName;

  KKStr  svmModelFileName = rootFileName + ".SVMModel";

  ofstream  o (svmModelFileName.Str ());

  Write (o, svmModelFileName, _successful);
  o.close ();
}  /* Save */




void  SVMModel::Write (ostream&      o,
                       const KKStr&  rootFileName,
                       bool&         _successful
                      )
{
  log.Level (20) << "SVMModel::Write  Saving Model in File." << endl;

  o << "<SVMModel>" << endl;

  o << "<Header>" << endl;

  o << "RootFileName"       << "\t" << rootFileName                                     << endl;
  o << "Time"               << "\t" << osGetLocalDateTime ()                            << endl;
  o << "NumOfModels"        << "\t" << numOfModels                                      << endl;
  o << "SelectedFeatures"   << "\t" << selectedFeatures.ToString ()                     << endl;
  o << "TrainingTime"       << "\t" << trainingTime                                     << endl;
  o << "CompressionStats"   << "\t" << compression_stats.ToTabDelString ().QuotedStr () << endl;
  o << "MachineType"        << "\t" << MachineTypeToStr (svmParam.MachineType ())       << endl;
  o << "ClassAssignments"   << "\t" << assignments.ToString ().QuotedStr ()             << endl;

  svmParam.WriteXML (o);

  o << "</Header>" << endl;

  if  (svmParam.MachineType () == OneVsOne)
  {
    o << "<OneVsOne>" << endl;

    KKStr  modelFileName = rootFileName + ".model";

    o << "ModelFileName" << "\t" << modelFileName.QuotedStr () << endl;

    SvmSaveModel (o, models[0]);

    o << "</OneVsOne>" << endl;
  }

  else if  (svmParam.MachineType () == OneVsAll)
  {
    o << "<OneVsAll>" << endl;

    // In the case of One to All there is a one-to-one relationshop between 
    // assignments[] and models[].  That is for each posible assignment there is
    // going to be a Support vector Machine.  assignments[i] is a pointer to the 
    // ClassAssignments structure that is used for the SVM that models[i] points 
    // to.
    for  (uint32  assignmentIDX = 0;  assignmentIDX < oneVsAllAssignment.size ();  assignmentIDX++)
    {
      o << "<OneVsAllEntry>" << endl;

      int32  assignmentNum = oneVsAllAssignment[assignmentIDX];   // The assignemnt as specified in the config file.

      // There can be more than one class with the same assignment, but we will use the 
      // one that 'GetMLClassByIndex' returns.
      MLClassConstPtr  classForOneSide = assignments.GetMLClassByIndex (assignmentNum);

      KKStr  modelFileName = rootFileName + "_" + classForOneSide->Name ();

      ClassAssignmentsPtr  oneVsAllAssignment = oneVsAllClassAssignments[assignmentIDX];

      o << "AssignmentNum"      << "\t" << assignmentNum                                << endl;
      o << "OneSideClassName"   << "\t" << classForOneSide->Name ().QuotedStr ()        << endl;  
      o << "ModelFileName"      << "\t" << modelFileName.QuotedStr ()                   << endl;
      o << "ClassAssignments"   << "\t" << oneVsAllAssignment->ToString ().QuotedStr () << endl;

      SvmSaveModel (models[assignmentIDX], modelFileName.Str (), _successful);
      if  (!_successful)
      {
        log.Level (-1) << "SVMModel::Save  Error saving Model into File["
                       << modelFileName << "]."
                       << endl;
        _successful = false;
        return;
      }

      o << "</OneVsAllEntry>" << endl;
    }

    o << "</OneVsAll>" << endl;
  }

  else if  (svmParam.MachineType () == BinaryCombos)
  {
    o << "<BinaryCombos>" << endl;

    int32  modelsIDX = 0;
    // With BinaryCombos, n = numberOfAssignmnets = assignments.size (), the number of binary combos is equal to 'n(n-1)/2'.

    for  (modelsIDX = 0;  modelsIDX < numOfModels;  modelsIDX++)
    {
      BinaryClassParmsPtr  binClassParms = binaryParameters[modelsIDX];
      o << "Classes" << "\t" << binClassParms->Class1Name () << "\t"  << binClassParms->Class2Name () << endl;
      SvmSaveModel (o, models[modelsIDX]);
    }

    o << "</BinaryCombos>" << endl;
  }

  o << "</SVMModel>" << endl;

  _successful = true;

} /* Write */





void  SVMModel::Read (istream&  i)
{
  validModel = true;

  ReadHeader (i);

  if  (svmParam.MachineType () == OneVsOne)
  {
    ReadOneVsOne (i);
  }
  else if  (svmParam.MachineType () == OneVsAll)
  {
    ReadOneVsAll (i);
  }
  else if  (svmParam.MachineType () == BinaryCombos)
  {
    ReadBinaryCombos (i);
  }

  if  (!cancelFlag)
  {
    bool  sectionFound;
    ReadSkipToSection (i, "</SVMModel>", sectionFound);
  }
}  /* Read */



void  SVMModel::ReadSkipToSection (istream&  i, 
                                   KKStr     sectName,
                                   bool&     sectionFound
                                  )
{
  sectionFound = false;
  char  buff[20480];
  sectName.Upper ();

  KKStr  field;

  // Skip to start of OneVsOne section
  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);
    field = ln.ExtractQuotedStr ("\n\r\t", 
                                 true      // true = decode escape charaters
                                );
    field.Upper ();
    if  (field == sectName)
    {
      sectionFound = true;
      break;
    }
  }

  if  (!sectionFound)
  {
    log.Level (-1) << endl << endl
                   << "SVMModel::ReadSkipToSection    *** Could not find section[" << sectName << "]" <<endl
                   << endl;
  }
}  /* ReadSkipToSection */




void  SVMModel::ReadHeader (istream&  i)
{
  log.Level (20) << "SVMModel::ReadHeader" << endl;
  bool  sectionFound = false;
  ReadSkipToSection (i, "<Header>", sectionFound);
  if  (!sectionFound)
  {
    validModel = false;
    return;
  }

  char  buff[20480];

  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);

    KKStr  field = ln.ExtractQuotedStr ("\n\r\t", true);      // true = decode escape charaters

    field.Upper ();

    if  (field.EqualIgnoreCase ("</HEADER>"))
    {
      break;
    }

    if  (field == "TIME")
    {
      log.Level (20) << "SVMModel::ReadHeader    Time[" << ln << "]" << endl;
    }

    else if  (field.EqualIgnoreCase ("NUMOFMODELS"))
      numOfModels = ln.ExtractTokenInt ("\n\t\r");

    else if  (field == "SELECTEDFEATURES")
    {
      bool  valid;
      FeatureNumList  tempFeatures (fileDesc);
      tempFeatures.ExtractFeatureNumsFromStr (ln.ExtractQuotedStr ("\n\r\t", true), valid);
      SetSelectedFeatures (tempFeatures);
    }

    else if  (field == "TRAININGTIME")
      trainingTime = ln.ExtractTokenDouble ("\n\r\t");

    else if  (field == "COMPRESSIONSTATS")
      compression_stats.UpdateFromTabDelStr (ln.ExtractQuotedStr ("\n\r\t", true));

    else if  (field == "CLASSASSIGNMENTS")
      assignments.ParseToString (ln.ExtractQuotedStr ("\n\r\t", true));

    else if  (field == "<SVMPARAM>")
      svmParam.ReadXML (i);
  }
}  /* ReadHeader */




void  SVMModel::ReadOneVsOne (istream&  i)
{
  log.Level (20) << "SVMModel::ReadOneVsOne" << endl;

  bool  sectionFound = false;
  ReadSkipToSection (i, "<OneVsOne>", sectionFound);
  if  (!sectionFound)
  {
    validModel = false;
    return;
  }

  delete  featureEncoder;
  featureEncoder = new FeatureEncoder (svmParam, 
                                       fileDesc,
                                       type_table,
                                       cardinality_table,
                                       NULL,              // class1, set to NULL because we are dealing with all classes
                                       NULL,              // class2,     ""          ""            ""            ""
                                       log
                                      );

  char  buff[20480];
  KKStr  field;

  KKStr  modelFileName;

  while  ((i.getline (buff, sizeof (buff)))  &&  validModel)
  {
    KKStr  ln (buff);
    field = ln.ExtractQuotedStr ("\n\r\t", true);
    field.Upper ();

    if  (field == "</ONEVSONE>")
    {
      break;
    }

    else if  (field == "MODELFILENAME")
    {
      modelFileName = ln.ExtractQuotedStr ("\n\r\t", true);
      numOfModels = 1;
      delete  models;
      models = new ModelPtr[numOfModels];
      models[0] = SvmLoadModel (i, log);
      if  (!models[0]) 
      {
        log.Level (-1) << endl << endl 
                       << "SVMModel::ReadOnevsOne - Error Loading Model from File["
                       << modelFileName << "]." 
                       << endl;
        validModel = false;
      }
    }
  }
 
  if  (validModel  &&  modelFileName.Empty ())
  {
     log.Level (-1) << endl << endl 
                    << "SVMModel::ReadOnevsOne - Model File Name was not specified." << endl
                    << modelFileName << "]." 
                    << endl;
     validModel = false;
  }
}  /* ReadOneVsOne */




void  SVMModel::ReadOneVsAll (istream&  i)
{
  log.Level (20) << "SVMModel::ReadOneVsAll" << endl;

  bool  sectionFound = false;
  ReadSkipToSection (i, "<OneVsAll>", sectionFound);
  if  (!sectionFound)
  {
    validModel = false;
    return;
  }

  delete  featureEncoder;
  featureEncoder = new FeatureEncoder (svmParam, 
                                       fileDesc,
                                       type_table,
                                       cardinality_table,
                                       NULL,              // class1, set to NULL because we are dealing with all classes
                                       NULL,              // class2,     ""          ""            ""            ""
                                       log
                                      );

  int32  modelIDX = 0;

  // allocate and initialize the model objects we'll need
  models = new ModelPtr[numOfModels];
  for  (modelIDX = 0;  modelIDX < numOfModels;  modelIDX++)
    models[modelIDX] = NULL;


  oneVsAllAssignment.clear ();
  while  (oneVsAllAssignment.size () < uint32 (numOfModels))
    oneVsAllAssignment.push_back (-1);

  {
    delete  oneVsAllClassAssignments;
    oneVsAllClassAssignments = new ClassAssignmentsPtr[numOfModels];
    for  (modelIDX = 0;  modelIDX < numOfModels;  modelIDX++)
      oneVsAllClassAssignments[modelIDX] = NULL;
  }


  char  buff[10240];
  KKStr  field;

  while  ((i.getline (buff, sizeof (buff)))  &&  validModel)
  {
    KKStr  ln (buff);
    field = ln.ExtractQuotedStr ("\n\r\t", true);
    field.Upper ();

    if  (field == "</ONEVSALL>")
      break;

    if  (field == "<ONEVSALLENTRY>")
    {
      ReadOneVsAllEntry (i, modelIDX);
    }
  }

}  /* ReadOneVsAll */




void  SVMModel::ReadOneVsAllEntry (istream&  i,
                                   int32     modelIDX
                                  )
{
  // If we are gere the header to this sub-section has alreadt been read.

  char  buff[20480];
  KKStr  field;

  int32                assignmentNum = -1;
  MLClassConstPtr   classForOneSide = NULL;
  KKStr                modelFileName;
  ClassAssignmentsPtr  assignment       = NULL;

  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);
    field = ln.ExtractQuotedStr ("\n\r\t", true);
    field.Upper ();

    KKStr  value = ln.ExtractQuotedStr ("\n\r\t", true);
  
    if  (field == "</ONEVSALLENTRY>")
      break;

    if  (field == "ASSIGNMENTNUM")
    {
      assignmentNum = atoi (value.Str ());
      oneVsAllAssignment[modelIDX] = assignmentNum;
    }

    else if  (field == "ONESIDECLASSNAME")
      classForOneSide = MLClass::CreateNewMLClass (value);

    else if  (field == "ModelFileName")
      modelFileName = value;

    else if  (field == "CLASSASSIGNMENTS")
    {
      assignment = new ClassAssignments (log);
      assignment->ParseToString(value);
      oneVsAllClassAssignments[modelIDX] = assignment;
    }
  }

  if  (modelFileName.Empty ())
  {
    log.Level (-1) << endl << endl 
                   << "SVMModel::ReadOneVsAllEntry    *** Model file name not sepcified ***" << endl
                   << endl;
    validModel = false;
    return;
  }

  if  (!assignment)
  {
    log.Level (-1) << endl << endl 
                   << "SVMModel::ReadOneVsAllEntry    *** OneVsAll assignment not specified for ***" << endl
                   << endl;
    validModel = false;
    return;
  }

  if  (!classForOneSide)
  {
    log.Level (-1) << endl << endl 
                   << "SVMModel::ReadOneVsAllEntry    *** Class name for one side not specified ***" << endl
                   << endl;
    validModel = false;
    return;
  }

  models[modelIDX] = SvmLoadModel (modelFileName.Str ());
  if  (!models[modelIDX])
  {
    log.Level (-1) << endl << endl
                   << "SVMModel::ReadOneVsAllEntry - Error Loading Model File[" << modelFileName << "]" << endl
               << endl;
    validModel = false;
    return;
  }
}  /* ReadOneVsAllEntry */




void  SVMModel::ReadBinaryCombos (istream& i)
{
  log.Level (20) << "SVMModel::ReadBinaryCombos" << endl;

  char  buff[40960];
  int32   modelsIDX = 0;

  delete  binaryParameters;
  binaryParameters       = new BinaryClassParmsPtr [numOfModels];
  models                 = new ModelPtr            [numOfModels];
  binaryFeatureEncoders  = new FeatureEncoderPtr   [numOfModels];

  for  (int32 x = 0;  x < numOfModels;  x++)
  {
    models                 [x] = NULL;
    binaryParameters       [x] = NULL;
    binaryFeatureEncoders  [x] = NULL;
  }

  KKStr  class1Name; 
  KKStr  class2Name;

  MLClassConstPtr  class1 = NULL;
  MLClassConstPtr  class2 = NULL;

  bool  endOfBinaryCombosRead = false;

  while  ((i.getline (buff, sizeof (buff)))  &&  (!cancelFlag)  &&  validModel)
  {
    KKStr  line (buff);
    line.TrimLeft ();
    line.TrimRight ();

    if  (line == "<BinaryCombos>")
      continue;

    if  (line == "</BinaryCombos>")
    {
      endOfBinaryCombosRead = true;
      break;
    }
    
    KKStr  lineName = line.ExtractToken2 ("\t");
    lineName.Upper ();

    if  (lineName == "CLASSES")  
    {
      if  (modelsIDX >= numOfModels)
      {
        log.Level (-1) << endl << endl << endl 
                       << "SVMModel::ReadBinaryCombos   ***ERROR***     Number of models defined exceeds what was expected." << endl
                       << endl;
        validModel = false;
        return;
      }

      class1Name = line.ExtractToken2 ("\t");
      class2Name = line.ExtractToken2 ("\t");

      class1 = MLClass::CreateNewMLClass (class1Name);
      class2 = MLClass::CreateNewMLClass (class2Name);

      log.Level (10) << class1Name << " - " << class2Name << "]" << endl;

      BinaryClassParmsPtr  binClassParms = svmParam.GetParamtersToUseFor2ClassCombo (class1, class2);
      if  (binClassParms == NULL)
      {
        validModel = false;
        return;
      }

      binaryParameters[modelsIDX] = binClassParms;

      models[modelsIDX] = SvmLoadModel (i, log);   
      if  (models[modelsIDX] == NULL)
      {
        log.Level (-1) << endl << endl << endl 
                       << "SVMModel::ReadBinaryCombos     ***ERROR***     Unable to load model." << endl
                       << endl
                       << "Class1[" << binClassParms->Class1Name () << "]  Class2[" << binClassParms->Class2Name () << "]" << endl
                       << endl;
        validModel = false;
      }
      else
      {
        binaryFeatureEncoders[modelsIDX] 
              = new FeatureEncoder (svmParam, 
                                    fileDesc,
                                    type_table,
                                    cardinality_table,
                                    binClassParms->Class1 (),
                                    binClassParms->Class2 (),
                                    log
                                   );
      }

      modelsIDX++;
    }
  }

  if  (!endOfBinaryCombosRead)
  {
    // We need to ski[p to end of Binary Section.
    while  ((i.getline (buff, sizeof (buff)))  &&  (!cancelFlag)  &&  validModel)
    {
      KKStr  line (buff);
      line.TrimRight ();
      line.TrimLeft ();
      if  (line.EqualIgnoreCase ("</BinaryCombos>"))
        break;
    }
  }


  if  ((modelsIDX < numOfModels)  &&  (!cancelFlag))
  {
    log.Level (-1) << endl << endl << endl 
                   << "SVMModel::ReadBinaryCombos     **** ERROR ****     Less BinaryComboParms were specifued than expected." << endl
                   << endl;
    validModel = false;
  }
}  /* ReadBinaryCombos */




double   SVMModel::DistanceFromDecisionBoundary (FeatureVectorPtr    example,
                                                 MLClassConstPtr  class1,
                                                 MLClassConstPtr  class2
                                                )
{
  if  (svmParam.MachineType () != BinaryCombos)
  {
    log.Level (-1) << endl << endl << "SVMModel::DistanceFromDecisionBoundary   ***ERROR***    This method only works with BinaryCombos." << endl << endl;
    return  0.0;
  }

  int32  modelIDX = 0;
  bool  revClassOrder = false;
  FeatureEncoderPtr  encoder  = NULL;

  while  (modelIDX < numOfModels)
  {
    encoder = binaryFeatureEncoders[modelIDX];
    if  ((encoder->Class1 () == class1)  &&  (encoder->Class2 () == class2))
    {
      revClassOrder = false;
      break;
    }
    else if  ((encoder->Class1 () == class2)  &&  (encoder->Class2 () == class1))
    {
      revClassOrder = true;
      break;
    }

    encoder = NULL;
    modelIDX++;
  }

  if  (encoder == NULL)
  {
    log.Level (-1) << endl << endl << "SVMModel::DistanceFromDecisionBoundary   ***ERROR***    Class[" << class1->Name () << "] and Class2[" << class2->Name () << "]  not one of the class pairs." << endl << endl;
    return 0.0;
  }

  int32  xSpaceUsed;
  encoder->EncodeAImage (example, predictXSpace, xSpaceUsed);

  double  distance = 0.0;

  svm_predictTwoClasses (models[modelIDX][0], predictXSpace, distance, -1);

  if  (revClassOrder)
    distance = 0.0 - distance;

  return  distance;
}  /* DistanceFromDecisionBoundary */




void   SVMModel::Predict (FeatureVectorPtr     example,
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
  breakTie = 0.0f;   // expirements.

  knownClassOneOfTheWinners = false;

  predClass  = NULL;
  predClass2 = NULL;

  predClass1Votes = -1;
  predClass2Votes = -1;

  probOfKnownClass  = 0.0;
  predClassProb   = 0.0;
  predClass2Prob  = -1.0f;

  numOfWinners = 0;

  if  (svmParam.MachineType () == OneVsAll)
  {
    EncodeImage (example, predictXSpace);
    PredictOneVsAll (predictXSpace,   //  used to be xSpace,  
                     knownClass,
                     predClass,
                     predClass2,
                     probOfKnownClass,
                     predClassProb,
                     predClass2Prob,
                     compact,
                     numOfWinners,
                     knownClassOneOfTheWinners,
                     breakTie
                    );
    predClass1Votes = 1;
    predClass2Votes = 1;
    //  free (xSpace);  // We are now allocating only once.
  }

  else if  (svmParam.MachineType () == OneVsOne)
  {  
    EncodeImage (example, predictXSpace);

    int32  knownClassNum  = -1;
    int32  prediction     = -1;
    int32  prediction2    = -1;

    if  (knownClass)
      knownClassNum = assignments.GetNumForClass (knownClass);

    vector<int32>  winners;

    SvmPredictClass (svmParam,
                     models[0],
                     predictXSpace, 
                     votes,
                     probabilities, 
                     knownClassNum,
                     prediction,
                     prediction2,
                     predClass1Votes,
                     predClass2Votes,
                     predClassProb,
                     predClass2Prob,
                     probOfKnownClass,
                     compact,
                     winners,
                     crossClassProbTable,
                     breakTie
                    );

    numOfWinners = (int32)winners.size ();

    for  (int32 idx = 0;  idx < (int32)winners.size ();  idx++)
    {
      if  (winners[idx] == knownClassNum)
      {
        knownClassOneOfTheWinners = true;
        break;
      }
    }

    predClass  = assignments.GetMLClass (prediction);
    predClass2 = assignments.GetMLClass (prediction2);
    //free (xSpace);
  }

  
  else if  (svmParam.MachineType () == BinaryCombos)
  {
    PredictByBinaryCombos (example, 
                           knownClass,
                           predClass,
                           predClass2,
                           predClass1Votes,
                           predClass2Votes,
                           probOfKnownClass,
                           predClassProb,
                           predClass2Prob,
                           breakTie,
                           compact,
                           numOfWinners,
                           knownClassOneOfTheWinners
                          );
  }
    
  else
  {
    log.Level (-1) << "***ERROR*** SVMModel::Predict Invalid Machine Type Specified."  << endl;
    exit (-1);
  }

  return;
}  /* Predict */






void   SVMModel::PredictOneVsAll (XSpacePtr            xSpace,
                                  MLClassConstPtr   knownClass,
                                  MLClassConstPtr&  predClass,  
                                  MLClassConstPtr&  predClass2,  
                                  double&              probOfKnownClass,
                                  double&              predClassProb,
                                  double&              predClass2Prob,
                                  double&              compact,
                                  int32&               numOfWinners,
                                  bool&                knownClassOneOfTheWinners,
                                  double&              breakTie
                                 )

{
  predClass  = NULL;
  predClass2 = NULL;
  knownClassOneOfTheWinners = false;

  probOfKnownClass = 0.0;
  predClassProb  = 0.0;

  vector<int32>   winningClasses;

  double*  probabilities = new double [numOfModels + 2];  // I am addin 2 as a deperate measure to deal with a memory corruption problem   kak

  double  largestLosingProbability    = FLT_MIN;
  int32     largestLosingProbabilityIDX = -1;

  double  secondLargestLosingProbability    = FLT_MIN;
  int32     secondLargestLosingProbabilityIDX = -1;

  double  largestWinningProbability    = FLT_MIN;
  int32     largestWinningProbabilityIDX = -1;

  double  secondLargestWinningProbability    = FLT_MIN;
  int32     secondLargestWinningProbabilityIDX = -1;

  int32  knownAssignmentIDX = -1;

  uint32  assignmentIDX;

  for  (assignmentIDX = 0;  assignmentIDX < oneVsAllAssignment.size ();  assignmentIDX++)
  {
    short  assignmentNum = oneVsAllAssignment[assignmentIDX];

    int32  knownClassNum = -1;
    int32  predClassNum  = -1;
    int32  predClassNum2 = -1;

    MLClassConstPtr  classWeAreLookingAt = assignments.GetMLClassByIndex (assignmentNum);

    if  (knownClass)
    {
      if  (knownClass == classWeAreLookingAt)
      {
        knownAssignmentIDX = assignmentIDX;
        knownClassNum = 0;
      }
      else
      {
        knownClassNum = 1;
      }
    }

    double  predictedClassProbability  = 0.0f;
    double  predictedClassProbability2 = 0.0f;
    double  knownClassProbabilioty     = 0.0f;

    vector<int32>  winners;

    double*  tempProbabilities = new double[numOfClasses + 2];  // kk 2004-12-22     // I am addin 2 as a deperate measure to deal with a memory corruption problem   kak
    int32*     tempVotes         = new int32[numOfClasses + 2];

    int32  predClass1Votes = -1;
    int32  predClass2Votes = -1;

    SvmPredictClass (svmParam,
                     models[assignmentIDX],
                     xSpace, 
                     tempVotes,
                     tempProbabilities,    // kk 2004-12-22
                     predClass1Votes,
                     predClass2Votes,
                     knownClassNum,    
                     predClassNum,
                     predClassNum2,
                     predictedClassProbability,
                     predictedClassProbability2,
                     knownClassProbabilioty,
                     compact,
                     winners,
                     crossClassProbTable,
                     breakTie
                    );

    delete[]  tempVotes;
    tempVotes = NULL;
    delete[]  tempProbabilities;  // kk 2004-12-22
    tempProbabilities = NULL;

    if  (predClassNum == 0)
    {
      winningClasses.push_back (assignmentIDX);
      probabilities[assignmentIDX] = predictedClassProbability;
      if  (predictedClassProbability > largestWinningProbability)
      {
        secondLargestWinningProbability    = largestWinningProbability;
        secondLargestWinningProbabilityIDX = largestWinningProbabilityIDX;

        largestWinningProbability = predictedClassProbability;
        largestWinningProbabilityIDX = assignmentIDX;
      }
      else if  (predictedClassProbability > secondLargestWinningProbability)
      {
        secondLargestWinningProbability    = predictedClassProbability;
        secondLargestWinningProbabilityIDX = assignmentIDX;
      }
    }
    else
    {
      probabilities[assignmentIDX] = 1.0 - predictedClassProbability;
      if  (probabilities[assignmentIDX] > largestLosingProbability)
      {
        secondLargestLosingProbabilityIDX = largestLosingProbabilityIDX;
        secondLargestLosingProbability    = largestLosingProbability;

        largestLosingProbabilityIDX = assignmentIDX;
        largestLosingProbability    = probabilities[assignmentIDX];
      }
      else if  (probabilities[assignmentIDX] > secondLargestLosingProbability)
      {
        secondLargestLosingProbabilityIDX = assignmentIDX;
        secondLargestLosingProbability    = probabilities[assignmentIDX];
      }
    }
  }

  numOfWinners = (int32)winningClasses.size ();

  int32 assignmentIDXthatWon = -1;
  int32 assignmentIDXsecond  = -1;

  if  (winningClasses.size () <= 0)
  {
    // There were no winners,  lets just use the 1 that had the highest probability.

    assignmentIDXthatWon = largestLosingProbabilityIDX;
    assignmentIDXsecond  = secondLargestLosingProbabilityIDX;

    predClass = assignments.GetMLClassByIndex (oneVsAllAssignment[assignmentIDXthatWon]);

    knownClassOneOfTheWinners = false;
  }

  else if  (winningClasses.size () == 1)
  {
    assignmentIDXthatWon = winningClasses[0];
    knownClassOneOfTheWinners = (assignmentIDXthatWon == knownAssignmentIDX);
    assignmentIDXsecond       = largestLosingProbabilityIDX;
  }

  else
  {
    // We had more than one Winner
    assignmentIDXthatWon = largestWinningProbabilityIDX;
    assignmentIDXsecond  = secondLargestLosingProbabilityIDX;

    for  (int32  idx = 0;  idx < (int32)winningClasses.size ();  idx++)
    {
      if  (winningClasses[idx] == knownAssignmentIDX)
      {
        knownClassOneOfTheWinners = true;
        break;
      }
    }
  }

  predClass  = assignments.GetMLClassByIndex (oneVsAllAssignment[assignmentIDXthatWon]);
  predClass2 = assignments.GetMLClassByIndex (oneVsAllAssignment[assignmentIDXsecond]);

  predClassProb   = (probabilities [oneVsAllAssignment[assignmentIDXthatWon]]);
  probOfKnownClass  = (probabilities [oneVsAllAssignment[knownAssignmentIDX]]);

  delete[]  probabilities;      
  probabilities = NULL;

  return;
}  /* PredictOneVsAll */





MLClassConstPtr  SVMModel::Predict (FeatureVectorPtr  example)
{
  double              breakTie         = -1.0f;
  double              compact          = -1.0;
  bool                knownClassOneOfTheWinners = false;
  int32               numOfWinners     = -1;
  MLClassConstPtr  pred1            = NULL;
  MLClassConstPtr  pred2            = NULL;
  double              predClass1Prob   = -1.0;
  double              predClass2Prob   = -1.0;
  double              probOfKnownClass = -1.0;

  int32               predClass1Votes  = -1;
  int32               predClass2Votes  = -1;
  

  Predict (example, 
           NULL,
           pred1,
           pred2,
           predClass1Votes,
           predClass2Votes,
           probOfKnownClass,
           predClass1Prob,
           predClass2Prob,
           compact,
           numOfWinners,
           knownClassOneOfTheWinners,
           breakTie
          );

  return  pred1;
}  /* Predict */





void  SVMModel::PredictByBinaryCombos (FeatureVectorPtr     example,
                                       MLClassConstPtr   knownClass,
                                       MLClassConstPtr&  predClass1,
                                       MLClassConstPtr&  predClass2,
                                       int32&               predClass1Votes,
                                       int32&               predClass2Votes,
                                       double&              probOfKnownClass,
                                       double&              predClass1Prob,
                                       double&              predClass2Prob,
                                       double&              breakTie,
                                       double&              compact,
                                       int32&               numOfWinners,
                                       bool&                knownClassOneOfTheWinners
                                      )
{
  int32     classIDX;

  predClass1        = NULL;
  predClass2        = NULL;
  probOfKnownClass  = -1.0f;
  predClass1Prob  = -1.0f;
  predClass2Prob  = -1.0f;

  predClass1Prob = 0;
  predClass2Prob = 0;

  knownClassOneOfTheWinners = false;

  double  probability  = -1.0;

  int32   knownClassIDX = numOfClasses - 1;
  int32   predClass1IDX  = -1;
  int32   predClass2IDX  = -1;

  int32   modelIDX = 0;

  for  (int32 x = 0; x < numOfClasses;  x++)
  {
    votes[x] = 0;
    probabilities[x] = 1.0f;
  }

  for  (int32  class1IDX = 0;  class1IDX < (numOfClasses - 1);  class1IDX++)
  {
    MLClassConstPtr  class1  = classIdxTable [class1IDX];

    if  (class1 == knownClass)
      knownClassIDX = class1IDX;
     
    for  (int32  class2IDX = (class1IDX + 1);  class2IDX < numOfClasses;  class2IDX++)
    {
      MLClassConstPtr  class2 = classIdxTable [class2IDX];
      BinaryClassParmsPtr  thisComboPrameters = binaryParameters[modelIDX];

      int32  xSpaceUsed;

      if  (binaryFeatureEncoders[modelIDX] == NULL)
      {
        KKStr  errMsg;
        errMsg << "SVMModel::PredictByBinaryCombos   ***ERROR***   No feature encoder for model[" << modelIDX << "]";
        log.Level (-1) << endl << endl << errMsg << endl << endl;
        throw KKStrException (errMsg);
      }

      binaryFeatureEncoders[modelIDX]->EncodeAImage (example, predictXSpace, xSpaceUsed);

      double  distance = 0.0;
      double  margin   = 0.0;

      svm_predictTwoClasses (models[modelIDX][0], predictXSpace, distance, -1);
      probability = (1.0 / (1.0 + exp (-1.0 * (thisComboPrameters->Param ().A) * distance)));
      probability = AdjProb (probability);  // KAK 2011-06-10

      if  (probability > 0.5)
      {
        votes[class1IDX]++;
        probabilities[class1IDX] *= probability;
        probabilities[class2IDX] *= (1.0f - probability);
      }
      else
      {
        votes[class2IDX]++;
        probabilities[class2IDX] *= (1.0 - probability);
        probabilities[class1IDX] *= probability;
      }

      modelIDX++;
    }
  }

  double  totProbability = 0.0;
  for  (classIDX = 0;  classIDX < numOfClasses;  classIDX++)
    totProbability += probabilities[classIDX];

  if  (totProbability <= 0.0)
    totProbability = 1.0;

  for  (classIDX = 0;  classIDX < numOfClasses;  classIDX++)
    probabilities[classIDX] = probabilities[classIDX] / totProbability;

  GreaterVotes (svmParam.SelectionMethod () == SelectByProbability,
                numOfClasses,
                votes,
                numOfWinners,
                probabilities,
                predClass1IDX,
                predClass2IDX
               );

  if  (predClass1IDX >= 0)
  {
    predClass1        = classIdxTable [predClass1IDX];
    predClass1Votes   = votes         [predClass1IDX];
    predClass1Prob  = probabilities [predClass1IDX];
  }

  if  (predClass2IDX >= 0)
  {
    predClass2        = classIdxTable [predClass2IDX];
    predClass2Votes   = votes         [predClass2IDX];
    predClass2Prob  = probabilities [predClass2IDX];
  }

  if  (knownClassIDX >= 0)
    probOfKnownClass = probabilities[knownClassIDX];


  breakTie = fabs (predClass1Prob - predClass2Prob);
  knownClassOneOfTheWinners = (predClass1IDX == knownClassIDX);

  return;
}  /* PredictByBinaryCombos */





int32  SVMModel::NumOfSupportVectors ()  const
{
  int32  numOfSupportVectors = 0;

  if  (models == NULL)
    return 0;

  if  (svmParam.MachineType () == BinaryCombos)
  {
    vector<KKStr> svNames = SupportVectorNames ();
    numOfSupportVectors = (int32)svNames.size ();
  }
  else
  {
    for  (int32 x = 0;  x < numOfModels;  x++)
      numOfSupportVectors += models[x][0]->l;
  }

  return  numOfSupportVectors;
}  /* NumOfSupportVectors */




void  SVMModel::SupportVectorStatistics (int32&  numSVs,
                                         int32&  totalNumSVs
                                        )
{
  numSVs = 0;
  totalNumSVs = 0;
  if  (models == NULL)
    return;

  if  (svmParam.MachineType () == BinaryCombos)
  {
    numSVs = NumOfSupportVectors ();
    for  (int32 modelIDX = 0;  modelIDX < numOfModels;  modelIDX++)
    {
      totalNumSVs += models[modelIDX][0]->l;
    }
  }
  else
  {
    svm_GetSupportVectorStatistics (models[0][0], numSVs, totalNumSVs);
  }

}  /* SupportVectorStatistics */



void  SVMModel::ProbabilitiesByClass (FeatureVectorPtr            example,
                                      const MLClassConstList&  _mlClasses,
                                      int32*                      _votes,
                                      double*                     _probabilities
                                     )
{
  // KAK 2012-087-02  Decided that since the code will only populate the entreis in '_votes'
  //                  and '_probabilities' based off  the '_mlClasses'  data structure
  //                  and if a predictedClass is not in '_mlClasses' then it will print 
  //                  a warning to log file and not corrupt data.  Only problem is that iof 
  //                  this happens then the sum of '_probabilities' will be less than 1.0.
  //
  //if  (_mlClasses.size () != assignments.size ())
  //{
  //  log.Level (-1) << endl
  //                 << endl
  //                 << "SVMModel::ProbabilitiesByClass    *** ERROR ***" << endl
  //                 << "                 _mlClasses[" << _mlClasses.QueueSize () << "] and "
  //                 <<                   "assignments["   << (int32)assignments.size ()  << "]." 
  //                 << "                 ar not the same length."
  //                 << endl
  //                 << endl;
  //  return;
  //}

  if  (svmParam.MachineType () == BinaryCombos)
  {
    PredictProbabilitiesByBinaryCombos (example, _mlClasses, _votes, _probabilities);
    return;
  }

  int32   predClass1Votes         = -1;
  int32   predClass2Votes         = -1;
  double  predClassProb           = 0.0;
  double  predClass2Prob          = 0.0;
  double  probOfKnownClass        = 0.0;
  double  smallestDistOfPredClass = 0.0;
  double  compact                 = 0.0;

  double  breakTie                = 0.0f;

  MLClassConstPtr  predictedClass   = NULL;
  XSpacePtr           xSpace           = NULL;
  int32               x, y;

  xSpace = featureEncoder->EncodeAImage (example);

  int32  knownClassNum = 0;
  int32  prediction    = 0;
  int32  prediction2   = 0;

  for  (x = 0;  x < numOfClasses;  x++)
  {
    probabilities [x] = 0.0;
    _probabilities[x] = 0.0;
    _votes        [x] = 0;
  }

  vector<int32>  winners;

  SvmPredictClass (svmParam,
                   models[0],
                   xSpace, 
                   votes,
                   probabilities, 
                   knownClassNum, 
                   prediction,
                   prediction2,
                   predClass1Votes,
                   predClass2Votes,
                   predClassProb,
                   predClass2Prob,
                   probOfKnownClass,
                   compact,
                   winners,
                   crossClassProbTable,
                   breakTie
                  );

  // We have  to do it this way, so that we pass back the probabilities in the 
  // same order that the user indicated with mlClasses being passed in.
  for  (x = 0;  x < numOfClasses;  x++)
  {
    predictedClass = assignments.GetMLClass (x);
    if  (predictedClass)
    {
      y = _mlClasses.PtrToIdx (predictedClass);
      if  ((y < 0)   ||   (y >= numOfClasses))
      {
        log.Level (-1) << endl << endl
                       << "SVMModel::ProbabilitiesByClass   ***ERROR***"                                                     << endl
                       << "                                 Invalid classIdx[" << y                       << "]  Specified." << endl
                       << "                                 ClassName       [" << predictedClass->Name () << "]"             << endl
                       << endl;
      }
      else
      {
        _votes        [y] = votes        [x];
        _probabilities[y] = probabilities[x];
      }
    }
  }

  delete  xSpace;  
  xSpace = NULL;

  return;
}  /* ProbabilitiesByClass */






void  SVMModel::PredictProbabilitiesByBinaryCombos (FeatureVectorPtr            example,  
                                                    const MLClassConstList&  _mlClasses,
                                                    int32*                      _votes,
                                                    double*                     _probabilities
                                                   )
{
  int32     classIDX;

  double  probability = -1.0;

  int32   modelIDX = 0;

  for  (int32 x = 0; x < numOfClasses;  x++)
  {
    probabilities[x] = 1.0;
    votes[x] = 0;
  }


  if  (numOfClasses != crossClassProbTableSize)
  {
    log.Level (-1) << endl << endl 
                   << "SVMModel::PredictProbabilitiesByBinaryCombos                     ***ERROR***" << endl
                   << "                      numfClasses != crossClassProbTableSize"                 << endl       
                   << endl;
    return;
  }


  for  (int32  class1IDX = 0;  class1IDX < (numOfClasses - 1);  class1IDX++)
  {

    for  (int32  class2IDX = (class1IDX + 1);  class2IDX < numOfClasses;  class2IDX++)
    {
      BinaryClassParmsPtr  thisComboPrameters = binaryParameters[modelIDX];

      if  (binaryFeatureEncoders[modelIDX] == NULL)
      {
        KKStr  errMsg;
        errMsg << "SVMModel::PredictProbabilitiesByBinaryCombos   ***ERROR***   No feature encoder for model[" << modelIDX << "]";
        log.Level (-1) << endl << endl << errMsg << endl << endl;
        throw KKStrException (errMsg);
      }

      int32  xSpaceUsed;
      binaryFeatureEncoders[modelIDX]->EncodeAImage (example, predictXSpace, xSpaceUsed);

      double  distance = 0.0;

      svm_predictTwoClasses (models[modelIDX][0], predictXSpace, distance, -1);
      probability = (1.0 / (1.0 + exp (-1.0 * (thisComboPrameters->Param ().A) * distance)));
      probability = AdjProb (probability);  // KAK 2011-06-10

      crossClassProbTable[class1IDX][class2IDX] = probability;
      crossClassProbTable[class2IDX][class1IDX] = 1.0 - probability;

      if  (probability > 0.5f)
      {
        probabilities[class1IDX] *= probability;
        probabilities[class2IDX] *= (1.0f - probability);
        votes[class1IDX]++;
      }
      else
      {
        probabilities[class2IDX] *= (1.0f - probability);
        probabilities[class1IDX] *= probability;
        votes[class2IDX]++;
      }

      modelIDX++;
    }
  }

  double  totProbability = 0.0;

  for  (classIDX = 0;  classIDX < numOfClasses;  classIDX++)
  {
    totProbability += probabilities[classIDX];
  }

  if  (totProbability == 0.0f)
    totProbability = 1.0f;

  {
    int32  callersIdx = 0;
    MLClassConstList::const_iterator  idx;
    for  (idx = _mlClasses.begin ();  idx != _mlClasses.end ();  idx++)
    {
      int32  ourIdx = assignments.GetNumForClass (*idx);
      if  ((ourIdx < 0)  ||  (ourIdx >= numOfClasses))
      {
        // For what ever reason the MLClass instance in '_mlClasses' provoded by caller
        // is not one of the classes that this model was built for.
        log.Level (-1) << endl 
          << "SVMModel::PredictProbabilitiesByBinaryCombos   ***WARNING***    MLClass["
          << (*idx)->Name () << "] is not one of the classes in SVMModel."
          << endl << endl;
        _votes        [callersIdx] = 0;
        _probabilities[callersIdx] = 0.0;
      }
      else
      {
        _votes        [callersIdx] = votes        [ourIdx];
        _probabilities[callersIdx] = probabilities[ourIdx] / totProbability;
      }
      callersIdx++;
    }
  }

  return;
}  /* PredictByBinaryCombos */



namespace  MLL
{
  bool  PairCompareOperator(ProbNamePair l, ProbNamePair  r) 
  { 
    return l.probability > r.probability;
  }
}



vector<KKStr>  SVMModel::SupportVectorNames (MLClassConstPtr  c1,
                                             MLClassConstPtr  c2
                                            )  const
{
  vector<KKStr>  results;
  if  (svmParam.MachineType () != BinaryCombos)
    return  results;

  // Locate the binary parms in question.
  int32  modelIDX = 0;
  BinaryClassParmsPtr  parms = NULL;
  for  (modelIDX = 0;  modelIDX < numOfModels;  modelIDX++)
  {
    parms = binaryParameters[modelIDX];
    if  ((parms->Class1 () == c1)  &&  (parms->Class2 () == c2))
      break;

    else if  ((parms->Class2 () == c1)  &&  (parms->Class1 () == c2))
      break;
  }

  if  (modelIDX >= numOfModels)
  {
    // Binary Combo not found.
    log.Level (-1) << endl << endl
                   << "SVMModel::SupportVectorNames   ***ERROR***  Class1[" << c1->Name () << "]  Class2[" << c2->Name () << "]  not part of model." << endl
                   << endl;
    return results;
  }

  int32  numSVs = models[modelIDX][0]->l;
  int32  svIDX = 0;
  for  (svIDX = 0;  svIDX < numSVs;  svIDX++)
  {
    KKStr  svName = models[modelIDX][0]->SupportVectorName (svIDX);
    results.push_back (svName);
  }
  return  results;
}  /* SupportVectorNames */



vector<KKStr>  SVMModel::SupportVectorNames () const
{
  vector<KKStr>  results;
  if  (svmParam.MachineType () != BinaryCombos)
    return  results;

  map<KKStr,KKStr>  names;
  map<KKStr,KKStr>::iterator  svnIDX;

  // Locate the binary parms in question.
  int32  modelIDX = 0;
  BinaryClassParmsPtr  parms = NULL;
  for  (modelIDX = 0;  modelIDX < numOfModels;  modelIDX++)
  {
    parms = binaryParameters[modelIDX];
  
    int32  numSVs = models[modelIDX][0]->l;
    int32  svIDX = 0;
    for  (svIDX = 0;  svIDX < numSVs;  svIDX++)
    {
      KKStr  svName = models[modelIDX][0]->SupportVectorName (svIDX);
      svnIDX = names.find (svName);
      if  (svnIDX == names.end ())
      {
        names.insert (pair<KKStr,KKStr>(svName, svName));
        results.push_back (svName);
      }
    }
  }

  return  results;
}  /* SupportVectorNames */



vector<ProbNamePair>  SVMModel::FindWorstSupportVectors (FeatureVectorPtr    example,
                                                         int32               numToFind,
                                                         MLClassConstPtr  c1,
                                                         MLClassConstPtr  c2
                                                        )
{
  vector<ProbNamePair>  results;
  if  (svmParam.MachineType () != BinaryCombos)
    return  results;

  // Locate the binary parms in question.
  bool  c1RevFlag = false;
  int32  modelIDX = 0;

  BinaryClassParmsPtr  parms = NULL;

  for  (modelIDX = 0;  modelIDX < numOfModels;  modelIDX++)
  {
    parms = binaryParameters[modelIDX];
    if  ((parms->Class1 () == c1)  &&  (parms->Class2 () == c2))
    {
      c1RevFlag = false;
      break;
    }
    else if  ((parms->Class2 () == c1)  &&  (parms->Class1 () == c2))
    {
      c1RevFlag = true;
      break;
    }
  }

  if  (modelIDX >= numOfModels)
  {
    // Binary Combo not found.
    log.Level (-1) << endl << endl
                   << "SVMModel::FindWorstSupportVectors   ***ERROR***  Class1[" << c1->Name () << "]  Class2[" << c2->Name () << "]  not part of model." << endl
                   << endl;
    return results;
  }


  if  (binaryFeatureEncoders[modelIDX] == NULL)
  {
    KKStr  errMsg;
    errMsg << "SVMModel::FindWorstSupportVectors   ***ERROR***   No feature encoder for model[" << modelIDX << "]";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  int32  xSpaceUsed;
  binaryFeatureEncoders[modelIDX]->EncodeAImage (example, predictXSpace, xSpaceUsed);

  int32  svIDX = 0;
  //int32  numSVs = models[modelIDX][0]->l;
  int32  numSVs = models[modelIDX][0]->l;

  double  origProbabilityC1 = 0.0;
  double  probabilityC1     = 0.0;
  double  distance          = 0.0;

  svm_predictTwoClasses (models[modelIDX][0], predictXSpace, distance, -1);
  origProbabilityC1 = ((1.0 / (1.0 + exp (-1.0 * (parms->Param ().A) * distance))));
  origProbabilityC1 = AdjProb (origProbabilityC1);  // KAK 2011-06-10

  if  (c1RevFlag)
    origProbabilityC1 = 1.0f - origProbabilityC1;

  vector<ProbNamePair>  candidates;

  for  (svIDX = 0;  svIDX < numSVs;  svIDX++)
  {
    svm_predictTwoClasses (models[modelIDX][0], predictXSpace, distance, svIDX);
    probabilityC1 = ((1.0 / (1.0 + exp (-1.0 * (parms->Param ().A) * distance))));
    probabilityC1 = AdjProb (probabilityC1);  // KAK 2011-06-10
    if  (c1RevFlag)
      probabilityC1 = 1.0f - probabilityC1;

    double  deltaProb = probabilityC1 - origProbabilityC1;
    KKStr   svName = models[modelIDX][0]->SupportVectorName (svIDX);
    candidates.push_back (ProbNamePair (svName, deltaProb));
  }

  sort (candidates.begin (), candidates.end (), PairCompareOperator);

  int32  zed = 0;
  for  (zed = 0;  (zed < (int32)candidates.size ())  &&  (zed < numToFind);  zed++)
    results.push_back (candidates[zed]);

  return  results;
}  /* FindWorstSupportVectors */



vector<ProbNamePair>  SVMModel::FindWorstSupportVectors2 (FeatureVectorPtr    example,
                                                          int32               numToFind,
                                                          MLClassConstPtr  c1,
                                                          MLClassConstPtr  c2
                                                         )
{
  vector<ProbNamePair>  results;
  if  (svmParam.MachineType () != BinaryCombos)
    return  results;

  // Locate the binary parms in question.
  bool  c1RevFlag = false;
  int32  modelIDX = 0;

  BinaryClassParmsPtr  parms = NULL;

  for  (modelIDX = 0;  modelIDX < numOfModels;  modelIDX++)
  {
    parms = binaryParameters[modelIDX];
    if  ((parms->Class1 () == c1)  &&  (parms->Class2 () == c2))
    {
      c1RevFlag = false;
      break;
    }
    else if  ((parms->Class2 () == c1)  &&  (parms->Class1 () == c2))
    {
      c1RevFlag = true;
      break;
    }
  }

  if  (modelIDX >= numOfModels)
  {
    // Binary Combo not found.
    log.Level (-1) << endl << endl
                   << "SVMModel::FindWorstSupportVectors   ***ERROR***  Class1[" << c1->Name () << "]  Class2[" << c2->Name () << "]  not part of model." << endl
                   << endl;
    return results;
  }


  if  (binaryFeatureEncoders[modelIDX] == NULL)
  {
    KKStr  errMsg;
    errMsg << "SVMModel::FindWorstSupportVectors2   ***ERROR***   No feature encoder for model[" << modelIDX << "]";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  int32  xSpaceUsed;
  binaryFeatureEncoders[modelIDX]->EncodeAImage (example, predictXSpace, xSpaceUsed);

  int32  svIDX = 0;
  //int32  numSVs = models[modelIDX][0]->l;
  int32  numSVs = models[modelIDX][0]->l;

  double  origProbabilityC1 = 0.0;
  double  probabilityC1     = 0.0;
  double  distance          = 0.0;

  svm_predictTwoClasses (models[modelIDX][0], predictXSpace, distance, -1);
  origProbabilityC1 = ((1.0 / (1.0 + exp (-1.0 * (parms->Param ().A) * distance))));
  origProbabilityC1 = AdjProb (origProbabilityC1);
  if  (c1RevFlag)
    origProbabilityC1 = 1.0 - origProbabilityC1;

  vector<ProbNamePair>  candidates;

  //{
  //  svm_problem*  subSetProb = svm_BuildProbFromTwoClassModel  (models[modelIDX][0], -1);
  //  svm_parameter  parameters = models[modelIDX][0]->param;
  //  parameters.nr_class = 2;

  //  svm_model*  subSetModel = svm_train (subSetProb, &parameters);

  //  svm_predictTwoClasses (subSetModel, predictXSpace, distance, -1);
  //  probabilityC1 = ((1.0 / (1.0 + exp (-1.0 * (parms->Param ().A) * distance))));
  //  probabilityC1 = AdjProb (probabilityC1);
  //  if  (c1RevFlag)
  //    probabilityC1 = 1.0f - probabilityC1;
  //  int32 zed = 100;
  //}

  for  (svIDX = 0;  svIDX < numSVs;  svIDX++)
  {
    svm_problem*  subSetProb = svm_BuildProbFromTwoClassModel  (models[modelIDX][0], svIDX);
    svm_parameter  parameters = models[modelIDX][0]->param;
    parameters.nr_class = 2;

    svm_model*  subSetModel = svm_train (subSetProb, &parameters);

    svm_predictTwoClasses (subSetModel, predictXSpace, distance, -1);
    probabilityC1 = ((1.0 / (1.0 + exp (-1.0 * (parms->Param ().A) * distance))));
    probabilityC1 = AdjProb (probabilityC1);
    if  (c1RevFlag)
      probabilityC1 = 1.0f - probabilityC1;

    double  deltaProb = probabilityC1 - origProbabilityC1;

    KKStr  svName = models[modelIDX][0]->SupportVectorName (svIDX);
    candidates.push_back (ProbNamePair (svName, deltaProb));

    subSetModel->Dispose ();

    delete  subSetModel;  subSetModel = NULL;
    delete  subSetProb;   subSetProb = NULL;
  }

  sort (candidates.begin (), candidates.end (), PairCompareOperator);

  int32  zed = 0;
  for  (zed = 0;  (zed < (int32)candidates.size ())  &&  (zed < numToFind);  zed++)
    results.push_back (candidates[zed]);

  return  results;
}  /* FindWorstSupportVectors2 */



void SVMModel::CalculatePredictXSpaceNeeded ()
{
  int32 z;
  int32 numFeaturesAfterEncoding = 0;
  int32 numOfFeaturesSelected = selectedFeatures.NumOfFeatures ( );

  switch (svmParam.EncodingMethod())
  {
  case BinaryEncoding:
    for  (z = 0;  z < numOfFeaturesSelected;  z++)
    {
      if  ((type_table[selectedFeatures[z]] == NominalAttribute)  ||  (type_table[selectedFeatures[z]] == SymbolicAttribute))
         numFeaturesAfterEncoding += cardinality_table[selectedFeatures[z]];
      else
         numFeaturesAfterEncoding ++;
    }
    break;

  case ScaledEncoding:
  case NoEncoding:
  default:
    //numFeaturesAfterEncoding = fileDesc->NumOfFields ( );
    numFeaturesAfterEncoding = selectedFeatures.NumOfFeatures ();
    break;
  }

  numFeaturesAfterEncoding++;  // extra node for -1 index 

  predictXSpaceWorstCase = numFeaturesAfterEncoding + 10;

  // We need to make sure that 'predictXSpace' is bigger than the worst possible case.
  // When doing the Binary Combo case we will assume that this can be all EncodedFeatures.
  // Since I am really only worried about the BinaryCombo case with Plankton data where there 
  // is not encded fields the number of attributes in the FileDesc should surfice.
  if  (predictXSpaceWorstCase < (fileDesc->NumOfFields () + 10))
     predictXSpaceWorstCase = fileDesc->NumOfFields () + 10;


  delete  predictXSpace;
  predictXSpace = new svm_node[predictXSpaceWorstCase];
}  /* CalculatePredictXSpaceNeeded */




int32  SVMModel::EncodeImage (FeatureVectorPtr  example,
                            svm_node*         row
                           )
{
  if  (!featureEncoder)
  {
    featureEncoder = new FeatureEncoder (svmParam, 
                                         fileDesc,
                                         type_table,
                                         cardinality_table,
                                         NULL,              // class1, set to NULL because we are dealing with all classes
                                         NULL,              // class2,     ""          ""            ""            ""
                                         log
                                        );
  }

  int32  xSpaceNodesNeeded = 0;
  featureEncoder->EncodeAImage (example, row, xSpaceNodesNeeded);
  return  xSpaceNodesNeeded;
}  /* EncodeImage */



void SVMModel::ConstructOneVsOneModel (FeatureVectorListPtr  examples,
                                       svm_problem&          prob
                                      )
{
  //**** Start of new compression replacement code.
  int32  totalxSpaceUsed = 0;

  numOfModels = 1;
  models      = new ModelPtr  [numOfModels];
  xSpaces     = new XSpacePtr [numOfModels];

  {
    int32 x;
    for  (x = 0;  x < numOfModels;  x++)
    {
      models[x]  = NULL;
      xSpaces[x] = NULL;
    }
  }

  delete  featureEncoder;
  featureEncoder = new FeatureEncoder (svmParam, 
                                       fileDesc,
                                       type_table,
                                       cardinality_table,
                                       NULL,              // class1, set to NULL because we are dealing with all classes
                                       NULL,              // class2,     ""          ""            ""            ""
                                       log
                                      );

  compression_stats  = featureEncoder->Compress (examples,
                                                 assignments,
                                                 xSpaces[0], 
                                                 totalxSpaceUsed,
                                                 prob
                                                );
  xSpacesTotalAllocated += totalxSpaceUsed;

  //**** End of new compression replacement code.
  // train the model using the svm_problem built above
  double  startTrainingTime = osGetSystemTimeUsed ();
  models[0] = SvmTrainModel (svmParam.Param (), prob);
  double  endTrainingTime = osGetSystemTimeUsed ();
  trainingTime = endTrainingTime - startTrainingTime;

  //delete  compressedImageList;  compressedImageList = NULL;
  //imagesForBuildProblem = NULL;

  // free the memory for the svm_problem
  delete[] prob.index;  prob.index = NULL;
  free (prob.y);        prob.y = NULL;
  if  (xSpaces[0] != NULL)
  {
    // We can only free prob.x if xSpacs[0] was allocated otherwise we need
    // prob.x to located xSpace that will need to be deleted later.
    free (prob.x);  prob.x = NULL;
  }
  delete[]  prob.W;     prob.W = NULL;
}  /* ConstructOneVsOneModel */




void SVMModel::ConstructOneVsAllModel (FeatureVectorListPtr examples,
                                       svm_problem&         prob
                                      )
{
  MLClassConstListPtr  allClasses = examples->ExtractMLClassConstList ();

  VectorShort  assignmentNums = assignments.GetUniqueListOfAssignments ();
  numOfModels = (int32)assignmentNums.size ();

  models      = new ModelPtr  [numOfModels];
  xSpaces     = new XSpacePtr [numOfModels];
  {
    int32 x;
    for  (x = 0;  x < numOfModels;  x++)
    {
      models[x]  = NULL;
      xSpaces[x] = NULL;
    }
  }

  featureEncoder = new FeatureEncoder (svmParam, 
                                       fileDesc,
                                       type_table,
                                       cardinality_table,
                                       NULL,              // class1, set to NULL because we are dealing with all classes
                                       NULL,              // class2,     ""          ""            ""            ""
                                       log
                                      );


  int32  modelIDX = 0;
  int32  assignmentIDX;

  trainingTime = 0;
  oneVsAllAssignment.erase (oneVsAllAssignment.begin (), oneVsAllAssignment.end ());

  {
    int32  modelIDX;
    delete  oneVsAllClassAssignments;
    oneVsAllClassAssignments = new ClassAssignmentsPtr[numOfModels];
    for  (modelIDX = 0;  modelIDX < numOfModels;  modelIDX++)
      oneVsAllClassAssignments[modelIDX] = NULL;
  }

  // build the models
  for (assignmentIDX = 0;  assignmentIDX < numOfModels;  assignmentIDX++)
  {
    short  assignmentNum = assignmentNums[assignmentIDX];
    oneVsAllAssignment.push_back (assignmentNum);

    MLClassConstList  classesThisAssignment = assignments.GetMLClasses (assignmentNum);

    CompressionStats  oneModelStats 
           = BuildProblemOneVsAll (*examples, 
                                   prob, 
                                   xSpaces[modelIDX], 
                                   classesThisAssignment, 
                                   featureEncoder, 
                                   *allClasses, 
                                   oneVsAllClassAssignments [modelIDX]
                                  );

    compression_stats += oneModelStats;

    // train the model using the svm_problem built above

    double  trainTimeStart = osGetSystemTimeUsed ();
    models[modelIDX] = SvmTrainModel (svmParam.Param (), prob);
    double  trainTimeEnd   = osGetSystemTimeUsed ();
    trainingTime += (trainTimeEnd - trainTimeStart);

    // free the memory for the svm_problem
    delete  [] prob.index;  prob.index = NULL;
    free (prob.y);   prob.y = NULL;
    free (prob.x);   prob.x = NULL;
    delete[] prob.W;

    modelIDX++;
  }

  delete  allClasses;
}  /* ConstructOneVsAllModel */



void SVMModel::ConstructBinaryCombosModel (FeatureVectorListPtr examples)
{
  log.Level (10) << "SVMModel::ConstructBinaryCombosModel" << endl;

  int32 maxXSpaceNeededPerImage = 0;
  
  numOfModels           = (numOfClasses * (numOfClasses - 1)) / 2;

  models                 = new ModelPtr            [numOfModels];
  xSpaces                = new XSpacePtr           [numOfModels];
  binaryParameters       = new BinaryClassParmsPtr [numOfModels];
  binaryFeatureEncoders  = new FeatureEncoderPtr   [numOfModels];

  int32  modelIDX = 0;
  int32  class1IDX;
  int32  class2IDX;
  int32  x;

  for  (x = 0;  x < numOfModels;  x++)
  {
    models                [x] = NULL;
    binaryParameters      [x] = NULL;
    binaryFeatureEncoders [x] = NULL;
    xSpaces               [x] = NULL;
  }

  FeatureVectorListPtr srcImages        = examples;
  FeatureVectorListPtr compressedImages = NULL;

  if  (svmParam.CompressionMethod () == BRcompressionPre)
  {
    FeatureNumList  featuresToUseForCompression (fileDesc);
    for (class1IDX = 0;  class1IDX < (numOfClasses - 1);  class1IDX++)
    {
      MLClassConstPtr  class1 = assignments.GetMLClassByIndex (class1IDX);
      for  (class2IDX = class1IDX + 1;   class2IDX < numOfClasses;   class2IDX++)
      {
        MLClassConstPtr  class2 = assignments.GetMLClassByIndex (class2IDX);
        FeatureNumList featuresTheseTwoClasses = svmParam.GetFeatureNums (class1, class2);
        featuresToUseForCompression += featuresTheseTwoClasses;
      }
    }

    // We want to do compression before processing examples by binary classes.
    compressedImages = new FeatureVectorList (fileDesc, true, log);
    BitReduction  br (svmParam, fileDesc, featuresToUseForCompression, log);
    compression_stats = br.compress (*examples, 
                                     compressedImages, 
                                     assignments
                                    );

    compressedImages->Owner (true);
    srcImages = compressedImages;
  }

  FeatureVectorListPtr*   imagesByClass = BreakDownImagesByClass (srcImages);


  // NOTE: compression is performed in the BuildProblemBinaryCombos() function since
  // we can do the compression on just the example for those two classes - KNS

  // build a model for each possible 2-class combination
  for (class1IDX = 0;  (class1IDX < (numOfClasses - 1))  &&  (!cancelFlag);  class1IDX++)
  {
    MLClassConstPtr  class1 = assignments.GetMLClassByIndex (class1IDX);

    for  (class2IDX = class1IDX + 1;   (class2IDX < numOfClasses)   &&  (!cancelFlag);   class2IDX++)
    {
      MLClassConstPtr  class2 = assignments.GetMLClassByIndex (class2IDX);

      log.Level (10) << "Constructing  " << class1->Name () << " - " << class2->Name () << endl;

      struct svm_problem  prob;
      //memset (&prob, 0, sizeof(svm_problem));  kak

      binaryParameters      [modelIDX] = NULL;
      binaryFeatureEncoders [modelIDX] = NULL;
      xSpaces               [modelIDX] = NULL;

      // build the svm_problem for the current combination
      BuildProblemBinaryCombos (imagesByClass[class1IDX],
                                imagesByClass[class2IDX],
                                svmParam,
                                binaryParameters      [modelIDX],
                                binaryFeatureEncoders [modelIDX],
                                prob, 
                                xSpaces               [modelIDX], 
                                class1, 
                                class2
                               );

      maxXSpaceNeededPerImage = Max (maxXSpaceNeededPerImage, binaryFeatureEncoders [modelIDX]->XSpaceNeededPerImage ());

      // train the model
      double  startTrainingTime = osGetSystemTimeUsed ();
      models[modelIDX] = SvmTrainModel (binaryParameters[modelIDX]->Param (), prob);
      double  endTrainingTime = osGetSystemTimeUsed ();
      trainingTime += (endTrainingTime - startTrainingTime);


      /*
      {
        {
          ofstream xxx ("c:\\Temp\\testModel.txt");
          SvmSaveModel (xxx, models[modelIDX]);
          xxx.close ();
        }

        {
          FILE*  xxx = fopen ("c:\\Temp\\testModel.txt", "r");
          svm_model **  m = SvmLoadModel (xxx);
          fclose (xxx);
        }
      }
      */

      // log.Level (10) << "Support Vectors[" << models[modelIDX][0]->l << "]" << endl;

      //osWaitForEnter ();

      // free the memory for the svm_problem
      delete  [] prob.index;  prob.index = NULL;
      free (prob.y);      prob.y     = NULL;
      free (prob.x);      prob.x     = NULL;
      delete[] prob.W;

      modelIDX++;   
    }
  }

  {
    int32  x;
    for  (x = 0;  x < (int32)assignments.size ();  x++)
      delete  imagesByClass[x];
    delete[]  imagesByClass;
    imagesByClass = NULL;
  }

  predictXSpaceWorstCase = maxXSpaceNeededPerImage + 10;

  // We need to make sure that 'predictXSpace' is bigger than the worst possible case.
  // When doing the Binary Combo case we will assume that this can be all EncodedFeatures.
  // Since I am really only worried about the BinaryCombo case with Plankton data where there 
  // is not encded fields the number of attributes in the FileDesc should surfice.
  if  (predictXSpaceWorstCase < (fileDesc->NumOfFields () + 10))
    predictXSpaceWorstCase = fileDesc->NumOfFields () + 10;

  delete  predictXSpace;  predictXSpace = NULL;
  predictXSpace = new svm_node[predictXSpaceWorstCase];

  delete  compressedImages;

  log.Level (10) << "SVMModel::ConstructBinaryCombosModel  Done." << endl;
}  /* ConstructBinaryCombosModel */



FeatureVectorListPtr*   SVMModel::BreakDownImagesByClass (FeatureVectorListPtr  examples)
{
  int32  x;

  FeatureVectorListPtr* imagesByClass = new FeatureVectorListPtr[numOfClasses];
  for  (x = 0;  x < numOfClasses;  x++)
    imagesByClass[x] = new FeatureVectorList (fileDesc, false, log, 1000);

  MLClassConstPtr  lastMLClass = NULL;
  int32            classIdx       = 0;

  FeatureVectorList::iterator  idx;

  for  (idx = examples->begin ();  idx != examples->end ();  idx++)
  {
    FeatureVectorPtr example = *idx;
    
    if  (lastMLClass != example->MLClass ())
    {
      lastMLClass = example->MLClass ();
      classIdx       = assignments.GetNumForClass (lastMLClass);
    }

    imagesByClass[classIdx]->PushOnBack (example);
  }

  return  imagesByClass;
}  /* BreakDownImagesByClass */



CompressionStats  SVMModel::Compress (FeatureVectorListPtr images_list, 
                                      FeatureVectorListPtr compressed_examples_list
                                     )
{
  CompressionStats stats;

  switch (svmParam.CompressionMethod())
  {
    case BRcompressionPost:
    {
      BitReduction br (svmParam, images_list->FileDesc (), selectedFeatures, log);
      stats = br.compress (*images_list, 
                           compressed_examples_list, 
                           assignments
                          );
    }
    break;

  default:
    double time_before, time_after;
    
    time_before = osGetSystemTimeUsed ();

    FeatureVectorList::iterator  i;
    for (i = images_list->begin (); i != images_list->end ();  i++)
    {
      FeatureVectorPtr  example = *i;
      compressed_examples_list->PushOnBack (new FeatureVector (*example));
    }
    
    time_after = osGetSystemTimeUsed ();

    stats.num_images_before = (long)images_list->size();
    stats.num_images_after = (long)compressed_examples_list->size();
    stats.compression_ratio = 0.0f;
    stats.elapsed_cpu_time = time_after-time_before;
    break;
  }

  log.Level (50) << "COMPRESSION: # examples before was "   << stats.num_images_before  << endl;
  log.Level (50) << "COMPRESSION: # examples after was "    << stats.num_images_after   << endl;
  log.Level (50) << "COMPRESSION: Compression ratio was "   << stats.compression_ratio  << endl;
  log.Level (50) << "COMPRESSION: Reduction Time was "      << stats.elapsed_cpu_time   << endl;

  return stats;
}  /* Compress */



bool  SVMModel::NormalizeNominalAttributes ()
{
  if  (svmParam.EncodingMethod () == NoEncoding)
    return  true;
  else
    return  false;
}  /* NormalizeNominalAttributes */



void  SVMModel::SetSelectedFeatures (const FeatureNumList& _selectedFeatures)
{
  selectedFeatures = _selectedFeatures;
  CalculatePredictXSpaceNeeded ();
}  /* SetSelectedFeatures */




void  SVMModel::RetrieveCrossProbTable (MLClassConstList&   classes,
                                        double**               crossProbTable  // two dimension matrix that needs to be classes.QueueSize ()  squared.
                                       )
{
  if  (classes.QueueSize () != crossClassProbTableSize)
  {
    // There Class List does not have the same number of entries as our 'CrossProbTable'
    log.Level (-1) << endl
                   << "SVMModel::RetrieveCrossProbTable    ***ERROR***" << endl
                   << "            classes.QueueSize ()[" << classes.QueueSize () << "] != crossClassProbTableSize[" << crossClassProbTableSize << "]" << endl
                   << endl;
    return;
  }

  int32*  indexTable = new int32[classes.QueueSize ()];
  int32  x, y;
  for  (x = 0;  x < classes.QueueSize ();  x++)
  {
    for  (y = 0;  y < classes.QueueSize ();  y++)
       crossProbTable[x][y] = 0.0;

    indexTable[x] = assignments.GetNumForClass (classes.IdxToPtr (x));
    if  (indexTable[x] < 0)
    {
      log.Level (-1) << endl << endl
                     << "SVMModel::RetrieveCrossProbTable      ***WARNING***" << endl
                     << endl
                     << "      Class Index[" << x << "]  Name[" << classes[x].Name () << "]" << endl
                     << "      will populate this index with zeros."                         << endl
                     << endl;
    }
  }

  if  (classes.QueueSize () != crossClassProbTableSize)
  {
    log.Level (-1) << endl
                   << "SVMModel::RetrieveCrossProbTable      ***ERROR***"                                       << endl
                   << "                                     'classes.QueueSize () != crossClassProbTableSize'"  << endl
                   << endl;
    return;
  }


  // x,y         = 'Callers'   Class Indexes..
  // xIdx, yIdx  = 'SVMNodel'  Class Indexed.
  for  (x = 0;  x < classes.QueueSize ();  x++)
  {
    int32 xIdx = indexTable[x];
    if  (xIdx >= 0)
    {
      for  (y = 0;  y < classes.QueueSize ();  y++)
      {
        int32  yIdx = indexTable[y];
        if  (yIdx >= 0)
        {
          if  ((x != xIdx)  ||  (y != yIdx))
          {
            //kak  I just added this check to see when this situation actually occurs.
            int32 zed = 111;
          }
          crossProbTable[x][y] = this->crossClassProbTable[xIdx][yIdx];
        }
      }
    }
  }

  delete  indexTable;  indexTable = NULL;
  return;
}  /* RetrieveCrossProbTable */
