namespace SipperSimulator
{
  partial class SipperSimulator
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose (bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose ();
      }
      base.Dispose (disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent ()
    {
      this.components = new System.ComponentModel.Container();
      this.startButon = new System.Windows.Forms.Button();
      this.endButton = new System.Windows.Forms.Button();
      this.conversation = new System.Windows.Forms.RichTextBox();
      this.textBoxUpdateTimer = new System.Windows.Forms.Timer(this.components);
      this.SuspendLayout();
      // 
      // startButon
      // 
      this.startButon.Location = new System.Drawing.Point(20, 463);
      this.startButon.Name = "startButon";
      this.startButon.Size = new System.Drawing.Size(75, 23);
      this.startButon.TabIndex = 1;
      this.startButon.Text = "Start";
      this.startButon.UseVisualStyleBackColor = true;
      this.startButon.Click += new System.EventHandler(this.startButon_Click);
      // 
      // endButton
      // 
      this.endButton.Location = new System.Drawing.Point(20, 492);
      this.endButton.Name = "endButton";
      this.endButton.Size = new System.Drawing.Size(75, 23);
      this.endButton.TabIndex = 2;
      this.endButton.Text = "End";
      this.endButton.UseVisualStyleBackColor = true;
      this.endButton.Click += new System.EventHandler(this.endButton_Click);
      // 
      // conversation
      // 
      this.conversation.HideSelection = false;
      this.conversation.Location = new System.Drawing.Point(20, 12);
      this.conversation.Name = "conversation";
      this.conversation.Size = new System.Drawing.Size(849, 445);
      this.conversation.TabIndex = 3;
      this.conversation.Text = "";
      // 
      // textBoxUpdateTimer
      // 
      this.textBoxUpdateTimer.Enabled = true;
      this.textBoxUpdateTimer.Tick += new System.EventHandler(this.textBoxUpdateTimer_Tick);
      // 
      // SipperSimulator
      // 
      this.AcceptButton = this.startButon;
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
      this.ClientSize = new System.Drawing.Size(881, 532);
      this.Controls.Add(this.conversation);
      this.Controls.Add(this.endButton);
      this.Controls.Add(this.startButon);
      this.Name = "SipperSimulator";
      this.Text = "Form1";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SipperSimulator_FormClosing);
      this.Load += new System.EventHandler(this.Form1_Load);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.Button startButon;
    private System.Windows.Forms.Button endButton;
    private System.Windows.Forms.RichTextBox conversation;
    private System.Windows.Forms.Timer textBoxUpdateTimer;
  }
}

