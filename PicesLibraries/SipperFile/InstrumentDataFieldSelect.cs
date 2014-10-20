using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SipperFile
{
  public partial class InstrumentDataFieldSelect : Form
  {
  
    private  string        selectedDataField = "";
    private  List<String>  dataFieldNames    = null;
  
  
    public InstrumentDataFieldSelect (List<String>  _dataFieldNames,
                                      String        _selectedDataField
                                     )
    {
      InitializeComponent ();
      dataFieldNames    = _dataFieldNames;
      selectedDataField = _selectedDataField;
      BuildDataFieldList ();
    }


    public  string  SelectedDataField
    {
      get {return selectedDataField;}
      set {selectedDataField = value;}
    }



    private void  BuildDataFieldList ()
    {
      int  origWindowHeight        = this.Height;
      int  origDataFieldListHeight = DataFieldList.Height;
    
      int  origWindowWidth         = this.Width;
      int  origDataFieldListWidth  = DataFieldList.Width;
    
      foreach  (String fn in dataFieldNames)
      {
        DataFieldList.Items.Add (fn);
      }
      DataFieldList.SelectedItem = selectedDataField;

      int  totalHeightNeeded = InstrumentData.NumOfFields () * DataFieldList.ItemHeight + 30;
      DataFieldList.Height   = totalHeightNeeded;
      
      int  newWindowHeight = DataFieldList.Height + (origWindowHeight - origDataFieldListHeight);
      int  newWindowWidth  = DataFieldList.Width  + (origWindowWidth  - origDataFieldListWidth);

      this.Height = newWindowHeight;
      this.Width  = newWindowWidth;
    }  /* BuildDataFieldList */
     

    private void DataFieldList_KeyPress(object sender, KeyPressEventArgs e)
    {
      if  (e.KeyChar == '\r')
      {
        // A data item has been selected.
        selectedDataField = (string)DataFieldList.SelectedItem;
        this.Close ();
      }
    }

    private void DataFieldList_MouseClick(object sender, MouseEventArgs e)
    {
      // A data item has been selected.
      selectedDataField = (string)DataFieldList.SelectedItem;
      this.Close ();
    }

  }  /* InstrumentDataFieldSelect */
}