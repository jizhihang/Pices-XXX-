using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

using  PicesInterface;

namespace SipperFile
{
  public partial class DisplayPicesImages : Form
  {
    private  String                dir;
    private  String                dir2 = null;
    private  ProbNamePair[]        nameList = null;

    private  int                   lastImageIndexLoaded = 0;
    private  FileInfo[]            files = null;
    private  List<FlowLayoutPanel> thumbNails = null;

    private  int                   lastHeight = 0;
    private  int                   lastWidth  = 0;

    private  String[]              svNames = null;



    public DisplayPicesImages (String  _dir)
    {
      InitializeComponent ();
      
      dir = OSservices.AddSlash (_dir);
      Text = dir;

      thumbNails = new List<FlowLayoutPanel> ();
      
      DirectoryInfo di = null;
      try  {di = new DirectoryInfo (dir);}
      catch (Exception)  {di = null;}
      if  (di == null)
        return;

      try  {files = di.GetFiles ("*.bmp");}  catch (Exception)  {files = null;}
      if  (files != null)
        UpdateDisplayTimer.Enabled = true;
    }


    public DisplayPicesImages (String          _dir,
                               String          _dir2,
                               ProbNamePair[]  _nameList
                              )
    {
      InitializeComponent ();
      
      dir  = OSservices.AddSlash (_dir);
      dir2 = OSservices.AddSlash (_dir2);

      nameList      = _nameList;

      Text = dir;

      if  (!String.IsNullOrEmpty (dir)  &&  !String.IsNullOrEmpty (dir2))
        Text = OSservices.GetRootNameOfDirectory (dir) + " vs " + OSservices.GetRootNameOfDirectory (dir2);

      thumbNails = new List<FlowLayoutPanel> ();
      
      DirectoryInfo di = null;
      try  {di = new DirectoryInfo (dir);}
      catch (Exception)  {di = null;}
      if  (di == null)
        return;

      try  {files = di.GetFiles ("*.bmp");}  catch (Exception)  {files = null;}
      if  (files != null)
        UpdateDisplayTimer.Enabled = true;
    }


    /*! 
     *\brief  Specify the root file names of S/V's.  They will be highlighted when displayed.
     */
    public  void  SummportVectorNames (String[]  _svNames)
    {
      svNames = _svNames;
    }


    private  bool  IsIt_A_SV (String  n)
    {
      if  ((svNames == null)  ||  (n == null))
        return  false;
      foreach  (String  s in svNames)
      {
        if  (s.CompareTo (n) == 0)
          return true;
      }
      return  false;
    }


    public bool GetThumbnailImageAbort()
    {
      return true;
    }  /* GetThumbnailImageAbort */

    private  System.IntPtr  thumbNailCallBackData = IntPtr.Zero;




