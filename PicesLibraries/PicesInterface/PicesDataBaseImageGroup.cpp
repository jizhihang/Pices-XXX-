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

#include "MemoryDebug.h"
#include "BasicTypes.h"
#include "Raster.h"
#include "OSservices.h"
using namespace  KKU;


#include "PicesDataBaseImageGroup.h"
#include "PicesKKStr.h"


using namespace  PicesInterface;


using namespace System;
using namespace System::Windows::Forms;



PicesDataBaseImageGroup::PicesDataBaseImageGroup ():
   imageGroup (NULL)
{
  GC::AddMemoryPressure (200);
}


PicesDataBaseImageGroup::PicesDataBaseImageGroup (DataBaseImageGroupPtr  _imageGroup):
  imageGroup (_imageGroup)
{
  GC::AddMemoryPressure (200);
}


PicesDataBaseImageGroup::PicesDataBaseImageGroup (int      _groupId,
                                                  String^  _name,
                                                  String^  _description,
                                                  uint     _count
                                                 ):
  imageGroup (NULL)
{
  imageGroup = new DataBaseImageGroup (_groupId,
                                       PicesKKStr::SystemStringToKKStr (_name),
                                       PicesKKStr::SystemStringToKKStr (_description),
                                       _count
                                      );
  GC::AddMemoryPressure (200);
}



  
PicesDataBaseImageGroup::!PicesDataBaseImageGroup ()
{
  delete  imageGroup;
  imageGroup = NULL;
  GC::RemoveMemoryPressure (200);
}


PicesDataBaseImageGroup::~PicesDataBaseImageGroup ()
{
  this->!PicesDataBaseImageGroup ();
}


int  PicesDataBaseImageGroup::ImageGroupId::get ()
{
  if  (!imageGroup)
    return -1;
  else
    return  imageGroup->ImageGroupId ();
}


void  PicesDataBaseImageGroup::ImageGroupId::set (int _groupId)
{
  if  (!imageGroup)
    imageGroup = new DataBaseImageGroup (_groupId, "DummyName_" + StrFormatInt (_groupId, "ZZZZZZZ0"), "", 0);
  else
    imageGroup->ImageGroupId (_groupId);
}



String^  PicesDataBaseImageGroup::Name::get ()
{
  if  (!imageGroup)
    return String::Empty;
  else
    return PicesKKStr::KKStrToSystenStr (imageGroup->Name ());
}




void   PicesDataBaseImageGroup::Name::set (String^  _name)
{
  if  (!imageGroup)
    imageGroup = new DataBaseImageGroup (-1, PicesKKStr::SystemStringToKKStr (_name), "", 0);
  else
    imageGroup->Name (PicesKKStr::SystemStringToKKStr (_name));
}



String^  PicesDataBaseImageGroup::Description::get ()
{
  if  (!imageGroup)
    return String::Empty;
  else
    return PicesKKStr::KKStrToSystenStr (imageGroup->Description ());
}




void   PicesDataBaseImageGroup::Description::set (String^  _description)
{
  if  (!imageGroup)
    imageGroup = new DataBaseImageGroup (-1, "", PicesKKStr::SystemStringToKKStr (_description), 0);
  else
    imageGroup->Description (PicesKKStr::SystemStringToKKStr (_description));
}



uint  PicesDataBaseImageGroup::Count::get ()
{
  if  (!imageGroup)
    return 0;
  else
    return  imageGroup->Count ();
}


void  PicesDataBaseImageGroup::Count::set (uint _count)
{
  if  (!imageGroup)
    imageGroup = new DataBaseImageGroup (-1, "DummyName_Count[" + StrFormatInt (_count, "ZZZZZZZ0") + "]", "", _count);
  else
    imageGroup->Count (_count);
}





void  PicesDataBaseImageGroup::ValidName (String^%  _name,
                                          String^%  _errorDesc,
                                          bool%     _valid
                                         )
{
  KKStr  name = PicesKKStr::SystemStringToKKStr (_name);
  KKStr  errorDesc = "";
  bool   valid = false;
  DataBaseImageGroup::ValidName (name, errorDesc, valid);

  _errorDesc = PicesKKStr::KKStrToSystenStr (errorDesc);
  _valid = valid;
}  /* ValidName */








PicesDataBaseImageGroupList::PicesDataBaseImageGroupList ()
{
}




PicesDataBaseImageGroupList::PicesDataBaseImageGroupList (DataBaseImageGroupListPtr  groups)
{
  if  (!groups)
    return;

  // If the provided list is not the owner of its contentes;  then we will need to create new instances.
  bool  createNewInstance = !(groups->Owner ());

  DataBaseImageGroupList::iterator  idx;
  for  (idx = groups->begin ();  idx != groups->end ();  idx++)
  {
    DataBaseImageGroupPtr  group = *idx;
    if  (createNewInstance)
      group = new DataBaseImageGroup (**idx);
    Add (gcnew PicesDataBaseImageGroup (group));
  }
  groups->Owner (false);
}




PicesDataBaseImageGroupList::!PicesDataBaseImageGroupList ()
{
  Clear ();
}



PicesDataBaseImageGroupList::~PicesDataBaseImageGroupList ()
{
  this->!PicesDataBaseImageGroupList ();
}
