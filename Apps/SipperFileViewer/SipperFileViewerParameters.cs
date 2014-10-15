using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using SipperFile;
using System.Drawing.Imaging;

using  PicesInterface;

namespace SipperFileViewer
{

  public  class  SipperFileViewerParameters 
  {
    private  Panel       panel    = null;
    private  Graphics    panelDC  = null;
    private  Bitmap      buffer   = null;
    private  Graphics    bufferDC = null;
    private  Color       backGroundColor;
                      
    private  int         width = 0;
    private  int         height = 0;
                      
    private  int         displayRowsToDisplay = 4096;
                      
    private  long        displayRowTop = 0;
    private  long        displayRowBot = 4096;
                      
    private  int         numPixelsOnEachRow = 0;  // will be width or smaller that allows a proper mutiple of ratio
    
    private  byte[]      displayRow = null;
    private  byte[][]    raster     = null;
    
    private  int         cropColumnLeft  = 0;
    private  int         cropColumnRight = 4095;

    private  PicesDataBase       dbConn       = null;
    private  SipperFileBuffered  sipperFile   = null;
    private  PicesSipperFile     dbSipperFile = null;

    private  Pen[]               colorValues = null;   // Pen color to use for each pixel value;  normaly 
                                                       // each value is set to the appropriate grayscale value.
                                                       // except when (colorize == true) selectd then Colors
                                                       // are used for the 7 levels of grayscale that sipper
                                                       // produce.

    private  BlobList            blobs = null;
    private  bool                extractBlobs = false;
    private  int                 blobMinSize  = 500;
    
    private  bool                colorize = false;

    private  ContextMenu         sipperStreamPanelContextMenu = null;

    int  rightMouseRow = 0;
    int  rightMouseCol = 0;

    
    public   int   BlobMinSize          ()  {return  blobMinSize;}
    public   int   CropColemnLeft       ()  {return  cropColumnLeft;}
    public   int   CropColemnRight      ()  {return  cropColumnRight;}
    public   int   DisplayRowsToDisplay ()  {return  displayRowsToDisplay;}
    public   bool  ExtractBlobs         ()  {return  extractBlobs;}
    public   long  DisplayRowBot        ()  {return  displayRowBot;}
    public   long  DisplayRowTop        ()  {return  displayRowTop;}
    
    public   void  BlobMinSize  (int   _blobMinSize)   {blobMinSize  = _blobMinSize;}
    public   void  ExtractBlobs (bool  _extractBlobs)  {extractBlobs = _extractBlobs;}



    public   float Ratio ()  
    {
      if  (sipperFile == null)
        return 1.0f;
      else
        return sipperFile.Ratio ();
    }



    public  SipperFileViewerParameters (Panel  _panel)
    {
      panel = _panel;
      panel.MouseClick += new MouseEventHandler (MonitorMouseClick);
      panelDC = panel.CreateGraphics ();
      BuildPenValues ();
      ComputeParameters ();
    }



    public  void  Dispose ()
    {
      if  (buffer != null)
      {
        buffer.Dispose ();
        buffer = null;
        bufferDC.Dispose ();
        bufferDC = null;
      }
      
      raster      = null;
      displayRow  = null;
      colorValues = null;
    }  /* Dispose */



    private  void  BuildPenValues ()
    {
      if  (colorize)
      {
        BuildColorizedPenValues ();
        return;
      }

      int x;
      colorValues = new Pen[256];
      for (x = 0; x < 256; x++)
      {
        int y = 255 - x;
        colorValues[x] = new Pen(Color.FromArgb(y, y, y));
      }

      backGroundColor = Color.FromArgb (255, 255, 255);
    }  /* BuildPenValues */




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
    }



