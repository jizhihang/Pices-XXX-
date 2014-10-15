


drop  procedure  if exists ImageGroupLoadAll;

delimiter //
create procedure ImageGroupLoadAll ()
begin
  select  ig.ImageGroupId, ig.ImageGroupName, ig.Description, count(ige.ImageGroupId) as  GroupCount
            from  ImageGroup  ig
            left join(ImageGroupEntries ige)  on(ige.ImageGroupId = ig.ImageGroupId)
            group by ig.ImageGroupId;
end;
//
delimiter ;







/****************************************************************************************************/
/****************************************************************************************************/
drop  procedure  if exists ImageGroupLoad;

delimiter //
create procedure ImageGroupLoad (in  _imageGroupName  varChar(64))
begin
  select  ig.ImageGroupId, ig.ImageGroupName, ig.Description, count(ige.ImageGroupId) as GroupCount
            from  ImageGroup  ig
            left join(ImageGroupEntries ige)  on(ige.ImageGroupId = ig.ImageGroupId)
            where  ig.ImageGroupName = _imageGroupName
            group by ig.ImageGroupId;
end;
//
delimiter ;



drop procedure if exists ImageGroupDelete;
delimiter //

CREATE PROCEDURE ImageGroupDelete(in  groupId  int)
BEGIN
  delete  from ImageGroupEntries  where  ImageGroupId = groupId;
  delete  from ImageGroup         where  ImageGroupId = groupId;
END
//
delimiter ;




delimiter ;

drop procedure if exists ImageGroupInsert;
delimiter //

CREATE PROCEDURE ImageGroupInsert(
    in  groupName     varchar(64),
    in  description   varchar(512)
 )
BEGIN
  insert into  ImageGroup (ImageGroupName, Description) values (groupName, description);
  select ImageGroupId from ImageGroup where ImageGroupName = groupName;
END
//
delimiter ;




