#include  "FirstIncludes.h"
#include  <fstream>
#include  <iostream>
#include  <map>
#include  <vector>

#include  "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "RasterSipper.h"
#include "Str.h"
using namespace  KKU;



#include "LogicalFrameBlob.h"
using namespace  ImageExtractionManager;



LogicalFrameBlob::LogicalFrameBlob (uint32  _maxFrameHeight,
                                    uint32  _maxFrameWidth
                                   ):
  id (-1),
  explored            (false),
  maxFrameHeight      (_maxFrameHeight),
  maxFrameWidth       (_maxFrameWidth),
  rowBot              (0),
  rowTop              (0),
  colLeft             (0),
  colRight            (0),
  neighborIdLastAdded (-1),
  neighbors           (),
  pixelCount          (0)
  
{
}


LogicalFrameBlob::~LogicalFrameBlob ()
{
  neighbors.clear ();
}



void  LogicalFrameBlob::DialateBlob (uchar**   frame,
                                     int32**   blobIds,
                                     int32     size
                                    )
{
  rowTop   = Max (0, int32 (rowTop) - size);
  rowBot   = Min (maxFrameHeight - 1, rowBot + size);
  colLeft  = Max (0, int32 (colLeft) - size);
  colRight = Min (maxFrameWidth - 1, colRight + size);

  uint32  height = 1 + rowBot   - rowTop;
  uint32  width  = 1 + colRight - colLeft;

  uint32  totPixels = height * width;

  int32*  workArea = new int32[totPixels];
  int32** workRows = new int32*[height];

  uint32 row, col;
  uint32 workRow = 0;
  uint32 workCol = 0;
  int32  curWorkPixel = 0;
  int32* curWorkRow = NULL;

  int32* curRow = NULL;

  uint32  windowRow, windowCol;

  for  (row = rowTop;  row <= rowBot;  row++)
  {
    workRows[workRow] = curWorkRow = &(workArea[curWorkPixel]);
    curRow = blobIds[row];
    workCol = 0;
    
    for  (col = colLeft;  col <= colRight;  col++)
    {
      curWorkRow[workCol] = curRow[col];
      workCol++;
    }

    curWorkPixel += width;
    workRow++;
  }


  workRow = 0;
  for  (row = rowTop;  row <= rowBot;  row++)
  {
    curWorkRow = workRows[workRow];
    curRow = blobIds[row];
    workCol = 0;
    
    for  (col = colLeft;  col <= colRight;  col++)
    {
      if  (curWorkRow[workCol] >= 0)
      {
        // This pixel assigned to a blob already
        continue;
      }

      if  (frame[row][col] == 0)
      {
        // This is a background pixel anyway, it would not be assigned to a blob.
        continue;
      }


      // Next lets determine if this pixzel will be the recipeint of dialation.
      bool  dialatePixel = false;
      for  (windowRow = Max ((int32 (workRow) - size), 0);  (windowRow < (workRow + size))  &&  (!dialatePixel);  windowRow++)
      {
        if  (windowRow >= height)
          break;

        for  (windowCol = Max (0, (int32 (workCol) - size));  windowCol < (workCol + size);  windowCol++)
        {
          if  (windowCol >= width)
            break;

          if  (workRows[windowRow][windowCol] == id)
          {
            dialatePixel = true;
            pixelCount++;
            blobIds[row][col] = id;
            break;
          }
        }
      }

      workCol++;
    }

    workRow++;
  }

  delete  workArea;
  delete  workRows;
}  /* DialateBlob */




void  LogicalFrameBlob::AddNeighbor (LogicalFrameBlobPtr  _neighbor)
{
  int32  neighborId = _neighbor->Id ();
  if  (neighborId == neighborIdLastAdded)
    return;

  neighbors.insert (pair<int32, LogicalFrameBlobPtr> (neighborId, _neighbor));
  neighborIdLastAdded = neighborId;
}  /* AddNeighbor */


