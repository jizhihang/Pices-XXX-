using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;

namespace SipperInterface
{
  public class  SipperCommand
  {
    public enum  CmdTypes  
    {
      NullCmd,
      ChangeBatteryCmd,
      GetCammeraLineCmd,
      GetCameraParametersCmd,
      PreviewCmd,
      ReBootBatteryCmd,
      StatusCmd,
      StartRecordingCmd,
      StopRecordingCmd,
      SetFileNameCmd,
      SetFileNameAndStartRecordingCmd,
      ReadSerialPortCmd,
      UserCmd,
    };


    private  StreamWriter           logFile    = null;
    private  CircularBuffer         sipperBuff = null;

    private  CmdTypes               cmd                     = CmdTypes.NullCmd;
    private  int                    batteryNum              = 0;
    private  int                    blocksToRead            = 0;
    private  CameraParameters       cameraParams            = null;
    private  string                 fileName                = "";
    private  string                 fileDesc                = "";
    private  string                 fileNameCreated         = "";    // File name returned by StartRecord command
    private  long                   previewBlock            = 0;
    private  byte[]                 previewData             = null;
    private  int                    serialPort              = 0;
    private  string                 userCmd                 = "";

    private  string                 result            = "";
    private  bool                   executedOk        = false;
    private  byte[]                 cameraLine        = null;
    
    private  Queue<SipperCommand>   resultQueue       = null;  // Queue that we get added to when execution
                                                               // of this command is completed.
    

    public  SipperCommand (CmdTypes              _cmd,
                           Queue<SipperCommand>  _resultQueue,
                           CircularBuffer        _sipperBuff,
                           StreamWriter          _logFile
                          )
    {
      cmd            = _cmd;
      sipperBuff     = _sipperBuff;
      resultQueue    = _resultQueue;
      logFile        = _logFile;
    }

    
    public  CmdTypes               Cmd             ()  {return cmd;}
    
    public  int                    BlocksToRead    ()  {return blocksToRead;}
    public  byte[]                 CameraLine      ()  {return cameraLine;}
    public  CameraParameters       CameraParams    ()  {return cameraParams;}
    public  bool                   ExecutedOk      ()  {return executedOk;}
    public  string                 FileNameCreated ()  {return fileNameCreated;}
    public  long                   PreviewBlock    ()  {return previewBlock;}
    public  byte[]                 PreviewData     ()  {return previewData;}
    public  string                 Result          ()  {return result;}
    public  Queue<SipperCommand>   ResultQueue     ()  {return resultQueue;}
    



