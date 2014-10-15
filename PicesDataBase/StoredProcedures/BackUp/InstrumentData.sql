delimiter ;

drop procedure  if exists InstrumentDataInsert;

delimiter //

create procedure  InstrumentDataInsert 
      (in _SipperFileName       varChar(48), 
       in _ScanLine             int       unsigned, 
       in _ByteOffset           bigint    unsigned,
       in _CTDDateTime          datetime, 
       in _Latitde              double, 
       in _Longitude            double,
       in _CTDBattery           float,
       in _Conductivity         float,
       in _Density              float,
       in _Depth                float,
       in _FlowRate1            float,
       in _FlowRate2            float,
       in _Fluorescence         float,
       in _FluorescenceSensor   float,
       in _Oxygen               float,
       in _OxygenSensor         float,
       in _Pressure             float,
       in _RecordRate           float,
       in _Salinity             float,
       in _SoundVelocity        float,
       in _Temperature          float,
       in _Transmisivity        float,
       in _TransmisivitySensor  float,
       in _Turbidity            float,
       in _TurbiditySensor      float,
       in _Pitch                float,
       in _Roll                 float
      )

begin
  declare  _sipperFileId  int  unsigned  default 0;
  
  set  _sipperFileId = (select  sf.SipperFileId  from SipperFiles sf  where  sf.SipperFileName = _sipperFileName);
  
  insert into  InstrumentData  
              (SipperFileId,     ScanLine,            ByteOffset, 
               CTDDateTime,      Latitde,             Longitude,
               CTDBattery,       Conductivity,        Density, 
               Depth,            FlowRate1,           FlowRate2,
               Fluorescence,     FluorescenceSensor,  Oxygen,         
               OxygenSensor,     Pressure,            RecordRate,       
               Salinity,         SoundVelocity,       Temperature,      
               Transmisivity,    TransmisivitySensor, Turbidity,        
               TurbiditySensor,  Pitch,               Roll
              )
          values 
             (
               _sipperFileId, 
               _ScanLine,         
               _ByteOffset, 
               _CTDDateTime,
               _Latitde,
               _Longitude,
               _CTDBattery,
               _Conductivity,
               _Density, 
               _Depth,
               _FlowRate1,
               _FlowRate2,
               _Fluorescence,
               _FluorescenceSensor,
               _Oxygen,
               _OxygenSensor,
               _Pressure,
               _RecordRate,
               _Salinity,
               _SoundVelocity,
               _Temperature,
               _Transmisivity,
               _TransmisivitySensor,
               _Turbidity,
               _TurbiditySensor,
               _Pitch,
               _Roll
             );
end
//
delimiter ;


drop procedure  if exists InstrumentDataInsert2;

delimiter //

create procedure  InstrumentDataInsert2 
      (in _SipperFileName       varChar(48), 
       in _ScanLine             int       unsigned, 
       in _ByteOffset           bigint    unsigned,
       in _CTDDateTime          datetime, 
       in _Latitde              double, 
       in _Longitude            double,
       in _CTDBattery           float,
       in _Conductivity         float,
       in _Density              float,
       in _Depth                float,
       in _FlowRate1            float,
       in _FlowRate2            float,
       in _Fluorescence         float,
       in _FluorescenceSensor   float,
       in _CdomFluorescence       float,
       in _CdomFluorescenceSensor float,
       in _Oxygen               float,
       in _OxygenSensor         float,
       in _Pressure             float,
       in _RecordRate           float,
       in _Salinity             float,
       in _SoundVelocity        float,
       in _Temperature          float,
       in _Transmisivity        float,
       in _TransmisivitySensor  float,
       in _Turbidity            float,
       in _TurbiditySensor      float,
       in _Pitch                float,
       in _Roll                 float
      )

begin
  declare  _sipperFileId  int  unsigned  default 0;
  
  set  _sipperFileId = (select  sf.SipperFileId  from SipperFiles sf  where  sf.SipperFileName = _sipperFileName);
  
  insert into  InstrumentData  
              (SipperFileId,     ScanLine,            ByteOffset, 
               CTDDateTime,      Latitde,             Longitude,
               CTDBattery,       Conductivity,        Density, 
               Depth,            FlowRate1,           FlowRate2,
               Fluorescence,     FluorescenceSensor,  
               CdomFluorescence, CdomFluorescenceSensor,  
               Oxygen,         
               OxygenSensor,     Pressure,            RecordRate,       
               Salinity,         SoundVelocity,       Temperature,      
               Transmisivity,    TransmisivitySensor, Turbidity,        
               TurbiditySensor,  Pitch,               Roll
              )
          values 
             (
               _sipperFileId, 
               _ScanLine,         
               _ByteOffset, 
               _CTDDateTime,
               _Latitde,
               _Longitude,
               _CTDBattery,
               _Conductivity,
               _Density, 
               _Depth,
               _FlowRate1,
               _FlowRate2,
               _Fluorescence,
               _FluorescenceSensor,
               _CdomFluorescence,
               _CdomFluorescenceSensor,
               _Oxygen,
               _OxygenSensor,
               _Pressure,
               _RecordRate,
               _Salinity,
               _SoundVelocity,
               _Temperature,
               _Transmisivity,
               _TransmisivitySensor,
               _Turbidity,
               _TurbiditySensor,
               _Pitch,
               _Roll
             );
end
//
delimiter ;




/**********************************************************************************************************************/
drop procedure  if exists InstrumentDataGetByScanLine;

delimiter //
create procedure   InstrumentDataGetByScanLine (in _sipperFileName   varChar(48),
                                                in _scanLine         int unsigned
                                               )
