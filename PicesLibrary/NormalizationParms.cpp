#include "FirstIncludes.h"

#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "MemoryDebug.h"

using namespace  std;

#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace  KKU;

#include "ModelParam.h"
#include "NormalizationParms.h"
#include "FeatureNumList.h"
#include "MLClass.h"
#include "FeatureVector.h"
#include "TrainingConfiguration2.h"
using namespace  MLL;



NormalizationParms::NormalizationParms (bool                _normalizeNominalFeatures,
                                        FeatureVectorList&  _examples,
                                        RunLog&             _log
                                       ):

  fileDesc                 (NULL),
  fileName                 (),
  log                      (_log),
  mean                     (NULL),
  normalizeFeature         (NULL),
  normalizeNominalFeatures (_normalizeNominalFeatures),
  numOfFeatures            (0),
  numOfExamples            (0),
  sigma                    (NULL)

{
  log.Level (20) << "FeatureNormalization - Creating instance from[" 
                 << _examples.FileName () << "]."
                 << endl;

  fileDesc      = _examples.FileDesc ();
  attriuteTypes = fileDesc->CreateAttributeTypeTable ();
  numOfFeatures = _examples.NumOfFeatures ();
  DeriveNormalizationParameters (_examples);
}  /*  NormalizationParms */



NormalizationParms::NormalizationParms (const ModelParam&   _param,
                                        FeatureVectorList&  _examples,
                                        RunLog&             _log
                                       ):
  fileDesc                 (NULL),
  fileName                 (),
  log                      (_log),
  mean                     (NULL),
  normalizeFeature         (NULL),
  normalizeNominalFeatures (false),
  numOfFeatures            (0),
  numOfExamples            (0),
  sigma                    (NULL)
{
  log.Level (20) << "FeatureNormalization - Creating instance from[" << _examples.FileName () << "]." << endl;

  fileDesc                 = _examples.FileDesc ();
  numOfFeatures            = _examples.NumOfFeatures ();
  attriuteTypes            = fileDesc->CreateAttributeTypeTable ();
  normalizeNominalFeatures = _param.NormalizeNominalFeatures ();

  DeriveNormalizationParameters (_examples);
 }



NormalizationParms::NormalizationParms (TrainingConfiguration2Ptr  _config,
                                        FeatureVectorList&         _examples,
                                        RunLog&                    _log
                                       ):

  fileDesc                 (NULL),
  fileName                 (),
  log                      (_log),
  mean                     (NULL),
  normalizeFeature         (NULL),
  normalizeNominalFeatures (false),
  numOfFeatures            (0),
  numOfExamples            (0),
  sigma                    (NULL)

{
  log.Level (20) << "FeatureNormalization - Creating instance from[" 
                 << _examples.FileName () << "]."
                 << endl;

  fileDesc                 = _config->FileDesc ();
  attriuteTypes            = fileDesc->CreateAttributeTypeTable ();
  normalizeNominalFeatures = _config->NormalizeNominalFeatures ();
  numOfFeatures            = _examples.NumOfFeatures ();

  DeriveNormalizationParameters (_examples);
}  /*  NormalizationParms */






NormalizationParms::NormalizationParms (FileDescPtr  _fileDesc,
                                        KKStr        _fileName,
                                        bool&        _successfull,
                                        RunLog&      _log
                                       ):
  fileDesc                  (_fileDesc),
  fileName                  (_fileName),
  log                       (_log),
  mean                      (NULL),
  normalizeFeature          (NULL),
  normalizeNominalFeatures  (false),
  numOfFeatures             (0),
  numOfExamples             (0),
  sigma                     (NULL)

