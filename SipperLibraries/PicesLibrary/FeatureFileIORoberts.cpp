#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>
#include  <ctype.h>
#include  <time.h>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include "BasicTypes.h"
#include "DateTime.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace  KKU;


#include "FeatureFileIORoberts.h"
#include "FileDesc.h"
#include "MLClass.h"
using namespace  MLL;


FeatureFileIORoberts  FeatureFileIORoberts::driver;



FeatureFileIORoberts::FeatureFileIORoberts ():
   FeatureFileIO ("Roberts", false, true)
{
}

FeatureFileIORoberts::~FeatureFileIORoberts(void)
{
}



FileDescPtr  FeatureFileIORoberts::GetFileDesc (const KKStr&            _fileName,
                                                istream&                _in,
                                                MLClassConstListPtr  _classes,
                                                int32&                  _estSize,
                                                KKStr&                  _errorMessage,
                                                RunLog&                 _log
                                               )
{
  _log.Level (10) << endl << endl 
                  << "FeatureFileIORoberts::LoadFile   ***ERROR***      ARFF  read Functionality nopt implemented." << endl 
                  << endl;

  _errorMessage = "ROBERTS read functionality not implemeneted.";
  return NULL;
}





FeatureVectorListPtr  FeatureFileIORoberts::LoadFile (const KKStr&          _fileName,
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
  _log.Level (10) << endl << endl 
                  << "FeatureFileIORoberts::LoadFile   ***ERROR***      ARFF  read Functionality nopt implemented." << endl 
                  << endl;
                
  _errorMessage = "ROBERTS read functionality not implemeneted.";
  return NULL;
}  /* LoadFile */



void   FeatureFileIORoberts::SaveFile (FeatureVectorList&     _data,
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
  _log.Level (20) << "FeatureFileIORoberts::SaveFile    FileName[" << _fileName << "]" << endl;

  _numExamplesWritten = 0;
  
  FileDescPtr  fileDesc = _data.FileDesc ();

  const AttributePtr*  attrTable = fileDesc->CreateAAttributeTable ();

  int32  x;
  {
    KKStr  namesFileName = _fileName + ".names";
    // Write _out names file
    ofstream  nf (namesFileName.Str ());

    MLClassConstListPtr classes = _data.ExtractMLClassConstList ();
    classes->SortByName ();
    for  (x = 0;  x < classes->QueueSize ();  x++)
    {
      if  (x > 0)  nf << " ";
      nf << classes->IdxToPtr (x)->Name ();
    }
    delete  classes;
    classes = NULL;
    nf << endl << endl;

    for  (x = 0;  x < _selFeatures.NumOfFeatures ();  x++)
    {
      int32  featureNum = _selFeatures[x];
      AttributePtr  attr = attrTable[featureNum];
      if  ((attr->Type () == NominalAttribute)  ||  (attr->Type () == SymbolicAttribute))
      {
        int32 y;
        nf << "discrete"; 
        for  (y = 0;  y < attr->Cardinality ();  y++)
          nf << " " << attr->GetNominalValue (y);
      }
      else
      {
        nf << "continuous";
      }
      nf << endl;
    }
    nf.close ();
  }


  FeatureVectorPtr   example = NULL;

  int32 idx;
  for  (idx = 0;  (idx < _data.QueueSize ()) && (!_cancelFlag);  idx++)
  {
    example = _data.IdxToPtr (idx);

    for  (x = 0; x < _selFeatures.NumOfFeatures (); x++)
    {
      int32  featureNum = _selFeatures[x];
      AttributePtr attr = attrTable[featureNum];

      if  ((attr->Type () == NominalAttribute)  ||  (attr->Type () == SymbolicAttribute))
        _out << attr->GetNominalValue ((int32)(example->FeatureData (featureNum)));
      else
        _out << example->FeatureData (featureNum);
      _out << " ";
    }
    _out << example->ClassName ();
    _out << endl;
    _numExamplesWritten++;
  }

  if  (!_cancelFlag)
    _successful = true;

  delete  attrTable;
  return;
} /* WriteRobertsFile */





