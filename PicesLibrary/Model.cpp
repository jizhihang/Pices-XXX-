#include  "FirstIncludes.h"

#include  <exception>
#include  <fstream>
#include  <iomanip>
#include  <iostream>
#include  <math.h>
#include  <set>
#include  <sstream>
#include  <stdio.h>
#include  <string>
#include  <vector>


#include  "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace  KKU;


#include "Model.h"
#include "ModelOldSVM.h"
#include "ModelSvmBase.h"
#include "ModelKnn.h"
#include "ModelUsfCasCor.h"
#include "ModelDual.h"


#include "ClassProb.h"
#include "FeatureEncoder2.h"
#include "FeatureNumList.h"
#include "FeatureVector.h"
#include "ModelParam.h"
#include "ModelParamDual.h"
#include "ModelParamOldSVM.h"
#include "NormalizationParms.h"
using namespace  MLL;



Model::Model (const Model&  _model):
    alreadyNormalized          (false),
    cancelFlag                 (_model.cancelFlag),
    classes                    (NULL),
    classesIndex               (NULL),
    compression_stats          (),
    crossClassProbTable        (NULL),
    crossClassProbTableSize    (0),
    fileDesc                   (_model.fileDesc),
    encoder                    (NULL),
    log                        (_model.log),
    name                       (_model.name),
    normParms                  (NULL),
    numOfClasses               (_model.numOfClasses),
    classProbs                 (NULL),
    rootFileName               (_model.rootFileName),
    param                      (NULL),
    trainExamples              (NULL),
    trianingPrepTime           (0.0),
    trainingTime               (_model.trainingTime),
    trainingTimeStart          (_model.trainingTime),
    votes                      (NULL),
    validModel                 (_model.validModel),
    weOwnTrainExamples         (false)
{
  numOfClasses = 0;
  if  (_model.param != NULL)
    param = _model.param->Duplicate ();

  if  (_model.classes)
  {
    classes = new MLClassConstList (*_model.classes);
    numOfClasses = classes->QueueSize ();

    AllocatePredictionVariables ();
  }

  if  (_model.classesIndex)
    classesIndex = new ClassIndexList (*_model.classesIndex);


  if  (_model.encoder)
    encoder = new FeatureEncoder2 (*_model.encoder);
}


    
/**
 *@brief  Use this when you are planning on creating a empty model without parameters.
 */
Model::Model (FileDescPtr           _fileDesc,
              volatile const bool&  _cancelFlag,
              RunLog&               _log
             ):
    alreadyNormalized          (false),
    cancelFlag                 (_cancelFlag),
    classes                    (NULL),
    classesIndex               (NULL),
    compression_stats          (),
    crossClassProbTable        (NULL),
    encoder                    (NULL),
    fileDesc                   (_fileDesc),
    log                        (_log),
    name                       (),
    normParms                  (NULL),
    numOfClasses               (0),
    param                      (NULL),
    classProbs                 (NULL),
    rootFileName               (),
    trainExamples              (NULL),
    trianingPrepTime           (0.0),
    trainingTime               (0.0),
    trainingTimeStart          (0.0),
    validModel                 (true),
    votes                      (NULL),
    weOwnTrainExamples         (false)
{
}

    
    
/**
 *@brief Creates a new svm model from the provided image (example) data
 *@param[in] _name 
 *@param[in] _param Will make own locale copy.
 *@param[in] _examples The image data we will be building the model from
 *@param[in] _fileDesc A description of the data file.
 *@param[in] _log A logfile stream. All important events will be ouput to this stream
 */
Model::Model (const KKStr&          _name,
              const ModelParam&     _param,      // Create new model from
              FileDescPtr           _fileDesc,
              volatile const bool&  _cancelFlag,
              RunLog&               _log
             ):
    alreadyNormalized          (false),
    cancelFlag                 (_cancelFlag),
    classes                    (NULL),
    classesIndex               (NULL),
    compression_stats          (),
    crossClassProbTable        (NULL),
    encoder                    (NULL),
    fileDesc                   (_fileDesc),
    log                        (_log),
    name                       (_name),
    normParms                  (NULL),
    numOfClasses               (0),
    param                      (NULL),
    classProbs                 (NULL),
    rootFileName               (),
    trainExamples              (NULL),
    trianingPrepTime           (0.0),
    trainingTime               (0.0),
    trainingTimeStart          (0.0),
    validModel                 (true),
    votes                      (NULL),
    weOwnTrainExamples         (false)

