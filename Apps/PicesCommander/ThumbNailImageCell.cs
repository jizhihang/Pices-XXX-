using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using  PicesInterface;

namespace PicesCommander
{
  class ThumbNailImageCell: DataGridViewCell
  {

    public override  Type FormattedValueType { get  {return  typeof (PicesDataBaseImage) ;} } 


    public bool GetThumbnailImageAbort()
    {
      return true;
    }  /* GetThumbnailImageAbort */


    private  System.IntPtr  thumbNailCallBackData = IntPtr.Zero;



    private  Image  MakeImageFit (Image  src, 
                                  int    maxHeight,
                                  int    maxWidth
                                 )
    {
      int  destHeight = src.Height;
      int  destWidth  = src.Width;

      if  ((destHeight <= maxHeight)  &&  (destWidth <= maxWidth))
        return  src;

      float  heightFact = (float)destHeight / (float)maxHeight;
      float  widthFact  = (float)destWidth  / (float)maxWidth;

      float  factor = 0.0f;
      if  (heightFact < widthFact)
        factor = heightFact;
      else
        factor = widthFact;

      destWidth  = Math.Min (maxWidth,  (int)((float)destWidth  * factor + 0.5f));
      destHeight = Math.Min (maxHeight, (int)((float)destHeight * factor + 0.5f));

      return  src.GetThumbnailImage (destWidth, destHeight, GetThumbnailImageAbort, thumbNailCallBackData);
    }  /* MakeImageFit */


    static public  void  DisplayClass1 (bool _displayClass1)
    {
      displayClass1 = _displayClass1;
    }


    static public  void  DisplayClass2 (bool _displayClass2)
    {
      displayClass2 = _displayClass2;
    }



    //********************************************************************************************************
    //*                        Code to support the vreating of a single thumbnail image.                     *
    //********************************************************************************************************
    static Font   thumbNailFont            = new Font (FontFamily.GenericSansSerif, 8.25f, FontStyle.Regular, GraphicsUnit.Point);
    static Brush  thumbNailLabelBrush      = new SolidBrush  (Color.Gray);
    static Brush  thumbNailDataBrush       = new SolidBrush  (Color.Black);
    static Brush  thumbNailSelectedBrush   = new SolidBrush  (Color.Red);
    static Pen    thumbNailSelectedPen     = new Pen         (new SolidBrush (Color.LightGray));
    static Brush  thumbNailBackGround      = new SolidBrush  (Color.White);
    static Pen    thumbNailBorderPen       = new Pen         (Color.Black);

    static bool   displayClass1 = false;
    static bool   displayClass2 = false;

    static StringFormat  stringFormat = new StringFormat ();



