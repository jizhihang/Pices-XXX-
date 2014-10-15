#if  !defined(_FEATUREVECTOR_)
#define  _FEATUREVECTOR_


/**
 *@class  MLL::FeatureVector
 *@brief  Represents a Feature Vector of a single example, labeled or unlabele
 *@author  Kurt Kramer
 *@details Used for the representation of a Single example.  You create an instance of 
 *        this object for each single feature vector. You can subclass from this Class
 *        to make a specialized FeatureVector as in the ImageFeatures class.  Besides 
 *        keeping track of feature data this class will also track other fields such as 
 *        ImageFileName which should indicate where the FeatureVector was derived from,
 *        probability, breakTie, and others.
 *code
 ********************************************************************************
 **                             FeatureVector                                     *
 **                                                                               *
 ** Used for the representation of a Single example.  You create an instance of   *
 ** this object for each single feature vector.                                   *     
 **                                                                               *
 ** This class was split from "ImageFeatures"; such that "ImageFeatures" is now   *
 ** derived from this class and contains code that is specialized for the SIPPER  *
 ** system.                                                                       *
 **                                                                               *
 **********************************************************************************
 *@endcode
 *@see FeatureVectorList
 *@see ImageFeatures
 *@see FeatureFileIO
 */


#include "Str.h"
#include "KKQueue.h"
#include "RunLog.h"

#include "Attribute.h"
#include "ClassStatistic.h"
#include "FeatureFileIO.h"
#include "FeatureNumList.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "MLClassConstList.h"


namespace MLL
{
  #ifndef  _FEATUREFILEIODEFINED_
    class  FeatureFileIO;
    typedef  FeatureFileIO*  FeatureFileIOPtr;
  #endif


  /**
   *@class FeatureVector
   *@brief  Represents a Feature Vector of a single example, labeled or unlabele
   *@details Used for the representation of a Single example.  You create an instance of 
   *         this object for each single feature vector. You can subclass from this Class
   *         to make a specialized FeatureVector as in the ImageFeatures class.  Besides 
   *         keeping track of feature data this class will also track other fields such as 
   *         ImageFileName which should indicate where the FeatureVector was derived from,
   *         probability, breakTie, and others.
   *@see FeatureVectorList
   *@see ImageFeatures
   *@see FeatureFileIO
   */
  class  FeatureVector 
  {
  public:
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    //FeatureVector (MLClassPtr  mlClass);

    FeatureVector (int32  _numOfFeatures);

    FeatureVector (const FeatureVector&  _example);

    virtual  ~FeatureVector ();

    void  BreakTie         (float            _breakTie)       {breakTie       = _breakTie;}        /**< @brief Update the BreatTie value. */
    void  MLClass          (MLClassConstPtr  _mlClass)        {mlClass        = _mlClass;}         /**< @brief Assign a class to this example. */
    void  ImageFileName    (const KKStr&     _imageFileName)  {imageFileName  = _imageFileName;}   /**< @brief Name of source of feature vectpr, ex: file name of image that the veature vector was computed from. */
    void  MissingData      (bool             _missingData)    {missingData    = _missingData;}     /**< @brief True indicates that not all the feature data was preseant when this example was loaded from a data file. */
    void  OrigSize         (FFLOAT           _origSize)       {origSize       = _origSize;}        /**< @brief The value of Feature[0] before normalization. */
    void  PredictedClass   (MLClassConstPtr  _predictedClass) {predictedClass = _predictedClass;}
    void  Probability      (FFLOAT           _probability)    {probability    = _probability;}     /**< @brief Assign a prediction probability to this example.  */

    /**
     *@brief Assign a value to a specific feature number for the feature vector. 
     *@details This method will validate that '_featureNum' is not out of range (0 - 'numOfFeatures').
     * This will prevent the caller from corrupting memory.
     *@param[in] _featureNum Feature Num to assign '_featureValue' to. 
     *@param[in] _featureValue Value to assign to feature '_featureNum'.
     */
    void  FeatureData (int32    _featureNum,
                       FFLOAT _featureValue
                      );

    /**
     *@brief  Returns the total of all Feature Attributes for this feature vector.
     */
    float  TotalOfFeatureData ()  const;

    virtual
    int32   MemoryConsumedEstimated ()  const;

    /**  
     *@brief  Assign a specific example a higher weight for training purposes. 
     *@details The SVM will multiply the cost parameter by this amount when training the classifier 
     * for this specific example.  This was introduced when we implemented BitReduction. 
     */
    void  TrainWeight      (float  _trainWeight)   {trainWeight  = _trainWeight;}  


