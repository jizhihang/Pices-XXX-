using System;
using System.Collections.Generic;
using System.Text;

namespace SipperFile
{
  public  class SipperFrame
  {
    private byte[][]     scanLines     = null;
    private int          frameNum      = -1;
    private long         scanLineFirst = -1;
    private long         scanLineLast  = -1;
    private SipperClock  clock         = null;
    private long         time          = 0;


    public byte[][]  ScanLines           
    {
      get 
      {
        time = clock.Time;
        return scanLines;
      } 
      set 
      {
        time = clock.Time;
        scanLines = value;
      }
    }
    
    
    public int       FrameNum        {get {return frameNum;     } set {frameNum       = value;}}
    public long      ScanLineFirst   {get {return scanLineFirst;} set {scanLineFirst  = value;}}
    public long      ScanLineLast    {get {return scanLineLast; } set {scanLineLast   = value;}}
    public long      Time            {get {return time;         } set {time           = value;}}


    public SipperFrame (SipperClock _clock)
    {
      clock = _clock;
      
      int x = 0;
      scanLines = new byte[SipperBaseFile.ScanLinesPerFrame][];
      for (x = 0; x < SipperBaseFile.ScanLinesPerFrame; x++)
        scanLines[x] = new byte[4096];
    }
  } /* SipperFrame */
}
