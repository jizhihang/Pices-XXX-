#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <memory>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <map>

#include  "MemoryDebug.h"

using namespace std;

#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;


#include  "DataBaseImageEntry.h"
using namespace  MLL;


DataBaseImageEntry::DataBaseImageEntry (const KKStr&  _imageFileName,
                                        float         _depth
                                        ):
    imageFileName (_imageFileName),
    depth         (_depth)
{
}



DataBaseImageEntry::~DataBaseImageEntry ()
{
}




DataBaseImageEntryList::DataBaseImageEntryList (bool _owner):
      KKQueue<DataBaseImageEntry> (_owner)
{
}


DataBaseImageEntryList::~DataBaseImageEntryList ()
{
}