begin
  declare  _sipperFileId     int unsigned default 0;
  declare  _startScanLine   int unsigned default 0;

  if  _scanLine > 8192  then
    set _startScanLine = _scanLine - 8192;
  else
    set _startScanLine = 0;
  end if;

  set _sipperFileId = (select  sf.SipperFileId  from  SipperFiles sf  where  sf.SipperFileName = _sipperFileName);

  select *  from  InstrumentData id
            where  (id.SipperFileId = _sipperFileId)  and  (id.ScanLine >= _startScanLine)  and  (id.ScanLine <= _scanLine)
            order by id.ScanLine  desc
            limit 1;  

end
//
delimiter ;






/**********************************************************************************************************************/
drop procedure  if exists InstrumentDataDeleteOneSipperFile;

delimiter //
create procedure   InstrumentDataDeleteOneSipperFile (in _sipperFileName   varChar(48))
begin
  declare  _sipperFileId   int unsigned default 0;
  set _sipperFileId = (select  sf.SipperFileId  from  SipperFiles sf  where  sf.SipperFileName = _sipperFileName);

  delete from  InstrumentData  where  SipperFileId = _sipperFileId;
end
//
delimiter ;








/**********************************************************************************************************************/
drop procedure  if exists InstrumentDataLoadOneSipperFile;

delimiter //
create procedure   InstrumentDataLoadOneSipperFile (in _sipperFileName   varChar(48))
begin
  declare  _sipperFileId   int unsigned default 0;
  set _sipperFileId = (select  sf.SipperFileId  from  SipperFiles sf  where  sf.SipperFileName = _sipperFileName);

  select * from InstrumentData id  where  id.SipperFileId = _sipperFileId;
end
//
delimiter ;











/**********************************************************************************************************************/
drop procedure if exists  InstrumentDataGetMidPointOfDeployment;

delimiter //
create procedure  InstrumentDataGetMidPointOfDeployment (in  _cruiseName      varChar(64),
                                                         in  _stationName     varChar(10),
                                                         in  _deploymentNum   varChar(4)
                                                        )
begin
  declare _maxDepth     float     default 0.0;
  declare _midPoint     datetime;


  set  _maxDepth = (select max(id.Depth)
                        from InstrumentData id
                        join (SipperFiles sf)  on(sf.SipperFileId = id.SipperFileId)
                        where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum
                              and  id.CTDDateTime > "2000-01-01 00:01:01"
                              and  id.Depth        < 1000
                              and  id.Temperature  > 0.0   and  id.Temperature   < 40.0
                              and  id.Salinity     > 20    and  id.Salinity      < 40.0
                              and  id.Density      > 13    and  id.Density       < 40.0
                              and  id.Fluorescence > -2    and  id.Fluorescence  < 80.0
                   );

  set _midPoint = (select max(id.CTDDateTime)
                        from InstrumentData id
                        join (SipperFiles sf)  on(sf.SipperFileId = id.SipperFileId)
                        where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum
                              and  floor(id.Depth * 100) = floor(_maxDepth * 100)
                              and  id.CTDDateTime  > "2000-01-01 00:01:01"
                              and  id.Depth        < 1000
                              and  id.Temperature  > 0.0   and  id.Temperature   < 40.0
                              and  id.Salinity     > 20    and  id.Salinity      < 40.0
                              and  id.Density      > 13    and  id.Density       < 40.0
                              and  id.Fluorescence > -2    and  id.Fluorescence  < 80.0
                   );


  select  _midPoint  as "MidPoint";
end;
//

delimiter ;










/**********************************************************************************************************************/
drop procedure if exists  InstrumentDataByUpAndDownCast;

delimiter //
create procedure  InstrumentDataByUpAndDownCast (in  _cruiseName      varChar(64),
                                                 in  _stationName     varChar(10),
                                                 in  _deploymentNum   varChar(4),
                                                 in  _className       varChar(64),
                                                 in  _depthBinSize    int unsigned
                                                )
