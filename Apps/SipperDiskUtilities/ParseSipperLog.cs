using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;



using  PicesInterface;

namespace SipperDiskUtilities
{
  public  class ParseSipperLog  
  {
    private  string  fileName = "";

    private  uint      startSector   = 0;
    private  uint      currentSector = 0;
    private  uint      endSector     = 0;
    private  bool      recording     = false;
    private  SipperDateTimeStamp  statusDateTime = null;
    private  string    curFileName   = null;
    private  uint      curFileStartSector = 0;

    private  SipperFileControlBlockList  fileControlBlocks       = null;
    private  SipperFileControlBlock      currentFileControlBlock = null;

    private  bool  updateCurrentFileControlBlockEndSector = false;


    public  SipperFileControlBlockList  FileControlBlocks ()  {return  fileControlBlocks;}


    public  ParseSipperLog   (string  _fileName)
    {
      fileName = _fileName;
      fileControlBlocks = new SipperFileControlBlockList ();

      StreamReader sr = OpenLogFile ();
      if  (sr != null)
      {
        ParseFile (sr);
      }

      sr.Close ();
      sr = null;
    }



    private  StreamReader  OpenLogFile ()
    {
      StreamReader  sr = null;
      try
      {
        sr = new StreamReader (fileName);
      }
      catch  (Exception e)
      {
        MessageBox.Show ("Error opening Log File[" + fileName + "]" + "\n\n" + e.ToString ());
        return null;
      }

      return  sr;
    }



    private  void  ParseFile (StreamReader  sr)
    {
      string  line = null;
      
      while  ((line = sr.ReadLine ()) != null)
      {
        try
        {
          if  (line.StartsWith("bs="))
            ParseStatusLine (line);

          else if  (line.StartsWith ("Opening"))
            ParseOpenFileCommand (line);

          else if  (line.StartsWith ("File Closed."))
            ParseFileClose (line);
        }
        catch  (Exception e)
        {
          MessageBox.Show (e.ToString (), "ParseFile");
        }
      }

    }


    private  void   ParseStatusLine (string  line)
    {
      string  startSectorStr = GetStatusLineField (line, "bs=");
      if  (startSectorStr == null)
        return;

      string  currentSectorStr = GetStatusLineField (line, "bc=");
      if  (currentSectorStr == null)
        return;

      string  endSectorStr = GetStatusLineField (line, "be=");
      if  (endSectorStr == null)
        return;
  
      string  dateStr = GetStatusLineField (line, "da=");
      if  (dateStr == null)
        return;

      string  timeStr = GetStatusLineField (line, "ti=");
      if  (timeStr == null)
        return;

      string  recordStatusStr = GetStatusLineField (line, "rs=");
      if  (recordStatusStr == null)
        return;

      startSector   = (uint)OSservices.HexStrToLong (startSectorStr);
      currentSector = (uint)OSservices.HexStrToLong (currentSectorStr);
      endSector     = (uint)OSservices.HexStrToLong (endSectorStr);
      recording = (recordStatusStr == "r");
      statusDateTime = new SipperDateTimeStamp (dateStr, timeStr);

      if  ((currentFileControlBlock != null)  &&  recording)
      {
        currentFileControlBlock.EndSector = currentSector;
      }

      if  (updateCurrentFileControlBlockEndSector)
      {
        if  (currentFileControlBlock != null)
        {
          currentFileControlBlock.EndSector = currentSector;
          updateCurrentFileControlBlockEndSector = false;
        }
      }
    }  /* ParseStatusLine */
  



    private  void  ParseOpenFileCommand (string  line)
    {
      line = line.Substring (8);
      int  idx = line.IndexOf (". ");
      if  (idx < 0)
        return;

      curFileName = line.Substring (0, idx);
      line = line.Substring (idx);
      idx = line.IndexOf ("StartLBA=");
      if  (idx < 0)
        return;
      
      line = line.Substring (idx + 9);
      idx = line.IndexOf (',');
      if  (idx < 0)
        return;

      string  startLBAstr = line.Substring (0, idx).Trim ();

      curFileStartSector = (uint)OSservices.HexStrToLong (startLBAstr);

      currentFileControlBlock = new SipperFileControlBlock ();
      currentFileControlBlock.FileName    = curFileName;
      currentFileControlBlock.StartSector = curFileStartSector;
      currentFileControlBlock.SetSipperDateTimeStamp (statusDateTime);
      fileControlBlocks.Add (currentFileControlBlock);
    }  /* ParseOpenFileCommand */
  



    private  string  GetStatusLineField (string  statusLine,
                                         string  name
                                        )
    {
      int  idx = statusLine.IndexOf (name);
      if  (idx < 0)
        return null;
      string  s = statusLine.Substring (idx + name.Length);
      idx = s.IndexOf (' ');
      if  (idx < 0)
        return  s;
      string  value  = s.Substring (0, idx);
      return  value;
    }



    private  void  ParseFileClose (string  line)
    {
      if  (currentFileControlBlock == null)
        return;
      
      currentFileControlBlock.EndSector = this.currentSector;

      updateCurrentFileControlBlockEndSector = true;  // Lets the next status command know to update EndSector field
    }
  }  /* ParseSipperLog */
}

