using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;


//using PicesInterface;

namespace FlatFieldSipper4Image
{
  public partial class Sipper4ImageViewer : Form
  {
    String  lastDir = "";
    Image  curImage = null;

    public Sipper4ImageViewer ()
    {
      InitializeComponent ();
    }

    private void LoadImage_Click (object sender, EventArgs e)
    {
      int xxx = 100;
      OpenFileDialog  ofd = new OpenFileDialog ();
      if  (!String.IsNullOrEmpty (lastDir))
        ofd.InitialDirectory = lastDir;
      ofd.CheckFileExists = true;
      DialogResult dr =  ofd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        //lastDir = OSservices.GetPathPartOfFile (ofd.FileName);

        try  {curImage = Image.FromFile (ofd.FileName);}
        catch  (Exception)  {curImage = null;}

        if  (curImage != null)
        {
          Sipper4Image.Height = curImage.Height;
          Sipper4Image.Width  = curImage.Width;
          Sipper4Image.Image  = curImage;
          PopulateIntensityHistogram (curImage);
        }
      }
    }



    private  void  PopulateIntensityHistogram (Image image)
    {
      Bitmap b = new Bitmap (image);

      int[]  histogram = new int[256];

      Color  pv;

      for  (int row = 0;  row < b.Height;  ++row)
      {
        for  (int col = 0;  col < b.Width;  ++col)
        {
          pv = b.GetPixel (col, row);
          int  intensity = (int)((pv.R + pv.G + pv.B) / 3);
          if  (intensity < 0)
            intensity = 0;

          else if  (intensity > 255)
            intensity = 255;

          histogram[intensity] += 1;
        }
      }

      IntensityHistogram.Rows.Clear ();
      for  (int i = 0;  i < 256;  ++i)
      {
        if  (histogram[i] > 0)
        {
          Object[] cols = new Object[2];
          String s = i.ToString ("##0");
          cols[0] = s;

          String t = histogram[i].ToString ("#,###,##0");
          cols[1] = t;
          IntensityHistogram.Rows.Add (cols);
        }
      }

      return;
    }


    private  int  FindHighPixelValue (Bitmap b, 
                                      int    col
                                     )
    {
      int[]  counter = new int[256];
      for  (int x = 0;  x < 256;  ++x)
        counter[x] = 0;

      for  (int row = 0;  row < b.Height;  ++row)
      {
        Color c = b.GetPixel (col, row);
        int pv = (c.R + c.G + c.B) / 3;
        if  (pv > 255)  pv = 255;
        counter[pv]++;
      }

      float  th = 0.25f;
      int  hv = 255;
      int  totalPixsInTop = counter[hv];
      float  pecentPicsInTop = (float)totalPixsInTop / (float)b.Height;
      while  ((hv > 0)  &&  (pecentPicsInTop < th))
      {
        --hv;
        totalPixsInTop += counter[hv];
        pecentPicsInTop = (float)totalPixsInTop / (float)b.Height;
      }
      return hv;
    }  /* FindHighPixelValue */


    private  Image FlatFieldImage (Image i)
    {
      if  (i == null)
        return null;
      Bitmap b = new Bitmap (i);

      Bitmap  result = new Bitmap (b.Width, b.Height);

      for  (int col = 0;  col < b.Width;  ++col)
      {
        int  hv = FindHighPixelValue (b, col);
        for  (int  row = 0;  row < b.Height;  ++row)
        {
          Color c = b.GetPixel (col, row);
          int pv = (c.R + c.G + c.B) / 3;

          int  newPv = 255;
          if  (pv < hv)
            newPv = (int)(0.5f + ((float)pv / (float)hv) * 255.0f);

          if  (newPv <= 33)
            newPv = 0;

          result.SetPixel (col, row, Color.FromArgb (c.A, newPv, newPv, newPv));
        }
      }
      return  result;
    }  /* FlatFieldImage */



    private void FlatFieldButon_Click (object sender, EventArgs e)
    {
      curImage = FlatFieldImage (Sipper4Image.Image);
      Sipper4Image.Image = curImage;

      PopulateIntensityHistogram (curImage);
    }


    private void SaveButton_Click (object sender, EventArgs e)
    {
      int xxx = 100;
      SaveFileDialog  sfd = new SaveFileDialog ();
      if  (!String.IsNullOrEmpty (lastDir))
        sfd.InitialDirectory = lastDir;
      sfd.CheckPathExists = true;
      DialogResult dr =  sfd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        //lastDir = OSservices.GetPathPartOfFile (ofd.FileName);

        if  (System.IO.File.Exists (sfd.FileName))
        {
          dr = MessageBox.Show (this, "File already exists; are you sure you want to overwrite ?", "Save Image", MessageBoxButtons.OKCancel);
          if  (dr == DialogResult.Cancel)
            return;
        }

        try  {curImage.Save (sfd.FileName);}
        catch  (Exception e2)  
        {
          MessageBox.Show (this, "Exception saving image" + "\n" + e.ToString (), "Save Image");
        }
      }
    }
  }
}
