#ifndef  _MODELPARAM_
#define  _MODELPARAM_


#include "RunLog.h"
#include "Str.h"

#include "FeatureNumList.h"

namespace MLL 
{
  #ifndef _FileDescDefined_
  class  FileDesc;
  typedef  FileDesc*  FileDescPtr;
  #endif


  // All the classes subclassed from 'ModelParam'  will need this.
  #if  !defined(_MLCLASS_)
  class  MLClass;
  typedef  MLClass*  MLClassPtr;
  class  MLClassList;
  typedef  MLClassList*  MLClassListPtr;
  #endif


  /**
  *@brief  Abstract Base class for Machine Learning parameters.
  *@author Kurt Kramer
  *@details
  *For each Machine Learning algorithm implemented you would create a specialization of this class
  *to manage the parameters required by the algorithm.  Specifically for each new class that you
  *create that is derived from 'Model' you will new to create a class derived from 'ModelParam'.
  *This class encapsulates general parameters that are common to all Machine Learning Models.   *
  *@see  Model
  */
  class  ModelParam
  {
  public:
    typedef  ModelParam*  ModelParamPtr;

    typedef  enum  {mptNULL, mptDual, mptKNN, mptOldSVM, mptSvmBase, mptUsfCasCor}  ModelParamTypes;
    static KKStr            ModelParamTypeToStr   (ModelParamTypes _modelParamType);
    static ModelParamTypes  ModelParamTypeFromStr (const KKStr&    _modelParamTypeStr);
  
    typedef  enum  {NoEncoding, BinaryEncoding, ScaledEncoding, Encoding_NULL}  EncodingMethodType;
  
    typedef  enum  {BRNull,
                    BRnoCompression, 
                    BRcompressionPost, 
                    BRcompressionPre, 
                    BRunBalancedVariance,
                    BRunBalancedSpecified    // Meaning we will tell you wich features are unbalanced.
                   }       
                   CompressionMethodType;
  
 
    ModelParam  (FileDescPtr  _fileDesc,
                 RunLog&      _log
                );
  
  
    ModelParam  (const ModelParam&  _param);
  
    virtual
    ~ModelParam  ();

    static 
    ModelParamPtr  CreateModelParam (istream&     i,
                                     FileDescPtr  _fileDesc,
                                     RunLog&      _log
                                    );

    virtual
    ModelParamPtr  Duplicate () const = 0;


    virtual
    int32     MemoryConsumedEstimated ()  const;


    virtual
    void    ParseCmdLine (KKStr  _cmdLineStr,
                          bool&   _validFormat
                         );


    virtual
    void   ParseCmdLinePost ();


    virtual
    void    ReadXML (KKStr&  _fileName,
                     bool&   _successful
                    );
  
    virtual
    void    ReadXML (istream&  i);
  
    virtual
    void    ReadSpecificImplementationXML (istream&  i) = 0;
  

    /*!
     @brief Creates a a Command Line String that represents these parameters.
     @details  All derived classes should implement this method. They should first call this method and
               then append there own parameters that are specific to their implementation.
     */
    virtual
    KKStr   ToCmdLineStr ()  const;
  

    virtual
    void    WriteXML (std::ostream&  o)  const;
  
    virtual
    void    WriteSpecificImplementationXML (std::ostream&  o)  const = 0;


    // Member access methods

    virtual ModelParamTypes          ModelParamType             () const = 0;
    virtual KKStr                    ModelParamTypeStr          () const {return ModelParamTypeToStr (ModelParamType ());}

    virtual float                    AvgMumOfFeatures           () const {return (float)selectedFeatures.NumOfFeatures ();}
    virtual int32                    BitsToReduceBy             () const {return bitsToReduceBy;}
    virtual CompressionMethodType    CompressionMethod          () const {return compressionMethod;}
    virtual KKStr                    CompressionMethodStr       () const {return CompressionMethodToStr (compressionMethod);}
    virtual EncodingMethodType       EncodingMethod             () const {return encodingMethod;}
    virtual KKStr                    EncodingMethodStr          () const {return EncodingMethodToStr (encodingMethod);}
    virtual int32                    ExamplesPerClass           () const {return examplesPerClass;}
    virtual const KKStr&             FileName                   () const {return fileName;}
    virtual bool                     NormalizeNominalFeatures   () const {return normalizeNominalFeatures;}
    virtual int32                    NumOfFeaturesAfterEncoding () const;
    virtual const FeatureNumList&    SelectedFeatures           () const {return selectedFeatures;}
    virtual int32                    UnBalancedBits             () const {return unBalancedBits;}
    virtual KKStr                    UnBalancedBitsStr          () const {return unBalancedBitsStr;}
    virtual bool                     ValidParam                 () const {return validParam;}
    virtual bool                     WeightBitReduction         () const {return weightBitReduction;}
  
