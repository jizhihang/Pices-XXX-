using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace SipperInterface
{
	/// <summary>
	/// Summary description for SipperDescription.
	/// </summary>
	public class SipperDescription : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox tb_title;
		private System.Windows.Forms.RichTextBox rt_Description;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button bt_Save;
        private System.ComponentModel.Container components = null;

		private string 
			file_name="",
			file_description="";
		
		private bool set_data = false;

		public string File_Name ()
		{
			return file_name;
		}

		public string File_Description ()
		{
			return file_description;
		}

		public bool Set_Data ()
		{
			return set_data;
		}

		public SipperDescription()
		{
			InitializeComponent();
		}

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
      this.tb_title = new System.Windows.Forms.TextBox();
      this.rt_Description = new System.Windows.Forms.RichTextBox();
      this.label1 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.bt_Save = new System.Windows.Forms.Button();
      this.SuspendLayout();
      // 
      // tb_title
      // 
      this.tb_title.Location = new System.Drawing.Point(8, 32);
      this.tb_title.MaxLength = 20;
      this.tb_title.Name = "tb_title";
      this.tb_title.Size = new System.Drawing.Size(672, 20);
      this.tb_title.TabIndex = 0;
      this.tb_title.Text = "";
      // 
      // rt_Description
      // 
      this.rt_Description.Location = new System.Drawing.Point(8, 80);
      this.rt_Description.MaxLength = 450;
      this.rt_Description.Name = "rt_Description";
      this.rt_Description.Size = new System.Drawing.Size(672, 176);
      this.rt_Description.TabIndex = 1;
      this.rt_Description.Text = "";
      // 
      // label1
      // 
      this.label1.Location = new System.Drawing.Point(8, 9);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(168, 20);
      this.label1.TabIndex = 2;
      this.label1.Text = "Sipper File Name";
      this.label1.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      this.label1.Click += new System.EventHandler(this.label1_Click);
      // 
      // label2
      // 
      this.label2.Location = new System.Drawing.Point(8, 56);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(128, 23);
      this.label2.TabIndex = 3;
      this.label2.Text = "Sipper File Description";
      this.label2.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
      // 
      // bt_Save
      // 
      this.bt_Save.Location = new System.Drawing.Point(256, 272);
      this.bt_Save.Name = "bt_Save";
      this.bt_Save.Size = new System.Drawing.Size(152, 32);
      this.bt_Save.TabIndex = 4;
      this.bt_Save.Text = "Save";
      this.bt_Save.Click += new System.EventHandler(this.bt_Save_Click);
      // 
      // SipperDescription
      // 
      this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
      this.ClientSize = new System.Drawing.Size(688, 318);
      this.Controls.Add(this.bt_Save);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.rt_Description);
      this.Controls.Add(this.tb_title);
      this.Name = "SipperDescription";
      this.Text = "SipperDescription";
      this.Load += new System.EventHandler(this.SipperDescription_Load);
      this.ResumeLayout(false);

    }
		#endregion

		private void bt_Save_Click(object sender, System.EventArgs e)
		{
			//tb_title.Text = tb_title.Text.Replace (" ", "");
			Match m = Regex.Match(tb_title.Text, @"^[a-zA-Z0-9](\w|-){0,16}$");

			if (m.Success)
			{
				file_name = tb_title.Text;
				file_description = rt_Description.Text;
				set_data = true;
				this.Hide ();
			}
			else
				MessageBox.Show ("Please check File Title for illegal characters.  Only Alphanumberics are allowed.");
		}

		private void SipperDescription_Load(object sender, System.EventArgs e)
		{
			tb_title.Text=Form1.FILE_TITLE;
			rt_Description.Text=Form1.FILE_DESCRIPTION;		
		}

    private void label1_Click(object sender, System.EventArgs e)
    {
    
    }
	}
}
