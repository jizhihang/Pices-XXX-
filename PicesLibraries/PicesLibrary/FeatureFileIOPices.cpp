#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>

#include  <fstream>
#include  <iostream>
#include  <map>
#include  <string>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include "BasicTypes.h"
#include "DateTime.h"
#include "ImageIO.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;

#include "InstrumentDataFileManager.h"
using namespace  SipperHardware;

#include "FeatureFileIOPices.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "MLClassConstList.h"
#include "ImageFeatures.h"
#include "InstrumentData.h"
using namespace MLL;


FeatureFileIOPices  FeatureFileIOPices::driver;



#ifdef  IMPLEMENTSAMPLING
#define  MAX_NUM_OF_FEATURES  177      // Version 32
#else
//#define  MAX_NUM_OF_FEATURES  125    // Version 37
//#define  MAX_NUM_OF_FEATURES  60     // Version 40
//#define  MAX_NUM_OF_FEATURES  62       // Version 41
//#define  MAX_NUM_OF_FEATURES  70       // Version 43  Added 8 new Intensity Histogram Features
//#define  MAX_NUM_OF_FEATURES  74       // Version 43  Added 8 new Intensity Histogram Features
#define  MAX_NUM_OF_FEATURES  88       // Version 46  Added 8 new Intensity Histogram Features
#endif



FeatureFileIOPices::FeatureFileIOPices ():
   FeatureFileIO ("Pices",
                  true,      // true = _canRead,
                  true      // true = _canWrite,
                 )                
{
}



FeatureFileIOPices::~FeatureFileIOPices ()
{
}


void  FeatureFileIOPices::Parse_FEATURE_DATA_FILE_Line (KKStr&  line,
                                                        int32&  version,
                                                        int32&  numOfFields,
                                                        int32&  numOfExamples
                                                       )
{
  KKStr  field = line.ExtractToken (" ,\t\n\r");  // Skip past FEATURE_DATA_FILE field.
  KKStr  fieldValue;
  field = line.ExtractToken (" ,\t\n\r").ToUpper ();
  while  (!field.Empty ())
  {
    fieldValue = line.ExtractToken (" ,\t\n\r");

    if  (field == "VERSION")
    {
      version = atoi (fieldValue.Str ());
    }

    else if  (field == "NUMOFFEATURES")
    {
      numOfFields = atoi (fieldValue.Str ());
    }

    else if  (field == "NUMOFIMAGES")
    {
      numOfExamples = atoi (fieldValue.Str ());
    }

    field = line.ExtractToken (" ,\t\n\r").ToUpper ();
  }
}  /* Parse_FEATURE_DATA_FILE_Line */




VectorInt  FeatureFileIOPices::CreateIndirectionTable (const VectorKKStr&  fields,
                                                       int32               numOfFeatures
                                                      )
{
  VectorInt   indirectionTable;
  int32  fieldNum = 0;

  for  (fieldNum = 0;  fieldNum < numOfFeatures;  fieldNum++)
    indirectionTable.push_back (rfFeatureData);

  for  (fieldNum = numOfFeatures;  fieldNum < (int32)fields.size ();  fieldNum++)
  {
    KKStr  fieldName = fields[fieldNum].ToLower ();
    if  (fieldName == "classname")
      indirectionTable.push_back (rfClassName);

    else if  (fieldName == "classnameidx")
      indirectionTable.push_back (rfMLClassIDX);

    else if  (fieldName == "filename") 
      indirectionTable.push_back (rfImageFileName);

    else if  (fieldName == "origsize") 
      indirectionTable.push_back (rfOrigSize);

    else if  (fieldName == "edgepixels") 
      indirectionTable.push_back (rfNumOfEdgePixels);

    else if  (fieldName == "probability") 
      indirectionTable.push_back (rfProbability);

    else if  (fieldName == "disttoborder") 
      indirectionTable.push_back (rfUnKnown);

    else if  (fieldName == "centroidrow")
      indirectionTable.push_back (rfCentroidRow);

    else if  (fieldName == "centroidcol")
      indirectionTable.push_back (rfCentroidCol);

    else if  (fieldName == "predictedclass")
      indirectionTable.push_back (rfPredictedClass);

    else if  (fieldName == "predictedclassidx")
      indirectionTable.push_back (rfPredictedClassIDX);

    else if  (fieldName == "breaktie")
      indirectionTable.push_back (rfBreakTie);

    else if  (fieldName == "sfcentroidrow")
      indirectionTable.push_back (rfSfCentroidRow);

    else if  (fieldName == "sfcentroidcol")
      indirectionTable.push_back (rfSfCentroidCol);

    else if  (fieldName == "laditude")
      indirectionTable.push_back (rfLaditude);

    else if  (fieldName == "longitude")
      indirectionTable.push_back (rfLongitude);

    else if  (fieldName == "depth")
      indirectionTable.push_back (rfDepth);

    else
      indirectionTable.push_back (rfUnKnown);

  }

  return  indirectionTable;
}  /* CreateIndirectionTable */