    /** @brief  Indicated wheather an expert has validated the class assignment. */
    void  Validated        (bool   _validated)     {validated    = _validated;}


    float               BreakTie           () const  {return breakTie;}        /**< @brief The difference in probability between the two most likely classes. */
    const KKStr&        ClassName          () const;                           /**< @brief Name of class that this example is assigne to.                     */
    MLClassConstPtr     MLClass            () const  {return mlClass;}         /**< @brief Class that is example is assigned to.                              */
    const KKStr&        MLClassName        () const;                           /**< @brief Name of class that this example is assigned to.                    */
    const KKStr&        ImageFileName      () const  {return imageFileName;}   /**< @brief Name of file that this FeatureVector was computed from.            */
    bool                MissingData        () const  {return missingData;}     /**< @brief True indicates that one or more features were missing.             */        
    int32               NumOfFeatures      () const  {return numOfFeatures;}   /**< @brief Number of features in this FeatureVector.                          */
    FFLOAT              OrigSize           () const  {return origSize;}        /**< @brief The value of Feature[0] before normalization.                      */
    MLClassConstPtr     PredictedClass     () const  {return predictedClass;}
    const KKStr&        PredictedClassName () const;
    FFLOAT              Probability        () const  {return probability;}     /**< @brief The probability assigned by classifer to the predicted class.      */
    float               TrainWeight        () const  {return trainWeight;}
    bool                Validated          () const  {return validated;}


    FFLOAT         FeatureData      (int32 featureNum)  const;          /** @returns The value of 'featureNum'                             */
    const FFLOAT*  FeatureData      () const  {return featureData;}   /** @brief Returns as a pointer to the feature data itself.        */
    FFLOAT*        FeatureDataAlter ()        {return featureData;}   /** @brief ame as 'FeatureData() except you can modify the data.   */
                                                                      
    const FFLOAT*  FeatureDataConst () const  {return featureData;}
    bool           FeatureDataValid ();

    void    ResetNumOfFeatures (int32  newNumOfFeatures);  /*!< Used to reallocate memory for feature data. */

    void    AddFeatureData (int32   _featureNum,   /**< Indicates which feature number to update.  */
                            FFLOAT  _featureData   /**< New value to assign to '_featureNum'.      */
                           );

    bool  operator== (FeatureVector &other_example)  const;


    /** @brief Used by container classes such as 'FeatureVectorList'.  This way they can determine real underlyimg class. */
    virtual  const char*  UnderlyingClass ()  const  {return  "FeatureVector";}


  protected:
    void  AllocateFeatureDataArray ();

    FFLOAT*        featureData;
    int32          numOfFeatures;


  private:
    float            breakTie;       /**< @brief The diference in probability between the two most liklist 
                                      * classes as per the classifier. 
                                      */
    MLClassConstPtr  mlClass;
    KKStr            imageFileName;
    bool             missingData;    /**< @brief Indicates that some features were flagged as missing in 
                                      * data file. 
                                      */
    FFLOAT           origSize;
    MLClassConstPtr  predictedClass; /**< @brief Represents the class that the Classifier assigned to this 
                                      * image; added to aid in the grading function.2 
                                      */

    FFLOAT           probability;    /**< @brief Probability assigned by classifier to predicted Class. */

    float            trainWeight;    /**< @brief Weight to assign to this training image during Training.
                                      *@details  Added to help support Bit Reduction.  Will default to 1.0.
                                      * during the SVM training process the Cost parameter will be multiplied 
                                      * by this amount.  
                                      */

    bool             validated;      /**< @brief  If true then the 'mlClass' entry has been validated by 
                                      * an expert; was introduced when the DataBase was implemeneted.
                                      */
  };  /* FeatureVector */


  typedef  FeatureVector* FeatureVectorPtr;

  #define  _FeatureVectorDefined_


  class  FeatureVectorComparison;



  /**
   *@class FeatureVectorList
   *@brief   Container class for FeatureVector derived objects. 
   *@details Supports various functions with respect to maintaing a list of FeatureVector's.  These 
   *         include randomizing there order creating a stratified list by class, extracting a list 
   *         of classes,  sorting by various criteria.
   */
  class  FeatureVectorList:  public KKQueue<FeatureVector>
  {
  public: 
    typedef  FeatureVectorList*  FeatureVectorListPtr;

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    /**
     *@brief Will create a new empty list of FeatureVector's.
     *@param[in] _fileDesc Describes the feature data such as number of features and their attributes.
     *@param[in] _owner True indicates that this list willown its contents and when this list is deleted it 
     *           will call the destrctor for all its contents.
     *@param[out] _log Log file to send messages to.
     *@param[in] _size No longer used;  used to cause the preallocation of space.
     */
    FeatureVectorList (FileDescPtr  _fileDesc,
                       bool         _owner,
                       RunLog&      _log,
                       int32        _size = 1000
                      );

