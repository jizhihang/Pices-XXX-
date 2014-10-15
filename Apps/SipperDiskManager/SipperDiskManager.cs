using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

using PicesInterface;

using SipperDiskUtilities;
using SipperFileViewer;
using SipperFile;


namespace SipperDiskManager
{
  public partial class SipperDiskManager: Form
  {
    private  SipperDisk                  disk          = null;
    private  PartitionTableEntry         partition     = null;
    private  SipperFileControlBlockList  selectedFiles = null;

    private  long                        availableBytes     = 0;
    private  long                        selectedBytes      = 0;
    
    private  string                      destDir = "c:\\TrainingApp\\SipperFiles";

    private  int                         oldHeight = 0;
    private  int                         oldWidth  = 0;

    private  bool                        readDirFromLogFiles = false;


    bool  SipperFileListViewDisabled = false;


    public SipperDiskManager ()
    {
      destDir = OSservices.AddSlash (PicesSipperVariables.PicesHomeDir ()) + "SipperFiles";
      OSservices.CreateDirectory (destDir);

      InitializeComponent ();

      UpdateAvailableBytes ();
      DownLoadDir.Text = destDir;
      
      selectedFiles = new SipperFileControlBlockList ();
      UpdateAvailableBytes ();
      
      oldHeight = this.Height;
      oldWidth  = this.Width;
    }




    private  void  UpdateAvailableBytes ()
    {
      availableBytes = 0;
    
      DirectoryInfo ourDir = new DirectoryInfo (destDir);
      DirectoryInfo ourRoot  = ourDir.Root;
      string   ourRootStr = ourRoot.ToString ().ToLower ();
    
      DriveInfo[]  drives = DriveInfo.GetDrives ();
      
      foreach  (DriveInfo di in drives)
      {
        DirectoryInfo rootDir = di.RootDirectory;
        string  rootDirStr = rootDir.ToString ().ToLower ();
        if  (rootDirStr == ourRootStr)
        {
          availableBytes = di.AvailableFreeSpace;
          break;
        }
      }
      
      float  availableGig = (float)availableBytes / 1000000000.0f;
      FreeSpace.Text = availableGig.ToString ("#,##0.00" + " Gb");
    }  /* UpdateAvailableBytes */





    private  void  UpdateSelectedBytes ()
    {
      selectedBytes = 0;
      
      SipperFileControlBlockList  selecetedFiles = GetSelectedFiles ();
      foreach  (SipperFileControlBlock  fcb in  selecetedFiles)
      {
        selectedBytes += fcb.FileSizeInBytes;
      }
    
      double  selectedGig = (double)selectedBytes / 1000000000.0;
      SelectedSpace.Text = selectedGig.ToString ("#,##0.00") + " Gb";
    }  /* UpdateSelectedBytes */



    private  SipperFileControlBlockList  GetSelectedFiles ()
    {
      SipperFileControlBlockList  selectedFCBs = new SipperFileControlBlockList ();
      
      ListView.CheckedListViewItemCollection   checkedItems  = SipperFileListView.CheckedItems;
      
 
      foreach  (ListViewItem  lvi  in  checkedItems)
      {
        SipperFileControlBlock  fcb = ((FcbListViewItem)lvi).FCB;
        if  (!fcb.DownLoaded)
        {
          selectedFCBs.Add (fcb);
        }
      }

      return  selectedFCBs;
    }  /* GetSelectedFiles */



    class  FcbListViewItem:  ListViewItem
    {
      SipperFileControlBlock  fcb = null;
     
      public  FcbListViewItem (SipperFileControlBlock  _fcb):
                  base (_fcb.FileName)
      {
        fcb = _fcb;

        double  fileSizeMeg = (double)(fcb.FileSizeInBytes) / 1000000.0;
      
        SubItems.Add (fcb.TimeStamp.ToString   ("yyyy/MMM/dd - HH:mm:ss"));
        SubItems.Add (fileSizeMeg.ToString     ("###,##0.00" + " Mb"));
        SubItems.Add (fcb.StartSector.ToString ("###,###,##0"));
        SubItems.Add (fcb.EndSector.ToString   ("###,###,##0"));
        SubItems.Add (fcb.Desccription);
        if  (fcb.DownLoaded)
          BackColor = Color.LightGray;
      }

      public  SipperFileControlBlock  FCB  {get {return  fcb;}}
      
    }  /* FcbListViewItem */
    
    



