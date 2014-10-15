


ALTER TABLE `pices_new`.`images` 
    ADD COLUMN `ConnectedPixelDist`   TINYINT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Size of structure used for Connected Component Analysis.' AFTER `PixelCount`,
    ADD COLUMN `ExtractionLogEntryId` INT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Image Extraction LogEntryId that generated this image.' AFTER `ConnectedPixelDist`,
    ADD COLUMN `ClassLogEntryId`      INT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Image Classification LogEntryId that updated class assignment.' AFTER `ExtractionLogEntryId`,
    ADD COLUMN `ImageSize`            FLOAT NOT NULL DEFAULT 0.0 AFTER `ClassValidatedId`,
    ADD COLUMN `SizeCoordinates`      VARCHAR(32) NULL AFTER `ImageSize`,
    DROP INDEX `Class2Key`,
    ADD INDEX  `Class2Key` (`SipperFileId` ASC, `Class2id` ASC),
    ADD INDEX `ImagesClass1Id_idx` (`Class1Id` ASC),
    ADD INDEX `ImagesClass2Id_idx` (`Class2id` ASC),
    ADD INDEX `ImagesValidatedId_idx` (ClassValidatedId asc),

ALTER TABLE `pices_new`.`images` 
    ADD CONSTRAINT `ImagesClass1id`    FOREIGN KEY (`Class1Id`)         REFERENCES `pices_new`.`classes` (`ClassId`)
        ON DELETE NO ACTION
        ON UPDATE NO ACTION,
    
    ADD CONSTRAINT `ImagesClass2id`    FOREIGN KEY (`Class2id`)         REFERENCES `pices_new`.`classes` (`ClassId`)
        ON DELETE NO ACTION
        ON UPDATE NO ACTION,
    
    ADD CONSTRAINT `ImagesSipperFile`  FOREIGN KEY (`SipperFileId`)     REFERENCES `pices_new`.`sipperfiles` (`SipperFileID`)
        ON DELETE NO ACTION
        ON UPDATE NO ACTION,
      
    ADD CONSTRAINT `ImagesValidatedId` FOREIGN KEY (`ClassValidatedId`) REFERENCES `pices_new`.`classes` (`ClassId`)
        ON DELETE NO ACTION
        ON UPDATE NO ACTION;
  