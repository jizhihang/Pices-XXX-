using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using Microsoft.Win32.SafeHandles;

using PicesInterface;


/*!
 \class  SipperDiskUtilities.SipperDisk
 \brief  SipperDisk is used for reading and writting to a SIPPER disk drive.
 \details  The SIPPER disk has a very simple format who's details are described below.
 You read the Master Boot record and get the 1st sector from "Partition Entry 1". 
 The SIPPER Partition will start with one Partition decriptive block followed by
 2048 File Control Block, on efor each SIPPER file created.  The File control blocks
 will be followed by the actual SIPPER data.  SIPPER records data sequentially 
 starting at the begiining of the drive until the end of the drive.  There will be
 not fragmentation issues to deal with.  Each SIPPER file will have a start and end
 sector that is specified in its corresponding File Control Block.\n
 \see SipperDiskUtilities.PartitionTable
 \see SipperDiskUtilities.SipperFileControlBlock
 \see SipperDiskUtilities.SipperDiskFile
 * 
 \code
 *******************************************************************************************
 * Sipper Disk Drive Layout
 *
 *  Master Boot Record
 *   |Boot-Record-Code  (0-445)
 *   |
 *   |Partition-Table
 *   |  Partition-Entry-0            (446 - 461)
 *   |      Boot-Id           (446)
 *   |      Start-Head        (447)
 *   |      Start-Sector      (448)
 *   |      Start-Cylinder    (449)
 *   |      System-ID         (450)
 *   |      EndHead           (451)
 *   |      EndSector         (452)
 *   |      End-Cylinder      (453)
 *   |      Relative-Sector   (454 - 457)
 *   |      totalSectors      (458 - 461)
 *   |
 *   |  Partition-Entry-1            (462 - 477)     // Sipper will be partition "1".
 *   |  Partition-Entry-2            (478 - 493)
 *   |  Partition-Entry-3            (494 - 509)
 *   |  
 *   |End-Of-Sector                  (510 - 511)
 *
 *
 *
 *   (**********************************************************)
 *   (*  Start of Sipper Partition                             *)
 *   (*  The first sector will be the 'SipperPartitionSector'  *)
 *   (**********************************************************)
 *
 *  Sipper-Partition-Sector    Starts at ('Partition-Entry-1'->'Relative-Sector') * 'Logical-Block-Size(512)'
 *   | Sipper-Id             ( 0 -  11)
 *   | Total-Sectors         (12 -  15)
 *   | File-Data-Sector      (16 -  19)
 *   | End-Sector            (20 -  23)
 *   | Next-Fcb-Sector       (24 -  27)
 *   | Next-File-Handle      (28 -  31)
 *   | Unused-Space          (32 - 511)
 *
 *
 *
 *   (**********************************************************)
 *   (*  followed by 2048 'SipperFileControlBlock' sectors.    *)
 *   (*  where each one represents a single Sipper File.       *)
 *   (**********************************************************)
 *
 *  Sipper-File-Control-Block
 *   | Allocated             ( 0 -  3)
 *   | Status                ( 4 -  7)     0=Closed,  1=Opened
 *   | Start-Sector          ( 8 - 11)
 *   | End-Sector            (12 - 15)
 *   | Current-Access-Sector (16 - 19)
 *   | File-Name             (20 - 49)
 *   |
 *   | filler                (50 - 51)      For some reason SIPPER is inserting 2 empty bytes here
 *   |                                      probably beciause it is accessing memory on 4-byte boundaries.
 *   |
 *   | Date-Time-Stamp       (52 - 59)
 *   |     Second                     (52)
 *   |     Minute                     (53)
 *   |     Hour                       (54)
 *   |     Unused                     (55)
 *   |     Day                        (56)
 *   |     Date                       (57)  (1 -> 31)
 *   |     Month                      (58)  (1 -> 12)
 *   |     Year                       (59)  (0 -> 99)   ex: 7 = 2007.
 *   |
 *   | Description           (60 - 364)
 *   |
 *   | UnUsed               (365 - 511)
 *   // Unused Space
 \endcode
 */




