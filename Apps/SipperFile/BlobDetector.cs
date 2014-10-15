using System;
using System.Collections.Generic;
using System.Text;

namespace SipperFile
{
  public  class BlobDetector
  {
     byte[][]    raster  = null;
     int[][]     blobIds = null;

     int         height  = -1;
     int         width   = -1;

     int         prev1Row = 0;
     int         prev2Row = 0;
     int         prev3Row = 0;

     int         minSize = 300;




     public  BlobDetector (byte[][]  _raster,
                           int       _minSize  
                          )
     {
       int  row, col;

       raster  = _raster;
       minSize = _minSize;

       if  (raster == null)
       {
         return;
       }

       height  = raster.Length;
       width   = raster[0].Length;

       blobIds = new int[height][];
       
       for  (row = 0; row < height; row++)
       {
         blobIds[row] = new int[width];
         for  (col = 0; col < width; col++)
         {
           blobIds[row][col] = -1;
         }
       }
     }


    public  void  Dispose ()
    {
      blobIds = null;
    }




     public  BlobList  ExtractBlobs ()
     {
       byte[]    curRow          = null;
       int[]     curRowBlobIds   = null;

       int       col             = 2;
       int       row             = 2;

       Blob      curBlob         = null;
       int       curBlobId       = 0;
       int       nearBlobId      = 0;

       int       blankColsInARow = 0;

       if  (raster == null)
         return null;


       BlobList blobs = new BlobList (true);

       for  (row = 0;  row < height;  row++)
       {
         curRow            = raster[row];
         curRowBlobIds     = blobIds[row];
         
         prev1Row = row - 1;
         prev2Row = row - 2;
         prev3Row = row - 3;

         curBlob = null;

         col = 0;
         while  (col < width)
         {
           if  (ForegroundPixel (curRow[col]))
           {
             blankColsInARow = 0;

             // Check Upper Left
             nearBlobId = UpperLeft (col);

             if  (nearBlobId < 0)
             {
               // If nothing in Top Left  then check Top Right
               nearBlobId = Max (BlobId (prev1Row, col - 1),
                                 BlobId (prev2Row, col - 1),
                                 BlobId (prev3Row, col - 1),
                                 BlobId (prev1Row, col - 2),
                                 BlobId (prev2Row, col - 2),
                                 BlobId (prev1Row, col - 3)
                                );
             }
  
             if  (curBlob != null)
             {
               if  (nearBlobId >= 0)
               {
                 if  (nearBlobId != curBlobId)
                 {
                   curBlob = blobs.MergeIntoSingleBlob (curBlob, nearBlobId, blobIds);
                   curBlobId = curBlob.Id;
                 }
               }

               curRowBlobIds[col] = curBlobId;
               curBlob.ColRightMaybe = col;
               curBlob.RowBotMaybe   = row;
               curBlob.PixelCountIncrement ();
             }
            
             else
             {
               // No Current Blob
               if  (nearBlobId >= 0)
               {
                 curBlob   = blobs.LookUpByBlobId (nearBlobId);
                 curBlobId = curBlob.Id;
               }
             
               else
               {
                 curBlob = blobs.NewBlob (row, col);
                 curBlobId = curBlob.Id;
               }

               curRowBlobIds[col] = curBlobId;
               curBlob.ColLeftMaybe  = col;
               curBlob.ColRightMaybe = col;
               curBlob.RowBotMaybe   = row;
               curBlob.RowTopMaybe   = row;
               curBlob.PixelCountIncrement ();
             }
           }

           else
           {
             blankColsInARow++;

             if  (blankColsInARow > 3)
             {
               curBlob = null;
               curBlobId = -1;
             }

             else if  (curBlob != null)
             {
               // Check Upper Left
               nearBlobId = UpperLeft (col);

               if  (nearBlobId >= 0)
               {
                 if  (nearBlobId != curBlobId)
                 {
                   curBlob = blobs.MergeIntoSingleBlob (curBlob, nearBlobId, blobIds);
                   curBlobId = curBlob.Id;
                 }
               }
             }
           }

           col++;
         }
       }  /* ExtractBlobs */


            
       BlobList  compressedBlobList = new BlobList (true);
       {
         int  idx;

         for  (idx = 0;  idx < blobs.Count;  idx++)
         {
           Blob b = blobs[idx];

           if  (b != null)
           {
             if  (b.PixelCount < minSize)
             {
               b.Dispose ();
               blobs[idx] = null;
             }
             else
             {
               float widthHeightRatio = (float)b.Width / (float)b.Height;
               if   (widthHeightRatio < 0.1)
               {
                 b.Dispose ();
                 blobs[idx] = null;
               }

               else
               {
                 compressedBlobList.Add (b);
               }
             }

             blobs[idx] = null;
           }
         }
       }

       blobs.Dispose ();
       blobs = null;

       return  compressedBlobList;
     }  /* ExtractBlobs */

     

       
      public  byte[][]  ExtractedBlobImage (Blob  blob)
      {
        if  (blob == null)
          return null;

        int width  = blob.Width;
        int height = blob.Height;
        int blobId = blob.Id;

        byte[][]  subRaster = new byte[height][];

        int row = 0;
        int col = 0;
        int[]   blobIdsRow       = null;
        byte[]  rasterRowData    = null;
        byte[]  subRasterRowData = null;

        int  subRasterRow = 0;
        int  subRasterCol = 0;
      
        for  (row = blob.RowBot;  row <= blob.RowTop;  row++)
        {
          blobIdsRow    = blobIds[row];
          rasterRowData = raster[row];

          subRasterRowData         = new byte[width];
          subRaster[subRasterRow]  = subRasterRowData;

          subRasterCol = 0;
          for  (col = blob.ColLeft;  col <= blob.ColRight;  col++)
          {
            if  (blobIdsRow[col] == blobId)
              subRasterRowData[subRasterCol] = rasterRowData[col];
            else
              subRasterRowData[subRasterCol] = 0;

            subRasterCol++;
          }

          subRasterRow++;
        }

        return  subRaster;
      }  /* ExtractedBlobImage */


       


