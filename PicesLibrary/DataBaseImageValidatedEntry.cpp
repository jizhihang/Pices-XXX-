#include "FirstIncludes.h"
#include <stdlib.h>
#include <math.h>
#include <memory>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "MemoryDebug.h"

using namespace std;

#include "BasicTypes.h"
#include "OSservices.h"
#include "Str.h"
using namespace KKU;

#include "MLClass.h"

#include "DataBaseImageValidatedEntry.h"
using namespace  MLL;


DataBaseImageValidatedEntry::DataBaseImageValidatedEntry (const KKStr&        _imageFileName,
                                                          PointListPtr        _sizeCoordinates,
                                                          MLClassConstPtr  _validatedClass
                                                         ):
    imageFileName   (_imageFileName),
    sizeCoordinates (_sizeCoordinates),
    validatedClass  (_validatedClass)
{
}



DataBaseImageValidatedEntry::DataBaseImageValidatedEntry (const KKStr&        _imageFileName,
                                                          const KKStr&        _sizeCoordinatesDelStr,
                                                          MLClassConstPtr  _validatedClass
                                                         ):
    imageFileName   (_imageFileName),
    sizeCoordinates (NULL),
    validatedClass  (_validatedClass)
{
  if  (!_sizeCoordinatesDelStr.Empty ())
    sizeCoordinates = PointList::FromDelStr (_sizeCoordinatesDelStr);
}



DataBaseImageValidatedEntry::~DataBaseImageValidatedEntry ()
{
  delete  sizeCoordinates;
  sizeCoordinates = NULL;
}


const KKStr&   DataBaseImageValidatedEntry::ValidatedClassName () const
{
  if  (!validatedClass)
    return KKStr::EmptyStr ();
  else
    return validatedClass->Name ();
}



KKStr  DataBaseImageValidatedEntry::SizeCoordinatesDelStr () const
{
  if  (sizeCoordinates)
    return sizeCoordinates->ToDelStr (',');
  else
    return "";
}



float  DataBaseImageValidatedEntry::ImageSize (float  heightFactor,
                                               float  widthFactor
                                              )  const
{
  if  (!sizeCoordinates)
    return 0.0f;

  return  sizeCoordinates->ComputeSegmentLens (heightFactor, widthFactor);
}  /* ImageSize */




DataBaseImageValidatedEntryList::DataBaseImageValidatedEntryList (bool _owner):
      KKQueue<DataBaseImageValidatedEntry> (_owner),
      dateWritten (),
      fileName    (),
      hostName    ()
{
}



DataBaseImageValidatedEntryList::DataBaseImageValidatedEntryList (const KKStr&  _fileName,
                                                                  RunLog&       log
                                                                 ):
      KKQueue<DataBaseImageValidatedEntry> (true),
      dateWritten (),
      fileName    (),
      hostName    ()
{
  Load (_fileName, log);
}





DataBaseImageValidatedEntryList::~DataBaseImageValidatedEntryList ()
{
}



void  DataBaseImageValidatedEntryList::Save (const KKStr&  _fileName,
                                             RunLog&       log
                                            )
{
  fileName = _fileName;
  ofstream  o (fileName.Str ());
  if  (!o.is_open ())
  {
    log.Level (-1) << endl 
      << "DataBaseImageValidatedEntryList::Save    ***ERROR***     Could not open '" << _fileName << "'." << endl
      << endl;
    return;
  }

  dateWritten = osGetLocalDateTime ();
  fileName    = _fileName;
  hostName    = osGetHostName ();

  o << "//DataBaseImageValidatedEntryList"   << endl
    << "DateWritten" << "\t" << dateWritten  << endl
    << "NumEntries"  << "\t" << QueueSize () << endl
    << "HostName"    << "\t" << hostName     << endl
    << "FileName"    << "\t" << fileName     << endl;

  DataBaseImageValidatedEntryList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  ++idx)
  {
    DataBaseImageValidatedEntryPtr  e = *idx;
    o << "Entry"  << "\t"  << e->ImageFileName () 
                  << "\t"  << e->ValidatedClassName () 
                  << "\t"  << e->SizeCoordinatesDelStr () 
                  << endl;
  }
  o << "EndOfFile" << endl;
}  /* Save */



void  DataBaseImageValidatedEntryList::Load (const KKStr&  _fileName,
                                             RunLog&       log
                                            )
{
  fileName = _fileName;
  ifstream  i (fileName.Str ());
  if  (!i.is_open ())
  {
    log.Level (-1) << "DataBaseImageValidatedEntryList::Load    ***ERROR***     Could not open FileName[" << _fileName << "]" << endl;
    return;
  }

  bool  eof = false;
  KKStr  ln = osReadRestOfLine (i, eof);
  while  (!eof)
  {
    if  (!ln.StartsWith ("//"))
    {
      KKStr  lineName = ln.ExtractToken2 ("\t");

      if  (lineName.EqualIgnoreCase ("DateWritten"))
        dateWritten = DateTime (ln.ExtractToken2 ("\t"));

      else if  (lineName.EqualIgnoreCase ("HostName"))
        hostName = ln.ExtractToken2 ("\t");

      else if  (lineName.EqualIgnoreCase ("FileName"))
        fileName = ln.ExtractToken2 ("\t");

      else if  (lineName.EqualIgnoreCase ("Entry"))
      {
        KKStr  imageFileName = ln.ExtractToken2 ("\t");
        KKStr  validatedClassName = ln.ExtractToken2 ("\t");
        MLClassPtr  validatedClass = MLClass::CreateNewMLClass (validatedClassName);
        KKStr  sizeCoordinateDelStr = ln.ExtractToken2 ("\t");
        PushOnBack (new DataBaseImageValidatedEntry (imageFileName, sizeCoordinateDelStr, validatedClass));
      }
    }

    ln = osReadRestOfLine (i, eof);
  }

  i.close ();
}  /* Load */




/*
bool  DataBaseImageValidatedEntryList::Order::operator() (DataBaseImageValidatedEntryPtr p1,  DataBaseImageValidatedEntryPtr p2)
{
  if  (p1 == NULL)
  {
    return  (p2 != NULL);
  }

  if  (p2 != NULL)
    return  false;

  return  (p1->ImageFileName () < p2->ImageFileName ());
}
*/
