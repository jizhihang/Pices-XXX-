using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace SipperDiskUtilities
{
  public  class  SipperFileControlBlock
  {
    /* FCB_HEADER */
    UInt32  allocated;
    UInt32  status;
    UInt32  startSector;
    UInt32  endSector;
    UInt32  currentAccessSector;

    public  const uint    SIPPER_FILE_FREE       = 0;
    public  const uint    SIPPER_FILE_ALLOCATED  = 0xFFFFFFFF;
    public  const uint    SIPPER_FILE_CLOSED     = 0;
    public  const uint    SIPPER_FILE_OPEN       = 1;
    
    
    /* SIPPER_FILE_INFO */
    char[]               fileName      = new char[Constants.SIPPER_FILENAME_SIZE];
    SipperDateTimeStamp  dateTimeStamp = null;
    char[]               description   = new char[Constants.SIPPER_FILE_DESCRIPTION_SIZE];

    /* Unused Space */
    byte[]               unUsedSpace   = null;


    //  
    private bool         downLoaded    = false;  // Used by SipperDiskManager to flag some files
                                                 // as downloaded alrady.

    private int          fcbIndex      = 0;      // Index  in Sipper Directory Table (0 - 2047)
                                                 
    public  UInt32    Allocated           {get {return  allocated;}     set  {allocated  = value;}}
    public  UInt32    Status              {get {return  status;}        set  {status     = value;}}
    
    public  string    Desccription        {get {return  new string (description).Trim ();}}
    public  bool      DownLoaded          {get {return  downLoaded;}    set  {downLoaded = value;}}
    public  uint      EndSector           {get {return  endSector;}     set  {endSector  = value;}}
    public  int       FcbIndex            {get {return  fcbIndex;}      set  {fcbIndex = value;}}
    public  long      FileSizeInBytes     {get {return  (long)(1 + endSector - startSector) * (long)Constants.LOGICAL_BLOCK_SIZE;}}
    public  long      FileSizeInSectores  {get {return  (1 + endSector - startSector);}}
    public  uint      StartSector         {get {return  startSector;}   set  {startSector = value;}}


    public  void  SetSipperDateTimeStamp (SipperDateTimeStamp  _dateTimeStamp)
    {
      dateTimeStamp = _dateTimeStamp;
    }


    public  DateTime  TimeStamp           
    {
      get 
      {
        if  (dateTimeStamp == null)
          return DateTime.Now;
        else
          return  dateTimeStamp.DotNetDateTime;
      }
    }



    public  string    FileName            
    {
      get 
      {
        return  new string (fileName).Trim (' ','\0');
      }
      set
      {
        fileName = value.ToCharArray ();
      }
    }      




    public  SipperFileControlBlock (byte[]   buffer,
                                    ref int  nextIndex
                                   )
    {
      int  x = 0;
        
      int  indexStart = nextIndex;
        
      allocated           = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);
      status              = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);
      startSector         = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);
      endSector           = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);
      currentAccessSector = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);

      for  (x = 0;  x < fileName.Length;  x++)
      {
        fileName[x] = (char)buffer[nextIndex];  nextIndex++;
      }
        
      dateTimeStamp = new SipperDateTimeStamp (buffer, ref nextIndex);
        
      for  (x = 0;  x < description.Length;  x++)
      {
        description[x] = (char)buffer[nextIndex];  nextIndex++;
      }
        
      int  indexEnd = nextIndex;
        
      int  unUsedSpaceLen = Constants.LOGICAL_BLOCK_SIZE - (indexEnd - indexStart);
        
      unUsedSpace = new byte[unUsedSpaceLen];
      for  (x = 0;  x < unUsedSpace.Length;  x++)
      {
        unUsedSpace[x] = buffer[nextIndex];  nextIndex++;
      }
    }
     
     
     
    static  public  SipperFileControlBlock  CreateFromFileStream (FileStream  fs)
    {
      int  spaceNeeded =  BuffSpaceNeeded ();
      byte[]  buffer = new byte[spaceNeeded];
      fs.Read (buffer, 0, buffer.Length);
      int  nextIndex = 0;
      return  new SipperFileControlBlock (buffer, ref nextIndex);
    }  /* CreateFromFileStream */
      
      

    //*************************************************************************************************
    //* Construct an empty 'SipperFileControlBlock' that would be used when initializing the SIPPER   *
    //* directory system.                                                                             *
    //*************************************************************************************************
    public  SipperFileControlBlock ()
    {
      allocated            = SIPPER_FILE_FREE;
      status               = SIPPER_FILE_CLOSED;
      startSector          = 0;
      endSector            = 0;
      currentAccessSector  = 0;
      
      int  x = 0;
      string  fileNameStr = "NO FILE";
      for (x = 0;  x < fileName.Length;  x++)
      {
        if  (x < fileNameStr.Length)
          fileName[x] = fileNameStr[x];
        else
          fileName[x] = (char)0;
      }
      
      string  descriptionStr = "NO DESCRIPTION";
      for (x = 0;  x < description.Length;  x++)
      {
        if  (x < descriptionStr.Length)
          description[x] = descriptionStr[x];
        else
          description[x] = (char)0;
      }
    }  /* SipperFileControlBlock */



    public  bool  Equals (SipperFileControlBlock  other)
    {
      if  ((allocated           != other.allocated)    ||
           (status              != other.status)       ||
           (startSector         != other.startSector)  ||
           (endSector           != other.endSector)    ||
           (currentAccessSector != other.currentAccessSector)
          )
        return  false;
      else 
        return  true;
    }  /* Equals */




    static public  int  BuffSpaceNeeded ()
    {
      return  Constants.LOGICAL_BLOCK_SIZE;
    }

      
      
    public  void  FillBuffer (byte[]   buffer,
                              ref int  nextIndex
                             )
    {
      int  indexStart = nextIndex;

      int  x = 0;
      Constants.FillBuffer (buffer, ref nextIndex, allocated);
      Constants.FillBuffer (buffer, ref nextIndex, status);
      Constants.FillBuffer (buffer, ref nextIndex, startSector);
      Constants.FillBuffer (buffer, ref nextIndex, endSector);
      Constants.FillBuffer (buffer, ref nextIndex, currentAccessSector);

      for  (x = 0;  x < fileName.Length;  x++)
      {
        buffer[nextIndex] = (byte)fileName[x];  nextIndex++;
      }
        
      if  (dateTimeStamp == null)
        dateTimeStamp = new SipperDateTimeStamp (DateTime.Now);
      
      dateTimeStamp.FillBuffer (buffer, ref nextIndex);
        
      for  (x = 0;  x < description.Length;  x++)
      {
        buffer[nextIndex] = (byte)description[x];  nextIndex++;
      }
        
      if  (unUsedSpace != null)
      {
        for  (x = 0;  x < unUsedSpace.Length;  x++)
        {
          buffer[nextIndex] = unUsedSpace[nextIndex];  nextIndex++;
        }
      }

      int  indexEnd = nextIndex;
      int  unUsedSpaceLen = Constants.LOGICAL_BLOCK_SIZE - (indexEnd - indexStart);

      for  (x = 0;  x < unUsedSpaceLen;  x++)
      {
         buffer[nextIndex] = 0;  nextIndex++;
      }
    }  /* FillBuffer */
    
    


    //**************************************************************************
    //*   Returns user message that is to be added to begining of SIPPER file. *
    //**************************************************************************
    public  string  UserMessage ()
    {
      string  userMessage = "";
      
      int  x = 0;
      
      for  (x = 0;  x < fileName.Length;  x++)
      {
        char c = fileName[x];
        if  (c != '\0')
          userMessage += c;
      }
      
      userMessage += "\t"
                  + dateTimeStamp.DotNetDateTime.ToString ("u")
                  + "\t";
                  
      for  (x = 0;  x < description.Length;  x++)               
      {
        char  c = description[x];
         if  (c != '\0')
           userMessage += c;
      }
                  
      userMessage += '\0';

      return  userMessage;
    }  /* UserMessage */





    public  void  Write (FileStream  fs)
    {
      int  nextIdx = 0;
      byte[]  buffer = new byte[BuffSpaceNeeded ()];
      FillBuffer (buffer, ref nextIdx);
      fs.Write (buffer, 0, buffer.Length);
    }  /* Write */




  }  /* SipperFileControlBlock */
    
}
