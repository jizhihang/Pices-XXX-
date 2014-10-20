#ifndef  _SVMMODEL_
#define  _SVMMODEL_
//***********************************************************************
//*                           SVMModel                                  *
//*                                                                     *
//*  Represents a training model for svmlib.  Uses Image Feature data   *
//*  MLClasses, libsvm parameters, and included features to build    *
//*  a svm training model.  You can save this model to disk for later   *
//*  use without having to train again. An application can create       *
//*  several instances of this model for different phases of a          *
//*  clasification architectuure.                                       *
//***********************************************************************


#include "Str.h"
#include "ClassAssignments.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "MLClassConstList.h"
#include "FeatureVector.h"
#include "svm.h"
#include "BitReduction.h"
#include "SVMparam.h"



namespace MLL 
{

  #ifndef  _FEATURENUMLIST_
  class  FeatureNumList;
  typedef  FeatureNumList*  FeatureNumListPtr;
  #endif

  #ifndef  _FEATUREENCODER_
  class  FeatureEncoder;
  typedef  FeatureEncoder*  FeatureEncoderPtr;
  #endif



  #ifndef  _RUNLOG_
  class  RunLog;
  typedef  RunLog*  RunLogPtr;
  #endif


  #ifndef _FILEDESC_
  class  FileDesc;
  typedef  FileDesc*  FileDescPtr;
  #endif





  typedef  struct svm_node*     XSpacePtr;



  class  ProbNamePair
  {
  public:
    ProbNamePair (KKStr   _name,
                  double  _probability
                  ):
        name (_name),  probability (_probability)
        {}

    KKStr   name;
    double  probability;
  };



  class  SVMModel
  {
  public:
    typedef  KKU::uint32  uint32;
    typedef  KKU::ulong   ulong;

    /**
     *@brief Loads an SVM model from disk
     *@param[in]  _rootFileName The filename for the model; without an extension.
     *@param[out] _successful Set to true if the model is succesfulyl loaded, false otherwise
     *@param[in]  _fileDesc A description of the training data that was used to train the classifier.
     *@param[in]  _log A logfile stream. All important events will be ouput to this stream
     *@param[in]  _cancelFlag  If set to true any process running in SVMModel will terminate.
     */
    SVMModel (const KKStr&          _rootFileName,   
              bool&                 _successful,
              FileDescPtr           _fileDesc,
              RunLog&               _log,
              volatile const bool&  _cancelFlag
             );
    

    /**
     *@brief Loads an SVM model from disk.
     *@details  The '_cancelFlag' parameter is meant to allow another thread to cancel processing
     *          by a different thread in SVMmodel.  Ex:  If one thread is building a new SVM for
     *          this instance of SVMmodel, that thread will periodcaly monitor '_cancelFlag', if
     *          it is set to true it will terminate its processing.
     *@param[in]  _in A file stream to read and build SVMModel from.
     *@param[out] _successful Set to true if the model is succesfulyl loaded, false otherwise
     *@param[in]  _fileDesc A description of the data file. I'm not sure this is needed for this function.
     *@param[in]  _log A logfile stream. All important events will be ouput to this stream
     *@param[in]  _cancelFlag  If set to true any process running in SVMModel will terminate.
     */
    SVMModel (istream&              _in,   // Create from existing Model on Disk.
              bool&                 _successful,
              FileDescPtr           _fileDesc,
              RunLog&               _log,
              volatile const bool&  _cancelFlag
             );



    /**
     *@brief  Constructor that will create a svmlib training model using the 
     *        features and classes for training purposes.
     *@param[in]  _svmParam  Specifies the parameters to be used for trainig.  These
     *            are the same parameters that you would specify in the command line 
     *            to svm_train.  Plus the feature nums to be  used.
     *@param[in]  _examples  Training data for the classifier.
     *@param[in]  _assignments List of classes and there assovciated number ti be 
     *            used by the SVM.  You can merge 1 or more classes by assigning them 
     *            the same number.  This nummber will be used by the SVM.  When 
     *            predictions are done by SVM it return a number, _assignments will 
     *            then be used to map backto the correct class.
     *@param[in] _fileDesc  ile Description thaty describes the training data.
     *@param[out] _log Log file to log messages to.
     *@param[in]  _cancelFlag  The training process will monitor this flag; if it goes true it
     *            return to caller.
     */
    SVMModel (SVMparam&             _svmParam,
              FeatureVectorList&    _examples,
              ClassAssignments&     _assignments,
              FileDescPtr           _fileDesc,
              RunLog&               _log,
              volatile const bool&  _cancelFlag
             );


