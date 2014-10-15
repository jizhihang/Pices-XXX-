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


namespace GradeTrainingModel
{
  public partial class ImageListDisplay : Form
  {
    Dictionary<String, Prediction>  predictions    = null;
    String                          rootDir        = null;
    TrainingModel2                  trainingModel  = null;

    int   lastHeight = 0;
    int   lastWidth  = 0;

    Font   probFont = new Font (FontFamily.GenericMonospace, 10, FontStyle.Regular); 


    String  curSelImageFileName = "";

    public ImageListDisplay (String                          _rootDir,
                             Dictionary<String, Prediction>  _predictions,
                             TrainingModel2                  _trainingModel 
                            )
    {
      predictions    = _predictions;
      rootDir        = _rootDir;
      trainingModel  = _trainingModel;
      
      InitializeComponent ();
    }


    public  void  LoadImageGrid ()
    {
      if  (predictions.Count < 1)
        return;

      lastHeight = Height;
      lastWidth  = Width;

      ImageGrid.Rows.Clear ();

      bool  firstPass = true;

      foreach  (KeyValuePair<String, Prediction>  pair in predictions)
      {
        Prediction p = pair.Value;
        if  (firstPass)
        {
          KnownClass.Text     = p.knowClass.Name;
          PredictedClass.Text = p.predClass.Name;
        }

        Object[]  data = new Object[3];

        String  fullFileName = null;
        {
          String fn = p.FileName;
          if  ((fn.Length > 2)  &&  (fn[1] == ':'))
            fullFileName = fn;
          else
            fullFileName = OSservices.AddSlash (rootDir) + p.FileName;
        }

        //Bitmap  i = new Bitmap (fileName);

        //int  heightToDisplay = Math.Min (300, i.Height + 4);
        //heightToDisplay = Math.Max (50, heightToDisplay);
        
        data[0] = fullFileName;
        data[1] = p.prob;
        data[2] = p.FileName;

        ImageGrid.Rows.Add (data);
      }
    }  /* LoadImageGrid */
    

    private  Prediction  GetPrediction (String  imageFileName)
    {
      imageFileName = OSservices.GetRootName (imageFileName);
      Prediction p = null;
      try  {p = predictions[imageFileName];}  catch (Exception) {p = null;}
      return p;
    }  /* GetPrediction */



    private void ImageListDisplay_Load(object sender, EventArgs e)
    {
      LoadImageGrid ();
    }  /* LoadImageGrid */



    private  void  UpdateDetailProbabilityDisplay (String  fileName)
    {
      if  (fileName == null)
        return;

      curSelImageFileName = fileName;

      String  fullFileName = null;
      if  ((fileName.Length > 1)  &&  (fileName[1] == ':'))
        fullFileName = fileName;
      else
        fullFileName = OSservices.AddSlash (rootDir) + fileName;

      if  (!File.Exists (fullFileName))
      {
        String  fn = OSservices.GetRootName (fileName) + ".bmp";
        fullFileName = OSservices.LookForFile (fn, rootDir);
      }

      Prediction  prediction = GetPrediction (fileName); 
   
      // We need to call 'PredictProbabilities'  twice.  Once toget the intermediate images and the
      // second time to perform prediction using the FeatureVector that was used in teh original
      // prediction.  It has to be done this way becuase the version of 'PredictProbabilities' that
      // derives teh FeatureVector from an image does not always have access of instrumentation data.

      PicesRasterList featureCalcImages = new PicesRasterList();
      PicesPredictionList guesses2 = prediction.trainModel.PredictProbabilities(fullFileName, featureCalcImages);

      // Perform Prediction using original FeatureVector.
      PicesPredictionList  guesses = prediction.trainModel.PredictProbabilities (prediction.fv);

      ImageFileName.Text = fileName;

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
      if  ((featureCalcImages == null) || (featureCalcImages.Count < 1))
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

      Prediction  pred = GetPrediction (curSelImageFileName);
        
      PicesPredictionList  binaryPredictions = pred.trainModel.BinaryProbailitiesForClass (leftClass);
      if  (binaryPredictions == null)
        return;

      BinaryClassName.Text = leftClass.Name;
      
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
    }  /* UpdateBinaryClassifierProbabilities */





    public  bool  ThumbnailCallback()
    {
      return false;
    }