  private:
    /**
     *@brief  Will create a duplicate List of examples, in the same order.
     *@details If the source 'examples' ownes its entries, then new duplicate entries will be created, and the 
     *  new 'FeatureVectorList' will own them, otherwise will only get pointers to existing instances in 'examples'.
     */
    FeatureVectorList (FeatureVectorList&  examples); 


  public:
    /** 
     *@brief  Create a duplicate list, depending on the '_owner' parameter may also duplicate the contents.
     *@details If '_owner' = true will create new instancs of contents and own them.  If 'owner' = false, will 
     *         copy over pointers to existing instances.
     *@param[in]  examples  Existing list of examples that we are going to copy.
     *@param[in]  _owner  If set to true will make a duplicate of the FeatureVectors in 'examples' and own then 
     *                    otherwise will just point to the same existing examples and not own them.
     */
    FeatureVectorList (const FeatureVectorList&  examples,
                       bool                      _owner
                      );


    /**
     *@brief  Will Create a construct list from _examples who's belong to one of the ImagesClasses in '_mlClasses'.
     *@details The subset will consist of the examples who's mlClass is one of the ones in _mlClasses.  We will not own these examples.
     *@param[in] _mlClasses  List of classes that we want to include.
     *@param[in] _examples      Source of feature Vectors to extract from.
     *@param[out] _log          Log file to send messages to.
    */ 
    FeatureVectorList (MLClassConstList&  _mlClasses,
                       FeatureVectorList&    _examples,
                       RunLog&               _log
                      );

    
    /** 
     *@enum IFL_SortOrder
     *@brief  Represents the different orders that a list of imageFeatures in an instance of FeatureVectorList object can be in.  
     */
    typedef  enum {IFL_UnSorted, 
                   IFL_ByName, 
                   IFL_ByProbability, 
                   IFL_ByBreakTie, 
                   IFL_ByRootName, 
                   IFL_ByClassName
                  } 
                   IFL_SortOrder;


    virtual  ~FeatureVectorList ();


    void                      AddSingleExample (FeatureVectorPtr  _imageFeatures);  /**< @brief Same as PushOnBack */

    void                      AddQueue (const FeatureVectorList&  examplesToAdd);   /**< @brief Add the contents of 'examplesToAdd' to the end of this list. */

    FeatureNumList            AllFeatures ();                                       /**< @brief Will return a FeatureNumList instance with all features selected. */

    void                      AppendToFile (KKStr                  _fileName,
                                            FeatureFileIOPtr       _driver,
                                            const FeatureNumList&  _selFeatures
                                           );

    /**
     *@brief Will search forthe example with the same nam as '_imageFileName'.
     *@details  If the list is  already sorted in name order will use a Binary Search otherwis a 
     *  linear search.  The method 'SortByImageFileName' will set a flag 'curSortOrder' indicating
     *  if the examples are sorted.  The idea is that if you know that will will be doing many 
     *  searches then for performance reasons you should call 'SortByImageFileName' first. The
     *  methods 'PushOnBack', 'PushOnFront', and 'AddSingleExample' will reset 'curSortOrder' to 
     *  unsorted.
     */
    FeatureVectorPtr          BinarySearchByName (const KKStr&  _imageFileName)  const;

    void                      CalcStatsForFeatureNum (int32   _featureNum,
                                                      int32&  _count,
                                                      float&  _total,
                                                      float&  _mean,
                                                      float&  _var,
                                                      float&  _stdDev
                                                     );
   
    KKStr                     ClassStatisticsStr ()  const;

    MLL::AttributeTypeVector  CreateAttributeTypeTable ()  const;

    vector<int32>             CreateCardinalityTable ()  const;


    /**
     *@brief Will create a list of FeatureVectors where the class assignment will refect the specified Hierarhy level specified by 'level'.
     *@details The hierarchy of a given class will be indicated by underscore characters in the class name.
     *@code
     *     ex:   Level 1:  gelatinous
     *           Level 2:  gelatinous_hydromedusae
     *           Level 3:  gelatinous_hydromedusae_solmundella
     *     If the 'level' parameter is set to 1 then all FeatureVectors who's class name starts with 'gelatinous' will be grouped
     *     together under the class name 'gelatinous_hydromedusae'.
     *endcode
     */
    FeatureVectorListPtr      CreateListForAGivenLevel (int32  level);

