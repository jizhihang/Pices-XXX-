using System;
using System.Collections.Generic;
using System.Windows.Forms;

using  PicesInterface;

namespace PicesCommander
{
  static class Program
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main()
    {
#if DEBUG
      PicesMethods.StartMemoryLeakDetection ();
#endif
      PicesDataBase.Initialization ();
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      Application.Run(new PicesCommander());
      GC.Collect ();
    }
  }
}