delimiter ;

drop procedure  if exists SipperFilesUpdateScanLinesAllFiles;

delimiter //
create procedure  SipperFilesUpdateScanLinesAllFiles ()
begin
  declare done             int default 0;
  declare _sipperFileName  varchar(48);
  declare _sipperFileId    int default 0;

  declare cur1 cursor for  select sf.SipperFileId, sf.SipperFileName
                                  from SipperFiles sf
                                  order by sf.SipperFileName;

  declare continue HANDLER for not found set done = 1;

  open cur1;

  repeat
    fetch  cur1 into _sipperFileId, _sipperFileName;
    if not done then
       update SipperFiles sf
              set sf.NumScanLines   = (select Max(id.ScanLine) from InstrumentData id
                                              where id.SipperFileId = _sipperFileId
                                      ),
                  sf.DateTimeStart  = (select Min(id.CTDDateTime)  from InstrumentData id
                                              where (id.SipperFileId = _sipperFileId)   and   (id.CTDDateTime > "1990-01-01 12:00:00")  and  (not isnull(id.CTDDateTime)) and
																							  (id.CTDBattery > 7)  and  (id.CTDBattery < 15)  and  (Time(id.CTDDateTime) <> "00:00:00")
                                      ),
                  sf.latitude       = (select id.Latitde  from InstrumentData id  
                                              where  (id.SipperFileId = _sipperFileId)   and   (id.Latitde != 0.0)  and  (not isnull(id.Latitde))  and  (id.ScanLine > 20000)
                                              order by id.ScanLine  
                                              limit 1
                                      ),
                  sf.Longitude      = (select id.Longitude  from InstrumentData id  
                                              where  (id.SipperFileId = _sipperFileId)   and   (id.Longitude != 0.0)  and  (not isnull(id.Longitude))  and  (id.ScanLine > 20000)
                                              order by id.ScanLine  
                                              limit 1
                                      )
               where sf.SipperFileId = _sipperFileId;

    end if;
  until done end repeat;

  close cur1;
end;
//
delimiter ;






/****************************************************************************************************************************/
drop procedure  if exists SipperFileFieldsFromInstrumentDataForDeployment;

delimiter //
create procedure  SipperFileFieldsFromInstrumentDataForDeployment 
                                    (in  _cruiseName    varChar(10),
                                     in  _stationName   varChar(10),
                                     in  _deploymentNum varChar(4)
                                    )
begin
  declare done             int default 0;
  declare _sipperFileName  varchar(48);
  declare _sipperFileId    int default 0;

  declare  _latitude       float default 0.0;
  declare  _longitude      float default 0.0;

  declare cur1 cursor for  select sf.SipperFileId, sf.SipperFileName
                                  from SipperFiles sf
                                  where sf.CruiseName    = _cruiseName     and
                                        sf.Stationname   = _stationName    and
                                        (sf.DeploymentNum = _deploymentNum  or  _deploymentNum = "")
                                  order by sf.SipperFileName;

  declare continue HANDLER for not found set done = 1;

  open cur1;

  repeat
    fetch  cur1 into _sipperFileId, _sipperFileName;
    if not done then
       update SipperFiles sf
              set sf.NumScanLines   = (select Max(id.ScanLine) from InstrumentData id
                                              where id.SipperFileId = _sipperFileId
                                      ),
                  sf.DateTimeStart  = (select Min(id.CTDDateTime)  from InstrumentData id
                                              where (id.SipperFileId = _sipperFileId)   and   (id.CTDDateTime > "1990-01-01 12:00:00")  and  (not isnull(id.CTDDateTime)) and
																							  (id.CTDBattery > 7)  and  (id.CTDBattery < 15)  and  (Time(id.CTDDateTime) <> "00:00:00")
                                      )
               where sf.SipperFileId = _sipperFileId;
    end if;
       
    set _latitude = 0;
    set _longitude = 0;
    
    set _latitude = (select  id.Latitde  from InstrumentData id  
                             where  (id.SipperFileId = _sipperFileId)   and   (id.Latitde != 0.0)  and  (not isnull(id.Latitde))  and  (id.ScanLine > 20000)
                             order by id.ScanLine  
                             limit 1
                    );
                    
    set  _longitude = (select id.Longitude  from InstrumentData id  
                                              where  (id.SipperFileId = _sipperFileId)   and   (id.Longitude != 0.0)  and  (not isnull(id.Longitude))  and  (id.ScanLine > 20000)
                                              order by id.ScanLine  
                                              limit 1
                      );
     
    if  (_latitude <> 0.0)  and  (_longitude <> 0.0)  then
       update SipperFiles sf
              set sf.latitude   = _latitude,
                  sf.Longitude  = _longitude
           where sf.SipperFileId = _sipperFileId;
    end if;
    
    
  until done end repeat;

  close cur1;
end;
//
delimiter ;






/****************************************************************************************************************************/
drop procedure  if exists SipperFilesUpdateExtractionStatus;

delimiter //
create procedure  SipperFilesUpdateExtractionStatus (in  _sipperFileName    varChar(48),
                                                     in  _extractionStatus  char(1)
                                                    )
begin
  update SipperFiles sf  
        set  sf.ExtractionStatus = _extractionStatus
        where  sf.SipperFileName = _sipperFileName;
end;
//
delimiter ;








/****************************************************************************************************************************/
drop procedure  if exists SipperFilesGetCTDDateTime;