namespace SipperDiskUtilities
{

  public class  SipperDisk
  {
    private  string                      deviceName            = null;
    private  FileStream                  fs                    = null;
    private  long                        lastSectorUsed        = 0;       // Updated by 'GetListOfSipperFiles', used to calculate amount of free space left
    private  MasterBootRecord            mbr                   = null;    // Since there is only one on the hard drive; we can keep a gobal entry.
    List<SipperDiskFile>                 openedSipperFiles     = null;
    SipperFileControlBlock               origFirstFileControlBlock  = null;  // We ue this to hel determine if the SIPPER drive has been changed.
    private  int                         physicalDriveNumber   = 0;
    private  IntPtr                      physicalDriveHandle   = IntPtr.Zero;
    private  bool                        readDirFromLogFiles   = false;
    private  SipperFileControlBlockList  sipperFiles           = null;    // Since there is only one on the hard drive; we can keep a gobal entry.f
    private  SipperPartitionSector       sipperPartitionSector = null;    // Since there is only one on the hard drive; we can keep a gobal entry.
    private  long                        totalSectorsUsed      = 0;       // Updated by 'GetListOfSipperFiles'
    private  bool                        validSipperDrive      = false;


    public  int   PhysicalDriveNumber  {get  {return  physicalDriveNumber;}}

    //public  FileStream  FS  {get  {return  fs;}}




    public  long  FSseek (long        diskPosition,
                          SeekOrigin  so
                         )
    {
      return  fs.Seek (diskPosition, SeekOrigin.Begin);
    }


    public  int  FSread (byte[]  buff,
                         int     offSet,
                         int     count
                        )
    {
      int  bytesReadFromDrive = 0;
      try
      {
        bytesReadFromDrive = fs.Read (buff, offSet, count);
      }
      catch  (IOException e1)
      {
        String  exceptionStr = e1.ToString ();
        throw new IOException (exceptionStr);
      }
      catch  (Exception  e2)
      {
        MessageBox.Show (e2.ToString (), "FSread", MessageBoxButtons.OK);
        throw  new Exception (e2.ToString ());
      }

      return  bytesReadFromDrive;
    }  /* FSread */
    


    public  long  FSposition  
    {
      get  {return  fs.Position;}
    }



    
    public  long  DiskSizeBytes 
    {
      get
      {
        long  diskSizeBytes = 0;
        if  (sipperPartitionSector != null)
          diskSizeBytes = sipperPartitionSector.DiskSizeBytes;
        return  diskSizeBytes;      
      }
    }


    public  SipperFileControlBlockList   SipperFiles ()  {return  sipperFiles;}


    public int   SipperFilesOpenedCount ()
    {
      int  sipperFilesOpenedCount = 0;
      
      BlockStart ();
      if  (openedSipperFiles == null)  
         return  0;   
      sipperFilesOpenedCount = openedSipperFiles.Count;
      BlockEnd ();

      return  sipperFilesOpenedCount;
    }


    public  long  TotalFreeDiskSectors
    {
      get
      {
        long  totalFreeDiskSectors = 0;
        if  ((sipperPartitionSector != null)  &&  (mbr != null))
          totalFreeDiskSectors = sipperPartitionSector.TotalSectors - (this.lastSectorUsed - mbr.SipperPartitionBeginSector);
         
        return  totalFreeDiskSectors;
      }
    }    public long  TotalSectorsUsed   {get {return  totalSectorsUsed;}}
    


    public long  TotalBytesUsed     {get {return  (totalSectorsUsed * Constants.LOGICAL_BLOCK_SIZE);}}


    public  PartitionTableEntry  GetSipperPartitionTableEntry ()
    {
      return  mbr.GetSipperPartitionTableEntry ();
    }

    public  bool  ValidSipperDrive     {get  {return  validSipperDrive;}}

    



