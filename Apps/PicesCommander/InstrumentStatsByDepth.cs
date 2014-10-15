using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PicesCommander
{
  public  class InstrumentStatsByDepth
  {
    public  InstrumentStatsByDepth (int binId)
    {
      binId                   = binId;
      binDepth                = 0;
      scanLines               = 0;
      volumeSampled           = 0.0f;
      temperatureMean         = 0.0f;
      fluorescenceMean        = 0.0f;
      fluorescenceSensorMean  = 0.0f;
      densityMean             = 0.0f;
      oxygenMean              = 0.0f;
      depthMean               = 0.0f;
      salinityMean            = 0.0f;
      transmisivityMean       = 0.0f;
      turbidityMean           = 0.0f;
      cdomFluorescenceMean    = 0.0f;
    }
      

    public  InstrumentStatsByDepth (int     _binId,
                                    float   _binDepth,
                                    float   _scanLines,
                                    float   _volumeSampled,
                                    float   _temperatureMean,
                                    float   _salinityMean,
                                    float   _densityMean,
                                    float   _fluorescenceMean,
                                    float   _fluorescenceSensorMean,
                                    float   _oxygenMean,
                                    float   _depthMean,
                                    float   _transmisivityMean,
                                    float   _turbidityMean,
                                    float   _cdomFluorescenceMean
                                   )
    {

      binId                   = _binId;
      binDepth                = _binDepth;
      scanLines               = _scanLines;
      volumeSampled           = _volumeSampled;
      temperatureMean         = _temperatureMean;
      salinityMean            = _salinityMean;
      densityMean             = _densityMean;
      fluorescenceMean        = _fluorescenceMean;
      fluorescenceSensorMean  = _fluorescenceSensorMean;
      oxygenMean              = _oxygenMean;
      depthMean               = _depthMean;
      transmisivityMean       = _transmisivityMean;
      turbidityMean           = _turbidityMean;
      cdomFluorescenceMean    = _cdomFluorescenceMean;
      count = 1;
    }


    public  void  Add (InstrumentStatsByDepth  p)
    {
      binId                  = p.binId;
      binDepth               = p.binDepth;
      scanLines              += p.scanLines;
      volumeSampled          += p.volumeSampled;
      temperatureMean        += p.temperatureMean;
      salinityMean           += p.salinityMean;
      densityMean            += p.densityMean;
      fluorescenceMean       += p.fluorescenceMean;
      fluorescenceSensorMean += p.fluorescenceSensorMean;
      oxygenMean             += p.oxygenMean;
      depthMean              += p.depthMean;
      transmisivityMean      += p.transmisivityMean;
      turbidityMean          += p.turbidityMean;
      cdomFluorescenceMean   += p.cdomFluorescenceMean;
      ++count;
    }


    public  void  ComputeMean ()
    {
      if  (count == 0)
      {
        scanLines              = 0;
        volumeSampled          = 0;
        temperatureMean        = 0;
        salinityMean           = 0;
        densityMean            = 0.0f;
        fluorescenceMean       = 0;
        fluorescenceSensorMean = 0;
        oxygenMean             = 0;
        depthMean              = 0;
        transmisivityMean      = 0;
        turbidityMean          = 0;
        cdomFluorescenceMean   = 0;
        count = 0;
      }
      else
      {
        float  countF = (float)count;
        //  Since  'scanLines'  and  'volumeSampled'  are total and NOT means we do not need to difide them by count.
        //scanLines            /= count;
        //volumeSampled        /= count;
        temperatureMean        /= countF;
        salinityMean           /= countF;
        densityMean            /= countF;
        fluorescenceMean       /= countF;
        fluorescenceSensorMean /= countF;
        oxygenMean             /= countF;
        depthMean              /= countF;
        transmisivityMean      /= countF;
        turbidityMean          /= countF;
        cdomFluorescenceMean /= countF;
        count = 1;
      }
    }

    public  float   Oxygen_molPerKg  {get {return  (oxygenMean * 44.64f) / 1.027f;}}

    public  int     binId;
    public  float   binDepth;
    public  float   scanLines;
    public  float   volumeSampled;
    public  float   temperatureMean;
    public  float   salinityMean;
    public  float   densityMean;
    public  float   fluorescenceMean;
    public  float   fluorescenceSensorMean;
    public  float   oxygenMean;
    public  float   depthMean;
    public  float   transmisivityMean;
    public  float   turbidityMean;
    public  float   cdomFluorescenceMean;

    private int    count;  /**<  Number of enties added into this record;  so we can compute propper mean */
  }
}
