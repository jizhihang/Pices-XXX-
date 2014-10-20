#include "FirstIncludes.h"

#include <stdio.h>

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "MemoryDebug.h"
using namespace std;

#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace KKU;


#include "ModelParam.h"
#include "ModelParamKnn.h"
#include "ModelParamOldSVM.h"
#include "ModelParamSvmBase.h"
#include "ModelParamUsfCasCor.h"
#include "ModelParamDual.h"

#include "MLLTypes.h"
#include "Model.h"
#include "FileDesc.h"
#include "MLClass.h"
using namespace MLL;



ModelParam::ModelParam  (FileDescPtr  _fileDesc,
                         RunLog&      _log
                        ):

  bitsToReduceBy            (0),
  compressionMethod         (BRnoCompression),
  encodingMethod            (NoEncoding),
  examplesPerClass          (int32_max),
  fileDesc                  (_fileDesc),
  fileName                  (),
  log                       (_log),
  normalizeNominalFeatures  (false),
  selectedFeatures          (_fileDesc), 
  unBalancedBits            (0),
  unBalancedBitsStr         (""),
  validParam                (true),
  weightBitReduction        (false),
  // SVM related parameters
  cost                      (0.0),
  gamma                     (0.0),
  prob                      (0.0f)
{
  if  (!fileDesc)
  {
    log.Level (-1) << endl
                   << "ModelParam::ModelParam      *** ERROR ***" << endl
                   << "                       fileDesc == NULL" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }
}





ModelParam::ModelParam  (const ModelParam&  _param):

  bitsToReduceBy             (_param.bitsToReduceBy),
  compressionMethod          (_param.compressionMethod),
  encodingMethod             (_param.encodingMethod),
  examplesPerClass           (_param.examplesPerClass),
  fileDesc                   (_param.fileDesc),
  fileName                   (_param.fileName),
  log                        (_param.log),
  normalizeNominalFeatures   (_param.normalizeNominalFeatures),
  selectedFeatures           (_param.selectedFeatures),
  unBalancedBits             (_param.unBalancedBits),
  unBalancedBitsStr          (_param.unBalancedBitsStr),
  validParam                 (_param.validParam),
  weightBitReduction         (_param.weightBitReduction),
  // SVM related parameters
  cost                       (_param.cost),
  gamma                      (_param.gamma),
  prob                       (_param.prob)
{
}



ModelParam::~ModelParam  ()
{
}


int32  ModelParam::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (ModelParam)
    +  fileName.MemoryConsumedEstimated ()
    +  selectedFeatures.MemoryConsumedEstimated ()
    +  unBalancedBitsStr.MemoryConsumedEstimated ();

  return  memoryConsumedEstimated;
}


ModelParamPtr  ModelParam::CreateModelParam (istream&     i,
                                             FileDescPtr  _fileDesc,
                                             RunLog&      _log
                                            )
{
  istream::pos_type startPos = i.tellg ();

  char  buff[20480];
  KKStr  ln;

  ModelParamTypes  modelParamType = mptNULL;


  // Fisrt we need to determine which type of model this is.  We will
  // scan through the file until we locate a ModelParamType field.
  while  (i.getline (buff, sizeof (buff)))
  {
    ln = buff;
    KKStr  lineName= ln.ExtractToken2 ("\t");

    if  (lineName.EqualIgnoreCase ("</ModelParam>"))
    {
      // We did not find the parameter type 
      break;
    }

    if  (lineName.EqualIgnoreCase ("ModelParamType"))
    {
      KKStr  modelParamTypeStr = ln.ExtractToken2 ("\t");
      modelParamType = ModelParamTypeFromStr (modelParamTypeStr);
      if  (modelParamType == mptNULL)
      {
        _log.Level (-1) << endl
          << "ModelParam::CreateModelParam  ***ERROR***   Invalid ModelParamType[" << modelParamTypeStr << "]." << endl
          << endl;
        
      }
      break;
    }
  }

  if  (modelParamType == mptNULL)
  {
    // We never found the type of parameter we are looking for.
    _log.Level (-1) << endl
      << "ModelParam::CreateModelParam  ***ERROR***   No Parameter Tye was defined." << endl
      << endl;
    return NULL;
  }

  i.seekg (startPos);

  ModelParamPtr modelParam = NULL;
  switch  (modelParamType)
  {
  case  mptDual:      modelParam = new ModelParamDual      (_fileDesc, _log);
                      break;

  case  mptKNN:       modelParam = new ModelParamKnn       (_fileDesc, _log);
                      break;

  case  mptOldSVM:    modelParam = new ModelParamOldSVM    (_fileDesc, _log);
                      break;
 
  case  mptSvmBase:   modelParam = new ModelParamSvmBase   (_fileDesc, _log);
                      break;

  case  mptUsfCasCor: modelParam = new ModelParamUsfCasCor (_fileDesc, _log);
                      break;

  }

  if  (!modelParam)
    return  NULL;

  modelParam->ReadXML (i);
  return  modelParam;
}  /* CreateModelParam */





