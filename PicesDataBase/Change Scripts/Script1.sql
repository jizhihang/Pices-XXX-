uses  pices_new
ALTER TABLE `pices_new`.`deployments` ADD COLUMN `CTD_GPS_Adj` INT NULL DEFAULT 0 COMMENT 'Number of secs diff between CTD and GPS clocks; allow GPS import function to sync clocks.'  AFTER `Longitude` ;
