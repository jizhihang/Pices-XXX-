#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;

#include  "DataBaseImageGroup.h"
using namespace  MLL;


#include  "PicesRunLog.h"
#include  "PicesKKStr.h"


namespace PicesInterface 
{
  public  ref class PicesDataBaseImageGroup
  {
  public:
    PicesDataBaseImageGroup ();

    PicesDataBaseImageGroup (DataBaseImageGroupPtr  _imageGroup);  // Will take ownership of _imageGroup

    PicesDataBaseImageGroup (int      _groupId,
                             String^  _name,
                             String^  _description,
                             uint     _count
                            );


  protected:
    !PicesDataBaseImageGroup ();

  private:
    ~PicesDataBaseImageGroup (); 

  public:
    property  int        ImageGroupId  {int        get ();   void  set (int      _groupId);}
    property  String^    Name          {String^    get ();   void  set (String^  _name);}
    property  String^    Description   {String^    get ();   void  set (String^  _description);}
    property  uint       Count         {uint       get ();   void  set (uint     _count);}

    DataBaseImageGroupPtr  UnManagedClass ()  {return imageGroup;}

    static  void  ValidName (String^%  _name,
                             String^%  _errorDesc,
                             bool%     _valid
                            );

  private:
    DataBaseImageGroupPtr  imageGroup;
  };




  public  ref  class  PicesDataBaseImageGroupList:  List<PicesDataBaseImageGroup^>
  {
  public:
    PicesDataBaseImageGroupList ();

    /// Will take ownership of contents of 'entries';  if entries does not own its contents then 
    /// will create new instances.  The entries->Owner () flag will be set to 'false' 
    PicesDataBaseImageGroupList (DataBaseImageGroupListPtr  entries);

  protected:
    !PicesDataBaseImageGroupList ();

  private:
    ~PicesDataBaseImageGroupList ();
  };  /* PicesDataBaseImageGroupList */

};