{
  log.Level (10) << "NormalizationParms::NormalizationParms - Loading instance from File[" 
                 << fileName << "]." 
                 << endl;

  attriuteTypes            = fileDesc->CreateAttributeTypeTable ();

  KKStr  field;

  _successfull = true;

  FILE*  inputFile = osFOPEN (fileName.Str (), "r");
  if  (!inputFile)
  {
    log.Level (-1) << "NormalizationParms::NormalizationParms   *** ERROR ***" << endl
                   << "                    Input File[" << fileName << "] is not Valid." << endl;
    _successfull = false;
    return;
  }

  Read (inputFile, _successfull);

  fclose (inputFile);

  ConstructNormalizeFeatureVector ();

  return;
}


NormalizationParms::NormalizationParms (FileDescPtr  _fileDesc,
                                        FILE*        _in,
                                        bool&        _successfull,
                                        RunLog&      _log
                                       ):

  fileDesc                  (_fileDesc),
  fileName                  (),
  log                       (_log),
  mean                      (NULL),
  normalizeFeature          (NULL),
  normalizeNominalFeatures  (false),
  numOfFeatures             (0),
  numOfExamples             (0),
  sigma                     (NULL)

{
  attriuteTypes  = fileDesc->CreateAttributeTypeTable ();

  Read (_in, _successfull);

  ConstructNormalizeFeatureVector ();
}


NormalizationParms::NormalizationParms (FileDescPtr  _fileDesc,
                                        istream&     _in,
                                        bool&        _successfull,
                                        RunLog&      _log
                                       ):

  fileDesc                  (_fileDesc),
  fileName                  (),
  log                       (_log),
  mean                      (NULL),
  normalizeFeature          (NULL),
  normalizeNominalFeatures  (false),
  numOfFeatures             (0),
  numOfExamples             (0),
  sigma                     (NULL)

{
  attriuteTypes  = fileDesc->CreateAttributeTypeTable ();

  Read (_in, _successfull);

  ConstructNormalizeFeatureVector ();
}



NormalizationParms::~NormalizationParms ()
{
  delete  [] mean;               mean             = NULL;
  delete  [] sigma;              sigma            = NULL;
  delete  [] normalizeFeature;   normalizeFeature = NULL;
}


int32  NormalizationParms::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (NormalizationParms)
    + attriuteTypes.size () * sizeof (AttributeType)
    + fileName.MemoryConsumedEstimated ()
    + numOfFeatures * (sizeof (bool) + sizeof (double) + sizeof (double));  //  mean + sigma

  return  memoryConsumedEstimated;
}



void  NormalizationParms::DeriveNormalizationParameters (FeatureVectorList&  _examples)
{
  numOfExamples   = 0;
  int32 numOfNoise    = 0;

  mean  = new double[numOfFeatures];
  sigma = new double[numOfFeatures];

  double*  total    = new double [numOfFeatures];
  double*  sigmaTot = new double [numOfFeatures];
 
  int32  i;

  for  (i = 0; i < numOfFeatures; i++)
  {
    mean[i]     = 0.0;
    sigma[i]    = 0.0;
    total[i]    = 0.0;
    sigmaTot[i] = 0.0;
  }

  double  featureValue;

  FeatureVectorPtr  image;

  FeatureVectorList::iterator imageIDX;

  for  (imageIDX = _examples.begin ();  imageIDX != _examples.end ();  imageIDX++)
  {
    image = *imageIDX;
    if  ((image->MLClass ()->UnDefined ())  ||  
         (image->MissingData ())               ||  
         (!image->FeatureDataValid ())
        )
    {
      // We have a noise image and do not want this as partof our Normalization 
      // procedure.
      numOfNoise++;
    }
    else
    {
      // Since this image is defimed then we can use it in our normalization calculations.
      for  (i = 0; i < numOfFeatures; i++)
      {
        featureValue = double (image->FeatureData (i));
        total[i]    += featureValue;
      }

      numOfExamples++;
    }
  }

  for  (i = 0; i < numOfFeatures; i++)
  {
    double meanDouble  = total[i] / double (numOfExamples);
    mean[i]  = meanDouble;
  }


  for  (imageIDX = _examples.begin ();  imageIDX != _examples.end ();  imageIDX++)
  {
    image = *imageIDX;
    if  ((image->MLClass ()->UnDefined ())  ||  
         (image->MissingData ())               ||  
         (!image->FeatureDataValid ())
        )
    {
      // We have a noise image and do not want this as partof our Normalization 
      // procedure.
    }
    else
    {
      // Since this image is defimed then we can use it in our normalization calculations.
      for  (i = 0; i < numOfFeatures; i++)
      {
        featureValue = double (image->FeatureData (i));
        double  delta = featureValue - mean[i];
        sigmaTot[i] += delta * delta;
      }
    }
  }

  for  (i = 0; i < numOfFeatures; i++)
  {
    sigma[i] = sqrt (sigmaTot[i] / numOfExamples);
  }

  delete[]  sigmaTot;
  delete[]  total;

  ConstructNormalizeFeatureVector ();

} /* DeriveNormalizationParameters */



