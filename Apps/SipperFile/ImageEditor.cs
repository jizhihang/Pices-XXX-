using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;

using PicesInterface;


namespace SipperFile
{
  public partial class ImageEditor : Form
  {
    Color           backGroundColor = Color.FromArgb (255, 255, 255);
    Pen[]           colorValues    = null;
    bool            colorize       = false;
    string          fileName       = null;
    int             height         = 0;
    Bitmap          image          = null;
    InstrumentData  instrumentData = null;
    
    float       sizeRatio  = 1.0f;
    byte[][]    raster;
    int         width;

    float       lowerBound = 0.0f;
    float       upperBound = 1.0f;

    uint        thLowerBound = 0;
    uint        thUpperBound = 255;


    String      imageFileName  = "";

    

    private  class  Operation
    {
      PicesRaster.OperationType  operation  = PicesRaster.OperationType.Null;
      PicesRaster.MaskType       mask       = PicesRaster.MaskType.SQUARE3;
      int                        maskSize   = 3;
      float                      lowerBound = 0.0f;
      float                      upperBound = 1.0f;
      uint                       thLowerBound = 0;
      uint                       thUpperBound = 255;

      public  Operation (PicesRaster.OperationType  _op)
      {
        operation = _op;
      }

      public  Operation (PicesRaster.OperationType  _op,
                         PicesRaster.MaskType       _mt
                        )
      {
        operation = _op;
        mask      = _mt;
        maskSize  = PicesRaster.MaskSize (_mt);
      }

      
      public  Operation (PicesRaster.OperationType  _op,
                         PicesRaster.MaskType       _mt,
                         float                      _lowerBound,
                         float                      _upperBound
                        )
      {
        operation  = _op;
        mask       = _mt;
        maskSize   = PicesRaster.MaskSize (_mt);
        lowerBound = _lowerBound;
        upperBound = _upperBound;
      }


      public  Operation (PicesRaster.OperationType  _op,
                         PicesRaster.MaskType       _mt,
                         uint                       _thLowerBound,
                         uint                       _thUpperBound
                        )
      {
        operation  = _op;
        mask       = _mt;
        maskSize   = PicesRaster.MaskSize (_mt);
        thLowerBound = _thLowerBound;
        thUpperBound = _thUpperBound;
      }


      public  override  String  ToString ()
      {
        String  s = PicesRaster.OperationTypeToStr (operation);

        switch  (operation)
        {
          case  PicesRaster.OperationType.Closing:  
          case  PicesRaster.OperationType.Dialation:
          case  PicesRaster.OperationType.Erosion:
          case  PicesRaster.OperationType.Opening:
            {
              s =  s.PadRight (12);
              s += PicesRaster.MaskTypeToStr (mask);
            }
            break;

          case  PicesRaster.OperationType.SmoothAveraging:
          case  PicesRaster.OperationType.SmoothMedium:
            {
              s = s.PadRight (12);
              s += PicesRaster.MaskSize (mask);
            }
            break;

          case  PicesRaster.OperationType.BandPass:
            {
              s = s.PadRight (12);
              s += ((float)(lowerBound * 100.0)).ToString ("##0.0") + " - " + 
                   ((float)(upperBound * 100.0)).ToString ("##0.0");
            }
            break;


          case  PicesRaster.OperationType.BinarizeTH:
            {
              s = s.PadRight (12);
              s += thLowerBound.ToString ("##0.0") + " - " + thUpperBound.ToString ("##0.0");
            }
            break;
        }

        return  s;
      }  /* ToString */



