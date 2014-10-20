using System;
using System.Collections;
using PicesInterface;


namespace SipperFile
{
	/// <summary>
	/// Summary description for InstrumentDataReportInclination.
	/// </summary>
	public class InstrumentDataReportInclination: InstrumentDataReport
	{
    private  float  pitch = 0.0f;
    private  float  roll  = 0.0f;



		public  InstrumentDataReportInclination (InstrumentDataManager  _manager,
		                                         byte                   _deviceId
		                                        ):
          base (_manager, _deviceId)
		{
		}



    private  string[]  SplitUpIntoTokensStrings (string  s)
    {
      // This method will split up the report string 's' into several 'tokens'.  Leading white space willl
      // be eliminated
    
      ArrayList tokens = new ArrayList ();

      int  cp = 0;

      while  (cp < s.Length)
      {
        // Skip leading white space characters
        bool  whiteSpaceChar = true;
        while  ((cp < s.Length)  &&  (whiteSpaceChar))
        {
          char ch = s[cp];
          if  ((ch == ' ')  ||  (ch == '\t')  ||  (ch == '\n')  || (ch == '\r'))
            cp++;
          else
            whiteSpaceChar = false;
        }  

        if  (whiteSpaceChar)
          break;

        int  tokenStart = cp;
        int  tokenLen   = 0;

        while  ((cp < s.Length)  &&  (!whiteSpaceChar))
        {
          char ch = s[cp];
          if  ((ch == ' ')  ||  (ch == '\t')  ||  (ch == '\n')  || (ch == '\r'))
          {
            whiteSpaceChar = true;
          }
          else
          {
            tokenLen++;
            cp++;
          }
        }  

        tokens.Add (s.Substring (tokenStart, tokenLen));
      }

      return  (string[])tokens.ToArray (typeof (string));
    }  /* SplitUpIntoTokensStrings */



    override  public void  ParseTxtLine (string  txtLine)
    {
      string[]  fields = SplitUpIntoTokensStrings (txtLine);
      if  (fields.Length != 4)
        return;

      string  fieldName  = "";
      string  fieldValue = "";
      int     fieldNum   = 0;

      while  (fieldNum < fields.Length)
      {
        fieldName  = fields[fieldNum].ToLower ();  
        fieldNum++;

        if  (fieldNum < fields.Length)
        {
          fieldValue = fields[fieldNum];  
          fieldNum++;
        }
        else
        {
          fieldValue = "";
        }

        if  (fieldName == "r")
        {
          roll = PicesKKStr.StrToFloat (fieldValue);
        }
        else if  (fieldName == "p")
        {
          pitch = PicesKKStr.StrToFloat (fieldValue);
        }
      } 

      Manager ().CurrentDataRow ().UpdatePitchAndRoll (pitch, roll);
    }  /* ParseTxtLine */
	}
}
