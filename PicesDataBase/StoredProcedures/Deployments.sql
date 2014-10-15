delimiter ;


/**********************************************************************************************************************/
drop  procedure if exists DeploymentLoadList;
delimiter //
create procedure DeploymentLoadList (_cruiseName    char(10),
                                     _stationName   char(10)
                                    )
begin
  if  (_cruiseName = "")  then
    if  (_stationName = "")  then
        select * from  Deployments;
    else
        select * from  Deployments d  where  StationName = _stationName;
    end if;
  else
    if  (_stationName = "")  then
        select * from  Deployments d  where  CruiseName = _cruiseName;
    else
        select * from  Deployments d  where  (CruiseName = _cruiseName)  and  (StationName = _stationName);
    end if;
  end if;
end
//
delimiter ;


 
 
/**********************************************************************************************************************/
drop  procedure  if exists DeploymentLoad;
delimiter //
create procedure DeploymentLoad (_cruiseName     char(10),
                                 _stationName    char(10),
                                 _deploymentNum  char(4)
                                )
begin
  select * from  Deployments d  where  (d.CruiseName = _cruiseName)  and  (d.StationName = _stationName)  and  (d.DeploymentNum = _deploymentNum);
end
//
delimiter ;

 
 





/**********************************************************************************************************************/

drop  procedure  if exists  DeploymentInsert;
delimiter //
create procedure DeploymentInsert (_cruiseName           char(10),
                                   _stationName          char(10),
                                   _deploymentNum        char(4),
                                   _description          varchar(255),
                                   _dateTimeStart        datetime,
                                   _dateTimeEnd          datetime,
                                   _latitude             double,
                                   _longitude            double,
                                   _syncTimeStampActual  datetime,
                                   _syncTimeStampCTD     datetime,
                                   _syncTimeStampGPS     datetime,
                                   _cropLeft             int unsigned,
                                   _cropRight            int unsigned
                                  )
begin
  Insert into Deployments (CruiseName, StationName, DeploymentNum, Description, DateTimeStart, DateTimeEnd, Latitude, Longitude, SyncTimeStampActual, SyncTimeStampCTD, SyncTimeStampGPS, CropLeft, CropRight)
         values (_cruiseName, _stationName, _deploymentNum, _description, _dateTimeStart, _dateTimeEnd, _latitude, _longitude, _syncTimeStampActual, _syncTimeStampCTD, _syncTimeStampGPS, _cropLeft, _cropRight);
end
//
delimiter ;

 
 
 
 
 /**********************************************************************************************************************/
drop procedure  if exists  DeploymentUpdate;

delimiter //
create procedure DeploymentUpdate (_cruiseName           char(10),
                                   _stationName          char(10),
                                   _deploymentNum        char(4),
                                   _description          varchar(255),
                                   _dateTimeStart        datetime,
                                   _dateTimeEnd          datetime,
                                   _latitude             double,
                                   _longitude            double,
                                   _syncTimeStampActual  datetime,
                                   _syncTimeStampCTD     datetime,
                                   _syncTimeStampGPS     datetime,
                                   _cropLeft             int unsigned,
                                   _cropRight            int unsigned
                                  )
begin
  update Deployments
    set  Description         = _description, 
         DateTimeStart       = _dateTimeStart,
         DateTimeEnd         = _dateTimeEnd,
         Latitude            = _latitude, 
         Longitude           = _longitude,
         SyncTimeStampActual = _syncTimeStampActual,
         SyncTimeStampCTD    = _syncTimeStampCTD,
         SyncTimeStampGPS    = _syncTimeStampGPS,
         CropLeft            = _cropLeft,
         CropRight           = _cropRight
         
    where  (CruiseName = _cruiseName)  and  (StationName  = _stationName)  and  (DeploymentNum = _deploymentNum);
end
//
delimiter ;



 
 /**********************************************************************************************************************/
drop procedure if exists  DeploymentDelete;

delimiter //
create procedure DeploymentDelete (_cruiseName     char(10),
                                   _stationName    char(10),
                                   _deploymentNum  char(4)
                                  )
begin
  delete  from  Deployments
    where  (CruiseName = _cruiseName)  and  (StationName  = _stationName)  and  (DeploymentNum = _deploymentNum);
end
//
delimiter ;
