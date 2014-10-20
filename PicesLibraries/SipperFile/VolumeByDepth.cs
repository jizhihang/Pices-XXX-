using System;
using System.Collections.Generic;
using System.Text;

namespace SipperFile
{
  class VolumeByDepth
  {
    private  List<float>   volumes;   // Each entry will represent volume for a given meter of depth.
                                      // volumes[1] = Volume sampled > 0.0  and   <= 1.0 meters
                                      // volumes[2] = Volume sampled > 1.0  and   <= 2.0 meters

    private  float         area         = 0.0f;
    private  float         curFlowRate  = 0.0f;
    private  float         curDepth     = 0.0f;
    private  DateTime      lastReportedTime = DateTime.Now;




    public  VolumeByDepth (float _area)
    {
      area = _area;
      volumes = new List<float> ();
    }

  
    public  void  ReportFlow (float      _flowRate,
                              float      _depth,  
                              DateTime   _curTime
                             )
    {
      TimeSpan ts   = _curTime - lastReportedTime;
      float    secs = ts.Seconds;

      float  volumneAcumulated = secs * curFlowRate;

      int  depthIdx = (int)Math.Ceiling (curDepth);
      while  (volumes.Count <= depthIdx)
        volumes.Add (0.0f);

      volumes[depthIdx] += volumneAcumulated;

      curFlowRate      = _flowRate;
      curDepth         = _depth;
      lastReportedTime = _curTime;
    }  /* ReportFlow */
  
  
    public  float  VolumeForDepth (int depth)
    {
      if  ((depth <= 0)  ||  (depth >= volumes.Count))
        return 0.0f;

      return  volumes[depth];
    }  /* VolumeForDepth */



    public  float  VolumeForDepth (int startDepth,
                                   int endDepth
                                  )
    {
      startDepth = Math.Max (0, startDepth);
      endDepth   = Math.Min (volumes.Count - 1, endDepth);

      float  totalVolume = 0.0f;
      for  (int depth = startDepth;  depth < endDepth;  depth++)
        totalVolume += volumes[depth];

      return  totalVolume;
    }  /* VolumeForDepth */

  }
}