    protected void  ViewSipperFile (Object     sender,
                                    EventArgs  e
                                   )
    {
      //int  x = 7878;
      //SipperFileListView.Mou
      
      if  (SipperFileListView.SelectedItems.Count == 1)
      {
        SipperFileControlBlock  fcb = ((FcbListViewItem)SipperFileListView.SelectedItems[0]).FCB;
        SipperDiskFile  sdf = null;
        
        try
        {
          sdf = disk.OpenSipperDiskFile (fcb.FcbIndex);
        }
        catch  (Exception)
        {
          MessageBox.Show ("ViewSipperFile[" + fcb.FileName + "] Could not be opened");
          return;
        }
        
        BufferedStream  bs = new BufferedStream (sdf, 100 * 1024);
        SipperFileViewer.SipperFileViewer  sfv = new SipperFileViewer.SipperFileViewer (bs, "SIPPER Disk Manager [" + fcb.FileName + "]", fcb.FileName);

        //SipperFileViewer.SipperFileViewer  sfv = new SipperFileViewer.SipperFileViewer (sdf, "SIPPER Disk Manager [" + fcb.FileName + "]", fcb.FileName);


        sfv.ShowDialog ();
        sfv.Dispose ();
       
        bs.Dispose ();
        bs = null;        

        sdf.Close ();
        sdf.Dispose ();
        sdf = null;
      }
    
    }  /* ViewSipperFile */





    protected void  ViewSipperHeader (Object     sender,
                                      EventArgs  e
                                     )
    {
    }  /* ViewSipperHeader */




    private  void  LoadSipperFileListView ()
    {
      SipperFileListView.Items.Clear ();
      
      
      {
        ContextMenuStrip  cms = new ContextMenuStrip ();
        cms.Items.Add ("View File",        null, ViewSipperFile);
        cms.Items.Add ("View Header Info", null, ViewSipperHeader);
        SipperFileListView.ContextMenuStrip = cms;
      } 


      disk = null;
      try  
      {
        disk = new SipperDisk (this.readDirFromLogFiles);
      }
      catch
      {
        if  (disk != null)
        {
          disk.Dispose ();
          disk= null;
        }
      }
      
      if  (disk == null)
        return;

      SipperFileListView.Enabled = false;
      SipperFileListViewDisabled = true;

      partition = disk.GetSipperPartitionTableEntry ();

      SipperFileControlBlockList  files = disk.SipperFiles ();
      foreach  (SipperFileControlBlock  fcb  in  files)
      {
        FcbListViewItem  flvi = new FcbListViewItem (fcb);
        SipperFileListView.Items.Add (flvi);
      }

      SipperFileListViewDisabled = false;
      SipperFileListView.Enabled = true;
      UpdateSelectedBytes ();     
    }  /* LoadSipperFileListView */





    private void  SipperDiskManager_Load (object sender, EventArgs e)
    {
      RefreshSipperDisk ();
      oldHeight = this.Height;
      oldWidth  = this.Width;
    }



    private void  RefreshSipperDisk ()
    {
      LoadSipperFileListView ();
      UpdateSipperDriveStats ();
      UpdateSelectedBytes ();

      if  (disk == null)
      {
        MessageBox.Show ("Could not locate a SIPPER Disk");
      }
      else
      {
        UpdateSipperDriveStats ();
      }
    }  /* RefreshSipperDisk */




    private void  UpdateSipperDriveStats ()
    {
      float  diskSize  = 0.0f;
      float  spaceUsed = 0.0f;
      float  available = 0.0f;
      DriveNumber.Text = "";

      if  (disk != null)
      {
        DriveNumber.Text  = disk.PhysicalDriveNumber.ToString ("0");
        diskSize  = (float)disk.DiskSizeBytes  / (float)1000000000.0f;
        spaceUsed = (float)disk.TotalBytesUsed / (float)1000000000.0f;
        available = diskSize - spaceUsed;
      }

      DiskSize.Text          = diskSize.ToString  ("##,##0.00" + " Gb");
      DiskSpaceUsed.Text     = spaceUsed.ToString ("##,##0.00" + " Gb");
      DiskSpaceAvailabe.Text = available.ToString ("##,##0.00" + " Gb");
    }  /* UpdateSipperDriveStats */




    private void  Download_Click (object sender, EventArgs e)
    {
      UpdateAvailableBytes ();
      SipperFileControlBlockList  selectedFCBs = GetSelectedFiles ();
     
      if  (selectedFCBs.Count < 1)
      {
        MessageBox.Show ("No files are selected");
        return;
      }
      
      CheckHardDriveTimer.Enabled = false;  // While we are downloading;  we do not want to be periodically 
                                            // polling the hard drive.  That would interfere with the download.
      
      DownLoadFiles  downLoadFiles = new DownLoadFiles (disk, selectedFCBs, destDir);
      downLoadFiles.ShowDialog ();
      downLoadFiles.Dispose ();
      downLoadFiles = null;

      UpdateAvailableBytes ();

      for  (int x =0;  x < SipperFileListView.Items.Count;  x++)
      {
        FcbListViewItem  fcbLVI = (FcbListViewItem)SipperFileListView.Items[x];
        SipperFileControlBlock  fcb = fcbLVI.FCB;
        if  (fcb.DownLoaded)
          fcbLVI.BackColor = Color.LightGray;
      }
      
      CheckHardDriveTimer.Enabled = true;

    }  /* Download_Click */



    private void  SipperFileListView_ItemChecked (object sender, ItemCheckedEventArgs e)
    {
      if  (!SipperFileListViewDisabled)
        UpdateSelectedBytes ();     
    }



