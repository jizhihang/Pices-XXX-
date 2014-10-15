using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

// TODO When no data is recorded and the preview function is pressed. The Sipper Decoder program blows up trying to create an image  with 0 rows.

namespace SipperInterface
{
	/// <summary>
	/// SipperImage was taked from Carmen Neustaedter's code and modified to suit the need of this program
	/// http://pages.cpsc.ucalgary.ca/~carman/481/examples/csharp/radaroverview.html
	/// </summary>
	public class SipperImage : System.Windows.Forms.Form
	{
		private System.Windows.Forms.PictureBox largePic;
		private System.Windows.Forms.PictureBox smallPic;
		private System.Windows.Forms.VScrollBar vBar;
		private System.Windows.Forms.HScrollBar hBar;

		private int w; // width
		private int h; // height
		private int scaleFactor; // proportional size of the radar
    private string  previewFileName;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;



		public SipperImage(string  _previewFileName)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
      previewFileName = _previewFileName;
		}


		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.largePic = new System.Windows.Forms.PictureBox();
			this.smallPic = new System.Windows.Forms.PictureBox();
			this.vBar = new System.Windows.Forms.VScrollBar();
			this.hBar = new System.Windows.Forms.HScrollBar();
			this.SuspendLayout();
			// 
			// largePic
			// 
			this.largePic.Location = new System.Drawing.Point(240, 24);
			this.largePic.Name = "largePic";
			this.largePic.Size = new System.Drawing.Size(888, 624);
			this.largePic.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.largePic.TabIndex = 0;
			this.largePic.TabStop = false;
			// 
			// smallPic
			// 
			this.smallPic.Location = new System.Drawing.Point(24, 24);
			this.smallPic.Name = "smallPic";
			this.smallPic.Size = new System.Drawing.Size(216, 160);
			this.smallPic.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.smallPic.TabIndex = 1;
			this.smallPic.TabStop = false;
			this.smallPic.Paint += new System.Windows.Forms.PaintEventHandler(this.smallPic_Paint);
			// 
			// vBar
			// 
			this.vBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.vBar.Location = new System.Drawing.Point(1128, 0);
			this.vBar.Name = "vBar";
			this.vBar.Size = new System.Drawing.Size(24, 656);
			this.vBar.TabIndex = 4;
			this.vBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vBar_Scroll);
			// 
			// hBar
			// 
			this.hBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.hBar.Location = new System.Drawing.Point(0, 648);
			this.hBar.Name = "hBar";
			this.hBar.Size = new System.Drawing.Size(1128, 24);
			this.hBar.TabIndex = 5;
			this.hBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hBar_Scroll);
			// 
			// SipperImage
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(1152, 670);
			this.Controls.Add(this.hBar);
			this.Controls.Add(this.vBar);
			this.Controls.Add(this.smallPic);
			this.Controls.Add(this.largePic);
			this.Name = "SipperImage";
			this.Text = "SipperImage";
			this.Load += new System.EventHandler(this.SipperImage_Load);
			this.ResumeLayout(false);

		}
		#endregion

		private void button1_Click(object sender, System.EventArgs e)
		{
			this.Close ();
		}

		private void SipperImage_Load(object sender, System.EventArgs e)
		{
			string msg;

			try 
			{
				//string path = "TestFrame.jpg";
				string path = previewFileName;

				try
				{
					largePic.Image = new Bitmap(path);
				}
				catch (Exception loadException)
				{
					msg = loadException.Message;
					MessageBox.Show ("Exception occured loading image[" + msg + "]");
					this.Close ();
				}

				smallPic.Image = new Bitmap (path);

				// set the scrollable area
				hBar.Maximum = largePic.Width;
				vBar.Maximum = largePic.Height;

				// set how much smaller we want the radar to be
				scaleFactor = 8;

				// the radar view will be exactly one quarter the size of the window
				smallPic.Width = largePic.Width / scaleFactor;
				smallPic.Height = largePic.Height / scaleFactor;

				// calculate the width and height of the viewable area
				w = this.Width / scaleFactor; 
				h = this.Height / scaleFactor;

				// position the radar in the top left corner of the window
				smallPic.Left = 0;
				smallPic.Top = 0;
			}
			catch (Exception)
			{}
		}

		private void hBar_Scroll(object sender, System.Windows.Forms.ScrollEventArgs e)
		{
			// move the large picture according to the scrollbar
			largePic.Left = -hBar.Value;			
			// repaint the radar
			this.smallPic.Refresh();
		}

		private void vBar_Scroll(object sender, System.Windows.Forms.ScrollEventArgs e)
		{
			// move the large picture according to the scrollbar
			largePic.Top = -vBar.Value;			
			// repaint the radar
			this.smallPic.Refresh();
		}
	
		private void smallPic_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
			// a pen for drawing
			Pen p = new Pen(Color.Red,2);
			// calculate the x, y position of the top left corner of the red box
			int x = hBar.Value / scaleFactor;
			int y = vBar.Value / scaleFactor;
			// draw a rectangle in the graphics object belonging to our small picture box
			e.Graphics.DrawRectangle(p, x, y, w, h);
		}

	}
}