KKStr   ModelParam::ModelParamTypeToStr (ModelParamTypes _modelParamType)
{
  if  (_modelParamType == mptNULL)
    return "NULL";
  
  else if  (_modelParamType == mptDual)
    return "ModelParamDual";

  else if  (_modelParamType == mptKNN)
    return "ModelParamKnn";

  else if  (_modelParamType == mptOldSVM)
    return "ModelParamOldSVM";
  
  else if  (_modelParamType == mptSvmBase)
    return "ModelParamSvmBase";

  else if  (_modelParamType == mptUsfCasCor)
    return  "UsfCasCor";

  else
    return "NULL";
}



ModelParam::ModelParamTypes  ModelParam::ModelParamTypeFromStr (const KKStr&  _modelParamTypeStr)
{
  if  (_modelParamTypeStr.EqualIgnoreCase ("ModelParamDual"))
    return mptDual;

  else if  (_modelParamTypeStr.EqualIgnoreCase ("ModelParamOldSVM"))
    return mptOldSVM;

  else if  (_modelParamTypeStr.EqualIgnoreCase ("ModelParamSvmBase"))
    return mptSvmBase;

  else if  (_modelParamTypeStr.EqualIgnoreCase ("ModelParamKnn"))
    return mptKNN;

  else if  (_modelParamTypeStr.EqualIgnoreCase ("UsfCasCor"))
    return mptUsfCasCor;

  else
    return mptNULL;
}




float  ModelParam::A_Param  () const
{
  return  prob;
}

double  ModelParam::C_Param  () const
{
  return cost;
}

double  ModelParam::Cost     () const
{
  return cost;
}

double  ModelParam::Gamma    () const
{
  return  gamma;
}

float  ModelParam::Prob () const
{
  return  prob;
}





void  ModelParam::A_Param  (float   _prob)
{
  prob = _prob;
}


void  ModelParam::C_Param  (double  _cost)
{
  cost = _cost;
}


void  ModelParam::Cost (double  _cost)
{
  cost = _cost;
}


void  ModelParam::Gamma (double  _gamma)
{
  gamma = _gamma;
}


void  ModelParam::Prob (float _prob)
{
  prob = _prob;
}