FileDescPtr  FeatureFileIOPices::GetFileDesc (const KKStr&            _fileName,
                                              istream&                _in,
                                              MLClassConstListPtr  _classes,
                                              int32&                  _estSize,
                                              KKStr&                  _errorMessage,
                                              RunLog&                 _log
                                             )
{
  char  buff[102400];

  _errorMessage = "";
  
  _in.getline (buff, sizeof (buff) - 1, '\n');
  KKStr  firstLine (buff);
  firstLine.Upper ();

  int32  numOfFields = 0;
  if  (firstLine.SubStrPart (0, 16) != "FEATURE_DATA_FILE")
  {
    // We are looking at a very old RAW file,  need to scan for number of fields.
    // Lets count the number of fields in each lime,  
    _log.Level (-1) << endl << endl 
                    << "FeatureFileIOPices::GetFileDesc    ***ERROR***    the first line must contain" << endl
                    << "                                                  'FEATURE_DATA_FILE'" << endl
                    << endl;
    _errorMessage = "The first line must contain FEATURE_DATA_FILE.";
    return NULL;
  }

  int32  version       = -1;
  int32  numOfFeatures = -1;
  int32  numOfExamples = -1;
  Parse_FEATURE_DATA_FILE_Line (firstLine, version, numOfFeatures, numOfExamples);
   if  (version < 39)
  {
    _log.Level (-1) << endl << endl 
                    << "FeatureFileIOPices::GetFileDesc    ***ERROR***"  << endl
                    << endl
                    << "                     Version Number[" << version << "]" << endl
                    << "                     Can not load versions prior to 39." << endl
                    << endl;
    _errorMessage << "Version[" << version << "]  is prior to '39'.";
    return NULL;
  }

  _in.getline (buff, sizeof (buff) - 1);
  KKStr  fieldDescLn (buff);
  VectorKKStr  fields = fieldDescLn.Split ("\n\r\t");
  if  (fields.size () < (uint32)numOfFeatures)
  {
    _log.Level (-1) << endl << endl << endl
                    << "FeatureFileIOPices::GetFileDesc     **** Error ***"  << endl
                    << endl
                    << "               Num fields[" << (uint32)fields.size () << "]  is less than the number specified." << endl
                    << endl
                    << endl;
    _errorMessage << "Num fields[" << (uint32)fields.size () << "]  is less than the number specified." << endl;
    return NULL;
  }
  
  FileDescPtr  fd = new FileDesc ();
  fd->Version (version);

  bool  alreadyExists;
  int32  featureNum = 0;
  for  (featureNum = 0;  featureNum < numOfFeatures;  featureNum++)
  {
    KKStr&  fieldName = fields[featureNum];
    fd->AddAAttribute (fieldName, NumericAttribute, alreadyExists);
    if  (alreadyExists)
    {
      _log.Level (-1) << endl << endl 
                      << "FeatureFileIOPices::GetFileDesc    ***ERROR***"  << endl
                      << endl
                      << "                     Duplicate Attribute Name[" << fieldName << "]" << endl
                      << endl;
      _errorMessage << "Duplicate Attribute Name[" << fieldName << "]";

      // Can not delete an instance of a 'FileDesc' class once it has been created.
      // delete  fd;
      return NULL;
    }
  }

  return  fd;
}  /* GetFileDesc */





