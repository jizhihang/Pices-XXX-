#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;

#include  "DataBaseImageGroupEntry.h"
using namespace  MLL;

#include  "PicesRunLog.h"
#include  "PicesKKStr.h"




namespace PicesInterface 
{
  public  ref class PicesDataBaseImageGroupEntry
  {
  public:
    PicesDataBaseImageGroupEntry ();
    PicesDataBaseImageGroupEntry (DataBaseImageGroupEntryPtr  _entry);

  protected:
    !PicesDataBaseImageGroupEntry ();

  private:
    ~PicesDataBaseImageGroupEntry ();

  public:
    property  int      ImageGroupId  {int      get ();   void  set (int      _groupId);}
    property  String^  ImageFileName {String^  get ();   void  set (String^  _name);}

    DataBaseImageGroupEntryPtr   UnManagedClass ()  {return entry;}

  private:
    DataBaseImageGroupEntryPtr  entry;
  };



  public  ref  class  PicesDataBaseImageGroupEntryList:  List<PicesDataBaseImageGroupEntry^>
  {
  public:
    PicesDataBaseImageGroupEntryList ();

    /// Will take ownership of contents of 'entries';  if entries does not own its contents then 
    /// will create new instances.  The entries->Owner () flag will be set to 'false' 
    PicesDataBaseImageGroupEntryList (DataBaseImageGroupEntryListPtr  entries);

  protected:
    !PicesDataBaseImageGroupEntryList ();

  private:
    ~PicesDataBaseImageGroupEntryList ();
  };  /* PicesDataBaseImageGroupEntryList */


};
