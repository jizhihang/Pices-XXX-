#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


#include  <ctype.h>
#include  <time.h>

#include  <fstream>
#include  <iostream>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;


using namespace System;
using namespace System::Threading;
using namespace System::Windows::Forms;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"
#include  "..\\BaseLibrary\\GoalKeeper.h"
#include  "OSservices.h"

using namespace KKU;



#include "FileDesc.h"
using namespace MLL;

#include "PicesKKStr.h"
#include "PicesRunLog.h"


#include "PicesInstrumentData.h"
#include "PicesMethods.h"
using namespace SipperHardware;
using namespace  PicesInterface;


namespace  PicesInterface
{

  PicesInstrumentData::PicesInstrumentData ()
  {
    instrumentData = new InstrumentData ();
  }



  PicesInstrumentData::PicesInstrumentData (InstrumentDataPtr  _instrumentData)
  {
    instrumentData = _instrumentData;
  }



  PicesInstrumentData::~PicesInstrumentData ()
  {
    this->!PicesInstrumentData ();
  }


  PicesInstrumentData::!PicesInstrumentData ()
  {
    delete  instrumentData;
    instrumentData = NULL;
  }



  String^  PicesInstrumentData::FieldName (int fieldIndex)
  {
    return  PicesKKStr::KKStrToSystenStr (InstrumentData::FieldName (fieldIndex));
  }



  String^  PicesInstrumentData::ShortFieldName (int fieldIndex)
  {
    return  PicesKKStr::KKStrToSystenStr (InstrumentData::ShortFieldName (fieldIndex));
  }


  String^  PicesInstrumentData::FieldUnitOfMeasure (int fieldIndex)
  {
    return  PicesKKStr::KKStrToSystenStr (InstrumentData::FieldUnitOfMeasure (fieldIndex));
  }


  String^  PicesInstrumentData::FieldFormatStr (int fieldIndex)
  {
    return  PicesKKStr::KKStrToSystenStr (InstrumentData::FieldFormatStr (fieldIndex));
  }


  float PicesInstrumentData::FieldMinValue (int fieldIndex)
  {
    return  InstrumentData::FieldMinValue (fieldIndex);
  }


  int  PicesInstrumentData::GetFieldNum (String^  s)
  {
    return  InstrumentData::GetFieldNum (PicesKKStr::SystemStringToKKStr (s));
  }



  int  PicesInstrumentData::GetFieldNumFromShortName (String^  s)
  {
    return  InstrumentData::GetFieldNum (PicesKKStr::SystemStringToKKStr (s));
  }



  int  PicesInstrumentData::NumFields ()
  {
    return  InstrumentData::NumFields ();
  }




  String^  PicesInstrumentData::FieldToStr (int  fieldIndex)  // 'fieldIndex' = Same number returned by 'GetFieldNum'
  {
    if  (instrumentData == NULL)
      return  String::Empty;

    return  PicesKKStr::KKStrToSystenStr (instrumentData->FieldToStr (fieldIndex));
  }

  int       PicesInstrumentData::ActiveBattery::get          () {return instrumentData->ActiveBattery       ();}
  uint64    PicesInstrumentData::ByteOffset::get             () {return instrumentData->ByteOffset          ();}
  float     PicesInstrumentData::Bat1Level::get              () {return instrumentData->Bat1Level           ();}
  float     PicesInstrumentData::Bat2Level::get              () {return instrumentData->Bat2Level           ();}
  float     PicesInstrumentData::Bat3Level::get              () {return instrumentData->Bat3Level           ();}
  float     PicesInstrumentData::Bat4Level::get              () {return instrumentData->Bat4Level           ();}

  float     PicesInstrumentData::Conductivity::get           () {return instrumentData->Conductivity        ();}
  float     PicesInstrumentData::CTDBattery::get             () {return instrumentData->CTDBattery          ();}
  System::DateTime PicesInstrumentData::CtdDateTime::get       () {return PicesMethods::DateTimeKKUtoSystem (instrumentData->CtdDate ());}
  float     PicesInstrumentData::Density::get                () {return instrumentData->Density             ();}
  float     PicesInstrumentData::Depth::get                  () {return instrumentData->Depth               ();}
  float     PicesInstrumentData::Fluorescence::get           () {return instrumentData->Fluorescence        ();}
  float     PicesInstrumentData::FluorescenceSensor::get     () {return instrumentData->FluorescenceSensor  ();}
  float     PicesInstrumentData::CdomFluorescence::get       () {return instrumentData->CdomFluorescence        ();}
  float     PicesInstrumentData::CdomFluorescenceSensor::get () {return instrumentData->CdomFluorescenceSensor  ();}
  float     PicesInstrumentData::FlowRate1::get              () {return instrumentData->FlowRate1           ();}
  float     PicesInstrumentData::FlowRate2::get              () {return instrumentData->FlowRate2           ();}
  float     PicesInstrumentData::Illumination::get           () {return instrumentData->Illumination        ();}
  float     PicesInstrumentData::Oxygen::get                 () {return instrumentData->Oxygen              ();}
  float     PicesInstrumentData::OxygenSensor::get           () {return instrumentData->OxygenSensor        ();}
  float     PicesInstrumentData::Pitch::get                  () {return instrumentData->Pitch               ();}
  float     PicesInstrumentData::Pressure::get               () {return instrumentData->Pressure            ();}
  float     PicesInstrumentData::RecordRate::get             () {return instrumentData->RecordRate          ();}
  float     PicesInstrumentData::Roll::get                   () {return instrumentData->Roll                ();}
  float     PicesInstrumentData::Salinity::get               () {return instrumentData->Salinity            ();}
  uint      PicesInstrumentData::ScanLine::get               () {return instrumentData->ScanLine            ();}
  float     PicesInstrumentData::SoundVelocity::get          () {return instrumentData->SoundVelocity       ();}
  float     PicesInstrumentData::Temperature::get            () {return instrumentData->Temperature         ();}
  float     PicesInstrumentData::Transmissivity::get         () {return instrumentData->Transmisivity       ();}
  float     PicesInstrumentData::TransmissivitySensor::get   () {return instrumentData->TransmisivitySensor ();}
  float     PicesInstrumentData::Turbidity::get              () {return instrumentData->Turbidity           ();}
  float     PicesInstrumentData::TurbiditySensor::get        () {return instrumentData->TurbiditySensor     ();}

