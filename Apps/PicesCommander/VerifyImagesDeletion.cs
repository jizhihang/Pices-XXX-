using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;


using  PicesInterface;
using  SipperFile;


namespace PicesCommander
{
  public partial class VerifyImagesDeletion : Form
  {
    private  int  thumbNailHeight = 180;
    private  int  thumbNailWidth  = 150;
    private  int  thumbnailCurRow = 0;
    private  int  thumbnailCurCol = 0;
    private  bool deletionOk = false;
    private  int   thumbnailGridViewLastHeight = 0;
    private  int   thumbnailGridViewLastWidth  = 0;

    private  int   lastHeight      = 0;
    private  int   lastWidth       = 0;
    private  bool  formIsMaximized = false;


    private  PicesDataBaseImageList  images = null;



    public VerifyImagesDeletion ()
    {
      InitializeComponent ();
      lastHeight = Height;
      lastWidth  = Width;
      thumbnailGridViewLastHeight = ThumbnailDataGridView.Height;
      thumbnailGridViewLastWidth  = ThumbnailDataGridView.Width;
    }



    public  void  Images (PicesDataBaseImageList  _images)
    {
      images = _images;
    }


    public  bool  DeletionOk ()
    {
      return deletionOk;
    }


    
    private void VerifyImagesDeletion_Load (object sender, EventArgs e)
    {
      DefineThumbnailGridViewRowAndColumns ();
      AddToThumbNailPanel ();
    }


    static
    public  DialogResult  Show (IWin32Window            parent,
                                PicesDataBaseImageList  images
                               )
    {
      VerifyImagesDeletion vi = new VerifyImagesDeletion ();
      vi.Images (images);

      vi.ShowDialog (parent);
      if  (vi.DeletionOk ())
        return DialogResult.Yes;
      else
        return DialogResult.No;
    }


    private  void  DefineThumbnailGridViewRowAndColumns ()
    {
      int  width  = ThumbnailDataGridView.Width;
      int  height = ThumbnailDataGridView.Height;

      int  deltaHeight = height - thumbnailGridViewLastHeight;
      int  deltaWidth  = width  - thumbnailGridViewLastWidth;

      thumbnailGridViewLastHeight = ThumbnailDataGridView.Height;
      thumbnailGridViewLastWidth  = ThumbnailDataGridView.Width;

      int  requiredHeight = thumbNailHeight;
      int  requiredWidth  = thumbNailWidth;

      int  numCols = (int)Math.Floor ((float)(width - 15)  / (float)requiredWidth);
      if  (numCols < 1)  
        numCols = 1;

      int  numRows = (int)Math.Floor ((float)height / (float)requiredHeight);
      if  (numRows < 1)
        numRows = 1;

      ThumbnailDataGridView.Rows.Clear ();
      ThumbnailDataGridView.Columns.Clear ();
      //ThumbnailDataGridView.ColumnCount = numCols;

      for  (int colIDX = 0;  colIDX < numCols;  colIDX++)
      {
        //DataGridViewImageColumn  dgvic = new DataGridViewImageColumn ();
        ThumbNailImageColumn  dgvic = new ThumbNailImageColumn (this);
        
        dgvic.Name = "ThumbCol" + colIDX;
        dgvic.HeaderText = "";
        dgvic.Width = thumbNailWidth;
        ThumbnailDataGridView.Columns.Add (dgvic);
      }

      {
        // Define Row Info
        DataGridViewCellStyle  dgvcs = new DataGridViewCellStyle ();
        dgvcs.Alignment = DataGridViewContentAlignment.MiddleCenter;
        ThumbnailDataGridView.RowsDefaultCellStyle = dgvcs;
        ThumbnailDataGridView.RowCount = 0;
      }
    }  /* DefineThumbnailGridViewRowAndColumns */




    private void  AddToThumbNailPanel ()
    {
      if  (images == null)
        return;

      int  numCols = ThumbnailDataGridView.Columns.Count;

      thumbnailCurRow = ThumbnailDataGridView.Rows.Count - 1;
      if  (ThumbnailDataGridView.Rows.Count == 0)
        thumbnailCurCol = numCols;

      int  idx = 0;
      while  (idx < images.Count)
      {
        PicesDataBaseImage  i = images[idx];
         
        if  (thumbnailCurCol >= numCols)
        {
          object[] row = new object[numCols];
          ThumbnailDataGridView.Rows.Add (row);
          thumbnailCurRow = ThumbnailDataGridView.Rows.Count - 1;
          thumbnailCurCol = 0;
          ThumbnailDataGridView.Rows[thumbnailCurRow].Height = thumbNailHeight + 2;
        }
      
        ThumbnailDataGridView.Rows[thumbnailCurRow].Cells[thumbnailCurCol].Value = i;
        thumbnailCurCol++;
        idx++;
      }
    }  /* AddToThumbNailPanel */



    private void DeleteButton_Click (object sender, EventArgs e)
    {
      if  (YesToConfirm.Text.Equals ("YES", StringComparison.InvariantCultureIgnoreCase))
      {
        deletionOk = true;
        this.Close ();
      }
      else
      {
        MessageBox.Show (this, "You must enter \"Yes\" above to confirm that you want to delete images.", "Delete Images");
      }
    }

    private void CancelButton_Click (object sender, EventArgs e)
    {
      deletionOk = false;
      Close ();
    }

    private void ThumbnailDataGridView_CellFormatting (object sender, DataGridViewCellFormattingEventArgs e)
    {
      return;
    }

    private void ThumbnailDataGridView_DataError (object sender, DataGridViewDataErrorEventArgs e)
    {
      int  colIdx = e.ColumnIndex;
      int  rowIdx = e.RowIndex;
    }


    private  void  VerifyImagesDeletionFormResized ()
    {
      if  (lastHeight == 0)
        return;

      int  deltaHeight = Height - lastHeight;
      int  deltaWidth  = Width  - lastWidth;

      lastHeight = Height;
      lastWidth  = Width;

      ThumbnailDataGridView.Height  += deltaHeight;
      ThumbnailDataGridView.Width   += deltaWidth;

      DeleteButton.Top  += deltaHeight;
      CancelButton.Top  += deltaHeight;
      CancelButton.Left += deltaWidth;

      DefineThumbnailGridViewRowAndColumns ();
      AddToThumbNailPanel ();
    }  /* VerifyImagesDeletionFormResized */




    private void VerifyImagesDeletion_SizeChanged (object sender, EventArgs e)
    {
      if  (WindowState == FormWindowState.Maximized)
      {
        // Looks like user has pressed the Maximized button.  We have to trap it here because
        // the ResizeEnd envent does not trap when form is Maximized.
        VerifyImagesDeletionFormResized ();
        formIsMaximized = true;
      }
      else if  (WindowState == FormWindowState.Normal)
      {
        if  (formIsMaximized)
        {
          // We normally trap the ResizeEnd event;  but when the form was already maximized and the user
          // presses the button to unmaximize.  the ResizeEnd does not trap that.  So we check to 
          // see if the form was already maximize.  If so then we resized the form.
          VerifyImagesDeletionFormResized ();
          formIsMaximized = false;
        }
      }
    }

    private void VerifyImagesDeletion_ResizeEnd (object sender, EventArgs e)
    {
      VerifyImagesDeletionFormResized ();
    } 
  }
}
