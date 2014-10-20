using System;
using System.Collections.Generic;
using System.Text;

namespace SipperDiskUtilities
{
  public  class SipperFileControlBlockList: List<SipperFileControlBlock>
  {
    public  SipperFileControlBlock  LookUpByName (string  fileName)
    {
      SipperFileControlBlock  foundFCB = null;
    
      foreach (SipperFileControlBlock fcb in this)
      {
        if  (fcb.FileName == fileName)
        {
          foundFCB = fcb;
          break;
        }
      }

      return  foundFCB;
    }   /* LookUpByName */
  



    public  SipperFileControlBlock  LookUpByFcbIndex (int  fcbIndex)
    {
      SipperFileControlBlock  foundFCB = null;
    
      foreach (SipperFileControlBlock fcb in this)
      {
        if  (fcb.FcbIndex == fcbIndex)
        {
          foundFCB = fcb;
          break;
        }
      }

      return  foundFCB;
    }   /* LookUpByFcbIndex */



  }  /* SipperFileControlBlockList */
}