    IFL_SortOrder             CurSortOrder ()  const  {return  curSortOrder;}

    FeatureVectorListPtr      DuplicateListAndContents ()  const;

    KKStrListPtr              ExtractDuplicatesByImageFileName ();


    /**
     *@brief  Returns: a list of 'FeatureVector' objects that have duplicate root file names.
     *@details The returned list will not own these items.  All instances of the duplicate objects will be returned.
     *        Ex:  if three instances have the same ImageFileName all three will be returned.
     */
    FeatureVectorListPtr      ExtractDuplicatesByRootImageFileName ();


    FeatureVectorListPtr      ExtractImagesForAGivenClass (MLClassConstPtr  _mlClass,
                                                           int32               _maxToExtract = -1,
                                                           FFLOAT              _minSize      = -1.0f
                                                          )  const;

    VectorDouble              ExtractMeanFeatureValues ();


    /**
     *@brief  Will return a random sampling by class of our FeatureVector's; with a minumum per class of 'minClassCount'.
     *@param[in] percentage  Percentage between 0.0 and 100.0 of each class to randomly sample.
     *@param[in] minClassCount The minimum per class to keep.
     */
    FeatureVectorListPtr      ExtractRandomSampling (float  percentage,    /**<  A percentage between 0.0 and 100.0 */
                                                     int32    minClassCount
                                                    );

    /**
     *@brief Will create a list of FeatureVectors where the class assignment will refect the specified Hierarhy level specified by 'level'.
     *@details The hierarchy of a given class will be indicated by underscore characters in the class name.
     *@code
     *     ex:   Level 1:  gelatinous
     *           Level 2:  gelatinous_hydromedusae
     *           Level 3:  gelatinous_hydromedusae_solmundella
     *     If the 'level' parameter is set to 1 then all FeatureVectors whow's class name starts with 'gelatinous' will be 
     *     grouped together under the class name 'gelatinous_hydromedusae'.
     *@endcode
     *@bug  This method appears to be a suplicate of 'CreateListForAGivenLevel'; We should verify this and get rid of one of them.
     */
    FeatureVectorListPtr      ExtractExamplesForHierarchyLevel (uint32 level);


    //MLClassListPtr         ExtractListOfClasses ()  const;

    MLClassConstListPtr    ExtractMLClassConstList ()  const;

    void                      FixSipperFileScanLineAndColFields ();

    // Attribute Type information of diff fields.
    bool                      AllFieldsAreNumeric ()  const;                  /**< @brief  Returns true if all fields are numeric, no nominal fields.              */
    MLL::AttributeType        FeatureType        (int32 featureNum) const;      /**< @brief  Returns the type of attribute for specified 'featureNum'. @see FileDesc */
    KKStr                     FeatureTypeStr     (int32 featureNum) const;
    int32                     FeatureCardinality (int32 featureNum) const;      /**< @brief Returns the number of values defined for a Nommnal Field. @see FileDesc::Cardinality */
    const KKStr&              FieldName          (int32 featureNum) const;      /**< @bnrie Returns name of Attribute Field.                                        */

    int32                     FeatureCount       ()  const  {return numOfFeatures;}
    const FileDescPtr         FileDesc           ()  const  {return fileDesc;}

    ClassStatisticListPtr     GetClassStatistics ()  const;                  /**< @brief Returns the number of FeatureVectors per class @see ClassStatisticList */

    int32                     GetClassCount (MLClassConstPtr  c)  const;       /**< @brief Returns number of examples for a specific Class (MLClass).   */

    RunLog&                   Log () {return  log;}


    /** @brief  Returns a pointer to the FeatureVector which has '_imageFileName' 
     *@details If the list is currently sorted by ImageFileName  (curSortOrder == IFL_ByName)  then a Binary Search is performed
     *           otherwise a sequential search is performed.
     */
    FeatureVectorPtr          LookUpByImageFileName (const KKStr&  _imageFileName)  const;


    /** 
     *@brief   Returns a pointer to the FeatureVector who's ImageFileName rootname = _rootName *\
     *@details If the list is currently sorted by ImageFileName  (curSortOrder == IFL_ByRootName)  then a Binary Search is performed
     *         otherwise a sequential search is performed.   The parameter _rootName is assumed to be just the root name of the file.
     *         that is you used osGetRootName  to et the root part.
     */
    FeatureVectorPtr          LookUpByRootName (const KKStr&  _rootName);

    // void                  Sort (FeatureVectorComparison   comparison);

    float                     MajorityClassFraction () const; /*!< Return's the fraction that the majority class makes up in this list. */

    virtual
    int32                     MemoryConsumedEstimated ()  const;