    /**
     *@brief Frees any memory allocated by, and owned by the SVMModel
     */
    virtual ~SVMModel ();

    const FeatureNumList&   GetFeatureNums ()  const;

    const FeatureNumList&   GetFeatureNums (MLClassConstPtr  class1,
                                            MLClassConstPtr  class2
                                           )  const;

    //MLClassList&  MLClasses ()  {return mlClasses;}  
    const ClassAssignments&  Assignments () const  {return assignments;}

    //int32                DuplicateDataCount () const {return duplicateCount;}

    int32              MemoryConsumedEstimated ()  const;

    virtual
    bool               NormalizeNominalAttributes ();  // Return tru, if nominal fields
                                                       // need to be normalized.

    int32              NumOfClasses ()  const  {return numOfClasses;}

    int32              NumOfSupportVectors () const;

    void               SupportVectorStatistics (int32& numSVs,
                                                int32& totalNumSVs
                                               );

    const KKStr&       BitReductionByFeature     () const {return compression_stats.bitReductionByFeature;}
    double             ReductionTime             () const {return compression_stats.elapsed_cpu_time;  }
    int32              ReductionPreExampleCount  () const {return compression_stats.num_images_before; }
    int32              ReductionPostExampleCount () const {return compression_stats.num_images_after;  }
    float              ReductionRatio            () const {return compression_stats.compression_ratio; }

    const CompressionStats&  Compression_Stats () const {return compression_stats;}

    const SVMparam&    SVMParameters           () const {return svmParam;}

    const KKStr&       RootFileName            () const {return rootFileName;}


    SVM_SelectionMethod   SelectionMethod      () const {return svmParam.SelectionMethod ();}

    double             TrainingTime            () const {return trainingTime;}


    double   DistanceFromDecisionBoundary (FeatureVectorPtr    example,
                                           MLClassConstPtr  class1,
                                           MLClassConstPtr  class2
                                          );


    MLClassConstPtr   Predict (FeatureVectorPtr  example);


    /**
     *@brief  Will predict the two most likely classes of 'example'.
     *@param[in]  example Example to predict on.
     *@param[in]  knownClass  Class that we already no the example to be;  can be pointing to NULL indicating that you do not know.
     *@param[out] predClass1  Most likely class; depending on classifier paraketers this could be by number of votes or probability.
     *@param[out] predClass2  Second most likely class.
     *@param[out] predClass1Votes Number votes 'predClass1' received.
     *@param[out] predClass2Votes Number votes 'predClass2' received.
     *@param[out] probOfKnownClass Probability of 'knownClass' if specified.
     *@param[out] predClassProb Probability of 'predClass1' if specified.
     *@param[out] predClass2Prob Probability of 'predClass2' if specified.
     *@param[out] compact  Minus log likilihood of known class (-log (probabilities[knownClassNum]));
     *@param[out] numOfWinners  Number of classes that had the sam enumber of votes as the winning class.
     *@param[out] knownClassOneOfTheWinners  Will return true if the known class was one of the classes that had the highest numberof votes.
     *@param[out] breakTie The difference in probability between the classes with thetwo hiest probabilities.
     */
    void   Predict (FeatureVectorPtr     example,
                    MLClassConstPtr   knownClass,
                    MLClassConstPtr&  predClass1,
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
                   );


    /**
     *@brief  Will get the probabilities assigned to each class.
     *@param[in]  example unknown example that we want to get predicted probabilities for. 
     *@param[in]  _mlClasses  List classes that caller is aware of.  This should be the same list that 
     *                           was used when consttructing this SVMModel object.  The list must be the 
     *                           same but not nessasarily in the same order as when SVMModel was 1st 
     *                          constructed.
     *@param[out] _votes  An array that must be as big as the number of classes in _mlClasses.
     *@param[out] _probabilities  An array that must be as big as the number of classes in _mlClasses.  
     *            The probability of class in _mlClasses[x] will be returned in _probabilities[x].
     */
    void  ProbabilitiesByClass (FeatureVectorPtr            example,
                                const MLClassConstList&  _mlClasses,
                                int32*                      _votes,
                                double*                     _probabilities
                               );



