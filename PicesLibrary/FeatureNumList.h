#if  !defined(_FEATURENUMLIST_)
#define  _FEATURENUMLIST_

//************************************************************************
//*                           FeatureNumList                             *
//************************************************************************

/**
 *@class MLL::FeatureNumList
 *@brief Keeps track of selected features.  
 *@details Used by SVMModel and the newer 'Model', 'ModelParam' based classes.
 * Each instance of this class will have an associated 'FileDesc' instance. It
 * is meant to keep track of selected features from that instance('FileDesc').
 * <br />
 * This class is meant to work with both the FeaureVector, Model, and ModelParm
 * based classes.  This will allow us to select specific features
 * from the FeatureVector instances that are to be used.
 * <br />
 * At no time are feature numbers that are out of range of the associated 
 * FileDesc instance to be selected.
 * <br />
 * Two of the constructors allow you to specify a list of features in a string.
 * The list can consist of single features and/or ranges of features. Ranges of
 * features are specified by using the dash('-') character between two numbers.
 * The comma(',') character will be used as a separator.  "All" specifies all are
 * to be selected except those that are flagged as  'IgnoreAttribute' in the 
 * associated FileDesc instance. The list should be in ascending order.
 *
 *@code
 *  Example Strings:
 *    "1,2,3,10,20"    Selects [1,2,3,10, 20].
 *    "1,4-7,9-12,13"  Selects [1,4,,5,6,7,9,10,11,12,13]
 *    "All"            Selects all features that '_fileDesc' includes accept
 *                     those that are flagged as 'IgnoreAttribute' in the 
 *                     associated FileDesc instance.
 *@endcode
 */

#include  "Attribute.h"
#include  "BasicTypes.h"
#include  "BitString.h"
#include  "RunLog.h"
#include  "Str.h"


namespace MLL 
{
  #ifndef  _FILEDESC_
  class  FileDesc;
  typedef  FileDesc*  FileDescPtr;
  #endif


  class  FeatureNumList;
  typedef  FeatureNumList*  FeatureNumListPtr;

  class  FeatureNumList
  {
  public:
    typedef  FeatureNumList*  FeatureNumListPtr;

    typedef  KKU::uint16  uint16;
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    typedef enum {IncludeFeatureNums, ExcludeFeatureNums}  FeatureSelectionType;

    /*
     *@brief  Constructs an instance with no features selected and no associated 'FileDesc' instance.
     *        This is a private constructor and is used for internal use of 'FeatureNumList only.
     */
    FeatureNumList (FileDescPtr  _fileDesc);


    /**  
     *@brief  Constructs a new 'FeatureNumList' instance using 'bitString' to select 
     *        which features will be selected.
     *@details For each bit position in 'bitString' that is set to '1' the corresponding
     *         feature will be selected.  So the bit string '0110111' which consists of
     *         bits 0, 1, 2, 5, and 6  will cause the features selected to be set to (0, 
     *         1, 2, 4, 6).<br />.  This is a useful constructor when dealing with datasets
     *         that have a large number of features such as DNA based datasets.
     *@param[in]  _fileDesc Description of the feature data.
     *@param[in]  bitString A bit string that indicates which features are selected.  Bits that are set to 1 indicate that
     *            the corresponding feature is selected.
     */
    FeatureNumList (FileDescPtr       _fileDesc,
                    const BitString&  bitString
                   );


    /**
     *@brief  Constructs a instance of 'FeatureNumList' where '_featureListStr' will consist of a list
     *        of features that are to be selected.  
     *@details The list can consist of single features and/or ranges of features. Ranges of features 
     *         are specified by using the dash('-') character between two numbers.  The comma(',')
     *         character will be used as a separator.  "All" specifies all ate to be selected except
     *         those that are flagged as  'IgnoreAttribute' in the associated FileDesc instance.
     *         The list should be in ascending order.
     *@code
     *  ex's:  
     *    "1,2,3,10,20"    Selects [1,2,3,10, 20].
     *    "1,4-7,9-12,13"  Selects [1,4,,5,6,7,9,10,11,12,13]
     *    "All"            Selects all features that '_fileDesc' includes accept those that are
     *                     flagged as 'IgnoreAttribute' in the associated FileDesc instance.
     *@endcode
     *@see ExtractFeatureNumsFromStr
     *@param[in]  _fileDesc        FileDesc instance that the featureNumList is going to be based off of.
     *@param[in]  _featureListStr  String consisting of a list of features in ascending order that are to be selected.
     *@param[out] _valid           Indicates if the format of '_featureListStr' was valid.
     */
    FeatureNumList (FileDescPtr   _fileDesc,
                    const KKStr&  _featureListStr,
                    bool&         _valid
                   );


