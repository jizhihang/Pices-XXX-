using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;
using System.IO;

using PicesInterface;
using SipperFile;


namespace SipperFile
{
  public partial class PredictionBreakDownDisplay : Form
  {
    String               fileName        = null;
    PicesFeatureVector   featureVector   = null;
    PicesFeatureVector   dbFeatureVector = null;
    PicesInstrumentData  instrumentData  = null;
    int                  lastHeight      = 0;
    int                  lastWidth       = 0;
    Font                 probFont        = new Font (FontFamily.GenericMonospace, 10, FontStyle.Regular); 
    PicesRunLog          runLog          = null;
    
    TrainingModel2       trainingModel = null;

    PicesRasterList      featureCalcImages  = null;
    PicesClass           selectedTopClass   = null;


    public PredictionBreakDownDisplay (PicesDataBase       _dbConn,
                                       String              _fileName,
                                       TrainingModel2      _trainingModel,
                                       PicesInstrumentData _instrumentData,  /*!<  Instrument data that goes with image,  if null will read from dtabase. */
                                       PicesRunLog         _runLog
                                      )
    {
      if  (_runLog == null)
        runLog = new PicesRunLog ();
      else
        runLog = _runLog;

      fileName       = _fileName;
      instrumentData = _instrumentData;
      trainingModel  = _trainingModel;

      PicesRaster  raster = _dbConn.ImageFullSizeFind (fileName);
      if  (raster == null)
        return;

      if  (_dbConn != null)
      {
        dbFeatureVector = _dbConn.FeatureDataRecLoad (_fileName);
        if  (instrumentData == null)
        {
          PicesDataBaseImage  image = _dbConn.ImageLoad (fileName);
          if  (image != null)
            instrumentData = _dbConn.InstrumentDataGetByScanLine (image.SipperFileName, image.TopLeftRow);
        }
      }

      featureCalcImages = new PicesRasterList ();
      featureVector = new PicesFeatureVector (raster, fileName, featureCalcImages, runLog);
     
      if  (instrumentData != null)
        featureVector.AddInstrumentData (instrumentData);

      InitializeComponent ();
    }



    public PredictionBreakDownDisplay (PicesDataBase       _dbConn,
                                       PicesDataBaseImage  _image,
                                       PicesRaster         _raster,
                                       TrainingModel2      _trainingModel,
                                       PicesInstrumentData _instrumentData,  /*!<  Instrument data that goes with image,  if null will read from dtabase. */
                                       PicesRunLog         _runLog
                                      )
    {
      if  (_runLog == null)
        runLog = new PicesRunLog ();
      else
        runLog = _runLog;
        
      if  (_image == null)
        return;

      fileName       = _image.ImageFileName;
      instrumentData = _instrumentData;
      trainingModel  = _trainingModel;

      PicesRaster raster = _raster;
      if  (_dbConn != null)
      {
        if  (raster == null)
         raster = _dbConn.ImageFullSizeFind (fileName);

        if  (instrumentData == null)
          instrumentData = _dbConn.InstrumentDataGetByScanLine (_image.SipperFileName, _image.TopLeftRow);

        dbFeatureVector = _dbConn.FeatureDataRecLoad (fileName);
      }

      if  (raster == null)
        return;

      //raster = raster.ConnectedComponent ();

      featureCalcImages = new PicesRasterList ();
      featureVector = new PicesFeatureVector (raster, fileName, featureCalcImages, runLog);
      if  (instrumentData != null)
        featureVector.AddInstrumentData (instrumentData);

      InitializeComponent ();
    }



    private void ImageListDisplay_Load (object sender, EventArgs e)
    {
      lastHeight = Height;
      lastWidth  = Width;

      UpdateDetailProbabilityDisplay ();
    }  /* LoadImageGrid */