    // Access members that were originally put here for 'ModelSVMBase'  and  'ModelOldSVM'
    virtual float   A_Param  () const;
    virtual double  C_Param  () const;  // Same as 'Cost'
    virtual double  Cost     () const;
    virtual double  Gamma    () const;
    virtual float   Prob     () const;


    // Member update methods
    virtual void  BitsToReduceBy     (int32                  _bitsToReduceBy)     {bitsToReduceBy     = _bitsToReduceBy;}
    virtual void  CompressionMethod  (CompressionMethodType  _compMethod)         {compressionMethod  = _compMethod;}
    virtual void  EncodingMethod     (EncodingMethodType     _encodingMethod)     {encodingMethod     = _encodingMethod;}
    virtual void  ExamplesPerClass   (int32                  _examplesPerClass)   {examplesPerClass   = _examplesPerClass;}
    virtual void  FileName           (const KKStr&           _fileName)           {fileName           = _fileName;}
    virtual void  SelectedFeatures   (const FeatureNumList&  _selectedFeatures)   {selectedFeatures   = _selectedFeatures;}
    virtual void  UnBalancedBits     (int32                  _unBalancedBits)     {unBalancedBits     = _unBalancedBits;}
    virtual void  UnBalancedBitsStr  (const KKStr&           _unBalancedBitsStr)  {unBalancedBitsStr  = _unBalancedBitsStr;}
    virtual void  ValidParam         (bool                   _validParam)         {validParam         = _validParam;}
    virtual void  WeightBitReduction (bool                   _weightBitReduction) {weightBitReduction = _weightBitReduction;}
  


    virtual void  A_Param  (float   _prob);
    virtual void  C_Param  (double  _cost);   // Same as 'Cost'
    virtual void  Cost     (double  _cost);
    virtual void  Gamma    (double  _gamma);
    virtual void  Prob     (float   _prob);

    static  KKStr   CompressionMethodToStr (CompressionMethodType  compressionMethod);
    static  KKStr   EncodingMethodToStr    (EncodingMethodType     encodingMethod);


    static  CompressionMethodType  CompressionMethodFromStr (const KKStr&  compressionMethodStr);
    static  EncodingMethodType     EncodingMethodFromStr    (const KKStr&  encodingMethodStr);


  protected:
    FileDescPtr    fileDesc;

    RunLog&        log;
  

  private:
 
    virtual
    void  ParseCmdLineParameter (const KKStr&  parameter,
                                 const KKStr&  value,
                                 bool&         parameterUsed
                                ) = 0;


    int32                    bitsToReduceBy;

    CompressionMethodType    compressionMethod;

    EncodingMethodType       encodingMethod;

    int32                    examplesPerClass;

    KKStr                    fileName;

    bool                     normalizeNominalFeatures;

    FeatureNumList           selectedFeatures;    /**< Feature Number to use. */

    int32                    unBalancedBits;      /**< Works with BitReduction compression when UnBalnced Bit reduction
                                                   * is specified, ex: when (compressionMethod == BRunBalancedVariance)
                                                   */


    KKStr                    unBalancedBitsStr;   /**< Works with 'BRunBalancedSpecified'  Comma delimited String that
                                                   * Specifies which features to reduce one extra bit.
                                                   */

    bool                     validParam;

    bool                     weightBitReduction;  // if false will not perform weighting
                                                  // of examples from BitReduction aggregate.


    // Following parameters are placed hear to support SVM based algorithms.
    // to help transition from old(CRAPPY) design to newer less crapy design.
    double  cost;
    double  gamma;
    float   prob;


  };  /* ModelParam */

  typedef  ModelParam::ModelParamPtr   ModelParamPtr;

}  /* namespace MLL */



#endif