void  NormalizationParms::Save (const KKStr&  _fileName,
                                bool&          _successfull
                               )
{
  log.Level (20) << "NormalizationParms::Save  FileName[" << _fileName << "]." << endl;

  fileName = _fileName;

  _successfull = true;

  ofstream outFile (fileName.Str ());
  if  (!outFile.is_open ())
  {
    log.Level (-1) << "NormalizationParms::Save - Error writing to file["
                   << _fileName << "]."
                   << endl;
    _successfull = false;
    return;
  }

  Write (outFile);

  outFile.close ();

  return;
} /* Save */



void  NormalizationParms::Write (ostream&  o)
{
  int32  i = 0;

  int32  origPrecision = o.precision ();
  o.precision (12);

  o << "<NormalizationParms>"      << endl;
  o << "NumOfFeatures"             << "\t" << numOfFeatures                             << endl;
  o << "NumOfExamples"             << "\t" << numOfExamples                             << endl;
  o << "NormalizeNominalFeatures"  << "\t" << (normalizeNominalFeatures ? "Yes" : "No") << endl;

  o << "Means";
  for  (i = 0;  i < numOfFeatures;  i++)
    o << "\t" << mean[i];
  o << endl;


  o << "Sigmas";
  for  (i = 0;  i < numOfFeatures;  i++)
     o << "\t" << sigma[i];
  o << endl;

  o << "</NormalizationParms>" << endl;
  o.precision (origPrecision);
}  /* Write */




void  NormalizationParms::Read (FILE*  i,
                                bool&  sucessful
                               )
{
  char  buff[40960];

  sucessful = true;

  numOfFeatures = -1;
  numOfExamples   = -1;
  delete[]  mean;   mean = NULL;
  delete[]  sigma;  sigma = NULL;

  while  (fgets (buff, sizeof (buff), i))
  {
    KKStr  ln (buff);
    KKStr  field = ln.ExtractQuotedStr ("\n\r\t", true);
    field.Upper ();

    if  (field == "</NORMALIZATIONPARMS>")
      break;

    if  (field == "NUMOFFEATURES")
    {
      numOfFeatures = ln.ExtractTokenInt ("\t");
      if  (numOfFeatures < 1)
      {
        log.Level (-1) << endl << endl << endl
                       << "NormalizationParms::Read      NumOfFeatures[" << numOfFeatures << "]" << endl
                       << endl
                       << "                              Must be greater than 0." << endl
                       << endl;
        sucessful = false;
        continue;
      }

      if  (numOfFeatures != fileDesc->NumOfFields ())
      {
        log.Level (-1) << endl << endl << endl
                       << "NormalizationParms::Read      NumOfFeatures dose not agree with FileDesc provided." << endl
                       << endl
                       << "           NumOfFeatures[" << numOfFeatures            << "]" << endl
                       << "           FileDesc     [" << fileDesc->NumOfFields () << "]" << endl
                       << endl;
        sucessful = false;
        continue;
      }

      mean  = new double[numOfFeatures];
      sigma = new double[numOfFeatures];

      int32 x;
      for  (x = 0;  x < numOfFeatures;  x++)
      {
        mean [x] = 0.0;
        sigma[x] = 0.0;
      }
    }

    else if  (field == "NUMOFEXAMPLES")
      numOfExamples = FFLOAT (ln.ExtractTokenDouble ("\t"));

    
    else if  (field == "NORMALIZENOMINALFEATURES")
      normalizeNominalFeatures  = ln.ExtractTokenBool ("\t");


    else if  (field == "MEANS")
    {
      if  (!mean)
      {
        log.Level (-1) << endl << endl << endl
                       << "NormalizationParms::Read      NumOfFeatures was not specified before 'Means'." << endl
                       << endl;
        sucessful = false;
        continue;
      }

      int32  x = 0;
      while  ((!ln.Empty ())  &&  (x < numOfFeatures))
      {
        mean[x] = ln.ExtractTokenDouble ("\t");
        x++;
      }
    }


    else if  (field == "SIGMAS")
    {
      if  (!sigma)
      {
        log.Level (-1) << endl << endl << endl
                       << "NormalizationParms::Read      NumOfFeatures was not specified before 'Sigmas'." << endl
                       << endl;
        sucessful = false;
        continue;
      }

      int32  x = 0;
      while  ((!ln.Empty ())  &&  (x < numOfFeatures))
      {
        sigma[x] = ln.ExtractTokenDouble ("\t\n\r");
        x++;
      }
    }
  }
}  /* Read */





