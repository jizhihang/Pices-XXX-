using System;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Text;
using System.Xml;
using System.IO;

using ChartControls;
using PicesInterface;
using SipperFile;
using SipperFileViewer;

// TODO When the GUI starts up, the STOP button is not grayed-out.  If I click it, then I get an error message as shown in the attached screenshot.




namespace SipperInterface
{
  /// <summary>
  /// This is the Sipper 3 interface.
  /// </summary>
  public class CommandWindow : System.Windows.Forms.Form
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>

    private System.Windows.Forms.MainMenu mainMenu1;
    private System.Windows.Forms.MenuItem MainMenu;
    private System.Windows.Forms.MenuItem Conn;
    private System.Windows.Forms.MenuItem Disconn;
    private System.Windows.Forms.StatusBar statusBar;
    private System.Windows.Forms.Button startButton;
    private System.Windows.Forms.Button stopButton;
    private System.Windows.Forms.Button StatusButton;
    private System.Windows.Forms.RichTextBox tb_info;
    private System.Windows.Forms.Button bt_preview;
    private System.Windows.Forms.TextBox tb_sLBA;
    private System.Windows.Forms.TextBox tb_cLBA;
    private System.Windows.Forms.TextBox tb_eLBA;
    private System.Windows.Forms.Label lb_sLBA;
    private System.Windows.Forms.Label lb_cLBA;
    private System.Windows.Forms.Label lb_eLBA;
    static AutoResetEvent JobDone = new AutoResetEvent(false);
    private Encoding ASCII = Encoding.ASCII;
    private System.Windows.Forms.MenuItem menuItem1;
    private System.Windows.Forms.MenuItem Exit;
    private System.Windows.Forms.TextBox blocks;
    private System.Windows.Forms.SaveFileDialog saveFile;
    //private System.Windows.Forms.MenuItem menuOption;
    private System.Windows.Forms.MenuItem menuItem2;
    private System .Windows .Forms .MenuItem ConfigMenuItem;
    private System.Windows.Forms.Label lb_usage;
    private System.Windows.Forms.TextBox tb_usage;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.TextBox tb_Flow;
    private System.Windows.Forms.Label label4;
    private System .Windows .Forms .TextBox tb_Record;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.TextBox tb_available;
    private System .Windows .Forms .TextBox sipperFileName;
    private System.Windows.Forms.Button setFileNameDescButton;
    private IContainer components;
    private Button cameraLineButton;
    private System .Windows .Forms .Timer timer1;
    private  TextBox           ScanRate;
    private  Label             ScanRateLabel;
    private  BatteryPack       sipperBatteryPack;
    private  TeminalWindow     teminalWindow;
    private  bool              enableAppropiateButtons   = false;
    private TabControl DialogTabs;
    private TabPage tabPage1;
    private TabPage tabPage2;
    private ChartControls.InclinationPanel Pitch;
    private ChartControls.InclinationPanel Roll;
    private MenuItem menuItem3;
    private MenuItem ReBootXportMenuItem;
    private MenuItem ReBootBatteryMenuItem;




    private  CameraParameters       cameraParms          = null;
    private  CircularBuffer         circularBuff         = null;
    private  InstrumentData         currentDataRow       = null;
    private  InstrumentDataList     history              = null;
    
    private  Queue<SipperCommand>   sipperCmdsExecuted   = null;
    
    private  DepthPlot         depthPlot                 = null;
    private  StreamWriter      logFile                   = null;
    private  string            logFileName               = "SipperLog.txt";
    private  bool              sipperFileNameHsBeenSetAtLeastOnce = false;    // Set to true 1st time SetFile Name command  issued.
    private  SipperConfigRec   sipperConfigRec           = null;
    private  SipperHeaderRec   headerRec                 = null;

    private  string            sipperInterfaceDir        = "";
    private  string            logDirectory              = "";
    private List<SipperFileViewer.SipperFileViewer>  sipperPreviews = null;  
                                                             // List of all SipperFileViewers displayed
                                                             // This will allow me to properly close them
                                                             // when the main app shuts down.

    private  List<CameraLineDisplay>  cameraLineDisplays = null;

    private  bool              statusRecording           = false;
    private  TimePlot          timePlot                  = null;
    private  Queue             textBoxBuffer             = null;

    // Flags; set when you are to update Controls from the 'timer1_Tick' event.
    private  bool              updateBatteryPack         = false;
    private Label DepthLabel;
    private TextBox Depth;
    private TextBox CurrentSeqNum;

    private  bool              awaitingChangeBatteryCmd  = false;  // while true BatteryPack can NOT be enabled.

    private  string            lastFileNameCreated = "";



    public CommandWindow ()
    {
      MessageBox.Show ("Starting Command Window");

      try
      {
        sipperInterfaceDir = PicesSipperVariables.SipperInterfaceDir ();
        OSservices.CreateDirectory (sipperInterfaceDir);
        logDirectory = OSservices.AddSlash (sipperInterfaceDir) + "LogFiles";
        OSservices.CreateDirectory (logDirectory);
      }
      catch  (Exception e)
      {
        MessageBox.Show ("Received Exception at start of CommandWindow" + "\n" + e.ToString ());
        throw new Exception ("Exception at start of 'CommandWindow'", e);
      }


      try
      {
        OpenLogFile ();

        logFile.WriteLine ("CommandWindow     Starting Up   Date["  + DateTime.Now.ToString () + "]");

        sipperConfigRec    = new SipperConfigRec ();
        sipperPreviews     = new List<SipperFileViewer.SipperFileViewer> ();  
        cameraLineDisplays = new List<CameraLineDisplay> ();

        InitializeComponent();
        textBoxBuffer = Queue.Synchronized (new Queue ());
      }
      catch  (Exception e2)
      {
        MessageBox.Show ("Received Exception at start of CommandWindow" + "\n" + e2.ToString ());
        throw new Exception ("Exception at 2nd part of 'CommandWindow'", e2);
      }

      logFile.WriteLine ("CommandWindow     Exiting");
      logFile.Flush ();
    }
    


    
    private  void  CloseExternalWindows ()
    {
      if  (sipperPreviews != null)
      {
        for  (int x = 0;  x < sipperPreviews.Count;  x++)
        {
          SipperFileViewer.SipperFileViewer  viewer = sipperPreviews[x];
          if  (viewer != null)
          {
            if  (!(viewer.IsDisposed))
              viewer.Dispose ();

            sipperPreviews[x] = null;
          }
          
        }
        sipperPreviews = null;
      }
      
      if  (cameraLineDisplays != null)
      {
        for  (int x = 0;  x < cameraLineDisplays.Count;  x++)
        {
          if  (cameraLineDisplays[x] != null)
          {
            cameraLineDisplays[x].Close ();
            cameraLineDisplays[x].Dispose ();
            cameraLineDisplays[x] = null;
          }
        }
      }
      cameraLineDisplays = null;

      if  (depthPlot != null)
      {
        try
        {
          depthPlot.Close ();
          depthPlot.Dispose ();
        }
        catch  (Exception e)
        {
          MessageBox.Show ("Error Clossing DepthPlot window" + "\n\n" + e.ToString ());
        }
        depthPlot = null;
      }
      
      if  (timePlot != null)
      {
        try
        {
          timePlot.Close   ();
          timePlot.Dispose ();
        }
        catch  (Exception e2)
        {
          MessageBox.Show ("Error Clossing TimePlot window" + "\n\n" + e2.ToString ());
        }
        timePlot = null;
      }

    }  /* CloseExternalWindows */



        
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    protected override void Dispose( bool disposing )
    {
      CloseExternalWindows ();
      base.Dispose( disposing );
    }



