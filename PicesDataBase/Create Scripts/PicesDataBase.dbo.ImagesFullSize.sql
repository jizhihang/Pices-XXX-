drop table if exists ImagesFullSize;
Create Table ImagesFullSize
(
  ImageId             int unsigned          not null  references Images(ImageId),
  FullSizeImage       blob(104857600)       null,
  
  unique ImageIdKey (ImageId    asc)
)
  Comment="Copy of original images extracted."
;


