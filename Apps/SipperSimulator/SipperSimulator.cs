using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;


using PicesInterface;
using SipperFile;




namespace SipperSimulator
{
  public partial class SipperSimulator : Form
  {
    private TcpListener  myList              = null;
    private Thread       sipperConsoleThread = null;
    private Socket       socket              = null;	
    private bool         terminateConnection = false;
    
    private bool         statusConnection           = false;
    private bool         statusWaitingForConnection = false;
    
    private Queue        conversationBuffer = null;
 
    private SerialPortBuffer[]  serialPortBuffers = null;

    private List<char>      ctdData                    = new List<char>();
    private int             ctdDataNextByte            = 0;
    private int             ctdDataSerialPort          = 0;
    private bool            ctdDataInstrumentRunning   = false;
    private Thread          ctdThread                  = null;
    
    private ArrayList       pitchRollData              = null;
    private int             pitchRollDataNextRow       = 0;
    private int             pitchRollSerialPort        = 1;
    private bool            pitchRollInstrumentRunning = false;
    private Thread          pitchRollThread            = null;
    
    private BatteryPack     batteryPack                = null;
    private int             batteryPackSerialPort      = 2;
    private bool            batteryPackRunning         = false;
    private Thread          batteryPackThread          = null;

    private SipperCamera    sipperCamera               = null;

    private InstrumentData  currentDataRow             = null;
    private SipperConfigRec configRec                  = null;
    private bool            terminateThreads           = false;

    private bool            cameraMode                 = false;

    
    //string                  dataFileDir                = "C:\\temp\\SIPPER3_Deployment_2007_03_08\\";
    List<List<string>>      cameraLines                = null;
    int                     cameraLineLastRead         = 0;
    string                  simulatorDir               = "E:\\Pices\\SipperInterface\\Simulator";
    

    String[]                sipperFileNames            = null;

    Random                  randomNum = new Random ();

    PicesRunLog             runLog = new PicesRunLog ();


    public   SipperSimulator (String  sipperFileName)
    {
      InitializeComponent ();

      if  (String.IsNullOrEmpty (sipperFileName))
      {
        sipperFileName = "C:\\Pices\\SipperFiles\\WB0813\\WB0813DSH09\\WB0813DSH09_03.spr";
      }

      //ConvertCTDfile ();

      simulatorDir = OSservices.AddSlash (OSservices.AddSlash (PicesSipperVariables.SipperInterfaceDir ()) + "Simulator");

      configRec      = new SipperConfigRec ();
      currentDataRow = new InstrumentData ();

      conversationBuffer = Queue.Synchronized (new Queue ());
      serialPortBuffers = new SerialPortBuffer[3];
      serialPortBuffers[0] = new SerialPortBuffer (512);
      serialPortBuffers[1] = new SerialPortBuffer (512);
      serialPortBuffers[2] = new SerialPortBuffer (512);

      // SplitFile ();
      // ChkFile ();
      //LoadSipperFile();

      ctdData = new List<char>();
      ctdDataNextByte = 0;

      pitchRollData = new ArrayList();
      pitchRollDataNextRow = 0;

      ExtractDataFromSIPPERFile (sipperFileName);

      LoadCameraLines ();

      batteryPack  = new BatteryPack ();
      sipperCamera = new SipperCamera ();
      
      ctdThread         = new Thread (new ThreadStart (CTDDataInstrument));
      ctdThread.Name    = "SipperCtdData";
      ctdThread.Start ();
      
      pitchRollThread   = new Thread (new ThreadStart (PitchRollDataInstrument));
      pitchRollThread.Name = "SipperPitchRollData";
      pitchRollThread.Start ();
      
      batteryPackThread = new Thread (new ThreadStart (BatteryPackInstrument));
      batteryPackThread.Name = "SipperBatteryData";
      batteryPackThread.Start ();

      LoadSipperFilesNames ();
    }


    private  void  ConvertCTDfile ()
    {
      System.IO.StreamReader  inFile  = new System.IO.StreamReader ("D:\\TrainingApp\\SipperInterfaceDir\\Simulator\\SipperInterfaceLog_20081224-1800.txt");
      System.IO.StreamWriter  outFile = new System.IO.StreamWriter ("D:\\TrainingApp\\SipperInterfaceDir\\Simulator\\CtdData2.txt");

      String  line = null;
      while  ((line = inFile.ReadLine ()) != null)
      {
        if  (line.Length < 60)
          continue;

        if  (line.StartsWith ("bs="))
          continue;

        if  (line.StartsWith ("//"))
          continue;
        outFile.WriteLine (line);
      }

      inFile.Close ();
      outFile.Close ();
    }