    private  void  UpdateDetailProbabilityDisplay ()
    {
      if  (trainingModel == null)
        return;
    
      PicesPredictionList  guesses = trainingModel.PredictProbabilities (featureVector);
      if  (guesses == null)
        return;
           
      ClassificationProbabilities.Rows.Clear ();

      foreach  (PicesPrediction p in guesses)
      {
        Object[]  data = new Object[3];

        data[0] = p.ClassName;
        data[1] = p.Probability;
        data[2] = p.Votes;
        ClassificationProbabilities.Rows.Add (data);
      }

      FeatureCalcImages.Rows.Clear ();
      if  (featureCalcImages == null)
        return;

      for (int x = 0;  x < featureCalcImages.Count;  x++)
      {
        Object[]  data = new Object[2];

        data[0] = featureCalcImages[x].BuildBitmap ();
        data[1] = featureCalcImages[x].FileName;
        FeatureCalcImages.Rows.Add (data);
      }
    }  /* UpdateDetailProbabilityDisplay */



    private  void  UpdateBinaryClassifierProbabilities (String  className)
    {
      PicesClass  leftClass = PicesClassList.GetUniqueClass (className, "");
      BinaryClassName.Text = "";
      
      BinaryProbabilities.Rows.Clear ();
      
      PicesPredictionList  binaryPredictions = trainingModel.BinaryProbailitiesForClass (leftClass);
      if  (binaryPredictions == null)
        return;

      BinaryClassName.Text = leftClass.Name;

      selectedTopClass = leftClass;
      
      foreach  (PicesPrediction p in binaryPredictions)
      {
        if  (leftClass == p.MLClass)
          continue;

        Object[]  data = new Object[2];

        data[0] = p.ClassName;
        double  probForRightSide = 1.0 - p.Probability;
        data[1] = probForRightSide;
        BinaryProbabilities.Rows.Add (data);
      }

      BuildBinaryProbabilitiesGridViewConetextMenu ();
    }  /* UpdateBinaryClassifierProbabilities */



    private  void  BuildBinaryProbabilitiesGridViewConetextMenu ()
    {
      ContextMenuStrip cms = new ContextMenuStrip ();

      cms.Items.Add ("Display Training Library Images",  null, DisplayTrainingLibraryImagesBottomClass);
      cms.Items.Add ("Display Worst Training Images",    null, DisplayWorstTrainingImages);
      cms.Items.Add ("Display Worst Training Images2",   null, DisplayWorstTrainingImages2);
      foreach (DataGridViewColumn dgvc in BinaryProbabilities.Columns)
        dgvc.ContextMenuStrip = cms;
    }  /* BuildThumbnailGridViewConetextMenu */



    private  PicesClass  GetSelectedBottomClass ()
    {
      if  (BinaryProbabilities.SelectedCells.Count != 1)
        return null;

      int  row = BinaryProbabilities.SelectedCells[0].RowIndex;
      String  className = BinaryProbabilities.Rows[row].Cells[0].Value.ToString ();
      return  PicesClassList.GetUniqueClass (className, "");
    }  /* GetSelectedBottomClass */





    private void  DisplayTrainingImages (PicesClass  mlClass)
    {
      if  (trainingModel== null)
        return;
      
      String  dir = trainingModel.DirectoryPathForClass (mlClass);
      if  (String.IsNullOrEmpty (dir))
        return;

      DisplayPicesImages  dpi = new DisplayPicesImages (dir);
      dpi.ShowDialog ();
      dpi.Dispose ();
      dpi = null;
    }  /* DisplayTrainingImages */




    public  bool  ThumbnailCallback()
    {
      return false;
    }


    private void ImageListDisplay_Resize(object sender, EventArgs e)
    {
      if  (lastHeight == 0)
        return;

      int  changeInWidth  = Width  - lastWidth;
      int  changeInHeight = Height - lastHeight;
      lastWidth  = Width;
      lastHeight = Height;

      FeatureCalcImages.Height += changeInHeight;

      int  changeInHeight1stHalf = changeInHeight / 2;
      int  changeInHeight2ndHalf = changeInHeight - changeInHeight1stHalf; 

      ClassificationProbabilities.Height  += changeInHeight1stHalf;
      BinaryClassName.Top                 += changeInHeight1stHalf;
      BinaryProbabilities.Top             += changeInHeight1stHalf;
      BinaryProbabilities.Height          += changeInHeight2ndHalf;
      BinaryClassVsClassProbablities.Top  += changeInHeight;
      DisplayFeatureDataButton.Top        += changeInHeight;
    }






