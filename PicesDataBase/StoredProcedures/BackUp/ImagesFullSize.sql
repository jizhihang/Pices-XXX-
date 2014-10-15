delimiter ;


drop procedure  if exists ImagesFullSizeLoad;

delimiter //

create procedure  ImagesFullSizeLoad (in  _ImageFileName    varChar(64))

begin
  select i.ImageId, i.ImageFileName, ifs.FullSizeImage
        from  images  i
        join  (ImagesFullSize ifs)  on(ifs.ImageId = i.ImageId)  
        where  i.ImageFileName = _ImageFileName;
end
//
delimiter ;




drop procedure  if exists ImagesFullSizeSave;
delimiter //

create procedure ImagesFullSizeSave (in  _ImageFileName       varChar(64),
                                     in  _FullSizeImage       blob(104857600)
                                    )

begin
  declare  _imageId  int unsigned  default 0;

  set _imageId = (select i.ImageId from images i  where  i.ImageFileName = _ImageFileName);

  if  (!isnull(_imageId)  and  (_imageId > 0))  then
    Insert into ImagesFullSize (ImageId, FullSizeImage)
         values (_imageId, _FullSizeImage);
    select @imageId as ImageId;
  else
    select -1 as ImageId;
  end if;

end
//
delimiter ;





/** *************************************************************************************************
 * Used by "GetImagesAlreadyInImagesFullSize"  in "MergeFeatureFiles" which uses it to update all 
 * full size images.
 */

drop procedure  if exists ImagesFullSizeFileNamesForSipperFile;
delimiter //

create procedure ImagesFullSizeFileNamesForSipperFile (in  _sipperFileName  varChar(48))

begin
  declare  _imageId       int unsigned  default 0;
  declare  _sipperFileId  int unsigned  default 0;

  set _sipperFileId = (select sf.SipperFileId from SipperFiles sf  where  sf.SipperFileName = _sipperFileName);


  select  ifs.ImageId, i.ImageFileName, Length(ifs.FullSizeImage) as FullSizeImageLength
        from  ImagesFullSize ifs
        join(Images i)  on (i.ImageId = ifs.ImageId)
        where  i.SipperFileId = _sipperFileId;
end
//
delimiter ;