using System;
using System.Collections.Generic;
using System.Text;

namespace SipperSimulator
{
  class SerialPortBuffer
  {
    private  int    maxBuffSize = 512;
    private  char[] buff = null;
    private  int    begPtr = 0;
    private  int    endPtr = -1;
    private  int    spaceUsed = 0;
    
    
    
    public  SerialPortBuffer  (int  _maxBuffSize)
    {
      maxBuffSize = _maxBuffSize;
      buff = new char[maxBuffSize];
      
      begPtr = maxBuffSize - 1;
      endPtr = 0;
      spaceUsed = 0;
    }
    
    
    
    public int  SpaceUsed ()  
    {
      int  x;
      
      lock (this)
      {
        x = spaceUsed;
      }
      return x;
    }   /* SpaceUsed */ 
    
    
    
    
    public  int  AvailablSpace ()
    {
      int  x = 0;
      lock (this)
      {
        x = maxBuffSize - spaceUsed;
      }
      
      return  x;
    }  /* AvailablSpace */




    public  void  WriteChar (char  ch)
    {
      lock (this)
      {
        if  (spaceUsed >= maxBuffSize)
        {
          // Buffer overflow,  data is lost
        }
        else
        {
          begPtr++;
          if  (begPtr >= maxBuffSize)
            begPtr = 0;
          buff[begPtr] = ch;
          spaceUsed++;
        }
      }
    }  /* WriteChar */
    


    public  char  ReadChar ()
    {
      char  ch = (char)0;
      
      lock (this)
      {
        if  (spaceUsed < 1)
        {
          // request to rad data when none available
         ch = (char)0;
        }
        else
        {
          ch = buff[endPtr];
          endPtr++;
          if  (endPtr >= maxBuffSize)
            endPtr = 0;
      
          spaceUsed--;
        }
      }
      
      return  ch;
    }  /* ReadChar */    
  }
}