    public  SipperDisk (bool  _readDirFromLogFiles)
    {
      BlockStart ();

      readDirFromLogFiles = _readDirFromLogFiles;

      openedSipperFiles = new List<SipperDiskFile> ();
      
      OpenSipperDrive ();
      if  (fs == null)
      {
        validSipperDrive = false;
        BlockEnd ();
        throw new Exception ("No SIPPER drive mounted on this system.");
      }
      
      if  (!_readDirFromLogFiles)
      {
        GetListOfSipperFiles ();
      }
      else
      {
        String  logDirName = PicesSipperVariables.SipperInterfaceLogDir ();
        GetListOfSipperFilesFromLogFiles (logDirName);
        //GetListOfSipperFilesFromLogFiles ("D:\\TrainingApp\\SipperInterfaceDir\\LogFiles");
        //GetListOfSipperFilesFromLogFiles ("C:\\Temp\\Temp22");
        //GetListOfSipperFilesFromLogFiles ("C:\\Pices\\SipperInterfaceDir\\LogFiles");
      }
      
      if  ((sipperFiles != null)  &&  (sipperFiles.Count > 0))
        origFirstFileControlBlock = this.sipperFiles[0];
      
      // We are going to keep the handle to the physical disk close.  If we were to leave it open;
      // it could cause stability issues for the system.  We will open it when ever we need to perform 
      // an operation on it.
      CloseHandleToPhysicalDisk ();
      BlockEnd ();
    }



    public  void  Dispose ()
    {
      if  (openedSipperFiles != null)
      {
        // Close all SipperDiskFiles that are still opened.
        while  (openedSipperFiles.Count > 0)
        {
          SipperDiskFile sdf = openedSipperFiles[0];
          sdf.Close ();  //The close method will call 'CloseSipperDiskFile' which will remove from 'openedSipperFiles' list.
          sdf.Dispose ();
          sdf = null;
        }
      }

      BlockStart ();
      
      if  (fs != null)
        CloseHandleToPhysicalDisk  ();
      
      if  (mbr != null)
      {
        mbr.Dispose ();
        mbr = null;
      }

      BlockEnd ();
      
      sipperFiles = null;
    }  /* Dispose */




    public  SipperDiskFile  OpenSipperDiskFile (string  _fileName)
    {
      BlockStart ();
      if  (fs == null)
        ReOpen (false);
    
      SipperFileControlBlock  fcb = sipperFiles.LookUpByName (_fileName);
      if  (fcb == null)
        return null;
        
      SipperDiskFile  sipperDiskFile = new SipperDiskFile (fcb, this);
      
      openedSipperFiles.Add (sipperDiskFile);
      
      BlockEnd ();
      return  sipperDiskFile;
    }  /* OpenSipperDiskFile */
    



    public  SipperDiskFile  OpenSipperDiskFile (int  fcbIndex)
    {
      BlockStart ();
      
      if  (fs == null)
        ReOpen (false);
    
      if  ((fs == null)  ||  (mbr == null)  ||  (sipperFiles == null))
      {
        BlockEnd ();
        return null;
      }
    
      if  (fcbIndex < 0)
      {
        BlockEnd ();
        return  null;
      }

      SipperFileControlBlock  fcb = sipperFiles.LookUpByFcbIndex (fcbIndex);
      if  (fcb == null)
      {
        BlockEnd ();
        return  null;
      }

      SipperDiskFile  sipperDiskFile = new SipperDiskFile (fcb, this);
      openedSipperFiles.Add (sipperDiskFile);

      BlockEnd ();
      
      return  sipperDiskFile;
    }  /* OpenSipperDiskFile */
    




