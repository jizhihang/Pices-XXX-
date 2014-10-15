using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;


using PicesInterface;

using SipperFile;
using SipperDiskUtilities;



namespace SipperDiskManager
{
  public partial class DownLoadFiles: Form
  {
    private  bool                        closeForm   = false;
    private  SipperFileControlBlockList  filesToCopy = null;
    private  string                      destDir     = null;
    private  SipperDisk                  disk        = null;
  
    private  int                         filesTotal  = 0;
    private  int                         filesCopied = 0;
    private  long                        bytesTotal  = 0;
    private  long                        bytesCopied = 0;
    private  FileCopyStats               currentFCS  = null;
    private  List<FileCopyStats>         fileCopyStatsList = null;
    
    private  Thread                      downLoadingThread    = null;
    private  bool                        downLoadingRunning   = false;
    private  bool                        downLoadingTerminate = false;

  
    public DownLoadFiles (SipperDisk                  _disk,
                          SipperFileControlBlockList  _filesToCopy,
                          string                      _destDir
                         )
    {
      disk        = _disk;
      filesToCopy = _filesToCopy;
      destDir     = _destDir;
      
      fileCopyStatsList = new List<FileCopyStats> ();
      
      InitializeComponent ();

      BuildFileListView ();
       
      downLoadingThread = new Thread (new ThreadStart (CopyFileList));
      downLoadingThread.Start ();
    }


    private  void  CleanUp ()
    {
      TerminateDownloadingThread ();
      fileCopyStatsList = null;
    }  /* CleanUp */



    private  void  UpdateScreen ()
    {
      int  filesLeft = filesTotal - filesCopied;
      FilesTotal.Text   = filesTotal.ToString  ("##0");
      FilesCopied.Text  = filesCopied.ToString ("##0");
      FilesLeft.Text    = filesLeft.ToString   ("##0");
      
      float  megTotal   = (float)bytesTotal  / 1000000.0f;
      float  megCopied  = (float)bytesCopied / 1000000.0f;
      float  megLeft    = megTotal - megCopied;

      MegTotal.Text  = megTotal.ToString  ("##,##0.00");
      MegCopied.Text = megCopied.ToString ("##,##0.00");
      MegLeft.Text   = megLeft.ToString   ("##,##0.00");
      
      foreach (FileCopyStats  fcs in  fileCopyStatsList)
        fcs.UpdateDisplayFields ();

      if  (currentFCS != null)
      {
        int  maximum = (int)(currentFCS.Size / 1000000);
        int  value   = Math.Min (((int)(currentFCS.Copied / 1000000)), maximum);
        CurrentFileProgress.Minimum = 0;
        CurrentFileProgress.Maximum = maximum;
        CurrentFileProgress.Value   = value;
        float  percentCopied = 100.0f * (float)value / (float)maximum;
        CurrentFilePercentDone.Text = percentCopied.ToString ("##0.0") + "%"; 
      }

      {
        int  maximum = (int)(bytesTotal / 1000000);
        int  value   = Math.Min (((int)(bytesCopied / 1000000)), maximum);
        AllFilesProgress.Minimum  = 0;
        AllFilesProgress.Maximum  = maximum;
        AllFilesProgress.Value    = value;
        float percentCopied = 100.0f * (float)value / (float)maximum;
        AllFilesPercentDone.Text = percentCopied.ToString("##0.0") + "%"; 
      }
      
    }  /* UpdateScreen */
    

    

    public  class  FileCopyStats
    {
      SipperFileControlBlock   fcb;
      long                     size;
      long                     copied;
      long                     left;
      ListViewItem             lvi           = null;
      bool                     updateDisplay = true;
      long                     bytesSkipped = 0;


      public  FileCopyStats (SipperFileControlBlock  _fcb,
                             ListViewItem            _lvi
                            )
      {
        lvi      = _lvi;
        fcb      = _fcb;
        size     = fcb.FileSizeInBytes;
        copied   = 0;
        left     = fcb.FileSizeInBytes;
        updateDisplay = true;
        bytesSkipped = 0;
        UpdateDisplayFields ();
      }
 
 
 