ImageFeaturesListPtr  FeatureFileIOPices::LoadFile (const KKStr&          _fileName,
                                                    const FileDescPtr     _fileDesc,
                                                    MLClassConstList&  _classes, 
                                                    istream&              _in,
                                                    long                  _maxCount,    // Maximum # images to load,  less than '0'  indicates all.
                                                    volatile const bool&  _cancelFlag,
                                                    bool&                 _changesMade,
                                                    KKStr&                _errorMessage,
                                                    RunLog&               _log
                                                   )
{
  _log.Level (10) << "FeatureFileIOPices::LoadFile   Loading file[" << _fileName << "]." << endl;
  _log.Flush ();

  InstrumentDataFileManager::InitializePush ();

  VectorInt  featureFieldIndexTable;

  char buff[20480];

  _in.getline (buff, sizeof (buff) - 1);
  KKStr  firstLine (buff);

  firstLine.Upper ();

  if  (firstLine.SubStrPart (0, 16) != "FEATURE_DATA_FILE")

  {
    _log.Level (-1) << endl << endl << endl 
                    << "FeatureFileIOPices::LoadFile     **** Error missing 'FEATURE_DATA_FILE' line ****    can not load" << endl
                    << endl;
    _errorMessage = "Error missing 'FEATURE_DATA_FILE' line";
    return  NULL;
  }

  int32  version         = -1;
  int32  numOfFeatures   = -1;
  int32  numOfExamples   = -1;
  Parse_FEATURE_DATA_FILE_Line (firstLine, version, numOfFeatures, numOfExamples);
  if  (version < 38)
  {
    _log.Level (-1) << endl << endl << endl
                   << "FeatureFileIOPices::LoadFile     ***Error***    File version[" << version << "]  prior to 38.   can not load."
                   << endl
                   << endl;
    _errorMessage << "File version[" << version << "]  prior to 38.   can not load.";
    return  NULL;
  }
  

  // The second line will have a list of fields;  we will use ths line to build
  // an indirection table.
  _in.getline (buff, sizeof (buff) - 1);
  KKStr  fieldDescLine (buff);
  VectorKKStr  fields = fieldDescLine.Parse ("\n\r\t");
  if  ((int32)fields.size () < numOfFeatures)
  {
    _log.Level (-1) << endl << endl << endl
                   << "FeatureFileIOPices::LoadFile     **** Error ***"  << endl
                   << endl
                   << "               Num fields[" << (uint32)fields.size () << "]  is less than the number specified." << endl
                   << endl
                   << endl;
    _errorMessage << "Num fields[" << (uint32)fields.size () << "]  is less than the number specified." << endl;
    return  NULL;
  }

  VectorInt   indirectionTable = CreateIndirectionTable (fields, numOfFeatures);

  ImageFeaturesListPtr examples = new ImageFeaturesList (_fileDesc, 
                                                         true,          // true=Owner  examples will own the ImageFeatures instances it contains.
                                                         _log,
                                                         numOfExamples
                                                        );

  examples->Version (version);

  int32  fieldNum = 0;

  bool  eof = false;
  bool  eol = false;

  KKStr  className;
  char   firstChar;
  int32    lineCount = 0;

  FFLOAT  value;

  int32  lineNum = 0;

  eof = false;
  eol = false;

  KKStr field (128);

  GetToken (_in, ",\t", field, eof, eol);
  while  ((!eof)   &&  (!_cancelFlag)   &&  (((int32)examples->size () < _maxCount)  ||  (_maxCount < 0)))
  {
    if  (eol)
    {
      // We must have a blank line.  There is nothing to do in this case.
    }

    else if  (field.SubStrPart (0, 1) == "//")
    {
      // We are looking at a comment line.  Will skip to end of line
      while  ((!eol)  &&  (!eof))
        GetToken (_in, ",\t", field, eof, eol);
    }
    else
    {
      // We have an example.
      fieldNum  = 0;
      ImageFeaturesPtr  example = new ImageFeatures (numOfFeatures);
      example->Version (version);

      while  ((!eol)  &&  (!eof))
      {
        field.TrimRight ();

        if  (fieldNum >= (int32)indirectionTable.size ())
        {
          _log.Level (-1) << endl << endl << endl
                          << "FeatureFileIOPices::LoadFile     **** Error ***"  << endl
                          << endl
                          << "               LineNum[" << lineNum << "]  has to many fields defined." << endl
                          << endl
                          << endl;
          _errorMessage << "LineNum[" << lineNum << "]  has to many fields defined.";
          delete  examples;
          delete  example;
          return NULL;
        }
      
        switch  (indirectionTable[fieldNum])
        {
        case rfFeatureData:
          firstChar = field.FirstChar ();
          if  (((firstChar < '0')  ||  (firstChar > '9'))  &&  (firstChar != '-'))
          {
            KKStr  temp = field.ToUpper ();
            if  ((temp == "INFINITY")  || (temp == "NAN"))
            {
              example->AddFeatureData (fieldNum,  FFLOAT_MAX);
            }
            else
            {
              _log.Level (-1) << endl << endl << endl
                              << "FeatureFileIOPices::LoadFile     **** Error ***"  << endl
                              << endl
                              << "               Invalid Feature Field[" << fieldNum << "]  on LineNum[" << lineNum << "]." << endl
                              << endl
                              << endl;
              _errorMessage << "Invalid Feature Field[" << fieldNum << "]  on LineNum[" << lineNum << "].";
              delete  examples;
              delete  example;
              return NULL;
            }
          }

          else
          {
            value = (FFLOAT)atof (field.Str ());
            example->AddFeatureData (fieldNum,  value);
          }
          break;

        case rfClassName: 
          className = field;
          if  (className.Empty ())
             className = "UnKnown";
          example->MLClass (_classes.GetMLClassPtr (className));
          break;

        case rfMLClassIDX:
          // Ignore this field.
          break;

        case rfImageFileName: 
          example->ImageFileName (field);
          break;

        case rfOrigSize: 
          example->OrigSize ((FFLOAT)atof (field.Str ()));
          break;

        case rfNumOfEdgePixels: 
          example->NumOfEdgePixels (atoi (field.Str ()));
          break;

        case rfProbability: 
          example->Probability ((float)atof (field.Str ()));
          break;

        case rfCentroidRow: 
          example->CentroidRow((FFLOAT)atof (field.Str ()));
          break;

        case rfCentroidCol: 
          example->CentroidCol ((FFLOAT)atof (field.Str ()));
          break;

        case rfPredictedClass: 
          if  (!field.Empty ())
          example->PredictedClass (_classes.GetMLClassPtr (field));
          break;
  
        case rfPredictedClassIDX:
          // We ignore this field
          break;

        case rfBreakTie: 
          example->BreakTie  ((float)atof (field.Str ()));
          break;

        case rfSfCentroidRow: 
          example->SfCentroidRow ((double)atof (field.Str ()));
          break;

        case rfSfCentroidCol:
          example->SfCentroidCol ((float)atof (field.Str ()));
          break;

        case rfLaditude:
          example->Latitude (atof (field.Str ()));
          break;

        case rfLongitude:
          example->Longitude ((float)atof (field.Str ()));
          break;

        case rfDepth:
          example->Depth ((float)atof (field.Str ()));
          break;

        case rfUnKnown:
          // ignore this field.
          break;
                  
        default:
          _log.Level(-1) << endl << endl << endl
                         << "FeatureFileIOPices::LoadFile   *** Undefined File Format Specified." << endl
                         << endl;
          break;
        }

        GetToken (_in, ",\t", field, eof, eol);
        fieldNum++;
      }

      if  (example->OrigSize () <= 0)
        example->OrigSize (example->FeatureData (0));

      if  (example->ImageFileName ().Empty ())
      {
        KKStr  imageFileName = _fileName + "_" + StrFormatInt (lineCount, "ZZZZZZ0");
        example->ImageFileName (imageFileName);
      }

      if  (example->ImageFileName ().Empty ())
        example->ImageFileName (osGetRootName (_fileName) + "_" + StrFormatInt (lineCount, "000000"));


      if  (numOfFeatures  >= MaxNumPlanktonRawFields)
      {
        // We only want to look up Instrumentation data if we are dealing with a current Pices Feature file.
        // otherwise we will end up with invalid indexes.
        if  ((example->Depth () == 0.0f)  ||  (example->Salinity () == 0.0f))
        {
          // Since these fields seem to be missing we will need to go look them up in the
          // instruments data file.
  
          InstrumentDataPtr  id = NULL;
          try
          {
            id = InstrumentDataFileManager::GetClosestInstrumentData (example->ImageFileName (), _cancelFlag, _log);
          }
          catch  (...)
          {
            id = NULL;
          }
          if  (id)
          {
            _changesMade = true;
            example->Depth        (id->Depth        ());
            example->Oxygen       (id->Oxygen       ());
            example->Salinity     (id->Salinity     ());
            example->Fluorescence (id->Fluorescence ());
            example->FlowRate1    (id->FlowRate1    ());
            example->FlowRate2    (id->FlowRate2    ());
          }
        }
      }

      if  (example->PredictedClass () == NULL)
        example->PredictedClass (example->MLClass ());

      examples->PushOnBack (example);

      lineNum++;
    }

    GetToken (_in, ",\t", field, eof, eol);
  }

  InstrumentDataFileManager::InitializePop ();

  _log.Level (10) << "LoadFile - Loaded [" << (int32)examples->size () << "] images from file[" << _fileName << "]." << endl;

  return  examples;
}  /* LoadFile */