    //**********************************************************************************************************************
    // Will be called by 'SipperDiskFile' 'Close' method to let the controlling 'SipperDisk' objct know the file is close. *
    //**********************************************************************************************************************
    public  void  CloseSipperDiskFile (SipperDiskFile  sdf)
    {
      BlockStart ();
      
      int  x = 0;
      int  indexBeingUsed = -1;
      for  (x = 0;  x < openedSipperFiles.Count;  x++)
      {
        if  (openedSipperFiles[x] == sdf)
        {
          indexBeingUsed = x;
          break;
        }
      }
      
      if  (indexBeingUsed < 0)
      {
        // We are trying to close a SipperDiskFile that we don't know anything about.
        BlockEnd ();
        throw new Exception ("'SipperDisk'::'CloseSipperDiskFile'  Trying to close file that is not supposed to be oppend.");
      }
      
      openedSipperFiles.RemoveAt (indexBeingUsed);
      
      if  (openedSipperFiles.Count == 0)
      {
        // Since no 'SipperDiskFile' objects are open can close the PhysicalDisk handle.
        CloseHandleToPhysicalDisk ();
      }
      
      BlockEnd ();
    } /* CloseSipperDiskFile */





    [DllImport("Kernel32.dll", SetLastError = true)]
    static extern IntPtr CreateFile (
                    string filename,
                    [MarshalAs(UnmanagedType.U4)]FileAccess fileAccess,
                    [MarshalAs(UnmanagedType.U4)]FileShare  fileShare,
                    int                                     securityAttributes,
                    [MarshalAs(UnmanagedType.U4)]FileMode   creationDisposition,
                    int                                     flags,
                    IntPtr                                  template
                   );

    [DllImport("kernel32.dll", SetLastError = true)]
    static extern int CloseHandle (IntPtr hObject);


    [DllImport("Kernel32.dll")]
    static extern  ulong  GetLastError ();





    private  void   CreateHandleToPhysicalDisk (int driveNum)
    {
      if  (fs != null)
      {
        // For some reason still open so we shall close it.
        CloseHandleToPhysicalDisk ();
      }

      deviceName = "\\\\.\\PhysicalDrive" + driveNum.ToString ();

      physicalDriveHandle = CreateFile 
                              (deviceName,
                               FileAccess.Read,
                               FileShare.ReadWrite,
                               0,
                               FileMode.Open,
                               0,
                               IntPtr.Zero //pointed out by Parrys
                              );

      if  (physicalDriveHandle.ToInt32 () < 0)
      {
        ulong  lastError = GetLastError ();
        return;
      }
      
      SafeFileHandle sfh = new SafeFileHandle (physicalDriveHandle, false);
      fs = new FileStream (sfh, FileAccess.Read);
      //fs = new FileStream (ptr, FileAccess.Read, false);

      return;
    }  /* CreateHandleToPhysicalDisk0 */
    







    private void  CloseHandleToPhysicalDisk ()
    {
      if  (fs == null)
      {
        // There is nothing to close.
        return;
      }

      //IntPtr  handle = fs.Handle;
      SafeFileHandle  sfh = fs.SafeFileHandle;

      sfh.Close ();

      fs.Close ();
      fs.Dispose ();
      fs = null;

      int  returnCd = CloseHandle (physicalDriveHandle);
      physicalDriveHandle = IntPtr.Zero;
    }  /* CloseHandleToPhysicalDisk */





    private  void  ReOpen (bool  allowUpdates)
    {
      if  (fs != null)
      {
        // For some reason still open so we shall close it.
        CloseHandleToPhysicalDisk ();
      }
      
      if  (allowUpdates)
      {
        physicalDriveHandle = CreateFile 
                         (deviceName,
                          FileAccess.ReadWrite,
                          FileShare.ReadWrite,
                          0,
                          FileMode.Open,
                          0,
                          IntPtr.Zero //pointed out by Parrys
                         );
      }
      else
      {
        physicalDriveHandle = CreateFile 
                         (deviceName,
                          FileAccess.Read,
                          FileShare.ReadWrite,
                          0,
                          FileMode.Open,
                          0,
                          IntPtr.Zero //pointed out by Parrys
                         );
      }

      if  (physicalDriveHandle.ToInt32 () < 0)
      {
        // Something has gone very wrong.  Since we already know that 'deviceName' is good the 
        // 'CreateFile' call should have worked.
        throw new Exception ("'SipperDisk'   Failure to open drive[" + physicalDriveNumber.ToString () + "]  aka[" + deviceName + "]");
      }
      
      SafeFileHandle  sfh = new SafeFileHandle (physicalDriveHandle, false);

      if  (allowUpdates)
        fs = new FileStream (sfh, FileAccess.ReadWrite);
      else
        fs = new FileStream (sfh, FileAccess.Read);
      
    }  /* ReOpen */
    
    
    

