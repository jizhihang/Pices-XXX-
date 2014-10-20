#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;


//using namespace PicesCSharpClasses;

#include  "SipperCruise.h"
using namespace SipperHardware;

#include  "PicesRunLog.h"


namespace PicesInterface 
{
  public ref class PicesSipperCruise  
  {
  public:
    PicesSipperCruise ();

    PicesSipperCruise (SipperCruisePtr _cruise);

  private:
    ~PicesSipperCruise  ();

  protected:
    !PicesSipperCruise  ();

  public:
    SipperCruisePtr  UnmanagedSipperCruise ()  {return  sipperCruise;}

    property  String^    CruiseName   {String^   get  ();   void  set (String^    _cruiseName);     }
    property  String^    ShipName     {String^   get  ();   void  set (String^    _shipName);       }
    property  String^    Description  {String^   get  ();   void  set (String^    _description);    }

    property  System::DateTime  DateStart    {System::DateTime get  ();   void  set (System::DateTime  _dateStart);  }
    property  System::DateTime  DateEnd      {System::DateTime get  ();   void  set (System::DateTime  _dateEnd);    }

    property  String^    Location     {String^   get  ();   void  set (String^    _location);   }
    property  String^    Objective    {String^   get  ();   void  set (String^    _objective);  }
    property  String^    Principal    {String^   get  ();   void  set (String^    _principal);  }
    property  String^    ResearchOrg  {String^   get  ();   void  set (String^    _researchOrg);}

    virtual
      String^  ToString ()  override;


    static  String^  ValidateCruiseName (String^  _cruiseName);


  private:
    SipperCruisePtr  sipperCruise;  // We own this instance of sipperCruise.
  };



  public ref class  PicesSipperCruiseList:  List<PicesSipperCruise^>
  {
  public:
    PicesSipperCruiseList (SipperCruiseListPtr  sipperCruises);

  private:
    ~PicesSipperCruiseList ();

  protected:
    !PicesSipperCruiseList ();

  public:
    PicesSipperCruise^  LookUpByName (String^  cruiseName);


  private:

  };

}  /* PicesInterface */
