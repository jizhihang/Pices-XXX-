#include  "FirstIncludes.h"

#include  <stdio.h>

#include  <fstream>
#include  <string>
#include  <iostream>
#include  <vector>



#include  "MemoryDebug.h"
using namespace std;

#include  "BasicTypes.h"
#include  "MLLTypes.h"

using namespace KKU;
using namespace MLL;




#include  "SVMparam.h"


#include  "BinaryClassParms.h"
#include  "ClassAssignments.h"
#include  "FeatureVector.h"
#include  "FileDesc.h"
#include  "MLClass.h"
#include  "OSservices.h"
#include  "RunLog.h"



SVMparam::SVMparam  (KKStr&                 _cmdLineStr,
                     const FeatureNumList&  _selectedFeatures,
                     FileDescPtr            _fileDesc,
                     RunLog&                _log,
                     bool&                  _validFormat
                    ):

  binaryParmsList             (NULL),
  bitsToReduceBy              (0),
  compressionMethod           (BRnoCompression),
  encodingMethod              (NoEncoding),
  fileDesc                    (_fileDesc),
  fileName                    (),
  log                         (_log),
  param                       (),
  samplingRate                (0.0f),
  selectedFeatures            (_selectedFeatures), 
  selectionMethod             (SelectByVoting),
  unBalancedBits              (0),
  unBalancedBitsStr           (""),
  useProbabilityToBreakTies   (false),
  validParam                  (false),
  weightBitReduction          (false)
{
  if  (!fileDesc)
  {
    log.Level (-1) << endl
                   << "SVMparam::SVMparam      *** ERROR ***" << endl
                   << "                       fileDesc == NULL" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  ParseCmdLine (_cmdLineStr, _validFormat);
}





SVMparam::SVMparam  (FileDescPtr  _fileDesc,
                     RunLog&      _log
                    ):

  binaryParmsList           (NULL),
  bitsToReduceBy            (0),
  compressionMethod         (BRnoCompression),
  encodingMethod            (NoEncoding),
  fileDesc                  (_fileDesc),
  fileName                  (),
  log                       (_log),
  machineType               (OneVsOne),
  param                     (),
  samplingRate              (0.0f),
  selectedFeatures          (_fileDesc), 
  selectionMethod           (SelectByVoting),
  unBalancedBits            (0),
  unBalancedBitsStr         (""),
  useProbabilityToBreakTies (false),
  validParam                (false),
  weightBitReduction        (false)
{
  if  (!fileDesc)
  {
    log.Level (-1) << endl
                   << "SVMparam::SVMparam      *** ERROR ***" << endl
                   << "                       fileDesc == NULL" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }
}





SVMparam::SVMparam  (const SVMparam&  _svmParam):

  binaryParmsList            (NULL),
  bitsToReduceBy             (_svmParam.bitsToReduceBy),
  compressionMethod          (_svmParam.compressionMethod),
  encodingMethod             (_svmParam.encodingMethod),
  fileDesc                   (_svmParam.fileDesc),
  fileName                   (_svmParam.fileName),
  log                        (_svmParam.log),
  machineType                (_svmParam.machineType),
  param                      (_svmParam.param),
  samplingRate               (_svmParam.samplingRate),
  selectedFeatures           (_svmParam.selectedFeatures),
  selectionMethod            (_svmParam.selectionMethod),
  unBalancedBits             (_svmParam.unBalancedBits),
  unBalancedBitsStr          (_svmParam.unBalancedBitsStr),
  useProbabilityToBreakTies  (_svmParam.useProbabilityToBreakTies),
  validParam                 (_svmParam.validParam),
  weightBitReduction         (_svmParam.weightBitReduction)
{
  if  (_svmParam.binaryParmsList)
  {
    binaryParmsList = _svmParam.binaryParmsList->DuplicateListAndContents ();
      
    //  new BinaryClassParmsList (true, _svmParam.binaryParmsList->QueueSize ());
    //  binaryParmsList  = new BinaryClassParmsList (*(_svmParam.binaryParmsList));
  }
  param = _svmParam.param;
}



SVMparam::~SVMparam  ()
{
  delete  binaryParmsList;  binaryParmsList = NULL;
}


int32  SVMparam::MemoryConsumedEstimated () const
{
  int32  memoryConsumedEstimated = sizeof (SVMparam) 
    + fileName.MemoryConsumedEstimated ()
    + selectedFeatures.MemoryConsumedEstimated ()
    + unBalancedBitsStr.MemoryConsumedEstimated ();

  if  (binaryParmsList)
    memoryConsumedEstimated += binaryParmsList->MemoryConsumedEstimated ();

  // fileDesc   We do not own 'fileDesc'.

  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated  */



void  SVMparam::A_Param (float  _A)
{
  param.A = _A;
}




void  SVMparam::C_Param (double  _CC)
{
  param.C = _CC;
}





void  SVMparam::ParseCmdLineParameter (const KKStr&  parameter,
                                       const KKStr&  value,
                                       bool&         parameterUsed,
                                       bool&         _validFormat
                                      )
{
  parameterUsed = true;
  _validFormat  = true;

  KKStr  field = parameter;
  double  valueNum = value.ToDouble ();

  field.Upper ();
  KKStr valueUpper (value);
  valueUpper.Upper ();


  param.ProcessSvmParameter (parameter,value, valueNum, parameterUsed);
  if  (parameterUsed)
    return;

  parameterUsed = true;

  if  ((field == "-B")  ||  (field == "-BITREDUCTION")  ||  (field == "-BR"))
  {
    bitsToReduceBy = atoi (value.Str ());
  }

  else if (field == "-COMPRESS")
  {
    compressionMethod = CompressionMethodFromStr (valueUpper);
  }

  else if ((field == "-ENCODE"))
  {
    encodingMethod = EncodingMethodFromStr (valueUpper);
  }

  else if  (field == "-MT")
  {
    machineType = MachineTypeFromStr (valueUpper);
    if  (machineType == MachineType_NULL)
    {
      _validFormat = false;
      log.Level (-1) << endl 
        << "SVMparam::ParseCmdLineParameter     *** ERROR ***" << endl
        << "                Invalid -MT Parm[" << value << "]" << endl
        << endl;
    }
  }

  else if  ((field == "-SM")  ||  (field == "-SELECTIONMETHOD"))
  {
    selectionMethod  = SelectionMethodFromStr (valueUpper);
    if  (selectionMethod == SelectionMethod_NULL)
    {
      log.Level (-1) << endl
        << "SVMparam::ParseCmdLineParameter    *** ERROR ***"  << endl
        << "            Invalid SelectionMethod (-SM)[" << value << "]." << endl
        << endl;
      _validFormat = false;
    }
  }

  else if  ((field == "-SR")  ||  (field == "-SAMPLINGRATE"))
  {
    samplingRate = float (atof (value.Str ()));
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

  else if  ((field == "-UP")  ||  (field == "-USEPROBABILITY"))
  {
    useProbabilityToBreakTies = true;
  }

  else if  ((field == "-WEIGHTBITREDUCTION")  ||  
    (field == "-WBR")
    )
  {
    weightBitReduction = false;
    if  ((valueUpper == "Y") ||  (valueUpper == "YES")  ||  (valueUpper == "ON")   ||  (valueUpper.Empty ()))
    {
      weightBitReduction = true;
    }
    else
    {
      weightBitReduction = false;
    }
  }

  else
  {
    parameterUsed = false;
  }
}  /* ParseCmdLineParameter */




void  SVMparam::ParseCmdLine (KKStr  _cmdLineStr,
                              bool&   _validFormat
                             )
{
  _validFormat = true;

  {
    // Will create a svm_parameter object from _cmdLineStr,  the paramters that 
    // are not specific to svm_parameter will be left over and then we can 
    // process them.
    svm_parameter  tempParam (_cmdLineStr);
    param = tempParam;
  }

  //DecodeParamStr (_cmdLineStr, param);

  machineType = OneVsOne;

  useProbabilityToBreakTies = false;

  KKStr  field (_cmdLineStr.ExtractToken (" \t\n\r"));
  KKStr  value;

  double  valueNum;

  while  (!field.Empty ()  &&  _validFormat)
  {
    if  (field[0] != '-')  
    {
      log.Level (-1) << "SVMparam::ParseCmdLine  *** Invalid Parameter["
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

    bool  parameterUsed = false;
    ParseCmdLineParameter (field, value, parameterUsed, _validFormat);
    if  (!parameterUsed)
    {
      log.Level (-1) << "SVMparam::ParseCmdLine - Invalid Parameter["  
        << field << "]  Value[" << value << "]."
        << endl;
      _validFormat = false;
      break;
    }
    field = _cmdLineStr.ExtractToken (" \t\n\r");
  } 


  validParam = _validFormat;
}  /* ParseCmdLine */




/**
 * @brief Convert a svm_parameter struct to a cmdline str.
*/
KKStr   SVMparam::SvmParamToString (const svm_parameter&  _param)  const
{
  log.Level (60) << "SVMparam::SvmParamToString Entered." << endl;

  KKStr  cmdStr (300);

  cmdStr << _param.ToCmdLineStr ();

  return  cmdStr;
}  /* SvmParamToString */




/**
 * @brief Convert all parameters to a command line string.
*/
KKStr   SVMparam::ToString () const
{
  log.Level (60) << "SVMparam::ToString - Entered." << endl;

  KKStr  cmdStr (300);

  cmdStr = SvmParamToString (param);

  if  (encodingMethod != NoEncoding)
    cmdStr << "  -Encode " + EncodingMethodToStr (encodingMethod);

  cmdStr << "  -MT " << MachineTypeToStr (machineType);
  cmdStr << "  -SM " << SelectionMethodToStr (selectionMethod);

  if  (compressionMethod != BRnoCompression)
  {
    cmdStr << "  -Compress " << CompressionMethodStr (compressionMethod);

    cmdStr << "  -b " << bitsToReduceBy;

    cmdStr << "  -WBR " << (weightBitReduction ? "Yes":"No");

    if  (unBalancedBits > 0)
      cmdStr << "  -UnBalBits " << unBalancedBits;

    if  (!unBalancedBitsStr.Empty ())
      cmdStr << "  -UnBalBitsStr " << unBalancedBitsStr;
  }

  if  (machineType == BoostSVM)
  {
    cmdStr << "  -SamplingRate " << samplingRate;
  }


  if  (useProbabilityToBreakTies)
    cmdStr << "  -UseProbability";

  return  cmdStr;
}  /* ToString */






void  SVMparam::Save (KKStr&  _fileName,
                      bool&    _successful
                     )
{
  log.Level (20) << "SVMparam::Save - File[" << fileName << "]." << endl;

  _successful = true;

  fileName = _fileName;

  ofstream outFile (fileName.Str ());

  WriteXML (outFile);

  outFile.close ();
}  /* Save */




void  SVMparam::WriteXML (ostream&  o)  const
{
  log.Level (20) << "SVMparam::Save to XML to ostream." << endl;

  o << "<SVMparam>" << endl;

  o << "<CmdLine>"  << "\t" << ToString ().QuotedStr () << "\t" << "</CmdLine>" << endl;

  o << "<SelectedFeatures>"         << "\t"
    << selectedFeatures.ToString () << "\t"
    << "</SelectedFeatures>"
    << endl;


  if  (binaryParmsList)
    binaryParmsList->WriteXML (o);

  o << "</SVMparam>" << endl;
}  /* WriteXML */



void  SVMparam::ReadXML (istream&  i)
{
  log.Level (20) << "SVMparam::Read from XML file." << endl;

  char  buff[20480];
  
  delete  binaryParmsList;  binaryParmsList = NULL;

  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);
    KKStr  field = ln.ExtractQuotedStr ("\n\r\t", true);      // true = decode escape charaters
    field.Upper ();

    if  (field == "</SVMPARAM>")
      break;

    else if  (field == "<CMDLINE>")
    {
      bool  validFormat;
      KKStr  cmdLine = ln.ExtractQuotedStr ("\n\r\t", 
                                             true      // true = decode escape charaters
                                            );
      ParseCmdLine (cmdLine, validFormat);
    }

    else if  (field == "<SELECTEDFEATURES>")
    {
      bool sucessful;
      KKStr  featureNumStr = ln.ExtractQuotedStr ("\n\r\t", true);      // true = decode escape charaters
      selectedFeatures.ExtractFeatureNumsFromStr (featureNumStr, sucessful);
    }

    else if  (field == "<BINARYCLASSPARMSLIST>")
    {
      binaryParmsList = BinaryClassParmsList::CreateFromXML (i, fileDesc, log);
    }
  }
}  /* ReadXML */



void  SVMparam::ReadXML (FILE*  i)
{
  log.Level (20) << "SVMparam::Read from XML file." << endl;

  char  buff[10240];
  
  delete  binaryParmsList;  binaryParmsList = NULL;

  while  (fgets (buff, sizeof (buff), i))
  {
    KKStr  ln (buff);
    KKStr  field = ln.ExtractQuotedStr ("\n\r\t", 
                                         true      // true = decode escape charaters
                                        );
    field.Upper ();

    if  (field == "</SVMPARAM>")
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

    else if  (field == "<SELECTEDFEATURES>")
    {
      bool sucessful;
      KKStr  featureNumStr = ln.ExtractQuotedStr ("\n\r\t", 
                                                   true      // true = decode escape charaters
                                                  );
      selectedFeatures.ExtractFeatureNumsFromStr (featureNumStr, sucessful);
    }

    else if  (field == "<BINARYCLASSPARMSLIST>")
    {
      binaryParmsList = BinaryClassParmsList::CreateFromXML (i, fileDesc, log);
    }
  }
}  /* ReadXML */








void  SVMparam::Load (ClassAssignments& _assignments,
                      KKStr&            _fileName,
                      bool&             _successful
                     )
{
  log.Level (10) << "SVMparam::Load - File[" << _fileName << "]." << endl;

  _successful = true;

  fileName = _fileName;
  FILE*  inputFile = osFOPEN (fileName.Str (), "r");

  if  (!inputFile)
  {
    log.Level (-1) << "SVMparam::Load      *** ERROR ***" << endl;
    log.Level (-1) << "                Could Not Open File[" << fileName << "]." << endl;
    _successful = false;
    return;
  }

  this->ReadXML (inputFile);
   
  fclose (inputFile);
}  /* Load */





BinaryClassParmsPtr   SVMparam::GetParamtersToUseFor2ClassCombo (MLClassConstPtr  class1,
                                                                 MLClassConstPtr  class2
                                                                )
{
  BinaryClassParmsPtr  twoClassComboParms = NULL;

  if  (binaryParmsList == NULL)
  {
    binaryParmsList = new BinaryClassParmsList (true, 20);
    twoClassComboParms = NULL;
  }
  else
  {
    twoClassComboParms = binaryParmsList->LookUp (class1, class2);
  }

  if  (!twoClassComboParms)
  {
    twoClassComboParms = new BinaryClassParms (class1, class2, param, selectedFeatures, 1.0f);
    binaryParmsList->PushOnBack (twoClassComboParms);
  }

  return  twoClassComboParms;
}  /* GetParamtersToUSeFor2ClassCombo */





BinaryClassParmsPtr   SVMparam::GetBinaryClassParms (MLClassConstPtr  class1,
                                                     MLClassConstPtr  class2
                                                    )
{
  if  (binaryParmsList == NULL)
    return NULL;
  else
    return binaryParmsList->LookUp (class1, class2);
}  /* GetBinaryClassParms */





const FeatureNumList&  SVMparam::GetFeatureNums ()  const
{
  return  selectedFeatures;
}


const FeatureNumList&  SVMparam::GetFeatureNums (MLClassConstPtr  class1,
                                                 MLClassConstPtr  class2
                                                )  const
{
  if  (!binaryParmsList)
  {
    // This configuration file does not specify feature selction by binary classes,
    // so return the genral one specifiedc for model.
    return  selectedFeatures;
  }

  BinaryClassParmsPtr  twoClassComboParm =  binaryParmsList->LookUp (class1, class2);
  if  (!twoClassComboParm)
    return  selectedFeatures;


  return  twoClassComboParm->SelectedFeatures ();
}  /* GetFeatureNums */




void    SVMparam::AddBinaryClassParms (BinaryClassParmsPtr  binaryClassParms)
{
  if  (!binaryParmsList)
    binaryParmsList = new BinaryClassParmsList (true, 20);

  binaryParmsList->PushOnBack (binaryClassParms);
}  /* AddBinaryClassParms */



float   SVMparam::AvgMumOfFeatures ()
{
  //if  (!binaryParmsList)
  //  return  (float)selectedFeatures.NumOfFeatures ();

  if  ((machineType == BinaryCombos)  &&  (binaryParmsList))
  {
    int32  totalNumOfFeatures = 0;
    BinaryClassParmsList::iterator  idx;
    for  (idx = binaryParmsList->begin ();  idx != binaryParmsList->end ();  idx++)
    {
      BinaryClassParmsPtr  bcp = *idx;
      totalNumOfFeatures += bcp->SelectedFeatures ().NumOfFeatures ();
    }

    if  (binaryParmsList->QueueSize () == 0)
      return 0.0f;
    return  (float)totalNumOfFeatures / (float)(binaryParmsList->QueueSize ());
  }

  else
  {
    return  (float)selectedFeatures.NumOfFeatures ();
  }
}  /* AvgMumOfFeatures */



/**
 @brief  returns back the average number of features per training example.
 @details  Will calculate the average number of features per tyraining example. For each
           binary class combination, multiplies the number of training examples for that pair 
           by the number of features for that pair.  the sum of all class pairs are then 
           divided by the total number of examples.
 @param[in] trainExamples  List of traninig examples that were or are to be used to train with.
 */

float  SVMparam::AvgNumOfFeatures (FeatureVectorListPtr  trainExamples)  const
{
  if  (machineType == BinaryCombos)
  {
    long  totalNumFeaturesUsed = 0;
    long  toatlNumExamples     = 0;
    ClassStatisticListPtr  stats = trainExamples->GetClassStatistics ();
    if  (!stats)
      return (float)selectedFeatures.NumOfFeatures ();

    uint32  idx1 = 0;
    uint32  idx2 = 0;

    for  (idx1 = 0;  idx1 < (stats->size() - 1);  idx1++)
    {
      ClassStatisticPtr   class1Stats = stats->IdxToPtr (idx1);
      MLClassConstPtr  class1      = class1Stats->MLClass ();
      uint32              class1Qty   = class1Stats->Count ();

      for  (idx2 = idx1 + 1;  idx2 < (stats->size());  idx2++)
      {
        ClassStatisticPtr   class2Stats = stats->IdxToPtr (idx2);
        MLClassConstPtr  class2      = class2Stats->MLClass ();
        uint32              class2Qty   = class2Stats->Count ();

        int32  numFeaturesThisCombo = GetFeatureNums (class1, class2).NumSelFeatures ();
        int32  numExamplesThisCombo = class1Qty + class2Qty;

        totalNumFeaturesUsed += numFeaturesThisCombo * numExamplesThisCombo;
        toatlNumExamples     += numExamplesThisCombo;
      }
    }

    delete  stats;  stats = NULL;

    return  (float)totalNumFeaturesUsed / (float)toatlNumExamples;
  }

  else
  {
    return  (float)selectedFeatures.NumOfFeatures ();
  }
}  /* AvgMumOfFeatures */








int32  SVMparam::NumOfFeaturesAfterEncoding ()  const
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




void  SVMparam::SetFeatureNums (MLClassConstPtr     class1,
                                MLClassConstPtr     class2,
                                const FeatureNumList&  _features,
                                float                  _weight
                               )
{
  if  (!binaryParmsList)
  {
    if  (_weight < 0)
      _weight = 1;
    AddBinaryClassParms (class1, class2, param, _features, _weight);
  }
  else
  {
    BinaryClassParmsPtr  binaryParms = binaryParmsList->LookUp (class1, class2);
    if  (binaryParms)
    {
      if  (_weight < 0)
        _weight = binaryParms->Weight ();
      binaryParms->SelectedFeatures (_features);
    }
    else
    {
      if  (_weight < 0)
        _weight = 1.0f;
      AddBinaryClassParms (class1, class2, param, _features, _weight);
    }
  }
}  /* SetFeatureNums */




void  SVMparam::SetFeatureNums (const FeatureNumList&  _features)
{
  selectedFeatures = _features;
}  /* SetFeatureNums */



double  SVMparam::C_Param (MLClassConstPtr  class1,
                           MLClassConstPtr  class2
                          )  const
{
  if  (!binaryParmsList)
  {
    cerr << endl << endl
      << "SVMparam::C_Param   ***ERROR***     'binaryParmsList'  is not defined." << endl
      << endl;
    return 0.0;
  }

  BinaryClassParmsPtr  binaryParms = binaryParmsList->LookUp (class1, class2);
  if  (!binaryParms)
  {
    cerr << endl << endl 
      << "SVMparam::C_Param   ***ERROR***    No entry for Class[" << class1->Name () << ", " << class2->Name () << "]" << endl
      << endl;
    return 0.0;
  }
  else
  {
    return binaryParms->C ();
  }
}




void  SVMparam::C_Param (MLClassConstPtr  class1,
                         MLClassConstPtr  class2,
                         double              cParam
                        )
{
  if  (!binaryParmsList)
  {
    cerr << endl << endl
      << "SVMparam::C_Param   ***ERROR***     'binaryParmsList'  is not defined." << endl
      << endl;
    return;
  }

  BinaryClassParmsPtr  binaryParms = binaryParmsList->LookUp (class1, class2);
  if  (!binaryParms)
  {
    svm_parameter binaryParam = param;
    binaryParam.C = C_Param ();
    AddBinaryClassParms (class1, class2, binaryParam, this->SelectedFeatures (), 1);
  }
  else
  {
    binaryParms->C (cParam);
  }
}  /* C_Param */







void  SVMparam::SetBinaryClassFields (MLClassConstPtr     class1,
                                      MLClassConstPtr     class2,
                                      const svm_parameter&   _param,
                                      const FeatureNumList&  _features,
                                      float                  _weight
                                     )
{
  if  (!binaryParmsList)
  {
    AddBinaryClassParms (class1, class2, _param, _features, _weight);
  }
  else
  {
    BinaryClassParmsPtr  binaryParms = binaryParmsList->LookUp (class1, class2);
    if  (binaryParms)
    {
      binaryParms->Param            (_param); 
      binaryParms->SelectedFeatures (_features);
      binaryParms->Weight           (_weight);
    }
    else
    {
      AddBinaryClassParms (class1, class2, _param, _features, _weight);
    }
  }
}  /* SetBinaryClassFields */





/**
 * @brief  Add a Binary parameters using svm_parametr cmd line str.
 *         Typically used by TrainingConfiguration.
*/
void  SVMparam::AddBinaryClassParms (MLClassConstPtr    class1,
                                     MLClassConstPtr    class2,
                                     const svm_parameter&  _param,
                                     const FeatureNumList& _selectedFeatures,
                                     float                 _weight
                                    )
{
  AddBinaryClassParms (new BinaryClassParms (class1, class2, _param, _selectedFeatures, _weight));
}  /* AddBinaryClassParms */




KKStr  MLL::EncodingMethodToStr (SVM_EncodingMethod  encodingMethod)
{
  if  (encodingMethod == BinaryEncoding)
    return  "Binary";

  else if  (encodingMethod == ScaledEncoding)
    return  "Scale";

  else
    return  "None";
}  /* EncodingMethodToStr */


        
SVM_EncodingMethod  MLL::EncodingMethodFromStr (const KKStr&  encodingMethodStr)
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



KKStr  MLL::KernalTypeToStr (SVM_KernalType  kernalType)
{
  switch  (kernalType)
  {
  case  KT_Linear:      return "Linear";
  case  KT_Polynomial:  return "Polynomial";
  case  KT_RBF:         return "RBF";
  default: return "UnKnown";
  }

  return "";
}  /* KernalTypeToStr */



SVM_KernalType  MLL::KernalTypeFromStr (const KKStr&  kernalTypeStr)
{
  KKStr kernalTypeUpper = kernalTypeStr;
  kernalTypeUpper.Upper ();

  if  ((kernalTypeUpper == "LINEAR")  ||  
       (kernalTypeUpper == "LIN")     ||
       (kernalTypeUpper == "L")
      )
    return  KT_Linear;

  if  ((kernalTypeUpper == "POLYNOMIAL")  ||
       (kernalTypeUpper == "POLY")        ||
       (kernalTypeUpper == "P")           ||
       (kernalTypeUpper == "PN")
       )
    return  KT_Polynomial;
  

  if  ((kernalTypeUpper == "RBF")                 ||
       (kernalTypeUpper == "R")                   ||
       (kernalTypeUpper == "RADIALBASIS")         ||
       (kernalTypeUpper == "RADIALBASISFUNC")     ||
       (kernalTypeUpper == "RADIALBASISFUNCTION")
      )
    return  KT_RBF;

  return  KT_Linear;
}  /* KernalTypeToStr */




KKStr  MLL::MachineTypeToStr (SVM_MachineType  machineType)
{
  if  (machineType == OneVsOne)
    return  "OneVsOne";

  else if  (machineType == OneVsAll)
    return  "OneVsAll";

  else if  (machineType == BinaryCombos)
    return "Binary";

  else if  (machineType == BoostSVM)
    return "BoostSVM";

  return "";
}  /* MachineTypeToStr */





SVM_MachineType  MLL::MachineTypeFromStr (const KKStr&  machineTypeStr)
{
  KKStr  machineTypeUpper = machineTypeStr.ToUpper ();

  if  (machineTypeUpper == "ONEVSONE")
    return  OneVsOne;

  else  if  (machineTypeUpper == "ONEVSALL")
    return  OneVsAll;

  else  if  (machineTypeUpper == "BINARY")
    return  BinaryCombos;

  else  if  (machineTypeUpper == "BOOSTSVM")
    return  BoostSVM;

  return  MachineType_NULL;
}  /* MachineTypeFromStr */
 



KKStr  MLL::SelectionMethodToStr (SVM_SelectionMethod  selectionMethod)
{
  if  (selectionMethod == SelectByVoting)
    return  "Voting";

  if  (selectionMethod == SelectByProbability)
    return  "Probability";

  return "";
}  /* SelectionMethodToStr */




SVM_SelectionMethod  MLL::SelectionMethodFromStr (const KKStr&  selectionMethodStr)
{
  KKStr  selectionMethodUpper = selectionMethodStr.ToUpper ();

  if  ((selectionMethodUpper == "VOTE")  ||  (selectionMethodUpper == "VOTING"))
    return  SelectByVoting;

  if  ((selectionMethodUpper == "P")     ||  
       (selectionMethodUpper == "PROB")  ||
       (selectionMethodUpper == "PROBABILITY")
      )
    return  SelectByProbability;

  return  SelectionMethod_NULL;

}  /* SelectionMethodFromStr */




KKStr   MLL::CompressionMethodStr (SVM_CompressionMethod  compressionMethod)
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



SVM_CompressionMethod   MLL::CompressionMethodFromStr (const KKStr& compressionMethodStr)
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


