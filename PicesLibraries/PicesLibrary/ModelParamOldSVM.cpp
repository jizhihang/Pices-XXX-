#include "FirstIncludes.h"

#include <stdio.h>

#include <fstream>
#include <string>
#include <iostream>
#include <vector>


#include  "MemoryDebug.h"
using namespace std;

#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace KKU;


#include "ModelParamOldSVM.h"
#include "BinaryClassParms.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "MLLTypes.h"
using namespace MLL;



ModelParamOldSVM::ModelParamOldSVM  (FileDescPtr  _fileDesc,
                                     RunLog&      _log
                                   ):

  ModelParam (_fileDesc, _log),
  svmParameters (NULL)
{
  if  (!fileDesc)
  {
    KKStr errMsg = "ModelParamOldSVM::ModelParamOldSVM  *** ERROR ***  (fileDesc == NULL)";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw  KKStrException (errMsg);
  }

  svmParameters = new SVMparam (_fileDesc, _log);
}



ModelParamOldSVM::ModelParamOldSVM  (const ModelParamOldSVM&  _param):
  ModelParam (_param),
  svmParameters (NULL)

{
  if  (_param.svmParameters)
    svmParameters = new SVMparam (*_param.svmParameters);
}



ModelParamOldSVM::~ModelParamOldSVM  ()
{
  delete  svmParameters;
  svmParameters = NULL;
}


ModelParamOldSVMPtr  ModelParamOldSVM::Duplicate () const
{
  return new ModelParamOldSVM (*this);
}


void  ModelParamOldSVM::A_Param (float  _A)
{
  ModelParam::A_Param (_A);
  svmParameters->A_Param (_A);
}


const
BinaryClassParmsListPtr  ModelParamOldSVM::BinaryParmsList () const
{
  return  svmParameters->BinaryParmsList ();
}


void  ModelParamOldSVM::C_Param (double  _CC)
{
  ModelParam::Cost (_CC);
  svmParameters->C_Param (_CC);
}




void  ModelParamOldSVM::C_Param (MLClassConstPtr  class1,
                                 MLClassConstPtr  class2,
                                 double              cParam
                                )
{
  if  (!this->BinaryParmsList ())
  {
    cerr << endl << endl
      << "ModelParamOldSVM::C_Param   ***ERROR***     'binaryParmsList'  is not defined." << endl
      << endl;
    return;
  }

  BinaryClassParmsPtr  binaryParms = BinaryParmsList ()->LookUp (class1, class2);
  if  (!binaryParms)
  {
    svm_parameter binaryParam = this->Param ();
    binaryParam.C = C_Param ();
    AddBinaryClassParms (class1, class2, binaryParam, SelectedFeatures (), 1.0f);
  }
  else
  {
    binaryParms->C (cParam);
  }
}  /* C_Param */




double  ModelParamOldSVM::C_Param (MLClassConstPtr  class1,
                                   MLClassConstPtr  class2
                                  )  const
{
  if  (!BinaryParmsList ())
  {
    cerr << endl << endl
      << "ModelParamOldSVM::C_Param   ***ERROR***     'binaryParmsList'  is not defined." << endl
      << endl;
    return 0.0;
  }

  BinaryClassParmsPtr  binaryParms = BinaryParmsList ()->LookUp (class1, class2);
  if  (!binaryParms)
  {
    cerr << endl << endl 
      << "ModelParamOldSVM::C_Param   ***ERROR***    No entry for Class[" << class1->Name () << ", " << class2->Name () << "]" << endl
      << endl;
    return 0.0;
  }
  else
  {
    return binaryParms->C ();
  }
}






void  ModelParamOldSVM::EncodingMethod (SVM_EncodingMethod _encodingMethod)
{
  svmParameters->EncodingMethod (_encodingMethod);
  ModelParam::EncodingMethod ((EncodingMethodType)_encodingMethod);
}



void  ModelParamOldSVM::EncodingMethod (EncodingMethodType _encodingMethod)
{
  svmParameters->EncodingMethod ((SVM_EncodingMethod)_encodingMethod);
  ModelParam::EncodingMethod (_encodingMethod);
}


void  ModelParamOldSVM::Gamma (double _gamma)
{
  ModelParam::Gamma (_gamma);
  svmParameters->Gamma (_gamma);
}



void  ModelParamOldSVM::Gamma_Param (double _gamma)
{
  ModelParam::Gamma (_gamma);
  svmParameters->Gamma (_gamma);
}


void  ModelParamOldSVM::KernalType (SVM_KernalType   _kernalType)
{
  svmParameters->KernalType (_kernalType);
}




int32   ModelParamOldSVM::NumOfFeaturesAfterEncoding () const
{
  return  svmParameters->NumOfFeaturesAfterEncoding ();
}



void  ModelParamOldSVM::MachineType (SVM_MachineType  _machineType)
{
  svmParameters->MachineType (_machineType);
}