    private  void  LoadSipperFilesNames ()
    {
      try
      {
        sipperFileNames = PicesMethods.GetListOfSipperFiles (runLog);
      }
      catch  (Exception e)
      {
        sipperFileNames = null;
      }
    }  /* LoadSipperFileNames */



    private  void  ExtractSerialData (byte    deviceId, 
                                      string  txt
                                     )
    {
      string  x = txt;
    }





    private void  ChkFile ()
    {
      StreamReader f = null;
      string fileName = "D:\\TrainingApp\\SipperInterface\\LogFiles\\SipperInterfaceLog_20080701-1236.txt";

      f = new StreamReader(fileName, System.Text.Encoding.UTF8);

      StreamWriter dataFile = new StreamWriter("C:\\temp\\SIPPER3_Deployment_2007_03_08\\LogFiles\\SipperInterfaceLog_20070306-1524_data.txt");
 
      string  txtLinePrev = "";
      string  txtLine = "";
      ArrayList  lines = new ArrayList ();

      int  lineNum = 0;
      
      //
      txtLine = f.ReadLine();
      while (!f.EndOfStream)
      {
        lines.Add (txtLine);
      
        if  (txtLine.Contains ("fileNameBase"))
        {
          dataFile.WriteLine ("");
          dataFile.WriteLine ("");
          dataFile.WriteLine ("");
          dataFile.WriteLine ("[" + (lineNum - 1) + "]:" + txtLinePrev);
          dataFile.WriteLine ("[" + lineNum       + "]:" + txtLine);
        
          int  z = 0;

          for (z = 0; z < txtLinePrev.Length; z++)
          {
            if (z > 0)
              dataFile.Write(", ");
            dataFile.Write((int)txtLinePrev[z]);
          }
          dataFile.WriteLine("");

          for (z = 0; z < txtLine.Length; z++)
          {
            if  (z > 0)
              dataFile.Write (", ");
            dataFile.Write ((int)txtLine[z]);
          }

          dataFile.WriteLine ("");
        }

        lineNum++;
        txtLinePrev = txtLine;
        txtLine = f.ReadLine();
      }

      dataFile.Close ();
      f.Close();
    }  /* ChkFile */





    
    private void Form1_Load (object sender, EventArgs e)
    {
    }



    private  void  TerminatAllThreads ()
    {
      terminateThreads    = true;
      terminateConnection = true;
      int  loopCount = 0;
      
      batteryPack.TerminateMonitorThreadReq ();
      sipperCamera.TerminateMonitorThreadReq ();
      
      while  (ctdDataInstrumentRunning       &&  
              pitchRollInstrumentRunning     &&  
              batteryPackRunning             &&  
              sipperCamera.MonitorRunning () && 
              (loopCount < 10)
             )
      {
        Thread.Sleep (100);
      }

      if  (ctdThread.IsAlive)
      {
        try
        {
          ctdThread.Abort ();
        }
        catch  (Exception e)
        {
          MessageBox.Show(e.ToString(), "Aborting CTD Thread");
        }
      }
      ctdThread = null;

      if  (pitchRollThread.IsAlive)
      {
        try
        {
          pitchRollThread.Abort ();
        }
        catch  (Exception e)
        {
          MessageBox.Show (e.ToString (), "Aborting Pitch And Roll thread");
        }
      }
      pitchRollThread = null;
    }  /* TerminatAllThreads */



    private void SplitFile ()
    {
      StreamReader f = null;
      string  fileName = "D:\\TrainingApp\\SipperInterface\\LogFiles\\SipperInterfaceLog_20080701-1236.txt";
      f = new StreamReader (fileName);
      
      StreamWriter ctdFile = new StreamWriter ("D:\\TrainingApp\\SipperInterface\\Simulator\\CtdData.txt");
      
      StreamWriter pitchAndRollFile = new StreamWriter("D:\\TrainingApp\\SipperInterface\\Simulator\\PitchRollData.txt");
      
      StreamWriter batteryFile = new StreamWriter("D:\\TrainingApp\\SipperInterface\\Simulator\\BatteryData.txt");
      
      
      string  txtLine = null;

      txtLine = f.ReadLine();
      while (!f.EndOfStream)
      {
        txtLine = txtLine.Trim ();
        char  firstChar = ' ';
        if  (txtLine.Length > 0)
          firstChar = txtLine[0];
          
      
        if  ((txtLine.StartsWith ("**"))  ||  (txtLine.Length < 10))
        {
        }
        
        else if  (txtLine.Length > 100)
        {
          ctdFile.WriteLine (txtLine);
        }
        
        else if  ((txtLine.Length >= 24)  &&  (txtLine.Length < 28)  &&  (txtLine[0] == 'R'))
        {
          pitchAndRollFile.WriteLine (txtLine);
        }
          
        else if  ((txtLine.Length >= 32)  &&  (txtLine.Length < 39))
        {
          if  ((firstChar >= '0') && (firstChar <= '5')  &&  txtLine[1] == ',')
             batteryFile.WriteLine (txtLine);
        }
          
        else
        {
          string  s2 = txtLine;
        }

        txtLine = f.ReadLine();
      }

      ctdFile.Close ();
      pitchAndRollFile.Close ();
      batteryFile.Close ();
      
      f.Close();
    }  /* SplitFile */




