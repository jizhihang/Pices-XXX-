#if  !defined(_NORMALIZATIONPARMS_)
#define  _NORMALIZATIONPARMS_

#include  "Attribute.h"
#include  "BasicTypes.h"
#include  "FeatureNumList.h"
#include  "Str.h"



namespace MLL 
{

  #ifndef  _RUNLOG_
  class  RunLog;
  typedef  RunLog*  RunLogPtr;
  #endif


  #ifndef  _FeatureVectorDefined_
  class  FeatureVector;
  typedef  FeatureVector*  FeatureVectorPtr;
  #endif


  #ifndef  _FeatureVectorListDefined_
  class  FeatureVectorList;
  typedef  FeatureVectorList*  FeatureVectorListPtr;
  #endif


  #ifndef  _FILEDESC_
  class  FileDesc;
  typedef  FileDesc*  FileDescPtr;
  #endif


  #ifndef  _TrainingConfiguration2Defined_
  class    TrainingConfiguration2;
  typedef  TrainingConfiguration2*  TrainingConfiguration2Ptr;
  #endif

  #ifndef  _MODELPARAM_
  class  ModelParam;
  typedef  ModelParam*  ModelParamPtr;
  #endif



  /**
   *@brief  Normalization Parameters;  calculation and implementation.
   *@details Normalization parameters will be calculated for all features but 
   *when individual images are normalized, only the ones specified by 
   *'_featuresToNormalize' will be normalized.              
   */
  class  NormalizationParms
  {
  public:
    NormalizationParms (bool                _normalizeNominalFeatures,
                        FeatureVectorList&  _examples,
                        RunLog&             _log
                       );

    NormalizationParms (const ModelParam&   _param,
                        FeatureVectorList&  _images,
                        RunLog&             _log
                       );

    NormalizationParms (TrainingConfiguration2Ptr  _config,
                        FeatureVectorList&         _images,
                        RunLog&                    _log
                       );

    NormalizationParms (FileDescPtr   _fileDesc,
                        KKStr         _fileName,
                        bool&         _successfull,
                        RunLog&       _log
                       );

    NormalizationParms (FileDescPtr   _fileDesc,
                        FILE*         _in,
                        bool&         _successfull,
                        RunLog&       _log
                       );

    NormalizationParms (FileDescPtr   _fileDesc,
                        istream&      _in,
                        bool&         _successfull,
                        RunLog&       _log
                       );

    ~NormalizationParms ();

    int32   MemoryConsumedEstimated ()  const;

    void  NormalizeImages (FeatureVectorListPtr  images);

    void  NormalizeAImage (FeatureVectorPtr  image);

    FeatureVectorPtr  ToNormalized (FeatureVectorPtr  example)  const;

    int32   NumOfFeatures ()  {return numOfFeatures;}


    void  Read (FILE*  i,
                bool&  sucessful
               );


    void  Read (istream&  i,
                bool&     sucessful
               );


    void  Save (const KKStr&  _fileName,
                bool&         _successfull
               );

    void  Write (std::ostream&  o);

    double  Mean (int32 i);
    double  Sigma (int32 i);

    const double*  Mean ()  {return  mean;}
    const double*  Sigma () {return  sigma;}


  private:
    void  ConstructNormalizeFeatureVector ();
    void  DeriveNormalizationParameters (FeatureVectorList&  _examples);


    MLL::AttributeTypeVector  attriuteTypes;
    FileDescPtr               fileDesc;
    KKStr                     fileName;
    RunLog&                   log;
    double*                   mean;
    bool*                     normalizeFeature;
    bool                      normalizeNominalFeatures;
    int32                     numOfFeatures;
    FFLOAT                    numOfExamples;
    double*                   sigma;
  };  /* NormalizationParms */


  typedef  NormalizationParms*  NormalizationParmsPtr;


  #define  _NormalizationParmsDefined_

}  /* namespace MLL { */


#endif