    /**
     *@brief  For a given two class pair return the names of the 'numToFind' worst S/V's.
     *@details  This method will iterate through all the S/V's removing them one at a 
     *          time and recompute the decision boundary and probability.  It will then
     *          return the S/V's that when removed improve the probability in 'c1's 
     *          the most.
     *@param[in]  example  Example that was classified incorrectly.
     *@param[in]  numToFind  The number of teh worst examples you are looking for.
     *@param[in]  c1  Class that the 'example; parameter should have been classed as.
     *@param[in]  c2  Class that it was classified as.
     */
    vector<ProbNamePair>  FindWorstSupportVectors (FeatureVectorPtr    example,
                                                   int32               numToFind,
                                                   MLClassConstPtr  c1,
                                                   MLClassConstPtr  c2
                                                  );

   
    /**
     *@brief  For a given two class pair return the names of the 'numToFind' worst S/V's.
     *@details  This method will iterate through all the S/V's removing them one at a 
     *          time and retraining a new SVM and then comparing with the new prediction results.
     *@param[in]  example  Example that was classified incorrectly.
     *@param[in]  numToFind  The number of teh worst examples you are looking for.
     *@param[in]  c1  Class that the 'example; parameter should have been classed as.
     *@param[in]  c2  Class that it was classified as.
     */
    vector<ProbNamePair>  FindWorstSupportVectors2 (FeatureVectorPtr    example,
                                                    int32               numToFind,
                                                    MLClassConstPtr  c1,
                                                    MLClassConstPtr  c2
                                                   );

   
    vector<KKStr>  SupportVectorNames ()  const;


    vector<KKStr>  SupportVectorNames (MLClassConstPtr  c1,
                                       MLClassConstPtr  c2
                                      )  const;


    /**
     *@brief  Will return the probabilities for all pairs of the classes listed in 'classes'.
     *@param[in]  classes  Classes that you wish to get class pair probabilities for;  the order will 
     *            dictate how the two dimensional matrix 'crossProbTable' will be populated.
     *@param[out] crossProbTable  Will contain the probabilities of all the class pairs that can be formed from
     *            the list of classes in 'classes'.  'crossProbTable' will be a two dimension square matrix 
     *            size will be dictated by the number of classes in 'classes'.  Ex:  Entry[3][2] will have the 
     *            contain the probability between classes[3] and classes[2].
     */
    void  RetrieveCrossProbTable (MLClassConstList&  classes,
                                  double**              crossProbTable  // two dimension matrix that needs to be classes.QueueSize ()  squared.
                                 );


    void  Save (const KKStr&  _rootFileName,
                bool&         _successful
               );

    void  Write (ostream&       o,
                 const KKStr&   rootFileName,
                 bool&          _successful
                );



  private:
    typedef  struct svm_model**   ModelPtr;

    FeatureVectorListPtr*   BreakDownImagesByClass (FeatureVectorListPtr  examples);


    void  BuildClassIdxTable ();


    void  BuildCrossClassProbTable ();


