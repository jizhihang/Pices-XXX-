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

using namespace  std;


#include  "BasicTypes.h"
#include  "DateTime.h"
#include  "OSservices.h"
#include  "RunLog.h"
#include  "Str.h"
using namespace  KKU;


#include "FeatureFileIOArff.h"
#include "FileDesc.h"
#include "MLClass.h"
using namespace  MLL;



FeatureFileIOArff  FeatureFileIOArff::driver;




FeatureFileIOArff::FeatureFileIOArff ():
   FeatureFileIO ("ARFF", false, true)
{
}


FeatureFileIOArff::~FeatureFileIOArff(void)
{
}



FileDescPtr  FeatureFileIOArff::GetFileDesc (const KKStr&            _fileName,
                                             istream&                _in,
                                             MLClassConstListPtr  _classes,
                                             int32&                  _estSize,
                                             KKStr&                  _errorMessage,
                                             RunLog&                 _log
                                            )
{
  _log.Level (10) << endl << endl 
                  << "FeatureFileIOArff::LoadFile   ***ERROR***      ARFF  read Functionality nopt implemented." << endl 
                  << endl;

  _errorMessage = "ARFF read functionality not implemeneted.";
  return NULL;
}





FeatureVectorListPtr  FeatureFileIOArff::LoadFile (const KKStr&          _fileName,
                                                   const FileDescPtr     _fileDesc,
                                                   MLClassConstList&  _classes, 
                                                   istream&              _in,
                                                   long                  _maxCount,    // Maximum # images to load.
                                                   volatile const bool&  _cancelFlag,
                                                   bool&                _changesMade,
                                                   KKStr&               _errorMessage,
                                                   RunLog&              _log
                                                  )
{
  _log.Level (10) << endl << endl 
                  << "FeatureFileIOArff::LoadFile   ***ERROR***      ARFF  read Functionality nopt implemented." << endl 
                  << endl;
                
  _errorMessage = "ARFF read functionality not implemeneted.";
  return NULL;
}  /* LoadFile */




void   FeatureFileIOArff::SaveFile (FeatureVectorList&     _data,
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
  _log.Level (20) << "FeatureFileIOArff::SaveFile    FileName[" << _fileName << "]" << endl;

  _numExamplesWritten = 0;

  FileDescPtr  fileDesc = _data.FileDesc ();
  const AttributePtr*  attrTable = fileDesc->CreateAAttributeTable ();

  int32  x;
  {
    _out << "% ARFF Format Definition: http://www.cs.waikato.ac.nz/~ml/weka/arff.html"  << endl
         << "%"                                                             << endl
         << "% FileName         [" << _fileName                      << "]" << endl
         << "% DateWritten      [" << osGetLocalDateTime ()          << "]" << endl
         << "% SelectedFeatures [" << _selFeatures.ToString ()       << "]" << endl
         << "% TotalRecords     [" << _data.QueueSize ()             << "]" << endl
         << "% NumAttributes    [" << _selFeatures.NumOfFeatures ()  << "]" << endl
         << "%"                                                      << endl
         << "%  ClassName" << "\t" << "Count"                        << endl;
    
    ClassStatisticListPtr  classStatistics = _data.GetClassStatistics ();
    KKStr  classListStr (classStatistics->QueueSize () * 15);
    for  (x = 0;  x < classStatistics->QueueSize ();  x++)
    {
      ClassStatisticPtr  classStatistic = classStatistics->IdxToPtr (x);
      _out << "%  " << classStatistic->Name () << "\t" << classStatistic->Count () << endl;
      if  (x > 0)
        classListStr << ", ";
      classListStr << classStatistic->Name ();
    }

    _out << "%"  << endl
         << "%  Total" << "\t" << _data.QueueSize () << endl
         << "%"                                << endl
         << "%"                                << endl
         << "@relation  image_features"        << endl
         << "%"                                << endl;

    
    for  (int32 fnIDX = 0;  fnIDX < _selFeatures.NumOfFeatures ();  fnIDX++)
    {
      int32  featureNum = _selFeatures[fnIDX];
      AttributePtr attr = attrTable[featureNum];
      _out << "@attribute " 
           << attr->Name ()
           << " ";

      if  ((attr->Type () == NominalAttribute)   ||  (attr->Type () == SymbolicAttribute))
      {
        _out << "(";
        for (x = 0;  x < attr->Cardinality ();  x++)
        {
          if  (x > 0)
            _out << ",";
          _out << attr->GetNominalValue (x);
        }
        _out << ")" << endl;
      }
      else
      {
        _out << "NUMERIC";
      }

      _out << endl;
    }

    _out << "@attribute ImageFileName  " << "string" << endl;

    _out << "@attribute Classes? { " << classListStr << " }" << endl;

    _out << endl
         << endl
         << "@data" << endl
         << "%"     << endl
         << "% " << _data.QueueSize () << " Instances" << endl
         << "%"     << endl;
    delete  classStatistics;
  }

  int32  numOfDigistsNeededInRowMask = Min ((int32)1, (int32) log10 (float (_data.QueueSize ()))) + 1;

  KKStr rowMask = "0";
  rowMask.RightPad (numOfDigistsNeededInRowMask, '0');

  FeatureVectorPtr   example = NULL;

  int32 idx;
  for  (idx = 0; idx < _data.QueueSize (); idx++)
  {
    example = _data.IdxToPtr (idx);

    for  (x = 0;  x < _selFeatures.NumOfFeatures ();  x++)
    {
      int32  featureNum = _selFeatures[x];

      if  ((attrTable[featureNum]->Type () == NominalAttribute)  ||  (attrTable[featureNum]->Type () == SymbolicAttribute))
        _out << attrTable[featureNum]->GetNominalValue ((int32)(example->FeatureData (featureNum)));
      else
        _out << example->FeatureData (featureNum);
      _out << ",";
    }

    KKStr  imageFileName = osGetRootNameWithExtension (example->ImageFileName ());
    if  (imageFileName.Empty ())
      imageFileName == "Row_" + StrFormatInt (idx, rowMask.Str ());
    _out << imageFileName << ",";
    _out << example->ClassName ();
    _out << endl;
    _numExamplesWritten++;
  }

  _successful = true;
  delete  attrTable;
  return;
}  /* SaveFile */

