using System;
using System.Collections.Generic;
using System.Text;
using PicesInterface;

namespace SipperDiskUtilities
{
  public  class  SipperTimeStamp
  {
    byte  second;
    byte  minute;
    byte  hour;
    byte  unUsed;

    public  SipperTimeStamp (byte[]   buffer,
                             ref int  nextIntex
                            )
    {
      second = buffer[nextIntex];  nextIntex++;
      minute = buffer[nextIntex];  nextIntex++;
      hour   = buffer[nextIntex];  nextIntex++;
      unUsed = buffer[nextIntex];  nextIntex++;
    }


    public  SipperTimeStamp (DateTime  dt)
    {
      second = (byte)dt.Second; 
      minute = (byte)dt.Minute;
      hour   = (byte)dt.Hour;
      unUsed = 0;
    }


    public  SipperTimeStamp (string  timeStr)
    {
      hour   = 0;
      minute = 0;
      second = 0;

      string[]  fields = timeStr.Split (':');
      if  (fields.Length != 3)
        return;
      
      hour = PicesKKStr.StrToByte (fields[0]);
      if  ((hour < 0)  ||  (hour > 24))
      {
        hour = 0;
        return;
      }

      minute = PicesKKStr.StrToByte (fields[1]);
      if  ((minute < 0)  ||  (minute > 60))
      {
        minute = 0;
        return;
      }

      second = PicesKKStr.StrToByte (fields[2]);
      if  ((second < 0)  || (second > 60))
      {
        second = 0;
        return;
      }
    }



    public  int  Hour   {get {return  hour;}}
    public  int  Minute {get {return  minute;}}
    public  int  Second {get {return  second;}}
    
  
    public  bool  Valid  ()
    {
      if  ((hour > 24)  ||  (minute > 60)  ||  (second > 60))
        return  false;
      return  true;
    }

      
    static public  int  BuffSpaceNeeded ()
    {
      return  4;
    }  /* BuffSpaceNeeded */


      
    public  void  FillBuffer (byte[]   buffer,
                              ref int  nextIndex
                             )
    {
      buffer[nextIndex] = second;  nextIndex++;
      buffer[nextIndex] = minute;  nextIndex++;
      buffer[nextIndex] = hour;    nextIndex++;
      buffer[nextIndex] = unUsed;  nextIndex++;
    }  /* FillBuffer */

  }  /* SipperTimeStamp */
}