    #region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container ();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (CommandWindow));
      this.mainMenu1 = new System.Windows.Forms.MainMenu (this.components);
      this.MainMenu = new System.Windows.Forms.MenuItem ();
      this.Conn = new System.Windows.Forms.MenuItem ();
      this.Disconn = new System.Windows.Forms.MenuItem ();
      this.menuItem1 = new System.Windows.Forms.MenuItem ();
      this.Exit = new System.Windows.Forms.MenuItem ();
      this.menuItem2 = new System.Windows.Forms.MenuItem ();
      this.ConfigMenuItem = new System.Windows.Forms.MenuItem ();
      this.menuItem3 = new System.Windows.Forms.MenuItem ();
      this.ReBootXportMenuItem = new System.Windows.Forms.MenuItem ();
      this.ReBootBatteryMenuItem = new System.Windows.Forms.MenuItem ();
      this.statusBar = new System.Windows.Forms.StatusBar ();
      this.startButton = new System.Windows.Forms.Button ();
      this.stopButton = new System.Windows.Forms.Button ();
      this.StatusButton = new System.Windows.Forms.Button ();
      this.lb_sLBA = new System.Windows.Forms.Label ();
      this.tb_sLBA = new System.Windows.Forms.TextBox ();
      this.tb_info = new System.Windows.Forms.RichTextBox ();
      this.bt_preview = new System.Windows.Forms.Button ();
      this.lb_cLBA = new System.Windows.Forms.Label ();
      this.tb_cLBA = new System.Windows.Forms.TextBox ();
      this.lb_eLBA = new System.Windows.Forms.Label ();
      this.tb_eLBA = new System.Windows.Forms.TextBox ();
      this.blocks = new System.Windows.Forms.TextBox ();
      this.saveFile = new System.Windows.Forms.SaveFileDialog ();
      this.lb_usage = new System.Windows.Forms.Label ();
      this.tb_usage = new System.Windows.Forms.TextBox ();
      this.label3 = new System.Windows.Forms.Label ();
      this.tb_Flow = new System.Windows.Forms.TextBox ();
      this.label4 = new System.Windows.Forms.Label ();
      this.tb_Record = new System.Windows.Forms.TextBox ();
      this.sipperFileName = new System.Windows.Forms.TextBox ();
      this.setFileNameDescButton = new System.Windows.Forms.Button ();
      this.label5 = new System.Windows.Forms.Label ();
      this.tb_available = new System.Windows.Forms.TextBox ();
      this.cameraLineButton = new System.Windows.Forms.Button ();
      this.timer1 = new System.Windows.Forms.Timer (this.components);
      this.ScanRateLabel = new System.Windows.Forms.Label ();
      this.ScanRate = new System.Windows.Forms.TextBox ();
      this.DialogTabs = new System.Windows.Forms.TabControl ();
      this.tabPage2 = new System.Windows.Forms.TabPage ();
      this.tabPage1 = new System.Windows.Forms.TabPage ();
      this.DepthLabel = new System.Windows.Forms.Label ();
      this.Depth = new System.Windows.Forms.TextBox ();
      this.CurrentSeqNum = new System.Windows.Forms.TextBox ();
      this.Roll = new ChartControls.InclinationPanel ();
      this.Pitch = new ChartControls.InclinationPanel ();
      this.sipperBatteryPack = new ChartControls.BatteryPack ();
      this.teminalWindow = new ChartControls.TeminalWindow ();
      this.DialogTabs.SuspendLayout ();
      this.tabPage2.SuspendLayout ();
      this.tabPage1.SuspendLayout ();
      this.SuspendLayout ();
      // 
      // mainMenu1
      // 
      this.mainMenu1.MenuItems.AddRange (new System.Windows.Forms.MenuItem[] {
            this.MainMenu,
            this.menuItem2,
            this.menuItem3});
      // 
      // MainMenu
      // 
      this.MainMenu.Index = 0;
      this.MainMenu.MenuItems.AddRange (new System.Windows.Forms.MenuItem[] {
            this.Conn,
            this.Disconn,
            this.menuItem1,
            this.Exit});
      this.MainMenu.Text = "Commands";
      // 
      // Conn
      // 
      this.Conn.Index = 0;
      this.Conn.Text = "&Connect";
      this.Conn.Click += new System.EventHandler (this.Conn_Click);
      // 
      // Disconn
      // 
      this.Disconn.Index = 1;
      this.Disconn.Text = "&Disconnect";
      this.Disconn.Click += new System.EventHandler (this.Disconn_Click);
      // 
      // menuItem1
      // 
      this.menuItem1.Index = 2;
      this.menuItem1.Text = "-";
      // 
      // Exit
      // 
      this.Exit.Index = 3;
      this.Exit.Text = "E&xit";
      this.Exit.Click += new System.EventHandler (this.Exit_Click);
      // 
      // menuItem2
      // 
      this.menuItem2.Index = 1;
      this.menuItem2.MenuItems.AddRange (new System.Windows.Forms.MenuItem[] {
            this.ConfigMenuItem});
      this.menuItem2.Text = "&Options";
      // 
      // ConfigMenuItem
      // 
      this.ConfigMenuItem.Index = 0;
      this.ConfigMenuItem.Text = "Configuration";
      this.ConfigMenuItem.Click += new System.EventHandler (this.Config_Click);
      // 
      // menuItem3
      // 
      this.menuItem3.Index = 2;
      this.menuItem3.MenuItems.AddRange (new System.Windows.Forms.MenuItem[] {
            this.ReBootXportMenuItem,
            this.ReBootBatteryMenuItem});
      this.menuItem3.Text = "&Re-Boot";
      // 
      // ReBootXportMenuItem
      // 
      this.ReBootXportMenuItem.Index = 0;
      this.ReBootXportMenuItem.Text = "X-Port";
      this.ReBootXportMenuItem.Click += new System.EventHandler (this.ReBootXportMenuItem_Click);
      // 
      // ReBootBatteryMenuItem
      // 
      this.ReBootBatteryMenuItem.Index = 1;
      this.ReBootBatteryMenuItem.Text = "Battery";
      this.ReBootBatteryMenuItem.Click += new System.EventHandler (this.ReBootBatteryMenuItem_Click);
      // 
      // statusBar
      // 
      this.statusBar.Location = new System.Drawing.Point (0, 811);
      this.statusBar.Name = "statusBar";
      this.statusBar.Size = new System.Drawing.Size (1064, 27);
      this.statusBar.TabIndex = 0;
      // 
      // startButton
      // 
      this.startButton.AutoSize = true;
      this.startButton.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.startButton.Location = new System.Drawing.Point (8, 10);
      this.startButton.Name = "startButton";
      this.startButton.Size = new System.Drawing.Size (104, 29);
      this.startButton.TabIndex = 5;
      this.startButton.Text = "Start";
      this.startButton.Click += new System.EventHandler (this.startButton_Click);
      // 
      // stopButton
      // 
      this.stopButton.AutoSize = true;
      this.stopButton.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.stopButton.Location = new System.Drawing.Point (8, 39);
      this.stopButton.Name = "stopButton";
      this.stopButton.Size = new System.Drawing.Size (104, 29);
      this.stopButton.TabIndex = 6;
      this.stopButton.Text = "Stop";
      this.stopButton.Click += new System.EventHandler (this.stopButton_Click);
      // 
      // StatusButton
      // 
      this.StatusButton.AutoSize = true;
      this.StatusButton.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.StatusButton.Location = new System.Drawing.Point (134, 10);
      this.StatusButton.Name = "StatusButton";
      this.StatusButton.Size = new System.Drawing.Size (104, 29);
      this.StatusButton.TabIndex = 7;
      this.StatusButton.Text = "Status";
      this.StatusButton.Click += new System.EventHandler (this.StatusButtonClicked);
      // 
      // lb_sLBA
      // 
      this.lb_sLBA.AutoSize = true;
      this.lb_sLBA.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.lb_sLBA.Location = new System.Drawing.Point (8, 84);
      this.lb_sLBA.Name = "lb_sLBA";
      this.lb_sLBA.Size = new System.Drawing.Size (56, 16);
      this.lb_sLBA.TabIndex = 13;
      this.lb_sLBA.Text = "Start Bk";
      this.lb_sLBA.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // tb_sLBA
      // 
      this.tb_sLBA.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.tb_sLBA.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.tb_sLBA.Location = new System.Drawing.Point (80, 81);
      this.tb_sLBA.Name = "tb_sLBA";
      this.tb_sLBA.ReadOnly = true;
      this.tb_sLBA.Size = new System.Drawing.Size (126, 22);
      this.tb_sLBA.TabIndex = 12;
      this.tb_sLBA.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // tb_info
      // 
      this.tb_info.HideSelection = false;
      this.tb_info.Location = new System.Drawing.Point (-1, 0);
      this.tb_info.Name = "tb_info";
      this.tb_info.Size = new System.Drawing.Size (620, 533);
      this.tb_info.TabIndex = 16;
      this.tb_info.Text = "";
      // 
      // bt_preview
      // 
      this.bt_preview.AutoSize = true;
      this.bt_preview.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.bt_preview.Location = new System.Drawing.Point (257, 10);
      this.bt_preview.Name = "bt_preview";
      this.bt_preview.Size = new System.Drawing.Size (104, 29);
      this.bt_preview.TabIndex = 17;
      this.bt_preview.Text = "Preview";
      this.bt_preview.Click += new System.EventHandler (this.bt_preview_Click);
      // 
      // lb_cLBA
      // 
      this.lb_cLBA.AutoSize = true;
      this.lb_cLBA.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.lb_cLBA.Location = new System.Drawing.Point (8, 114);
      this.lb_cLBA.Name = "lb_cLBA";
      this.lb_cLBA.Size = new System.Drawing.Size (70, 16);
      this.lb_cLBA.TabIndex = 19;
      this.lb_cLBA.Text = "Current Bk";
      this.lb_cLBA.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // tb_cLBA
      // 
      this.tb_cLBA.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.tb_cLBA.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.tb_cLBA.Location = new System.Drawing.Point (80, 111);
      this.tb_cLBA.Name = "tb_cLBA";
      this.tb_cLBA.ReadOnly = true;
      this.tb_cLBA.Size = new System.Drawing.Size (126, 22);
      this.tb_cLBA.TabIndex = 18;
      this.tb_cLBA.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // lb_eLBA
      // 
      this.lb_eLBA.AutoSize = true;
      this.lb_eLBA.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.lb_eLBA.Location = new System.Drawing.Point (9, 144);
      this.lb_eLBA.Name = "lb_eLBA";
      this.lb_eLBA.Size = new System.Drawing.Size (51, 16);
      this.lb_eLBA.TabIndex = 21;
      this.lb_eLBA.Text = "End Bk";
      this.lb_eLBA.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // tb_eLBA
      // 
      this.tb_eLBA.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.tb_eLBA.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.tb_eLBA.Location = new System.Drawing.Point (80, 141);
      this.tb_eLBA.Name = "tb_eLBA";
      this.tb_eLBA.ReadOnly = true;
      this.tb_eLBA.Size = new System.Drawing.Size (126, 22);
      this.tb_eLBA.TabIndex = 20;
      this.tb_eLBA.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // blocks
      // 
      this.blocks.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.blocks.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.blocks.Location = new System.Drawing.Point (368, 11);
      this.blocks.Name = "blocks";
      this.blocks.Size = new System.Drawing.Size (156, 22);
      this.blocks.TabIndex = 23;
      // 
      // saveFile
      // 
      this.saveFile.Filter = "Text Files|*.txt";
      this.saveFile.Title = "Specify Destination Filename";
      // 
      // lb_usage
      // 
      this.lb_usage.AutoSize = true;
      this.lb_usage.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.lb_usage.Location = new System.Drawing.Point (256, 114);
      this.lb_usage.Name = "lb_usage";
      this.lb_usage.Size = new System.Drawing.Size (59, 16);
      this.lb_usage.TabIndex = 33;
      this.lb_usage.Text = "File Size";
      this.lb_usage.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // tb_usage
      // 
      this.tb_usage.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.tb_usage.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.tb_usage.Location = new System.Drawing.Point (337, 111);
      this.tb_usage.Name = "tb_usage";
      this.tb_usage.ReadOnly = true;
      this.tb_usage.Size = new System.Drawing.Size (126, 22);
      this.tb_usage.TabIndex = 32;
      this.tb_usage.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label3.Location = new System.Drawing.Point (658, 555);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size (66, 16);
      this.label3.TabIndex = 35;
      this.label3.Text = "Flow Rate";
      this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // tb_Flow
      // 
      this.tb_Flow.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.tb_Flow.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.tb_Flow.Location = new System.Drawing.Point (729, 552);
      this.tb_Flow.Name = "tb_Flow";
      this.tb_Flow.ReadOnly = true;
      this.tb_Flow.Size = new System.Drawing.Size (116, 22);
      this.tb_Flow.TabIndex = 34;
      this.tb_Flow.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label4.Location = new System.Drawing.Point (256, 144);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size (80, 16);
      this.label4.TabIndex = 37;
      this.label4.Text = "Record Rate";
      this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // tb_Record
      // 
      this.tb_Record.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.tb_Record.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.tb_Record.Location = new System.Drawing.Point (338, 141);
      this.tb_Record.Name = "tb_Record";
      this.tb_Record.ReadOnly = true;
      this.tb_Record.Size = new System.Drawing.Size (126, 22);
      this.tb_Record.TabIndex = 36;
      this.tb_Record.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // sipperFileName
      // 
      this.sipperFileName.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.sipperFileName.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.sipperFileName.Location = new System.Drawing.Point (368, 41);
      this.sipperFileName.Name = "sipperFileName";
      this.sipperFileName.ReadOnly = true;
      this.sipperFileName.Size = new System.Drawing.Size (165, 22);
      this.sipperFileName.TabIndex = 29;
      // 
      // setFileNameDescButton
      // 
      this.setFileNameDescButton.AutoSize = true;
      this.setFileNameDescButton.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.setFileNameDescButton.Location = new System.Drawing.Point (257, 39);
      this.setFileNameDescButton.Name = "setFileNameDescButton";
      this.setFileNameDescButton.Size = new System.Drawing.Size (104, 29);
      this.setFileNameDescButton.TabIndex = 31;
      this.setFileNameDescButton.Text = "Set Name";
      this.setFileNameDescButton.Click += new System.EventHandler (this.SetFileNameDescButtonClicked);
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label5.Location = new System.Drawing.Point (256, 84);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size (76, 16);
      this.label5.TabIndex = 39;
      this.label5.Text = "Avail Space";
      this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // tb_available
      // 
      this.tb_available.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.tb_available.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.tb_available.Location = new System.Drawing.Point (337, 81);
      this.tb_available.Name = "tb_available";
      this.tb_available.ReadOnly = true;
      this.tb_available.Size = new System.Drawing.Size (126, 22);
      this.tb_available.TabIndex = 38;
      this.tb_available.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // cameraLineButton
      // 
      this.cameraLineButton.AutoSize = true;
      this.cameraLineButton.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.cameraLineButton.Location = new System.Drawing.Point (134, 39);
      this.cameraLineButton.Name = "cameraLineButton";
      this.cameraLineButton.Size = new System.Drawing.Size (104, 29);
      this.cameraLineButton.TabIndex = 41;
      this.cameraLineButton.Text = "Camera Line";
      this.cameraLineButton.Click += new System.EventHandler (this.cameraLineButton_Click);
      // 
      // timer1
      // 
      this.timer1.Enabled = true;
      this.timer1.Interval = 25;
      this.timer1.Tick += new System.EventHandler (this.timer1_Tick);
      // 
      // ScanRateLabel
      // 
      this.ScanRateLabel.AutoSize = true;
      this.ScanRateLabel.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanRateLabel.Location = new System.Drawing.Point (658, 585);
      this.ScanRateLabel.Name = "ScanRateLabel";
      this.ScanRateLabel.Size = new System.Drawing.Size (69, 16);
      this.ScanRateLabel.TabIndex = 45;
      this.ScanRateLabel.Text = "Scan Rate";
      this.ScanRateLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // ScanRate
      // 
      this.ScanRate.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.ScanRate.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanRate.Location = new System.Drawing.Point (729, 582);
      this.ScanRate.Name = "ScanRate";
      this.ScanRate.ReadOnly = true;
      this.ScanRate.Size = new System.Drawing.Size (116, 22);
      this.ScanRate.TabIndex = 46;
      this.ScanRate.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // DialogTabs
      // 
      this.DialogTabs.Controls.Add (this.tabPage2);
      this.DialogTabs.Controls.Add (this.tabPage1);
      this.DialogTabs.Location = new System.Drawing.Point (9, 182);
      this.DialogTabs.Name = "DialogTabs";
      this.DialogTabs.SelectedIndex = 0;
      this.DialogTabs.Size = new System.Drawing.Size (633, 562);
      this.DialogTabs.TabIndex = 48;
      // 
      // tabPage2
      // 
      this.tabPage2.Controls.Add (this.tb_info);
      this.tabPage2.Location = new System.Drawing.Point (4, 22);
      this.tabPage2.Name = "tabPage2";
      this.tabPage2.Padding = new System.Windows.Forms.Padding (3);
      this.tabPage2.Size = new System.Drawing.Size (625, 536);
      this.tabPage2.TabIndex = 1;
      this.tabPage2.Text = "SIPPER Session Dialog";
      this.tabPage2.UseVisualStyleBackColor = true;
      // 
      // tabPage1
      // 
      this.tabPage1.Controls.Add (this.teminalWindow);
      this.tabPage1.Location = new System.Drawing.Point (4, 22);
      this.tabPage1.Name = "tabPage1";
      this.tabPage1.Padding = new System.Windows.Forms.Padding (3);
      this.tabPage1.Size = new System.Drawing.Size (625, 536);
      this.tabPage1.TabIndex = 0;
      this.tabPage1.Text = "Manual Entry";
      this.tabPage1.UseVisualStyleBackColor = true;
      // 
      // DepthLabel
      // 
      this.DepthLabel.AutoSize = true;
      this.DepthLabel.Font = new System.Drawing.Font ("Arial", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DepthLabel.Location = new System.Drawing.Point (658, 618);
      this.DepthLabel.Name = "DepthLabel";
      this.DepthLabel.Size = new System.Drawing.Size (42, 16);
      this.DepthLabel.TabIndex = 51;
      this.DepthLabel.Text = "Depth";
      this.DepthLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // Depth
      // 
      this.Depth.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.Depth.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Depth.Location = new System.Drawing.Point (729, 616);
      this.Depth.Name = "Depth";
      this.Depth.ReadOnly = true;
      this.Depth.Size = new System.Drawing.Size (116, 22);
      this.Depth.TabIndex = 52;
      this.Depth.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      // 
      // CurrentSeqNum
      // 
      this.CurrentSeqNum.BackColor = System.Drawing.SystemColors.ControlLightLight;
      this.CurrentSeqNum.Font = new System.Drawing.Font ("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CurrentSeqNum.Location = new System.Drawing.Point (541, 41);
      this.CurrentSeqNum.Name = "CurrentSeqNum";
      this.CurrentSeqNum.ReadOnly = true;
      this.CurrentSeqNum.Size = new System.Drawing.Size (44, 22);
      this.CurrentSeqNum.TabIndex = 54;
      this.CurrentSeqNum.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // Roll
      // 
      this.Roll.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.Roll.LineColor = System.Drawing.Color.Blue;
      this.Roll.Location = new System.Drawing.Point (685, 373);
      this.Roll.Margin = new System.Windows.Forms.Padding (4);
      this.Roll.Name = "Roll";
      this.Roll.Size = new System.Drawing.Size (186, 150);
      this.Roll.TabIndex = 50;
      this.Roll.Title = "Roll";
      // 
      // Pitch
      // 
      this.Pitch.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.Pitch.LineColor = System.Drawing.Color.Blue;
      this.Pitch.Location = new System.Drawing.Point (685, 204);
      this.Pitch.Margin = new System.Windows.Forms.Padding (4);
      this.Pitch.Name = "Pitch";
      this.Pitch.Size = new System.Drawing.Size (186, 150);
      this.Pitch.TabIndex = 49;
      // 
      // sipperBatteryPack
      // 
      this.sipperBatteryPack.Battery1Voltage = 12F;
      this.sipperBatteryPack.Battery2Voltage = 12F;
      this.sipperBatteryPack.Battery3Voltage = 12F;
      this.sipperBatteryPack.Battery4Voltage = 12F;
      this.sipperBatteryPack.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.sipperBatteryPack.ColorEmpty = System.Drawing.Color.Red;
      this.sipperBatteryPack.ColorFull = System.Drawing.Color.Green;
      this.sipperBatteryPack.ColorNearEmpty = System.Drawing.Color.Orange;
      this.sipperBatteryPack.Font = new System.Drawing.Font ("Microsoft Sans Serif", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.sipperBatteryPack.Location = new System.Drawing.Point (599, 10);
      this.sipperBatteryPack.MaxVoltage = 26F;
      this.sipperBatteryPack.MinimumSize = new System.Drawing.Size (239, 141);
      this.sipperBatteryPack.Name = "sipperBatteryPack";
      this.sipperBatteryPack.RequestedBattery = -1;
      this.sipperBatteryPack.RequestedBatteryEnabled = false;
      this.sipperBatteryPack.Size = new System.Drawing.Size (272, 166);
      this.sipperBatteryPack.TabIndex = 44;
      this.sipperBatteryPack.VoltageFont = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.sipperBatteryPack.BatteryClicked += new ChartControls.BatteryPack.BatteryClickedHandler (this.sipperBatteryPack_BatteryClicked);
      // 
      // teminalWindow
      // 
      this.teminalWindow.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.teminalWindow.History = ((System.Collections.Generic.List<string>)(resources.GetObject ("teminalWindow.History")));
      this.teminalWindow.Location = new System.Drawing.Point (0, 1);
      this.teminalWindow.Margin = new System.Windows.Forms.Padding (4);
      this.teminalWindow.Name = "teminalWindow";
      this.teminalWindow.Size = new System.Drawing.Size (625, 529);
      this.teminalWindow.TabIndex = 47;
      this.teminalWindow.NewCmdEntered += new ChartControls.TeminalWindow.NewCmdEnteredHandler (this.teminalWindow1_NewCmdEntered);
      // 
      // CommandWindow
      // 
      this.AutoScaleBaseSize = new System.Drawing.Size (5, 13);
      this.AutoSize = true;
      this.ClientSize = new System.Drawing.Size (1064, 838);
      this.Controls.Add (this.CurrentSeqNum);
      this.Controls.Add (this.Depth);
      this.Controls.Add (this.DepthLabel);
      this.Controls.Add (this.Roll);
      this.Controls.Add (this.Pitch);
      this.Controls.Add (this.ScanRate);
      this.Controls.Add (this.ScanRateLabel);
      this.Controls.Add (this.sipperBatteryPack);
      this.Controls.Add (this.cameraLineButton);
      this.Controls.Add (this.label5);
      this.Controls.Add (this.tb_available);
      this.Controls.Add (this.label4);
      this.Controls.Add (this.tb_Record);
      this.Controls.Add (this.label3);
      this.Controls.Add (this.tb_Flow);
      this.Controls.Add (this.lb_usage);
      this.Controls.Add (this.tb_usage);
      this.Controls.Add (this.setFileNameDescButton);
      this.Controls.Add (this.sipperFileName);
      this.Controls.Add (this.blocks);
      this.Controls.Add (this.lb_eLBA);
      this.Controls.Add (this.tb_eLBA);
      this.Controls.Add (this.lb_cLBA);
      this.Controls.Add (this.tb_cLBA);
      this.Controls.Add (this.bt_preview);
      this.Controls.Add (this.lb_sLBA);
      this.Controls.Add (this.tb_sLBA);
      this.Controls.Add (this.StatusButton);
      this.Controls.Add (this.stopButton);
      this.Controls.Add (this.startButton);
      this.Controls.Add (this.statusBar);
      this.Controls.Add (this.DialogTabs);
      this.Menu = this.mainMenu1;
      this.MinimumSize = new System.Drawing.Size (900, 850);
      this.Name = "CommandWindow";
      this.Text = "CommandWindow";
      this.Load += new System.EventHandler (this.CommandWindow_Load);
      this.Closing += new System.ComponentModel.CancelEventHandler (this.Form1_Closing);
      this.DialogTabs.ResumeLayout (false);
      this.tabPage2.ResumeLayout (false);
      this.tabPage1.ResumeLayout (false);
      this.ResumeLayout (false);
      this.PerformLayout ();

    }
    #endregion

    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main() 
    {
      try 
      {
        Application.Run(new CommandWindow());
      }
      catch  (Exception e)
      {
        MessageBox.Show ("Unhandled exception in SipperInterface" + "\n\n" +
                         e.ToString ()
                        );
      }
    }


    
    private  string  ConstructInstrumentFileName (string  sipperFileName)
    {
      String  instDataDir = OSservices.AddSlash (PicesSipperVariables.SipperInterfaceDir ()) + "InstrumentData";

      if  (instDataDir == "")
        instDataDir = OSservices.AddSlash (PicesSipperVariables.SipperInterfaceDir ()) + "InstrumentData";
      OSservices.CreateDirectory (instDataDir);
      
      string  instDataRootName = sipperFileName;
      if  (instDataRootName == "")
        instDataRootName = "InstrumentData";
      else
        instDataRootName = instDataRootName + "_InstrumentData";

      string instDataFileName = OSservices.AddSlash (instDataDir) + instDataRootName + ".txt";

      return  instDataFileName;
    }  /* ConstructInstrumentFileName */



    private void  CommandWindow_Load (object sender, System.EventArgs e)
    {
      logFile.WriteLine ("CommandWindow_Load   Entering");
    
      TurnOffAllButtons ();
      
      //initial variables
      tb_sLBA.Text = tb_cLBA.Text = tb_eLBA.Text = "0";
      statusBar.Text = "Initial ip and port information loaded";
      tb_Flow.Text   = "0.00 m/s";
      tb_Record.Text = "0.00 Mb/sec";
      tb_usage.Text  = "0.00 Mb";			

      //saving temporary file in case Sipper Crashes

      //StreamWriter Background_Writer = new StreamWriter (@"temporary_save.txt", true);

      logFile.WriteLine ("//Starting Sipper Interface");
      logFile.WriteLine ("//Cur Time:" + System.DateTime.Now.ToString ());
      logFile.WriteLine ("//");
      logFile.Flush ();

      if  (!sipperConfigRec.LoadedFromConfigFile ())
        PromptConfigurationInfoUntilUpdated ();

      if  (!sipperConfigRec.LoadedFromConfigFile ())
        return;


       headerRec = sipperConfigRec.HeaderRec ();

      //load info from config files
       sipperFileName.Text = headerRec.SipperFileName ();

      //Auto Connect.  Copied straight from the conn_click function
      //ConnectSocket ();  


      logFile.WriteLine ("");
      logFile.WriteLine ("//Load History Data");
      logFile.Flush ();
      history = new InstrumentDataList (ConstructInstrumentFileName (sipperConfigRec.HeaderRec ().SipperFileName ()), logFile);

      logFile.WriteLine ("");
      logFile.WriteLine ("//Create DataWindow for depth plot");
      logFile.Flush ();

      currentDataRow = new InstrumentData ();
      depthPlot = new DepthPlot (this, sipperConfigRec ,history);
      depthPlot.Show ();

      logFile.WriteLine ("");
      logFile.WriteLine ("//Create TimePlot window.");
      logFile.Flush ();

      timePlot = new TimePlot (sipperConfigRec, history);
      timePlot.Show ();

      sipperCmdsExecuted = new Queue<SipperCommand> ();
      
      EnableAppropriateButtons ();
    }  /* CommandWindow_Load */


    


    private void  OpenLogFile ()
    {
      if  (logFile != null)
      {
        try
        {
          logFile.Close ();
          logFile.Dispose ();
        }
        catch  (Exception)
        {
        }
        
        logFile = null;
      }

      DateTime curTime = System.DateTime.Now;

      int year, month, day, hour, min, sec;
      year  = curTime.Year;
      month = curTime.Month;
      day   = curTime.Day;
      hour  = curTime.Hour;
      min   = curTime.Minute;
      sec   = curTime.Second;

      string  rootLogFileName = "SipperInterfaceLog_" +
                                year.ToString  ("0000") +
                                month.ToString ("00")   +
                                day.ToString   ("00")   +
                                "-"                     +
                                hour.ToString  ("00")   +
                                min.ToString   ("00")   +
                                ".txt";

      logFileName = OSservices.AddSlash (logDirectory) + rootLogFileName;
      
      logFile = new StreamWriter (logFileName, true);
    }  /* OpenLogFile */

   
   
   

    /************************************************** 
     * Conn Click is a menu item that when click will 
     * try and establish a connection with Sipper 
     **************************************************/
    private void Conn_Click (object sender, System.EventArgs e)
    {
      TurnOffAllButtons ();
      statusBar.Text = "Connected to Sipper....";

      try
      {
        circularBuff = new CircularBuffer (sipperConfigRec, 40960, this, history, currentDataRow, logFile);
        statusBar.Text += "  Done";
      }
      catch  (Exception e2)
      {
        MessageBox.Show (e2.ToString (), "Error Opening conection to SIPPER");
        if  (circularBuff != null)
        {
          circularBuff.Dispose ();
          circularBuff = null;
        }
        statusBar.Text += "  *** FAILED ***";
      }

      if  (circularBuff != null)
      {
        circularBuff.QueueSipperCommand (SipperCommand.CreateGetCammeraParametersCmd (sipperCmdsExecuted, circularBuff, logFile));
        statusBar.Text = "Connected to Sipper....";
      }


      EnableAppropriateButtons ();
    }  /* Conn_Click */



    private  ArrayList  textHistory = new ArrayList ();



    public  void  UpdateTextWindow ()
    {
      long  startBlockAddr = 0;
      long  curBlockAddr   = 0;
      long  endBlockAddr   = 0;
      float flowRate       = 0.0f;

      if  (circularBuff != null)
      {
        startBlockAddr  = circularBuff.StartBlockAddr();
        curBlockAddr    = Math.Max (circularBuff.CurBlockAddr(), startBlockAddr);
        endBlockAddr    = circularBuff.EndBlockAddr();
        flowRate        = circularBuff.FlowRate();
        
        if  (statusRecording != circularBuff.StatusRecording ())
          enableAppropiateButtons = true;
        
        statusRecording = circularBuff.StatusRecording ();
        tb_Record.Text  = circularBuff.RecordingRateMegPerSec().ToString("0.000") + @" Mb/sec";
      }

      if  (currentDataRow != null)
        Depth.Text = currentDataRow.Depth ().ToString ("###0.00");


      tb_sLBA.Text = startBlockAddr.ToString ("##,###,##0");
      tb_cLBA.Text = curBlockAddr.ToString   ("##,###,##0");
      tb_eLBA.Text = endBlockAddr.ToString   ("##,###,##0");

      tb_Flow.Text = flowRate.ToString ("#,##0.00");

      //calculations
      double total = (float)(endBlockAddr - startBlockAddr) * 512 / 1000000;
      double usage = (float)(curBlockAddr - startBlockAddr) * 512 / 1000000;
      double available = total - usage;

      tb_usage.Text     = usage.ToString     ("##,##0.00") + " Mb";
      tb_available.Text = available.ToString ("##,##0.00") + " Mb";	

      if  (textHistory.Count > 500)
      {
        tb_info.Text = "";
        for  (int x = 0;  x < textHistory.Count;  x++)
        {
          tb_info.AppendText ((string)(textHistory[x]));
        }
        textHistory.Clear ();
      }
      
      lock (textBoxBuffer)
      {
        int  x = 0;
      
        while ((textBoxBuffer.Count > 0)  &&  (x < 5))
        {
          string s = (string)textBoxBuffer.Dequeue();

          try
          {
            tb_info.AppendText (s);
            //tb_info.SelectionStart = this.tb_info.Text.Length;
            textHistory.Add (s);
          }
          catch (Exception e)
          {
            MessageBox.Show("Error Writting to text box [" + e.ToString() + "].");
          }
          x++;
        }

        //tb_info.ScrollToCaret ();
      }
     
    }  /* UpdateTextWindow */




    public void  WriteToTextWindow (string ln)
    {
      lock (textBoxBuffer)
      {
        if (logFile == null)
          logFile = new StreamWriter(logFileName, true);
        logFile.WriteLine(ln);
        logFile.Flush();
 
        try
        {
          textBoxBuffer.Enqueue (ln);
        }
        catch  (Exception e)
        {
          MessageBox.Show ("Error adding to TextBox Queue [" + e.ToString () + "].");
        }
      }
    }  /* WriteToTextWindow */





    private  void  CloseSipperConnection ()
    {
      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");
        logFile.WriteLine ("// CloseSipperConnection    CurTime[" + System.DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }


      if  (circularBuff != null)
      {
        circularBuff.ShutDownThreads ();
        circularBuff.Dispose ();
        circularBuff = null;
      }
      
      // By turningoff all button we will force the timer to set the appropriate 
      // butons based off the current recording status.
      TurnOffAllButtons ();

    }  /* CloseSipperConnection */



    /***********************************************************
     * Disconnect menu button was clicked
     ************************************************************/
    private void Disconn_Click (object sender, System.EventArgs e)
    {
      TurnOffAllButtons ();

      CloseSipperConnection ();
      EnableAppropriateButtons ();
    }




    /***********************************************************
     * Start button was clicked.  Must be connected to Sipper
     ************************************************************/
    private void  startButton_Click (object sender, System.EventArgs e)
    {
      TurnOffAllButtons ();
      
      SipperCommand cmd = null;

      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");
        logFile.WriteLine ("//StartButton_Clicked  CurTime[" + System.DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      if  ((sipperFileName.Text.Trim ().Length < 1) || (!sipperFileNameHsBeenSetAtLeastOnce))
      {
        lock  (logFile)
        {
          logFile.WriteLine ("//");
          logFile.WriteLine ("//startButton_Click    File Name and Description not set yet.");
          logFile.WriteLine ("//");
          logFile.Flush ();
        }

        DialogResult dr = MessageBox.Show 
              ("You have not set the file name and description.  Do so now?", 
               "Set File Name and Description", MessageBoxButtons.YesNo
              );

        if  (dr == DialogResult.No)
        {
          lock  (logFile)
          {
            logFile.WriteLine ("//");
            logFile.WriteLine ("// User has opted   NOT   to set the file name.");
            logFile.WriteLine ("//");
            logFile.Flush ();
          }
          sipperFileNameHsBeenSetAtLeastOnce = true;  // We do not wish to ask this question more than once
        }

        else
        {
          lock  (logFile)
          {
            logFile.WriteLine ("//");
            logFile.WriteLine ("// startButton_Click    Will process config form.");
            logFile.WriteLine ("//");
            logFile.Flush ();
          }
          ProcessConfigForm ();

          cmd = SipperCommand.CreateSetFileNameAndStartRecordingCmd
                                        (sipperCmdsExecuted,
                                         sipperConfigRec.HeaderRec ().SipperFileName (), 
                                         sipperConfigRec.HeaderRec ().HeaderStr (),
                                         circularBuff, 
                                         logFile
                                        );
          
        }

      }

      if  (cmd == null)
      {
        cmd = SipperCommand.CreateStartRecordingCmd (sipperCmdsExecuted, circularBuff, logFile);
      }

     
      if  (cmd != null)
      {
        circularBuff.QueueSipperCommand (cmd);
        statusBar.Text = "Submitting   'Start Record'   Command.";
      }
    }  /* startButton_Click */




    /***********************************************************
     * Stop button was clicked.  Must be connected to Sipper
     ************************************************************/
    private void   stopButton_Click (object sender, System.EventArgs e)
    {
      TurnOffAllButtons ();

      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");
        logFile.WriteLine ("//stopButton_Click       CurTime[" + System.DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      circularBuff.QueueSipperCommand (SipperCommand.CreateStopRecordingCmd (sipperCmdsExecuted, circularBuff, logFile));
    }  /* stopButton_Click */





    /***********************************************************
     * Status button was clicked.  Must be connected to Sipper
     ************************************************************/
    private void   StatusButtonClicked  (object sender, System.EventArgs e)
    {
      TurnOffAllButtons ();

      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//dataLoopButtonClicked    Data/Status loop button pressed    CurTime[" + System.DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      if  (circularBuff == null)
        return;

      if  (circularBuff.InstrumentDataCollecting ())
      {
        // data loop is already running;  So we want to shut it down.
        circularBuff.CollectingInstrumentDataEnd ();
      }
      else
      {
        // DataLoop was not running so we want to start it now.
        circularBuff.CollectingInstrumentDataStart ();
      }
      
      EnableAppropriateButtons ();
    }  /* StatusButtonClicked */





    /***********************************************************
     * Preview button was clicked.  Must not be recording
     ************************************************************/
    private void  bt_preview_Click (object sender, System.EventArgs e)
    {	
      TurnOffAllButtons ();

      if  (circularBuff == null)
          return;

      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("// Preview Button:    CurTime[" + System.DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }
      
      int  blocksToRead = PicesKKStr.StrToInt (blocks.Text);

      circularBuff.QueueSipperCommand (SipperCommand.CreatePreviewCmd (sipperCmdsExecuted, blocksToRead, circularBuff, logFile));
    }  /* bt_preview_Click */



    private  void  EnableAppropriateButtons ()
    {
      if  (circularBuff == null)
      {
        NotConnectedButtonSetup ();
      }

      else 
      {
        if  (statusRecording)
        {
          RecordingButtonSetup ();
        }
        else
        {
          NotRecordingButtonSetup ();
        }
      }
      
      enableAppropiateButtons = false;
    }  /* EnableAppropriateButtons */





    private void  TurnOffAllButtons ()
    {
      cameraLineButton.Enabled       = false;
      Conn.Enabled                   = false;
      StatusButton.Enabled           = false;
      startButton.Enabled            = false;
      stopButton.Enabled             = false;
      bt_preview.Enabled             = false;
      setFileNameDescButton.Enabled  = false;
      ConfigMenuItem.Enabled         = false;
      ReBootXportMenuItem.Enabled    = false;
      ReBootBatteryMenuItem.Enabled  = false;
      sipperBatteryPack.Enabled      = false;
      sipperBatteryPack.Enabled      = false;
      sipperBatteryPack.RequestedBatteryEnabled = false;

      enableAppropiateButtons        = false;
    }  /* TurnOffAllButtons */





    private void  NotConnectedButtonSetup ()
    {
      Conn.Enabled                   = true;
      startButton.Enabled            = false;
      stopButton.Enabled             = false;
      StatusButton.Enabled           = false;
      bt_preview.Enabled             = false;
      setFileNameDescButton.Enabled  = false;
      ConfigMenuItem.Enabled         = true;
      cameraLineButton.Enabled       = false;
      ReBootXportMenuItem.Enabled    = true;
      ReBootBatteryMenuItem.Enabled  = false;
      sipperBatteryPack.Enabled      = false;

      sipperBatteryPack.Enabled      = false;
      sipperBatteryPack.RequestedBatteryEnabled = false;
    }  /* NotConnectedButtonSetup */





    private void  RecordingButtonSetup ()
    {
      Conn.Enabled                   = false;
      startButton.Enabled            = false;
      stopButton.Enabled             = true;
      StatusButton.Enabled           = true;
      bt_preview.Enabled             = false;
      setFileNameDescButton.Enabled  = false;
      ConfigMenuItem.Enabled         = true;
      cameraLineButton.Enabled       = false;
      ReBootXportMenuItem.Enabled    = true;
      ReBootBatteryMenuItem.Enabled  = true;

      sipperBatteryPack.Enabled      = true;
      
      if  (circularBuff.InstrumentDataCollecting ())
        StatusButton.Text = "Stop Status";
      else
        StatusButton.Text = "Status";

      if (awaitingChangeBatteryCmd)
      {
        sipperBatteryPack.Enabled = false;
        sipperBatteryPack.RequestedBatteryEnabled = false;
      }
      else
      {
        sipperBatteryPack.Enabled = true;
        sipperBatteryPack.RequestedBatteryEnabled = true;
      }
      
    }  /* RecordingButtonSetup */





    private void  NotRecordingButtonSetup ()
    {
      Conn.Enabled                   = false;
      startButton.Enabled            = true;
      stopButton.Enabled             = false;
      StatusButton.Enabled           = true;
      bt_preview.Enabled             = true;
      setFileNameDescButton.Enabled  = true;
      ConfigMenuItem.Enabled         = true;
      cameraLineButton.Enabled       = true;
      ReBootXportMenuItem.Enabled    = true;
      ReBootBatteryMenuItem.Enabled  = true;
      sipperBatteryPack.Enabled      = true;

      if  (circularBuff.InstrumentDataCollecting ())
        StatusButton.Text = "Stop Status";
      else
        StatusButton.Text = "Status";

      if  (awaitingChangeBatteryCmd)
      {
        sipperBatteryPack.Enabled                 = false;
        sipperBatteryPack.RequestedBatteryEnabled = false;
      }
      else
      {
        sipperBatteryPack.Enabled                 = true;
        sipperBatteryPack.RequestedBatteryEnabled = true;
      }

    }  /* NotRecordingButtonSetup */






 
    //Exiting program
    private void Exit_Click (object sender, System.EventArgs e)
    {
      TurnOffAllButtons ();

      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("// Exit Button");
        logFile.Flush ();
      }

      DialogResult result = MessageBox.Show( "Please confirm that you want to exit the Sipper Interface.", "Confirm Exit", MessageBoxButtons.YesNo, MessageBoxIcon.Question );

      //cleaning up resources before shutting down	
      if  (result == DialogResult.Yes) 
      {
        CloseSipperConnection ();

        CloseExternalWindows ();

        Application.Exit ();  
      }

      EnableAppropriateButtons ();
    }  /* Exit_Click */



    //config menu click, setting the config file and current usage
    private void Config_Click (object sender, System.EventArgs e)
    {
      TurnOffAllButtons ();
      ProcessConfigForm ();
      EnableAppropriateButtons ();
    }

    

    //before the form closes, closing all threads and getting rid of resourses 
    private void Form1_Closing (object sender, System.ComponentModel.CancelEventArgs e)
    {
      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");      
        logFile.WriteLine ("// Closing Application   CurTime[" + System.DateTime.Now.ToString () +"]");
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");      
        logFile.Flush ();
      }

      CloseExternalWindows ();

      //Cleaning up the resources
      CloseSipperConnection ();

      //deleting temporary file
      if (File.Exists ("SipperConfig_temp.txt"))
      {	
        File.Delete( "SipperConfig_temp.txt" );
      }

      lock  (logFile)  
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("// Application Closed");
        logFile.WriteLine ("//");
        logFile.Close ();
      }
      logFile = null;
    }  /* Form1_Closing */




   
    private void  SetFileNameDescButtonClicked (object sender, System.EventArgs e)
    {
      TurnOffAllButtons ();

      logFile.WriteLine ("//");
      logFile.WriteLine ("//");
      logFile.WriteLine ("//SetFileNameDescButtonClicked    CurTime[" + System.DateTime.Now.ToString () +"]");
      logFile.WriteLine ("//");
      logFile.WriteLine ("//");
      logFile.Flush ();



      DialogResult  dr = MessageBox.Show ("Are you sure that you want to set the File Name ?",
                                          "Set File Name",
                                          MessageBoxButtons.YesNo
                                         );
      if  (dr == DialogResult.No)
      {
        logFile.WriteLine ("//SetFileNameDescButtonClicked    User Decided NOT TO SET FILE NAME.");
        logFile.Flush ();
        EnableAppropriateButtons ();
      }

      logFile.WriteLine ("//");
      logFile.WriteLine ("//");
      logFile.WriteLine ("//SetFileNameDescButtonClicked    CurTime[" + System.DateTime.Now.ToString () +"]");
      logFile.WriteLine ("//");
      logFile.WriteLine ("//");
      logFile.Flush ();

      SipperCommand cmd = SipperCommand.CreateSetFileNameCmd (sipperCmdsExecuted, 
                                                              sipperConfigRec.HeaderRec ().SipperFileName (),
                                                              sipperConfigRec.HeaderRec ().HeaderStr (), 
                                                              circularBuff, 
                                                              logFile
                                                             );
      circularBuff.QueueSipperCommand (cmd);

      EnableAppropriateButtons ();
      ConfigMenuItem.Enabled = false;
    }  /* SetFileNameDescButtonClicked */

    
    
    
    
    private  void  PromptConfigurationInfoUntilUpdated ()
    {
      ProcessConfigForm ();
      while  (!sipperConfigRec.LoadedFromConfigFile ())
      {
        DialogResult dr = MessageBox.Show (
          "You need to update configuration; Do you wish to do this now?", 
          "Invalid Configuration Data", 
          MessageBoxButtons.YesNo
          );

        if  (dr == DialogResult.Yes)
        {
          ProcessConfigForm ();
        }
        else
        {
          lock  (logFile)
          {
            logFile.WriteLine ("//");
            logFile.WriteLine ("// User opted not to update configuration data.");
            logFile.Flush ();
          }
          Close ();
          break;
        }
      }
    }  /* PromptConfigurationInfoUntilUpdated */

    
    
    
    private  void  ProcessConfigForm ()
    {
      SipperConfigForm  configForm = new SipperConfigForm (sipperConfigRec);
      configForm.ShowDialog ();
      if  (configForm.UserSavedDataToFile ())
        sipperFileName.Text = sipperConfigRec.HeaderRec ().SipperFileName ();
      configForm.Close ();
      configForm.Dispose ();
      configForm = null;
    }





    private  void  CompleteChangeBatteryCmd  (SipperCommand cmd)
    {
      awaitingChangeBatteryCmd = false;     // This way the EnableAppropriateButtons procedures 
                                            // knows it can enable 'sipperBatteryPack'.

      TurnOffAllButtons();
    }  /* CompleteChangeBatteryCmd */



    private  void  CompleteGetCameraParameters (SipperCommand cmd)
    {
      cameraParms = cmd.CameraParams ();
      if  (cameraParms != null)
        ScanRate.Text = cameraParms.ScanRate.ToString ("##,##0.0");
    }  /* CompleteGetCameraParameters */





    private  void  CompletedPreviewCmd (SipperCommand cmd)
    {
      if  (!cmd.ExecutedOk ())
      {
        return;
      }

      if  (cmd.PreviewData () == null)
      {
        MessageBox.Show ("Sipper did not return any daya to Preview.", "Sipper Preview Command", MessageBoxButtons.OK);
        return;
      }
      
      if  (cmd.PreviewData ().Length < 100)
      {
        MessageBox.Show ("Sipper did not return enough data to build a preview screen with.", "Sipper Preview Command", MessageBoxButtons.OK);
        return;
      }

      {
        // Save Preview data to disk.

        DateTime  dt = DateTime.Now;

        string  previewDir = OSservices.AddSlash (logDirectory) + "Previews";
        OSservices.CreateDirectory (previewDir);
        string  previwFileName = OSservices.AddSlash (previewDir) + 
                                 dt.Year.ToString   ("0000")  +
                                 dt.Month.ToString  ("00")    +
                                 dt.Day.ToString    ("00")    +  "-" +
                                 dt.Hour.ToString   ("00")    +
                                 dt.Minute.ToString ("00")    +
                                 dt.Second.ToString ("00")    +
                                 "-"                          +
                                 cmd.PreviewBlock ().ToString ("0000000000") + "-" + cmd.BlocksToRead ().ToString ("00000") +
                                 ".spr";

        FileStream   fs = null;
        BinaryWriter bw = null;

        try { fs = new FileStream (previwFileName, FileMode.Create);  bw = new BinaryWriter (fs);} 
        catch (Exception e)
        {
          MessageBox.Show ("Error Loging PreviewFile" + "\n\n" + e.ToString ());
          bw = null;
        }

        if  (bw != null)
        {
          bw.Write (cmd.PreviewData ());
          bw.Close ();
          bw = null;
          fs.Close ();
          fs = null;
        }
      }

      MemoryStream  ms = new MemoryStream (cmd.PreviewData ());
       
      try
      {
        SipperFileViewer.SipperFileViewer  previewer = new SipperFileViewer.SipperFileViewer (ms, "SipperInterface Preview", lastFileNameCreated);
        previewer.Show ();
        sipperPreviews.Add (previewer);
      }
      catch  (Exception e)
      {
        MessageBox.Show (e.ToString (), "Exception displaying preview image", MessageBoxButtons.OK);
      }
    }  /* CompletedPreviewCmd */




    private void  CompleteReBootBatteryCmd (SipperCommand cmd)
    {
      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");
        logFile.WriteLine ("// CompleteReBootBattery");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }
      CloseSipperConnection ();
      return;
    }  /* CompleteReBootBattery */




