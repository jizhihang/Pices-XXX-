/*********************************************************************************************/
drop procedure if exists  ImageGroupEntryInsert;
delimiter //

create procedure ImageGroupEntryInsert(
    in  _groupId         int,
    in  _imageFileName   varchar(64)
 )
begin
  declare  _imageId  int unsigned  default 0;

  set _imageId = (select ImageId from Images i  where  i.ImageFileName = _imageFileName);

  insert into  ImageGroupEntries (ImageGroupId, ImageId)
     values(_groupId, _imageId);
end
//
delimiter ;






/*********************************************************************************************/
/*********************************************************************************************/
drop procedure if exists ImageGroupEntryInsertList;
delimiter //

create procedure ImageGroupEntryInsertList(
    in  _groupId            int,
    in  _imageFileNameList  varchar(64000)  /* Tab delimted list of image file names. */
 )
begin
  /*  Will scan through the Tab delimited list of image file names and  */
  /*  insert them into the ImageGroupEntries table.  The results of the */
  /*  insertions will returned in the for for one row for each image    */
  /*  file name and 4 columns.                                          */
  /*       ImageId, ImageFileName, Successful, and  reason for failure  */

  declare  _imageId        int unsigned  default 0;
  declare  _idx            int  default 0;
  declare  _imageFileName  varchar(64);

  drop table  if exists  ResultsTable;

  create temporary table ResultsTable
   (
     ImageId        int unsigned  not null,
     ImageFileName  varchar(64),
     Successful     bool,
     ErrorDesc      varchar(255)
   );


  while  (Length(_imageFileNameList) > 0)  do
    set _idx = Locate("\t", _imageFileNameList);
    if  (_idx > 0)  then
      set  _imageFileName = Left(_imageFileNameList,_idx-1);
      set  _imageFileNameList = substr(_imageFileNameList,_idx+1);
    else
      set  _imageFilename = _imageFileNameList;
      set  _imageFileNameList = "";
    end if;

    set _imageId = (select ImageId from Images i  where  i.ImageFileName = _imageFileName);
    if  (isnull(_imageId)  or  (_imageId < 1))  then
      insert into  ResultsTable(ImageId, ImageFileName, Successful, ErrorDesc)
             values (0, _imageFileName, false, "Image not in Database");
    else
      insert into  ImageGroupEntries (ImageGroupId, ImageId)
         values(_groupId, _imageId);
      insert into  ResultsTable(ImageId, ImageFileName, Successful, ErrorDesc)
             values (_imageId, _imageFileName, true, "");
    end if;
  end while;

  select * from  ResultsTable;
end
//
delimiter ;