    public void   ComputeParameters ()
    {
      int  x = 0;

      double  oldRatio = 1.0;
      if  (numPixelsOnEachRow != 0)
        oldRatio = (double)Sipper3File.PixelsPerScanLine / (double)numPixelsOnEachRow;
      if  (sipperFile != null)
        oldRatio = sipperFile.Ratio ();
      long  actualRowTop = (long)((double)displayRowTop * oldRatio);
   
      width   = panel.Width;
      height  = panel.Height;

      numPixelsOnEachRow = width;
  
      float  ratio = Sipper3File.PixelsPerScanLine / numPixelsOnEachRow;    
      if  (sipperFile != null)
      {
        sipperFile.SetPixelsPerDisplayLine (numPixelsOnEachRow, cropColumnLeft, cropColumnRight);
        ratio = sipperFile.Ratio ();
      }
       
      if  (buffer != null)
        buffer.Dispose();

      panelDC = panel.CreateGraphics ();
      buffer = new Bitmap (panel.Width, panel.Height);
      bufferDC = Graphics.FromImage (buffer);
      
      displayRowsToDisplay = height;
      displayRowBot = displayRowTop + displayRowsToDisplay - 1;
      
      int  sipperPixelsThatCanFitOnARow = (int)Math.Ceiling ((float)numPixelsOnEachRow * ratio);
      displayRow = new byte[numPixelsOnEachRow];      
      
      raster = new byte[height][];
      for  (x = 0;  x < height;  x++)
        raster[x] = new byte[width];

      SetUpSipperStreamPanelContectMenu ();

      long  newDisplayRowTop = (long)((double)actualRowTop / (double)ratio);
      SetNewDisplayRowTop (newDisplayRowTop);
    }  /* ComputeParameters */
  
  
  
  
    public  void  SetNewDisplayRowTop (long _displayRowTop)
    {
      displayRowTop = _displayRowTop;
      displayRowBot = displayRowTop + displayRowsToDisplay - 1;
    }  /* SetNewDisplayRowTop */



    
    public  bool  Colorize
    {
      get  {return  colorize;}
      set  
      {
        colorize = value;
        BuildPenValues ();
      }
    }  /* Colorize */

    


    public  void  SetSipperFile (SipperFileBuffered  _sipperFile,
                                 PicesSipperFile     _dbSipperFile,
                                 PicesDataBase       _dbConn
                                )
    {
      sipperFile    = _sipperFile;
      dbSipperFile  = _dbSipperFile;
      dbConn        = _dbConn;
      displayRowTop = 0;

      if  (sipperFile == null)
      {
        cropColumnLeft  = 0;
        cropColumnRight = 4095;
      }
      else
      {
        cropColumnLeft  = sipperFile.CropColLeft ();
        cropColumnRight = sipperFile.CropColRight ();
      }

      ComputeParameters ();
    }  /* SetSipperFile */



    public  void  SetDataBase (PicesDataBase  _dbConn)
    {
      dbConn = _dbConn;
    }


  
    private  void  ComputeARowToDisplay (long    displayRowNum,
                                         byte[]  displayRow
                                        )
    {
      if  (sipperFile == null)
      {
        int  col = 0;
        for  (col = 0;  col < displayRow.Length;  col++)
          displayRow[col] = 0;
        return;
      }
      
      byte[][]  displayRows = new byte[1][];
      displayRows[0] = new byte[numPixelsOnEachRow];
      
      sipperFile.GetDisplayRows (displayRowNum, displayRowNum, displayRows);
      byte[]    firstDisplayRow = displayRows[0];
      
      int  lineLen = Math.Min (displayRow.Length, firstDisplayRow.Length);
      
      for  (int x = 0;  x < lineLen;  x++)
        displayRow[x] = (byte)firstDisplayRow[x];
    }  /* ComputeARowToDisplay */
  
  
  
  
    private  void  PaintARow (int       screenRow,
                              Graphics  dc
                             )
    {
      ComputeARowToDisplay (screenRow + displayRowTop, displayRow);
      
      int  col = 0;
      
      byte  oldColor = 0;
      byte  curColor = 0;
      int   startCol = 0;
      
      while  (col < (displayRow.Length - 1))
      {
        oldColor = displayRow[col];
        curColor = oldColor;
        startCol =  col;
        
        while  ((oldColor == curColor)  &&  (col < (displayRow.Length - 1)))
        {
          col++;
          curColor = displayRow[col];
        }
        
        int  endCol = col - 1;

        if  (oldColor != 0)
        {
          //Brush b = new SolidBrush (colorValues[oldColor].Color);
          //dc.FillRectangle (b, startCol, screenRow, 1 + (endCol - startCol), 1);
          dc.DrawLine (colorValues[oldColor], startCol, screenRow, endCol + 1, screenRow);
        }
      }
    }  /* PaintARow */



    
    public static bool  PaintARow2 (Bitmap  b,
                                    int     row,
                                    byte[]  rowData
                                   )
    {
      // GDI+ return format is BGR, NOT RGB. 
      BitmapData bmData = b.LockBits (new Rectangle(0, 0, b.Width, b.Height),
                                      ImageLockMode.ReadWrite,
                                      PixelFormat.Format24bppRgb
                                     );

      int stride = bmData.Stride;
      System.IntPtr Scan0 = bmData.Scan0;

      unsafe
      {
        int nOffset = stride - b.Width * 3;
        int bytesPerRow = b.Width * 3 + nOffset;
        int col = 0;
        byte  pixelValue;

        byte* ptr = (byte*)(void*)Scan0 + bytesPerRow * row;

        for (col = 0;  col < rowData.Length;  col++)
        {
          pixelValue = (byte)((byte)255 - rowData[col]);
          ptr[0] = pixelValue; ptr++;
          ptr[0] = pixelValue; ptr++;
          ptr[0] = pixelValue; ptr++;
        }
      }  /* Unsafe */

      b.UnlockBits(bmData);

      return true;
    }  /* PaintARow2 */
    




