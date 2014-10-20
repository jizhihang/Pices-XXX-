#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;


/**
 @brief  Wrapper class for unmanaged InstrumentData class in SipperInstruments library.
 */



#include "InstrumentData.h"
#include "InstrumentDataList.h"
using namespace SipperHardware;

#include "OSservices.h"
#include "PicesKKStr.h"
#include "PicesRunLog.h"


namespace PicesInterface 
{
  public  ref class PicesInstrumentData
  {
  public:
    typedef  System::DateTime  SDateTime;
    typedef  KKU::DateTime     KDateTime;

    PicesInstrumentData ();
    PicesInstrumentData (InstrumentDataPtr  _instrumentData);

  protected:
    !PicesInstrumentData ();

  private:
    ~PicesInstrumentData ();

  public:
    static  String^  FieldFormatStr           (int fieldIndex);
    static  float    FieldMinValue            (int fieldIndex);
    static  String^  FieldName                (int fieldIndex);
    static  String^  FieldUnitOfMeasure       (int fieldIndex);
    static  int      GetFieldNum              (String^  s);
    static  int      GetFieldNumFromShortName (String^  s);
    static  int      NumFields                ();
    static  String^  ShortFieldName           (int fieldIndex);   // Short version of 'FieldName'

    String^  FieldToStr (int  fieldIndex);  // 'fieldIndex' = Same number returned by 'GetFieldNum'


    property  System::DateTime   CtdDate   {System::DateTime  get  ();   void  set (System::DateTime  _ctdDate);}

    property  int       ActiveBattery          {int       get ();}
    property  uint64    ByteOffset             {uint64    get ();   void  set (uint64 _byteOffset   );}
    property  float     Bat1Level              {float     get ();}
    property  float     Bat2Level              {float     get ();}
    property  float     Bat3Level              {float     get ();}
    property  float     Bat4Level              {float     get ();}
    property  float     Conductivity           {float     get ();   void  set (float  _conductivity );}
    property  float     CTDBattery             {float     get ();}
    property  SDateTime CtdDateTime            {SDateTime get ();}
    property  float     Depth                  {float     get ();   void  set (float  _depth        );}
    property  float     Density                {float     get ();   void  set (float  _density      );}
    property  float     Fluorescence           {float     get ();}
    property  float     FluorescenceSensor     {float     get ();}
    property  float     CdomFluorescence       {float     get ();}
    property  float     CdomFluorescenceSensor {float     get ();}
    property  float     FlowRate1              {float     get ();}
    property  float     FlowRate2              {float     get ();}
    property  float     Illumination           {float     get ();}
    property  float     OxygenSensor           {float     get ();}
    property  float     Oxygen                 {float     get ();}
    property  float     Pitch                  {float     get ();}
    property  float     Pressure               {float     get ();}
    property  float     RecordRate             {float     get ();   void  set (float  _recordRate    );}
    property  float     Roll                   {float     get ();}
    property  float     Salinity               {float     get ();}
    property  uint      ScanLine               {uint      get ();   void  set (uint   _scanLine     );}
    property  float     SoundVelocity          {float     get ();}
    property  float     Temperature            {float     get ();   void  set (float  _temperature  );}
    property  float     Transmissivity         {float     get ();}
    property  float     TransmissivitySensor   {float     get ();}
    property  float     Turbidity              {float     get ();}
    property  float     TurbiditySensor        {float     get ();}
    property  double    Latitude               {double    get ();}
    property  double    Longitude              {double    get ();}

    InstrumentDataPtr  UnmanagedClass ()  {return  instrumentData;}

    property static String^  DensityUnit          {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (InstrumentData::DensityUnit          ());}}
    property static String^  FluorescenceUnit     {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (InstrumentData::FluorescenceUnit     ());}}
    property static String^  OxygenUnit           {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (InstrumentData::OxygenUnit           ());}}
    property static String^  SalinityUnit         {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (InstrumentData::SalinityUnit         ());}}
    property static String^  TemperatureUnit      {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (InstrumentData::TemperatureUnit      ());}}
    property static String^  TransmisivityUnit    {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (InstrumentData::TransmisivityUnit    ());}}
    property static String^  TurbidityUnit        {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (InstrumentData::TurbidityUnit        ());}}
    property static String^  CdomFluorescenceUnit {String^  get ()  {return  PicesKKStr::KKStrToSystenStr (InstrumentData::CdomFluorescenceUnit ());}}


  private:
    InstrumentDataPtr  instrumentData;
  };  /* PicesInstrumentData */




 public ref class PicesInstrumentDataList: List<PicesInstrumentData^>
  {
  public:
    PicesInstrumentDataList ();
    PicesInstrumentDataList (InstrumentDataListPtr   dataList);


    // Will calculate the volume of water that was sampled by depth in meters.  Will use
    // the ScanLine and ScanRate(from SipperFile) to determine time at given depth and flowRate 
    // to determine rate at which water is passing through sampling tube.
    array<double>^  PicesInstrumentDataList::ExtractVolumeByDepth (float  scanRate,   // Scan lines per second.
                                                                   float  defaultFlowRate
                                                                  );

  private:
    ~PicesInstrumentDataList ();

  protected:
    !PicesInstrumentDataList ();
 };  /* PicesInstrumentDataList */

}