begin
  declare _classId      int       default 0;
  declare _midPoint     datetime;

  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else
    set  _classId = -1;
  end if;

  set  _midPoint = InstrumentDataGetMidPoint(_cruiseName, _stationName, _deploymentNum);

  drop  temporary table if exists  ImageCountStats;

  create TEMPORARY table ImageCountStats
   (
     UpCast           bool   not null,
     DepthIdx         int    not null,
     Depth            float  not null,
     ImageCount       int    not null,
     TotalPixelCount  int    not null,
     Less500          int    not null,
     Less1000         int    not null,
     Less5000         int    not null,
     Less10000        int    not null,
     More10000        int    not null
   );

  INSERT into ImageCountStats
      select (id.CTDDateTime >= _midPoint)                     as upCast,
              floor(i.depth / _depthBinSize)                   as bucketIdx,
             (floor(i.depth / _depthBinSize) * _depthBinSize)  as bucketDepth,
             Count(i.ImageId)                                  as ImageCount,
             sum(i.PixelCount)                                 as totalPixelCount,
             sum(i.PixelCount <=   500.0)                                   as  Less500,
             sum((i.PixelCount >   500.0)  and  (i.PixelCount <=  1000.0))  as  Less1000,
             sum((i.PixelCount >  1000.0)  and  (i.PixelCount <=  5000.0))  as  Less5000,
             sum((i.PixelCount >  5000.0)  and  (i.PixelCount <= 10000.0))  as  Less10000,
             sum((i.PixelCount >  10000.0))                                 as  More10000
             
          from Images i
          join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
          join (InstrumentData id)  on  (id.SipperFileId = i.SipperFileId  and  id.ScanLine = (floor(i.TopLeftRow / 4096) * 4096))
          where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum
                 and  ((_classId < 0)  or  (i.Class1Id = _classId))
          group by (id.CTDDateTime >= _midPoint), floor(i.depth / _depthBinSize);



  select (id.CTDDateTime >= _midPoint)                       as upCast,
          floor(id.depth /  _depthBinSize)                   as bucketIdx,
         (floor(id.depth /  _depthBinSize) * _depthBinSize)  as bucketDepth,
         count(*) * 4096   as NumScanLines,
         max(id.CTDDateTime)          as  CTDDateTime,
         ics.ImageCount               as  ImageCont,
         ics.TotalPixelCount          as  TotalPixelCount,
         ics.Less500                  as  "<=500",
         ics.Less1000                 as  "<=1000",
         ics.less5000                 as  "<=5000",
         ics.less10000                as  "<=10000",
         ics.More10000                as  ">10000",
         avg(id.FlowRate1)            as  FlowRate1,           stddev(id.FlowRate1)            as  FlowRate1SD,
         avg(id.Density)              as  Density,             stddev(id.Density)              as  DensitySD,
         avg(id.Fluorescence)         as  Fluorescence,        stddev(id.Fluorescence)         as  FluorescenceSD,
         avg(id.FluorescenceSensor)   as  FluorescenceSensor,  stddev(id.FluorescenceSensor)   as  FluorescenceSensorSD,
         avg(id.Oxygen)               as  Oxygen,              stddev(id.Oxygen)               as  OxygenSD,
         avg(id.OxygenSensor)         as  OxygenSensor,        stddev(id.OxygenSensor)         as  OxygenSensorSD,
         avg(id.Pressure)             as  Pressure,            stddev(id.Pressure)             as  PressureSD,
         avg(id.RecordRate)           as  RecordRate,          stddev(id.RecordRate)           as  RecordRateSD,
         avg(id.Salinity)             as  Salinity,            stddev(id.Salinity)             as  SalinitySD,
         avg(id.SoundVelocity)        as  SoundVelocity,       stddev(id.SoundVelocity)        as  SoundVelocitySD,
         avg(id.Temperature)          as  Temperature,         stddev(id.Temperature)          as  TemperatureSD,
         avg(id.Transmisivity)        as  Transmisivity,       stddev(id.Transmisivity)        as  TransmisivitySD,
         avg(id.TransmisivitySensor)  as  TransmisivitySensor, stddev(id.TransmisivitySensor)  as  TransmisivitySensorSD,
         avg(id.Turbidity)            as  Turbidity,           stddev(id.Turbidity)            as  TurbiditySD,
         avg(id.TurbiditySensor)      as  TurbiditySensor,     stddev(id.TurbiditySensor)      as  TurbiditySensorSD

      from InstrumentData id
      join (SipperFiles sf)            on  (sf.SipperFileId = id.SipperFileId)
      left join (ImageCountStats ics)  on  (ics.UpCast = (id.CTDDateTime >= _midPoint)  and  ics.Depth = (floor(id.depth / _depthBinSize) * _depthBinSize))
      where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum
        and  id.Depth  < 1000
        and  id.Temperature  > 0.0  and  id.Temperature   < 40.0
        and  id.Salinity     > 20   and  id.Salinity      < 40.0
        and  id.Density      > 13   and  id.Density       < 40.0
        and  id.Fluorescence > -2   and  id.Fluorescence  < 80.0
      group by (id.CTDDateTime >= _midPoint), floor(id.depth / _depthBinSize);

end;
//

delimiter ;



/**********************************************************************************************************************/
drop procedure if exists  InstrumentDataByUpAndDownCastUnfiltered;


delimiter //
create procedure InstrumentDataByUpAndDownCastUnfiltered (in  _cruiseName      varChar(64),
                                                          in  _stationName     varChar(10),
                                                          in  _deploymentNum   varChar(4),
                                                          in  _className       varChar(64),
                                                          in  _depthBinSize    int unsigned
                                                        )
