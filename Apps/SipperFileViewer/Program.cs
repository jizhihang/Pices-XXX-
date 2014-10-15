using System;
using System.Collections.Generic;
using System.Windows.Forms;

using  PicesInterface;


namespace SipperFileViewer
{
  static class Program
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main (string[]  args)
    {
#if DEBUG
      PicesMethods.StartMemoryLeakDetection ();
#endif

      Application.EnableVisualStyles ();
      Application.SetCompatibleTextRenderingDefault (false);

      String  fileName    = null;
      if  ((args.Length == 1)  &&  (args[0][0] != '-'))
        fileName = args[0];

      else
      {
        int  x = 0;
        while  (x < args.Length)
        {
          String  fieldName = "";
          String  fieldValue = "";

          if  (args[x][0] != '-')
          {
            if  (String.IsNullOrEmpty (fileName))
              fileName = args[x];
            x++;
          }
          else
          {
            fieldName = args[x];
            x++;

            if  ((x < args.Length)  &&  (args[x][0] != '-'))
            {
              fieldValue = args[x];
              x++;
            }

            fieldName = fieldName.ToUpper ();

            if  ((fieldName == "-NAME")  ||  (fieldName == "-FILENAME"))
              fileName = fieldValue;

            else
            {
              System.Console.WriteLine ("Invalid Parameter[" + fieldName + "]");
              return;
            }
          }
        }
      }
      

      if  (String.IsNullOrEmpty (fileName))
      {
        try
        {
          Application.Run (new SipperFileViewer ());
        }
        catch  (Exception e)
        {
          MessageBox.Show ("Exception Starting SipperFileViewer" + "\n\n" +
                           e.ToString ()
                          );
        }
      }
      else
      {
        try
        {
          Application.Run (new SipperFileViewer (fileName));
        }
        catch  (Exception e2)
        {
          MessageBox.Show ("Exception Starting SipperFileViewer" + "\n\n" + e2.ToString ());
        }
      }
    }
  }
}