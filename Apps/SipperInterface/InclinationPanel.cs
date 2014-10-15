using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;


namespace SipperInterface
{
  class InclinationPanel : System.Windows.Forms.Panel
  {
    private int    centerRow        = 0;
    private int    centerCol        = 0;
    private float  inclinationAngle = 0.0f;
    private int    lastHeight       = 0;
    private int    lastWidth        = 0;
    private System.Windows.Forms.PrintPreviewDialog printPreviewDialog1;
    private float  topLeftAngle     = 0.0f;  // Angle between center at top left corner.
       

    public void  InclinationAngle (float _inclinationAngle)
    {
      inclinationAngle = _inclinationAngle;
    }


    InclinationPanel ()
    {
    }
    
    public  void  UpdateAngles ()
    {
      centerRow = this.Height / 2;
      centerCol = this.Width  / 2;
      
      topLeftAngle = -(float)Math.Atan2 (centerRow, centerCol);
      lastHeight   = Height;
      lastWidth    = Width;
    }  /* UpdateAngles */


    public void  DrawAngleLine ()
    {
      if  ((lastHeight != Height)  ||  (lastWidth != Width))
        UpdateAngles ();
        
      System.Drawing.Graphics graphics = CreateGraphics ();
      
      graphics.Clear (this.BackColor);

      inclinationAngle = (float)Math.PI * inclinationAngle / 180.0f;
     
      int  startRow = 0;
      int  startCol = 0;
      int  endRow   = 0;
      int  endCol   = 0;
      
      int  x = 0;
        
      // Calc starrting point.
      if  (inclinationAngle <= topLeftAngle)
      {
        // Line starts on top line and ends on botom line
        startRow = 2;
        x = (int)(0.5f + ((float)centerRow / 2.0f) / (float)Math.Tan (-inclinationAngle));
        startCol = centerCol - x;
          
        endRow = this.Height - 4;
        endCol = centerCol + x;
      }
      
      else if (inclinationAngle >= topLeftAngle)
      {
        // Line starts and ends on left and right border.
          
        x = (int)(0.5f + ((float)centerRow - 2.0) / Math.Tan (inclinationAngle));
        startRow = 2 * centerRow - 2;
        startCol = centerCol - x;
         
        endRow = 2;
        endCol =  centerCol + x;
      }
      
      else if  (inclinationAngle >= 0)
      {
        x = (int)(0.5f + Math.Tan (inclinationAngle) * ((float)centerCol - 2.0));
        startCol = 2;
        startRow = -x;
        
        endCol = centerCol + centerCol - 2;
        endRow = x;
      }

      else
      {
        // Starts at left border below (y = 0), and goes to right border.
        x = (int)(0.5f + Math.Tan (inclinationAngle) * ((float)centerCol - 2.0f));
        startRow = 2;
        startCol = -x;
        endRow   = x;
        endCol   = centerCol + centerCol - 2;
      }

      {
        // Draw CrossHairs
        Pen pen = new Pen (Color.Black, 1);
        graphics.DrawLine (pen, 1, centerRow, Width - 2, centerRow);
        graphics.DrawLine (pen, centerCol, 1, Height - 2, centerCol);
      }

      {
        // Draw Inclination Line
        Pen pen = new Pen (Color.Blue, 3);
        graphics.DrawLine (pen, startCol, startRow, endCol, endRow);
      }
    } /* DrawAngleLine */



    protected override void OnPaint (System.Windows.Forms.PaintEventArgs e)
    {
      base.OnPaint(e);
      DrawAngleLine ();
    }

    private void InitializeComponent ()
    {
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (InclinationPanel));
      this.printPreviewDialog1 = new System.Windows.Forms.PrintPreviewDialog ();
      this.SuspendLayout ();
      // 
      // printPreviewDialog1
      // 
      this.printPreviewDialog1.AutoScrollMargin = new System.Drawing.Size (0, 0);
      this.printPreviewDialog1.AutoScrollMinSize = new System.Drawing.Size (0, 0);
      this.printPreviewDialog1.ClientSize = new System.Drawing.Size (400, 300);
      this.printPreviewDialog1.Enabled = true;
      this.printPreviewDialog1.Icon = ((System.Drawing.Icon)(resources.GetObject ("printPreviewDialog1.Icon")));
      this.printPreviewDialog1.Name = "printPreviewDialog1";
      this.printPreviewDialog1.Visible = false;
      this.ResumeLayout (false);

    }
    
  }  /* InclinationPanel */
}