begin
  declare _classId      int       default 0;
  declare _midPoint     datetime;

  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else
    set  _classId = -1;
  end if;

  set  _midPoint = InstrumentDataGetMidPoint(_cruiseName, _stationName, _deploymentNum);

  drop  temporary table if exists  ImageCountStats;


  create TEMPORARY table ImageCountStats
   (
     UpCast           bool   not null,
     DepthIdx         int    not null,
     Depth            float  not null,
     ImageCount       int    not null,
     TotalPixelCount  int    not null,
     Less500          int    not null,
     Less1000         int    not null,
     Less5000         int    not null,
     Less10000        int    not null,
     More10000        int    not null
   );

  INSERT into ImageCountStats
      select (id.CTDDateTime >= _midPoint)                     as upCast,
              floor(i.depth / _depthBinSize)                   as bucketIdx,
             (floor(i.depth / _depthBinSize) * _depthBinSize)  as bucketDepth,
             Count(i.ImageId)                                  as ImageCount,
             sum(i.PixelCount)                                 as totalPixelCount,
             sum(i.PixelCount <=   500.0)                                   as  Less500,
             sum((i.PixelCount >   500.0)  and  (i.PixelCount <=  1000.0))  as  Less1000,
             sum((i.PixelCount >  1000.0)  and  (i.PixelCount <=  5000.0))  as  Less5000,
             sum((i.PixelCount >  5000.0)  and  (i.PixelCount <= 10000.0))  as  Less10000,
             sum((i.PixelCount >  10000.0))                                 as  More10000
             
          from Images i
          join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
          join (InstrumentData id)  on  (id.SipperFileId = i.SipperFileId  and  id.ScanLine = (floor(i.TopLeftRow / 4096) * 4096))
          where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum
                 and  ((_classId < 0)  or  (i.Class1Id = _classId))
          group by (id.CTDDateTime >= _midPoint), floor(i.depth / _depthBinSize);



  select (id.CTDDateTime >= _midPoint)                       as upCast,
          floor(id.depth /  _depthBinSize)                   as bucketIdx,
         (floor(id.depth /  _depthBinSize) * _depthBinSize)  as bucketDepth,
         count(*) * 4096   as NumScanLines,
         max(id.CTDDateTime)          as  CTDDateTime,
         ics.ImageCount               as  ImageCont,
         ics.TotalPixelCount          as  TotalPixelCount,
         ics.Less500                  as  "<=500",
         ics.Less1000                 as  "<=1000",
         ics.less5000                 as  "<=5000",
         ics.less10000                as  "<=10000",
         ics.More10000                as  ">10000",
         avg(id.FlowRate1)            as  FlowRate1,           stddev(id.FlowRate1)            as  FlowRate1SD,
         avg(id.Density)              as  Density,             stddev(id.Density)              as  DensitySD,
         avg(id.Fluorescence)         as  Fluorescence,        stddev(id.Fluorescence)         as  FluorescenceSD,
         avg(id.FluorescenceSensor)   as  FluorescenceSensor,  stddev(id.FluorescenceSensor)   as  FluorescenceSensorSD,
         avg(id.Oxygen)               as  Oxygen,              stddev(id.Oxygen)               as  OxygenSD,
         avg(id.OxygenSensor)         as  OxygenSensor,        stddev(id.OxygenSensor)         as  OxygenSensorSD,
         avg(id.Pressure)             as  Pressure,            stddev(id.Pressure)             as  PressureSD,
         avg(id.RecordRate)           as  RecordRate,          stddev(id.RecordRate)           as  RecordRateSD,
         avg(id.Salinity)             as  Salinity,            stddev(id.Salinity)             as  SalinitySD,
         avg(id.SoundVelocity)        as  SoundVelocity,       stddev(id.SoundVelocity)        as  SoundVelocitySD,
         avg(id.Temperature)          as  Temperature,         stddev(id.Temperature)          as  TemperatureSD,
         avg(id.Transmisivity)        as  Transmisivity,       stddev(id.Transmisivity)        as  TransmisivitySD,
         avg(id.TransmisivitySensor)  as  TransmisivitySensor, stddev(id.TransmisivitySensor)  as  TransmisivitySensorSD,
         avg(id.Turbidity)            as  Turbidity,           stddev(id.Turbidity)            as  TurbiditySD,
         avg(id.TurbiditySensor)      as  TurbiditySensor,     stddev(id.TurbiditySensor)      as  TurbiditySensorSD

      from InstrumentData id
      join (SipperFiles sf)            on  (sf.SipperFileId = id.SipperFileId)
      left join (ImageCountStats ics)  on  (ics.UpCast = (id.CTDDateTime >= _midPoint)  and  ics.Depth = (floor(id.depth / _depthBinSize) * _depthBinSize))
      where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum
        and  id.Depth  < 1000
        and  id.Temperature  > 0.0  and  id.Temperature   < 40.0
      group by (id.CTDDateTime >= _midPoint), floor(id.depth / _depthBinSize);

end;
//

delimiter ;









/**********************************************************************************************************************/
drop procedure if exists  InstrumentDataByUpAndDownCast2;

DELIMITER $$

CREATE DEFINER=`root`@`localhost` PROCEDURE `InstrumentDataByUpAndDownCast2`(in  _cruiseName      varChar(64),
                                                                             in  _stationName     varChar(10),
                                                                             in  _deploymentNum   varChar(4)
                                                                            )
begin
  declare _midPoint     datetime;

  set  _midPoint = InstrumentDataGetMidPoint(_cruiseName, _stationName, _deploymentNum);

  select sf.SipperFileName,
         id.ScanLine             as  ScanLine,
         (id.CTDDateTime >= _midPoint) as upCast,
         id.depth                as  Depth,
         id.CTDDateTime          as  CTDDateTime,
         id.FlowRate1            as  FlowRate1,           
         id.Density              as  Density,             
         id.Fluorescence         as  Fluorescence,        
         id.FluorescenceSensor   as  FluorescenceSensor,  
         id.Oxygen               as  Oxygen,              
         id.OxygenSensor         as  OxygenSensor,        
         id.Pressure             as  Pressure,            
         id.RecordRate           as  RecordRate,          
         id.Salinity             as  Salinity,            
         id.SoundVelocity        as  SoundVelocity,       
         id.Temperature          as  Temperature,         
         id.Transmisivity        as  Transmisivity,       
         id.TransmisivitySensor  as  TransmisivitySensor, 
         id.Turbidity            as  Turbidity,           
         id.TurbiditySensor      as  TurbiditySensor

      from InstrumentData id
      join (SipperFiles sf)            on  (sf.SipperFileId = id.SipperFileId)
      where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum
        and  id.Depth  < 1000
        and  id.Temperature  > 0.0  and  id.Temperature   < 40.0
      order by (id.CTDDateTime >= _midPoint), id.depth ;

end

$$

delimiter ;












/**********************************************************************************************************************/
drop procedure  if exists InstrumentDataByDepth;

delimiter //
create procedure  InstrumentDataByDepth (in  _cruiseName      varChar(10),
                                         in  _stationName     varChar(10),
                                         in  _deploymentNum   varchar(4),
                                         in  _className       varChar(64),
                                         in  _depthBinSize    int unsigned
                                        )