    private  void  LoadData (string     fileName,
                             ArrayList  dataList
                            )
    {
      bool  fileOpened = false;
      StreamReader  f = null;
      
      try
      {
        f = new StreamReader(fileName);
        fileOpened = true;
      }
      catch (Exception openException)
      {
        fileOpened = false;
        MessageBox.Show (openException.ToString (), "Opening [" + fileName + "]");
      }
      
      if  (!fileOpened)
        return;

      string txtLine = null;
      
      txtLine = f.ReadLine();
      while  (!f.EndOfStream)
      {
        dataList.Add (txtLine);
        txtLine = f.ReadLine();
      }
      
      f.Close ();
    }  /* LoadData */




    private  void  LoadCameraLines ()
    {
      cameraLines = new List<List<string>> ();

      DirectoryInfo  di = null;
      try  
      {
        di = new DirectoryInfo (simulatorDir);
      }
      catch (Exception)
      {
        MessageBox.Show ("No Camera Line data files locatd.");
        return;
      }

      FileInfo[] cameraLineFiles = null;
      try
      {
        cameraLineFiles = di.GetFiles("CameraDataLine_*.txt");
      }
      catch  (Exception)
      {
        cameraLineFiles = null;
      }
      if  (cameraLineFiles == null)
      {
        MessageBox.Show ("No Camera Line data files locatd.");
        return;
      }

      for  (int x = 0;  x < cameraLineFiles.Length;  x++)
      {
        cameraLines.Add (LoadCameraLine (cameraLineFiles[x]));
      }
    }  /* LoadCameraLines */




    private List<string>  LoadCameraLine (FileInfo  fi)
    {
      List<string>  lines = new List<string> ();
      StreamReader sr = fi.OpenText ();
      string  textLine = "";
      while (!sr.EndOfStream)
      {
        textLine = sr.ReadLine ();
        lines.Add (textLine);
      }
      sr.Close ();
      sr.Dispose ();
      sr = null;
      return lines;
    }  /* LoadCameraLine */




    ArrayList  displayBuff = new ArrayList ();
    
    public  void  UpdateTextWindowFromBuffer ()
    {
      lock (this)
      {
        if (displayBuff.Count >= 100)
        {
          int  x;
          conversation.Text = "";
          for  (x = 0;  x < displayBuff.Count; x++)
          {
            conversation.Text += (string)(displayBuff[x]);
          }

          conversation.SelectionStart = this.conversation.Text.Length;
          conversation.ScrollToCaret();
          displayBuff = new ArrayList();
        }


        if (conversationBuffer.Count > 0)
        {
          while  (conversationBuffer.Count > 0)
          {
            string s = (string)conversationBuffer.Dequeue ();
          
            try 
            {
              conversation.Text += s;
              displayBuff.Add (s);
              //conversation.SelectionStart = this.conversation.Text.Length;
              //conversation.ScrollToCaret ();
            }
            catch  (Exception e)
            {
             MessageBox.Show ("Error Writting to text box [" + e.ToString () + "].");
            }
          }

          conversation.SelectionStart = this.conversation.Text.Length;
          conversation.ScrollToCaret();
        }
      }
    }  /* UpdateTextWindowFromBuffer */




    public void   WriteToTextWindow (string ln)
    {
      try
      {
        conversationBuffer.Enqueue (ln);
      }
      catch  (Exception e)
      {
        MessageBox.Show ("Error adding to TextBox Queue [" + e.ToString () + "].");
      }
    }



