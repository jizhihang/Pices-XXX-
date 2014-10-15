#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <memory>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;

#include  "BasicTypes.h"
#include  "DateTime.h"
using namespace KKU;



#include "InstrumentDataMeans.h"
#include "SipperVariables.h"
using namespace SipperHardware;



InstrumentDataMeans::InstrumentDataMeans ():
      downCast               (true),
      depthBin               (0),
      binDepth               (0.0f),
      scanLines              (0),
      volumeSampled          (0.0f),
      temperatureMean        (0.0f),
      salinityMean           (0.0f),
      denisityMean           (0.0f),
      fluorescenceMean       (0.0f),
      fluorescenceSensorMean (0.0f),
      oxygenMean             (0.0f),
      depthMean              (0.0f),
      transmisivityMean      (0.0f),
      turbidityMean          (0.0f),
      cdomFluorescenceMean   (0.0f)
{}


InstrumentDataMeans::InstrumentDataMeans (bool   _downCast,
                                          int32  _depthBin,
                                          float  _binDepth,
                                          int32  _scanLines,
                                          float  _volumeSampled,
                                          float  _temperatureMean,
                                          float  _salinityMean,
                                          float  _denisityMean,
                                          float  _fluorescenceMean,
                                          float  _fluorescenceSensorMean,
                                          float  _oxygenMean,
                                          float  _depthMean,
                                          float  _transmisivityMean,
                                          float  _turbidityMean,
                                          float  _cdomFluorescenceMean
                                         ):
      downCast               (_downCast),
      depthBin               (_depthBin),
      binDepth               (_binDepth),
      scanLines              (_scanLines),
      volumeSampled          (_volumeSampled),
      temperatureMean        (_temperatureMean),
      salinityMean           (_salinityMean),
      denisityMean           (_denisityMean),
      fluorescenceMean       (_fluorescenceMean),
      fluorescenceSensorMean (_fluorescenceSensorMean),
      oxygenMean             (_oxygenMean),
      depthMean              (_depthMean),
      transmisivityMean      (_transmisivityMean),
      turbidityMean          (_turbidityMean),
      cdomFluorescenceMean   (_cdomFluorescenceMean)
{
}





InstrumentDataMeansList::InstrumentDataMeansList (bool _owner):
    KKQueue<InstrumentDataMeans> (_owner)
{

}



InstrumentDataMeansList::InstrumentDataMeansList (const InstrumentDataMeansList& data):
    KKQueue<InstrumentDataMeans> (data)
{
}



InstrumentDataMeansPtr  InstrumentDataMeansList::LookUp (bool downCast, float  depth)
{
  InstrumentDataMeansList::iterator  idx;
  for  (idx = begin ();  idx != end ();  ++idx)
  {
    InstrumentDataMeansPtr  idm = *idx;
    if  ((idm->downCast == downCast)  &&  (idm->depthBin == depth))
      return  idm;
  }

  return NULL;
}  /* LookUp */