{
  param = _param.Duplicate ();

  log.Level (20) << "ModelKnn::ModelKnn - Constructing From Training Data." << endl;
}




/**
 * @brief Frees any memory allocated by, and owned by the Model
 */
Model::~Model ()
{
  DeAllocateSpace ();

  delete  classesIndex;  classesIndex = NULL;
  delete  classes;       classes = NULL;
  delete  encoder;       encoder = NULL;
  delete  normParms;     normParms = NULL;

  if  (weOwnTrainExamples)
  {
    delete  trainExamples;
    trainExamples = NULL;
  }

  delete  param;
  param = NULL;
}


KKU::int32  Model::MemoryConsumedEstimated ()  const
{
  KKU::int32  memoryConsumedEstimated = sizeof (Model) + rootFileName.MemoryConsumedEstimated ();
  if  (classes)              memoryConsumedEstimated += classes->MemoryConsumedEstimated ();
  if  (classesIndex)         memoryConsumedEstimated += classesIndex->MemoryConsumedEstimated ();
  if  (classProbs)           memoryConsumedEstimated += numOfClasses * sizeof (double);
  if  (crossClassProbTable)  memoryConsumedEstimated += (numOfClasses * sizeof (double*)  +  numOfClasses * numOfClasses * sizeof (double));
  if  (encoder)              memoryConsumedEstimated += encoder->MemoryConsumedEstimated ();
  if  (normParms)            memoryConsumedEstimated += normParms->MemoryConsumedEstimated ();
  if  (param)                memoryConsumedEstimated += param->MemoryConsumedEstimated ();
  if  (votes)                memoryConsumedEstimated += numOfClasses * sizeof (int32);

  if  (weOwnTrainExamples  &&  (trainExamples != NULL))
    memoryConsumedEstimated += trainExamples->MemoryConsumedEstimated ();

  return  memoryConsumedEstimated;
}



KKStr  Model::Description ()  const
{
  return ModelTypeStr () + "(" + Name () + ")";
}



ModelPtr  Model::CreateFromStream (istream&              i,
                                   FileDescPtr           _fileDesc,
                                   volatile const bool&  _cancelFlag,
                                   RunLog&               _log
                                  )
{
  istream::pos_type startPos = i.tellg ();

  char  buff[20480];
  KKStr  ln;

  ModelTypes  modelType = mtNULL;


  // Fisrt we need to determine which type of model this is.  We will
  // scan through the file until we locate a ModelParamType field.
  while  (i.getline (buff, sizeof (buff)))
  {
    ln = buff;
    KKStr  lineName = ln.ExtractToken2 ("\t");

    if  (lineName.EqualIgnoreCase ("</Model>"))
    {
      // We did not find the parameter type 
      break;
    }

    if  (lineName.EqualIgnoreCase ("ModelType"))
    {
      KKStr  modelTypeStr = ln.ExtractToken2 ("\t");
      modelType = ModelTypeFromStr (modelTypeStr);
      if  (modelType == mtNULL)
      {
        _log.Level (-1) << endl
          << "ModelParam::CreateFromStream  ***ERROR***   Invalid ModelType[" << modelTypeStr << "]." << endl
          << endl;
      }
      break;
    }
  }

  if  (modelType == mtNULL)
  {
    // We never found the type of parameter we are looking for.
    _log.Level (-1) << endl
      << "Model::CreateFromStream   ***ERROR***   No Parameter Type was defined." << endl
      << endl;
    return NULL;
  }

  i.seekg (startPos);

  ModelPtr model = NULL;
  switch  (modelType)
  {
  case  mtOldSVM:    model = new ModelOldSVM    (_fileDesc, _cancelFlag, _log);
                     break;
 
  case  mtSvmBase:   model = new ModelSvmBase   (_fileDesc, _cancelFlag, _log);
                     break;

  case  mtKNN:       model = new ModelKnn       (_fileDesc, _cancelFlag, _log);
                     break;

  case  mtUsfCasCor: model = new ModelUsfCasCor (_fileDesc, _cancelFlag, _log);
                     break;

  case  mtDual:      model = new ModelDual      (_fileDesc, _cancelFlag, _log);
                     break;
  }

  if  (!model)
    return  NULL;

  bool  successful = false;
  try
  {
    model->ReadXML (i, successful);
  }
  catch  (const KKStrException& e)
  {
    _log.Level (-1) << endl << endl
      << "Model::CreateFromStream    ***ERROR***  Exception occured in executing 'ReadXML'" << endl
      << "      " << e.ToString ()  << endl
      << endl;
    successful = false;
    delete  model;
    model = NULL;
  }
  catch (...)
  {
    _log.Level (-1) << endl << endl
      << "Model::CreateFromStream    ***ERROR***  Exception occured in executing 'ReadXML'" << endl
      << endl;
    successful = false;
    delete  model;
    model = NULL;
  }

  if  (!successful)
  {
    _log.Level (-1) << endl << endl
      << "Model::CreateFromStream    ***ERROR***  Loading Model from file." << endl
      << endl;
    delete  model;
    model = NULL;
  }

  return  model;
}  /* CreateFromStream */




