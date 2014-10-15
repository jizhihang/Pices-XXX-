using System;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Text;
using System.Xml;
using System.IO;

using PicesInterface;
using SipperFile;


namespace SipperInterface
{
  /// <summary>
  /// Summary description for Bufer.
  /// </summary>
  public class CircularBuffer
  {
    public  enum  Operation  {GetCammeraLine,
                              GetSampleSipperData,
                              RecordStart,
                              RecordEnd,
                              SetFileName
                             };




    private Socket                 socket                  = null;
    private bool                   statusConnected         = false;


    private bool                   blockIsSet              = false;   // Used to insure thread syncronization
                                                                      // access to socket; since more than one
                                                                      // thread will be using it.
    private int                    buffSize;
    private byte[]                 buffer;
    private int                    end;
    private bool                   echoText;
    
    private StreamWriter           logFile                 = null;
    private CommandWindow          mainForm                = null;
    private Queue<SipperCommand>   sipperCmdsPending       = null;
    private SipperCommand          sipperCmdBeingExecuted  = null;
    private SipperConfigRec        sipperConfigRec         = null;
    private SipperHeaderRec        headerRec               = null;
    private int                    start;
    private string                 txtForMainForm;

    private Thread                 receiveThread           = null;
    private bool                   receiveThreadRunning    = false;
    bool                           receiveThreadTerminate  = false;

    private  Thread                dataLoopThread           = null;
    private  bool                  dataLoopRunning          = false;
    private  bool                  dataLoopTerminate        = false;

    private  InstrumentDataManager instrumentManager        = null;
    private  bool                  instrumentDataStart      = false;
    private  bool                  instrumentDataStarted    = false;
    private  bool                  instrumentDataCollecting = true;
    private  bool                  instrumentDataEnd        = false;
    private  bool                  instrumentDataEnded      = false;


    private  int                   inclinationSerailPort    = -1;


    private  InstrumentData        currentDataRow           = null;
    private  InstrumentDataList    history                  = null;   // We will not own this;  bit will be only
                                                                      // process adding data to it.
    private  InstrumentData        lastDataRow              = null;
    private  RecordingRate         recordingRate            = null;

    private  int                   instrumentsMaxNum        = 6;

    private  string                lastPrompt = "";


    private long   startBlockAddr  = 0;
    private long   curBlockAddr    = 0;
    private long   endBlockAddr    = 0;
    private float  flowRate        = 0.0f;  // Updated by 'ParseStatusResults'
    private bool   statusRecording = false;




    public  void  EchoText (bool  _echoText)  {echoText = _echoText;}

    public  bool  EchoText () {return  echoText;}

    public  bool  ThreadsTerminated ()  {return  ((!receiveThreadRunning)  &&  (!dataLoopRunning));}




    public  long   CurBlockAddr    ()  {return curBlockAddr;}
    public  long   EndBlockAddr    ()  {return endBlockAddr;}
    public  float  FlowRate        ()  {return flowRate;}
    public  long   StartBlockAddr  ()  {return startBlockAddr;}
    public  bool   StatusConnected ()  {return statusConnected;}
    public  bool   StatusRecording ()  {return statusRecording;}
    
    public  bool   InstrumentDataCollecting ()  {return  instrumentDataCollecting;}




    public CircularBuffer (SipperConfigRec     _sipperConfigRec,
                           int                 _buffSize,
                           CommandWindow       _mainForm,
                           InstrumentDataList  _history,
                           InstrumentData      _currentDataRow,
                           StreamWriter        _logFile
                          )
    {
      sipperConfigRec         = _sipperConfigRec;
      buffSize                = _buffSize;
      mainForm                = _mainForm;
      history                 = _history;
      currentDataRow          = _currentDataRow;
      logFile                 = _logFile;
      sipperCmdsPending       = new Queue<SipperCommand> ();
      sipperCmdBeingExecuted  = null;
      buffer                  = new byte[buffSize];
      start                   = 0;
      end                     = buffSize - 1;

      headerRec = sipperConfigRec.HeaderRec();

      echoText = true;
      txtForMainForm = "";

      receiveThreadTerminate  = false;


      try
      {
        ConnectSocket ();
      }
      catch  (Exception  e)
      {
        throw e;
      }

      //Setting up thread that will just read from the buffer
      receiveThread = new Thread (new ThreadStart (StartReceive));
      receiveThread.Start ();

      StartDataLoop ();
    }



   public  void  Dispose ()
   {
     ShutDownThreads ();

     instrumentManager.Dispose ();
     instrumentManager = null;

     if  (socket != null)
     {
       if  (socket.Connected)
         socket.Disconnect (false);
       socket.Close ();
       socket = null;
     }
   }  /* Dispose */






