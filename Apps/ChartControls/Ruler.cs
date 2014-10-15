using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;


namespace ChartControls
{
  public partial class Ruler : UserControl
  {

    private int   firstCol  = 0;
    private int   lastCol   = 0;
    private Color lineColor = Color.Black;
    private float minValue = 0.0f;
    private float maxValue = 100.0f;
    private int   numTics = 3;


    public delegate void  MinMaxChangedHandler ();

    public delegate void  PlotChangedHandler();


    public Ruler ()
    {
      this.SizeChanged += new System.EventHandler (this.OnSizeChanged);
      this.Load += new System.EventHandler (this.OnLoad);
      InitializeComponent ();
    }


    // Declare the event, which is associated with our
    // delegate MinMaxChanged(). Add some attributes
    // for the Visual C# control property.
    [Category ("Action")]
    [Description ("Fires when either the min and max fields are changed.")]
    public event MinMaxChangedHandler  MinMaxChanged;


    // Declare the event, which is associated with our
    // delegate PlotChangedHandler(). Add some attributes
    // for the Visual C# control property.
    [Category("Action")]
    [Description("Fires when the plot box has been changed.")]
    public event PlotChangedHandler  PlotChanged;




    protected virtual void OnMinMaxChanged ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      if (MinMaxChanged != null)
      {
        MinMaxChanged ();  // Notify Subscribers
      }
    }


    protected virtual void OnPlotChanged ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      if (PlotChanged != null)
      {
        PlotChanged();  // Notify Subscribers
      }
    }



    /// <summary>
    /// Specify the color of the ruller line.
    /// </summary>
    [Bindable(true), Category("Ruler Options"),
    Description("Specify color of ruler line.")]
    public Color LineColor
    {
      get { return lineColor; }
      set { lineColor = value; }
    }



    /// <summary>
    /// Specify the number tic marks on the ruler
    /// </summary>
    [Bindable(true), Category("Ruler Options"),
     DefaultValue(5),
     Description("Specify number of tic marks on ruler.")]
    public int NumTics
    {
      get { return numTics; }
      set
      {
        numTics = value;
        if (numTics < 3)
          numTics = 3;
      }
    }



    /// <summary>
    /// Specify minimum value of ruler (Left Side).
    /// </summary>
    [Bindable(true), Category("Ruler Options"),
     DefaultValue(0),
     Description("Specify minimum value of ruler(Left Side).")]
    public float MinValue
    {
      get { return minValue; }
      set
      {
        minValue = value;
        minValueField.Text = minValue.ToString ();
      }
    }


    /// <summary>
    /// Specify maximum value of ruler (Right Side).
    /// </summary>
    [Bindable(true), Category("Ruler Options"),
     DefaultValue(100),
     Description("Specify maximum value of ruler(Right Side).")]
    public float MaxValue
    {
      get { return maxValue; }
      set
      {
        maxValue = value;
        maxValueField.Text = maxValue.ToString ();
      }
    }


    /// <summary>
    /// Specify if  you want to plaoit data associated with this ruler.
    /// </summary>
    [Bindable(true), Category("Ruler Options"),
     DefaultValue(100),
    Description("Specify if  you want to plaoit data associated with this ruler.")]
    public bool Plot
    {
      get { return plot.Checked; }
      set
      {
        plot.Checked = value;
      }
    }

   
    // A caller canuse the next two methods to determine the start and end
    // pixels used by the ruler line.
    public  int  FirstCol ()  {return firstCol;}
    public  int  LastCol  ()  {return lastCol;}


    private void UpdateComponentsForSizeChange()
    {
      int distRightSideFromLeft = this.maxValueField.Right;
      int width = this.Width;
      int distToMoveRright = width - distRightSideFromLeft;
      int newDistFromLeft = maxValueField.Left + distToMoveRright;
      maxValueField.Left = newDistFromLeft;
      this.DrawRulerLine();
    }



    private void OnLoad (object sender, EventArgs e)
    {
      // Place code to be executed when initialy placed on form.

    }  /* OnLoad */



    private void OnSizeChanged (object sender, EventArgs e)
    {
      // Place code that is to be executed when ever this control is resized.
      UpdateComponentsForSizeChange();
    }

   

    private void DrawRulerLine ()
    {
      Graphics g = this.CreateGraphics ();

      int lineRow = Height - 8;
      int lineFirstCol = (int)((float)maxValueField.Width / 2.0f + 0.5f);
      firstCol = lineFirstCol;
      int lineWidth = Width - (lineFirstCol * 2);
      lastCol  = firstCol + lineWidth - 1;

      Pen p = new Pen (lineColor, 3);
      g.DrawLine (p, lineFirstCol, lineRow, lineFirstCol + lineWidth, lineRow);

      float valueRange = maxValue - minValue;
      float colsBetweenTics = lineWidth / (float)(numTics - 1);
      float valueRangeBetweenTics = valueRange / (float)(numTics - 1);

      p.Width = 1;
      p.Color = Color.White;
      int x;
      for (x = 0; x < numTics; x++)
      {
        int ticCol = lineFirstCol + (int)((float)x * colsBetweenTics + 0.5f);
        g.DrawLine (p, ticCol, lineRow - 4, ticCol, lineRow + 4);

        float ticValue = (float)x * valueRangeBetweenTics + minValue;
        //g.DrawString
      }
    }  /* DrawRulerLine */



    protected override void OnPaint (PaintEventArgs e)
    {
      base.OnPaint (e);
      DrawRulerLine ();
    }



    private float StrToFloat (string s)
    {
      float f = 0.0f;
      try { f = float.Parse (s); }
      catch (Exception) { f = 0.0f; }
      return f;
    }



    private int  StrToInt (string s)
    {
      int  i = 0;
      try { i = int.Parse (s); }
      catch (Exception) { i = 0; }
      return i;
    }


    private void minValueField_Leave (object sender, EventArgs e)
    {
      float  newMinValue = StrToFloat (minValueField.Text);
      if  (newMinValue != minValue)
      {
        minValue = newMinValue;
        OnMinMaxChanged ();
      }
    }
    

    private void maxValueField_Leave (object sender, EventArgs e)
    {
      float  newMaxValue = StrToFloat (maxValueField.Text);
      if (newMaxValue != maxValue)
      {
        maxValue = newMaxValue;
        OnMinMaxChanged ();
      }
    }

    
    private void plot_CheckedChanged(object sender, EventArgs e)
    {
      OnPlotChanged ();
    }


    private void Ruler_Load (object    sender, 
                             EventArgs e
                            )
    {
      UpdateComponentsForSizeChange ();
    }

    
    private void Ruler_Layout (object sender, LayoutEventArgs e)
    {
      UpdateComponentsForSizeChange ();
    }
  }  /* Ruler */
}