    bool                      MissingData () const;  /*! Returns true if 1 or more entries have missing data. */

    int32                     NumOfFeatures () const  {return  numOfFeatures;}

    int32                     NumEntriesOfAGivenClass (MLClassConstPtr  mlClass) const  {return GetClassCount (mlClass);}

    /**
     *@brief  Using list of ImageFileNames in a file('fileName') create a new FeatureVectorList instance with examples in order based 
     *       off contents of file. If error occurs will return NULL.
     *@param[in]  fileName  Name of file that contains a list of ImageFileName's  with one entry per line.
     *@returns  A new list of FeatureVector instancs in the order dictated by 'fileName'.
     */
    FeatureVectorListPtr      OrderUsingNamesFromAFile (const KKStr&  fileName);

    void                      RemoveDuplicateEntries ();

    /**
     *@brief  Will save into a file the current ordering of FeatureVector instances in list.  
     *@details This file can then be used at a later time to reproduce the exact same ordereing of FeatureVector objects from a file.
     *@see OrderUsingNamesFromAFile
     *@param[in]  fileName  Name of file where ImagFileNames will be writen to.
     *@param[out] successful Wll indicate of list is successfuly written.
    */
    void                      SaveOrderingOfImages (const KKStr&  fileName,
                                                    bool&         successful
                                                   );

    void                      SynchronizeSymbolicData (FeatureVectorList& otherData);

    void                      PrintClassStatistics (std::ostream&  o)  const;

    void                      PrintClassStatisticsHTML (std::ostream&  o)  const;

    void                      PrintFeatureStatisticsByClass (std::ostream&  o)  const;

    void                      PushOnBack (FeatureVectorPtr  image);   /**< @brief Overloading the PushOnBack function in KKQueue so we can monitor the Version and Sort Order. */

    void                      PushOnFront (FeatureVectorPtr  image);  /**< @brief Overloading the PushOnFront function in KKQueue so we can monitor the Version and Sort Order. */
   
    void                      ResetNumOfFeaturs (int32 newNumOfFeatures);

    void                      ResetFileDesc (FileDescPtr  newFileDesc);  /*!< You would use this if youRecalc all the data to a newer version of the file. */

    void                      ReSyncSymbolicData (FileDescPtr  newFileDesc);

    void                      RemoveEntriesWithMissingFeatures ();  /*! Will delete entries from list that have missing data. */

    bool                      SameExceptForSymbolicData (const FeatureVectorList&  otherData)  const;

    FeatureVectorListPtr      StratifyAmoungstClasses (int32  numOfFolds);

    FeatureVectorListPtr      StratifyAmoungstClasses (MLClassConstListPtr  mlClasses,
                                                       int32                   maxImagesPerClass,
                                                       int32                   numOfFolds
                                                      );
   
    void                      SortByClass            (bool  reversedOrder = false);

    void                      SortByImageFileName    (bool  reversedOrder = false);

    void                      SortByProbability      (bool  reversedOrder = false);

    void                      SortByBreakTie         (bool  reversedOrder = false);

    void                      SortByRootName (bool  reversedOrder = false);

   
    const  KKStr&  FileName () const  {return fileName;}

    void   FileName (const KKStr& _fileName)  {fileName = _fileName;}


    virtual  const char*  UnderlyingClass ()  const  {return  "FeatureVectorList";}




    RunLog&        log;


  private:
    class  BreakTieComparison;
    class  BreakTieComparisonReversed;
    class  ClassNameComparrison;
    class  ClassNameComparrisonReversed;
    class  ImageFileNameComparison;
    class  ImageFileNameComparisonReversed;
    class  ProbabilityComparison;
    class  ProbabilityComparisonReversed;
    class  RootNameComparrison;
    class  RootNameComparrisonReversed;


    void  ValidateFileDescAndFieldNum (int32          fieldNum, 
                                       const char*  funcDesc
                                      )  const;



    /** 
     *@brief  Keeps track of the current order of FeatureVector entries in the list.  
     *@details This helps functions such as LookUpByImageFileName to work more efficiently.  If in ImageFileName order 
     *          it can then perform a binary search rather than a seq. scan.  This field is updated by the diff sort 
     *          routines, and by the methods that allow you to add an entry.
     */
    IFL_SortOrder  curSortOrder;   

    FileDescPtr    fileDesc;

    KKStr          fileName;

    int32          numOfFeatures;
  };  /* FeatureVectorList */


  #define  _FeatureVectorListDefined_


  typedef  FeatureVectorList*  FeatureVectorListPtr;

}  /* namespace MLL */





#endif