    public void  PaintWholePanel ()
    {
      //panelDC.Clear (backGroundColor);
      bufferDC.Clear(backGroundColor);
      
      if  (sipperFile != null)
      {
        try
        {
          sipperFile.GetDisplayRows (displayRowTop, displayRowBot, raster);
        }
        catch  (Exception  e)
        {
          MessageBox.Show (e.ToString (), "PaintWholePanel   Error calling 'GetDisplayRows'");
        }
      }
      
      // GDI+ return format is BGR, NOT RGB. 
      BitmapData bmData = buffer.LockBits (new Rectangle(0, 0, width, height),
                                           ImageLockMode.ReadWrite,
                                           PixelFormat.Format24bppRgb
                                          );

      int stride = bmData.Stride;
      System.IntPtr Scan0 = bmData.Scan0;

      unsafe
      {
        int  nOffset = stride - width * 3;
        int  bytesPerRow = width * 3 + nOffset;

        byte pixelValue    = 0;
        byte oldPixelValue = 0;

        byte  red   = 255;
        byte  green = 255;
        byte  blue  = 255;
        
        int  row, col;

        byte* ptr = (byte*)(void*)Scan0;

        for  (row = 0;  row < height;  row++)
        {
          byte[]  rowData = raster[row];
          
          for  (col = 0;  col < rowData.Length;  col++)
          {
            //pixelValue = (byte)((byte)255 - rowData[col]);
            //ptr[0] = pixelValue; ptr++;
            //ptr[0] = pixelValue; ptr++;
            //ptr[0] = pixelValue; ptr++;

            pixelValue = (byte)rowData[col];
            if  (oldPixelValue != pixelValue)
            {
              oldPixelValue = pixelValue;
              red   = colorValues[pixelValue].Color.R;
              green = colorValues[pixelValue].Color.G;
              blue  = colorValues[pixelValue].Color.B;
            }

            ptr[0] = blue;   ptr++;
            ptr[0] = green;  ptr++;
            ptr[0] = red;    ptr++;
          }
          
          ptr += nOffset;
        }
      }  /* Unsafe */

      buffer.UnlockBits (bmData);

      panelDC.DrawImageUnscaled (buffer, 0, 0);

      if  (extractBlobs)
        ExtractBlobsFromCurrentScreen ();
      else
        ClearBobs ();
    }  /* PaintWholePanel*/




    public  void  ClearBobs ()
    {
      if  (blobs != null)
      {
        blobs.Dispose ();
        blobs = null;
      }
    }  /* ClearBobs */




    private  void  ExtractBlobsFromCurrentScreen ()
    {
      if  (blobs != null)
      {
        blobs.Dispose ();
        blobs = null;
      }

      if  (raster == null)
        return;

      BlobDetector bd = new BlobDetector (raster, (int)((float)blobMinSize / Math.Ceiling (sipperFile.Ratio () * sipperFile.Ratio ())));
      blobs = bd.ExtractBlobs ();
      bd.Dispose ();

      //Lets Paint Boxes for each Blob
      foreach (Blob b in blobs)  
      {
        PaintRectangle (panelDC,  Color.Red, b);
        PaintRectangle (bufferDC, Color.Red, b);
      }
    }  /* ExtractBlobsFromCurrentScreen */




