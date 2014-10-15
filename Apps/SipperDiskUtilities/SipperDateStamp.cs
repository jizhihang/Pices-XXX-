using System;
using System.Collections.Generic;
using System.Text;
using PicesInterface;


namespace SipperDiskUtilities
{
  class  SipperDateStamp
  {
    byte  dayOfWeek;
    byte  day;
    byte  month;
    byte  year;


    public  SipperDateStamp (DateTime  dt)
    {
      dayOfWeek = (byte)dt.DayOfWeek;
      day       = (byte)dt.Day;
      month     = (byte)dt.Month;
      year      = (byte)dt.Year;

    }
    
    
    public  SipperDateStamp (byte[]   buffer,
                             ref int  nextIntex
                            )
    {
      dayOfWeek   = buffer[nextIntex];  nextIntex++;
      day  = buffer[nextIntex];  nextIntex++;
      month = buffer[nextIntex];  nextIntex++;
      year  = buffer[nextIntex];  nextIntex++;
    }


    public  SipperDateStamp (string  dateStr)
    {
      dayOfWeek   = 1;
      day  = 1;
      month = 1;
      year  = 8;

      bool   validDate = true;

      // Expect string in format of  'mm/dd/yy'
      string[]  fields = dateStr.Split ('/');
      if  (fields.Length != 3)
        return;

      month = PicesKKStr.StrToByte (fields[0]);
      if  ((month < 1)  ||  (month > 12))
        validDate = false;

      day = PicesKKStr.StrToByte (fields[1]);
      if  ((day < 1)  ||  (day > 31))
        validDate = false;

      uint  xxx = PicesKKStr.StrToUint (fields[2]);
      if  (xxx > 2000)
      {
        xxx = xxx - 2000;
        if  (xxx < 100)
          year = (byte)xxx;
      }
      if  (!validDate)
      {
        DateTime dt = DateTime.Now;
        month = 1;
        day  = 1;
        year  = 1;
      }

      return;
    }


    

    public  int  Year   {get {return  ((int)year + 2000);}}
    public  int  Month  {get {return  month;}}
    public  int  Day    {get {return  day;}}

    

    public  bool  Valid  ()
    {
      if  ((year > 99) ||  (month > 12)  ||  (day > 31))
        return false;
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
      buffer[nextIndex] = dayOfWeek;   nextIndex++;
      buffer[nextIndex] = day;         nextIndex++;
      buffer[nextIndex] = month;       nextIndex++;
      buffer[nextIndex] = year;        nextIndex++;
        
    }  /* FillBuffer */
                                    

  }  /* SipperDateStamp */

}