KKStr  Model::ModelTypeToStr (ModelTypes   _modelingType)
{
  if  (_modelingType == mtNULL)
    return "NULL";
  
  else if  (_modelingType == mtOldSVM)
    return "OldSVM";

  else if  (_modelingType == mtSvmBase)
    return "SvmBase";

  else if  (_modelingType == mtKNN)
    return "KNN";

  else if  (_modelingType == mtUsfCasCor)
    return "UsfCasCor";
  
  else if  (_modelingType == mtDual)
    return "Dual";
  
  else
    return "NULL";
}  /* ModelingMethodToStr */




Model::ModelTypes  Model::ModelTypeFromStr (const KKStr&  _modelingTypeStr)
{
  if  (_modelingTypeStr.EqualIgnoreCase ("OldSVM")  ||  _modelingTypeStr.EqualIgnoreCase ("One_Level"))
    return mtOldSVM;

  else if  (_modelingTypeStr.EqualIgnoreCase ("SvmBase"))
    return mtSvmBase;

  else if  (_modelingTypeStr.EqualIgnoreCase ("KNN"))
    return mtKNN;

  else if  (_modelingTypeStr.EqualIgnoreCase ("UsfCasCor"))
    return mtUsfCasCor;

  else if  (_modelingTypeStr.EqualIgnoreCase ("Dual"))
    return mtDual;

  else
    return mtNULL;
}  /* ModelingMethodFromStr */




ModelPtr  Model::CreateAModel (ModelTypes            _modelType,
                               const KKStr&          _name,
                               const ModelParam&     _param,  
                               FileDescPtr           _fileDesc,
                               volatile const bool&  _cancelFlag,
                               RunLog&               _log
                              )
{
  ModelPtr  model = NULL;
  try
  {
    switch  (_modelType)
    {
    case  mtOldSVM:    model = new ModelOldSVM    (_name, dynamic_cast<const ModelParamOldSVM&>    (_param), _fileDesc, _cancelFlag, _log);
                       break;

    case  mtSvmBase:   model = new ModelSvmBase   (_name, dynamic_cast<const ModelParamSvmBase&>   (_param), _fileDesc, _cancelFlag, _log);
                       break;

    case  mtKNN:       model = new ModelKnn       (_name, dynamic_cast<const ModelParamKnn&>       (_param), _fileDesc, _cancelFlag, _log);
                       break;

    case  mtUsfCasCor: model = new ModelUsfCasCor (_name, dynamic_cast<const ModelParamUsfCasCor&> (_param), _fileDesc, _cancelFlag, _log);
                       break;

    case  mtDual:      model = new ModelDual      (_name, dynamic_cast<const ModelParamDual&>      (_param), _fileDesc, _cancelFlag, _log);
                       break;
    }  /* end of switch */
  }
  catch  (const KKStrException&  e)
  {
    delete  model; model = NULL; throw  e;
  }
  catch  (const std::exception&  e)
  {
    delete  model;  model = NULL;
    throw  KKStrException ("Model::CreateAModel  Exception calling constructor.", e);
  }
  catch  (const char*  e2)
  {
    delete  model; model = NULL;
    KKStr  exceptionStr = "Model::CreateAModel  Exception calling constructor[";
    exceptionStr << e2 << "]."; 
    throw  KKStrException (exceptionStr);
  }
  catch  (...)
  {
    delete  model;  model = NULL;
    throw  KKStrException ("Model::CreateAModel  Exception calling constructor.  No info provided.");
  }

  return  model;
}  /*  CreateAModel */




