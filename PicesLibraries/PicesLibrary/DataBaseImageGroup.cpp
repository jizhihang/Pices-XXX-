#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <memory>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <map>


using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"
using namespace KKU;

#include "DataBaseImageGroup.h"
using namespace  MLL;



DataBaseImageGroup::DataBaseImageGroup (int32           _imageGroupId,
                                        const KKStr&  _name,
                                        const KKStr&  _description,
                                        uint32          _count
                                        ):
     imageGroupId (_imageGroupId),
     name         (_name),
     description  (_description),
     count        (_count)
{
}



DataBaseImageGroup::DataBaseImageGroup (const DataBaseImageGroup&  _dataBaseImageGroup):
     
     imageGroupId (_dataBaseImageGroup.imageGroupId),
     name         (_dataBaseImageGroup.name),
     description  (_dataBaseImageGroup.description),
     count        (_dataBaseImageGroup.count)
{
}



DataBaseImageGroup::~DataBaseImageGroup ()
{
}




void  DataBaseImageGroup::ValidName (KKStr&  _name,
                                     KKStr&  _errorDesc,
                                     bool&   _valid
                                    )  // Returns back true if 'name' is formatted ok.
{
  _name.TrimLeft ();
  _name.TrimRight ();
  _valid = false;
  _errorDesc = "";

  if  (_name.Len () < 1)
  {
    _errorDesc = "Name is Blank";
    return;
  }

  KKStr  invalidChars;
  char  c = toupper (_name[0]);
  if  ((c < 'A')  ||  (c > 'Z'))
    _errorDesc << "First characted must be ('A' thru 'Z')";

  for  (int32 x = 0;  x < _name.Len ();  x++)
  {
    c = toupper (_name[(int)x]);
    if  ((c >= 'A')  &&  (c <= 'Z'))
      continue;
    if  ((c >= '0')  &&  (c <= '9'))
      continue;
    if  ((c == '_')  ||  (c == '-'))
      continue;

    if  (invalidChars.Len () > 0)
      invalidChars << ",";
    invalidChars.Append (c);
  }
  if  (!invalidChars.Empty ())
  {
    if  (!_errorDesc.Empty ())
      _errorDesc << ", ";
    _errorDesc << "Invalid Characters[" << invalidChars << "]";
  }

  _valid = _errorDesc.Empty ();
}  /* ValidName */



DataBaseImageGroupList::DataBaseImageGroupList (bool _owner):
  KKQueue<DataBaseImageGroup> (_owner)
{
}




DataBaseImageGroupList::DataBaseImageGroupList (const  DataBaseImageGroupList&  _dataBaseImageGroupList):
  KKQueue<DataBaseImageGroup> (_dataBaseImageGroupList)
{
}



DataBaseImageGroupList::~DataBaseImageGroupList ()
{
}



