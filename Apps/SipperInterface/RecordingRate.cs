using System;

namespace SipperInterface
{
	/// <summary>
	/// Summary description for RecordingRate.
	/// </summary>
	public class RecordingRate
	{
    private int         historySize = 1;
    private DateTime[]  times;
    private long[]      blockAddresses;
    private int         curIdx;
    private int         oldestIdx;

		public  RecordingRate (long  _startBlockAddress,
                           int   _historySize
                          )
		{
      curIdx         = 0;
      oldestIdx      = 0;
      historySize    = _historySize;
      times          = new DateTime[historySize];
      blockAddresses = new long[historySize];

      if  (_startBlockAddress < 0)
        _startBlockAddress = 0;

      int  x;
      for  (x = 0;  x < historySize;  x++)
      {
        times[x]          = DateTime.Now;
        blockAddresses[x] = _startBlockAddress;
      }
		}


    public  void  RecordNextBlock (long blockAddress)
    {
      lock (this)
      {
        curIdx++;
        if  (curIdx >= historySize)
          curIdx = 0;

        if  (curIdx == oldestIdx)
        {
          oldestIdx++;
          if  (oldestIdx >= historySize)
            oldestIdx = 0;
        }

        times[curIdx] = DateTime.Now;
        blockAddresses[curIdx] = blockAddress;
      }
    }  /* RecordNextBlock */


    public  double  BytesPerSec ()
    {
      lock (this)
      {
        TimeSpan  timeSpan = times[curIdx].Subtract (times[oldestIdx]);
        int  deltaSecs   = timeSpan.Seconds + timeSpan.Minutes * 60 + timeSpan.Hours * 3600;

        if  (deltaSecs <= 0)
          return 0;

        long  deltaBlocks = blockAddresses[curIdx] - blockAddresses[oldestIdx];
        long   deltaBytes  = deltaBlocks * 512;
        return  (double) deltaBytes / (double) deltaSecs;
      }
    }


    public  double  MegaBytesPerSec ()
    {
      return  BytesPerSec () / 1000000.0;
    }
  }  /* RecordingRate */
}