void   FeatureFileIOPices::SaveFile (FeatureVectorList&     _data,
                                     const KKStr&           _fileName,
                                     const FeatureNumList&  _selFeatures,
                                     ostream&               _out,
                                     uint32&                _numExamplesWritten,
                                     volatile const bool&   _cancelFlag,
                                     bool&                  _successful,
                                     KKStr&                 _errorMessage,
                                     RunLog&                _log
                                    )
{
  ImageFeaturesListPtr  examples  = NULL;

  _numExamplesWritten = 0;

  bool  weOwnImages = false;

  if  (strcmp (_data.UnderlyingClass (), "ImageFeatures") == 0)
  {
    examples = dynamic_cast<ImageFeaturesListPtr>(&_data);
  }
  else
  {
    weOwnImages = true;
    examples = new ImageFeaturesList (_data, true);
  }

  const FileDescPtr  fileDesc = _data.FileDesc ();

  ClassStatisticListPtr  classStatistics = examples->GetClassStatistics ();

  int32  fileVersionNum = examples->Version ();
  if  (fileVersionNum < 1)
  {
    fileVersionNum = CurrentFeatureFileVersionNum;
    examples->Version (fileVersionNum);
  }


  ClassIndexList  classIdx;

  _out << "FEATURE_DATA_FILE"                                                << "\t"  
       << "Version"        << "\t" << StrFormatInt (fileVersionNum, "ZZZ0")  << "\t" 
       << "NumOfFeatures"  << "\t" << _selFeatures.NumOfFeatures ()          << "\t" 
       << "NumOfImages"    << "\t" << examples->QueueSize ()
       << endl;

  {
    int32  idx;
    for  (idx = 0;  idx < _selFeatures.NumOfFeatures ();  idx++)
    { 
      int32  featureNum = _selFeatures[idx];
      _out << fileDesc->FieldName (featureNum) << "\t";
    }
 
    _out << "ClassName"         << "\t"
         << "ClassNameIdx"      << "\t"
         << "FileName"          << "\t"
         << "OrigSize"          << "\t"
         << "EdgePixels"        << "\t"
         << "Probability"       << "\t"
         << "CentroidRow"       << "\t"
         << "CentroidCol"       << "\t"
         << "PredictedClass"    << "\t"
         << "PredictedClassIdx" << "\t"
         << "BreakTie"          << "\t"
         << "SfCentroidRow"     << "\t"
         << "SfCentroidCol"     << "\t"
         << "Laditude"          << "\t"
         << "Longitude"
         << endl;
  }

  {
    // Write out Class Statistics
    int32  seqNum = 0;
    ClassStatisticList::iterator  idx;
    for  (idx = classStatistics->begin ();  idx != classStatistics->end ();  idx++)
    {
      ClassStatisticPtr  cs = *idx;
      _out << "//" << cs->Name () << "\t" 
           <<         seqNum      << "\t"
           <<         cs->Count ()
           << endl;

      bool  dupEntry = false;
      classIdx.AddClassIndexAssignment (cs->MLClass (), seqNum, dupEntry);
      seqNum++;
    }
  }

  {
    // Write out the actual examples.
    ImageFeaturesPtr   example = NULL;
    for  (int32 idx = 0; idx < (int32)examples->size (); idx++)
    {
      example = examples->IdxToPtr (idx);

      int32  x;  
      for  (x = 0; x < _selFeatures.NumOfFeatures (); x++)
      {
        int32  featureNum = _selFeatures[x];
        _out << example->FeatureData (featureNum) << "\t";
      }

      {
        int32  mlClassIdx = classIdx.GetClassIndex (example->MLClass        ());
        int32  predClassIdx  = classIdx.GetClassIndex (example->PredictedClass ());
        int32  p = _out.precision ();
        _out.precision (11);
        _out << example->ClassName          () << "\t" 
             << mlClassIdx                     << "\t"
             << example->ImageFileName      () << "\t" 
             << example->OrigSize           () << "\t"
             << example->NumOfEdgePixels    () << "\t"
             << example->Probability        () << "\t"
             << example->CentroidRow        () << "\t"
             << example->CentroidCol        () << "\t"
             << example->PredictedClassName () << "\t"
             << predClassIdx                   << "\t"
             << example->BreakTie           () << "\t"
             << example->SfCentroidRow      () << "\t"
             << example->SfCentroidCol      () << "\t"
             << example->Latitude           () << "\t"
             << example->Longitude          ()
             << endl;
        _out.precision (p);
      }
      _numExamplesWritten++;
    }
  }

  delete  classStatistics;
  if  (weOwnImages)
  {
    delete  examples;
    examples = NULL;
  }

  _successful = true;
  return;

}  /* SaveFile */





