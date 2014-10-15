delimiter ;



drop procedure  if exists MissingFeatureDataEntriesAddAllSipperFiles;

delimiter //

create procedure  MissingFeatureDataEntriesAddAllSipperFiles ()

begin
  declare  done                   int default 0;
  declare  _imageFileName         varchar(64);
  declare  _sipperFileId          int unsigned default 0;
  declare  _sipperFileName        char(48);
  declare  _imageId               int unsigned default 0;

  declare cur1 cursor for  select sf.SipperFileID, sf.SipperFileName  from SipperFiles sf  order by  sf.CruiseName, sf.DateTimeStart;

  declare continue HANDLER for not found set done = 1;

  open cur1;

  repeat
    fetch  cur1 into _sipperFileId, _sipperFileName;
    if  not done then
      call MissingFeatureDataAddOneSipperFile (_sipperFileName);
    end if;
  until done end repeat;

  close cur1;
end
//
delimiter ;





drop procedure  if exists MissingFeatureDataAddOneSipperFile;

delimiter //

create procedure  MissingFeatureDataAddOneSipperFile (in _sipperFileName  varChar(48))

begin
  declare  _sipperFileId    int unsigned default 0;
  set _sipperFileId = (select  sf.SipperFileId  from SipperFiles sf  where  sf.SipperFileName = _sipperFileName);

  insert into MissingFeatureData(ImageId, DateTimeAdded, SipperFileId)
       select  i.ImageId, CurTime(), _sipperFileId  
          from  Images i
          left join(FeatureData fd)  on  (fd.ImageId = i.ImageId)
          where  (i.SipperFileId = _sipperFileId)  and  (fd.ImageId is NULL);

end
//
delimiter ;




drop procedure  if exists  MissingFeatureDataRetrieveNextGroup;

delimiter //

create procedure  MissingFeatureDataRetrieveNextGroup (in _groupSize  int)

begin
  do  get_lock("MissingFeatureDataRetrieve", 3);

  select  mfd.ImageId, mfd.DateTimeAdded, mfd.SipperFileId
        from  MissingFeatureData mfd
        order by mfd.ImageId
        limit  _groupSize;
       
  delete  from  MissingFeatureData
          order by ImageId
          limit  _groupSize;

 do  release_lock("MissingFeatureDataRetrieve");

end
//
delimiter ;




drop procedure  if exists  MissingFeatureDataRetrieveNextGroupFromSipperFile;

delimiter //

create procedure  MissingFeatureDataRetrieveNextGroupFromSipperFile (in _sipperFileName varChar(48),
                                                                     in _groupSize      int
                                                                    )

begin
  declare  _sipperFileId  int unsigned default 0;
  
  set  _sipperFileId = (select  sf.SipperFileid  from SipperFiles sf  where sf.SipperFileName = _sipperFileName);
    
  do  get_lock("MissingFeatureDataRetrieve", 3);

  select  mfd.ImageId, mfd.DateTimeAdded, mfd.SipperFileId
        from  MissingFeatureData mfd
        where mfd.SipperFileId = _sipperFileId
        order by mfd.ImageId
        limit  _groupSize;
       
  delete  from  MissingFeatureData
          where SipperFileId = _sipperFileId
          order by ImageId
          limit  _groupSize;
          
 do  release_lock("MissingFeatureDataRetrieve");
end
//
delimiter ;