void  ModelParam::ParseCmdLine (KKStr   _cmdLineStr,
                                bool&   _validFormat
                               )
{
  _validFormat = true;

  //DecodeParamStr (_cmdLineStr, param);

  KKStr  field (_cmdLineStr.ExtractToken (" \t\n\r"));
  KKStr  value;

  double  valueNum;

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
    value == "";
    if  (_cmdLineStr.Len () > 0)
    {
      if  (_cmdLineStr[0] != '-')
        value = _cmdLineStr.ExtractToken (" \t\n\r");
    }

    valueNum = atof (value.Str ()); 

    field.Upper ();
    KKStr valueUpper (value);

    valueUpper.Upper ();

    if  ((field == "-FS")  ||  (field == "-FEATURESSELECTED")  ||  (field == "-FEATURESSEL")  ||  (field == "FEATURESEL"))
    {
      bool  valid = true;
      selectedFeatures.ExtractFeatureNumsFromStr (value, valid);
    }

    if  ((field == "-BITREDUCTION")  ||  (field == "-BR"))
    {
      bitsToReduceBy = atoi (value.Str ());
    }

    else if (field == "-COMPRESS")
    {
      compressionMethod = CompressionMethodFromStr (valueUpper);
    }

    else if  (field.EqualIgnoreCase ("-C")  ||  field.EqualIgnoreCase ("-Cost"))
    {
      Cost (valueNum);
    }

    else if ((field == "-ENCODE"))
    {
      encodingMethod = EncodingMethodFromStr (valueUpper);
    }

    else if  ((field.EqualIgnoreCase ("-EPC"))  ||  
              (field.EqualIgnoreCase ("-ExamplesPerClass"))
             )
    {
      examplesPerClass = value.ToInt ();
      if  (examplesPerClass < 1)
      {
        log.Level (-1) 
          << endl << endl
          << "ModelParam::ParseCmdLine ***ERROR***  Invaqlid '-ExamplsPerClass' param,eter specified[" << value << "]" << endl
          << endl;
        _validFormat = false;
        examplesPerClass = int32_max;
        break;
      }
    }

    else if  (field.EqualIgnoreCase ("-G")  ||  field.EqualIgnoreCase ("-Gamma"))
    {
      Gamma (valueNum);
    }

    else if  ((field.EqualIgnoreCase ("-NormNominal"))               ||
              (field.EqualIgnoreCase ("-NormalizeNominal"))          ||
              (field.EqualIgnoreCase ("-NormalizeNominalFeatures"))  ||
              (field.EqualIgnoreCase ("-NN"))
             )
    {
      if  (value.Empty ())
        normalizeNominalFeatures = true;
      else
        normalizeNominalFeatures = value.ExtractTokenBool ("\t");
    }

    else if  ((field == "-UNBALBITS")  ||  
              (field == "-UNBALANCEDBITS")
             )
    {
      unBalancedBits = atoi (value.Str ());
    }

    else if  ((field == "-UNBALBITSSTR")  ||  
              (field == "-UNBALANCEDBITSSTR")
             )
    {
      unBalancedBitsStr = value;
    }

    else if  ((field == "-WEIGHTBITREDUCTION")  ||  (field == "-WBR"))
    {
      weightBitReduction = true;
      if  (!valueUpper.Empty ())
      {
        weightBitReduction = valueUpper.ToBool ();
      }

      else
      {
        log.Level (-1) << endl
          << "ModelParam::ParseCmdLine    *** ERROR ***"  << endl
          << "            Invalid WeightedBitReduction[" << value << "]." << endl
          << endl;
        _validFormat = false;
        break;
      }
    }

    else
    {
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
    }

    field = _cmdLineStr.ExtractToken (" \t\n\r");
  } 

  ParseCmdLinePost ();

  validParam = _validFormat;
}  /* ParseCmdLine */



/**
 *@brief Called after 'ParseCmdLine' is completed.  Classed derived from 'ModelParam' can implement this
 *       method to do any processing that they want after the entire command line has been processed.
 *@details  An example use of this is in 'ModelParamSvmBase' where the local 'ParseCmdLineParameter'
 *          routine processes parameters that 'Model' needs to be aware of.
 */
void  ModelParam::ParseCmdLinePost ()
{
}



/**
 *@brief Convert all parameters to a command line string.
*/
KKStr   ModelParam::ToCmdLineStr () const
{
  log.Level (60) << "ModelParam::ToCmdLineStr - Entered." << endl;

  KKStr  cmdStr (300);

  cmdStr = "-FS " + selectedFeatures.ToCommaDelStr ();

  if  (examplesPerClass < int32_max)
    cmdStr << "  -EPC " << examplesPerClass;

  if  (encodingMethod != NoEncoding)
    cmdStr << "  -Encode " + EncodingMethodToStr (encodingMethod);

  if  (compressionMethod != BRnoCompression)
  {
    cmdStr << "  -Compress " << CompressionMethodToStr (compressionMethod);

    cmdStr << "  -b " << bitsToReduceBy;

    cmdStr << "  -WBR " << (weightBitReduction ? "Yes":"No");

    if  (unBalancedBits > 0)
      cmdStr << "  -UnBalBits " << unBalancedBits;

    if  (!unBalancedBitsStr.Empty ())
      cmdStr << "  -UnBalBitsStr " << unBalancedBitsStr;

    if  (normalizeNominalFeatures)
      cmdStr << "  -NormalizeNominal " << (normalizeNominalFeatures ? "Yes" : "No");
  }

  return  cmdStr;
}  /* ToCmdLineStr */




