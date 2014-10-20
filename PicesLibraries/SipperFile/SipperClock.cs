using System;
using System.Collections.Generic;
using System.Text;

namespace SipperFile
{
  //****************************************************************************
  //* Used by SipperFileBuffered to keep track of age if Buffered Frames       *
  //* Every time a buffered frame is accesed it will updates its TimeStamp     *
  //* from a global instance of 'SipperClock'                                  *
  //****************************************************************************
  public  class SipperClock
  {
    private  long  time = 0;

    /// <summary>
    /// Gets or sets the time.
    /// </summary>
    /// <value>The time.</value>
    public long  Time
    {
      get 
      {
        long  x = time;
        time++;
        return  x;
      }
      set
      {
        time = value;
      }
    }
  }
}