void  NormalizationParms::Read (istream&  i,
                                bool&     sucessful
                               )
{
  char  buff[40960];

  sucessful = true;

  numOfFeatures = -1;
  numOfExamples   = -1;
  delete[]  mean;   mean = NULL;
  delete[]  sigma;  sigma = NULL;

  while  (i.getline( buff, sizeof (buff)))
  {
    KKStr  ln (buff);
    KKStr  field = ln.ExtractQuotedStr ("\n\r\t", true);
    field.Upper ();

    if  (field == "</NORMALIZATIONPARMS>")
      break;

    if  (field == "NUMOFFEATURES")
    {
      numOfFeatures = ln.ExtractTokenInt ("\t");
      if  (numOfFeatures < 1)
      {
        log.Level (-1) << endl << endl << endl
                       << "NormalizationParms::Read      NumOfFeatures[" << numOfFeatures << "]" << endl
                       << endl
                       << "                              Must be greater than 0." << endl
                       << endl;
        sucessful = false;
        continue;
      }

      if  (numOfFeatures != fileDesc->NumOfFields ())
      {
        log.Level (-1) << endl << endl << endl
                       << "NormalizationParms::Read      NumOfFeatures dose not agree with FileDesc provided." << endl
                       << endl
                       << "           NumOfFeatures[" << numOfFeatures            << "]" << endl
                       << "           FileDesc     [" << fileDesc->NumOfFields () << "]" << endl
                       << endl;
        sucessful = false;
        continue;
      }

      mean  = new double[numOfFeatures];
      sigma = new double[numOfFeatures];

      int32 x;
      for  (x = 0;  x < numOfFeatures;  x++)
      {
        mean [x] = 0.0;
        sigma[x] = 0.0;
      }
    }

    else if  (field == "NUMOFEXAMPLES")
      numOfExamples = (float)ln.ExtractTokenDouble ("\t");

    
    else if  (field == "NORMALIZENOMINALFEATURES")
      normalizeNominalFeatures  = ln.ExtractTokenBool ("\t");


    else if  (field == "MEANS")
    {
      if  (!mean)
      {
        log.Level (-1) << endl << endl << endl
                       << "NormalizationParms::Read      NumOfFeatures was not specified before 'Means'." << endl
                       << endl;
        sucessful = false;
        continue;
      }

      int32  x = 0;
      while  ((!ln.Empty ())  &&  (x < numOfFeatures))
      {
        mean[x] = ln.ExtractTokenDouble ("\t");
        x++;
      }
    }


    else if  (field == "SIGMAS")
    {
      if  (!sigma)
      {
        log.Level (-1) << endl << endl << endl
                       << "NormalizationParms::Read      NumOfFeatures was not specified before 'Sigmas'." << endl
                       << endl;
        sucessful = false;
        continue;
      }

      int32  x = 0;
      while  ((!ln.Empty ())  &&  (x < numOfFeatures))
      {
        sigma[x] = ln.ExtractTokenDouble ("\t\n\r");
        x++;
      }
    }
  }
}  /* Read */







