using System;
using System.Collections.Generic;
using System.Text;

namespace SipperFile
{
  public  class  BlobList:  List<Blob> 
  {
    private  int  nextBlobId = 0;
    private  bool owner      = false;




    public  BlobList (bool  _owner)
    {
      owner = _owner;
      nextBlobId = 0;
    }


    public  Blob  LookUpByBlobId (int  blobId)
    {
      if  ((blobId < 0)  ||  (blobId >= nextBlobId))
      {
        throw new Exception ("BlobList::LookUpByBlobId    Invalid BlobID[" + blobId.ToString () + "]");
      }

      return  this[blobId];
    }  /* LookUpByBlobId */




    public  void  Dispose ()
    {
      if  (owner)
      {
        int  idx = 0;
        for  (idx = 0;  idx < Count;  idx++)
        {
          if  (this[idx] != null)
          {
            this[idx].Dispose ();
            this[idx] = null;
          }
        }
      }
    }



    
    public  Blob  NewBlob (int  rowTop, 
                           int  colLeft
                          )
    {
      Blob  blob = new Blob (nextBlobId);
      blob.RowTop   = rowTop;
      blob.RowBot   = rowTop;
      blob.ColLeft  = colLeft;
      blob.ColRight = colLeft; 
      nextBlobId++;
      this.Add (blob);
      return  blob;
    }


    public  Blob  MergeIntoSingleBlob (Blob     blob1,
                                       int      blob2Id,
                                       int[][]  blobIds
                                      )
    {                               
      Blob  blob2 = LookUpByBlobId (blob2Id);
      if  (blob2 == null)
         return  blob1;

      Blob srcBlob = null;
      Blob destBlob = null;

      if (blob1.PixelCount > blob2.PixelCount)
      {
        srcBlob = blob2;
        destBlob = blob1;
      }
      else
      {
        srcBlob = blob1;
        destBlob = blob2;
      }

      int srcBlobId  = srcBlob.Id;
      int destBlobId = destBlob.Id;

      int  col = 0;
      int  row = 0;

      int  rowBot   = srcBlob.RowBot;
      int  rowTop   = srcBlob.RowTop;
      int  colLeft  = srcBlob.ColLeft;
      int  colRight = srcBlob.ColRight;

      for  (row = rowBot;  row <= rowTop;  row++)
      {
        for  (col = colLeft;  col <= colRight;  col++)
        {
          if  (blobIds[row][col] == srcBlobId)
             blobIds[row][col] = destBlobId;
        }
      }

      destBlob.RowTopMaybe   = srcBlob.RowTop;
      destBlob.RowBotMaybe   = srcBlob.RowBot;
      destBlob.ColLeftMaybe  = srcBlob.ColLeft;
      destBlob.ColRightMaybe = srcBlob.ColRight;
      destBlob.PixelCount    = srcBlob.PixelCount + destBlob.PixelCount;

      DeleteEntry (srcBlobId);
      srcBlob.Dispose ();
      return destBlob;
    }  /* MergeIntoSingleBlob */





      
    public  void  DeleteEntry (int blobId)
    {
      this[blobId] = null;
    }



    public  Blob  LocateLargestBlob ()
    {
      int       idx;
      int       qSize       = Count;
      Blob      blob        = null;
      Blob      tempBlob    = null;
      int       largestSize = 0;

      for  (idx = 0;  idx < qSize;  idx++)
      {
        tempBlob = this[idx];
        if  (tempBlob == null)
          continue;

        if  (tempBlob.PixelCount > largestSize)
        {
          largestSize = tempBlob.PixelCount;
          blob = tempBlob;
        }
      }

      return  blob;
    } /* LocateLargestBlob */

  }  /* BlobList */
}

