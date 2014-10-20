using System;
using System.Collections.Generic;
using System.Text;

using  PicesInterface;

namespace SipperFile
{
  /*!
   \brief  Used for Connected Component Analysis.  One instance for each blob detected.
   \see  BlobDetector
   \see  BlobList
   */
  
  public  class Blob
  {
    private  int  colLeft    = int.MaxValue;
    private  int  colRight   = int.MinValue;
    private  int  id         = -1;
    private  int  pixelCount = 0;
    private  int  rowBot     = int.MaxValue;
    private  int  rowTop     = int.MinValue;

    private  PicesPrediction   pred = null;


    public  Blob (int  _id)
    {
      id = _id;
    }


    public  Blob (int  _id,
                  int  _tlRow, int  _tlCol,  
                  int  _brRow, int  _brCol
                 )
    {
      id = -id;
      colLeft   = _tlCol;
      colRight  = _brCol;
      rowTop    = _tlRow;
      rowBot    = _brRow;
    }
       


    public  void  Dispose ()
    {
    }
    

    public  int  Id         {get {return id;}         set  {id = value;}}
    
    public  int  ColLeft    {get {return colLeft;}    set  {colLeft  = value;}}
    public  int  ColRight   {get {return colRight;}   set  {colRight = value;}}
    public  int  Height     {get {return (1 + rowTop - rowBot);}}
    public  int  RowBot     {get {return rowBot;}     set  {rowBot = value;}}
    public  int  RowTop     {get {return rowTop;}     set  {rowTop = value;}}
    public  int  Width      {get {return (1 + colRight - colLeft);}}
    public  int  PixelCount {get {return pixelCount;} set  {pixelCount = value;}}

    public  int  ColLeftMaybe   {set {if  (value < colLeft)   colLeft  = value;}}
    public  int  ColRightMaybe  {set {if  (value > colRight)  colRight = value;}}
    public  int  RowBotMaybe    {set {if  (value < rowBot)    rowBot   = value;}}
    public  int  RowTopMaybe    {set {if  (value > rowTop)    rowTop   = value;}}

    public  PicesPrediction  Pred {get {return pred;}   set {pred = value;}}


    public  void  PixelCountIncrement ()
    {
      pixelCount++;
    }


    public  void  Scroll (int numRows)
    {
      rowBot -= numRows;
      rowTop -= numRows;
    }

  }
}