    private IPAddress  GetLocalIPAdress ()
    {
      IPHostEntry ihe =  Dns.GetHostEntry ("");
      
      IPAddress[] addresses = ihe.AddressList;
      if  (addresses.Length == 0)
        return null;

      // Want to pick the first valid one listed
      int idx = 0;
      while  (idx < addresses.Length)
      {
        if  (addresses[idx].AddressFamily == AddressFamily.InterNetwork)
          break;
        idx++;
      }

      return  addresses[idx];
    }  /* GetLocalIPAdress */



 
    private  void  WaitForClientConnection ()
    {
      Thread.Sleep (200);
    
      while  (!terminateConnection)
      {
        statusWaitingForConnection = true;
        WriteToTextWindow ("Setting Up to Receive Connections\n.");
    
        if  (socket != null)
        {
          if (socket.Connected)
            socket.Disconnect (false);
        }
    
        socket = null;

        //IPAddress ipAd = IPAddress.Any;
        IPAddress ipAd = GetLocalIPAdress ();

        WriteToTextWindow ("IP Address " + ipAd.ToString () + "\n");

        //IPAddress ipAd = IPAddress.Parse ("192.168.1.102"); //use local m/c IP address, and use the same in the client
        myList = new TcpListener (ipAd, 8002);
//        IPAddress ipAd = IPAddress.Parse ("131.247.151.34"); //use local m/c IP address, and use the same in the client
//        myList = new TcpListener (ipAd, 8001);

        try
        {
          myList.Start ();
        }
        catch  (Exception e1)
        {
          WriteToTextWindow ("Error starting socket\n");
          WriteToTextWindow (e1.ToString ());
          break;
        }

        int  checkCount = 0;

        while  ((!terminateConnection)  &&  (socket == null))
        {
          if  (myList.Pending ())
          {
            try  
            {
              socket = myList.AcceptSocket ();
            }
            catch  (Exception e3)
            {
              WriteToTextWindow ("Error Accepting Socket\n");
              WriteToTextWindow (e3.ToString ());
              socket = null;
            }
          }
          else
          {
            // No one is trying to connect yet.
            checkCount++;
            
            if  ((checkCount % 30) == 0)
              WriteToTextWindow ("Still waiting for a connection.\n");

            Thread.Sleep (100);
          }
        }
        
        myList.Stop ();
        myList = null;
        
        if  ((!terminateConnection)  &&  (socket != null))
        {
          statusWaitingForConnection = false;
          WriteToTextWindow ("Connectiion accepted from[" + socket.RemoteEndPoint.ToString () + "].");
          statusConnection = true;
          Thread.MemoryBarrier ();

          try
          {
            SipperConsole ();
          }
          catch  (Exception e5)
          {
            WriteToTextWindow ("Console had an exception\n");
            WriteToTextWindow (e5.ToString ());
            statusConnection = true;
          }
          statusConnection = false;
          Thread.MemoryBarrier ();
        }
        
        if  (socket != null)
        {
          if  (socket.Connected)
          {
            try 
            {
              socket.Disconnect (false);
            }
            catch  (Exception e6)
            {
              WriteToTextWindow ("Error disconecting from socket\n");
              WriteToTextWindow (e6.ToString ());
            }
          }
          socket = null;
        }
      }
      
      statusWaitingForConnection = false;
    }  /* WaitForClientConnection */



    private  void  TerminateConnection ()
    {
      terminateConnection = true;
      
      int  loopCount = 0;
      
      while  (statusConnection  ||  statusWaitingForConnection  ||  (loopCount < 10))
      {
        Thread.Sleep (100);
        loopCount++;
      }
      
      sipperConsoleThread.Abort ();
      sipperConsoleThread = null;
      
    }  /* TerminateConnection */



    
    byte[] buffer = new byte[10240];
    int    bufferNextChar = 0;
    int    bytesInBuffer  = 0;



    private  string  GetNextComand ()
    {
      string  s = "";
      
      bool  crDetected = false;
      
      while  ((!crDetected)  &&  (!terminateConnection)  &&  socket.Connected)
      {
        if  (bufferNextChar >= bytesInBuffer)
        {
          bytesInBuffer = this.socket.Receive (buffer);
          bufferNextChar = 0;
        }
        
        if  (bytesInBuffer == 0)
        {
          Thread.Sleep (10);
        }
        
        while  ((!crDetected)  &&  (bufferNextChar < bytesInBuffer))
        {
          char ch = Convert.ToChar (buffer[bufferNextChar]);
          
          if  (ch == '\r')
            crDetected = true;
          else
            s += ch;
          bufferNextChar++;
        }
      }

      return  s;
    }  /* GetNextComand */



