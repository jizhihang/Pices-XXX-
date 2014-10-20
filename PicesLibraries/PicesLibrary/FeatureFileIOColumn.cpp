#include "FirstIncludes.h"

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "MemoryDebug.h"

using namespace std;


#include "BasicTypes.h"
#include "DateTime.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;


#include "FeatureFileIOColumn.h"
#include "FileDesc.h"
#include "MLClass.h"
using namespace MLL;



FeatureFileIOColumn  FeatureFileIOColumn::driver;



FeatureFileIOColumn::FeatureFileIOColumn ():
    FeatureFileIO ("Column", true, true)
{
}



FeatureFileIOColumn::~FeatureFileIOColumn ()
{
}



FileDescPtr  FeatureFileIOColumn::GetFileDesc (const KKStr&            _fileName,
                                               istream&                _in,
                                               MLClassConstListPtr  _classes,
                                               int32&                  _estSize,
                                               KKStr&                  _errorMessage,
                                               RunLog&                 _log
                                              )
{
  _log.Level (20) << "FeatureFileIOColumn::GetFileDesc    FileName[" << _fileName << "]" << endl;

  bool  alreadyExists;
  bool  eof = false;
  bool  eol = false;
 
  int32  rowNum = 0;

  _estSize = 0;

  {
    // Read first Row to get number of examples.
    // First row specifies the Class for the correspondinmg Collumn.
    KKStr  field;
    GetToken (_in, " ", field, eof, eol);  rowNum++;
    while  ((!eol)  &&  (!eof))
    {
      MLClassConstPtr  mlClass = _classes->GetMLClassPtr (field);
      _estSize++;
      GetToken (_in, " ", field, eof, eol);
    }
  }

  FileDescPtr  fileDesc = new FileDesc ();


  int32  numOfFeatures = 0;
  while  (!eof)
  {
    // Read rest of file to get number of features.  There will be on row per attribute.
    KKStr  field;
    GetToken (_in, " ", field, eof, eol); 
    if  (!eof)
    {
      rowNum++;
      int32  numOfExamples = 0;

      while  ((!eol)  &&  (!eof))
      {
        numOfExamples++;
        GetToken (_in, " ", field, eof, eol);
      }

      if  (numOfExamples != _estSize)
      {
        // We have a bad row of data
        _errorMessage << "Row[" << rowNum << "] missing columns;  Expected[" << _estSize << "] found[" << numOfExamples << "]";
        _log.Level (-1) << endl << endl << endl 
                        << "ReadColumnFile    *** Invalid Row in File[" << _fileName << "]" << endl
                        << "         Row         [" << rowNum           << "]" << endl
                        << "         Num Examples[" << numOfExamples    << "]" << endl
                        << "         Not Match Header Row[" << _estSize << "]" << endl
                        << endl;

        // Can not delete an instance of a 'FileDesc' class once it has been created.
        // delete  fileDesc;

        return NULL;
      }


      fileDesc->AddAAttribute ("Field_" + StrFormatInt (numOfFeatures, "ZZZZ0"), NumericAttribute, alreadyExists);
      numOfFeatures++;
    }
  }

  return  fileDesc;
}  /* GetFileDesc */





FeatureVectorListPtr  FeatureFileIOColumn::LoadFile (const KKStr&          _fileName,
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
  _log.Level (20) << "FeatureFileIOColumn::LoadFile   FileName[" << _fileName << "]" << endl;

  KKStr  rootName = osGetRootName (_fileName);

  bool                eof           = false;
  bool                eol           = false;
  MLClassConstPtr  exampleClass  = NULL;
  KKStr               field;
  int32               lineCount     = 0;
  int32               numOfFeatures = _fileDesc->NumOfFields ();

  // Each row will represent a specific feature
  // Will initially create empty examples, then populate
  // each feature value as row's are read in.

  FeatureVectorListPtr  examples = new FeatureVectorList (_fileDesc, true, _log, 100);

  {
    // Read first row to get count of number of examples,

    lineCount = 0;

    GetToken (_in, " ", field, eof, eol);
    while  ((!eof)  &&  (!eol))
    {
      exampleClass = _classes.GetMLClassPtr (field);
      FeatureVectorPtr  example = new FeatureVector (numOfFeatures);
      example->MLClass (exampleClass);
      example->ImageFileName (rootName + "_" + StrFormatInt (lineCount, "00000"));
      examples->PushOnBack (example);
      lineCount++;
      GetToken (_in, " ", field, eof, eol);
    }
  }

  int32  featureNum = 0;

  while  (!eof)
  {
    // Process Next Row
    GetToken (_in, " ", field, eof, eol);
    if  (eof  ||  eol)
      continue;

    if  (featureNum >= numOfFeatures)
    {
      // We have more rows than there are supposed to be features.  Something 
      // has gone VERY WRONG.  Will abord load.
      _errorMessage << "Line more rows that there are supposed to be features.";
      _log.Level (-1) << endl << endl << endl
                      << "FeatureFileIOColumn::LoadFile    ***ERROR***" << endl
                      << endl
                      << "          FileName [" << _fileName    << "]" << endl
                      << "          Line Num [" << featureNum   << "]" << endl
                      << "          Error    [" << "Number of rows exceed number of features." << "]" << endl
                      << endl;
      delete  examples;
      return  NULL;
    }


    int32  lineNum = 0;
    while  ((!eof)  &&  (!eol))
    {
      if  (lineNum >= lineCount)
      {
        // We have more columns in this line than we have examples.
        // This is not a very good situation.  We will need to flag 
        // as error and abort loading.
        _errorMessage << "Line[" << featureNum << "]  More columns than there are supposed to be examples.";
        _log.Level (-1) << endl << endl << endl
                        << "FeatureFileIOColumn::LoadFile    ***ERROR***" << endl
                        << endl
                        << "          FileName [" << _fileName << "]" << endl
                        << "          Line Num [" << lineNum   << "]" << endl
                        << "          Error    [" << "Number of columns exceeds number of examples defined in first row." << "]" << endl
                        << endl;
        delete examples;
        return  NULL;
      }

      FeatureVectorPtr example = examples->IdxToPtr (lineNum);
      example->AddFeatureData (featureNum, FFLOAT (atof (field.Str ())));

      lineNum++;
      GetToken (_in, " ", field, eof, eol);
    }

    featureNum++;
  }


  return  examples;
}  /* LoadFile */





void   FeatureFileIOColumn::SaveFile (FeatureVectorList&     _data,
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
  _log.Level (20) << "FeatureFileIOColumn::SaveFile     FileName[" << _fileName << "]." << endl;
  int32  p = _out.precision ();
  _out.precision (9);

  FileDescPtr  fileDesc = _data.FileDesc ();

  {
    // Lets first write out the ClassName row

    int32  lineNum = 0;

    while  (lineNum < _data.QueueSize ())
    {
      if  (lineNum > 0)
        _out << " ";
      _out << _data[lineNum].MLClassName ();
    }
    _out << endl;
  }

  int32  featureNum = 0;
  for  (featureNum = 0;  featureNum < (int32)fileDesc->NumOfFields ();  featureNum++)
  {
    int32  lineNum = 0;

    while  (lineNum < _data.QueueSize ())
    {
      if  (lineNum > 0)
        _out << " ";
      _out << _data[lineNum].FeatureData (featureNum);
    }

    _out << endl;

    _numExamplesWritten = (uint32)((double)(_data.QueueSize ()) * ((double)featureNum / (double)(fileDesc->NumOfFields ())));
  }

  _out.precision (p);

  _successful = true;
  return;
}  /* SaveFile */
