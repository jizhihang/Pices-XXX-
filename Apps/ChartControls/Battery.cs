using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace ChartControls
{
  public partial class Battery : UserControl
  {
    private Color colorDead      = Color.Red;
    private Color colorEmpty     = Color.Gray;
    private Color colorFull      = Color.Green;
    private Color colorNearEmpty = Color.Orange;

    private Color currentColor = Color.Green;



    private float maxVoltage  = 12.0f;
    private float voltage     = 12.0f;

    private bool  alive = true; 

    private float oldVoltage = 0.0f;

    private float termHeightPerc = 0.10f;
    private int spaceBetweenVoltgeFieldAndBattery = 3;

    private int batHeight = 0;
    private int batWidth = 0;

    private int termHeight = 0;
    private int termWidth = 0;

    private int batTop = 0;
    private int batLeft = 0;
    private int termTop = 0;
    private int termLeft = 0;

    private int batFullRow = 0;
    private int batFullRowsHeight = 0;
    private int batEmptyRowsHeight = 0;


    public Battery ()
    {
      this.SizeChanged += new System.EventHandler (this.OnSizeChanged);
      this.Load += new System.EventHandler (this.OnLoad);
      InitializeComponent ();
    }


    /// <summary>
    /// Specifies if the battery is alive
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    DefaultValue (true),
    Description ("Specifies if battery is alive or dead.")]
    public bool Alive
    {
      get {return alive;}
      set
      {
        if  (alive != value)
        {
          alive = value;
          SetColorOfBattery ();
          DrawBattery ();
        }
      }
    }



    /// <summary>
    /// Specifies the max voltage that the batery will support,  
    /// this will allow us to compute the full part of the battery.
    /// </summary>
   [Bindable(true), Category("Battery Options"), 
   DefaultValue(12),
   Description("Specify the maximum voltage that can be detected.")]    
   public  float  MaxVoltage
    {
      get  {return  maxVoltage;}
      set 
      {
        maxVoltage = value;
      }
    }



    /// <summary>
    /// Specifies the current voldtage reading on the battery.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    DefaultValue (12),
    Description ("Specify the current voltage reading on the battery.")]
    public float Voltage
    {
      get  
      {
        return  voltage;
      }
      set
      {
        bool  voltageChanged = (value != voltage);
        voltage = value;
        voltageField.Text = value.ToString () + "V";
        if  (voltageChanged)
        {
          CalcBatteryDimensions ();
          SetColorOfBattery ();
          DrawBattery();
        }
      }
    }



    /// <summary>
    /// Specifies the Color to use for representing the full part of the battery.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    Description ("Specify the color that represents the full part.")]
    public  System.Drawing.Color ColorFull
    {
      get  {return  colorFull;}
      set  {colorFull = value;}
    }



    /// <summary>
    /// Specifies the Color to use for representing the empty part of the battery.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    Description ("Specify the color that represents the empty part.")]
    public System.Drawing.Color ColorEmpty
    {
      get  {return colorEmpty;}
      set  {colorEmpty = value;}
    }



    /// <summary>
    /// Specifies the Color to use for representing the near empty part of the battery.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    Description ("Specify the color that represents the near empty part.")]
    public System.Drawing.Color ColorNearEmpty
    {
      get {return colorNearEmpty;}
      set {colorNearEmpty = value;}
    }



    /// <summary>
    /// Font to use for the numeric voltage display.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    Description ("Specify the Font to use for displaying voltage level.")]
    public  System.Drawing.Font  VoltageFont
    {
      get  {return voltageField.Font;}
      set  {voltageField.Font = value;}
    }
   


    private void  SetColorOfBattery ()
    {
      Color  oldColor = currentColor;
    
      if  (!alive)
      {
        currentColor = colorDead;
      }
      
      else if  (voltage < 18.0f)
      {
        alive = false;
        currentColor = colorDead;
      }
      
      else if  (voltage <= 22.0f)
      {
        currentColor = colorNearEmpty;
      }
      
      else
      {
        currentColor = colorFull;
      }
    }  /* SetColorOfBattery */



    private void OnLoad (object sender, EventArgs e)
    {
      SetColorOfBattery();
      PlaceVoltageField();
    }  /* OnLoad */



    private void OnSizeChanged (object sender, EventArgs e)
    {
      PlaceVoltageField ();
    }



    private  void  PlaceVoltageField ()
    {
      // Place code that is to be executed when ever this control is resized.
      CalcBatteryDimensions ();

      int newVoltageFieldWidth = Math.Min (80, Math.Max (30, Width - 7));
      voltageField.Width = newVoltageFieldWidth;

      int newTop = Height - (voltageField.Height + 2);
      voltageField.Top = newTop;

      int newLeft = (int)(0.5f + (float)(Width - voltageField.Width) / 2.0f);
      voltageField.Left = newLeft;
    }



    private  void  CalcBatteryDimensions ()
    {
      termTop = 3;
      
      int  availHeight = this.Height - (voltageField.Height + termTop + spaceBetweenVoltgeFieldAndBattery);
      
      termHeight = (int)((float)availHeight * termHeightPerc + 0.5f);
      batHeight  = availHeight - termHeight;
      batWidth   = this.Width - 12;
      termWidth  = (int)((0.2 * (float)batWidth)  + 0.5f);
      
      batTop   = termTop + termHeight;
      batLeft  = (int)(0.5f + (float)(this.Width - batWidth) / 2.0f);
      termLeft = batLeft + (int)(0.5f + (float)(batWidth - termWidth) / 2.0f);
      
      float  percFull = 1.0f;
      if  ((maxVoltage > 0.0f)  &&  (voltage < maxVoltage))
        percFull = voltage / maxVoltage;
        
      batFullRowsHeight  = (int)(0.5f + (float)batHeight * percFull);
      batEmptyRowsHeight = batHeight - batFullRowsHeight;

      batFullRow = batTop + batEmptyRowsHeight;

      batFullRow = batTop + (int)(0.5f + (float)batHeight * (1.0f - percFull));
      
      oldVoltage = voltage;
    }  /* CalcBatteryDimensions */



    private void DrawBattery ()
    {
      
      Graphics g = null;
      
      try
      {
        g = this.CreateGraphics ();
      }
      catch  (Exception e)
      {
        MessageBox.Show ("Error creating Graphics object in 'Battery'" + "\n\n" + e.ToString ());
        return;
      }


      Pen p = new Pen (Color.Black);
      
      g.DrawRectangle (p, termLeft, termTop,    termWidth, termHeight);

      if  (batEmptyRowsHeight > 0)
      {
        Brush  b = new SolidBrush (Color.Gray);
        g.FillRectangle (b, batLeft, batTop, batWidth, batEmptyRowsHeight);
      }

      {
        Brush b = new SolidBrush (currentColor);
        g.FillRectangle (b, batLeft, batFullRow, batWidth, batFullRowsHeight);
      }
    }  /* DrawBattery */




    private  void  PaintBattery ()
    {
      if  (oldVoltage != voltage)
      {
        SetColorOfBattery ();
        CalcBatteryDimensions ();
      }

      DrawBattery ();
    }  /* PaintBattery */




    protected override void OnPaint (PaintEventArgs e)
    {
      base.OnPaint (e);
      PaintBattery ();
    }


    public void RePaint (PaintEventArgs e)
    {
      OnPaint (e);
    }


    private void  UpdateComponentEnabledFlags ()
    {
      voltageField.Enabled = this.Enabled;
    }


    private void Battery_EnabledChanged (object sender, EventArgs e)
    {
      UpdateComponentEnabledFlags ();
    }
  }
}