int32 FeatureFileIOPices::MaxNumPlanktonRawFields = 88;
const  char*  FeatureFileIOPices::PlanktonRawFeatureDecriptions[] =
{
  "Size",                    //  0
  "Moment1",                 //  1 
  "Moment2",                 //  2
  "Moment3",                 //  3
  "Moment4",                 //  4
  "Moment5",                 //  5
  "Moment6",                 //  6
  "Moment7",                 //  7
  
  "EdgeSize",                //  8
  "EdgeMoment1",             //  9
  "EdgeMoment2",             // 10
  "EdgeMoment3",             // 11
  "EdgeMoment4",             // 12
  "EdgeMoment5",             // 13
  "EdgeMoment6",             // 14
  "EdgeMoment7",             // 15

  "TransparancyConvexHull",  // 16
  "TransparancyPixelCount",  // 17
  
  "TransparancyOpen3",       // 18    Granulometric features
  "TransparancyOpen5",       // 19
  "TransparancyOpen7",       // 20
  "TransparancyOpen9",       // 21
  "TransparancyClose3",      // 22
  "TransparancyClose5",      // 23
  "TransparancyClose7",      // 24

  "EigenRatio",              // 25
  "EigenHead",               // 26

  "ConvexArea",              // 27
  "TransparancySize",        // 28
  "TransparancyWtd",         // 29

  "WeighedMoment0",          // 30
  "WeighedMoment1",          // 31
  "WeighedMoment2",          // 32
  "WeighedMoment3",          // 33
  "WeighedMoment4",          // 34
  "WeighedMoment5",          // 35
  "WeighedMoment6",          // 36
  "WeighedMoment7",          // 37

  "Fourier0",                // 38
  "Fourier1",                // 39
  "Fourier2",                // 40
  "Fourier3",                // 41
  "Fourier4",                // 42

  "ContourFourierDescriptor0", // 43
  "ContourFourierDescriptor1", // 44
  "ContourFourierDescriptor2", // 45
  "ContourFourierDescriptor3", // 46
  "ContourFourierDescriptor4", // 47

  "FourierDescRight1",         // 48
  "FourierDescLeft2",          // 49
  "FourierDescRight2",         // 50
  "FourierDescLeft3",          // 51
  "FourierDescRight3",         // 52
  "FourierDescLeft4",          // 53
  "FourierDescRight4",         // 54
  "FourierDescLeft5",          // 55
  "FourierDescRight5",         // 56
  "FourierDescLeft6",          // 57
  "FourierDescRight6",         // 58
  "FourierDescLeft7",          // 59
  "FourierDescRight7",         // 60
  "FourierDescLeft8",          // 61
  "FourierDescRight8",         // 62

  "IntensityHist1",            // 63
  "IntensityHist2",            // 64
  "IntensityHist3",            // 65 
  "IntensityHist4",            // 66
  "IntensityHist5",            // 67
  "IntensityHist6",            // 68
  "IntensityHist7",            // 69

  "HeightVsWidth",             // 70

  "Length",                  // 71
  "Width",                   // 72
  "FilledArea",              // 73

  "FlowRate1",               // 74
  "FlowRate2",               // 75

  "IntensityHistB0",         // 76
  "IntensityHistB1",         // 77
  "IntensityHistB2",         // 78
  "IntensityHistB3",         // 79 
  "IntensityHistB4",         // 80
  "IntensityHistB5",         // 81
  "IntensityHistB6",         // 82
  "IntensityHistB7",         // 83

  "Depth",                   // 84
  "Salinity",                // 85
  "Oxygen",                  // 86
  "Florences"                // 87

};



KKStr   FeatureFileIOPices::PlanktonFieldName (int32  fieldNum)
{
  if  ((fieldNum < 0)  ||  (fieldNum >= MaxNumPlanktonRawFields))
  {
    // This should never happen,  if it does I want to know right away and fix
    // the problem, will probably be indicative a greater more involved 
    // problem.

    cerr << endl
         << "PlanktonFieldName   ***ERROR***  FieldNum[" << fieldNum << "] is out of bounds." << endl
         << endl;
    return  "Field_" + StrFormatInt (fieldNum, "ZZZZ0");
  }
  
  return  PlanktonRawFeatureDecriptions [fieldNum];
}  /* PlanktonFieldName */






int32   FeatureFileIOPices::PlanktonMaxNumOfFields ()
{
  return  MaxNumPlanktonRawFields;
}







/**                       LoadInSubDirectoryTree
 * @brief Loads in the contents of a sub-directory tree.
 *        Meant to work with SIPPER plankton images, it starts at a specified subdirectory and 
 *        transverses all subdirectories.  It makes use of FeatureDataReSink for each specific
 *        sub-directory.
 * @param[in] rootDir,   Strating directory.
 * @param[in,out] mlClasses, List of classes, any new classes in fileName will be added.
 * @param[in] useDirectoryNameForClassName, if true set class names to sub-directory name.
 *            This happens because the user may manually move images between dorectories using
 *            the sub-directory name as the class name.
 * @param[in] log, where to send diagnostic messages to.
 * @param[in] rewiteRootFeatureFile, If true rewite the feature file in the specified 'rootDir'.  This
 *            feature file will contain all entries from all sub-directories below it.
 * @return - A ImageFeaturesList container object.  This object will own all the examples loaded.
 */