    private void  CompleteSetFileNameCmd (SipperCommand cmd)
    {
      EnableAppropriateButtons ();
      if  (cmd.ExecutedOk ())
      {
        sipperFileNameHsBeenSetAtLeastOnce = true;
        history.StartNewDataFile (sipperConfigRec.HeaderRec ().SipperFileName ());
      }
    }



    private  void  CompleteSetFileNameAndStartRecording (SipperCommand cmd)
    {
      if  (cmd.ExecutedOk ())
      {
        sipperFileNameHsBeenSetAtLeastOnce = true;
      
        statusRecording = true;
        SetCurrentSeqNum (cmd.FileNameCreated ());
        lastFileNameCreated = cmd.FileNameCreated ();

        history.StartNewDataFile (sipperConfigRec.HeaderRec ().SipperFileName ());
        
        lock  (logFile)
        {
          logFile.WriteLine ("//");
          logFile.WriteLine ("//StartButton  Completed   New Sipper File[" + cmd.FileNameCreated () + "]");
          logFile.WriteLine ("//");
          logFile.Flush ();
        }
      }
      else
      {
        MessageBox.Show ("Error trying to start recording\nSee session dialog for details.", "Record Failure", MessageBoxButtons.OK);
      }
    }  /* CompleteSetFileNameAndStartRecording */



