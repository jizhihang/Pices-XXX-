using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using SipperFile;

namespace SipperFileViewer
{
  public partial class SelectAInstrumentDataField : Form
  {
    string  selectedDataField = "";

    bool  includeNonDataFields = false;
  
  
    public SelectAInstrumentDataField (string  _selectedDataField,
                                       bool    _includeNonDataFields
                                      )
    {
      InitializeComponent ();
      selectedDataField = _selectedDataField;
      includeNonDataFields = _includeNonDataFields;
      BuildDataFieldList ();
    }

    
    private void  BuildDataFieldList ()
    {
      int  origWindowHeight        = this.Height;
      int  origDataFieldListHeight = DataFieldList.Height;
    
      int  origWindowWidth         = this.Width;
      int  origDataFieldListWidth  = DataFieldList.Width;
         
      int  numFields = InstrumentData.NumOfFields ();
      if  (!includeNonDataFields)
        numFields = InstrumentData.NumDataFields ();

      int  fieldNum = 0;
      for (fieldNum = 0; fieldNum < numFields; fieldNum++)
      {
        DataFieldList.Items.Add (InstrumentData.FieldName (fieldNum));
      }
      DataFieldList.SelectedItem = selectedDataField;

      int  totalHeightNeeded = InstrumentData.NumOfFields () * DataFieldList.ItemHeight + 30;
      DataFieldList.Height   = totalHeightNeeded;
      
      int  newWindowHeight = DataFieldList.Height + (origWindowHeight - origDataFieldListHeight);
      int  newWindowWidth  = DataFieldList.Width  + (origWindowWidth  - origDataFieldListWidth);

      this.Height = newWindowHeight;
      this.Width  = newWindowWidth;
      
    }


    public  string  SelectedDataField
    {
      get {return selectedDataField;}
      set {selectedDataField = value;}
    }
    


    private void DataFieldList_KeyPress (object sender, KeyPressEventArgs e)
    {
      if  (e.KeyChar == '\r')
      {
        // A data item has been selected.
        selectedDataField = (string)DataFieldList.SelectedItem;
        this.Close ();
      }
    }

    private void DataFieldList_MouseClick (object sender, MouseEventArgs e)
    {
      // A data item has been selected.
      selectedDataField = (string)DataFieldList.SelectedItem;
      this.Close ();
    }  /* DataFieldList_KeyPress */
    


    
      
    /* 
    public static  int     NumDataFields ()  {return  numDataFields;}

    static public  int     NumOfFields   ()  {return fieldNames.Length;}
    
    static public  string  FieldName (int idx) 
      
    */
      
    
    
  }
}