    /**
     *@brief Constructs svm_problem structure from the examples passed to it. 
     *@details This is called once for each logical class tyhat is going to be built in 
     *         the One-vs-All. For each class there will be a binary SVM where one class
     *         is pitted against all other classes.  Using the 'classesThisAssignment'
     *         parameter several classes can be grouped together as one logival class.
     *@param[in]  examples The examples to build the svm_problem(s) with
     *@param[out] prob The svm_problem struct that will be constructed
     *@param[in]  xSpace A list of pointers to the memory allocated for all of the 
     *            svm_node stuctures used in building the svm_problem.
     *@param[in]  classesThisAssignment The list of classes that are to be treated 
     *            as class '0' all other classes are treated as class '1'.
     *@param[in]  featureEncoder Used to encode the feature data in 'examples' into 
     *            the format expected by libSVM.
     *@param[in]  allClasses List of all classes;  The ones that are contained in
     *            'classesThisAssignment' will be the '0' class or the "One" in "One-Vs-All"
     *            while classes that are not in 'classesThisAssignment' will be coded as
     *            '1' or the "All" in "One-Vs-All".
     */ 
    CompressionStats  BuildProblemOneVsAll (FeatureVectorList&          examples,
                                            struct svm_problem&         prob,
                                            XSpacePtr&                  xSpace,
                                            const MLClassConstList&  classesThisAssignment,
                                            FeatureEncoderPtr           featureEncoder,
                                            MLClassConstList&        allClasses,
                                            ClassAssignmentsPtr&        classAssignments
                                           );


    /**
     *@brief Constructs svm_problem For two classes SVM. 
     *@param[in] examples The examples to build the svm_problem(s) with
     *@param[in]  class1Examples  Examples for 1st class.  
     *@param[in]  class2Examples  Examples for 2nd class.  
     *@param[in]  _svmParam  SVM Parameters used for building overall classfier.
     *@param[in]  _twoClassParms Parameters fo rthe specific two classes in question.
     *@param[out] _encoder Based off parameters a FeatureEncoder will be built and 
     *            returnede to caller.  Caller will get ownership and be resposnable
     *            for deleting it.
     *@param[out] prob The Resultant two class classifier that will be built; caller
     *            will get ownership.
     *@param[out] xSpace A list of pointers to the S/V's that the built classifier 'prob'
     *            will be referring to.
     *@param[in]  class1 The first class that 'class1Examples' represent.
     *@param[in]  class2 The second class that 'class1Examples' represent.
     */ 
    void  BuildProblemBinaryCombos (FeatureVectorListPtr  class1Examples, 
                                    FeatureVectorListPtr  class2Examples, 
                                    SVMparam&             _svmParam,
                                    BinaryClassParmsPtr&  _twoClassParms,
                                    FeatureEncoderPtr&    _encoder,
                                    struct svm_problem&   prob, 
                                    XSpacePtr&            xSpace, 
                                    MLClassConstPtr    class1, 
                                    MLClassConstPtr    class2
                                   );


    void  PredictProbabilitiesByBinaryCombos (FeatureVectorPtr            example,  
                                              const MLClassConstList&  _mlClasses,
                                              int32*                      _votes,
                                              double*                     _probabilities
                                             );



    /**
     *@brief  calculates the number of features that will be present after encoding, and allocates
     *        predictedXSpace to acomadate the size.
     */
    void  CalculatePredictXSpaceNeeded ();


    /**
     *@brief Attempts to reduce the number of examples in the images_list via compression
     *        This method is not used in MachineType = "Binary"
     *@param[in] images_list The list of examples you want to attempt to reduce
     *@param[out] compressed_examples_list The reduced list of examples
     *@return A CompressionStats struct containing the details of the compression
     */
    CompressionStats Compress (FeatureVectorListPtr images_list, 
                               FeatureVectorListPtr compressed_examples_list
                              );


    /**
     *@brief Builds a BinaryCombo svm model
     *@param[in] examples The examples to use when training the new model
     */
    void ConstructBinaryCombosModel (FeatureVectorListPtr examples);


    /**
     *@brief Builds a OneVsAll svm model to be used by SvnLib
     *@param[in] examples The examples to use when training the new model.
     *@param[out] prob  Data structure used by SvnLib.
     */
    void ConstructOneVsAllModel (FeatureVectorListPtr examples,
                                 svm_problem&         prob
                                );


    /**
     *@brief Builds a OneVsOne svm model
     *@param[in] examples The examples to use when training the new model
     *@param[out] prob  Data structure used by SvnLib.
     */
    void ConstructOneVsOneModel (FeatureVectorListPtr  examples,
                                 svm_problem&          prob
                                );


    /**
     *@brief Converts a single example into the svm_problem format, using the method specified 
     *  by the EncodingMethod() value returned by svmParam
     *@param[in] example That we're converting
     *@param[in] row      The svm_problem structue that the converted data will be stored
     */
    int32  EncodeImage (FeatureVectorPtr  example,
                        svm_node*         row
                       );