    private  void  CompleteStartRecordingCmd (SipperCommand cmd)
    {
      if  (cmd.ExecutedOk ())
      {
        statusRecording = true;
        //  At this point we have the name of the file created by sipper.
        // We should be saving this somewhere.

        SetCurrentSeqNum (cmd.FileNameCreated ());

        lastFileNameCreated = cmd.FileNameCreated ();

        lock  (logFile)
        {
          logFile.WriteLine ("//");
          logFile.WriteLine ("//StartButton  Completed   New Sipper File[" + cmd.FileNameCreated () + "]");
          logFile.WriteLine ("//");
          logFile.Flush ();
        }
      }
      else
      {
        MessageBox.Show ("Error trying to start recording\nSee session dialog for details.", "Record Failure", MessageBoxButtons.OK);
      }
    }  /* CompleteStartRecordingCmd */


    private  void  SetCurrentSeqNum (string  fileNameCreated)
    {
      int  idx = fileNameCreated.LastIndexOf ('_');
      if  (idx > 0)
      {
        CurrentSeqNum.Text = fileNameCreated.Substring (idx + 1);
      }
      else
      {
        CurrentSeqNum.Text = "";
      }
    }  /* SetCurrentSeqNum */




    private  void  CompleteStopRecordingCmd (SipperCommand cmd)
    {
      if  (cmd.ExecutedOk())
      {
        lock  (logFile)
        {
          logFile.WriteLine ("//");
          logFile.WriteLine ("//stopButton_Click    recording succesfully stopped    CurTime[" + System.DateTime.Now.ToString () + "]");
          logFile.WriteLine ("//");
          logFile.Flush ();
        }
        statusBar.Text = "Recording has been terminated.";
        tb_Record.Text = "0.00 Mb/sec";
        statusRecording = false;
      }
      else 
      {
        lock  (logFile)
        {
          logFile.WriteLine ("//");
          logFile.WriteLine ("//stopButton_Click  *** ERROR TRYING TO STOP RECORDING ***     CurTime[" + System.DateTime.Now.ToString () + "]");
          logFile.WriteLine ("//");
          logFile.Flush ();
        }
        MessageBox.Show ("Error Occured when sending stop command.");
      }
      
    } /* CompleteStopRecordingCmd */