    private  void  OpenSipperDrive ()
    {
      fs  = null;
      mbr = null;
    
      bool    sipperDriveFound = false;
       
      physicalDriveNumber = (int) 0;
      while  ((!sipperDriveFound)  &&  (physicalDriveNumber < 32)) 
      {
        CreateHandleToPhysicalDisk (physicalDriveNumber);
         
        if  (fs != null)
        {
          byte[]  buffer = new byte [MasterBootRecord.BuffSpaceNeeded ()];
           
          fs.Read (buffer, 0, buffer.Length);
           
          int  nextIndex = 0;
          mbr = new MasterBootRecord (buffer, ref nextIndex);
           
          sipperDriveFound = false;
           
          ReadSipperPartitionSector ();   // will read in class global 'sipperPartitionSector'
           
          sipperDriveFound = sipperPartitionSector.IsASipperPartion ();
          if  (!sipperDriveFound)
          {
            CloseHandleToPhysicalDisk ();
            mbr.Dispose ();
            mbr = null;
          }
        }
 
        if  (!sipperDriveFound)
          physicalDriveNumber++;
      }


      if  (!sipperDriveFound)
      {
        if  (fs != null)
        {
          fs.Close ();
          fs = null;
        }
        
        if  (mbr != null)
        {
          mbr.Dispose ();
          mbr = null;
        }
      }

      return;
    }  /* OpenSipperDrive */




    private  void   GetListOfSipperFiles ()
    {
      totalSectorsUsed = Constants.MAX_SIPPER_FILES + 1;  // 1 Sector for each posible file + 1 sector for the SipperPartitionSector
      
      sipperFiles = new SipperFileControlBlockList ();
     
      Int64  sipperDirectorySectorByteOffset = mbr.GetSipperDirectoryByteOffset ();
      fs.Seek (sipperDirectorySectorByteOffset, SeekOrigin.Begin);
       
      for  (int  i = 0;  i < Constants.MAX_SIPPER_FILES;  i++)
      {
        SipperFileControlBlock  fcb = SipperFileControlBlock.CreateFromFileStream (fs);
        if  (fcb == null)
          throw new Exception ("Error reading Sipper FileControlBlock");

        fcb.FcbIndex = i;
        
        if  (fcb.EndSector <= 0)
        {
          if  (fcb.Allocated != SipperFileControlBlock.SIPPER_FILE_FREE)
          {
            sipperFiles.Add (fcb);
            lastSectorUsed = Math.Max (fcb.EndSector, lastSectorUsed);
            totalSectorsUsed += 1 + fcb.EndSector - fcb.StartSector;
          }
        }

        else if  (fcb.EndSector > 0)
        {
          sipperFiles.Add (fcb);
          lastSectorUsed = Math.Max (fcb.EndSector, lastSectorUsed);
          totalSectorsUsed += 1 + fcb.EndSector - fcb.StartSector;
        }
      }

      return;
    }  /* GetListOfSipperFiles */



