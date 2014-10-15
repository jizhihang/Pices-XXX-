using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using  PicesInterface;

namespace PicesCommander
{
  public partial class SaveImagesToDisk : Form
  {
    PicesDataBase           dbConn         = null;

    PicesDataBaseImageList  images         = null;
    String                  cruise         = "";
    String                  station        = "";
    String                  deployment     = "";
    String                  sipperFileName = "";

    int     increments            = 1;
    bool    saveByDepthIncrements = true;
    bool    saveByClass           = false;

    PicesRunLog  runLog = null;

    bool    errorsFound = false;

    // 
    Thread  savingThread          = null;
    bool    savingThreadRunning   = false;
    bool    savingThreadCompleted = false;
    bool    cancelRequested       = false;
    int     imagesSaved           = 0;
    int     imagesFailedToExtract = 0;
    String  curDir                = "";

    
    public SaveImagesToDisk (PicesDataBase           _dbConn,
                             String                  _cruise,
                             String                  _station,
                             String                  _deployment,
                             String                  _sipperFileName,
                             PicesDataBaseImageList  _images
                            )
    {
      dbConn         = _dbConn;
      images         = _images;
      cruise         = _cruise;
      station        = _station;
      deployment     = _deployment;
      sipperFileName = _sipperFileName;

      runLog = new PicesRunLog ();
      InitializeComponent ();
      DestinationDirectory.Text =  OSservices.AddSlash  (OSservices.AddSlash (PicesSipperVariables.PicesHomeDir ()) + "SavedImages");
      OSservices.CreateDirectoryPath (DestinationDirectory.Text);
       
      //  PicesTempDirectory ());
      if  (!String.IsNullOrEmpty (cruise))
      {
        DestinationDirectory.Text += cruise;
        if  (!String.IsNullOrEmpty (station))
        {
          DestinationDirectory.Text += "-" + station;
          if  (!String.IsNullOrEmpty (deployment))
          {
            DestinationDirectory.Text += "-" + deployment;
            if  (!String.IsNullOrEmpty (sipperFileName))
              DestinationDirectory.Text += "-" + sipperFileName;
          }
        }
      }

      ImageCount.Text = images.Count.ToString ("###,##0");
    }



    private  void  ValidateDestinationDirectory ()
    {
      errorProvider1.SetError (DestinationDirectory, "");

      String  parentDir = OSservices.GetParentDirectoryOfDirPath (DestinationDirectory.Text);
      DirectoryInfo di = new DirectoryInfo (parentDir);
      if  (!di.Exists)
      {
        errorsFound = true;
        errorProvider1.SetError (DestinationDirectory, "No such directory[" + parentDir + "].");
      }
    }



    private  void  ValidateIncrements ()
    {
      errorProvider1.SetError (Increments, "");

      if  (saveByClass)
        return;

      char[]  delimeters = {' ', '\n', '\r'};
      String[]  fields = Increments.Text.Split  (delimeters);
      if  (fields.Length < 1)
      {
        errorsFound = true;
        errorProvider1.SetError (Increments, "No Increment specified.");
      }

      int  temp = PicesKKStr.StrToInt (fields[0]);
      if  ((temp < 1)  ||  (temp > 100))
      {
        errorsFound = true;
        errorProvider1.SetError (Increments, "Valid Increment range is (1 - 100).");

      }
      increments  = temp;
    }


    private  void  ValidateAll ()
    {
      errorsFound = false;
      ValidateDestinationDirectory ();
      ValidateIncrements ();
    }



    private  void  StartSavingImagesThread ()
    {
      OSservices.CreateDirectoryPath (DestinationDirectory.Text);
      savingThread = new Thread (new ThreadStart (SaveImages));
      savingThread.Start ();
      Thread.Sleep (10);
      savingTimer.Enabled = true;
    }




    private  void  SaveImages ()
    {
      savingThreadRunning = true;
      imagesSaved           = 0;
      imagesFailedToExtract = 0;

      if  (saveByDepthIncrements)
        SaveImagesByDepthImcrements ();
      else
        SaveImagesByClass ();

      savingThreadCompleted = true;
      savingThreadRunning   = false;
    }  /* SaveImages */




    private  int  DepthIndex (double  depth)
    {
      double  incrementdDbl = (double)increments;

      int  depthIndex = (int)Math.Floor (depth / incrementdDbl);
      if  (depthIndex < 0)
        depthIndex = 0;

      return  depthIndex;
    }