      public  PicesRaster  PerformOperation (PicesRaster  r)
      {
        PicesRaster  result = null;
        switch  (operation)
        {
          case  PicesRaster.OperationType.BandPass:           result = r.BandPass            (lowerBound,   upperBound); break;
          case  PicesRaster.OperationType.Binarize:           result = r.Binarize            ();           break;
          case  PicesRaster.OperationType.BinarizeTH:         result = r.BinarizeByThreshold (thLowerBound, thUpperBound); break;
          case  PicesRaster.OperationType.Closing:            result = r.CloseImage          (mask);              break;
          case  PicesRaster.OperationType.ConnectedComponent: result = r.ConnectedComponent  ((byte)maskSize);    break;
          case  PicesRaster.OperationType.ConvexHull:         result = r.ConvexHull          ();                  break;
          case  PicesRaster.OperationType.Dialation:          result = r.DialateImage        (mask);              break;
          case  PicesRaster.OperationType.Edge:               result = r.EdgeImage           ();                  break;
          case  PicesRaster.OperationType.Erosion:            result = r.ErodeImage          (mask);              break;
          case  PicesRaster.OperationType.FillHoles:          result = r.FillHoles           ();                  break;
          case  PicesRaster.OperationType.FourierTransform:   result = r.FourierTransform    ();                  break;
          case  PicesRaster.OperationType.Opening:            result = r.OpenImage           (mask);              break;
          case  PicesRaster.OperationType.SmoothAveraging:    result = r.SmoothAveraging     (maskSize);          break;
          case  PicesRaster.OperationType.SmoothMedium:       result = r.SmoothMedium        (maskSize);          break;
          case  PicesRaster.OperationType.Thinning:           result = r.Thinning            ();                  break;

        }  /* operation */

        return  result;
      }  /* PerformOperation */
    }


    List<Operation>  operations = new List<Operation> ();




    public  ImageEditor (String _fileName)
    {
      InitializeComponent();

      imageFileName = _fileName;

      PicesRaster  pr = null;
      try
      {
        pr = new PicesRaster (_fileName);
      }
      catch (Exception e)
      {
        pr = null;
        throw new Exception ("ImageEditor", e);
     }

      raster         = pr.BuildRasterArray2 ();
      fileName       = _fileName;
      instrumentData = null;
      height         = pr.Height;
      width          = pr.Width;

      pr = null;

      BuildColorValues ();

      SizeOptions.SelectedOption = "100%";

      image = BuildBitmapFromRasterData (raster);
      
      pictureBox1.Height = image.Height;
      pictureBox1.Width  = image.Width;
      pictureBox1.Image  = image;

      ImagePanel.AutoScroll = true;
      //ImagePanel.SetStyle   (ControlStyles.ResizeRedraw, false);
      ImagePanel.AutoScroll = true;
      ImagePanel.AutoScrollMinSize = image.Size;
      ImagePanel.Invalidate();

      MakePredictions ();
    }
  



    public  ImageEditor (PicesRaster  _raster)
    {
      InitializeComponent();

      //raster         = (byte[][])_raster.BuildRasterArray ();
      raster         = _raster.BuildRasterArray2 ();
      imageFileName  = _raster.FileName;

      instrumentData = null;
      height         = _raster.Height;
      width          = _raster.Width;

      _raster = null;

      BuildColorValues ();

      SizeOptions.SelectedOption = "100%";

      image = BuildBitmapFromRasterData (raster);
      
      pictureBox1.Height = image.Height;
      pictureBox1.Width  = image.Width;
      pictureBox1.Image  = image;

      ImagePanel.AutoScroll = true;
      //ImagePanel.SetStyle   (ControlStyles.ResizeRedraw, false);
      ImagePanel.AutoScroll = true;
      ImagePanel.AutoScrollMinSize = image.Size;
      ImagePanel.Invalidate();

      MakePredictions ();
    }
  