   private void   ConnectSocket ()
   {
     IPAddress ipAddress = sipperConfigRec.IpAddress ();
     int       ipPort    = sipperConfigRec.IpPort ();

     lock (logFile)
     {
       logFile.WriteLine ("//");
       logFile.WriteLine ("// ConnectSocket  ipAddress[" + ipAddress.ToString () +"]  Port[" + ipPort.ToString () + "]");
       logFile.WriteLine ("//");
       logFile.Flush ();
     }

     try
     {
       //creating socket and connecting to IP address

       socket = new Socket (AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
       IPEndPoint EPhost = new IPEndPoint (ipAddress, ipPort);
       
       socket.Connect (EPhost);	//actual physical connection

       if (socket.Connected)
       {
         statusConnected = true;

         lock (logFile)
         {
           logFile.WriteLine ("// ConnectSocket Connected Successfully");
           logFile.WriteLine ("//");
           logFile.Flush ();
         }
       }
       else
       {
         MessageBox.Show ("The connection to Sipper has been lost.  Please check the connections "+
                           "and try again.  If all else fails, disconnect and reconnect."
                         );
         statusConnected = false;
       }
     }
     catch (Exception e1)
     {
       statusConnected = false;
       throw new Exception ("Error connecting to SIPPER", e1);
     }
   }  /* ConnectSocket */




    public double  RecordingRateMegPerSec ()
    {
      if  (recordingRate == null)
        return 0.0f;
        
      return recordingRate.MegaBytesPerSec ();
    }



    public  void  QueueSipperCommand (SipperCommand cmd)
    {
      lock  (sipperCmdsPending)
      {
        sipperCmdsPending.Enqueue (cmd);
      }
    }  /* QueueSipperCommand */






    private  SipperCommand  GetNextSipperCommand ()
    {
      SipperCommand  cmd = null;
      lock (sipperCmdsPending)
      {
        if  (sipperCmdsPending.Count > 0)
          cmd = sipperCmdsPending.Dequeue ();
          
        sipperCmdBeingExecuted = cmd;
      }
      
      return  cmd;
    } /* GetNextSipperCommand */


   

    public  int  SipperCmdsPendingCount ()
    {
      int  count = 0;
      lock  (sipperCmdsPending)
      {
        count  = sipperCmdsPending.Count;
      }

      return  count;
    }  /* SipperCmdsPendingCount */




    public  SipperCommand  SipperCmdBeingExecuted ()
    {
      SipperCommand  result = null;
      lock (sipperCmdsPending)
      {
        result = sipperCmdBeingExecuted;
      }

      return result;
    }  /* SipperCmdBeingExecutedCount */




    private  int  BuffSpaceUsed ()
    {
      int  xxEbuffer = end;
      int  xxSbuffer = start;

      if  ((xxSbuffer - xxEbuffer) == 1)
        return 0;

      if  ((xxSbuffer == 0)  &&  (xxEbuffer == (buffSize - 1)))
        return 0;

      if  (xxSbuffer <= xxEbuffer)
        return  1 + xxEbuffer - xxSbuffer;


      return  (buffSize - xxEbuffer) + (xxSbuffer + 1);
    }



    public Boolean  Empty ()
    {
      int  bBuffSpaceUsed = 0;
      lock (this)
      {
        bBuffSpaceUsed = BuffSpaceUsed ();
      }
      return  bBuffSpaceUsed == 0;
    }



    public Boolean  Full ()
    {
      int  buffSpaceLeft = 0;
      lock (this)
      {
        buffSpaceLeft =  buffSize - BuffSpaceUsed ();
      }

      return  (buffSpaceLeft < 5);
    }  /* buffFull */



    public byte GetChar ()
    {
      try 
      {
        Byte tchar;	

        while (Empty ())
          Thread.Sleep (10);

        lock (this)
        {
          tchar = buffer [start];

          start++;

          if  (start >= buffSize)
            start  = 0;
        }

        return  tchar;
      }
      catch  (Exception  getCharException)
      {
        MessageBox.Show ("Exception in GetChar[" + getCharException.ToString () + "]");
        return  0;
      }
    }  /* GetChar */





    public char  PeekChar ()
    {
      byte  ch = 0;
      
      lock  (this)
      {
        if  (!Empty ())
          ch = buffer[start];
      }
      
      return  (char)ch;
    } /* PeekChar */





    public void  PutChar (byte tchar)
    {
      while (Full ())
        Thread.Sleep (10);

      lock  (this)
      {
        end++;
        if  (end >= buffSize)
          end = 0;

        buffer[end] = tchar;
      }

    }  /* PutChar */




    public string  GetText (int timeToWait)
    {
      String  result = GetTextNoEcho (timeToWait);
      //mainForm.WriteToTextWindow (result);
      return result;
    }  /* GetText */





    public string  GetTextNoEcho (int timeToWait)
    {
      lastPrompt = "";
    
      BlockSet ();

      char lastChar1=' ', lastChar2=' ', lastChar3 = ' ';
      string result="";
      int  totalTimeWaited = 0;

      Boolean carrotsFound = false;

      while (!carrotsFound && (totalTimeWaited < timeToWait))
      {
        if  (Empty())
        {
          Thread.Sleep (100);
          totalTimeWaited += 100;
        }
        else
        {
          lastChar3 = lastChar2;
          lastChar2 = lastChar1;
          lastChar1 = (char)GetChar ();
          result += lastChar1.ToString ();

          if  (Empty ())
          {
            if ((lastChar1 == '>') && (lastChar2 == '>'))
            {
              carrotsFound = true;
              result = result.Substring (0, result.Length - 2);
              lastPrompt = ">>";
            }

            else 
            if  ((lastChar1 == ' ') && (lastChar2 == '>')  &&  (lastChar3 == '>'))
            {
              carrotsFound = true;    
              result = result.Substring (0, Math.Max (0, result.Length - 3));
              lastPrompt = ">> ";
            }
            
            else
            if  ((lastChar1 == '>') && (lastChar2 == 'K')  &&  (lastChar3 == 'O'))
            {
              carrotsFound = true;    
              result = result.Substring (0, Math.Max (0, result.Length - 3));
              lastPrompt = "OK>";
            }
          }
        }
      }
      
      BlockRelease ();

      return result;
    }  /* GetTextNoEcho */





   private  void  WaitForPrompt ()
   {
     bool  promptFound = false;
     FlushReceiveBufer ();

     do
     {
       SendText ("");
       Thread.Sleep (50);
       string  ln = GetText (2000);
       promptFound = ((lastPrompt == ">> ")  ||  (lastPrompt == ">>") ||  (lastPrompt == "OK>"));
     }  
     while  (!promptFound);
   }  /* WaitForPrompt */


   
   
   public  void  TurnOnCameraMode ()
   {
     WaitForPrompt ();

     int  loopCount = 0;
     
     while  (!receiveThreadTerminate)
     {
       SendText ("sipper_camera");
       
       string s1 = GetCameraLineText (2000);

       if  (lastPrompt == "OK>")
       {
         // We made it into camera mode
         break;
       }

       // But sometimes after sending the camera comand you have to send a blank line to
       // get th eprompt to display.
       SendText ("");
         
       if  (receiveThreadTerminate)
         break;
       
       string s = GetCameraLineText (2000);
       
       if  (lastPrompt == "OK>")
       {
         // We are in camera mode
         break;
       }
       
       //else if  (lastPrompt != ">> ")
       //{
       //  SendText ("");
       //  s = GetCameraLineText (1000);
       //}
       
       ++loopCount;
       if  (loopCount > 3)
         break;
     }
   }  /* TurnOnCameraMode */



   
   public void  TurnOffCameraMode ()
   {
     SendText ("");
     FlushReceiveBufer ();
     SendText ("exit");
     Thread.Sleep (100);
     SendText ("");
     Thread.Sleep (100);
     FlushReceiveBufer ();
   }  /* TurnOnCameraMode */



   public  string   GetCameraLineText (int timeToWait)
   {
     lastPrompt = "";
     char lastChar1=' ', lastChar2=' ', lastChar3 = ' ';
     string result="";
     int totalTimeWaited = 0;

     Boolean cammeraPromptFound = false;

     while (!cammeraPromptFound && (totalTimeWaited < timeToWait))
     {
       if (Empty ())
       {
         Thread.Sleep (100);
         totalTimeWaited += 100;
       }
       else
       {
         totalTimeWaited = 0;
         lastChar3 = lastChar2;
         lastChar2 = lastChar1;
         lastChar1 = (char)GetChar ();
         result += lastChar1.ToString ();

         if (Empty ())
         {
           if ((lastChar1 == '>') && (lastChar2 == 'K')  &&  (lastChar3 == 'O'))
           {
             cammeraPromptFound = true;
             result = result.Substring (0, result.Length - 3);
             lastPrompt = "OK>";
           }
         }
       }
     }
     
     return  result;
   }  /* GetCameraText */



    public  string  GetCameraParametersStr ()
    {
      TurnOnCameraMode ();
      
      SendText ("gcp");

      string cameraParameterStr = GetCameraLineText (500);
      
      TurnOffCameraMode ();
      
      return  cameraParameterStr;
    }  /* GetCameraParametersStr */




    public  byte[]  GetCameraLineData (int timeToWait)
    {
      char lastChar1=' ', lastChar2=' ', lastChar3 = ' ';
      string result="";
      int totalTimeWaited = 0;

      Boolean carrotsFound = false;

      while (!carrotsFound && (totalTimeWaited < timeToWait))
      {
        if (Empty ())
        {
          Thread.Sleep (100);
          totalTimeWaited += 100;
        }
        else
        {
          totalTimeWaited = 0;
          lastChar3 = lastChar2;
          lastChar2 = lastChar1;
          lastChar1 = (char)GetChar ();
          result += lastChar1.ToString ();

          if (Empty ())
          {
            if ((lastChar1 == '>') && (lastChar2 == 'K')  &&  (lastChar3 == 'O'))
            {
              carrotsFound = true;
              result = result.Substring (0, result.Length - 5);
            }
          }
        }
      }
      
      if  (result.Length > 1000)
        return  ParseCameraLineText (result);
      else 
        return null;
    }  /* GetCameraText */



    private  int  StrToInt (string  s)
    {
      int i = 0;
      try  {i = int.Parse (s);}
      catch (Exception)  {i = 0;}
      return  i;
    }



    ArrayList  ParseStrInToFields (string  s)
    {
       ArrayList  fields = new ArrayList ();
       int  x = 0;
       
       while  (x < s.Length)
       {
         // Skip any leading white space characters
         while  ((x < s.Length)  &&  (s[x] == ' '))
           x++;
           
         if  (x >= s.Length)
           break;

         string  f = "";
         while  ((x < s.Length)  &&  (s[x] != ' '))
         {
           f += s[x];
           x++;
         }
         
         fields.Add (f);
       }
       
       return  fields;
       
    }  /* ParseStrInToFields */





    private  byte[]  ParseCameraLineText (string  s)
    {
      string[]  lines = s.Split ('\n');
      
      int  lineNum = 0;
      
      byte[]  data = new byte[4096];
      
      for  (lineNum = 0;  lineNum < lines.Length;  lineNum++)
      {
        ArrayList fields = ParseStrInToFields (lines[lineNum].TrimEnd ());
        if  (fields.Count < 9)
          continue;
          
        string  firstField = (string)fields[0];
        if  ((!firstField.EndsWith (":"))  ||  (firstField.Length > 5))
          continue;

        int byteOffset = StrToInt (firstField.Substring (0, firstField.Length - 1)) - 1;  // Retuned data is 1 based so need to subtract 1.
        
        if  ((byteOffset < 0)  ||  (byteOffset > 4088))
          continue;
          
        for  (int x = 1;  x < fields.Count;  x++)
        {
          int  intensity = StrToInt ((string)fields[x]);
          if  ((intensity < 0)  ||  (intensity > 255))
            continue;
          data[byteOffset + x - 1] = (byte)intensity;
        }
      }
        
      return  data;
    }  /* ParseCameraLineText */






    public string  GetTextOneLine (int timeToWait)
    {
      BlockSet ();
    
    
      char    lastChar1       = ' ';
      Boolean lineFeedFound   = false;
      string  result          = "";
      int     totalTimeWaited = 0;

      while ((!lineFeedFound)  &&  (totalTimeWaited < timeToWait))
      {
        if  (Empty())
        {
          Thread.Sleep (100);
          totalTimeWaited += 100;
        }
        else
        {
          lastChar1 = (char)GetChar ();
          if  (lastChar1 == '\n') 
          { 
            byte  x;
            lineFeedFound = true;
            char  nextChar = PeekChar ();  
            if  (nextChar == '\r')
              x = GetChar ();
          }
            
          else if  (lastChar1 == '\r')
          {
            byte x;
            lineFeedFound = true;
            char nextChar = PeekChar();
            if (nextChar == '\f')
              x = GetChar();
          }
            
          else
            result += lastChar1.ToString ();
        }
      } 


      BlockRelease ();
      return result;
    }  /* GetTextOneLine */





    public  byte[]  GetBinaryData (int numBytesToRetrieve)
    {
      BlockSet ();
    
      byte[]  dataBlock = new byte[numBytesToRetrieve];
      dataBlock.Initialize ();
      int  bytesRetrieved = 0;

      Thread.Sleep (10);

      int sleepcount = 0;

      bool checkIfEnoughTimeHasElapsed = false;
      DateTime  lastTimeStatusMsgSent = DateTime.Now;

      while ((sleepcount < 100)  &&  (bytesRetrieved < numBytesToRetrieve))
      {
        if  (Empty ())
        {
          Thread.Sleep (100);
          sleepcount++;
          checkIfEnoughTimeHasElapsed = true;
        }
        else
        {
          sleepcount = 0;
          dataBlock[bytesRetrieved] = GetChar ();
          bytesRetrieved++;

          if  ((bytesRetrieved % 4096) == 0)
            checkIfEnoughTimeHasElapsed = true;
        }

        if (checkIfEnoughTimeHasElapsed)
        {
          checkIfEnoughTimeHasElapsed = false;
          DateTime zed = DateTime.Now;
          TimeSpan ts = (zed - lastTimeStatusMsgSent);
          if (ts.Seconds > 4)
          {
            mainForm.WriteToTextWindow ("// Retrieved " + bytesRetrieved.ToString("###,##0") + " bytes of " + numBytesToRetrieve.ToString("###,##0"));
            lastTimeStatusMsgSent = DateTime.Now;
          }
        }
      }

      if (bytesRetrieved < numBytesToRetrieve)
      {
        MessageBox.Show ("Not all data was received!   Expected[" + numBytesToRetrieve + "]  Received[" + bytesRetrieved + "]");
        byte[]  newDataBlock = new byte[bytesRetrieved];

        for  (int x = 0;  x < bytesRetrieved;  x++)
          newDataBlock[x] = dataBlock[x];

        dataBlock = newDataBlock;
      }

      BlockRelease ();

      return  dataBlock;
    }  /* GetBinaryData */

  


    private void StartReceive ()
    {
      int ret = 0;
      receiveThreadTerminate = false;
      receiveThreadRunning   = true;

      Byte[] receive = new Byte[10000];

      int  sleepsInARow = 0;

      while (!receiveThreadTerminate)
      {
        bool  conectionNotAvaiable = false;
        if  (socket == null)
          conectionNotAvaiable = true;
        else 
          conectionNotAvaiable = !socket.Connected;
          
        if  (conectionNotAvaiable)
        {
          Thread.Sleep (100);
          continue;
        }
          
        // Peek to see if there is data to receive.
        int  amtDataAvailable = 0;
          
        //BlockSet ();
        try
        {
          amtDataAvailable = socket.Available;
        }
        catch  (Exception)
        {
          amtDataAvailable = 0;
          socket.Close ();
          socket = null;
        }
        
        if  ((socket != null)  &&  (amtDataAvailable > 0))
        {
          try
          {
            ret = socket.Receive (receive, 0, receive.Length, 0);
          }
          catch (Exception)
          {
            socket.Close ();
            socket = null;
          }
        }

        //BlockRelease ();

        if  (socket == null)
          continue;

        if  (amtDataAvailable <= 0)
        {
          // We will sleep for a very short bit.
          if  (sleepsInARow < 5)
            Thread.Sleep (10);
          else
            Thread.Sleep (100);

          sleepsInARow++;
          
          continue;
        }
 
        //Putting data into buffer array			
        for (int a = 0;  a < ret;  a++)
          PutChar (receive[a]);
 
        if  (echoText)
        {
          for  (int a = 0;  a < ret;  a++)
            txtForMainForm += (char)receive[a];
 
          mainForm.WriteToTextWindow (txtForMainForm);
          txtForMainForm = "";
        }

        sleepsInARow = 0;
      }


      receiveThreadRunning = false;
    }  /* StartReceive */



    public  int  SendText (string  txt)
    {
      int  ret = 0;
      BlockSet ();
      ret = SendTextNoBlock (ref txt);
      BlockRelease ();
      return  ret;
    }  /* SendText */



    private  int  SendTextNoBlock (ref string txt)
    {
      int ret = 0;
      lock (this)
      {
        Byte[] bBuf;
        string buf;
        try
        {
          // buf = String.Format ("{0}", txt + "\r");
          // bBuf = Encoding.ASCII.GetBytes (buf);
          // ret = socket.Send (bBuf, 0, bBuf.Length, SocketFlags.None);

          buf = String.Format ("{0}", txt + "\r");
          bBuf = Encoding.ASCII.GetBytes (buf);
          //ret = socket.Send (bBuf, 0, bBuf.Length, SocketFlags.None);
          
          for  (int zed = 0;  zed < bBuf.Length;  zed++)
          {
            Thread.Sleep (8);
            ret = socket.Send (bBuf, zed, 1, SocketFlags.None);
          }
        }
        catch (Exception)
        {
          ret = 0;
        }
      }

      return ret;
    }  /* SendTextNoBlock */



    private  void  BlockSet ()
    {
      bool  weAreBlocked = true;

      while (weAreBlocked)
      {
        lock (this)
        {
          if  (blockIsSet)
          {
            weAreBlocked = true;
          }
          else
          {
            weAreBlocked = false;
            blockIsSet = true;
          }
        }

        if (weAreBlocked)
          Thread.Sleep (10);
      }
    }  /* BlockSet */




    private  void  BlockRelease ()
    {
      if  (!blockIsSet)
        return;
        
      lock (this)
      {
        blockIsSet = false;
      }

    }  /* BlockRelease */



    public  void  FlushReceiveBufer ()
    {
      while (!Empty ())
      {
        GetText (200);
        Thread.Sleep (10);
      }
    }    /* FlushReceiveBufer */


   
    public  string  SubmitCommandAndReturnResults (string  cmd,
                                                   int     timeToWait
                                                  )
    {
      BlockSet ();
       
      SendTextNoBlock (ref cmd);

      char lastChar1=' ', lastChar2=' ', lastChar3 = ' ';
      string result = "";
      int totalTimeWaited = 0;

      Boolean  carrotsFound = false;

      while (!carrotsFound && (totalTimeWaited < timeToWait))
      {
        if  (Empty ())
        {
          Thread.Sleep (50);
          totalTimeWaited += 50;
        }
        else
        {
          totalTimeWaited = 0;
          lastChar3 = lastChar2;
          lastChar2 = lastChar1;
          lastChar1 = (char)GetChar ();
          result += lastChar1.ToString ();

          if (Empty ())
          {
            if ((lastChar1 == '>') && (lastChar2 == '>'))
            {
              lastPrompt = ">>";
              carrotsFound = true;
              result = result.Substring (0, result.Length - 2);
            }

            else
            if  ((lastChar1 == ' ')  &&  (lastChar2 == '>')  &&  (lastChar3 == '>'))
            {
              lastPrompt = ">> ";
              carrotsFound = true;
              result = result.Substring (0, result.Length - 3);
            }
            
            else
            if  ((lastChar1 == '>')  &&  (lastChar2 == 'K')  &&  (lastChar3 == 'O'))
            {
              lastPrompt = "OK>";
              carrotsFound = true;
              result = result.Substring(0, result.Length - 3);
            }
          }
        }
      }
       
      BlockRelease ();
       
      return  result;      
    }  /* SubmitCommandAndReturnResults */





    private  void  StartDataLoop ()
    {
      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//  'StartDataLoop'   CurTime[" + System.DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      dataLoopThread = new Thread (new ThreadStart (DataLoop));
      dataLoopThread.Start ();
      
      {
        // Wait till dataLoop starts up;  or at least 2 seconds.
        for  (int x = 0;  (x < 200)  &&  (!dataLoopRunning);  x++)
        {
          Thread.Sleep (10);
        }
      }

      lock  (logFile)
      {
        if  (dataLoopRunning)
        {
          logFile.WriteLine ("//  'StartDataLoop'  sucessfully started  CurTime[" + System.DateTime.Now.ToString () + "]");
        }
        else
        {
          logFile.WriteLine ("//");
          logFile.WriteLine ("//  'StartDataLoop'  *** DID NOT START YET ***    CurTime[" + System.DateTime.Now.ToString () + "]");
          logFile.WriteLine ("//");
        }
        logFile.Flush ();
      }
      
    }  /* StartDataLoop */



    private void   DataLoop ()
    {
      lock (this)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//DataLoop    Starting Data Loop");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      headerRec = sipperConfigRec.HeaderRec ();

      string logFileDir = PicesSipperVariables.SipperInterfaceLogDir ();
      if (logFileDir == "")
        logFileDir = Directory.GetCurrentDirectory ();

      OSservices.CreateDirectory (logFileDir);

      DateTime d = DateTime.Now;
      String instrumentLogFileName = OSservices.AddSlash (logFileDir) +
                                      "InstrumentDataLog_"         +
                                      d.Year.ToString  ("0000")    +
                                      d.Month.ToString ("00")      +
                                      d.Day.ToString   ("00")      +
                                      ".txt";

      {
        InstrumentDataLogger dataLogger = new InstrumentDataLogger (instrumentLogFileName);

        instrumentManager = new InstrumentDataManager (dataLogger,   // Will take ownership of 'DataLogger'
                                                       null,         // No SipperFile rec;
                                                       sipperConfigRec,
                                                       currentDataRow
                                                      );
        dataLogger = null;
      }

      //wait 1 secs for all the cmds from start to be displayed before sending the status command.
      Thread.Sleep (1000);

      Instrument inclinationMeter = headerRec.InclinationMeter (ref inclinationSerailPort);

      int loopCount = 0;

      //  currentDataRow = instrumentManager.CurrentDataRow ();
      lastDataRow =  new InstrumentData ();

      dataLoopTerminate = false;
      dataLoopRunning   = true;

      while (!dataLoopTerminate)
      {
        if  (socket == null)
        {
          statusConnected = false;
          break;
        }
        
        if  (!socket.Connected)
        {
          statusConnected = false;
          break;
        }

        {
          SipperCommand cmd = GetNextSipperCommand ();
          if  (cmd != null)
          {
            FlushReceiveBufer ();
          
            cmd.Execute ();
            
            lock  (cmd.ResultQueue ())
            {
              cmd.ResultQueue ().Enqueue (cmd);
            }
            
            lock  (sipperCmdsPending)
            {
              sipperCmdBeingExecuted = null;
            }
          }
        }


        bool  performStatusCmd = ((loopCount % 16) == 0);

        if  (instrumentDataStart)
        {
          instrumentDataStart      = false;
          instrumentDataStarted    = true;
          instrumentDataCollecting = true;
        }

        if (instrumentDataEnd)
        {
          instrumentDataEnd        = false;
          instrumentDataEnded      = true;
          instrumentDataCollecting = false;
        }

        if  (instrumentDataCollecting)
          CollectInstrumentData (performStatusCmd);

        loopCount++;

        // KAK 2013-08-09  Changed sleep from 100 to 25 mili secs.
        //wait 1 milisecond for Sipper to record more data
        if (!dataLoopTerminate)
          Thread.Sleep (25);
//          Thread.Sleep (100);
      }
      
      instrumentDataCollecting = false;
      instrumentDataEnded = true;

      dataLoopRunning = false;
    }  /* DataLoop */




    private void   CollectInstrumentData (bool performStatusCmd)
    {
      if  (performStatusCmd)
      {
        bool  origStatusRecording = statusRecording;

        PerformStatusCommandAndUpdateSipperDiskStats ();
        
        FlushReceiveBufer ();
        
        float recordingRateMegPerSec  = 0.0f;
        if  (recordingRate != null)
          recordingRateMegPerSec = (float)recordingRate.MegaBytesPerSec ();
          
        currentDataRow.UpdateRecordingRate (recordingRateMegPerSec);
        
        currentDataRow.FlowRate2 = flowRate;

        bool processThisRec = false;

        if  (currentDataRow.UpdatedFlag)
          processThisRec = true;

        else if (!currentDataRow.Equals (lastDataRow))
        {
          processThisRec = true;
        }

        if (processThisRec)
        {
          if  (currentDataRow.BatteryDataChanged (lastDataRow))
          {
            mainForm.UpdateBatteryPack (true);
          }

          currentDataRow.Time = DateTime.Now;

          lock (history)
          {
            history.Add (new InstrumentData (currentDataRow));
          }
          currentDataRow.UpdatedFlag = false;
          lastDataRow = new InstrumentData (currentDataRow);
        }
      }

      byte serialPort;

      for (serialPort = 0; ((serialPort < instrumentsMaxNum)  &&  (!dataLoopTerminate));  ++serialPort)
      {
        if (!performStatusCmd  &&  (serialPort != inclinationSerailPort))
          continue;

        PortAssignment portAssignment = headerRec.PortAssignments ()[serialPort];
        if (portAssignment != null)
        {
          Instrument instrument = portAssignment.Instrument ();

          bool bufferNotEmpty = false;
          do
          {
            bufferNotEmpty = false;
            string cmd = "sipper_rsp " + serialPort.ToString ();

            string  result = SubmitCommandAndReturnResults (cmd, 2000);
            if  (result.StartsWith (cmd))
            {
              result = result.Substring (cmd.Length);
              if  (result.Length > 1)
              {
                if  ((result[0] == '\n')  ||  (result[0] == '\r'))
                {
                  result = result.Substring (1);
                  if  ((result[0] == '\n')  ||  (result[0] == '\r'))
                    result = result.Substring (1);
                }
              }
            }

            if (result.EndsWith ("not emptied.\n\r"))
            {
              if (result.EndsWith ("** WARNING ** Buffer not emptied.\n\r"))
              {
                int newLen = result.Length - (35 + 2);
                result = result.Substring (0, newLen);
                bufferNotEmpty = true;
              }
            }

            instrumentManager.ReportSerialPortData (serialPort, result);
          }
          while  (bufferNotEmpty  && (!dataLoopTerminate));
        }
      }
    }  /* CollectInstrumentData */



    public  void  ShutDownThreads ()
    {
      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//ShutDownThreads CurTime[" + System.DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");

        if (dataLoopThread == null)
        {
          logFile.WriteLine ("//  'dataLoopThread'  was not defined.");
          dataLoopRunning = false;
        }
        
        else if  (!dataLoopThread.IsAlive)
        {
          logFile.WriteLine ("//  'dataLoopThread'  was not alive.");
          dataLoopRunning = false;
          dataLoopThread = null;
        }
        
        if  (receiveThread == null)
        {
          logFile.WriteLine ("//  'receiveThread'  was not defined.");
          receiveThreadRunning = false;
        }
        
        else if  (!receiveThread.IsAlive)
        {
          logFile.WriteLine ("//  'receiveThread'  was not alive.");
          receiveThreadRunning = false;
          receiveThread = null;
        }

        logFile.Flush ();
      }

      if  ((dataLoopThread == null)   &&   (receiveThread == null))
        return;

      // Set flag to let threads know to terminate.
      dataLoopTerminate      = true;
      receiveThreadTerminate = true;

      int  numTimesThatWeWaited = 0;
      // We are going to wait up to 2 seconds for the status loop to terminate.

      while  ((dataLoopRunning  ||  receiveThreadRunning)  && (numTimesThatWeWaited < 30))
      {
        Thread.Sleep (100);
        numTimesThatWeWaited++;
      }
 
      if  ((dataLoopThread != null)  &&  (dataLoopRunning))
      {
        if (dataLoopThread.IsAlive)
        {
          lock  (logFile)
          {
            logFile.WriteLine ("//");
            logFile.WriteLine ("//  'dataLoopThread'    *** STILL ALIVE/RUNNING ***;     timed out waiting for it to terminate;  will fource.");
            logFile.WriteLine ("//");
            logFile.Flush ();
          
            dataLoopThread.Abort ();
          }
        }
        else
        {
          lock  (logFile)
          {
            logFile.WriteLine ("//  'dataLoopThread'  Terminated.");
            logFile.Flush ();
          }
        }
      }

      if  ((receiveThread != null)  &&  receiveThreadRunning)
      {
        if (receiveThread.IsAlive)
        {
          lock  (logFile)
          {
            logFile.WriteLine ("//");
            logFile.WriteLine ("//  'receiveThread'    *** STILL ALIVE/RUNNING ***;     timed out waiting for it to terminate;  will fource.");
            logFile.WriteLine ("//");
            logFile.Flush ();
          
            receiveThread.Abort ();
          }
        }
        else
        {
          lock  (logFile)
          {
            logFile.WriteLine ("//  'receiveThread'  Terminated.");
            logFile.Flush ();
          }
        }
      }

      dataLoopThread = null;
      receiveThread  = null;

    }  /* ShutDownThreads */