    private void destinationToolStripMenuItem_Click (object sender, EventArgs e)
    {
      FolderBrowserDialog dialog = new FolderBrowserDialog ();
      dialog.RootFolder = Environment.SpecialFolder.MyComputer;
    
      if (dialog.ShowDialog() == DialogResult.OK)
      {
        string  selectedDir = dialog.SelectedPath;
        if  (Directory.Exists (selectedDir))
        {
          destDir = selectedDir;
          DownLoadDir.Text = destDir;
          UpdateAvailableBytes ();
        }
        else
        {
          MessageBox.Show ("[" + selectedDir +"] is not a valid dirctory", "Select Desination Directory", MessageBoxButtons.OK);
        }
      }
      
      dialog.Dispose ();
      dialog = null;
    }


    private void exitToolStripMenuItem1_Click (object sender, EventArgs e)
    {
      DialogResult result = MessageBox.Show ("Are you sure that you want to exit?", "Download SIPPER Files", MessageBoxButtons.YesNo);
      if  (result == DialogResult.Yes)
      {
        Close ();
      }
    }


    private  bool  SeeIfAllFilesHaveBeenDownLoaded ()
    {
      return  true;
    }  /* SeeIfAllFilesHaveBeenDownLoaded */



    private void resetDirectoryToolStripMenuItem_Click (object sender, EventArgs e)
    {
      bool  allFilesDownLoaded = SeeIfAllFilesHaveBeenDownLoaded ();
      if  (!allFilesDownLoaded)
      {
        DialogResult okToContinue =   
               MessageBox.Show ("Not all SIPPER files have been downloaded" + "\n\n" +
                                "    Are You Sure You Want To Continue ?",
                                "Reset SIPPER Drive",
                                MessageBoxButtons.YesNo
                               );
        if  (okToContinue == DialogResult.No)
          return;
      }
   
     
      {
        DialogResult okToContinue =   
               MessageBox.Show ("You have asked to reset the SIPPER drive; once" + "\n" +
                                "we do this the data on the drive will be lost"  + "\n\n" +
                                "      Are You Sure You Want To Do This ?",
                                "Reset SIPPER Drive",
                                MessageBoxButtons.YesNo
                               );
        if  (okToContinue == DialogResult.No)
          return;
      }
   
      disk.Dispose ();
      disk = null;
      
      LoadSipperFileListView ();
      UpdateAvailableBytes ();
      selectedFiles = new SipperFileControlBlockList ();
      
      disk.ResetSipperDirectory ();
      MessageBox.Show ("The SIPPER drive has ben reset", "Reset SIPPER Drive",  MessageBoxButtons.OK);      
    }



    private void CheckHardDriveTimer_Tick (object sender, EventArgs e)
    {
      // Lets see if the same Sipper Hard drive is plugged in
      if  (disk == null)
      {
        // Since there was not a disk attached;  lets see if there is one now.
        try
        {
          disk = new SipperDisk (readDirFromLogFiles);
        }
        catch  (Exception)
        {
          // Still no disk attached.
          disk = null;
          return;
        }
        
        disk.Dispose ();
        disk = null;
        
        // There is a SIPPER drive attached mow;  lets refresh the screen.
        RefreshSipperDisk ();
        return;
      }
      
      else
      {
        if  (disk.SipperFilesOpenedCount() > 0)
        {
          // If sipper files are opened for this drive;  we do not want to fool around with 
          return;
        }
          
        if  (disk.AreWeTheSameDrive ())
          return;
      }
 
      // Not the same SIPPER drive loaded now.  We need to clear the screen
      RefreshSipperDisk ();
    }  /* CheckHardDriveTimer_Tick */



    private void ReFresh_Click (object sender, EventArgs e)
    {
      RefreshSipperDisk ();
    }



    private void SipperDiskManager_Resize (object sender, EventArgs e)
    {
      int  increaseInHeight = Height - oldHeight;
      int  increaseInWidth  = Width  - oldWidth;
               
      SipperFileListView.Height = SipperFileListView.Height + increaseInHeight;
      SipperFileListView.Width  = SipperFileListView.Width  + increaseInWidth;
      
      BottomPanel.Top   =  BottomPanel.Top    +  increaseInHeight;
      BottomPanel.Width =  BottomPanel.Width  +  increaseInWidth;
      
      ReFresh.Left = ReFresh.Left + increaseInWidth;
      
      oldHeight = Height;
      oldWidth  = Width;
    }


    private void SipperDiskManager_ResizeEnd (object sender, EventArgs e)
    {
    }

    private void initializeDrivToolStripMenuItem_Click (object sender, EventArgs e)
    {

    }

    private void readDirectoryFromLogFilesToolStripMenuItem_Click (object sender, EventArgs e)
    {
      if  (readDirFromLogFiles)
      {
        readDirFromLogFiles = false;
        readDirectoryFromLogFilesToolStripMenuItem.Text = "Read Directory From Log Files";
      }
      else
      {
        readDirFromLogFiles = true;
        readDirectoryFromLogFilesToolStripMenuItem.Text = "Read Directory From SIPPER drive";
      }

      LoadSipperFileListView ();
    }
  }
}