    public static bool  ScrollUp (Bitmap  b, 
                                  int     numRows
                                 )
    {
      // GDI+ return format is BGR, NOT RGB. 
      BitmapData bmData = b.LockBits (new Rectangle (0, 0, b.Width, b.Height),
                                      ImageLockMode.ReadWrite,
                                      PixelFormat.Format24bppRgb
                                     );

      int stride = bmData.Stride;
      System.IntPtr  Scan0 = bmData.Scan0;

      unsafe
      {
        int  nOffset     = stride - b.Width * 3;
        int  bytesPerRow = b.Width * 3 + nOffset;
      
        byte*  destPtr = (byte*)(void*)Scan0;
        byte*  srcPtr =  ((byte*)(void*)Scan0) + bytesPerRow * numRows;

        int nWidth = b.Width * 3;

        for (int y = 0; y < (b.Height - numRows); ++y)
        {
          for (int x = 0; x < nWidth; ++x)
          {
            destPtr[0] = srcPtr[0];
            ++srcPtr;
            ++destPtr;
          }
          
          srcPtr  += nOffset;
          destPtr += nOffset;
        }
      }  /* Unsafe */

      b.UnlockBits (bmData);
      
      return true;
    }  /* ScrollUp */





    public  void  ScrollRows2 (int  numRows)
    {
      int  row = 0;
      
      if  (numRows < 0)
      {
        // Scroll down
        Bitmap    newBuffer   = new Bitmap (buffer.Width, buffer.Height);
        Graphics  newBufferDC = Graphics.FromImage (newBuffer);
        newBufferDC.Clear (backGroundColor);
        newBufferDC.DrawImageUnscaled (buffer, 0, -numRows);
        
        SetNewDisplayRowTop (displayRowTop + numRows);
        
        for  (row = 0;  row < (-numRows);  row++)
          PaintARow (row, newBufferDC);

        buffer.Dispose ();
        bufferDC.Dispose ();
      
        buffer   = newBuffer;
        bufferDC = newBufferDC;
      }
      
      else
      {
        // Scroll Up
        ScrollUp (buffer, numRows);
        bufferDC.FillRectangle (new SolidBrush (backGroundColor),
                                new Rectangle (0, (height - numRows), buffer.Width, numRows)
                               );
        
        SetNewDisplayRowTop (displayRowTop + numRows);
        for (row = height - numRows;  row < height;  row++)
          PaintARow (row, bufferDC);
      }

      panelDC.DrawImageUnscaled (buffer, 0, 0);
      {
        // Scroll Blobs
        if  (blobs != null)
        {
          foreach  (Blob  b in blobs)
          {
            b.Scroll (numRows);
            PaintRectangle (panelDC, Color.Red, b);
          }
        }
      }

    }  /* ScrollRows */



    public  void  AddUserSpecifiedBlob (int tlCol,  int tlRow,
                                        int brCol,  int brRow
                                       )
    {
      if  (blobs == null)
        blobs = new BlobList (true);
      

      Blob b = new Blob (-1, tlCol, tlRow, brCol, brRow);
      blobs.Add (b);
    }




    public  Blob  SelectedBlob (int col,
                                int row
                               )
    {
      if  (blobs == null)
        return  null;

      foreach  (Blob  b in blobs)
      {
        if  ((col >= b.ColLeft)  &&  (col <= b.ColRight)  &&  (row >= b.RowBot)  &&  (row <= b.RowTop))
          return  b;
      }

      return  null;
    }

    


    public  void  PaintFromBuffer ()
    {
      if  (buffer == null)
        PaintWholePanel ();
      else
        panelDC.DrawImageUnscaled (buffer, 0, 0);
    }



    public  void  PaintAllBlobs ()
    {
      if  (blobs == null)
        return;

      foreach  (Blob b in blobs)
      {
        if  (b != null)
          PaintRectangle (panelDC, Color.Red, b);
      }
    }  /* PaintAllBlobs */




    private  void  PaintRectangle  (Graphics  g,
                                    Color     lineColor,
                                    Blob      b
                                   )
    {
      int  colLeft  = Math.Max (b.ColLeft,  0);
      int  colRight = Math.Min (b.ColRight, width  - 1);
      int  rowTop   = Math.Min (b.RowTop,   height - 1);
      int  rowBot   = Math.Max (b.RowBot,   0);
      
      Pen p = new Pen (lineColor);

      g.DrawLine (p, colLeft,  rowTop, colRight, rowTop);
      g.DrawLine (p, colRight, rowTop, colRight, rowBot);
      g.DrawLine (p, colRight, rowBot, colLeft,  rowBot);
      g.DrawLine (p, colLeft,  rowBot, colLeft,  rowTop);
    }  /* PaintRectangle */
                                         



