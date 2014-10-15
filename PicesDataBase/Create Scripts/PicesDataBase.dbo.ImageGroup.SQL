drop table if exists ImageGroup;

create table  ImageGroup 
(
  ImageGroupId      int(10) unsigned   not null AUTO_INCREMENT,
  ImageGroupName    varchar(64)        not null,
  Description       varchar(512)       default null,
  primary key (ImageGroupId),
  unique key   ImageGroupNameKey (ImageGroupName)
);