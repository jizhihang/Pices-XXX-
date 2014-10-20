#if  !defined(_FEATUREENCODER2_)
#define  _FEATUREENCODER2_
/*!
 \class  MLL::FeatureEncoder2
 \code
 ***********************************************************************
 *                       FeatureEncoder2                               *
 *                                                                     *
 *  This will eventually replace "FeatureEncoder" when we shift over   *
 *  to the new paradigm where all Machine Learning Algorithms will be  *
 *  derived from 'Model'.                                              *
 ***********************************************************************
 \endcode
 */
#include  "RunLog.h"

#include  "Attribute.h"
#include  "BitReduction.h"
#include  "FeatureVector.h"
#include  "FileDesc.h"
#include  "MLClass.h"
#include  "ModelParam.h"



namespace MLL 
{

  class  FeatureEncoder2
  {
  public:
    typedef  FeatureEncoder2*  FeatureEncoder2Ptr;

    typedef  enum  {FeAsIs, FeBinary, FeScale}  FeWhatToDo;
    typedef  FeWhatToDo*  FeWhatToDoPtr;


    FeatureEncoder2 (const ModelParam&       _param,
                     FileDescPtr             _fileDesc,
                     RunLog&                 _log
                   );
  
    FeatureEncoder2 (const FeatureEncoder2&  _encoder);


    ~FeatureEncoder2 ();


    int32  CodedNumOfFeatures () const  {return codedNumOfFeatures;}

    CompressionStats  CompressExamples (const FeatureVectorListPtr  srcImages,
                                        FeatureVectorListPtr&       dest
                                       );

    FileDescPtr       CreateEncodedFileDesc (ostream*  o)  const;  /*!< If 'o' is not NULL will write out a table showing assignments from old to new.  */

    FeatureVectorListPtr  EncodedFeatureVectorList (const  FeatureVectorList&  srcData)  const;

    FeatureVectorListPtr  EncodeAllExamples (const FeatureVectorListPtr  srcData);

    FeatureVectorPtr  EncodeAExample (FeatureVectorPtr  src)  const;

    int32             MemoryConsumedEstimated ()  const;

    int32             NumEncodedFeatures ()  const;

    void              ReadXML (istream&  i);

    void              WriteXML (istream&  o);


  private:
    // The next several routines were coppied from the BitReduction class with minimal changs.
    CompressionStats  compress (const FeatureVectorList&  examples,
                                FeatureVectorListPtr      compressed_example_list
                               );
    void   divide (FeatureVectorPtr example, double t);
    void   add (FeatureVectorPtr a,   const FeatureVectorPtr b);
    void   SetupNominalCounters ();
    void   ZeroNominalCounters ();
    void   DestroyNominalCounters ();

    void   IncrementNominalCounters (uint32 feature_num, 
                                     int32  value
                                    );

    KKStr      BitReductionByFeature (VectorInt&  bitsToReduceByFeature);
    VectorInt  DeriveBitReductionPlain (const FeatureVectorList&  examples);
    VectorInt  DeriveBitReductionForEachFeatureFromSpecifoedParameter ();
    VectorInt  DeriveBitReductionForEachFeatureByVariance (const FeatureVectorList&  examples,  KKStr&  unBalFeatures);
    int32      NominalFeatureAverage (uint32 feature_num);


    const AttributeTypeVector&        attributeVector;
    int32*                            cardinalityDest;
    const VectorInt&                  cardinalityVector;   // Will not own, passed in by creater
    int32                             codedNumOfFeatures;
    ModelParam::CompressionMethodType compressionMethod;
    int32*                            destFeatureNums;
    FeWhatToDoPtr                     destWhatToDo;
    FileDescPtr                       encodedFileDesc;
    ModelParam::EncodingMethodType    encodingMethod;
    FileDescPtr                       fileDesc;
    RunLog&                           log;
    int32**                           nominal_counters;
    int32                             nominalCountersSize;
    int32                             numOfFeatures;
    int32*                            srcFeatureNums;
    const ModelParam&                 param;

    struct  FeatureVar2;
    typedef  FeatureVar2* FeatureVar2Ptr;
    class  FeatureVar2List;
    typedef  FeatureVar2List*  FeatureVar2ListPtr;
    class  FeatureVarComparrison2;
  };  /* FeatureEncoder2 */


  typedef  FeatureEncoder2::FeatureEncoder2Ptr  FeatureEncoder2Ptr;

} /* namespace MLL */

#endif