    private void ClassificationProbabilities_RowEnter (object sender, DataGridViewCellEventArgs e)
    {
      DataGridView  dgv = (DataGridView)sender;

      String  className = (String)dgv.Rows[e.RowIndex].Cells[0].Value;

      UpdateBinaryClassifierProbabilities (className);
    }



    private void FeatureCalcImages_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
    {
      DataGridView  dgv = (DataGridView)sender;

      if  (e.Value == null)
        return;

      if  (dgv.Columns[e.ColumnIndex].Name == "FeatureCalcedImage")
      {
        Image  i = null;

        //if  (dgv.Rows[e.RowIndex].Cells[0].ValueType == typeof (String))
        
        i = (Image)e.Value;
        int  biggestDim = Math.Max (i.Height, i.Width);
        if  (biggestDim > 200)
        {
          float  reductionFactor = 200.0f / (float)biggestDim;

          int  newHeight = (int)(0.5f + (float)i.Height * reductionFactor);
          int  newWidth  = (int)(0.5f + (float)i.Width  * reductionFactor);

          Image.GetThumbnailImageAbort myCallback = new Image.GetThumbnailImageAbort (ThumbnailCallback);
          i = i.GetThumbnailImage (newWidth, newHeight, myCallback, IntPtr.Zero);

          dgv.Rows[e.RowIndex].Cells[0].Value = i;
          e.Value = i;
          e.FormattingApplied = true;
        }

        int  displayHeight = Math.Max (i.Height, 50);
        displayHeight = Math.Min (200, displayHeight);
        dgv.Rows[e.RowIndex].Height = displayHeight;
      }

      e.CellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
    }



    private void ClassificationProbabilities_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
    {
      DataGridView  dgv = (DataGridView)sender;

      if  (e.Value == null)
        return;

      if  (dgv.Columns[e.ColumnIndex].Name == "PredProbability")
      {
        e.CellStyle.Font      = probFont;
        e.CellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        e.Value = ((double)e.Value).ToString ("P2").PadLeft (8);
        e.FormattingApplied = true;
      }

      else if  (dgv.Columns[e.ColumnIndex].Name == "PredVotes")
      {
        e.CellStyle.Font      = probFont;
        e.CellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        e.Value = ((int)e.Value).ToString ("#,##0");
        e.FormattingApplied = true;
      }
    }



    private void BinaryProbabilities_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
    {
      DataGridView  dgv = (DataGridView)sender;

      if  (e.Value == null)
        return;

      if  (dgv.Columns[e.ColumnIndex].Name == "CounterProbability")
      {
        e.CellStyle.Font      = probFont;
        e.CellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        e.Value               = ((double)(e.Value)).ToString ("P2");
        e.FormattingApplied   = true;
      }
    }



    private void BinaryProbabilities_RowEnter (object sender, DataGridViewCellEventArgs e)
    {
      DataGridView  dgv = (DataGridView)sender;
      int  rowIndex = e.RowIndex;

      String  rightName = (String)dgv.Rows[rowIndex].Cells[0].Value;
      double  rightProb = (double)dgv.Rows[rowIndex].Cells[1].Value;
      String  leftName  = BinaryClassName.Text;
      double  leftProb  = 1.0 - rightProb;

      BinaryClassVsClassProbablities.Text = leftName   + "(" + leftProb.ToString  ("P2") + ")" + "  vs  " +
                                            rightName  + "(" + rightProb.ToString ("P2") + ")";
    }


    private void ClassificationProbabilities_MouseDoubleClick (object sender, MouseEventArgs e)
    {
      if  (e.Button == MouseButtons.Right)
      {
        DataGridView  dgv = (DataGridView)sender;

        DataGridViewSelectedCellCollection dgvscc = dgv.SelectedCells;
        if  ((dgvscc == null)  ||  (dgvscc.Count != 1))
          return;

        String  className = (String)dgvscc[0].Value;
        PicesClass  pc = PicesClassList.GetUniqueClass (className, "");
        if  (pc != null)
          DisplayTrainingImages (pc);
      }
    }




