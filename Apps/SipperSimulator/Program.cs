using System;
using System.Collections.Generic;
using System.Windows.Forms;


// C:\Pices\SipperFiles\ETP2008\ETP2008_1F\ETP2008_1F_01.spr

namespace SipperSimulator
{
  static class Program
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main (string[]  args)
    {
      String  fileName = "";
      if  (args.Length > 0)
        fileName = args[0];

      Application.EnableVisualStyles ();
      Application.SetCompatibleTextRenderingDefault (false);
      Application.Run (new SipperSimulator (fileName));
    }
  }
}