    private  void  SipperConsole ()
    {
      byte[] b = new byte[1024];

      while  (!terminateConnection)
      {
        if  (socket == null)
          break;
          
        if  (!socket.Connected)
          break;
      
        if  (cameraMode)
          WriteToSocket ("OK>");
        else
          WriteToSocket (">> ");
        
        string  s = GetNextComand ();

        if (!socket.Connected)
          break;
        
        if  (terminateConnection)
        {
          WriteToTextWindow ("\n\nTerminating Connection\n");
          break;
        }
                
        WriteToTextWindow (s + "\r");
        WriteToSocket (s + "\r");
        
        if  (cameraMode)
          ProcessCameraCommand (s);
        else
          ProcessCommand (s);
      }
       
      if  (socket != null)
      {
        socket.Close ();
        socket = null;
      }
      
      statusConnection = false;
    }  /* SipperConsole */


           

    private  void  WriteToSocket (string  s)
    {
      ASCIIEncoding asen = new ASCIIEncoding ();
      socket.Send (asen.GetBytes (s));
    }  /* WriteToSocket */



    
    private  void  WriteToSocket (byte[]  buff)
    {
      socket.Send (buff);
    }  /* WriteToSocket */




    private  string  ExtractField (ref string  s)
    {
      string  result = "";
      s = s.Trim ();
      
      int x = 0;
      
      while  (x < s.Length)
      {
        char c = s[x];
        if  ((c == ' ')  ||  (c == '\n')  ||  (c == '\t'))
          break;
      
        x++;
      }
      
      if  (x >= s.Length)
      {
        result = s.ToString ();
        s = "";
      }
      else
      {
        result = s.Substring (0, x);
        s = s.Substring (x + 1);
      }

      return result;
    }  /* ExtractField */




    private  void  ProcessCommand (string  s)
    {
      string  cmdStr = ExtractField (ref s);
      s = s.Trim ();
      
      if  (cmdStr == "sipper_battery")
        BatteryCommand (s);

      else if  (cmdStr == "sipper_camera")
      {
        cameraMode = true;
        WriteToSocket     ("\nType \"exit\" to return.  Timeout in 15 seconds      ");
        WriteToTextWindow ("\nType \"exit\" to return.  Timeout in 15 seconds      ");
      }

      else if (cmdStr == "sipper_record")
        StartRecording ();

      else if  (cmdStr == "sipper_rhd")
        ReadHardDrive (s);

      else if  (cmdStr == "sipper_rsp")
        ReadSerialPort (s);
        
      else if (cmdStr == "sipper_stop")
        StopRecording ();

      else if (cmdStr == "sipper_sfn")
        SetFileName (s);

      else if (cmdStr == "sipper_sfd")
        SetFileDesc (s);
        
      else if  (cmdStr == "sipper_status")
        StatusCommand ();


    }  /* ProcessCommand */




    private void  ProcessCameraCommand (string s)
    {
      string cmdStr = ExtractField(ref s);
      s = s.Trim();

      if  (cmdStr == "gcp")
      {
        WriteToSocket ("");
      }
      
      else if  (cmdStr == "exit")
      {
        cameraMode = false;
      }

      else if ((cmdStr == "get_line")  ||  (cmdStr == "gl"))
      {
        GetCameraLine ();
      }

    }  /* ProcessCommand */




    private  void  CameraParameters ()
    {
      string[]  cameraParameters = 
      {
"G E N E R A L   C A M E R A   S E T T I N G S:",
"",
"Camera Model No.:               P2-40-04K40",
"Camera Serial No.:              11012087",
"Camera Network ID:              0",
"Network Message Mode:           disabled",
"",
"Firmware Design Rev.:           03-081-20018-01 Aug 29 2007",
"DSP Design Rev.:                03-056-20010-00",
"",
"SETTINGS FOR UNCALIBRATED MODE:",
"",
"Analog Gain (dB):               +0.0    +0.0    +0.0    +0.0    ",
"Analog Offset:                  691     688     683     744    ",
"",
"SETTINGS FOR CALIBRATED MODE:",
"",
"Analog Gain (dB):               +5.0    +5.0    +5.0    +5.0    ",
"",
"Analog Offset:                  560     560     530     650    ",
"Digital Offset:                 16      16      15      11     ",
"Calibration Status:             FPN [calibrated] PRNU [calibrated]",
"",
"SETTINGS COMMON TO CALIBRATED AND UNCALIBRATED MODES:",
"",
"System Gain:                    0       0       0       0      ",
"Background Subtract:            0       0       0       0      ",
"",
"Pretrigger:                     0      ",
"Number Of Line Samples:         64     ",
"Video Mode:                     calibrated",
"Data Mode:                      2      ",
"Exposure Mode:                  2      ",
"",
"",
"SYNC Frequency:                 25000 (24950.3) Hz",
"Exposure Time:                  39.700 uSec",
"",
"End-Of-Line Sequence:           on",
"Upper Threshold:                240 ",   
"Lower Threshold:                15    ", 
"Region Of Interest:             0001 - 4096"
    };
      
      for  (int x = 0;  x < cameraParameters.Length;  x++)
      {
        WriteToSocket     (cameraParameters[x] + "\n");
        WriteToTextWindow (cameraParameters[x] + "\n");
      }
    }  /* CameraParameters */

    