    public  void  CollectingInstrumentDataStart ()
    {
      if  (instrumentDataCollecting)
        return;
        
      instrumentDataStart = true;
      
      int  loopCount = 0;
      while  ((!instrumentDataStarted)  &&  (loopCount < 30))
      {
        Thread.Sleep (100);
        loopCount++;
      }
      
      if  (!instrumentDataStarted)
      {
        MessageBox.Show ("Could not START collection of Instrument Data.");
      }
        
      instrumentDataStarted = false;
    }  /* CollectingInstrumentDataStart */



    public  void  CollectingInstrumentDataEnd ()
    {
      if  (!instrumentDataCollecting)
        return;
        
      instrumentDataEnd = true;
      
      int  loopCount = 0;
      while  ((!instrumentDataEnded)  &&  (loopCount < 30))
      {
        Thread.Sleep (100);
        loopCount++;
      }
      
      if  (!instrumentDataEnded)
      {
        MessageBox.Show ("Could not END collection of Instrument Data.");
      }
        
      instrumentDataEnded = false;
    }  /* CollectingInstrumentDataEnd */



    public  void  PerformStatusCommandAndUpdateSipperDiskStats ()
    {
      FlushReceiveBufer ();
      string  statusStr = SubmitCommandAndReturnResults ("sipper_status a", 500);
      ParseStatusCmdResultsStr (statusStr);
    }  /* ProcessStatusCommand */