    private  void  LoadNextImageFromDir ()
    {
      if  (files == null)
        return;

      if  (lastImageIndexLoaded >= files.Length)
      {
        UpdateDisplayTimer.Enabled = false;
        return;
      }

      Font font = new System.Drawing.Font("Courier New", 8.0f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

      Bitmap  image = null;
      
      FileInfo fi = files[lastImageIndexLoaded];
      lastImageIndexLoaded++;

      try  {image = new Bitmap (fi.FullName);}
      catch (Exception) {image = null;}
      if  (image == null)
         return;

      float  ratio = 1.0f;
      int  maxDim = Math.Max (image.Height, image.Width);
      if  (maxDim > 150)
         ratio = 150.0f / (float)maxDim;

      PictureBox pb = new PictureBox ();
      pb.BorderStyle = BorderStyle.FixedSingle;

      int h = (int)((float)image.Height * ratio);
      int w = (int)((float)image.Width  * ratio);

      pb.Height = h + 2;
      pb.Width  = w + 2;
      pb.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(ThumbNail_MouseDoubleClick);
      pb.Name = fi.FullName;

      Image thumbNail = image.GetThumbnailImage (w, h, GetThumbnailImageAbort, thumbNailCallBackData);
      pb.Image = thumbNail;

      FlowLayoutPanel pan = new FlowLayoutPanel ();
      pan.Size = new Size (Math.Max (170, w + 10), 200);
      //pan.Height = h + 80;
      //pan.Width  = Math.Max (160, w + 10);

      String  rootName = OSservices.GetRootName (fi.Name);
      
      pan.Controls.Add (pb);
      TextBox tb = new TextBox ();
      tb.Width = pan.Width - 8;
      tb.Text = rootName;
      tb.Font = font;

      pan.Controls.Add (tb);

      Button b = new Button ();
      b.Text = "Delete";
      b.Name = rootName;
      b.Enabled = true;
      b.TabStop = false;
      b.Click += new EventHandler (RemoveImageFromTrainigLibray);
      pan.Controls.Add (b);

      pan.BorderStyle = BorderStyle.FixedSingle;

      if  (IsIt_A_SV (rootName))
        pan.BackColor = Color.Gold;
      else
        pan.BackColor = Color.White;
      
      thumbNails.Add (pan);
      ImageDisplayPanel.Controls.Add (pan);
    }  /* LoadNextImageFromDir */



    private  String  LocateRootName (String  rootName)
    {
      String  subDir = null;
      if  (!String.IsNullOrEmpty (dir))
        subDir = OSservices.LookForFile (rootName + ".bmp", dir);

      if  (String.IsNullOrEmpty (subDir))
      {
        if  (!String.IsNullOrEmpty (dir2))
        {
          subDir = OSservices.LookForFile (rootName + ".bmp", dir2);
          if  (subDir == null)
            return  null;
        }
      } 

      return  OSservices.AddSlash (subDir) + rootName + ".bmp";
    }  /* LocateRootName */


    private  void  LoadNextImageFromNameList ()
    {
      if  (nameList == null)
        return;

      if  (lastImageIndexLoaded >= nameList.Length)
      {
        UpdateDisplayTimer.Enabled = false;
        return;
      }

      String  rootName    = nameList[lastImageIndexLoaded].name;
      double  probability = nameList[lastImageIndexLoaded].probability;

      lastImageIndexLoaded++;

      String  fullName = LocateRootName (rootName);
      if (fullName == null)
        return;

      String  subDir = OSservices.GetPathPartOfFile (fullName);

      FileInfo fi = new FileInfo (fullName);

      Bitmap  image = null;

      try  {image = new Bitmap (fi.FullName);}
      catch (Exception) {image = null;}
      if  (image == null)
         return;

      float  ratio  = 1.0f;
      int  maxDim = Math.Max (image.Height, image.Width);
      if  (maxDim > 150)
        ratio = 150.0f / (float)maxDim;

      PictureBox pb = new PictureBox ();
      pb.BorderStyle = BorderStyle.FixedSingle;

      int h = (int)((float)image.Height * ratio);
      int w = (int)((float)image.Width  * ratio);

      Font font = new System.Drawing.Font("Courier New", 8.0f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

      pb.Height = h + 2;
      pb.Width  = w + 2;
      pb.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(ThumbNail_MouseDoubleClick);
      pb.Name = fi.FullName;

      //ContextMenuStrip cms = new ContextMenuStrip ();
      //String  menuItemStr = "Remove " + rootName + " From Training Library";
      //cms.Items.Add (menuItemStr, null, RemoveImageFromTrainigLibray);
      //pb.ContextMenuStrip = cms;

      Image thumbNail = image.GetThumbnailImage (w, h, GetThumbnailImageAbort, thumbNailCallBackData);
      pb.Image = thumbNail;

      FlowLayoutPanel pan = new FlowLayoutPanel ();
      pan.Size = new Size (Math.Max (170, w + 10), 260);
      //pan.Height = h + 80;
      //pan.Width  = Math.Max (160, w + 10);

      pan.Controls.Add (pb);
      TextBox tb = new TextBox ();
      tb.Width = pan.Width - 8;
      tb.Text = rootName;
      tb.Font = font;

      pan.Controls.Add (tb);

      tb = new TextBox ();
      tb.Width = pan.Width - 8;
      tb.Text = OSservices.GetRootNameOfDirectory (subDir);
      tb.Font = font;
      pan.Controls.Add (tb);

        tb = new TextBox ();
        tb.Width = pan.Width - 8;
        tb.Text  = probability.ToString ("p2");
        tb.Font  = font;
        pan.Controls.Add (tb);
      
      Button b = new Button ();
      b.Text = "Delete";
      b.Name = rootName;
      b.Enabled = true;
      b.TabStop = false;
      b.Click += new EventHandler (RemoveImageFromTrainigLibray);
      pan.Controls.Add (b);

      pan.BorderStyle = BorderStyle.FixedSingle;

      thumbNails.Add (pan);

      ImageDisplayPanel.Controls.Add (pan);
    }  /* LoadNextImageFromDir */


    void  RemoveImageFromTrainigLibray (Object sender, EventArgs e)
    {
      if  (sender == null)
        return;
      Button  b = (Button)sender;
      String  rootName = b.Name;

      String  fullName = LocateRootName (rootName);
      if (fullName == null)
        return;

      DialogResult  dr = MessageBox.Show (this, "Delete '" + rootName + "' from Training Library.", "Remove Image", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        b.Enabled = false;
        File.Delete (fullName);
        if  (File.Exists (fullName))
        {
          MessageBox.Show (this, "Failed to delete '" + rootName + "' from Training Library.", "Remove Image");
        }
        else
        {
          MessageBox.Show (this, "'" + rootName + "' Removed form Training Library.", "Remove Image");
        }
      }
    }  /* RemoveImageFromTrainigLibray */



    private  void  ThumbNail_MouseDoubleClick (object sender, MouseEventArgs e)
    {
      if  (e.Button == MouseButtons.Left)
      {
        PictureBox pb = (PictureBox)sender;
        String  imageFileName = pb.Name;
        PicesRaster  pr = new PicesRaster (imageFileName);
        ImageEditor  ie = new ImageEditor (pr);
        ie.ShowDialog (this);
      }
    }


    private void  UpdateDisplayTimer_Tick (object sender, EventArgs e)
    {
      if  (nameList != null)
        LoadNextImageFromNameList ();
      else
        LoadNextImageFromDir ();
    }


    private void DisplayPicesImages_Load(object sender, EventArgs e)
    {
      lastHeight = Height;
      lastWidth  = Width;
    }



    private void DisplayPicesImages_ResizeEnd(object sender, EventArgs e)
    {
      if  (lastHeight == 0)
        return;

      int  deltaWidth  = Width  - lastWidth;
      int  deltaHeight = Height - lastHeight;

      if  ((deltaHeight == 0)  &&  (deltaWidth == 0))
        return;

      ImageDisplayPanel.Height += deltaHeight;
      ImageDisplayPanel.Width  += deltaWidth;

      ImageDisplayPanel.Controls.Clear ();

      foreach  (FlowLayoutPanel  flp  in  thumbNails)
      {
        ImageDisplayPanel.Controls.Add (flp);
      }

      ImageDisplayPanel.AutoScroll = true;
      ImageDisplayPanel.PerformLayout ();
    } 
  }
}
