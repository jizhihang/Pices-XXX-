namespace ChartControls
{
  partial class TeminalWindow
  {
    /// <summary> 
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary> 
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Component Designer generated code

    /// <summary> 
    /// Required method for Designer support - do not modify 
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this .components = new System .ComponentModel .Container ();
      this .TerminalScreen = new System .Windows .Forms .Panel ();
      this .StatusBar = new System .Windows .Forms .Panel ();
      this .RefreshTimer = new System .Windows .Forms .Timer (this .components);
      this .SuspendLayout ();
      // 
      // TerminalScreen
      // 
      this .TerminalScreen .BackColor = System .Drawing .Color .White;
      this .TerminalScreen .BorderStyle = System .Windows .Forms .BorderStyle .FixedSingle;
      this .TerminalScreen .Font = new System .Drawing .Font ("Courier New", 9.75F, System .Drawing .FontStyle .Regular, System .Drawing .GraphicsUnit .Point, ((byte)(0)));
      this .TerminalScreen .Location = new System .Drawing .Point (-1, -1);
      this .TerminalScreen .Name = "TerminalScreen";
      this .TerminalScreen .Size = new System .Drawing .Size (780, 533);
      this .TerminalScreen .TabIndex = 0;
      this .TerminalScreen .MouseClick += new System .Windows .Forms .MouseEventHandler (this .TerminalScreen_MouseClick);
      this .TerminalScreen .Paint += new System .Windows .Forms .PaintEventHandler (this .TerminalScreen_Paint);
      // 
      // StatusBar
      // 
      this .StatusBar .Location = new System .Drawing .Point (0, 531);
      this .StatusBar .Name = "StatusBar";
      this .StatusBar .Size = new System .Drawing .Size (779, 20);
      this .StatusBar .TabIndex = 1;
      // 
      // RefreshTimer
      // 
      this .RefreshTimer .Interval = 25;
      this .RefreshTimer .Tick += new System .EventHandler (this .RefreshTimer_Tick);
      // 
      // TeminalWindow
      // 
      this .AutoScaleDimensions = new System .Drawing .SizeF (6F, 13F);
      this .AutoScaleMode = System .Windows .Forms .AutoScaleMode .Font;
      this .BorderStyle = System .Windows .Forms .BorderStyle .FixedSingle;
      this .Controls .Add (this .StatusBar);
      this .Controls .Add (this .TerminalScreen);
      this .Name = "TeminalWindow";
      this .Size = new System .Drawing .Size (778, 551);
      this .Enter += new System .EventHandler (this .TeminalWindow_Enter);
      this .Leave += new System .EventHandler (this .TeminalWindow_Leave);
      this .KeyPress += new System .Windows .Forms .KeyPressEventHandler (this .TeminalWindow_KeyPress);
      this .SizeChanged += new System .EventHandler (this .TeminalWindow_SizeChanged);
      this .ResumeLayout (false);

    }

    #endregion

    private System.Windows.Forms.Panel TerminalScreen;
    private System.Windows.Forms.Panel StatusBar;
    private System.Windows.Forms.Timer RefreshTimer;
  }
}