    private  String  ValueToPercentStr (Object o)
    {
      if  (o.GetType () == typeof (float))
        return  ((float)(o)).ToString ("P2").PadLeft (8);

      else if  (o.GetType () == typeof (double))
        return  ((double)(o)).ToString ("P2").PadLeft (8);
        
      return  o.ToString ();
    }


    
    private  Image  FindBitmap (String fileName)
    {
      String  rootName = OSservices.GetRootName (fileName);
      String  fullFileName = OSservices.LookForFile (rootName + ".bmp", rootDir);

      if  (String.IsNullOrEmpty (fullFileName))
        return null;
      else
        return new Bitmap (fullFileName);
    }



    private void ImageGrid_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
    {
      DataGridView  dgv = (DataGridView)sender;

      if  (e.Value == null)
        return;


      if  (dgv.Columns[e.ColumnIndex].Name == "ThumbNail")
      {
        Image  i = null;

        //if  (dgv.Rows[e.RowIndex].Cells[0].ValueType == typeof (String))
        
        if  (e.Value.GetType () == typeof (String))
        {
          String  fileName = dgv.Rows[e.RowIndex].Cells[0].Value.ToString ();
          i = new Bitmap (fileName);
          if  (i == null)
            i = FindBitmap (fileName);

          int  biggestDim = Math.Max (i.Height, i.Width);
          if  (biggestDim > 300)
          {
            float  reductionFactor = 300.0f / (float)biggestDim;

            int  newHeight = (int)(0.5f + (float)i.Height * reductionFactor);
            int  newWidth  = (int)(0.5f + (float)i.Width  * reductionFactor);

            Image.GetThumbnailImageAbort myCallback = new Image.GetThumbnailImageAbort (ThumbnailCallback);
            i = i.GetThumbnailImage (newWidth, newHeight, myCallback, IntPtr.Zero);
          }
          dgv.Rows[e.RowIndex].Cells[0].Value = i;
          e.Value = i;
          e.FormattingApplied = true;
        }
        else
        {
          i = (Image)e.Value;
        }

        int  displayHeight = Math.Max (i.Height, 30);
        displayHeight = Math.Min (300, displayHeight);
        dgv.Rows[e.RowIndex].Height = displayHeight;
      }
      
      
      else if  (dgv.Columns[e.ColumnIndex].Name == "Probability")
      {
        e.CellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        e.Value = ValueToPercentStr (e.Value);
        e.FormattingApplied = true;
      }
    }


    private void ImageListDisplay_Resize(object sender, EventArgs e)
    {
      int  changeInWidth  = Width  - lastWidth;
      int  changeInHeight = Height - lastHeight;
      lastWidth  = Width;
      lastHeight = Height;

      ImageGrid.Height         += changeInHeight;
      FeatureCalcImages.Height += changeInHeight;

      int  changeInHeight1stHalf = changeInHeight / 2;
      int  changeInHeight2ndHalf = changeInHeight - changeInHeight1stHalf; 

      ClassificationProbabilities.Height += changeInHeight1stHalf;
      BinaryClassName.Top                += changeInHeight1stHalf;
      BinaryProbabilities.Top            += changeInHeight1stHalf;
      BinaryProbabilities.Height         += changeInHeight2ndHalf;
    }



    private void ImageGrid_RowEnter(object sender, DataGridViewCellEventArgs e)
    {
      DataGridView  dgv = (DataGridView)sender;

      String  fileName = (String)dgv.Rows[e.RowIndex].Cells[2].Value;

      UpdateDetailProbabilityDisplay (fileName);
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
        e.Value = ValueToPercentStr (e.Value);
        e.FormattingApplied = true;
      }
        
      else if  (dgv.Columns[e.ColumnIndex].Name == "PredVotes")
      {
        e.CellStyle.Font      = probFont;
        e.CellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        //e.Value = e.ToString ();
        //e.FormattingApplied = true;
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
        e.Value = ValueToPercentStr  (e.Value);
        e.FormattingApplied   = true;
      }
    }



    private void ImageGrid_CellClick (object sender, DataGridViewCellEventArgs e)
    {
    }



    private void ImageGrid_CellContentDoubleClick(object sender, DataGridViewCellEventArgs e)
    {
      String  fileName = OSservices.AddSlash (rootDir) + (String)ImageGrid.Rows[e.RowIndex].Cells[2].Value;
      ImageEditor ie = new ImageEditor (fileName);
      ie.ShowDialog ();
      ie.Dispose ();
      ie = null;
    }  
  }
}