    private void  CompleteUserCmd (SipperCommand  cmd)
    {
      teminalWindow.AddTextToConsole (cmd.Result ());
    }  /* CompleteUserCmd */




    private  SipperCommand  GetNextCompletedSipperCommand ()
    {
      SipperCommand  cmd = null;
      
      lock  (sipperCmdsExecuted)
      {
        if  (sipperCmdsExecuted.Count > 0)
          cmd = sipperCmdsExecuted.Dequeue ();
      }
      
      return  cmd;
    }  /* GetNextCompletedSipperCommand */




    int  ticCount = 0;

    private void  timer1_Tick (object sender, EventArgs e)
    {
      if  ((ticCount % 100) == 0)
      {
        // Lets see if there are any preview screens that we can dispoe of.
        if  (sipperPreviews != null)
        {
          SipperFileViewer.SipperFileViewer  previewerThatIsDisposed = null;
          foreach  (SipperFileViewer.SipperFileViewer  previewer in sipperPreviews)
          {
            bool  disposed = previewer.IsDisposed;
            if  (disposed)
            {
              previewerThatIsDisposed = previewer;
              break;
            }
            
            //previewer.          }
          }

          if  (previewerThatIsDisposed != null)
          {
            sipperPreviews.Remove (previewerThatIsDisposed);
            previewerThatIsDisposed.Dispose ();
            previewerThatIsDisposed = null;
            GC.Collect ();
          }
        }
      }

      if  (circularBuff != null)
      {
        if  (!circularBuff.StatusConnected ())
        {
          // Connection to SIPPER must have been broken.

          CloseSipperConnection ();
          timer1.Enabled = false;
          TurnOffAllButtons ();
          MessageBox.Show ("Connection to SIPPER has been lost.");
          timer1.Enabled = true;
          enableAppropiateButtons = true;
        }
      }

      
      if  (circularBuff != null)
      {
        SipperCommand  cmd = null;
    
        if  (sipperCmdsExecuted != null)
        {
          cmd = GetNextCompletedSipperCommand ();
          if  (cmd != null)
          {
            switch  (cmd.Cmd ())
            {
              case SipperCommand.CmdTypes.ChangeBatteryCmd:                CompleteChangeBatteryCmd             (cmd);  break;
              case SipperCommand.CmdTypes.GetCameraParametersCmd:          CompleteGetCameraParameters          (cmd);  break;
              case SipperCommand.CmdTypes.ReBootBatteryCmd:                CompleteReBootBatteryCmd             (cmd);  break;
              case SipperCommand.CmdTypes.PreviewCmd:                      CompletedPreviewCmd                  (cmd); break;
              case SipperCommand.CmdTypes.SetFileNameCmd:                  CompleteSetFileNameCmd               (cmd);  break;
              case SipperCommand.CmdTypes.SetFileNameAndStartRecordingCmd: CompleteSetFileNameAndStartRecording (cmd);  break;
              case SipperCommand.CmdTypes.StartRecordingCmd:               CompleteStartRecordingCmd            (cmd);  break;
              case SipperCommand.CmdTypes.StopRecordingCmd:                CompleteStopRecordingCmd             (cmd);  break;
              case SipperCommand.CmdTypes.UserCmd:                         CompleteUserCmd                      (cmd);  break;
            }
            
            enableAppropiateButtons = true;
          }
        
        }

        if  (updateBatteryPack)
        {
          updateBatteryPack = false;
          currentDataRow.UpdateBatteryPack (sipperBatteryPack);
        }
      
        Pitch.Inclination = currentDataRow.Pitch ();
        Roll.Inclination  = currentDataRow.Roll  ();
      }

      UpdateTextWindow ();
      
      if  (enableAppropiateButtons)
      {
        if  (circularBuff == null)
          EnableAppropriateButtons ();
          
        else if  ((circularBuff.SipperCmdsPendingCount() < 1)  &&  (circularBuff.SipperCmdBeingExecuted () == null))
        {
          // It is OK to turn on the appropriate buttons.
          EnableAppropriateButtons ();
        }
      }

      ticCount++;
    }  /* timer1_Tick */


    
    
    
    public  void  ChangeBattery (int requestedBattery)
    {
      // A caller has requested that we change the active battery.  This can 
      // occur in the DataWindow program.  It will make a request from us to 
      // change batteries for it

      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");
        logFile.WriteLine ("// ChangeBattery   Current Time    ["  + DateTime.Now.ToString ()     +  "]");
        logFile.WriteLine ("//                 RequestedBattery["  + requestedBattery.ToString () +  "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      awaitingChangeBatteryCmd = true;  // When the the change Battery Command is completed this
                                        // flag will be set to false.  While it is true the
                                        // 'sipperBatteryPack' will be disabled.
      
      sipperBatteryPack.Enabled = false;
      circularBuff.QueueSipperCommand (SipperCommand.CreateChangeBatteryCmd (sipperCmdsExecuted, requestedBattery, circularBuff, logFile));
    }  /* ChangeBattery */



    private void  cameraLineButton_Click (object sender, EventArgs e)
    {
      TurnOffAllButtons ();

      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("// CameraLineButton:    CurTime[" + System.DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
      }

      CameraLineDisplay dis = new CameraLineDisplay (circularBuff, sipperConfigRec, logFile);
      dis.Show ();
      cameraLineDisplays.Add (dis);
      EnableAppropriateButtons ();
    }  /* cameraLineButton_Click */





    private  void  SendXPortCommand (string  cmdDesc,
                                     Socket  s,
                                     byte[]  buff
                                    )
    {
      int  ret;

      try
      {
        ret = s.Send (buff, 0, 9, SocketFlags.None);
      }
      catch (Exception e)
      {
        logFile.WriteLine ("// Error sending cmd[" + cmdDesc + "]  Msg[" + e.ToString () + "]");
        return;
      }

      Thread.Sleep (200);
      
      int  bytesToReceive = s.Available;

      try
      {
        s.ReceiveTimeout = 500;
        ret = s.Receive (buff, 0, bytesToReceive, 0);
      }
      catch (Exception e2)
      {
        logFile.WriteLine ("// Error Receiving Results for cmd[" + cmdDesc + "]  Msg[" + e2.ToString () + "]");
        return;
      }
    }  /* SendXPortCommand */






    private  void  ReBootSipperUsingXport ()
    {
      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");
        logFile.WriteLine ("// ReBootSipperUsingXport");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      CloseSipperConnection ();
      
      IPAddress ipAddress = sipperConfigRec.IpAddress ();
      int       ipPort    = 30704;  //   hex 0x77F0 

      //creating socket and connecting to IP address
      Socket  socket = new Socket (AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
      IPEndPoint EPhost = new IPEndPoint (ipAddress, ipPort);

      try
      {
        socket.Connect (EPhost);	//actual physical connection
      }
      catch (Exception  e)
      {
        lock  (logFile)
        {
          logFile.WriteLine ("// Could not connect,  error[" + e.ToString () + "]");
          logFile.Flush ();
        }
        
        MessageBox.Show (e.ToString (), "ReBootSipperUsingXport  Failed");
        return;
      }

      {

        byte[]  buff = new byte[1024];

        for  (int x = 0;  x < buff.Length;  x++)
          buff[x] = 0;


        buff[0] = 0x1B;
        buff[1] = 0x01;
        buff[5] = 0xFF;
        SendXPortCommand ("SetStates", socket, buff);

        buff[0] = 0x1A;
        buff[1] = 0x01;
        buff[5] = 0x00;
        SendXPortCommand ("SetActiveLevels", socket, buff);

        buff[0] = 0x19;
        buff[1] = 0x01;
        buff[5] = 0xff;
        SendXPortCommand ("SetDirections", socket, buff);

        Thread.Sleep (7);

        buff[0] = 0x1B;
        buff[1] = 0x01;
        buff[5] = 0xFF;
        SendXPortCommand ("SetStates", socket, buff);

        buff[0] = 0x1A;
        buff[1] = 0x01;
        buff[5] = 0xFF;
        SendXPortCommand ("SetActiveLevels", socket, buff);
      }
      
      try
      {
        socket.Disconnect (false);
      }
      catch  (Exception e2)
      {
        MessageBox.Show (e2.ToString (),  "Error trying to disconnect from Socket", MessageBoxButtons.OK);
      }
      socket = null;
    }  /* ReBootSipperUsingXport */



    private void ReBootXportMenuItem_Click (object sender, EventArgs e)
    {
      TurnOffAllButtons ();
    
      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");
        logFile.WriteLine ("// reBootXportButton_Click    Curent Time[" + DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      DialogResult dr = MessageBox.Show 
              ("Reboot SIPPER (Yes/No) ?",
               "You are requesting to reboot the SIPPER device",
               MessageBoxButtons.YesNo
              );

      if  (dr != DialogResult.Yes)
      {
        lock  (logFile)
        {
          logFile.WriteLine ("//");
          logFile.WriteLine ("// reBootXportButton_Click    User decided not to Re-Boot");
          logFile.WriteLine ("//");
          logFile.Flush ();
        }
      }
      else
      {
        bool  performReboot = true;

        if  (statusRecording)
        {
          DialogResult sr = MessageBox.Show 
                ("SIPPER is Recording,  still want to reboot (Yes/No) ?",
                 "SIPPER is still recordring",
                 MessageBoxButtons.YesNo
                );

          if (sr == DialogResult.Yes)
          {
            lock  (logFile)
            {
              logFile.WriteLine ("//");
              logFile.WriteLine ("// reBootXportButton_Click    SIPPER is recording but user still wants to re-boot.");
              logFile.WriteLine ("//");
              logFile.Flush ();
            }
            performReboot = true;
          }
          else
          {
            performReboot = false;
          }
        }

        if  (performReboot)
          ReBootSipperUsingXport ();
      }
      
      EnableAppropriateButtons ();
    }  /* reBootXportButton_Click */
    
    
    
    private void  ReBootBatteryMenuItem_Click (object sender, EventArgs e)
    {
      TurnOffAllButtons ();

      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("//");
        logFile.WriteLine ("// reBootBatteryButton_Click    Curent Time[" + DateTime.Now.ToString () + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      DialogResult dr = MessageBox.Show
              ("Reboot SIPPER using battery (Yes/No) ?",
               "You are requesting to reset battery.",
               MessageBoxButtons.YesNo
              );

      if  (dr != DialogResult.Yes)
      {
        lock  (logFile)
        {
          logFile.WriteLine ("//");
          logFile.WriteLine ("// reBootBatteryButton_Click    User decided not to Re-Boot");
          logFile.WriteLine ("//");
          logFile.Flush ();
        }
      }
      else
      {
        bool performReboot = true;

        if  (statusRecording)
        {
          DialogResult sr = MessageBox.Show
                ("SIPPER is Recording,  still want to reboot (Yes/No) ?",
                 "SIPPER is still recordring",
                 MessageBoxButtons.YesNo
                );

          if (sr == DialogResult.Yes)
          {
            lock  (logFile)
            {
              logFile.WriteLine ("//");
              logFile.WriteLine ("// reBootBatteryButton_Click    SIPPER is recording but user still wants to re-boot.");
              logFile.WriteLine ("//");
              logFile.Flush ();
              performReboot = true;
            }
          }
          else
          {
            performReboot = false;
          }
        }

        if  (performReboot)
          circularBuff.QueueSipperCommand (SipperCommand.CreateReBootBatteryCmd (sipperCmdsExecuted, circularBuff, logFile));
      }

    }  /* reBootBatteryButton_Click */



    private void  sipperBatteryPack_BatteryClicked ()
    {
      ChangeBattery (sipperBatteryPack.RequestedBattery);
    }



    private void teminalWindow1_NewCmdEntered ()
    {
      TurnOffAllButtons ();
      if  (circularBuff == null)
        return;
      
      string  cmdLine = teminalWindow.GetNextPendingCmd ();

      lock  (logFile)
      {
        logFile.WriteLine ("//");
        logFile.WriteLine ("// Submitting Command[" + cmdLine + "]");
        logFile.WriteLine ("//");
        logFile.Flush ();
      }

      circularBuff.QueueSipperCommand (SipperCommand.CreateUserCmd (sipperCmdsExecuted, cmdLine, circularBuff, logFile));
    }




    public  void  UpdateBatteryPack (bool  _updateBatteryPack)
    {
      updateBatteryPack = _updateBatteryPack;
    }

  }
}