begin
  declare _classId      int       default 0;
  declare _maxDepth     float     default 0.0;
  declare _midPoint     datetime;

  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else
    set  _classId = -1;
  end if;


  drop  temporary table if exists  ImageCountStats;



  create TEMPORARY table ImageCountStats
   (
     CruiseName       varchar(10),
     StationName      varChar(10),
     deploymentNum    varChar(4),
     DepthIdx         int    not null,
     Depth            float  not null,
     ImageCount       int    not null,
     TotalPixelCount  int    not null,
     Less254          int    not null,
     Less1016         int    not null,
     Less2287         int    not null,
     Less4065         int    not null,
     More4065         int    not null,

     key TheKey (CruiseName asc, StationName asc, deploymentNum asc, DepthIdx asc)
   );


/* 254,572,1016,2287 and 4065 pixels */


  INSERT into ImageCountStats
      select sf.CruiseName,
             sf.StationName,
             sf.DeploymentNum,
             floor(i.depth / _depthBinSize)                    as bucketIdx,
             (floor(i.depth / _depthBinSize) * _depthBinSize)  as bucketDepth,
             Count(i.ImageId)                                  as ImageCount,
             sum(i.PixelCount)                                 as totalPixelCount,
             sum(i.PixelCount <=   254.0)                                  as  Less254,
             sum((i.PixelCount >   254.0)  and  (i.PixelCount <= 1016.0))  as  Less1016,
             sum((i.PixelCount >  1016.0)  and  (i.PixelCount <= 2287.0))  as  Less2287,
             sum((i.PixelCount >  2287.0)  and  (i.PixelCount <= 4065.0))  as  Less4065,
             sum((i.PixelCount >  4065))                                   as  More4065

          from Images i
          join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
          join (classes c)          on  (c.ClassId       = i.Class1Id)
          where  (sf.CruiseName = _cruiseName)  and  (sf.StationName = _stationName)  and  (sf.DeploymentNum = _deploymentNum)  and  (InStr (c.ClassName, _className) > 0)
          group by sf.CruiseName, sf.StationName, sf.DeploymentNum, floor(i.depth / _depthBinSize);



  select  sf.CruiseName,
          sf.StationName,
          sf.DeploymentNum,
          floor(id.depth /  _depthBinSize)                    as bucketIdx,
          (floor(id.depth /  _depthBinSize) * _depthBinSize)  as bucketDepth,
          count(*) * 4096   as NumScanLines,
          ics.ImageCount               as  ImageCont,
          ics.TotalPixelCount          as  TotalPixelCount,
          ics.Less254                  as  "<=254",
          ics.Less1016                 as  "<=1016",
          ics.Less2287                 as  "<=2287",
          ics.Less4065                 as  "<=4065",
          ics.More4065                 as  ">4065",
          avg(id.FlowRate1)            as  FlowRate1,           stddev(id.FlowRate1)            as  FlowRate1SD,
          avg(id.Density)              as  Density,             stddev(id.Density)              as  DensitySD,
          avg(id.Fluorescence)         as  Fluorescence,        stddev(id.Fluorescence)         as  FluorescenceSD,
          avg(id.FluorescenceSensor)   as  FluorescenceSensor,  stddev(id.FluorescenceSensor)   as  FluorescenceSensorSD,
          avg(id.Oxygen)               as  Oxygen,              stddev(id.Oxygen)               as  OxygenSD,
          avg(id.OxygenSensor)         as  OxygenSensor,        stddev(id.OxygenSensor)         as  OxygenSensorSD,
          avg(id.Pressure)             as  Pressure,            stddev(id.Pressure)             as  PressureSD,
          avg(id.RecordRate)           as  RecordRate,          stddev(id.RecordRate)           as  RecordRateSD,
          avg(id.Salinity)             as  Salinity,            stddev(id.Salinity)             as  SalinitySD,
          avg(id.SoundVelocity)        as  SoundVelocity,       stddev(id.SoundVelocity)        as  SoundVelocitySD,
          avg(id.Temperature)          as  Temperature,         stddev(id.Temperature)          as  TemperatureSD,
          avg(id.Transmisivity)        as  Transmisivity,       stddev(id.Transmisivity)        as  TransmisivitySD,
          avg(id.TransmisivitySensor)  as  TransmisivitySensor, stddev(id.TransmisivitySensor)  as  TransmisivitySensorSD,
          avg(id.Turbidity)            as  Turbidity,           stddev(id.Turbidity)            as  TurbiditySD,
          avg(id.TurbiditySensor)      as  TurbiditySensor,     stddev(id.TurbiditySensor)      as  TurbiditySensorSD

       from InstrumentData id
       join (SipperFiles sf)            on  (sf.SipperFileId = id.SipperFileId)
       left join (ImageCountStats ics)  on  ((ics.CruiseName = sf.CruiseName)  and  (ics.StationName = sf.StationName)  and  (ics.DeploymentNum = sf.DeploymentNum)  and  (ics.Depth = (floor(id.depth / _depthBinSize) * _depthBinSize)))
       where  (sf.CruiseName = _cruiseName)  and  (sf.StationName = _stationName)  and  (sf.DeploymentNum = _deploymentNum) 
         and  id.Depth  < 1000
         and  id.Temperature  > 0.0  and  id.Temperature   < 40.0
         and  id.Salinity     > 20   and  id.Salinity      < 40.0
         and  id.Density      > 18   and  id.Density       < 40.0
         and  id.Fluorescence > -2   and  id.Fluorescence  < 80.0
       group by sf.CruiseName, sf.StationName, sf.DeploymentNum, floor(id.depth / _depthBinSize);

