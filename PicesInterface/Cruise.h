#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;

//using namespace PicesCSharpClasses;



namespace PicesInterface {
  public ref  class Cruise
  {
  public:
    typedef  System::DateTime  DateTime;

    Cruise ();

    Cruise  (String^  fileName);

    property  String^    Name         {String^    get () {return name;       }  void  set (String^   _name)        {name        = _name;        }}
    property  String^    Location     {String^    get () {return location;   }  void  set (String^   _location)    {location    = _location;    }}
    property  String^    Objective    {String^    get () {return objective;  }  void  set (String^   _objective)   {objective   = _objective;   }}
    property  String^    Principal    {String^    get () {return principal;  }  void  set (String^   _principal)   {principal   = _principal;   }}
    property  String^    ResearchOrg  {String^    get () {return researchOrg;}  void  set (String^   _researchOrg) {researchOrg = _researchOrg; }}
    property  DateTime^  DateStart    {DateTime^  get () {return dateStart;  }  void  set (DateTime^ _dateStart)   {dateStart   = _dateStart;   }}
    property  DateTime^  DateEnd      {DateTime^  get () {return dateEnd;    }  void  set (DateTime^ _dateEnd)     {dateStart   = _dateEnd;     }}
    property  String^    Notes        {String^    get () {return notes;      }  void  set (String^   _notes)       {notes       = _notes;       }}
  

    void  Save (String^ baseDir);


    void  Load (String^ fileName);

  
  private:

    DateTime^  StrToDateTime (String^  s);
  
    void  ProcessField (String^  fieldName, 
                        String^  fieldValue
                       );

    String^    name;
    String^    location;
    String^    objective;
    String^    principal;
    String^    researchOrg;
    DateTime^  dateStart;
    DateTime^  dateEnd;
    String^    notes;

  };  /* Cruise */

}