    private void  ParseStatusCmdResultsStr (string temp)
    {
      string [] split;
      string [] LBA;

      string body = temp;

      //getting rid of unwanted characters 
      body = body.Replace ("\n"," ");
      body = body.Replace ("\r"," ");
      body = body.Replace ("\0"," ");

      split = body.Trim().Split (' ');

      foreach (string s in split) 
      {
        if (!s.Equals("\n\r"))
        {
          LBA = s.Trim ().Split ('=');
				        
          if (LBA.Length < 2)
            continue;

          switch(LBA[0])       
          {         
            case "bs":
            {
              startBlockAddr = OSservices.HexStrToLong (LBA[1]);
            }
            break;                  

            case "bc":
            {
              curBlockAddr = OSservices.HexStrToLong (LBA[1]);
            }
            break;          

            case "be":
            {
              endBlockAddr = OSservices.HexStrToLong (LBA[1]);
            }
            break;     
    
            case "fr":
            {
              try 
              {
                flowRate = (float)(Convert.ToDouble (LBA[1])) / 100;
              }
              catch (Exception)
              {
              }
            }
            break;  

            case "rs":
            {
              if  (LBA[1] == "r")
                statusRecording = true;
              else if  (LBA[1] == "s")
                statusRecording = false;
            }
            break;

          }  /* end of switch */
        }
      }


      if  (statusRecording)
      {
        if  (recordingRate == null)
          recordingRate = new RecordingRate (startBlockAddr, 10);
        recordingRate.RecordNextBlock (curBlockAddr);
      }


    }  /* ParseStatusCmdResultsStr */