    private void  GetCameraLine ()
    {
      if  (cameraLineLastRead >= cameraLines.Count)
        cameraLineLastRead = 0;

      List<string>  lines = cameraLines[cameraLineLastRead];

      for (int x = 0; x < lines.Count; x++)
      {
        WriteToSocket (lines[x] + "\n");
        WriteToTextWindow (lines[x] +"\n");
      }

      cameraLineLastRead++;
    } /* GetCameraLine */



    private  void  ReadHardDrive  (string  s)
    {
      string[]  fields = s.Split (' ');

      if  (fields.Length != 2)
      {
        WriteToSocket ("Invalid Parameters for rhd command\n");
        WriteToTextWindow ("Invalid Parameters for rhd command\n");
      }

      int  numBlocksToRead = PicesKKStr.StrToInt (fields[1]);
      if  (numBlocksToRead < 1)
      {
        WriteToSocket ("Invalid Parameters for rhd command\n");
        WriteToTextWindow ("Invalid Parameters for rhd command\n");
        return;
      }


      if  (sipperFileNames == null)
      {
        WriteToSocket ("No SIPPER Data to work with\n");
        WriteToTextWindow ("No SIPPER Data to work with\n");
        return;
      }
      
      if  (sipperFileNames.Length < 1)
      {
        WriteToSocket ("No SIPPER Data to work with\n");
        WriteToTextWindow ("No SIPPER Data to work with\n");
        return;
      }

      int  x = randomNum.Next () % sipperFileNames.Length;
      
      FileStream  fs = null;
      BinaryReader br = null;
      try  
      {
        fs = new FileStream (sipperFileNames[x], FileMode.Open);
        br = new BinaryReader (fs);
      }
      catch  (Exception e)
      {
        WriteToSocket ("Error Opening SIPPER file\n");
        WriteToTextWindow ("Error Opening SIPPER file" + "\n" +
                           "\n" +
                           e.ToString ()
                          );
        return;
      }

      byte[]  buff  = br.ReadBytes (numBlocksToRead * 512);
      WriteToSocket (buff);

      br.Close ();
      br = null;

      fs.Close ();
      fs = null;
    }  /* ReadHardDrive */



    private  void  ReadSerialPort (string  s)
    {
      s = s.Trim ();
      
      int  serialPortNum = PicesKKStr.StrToInt (s);
      
      if  ((serialPortNum < 0)  ||  (serialPortNum >= serialPortBuffers.Length))
      {
        WriteToSocket ("Invalid Serial Port[" + serialPortNum + "]\n");
        WriteToTextWindow ("Invalid Serial Port[" + serialPortNum + "]\n");
      }
      
      else 
      {
        SerialPortBuffer sp = serialPortBuffers[serialPortNum];
        if  (sp == null)
        {
          WriteToSocket ("Invalid Serial Port[" + serialPortNum + "]\n");
          WriteToTextWindow ("Invalid Serial Port[" + serialPortNum + "]\n");
        }
      
        else
        {
          int amtDataAvailable = sp.SpaceUsed ();
          int  x = 0;
          
          string  textLine = "";
          
          for  (x = 0;  x < amtDataAvailable;  x++)
          {
            char ch = sp.ReadChar ();
            textLine += ch.ToString ();
          }
          
          WriteToSocket     (textLine);
          WriteToTextWindow (textLine);

          if  (sp.SpaceUsed () > 0)
          {
            WriteToSocket("** WARNING ** Buffer not emptied.");
            WriteToTextWindow("** WARNING ** Buffer not emptied.");
          }
        }
      }
    }  /* ReadSerialPort */



    private  void  StatusCommand ()
    {
      string s = sipperCamera.StatusStr () + "\r";
      WriteToTextWindow (s);
      WriteToSocket (s);
    }



    private void  StartRecording ()
    {
      string  result = sipperCamera.StartRecording ();
      WriteToTextWindow (result);
      WriteToSocket (result);
    }