    public ImageEditor (string          _fileName,
                        int             _height,
                        int             _width,
                        byte[][]        _raster,   // Will take ownership of '_raster'
                        InstrumentData  _instrumentData
                       )
    {
      InitializeComponent();
      
      fileName       = _fileName;
      instrumentData = _instrumentData;
      raster         = _raster;
      height         = _height;
      width          = _width;

      imageFileName = _fileName;;

      BuildColorValues ();

      SizeOptions.SelectedOption = "100%";

      image = BuildBitmapFromRasterData (raster);
      
      pictureBox1.Height = image.Height;
      pictureBox1.Width  = image.Width;
      pictureBox1.Image  = image;

      ImagePanel.AutoScroll = true;
      //ImagePanel.SetStyle   (ControlStyles.ResizeRedraw, false);
      ImagePanel.AutoScroll = true;
      ImagePanel.AutoScrollMinSize = image.Size;
      ImagePanel.Invalidate();

      MakePredictions ();
    }



    private  void  AddOperation (PicesRaster.OperationType  _operation)
    {
      PicesRaster.MaskType  mt = PicesRaster.MaskTypeFromStr (MaskTypesComboBox.SelectedItem.ToString ());

      if  (_operation == PicesRaster.OperationType.BandPass)
        operations.Add (new Operation (_operation, mt, lowerBound, upperBound));

      else if  (_operation == PicesRaster.OperationType.BinarizeTH)
        operations.Add (new Operation (_operation, mt, thLowerBound, thUpperBound));

      else
        operations.Add (new Operation (_operation, mt));
      UpdateOperationsList ();
      RePaintImage ();
    }  /* AddOperation */



    private  void  UndoOperation ()
    {
      if  (operations.Count < 1)
        return;
      
      Operation  lastOperation = operations[operations.Count - 1];
      operations.RemoveAt (operations.Count - 1);
      lastOperation = null;

      UpdateOperationsList ();

      RePaintImage ();
    }  /* UndoOperation */



    private  void  UpdateOperationsList ()
    {
      OperationsList.Items.Clear ();
      foreach  (Operation op in operations)
      {
        OperationsList.Items.Add (op);
      }
    }  /* UpdateOperationsList */




    private  byte[][]  PerformAllOperations (byte[][]  startRaster)
    {
      PicesRaster  lastRaster = new PicesRaster (startRaster);

      foreach  (Operation o in  operations)
      {
        PicesRaster  nextRaster = o.PerformOperation (lastRaster);
        if  (nextRaster == null)
        {
          MessageBox.Show ("Morphilogical operation[" + o.ToString () + "] failed.");
          break;
        }
        lastRaster = nextRaster;
      }

      byte[][]  finalResult = null;

      try
      {
        finalResult = lastRaster.BuildRasterArray2 ();
      }
      catch  (Exception)
      {
        // Lets try one more time.
        finalResult = lastRaster.BuildRasterArray2 ();
        MessageBox.Show ("PerformAllOperations     Called  'BuildRasterArray2'  Two Times.");
      }

      lastRaster = null;

      return  finalResult;
    }  /* PerformAllOperations */