    private  void  GetListOfSipperFilesFromLogFiles (string  dirName)
    {
      sipperFiles = new SipperFileControlBlockList ();

      DirectoryInfo  di = new DirectoryInfo (dirName);
      FileInfo[] files = di.GetFiles ();
      foreach  (FileInfo fi in files)
      {
        if  (fi.Extension != ".txt")
          continue;

        if  (!(fi.Name.StartsWith ("SipperInterfaceLog_")))
          continue;

        ParseSipperLog  logFileParsed = new  ParseSipperLog (fi.FullName);
        SipperFileControlBlockList filesInLogFile = logFileParsed.FileControlBlocks ();
        foreach  (SipperFileControlBlock  fcb in filesInLogFile)
        {
          bool  fcbMerged = false;
          if  (sipperFiles.Count > 0)
          {
            SipperFileControlBlock  lastBlock = sipperFiles[sipperFiles.Count - 1];
            if  (fcb.FileName == lastBlock.FileName)
            {
              lastBlock.EndSector   = Math.Max (lastBlock.EndSector,   fcb.EndSector);
              lastBlock.StartSector = Math.Min (lastBlock.StartSector, fcb.StartSector);
              fcbMerged = true;
              if  (lastBlock.EndSector >  lastSectorUsed)
                lastSectorUsed = lastBlock.EndSector;
            }
          }

          if  (!fcbMerged)
          {
            fcb.FcbIndex = sipperFiles.Count;
            sipperFiles.Add (fcb);
            if  (fcb.EndSector >  lastSectorUsed)
              lastSectorUsed = fcb.EndSector;
          }

        }
      }

      return;

    }  /* GetListOfSipperFilesFromLogFiles */





    private  void  WriteSipperPartitionSector (SipperPartitionSector  partitionSector)
    {
      Int64  sipperPartitionSectorOffset = mbr.GetPartitionSectorByteOffset (Constants.SIPPER_PARTITION);
      fs.Seek (sipperPartitionSectorOffset, SeekOrigin.Begin);
      partitionSector.Write (fs);
    }  /* WriteSipperPartitionSector */




    private  void  ReadSipperPartitionSector ()
    {
      Int64  sipperPartitionSectorOffset = mbr.GetPartitionSectorByteOffset (Constants.SIPPER_PARTITION);
      fs.Seek (sipperPartitionSectorOffset, SeekOrigin.Begin);
      sipperPartitionSector = SipperPartitionSector.CreateFromFileStream (fs);
      return;
    }  /* ReadSipperPartitionSector */




    byte[]  blockToken = new byte[10];
    bool  blockIsSet = false;
    
    //bool  fsIsBlocked = false;

    public  void  BlockStart ()
    {
      bool  weAreBlocked = true;
      
      while  (weAreBlocked)
      {
        lock  (blockToken)
        {
          if  (blockIsSet)
          {
            weAreBlocked = true;
          }
          else
          {
            blockIsSet  = true;
            weAreBlocked = false;;
          }
        }
        
        if  (weAreBlocked)
        {
          Thread.Sleep (10);
        }
      }
    }  /* BlockStart */
    
    
    
    
    public  void  BlockEnd ()
    {
      lock  (blockToken)
      {
        blockIsSet = false;
      }
    
    } /* BlockEnd */
 
 
  
 
 