      private  bool  ForegroundPixel (byte  pixel)
      {
        return  (pixel > 0);
      }



      private  int   BlobId (int  row,
                             int  col
                            )  
      {
        if  ((row < 0)  ||  (row >= height)  ||
             (col < 0)  ||  (col >= width))
          return  -1;

        return  blobIds[row][col];
      }


      private  int  Max (int m1, int m2)  {return  (m1 > m2) ? m1 : m2;}

      private  int  Max (int i1,  int i2,  int i3)
      {
        if  (i1 > i2)
          return  ((i1 > i3) ?  i1 : i3);
        else
          return  ((i2 > i3) ?  i2 : i3);
      }

        
      private  int  Max (int i1, int i2, int i3, int i4, int i5, int i6)
      {
        int  m1 = Max (i1, i2, i3);
        int  m2 = Max (i4, i5, i6);

        return  ((m1 > m2) ? m1 : m2);
      }

    
      private  int  Max (int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9)
      {
        int  m1 = Max (i1, i2, i3);
        int  m2 = Max (i4, i5, i6);
        int  m3 = Max (i7, i8, i9);
        return  Max (m1, m2, m3);
      }

     
      private  int  UpperLeft (int col)
      {
         return  Max (BlobId (prev1Row, col + 3),
                      BlobId (prev1Row, col + 2),
                      BlobId (prev2Row, col + 2),
                      BlobId (prev1Row, col + 1),
                      BlobId (prev2Row, col + 1),
                      BlobId (prev3Row, col + 1),
                      BlobId (prev1Row, col),
                      BlobId (prev2Row, col),
                      BlobId (prev3Row, col)
                     );
      }
  

  
  }  /* BlobDetector */
}