void  ModelParam::WriteXML (ostream&  o)  const
{
  log.Level (20) << "ModelParam::Save to XML to ostream." << endl;

  o << "<ModelParam>" << endl
    << "ModelParamType"     << "\t"  << ModelParamTypeStr    ()             << std::endl
    << "BitsToReduceBy"     << "\t"  << bitsToReduceBy                      << std::endl
    << "CompressionMethod"  << "\t"  << CompressionMethodStr ()             << std::endl
    << "EncodingMethod"     << "\t"  << EncodingMethodStr    ()             << std::endl
    << "ExamplesPerClass"   << "\t"  << examplesPerClass                    << std::endl
    << "FileName"           << "\t"  << fileName                            << std::endl
    << "SelectedFeatures"   << "\t"  << selectedFeatures.ToCommaDelStr ()   << std::endl
    << "UnBalancedBits"     << "\t"  << unBalancedBits                      << std::endl
    << "UnBalancedBitsStr"  << "\t"  << unBalancedBitsStr                   << std::endl
    << "WeightBitReduction" << "\t"  << (weightBitReduction ? "Yes" : "No") << std::endl
    << endl;
  
  o << "<SpecificImplementation>"  << endl;
  WriteSpecificImplementationXML (o);
  o << "</SpecificImplementation>"  << endl;

 o << "</ModelParam>" << endl;
}  /* WriteXML */





void  ModelParam::ReadXML (istream&  i)
{
  log.Level (20) << "ModelParam::Read from XML file." << endl;

  char  buff[10240];
  
  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);
    KKStr  field = ln.ExtractQuotedStr ("\n\r\t", 
                                         true      // true = decode escape charaters
                                        );
    field.Upper ();

    if  (field.EqualIgnoreCase ("</ModelParam>"))
    {
      break;
    }

    else if  (field == "<CMDLINE>")
    {
      bool  validFormat;
      KKStr  cmdLine = ln.ExtractQuotedStr ("\n\r\t", 
                                             true      // true = decode escape charaters
                                            );
      ParseCmdLine (cmdLine, validFormat);
    }

    else if  (field.EqualIgnoreCase ("BitsToReduceBy"))
    {
      bitsToReduceBy = ln.ExtractTokenInt ("\t");
    }

    else if  (field.EqualIgnoreCase ("CompressionMethod"))
    {
      KKStr  compMethodStr = ln.ExtractToken2 ("\t");
        
      compressionMethod = CompressionMethodFromStr (compMethodStr);
      if  (compressionMethod == BRNull)
      {
        log.Level (-1) << "ModelParam::ReadXML   ***ERROR***  Invalid Compression Method[" << compMethodStr << "]" << endl;
        validParam = false;
      }
    }

    else if  (field.EqualIgnoreCase ("EncodingMethod"))
    {
      KKStr  encodingMethodStr =  ln.ExtractToken2 ("\t");
      encodingMethod = EncodingMethodFromStr (encodingMethodStr);
      if  (encodingMethod == Encoding_NULL)
      {
        log.Level (-1) << endl << "ModelParam::ReadXML   ***ERROR***  Invalid EncodingMethod Method[" << encodingMethodStr << "]" << endl;
        validParam = false;
      }
    }

    else if  (field.EqualIgnoreCase ("ExamplesPerClass"))
    {
      examplesPerClass = ln.ExtractTokenInt ("\t");
      if  (examplesPerClass < 1)
      {
        log.Level (-1) 
          << endl << endl
          << "ModelParam::ReadXML   ***ERROR***  Invalid ExamplesPerClass[" << examplesPerClass << "]" << endl
          << endl;
        validParam = false;
        examplesPerClass = int32_max;
      }
    }

    else if  (field.EqualIgnoreCase ("FileName"))
    {
      fileName = ln.ExtractToken2 ("\t");
    }
  
    else if  (field.EqualIgnoreCase ("SelectedFeatures"))
    {
      bool  validFeatureStr = false;
      KKStr  selectedFeaturesStr =  ln.ExtractToken2 ("\t");
      selectedFeatures.ExtractFeatureNumsFromStr (selectedFeaturesStr, validFeatureStr);
      if  (!validFeatureStr)
      {
        log.Level (-1) << "ModelParam::ReadXML   ***ERROR***  Invalid selected Features[" << selectedFeaturesStr << "]" << endl;
        validParam = false;
      }
    }

    else if  (field.EqualIgnoreCase ("<SpecificImplementation>"))
    {
      ReadSpecificImplementationXML (i);
    }

    else if  (field.EqualIgnoreCase ("UnBalancedBits"))
    {
      unBalancedBits = ln.ExtractTokenInt ("\t");
    }


    else if  (field.EqualIgnoreCase ("UnBalancedBitsStr"))
    {
      unBalancedBitsStr = ln.ExtractToken2 ("\t");
    }


    else if  (field.EqualIgnoreCase ("WeightBitReduction"))
    {
      weightBitReduction = ln.ExtractTokenBool ("\t");
    }
  }
}  /* ReadXML */






