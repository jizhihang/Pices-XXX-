drop table if exists TrainingLibraries;
Create Table TrainingLibraries
(
  TrainingLibraryId   int unsigned     not null auto_increment,
  TrainingLibraryName varchar(64)      not null,            comment 'Same name as Root Directory where training Images are stotred.'

  Primary key                   (TrainingLibraryId),
  unique TrainingLibraryNameKey (TrainingLibraryName   asc)
);


/*   Dump and Save Code for the Images table.  */

select * from TrainingLibraries 
   INTO OUTFILE 'd:\\Temp\\TrainingLibraries.Dump'
   FIELDS ESCAPED BY '\\'  TERMINATED BY ',' ENCLOSED BY '"'
   LINES TERMINATED BY '\r\n';



LOAD DATA INFILE 'd:\\Temp\\TrainingLibraries.Dump'
    INTO TABLE TrainingLibraries
    FIELDS ESCAPED BY '\\'  TERMINATED BY ',' ENCLOSED BY '"'
    LINES TERMINATED BY '\r\n';





drop table if exists TrainingModels;
Create Table TrainingModels
(
  TrainingModelId     int unsigned     not null auto_increment,
  TrainingModelName   varchar(64)      not null,
  TrainingLibraryId   int unsigned     not null  references TrainingLibraries(TrainingLibraryId),
  DateTimeLastBuilt   DateTime         default null,
  Parameters          varchar(255)     not null  comment 'Parameters to be used by the Learning Algorithm',

  primary key                 (TrainingModelId),
  unique TrainingModelNameKey (TrainingModelName  asc),
  key    TrainingLibraryKey   (TrainingLibraryId  asc,  TrainingModelId asc)
  
);





drop table if exists TrainingClasses;
Create Table TrainingClasses
(
  TrainingClassID     int unsigned     not null auto_increment,
  TrainingModelId     int unsigned     not null references TrainingModels(TrainingModelId),
  ClassId             int              not null references Classes(ClassId)  comment 'Class that we are training for',

  primary key                 (TrainingClassID),
  key     TrainingModelKey    (TrainingModelId  asc, ClassId asc),
  
);





drop table if exists TrainingClassExamples;
Create Table TrainingClassExamples
(
  TrainingModelId     int unsigned     not null references TrainingModels(TrainingModelId),
  TrainingClassID     int unsigned     not null references TrainingClasses(TrainingClassID),
  ImageId             int unsigned     not null references Images(ImageId),
  DateTimeAdded       DateTime         null   comment  'Date that image was added to Training Model'

  primary key                 (TrainingModelId asc, TrainingClassID asc, ImageId asc),
  unique  TrainingClassKey    (TrainingClassID asc, ImageId  asc),
  unique  ImageKey            (ImageId asc, TrainingModelId asc)
  
);