    public  void  ResetSipperDirectory ()
    {
      // 1) Open SipperDisk for update
      // 
      // 2) Read in current 'SippePartitonSector'
      //
      // 3) set SipperDiskPartiton.FileDataSector <= PartitionTableEntry.RelativeSector + MAX_SIPPER_FILES  + 1
      //                                             This is the 1st sector after the SIPPER Dierctry Structure.
      //
      // 4) reWrite  'SipperDiskPartiton'
      //
      // 5) sipperDirectorySector <= PartitionTableEntry.RelativeSector * LOGICAL_BLOCK_SIZE + sizeof(SipperPartitionSector);
      //    Disk Set File pointer to sipperDirectorySector
      //
      // 6) Initialize a FileControlBlock (FCB)
      //    a. memset  (&FCB, 0, sizeof (FCB)
      //    b. FCB.Allocatd    <= 0x0  (Indicates Sipper File Free)
      //    c. FCB.Status      <= 0    (Indicates Sipper File is Closed)
      //    d. FCB.FileName    <= "NO FILE"
      //    e. FCB.Description <= "NO DESCRIPTION"
      //
      // 
      // 7) Write out Blank FCB to all Directory Entries
      //    for  (x <= 0; x < MAX_SIPPER_FILES;  x++)
      //     |Write out FCB to Disk
      //    
      // 8) Update fields in SipperPartitionSector to reflect a empty drive.
      //    nextFcbSector  <= PartitionTableEntry.RelativeSector + 1;
      //    nextFileHandle <= 0;
      //    fileDataSector <= PartitionTableEntry.RelativeSector + MAX_SIPPER_FILES + 1
      //
      // 9) RwWrite SipperPartitionSector
      //
      // 10) Close Sipper Disk
      //
      // 
 
      ReOpen (true);                 // Mmaking sure the Siper disk is opened for update.
      ReadSipperPartitionSector ();  // Make sure it is the latest on the drive.
      
      PartitionTableEntry  sipperPartitionTableEntry = mbr.GetSipperPartitionTableEntry ();
      sipperPartitionSector.FileDataSector = sipperPartitionTableEntry.RelativeSector + Constants.MAX_SIPPER_FILES + 1;
      
      WriteSipperPartitionSector (sipperPartitionSector);
      

      Int64  sipperDirectorySectorByteOffset = mbr.GetSipperDirectoryByteOffset ();
      fs.Seek (sipperDirectorySectorByteOffset, SeekOrigin.Begin);
 
      SipperFileControlBlock  fcb = new SipperFileControlBlock ();
      fcb.Allocated = SipperFileControlBlock.SIPPER_FILE_FREE;
      fcb.Status    = SipperFileControlBlock.SIPPER_FILE_CLOSED;
      
      for  (int  fileCount = 0;  fileCount < Constants.MAX_SIPPER_FILES;  fileCount++)
      {
        fcb.FcbIndex  = fileCount;
        fcb.Write (fs);
      }
        

      sipperPartitionSector.NextFcbSector  = sipperPartitionTableEntry.RelativeSector + 1;
      sipperPartitionSector.NextFileHandle = 0;
      sipperPartitionSector.FileDataSector = sipperPartitionTableEntry.RelativeSector + Constants.MAX_SIPPER_FILES + 1;
 
      WriteSipperPartitionSector (sipperPartitionSector);

      CloseHandleToPhysicalDisk (); 
    }  /* ResetSipperDirectory */
 


    //***********************************************************************************
    //*                  Determines if the same disk drive is attached.                 *
    //***********************************************************************************
    public  bool  AreWeTheSameDrive ()
    { 
      if  ((sipperPartitionSector == null)  ||  (origFirstFileControlBlock == null))
        return  false;

      
      //  For now we will always return true;
      return true;


      BlockStart ();

      long  lastFsPosition = 0;

      bool  diskFileWasAlreadyOpened = false;
      if  (fs != null)
      {
        diskFileWasAlreadyOpened = true;
        lastFsPosition = fs.Position;
      }

      if  ((!diskFileWasAlreadyOpened)  &&  (openedSipperFiles != null)  &&  (openedSipperFiles.Count == 0))
      {
        // We will only reopen the Disk handle if no SIPPER files are open.
        try
        {
          ReOpen (false);
        }
        catch  (Exception)
        {
          BlockEnd ();
          return  false;
        }
      }

      bool  weAreTheSameHardDrive = false;
      
      
      Int64  sipperPartitionSectorOffset = mbr.GetPartitionSectorByteOffset (Constants.SIPPER_PARTITION);
      fs.Seek (sipperPartitionSectorOffset, SeekOrigin.Begin);
      SipperPartitionSector  currentSipperPartitionSector = SipperPartitionSector.CreateFromFileStream (fs);
      
      if  (sipperPartitionSector.Equals (currentSipperPartitionSector))
      {
        SipperFileControlBlock  firstFileControlBlock = SipperFileControlBlock.CreateFromFileStream (fs);
        if  (firstFileControlBlock.Equals (origFirstFileControlBlock))
          weAreTheSameHardDrive = true;
      }

      if  (diskFileWasAlreadyOpened)
      {
        fs.Seek (lastFsPosition, SeekOrigin.Begin);
      }
      else
      {
        CloseHandleToPhysicalDisk ();
      }
      
      BlockEnd ();
      return  weAreTheSameHardDrive;
    }

  }  /* SipperDisk */
}