    private  void  MakePredictions  ()
    {
      pred1ClassName.Text = "";
      pred2ClassName.Text = "";

      BlobDetector  bd = new BlobDetector (raster, 20);
      BlobList  blobs = bd.ExtractBlobs ();

      if  ((blobs == null)  ||  (blobs.Count < 1))
        return;

      Blob  largestBlob = blobs.LocateLargestBlob ();
      if  (largestBlob == null)
        return;

      byte[][]  largestRaster = bd.ExtractedBlobImage (largestBlob);

      PicesRunLog  runLog = new PicesRunLog ();
      PicesRaster  picesRaster = new PicesRaster (largestRaster);

      //PicesPredictionList  trainLibrary1Predictions = null;
      //PicesPredictionList  trainLibrary2Predictions = null;

      //ActiveTrainingLibraries.MakePredictions (picesRaster,
      //                                         instrumentData,
      //                                         imageFileName, 
      //                                         ref trainLibrary1Predictions, 
      //                                         ref trainLibrary2Predictions, 
      //                                         runLog
      //                                        );

      picesRaster.FileName = imageFileName;

      PicesPrediction  model1Prediction1 = new PicesPrediction (null, 0, 0.0f);
      PicesPrediction  model1Prediction2 = new PicesPrediction (null, 0, 0.0f);
      PicesPrediction  model2Prediction1 = new PicesPrediction (null, 0, 0.0f);
      PicesPrediction  model2Prediction2 = new PicesPrediction (null, 0, 0.0f);
      
      ActiveTrainingLibraries.MakePredictions (picesRaster, 
                                               ref model1Prediction1, 
                                               ref model1Prediction2, 
                                               ref model2Prediction1, 
                                               ref model2Prediction2, 
                                               runLog
                                              );

      if  (model1Prediction1 != null)
      {
        TrainLib1Name.Text = ActiveTrainingLibraries.Model1Name;
        pred1Prob.Text      = model1Prediction1.Probability.ToString ("p");
        pred1ClassName.Text = model1Prediction1.ClassName;
        this.Text           = model1Prediction1.ClassName;

        if  (model1Prediction2 != null)
        {
          pred2Prob.Text      = model1Prediction2.Probability.ToString ("p");
          pred2ClassName.Text = model1Prediction2.ClassName;
        }
      }

      if  (model2Prediction1 != null)
      {
        TrainLib2Name.Text = ActiveTrainingLibraries.Model2Name;
        lib2Pred1Prob.Text      = model2Prediction1.Probability.ToString ("p");
        lib2Pred1ClassName.Text = model2Prediction1.ClassName;
        if  (model2Prediction2 != null)
        {
          lib2Pred2Prob.Text      = model2Prediction2.Probability.ToString ("p");
          lib2Pred2ClassName.Text = model2Prediction2.ClassName;
        }
      }

      picesRaster = null;

      blobs.Dispose ();
      largestBlob.Dispose ();
      bd.Dispose ();
      bd = null;
    }  /* MakePredictions */  




    private void  ImageEditor_Load (object sender, EventArgs e)
    {
      MaskTypesComboBox.Items.Add (PicesRaster.MaskTypeToStr (PicesRaster.MaskType.CROSS3));
      MaskTypesComboBox.Items.Add (PicesRaster.MaskTypeToStr (PicesRaster.MaskType.CROSS5));
      MaskTypesComboBox.Items.Add (PicesRaster.MaskTypeToStr (PicesRaster.MaskType.SQUARE3));
      MaskTypesComboBox.Items.Add (PicesRaster.MaskTypeToStr (PicesRaster.MaskType.SQUARE5));
      MaskTypesComboBox.Items.Add (PicesRaster.MaskTypeToStr (PicesRaster.MaskType.SQUARE7));
      MaskTypesComboBox.Items.Add (PicesRaster.MaskTypeToStr (PicesRaster.MaskType.SQUARE9));
      MaskTypesComboBox.Items.Add (PicesRaster.MaskTypeToStr (PicesRaster.MaskType.SQUARE11));

      MaskTypesComboBox.SelectedIndex = 2;
    }


    private void BuildColorValues ()
    {
      if  (colorize)
        BuildColorizedPenValues ();
      else
        BuildGrayScaleValues ();
    }



    private void  BuildGrayScaleValues ()
    {
      int x;
      colorValues = new Pen[256];
      for (x = 0; x < 256; x++)
      {
        int y = 255 - x;
        colorValues[x] = new Pen (Color.FromArgb(y, y, y));
      }

      backGroundColor = Color.FromArgb (255, 255, 255);
    }  /* BuildColorValues */


