delimiter ;


drop procedure  if exists GpsDataInsert;

delimiter //

create procedure  GpsDataInsert (in  _CruiseName          char(10),
                                 in  _UtcDateTime         datetime,
                                 in  _Latitude            double,
                                 in  _Longitude           double,
                                 in  _CourseOverGround    float,   /* True North */
                                 in  _SpeedOverGround     float    /* Knots      */
                                )

begin

  insert into GpsData(CruiseName, UtcDateTime, Latitude, Longitude, CourseOverGround, SpeedOverGround)
         values(_CruiseName, _UtcDateTime, _Latitude, _Longitude, _CourseOverGround, _SpeedOverGround);

end
//
delimiter ;










/**********************************************************************************************************************/
drop procedure   if exists  GpcDataQuery;

delimiter //

create procedure  GpcDataQuery (in  _CruiseName         char(10),
                                in  _UtcDateTimeStart   datetime,
                                in  _UtcDateTimeEnd     dateTime
                               )
begin
  select  * from  GPSData gd  
     where  (gd.CruiseName  =  _CruiseName)         and
            (gd.UtcDateTime >= _UtcDateTimeStart)  and
            (gd.UtcDateTime <= _UtcDateTimeStart);
end
//
delimiter ;







/**********************************************************************************************************************/
drop procedure   if exists  GpsDataDelete;

delimiter //

create procedure  GpsDataDelete (in  _CruiseName         char(10),
                                 in  _UtcDateTimeStart   datetime,
                                 in  _UtcDateTimeEnd     dateTime
                                )
begin
  delete from  GPSData
     where  (CruiseName  =  _CruiseName)        and
            (UtcDateTime >= _UtcDateTimeStart)  and
            (UtcDateTime <= _UtcDateTimeStart);
end
//
delimiter ;





/**********************************************************************************************************************/
drop procedure   if exists  GpsDataDelete;

delimiter //

create procedure  GpsDataDelete (in  _CruiseName         char(10),
                                 in  _UtcDateTimeStart   datetime,
                                 in  _UtcDateTimeEnd     dateTime
                                )
begin
  delete from  GPSData
     where  (CruiseName  =  _CruiseName)        and
            (UtcDateTime >= _UtcDateTimeStart)  and
            (UtcDateTime <= _UtcDateTimeStart);
end
//
delimiter ;



select distinct  id.SipperFileId, id.CTDDateTime from InstrumentData id
      join(SipperFiles sf)  on(sf.SipperFileId = id.SipperFileId)
      where sf.CruiseName="WB0814"
      order by id.CTDDateTime;


drop procedure   if exists  GpsDataGetEstimate;

delimiter //

create procedure  GpsDataGetEstimate (in  _CruiseName   char(10),
                                      in  _UtcDateTime  datetime,
                                      out _latitude     double,
                                      out _longitude    double
                                     )
begin
 
  declare  latEnd       double default 0.0;
  declare  longEnd      double default 0.0;
  declare  dateTimeEnd  datetime;


  declare  latStart       double default 0.0;
  declare  longStart      double default 0.0;
  declare  dateTimeStart  datetime;


  select  gd.Latitude, gd.longitude, gd.UtcDateTime into latStart, longStart, dateTimeStart
       from  GpsData gd
       where gd.CruiseName=_CruiseName  and  gd.UtcDateTime<=_UtcDateTime  order by gd.UtcDateTime desc
       limit 1;
  

  select  gd.Latitude, gd.longitude, gd.UtcDateTime   into latEnd, longEnd, dateTimeEnd
       from  GpsData gd
       where gd.CruiseName=_CruiseName  and  gd.UtcDateTime>=_UtcDateTime
       limit 1;

  if  dateTimeStart = dateTimeEnd  then
    select latStart, longStart  into _latitude, _longitude;
  else
    /* select  _UtcDateTime, latStart, longStart, dateTimeStart, latEnd, longEnd, dateTimeEnd; */

    set  @secsStart = to_seconds(dateTimeStart);
    set  @secsEnd   = to_seconds(dateTimeEnd);
    set  @secsPoint = to_seconds(_UtcDateTime);

    set  @secsRange = @secsEnd - @secsStart;

    set  @beforeRange = @secsPoint - @secsStart;
    set  @afterRange  = @secsEnd   - @secsPoint;

    set  @beforeFrac = @beforeRange / @secsRange;
    set  @afterFrac  = @afterRange  / @secsRange;
  
    /* select  @secsStart, @secsEnd, @secsPoint, @secsRange, @beforeFrac, @afterFrac; */

    set  @latDelta  = latEnd  - latStart;
    set  @longDelta = longEnd - longStart;
  
    set  @latPoint  = latStart  + @latDelta  * @beforeFrac;
    set  @longPoint = longStart + @longDelta * @beforeFrac;

    select @latPoint, @longPoint  into _latitude, _longitude;
  end if;

end
//
delimiter ;