void  Model::AllocatePredictionVariables ()
{
  int32  x = 0;
  int32  y = 0;

  DeAllocateSpace ();

  if  (classes == NULL)
  {
    log.Level (-1) << endl << endl 
      << "Model::AllocatePredictionVariables   ***ERROR***      (classes == NULL)"  << endl
      << endl;

    validModel = false;
    return;
  }

  numOfClasses = classes->QueueSize ();

  if  (numOfClasses > 0)
  {
    crossClassProbTableSize = numOfClasses;
    classProbs    = new double[numOfClasses];
    votes         = new int32 [numOfClasses];
    crossClassProbTable = new double*[numOfClasses];
    for  (x = 0;  x < numOfClasses;  x++)
    {
      classProbs         [x] = 0.0;
      votes              [x] = 0;
      crossClassProbTable[x] = new double[numOfClasses];
      for  (y = 0;  y < numOfClasses;  y++)
        crossClassProbTable[x][y] = 0.0f;
    }
  }
}  /* AllocatePredictionVariables*/





void  Model::DeAllocateSpace ()
{
  int32  x = 0;
  if  (crossClassProbTable)
  {
    for  (x = 0;  x < numOfClasses;  x++)
    {
      delete  [] crossClassProbTable[x];
      crossClassProbTable[x] = NULL;
    }
    delete[]  crossClassProbTable;
    crossClassProbTable = NULL;
  }

  delete[] classProbs;
  classProbs = NULL;

  delete[] votes;
  votes = NULL;
}



int32  Model::BitsToReduceBy () const
{
  if  (!param)
    throw KKStrException ("Model::BitsToReduceBy  'param == NULL'.");
  return param->BitsToReduceBy ();
}



const FeatureEncoder2&  Model::Encoder () const
{
  if  (!encoder)
    throw KKStrException ("Model::GetFeatureNums  'encoder == NULL'.");
  return *encoder;
}



const FeatureNumList&   Model::GetFeatureNums ()  const
{
  if  (!param)
    throw KKStrException ("Model::GetFeatureNums  'param == NULL'.");
  return  param->SelectedFeatures ();
}


bool  Model::NormalizeNominalAttributes ()  const
{
  if  (!param)
    throw KKStrException ("Model::NormalizeNominalAttributes  'param == NULL'.");

  if  (param->EncodingMethod () == ModelParam::NoEncoding)
    return  true;
  else
    return  false;
}  /* NormalizeNominalAttributes */



const FeatureNumList&  Model::SelectedFeatures () const
{
  if  (!param)
    throw KKStrException ("Model::GetFeatureNums  'param'.");
  return  param->SelectedFeatures ();
}



void  Model::TrainingTimeStart ()
{
  trainingTimeStart = osGetSystemTimeUsed ();
}



void  Model::TrainingTimeEnd ()
{
  double  trainingTimeEnd = osGetSystemTimeUsed ();
  trainingTime = trainingTimeEnd - trainingTimeStart;
  if  (trainingTime < 0.0)
    trainingTime += (24.0 * 60.0 * 60.0);
}




void  Model::Load (const KKStr&  _rootFileName,
                   bool&         _successful
                  )
{
  log.Level (10) << "Model::Load  Load Model in File[" << _rootFileName << "]." << endl;
  rootFileName = _rootFileName;
  KKStr  fileName = rootFileName + ".Model";

  ifstream  i (fileName.Str (), ios_base::in);
  if  (!i.is_open ())
  {
    log.Level (-1) << endl << endl << "Model::Load   Failed to open file[" << fileName << "]" << endl;
    _successful = false;
    return;
  }

  ReadXML (i, _successful);

  i.close ();
}  /* Load */




void  Model::Save (const KKStr&  _rootFileName,
                   bool&         _successful
                  )
{
  log.Level (10) << "Model::Save  Saving Model in File["
                 << _rootFileName << "]."
                 << endl;

  _successful = true;

  rootFileName = _rootFileName;

  KKStr  fileName = rootFileName + ".Model";

  ofstream  o (fileName.Str ());
  if  (!o.is_open ())
  {
    _successful = false;
    log.Level (-1) << endl << endl 
      << "Model::Save    ***ERROR***   Opening[" << fileName << "]." << endl;
    return;
  }

  WriteXML (o);

  o.close ();
}  /* Save */