    private  void  BuildColorizedPenValues ()
    {
      float  h;
      float  s = 220.0f / 255.0f;
      float  i = 144.0f / 255.0f;

      float  r, g, b;
      int    rInt, gInt, bInt;

      float  H;

      float  TwoPI        = (float)Math.PI * 2.0f;
      float  TwoThirdsPI  = (float)Math.PI * 2.0f / 3.0f;
      float  OneThirdPI   = (float)Math.PI / 3.0f;
      float  FourThirdsPI = (float)Math.PI * 4.0f / 3.0f;

      int  hInt;
      for  (hInt = 0;  hInt < 256;  hInt++)
      {
        h = (float)(255 - hInt) / 255.0f;

        //H = TwoPI * h;
        //H = TwoThirdsPI * h;
        H = (float)TwoPI * h;
        if  (H <= 0.0f)
          H = H + TwoPI;
      
        if  ((s < 0.00001) || (i < 0.00001))
        {
          r = g = b = i;          
        }   

        else if  ((H > 0.0f)  &&  (H <= TwoThirdsPI))
        {
          r = (float)(i * (1 + ((s * Math.Cos (H)) / Math.Cos (OneThirdPI - H))));
          b = i * (1 - s);
          g = 3 * i * (1 - (r + b)/(3 * i));
        }

        else if  ((H > TwoThirdsPI)  &&  (H <= (FourThirdsPI)))
        {
          H = H - TwoThirdsPI;
          
          g = (float)(i * (1 + ((s * Math.Cos (H)) / Math.Cos (OneThirdPI - H))));
          r = i * (1 - s);
          b = 3 * i * (1 - ((r + g) / (3 * i)));
        }

        else if  ((H > FourThirdsPI) && (H <= TwoPI))
        {
          H = H - FourThirdsPI;
          b = (float)(i * (1 + ((s * Math.Cos (H)) / Math.Cos (OneThirdPI - H))));
          g = i * (1 - s);
          r = 3 * i * (1 - ((g + b) / (3 * i)));
        }

        else
        {
          r = g = b = 0;
        }

        rInt = (int)(255.0f * r + 0.5f);
        if  (rInt > 255)
          rInt = 255;

        gInt = (int)(255.0f * g + 0.5f);
        if  (gInt > 255)
          gInt = 255;

        bInt = (int)(255.0f * b + 0.5f);
        if  (bInt > 255)
          bInt = 255;


        colorValues[hInt] = new Pen (Color.FromArgb (255, rInt, gInt, bInt));
      }

      colorValues[0] = new Pen (Color.FromArgb (255, 255, 255, 255));
    }  /* BuildColorValues */



    private void  SaveButton_Click (object sender, EventArgs e)
    {
      SaveFileDialog  saveDialog = new SaveFileDialog ();
      saveDialog.InitialDirectory = "";
      saveDialog.Filter = "(Bitmaps)|*.bmp;";
      saveDialog.FilterIndex = 0;
      saveDialog.RestoreDirectory = true;
      saveDialog.FileName = fileName;
      if  (saveDialog.ShowDialog() == DialogResult.OK)
      {
        fileName = saveDialog.FileName;
        image.Save (saveDialog.FileName, ImageFormat.Bmp);
      }
    }  /* SaveButton_Click */