      public  long    Size
      {
        get {return  size;}
        set 
        {
          size = value;
          left = size - copied;
          updateDisplay = true;
        }
      }
 
 
 
      
      public  SipperFileControlBlock  Fcb  {get {return  fcb;}}
      
      public  string  FileName   {get {return  fcb.FileName;}}
      public  long    Copied     {get {return  copied;}         set {copied = value;}}
      public  long    Left       {get {return  left;}           set {left   = value;}}


      public  void  UpdateDisplayFields ()
      {
        if  (updateDisplay)
        {
          float  sizeMeg   = (float)size   / 1000000.0f;
          float  copiedMeg = (float)copied / 1000000.0f;
          float  leftMeg   = sizeMeg - copiedMeg;
          float  bytesSkippedMeg = (float)bytesSkipped / 1000000.0f;
 
          lvi.SubItems.Clear();
          lvi.Text = fcb.FileName;
          lvi.SubItems.Add (sizeMeg.ToString         ("##,##0.00") + " Mb");
          lvi.SubItems.Add (copiedMeg.ToString       ("##,##0.00") + " Mb");
          lvi.SubItems.Add (leftMeg.ToString         ("##,##0.00") + " Mb");
          lvi.SubItems.Add (bytesSkippedMeg.ToString ("##,##0.00") + " Mb");
          if  (left <= 0)
          {
            lvi.BackColor = Color.LightGray;
            lvi.ForeColor = Color.Black;
          }
          updateDisplay = false;
        }
      }
      


      public  void  AddToCopied (int  count)
      {
        copied += (long)count;
        left   -= (long)count;
        updateDisplay = true;
      }


      public  void  AddToSkipped (long  bytesSkipped)
      {
      }

    }  /* FileCopyStats */




    private  void  BuildFileListView ()
    {
      filesTotal  = 0;
      filesCopied = 0;
      bytesTotal  = 0;
      bytesCopied = 0;

      FileListView.Items.Clear ();
      
      
      foreach  (SipperFileControlBlock  fcb  in  filesToCopy)
      {
        string  fileName = fcb.FileName;
        ListViewItem  lvi = new ListViewItem (fileName);
        FileCopyStats  fcs = new FileCopyStats (fcb, lvi);
        fileCopyStatsList.Add (fcs);
        FileListView.Items.Add (lvi);
        filesTotal++;
        bytesTotal += fcb.FileSizeInBytes;
      }
    }  /* BuildFileListView */