void  ModelParamOldSVM::SamplingRate (float _samplingRate)
{
  svmParameters->SamplingRate (_samplingRate);
}


void  ModelParamOldSVM::SelectedFeatures (const FeatureNumList&  _selectedFeatures)
{
  svmParameters->SelectedFeatures (_selectedFeatures);
  ModelParam::SelectedFeatures (_selectedFeatures);
}


const FeatureNumList&   ModelParamOldSVM::SelectedFeatures () const
{
  return svmParameters->SelectedFeatures ();
}


void  ModelParamOldSVM::SelectionMethod (SVM_SelectionMethod  _selectionMethod)
{
  svmParameters->SelectionMethod (_selectionMethod);
}



void  ModelParamOldSVM::WeightBitReduction (bool _weightBitReduction)
{
  svmParameters->WeightBitReduction (_weightBitReduction);
  ModelParam::WeightBitReduction (_weightBitReduction);
}  /* WeightBitReduction */




SVMparamPtr  ModelParamOldSVM::SvmParameters ()  const
{
  return  svmParameters;
}



float  ModelParamOldSVM::A_Param ()  const
{
  return svmParameters->A_Param ();
}



int32  ModelParamOldSVM::BitsToReduceBy () const
{
  return svmParameters->BitsToReduceBy ();
}



double  ModelParamOldSVM::C_Param ()  const
{
  return svmParameters->C_Param ();
}




double  ModelParamOldSVM::Gamma () const 
{
  return svmParameters->Gamma ();
}



SVM_KernalType  ModelParamOldSVM::KernalType () const 
{
  return svmParameters->KernalType ();
}



SVM_MachineType  ModelParamOldSVM::MachineType () const 
{
  return svmParameters->MachineType ();
}


const svm_parameter&  ModelParamOldSVM::Param () const 
{
  return svmParameters->Param ();
}


float  ModelParamOldSVM::SamplingRate () const 
{
  return svmParameters->SamplingRate ();
}


SVM_SelectionMethod ModelParamOldSVM::SelectionMethod () const 
{
  return svmParameters->SelectionMethod ();
}


int32   ModelParamOldSVM::UnBalancedBits ()  const
{
  return svmParameters->UnBalancedBits ();
}



KKStr  ModelParamOldSVM::UnBalancedBitsStr () const
{
  return svmParameters->UnBalancedBitsStr ();
}



bool  ModelParamOldSVM::UseProbabilityToBreakTies  () const 
{
  return svmParameters->UseProbabilityToBreakTies ();
}


bool  ModelParamOldSVM::WeightBitReduction () const
{
  return svmParameters->WeightBitReduction ();
}



void  ModelParamOldSVM::ParseCmdLine (KKStr   _cmdLineStr,
                                      bool&   _validFormat
                                     )
{
  _validFormat = true;

  //DecodeParamStr (_cmdLineStr, param);

  KKStr  field (_cmdLineStr.ExtractToken (" \t\n\r"));
  KKStr  value;


  while  (!field.Empty ()  &&  _validFormat)
  {
    if  (field[0] != '-')  
    {
      log.Level (-1) << "ModelParam::ParseCmdLine  *** Invalid Parameter["
        << field << "] ***"
        << endl;
      _validFormat = false;
      break;
    }

    // See if next field is a Switch field or a parameter.
    _cmdLineStr.TrimLeft (" \t\n\r");
    value = "";
    if  (_cmdLineStr.Len () > 0)
    {
      if  (_cmdLineStr[0] != '-')
        value = _cmdLineStr.ExtractToken (" \t\n\r");
    }

    field.Upper ();
    KKStr valueUpper (value);

    valueUpper.Upper ();

    bool  parameterUsed = false;

    ParseCmdLineParameter (field, value, parameterUsed);
    if  (!parameterUsed)
    {
      log.Level (-1) << "ModelParam::ParseCmdLine - Invalid Parameter["  
        << field << "]  Value[" << value << "]."
        << endl;
      _validFormat = false;
      break;
    }

    field = _cmdLineStr.ExtractToken (" \t\n\r");
  }


  // Since this class is a special case that handles the old SvmParam paradign we need
  // to update the local Model:: parameters with those that were updated in 'SvmParam'.
  ModelParam::BitsToReduceBy     (svmParameters->BitsToReduceBy     ());
  ModelParam::CompressionMethod  ((ModelParam::CompressionMethodType)svmParameters->CompressionMethod  ());
  ModelParam::EncodingMethod     ((ModelParam::EncodingMethodType)svmParameters->EncodingMethod     ());
  ModelParam::UnBalancedBits     (svmParameters->UnBalancedBits     ());
  ModelParam::UnBalancedBitsStr  (svmParameters->UnBalancedBitsStr  ());
  ModelParam::WeightBitReduction (svmParameters->WeightBitReduction ());
  
  ModelParam::A_Param  (svmParameters->A_Param ());
  ModelParam::C_Param  (svmParameters->C_Param ());
  ModelParam::Gamma    (svmParameters->Gamma   ());

  ValidParam (_validFormat);
}  /* ParseCmdLine */




