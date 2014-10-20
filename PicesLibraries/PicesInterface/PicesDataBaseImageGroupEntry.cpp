#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

#include  "Raster.h"

#include  "OSservices.h"
#include  "PicesKKStr.h"

#include "PicesDataBaseImageGroupEntry.h"

using namespace  PicesInterface;

using namespace System;
using namespace System::Windows::Forms;
using namespace KKU;


PicesDataBaseImageGroupEntry::PicesDataBaseImageGroupEntry ()
{
  entry = new DataBaseImageGroupEntry (-1, "");
  GC::AddMemoryPressure (40);
}




PicesDataBaseImageGroupEntry::PicesDataBaseImageGroupEntry (DataBaseImageGroupEntryPtr  _entry):  
    entry (new DataBaseImageGroupEntry (*_entry))  
{
  GC::AddMemoryPressure (40);
}



PicesDataBaseImageGroupEntry::!PicesDataBaseImageGroupEntry ()
{
  delete  entry;
  entry = NULL;
  GC::RemoveMemoryPressure (40);
}



PicesDataBaseImageGroupEntry::~PicesDataBaseImageGroupEntry ()
{
  this->!PicesDataBaseImageGroupEntry ();
}



int  PicesDataBaseImageGroupEntry::ImageGroupId::get ()
{
  if  (!entry)
    return -1;
  else
    return entry->ImageGroupId ();
}



void  PicesDataBaseImageGroupEntry::ImageGroupId::set (int groupId)
{
  if  (!entry)
    entry = new DataBaseImageGroupEntry  (groupId, "");
  else
    entry->ImageGroupId (groupId);
}



String^  PicesDataBaseImageGroupEntry::ImageFileName::get ()
{
  if  (!entry)
    return String::Empty;
  else
    return PicesKKStr::KKStrToSystenStr (entry->ImageFileName ());
}



void  PicesDataBaseImageGroupEntry::ImageFileName::set (String^  imageFileName)
{
  if  (!entry)
    entry = new DataBaseImageGroupEntry  (-1, PicesKKStr::SystemStringToKKStr (imageFileName));
  else
    entry->ImageFileName (PicesKKStr::SystemStringToKKStr (imageFileName));
}





PicesDataBaseImageGroupEntryList::PicesDataBaseImageGroupEntryList ()
{
}




PicesDataBaseImageGroupEntryList::PicesDataBaseImageGroupEntryList (DataBaseImageGroupEntryListPtr  entries)
{
  if  (!entries)
    return;

  // If the provided list is not the owner of its contentes;  then we will need to create new instances.
  bool  createNewInstance = !(entries->Owner ());

  DataBaseImageGroupEntryList::iterator  idx;
  for  (idx = entries->begin ();  idx != entries->end ();  idx++)
  {
    DataBaseImageGroupEntryPtr  entry = *idx;
    if  (createNewInstance)
      entry = new DataBaseImageGroupEntry (**idx);
    Add (gcnew PicesDataBaseImageGroupEntry (entry));
  }
  entries->Owner (false);
}




PicesDataBaseImageGroupEntryList::!PicesDataBaseImageGroupEntryList ()
{
  Clear ();
}



PicesDataBaseImageGroupEntryList::~PicesDataBaseImageGroupEntryList ()
{
  this->!PicesDataBaseImageGroupEntryList ();
}
