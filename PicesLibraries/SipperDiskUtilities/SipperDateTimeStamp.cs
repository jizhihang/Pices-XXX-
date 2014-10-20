using System;
using System.Collections.Generic;
using System.Text;

namespace SipperDiskUtilities
{
  public  class  SipperDateTimeStamp
  {
    SipperTimeStamp  timeStamp = null;
    SipperDateStamp  dateStamp = null;


    public  SipperDateTimeStamp (DateTime  dt)
    {
      timeStamp = new SipperTimeStamp (dt);
      dateStamp = new SipperDateStamp (dt);
    }
    

    public  SipperDateTimeStamp (byte[]   buffer,
                                 ref int  nextIndex
                                )
    {
      timeStamp = new SipperTimeStamp (buffer, ref nextIndex);
      dateStamp = new SipperDateStamp (buffer, ref nextIndex);
    }

    public  SipperDateTimeStamp (string  dateStr,
                                 string  timeStr
                                )
    {
      dateStamp = new SipperDateStamp (dateStr.Trim ());
      timeStamp = new SipperTimeStamp (timeStr.Trim ());
    }


    public  bool  Valid ()
    {
      if  (!dateStamp.Valid ())
        return false;
      return  timeStamp.Valid ();
    }


    public  DateTime  DotNetDateTime
    {
      get
      {
        DateTime d;

        if  (!Valid ())
        {
          d = DateTime.MinValue;
        }
        else
        {
          try
          {
            d = new DateTime (dateStamp.Year, dateStamp.Month, dateStamp.Day,
                              timeStamp.Hour, timeStamp.Minute, timeStamp.Second
                             );
          }
          catch  (Exception)
          {
            d = DateTime.MinValue;
          }
        }
        return  d;
      }
    }
     
      
    static public  int  BuffSpaceNeeded ()
    {
      return  SipperTimeStamp.BuffSpaceNeeded () + SipperDateStamp.BuffSpaceNeeded ();
    }  /* BuffSpaceNeeded */


    public  void  FillBuffer (byte[]   buffer,
                              ref int  nextIndex
                             )
    {
      timeStamp.FillBuffer (buffer, ref nextIndex);
      dateStamp.FillBuffer (buffer, ref nextIndex);
    }  /* FillBuffer */

  }  /* SipperDateTimeStamp */



}