    private  void  SaveImagesByDepthImcrements ()
    {
      images.Sort (PicesDataBaseImageList.SortOrderType.soDepth, false);

      int  idx = 0;
      PicesDataBaseImage i = images[0];

      while  ((idx < images.Count)  &&  (!cancelRequested))
      {
        int  depthIndex = DepthIndex (i.Depth);
        int  curDepthIndex = depthIndex;

        int  fromDepth = depthIndex * increments;
        int  toDepth   = (depthIndex + 1) * increments;

        String  depthSubDirName = fromDepth.ToString ("000") + "-" + toDepth.ToString ("000");
        curDir = OSservices.AddSlash (DestinationDirectory.Text) + depthSubDirName;

        try {Directory.CreateDirectory (curDir);}  catch (Exception)  {}

        PicesDataBaseImageList  imagesThisDepthIndex = new PicesDataBaseImageList ();

        while  ((idx < images.Count)  &&  (!cancelRequested)  &&  (depthIndex == curDepthIndex))
        {
          imagesThisDepthIndex.Add (i);
          idx++;
          if  (idx < images.Count)
          {
            i = images[idx];
            depthIndex = DepthIndex (i.Depth);
          }
        }

        SaveImagesInList (imagesThisDepthIndex, curDir);
      }
    }  /* SaveImagesByDepthImcrements */



    private  void  SaveImagesByClass ()
    {
      images.Sort (PicesDataBaseImageList.SortOrderType.soClassName, false);

      int  idx = 0;
      PicesDataBaseImage i = images[0];

      while  ((idx < images.Count)  &&  (!cancelRequested))
      {
        String  className     = i.Class1Name;
        String  curClassName  = className;

        curDir = OSservices.AddSlash (DestinationDirectory.Text) + className;

        try {Directory.CreateDirectory (curDir);}  catch (Exception)  {}

        PicesDataBaseImageList  imagesThisClass = new PicesDataBaseImageList ();

        while  ((idx < images.Count)  &&  (!cancelRequested)  &&  (className == curClassName))
        {
          imagesThisClass.Add (i);
          idx++;
          if  (idx < images.Count)
          {
            i = images[idx];
            className = i.Class1Name;
          }
        }

        SaveImagesInList (imagesThisClass, curDir);
      }
    }  /* SaveImagesByClass */




    private  void  SaveImagesInList (PicesDataBaseImageList  list,
                                     String                  dirName
                                    )
    {
      dirName = OSservices.AddSlash (dirName);
      curDir = dirName;
      foreach (PicesDataBaseImage i in list)
      {
        if  (cancelRequested)
          break;

        String fullName = dirName + i.ImageFileName + ".bmp";
        PicesRaster r = dbConn.ImageFullSizeFind (i.ImageFileName);
        if  (r == null)
          imagesFailedToExtract++;
        else
          r.Save (fullName);
        imagesSaved++;
      }
    }  /* SaveImagesInList */



    private  void  CancelImageSaveThread ()
    {
      cancelRequested = true;

      // Will wait up till 4 seconds for savingprocess to pause.
      int x = 0;
      while  ((savingThreadRunning) && (x < 40))
      {
        Thread.Sleep (100);
        x++;
      }

      if  (savingThreadRunning)
      {
        savingThread.Abort ();
      }
    }  /* CancelImageSaveThread */



    private void ByDepthIncrements_CheckedChanged(object sender, EventArgs e)
    {
      saveByDepthIncrements = ByDepthIncrements.Checked;
      if  (ByDepthIncrements.Checked)
      {
        if  (ByClass.Checked)
          ByClass.Checked = false;
        Increments.Enabled = true;
      }
      else
      {
        if  (!ByClass.Checked)
          ByClass.Checked = true;
        Increments.Enabled = false;
      }
    }



    private void ByClass_CheckedChanged(object sender, EventArgs e)
    {
      saveByClass = ByClass.Checked;
      if  (ByClass.Checked)
      {
        if  (ByDepthIncrements.Checked)
          ByDepthIncrements.Checked = false;
      }
      else
      {
        if  (!ByDepthIncrements.Checked)
          ByDepthIncrements.Checked = true;
      }
    }



    private void BrowseDestinationDir_Click (object sender, EventArgs e)
    {
      FolderBrowserDialog  fbd = new FolderBrowserDialog ();
      fbd.SelectedPath = DestinationDirectory.Text;

      DialogResult dr = fbd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        DestinationDirectory.Text = fbd.SelectedPath;
        ValidateDestinationDirectory ();
      }
    }


    private void StartButton_Click(object sender, EventArgs e)
    {
      ValidateAll ();
      if  (errorsFound)
        return;
      DialogResult  dr = MessageBox.Show (this, "Start Saving Images ?", "Saving Images", MessageBoxButtons.YesNo);

      if  (dr == DialogResult.Yes)
        StartSavingImagesThread ();
    }


    private void savingTimer_Tick(object sender, EventArgs e)
    {
      ImagesSaved.Text           = imagesSaved.ToString           ("##,###,##0");
      ImagesFailedToExtract.Text = imagesFailedToExtract.ToString ("##,###,##0");
      CurrentDirectory.Text = curDir;
      if  (savingThreadCompleted)
      {
        savingTimer.Enabled = false;
        MessageBox.Show (this, "Done Saving Images");
        Close ();
      }
    }



    private void CancelSaveButton_Click(object sender, EventArgs e)
    {
      if  (!savingThreadRunning)
      {
        this.Close ();
      }

      DialogResult  dr = MessageBox.Show (this, "Cancel Saving of Images.", "Cancel", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        savingTimer.Enabled = false;
        CancelImageSaveThread ();
        this.Close ();
      }
    }
  }
}
