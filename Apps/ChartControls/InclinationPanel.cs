using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace ChartControls
{
  public partial class InclinationPanel : UserControl
  {
    private  float  inclination = 0.0f;
    private  float  lastInclinationDrawn = -999.99f;
    private  Color  lineColor   = Color.Blue;
    
    private  Point  startPoint;
    private  Point  endPoint;


    public InclinationPanel ()
    {
      this.SizeChanged += new System.EventHandler (this.OnSizeChanged);
      this.Load += new System.EventHandler (this.OnLoad);
      InitializeComponent ();
      
      startPoint.X = 0;
      startPoint.Y = 0;
      endPoint.X = 0;
      endPoint.Y = 0;
    }



    /// <summary>
    /// Specify the color of the inclination line.
    /// </summary>
    [Bindable (true), Category ("Inclination Options"),
    Description ("Specify color of inclination line.")]
    public Color LineColor
    {
      get {return lineColor;}
      set {lineColor = value;}
    }



    /// <summary>
    /// Specify Title,  ex Pitch, Roll.
    /// </summary>
    [Bindable (true), Category ("Inclination Options"),
    DefaultValue ("Pitch"),
    Description ("Specify Title.")]
    public string Title
    {
      get  {return  this.titleLabel.Text;}
      set  {this.titleLabel.Text = value;}
    }  /* Title */



    /// <summary>
    /// Specify ang that Inclination line forms with horizon
    /// </summary>
    [Bindable (true), Category ("Inclination Options"),
    DefaultValue (0.0f),
    Description ("Specify angle.")]
    public float Inclination
    {
      get  {return  inclination;}
      set
      {
        inclination = value;
        inclinationDisplay.Text = value.ToString ();
        DrawInclinationLine ();
      }
    }


    private void OnLoad (object sender, EventArgs e)
    {
      // Place code to be executed when initialy placed on form.
    }  /* OnLoad */


    private void AdjustForCurrentSize ()
    {
      titleLabel.Width = Width - 2;
      titleLabel.Left = 1;

      int  totalWidth  = this.Width;
      int  totalHeight = this.Height;

      this.chartPanel.Width = totalWidth;
      this.inclinationDisplay.Width = totalWidth;

      int heightUsedUp = titleLabel.Height + inclinationDisplay.Height;
      int heightLeft   = totalHeight - heightUsedUp;
      chartPanel.Height = heightLeft;

      titleLabel.Top = 0;
      chartPanel.Top = titleLabel.Height;
      inclinationDisplay.Top = (titleLabel.Height + chartPanel.Height);

      lastInclinationDrawn = 999.99f;

      DrawInclinationLine ();
    }  /* AdjustForCurrentSize */


    private void  OnSizeChanged (object sender, EventArgs e)
    {
      // Place code that is to be executed whenever this control is resized.
      AdjustForCurrentSize ();
    }  /* OnSizeChanged */



    protected override void OnPaint (PaintEventArgs e)
    {
      base.OnPaint (e);
      DrawInclinationLine ();
    }


    private  void  CalcStartAndEndPoints (ref int centerCol, ref int centerRow)
    {
      centerRow = chartPanel.Height / 2;
      centerCol = chartPanel.Width / 2;

      // inclination is in degree's we need radians.

      float inclinationAngle = (float)Math.PI * inclination / 180.0f;

      int startRow = 0;
      int startCol = 0;
      int endRow = 0;
      int endCol = 0;

      int x = 0;
      int y = 0;

      if (inclination == 90.0)
      {
        // We want a Vertical Line
        startRow = chartPanel.Height - 2;
        endCol = startCol = centerCol;
        endRow = 2;
      }

      else if (inclination == 0.0f)
      {
        // We have a horizontal line
        startRow = endRow = centerRow;
        startCol = 2;
        endCol = chartPanel.Width - 2;
      }

      else
      {
        y = (int)((float)(centerCol - 1) * Math.Tan (-inclinationAngle) + 0.5f);
        if ((y <= centerRow) && (y >= -centerRow))
        {
          startRow = centerRow - y;
          startCol = 1;
          endRow = centerRow + y;
          endCol = centerCol + centerCol - 1;
        }
        else
        {
          if (inclinationAngle > 0.0f)
          {
            x = (int)(0.5f + (float)(centerRow - 1) / Math.Tan (inclinationAngle));
            startRow = centerRow + centerRow - 2;
            startCol = centerCol - x;
            endRow = 1;
            endCol = centerCol + x;
          }
          else
          {
            x = (int)(0.5f + (float)(centerRow - 1) / Math.Tan (-inclinationAngle));
            startRow = 1;
            startCol = centerCol - x;
            endRow = centerRow + centerRow - 2;
            endCol = centerCol + x;
          }
        }
      }
      
      startPoint.X = startCol;
      startPoint.Y = startRow;
      
      endPoint.X = endCol;
      endPoint.Y = endRow;
    }  /* CalcStartAndEndPoints */


    private  void   DrawInclinationLine ()
    {
      int centerCol = 0;
      int centerRow = 0;
      
      if  (inclination != lastInclinationDrawn)
        CalcStartAndEndPoints (ref centerCol, ref centerRow);

      System.Drawing.Graphics graphics = null;
      try
      {
        graphics = chartPanel.CreateGraphics ();
        graphics.Clear (chartPanel.BackColor);
      }
      catch  (Exception e)
      {
        MessageBox.Show ("'InclinationPanel'  Error trying to CreateGraphics" + "\n\n" + e.ToString ());
      }
      

      {
        // Draw CrossHairs
        Pen pen = new Pen (Color.Black, 1);
        graphics.DrawLine (pen, 1, centerRow, chartPanel.Width - 2, centerRow);
        graphics.DrawLine (pen, centerCol, 1, centerCol, chartPanel.Height - 2);
      }

      {
        // Draw Inclination Line
        Pen pen = new Pen (lineColor, 3);
        graphics.DrawLine (pen, startPoint, endPoint);
      }
    }

    private void chartPanel_Layout (object sender, LayoutEventArgs e)
    {
      AdjustForCurrentSize ();
    } /* DrawAngleLine */

  }  /* InclinationPanel */
}