    /**
     *@brief  Constructs a instance of 'FeatureNumList' where '_featureListStr' provides a list of
     *        of features numbers that are to be included or excluded depending on '_selectionType'.
     *
     *@details The list can consist of single features and/or ranges of features. Ranges of features 
     *         are specified by using the ('-') character between two numbers.  The comma(',')
     *         character will be used as a separator.  The list should be in ascending order.
     *         The '_selectionType' parameter specifies weather we are going to select these
     *         features or exclude them form the list of all features(complement).
     *@code
     *  ex's:  
     *    "1,2,3,10,20"    Selects [1,2,3,10, 20].
     *    "1,4-7,9-12,13"  Selects [1,4,,5,6,7,9,10,11,12,13]
     *    "All"            Selects all features that '_fileDesc' includes accept those that are
     *                     flagged as 'IgnoreAttribute' in the associated FileDesc instance.
     *@endcode
     *@param[in]  _fileDesc        FileDesc instance that the featureNumList is going to be based off of.
     *@param[in]  _selectionType   IncludeFeatureNums  or  ExcludeFeatureNums.
     *@param[in]  _featureListStr  String consisting of a list of features in ascending order that are to be selected.
     *@param[out] _valid           Indicates if the format of '_featureListStr' was valid.
     */
    FeatureNumList (FileDescPtr           _fileDesc,
                    FeatureSelectionType  _selectionType,
                    const KKStr&          _featureListStr,
                    bool&                 _valid
                   );

    /** @brief  Copy constructor.  */
    FeatureNumList (const FeatureNumList&  featureNumList);

    ~FeatureNumList ();   

    /**@brief Adds 'featureNum' to the list of selected features; if it is  already selected nothing happens. */
    void    AddFeature (uint16  featureNum);

    /** @brief Returns true if all features are selected. */
    bool    AllFeaturesSelected ()  const;

    MLL::AttributeType  FeatureAttributeType (int32 idx)  const;

    static  
    FeatureNumList   AllFeatures (FileDescPtr  fileDesc);        /**< @brief Create a FeatureNumList with all features selected, except ones flagged as IgnoreAttribute in 'fileDesc'. */

    int32  Compare (const FeatureNumList&  _features)  const;      /**< @brief Compare with another featureNumList returning -1, 0, and 1 indicating less-than, equal, or greater-than.  */

    FeatureNumList  Complement ()  const;                        /**< @brief Perform a complement of selected features. */

    /**
     *@brief Allocates a array of int32's that is a copy  of FeatureNums.  The caller will own the array and is responsible for deleting it.
     *@returns A dynamically allocated array that will consist of a list of selected features.
     */
    uint16*         CreateFeatureNumArray ()  const;

    /**
     *@brief   Will select the features specified in "featureListStr".
     *@details The format is a comma delimited string, where each number represents a feature, 
     *         ranges can be specified with a dash("-").  "All" will select all features that 
     *         are not flagged as a 'IgnoreAttribute' in the associated FileDesc instance.
     *@code
     *  ex's:   String          Selected Features
     *         "1,2,3,10,20"    [1,2,3,10, 20].
     *         "1,4-7,9-12,23"  [1,4,5,6,7,9,10,11,12,23]
     *         "All"            Selects all features that '_fileDesc' includes accept those that are
     *                          flagged as 'IgnoreAttribute' in the associated FileDesc instance.
     *@endcode
     */
    void            ExtractFeatureNumsFromStr (KKStr  featureListStr,
                                               bool&   valid
                                              );

    const uint16*   FeatureNums () const {return  featureNums;}

    FileDescPtr     FileDesc () const  {return fileDesc;} 
    
    bool            IsSubSet (const FeatureNumList&  z);  /**< @brief  Returns true if 'z' is a subset of this instance. */

    bool            InList (uint16 featureNum)  const;    /**< @brief Indicates whether '_featureNum' is selected. */

    void            Load (const KKStr&  _fileName,
                          bool&         _successful,
                          RunLog&       _log
                         );

    int32             MemoryConsumedEstimated ()  const;

    int32             NumOfFeatures  () const   {return  numOfFeatures;}

    int32             NumSelFeatures () const   {return  numOfFeatures;}