delimiter //
create procedure  SipperFilesGetCTDDateTime (in  sipperFileName  varChar(48))
begin
  declare sipperFileId       int default 0;
  declare numScanLines       int default 0;
  declare CTDDateTimeStart   datetime  default null;
  declare CTDDateTimeEnd     datetime  default null;

  set sipperFileId = (select sf.SipperFileId  from SipperFiles sf  where sf.SipperFileName = sipperFileName);
  

  set  numScanLines  = (select Max(id.ScanLine) from InstrumentData id  where id.SipperFileId = sipperFileId);
                                              

  set  CTDDateTimeStart = (select Min(id.CTDDateTime)  from InstrumentData id
                                 where (id.SipperFileId = sipperFileId)   and   (id.CTDDateTime > "1990-01-01 12:00:00")  and  (not isnull(id.CTDDateTime))  and
                                                                                (id.CTDBattery > 7)  and  (id.CTDBattery < 15)  and  (Time(id.CTDDateTime) <> "00:00:00")
                                 );


  set  CTDDateTimeEnd   = (select Max(id.CTDDateTime)  from InstrumentData id
                               where (id.SipperFileId = sipperFileId)   and   (id.CTDDateTime > "1990-01-01 12:00:00")  and  (not isnull(id.CTDDateTime))  and
                                                                              (id.CTDBattery > 7)  and  (id.CTDBattery < 15)  and  (Time(id.CTDDateTime) <> "00:00:00")
                               );

  select  sipperFileId, sipperFileName, numScanLines, CTDDateTimeStart, CTDDateTimeEnd;

end;
//
delimiter ;





/************************************************************************************************************************/
drop procedure  if exists SipperFilesCalcStats;

delimiter //
create procedure  SipperFilesCalcStats (in  _sipperFileName  varChar(48))
begin
  /* Will return the number of scan lines and images in specified Sipper File */

  declare  _dateTimeStart    dateTime;
  declare  _numImages        int unsigned  default 0;
  declare  _numScanLines     int unsigned  default 0;
  declare  _sipperFileId     int unsigned  default 0;

  set _sipperFileId = (select sf.SipperFileId  from SipperFiles sf  where sf.SipperFileName = _sipperFileName);

  set _numScanLines = (select Max(id.ScanLine) from InstrumentData id
                              where id.SipperFileid = _sipperFileId
                      );


  set _dateTimeStart = (select Min(id.CTDDateTime)  from InstrumentData id
                              where (id.SipperFileId = _sipperFileId)     and
                                    (id.CTDDateTime > "1990-01-01 12:00:00")  and
                                    (not isnull(id.CTDDateTime)) and
                                    (id.CTDBattery > 7)  and  (id.CTDBattery < 15)  and  
                                    (Time(id.CTDDateTime) <> "00:00:00")
                      );

  set _numImages = (select count(*) from Images i where  i.SipperFileId = _sipperFileId);


  select  _numScanLines as NumScanLines, _dateTimeStart as  DateTimeStart,  _numImages as NumImages, _sipperFileId as SipperFileId;
end;
//

delimiter ;

  



/************************************************************************************************************************/
drop procedure  if exists SipperFilesDelete;

delimiter //
create procedure  SipperFilesDelete (in  _sipperFileName  varChar(48))
begin
  /* Will delete Specified SIPPER file and all related data */

  declare  _sipperFileID     int unsigned  default 0;

  set _sipperFileId = (select sf.SipperFileId  from SipperFiles sf  where sf.SipperFileName = _sipperFileName);

  delete ImagesFullSize   FROM ImagesFullSize   INNER JOIN Images i
     where  ImagesFullSize.ImageId = i.ImageId  and  i.SipperFileid = _sipperFileID;
  commit;

  delete ImageGroupEntries  from ImageGroupEntries   inner join Images i
     where  ImageGroupEntries.ImageId = i.ImageId  and  i.SipperFileid = _sipperFileID;
  commit;

  delete from  InstrumentData      where sipperFileId = _sipperFileID;
  commit;

  delete from  FeatureData         where sipperFileId = _sipperFileID;
  commit;
  
  delete from  MissingFeatureData  where SipperFileId = _sipperFileID;
  commit;

  delete from  Images              where sipperFileId = _sipperFileID;
  commit;

  delete from  SipperFiles         where sipperFileId = _sipperFileID;
  commit;
end;  
//
delimiter ;






/**************************************************************************************************/
/*                 Deletes all SipperFile data from tables for specified cruise.                  */
/**************************************************************************************************/

/************************************************************************************************************************/
drop procedure  if   exists  SipperFilesDeleteCruise;

delimiter //
create procedure  SipperFilesDeleteCruise (in  _cruiseName  char(10))
begin
  declare done             int default 0;
  declare _sipperFileName  varchar(48);
  declare _sipperFileId    int default 0;

  declare cur1 cursor for  select sf.SipperFileId, sf.SipperFileName
                                  from SipperFiles sf
                                  where sf.CruiseName = _cruiseName
                                  order by sf.SipperFileName;

  declare continue HANDLER for not found set done = 1;

  open cur1;

  repeat
    fetch  cur1 into _sipperFileId, _sipperFileName;
    if not done then
       call  SipperFilesDelete (_sipperFileName);
       commit;
    end if;
  until done end repeat;

  close cur1;
  
  delete  from deployments  where  CruiseName = _cruiseName;
  delete  from stations     where  CruiseName = _cruiseName;
  delete  from cruises      where  CruiseName = _cruiseName;

end;
//
delimiter ;