    public  void  PaintTemporayRectangle (Color  lineColor,
                                          int    x1, 
                                          int    y1, 
                                          int    x2, 
                                          int    y2
                                         )
    {
      Bitmap    tempBuffer = new Bitmap (buffer, buffer.Width, buffer.Height);
      Graphics  tempBufferDC = Graphics.FromImage (tempBuffer);
      
      Pen p = new Pen (lineColor);
      
      tempBufferDC.DrawLine (p, x1, y1, x2, y1);
      tempBufferDC.DrawLine (p, x2, y1, x2, y2);
      tempBufferDC.DrawLine (p, x2, y2, x1, y2);
      tempBufferDC.DrawLine (p, x1, y2, x1, y1);

      panelDC.DrawImageUnscaled (tempBuffer, 0, 0);
    }  /* PaintTemporayRectangle */






    public  byte[][]  ExtractImageFromSipper (Blob      blob,
                                              ref long  firstScanLine,
                                              ref int   colLeft
                                             )

    {
      if  (sipperFile == null)
        return  null;

      int  x1 = Math.Max (0, blob.ColLeft - 1);
      int  y1 = Math.Max (0, blob.RowBot  - 1);
      int  x2 = Math.Min (panel.Width - 1,  blob.ColRight + 1);
      int  y2 = Math.Min (panel.Height - 1, blob.RowTop   + 1);
    
       firstScanLine = (int)((displayRowTop + y1) * sipperFile.Ratio ());
       colLeft       = (int)Math.Floor ((float)x1 * sipperFile.Ratio ()) + sipperFile.CropColLeft ();

       int  lastScanLine = (int)((float)(displayRowTop + y2 + 1) * sipperFile.Ratio () + 0.5f);
       
       int  colRight     = (int)((float)(x2 + 1) * sipperFile.Ratio ()) + sipperFile.CropColLeft ();
       if  (colRight >= sipperFile.ScanLineSize ())
         colRight = sipperFile.ScanLineSize () - 1;
       
       if  (colLeft >= colRight)
         return null;

       byte[][] rows = sipperFile.GetScanLines (firstScanLine, lastScanLine);
       
       int  height = (int)(1 + lastScanLine - firstScanLine);
       int  width  = (int)(1 + colRight     - colLeft);
       
       byte[][]  raster = new byte[height][];

       int row, col, sipperCol;
       for  (row = 0;  row < height;  row++)
       {
         byte[]  rowData = rows[row];
         
         raster[row] = new byte[width];
         
         sipperCol = colLeft;
         col = 0;;
         
         while  (col < width)
         {
           raster[row][col] = rowData[sipperCol];
           col++;
           sipperCol++;
         }
       }

       return  raster;
    }   /* ExtractImageFromSipper */                                
                         
    
       
    private  void  SetUpSipperStreamPanelContectMenu ()
    {
      sipperStreamPanelContextMenu = new ContextMenu ();

      if  (sipperFile != null)
      {
        sipperStreamPanelContextMenu.MenuItems.Add ("Set LeftCrop", new EventHandler (SetLeftCrop));
        if  (sipperFile.CropColLeft () > 0)
          sipperStreamPanelContextMenu.MenuItems.Add ("Release LeftCrop", new EventHandler (ReleaseLeftCrop));
          
        sipperStreamPanelContextMenu.MenuItems.Add ("Set RightCrop", new EventHandler (SetRightCrop));
        if  (sipperFile.CropColRight () < (sipperFile.ScanLineSize () - 1))
          sipperStreamPanelContextMenu.MenuItems.Add ("Release RightCrop", new EventHandler (ReleaseRightCrop));

        sipperStreamPanelContextMenu.MenuItems.Add ("Set Extraction Start Point", new EventHandler (SetExtractionStartPoint));
        sipperStreamPanelContextMenu.MenuItems.Add ("Set Extraction End   Point", new EventHandler (SetExtractionEndPoint));
      }

      panel.ContextMenu = sipperStreamPanelContextMenu;
    }




    private  void  ReleaseLeftCrop (object sender, System.EventArgs e)
    {
      if  (sipperFile != null)
      {
        cropColumnLeft = 0;
        ComputeParameters ();
        PaintWholePanel ();
      }
    } /* ReleaseLeftCrop */