    public  Bitmap  BuildBitmapFromRasterData (byte[][]  r)
    {
      int  rowFirst = 99999;
      int  rowLast  = -1;
      int  colFirst = 99999;
      int  colLast  = -1;

      Bitmap  image = new Bitmap (width, height);
    
    
      // GDI+ return format is BGR, NOT RGB. 
      BitmapData bmData = image.LockBits (new Rectangle(0, 0, width, height),
                                          ImageLockMode.ReadWrite,
                                          PixelFormat.Format24bppRgb
                                         );
      int stride = bmData.Stride;
      System.IntPtr Scan0 = bmData.Scan0;

      unsafe
      {
        int  nOffset = stride - width * 3;
        int  bytesPerRow = width * 3 + nOffset;
        int  col = 0;
        int  row = 0;
        byte pixelValue = 0;
        byte lastPixelValue = 0;
        Pen  curPen = colorValues[pixelValue];

        byte  red   = curPen.Color.R;
        byte  green = curPen.Color.G;
        byte  blue  = curPen.Color.B;


        byte* ptr = (byte*)(void*)Scan0;

        for  (row = 0;  row < height;  row++)
        {
          byte[]  datRow = r[row];
          
          for (col = 0;  col < width;  col++)
          {
            pixelValue = datRow[col];
            if  (pixelValue != lastPixelValue)
            {
              lastPixelValue = pixelValue;
              curPen = colorValues[lastPixelValue];
              red   = curPen.Color.R;
              green = curPen.Color.G;
              blue  = curPen.Color.B;
            }

            ptr[0] = blue;  ptr++;
            ptr[0] = green; ptr++;
            ptr[0] = red;   ptr++;

            if  (pixelValue < 255)
            {
              // We are looking at a forground pixel.
              if  (row < rowFirst )
                rowFirst = row;
              rowLast = row;

              if  (col < colFirst)
                colFirst = col;

              if  (col > colLast)
                colLast = col;
            }
          }
          
          ptr += nOffset;
        }
      }  /* Unsafe */

      image.UnlockBits (bmData);


      {
        // We are now gping to add some scale lines
        int scaleColRight = Math.Min (colLast + 5, width - 1);
        int scaleColLeft  = Math.Max (0,           colFirst - 5);

        int  scaleRowBot = Math.Min (rowLast + 5, height - 1);
        int  scaleRowTop = Math.Max (0          , rowFirst - 5);

        Pen  p = new Pen (Color.Red);
        Graphics  dc = Graphics.FromImage (image);
        dc.DrawLine (p, scaleColLeft,  rowFirst, scaleColLeft,  rowLast);
        dc.DrawLine (p, scaleColRight, rowFirst, scaleColRight, rowLast);

        dc.DrawLine (p, colFirst, scaleRowTop, colLast,  scaleRowTop);
        dc.DrawLine (p, colFirst, scaleRowBot, colLast,  scaleRowBot);

        int  x = rowFirst;
        while  (x < rowLast)
        {
          dc.DrawLine (p, scaleColLeft  + 4, x, scaleColLeft,  x);
          if  (x > rowFirst)
            dc.DrawLine (p, scaleColRight - 4, x, scaleColRight, x);
          x += 40;
        }
        //dc.DrawLine (p, scaleColRight - 5, rowLast, scaleColRight, rowLast);
 
        x = colFirst;
        while  (x < colLast)
        {
          dc.DrawLine (p, x,  scaleRowTop + 4, x, scaleRowTop);
          if  (x > colFirst)
            dc.DrawLine (p, x,  scaleRowBot - 4, x, scaleRowBot);
          x += 40;
        }
        //dc.DrawLine (p, colLast, scaleRowBot, colLast - 5, scaleRowBot);
      }
      
      return  image;
    }  /* BuildBitmapFromRasterData */



    private  void  ResizeWindow ()
    {
      int  availRows = Height - (ImagePanel.Top + 40);
      int  availCols = Width  - (ImagePanel.Left + 20);
      
      ImagePanel.Width  = availCols;
      ImagePanel.Height = availRows;

      int lastRowWeCanUse = ImagePanel.Bottom;
      int newOperationsListHeight = lastRowWeCanUse - OperationsList.Top;
      OperationsList.Height = newOperationsListHeight;

      Invalidate ();
    }  /* ResizeWindow */




