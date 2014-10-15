using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace ChartControls
{
  public partial class RadioButtonList : UserControl
  {
    int            numColumns        = 1;
    RadioButton[]  options           = null;
    Font           optionsFont       = null;
    int            selectedOptionInt = 0;
    int            suspendLevel      = 0;
    

    public delegate void  SelectedOptionChangedHandler ();
    
    int  logMsgLevel = 0;

    void  LogMsgWrite (String s)
    {
      try
      {
        int pad = logMsgLevel* 2;
        StreamWriter w = new StreamWriter ("c:\\Temp\\RadioButtinList.log", true);
        s = s.PadLeft (s.Length + pad, ' ');
        w.WriteLine (s);
        w.Close ();
        w.Dispose ();
        w = null;
      }
      catch (Exception)
      {
      }
    }


    void  LogMsgEntering (string s)
    {
      LogMsgWrite (s + "   Entering");
      logMsgLevel++;
    }  /* LogMsgEntering */


    void  LogMsgMiddle (string s)
    {
      LogMsgWrite (s);
    }  /* LogMsgEntering */



    void  LogMsgExiting (string s)
    {
      logMsgLevel--;
      LogMsgWrite (s  + "   Exiting");
    }  /* LogMsgExiting */




    /// <summary>
    /// Specifies list of coices
    /// </summary>
    [Bindable (true), Category ("RadioButton Options"),
    Description ("Specify optins that user may choose from.")]
    public string[] Options
    {
      get  
      {
        if  (options == null)
        {
          string[] optionsStr = new string [1];
          optionsStr[0] = "Default Option";
          return optionsStr;
        }
        else
        {  
          string[]  optionsStr = new string[options.Length];
          for  (int x = 0;  x < options.Length;  x++)
            optionsStr[x] = options[x].Text;
          return  optionsStr;
        }
      }
      
      set
      {
        BuildRadioButtonList (value);
      }
    }  /* Options */



    /// <summary>
    /// Specifies list of coices
    /// </summary>
    [Bindable (true), Category ("RadioButton Options"),
    Description ("Specify number of columns.")]
    public int  NumColumns
    {
      get
      {
        return  numColumns;
      }

      set
      {
        if  (value < 1)
          numColumns = 1;
        else
          numColumns = value;
      }
    }




    /// <summary>
    /// Specifies current selected option as a string
    /// </summary>
    [Bindable (true), Category ("RadioButton Options"),
    DefaultValue (0),
    Description ("Specifies selected option as text string.")]
    public string  SelectedOption
    {
      get 
      { 
        if  (options == null)
          return "";
          
        if  ((selectedOptionInt < 0)  ||  (selectedOptionInt >= options.Length))
          return  "";
        else
          return options[selectedOptionInt].Text;
      }
      set
      {
        CheckNewOption (LookUpOption (value));
      }
    }




    /// <summary>
    /// Specifies current selected choice
    /// </summary>
    [Bindable (true), Category ("RadioButton Options"),
    DefaultValue (0),
    Description ("Specifies selected option.")]
    public  int  SelectedOptionInt
    {
      get  {return  selectedOptionInt;}
      set
      {
        CheckNewOption (value);
      }
    }


    private  void  CheckNewOption (int newOptionInt)
    {
      if  ((selectedOptionInt >= 0)  &&  (selectedOptionInt < options.Length))
        options[selectedOptionInt].Checked = false;

      selectedOptionInt = newOptionInt;

      if ((selectedOptionInt >= 0) && (selectedOptionInt < options.Length))
        options[selectedOptionInt].Checked = true;
    }  /* CheckNewOption */




    /// <summary>
    /// Specifies group box title
    /// </summary>
    [Bindable (true), Category ("RadioButton Options"),
    DefaultValue (0),
    Description ("Title/Caption.")]
    public string  Title
    {
      get {return this.optionsPanel.Text; }
      set {optionsPanel.Text = value;}
    }




    /// <summary>
    /// Specifies Font to use for all the options
    /// </summary>
    [Bindable (true), Category ("RadioButton Options"),
    Description ("Font to use for Options.")]
    public Font OptionsFont
    {
      get  {return optionsFont;}
      set 
      { 
        SuspendLayout ();

        optionsFont = value;
        if  (options != null)
        {
          for  (int x = 0;  x < options.Length;  x++)
            options[x].Font = optionsFont;
        }
        
        //AdjustButtonLocationsToFit ();
        
        ResumeLayout ();
      }
    }
    


    public RadioButtonList ()
    {
      LogMsgEntering ("RadioButtonList   *** Entering ***");
      //SizeChanged += new System.EventHandler (this.OnSizeChanged);
      //Load += new System.EventHandler (this.OnLoad);
      InitializeComponent ();
      optionsFont = new Font ("Microsoft Sans Serif", 6.75f);
      LogMsgExiting ("RadioButtonList   Exiting");
    }


    new void  SuspendLayout ()
    {
      LogMsgEntering ("SuspendLayout");
      suspendLevel++;
      if  (suspendLevel < 2)
      {
        base.SuspendLayout ();

        if (options != null)
        {
          for (int x = 0; x < options.Length; x++)
            options[x].SuspendLayout ();
        }
      }
      LogMsgMiddle ("SuspendLayout   suspendLevel[" + suspendLevel + "]");
      LogMsgExiting("SuspendLayout");
    }  /* SuspendLayout */


    private  void   ResumeLayoutOfOptions ()
    {
      LogMsgEntering ("ResumeLayoutOfOptions");

      if (options != null)
      {
        for (int x = 0; x < options.Length; x++)
          options[x].ResumeLayout ();
      }
      LogMsgExiting ("Exiting  ResumeLayoutOfOptions");
    }



    new void  ResumeLayout ()
    {
      LogMsgEntering ("ResumeLayout");
      if  (suspendLevel > 0)
      {
        suspendLevel--;
        if  (suspendLevel < 1)
        {
          base.ResumeLayout();
          ResumeLayoutOfOptions();
          Invalidate (true);
        }
      }

      LogMsgMiddle ("suspendLevel [" + suspendLevel + "]");
      LogMsgExiting ("ResumeLayout");
    }  /* ResumeLayout */



    new void ResumeLayout (bool perFormLayout)
    {
      LogMsgEntering ("ResumeLayout");
      if (suspendLevel <= 0)
        return;

      suspendLevel--;
      if (suspendLevel < 1)
      {
        base.ResumeLayout (perFormLayout);
        ResumeLayoutOfOptions ();
        Invalidate (true);
      }
      LogMsgMiddle ("suspendLevel [" + suspendLevel + "]");
      LogMsgExiting ("ResumeLayout");
    }  /* ResumeLayout */



    private void  AdjustButtonLocationsToFit ()
    {
      LogMsgEntering ("\nAdjustButtonLocationsToFit   *** Enetred ***");

      if  (options == null)
      {
        LogMsgExiting ("\nAdjustButtonLocationsToFit   (options == null)");
        return;
      }

      if  (options.Length < 1)
      {
        LogMsgExiting ("\nAdjustButtonLocationsToFit   (options.lenght < 1)");
        return;
      }

      LogMsgMiddle ("AdjustButtonLocationsToFit  height[" + Height + "]  Width[" + Width + "].");

      SuspendLayout ();
      optionsPanel.Height = Height - 1;
      optionsPanel.Width  = Width  - 1;
      
      int  spaceUsedByCaption = optionsPanel.Font.Height;

      int  firstRow = spaceUsedByCaption + 8;
      int  lastRow  = optionsPanel.Height - 8;
      int  availableRows = lastRow - firstRow + 1;
      int  availableCols = optionsPanel.Width;
      
      int  col = 6;

      if  (options.Length == 1)
      {
        options[0].Top = (int)(0.5f + (float)availableRows / 2.0f);
      }
      else
      {
        int  maxOptionsInACol = (int)Math.Ceiling ((float)options.Length / (float)numColumns);

        int interOptionsSpace = 0;
        if  (maxOptionsInACol > 1)
          interOptionsSpace = (availableRows - (maxOptionsInACol * options[0].Height)) / (maxOptionsInACol - 1);
        
        int  row = firstRow;
        int  optionsInThisCol = 0;
        int  maxColUsed = col;
        
        for (int x = 0; x < options.Length; x++)
        {
          options[x].Top  = row;
          options[x].Left = col;
          
          int  rightMostPixelUsed = col + options[x].Width;
          if  (rightMostPixelUsed > maxColUsed)
            maxColUsed = rightMostPixelUsed;
            
          optionsInThisCol++;
          row = row + options[x].Height + interOptionsSpace;
          
          if  (optionsInThisCol >= maxOptionsInACol)
          {
            optionsInThisCol = 0;
            col = maxColUsed + 10;
            row = firstRow;
          }
          
          LogMsgMiddle ("Button[" + options[x].Text + "]  Row[" + row.ToString () + "].");
        }
      }

      ResumeLayout ();
      
      LogMsgExiting ("\nAdjustButtonLocationsToFit    Exiting");
    }  /* AdjustBuutonLocationsToFit */




    // Declare the event, which is associated with our
    // delegate SelectedOptionChanged(). Add some attributes
    // for the Visual C# control property.
    [Category ("Action")]
    [Description ("Fires when the Selected option changes.")]
    public event  SelectedOptionChangedHandler  SelectedOptionChanged;


    protected virtual void OnSelectedOptionChanged ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      if  ((SelectedOptionChanged != null)  &&  (suspendLevel < 1))
      {
        SelectedOptionChanged ();  // Notify Subscribers
      }
    }



    private  void  BuildRadioButtonList (string[]  _options)
    {
      LogMsgEntering ("\n\n\n" +
              "BuildRadioButtonList  *** Enetering ***" );
              
      SuspendLayout ();
        
      if  (options != null)
        RemoveExistingRadioButtins ();

      LogMsgMiddle ("_options.Length = " + _options.Length.ToString ());

      options = new RadioButton[_options.Length];
      
      int  x;

      float interOptionSpace = (float)optionsPanel.Height / (float)_options.Length;
      int  offset = (int)(0.5f + interOptionSpace / 2.0f);
      for (x = 0; x < _options.Length; x++)
      {
        LogMsgMiddle ("Creating Button["  + _options[x] + "]");
      
        RadioButton b = new RadioButton ();
        b.AutoSize = true;
        b.Location = new System.Drawing.Point (6, offset + (int)(0.5f + (float)x * interOptionSpace));
        b.Name = this.Name + "_" + x.ToString ();
        //b.Size = new System.Drawing.Size (34, 16);
        b.TabIndex = x;
        b.Text = _options[x];
        b.TextAlign = ContentAlignment.MiddleLeft;
        b.UseVisualStyleBackColor = true;
        b.Checked = (x == selectedOptionInt);
        b.Font = optionsFont;
        b.CheckedChanged += new System.EventHandler (this.ButtenCheckStatusChanged);
        options[x] = b;
      }

      optionsPanel.Controls.AddRange (options);

      //AdjustButtonLocationsToFit ();

      ResumeLayout ();

      LogMsgExiting ("\n\n\nBuildRadioButtonList");
    }  /* BuildRadioButtonList */
    


    private  void  RemoveExistingRadioButtins ()
    {
      LogMsgEntering ("RemoveExistingRadioButtins");
      
      for  (int x = 0;  x < options.Length;  x++)
      {
        optionsPanel.Controls.Remove (options[x]);
        options[x].Dispose ();
        options[x] = null;
      }
      
      options = null;
      
      LogMsgExiting ("RemoveExistingRadioButtins");
    } /* RemoveExistingRadioButtins */




    private int  LookUpOption (string  s)
    {
      int x;
      for (x = 0; x < options.Length; x++)
      {
        if  (s.Equals (options[x].Text, StringComparison.OrdinalIgnoreCase))
          return x;
      }

      return -1;
    }  /* LookUpOption */




    private int LookUpOption (RadioButton button)
    {
      int  x;
      for  (x = 0;  x < options.Length;  x++)
      {
        if (button == options[x])
          return  x;
      }
      
      return -1;
    }  /* LookUpOption */
    


    
    
    private void ButtenCheckStatusChanged (object sender, EventArgs e)
    {
      LogMsgEntering ("ButtenCheckStatusChanged");
      RadioButton  b = (RadioButton)sender;
      if  (b.Checked)
      {
        selectedOptionInt = LookUpOption (b);
        if  (selectedOptionInt < 0)
          selectedOptionInt = 0;
        
          OnSelectedOptionChanged ();
      }
      LogMsgExiting ("ButtenCheckStatusChanged   Exiting");
    }  /* ButtenCheckStatusChanged */




    private void RadioButtonList_Layout (object sender, LayoutEventArgs e)
    {
      LogMsgEntering ("RadioButtonList_Layout");
      AdjustButtonLocationsToFit ();
      Invalidate ();
      LogMsgExiting ("RadioButtonList_Layout");
    }

  }
}