    public  static SipperCommand  CreateChangeBatteryCmd (Queue<SipperCommand>  _resultQueue,
                                                          int                   _batteryNum,
                                                          CircularBuffer        _circularBuffer,
                                                          StreamWriter          _logFile
                                                         )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.ChangeBatteryCmd, _resultQueue, _circularBuffer, _logFile);
      cmd.batteryNum = _batteryNum;
      return  cmd;
    }  /* CreateReadSerialPortCmd */




    public static SipperCommand  CreateReadSerialPortCmd (Queue<SipperCommand>  _resultQueue,
                                                          int                   _serialPort,
                                                          CircularBuffer        _circularBuffer,
                                                          StreamWriter          _logFile
                                                         )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.ReadSerialPortCmd, _resultQueue, _circularBuffer, _logFile);
      cmd.serialPort = _serialPort;
      return  cmd;
    }  /* CreateReadSerialPortCmd */



    public static SipperCommand CreateGetCammeraLineCmd (Queue<SipperCommand>  _resultQueue,
                                                         CircularBuffer        _circularBuffer,
                                                         StreamWriter          _logFile
                                                        )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.GetCammeraLineCmd, _resultQueue, _circularBuffer, _logFile);
      return  cmd;
    }  /* CreateReadSerialPortCmd */




    public static SipperCommand CreateGetCammeraParametersCmd (Queue<SipperCommand>  _resultQueue,
                                                               CircularBuffer        _circularBuffer,
                                                               StreamWriter          _logFile
                                                              )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.GetCameraParametersCmd, _resultQueue, _circularBuffer, _logFile);
      return  cmd;
    }  /* CreateReadSerialPortCmd */




    public  static  SipperCommand  CreatePreviewCmd (Queue<SipperCommand>  _resultQueue,
                                                     int                   _blocksToRead,
                                                     CircularBuffer        _circularBuffer,
                                                     StreamWriter          _logFile
                                                    )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.PreviewCmd, _resultQueue, _circularBuffer, _logFile);
      cmd.blocksToRead = _blocksToRead;
      return  cmd;
    }  /* CreateReadSerialPortCmd */



    public static SipperCommand CreateReBootBatteryCmd (Queue<SipperCommand> _resultQueue,
                                                        CircularBuffer       _circularBuffer,
                                                        StreamWriter         _logFile
                                                       )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.ReBootBatteryCmd, _resultQueue, _circularBuffer, _logFile);
      return  cmd;
    }  /* CreateReBootBatteryCmd */



    public static SipperCommand CreateStartRecordingCmd (Queue<SipperCommand>  _resultQueue,
                                                         CircularBuffer        _circularBuffer,
                                                         StreamWriter          _logFile
                                                        )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.StartRecordingCmd, _resultQueue, _circularBuffer, _logFile);
      return  cmd;
    }  /* CreateReadSerialPortCmd */



    public static SipperCommand CreateStopRecordingCmd (Queue<SipperCommand>  _resultQueue,
                                                        CircularBuffer        _circularBuffer,
                                                        StreamWriter          _logFile
                                                       )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.StopRecordingCmd, _resultQueue, _circularBuffer, _logFile);
      return cmd;
    }  /* CreateReadSerialPortCmd */



    public static SipperCommand  CreateSetFileNameCmd (Queue<SipperCommand> _resultQueue,
                                                       string               _fileName,
                                                       string               _fileDesc,
                                                       CircularBuffer       _circularBuffer,
                                                       StreamWriter         _logFile
                                                      )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.SetFileNameCmd, _resultQueue, _circularBuffer, _logFile);
      cmd.fileName = _fileName;
      cmd.fileDesc = _fileDesc;
      return cmd;
    }  /* CreateReadSerialPortCmd */



    public static SipperCommand  CreateSetFileNameAndStartRecordingCmd (Queue<SipperCommand>  _resultQueue,
                                                                        string                _fileName,
                                                                        string                _fileDesc,
                                                                        CircularBuffer        _circularBuffer,
                                                                        StreamWriter          _logFile
                                                                       )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.SetFileNameAndStartRecordingCmd, _resultQueue, _circularBuffer, _logFile);
      cmd.fileName = _fileName;
      cmd.fileDesc = _fileDesc;
      return cmd;
    }  /* CreateReadSerialPortCmd */





    public static  SipperCommand  CreateUserCmd (Queue<SipperCommand> _resultQueue,
                                                 string               _userCmd,
                                                 CircularBuffer       _circularBuffer,
                                                 StreamWriter         _logFile
                                                )
    {
      SipperCommand cmd = new SipperCommand (CmdTypes.UserCmd, _resultQueue, _circularBuffer, _logFile);
      cmd.userCmd = _userCmd;
      return cmd;
    }  /* CreateReadSerialPortCmd */




    public  void  Execute ()
    {
      switch  (cmd)
      {
        case CmdTypes.NullCmd:                                                             break;
        case CmdTypes.ChangeBatteryCmd:                  ChangeBattery       ();           break;
        case CmdTypes.GetCammeraLineCmd:                 GetCameraLine       ();           break;
        case CmdTypes.GetCameraParametersCmd:            GetCameraParameters ();           break;
        case CmdTypes.PreviewCmd:                        Preview             ();           break;
        case CmdTypes.ReBootBatteryCmd:                  ReBootBattery       ();           break;
        case CmdTypes.SetFileNameCmd:                    SetFileName         ();           break;
        case CmdTypes.SetFileNameAndStartRecordingCmd:   SetFileNameAndStartRecording ();  break;
        case CmdTypes.StartRecordingCmd:                 StartRecording      ();           break;
        case CmdTypes.StopRecordingCmd:                  StopRecording       ();           break;
        case CmdTypes.UserCmd:                           UserCommand         ();           break;
      }
    }  /* Execute */




    private void  ChangeBattery ()
    {
      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("// ChangeActiveBattery [" + batteryNum.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }
    
      Thread.Sleep (100);

      string cmd = "sipper_battery s" + batteryNum.ToString ();
      result = sipperBuff.SubmitCommandAndReturnResults (cmd, 1000);
      executedOk = true;
    }  /* ChangeBattery */





    private  void  GetCameraLine ()
    {
      lock (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("// GetCameraLine");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      sipperBuff.TurnOnCameraMode ();


      string  cmdStr = "get_line";
      sipperBuff.SendText (cmdStr);
      cameraLine = sipperBuff.GetCameraLineData (4000);


      sipperBuff.TurnOffCameraMode ();
      
      if  (cameraLine != null)
          executedOk = true;
      else
          executedOk = false;
    }  /* GetCameraLine */





    private void  ReBootBattery ()
    {
      lock  (logFile)
      { 
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");
        logFile.WriteLine ("// ReBootBattery    Curent Time[" + DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }
        
      string cmd = "battery r";
      result = sipperBuff.SubmitCommandAndReturnResults (cmd, 200);
      executedOk = true;
      
      return;
    }  /* ReBootBattery */



    private  void  Preview ()
    {
      sipperBuff.PerformStatusCommandAndUpdateSipperDiskStats ();

      if  (blocksToRead < 1)
        blocksToRead = 100;

      long  startBlockAddr = sipperBuff.StartBlockAddr ();
      long  curBlockAddr   = Math.Max (sipperBuff.CurBlockAddr(), startBlockAddr);
      long  endBlockAddr   = sipperBuff.EndBlockAddr();

      //checking hex values to decimal ones to be used
      previewBlock = curBlockAddr - blocksToRead; //starting block to read from 

      //making sure starting block is legit
      if  (previewBlock <= startBlockAddr)
      {
        try { blocksToRead = (int)(curBlockAddr - startBlockAddr); }
        catch (Exception)
        { blocksToRead = 100; }

        previewBlock = curBlockAddr - blocksToRead;
      }

      if  (blocksToRead <= 0)
      {
        lock (logFile)
        {
          logFile.WriteLine ("//");
          logFile.WriteLine ("// As per status command there is no data to preview.");
          logFile.WriteLine ("//");
          logFile.Flush ();
        }
        result = "Error! There is no data to be previewed.  Please record for longer.";
        executedOk = false;
        return;
      }

      previewData = sipperBuff.ReadHardDrive (previewBlock, blocksToRead);
      if  (previewData == null)
      {
        result = "Error reading hard drive.";
        executedOk = false;
        return;
      }
      
      executedOk = true;
    }  /* Preview */





    private  void  UserCommand ()
    {
      lock (logFile)
      {
        logFile.WriteLine("// Submitting User Cmd[" + this.userCmd + "].");
        logFile.Flush();
      }
      
      this.result = sipperBuff.SubmitCommandAndReturnResults (userCmd, 5000);
    }  /* UserCommand */




    private  void  SetFileName ()
    {
      //Before recording, setting up the file description to Bill
      lock  (logFile)
      {
        logFile.WriteLine ("// Setting File Name       :" + fileName);
        logFile.WriteLine ("// Setting File Description:" + fileDesc);
        logFile.Flush ();
      }
      
      sipperBuff.SetFileName (ref executedOk, fileName, fileDesc);

      return; 
    }  /* SetFileName */





    private  void  SetFileNameAndStartRecording ()
    {
      //Before recording, setting up the file description to Bill
      lock  (logFile)
      {
        logFile.WriteLine ("// Setting FileName And Start Recording");
        logFile.WriteLine ("//     File Name       :" + fileName);
        logFile.WriteLine ("//     File Description:" + fileDesc);
        logFile.Flush ();
      }

      sipperBuff.SetFileName (ref executedOk, fileName, fileDesc);
      
      if  (executedOk)
      {
        sipperBuff.StartRecording (ref executedOk, ref fileNameCreated);
      }

      return; 
    }  /* SetFileNameAndStartRecording */




    private  void  StartRecording ()
    {
      //Before recording, setting up the file description to Bill
      lock  (logFile)
      {
        logFile.WriteLine ("// Start Recording");
        logFile.Flush ();
      }

      sipperBuff.StartRecording (ref executedOk, ref fileNameCreated);

      return; 
    }  /* StartRecording */



    private  void  StopRecording ()
    {
      lock  (logFile)
      {
        logFile.WriteLine ("// Start Recording");
        logFile.Flush ();
      }

      sipperBuff.StopRecording (ref executedOk);
    }  /* StopRecording */



    
    
    private  void  GetCameraParameters ()
    {
      result = sipperBuff.GetCameraParametersStr ();
      cameraParams = new CameraParameters (result);

      if  (cameraParams.ScanRate <= 1.0f)
      {
        result = sipperBuff.GetCameraParametersStr ();
        cameraParams = new CameraParameters (result);
      }
    }  /* GetCameraParameters */
    
  }  /* SipperCommand */
}