void  Model::WriteXML (ostream&  o)
{
  log.Level (20) << "Model::WriteXML  Saving Model [" << Name () << "] in File." << endl;

  timeSaved = osGetLocalDateTime ();

  o << "<Model>" << endl;
  o << "ModelType" << "\t" << ModelTypeStr () << endl;

  o << "Name"               << "\t" << Name ()                                          << endl;
  o << "RootFileName"       << "\t" << rootFileName                                     << endl;
  o << "Classes"            << "\t" << classes->ToCommaDelimitedStr ()                  << endl;
  if  (classesIndex)
    o << "ClassesIndex"       << "\t" << classesIndex->ToCommaDelString ()              << endl;

  if  (param)
  {
    o << "<Parameters>"  << endl;
    param->WriteXML (o);
    o << "</Parameters>" << endl;
  }

  o << "TimeSaved"          << "\t" << timeSaved                                        << endl;
  o << "TrainingTime"       << "\t" << trainingTime                                     << endl;
  o << "CompressionStats"   << "\t" << compression_stats.ToTabDelString ().QuotedStr () << endl;
  o << "AlreadyNormalized"  << "\t" << (alreadyNormalized ? "Yes" : "No")               << endl;
  if  (normParms)
    normParms->Write (o);

  o << "<SpecificImplementation>"  << endl;
  WriteSpecificImplementationXML (o);
  o << "</SpecificImplementation>"  << endl;

  o << "</Model>" << endl;

} /* WriteXML */




void  Model::ReadXML (istream&  i,
                      bool&     _successful
                     )
{
  validModel = true;
  delete  normParms;
  normParms = NULL;
  _successful = true;

  delete  classesIndex;
  classesIndex = NULL;


  char  buff[40960];

  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);

    KKStr  field = ln.ExtractQuotedStr ("\n\r\t", true);
    field.Upper ();

    if  (field.Empty ())
      continue;

    if  (field.EqualIgnoreCase ("</Model>"))
      break;

    if  (field.EqualIgnoreCase ("CLASSES"))
    {
      delete  classes;
      classes = MLClassConstList::BuildListFromDelimtedStr (ln, ',');
      delete  classesIndex;
      classesIndex = new ClassIndexList (*classes);
    }

    else if  (field.EqualIgnoreCase ("ClassesIndex"))
    {
      delete  classesIndex;  classesIndex = NULL;
      classesIndex = new ClassIndexList ();
      classesIndex->ParseClassIndexList (ln);
    }

    else if  (field.EqualIgnoreCase ("Name"))
      name = ln.ExtractToken2 ("\n\r\t");

    else if  (field.EqualIgnoreCase ("TimeSaved"))
    {
      KKStr  timeSavedStr = ln.ExtractToken2 ("\t");
      timeSaved = DateTime (timeSavedStr);
    }

    else if  (field.EqualIgnoreCase ("<Parameters>"))
    {
      delete  param;
      param = NULL;

      try
      {
        param = ModelParam::CreateModelParam (i, fileDesc, log);
      }
      catch  (const exception&  e)
      {
        _successful = false;
        validModel = false;
        KKStr  errMsg;
        errMsg << "Exception executing fuction 'ModelParam::CreateModelParam'.  Exception[" << e.what () << "]";
        log.Level (-1) << endl << "Model::ReadXML    ***ERROR***    "  << errMsg << endl << endl;
        throw KKStrException (errMsg);
      }

      if  (!param)
      {
        _successful = false;
        validModel = false;
        KKStr  errMsg = "Model::ReadXML    ***ERROR***    (param == NULL)";
        log.Level (-1) << errMsg << endl;
      }

      else if  (!param->ValidParam ())
      {
        _successful = false;
        validModel = false;
        log.Level (-1) << endl << endl << "Model::ReadXML   ***ERROR***   <ModelParam>  was invalid." << endl << endl;
      }
    }

    else if  (field == "TRAININGTIME")
      trainingTime = ln.ExtractTokenDouble ("\n\r\t");

    else if  (field.EqualIgnoreCase ("COMPRESSIONSTATS"))
      compression_stats.UpdateFromTabDelStr (ln.ExtractQuotedStr ("\n\r\t", true));

    else if  (field.EqualIgnoreCase ("AlreadyNormalized"))
      alreadyNormalized = ln.ExtractTokenBool ("\t");

    else if  (field.EqualIgnoreCase ("<NormalizationParms>"))
    {
      delete  normParms;
      normParms = NULL;
      _successful = true;
      normParms = new NormalizationParms (fileDesc, i, _successful, log);
      if  (!_successful)
      {
        KKStr  errMsg = "Model::ReadXML    ***ERROR***     Reading in <NormalizationParms>";
        log.Level (-1) << endl << endl
                       << errMsg << endl
                       << endl;
        throw KKStrException (errMsg);
      }
    }
    else if  (field.EqualIgnoreCase ("<SpecificImplementation>"))
    {
      ReadSpecificImplementationXML (i, _successful);
    }
  }

  if  (classes == NULL)
  {
    _successful = false;
    log.Level (-1) << endl << endl 
                   << "Model::ReadXML    Class List was not defined." << endl
                   << endl;
  }

  else if  ((normParms == NULL)  &&  (!alreadyNormalized))
  {
    _successful = false;
    log.Level (-1) << endl << endl 
                   << "Model::ReadXML    Normalization Parmameters was not defined." << endl
                   << endl;
  }

  else
  {
    numOfClasses = classes->QueueSize ();
    AllocatePredictionVariables ();
  }

  log.Level (10) << "Model::ReadXML   Model[" << Name () << "]  Done Loading." << endl;
}  /* ReadXML */