    /**
     *@brief Generates a new FeatureNumList object that will select at random 'numToKeep' features from this instance.
     *@param[in] numToKeep Number of features to select randomly from existing instance.
     *@return Dynamically allocated instance of a ImageFeaturesList with randomly selected features.
     */
    FeatureNumListPtr  RandomlySelectFeatures (int32  numToKeep)  const;

    void        Save (const KKStr&  _fileName,
                      bool&         _successful,
                      RunLog&       _log
                     );
   
    void        Save (std::ostream&  o);

    void        SetAllFeatures ();                 /**< @brief  Selects all features except those flagged as 'IgnoreAttribute' in the associated FileDesc. */

    bool        Test (uint16 _featureNum)  const;  /**< @brief Indicates whether feature '_featureNum' is selected. */

    void        ToBitString (BitString&  bitStr)  const;

    KKStr       ToHexString ()  const;

    KKStr       ToString ()  const;                /**< @brief Returns comma delimited list of all features selected; will make use of range specification. */

    KKStr       ToCommaDelStr ()  const;

    void        UnSet ();                          /**< @brief Turns off all features so that no feature is selected.      */

    void        UnSet (uint16  featureNum);        /**< @brief Turns off specified feature 'featureNum'.  If 'featureNum' is not turned on then nothing happens. */

    /**
     *@brief  Allows you to access list of selected features like an array.
     *@param[in]  _idx  The position in this instance that you want to return.
     *@return  Selected feature at position '_idx'.
     */
    uint16      operator[] (int32  idx)  const;

    FeatureNumList&  operator=  (const FeatureNumList&  _features);
    FeatureNumList&  operator=  (const FeatureNumListPtr  _features);
    FeatureNumList   operator+  (const FeatureNumList&  rightSide)  const;  /**< @brief Returns new FeatureNumList that is a union of this instance and 'rightSide'.  */
    FeatureNumList   operator+  (uint16  rightSide)  const;                 /**< @brief Returns new FeatureNumList that is a union of this instance and 'rightSide'.  */
    FeatureNumList&  operator+= (const FeatureNumList&  rightSide);         /**< @brief Returns this FeatureNumList that is a union of this instance and 'rightSide'. */
    FeatureNumList&  operator+= (uint16  featureNum);                       /**< @brief Returns this FeatureNumList that is a union of this instance and 'rightSide'. */
    FeatureNumList   operator-  (const FeatureNumList&  rightSide)  const;  /**< Removes features that are selected in 'rightSide' from this instance and returns the result. */
    FeatureNumList   operator-  (uint16  rightSide)  const;                 /**< Returns this instance with the feature specified by 'rightSide'  removed.                    */
    FeatureNumList&  operator-= (uint16  rightSide);                        /**< Remove the feature specified by 'rightSide' from this instance.                              */
    FeatureNumList   operator*  (const FeatureNumList&  rightSide)  const;  /**<*@brief  Returns new instance that is the intersection of features.                      */
    bool             operator== (const FeatureNumList&  _features)  const;  /**< @brief  Indicates if the two FeatureNumLiost instances have the same features selected. */
    bool             operator>  (const FeatureNumList&  _features)  const;  /**< @brief  Indicates if the Left FeatureNumList instances is greater than the right one.   */
    bool             operator<  (const FeatureNumList&  _features)  const;  /**< @brief  Indicates if the Left FeatureNumList instances is less than the right one.      */

  private:
    FeatureNumList ();

    void   AllocateArraySize (int32 size);   /**< @brief  Make sure that FeatureNums is allocated to at least this size. */

    uint16*      featureNums;              /**< @brief The feature nums in this array are always kept in ascending order.  
                                            * @details There will be 'numOfFeatures' in this array.  'featureNumsAllocatedSize' 
                                            * indicates the size allocated, if more space is needed you need to call 
                                            * 'AllocateArraySize' to increase it.
                                            */
    int32          featureNumsAllocatedSize;
    FileDescPtr  fileDesc;
    int32          numOfFeatures;
  };  /* FeatureNumList */


  typedef  FeatureNumList*  FeatureNumListPtr;

  #define  _FeatureNumListDefined_


  std::ostream& operator<< (      std::ostream&     os, 
                            const FeatureNumList&   features
                           );


  std::ostream& operator<< (      std::ostream&       os, 
                            const FeatureNumListPtr&  features
                           );

  extern
  const  char*  FeatureDecriptions[];
} /* namespace MLL */

#endif

