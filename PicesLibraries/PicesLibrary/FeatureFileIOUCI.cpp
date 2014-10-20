#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;

#include  "BasicTypes.h"
#include  "DateTime.h"
#include  "OSservices.h"
#include  "RunLog.h"
#include  "Str.h"
using namespace  KKU;


#include "FeatureFileIOUCI.h"
#include "FileDesc.h"
#include "MLClass.h"
using namespace  MLL;



FeatureFileIOUCI   FeatureFileIOUCI::driver;


FeatureFileIOUCI::FeatureFileIOUCI ():
  FeatureFileIO ("UCI", true, true)
{
}



FeatureFileIOUCI::~FeatureFileIOUCI ()
{
}



FileDescPtr  FeatureFileIOUCI::GetFileDesc (const KKStr&            _fileName,
                                            istream&                _in,
                                            MLClassConstListPtr  _classes,
                                            int32&                  _estSize,
                                            KKStr&                  _errorMessage,
                                            RunLog&                 _log
                                           )
{
  _log.Level (20) << "FeatureFileIOUCI::GetFileDesc     FileName[" << _fileName << "]." << endl;


  // We are just going to read the first few lines to determine number of fields, etc
 
  int32  numOfFields       = 0;
  int32  numFieldsThisLine = 0;


  KKStr  ln;
  bool   eof;

  _estSize = 0;

  GetLine (_in, ln, eof);
  while  (!eof)
  {
    ln.TrimLeft ();
    ln.TrimRight ();
    if  ((ln.SubStrPart (0, 1) != "//")  &&  (!ln.Empty ()))
    {
      numFieldsThisLine = 0;

      KKStr  className = ln.ExtractToken (" ,\n\r\t");
      while (!ln.Empty ())
      {
        numFieldsThisLine++;
        className = ln.ExtractToken (" ,\n\r\t");
      }

      if  (className.Empty ())
        className = "UnKnown";
      MLClassConstPtr  mlClass = _classes->GetMLClassPtr (className);

      numOfFields = Max (numOfFields, numFieldsThisLine);
      _estSize++;
    }
    GetLine (_in, ln, eof);
  }

  bool  alreadyExists = false;
  int32  fieldNum = 0;

  FileDescPtr  fileDesc = new FileDesc ();

  for  (fieldNum = 0;  fieldNum < numOfFields;  fieldNum++)
  {
    fileDesc->AddAAttribute ("Field_" + StrFormatInt (fieldNum, "ZZZZ0"), NumericAttribute, alreadyExists);
  }

  return  fileDesc;
}  /* GetFileDesc */






FeatureVectorListPtr  FeatureFileIOUCI::LoadFile (const KKStr&          _fileName,
                                                  const FileDescPtr     _fileDesc,
                                                  MLClassConstList&  _classes, 
                                                  istream&              _in,
                                                  long                  _maxCount,    // Maximum # images to load.
                                                  volatile const bool&  _cancelFlag,
                                                  bool&                 _changesMade,
                                                  KKStr&                _errorMessage,
                                                  RunLog&               _log
                                                 )
{
  _log.Level (20) << "FeatureFileIOUCI::LoadFile   FileName[" << _fileName << "]" << endl;


  KKStr  rootName = osGetRootName (_fileName);


  int32  numOfFeatures = _fileDesc->NumOfFields ();
  int32  lineCount = 0;

  KKStr  ln (256);
  bool  eof;

  FeatureVectorListPtr  examples = new FeatureVectorList (_fileDesc, true, _log, 100);

  GetLine (_in, ln, eof);
  while  (!eof)
  {
    ln.TrimLeft ();
    ln.TrimRight ();

    if  ((ln.SubStrPart (0, 1) != "//")  &&  (!ln.Empty ()))
    {

      int32  featureNum = 0;
      FeatureVectorPtr  example = new FeatureVector (numOfFeatures);
  
      for  (featureNum = 0;  featureNum < numOfFeatures;  featureNum++)
      {
        KKStr  featureStr = ln.ExtractToken (" ,\n\r\t");
        example->AddFeatureData (featureNum, (FFLOAT)atof (featureStr.Str ()));
      }

      KKStr  className = ln.ExtractToken (" ,\n\r\t");
      MLClassConstPtr mlClass = _classes.GetMLClassPtr (className);
      example->MLClass (mlClass);

      KKStr  imageFileName = rootName + "_" + StrFormatInt (lineCount, "ZZZZZZ0");
      example->ImageFileName (imageFileName);

      examples->PushOnBack (example);

      lineCount++;
    }
    GetLine (_in, ln, eof);
  }

  return  examples;
}  /* LoadFile */






void   FeatureFileIOUCI::SaveFile (FeatureVectorList&     _data,
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
  FeatureVectorPtr   example = NULL;

  _numExamplesWritten = 0;

  int32  idx;
  int32  x;

  for  (idx = 0; idx < _data.QueueSize (); idx++)
  {
    example = _data.IdxToPtr (idx);

    for  (x = 0; x < _selFeatures.NumOfFeatures (); x++)
    {
      int32  featureNum = _selFeatures[x];
      _out << example->FeatureData (featureNum) << ",";
    }
    _out << example->ClassName ();
    _out << endl;
    _numExamplesWritten++;
  }

  _successful = true;
  return;
}  /* WriteUCIFile */