    public  void  StartRecording (ref bool   recordingStartedOK,
                                  ref string fileNameOpened
                                 )
    {
      FlushReceiveBufer ();
      
      recordingStartedOK = false;

      //Start the read process command
        
      string  resultStr = SubmitCommandAndReturnResults ("sipper_record", 7000);

      if  (resultStr.Contains ("Opening "))    //   Need to make sure of sucesfull Start Record String. 
      {
        // Opening Test10_01.  StartLBA= 002CF3FC, EndLBA= 04A8143F
        int  idx = resultStr.IndexOf ("Opening ");
        if  (idx >= 0)
        {
          fileNameOpened = resultStr.Substring (idx + 8);
          idx = fileNameOpened.IndexOf ('.');
          if  (idx >= 0)
            fileNameOpened = fileNameOpened.Substring (0, idx);
        }
      
        statusRecording = true;
        recordingStartedOK = true;
      }
      else 
      {
        recordingStartedOK = false;
        statusRecording    = false;
      }
   }  /* StartRecording */





    public  void  SetFileName (ref bool  fileNameSetOK,
                               string    fileName,
                               string    fileDesc
                              )
    {
      string  result = SubmitCommandAndReturnResults ("sipper_sfn " + fileName, 1000);
      Thread.Sleep (300);
      while  (!Empty ())
      {
        byte b = GetChar ();
      }

      SendText ("sipper_sfd");
      Thread.Sleep (300); // Giving Sipper a chance to receive data and process commands
      SendText (fileDesc);
      
      result = GetText (2000);
      fileNameSetOK = true;
      return; 
    }  /* SetFileName */




    public  void  StopRecording (ref  bool  executedOK)
    {
      string resultStr = SubmitCommandAndReturnResults ("sipper_stop", 7000);

      if  (resultStr.Contains ("File Closed."))   // 
      {
        statusRecording = false;
        executedOK = true;
      }
      else 
      {
        executedOK = false;
      }
    }  /* StopRecording */




    public  byte[]  ReadHardDrive (long  startingBlock, 
                                   int   blocksToRead
                                  )
    {
      if  (blocksToRead < 1)
        blocksToRead = 100;

      string cmd = "sipper_rhd" + " " + startingBlock.ToString () + " " + blocksToRead.ToString ();
      int  status = SendText (cmd);
      if  (status < 1)
        return  null;
     
      EchoText (false);
      
      // We first need to skip the echo of the sendCmd.
      //Thread.Sleep (30);
      for (int xxx = 0; xxx < cmd.Length + 1; xxx++)
         GetChar ();


      byte[] dataBlock = GetBinaryData (blocksToRead * 512);
     
      EchoText (true);
     
      return  dataBlock;
    }  /* ReadHardDrive */

  }  /* CircularBuffer */
}