void  Model::ReadSkipToSection (istream& i, 
                                KKStr    sectName,
                                bool&    sectionFound
                               )
{
  sectionFound = false;
  char  buff[10240];
  sectName.Upper ();

  KKStr  field;

  // Skip to start of OneVsOne section
  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln (buff);
    field = ln.ExtractQuotedStr ("\n\r\t", true);
    field.Upper ();
    if  (field == sectName)
    {
      sectionFound = true;
      break;
    }
  }

  if  (!sectionFound)
  {
    log.Level (-1) << endl << endl
                   << "Model::ReadSkipToSection    *** Could not find section[" << sectName << "]" <<endl
                   << endl;
  }
}  /* ReadSkipToSection */




/**
 *@brief Performs operations such as BitReduction, FeatureEncoding, and  Normailization.  The actual training 
 *       of models occurs in the specific implementation of 'Model'.
 */
void  Model::TrainModel (FeatureVectorListPtr  _trainExamples,
                         bool                  _alreadyNormalized,
                         bool                  _takeOwnership  /*!< True = Model will take ownership of these examples */
                        )
{
  log.Level (10) << "Model::TrainModel   Preparing for training of Model[" << Name () << "]  Examples[" << _trainExamples->QueueSize () << "]" << endl;

  double  prepStartTime = osGetSystemTimeUsed ();

  if  (_trainExamples == NULL)
  {
    KKStr  errMsg = "ModelSvmBase::TrainModel   (_trainExamples == NULL)";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  if  (_trainExamples->QueueSize () < 2)
  {
    KKStr  errMsg = "ModelSvmBase::TrainModel   (_trainExamples == NULL)";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  weOwnTrainExamples = _takeOwnership;
  trainExamples      = _trainExamples;
  alreadyNormalized  = _alreadyNormalized;
  _trainExamples     = NULL;

  delete  classes;
  classes = trainExamples->ExtractMLClassConstList ();
  classes->SortByName ();

  delete  classesIndex;
  classesIndex = new ClassIndexList (*classes);


  if  (param->ExamplesPerClass () < int32_max)
    ReduceTrainExamples ();

  if  (!_alreadyNormalized)
  {
    if  (!weOwnTrainExamples)
    {
      // Since we do not own the We will have to duplicate the trainExamples list and its contents before we normalize the data.
      FeatureVectorListPtr  temp = new FeatureVectorList (*trainExamples, true);
      weOwnTrainExamples = true;
      trainExamples = temp;
    }
    else if  (!trainExamples->Owner ())
    {
      // Even though we own 'trainExamples' we do not own its contents.  So we will need to create a new list and own those contents.
      FeatureVectorListPtr  temp = new FeatureVectorList (*trainExamples, true);
      weOwnTrainExamples = true;
      delete  trainExamples;
      trainExamples = temp;
    }
    delete  normParms;
    normParms = new NormalizationParms (*param, *trainExamples, log);
    normParms->NormalizeImages (trainExamples);
  }

  if  ((param->EncodingMethod     () == ModelParam::Encoding_NULL)  &&
       ((param->CompressionMethod () == ModelParam::BRnoCompression)  ||  (param->CompressionMethod () == ModelParam::BRNull))
      )
  {
    // There is nothing for us to do.
    return;
  }

  if  ((param->CompressionMethod () != ModelParam::BRnoCompression)  &&  (param->CompressionMethod () != ModelParam::BRNull))
  {
    if  (!encoder)
      encoder = new FeatureEncoder2 (*param, fileDesc, log);

    FeatureVectorListPtr  compData = NULL;
    compression_stats = encoder->CompressExamples (trainExamples, compData);
    if  (weOwnTrainExamples)
    {
      delete  trainExamples;
      trainExamples = NULL;
    }
    trainExamples = compData;
    weOwnTrainExamples = true;
  }

  if  ((param->EncodingMethod () != ModelParam::Encoding_NULL)  &&   (param->EncodingMethod () != ModelParam::NoEncoding))
  {
    if  (!encoder)
      encoder = new FeatureEncoder2 (*param, fileDesc, log);

    FeatureVectorListPtr  encodedImages = encoder->EncodeAllExamples (trainExamples);
    if  (weOwnTrainExamples)
    {
      delete  trainExamples;
      trainExamples = NULL;
    }

    trainExamples = encodedImages;
    weOwnTrainExamples = true;
  }

  AllocatePredictionVariables ();

  double  prepEndTime = osGetSystemTimeUsed ();
  trianingPrepTime = prepEndTime - prepStartTime;
  if  (trianingPrepTime < 0.0)
    trianingPrepTime += (24.0 * 60.0 * 60.0);
}  /* TrainModel */





/**
 *@brief  Every prediction  method in every class that is inherited from this class should call
 *        this method before performing there prediction.  Such things as Normalization and
 *        Feature Encoding.
 *@param[in]  fv  Feature vector of example that needs to be prepared.
 *@param[out]  newExampleCreated  Indicates if either Feature Encoding and/or Normalization needed
 *             to be done.  If neither then the original instance is returned.  If Yes then 
 *             a new instance which the caller will have to be delete will be returned.
 */
FeatureVectorPtr  Model::PrepExampleForPrediction (FeatureVectorPtr  fv,
                                                   bool&             newExampleCreated
                                                  )
{
  FeatureVectorPtr  oldFV = NULL;
  newExampleCreated = false;
  if  ((!alreadyNormalized)  &&  (normParms))
  {
    oldFV = fv;
    fv = normParms->ToNormalized (fv);
    if  (newExampleCreated)
    {
      delete  oldFV;
      oldFV = NULL;
    }
    newExampleCreated = true;
  }

  // I do not believe we need the encoder at this point. At least not for the Features Selected part.  Maybe the conversion from niminal fields will make sense.
  if  (encoder)
  {
    oldFV = fv;
    fv = encoder->EncodeAExample (fv);
    if  (newExampleCreated)
    {
      delete  oldFV;
      oldFV = NULL;
    }
    newExampleCreated = true;
  }

  return  fv;
}  /* PrepExampleForPrediction */



/**
 @brief  Reduces the Training Images down to the size dictated by the 'examplesPerClass' parameter.
 */
void  Model::ReduceTrainExamples ()
{
  int32   examplesPerClass = param->ExamplesPerClass ();
  uint32  zed = 0;

  if  (examplesPerClass < 0)
    examplesPerClass = int32_max;

  bool  reductionNeeded = false;

  {
    // First lets see if reduction is even nessasary.
    ClassStatisticListPtr  stats = trainExamples->GetClassStatistics ();
    if  (!stats)
    {
      log.Level (-1) << endl
        << "Model::ReduceTrainExamples   ***ERROR***  can not retrieve Class Stats for training data." << endl
        << endl;
      validModel = false;
      return;
    }

    for  (zed = 0;  (zed < stats->size ())  &&  (!reductionNeeded);  zed++)
    {
      if  (stats->IdxToPtr (zed)->Count () > (uint32)examplesPerClass)
        reductionNeeded  = true;
    }

    delete  stats;
    stats = NULL;
  }

  if  (!reductionNeeded)
  {
    log.Level (20) << "Model::ReduceTrainExamples    Was not needed.  No classes exceeded 'examplesPerClass'." << endl;
    return;
  }

  FeatureVectorListPtr  reducedSet = new FeatureVectorList (fileDesc, false, log);
  FeatureVectorListPtr  deleteSet  = new FeatureVectorList (fileDesc, false, log);  // Examples taht we do not use will need to be deleted.
  MLClassConstList::iterator  idx;

  for  (idx = classes->begin ();  idx != classes->end ();  idx++)
  {
    MLClassConstPtr  ic = *idx;
    FeatureVectorListPtr  examplesThisClass = trainExamples->ExtractImagesForAGivenClass (ic);
    if  ((!examplesThisClass)  ||  (examplesThisClass->size () < 1))
    {
      log.Level (-1) << endl
        << "Model::ReduceTrainExamples   ***ERROR***   No Training Examples for class[" << ic->Name () << "]." << endl
        << endl;
      continue;
    }

    if  (examplesThisClass->size () <= (uint32)examplesPerClass)
    {
      reducedSet->AddQueue (*examplesThisClass);
    }
    else
    {
      examplesThisClass->RandomizeOrder ();
      zed = 0;
      while  (zed < (uint32)examplesPerClass)
      {
        reducedSet->PushOnBack (examplesThisClass->IdxToPtr (zed));
        zed++;
      }
      while  (zed < examplesThisClass->size ())
      {
        deleteSet->PushOnBack (examplesThisClass->IdxToPtr (zed));
        zed++;
      }
    }

    delete  examplesThisClass;
    examplesThisClass = NULL;
  }

  if  (weOwnTrainExamples)
  {
    trainExamples->Owner (false);
    delete trainExamples;
    trainExamples = reducedSet;
    reducedSet = NULL;

    trainExamples->Owner (true);
    deleteSet->Owner (true);
    delete  deleteSet;
    deleteSet = NULL;
  }
  else
  {
    // Since we are replacing 'trainExamples' with 'reducedSet' we will now own 'trainExamples' buit not its contents.
    reducedSet->Owner (false);
    weOwnTrainExamples = true;
    trainExamples = reducedSet;
    reducedSet = NULL;
    deleteSet->Owner (false);
    delete  deleteSet;
    deleteSet = NULL;
  }
}  /* ReduceTrainExamples */




void  Model::RetrieveCrossProbTable (MLClassConstList&   classes,
                                     double**               crossClassProbTable  // two dimension matrix that needs to be classes.QueueSize ()  squared.
                                    )
{
  if  (classes.QueueSize () != crossClassProbTableSize)
  {
    // There Class List does not have the same number of entries as our 'CrossProbTable'
    log.Level (-1) << endl
                   << "SVMModel::RetrieveCrossProbTable    ***ERROR***" << endl
                   << "            classes.QueueSize ()[" << classes.QueueSize () << "] != crossClassProbTableSize[" << crossClassProbTableSize << "]" << endl
                   << endl;
    return;
  }

  int32*  indexTable = new int32[classes.QueueSize ()];
  int32   x, y;
  for  (x = 0;  x < classes.QueueSize ();  x++)
  {
    for  (y = 0;  y < classes.QueueSize ();  y++)
       crossClassProbTable[x][y] = 0.0;

    indexTable[x] = classesIndex->GetClassIndex (classes.IdxToPtr (x));
    if  (indexTable[x] < 0)
    {
      log.Level (-1) << endl << endl
                     << "SVMModel::RetrieveCrossProbTable      ***WARNING***" << endl
                     << endl
                     << "      Class Index[" << x << "]  Name[" << classes[x].Name () << "]" << endl
                     << "      will populate this index with zeros."                         << endl
                     << endl;
    }
  }

  if  (classes.QueueSize () != crossClassProbTableSize)
  {
    log.Level (-1) << endl
                   << "SVMModel::RetrieveCrossProbTable      ***ERROR***"                                       << endl
                   << "                                     'classes.QueueSize () != crossClassProbTableSize'"  << endl
                   << endl;
    return;
  }


  // x,y         = 'Callers'   Class Indexes..
  // xIdx, yIdx  = 'SVMNodel'  Class Indexed.
  for  (x = 0;  x < classes.QueueSize ();  x++)
  {
    int32 xIdx = indexTable[x];
    if  (xIdx >= 0)
    {
      for  (y = 0;  y < classes.QueueSize ();  y++)
      {
        int32  yIdx = indexTable[y];
        if  (yIdx >= 0)
        {
          if  ((x != xIdx)  ||  (y != yIdx))
          {
            //kak  I just added this check to see when this situation actually occurs.
            int32  zed = 111;
          }

          crossClassProbTable[x][y] = this->crossClassProbTable[xIdx][yIdx];
        }
      }
    }
  }

  delete  indexTable;  indexTable = NULL;
  return;
}  /* RetrieveCrossProbTable */



void  Model::ProbabilitiesByClassDual (FeatureVectorPtr   example,
                                       KKStr&             classifier1Desc,
                                       KKStr&             classifier2Desc,
                                       ClassProbListPtr&  classifier1Results,
                                       ClassProbListPtr&  classifier2Results
                                      )
{
  delete classifier1Results;  classifier1Results = NULL;
  delete classifier2Results;  classifier2Results = NULL;

  classifier1Desc = Description ();
  classifier2Desc = Description ();

  classifier1Results = ProbabilitiesByClass (example);
  if  (classifier1Results)
    classifier2Results = new ClassProbList (*classifier1Results);
}  /* ProbabilitiesByClassDual */