    private  void  CopyFileList ()
    {
      int  readBlockSizeMax  = 100 * 1024;
      int  readBlockSize     = readBlockSizeMax;
      int  readErrorsInARow  = 0;
      
      downLoadingRunning = true;
    
      byte[]  buff = new byte[readBlockSizeMax];
      foreach (FileCopyStats  fcs in  fileCopyStatsList)
      {
        SipperFileControlBlock fcb = fcs.Fcb;
        
        SipperDiskFile  s = disk.OpenSipperDiskFile (fcb.FcbIndex);
        if  (s == null)
          return;
 
        fcs.Size = s.Length;
 
        string  destFileName = OSservices.AddSlash (destDir) + fcs.FileName;
        destFileName = destFileName.Trim ('\0') + ".spr";
        
        FileStream destFile = null;
        try
        {
          destFile = new FileStream (destFileName, FileMode.Create, FileAccess.Write, FileShare.None);
        }
        catch  (Exception e)
        {
          MessageBox.Show ("Error opening File[" + destFileName + "] occured." + "\n\n" + e.ToString (),
                           "CopyFileList",
                           MessageBoxButtons.OK
                          );
          // We will try continuing on to the next file to download.
          continue;
        }
 
        currentFCS = fcs;
          
        bool  eof = false;
        long  nextReadOffset = 0;
        long  bytesToSkipped = 0;
        
        while  ((!downLoadingTerminate)  &&  (!eof))
        {
          int  bytesRead = 0;
          try
          {
            if  (nextReadOffset !=  s.Position)
            {
              s.Seek (nextReadOffset, SeekOrigin.Begin);
            }
          
            bytesRead = s.Read (buff, 0, readBlockSize);
            nextReadOffset = s.Position;
            if  (bytesRead == 0)
              eof = true;
            else
            {
              // Since we did not have an error;  we might as well set the 
              // readBlockSize to its max
              readBlockSize = buff.Length;
              readErrorsInARow = 0;
              bytesToSkipped   = 0;
            }
              
          }
          catch  (Exception e)
          {
            DialogResult  dr = MessageBox.Show ("Disk Error Occured trying to read SIPPER disk" + "\n\n" +
                                                "Do you want to keep on readng?"                + "\n\n" +
                                                e.ToString (),
                                                "CopyFileList", 
                                                MessageBoxButtons.YesNo
                                               );
            if  (dr == DialogResult.No)
            {
              eof = true;
              break;
            }
            else
            {
              readErrorsInARow++;
              readBlockSize = 4096;
              if  (readErrorsInARow < 5)
                bytesToSkipped = 4 * 1024;

              else if  (readErrorsInARow < 10)
                bytesToSkipped = 20 * 1024;

              else if  (readErrorsInARow < 15)
                bytesToSkipped = 100 * 1024;

              else if  (readErrorsInARow < 20)
                bytesToSkipped = 1024 * 1024;

              else 
                bytesToSkipped = 10 * 1024 * 1024;
            }
            nextReadOffset += bytesToSkipped;
          }
          
            
          if  (!eof)
          {
            try {destFile.Write (buff, 0, bytesRead);}
            catch  (Exception e)
            {
              downLoadingTerminate = true;
              MessageBox.Show (e.ToString (), "Error saving Sipper File[" + fcs.FileName + "]", MessageBoxButtons.OK);
              break;
            }
            
            fcs.AddToCopied (bytesRead);
            fcs.AddToSkipped (bytesToSkipped);
            bytesCopied += bytesRead;
          }
        }
        
        s.Close ();
        s.Dispose ();
        s = null;
        
        destFile.Close ();
        destFile.Dispose ();
        destFile = null;
        
        if  (fcs.Copied >= fcs.Size)
          fcb.DownLoaded = true;
        
        if  (downLoadingTerminate)
          break;
          
        filesCopied++;
      }
      
      downLoadingRunning = false;
      closeForm          = true;
    }  /* CopyFileList */




    private void ScreenUpdateTimer_Tick(object sender, EventArgs e)
    {
      UpdateScreen ();
      if  (closeForm)
        Close ();
    }

    
    private  void  TerminateDownloadingThread ()
    {
      if  (downLoadingThread == null)
        return;
        
      if  (downLoadingThread.IsAlive)
      {
        downLoadingTerminate= true;
      
        int  count = 0;
        while  ((downLoadingRunning)  &&  (count < 20))
        {
          Thread.Sleep (100);
          count++;
        }
      }
        
      if  (downLoadingThread != null)
      {
        if  (downLoadingThread.IsAlive)
          downLoadingThread.Abort ();
      
      }    
      
      downLoadingThread = null;
    }  /* TerminateDownloadingThread */



    private void CancelRequestButton_Click (object sender, EventArgs e)
    {
      DialogResult r = MessageBox.Show ("DO you want to cancel the download?", "", MessageBoxButtons.YesNo);
      
      if  (r == DialogResult.No)
        return;
    
      TerminateDownloadingThread ();
    }  /* CancelRequestButton_Click  */




    private void DownLoadFiles_FormClosing (object sender, FormClosingEventArgs e)
    {
      CleanUp ();
    }  /* DownLoadFiles_FormClosing */
    
    
  }  /* DownLoadFiles */
}