double  NormalizationParms::Mean (int32 i)
{
  if  ((i < 0)  ||  (i > numOfFeatures))
  {
    log.Level (-1) << "NormalizationParms::Mean feature Num[" << i << "]  out of bounds." << endl;
    return  -99999.99;
  }
  else
  {
    return  mean[i];
  }
}  /* Mean */





double  NormalizationParms::Sigma (int32 i)
{
  if  ((i < 0)  ||  (i > numOfFeatures))
  {
    log.Level (-1) << "NormalizationParms::Mean feature Num[" << i << "]  out of bounds." << endl;
    return  (FFLOAT)-99999.99;
  }
  else
  {
    return  sigma[i];
  }
}  /* Sigma */




void  NormalizationParms::ConstructNormalizeFeatureVector ()
{
  delete  normalizeFeature;
  normalizeFeature = new bool [numOfFeatures];

  int32  i;
  for  (i = 0;  i < numOfFeatures;  i++)
  {
    if  (normalizeNominalFeatures)
    {
      normalizeFeature[i] = true;
    }

    else 
    {
      if  ((attriuteTypes[i] == NominalAttribute)  ||  (attriuteTypes[i] == SymbolicAttribute))
      {
        normalizeFeature[i] = false;
      }
      else
      {
        normalizeFeature[i] = true;
      }
    }
  }
}  /* ConstructNormalizeFeatureVector */




void  NormalizationParms::NormalizeAImage (FeatureVectorPtr  image)
{
  FFLOAT*  featureData = image->FeatureDataAlter ();

  for  (int32 i = 0; i < numOfFeatures; i++)
  {
    if  (normalizeFeature[i])
    {
      double  normValue = 0.0;
      if  (sigma[i] != 0.0)
        normValue = ((double)featureData[i] - mean[i]) / sigma[i];
      featureData[i] = (FFLOAT)normValue;
    }
  }
}  /* NormalizeAImage */






void  NormalizationParms::NormalizeImages (FeatureVectorListPtr  images)
{
  if  (numOfFeatures != images->NumOfFeatures ())
  {
    log.Level (-1) << "NormalizationParms::NoralizeImage  **** ERROR ****     Mismatched Feature Count." << endl
                   << "            NormalizationParms [" << numOfFeatures            << "]" << endl
                   << "            ImageFeatiresList  [" << images->NumOfFeatures () << "]."  << endl
                   << endl;

    osWaitForEnter ();
    exit (-1);
    return;
  }

  FeatureVectorList::iterator idx;

  for  (idx = images->begin ();  idx != images->end ();  idx++)
    NormalizeAImage (*idx);

  return;
}  /* NoralizeImage */



FeatureVectorPtr  NormalizationParms::ToNormalized (FeatureVectorPtr  example)  const
{
  FeatureVectorPtr  result = new FeatureVector (*example);
  FFLOAT*  featureData = result->FeatureDataAlter ();
  for  (int32 i = 0; i < numOfFeatures; i++)
  {
    if  (normalizeFeature[i])
    {
      double  normValue = 0.0;
      if  (sigma[i] != 0.0)
        normValue = ((double)featureData[i] - mean[i]) / sigma[i];
      featureData[i] = (FFLOAT)normValue;
    }
  }

  return result;
}  /* ToNormalized */