    private  void  RePaintImage ()
    {
      if  ((sizeRatio <= 0.0f)  ||  (image == null))
        return;

      byte[][] processedRaster = PerformAllOperations (raster);


      Bitmap newImage = BuildBitmapFromRasterData (processedRaster);
      
      int  newWidth  = (int)((float)width  * sizeRatio + 0.5f);
      int  newHeight = (int)((float)height * sizeRatio + 0.5f);

      image = new Bitmap (newWidth, newHeight);
      Graphics imageDC = Graphics.FromImage (image);
      imageDC.Clear (backGroundColor);
      imageDC.ScaleTransform (sizeRatio, sizeRatio);
      imageDC.DrawImageUnscaled (newImage, 0, 0);
      
      newImage.Dispose ();
      newImage = null;
      
      pictureBox1.Height = newHeight;
      pictureBox1.Width = newWidth;
      pictureBox1.Image = image;

      ImagePanel.AutoScroll        = true;
      ImagePanel.AutoScrollMinSize = image.Size;
      ImagePanel.Invalidate ();
    }  /* RePaintImage */




    

    private void ImageEditor_SizeChanged(object sender, EventArgs e)
    {
      ResizeWindow ();
    }



    private void SizeOptions_SelectedOptionChanged ()
    {
      string  selOptStr = SizeOptions.SelectedOption;
      if  (selOptStr.EndsWith ("%"))
      {
        selOptStr = selOptStr.Substring (0, selOptStr.Length - 1);
        sizeRatio = PicesKKStr.StrToFloat (selOptStr) / 100.0f;
        RePaintImage ();
      }
    }



    private void ClassifyButton_Click (object sender, EventArgs e)
    {
    }




    private void Colorize_CheckedChanged(object sender, EventArgs e)
    {
      if  (Colorize.Checked == colorize)
        return;

      colorize = Colorize.Checked;
      BuildColorValues ();
      RePaintImage ();
    }


    private void ClossingButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.Closing);
    }

    private void OpeningButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.Opening);
    }

    private void ErosionButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.Erosion);
    }

    private void DialationButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.Dialation);
    }

    private void SmoothingButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.SmoothAveraging);
    }

    private void MediumButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.SmoothMedium);
    }

    private void EdgeButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.Edge);
    }

    private void FillHolesButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.FillHoles);
    }

    private void ThinningButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.Thinning);
    }

    private void UndoButton_Click(object sender, EventArgs e)
    {
      UndoOperation ();
    }

    private void ConnectedComponentButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.ConnectedComponent);
    }

    private void BinarizeButton_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.Binarize);
    }

    private void ConvexHullButon_Click(object sender, EventArgs e)
    {
      AddOperation (PicesRaster.OperationType.ConvexHull);
    }



    private void BandPassButton_Click(object sender, EventArgs e)
    {
      lowerBound = PicesKKStr.StrToFloat (LowerBound.Text) / 100.0f;
      upperBound = PicesKKStr.StrToFloat (UpperBound.Text) / 100.0f;

      if  (lowerBound < 0.0f)
        lowerBound = 0.0f;
      else if  (lowerBound > 1.0f)
        lowerBound = 1.0f;

      if  (upperBound < 0.0f)
        upperBound = 0.0f;
      else if  (upperBound > 1.0f)
        upperBound = 1.0f;

      if  (lowerBound > upperBound)
        lowerBound = upperBound;

      LowerBound.Text = ((float)(lowerBound * 100.0f)).ToString ("##0");
      UpperBound.Text = ((float)(upperBound * 100.0f)).ToString ("##0");

      AddOperation (PicesRaster.OperationType.BandPass);
    }



    private void BinarizeTHButton_Click(object sender, EventArgs e)
    {
      thLowerBound = (uint)PicesKKStr.StrToInt (ThLowerBound.Text);
      thUpperBound = (uint)PicesKKStr.StrToInt (ThUpperBound.Text);

      thLowerBound = Math.Max (0,   thLowerBound);
      thUpperBound = Math.Min (255, thUpperBound);
      
      if  (thLowerBound > thUpperBound)
        thLowerBound = thUpperBound - 1;

      ThLowerBound.Text = thLowerBound.ToString ("##0");
      ThUpperBound.Text = thUpperBound.ToString ("##0");

      AddOperation (PicesRaster.OperationType.BinarizeTH);
    }
  }
}