void  ModelParam::ReadXML (KKStr&  _fileName,
                           bool&   _successful
                          )
{
  log.Level (10) << "ModelParam::ReadXML - File[" << _fileName << "]." << endl;

  _successful = true;

  fileName = _fileName;

  ifstream  inputFile (fileName.Str ());

  if  (!inputFile.is_open ())
  {
    log.Level (-1) << "ModelParam::ReadXML      *** ERROR ***" << endl;
    log.Level (-1) << "                     Could Not Open File[" << fileName << "]." << endl;
    _successful = false;
    return;
  }

  ReadXML (inputFile);
   
  inputFile.close ();
}  /* ReadXML */




int32  ModelParam::NumOfFeaturesAfterEncoding ()  const
{
  int32 z;
  int32 numFeaturesAfterEncoding = 0;
  int32 numOfFeaturesSelected = selectedFeatures.NumOfFeatures ();

  switch (EncodingMethod ())
  {
  case BinaryEncoding:
    for  (z = 0; z < numOfFeaturesSelected; z++)
    {
      int32  fieldNum = selectedFeatures[z];
      if  ((fileDesc->Type (fieldNum) == NominalAttribute)  ||  (fileDesc->Type (fieldNum) == SymbolicAttribute))
        numFeaturesAfterEncoding += fileDesc->Cardinality (fieldNum, log);
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

  return  numFeaturesAfterEncoding;
}  /* NumOfFeaturesAfterEncoding */






KKStr  ModelParam::EncodingMethodToStr (EncodingMethodType  encodingMethod)
{
  if  (encodingMethod == BinaryEncoding)
    return  "Binary";

  else if  (encodingMethod == ScaledEncoding)
    return  "Scale";

  else
    return  "None";
}  /* EncodingMethodToStr */




        
ModelParam::EncodingMethodType  ModelParam::EncodingMethodFromStr (const KKStr&  encodingMethodStr)
{
  KKStr  encodingMethodUpper = encodingMethodStr.ToUpper ();

  if  ((encodingMethodUpper == "BINARY")  ||  (encodingMethodUpper == "BIN"))
     return  BinaryEncoding;

  if  (encodingMethodUpper == "SCALE")
     return  ScaledEncoding;

  if  (encodingMethodUpper == "NONE")
    return  NoEncoding;

  return  Encoding_NULL;
}  /* EncodingMethodFromStr */




KKStr   ModelParam::CompressionMethodToStr (CompressionMethodType  compressionMethod)
{
  if  (compressionMethod == BRNull)
    return  "brNULL";

  else if  (compressionMethod == BRnoCompression)
    return  "NoComp";

  else if (compressionMethod == BRcompressionPost)
    return  "BitRedPost";

  else if (compressionMethod == BRcompressionPre)
    return  "BitRedPre";

  else if  (compressionMethod == BRunBalancedVariance)
    return "UnBalancedVariance";

  else if  (compressionMethod == BRunBalancedSpecified)
    return  "UnBalancedSpecified";

  return "";
}  /* CompressionMethodStr */



ModelParam::CompressionMethodType   ModelParam::CompressionMethodFromStr (const KKStr& compressionMethodStr)
{
  KKStr  compMethodUpper (compressionMethodStr);
  compMethodUpper.Upper ();
  
  if  ((compMethodUpper == "NOCOMP")  ||  (compMethodUpper == ""))
    return  BRnoCompression;

  else if  ((compMethodUpper == "NULL")             ||
            (compMethodUpper == "BRNULL")           ||
            (compMethodUpper.Empty ())
           )
    return  BRNull;

  else if  ((compMethodUpper == "BITREDPOST")       ||  
            (compMethodUpper == "BITREDUCTION")     ||
            (compMethodUpper == "BITREDUCTIONPOST") ||
            (compMethodUpper == "BRPOST")
           )
    return  BRcompressionPost;

  else if  ((compMethodUpper == "BITREDPRE")        ||
            (compMethodUpper == "BITREDUCTIONPRE")  ||
            (compMethodUpper == "BRPRE")
           )
    return  BRcompressionPre;

  else if  ((compMethodUpper == "UNBALVAR")  ||  
            (compMethodUpper == "UNBALANCEDVARIANCE")
           )
    return  BRunBalancedVariance;


  else if  (compMethodUpper == "UNBALANCEDSPECIFIED")
    return BRunBalancedSpecified;


  return BRnoCompression;
}  /* CompressionMethodFromStr */


