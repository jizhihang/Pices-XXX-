using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using SipperFile;

using  PicesInterface;

namespace SipperFile
{
  public partial class SelectAInstrumentDataField : Form
  {
    string  selectedDataField = "";
  
  
    public SelectAInstrumentDataField (string  _selectedDataField)
    {
      InitializeComponent ();
      selectedDataField = _selectedDataField;
      BuildDataFieldList ();
    }



    private void  BuildDataFieldList ()
    {
      int  origWindowHeight        = this.Height;
      int  origDataFieldListHeight = DataFieldList.Height;
    
      int  origWindowWidth         = this.Width;
      int  origDataFieldListWidth  = DataFieldList.Width;
      
      int  fieldNum = 0;
      for (fieldNum = 0; fieldNum < PicesInstrumentData.NumFields (); fieldNum++)
      {
        DataFieldList.Items.Add (PicesInstrumentData.FieldName (fieldNum));
      }
      DataFieldList.SelectedItem = selectedDataField;

      int  totalHeightNeeded = PicesInstrumentData.NumFields () * DataFieldList.ItemHeight + 30;
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
    
  }
}