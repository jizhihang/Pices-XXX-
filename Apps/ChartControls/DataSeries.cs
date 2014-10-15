using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace ChartControls
{
  public partial class DataSeries : UserControl
  {
    float  minimum   = 0.0f;
    float  maximum   = 0.0f;
    bool   plot      = true;
    Color  plotColor = Color.Gray;

    int    suspendLevel = 0;
    
    //bool   enabled = true;


    public delegate void SelectedLabelChangedHandler ();
    public delegate void PlotChangedHandler ();
    public delegate void PlotColorChangedHandler ();
    public delegate void RangeChangedHandler ();
    public delegate void MinMaxCalcRequestHandler (DataSeries  ds);
  
    public DataSeries ()
    {
      this.SizeChanged += new System.EventHandler (this.OnSizeChanged);
      this.Load += new System.EventHandler (this.OnLoad);
      InitializeComponent ();
      AddContextMenu ();
    }



    private void OnSizeChanged (object sender, EventArgs e)
    {
    }



    private  void  AddContextMenu ()
    {
      ContextMenuStrip  cms = new ContextMenuStrip ();
      cms.Items.Add ("Change Color", null, ChangeColor);
      cms.Items.Add ("Calc Min/Max", null, CalcMinMax);
      this.ContextMenuStrip = cms;
    }  /* AddContextMenu */

    
    
    protected void  ChangeColor (Object     sender,
                                 EventArgs  e
                                )
    {
      SelectPlotColor ();
    }  /* InsertRequest */



    protected void  CalcMinMax (Object     sender,
                                EventArgs  e
                               )
    {
      OnMinMaxCalcRequest ();
    }  /* InsertRequest */
    


    new void SuspendLayout()
    {
      suspendLevel++;
      if (suspendLevel < 2)
      {
        base.SuspendLayout();
        dataLabel.SuspendLayout ();
        dataMin.SuspendLayout ();
        dataMax.SuspendLayout ();
        data.SuspendLayout ();
        plotData.SuspendLayout ();
      }
    }  /* SuspendLayout */


    new void ResumeLayout()
    {
      if (suspendLevel <= 0)
        return;

      suspendLevel--;
      if (suspendLevel < 1)
      {
        base.ResumeLayout();
        dataLabel.ResumeLayout ();
        dataMin.ResumeLayout ();
        dataMax.ResumeLayout ();
        data.ResumeLayout ();
        plotData.ResumeLayout ();
      }
    }  /* ResumeLayout */



    new void ResumeLayout(bool perFormLayout)
    {
      if (suspendLevel <= 0)
        return;

      suspendLevel--;
      if (suspendLevel < 1)
      {
        base.ResumeLayout();
        dataLabel.ResumeLayout ();
        dataMin.ResumeLayout ();
        dataMax.ResumeLayout ();
        data.ResumeLayout ();
        plotData.ResumeLayout ();
      }
    }  /* ResumeLayout */



    private void OnLoad (object sender, EventArgs e)
    {
    }  /* OnLoad */


    

    /// <summary>
    /// Wheather to plot this series.
    /// </summary>
    [Bindable (true), Category ("Series Options"),
    Description ("Plot this data series.")]
    public bool Plot
    {
      get { return plot; }
      set 
      { 
        plot = value;
        plotData.Checked = plot;
      }
    }  /* Labels */



    /// <summary>
    /// Plot Color
    /// </summary>
    [Bindable (true), Category ("Series Options"),
    Description ("Color to use in Plot.")]
    public Color PlotColor
    {
      get { return plotColor; }
      set
      {
        plotColor = value;
        this.BackColor = value;
      }
    }  /* Labels */




    /// <summary>
    /// Specifies list of choices
    /// </summary>
    [Bindable (true), Category ("Series Options"),
    Description ("Data that is to be ploted.")]
    public string[] Labels
    {
      get
      {
        int  numLabels = dataLabel.Items.Count;
        string[]  labels = new string[numLabels];
        int x;
        for  (x = 0;  x < numLabels;  x++)
          labels[x] = dataLabel.Items[x].ToString ();
        return  labels;
      }

      set
      {
        int  x;
        dataLabel.ForeColor = Color.Black;
        dataLabel.Items.Clear ();
        
        if  (value.Length == 0)
        {
          dataLabel.Items.Add ("");
        } 
        else
        {
          for (x = 0; x < value.Length; x++)
            dataLabel.Items.Add (value[x]);
        }

        dataLabel.SelectedIndex = 0;
      }
    }  /* Labels */



    /// <summary>
    /// Specifies list of coices
    /// </summary>
    [Bindable (true), Category ("Series Options"),
    Description ("Selected Label.")]
    public string SelectedLabel
    {
      get  
      {
        return  dataLabel.SelectedItem.ToString ();
      }
      set  
      {
        int  x = GetLabelIndex (value);
        if  (x >= 0)
        {
          dataLabel.SelectedIndex = x;
          dataLabel.Text = dataLabel.Items[x].ToString ();
        }
        else
        {
          dataLabel.Text = "";
        }
      }
    }  /* SelectedLabel */



    private int  GetLabelIndex (string  s)
    {
      int numLabels = dataLabel.Items.Count;
      int x;
      for  (x = 0;  x < numLabels;  x++)
      {
        if  (dataLabel.Items[x].ToString () == s)
          return  x;
      }
      return -1;
    }  /* GetLabelIndex */



    /// <summary>
    /// Specifies Index of currently selected Label
    /// </summary>
    [Bindable (true), Category ("Series Options"),
    Description ("Selected Index.")]
    public int SelectedIndex
    {
      get { return dataLabel.SelectedIndex; }
      set 
      { 
        if  ((value >= 0)  &&  (value < dataLabel.Items.Count))
          dataLabel.SelectedIndex = value; 
      }
    }  /* SelectedIndex */




    /// <summary>
    /// Minimum Value of this field.
    /// </summary>
    [Bindable (true), Category ("Series Options"),
    DefaultValue (0.0f),
    Description ("Minimum value for plot.")]
    public float Minimum
    {
      get { return  minimum; }
      set
      {
        minimum = value;
        dataMin.Text = minimum.ToString ();
      }
    }



    /// <summary>
    /// Maximum Value of
    /// </summary>
    [Bindable (true), Category ("Series Options"),
    DefaultValue (0.0f),
    Description ("Maximum value for plot.")]
    public float Maximum
    {
      get { return maximum; }
      set
      {
        maximum = value;
        dataMax.Text = maximum.ToString ();
      }
    }



    /// <summary>
    /// Specifies if the battery is alive
    /// </summary>
    [Bindable (true), Category ("Series Options"),
    DefaultValue (0.0f),
    Description ("Current data value for label.")]
    public float Data
    {
      get { return StrToFloat (data.Text); }
      set {data.Text = value.ToString ();}
    }



    // Declare the event, which is associated with our
    // delegate SelectedLabelChanged(). Add some attributes
    // for the Visual C# control property.
    [Category ("Action")]
    [Description ("Fires when the Selected label changes.")]
    public event SelectedLabelChangedHandler SelectedLabelChanged;


    protected virtual void OnSelectedLabelChanged ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      if  (Enabled  &&  (SelectedLabelChanged != null))
      {
        Enabled = false;
        SelectedLabelChanged ();  // Notify Subscribers
        Enabled = true;
      }
    }




    // Declare the event, which is associated with our
    // delegate PlotChanged(). Add some attributes
    // for the Visual C# control property.
    [Category ("Action")]
    [Description ("Fires when the Plot flag changes.")]
    public event PlotChangedHandler PlotChanged;

    protected virtual void OnPlotChanged ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      
      if  (plot == plotData.Checked)
      {
        // Nothing actually changed
        return;
      }
      
      if (Enabled  &&  (PlotChanged != null))
      {
        Enabled = false;
        plot = plotData.Checked;
        PlotChanged ();  // Notify Subscribers
        Enabled = true;
      }
    }  /* OnPlotChanged */




    // Declare the event, which is associated with our
    // delegate PlotColorChanged(). Add some attributes
    // for the Visual C# control property.
    [Category ("Action")]
    [Description ("Fires when the PlotColor changes.")]
    public event PlotColorChangedHandler PlotColorChanged;

    protected virtual void OnPlotColorChanged ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
  
      if (Enabled  &&  (PlotColorChanged != null))
      {
        Enabled = false;
        PlotColorChanged ();  // Notify Subscribers
        Enabled = true;
      }
    }  /* OnPlotColorChanged */



    [Category ("Action")]
    [Description ("Fires when user requests recalc of MinMax .")]
    public  event  MinMaxCalcRequestHandler  MinMaxCalcRequest;

    protected  virtual  void  OnMinMaxCalcRequest ()
    {
      MinMaxCalcRequest (this);
    }



    // Declare the event, which is associated with our
    // delegate RangeChanged(). Add some attributes
    // for the Visual C# control property.
    [Category ("Action")]
    [Description ("Fires when the minimum or maximum range changes.")]
    public event RangeChangedHandler RangeChanged;


    protected virtual void OnRangeChanged ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      
      float  xMin = StrToFloat (dataMin.Text);
      float  xMax = StrToFloat (dataMax.Text);
      
      if  ((xMin == minimum)  &&  (xMax == maximum))
      {
        // Nothing actually changed
        return;
      }


      if (Enabled)
      {
        minimum = xMin;
        maximum = xMax;
        if  (RangeChanged != null)
        {
          Enabled = false;
          RangeChanged ();  // Notify Subscribers
          Enabled = true;
        }
      }
    }  /* OnRangeChanged */




    public  void  SetDataLabels (string[]  dataLabels)
    {
      int  x;
      this.dataLabel.Items.Clear ();
      
      for  (x = 0;  x < dataLabels.Length;  x++)
      {
        dataLabel.Items.Add (dataLabels[x]);
      }
    }  /* SetDataLabels */




    private void  SelectPlotColor ()
    {
      if  (!Enabled)
        return;
        
      plotColorDialog.Color = plotColor;
      plotColorDialog.ShowDialog ();

      if  (plotColorDialog.Color != plotColor)
      {
        Enabled = false;
        plotColor = plotColorDialog.Color;
        BackColor = plotColor;
        Enabled = true;
        OnPlotColorChanged ();
      }

    }  /* SelectPlotColor */



    public  void  SetParameters (int   _labeblIdx, 
                                 bool  _plot,
                                 Color _plotColor
                                )
    {
      Plot      = _plot;
      PlotColor = _plotColor;
      dataLabel.SelectedIndex = _labeblIdx;
    }



    public  void  SetMinMax (float  _min,
                             float  _max
                            )
    {
      Minimum   = _min;
      Maximum   = _max;
    }




    public void SetSelectedDataLabel (int _labelIdx)
    {
      if  ((_labelIdx >= 0)  &&  (_labelIdx < dataLabel.Items.Count))
      {
        dataLabel.SelectedIndex = _labelIdx;
      }
    }
    


    private void dataLabel_SelectedIndexChanged (object sender, EventArgs e)
    {
      OnSelectedLabelChanged ();
    }


    private void plotData_CheckedChanged (object sender, EventArgs e)
    {
      OnPlotChanged ();
    }

    private void dataMin_Leave (object sender, EventArgs e)
    {
      OnRangeChanged ();
    }

    private void dataMax_Leave (object sender, EventArgs e)
    {
      OnRangeChanged ();
    }


    static public float StrToFloat (string s)
    {
      float f = 0.0f;
      try { f = float.Parse (s); }
      catch (Exception) { f = 0.0f; }
      return f;
    }

    private void DataSeries_Load (object sender, EventArgs e)
    {

    }


    private void DataSeries_DoubleClick (object sender, EventArgs e)
    {
      SelectPlotColor ();
    }  /* StrToFloat */
  }
}
