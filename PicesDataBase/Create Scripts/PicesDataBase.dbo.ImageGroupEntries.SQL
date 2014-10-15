drop table if exists ImageGroupEntries;

Create Table ImageGroupEntries
(
  ImageGroupId       int unsigned     not null  references ImageGroup(ImageGroupId),
  ImageId            int unsigned     not null  references Images(ImageId),

  Key ImageGroup (ImageGroupId  asc,  ImageId       asc),
  Key ImageIdKey (ImageId       asc,  ImageGroupId  asc)
);