ImageFeaturesListPtr  FeatureFileIOPices::LoadInSubDirectoryTree 
                                             (KKStr                 _rootDir,
                                              MLClassConstList&  _mlClasses,
                                              bool                  _useDirectoryNameForClassName,
                                              volatile const bool&  _cancelFlag, 
                                              bool                  _rewiteRootFeatureFile,
                                              RunLog&               _log
                                             )
{
  _log.Level (10) << "FeatureFileIOPices::LoadInSubDirectoryTree    rootDir[" << _rootDir << "]." << endl;

  osAddLastSlash (_rootDir);

  KKStr  featureFileName ("");
  KKStr  fullFeatureFileName ("");

  if  (!_rootDir.Empty ())
  {
    featureFileName = osGetRootNameOfDirectory (_rootDir) + ".data";
    fullFeatureFileName = _rootDir + featureFileName;
  }
  else
  {
    featureFileName     = "Root.data";
    fullFeatureFileName = "Root.data";
  }

  MLClassConstPtr  unKnownClass = _mlClasses.GetUnKnownClass ();
  if  (_useDirectoryNameForClassName)
  {
    KKStr className = MLClass::GetClassNameFromDirName (_rootDir);
    unKnownClass    = _mlClasses.GetMLClassPtr (className);
  }

  bool  changesMade = false;

  ImageFeaturesListPtr  dirImages = NULL;

  FileDescPtr  fileDesc = FeatureFileIOPices::NewPlanktonFile (_log);

  if  (_rewiteRootFeatureFile)
  {
    DateTime  timeStamp;
    dirImages = FeatureDataReSink (_rootDir,
                                   featureFileName,
                                   unKnownClass,
                                   _useDirectoryNameForClassName,
                                   _mlClasses,
                                   _cancelFlag,
                                   changesMade,
                                   timeStamp,
                                   _log
                                  );
    if  (_useDirectoryNameForClassName)
    {
      ImageFeaturesList::iterator  idx;
      for  (idx = dirImages->begin ();  idx != dirImages->end ();  idx++)
      {
        if  ((*idx)->MLClass () != unKnownClass)
        {
          (*idx)->MLClass (unKnownClass);
          changesMade = true;
        }
      }

      if  (changesMade)
      {
        KKStr  fullFileName = osAddSlash (_rootDir) + featureFileName;
        uint32  numExamplesWritten = 0;
        bool  cancel     = false;
        bool  successful = false;
        driver.SaveFeatureFile (fullFileName,
                                dirImages->AllFeatures (),
                                *dirImages,
                                numExamplesWritten,
                                cancel,
                                successful,
                                _log
                              );
      }
    }
  }
  else
  {
    dirImages =  new ImageFeaturesList (fileDesc, true, _log, 100);
  }


  // Now that we have processed all image files in "rootDir",
  // lets process any sub-directories.

  KKStr  dirSearchPath = osAddSlash (_rootDir) + "*.*";

  KKStrListPtr  subDirectories = osGetListOfDirectories (dirSearchPath);
  if  (subDirectories)
  {
    KKStrList::iterator  idx;

    for  (idx = subDirectories->begin ();  (idx != subDirectories->end ()  &&  (!_cancelFlag));   idx++)
    {
      KKStr  subDirName (**idx);
      if  (subDirName == "BorderImages")
      {
        // We ignore this director
        continue;
      }

      KKStr  newDirPath = osAddSlash (_rootDir) + subDirName;

      ImageFeaturesListPtr  subDirImages = LoadInSubDirectoryTree (newDirPath, 
                                                                   _mlClasses, 
                                                                   _useDirectoryNameForClassName, 
                                                                   _cancelFlag,
                                                                   true,     // true = ReWriteRootFeatureFile
                                                                   _log
                                                                  );
      ImageFeaturesPtr  image = NULL;

      osAddLastSlash (subDirName);

      // We want to add the directory path to the ImageFileName so that we can later locate the source image.
      ImageFeaturesList::iterator  idx = subDirImages->begin ();
      for  (idx = subDirImages->begin ();  idx != subDirImages->end ();  idx++)
      {
        image = *idx;
        KKStr  newImageFileName = subDirName + image->ImageFileName ();
        image->ImageFileName (newImageFileName);
      }

      dirImages->AddQueue (*subDirImages);
      subDirImages->Owner (false);
      delete  subDirImages;
    }

    delete  subDirectories;  subDirectories = NULL;
  }

  _log.Level (10) << "LoadInSubDirectoryTree - Done" << endl;

  return  dirImages;
}  /* LoadInSubDirectoryTree */