    static
    bool  GreaterThan (int32   leftVotes,
                       double  leftProb,
                       int32   rightVotes,
                       double  rightProb
                      );


    static
    void  GreaterVotes (bool     useProbability,
                        int32    numClasses,
                        int32*   votes,
                        int32&   numOfWinners,
                        double*  probabilities,
                        int32&   pred1Idx,
                        int32&   pred2Idx
                       );



    void  SetSelectedFeatures (const FeatureNumList& _selectedFeatures);


    void  PredictOneVsAll (XSpacePtr            xSpace,
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
                          );



    void  PredictByBinaryCombos (FeatureVectorPtr     example,
                                 MLClassConstPtr   knownClass,
                                 MLClassConstPtr&  predClass,
                                 MLClassConstPtr&  predClass2,
                                 int32&               predClass1Votes,
                                 int32&               predClass2Votes,
                                 double&              probOfKnownClass,
                                 double&              predClassProb,
                                 double&              predClass2Prob,
                                 double&              breakTie,
                                 double&              compact,
                                 int32&               numOfWinners,
                                 bool&                knownClassOneOfTheWinners
                                );


    void  Read         (istream& i);
    void  ReadHeader   (istream& i);
    void  ReadOneVsOne (istream& i);
    void  ReadOneVsAll (istream& i);
    void  ReadOneVsAllEntry (istream& i,
                             int32      modelIDX
                            );
   
    void  ReadBinaryCombos (istream& i);


    void  ReadSkipToSection (istream&   i, 
                             KKStr      sectName,
                             bool&      sectionFound
                            );


    ClassAssignments       assignments;

    FeatureEncoderPtr*     binaryFeatureEncoders;

    BinaryClassParmsPtr*   binaryParameters;      /**< only used when doing Classification with diff Feature 
                                                   * Selection by 2 class combo's
                                                   */
    volatile const bool&   cancelFlag;

    vector<int32>          cardinality_table;

    MLClassConstPtr*    classIdxTable;         /**< Supports reverse class lookUp,  indexed by ClassAssignments num,
                                                   * works with assignments.
                                                   */
    CompressionStats       compression_stats;

    double**               crossClassProbTable;   /**< Probabilities  between Binary Classes From last Prediction */

    int32                  crossClassProbTableSize;   // Dimension of of each side of 'crossClassProbTable'  

    FeatureEncoderPtr      featureEncoder;        /**< used when doing OneVsOne or OnevsAll processing
                                                   * When doing binary feature selection will use 
                                                   * binaryFeatureEncoders.
                                                   */
    FileDescPtr            fileDesc;

    RunLog&                log;

    ModelPtr*              models;

    int32                  numOfClasses;          /**< Number of Classes defined in crossClassProbTable.  */
    int32                  numOfModels;

    VectorShort            oneVsAllAssignment;
    ClassAssignmentsPtr*   oneVsAllClassAssignments;

    uint32                 predictXSpaceWorstCase;

    XSpacePtr              predictXSpace;  /**< Used by Predict OneVsOne, to avoid deleting and reallocating every call. */

    double*                probabilities;

    KKStr                  rootFileName;   /**< This is the root name to be used by all componet 
                                             * objects; such as svm_model, mlClasses, and
                                             * svmParam(including seleced features).  Each one
                                             * will have the same rootName with a different Suffix
                                             *     mlClasses  "<rootName>.image_classes"
                                             *     svmParam      "<rootName>.svm_parm"
                                             *     model         "<rootName>"
                                             */

    FeatureNumList         selectedFeatures;

    SVMparam               svmParam;

    double                 trainingTime;

    MLL::AttributeTypeVector  type_table;

    bool                   validModel;

    int32*                 votes;

    XSpacePtr*             xSpaces;    /**< There will be one xSpace structure for each libSVM classifier that has 
                                        *   to be built; for a ttal of 'numOfModels'.   This will be the input to 
                                        *   the trainer for each one.
                                        */

    int32                  xSpacesTotalAllocated;
  };



  typedef  SVMModel*  SVMModelPtr;


} /* namespace MLL */


#endif




