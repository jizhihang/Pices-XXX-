#if  !defined(_MODELPARAMOLDSVM_)
#define  _MODELPARAMOLDSVM_

#include "RunLog.h"
#include "Str.h"

#include "ModelParam.h"
#include "svm.h"
#include "SVMparam.h"
using namespace SVM233;


namespace MLL 
{

  #ifndef  _BINARYCLASSPARMS_
  class  BinaryClassParms;
  typedef  BinaryClassParms*  BinaryClassParmsPtr;
  class  BinaryClassParmsList;
  typedef  BinaryClassParmsList*  BinaryClassParmsListPtr;
  #endif


  /**
    ************************************************************************************************
    * this class encapsulates are the information neccesary to build a SVMModel class.             *
    ************************************************************************************************
    * @see  ModelSVM
    * @see  SVM233
    * @see  BinaryClassParms
    */
  class  ModelParamOldSVM:  public  ModelParam
  {
  public:
    typedef  ModelParamOldSVM*  ModelParamOldSVMPtr;
    typedef  SVM233::svm_parameter   svm_parameter;

    ModelParamOldSVM  (FileDescPtr  _fileDesc,
                       RunLog&      _log
                      );
  
  
    ModelParamOldSVM  (const ModelParamOldSVM&  _param);
  
    virtual 
    ~ModelParamOldSVM  ();
  
    virtual
    ModelParamOldSVMPtr  Duplicate () const;

    virtual ModelParamTypes  ModelParamType () const {return mptOldSVM;}

    void    ReadSpecificImplementationXML (istream& i);


    void    WriteSpecificImplementationXML (std::ostream&  o)  const;


    void    AddBinaryClassParms (BinaryClassParmsPtr  binaryClassParms);

    void    AddBinaryClassParms (MLClassConstPtr     class1,
                                 MLClassConstPtr     class2,
                                 const svm_parameter&   _param,
                                 const FeatureNumList&  _selectedFeatures,
                                 float                  _weight
                                );

    virtual  float   AvgMumOfFeatures ();

    BinaryClassParmsPtr  GetParamtersToUseFor2ClassCombo (MLClassConstPtr  class1,
                                                          MLClassConstPtr  class2
                                                         );

    FeatureNumList  GetFeatureNums (MLClassConstPtr  class1,
                                    MLClassConstPtr  class2
                                   )  const;


    // Member access methods
    const
    BinaryClassParmsListPtr        BinaryParmsList            () const;
    virtual float                  A_Param                    () const;
    virtual int32                  BitsToReduceBy             () const;
    virtual double                 C_Param                    () const;

    virtual double                 C_Param   (MLClassConstPtr  class1,
                                              MLClassConstPtr  class2
                                             )  const;

    virtual double                 Gamma                      () const;
    virtual SVM_KernalType         KernalType                 () const;
    virtual SVM_MachineType        MachineType                () const;
    virtual int32                  NumOfFeaturesAfterEncoding () const;
    virtual const svm_parameter&   Param                      () const;
    virtual float                  SamplingRate               () const;
    virtual const FeatureNumList&  SelectedFeatures           () const;
    virtual SVM_SelectionMethod    SelectionMethod            () const;
    virtual SVMparamPtr            SvmParameters              () const;
    virtual int32                  UnBalancedBits             () const;
    virtual KKStr                  UnBalancedBitsStr          () const;
    virtual bool                   UseProbabilityToBreakTies  () const;
    virtual bool                   WeightBitReduction         () const;


    // Member update methods
    virtual void  A_Param            (float                 _A);
    virtual void  C_Param            (double                _CC);

    virtual void  C_Param            (MLClassConstPtr  class1,
                                      MLClassConstPtr  class2,
                                      double              cParam
                                     );

    virtual void  EncodingMethod     (SVM_EncodingMethod    _encodingMethod);
    virtual void  EncodingMethod     (EncodingMethodType    _encodingMethod);
    virtual void  Gamma              (double                _gamma);
    virtual void  Gamma_Param        (double                _gamma);
    virtual void  KernalType         (SVM_KernalType        _kernalType);
    virtual void  MachineType        (SVM_MachineType       _machineType);
    virtual void  SamplingRate       (float                 _samplingRate);
    virtual void  SelectedFeatures   (const FeatureNumList& _selectedFeatures);
    virtual void  SelectionMethod    (SVM_SelectionMethod   _selectionMethod);
    virtual void  WeightBitReduction (bool                  _weightBitReduction);


    void  SetBinaryClassFields (MLClassConstPtr     class1,
                                MLClassConstPtr     class2,
                                const svm_parameter&   _param,
                                const FeatureNumList&  _features,
                                float                  _weight
                               );



    void  SetFeatureNums    (MLClassConstPtr     class1,
                             MLClassConstPtr     class2,
                             const FeatureNumList&  _features,
                             float                  _weight = -1  // -1 Indicats use existing value, 
                            );

    void  ParseCmdLine (KKStr   _cmdLineStr,
                        bool&   _validFormat
                       );

    KKStr  SvmParamToString (const  svm_parameter&  _param)  const;

    KKStr  ToCmdLineStr ()  const;


  private:
    virtual
    void  ParseCmdLineParameter (const KKStr&  parameter,
                                 const KKStr&  value,
                                 bool&         parameterUsed
                                );

    SVMparamPtr  svmParameters;
  };  /* ModelParamOldSVM */

  typedef  ModelParamOldSVM*   ModelParamOldSVMPtr;
}  /* namespace MLL */



#endif