end;
//

delimiter ;
      
      
      
      
      
/**********************************************************************************************************************/


drop procedure  if exists InstrumentDataByDepth_ZED;

delimiter //
create procedure  InstrumentDataByDepth_ZED (in  _cruiseName       varChar(10),
                                             in  _stationName      varChar(10),
                                             in  _deploymentNum1   varchar(4),
                                             in  _deploymentNum2   varchar(4),
                                             in  _deploymentNum3   varchar(4),
                                             in  _deploymentNum4   varchar(4),
                                             in  _className        varChar(64),
                                             in  _depthBinSize     int unsigned
                                            )
begin
  declare _classId      int       default 0;
  declare _maxDepth     float     default 0.0;
  declare _midPoint     datetime;

  declare _deploymentDesc  varchar(60);


  set  _deploymentDesc = concat(_deploymentNum1, "-", _deploymentNum2, "-", _deploymentNum3, "-", _deploymentNum4);

  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else
    set  _classId = -1;
  end if;


  drop  temporary table if exists  ImageCountStats;


  create TEMPORARY table ImageCountStats
   (
     CruiseName       varchar(10),
     StationName      varChar(10),
     deploymentNum    varChar(4),
     DepthIdx         int    not null,
     Depth            float  not null,
     ImageCount       int    not null,
     TotalPixelCount  int    not null,
     Less254          int    not null,
     Less1016         int    not null,
     Less2287         int    not null,
     Less4065         int    not null,
     More4065         int    not null,

     key TheKey (CruiseName asc, StationName asc, deploymentNum asc, DepthIdx asc)
   );


/* 254,572,1016,2287 and 4065 pixels */


  INSERT into ImageCountStats
      select sf.CruiseName,
             sf.StationName,
             sf.DeploymentNum,
             floor(i.depth / _depthBinSize)                    as bucketIdx,
             (floor(i.depth / _depthBinSize) * _depthBinSize)  as bucketDepth,
             Count(i.ImageId)                                  as ImageCount,
             sum(i.PixelCount)                                 as totalPixelCount,
             sum(i.PixelCount <=   254.0)                                  as  Less254,
             sum((i.PixelCount >   254.0)  and  (i.PixelCount <= 1016.0))  as  Less1016,
             sum((i.PixelCount >  1016.0)  and  (i.PixelCount <= 2287.0))  as  Less2287,
             sum((i.PixelCount >  2287.0)  and  (i.PixelCount <= 4065.0))  as  Less4065,
             sum((i.PixelCount >  4065))                                   as  More4065

          from Images i
          join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
          join (classes c)          on  (c.ClassId       = i.Class1Id)
          where  (sf.CruiseName      = _cruiseName)    and  
                 (sf.StationName     = _stationName)  and  
                 ((sf.DeploymentNum   = _deploymentNum1) or (sf.DeploymentNum   = _deploymentNum2)  or  (sf.DeploymentNum   = _deploymentNum3) or  (sf.DeploymentNum   = _deploymentNum4))  and  
                 (InStr (c.ClassName, _className) > 0)
          group by sf.CruiseName, sf.StationName, floor(i.depth / _depthBinSize);



  select  sf.CruiseName,
          sf.StationName,
          _deploymentDesc,
          floor(id.depth /  _depthBinSize)                    as bucketIdx,
          (floor(id.depth /  _depthBinSize) * _depthBinSize)  as bucketDepth,
          count(*) * 4096   as NumScanLines,
          ics.ImageCount               as  ImageCont,
          ics.TotalPixelCount          as  TotalPixelCount,
          ics.Less254                  as  "<=254",
          ics.Less1016                 as  "<=1016",
          ics.Less2287                 as  "<=2287",
          ics.Less4065                 as  "<=4065",
          ics.More4065                 as  ">4065",
          avg(id.FlowRate1)            as  FlowRate1,           stddev(id.FlowRate1)            as  FlowRate1SD,
          avg(id.Density)              as  Density,             stddev(id.Density)              as  DensitySD,
          avg(id.Fluorescence)         as  Fluorescence,        stddev(id.Fluorescence)         as  FluorescenceSD,
          avg(id.FluorescenceSensor)   as  FluorescenceSensor,  stddev(id.FluorescenceSensor)   as  FluorescenceSensorSD,
          avg(id.Oxygen)               as  Oxygen,              stddev(id.Oxygen)               as  OxygenSD,
          avg(id.OxygenSensor)         as  OxygenSensor,        stddev(id.OxygenSensor)         as  OxygenSensorSD,
          avg(id.Pressure)             as  Pressure,            stddev(id.Pressure)             as  PressureSD,
          avg(id.RecordRate)           as  RecordRate,          stddev(id.RecordRate)           as  RecordRateSD,
          avg(id.Salinity)             as  Salinity,            stddev(id.Salinity)             as  SalinitySD,
          avg(id.SoundVelocity)        as  SoundVelocity,       stddev(id.SoundVelocity)        as  SoundVelocitySD,
          avg(id.Temperature)          as  Temperature,         stddev(id.Temperature)          as  TemperatureSD,
          avg(id.Transmisivity)        as  Transmisivity,       stddev(id.Transmisivity)        as  TransmisivitySD,
          avg(id.TransmisivitySensor)  as  TransmisivitySensor, stddev(id.TransmisivitySensor)  as  TransmisivitySensorSD,
          avg(id.Turbidity)            as  Turbidity,           stddev(id.Turbidity)            as  TurbiditySD,
          avg(id.TurbiditySensor)      as  TurbiditySensor,     stddev(id.TurbiditySensor)      as  TurbiditySensorSD

       from InstrumentData id
       join (SipperFiles sf)            on  (sf.SipperFileId = id.SipperFileId)
       left join (ImageCountStats ics)  on  ((ics.CruiseName = sf.CruiseName)  and  (ics.StationName = sf.StationName)  and  (ics.DeploymentNum = sf.DeploymentNum)  and  (ics.Depth = (floor(id.depth / _depthBinSize) * _depthBinSize)))
       where  (sf.CruiseName = _cruiseName)
         and  (sf.StationName = _stationName)  
         and  ((sf.DeploymentNum = _deploymentNum1)  or  (sf.DeploymentNum = _deploymentNum2)  or  (sf.DeploymentNum = _deploymentNum3)  or  (sf.DeploymentNum = _deploymentNum4))
         and  id.Depth  < 1000
         and  id.Temperature  > 0.0  and  id.Temperature   < 40.0
         and  id.Salinity     > 20   and  id.Salinity      < 40.0
         and  id.Density      > 18   and  id.Density       < 40.0
         and  id.Fluorescence > -2   and  id.Fluorescence  < 80.0
       group by sf.CruiseName, sf.StationName, floor(id.depth / _depthBinSize);