void  ModelParamOldSVM::ParseCmdLineParameter (const KKStr&  parameter,
                                               const KKStr&  value,
                                               bool&         parameterUsed
                                              )
{
  bool  validFormat = true;
  svmParameters->ParseCmdLineParameter (parameter, value, parameterUsed, validFormat);
  if  (!validFormat)
  {
    log.Level (-1) << endl << endl
      << "ModelParamOldSVM::ParseCmdLineParameter  ***ERROR***  Invalid Parameters" << endl
      << "      Parameter[" << parameter << "]     Value[" << value << "]." << endl
      << endl;
    ValidParam (false);
  }
}  /* ParseCmdLineParameter */




/**
 * @brief Convert a svm_parameter struct to a cmdline str.
*/
KKStr   ModelParamOldSVM::SvmParamToString (const svm_parameter&  _param)  const
{
  log.Level (60) << "ModelParamOldSVM::SvmParamToString Entered." << endl;

  KKStr  cmdStr (300);

  cmdStr << _param.ToCmdLineStr ();

  return  cmdStr;
}  /* SvmParamToString */




/**
 * @brief Convert all parameters to a command line string.
*/
KKStr   ModelParamOldSVM::ToCmdLineStr () const
{
  log.Level (60) << "ModelParamOldSVM::ToCmdLineStr - Entered." << endl;

  KKStr  cmdStr (300);

  cmdStr = svmParameters->ToString ();
  return  cmdStr;
}  /* ToString */




void  ModelParamOldSVM::WriteSpecificImplementationXML (ostream&  o)  const
{
  log.Level (20) << "ModelParamOldSVM::WriteSpecificImplementationXML to XML to ostream." << endl;
  o << "<ModelParamOldSVM>" << std::endl;
  svmParameters->WriteXML (o);
  o << "</ModelParamOldSVM>" << endl;
}  /* WriteSpecificImplementationXML */





void  ModelParamOldSVM::ReadSpecificImplementationXML (istream& i)
{
  log.Level (20) << "ModelParamOldSVM::ReadSpecificImplementationXML file." << endl;

  char  buff[20480];
  
  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);
    KKStr  field = ln.ExtractQuotedStr ("\n\r\t", true);      // true = decode escape charaters
    field.Upper ();

    if  (field.EqualIgnoreCase ("</ModelParamOldSVM>"))
      break;

    if  (field.EqualIgnoreCase ("<SVMparam>"))
    {
      svmParameters->ReadXML (i);
    }
  }
}  /* ReadSpecificImplementationXML */






BinaryClassParmsPtr   ModelParamOldSVM::GetParamtersToUseFor2ClassCombo (MLClassConstPtr  class1,
                                                                         MLClassConstPtr  class2
                                                                        )
{
  return svmParameters->GetParamtersToUseFor2ClassCombo (class1, class2);
}  /* GetParamtersToUSeFor2ClassCombo */



FeatureNumList  ModelParamOldSVM::GetFeatureNums (MLClassConstPtr  class1,
                                                  MLClassConstPtr  class2
                                                 )  const
{
  return svmParameters->GetFeatureNums (class1, class2);
}  /* GetFeatureNums */




void    ModelParamOldSVM::AddBinaryClassParms (BinaryClassParmsPtr  binaryClassParms)
{
  svmParameters->AddBinaryClassParms (binaryClassParms);
}  /* AddBinaryClassParms */




float   ModelParamOldSVM::AvgMumOfFeatures ()
{
  return  svmParameters->AvgMumOfFeatures ();
}  /* AvgMumOfFeatures */





void  ModelParamOldSVM::SetFeatureNums (MLClassConstPtr     class1,
                                        MLClassConstPtr     class2,
                                        const FeatureNumList&  _features,
                                        float                  _weight
                                       )
{
  svmParameters->SetFeatureNums (class1, class2, _features, _weight);
}  /* SetFeatureNums */





void  ModelParamOldSVM::SetBinaryClassFields (MLClassConstPtr     class1,
                                              MLClassConstPtr     class2,
                                              const svm_parameter&   _param,
                                              const FeatureNumList&  _features,
                                              float                  _weight
                                             )
{
  svmParameters->SetBinaryClassFields (class1, class2, _param, _features, _weight);
}  /* SetBinaryClassFields */





/**
 * @brief  Add a Binary parameters using svm_parametr cmd line str.
 *         Typically used by TrainingConfiguration.
*/
void  ModelParamOldSVM::AddBinaryClassParms (MLClassConstPtr    class1,
                                             MLClassConstPtr    class2,
                                             const svm_parameter&  _param,
                                             const FeatureNumList& _selectedFeatures,
                                             float                 _weight
                                            )
{
  svmParameters->AddBinaryClassParms (class1, class2, _param, _selectedFeatures, _weight);
}  /* AddBinaryClassParms */