    protected override void Paint (Graphics                  graphics,
                                   Rectangle                 clipBounds,
                                   Rectangle                 cellBounds,
                                   int                       rowIndex,
                                   DataGridViewElementStates cellState,
                                   object                    value,
                                   object                    formattedValue,
                                   string                    errorText,
                                   DataGridViewCellStyle     cellStyle,
                                   DataGridViewAdvancedBorderStyle advancedBorderStyle,
                                   DataGridViewPaintParts    paintParts
                                  )
    {
      // Call the base class method to paint the default cell appearance.
      base.Paint (graphics,   
                  clipBounds,     
                  cellBounds, 
                  rowIndex,   
                  cellState,  
                  value,      
                  formattedValue, 
                  errorText,  
                  cellStyle,
                  advancedBorderStyle, 
                  paintParts
                );

      bool  selected = ((cellState & DataGridViewElementStates.Selected) == 
                       DataGridViewElementStates.Selected);
                       
      int  borderWidth = 1;
      graphics.FillRectangle (selected ? thumbNailSelectedBrush : thumbNailBackGround, cellBounds);
      
      if  ((value == null)  ||  (value.GetType () != typeof (PicesDataBaseImage)))
      {
        graphics.DrawRectangle (thumbNailBorderPen, cellBounds);
        return;
      } 

      PicesDataBaseImage  dbImage = (PicesDataBaseImage)value;
        
      int  availWidth  = cellBounds.Width  - 2 * borderWidth - 2;
      int  availHeight = cellBounds.Height - 2 * borderWidth;
      
      int  heightNeededForText = 2 * thumbNailFont.Height  // (Probability and Break-Tie) + (Size and Depth)
                               + (String.IsNullOrEmpty (dbImage.ValidatedClassName) ? 0 : thumbNailFont.Height)
                               + (displayClass1 ? thumbNailFont.Height : 0)
                               + (displayClass2 ? thumbNailFont.Height : 0);

      if  (!String.IsNullOrEmpty (dbImage.ValidatedClassName))
         heightNeededForText += thumbNailFont.Height;
      
      int  heightAvailForImage = availHeight - heightNeededForText;
 
      Image displayImage = dbImage.Thumbnail ();
      if  ((displayImage.Height > heightAvailForImage)  ||  (displayImage.Width > availWidth))

      displayImage = MakeImageFit (displayImage, heightAvailForImage, availWidth);

      // we want to paint the image in the center
      int  paintRow = cellBounds.Y + borderWidth + (heightAvailForImage - displayImage.Height) / 2;
      int  paintCol = cellBounds.X + borderWidth + (availWidth          - displayImage.Width)  / 2;

      graphics.DrawImage (displayImage, paintCol, paintRow, displayImage.Width, displayImage.Height);

      int  curPixelRow = heightAvailForImage + cellBounds.Y;  // This is where 1st line of text must go
      int  leftCol = cellBounds.X + borderWidth;

      if  (!String.IsNullOrEmpty (dbImage.ValidatedClassName))
      {
        RectangleF  rec = new RectangleF (leftCol + 20, curPixelRow - 1, cellBounds.Width - 22, thumbNailFont.Height);
        graphics.DrawString ("V:", thumbNailFont, thumbNailLabelBrush, leftCol + 1, curPixelRow);
        graphics.DrawString (dbImage.ValidatedClassName, thumbNailFont, thumbNailDataBrush, rec, stringFormat);
        curPixelRow += thumbNailFont.Height;
      }

      if  (displayClass1)
      {
        RectangleF  rec = new RectangleF (leftCol + 20, curPixelRow - 1, cellBounds.Width - 22, thumbNailFont.Height);
        graphics.DrawString ("C1:", thumbNailFont, thumbNailLabelBrush, leftCol + 1, curPixelRow);
        graphics.DrawString (dbImage.Class1Name, thumbNailFont, thumbNailDataBrush, rec, stringFormat);
        curPixelRow += thumbNailFont.Height;
      }

      if  (displayClass2)
      {
        RectangleF  rec = new RectangleF (leftCol + 20, curPixelRow - 1, cellBounds.Width - 22, thumbNailFont.Height);
        graphics.DrawString ("C2:", thumbNailFont, thumbNailLabelBrush, leftCol + 1, curPixelRow);
        graphics.DrawString (dbImage.Class2Name, thumbNailFont, thumbNailDataBrush, rec, stringFormat);
        curPixelRow += thumbNailFont.Height;
      }

      graphics.DrawString ("P:", thumbNailFont, thumbNailLabelBrush, leftCol + 1, curPixelRow);
      graphics.DrawString ((dbImage.Class1Prob).ToString ("##0.0%"), thumbNailFont, thumbNailDataBrush, leftCol  + 20, curPixelRow);

      graphics.DrawString ("B:", thumbNailFont, thumbNailLabelBrush, leftCol + 71, curPixelRow);
      graphics.DrawString ((dbImage.BreakTie).ToString ("##0.0%"), thumbNailFont, thumbNailDataBrush, leftCol + 90, curPixelRow);
      curPixelRow += thumbNailFont.Height;

      graphics.DrawString ("S:", thumbNailFont, thumbNailLabelBrush, leftCol + 1, curPixelRow);
      graphics.DrawString (dbImage.PixelCount.ToString ("###,##0"), thumbNailFont, thumbNailDataBrush, leftCol + 20, curPixelRow);

      graphics.DrawString ("D:", thumbNailFont, thumbNailLabelBrush, leftCol + 71, curPixelRow);
      graphics.DrawString (dbImage.Depth.ToString ("#,##0.0"), thumbNailFont, thumbNailDataBrush, leftCol + 90, curPixelRow);
      curPixelRow += thumbNailFont.Height;

      graphics.DrawRectangle (thumbNailBorderPen, cellBounds);
      for  (int  zed = 1;  zed < borderWidth;  zed++)
      {
        Rectangle  r = new Rectangle (cellBounds.X + borderWidth, cellBounds.Y + borderWidth, cellBounds.Width - 2 * borderWidth, cellBounds.Height - 2 * borderWidth);
        graphics.DrawRectangle (thumbNailBorderPen, cellBounds);
      }

      return;
    }  /* Paint */




    
    protected  override  void  OnDoubleClick (DataGridViewCellEventArgs e)
    {
      if  (DataGridView == null)
        return;

      if  (e.ColumnIndex >= DataGridView.Columns.Count)
        return;

      ThumbNailImageColumn  tnic = ((ThumbNailImageColumn)(this.DataGridView.Columns[e.ColumnIndex]));

      IWin32Window parent = tnic.Parent;
      if  (parent.GetType () != typeof (PicesCommander))
        return;
      
      PicesCommander  pc = (PicesCommander)parent;
      if  (pc == null)
        return;

      pc.ThumbnailDataGridView_CellContentDoubleClick (DataGridView, e);
    }  /* OnDoubleClick */


    protected override void OnMouseDoubleClick(DataGridViewCellMouseEventArgs e)
    {
      base.OnMouseDoubleClick(e);
    }

  }  /* ThumbNailImageCell */
}