    private void StopRecording()
    {
      string  result = sipperCamera.StopRecording();
      WriteToTextWindow (result);
      WriteToSocket (result);
    }


    private  void  SetFileName (string  s)
    {
      sipperCamera.SetFileName (s);
    }
     
     
    private  void  SetFileDesc (string s)
    {
      WriteToSocket("?");
      WriteToTextWindow ("?");

      s = GetNextComand();
      sipperCamera.SetFileDesc (s);
    }  /* SetFileDesc */



    private  void  BatteryCommand (string  s)
    {
      if  (s.Length < 2)
        return;
      
      if  (s[0] == 's')
        s = s.Substring (1);
        
      int  batteryIdx = 0;
      
      try  
      {
        batteryIdx = int.Parse (s);
        if  ((batteryIdx >= 0)  &&  (batteryIdx < 5))
        {
          batteryPack.RequestToSwitchBatteries (batteryIdx);
        }
      }  
      catch {}
      
      
    }
    


    private void  CTDDataInstrument  ()
    {
      ctdDataInstrumentRunning = true;

      Thread.Sleep(200);

      while (!terminateThreads)
      {
        Thread.Sleep (100);

        for  (int zed = 0;  zed < 100;  zed++)
        {
          if (ctdDataNextByte >= ctdData.Count)
            ctdDataNextByte = 0;

          char t = ctdData[ctdDataNextByte];
        
          if  (serialPortBuffers[ctdDataSerialPort].AvailablSpace () > 0)
            serialPortBuffers[ctdDataSerialPort].WriteChar(t);

          ctdDataNextByte++;
          if  (ctdDataNextByte >= ctdData.Count)
            ctdDataNextByte = 0;
        }
      }
      
      ctdDataInstrumentRunning = false;
    }  /* CTDDataInstrument */




    private void  PitchRollDataInstrument ()
    {
      pitchRollInstrumentRunning = true;

      Thread.Sleep(200);

      while (!terminateThreads)
      {
        Thread.Sleep(250);

        if (pitchRollDataNextRow >= pitchRollData.Count)
          pitchRollDataNextRow = 0;

        string t = (string)(pitchRollData[pitchRollDataNextRow]) + "\r";

        int x = 0;
        for (x = 0; x < t.Length; x++)
        {
          if (serialPortBuffers[pitchRollSerialPort].AvailablSpace () < 1)
            break;
          serialPortBuffers[pitchRollSerialPort].WriteChar(t[x]);
        }
        pitchRollDataNextRow++;
        if (pitchRollDataNextRow >= pitchRollData.Count)
          pitchRollDataNextRow = 0;
      }

      pitchRollInstrumentRunning = false;
    }  /* PitchRollDataInstrument  */



    private void  BatteryPackInstrument ()
    {
      batteryPackRunning = true;

      while (!terminateThreads)
      {
        Thread.Sleep(1000);

        string t = batteryPack.StatusStr () + "\r";

        int x = 0;
        for (x = 0; x < t.Length; x++)
        {
          if (serialPortBuffers[batteryPackSerialPort].AvailablSpace () < 1)
            break;
          serialPortBuffers[batteryPackSerialPort].WriteChar(t[x]);
        }
      }

      batteryPackRunning = false;
    }  /* BatteryPackInstrument */






    private void conversation_TextChanged (object sender, EventArgs e)
    {

    }




    private void startButon_Click (object sender, EventArgs e)
    {
      DisableAllButons ();

      statusWaitingForConnection = true;
      sipperConsoleThread = new Thread (new ThreadStart (WaitForClientConnection));
      sipperConsoleThread.Name = "SipperConnection";
      sipperConsoleThread.Start ();

      EnableButtons ();
    }  /* startButon_Click */



    private  void  DisableAllButons ()
    {
      startButon.Enabled = false;
      endButton.Enabled  = false;
    }  /* DisableAllButons */

 
 
    private  void  EnableButtons ()
    {
      if  (statusConnection)
      {
        startButon.Enabled = false;
        endButton.Enabled  = true;
      }
      
      else if  (!statusWaitingForConnection)
      {
        startButon.Enabled = true;
        endButton.Enabled = false;
      }
      
      else
      {
        startButon.Enabled = false;
        endButton.Enabled = true;
      }
    }  /* EnableButtons */





    private void endButton_Click (object sender, EventArgs e)
    {
      DisableAllButons ();
      
      if  (statusConnection  ||  statusWaitingForConnection)
        TerminateConnection ();
      
      EnableButtons ();
    }  /* endButton_Click */
    
    

    private void textBoxUpdateTimer_Tick (object sender, EventArgs e)
    {
      UpdateTextWindowFromBuffer ();
    }