    public  void  SetCropColumnLeft (int  _cropColumnLeft)
    {
      int  scanLineLen = 4096;
      cropColumnLeft = _cropColumnLeft;
      if  (sipperFile != null)
        scanLineLen = sipperFile.PixelsPerDisLine ();
      if  ((1 + cropColumnRight - cropColumnLeft) <= scanLineLen)
      {
        cropColumnRight = cropColumnLeft + scanLineLen - 1;
        
        //if  (cropColumnRight >= sipperFile.ScanLineSize ())
        if  (cropColumnRight >= scanLineLen)
        {
          //cropColumnRight = sipperFile.ScanLineSize () - 1;
          cropColumnRight = scanLineLen - 1;
          cropColumnLeft  = cropColumnRight - scanLineLen + 1;
        }
      }

      ComputeParameters ();
      PaintWholePanel ();
    }



    private  void  SetLeftCrop (object sender, System.EventArgs e)
    {
      if  (sipperFile != null)
      {
        SetCropColumnLeft (sipperFile.ScanColumnFromDisplayCol (rightMouseCol));
      }
    } /* SetLeftCrop */




    private  void  ReleaseRightCrop (object sender, System.EventArgs e)
    {
      if  (sipperFile != null)
      {
        cropColumnRight = sipperFile.ScanLineSize () - 1;
        ComputeParameters ();
        PaintWholePanel ();
      }
    } /* ReleaseRightCrop */



    public  void  SetCropColumnRight (int  _cropColumnRight)
    {
      cropColumnRight = _cropColumnRight;
      int  scanLineLen = 4096;
      if  (sipperFile != null)
        scanLineLen = sipperFile.PixelsPerDisLine ();

      if  ((1 + cropColumnRight - cropColumnLeft) <= scanLineLen)
      {
        cropColumnLeft = cropColumnRight - scanLineLen + 1;
        if  (cropColumnLeft < 0)
        {
          cropColumnLeft = 0;
          cropColumnRight = scanLineLen - 1;
        }
      }

      ComputeParameters ();
      PaintWholePanel ();
    }

    

    private  void  SetRightCrop (object sender, System.EventArgs e)
    {
      if  (sipperFile != null)
      {
        int  scanLineLen = sipperFile.PixelsPerDisLine ();
        SetCropColumnRight (sipperFile.ScanColumnFromDisplayCol (rightMouseCol));
      }
    } /* SetLeftCrop */




    private  void  SetExtractionStartPoint (object sender, System.EventArgs e)
    {
      if  (dbConn == null)
        return;

      uint  scanLine = (uint)((displayRowTop + rightMouseRow) * sipperFile.Ratio ());

      DialogResult  dr = MessageBox.Show (panel, "Set Image Extraction starting Scan-Line to " + 
                                          scanLine.ToString ("###,###,##0") +
                                          " ?", "Image Extraction Start Point", 
                                          MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        PicesSipperFile sf = dbConn.SipperFileRecLoad (dbSipperFile.SipperFileName);
        if  (sf == null)
        {
          MessageBox.Show (panel, "Error retrieving existing SipperFile Entry from database", "Update Starting Extraction Failed", MessageBoxButtons.OK);
        }
        else
        {
          sf.ExtractionScanLineStart = (uint)scanLine;
          dbConn.SipperFileUpdate (sf);
          dbSipperFile.ExtractionScanLineStart = scanLine;
        }
      }
    }




    private  void  SetExtractionEndPoint (object sender, System.EventArgs e)
    {
      if  (dbConn == null)
        return;

      uint  scanLine = (uint)((displayRowTop + rightMouseRow) * sipperFile.Ratio ());

      DialogResult  dr = MessageBox.Show (panel, "Set Image Extraction Ending Scan-Line to " + 
                                          scanLine.ToString ("###,###,##0") +
                                          " ?", "Image Extraction End Point", 
                                          MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        PicesSipperFile sf = dbConn.SipperFileRecLoad (dbSipperFile.SipperFileName);
        if  (sf == null)
        {
          MessageBox.Show (panel, "Error retrieving existing SipperFile Entry from database", "Update Ending Extraction Failed", MessageBoxButtons.OK);
        }
        else
        {
          sf.ExtractionScanLineEnd =  scanLine;
          dbConn.SipperFileUpdate (sf);
          dbSipperFile.ExtractionScanLineEnd = scanLine;
        }
      }
    }



    private  void  MonitorMouseClick (object  sender, MouseEventArgs mea)
    {
      if  (mea.Button == MouseButtons.Right)
      {
        rightMouseCol = mea.X;
        rightMouseRow = mea.Y;
      }
    }
    
  
  }  /* SipperFileViewerParameters */
}
