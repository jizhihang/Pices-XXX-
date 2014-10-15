using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace ChartControls
{
  public partial class BatteryPack : UserControl
  {
    private  int   activeBattery = 1;
    private  Color colorFull     = Color.Green;
    private  Color colorEmpty    = Color.Gray;
  
    private  float maxVoltage              = 12.0f;
    private  int   requestedBattery        = -1;
    private  bool  requestedBatteryEnabled = true;   // When set to true user can request a battery change
    private  Color normalBackGroundColor   = Color.Beige;


    public delegate void BatteryClickedHandler ();
  
  
    public BatteryPack ()
    {
      this.SizeChanged += new System.EventHandler (this.OnSizeChanged);
      this.Load += new System.EventHandler (this.OnLoad);
      InitializeComponent ();
      normalBackGroundColor = battery1.BackColor;
    }


    // Declare the event, which is associated with our
    // delegate SelectedOptionChanged(). Add some attributes
    // for the Visual C# control property.
    [Category ("Action")]
    [Description ("Fires when the Battery Clicked.")]
    public event BatteryClickedHandler BatteryClicked;




    protected virtual void OnBatteryClicked ()
    {
      requestedBatteryEnabled = false;
      SetBatteryEnabledFlags ();
      SetRequestedBatteryBackground ();
    
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      if  (BatteryClicked != null)
      {
        BatteryClicked ();  // Notify Subscribers
      }
    }




    /// <summary>
    /// Specifies which battery is active;  that battery will
    /// have a border drawn arround it and all others will not.
    /// </summary>
    [Bindable (true), 
     Category ("Battery Options"),
     DefaultValue (1),
     Description ("Specify which battery is active.")]
    public int  ActiveBattery
    {
      get
      {
        return activeBattery;
      }
      set
      {
        SetBorder (activeBattery, BorderStyle.None);
        SetBorder (value, BorderStyle.FixedSingle);
        activeBattery = value;
        
        if  (activeBattery == requestedBattery)
        {
          RequestedBatteryEnabled = true;
        }
      }
    }



    /// <summary>
    /// Specifies which battery user is requesting that we switch to.
    /// </summary>
    [Bindable (true),
     Category ("Battery Options"),
     DefaultValue (1),
     Description ("Specify which battery is requested.")]
    public int RequestedBattery
    {
      get  {return requestedBattery;}
      set  
      {
        requestedBattery = value;
        SetRequestedBatteryBackground ();
        if  (requestedBattery >= 0)
          this.OnBatteryClicked ();
      }
    }




    /// <summary>
    /// Specify if battery request is enabled
    /// </summary>
    [Bindable (true),
     Category ("Battery Options"),
     DefaultValue (true),
     Description ("Specify if battery request enabled.")]
    public bool  RequestedBatteryEnabled
    {
      get { return requestedBatteryEnabled; }
      set
      {
        requestedBatteryEnabled = value;
        SetBatteryEnabledFlags ();
        
        if  (value)
        {
          // Since we are saying that a battery request can be made
          // then one can not be pending; So we will set background
          // on all batteries to normal color.
          requestedBattery = -1;
        }

        SetRequestedBatteryBackground ();
      }
    }



    /// <summary>
    /// Specifies the max voltage that the batery will support,  
    /// this will allow us to compute the full part of the battery.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    DefaultValue (12),
    Description ("Specify the maximum voltage that can be detected.")]
    public float MaxVoltage
    {
      get 
      { 
        return maxVoltage; 
      }
      set
      {
        maxVoltage = value;
        battery1.MaxVoltage = value;
        battery2.MaxVoltage = value;
        battery3.MaxVoltage = value;
        battery4.MaxVoltage = value;
      }
    }



    /// <summary>
    /// Specifies voltage on battery 0.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    DefaultValue (12),
    Description ("Specify battery 0's voltage.")]
    public float Battery1Voltage
    {
      get  {return battery1.Voltage;}
      set  {battery1.Voltage = value;}
    }




    /// <summary>
    /// Specifies voltage on battery 1.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    DefaultValue (12),
    Description ("Specify battery 1's voltage.")]
    public float Battery2Voltage
    {
      get  {return battery2.Voltage;}
      set  {battery2.Voltage = value;}
    }




    /// <summary>
    /// Specifies voltage on battery 2.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    DefaultValue (12),
    Description ("Specify battery 2's voltage.")]
    public float Battery3Voltage
    {
      get  {return battery3.Voltage;}
      set  {battery3.Voltage = value;}
    }




    /// <summary>
    /// Specifies voltage on battery 3.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    DefaultValue (12),
    Description ("Specify battery 3's voltage.")]
    public float Battery4Voltage
    {
      get  {return battery4.Voltage;}
      set  {battery4.Voltage = value;}
    }



    /// <summary>
    /// Specifies the Color to use for representing the full part of the battery.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    Description ("Specify the color that represents the full part.")]
    public System.Drawing.Color ColorFull
    {
      get  {return battery1.ColorFull;}
      set  
      {
        battery1.ColorFull = value;
        battery2.ColorFull = value;
        battery3.ColorFull = value;
        battery4.ColorFull = value;
      }
    }




    /// <summary>
    /// Specifies the Color to use for representing the empty part of the battery.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    Description ("Specify the color that represents the empty part.")]
    public System.Drawing.Color ColorEmpty
    {
      get  {return battery1.ColorEmpty;}
      set
      {
        battery1.ColorEmpty = value;
        battery2.ColorEmpty = value;
        battery3.ColorEmpty = value;
        battery4.ColorEmpty = value;
      }
    }




    /// <summary>
    /// Specifies the Color to use for representing the "near empty" part of the battery.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    Description ("Specify the color that represents the empty part.")]
    public System.Drawing.Color ColorNearEmpty
    {
      get  {return battery1.ColorNearEmpty; }
      set
      {
        battery1.ColorNearEmpty = value;
        battery2.ColorNearEmpty = value;
        battery3.ColorNearEmpty = value;
        battery4.ColorNearEmpty = value;
      }
    }



    /// <summary>
    /// Font to use for the numeric voltage display.
    /// </summary>
    [Bindable (true), Category ("Battery Options"),
    Description ("Font to use for numeric voltage.")]
    public System.Drawing.Font VoltageFont
    {
      get  {return  battery1.VoltageFont;}
      set
      {
        battery1.VoltageFont = value;
        battery2.VoltageFont = value;
        battery3.VoltageFont = value;
        battery4.VoltageFont = value;
      }
    }



    private  void  SetBorder (int  batteryIdx, BorderStyle  borderStype)
    {
      switch  (batteryIdx)
      {
        case 1: battery1.BorderStyle = borderStype; break;
        case 2: battery2.BorderStyle = borderStype; break;
        case 3: battery3.BorderStyle = borderStype; break;
        case 4: battery4.BorderStyle = borderStype; break;
      }
    }



    private  void  SetRequestedBatteryBackground ()
    {
      battery1.BackColor = ((requestedBattery == 1) ? Color.Blue : normalBackGroundColor);
      battery2.BackColor = ((requestedBattery == 2) ? Color.Blue : normalBackGroundColor);
      battery3.BackColor = ((requestedBattery == 3) ? Color.Blue : normalBackGroundColor);
      battery4.BackColor = ((requestedBattery == 4) ? Color.Blue : normalBackGroundColor);
    }



    private void OnLoad (object sender, EventArgs e)
    {
      SetBorder (activeBattery, BorderStyle.FixedSingle); 
      PlaceBatteries ();
    }  /* OnLoad */



    private void OnSizeChanged (object sender, EventArgs e)
    {
      PlaceBatteries ();
    }


    private  void  PlaceBatteries ()
    {
      Title.Left = (Width - Title.Width) / 2;

      int  batWidth = (int)(0.5f + (float)(Width - 30) / 4.0);
      
      int  batHeight = (Height - (Title.Height + autoSelectButton.Height + 12));
      int  batTop    = Title.Height + 4;
      
      int  l = 3;
      battery1.Left   = l;
      battery1.Width  = batWidth;
      battery1.Height = batHeight;
      battery1.Top    = batTop;
      
      l += 7 + batWidth;
      battery2.Left = l;
      battery2.Width = batWidth;
      battery2.Height = batHeight;
      battery2.Top    = batTop;

      l += 7 + batWidth;
      battery3.Left = l;
      battery3.Width = batWidth;
      battery3.Height = batHeight;
      battery3.Top    = batTop;

      l += 7 + batWidth;
      battery4.Left = l;
      battery4.Width = batWidth;
      battery4.Height = batHeight;
      battery4.Top    = batTop;
      
      autoSelectButton.Top  = batTop + batHeight + 4;
      autoSelectButton.Left = (Width - autoSelectButton.Width) / 2;
    }  /* PlaceBatteries */



    protected override void OnPaint (PaintEventArgs e)
    {
      base.OnPaint (e);
      battery1.RePaint (e);
      battery2.RePaint (e);
      battery3.RePaint (e);
      battery4.RePaint (e);
    }


    public  void  PopulateFields (int    _activeBattery,
                                  float  _bat1Voltage,
                                  float  _bat2Voltage,
                                  float  _bat3Voltage,
                                  float  _bat4Voltage,
                                  string _batteryStatuses
                                 )
    {
       if  (_activeBattery != activeBattery)
         ActiveBattery = _activeBattery;
         
       battery1.Voltage = _bat1Voltage;
       battery2.Voltage = _bat2Voltage; 
       battery3.Voltage = _bat3Voltage;
       battery4.Voltage = _bat4Voltage;
       
       if  (_batteryStatuses.Length >= 4)
       {
         battery1.Alive = (_batteryStatuses[0] == 'L');
         battery2.Alive = (_batteryStatuses[1] == 'L');
         battery3.Alive = (_batteryStatuses[2] == 'L');
         battery4.Alive = (_batteryStatuses[3] == 'L');
       }
    }




    private  void SetBatteryEnabledFlags ()
    {
      autoSelectButton.Enabled = requestedBatteryEnabled;
      
      battery1.Enabled = requestedBatteryEnabled;
      battery2.Enabled = requestedBatteryEnabled;
      battery3.Enabled = requestedBatteryEnabled;
      battery4.Enabled = requestedBatteryEnabled;
    }



    private void battery1_DoubleClick (object sender, EventArgs e)
    {
      if  (battery1.Alive)
      {
        requestedBattery = 1;
        OnBatteryClicked ();
      }
    }



    private void battery2_DoubleClick (object sender, EventArgs e)
    {
      if  (battery2.Alive)
      {
        requestedBattery = 2;
        OnBatteryClicked ();
      }
    }



    private void battery3_DoubleClick (object sender, EventArgs e)
    {
      if  (battery3.Alive)
      {
        requestedBattery = 3;
        OnBatteryClicked ();
      }
    }



    private void battery4_DoubleClick (object sender, EventArgs e)
    {
      if  (battery4.Alive)
      {
        requestedBattery = 4;
        OnBatteryClicked ();
      }
    }



    private void autoSelectButton_Click (object sender, EventArgs e)
    {
      requestedBattery = 0;
      OnBatteryClicked ();
    }

    private void battery1_Load(object sender, EventArgs e)
    {

    }

    private void BatteryPack_Layout (object sender, LayoutEventArgs e)
    {
      PlaceBatteries ();
    }  /*  */
  }
}