end;
//

delimiter ;






/**********************************************************************************************************************/
drop procedure  if exists InstrumentDataUpdateLatitudeAndLongitude;

delimiter //

create procedure  InstrumentDataUpdateLatitudeAndLongitude 
      (
       in  _dateTimeStart          datetime, 
       in  _dateTimeEnd            datetime, 
       in  _latitude               double,
       in  _longitude              double
      )

begin
  update  InstrumentData id
    set  id.Latitde   = _latitude,
         id.Longitude = _longitude
         
    where  (CTDDateTime >= _dateTimeStart)  and  (CTDDateTime <= _dateTimeEnd);
end
//
delimiter ;




drop procedure  if exists InstrumentDataByTemporalOrder;
delimiter //


create procedure  InstrumentDataByTemporalOrder (in  _cruiseName      varChar(10),
                                                 in  _stationName     varChar(10),
                                                 in  _deploymentNum   varchar(4)
                                                 )
begin
  select id.* from InstrumentData id 
         join(SipperFiles sf) on(sf.SipperFileId = id.SipperFileId)
         where  ((sf.CruiseName    = _cruiseName)     or  (_cruiseName    = "")  or  (isnull(_cruiseName)))    and
                ((sf.StationName   = _stationName)    or  (_stationName   = "")  or  (isnull(_stationName)))   and
                ((sf.DeploymentNum = _deploymentNum)  or  (_deploymentNum = "")  or  (isnull(_deploymentNum))) and
                (id.ScanLine mod (32 * 1024) = 0)
         order by id.CTDDateTime, id.ScanLine;          
end
//
delimiter ;





drop procedure  if exists InstrumentDataGetVolumePerMeterDepth;
delimiter //


create procedure  InstrumentDataGetVolumePerMeterDepth (in  _sipperFileName   varChar(48),
                                                        in  _depthBinSize     float
                                                       )
begin
  declare  _sipperFileId   int unsigned default 0;
  declare  _scanRate       int unsigned default 0;
  declare  _secsPerRec     float default 0.0;

  set _sipperFileId = (select sf.SipperFileId  from  SipperFiles sf  where  sf.SipperFileName = _sipperFileName);
  set _scanRate     = (select sf.ScanRate      from  SipperFiles sf  where  sf.SipperFileName = _sipperFileName);
  
  if  _scanRate < 1  then
    set _scanRate = 24950;
  end if;
  
  set _secsPerRec = 4096 / _scanRate;

  select   Floor(id.depth / _depthBinSize)                  as BinId,
           Floor(id.depth / _depthBinSize) * _depthBinSize  as BinDepth,
           sum(id.FlowRate1 * _secsPerRec * 0.098 * 0.098)  as VolumeSampled
         from  InstrumentData id
         where id.SipperFileId = _sipperFileId
         group by Floor(id.depth / _depthBinSize)
         order by Floor(id.depth / _depthBinSize); 
end
//
delimiter ;






drop procedure  if exists InstrumentDataRetrieveGpsInfo;
delimiter //

create procedure  InstrumentDataRetrieveGpsInfo (in  _cruiseName      varChar(64),
                                                 in  _stationName     varChar(64),
                                                 in  _deploymentNum   varChar(4),
                                                 in  _timeInterval    int
                                                )
begin
  select  id.CTDDateTime                                                                         as  CTDDateTime, 
          Sec_To_Time(_timeInterval * Floor(Time_To_Sec(Time(id.CTDDateTime)) / _timeInterval))  as  GpsStartTime,
          sf.SipperFileId                                                                        as  SipperFileId,
          avg(id.ScanLine)                                                                       as  AvgScanLine,
          avg(id.Latitde)                                                                        as  AvgLatitude,
          avg(id.Longitude)                                                                      as  AvgLongitude,
          avg(FlowRate1)                                                                         as  AvgFlowRate
     from InstrumentData id
     join (SipperFiles sf) on (sf.SipperFileId = id.SipperFileId)
     where sf.CruiseName    = _cruiseName     and 
           sf.StationName   = _stationName    and 
           sf.deploymentNum = _deploymentNum  and 
           id.FlowRate1 > 0
     group by sf.CruiseName, sf.StationName, sf.DeploymentNum, Date(id.CTDDateTime), Floor(Time_To_Sec(Time(id.CTDDateTime)) / _timeInterval);
end
//
delimiter ;





drop procedure  if exists InstrumentDataBinByMeterDepth;
delimiter //