    private void BinaryProbabilities_MouseDoubleClick (object sender, MouseEventArgs e)
    {
      if  (e.Button == MouseButtons.Right)
      {
        DataGridView  dgv = (DataGridView)sender;

        DataGridViewSelectedCellCollection dgvscc = dgv.SelectedCells;
        if  ((dgvscc == null)  ||  (dgvscc.Count != 1))
          return;

        String  className = (String)dgvscc[0].Value;
        PicesClass  pc = PicesClassList.GetUniqueClass (className, "");
        if  (pc != null)
          DisplayTrainingImages (pc);
      }
    }



    private  void  DisplayTrainingLibraryImagesBottomClass (Object sender, EventArgs e)
    {
      PicesClass  bottomClass = GetSelectedBottomClass ();
      if  ((bottomClass == null)  ||  (trainingModel== null))
        return;

      String  dir = trainingModel.DirectoryPathForClass (bottomClass);
      if  (String.IsNullOrEmpty (dir))
        return;

      DisplayPicesImages  dpi = new DisplayPicesImages (dir);
      String[]  svNames = trainingModel.SupportVectorNames (selectedTopClass, bottomClass);
      if  (svNames != null)
        dpi.SummportVectorNames (svNames);

      dpi.ShowDialog ();
      dpi.Dispose ();
      dpi = null;
    }  /* DisplayTrainingLibraryImagesBottomClass */




    private  void  DisplayWorstTrainingImages (Object sender, EventArgs e)
    {
      PicesClass  bottomClass = GetSelectedBottomClass ();
      if  (bottomClass == null)
        return;

      DisplayWorstTrainingImages (bottomClass);
    }  /* DisplayWorstTrainingImages */


    private  void  DisplayWorstTrainingImages (PicesClass  bottomClass)
    {
      if  (trainingModel== null)
        return;

      String  dir1 = trainingModel.DirectoryPathForClass (selectedTopClass);
      String  dir2 = trainingModel.DirectoryPathForClass (bottomClass);

      if  (String.IsNullOrEmpty (dir1)  &&  String.IsNullOrEmpty (dir2))
        return;
      
      ProbNamePair[]  worstTrainingImages = trainingModel.FindWorstSupportVectors (featureVector, 500, selectedTopClass, bottomClass);

      DisplayPicesImages  dpi = new DisplayPicesImages (dir1, dir2, worstTrainingImages);
      dpi.ShowDialog ();
      dpi.Dispose ();
      dpi = null;
    }  /* DisplayWorstTrainingImages */


    private void DisplayWorstTrainingImages2(Object sender, EventArgs e)
    {
      PicesClass bottomClass = GetSelectedBottomClass();
      if (bottomClass == null)
        return;

      DisplayWorstTrainingImages2(bottomClass);
    }  /* DisplayWorstTrainingImages */


    private void DisplayWorstTrainingImages2(PicesClass bottomClass)
    {
      if (trainingModel == null)
        return;

      String dir1 = trainingModel.DirectoryPathForClass(selectedTopClass);
      String dir2 = trainingModel.DirectoryPathForClass(bottomClass);

      if (String.IsNullOrEmpty(dir1) && String.IsNullOrEmpty(dir2))
        return;

      ProbNamePair[] worstTrainingImages = trainingModel.FindWorstSupportVectors2 (featureVector, 500, selectedTopClass, bottomClass);

      DisplayPicesImages dpi = new DisplayPicesImages(dir1, dir2, worstTrainingImages);
      dpi.ShowDialog();
      dpi.Dispose();
      dpi = null;
    }


    private void DisplayFeatureDataButton_Click (object sender, EventArgs e)
    {
      FeatureDataDisplay  fdd = new FeatureDataDisplay (fileName, this.featureVector, dbFeatureVector);
      fdd.ShowDialog (this);
    }  /* DisplayWorstTrainingImages2 */
  }
}
