using System;
using System.Collections.Generic;
using System.Text;
using PicesInterface;


namespace SipperFile
{
  class InstrumentDataReportBatteryMeter : InstrumentDataReport
  {
    //private  int      activeBattery = 0;
    private  float[]  batteryLevels   = null;
    private  int      numOfBatteries  = 4;





    public  int  NumOfBatteries
    {
      get  {return numOfBatteries;}
      set  {numOfBatteries = value;}
    }




    public  InstrumentDataReportBatteryMeter (InstrumentDataManager _manager,
                                              byte                  _deviceId
                                             )
      :
        base (_manager, _deviceId)
    {
      batteryLevels   = new float[numOfBatteries + 1];  // Will be '1' based, bats 1-4
    }




    
    override public void ParseTxtLine (string txtLine)
    {
      // We will be expecting 5 fields;  
      // <Current battery> <\t> <Bat 0 Voltage>  <\t> .... <\t> <Bat-3 Voltage> <\t> "Battery Statuses"
      //
      // ex   "4, 21.55, 21.57, 0.47, 24.62, DDLL"
    
      string[] fields = txtLine.Split (',');
      if  (fields.Length < (numOfBatteries + 1))
        return;
       
      if  (fields[0].Length != 1)
        return;
         
      char ch = fields[0][0];
      if  ((ch != '1') &&  (ch != '2') &&  (ch != '3')  &&  (ch != '4'))
        return;

      int activeBatery = PicesKKStr.StrToInt (fields[0]);
      int  x;
      for  (x = 1;  x <= numOfBatteries;  x++)
      {
        batteryLevels[x]   = PicesKKStr.StrToFloat (fields[x]);
      }
      
      string  batteryStatusStr = "";
      if  (fields.Length > (numOfBatteries + 1))
        batteryStatusStr = fields[numOfBatteries + 1];

      Manager ().CurrentDataRow( ).UpdateSipperBatteries (activeBatery, batteryLevels, batteryStatusStr);
    }  /* ParseTxtLine */
  }
}
