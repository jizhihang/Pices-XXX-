


alter table `Deployments` 
       add column `CropLeft`             int     unsigned not null default 0     comment 'First scanline pixel visible in imaging chamber.'   AFTER `SyncTimeStampGPS` ,
       add column `CropRight`            int     unsigned not null default 4095  comment 'Last scanline pixel visible in imaging chamber.'    AFTER `CropLeft`; 
       