    private void SipperSimulator_FormClosing(object sender, FormClosingEventArgs e)
    {
      DisableAllButons();

      if (statusConnection || statusWaitingForConnection)
        TerminateConnection();

      TerminatAllThreads ();

      EnableButtons();
    }


    static  private  Sipper3DecodeRec[]  decodeRecs = null;

    static  private  void  BuildDecodeRecs ()
    {
      decodeRecs = new Sipper3DecodeRec[65536];
      uint x = 0;
      for (x = 0; x < decodeRecs.Length; x++)
        decodeRecs[x] = new Sipper3DecodeRec((ushort)x);
    }  /* BuildDecodeRecs */





    String  curCTDLine = "";


    private  void  AddToCTDData (char  c)
    {
      /*   kak 2010-05-06     I do not want to add or delete 'cr' characters.
      if ((c == '\n')  ||  (c == '\r'))
      {
        if  (!String.IsNullOrEmpty (curCTDLine))
          ctdData.Add (curCTDLine);
        curCTDLine = "";
      }
      else
      {
        curCTDLine += c;
      }
      */

      ctdData.Add (c);
    }  /* AddToCTDData */




    String  curPitchAndRollLine = "";

    private  void  AddToPitchAndRollData (char  c)
    {
      if ((c == '\n')  ||  (c == '\r'))
      {
        if  (!String.IsNullOrEmpty (curPitchAndRollLine))
          pitchRollData.Add (curPitchAndRollLine);
        curPitchAndRollLine = "";
      }
      else
      {
        curPitchAndRollLine += c;
      }
    }


    
    private  void  ExtractDataFromSIPPERFile (String  sipperFileName)
    {
      PicesRunLog  runLog = new PicesRunLog ();
      PicesDataBase  db = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      PicesSipperFile sf = null;

      if (db == null)
      {
        sf = new PicesSipperFile (sipperFileName);
        sf.Sp0 = configRec.HeaderRec().SerialPortShortName (0);
        sf.Sp1 = configRec.HeaderRec().SerialPortShortName (1);
        sf.Sp2 = configRec.HeaderRec().SerialPortShortName (2);
        sf.Sp3 = configRec.HeaderRec().SerialPortShortName (3);

        sf.CtdExt0 = configRec.HeaderRec ().CtdExt0Code ();
        sf.CtdExt1 = configRec.HeaderRec ().CtdExt1Code ();
        sf.CtdExt2 = configRec.HeaderRec ().CtdExt2Code ();
        sf.CtdExt3 = configRec.HeaderRec ().CtdExt3Code ();
      }
      else
      {
        sf = db.SipperFileRecLoad (OSservices.GetRootName (sipperFileName));
        if (sf == null)
          return;
      }

      byte  ctdSensorNum    = 0;
      byte  pitchAndRollNum = 1;

      switch  (sf.Sp0)
      {
        case  "CTD": ctdSensorNum    = 16;  break;
        case  "P-R": pitchAndRollNum = 16;  break;
      }

      switch  (sf.Sp1)
      {
        case  "CTD": ctdSensorNum    = 17;  break;
        case  "P-R": pitchAndRollNum = 17;  break;
      }

      switch  (sf.Sp2)
      {
        case  "CTD": ctdSensorNum    = 18;  break;
        case  "P-R": pitchAndRollNum = 18;  break;
      }

      switch  (sf.Sp3)
      {
        case  "CTD": ctdSensorNum    = 19;  break;
        case  "P-R": pitchAndRollNum = 19;  break;
      }

      BinaryReader  fs = null;
      try {fs = new BinaryReader (new FileStream (sipperFileName, FileMode.Open, FileAccess.Read, FileShare.Read));}
      catch (Exception)  {fs = null;}
      if  (fs == null)
        return;

      BuildDecodeRecs ();

      bool  eof = false;
      ushort  rec = 0;

      try  {rec = fs.ReadUInt16 ();}  catch  (Exception)  {eof = true;}
      while  (!eof)
      {
        Sipper3DecodeRec dr = decodeRecs[rec];
        if  (!dr.ImageData ())
        {
          if  (dr.SensorNum () == ctdSensorNum)
            AddToCTDData (dr.SensorData ());

          else if  (dr.SensorNum () == pitchAndRollNum)
            AddToPitchAndRollData (dr.SensorData ());
        }

        
        try  {rec = fs.ReadUInt16 ();}  catch  (Exception)  {eof = true;}
      }
    }  /* ExtractDataFromSIPPERFile */

  }   /* Form1 */
}