  double  PicesInstrumentData::Latitude::get       () {return instrumentData->Latitude      ();}
  double  PicesInstrumentData::Longitude::get      () {return instrumentData->Longitude     ();}

  void   PicesInstrumentData::ByteOffset::set   (uint64 _byteOffset)   {instrumentData->ByteOffset   (_byteOffset);}
  void   PicesInstrumentData::Conductivity::set (float  _conductivity) {instrumentData->Conductivity (_conductivity);}
  void   PicesInstrumentData::Density::set      (float  _density)      {instrumentData->Density      (_density);}
  void   PicesInstrumentData::Depth::set        (float  _depth)        {instrumentData->Depth        (_depth);}
  void   PicesInstrumentData::RecordRate::set   (float  _recordRate)   {instrumentData->RecordRate   (_recordRate);}
  void   PicesInstrumentData::ScanLine::set     (uint   _scanLine)     {instrumentData->ScanLine     (_scanLine);}
  void   PicesInstrumentData::Temperature::set  (float  _temperature)  {instrumentData->Temperature  (_temperature);}




  System::DateTime PicesInstrumentData::CtdDate::get  ()                             {return  OSservices::KKuDateTimeToSystemDateTime (instrumentData->CtdDate ());}
  void             PicesInstrumentData::CtdDate::set  (System::DateTime  _ctdDate)   {instrumentData->CtdDate    (OSservices::SystemDateTimeToKKuDateTime (_ctdDate));}







  PicesInstrumentDataList::PicesInstrumentDataList ()
  {
  }



  PicesInstrumentDataList::PicesInstrumentDataList (InstrumentDataListPtr   dataList)
  {
    InstrumentDataList::iterator  idx;
    for  (idx = dataList->begin ();  idx != dataList->end ();  idx++)
    {
      InstrumentDataPtr  id = *idx;
      Add (gcnew PicesInstrumentData (id));
    }

    dataList->Owner (false);  // We are taking ownership of the contents of dataList;
  }



  PicesInstrumentDataList::!PicesInstrumentDataList ()
  {
  }


  PicesInstrumentDataList::~PicesInstrumentDataList ()
  {
    this->!PicesInstrumentDataList ();
    Clear ();
  }




  array<double>^  PicesInstrumentDataList::ExtractVolumeByDepth (float  scanRate,   // Scan lines per second.
                                                                 float  defaultFlowRate
                                                                )
  {
    List<double>^  volByDepth = gcnew List<double>;

    // Should first sort data by SipperFile, then by scan line.


    if  (scanRate < 1.0f)
      scanRate = 24950.0f;

    PicesInstrumentData^  lastId = nullptr;

    for each  (PicesInstrumentData^  id  in  *this)
    {
      if  (lastId != nullptr)
      {
        long  deltaScanLines = id->ScanLine - lastId->ScanLine;
        if  (deltaScanLines > 0)
        {
          double  deltaTime = deltaScanLines / scanRate;

          double  flowRate = id->FlowRate1;
          if  (flowRate < 0.0)
            flowRate = defaultFlowRate;

          double  deltaMeters = flowRate * deltaTime;
          double  deltaVolume = deltaMeters * 0.098 * 0.098;
          int     depth = (int)floor (id->Depth);
          depth = Max (0, depth);
          if  (depth > 1000)
          {
            // Don't believe that it can be that deap so will ignore this Instrment Data line.
          }
          else
          {
            while  (volByDepth->Count <= depth)
              volByDepth->Add (0.0);
            volByDepth[depth] += deltaVolume;
          }
        }
          
        lastId = id;
      }
    }

    return  volByDepth->ToArray ();
  }  /* ExtractVolumeByDepth */


}  /* PicesInterface */