ImageFeaturesListPtr  FeatureFileIOPices::FeatureDataReSink 
                                        (KKStr                 _dirName, 
                                         const KKStr&          _fileName, 
                                         MLClassConstPtr    _unknownClass,
                                         bool                  _useDirectoryNameForClassName,
                                         MLClassConstList&  _mlClasses,
                                         volatile const bool&  _cancelFlag,    // will be monitored,  if set to True  Load will terminate.
                                         bool&                 _changesMade,
                                         DateTime&             _timeStamp,
                                         RunLog&               _log
                                        )
{
  _changesMade = false;
  _timeStamp = DateTime ();

  FileDescPtr  fileDesc = FeatureFileIOPices::NewPlanktonFile (_log);
  if  (_unknownClass == NULL)
    _unknownClass = MLClass::GetUnKnownClassStatic ();

  bool  logImagesGettingFeaturesCalced = false;
  if  (_unknownClass != NULL)
  {
    if  (_unknownClass->Name ().EqualIgnoreCase ("detritus_blob"))
      logImagesGettingFeaturesCalced = true;
  }

  KKStr  className = _unknownClass->Name ();

  _log.Level (10) << "FeatureDataReSink, dirName           [" << _dirName   << "]." << endl;
  _log.Level (10) << "                   fileName          [" << _fileName  << "]." << endl;
  _log.Level (10) << "                   UnKnownClass      [" << className  << "]." << endl;

  osAddLastSlash (_dirName);
  
  KKStr  fullFeatureFileName = osAddSlash (_dirName) +  _fileName;

  bool  successful;

  KKStr fileNameToOpen;
  if  (_dirName.Empty ())
    fileNameToOpen = _fileName;
  else
    fileNameToOpen = osAddSlash (_dirName) + _fileName;

  bool  versionsAreSame = false;

  FeatureVectorListPtr  origFeatureVectorData 
        = driver.LoadFeatureFile (fileNameToOpen, _mlClasses, -1, _cancelFlag, successful, _changesMade, _log);

  if  (origFeatureVectorData == NULL)
  {
    successful = false;
    origFeatureVectorData = new ImageFeaturesList (fileDesc, true, _log, 10);
  }

  if  (_cancelFlag)
  {
    delete  origFeatureVectorData;  origFeatureVectorData = NULL;
    return  new ImageFeaturesList (fileDesc, true, _log, 10);
  }

  ImageFeaturesListPtr  origFeatureData = NULL;

  if  (strcmp (origFeatureVectorData->UnderlyingClass (), "ImageFeaturesList") == 0)
     origFeatureData = dynamic_cast<ImageFeaturesListPtr>(origFeatureVectorData);
  else
  {
    origFeatureData = new ImageFeaturesList (*origFeatureVectorData, true);
    delete  origFeatureVectorData;
    origFeatureVectorData = NULL;
  }

  KKStr  fileSpec = osAddSlash (_dirName) + "*.*";
  KKStrListPtr   fileNameList = osGetListOfFiles (fileSpec);

  if  (!fileNameList)
  {
    // There are no Image Files,  so we need to return a Empty List of Image Features.

    if  (origFeatureData->QueueSize () > 0)
      _changesMade = true;

    delete  origFeatureData;  origFeatureData = NULL;

    return  new ImageFeaturesList (fileDesc, true, _log, 10);
  }

  InstrumentDataFileManager::InitializePush ();

  if  (successful)
  {
    if  (origFeatureData->Version () == CurrentFeatureFileVersionNum)
    {
      versionsAreSame = true;
      _timeStamp = osGetFileDateTime (fileNameToOpen);
    }

    else
    {
      _changesMade = true;
    }
  }
  else
  {
    delete  origFeatureData;
    origFeatureData = new ImageFeaturesList (fileDesc, true, _log);
  }

  origFeatureData->SortByRootName (false);


  ImageFeaturesListPtr  extractedFeatures = new ImageFeaturesList (fileDesc, true, _log, fileNameList->QueueSize () + 2);
  extractedFeatures->Version (CurrentFeatureFileVersionNum);

  fileNameList->Sort (false);

  KKStrList::iterator  fnIDX;    //StringListIterator  fnIDX (*fileNameList);

  KKStrPtr  imageFileName;

  int32  numImagesFoundInOrigFeatureData = 0;
  int32  numOfNewFeatureExtractions = 0;

  for  (fnIDX = fileNameList->begin ();  (fnIDX != fileNameList->end ())  &&  (!_cancelFlag);  ++fnIDX)
  {
    imageFileName = *fnIDX;
    bool validImageFileFormat = SupportedImageFileFormat (*imageFileName);
    
    if  (!validImageFileFormat)
      continue;

    if  (imageFileName->SubStrPart (0, 5) == "Frame_")
    {
      // We suspect that we have a Frame image
      if  ((imageFileName->StrInStr ("Before."))  ||
           (imageFileName->StrInStr ("After."))
          )
      {
        // We do not want to process frame images.
        continue;
      }
    }

    ImageFeaturesPtr  origImage = origFeatureData->BinarySearchByName (*imageFileName);
    if  (origImage)
      numImagesFoundInOrigFeatureData++;

    if  (origImage  &&  versionsAreSame)
    {
      if  (_useDirectoryNameForClassName)
      {
        if  (origImage->MLClass () != _unknownClass)
        {
          _changesMade = true;
          origImage->MLClass (_unknownClass);
        }
      }

      else if  ((origImage->MLClass ()->UnDefined ())  &&  (origImage->MLClass () != _unknownClass))
      {
        _changesMade = true;
        origImage->MLClass (_unknownClass);
      }


      if  (origImage->Depth () == 0.0)
      {
        // It seems that the Instrumentaton data is missing;

        InstrumentDataPtr  id = NULL;
        try
        {
          id = InstrumentDataFileManager::GetClosestInstrumentData (imageFileName, _cancelFlag, _log);
        }
        catch  (...)
        {
          id = NULL;
        }
        if  (id)
        {
          origImage->FlowRate1    (id->FlowRate1    ());
          origImage->FlowRate2    (id->FlowRate2    ());
          origImage->Latitude     (id->Latitude     ());
          origImage->Longitude    (id->Longitude    ());
          origImage->Depth        (id->Depth        ());
          origImage->Oxygen       (id->Oxygen       ());
          origImage->Salinity     (id->Salinity     ());
          origImage->Fluorescence (id->Fluorescence ());
          _changesMade = true;
        }
      }

      extractedFeatures->PushOnBack (origImage);
      origFeatureData->DeleteEntry (origImage);
    }
    else
    {
      // We either  DON'T have an original image    or    versions are not the same.
      KKStr  fullFileName = _dirName + (*imageFileName);
      ImageFeaturesPtr image = NULL;
      try
      {
        if  (logImagesGettingFeaturesCalced)
          _log.Level (10) << "FeatureDataReSink " << imageFileName << endl;
        image = new ImageFeatures (fullFileName, _unknownClass, successful);
        if  (logImagesGettingFeaturesCalced)
          _log.Level (10) << "FeatureDataReSink " << imageFileName << "  Computed." << endl;
      }
      catch  (...)
      {
        _log.Level (-1) << endl << endl
          << "FeatureDataReSink   ***ERROR***"  << endl
          << "       Exception occured calling constructor 'ImageFeatures'  trying to compute FeatureVector." << endl
          << endl;
        successful = false;
        image = NULL;
      }

      if  (!successful)
      {
        _log.Level (-1) << " FeatureFileIOPices::FeatureDataReSink  *** ERROR ***, Processing Image File["
                       << imageFileName << "]."
                       << endl;
        delete  image;
      }

      else
      {
        _changesMade = true;
        image->ImageFileName (*imageFileName);

        // We will need to check the InstrumentDataFilesManager object for other data.
        InstrumentDataPtr  id =  NULL;
        try
        {
          id = InstrumentDataFileManager::GetClosestInstrumentData (imageFileName, _cancelFlag, _log);
        }
        catch (...)
        {
          _log.Level (-1) << endl << endl
            << "FeatureDataReSink   ***ERROR***"  << endl
            << "       Exception occured calling  'GetClosestInstrumentData'  ImageFileName[" << imageFileName << "]." << endl
            << endl;
          id = NULL;
        }
        
        if  ((origImage)  &&  (id == NULL))
        {
          // Since we do not have access to Instruementation data for this image but we do have an
          // older version of the ImageFeatures oject;  we will try to recover what we need from the 
          // older version of the ImageFeatures object.

          if  (origImage->Depth () != 0.0f)
          {
            image->FlowRate1    (origImage->FlowRate1    ());
            image->FlowRate2    (origImage->FlowRate2    ());
            image->Latitude     (origImage->Latitude     ());
            image->Longitude    (origImage->Longitude    ());
            image->Depth        (origImage->Depth        ());
            image->Oxygen       (origImage->Oxygen       ());
            image->Salinity     (origImage->Salinity     ());
            image->Fluorescence (origImage->Fluorescence ());
          }
        }
        else
        {
          if  (id)
          {
            image->FlowRate1    (id->FlowRate1    ());
            image->FlowRate2    (id->FlowRate2    ());
            image->Latitude     (id->Latitude     ());
            image->Longitude    (id->Longitude    ());
            image->Depth        (id->Depth        ());
            image->Oxygen       (id->Oxygen       ());
            image->Salinity     (id->Salinity     ());
            image->Fluorescence (id->Fluorescence ());
          }
        }

        _log.Level (30) << image->ImageFileName () << "  " << image->OrigSize () << endl;
        extractedFeatures->PushOnBack (image);
        numOfNewFeatureExtractions++;

        if  ((numOfNewFeatureExtractions % 100) == 0)
          cout << numOfNewFeatureExtractions << " Images Extracted." << endl;
      }
    }
  }

  if  (numImagesFoundInOrigFeatureData != extractedFeatures->QueueSize ())
    _changesMade = true;
  
  extractedFeatures->Version (CurrentFeatureFileVersionNum);

  if  ((_changesMade)  &&  (!_cancelFlag))
  {
    //extractedFeatures->WriteImageFeaturesToFile (fullFeatureFileName, RawFormat, FeatureNumList::AllFeatures (extractedFeatures->FileDesc ()));

    uint32  numExamplesWritten = 0;

    driver.SaveFeatureFile (fullFeatureFileName,
                            FeatureNumList::AllFeatures (extractedFeatures->FileDesc ()),
                            *extractedFeatures,
                            numExamplesWritten,
                            _cancelFlag,
                            successful,
                            _log
                           );

    _timeStamp = osGetLocalDateTime ();
  }

  delete  fileNameList;
  delete  origFeatureData;

  InstrumentDataFileManager::InitializePop ();

  _log.Level (10) << "FeatureDataReSink  Exiting     Dir["  << _dirName << "]." << endl;

  return  extractedFeatures;
}  /* FeatureDataReSink */



FileDescPtr  FeatureFileIOPices::planktonFileDesc = NULL;


FileDescPtr  FeatureFileIOPices::NewPlanktonFile (RunLog&  _log)
{
  if  (planktonFileDesc)
    return  planktonFileDesc;

  bool  alreadyExists = false;
  planktonFileDesc = new FileDesc ();
  for  (int32 fieldNum = 0;  fieldNum < MaxNumPlanktonRawFields;  fieldNum++)
  {
    planktonFileDesc->AddAAttribute (PlanktonFieldName (fieldNum), NumericAttribute, alreadyExists);
  }

  // Lets make sure that one was already created by opening up a data file.
  planktonFileDesc = FileDesc::GetExistingFileDesc (planktonFileDesc);

  return  planktonFileDesc;
}  /* NewPlanktonFile () */

