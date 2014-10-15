using System;
using System.Net;
using System.IO;
using System.Windows.Forms;
using PicesInterface;

namespace SipperFile
{
	/// <summary>
	/// Summary description for SipperConfigRec.
	/// </summary>
  public class SipperConfigRec
  {
    private  bool              loadedFromConfigFile        = false;
    private  SipperHeaderRec   headerRec                   = new SipperHeaderRec ();
    private  int               ipPort                      = 100;
    private  IPAddress         ipAddress                   = IPAddress.Parse ("192.168.0.22");
    private  bool              useDataBase                 = true;
    
    private  string            sipperConfigurationFileName = null;

    public SipperConfigRec()
    {
      OSservices.CreateDirectory (PicesSipperVariables.PicesConfigurationDirectory ());
      sipperConfigurationFileName = PicesSipperVariables.SipperConfigrationFileName ();
      Load ();
    }


    public  bool  LoadedFromConfigFile ()  {return loadedFromConfigFile;}

    public  SipperHeaderRec  HeaderRec         ()  {return  headerRec;}
    public  IPAddress        IpAddress         ()  {return  ipAddress;}
    public  string           IpAddressStr      ()  {return  ipAddress.ToString ();}
    public  int              IpPort            ()  {return  ipPort;}
    public  bool             UseDataBase       ()  {return  useDataBase;}

    public void  HeaderRec         (SipperHeaderRec  _headerRec)          {headerRec         = _headerRec;}
    public void  IpAddress         (IPAddress        _ipAddress)          {ipAddress         = _ipAddress;}
    public void  UseDataBase       (bool             _useDataBase)        {useDataBase       = _useDataBase;}

    public void  IpAddress (string _addrStr)
    {
      try  
      {
        ipAddress = IPAddress.Parse (_addrStr);
      }
      catch  (Exception)
      {
        ipAddress = new IPAddress (0);
      }
    }

    public  void  IpPort     (int    _ipPort)     {ipPort     = _ipPort;}


    public void  Load ()
    {
      string temp = "";
      headerRec = new SipperHeaderRec ();

      string  sipperInterfaceDir = PicesSipperVariables.SipperInterfaceDir ();

      //checking to see if the config file exist and if so load that first
      if (!File.Exists (sipperConfigurationFileName))
      {
        loadedFromConfigFile = false;
      }
      else
      {
        StreamReader Reader = new StreamReader (sipperConfigurationFileName);
				
        while ((temp=Reader.ReadLine ())!=null)
          ParseConfigTextLine (temp);

        Reader.Close ();
        loadedFromConfigFile = true;
      }

      return;
    }  /* Load */


    
    
    
    private void ParseConfigTextLine (string data)
    {
      string [] split;

      if  (headerRec == null)
        headerRec = new SipperHeaderRec ();

      split = data.Trim ().Split ('=');
				
      if (split.Length < 2)
        return;

      string  fieldName  = split[0].Trim ().ToLower ();
      string  fieldValue = split[1].Trim ();
      string  fieldValueLower = fieldValue.ToLower ();
      
      switch  (fieldName)       
      {         
        case "ipaddress":
          IpAddress (fieldValue);
          break;                  

        case "ipport":
          ipPort = int.Parse (fieldValue);
          break;

        case "usedatabase":
          useDataBase = ((fieldValueLower == "yes")  ||  (fieldValueLower == "y")  ||  (fieldValueLower == "true")  ||  (fieldValueLower == "t"));
          break;
          
        default:
          {
            bool  found = false;
            headerRec.ProcessNameAndDataStrings (fieldName, fieldValue, ref found);
          }
          break;
      }
    }  /* ParseConfigTextLine */




    public  void  Save ()
    {
      DateTime n = DateTime.Now;
      string  backUpName = OSservices.RemoveExtension (sipperConfigurationFileName) + 
                           "_" +
                           n.Year.ToString   ("0000") +
                           n.Month.ToString  ("00")   +
                           n.Day.ToString    ("00")   +
                           "-"                        +
                           n.Hour.ToString   ("00")   +
                           n.Minute.ToString ("00")   +
                           n.Second.ToString ("00")   +
                           ".txt";

      OSservices.RenameFile (sipperConfigurationFileName, backUpName);

      StreamWriter writer = new StreamWriter (sipperConfigurationFileName);
     
      writer.WriteLine ("IPAddress         = " + ipAddress.ToString ());
      writer.WriteLine ("IPPort            = " + ipPort.ToString ());
      writer.WriteLine ("UseDataBase       = " + (useDataBase ? "Yes" : "No"));

      if  (headerRec != null)
        headerRec.Save (writer);

      writer.Close ();
      loadedFromConfigFile = true;
    }  /* Save */
  }
}