create procedure  InstrumentDataBinByMeterDepth (in  _cruiseName      varChar(10),
                                                 in  _stationName     varChar(10),
                                                 in  _deploymentNum   varchar(4),
                                                 in  _depthBinSize    float
                                                )
begin
  declare  _scanRate       float unsigned default 24950;
  declare  _secsPerRec     float default 0.0;
  declare  _midPoint       DateTime default null;

  set _scanRate = (select max(sf.ScanRate)  from  SipperFiles sf  
											where  (sf.CruiseName    = _cruiseName)   and
								                   (sf.StationName   = _stationName) and 
												   ((sf.DeploymentNum = _deploymentNum)  or (_deploymentNum = "")));
  if  _scanRate < 100  then
    set _scanRate = 29990;
  end if;
  
  
  set _secsPerRec = 4096.0 / _scanRate;
  set _midPoint = InstrumentDataGetMidPoint(_cruiseName, _stationName,_deploymentNum);


  select   (id.CtdDateTime < _midPoint)                     as DownCast,
           Floor(id.depth / _depthBinSize)                  as BinId,
           Floor(id.depth / _depthBinSize) * _depthBinSize  as BinDepth,
           4096 * count(id.ScanLine)                        as ScanLines,
           sum(id.FlowRate1 * _secsPerRec * 0.098 * 0.098)  as VolumeSampled,
           avg(id.Temperature)                              as TemperatureMean,
           avg(id.Salinity)                                 as SalinityMean,
           avg(id.Density)                                  as DenisityMean,
           avg(id.Fluorescence)                             as FluorescenceMean,
           avg(id.FluorescenceSensor)                       as FluorescenceSensorMean,
           avg(id.Oxygen)                                   as OxygenMean,
           avg(id.Depth)                                    as DepthMean,
           avg(id.Transmisivity)                            as TransmisivityMean,
           avg(id.Turbidity)                                as TurbidityMean,
           avg(id.CdomFluorescence)                         as CdomFluorescenceMean
           
         from  InstrumentData id
         join(SipperFiles sf)  on  (sf.SipperFileId = id.SipperFileId)
         where      (sf.CruiseName    = _cruiseName)  
               and  (sf.StationName   = _stationName) 
               and  ((sf.DeploymentNum = _deploymentNum)  or  (_deploymentNum = " "))
               and  id.CTDDateTime > "2000-01-01 00:01:01"
               and  id.CTDDateTime < "2030-01-01 00:01:01"
               and  id.CTDBattery   > 5.5   and  id.CTDBattery    < 14.0
               and  id.Depth        > 0.5   and  id.Depth         < 500
               and  id.Temperature  > 0.0   and  id.Temperature   < 40.0
               and  id.Salinity     > 20    and  id.Salinity      < 40.0
               and  id.Density      > 13    and  id.Density       < 40.0
               and  id.Fluorescence > -2    and  id.Fluorescence  < 80.0
               
         group by (id.CtdDateTime < _midPoint), Floor(id.depth / _depthBinSize)
         order by (id.CtdDateTime < _midPoint), Floor(id.depth / _depthBinSize); 
end
//
delimiter ;





/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop  function  if exists InstrumentDataGetMidPoint;

delimiter //
create function InstrumentDataGetMidPoint (_cruiseName      varChar(10),
                                           _stationName     varChar(10),
                                           _deploymentNum   varchar(4)
                                          )
  returns DateTime  deterministic

begin

  declare _maxDepth     float     default 0.0;
  declare _midPoint     datetime;

  set  _maxDepth = (select max(id.Depth)
                        from InstrumentData id
                        join (SipperFiles sf)  on(sf.SipperFileId = id.SipperFileId)
                        where      sf.CruiseName = _cruiseName  
                              and  sf.StationName = _stationName  
                              and  (sf.DeploymentNum = _deploymentNum   or  _deploymentNum = "")
                              and  id.CTDDateTime > "2000-01-01 00:01:01"
                              and  id.CTDDateTime < "2030-01-01 00:01:01"
                              and  id.CTDBattery   > 5.5   and  id.CTDBattery    < 14.0
                              and  id.Depth        > 0.5   and  id.Depth         < 500
                              and  id.Temperature  > 0.0   and  id.Temperature   < 40.0
                              and  id.Salinity     > 20    and  id.Salinity      < 40.0
                              and  id.Density      > 13    and  id.Density       < 40.0
                              and  id.Fluorescence > -2    and  id.Fluorescence  < 80.0
                   );

  set _midPoint = (select max(id.CTDDateTime)
                        from InstrumentData id
                        join (SipperFiles sf)  on(sf.SipperFileId = id.SipperFileId)
                        where      sf.CruiseName = _cruiseName  
							  and  sf.StationName = _stationName  
                              and  (sf.DeploymentNum = _deploymentNum  or  _deploymentNum  = "")
                              and  floor(id.Depth * 100) = floor(_maxDepth * 100)
                              and  id.CTDDateTime  > "2000-01-01 00:01:01"
                              and  id.CTDDateTime  < "2030-01-01 00:01:01"
                              and  id.CTDBattery   > 5.5   and  id.CTDBattery    < 14.0
                              and  id.Depth        > 0.5   and  id.Depth         < 500
                              and  id.Temperature  > 0.0   and  id.Temperature   < 40.0
                              and  id.Salinity     > 20    and  id.Salinity      < 40.0
                              and  id.Density      > 13    and  id.Density       < 40.0
                              and  id.Fluorescence > -2    and  id.Fluorescence  < 80.0
                   );

  return  _midPoint;
end;
//
delimiter ;
