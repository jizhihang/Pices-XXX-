delimiter ;

drop procedure  if exists CreateAllTables;

delimiter //

create procedure CreateAllTables()
begin

  drop table if exists FeatureData;
  drop table if exists ImageGroupEntries;
  drop table if exists ImageGroup;
  drop table if exists ImagesFullSize;
  drop table if exists Images;
  drop table if exists InstrumentData;
  drop table if exists SipperFiles;
  drop table if exists Deployments;
  drop table if exists Stations;
  drop table if exists Cruises;
  drop table if exists Classes;
  drop table if exists CtdExternalSensors;
  drop table if exists Instruments;


  create table  `Classes` 
  (
    `ClassId`      int(10) unsigned          NOT NULL AUTO_INCREMENT,  
    `ClassName`    varchar(64)               NOT NULL,
    `ParentId`     int(10) unsigned          DEFAULT NULL,
    `Description`  varchar(255)              DEFAULT NULL,
    PRIMARY KEY (`ClassId`),
    UNIQUE KEY `ClassNameKey` (`ClassName`),
    KEY `ParentKey` (`ParentId`,`ClassName`)
  );
  
  
  create table  Cruises 
  (
    CruiseName   char(10)      not null,
    ShipName     varchar(50)   null,
    Description  varchar(1024) null,
    DateStart    date          null,
    DateEnd      date          null,
    Location     varchar(50)   null,
    Objective    varchar(50)   null,
    Principal    varchar(50)   null,
    ResearchOrg  varchar(50)   null,
    primary key  (CruiseName  asc)
  );
  
  
  create table Stations
  (
     CruiseName    char(10)      not null,
     StationName   char(10)      not null,
     Description   varchar(255)  null,
     Latitude      double        null,
     Longitude     double        null,
     DateTimeStart datetime      null,
 
     primary key    (CruiseName  asc,  StationName asc),
     foreign key  (CruiseName)  references Cruises (CruiseName)
  );
  
 
 
  create table Deployments   
  (
    CruiseName           char(10)     not null,
    StationName          char(10)     not null,  
    DeploymentNum        char(4)      not null,
    Description          varchar(255) null,
    DateTimeStart        datetime     null,
    DateTimeEnd          datetime     null,
    Latitude             double       null,
    Longitude            double       null,
    
    SyncTimeStampActual  datetime     null,
    SyncTimeStampCTD     datetime     null,
    SyncTimeStampGPS     datetime     null,

    primary key (CruiseName asc,  StationName asc,  DeploymentNum asc),
    foreign key (CruiseName, StationName)  references Stations (CruiseName, StationName)
  );

  
  
  
  create table  CtdExternalSensors
  (
    SensorName     char(3)     NOT NULL,
    Description    varchar(50) NULL,
 
    Primary Key (SensorName    asc)
  );

  insert into CtdExternalSensors values ("OXG", "Oxygen");
  insert into CtdExternalSensors values ("FLO", "Florometer");
  insert into CtdExternalSensors values ("TRN", "Transometer");
  insert into CtdExternalSensors values ("TUR", "Turbidity");

          
  
  
  create table Instruments
  (
	  InstrumentCode char(3)     NOT NULL,
	  Description    varchar(50) NULL,
   
      Primary Key (InstrumentCode  asc)
   );
   
   insert into Instruments values ("FLM", "Flow Meter");
   insert into Instruments values ("BAT",  "Batery Meter");
   insert into Instruments values ("P-R", "Pitch and Roll");
   insert into Instruments values ("CTD", "Conductivity Tempereture Depth");
            
    
  
  create table SipperFiles
  (
     SipperFileID    int unsigned    not null auto_increment,
     SipperFileName  char(48)        not null,
     CruiseName      char(10)        not null,
     StationName     char(10)        not null,
     DeploymentNum   char(4)         not null,
     Description     varchar(255)    null,
     Latitude        double          null,
     Longitude       double          null,
     DateTimeStart   datetime        null,
   
     SP0Instrument   char(3)         null,
     SP1Instrument   char(3)         null,
     SP2Instrument   char(3)         null,
     SP3Instrument   char(3)         null,

     CtdExt0         char(3)         null,
     CtdExt1         char(3)         null,
     CtdExt2         char(3)         null,
     CtdExt3         char(3)         null,
   
     SizeInBytes     bigint unsigned null  default 0,
     NumScanLines    int unsigned    null  default 0,
     ScanRate        float                 default 24950.0,
     Depth           float           null  default 0.0            comment 'to be used if no Instrumentation Data',
   
     ExtractionStatus         char(1)         null default '0'    comment '0=Not Extracted, 1=Extract, 2=Extracting, 3=Extracted',
     ExtractionScanLineStart  int unsigned null default 0,
     ExtractionScanLineEnd    int unsigned null default 0,
   
     primary key                  (SipperFileID    asc),
     unique        SipperFileName (SipperFileName  asc),
     key           CruiseKey      (CruiseName asc,  StationName asc,  DeploymentNum asc),
  
     foreign key (CruiseName)                             references Cruises     (CruiseName),
     foreign key (CruiseName, StationName)                references Stations    (CruiseName, StationName),
     foreign key (CruiseName, StationName, DeploymentNum) references Deployments (CruiseName, StationName, DeploymentNum)
  );


  Create Table Images
  (
    ImageId             int unsigned     not null auto_increment,
    ImageFileName       varchar(64)      not null,
    SipperFileId        int unsigned     not null  references SipperFiles(SipperFileId),
    ByteOffset          bigint unsigned  not null  default 0  comment 'Byte offset of start of scanline "TopLeftRow"',
    TopLeftRow          int unsigned     not null  default 0  comment 'First scan line in "SipperFileName"  that contains the image.',
    TopLeftCol          int              not null,
    Height              int              not null,
    Width               int              not null,
    PixelCount          int              not null,
    CentroidRow         int              not null,
    CentroidCol         int              not null,
    Depth	              float            not null  default 0,
    Class1Id            int unsigned     null      references Classes(ClassId),
    Class1Prob          float            null,
    Class2id            int unsigned     null      references Classes(ClassId),
    Class2Prob          float            null,
    ClassValidatedId    int unsigned     null      references Classes(ClassId),
    ThumbNail           blob(32768)      null,

    Primary key          (ImageId),
    unique ImageFileNameKey  (ImageFileName  asc),
    Key    SipperFileKey     (SipperFileId   asc,  TopLeftRow         asc),
    Key    ClassKey          (SipperFileId   asc,  Class1Id           asc,  ImageId asc),
    Key    DepthKey          (SipperFileId   asc,  Depth              asc,  ImageId asc),
    Key    ValidatedClassKey (SipperFileId   asc,  ClassValidatedId   asc,  ImageId asc)
  );





  Create Table ImagesFullSize
  (
    ImageId             int unsigned          not null  references Images(ImageId),
    FullSizeImage       blob(104857600)       null,
    
    unique ImageIdKey (ImageId    asc)
  )
    Comment="Copy of original images extracted.";




  create table  ImageGroup 
  (
    ImageGroupId      int(10) unsigned   not null AUTO_INCREMENT,
    ImageGroupName    varchar(64)        not null,
    Description       varchar(512)       default null,
    primary key (ImageGroupId),
    unique key   ImageGroupNameKey (ImageGroupName)
  );



  Create Table ImageGroupEntries
  (
    ImageGroupId       int unsigned     not null  references ImageGroup(ImageGroupId),
    ImageId            int unsigned     not null  references Images(ImageId),

    Key ImageGroup (ImageGroupId  asc,  ImageId       asc),
    Key ImageIdKey (ImageId       asc,  ImageGroupId  asc)
  );





  create table   InstrumentData
  (
      SipperFileId           int unsigned     not null                references SiperFiles(SipperFileId),
      ScanLine               int unsigned     not null  default 0,
      ByteOffset             bigint unsigned  not null  default 0  comment 'Byte offset of start of ScanlLne in "SipperFileName" ',
      CTDDateTime            datetime         null,
      Latitde                double           null,
      Longitude              double           null,
      CTDBattery             float            null   default 0.0,
      Conductivity           float            null   default 0.0,
      Density                float            null   default 0.0,
      Depth                  float            null   default 0.0,
      FlowRate1              float            null   default 0.0,
      FlowRate2              float            null   default 0.0,
      Fluorescence           float            null   default 0.0,
      FluorescenceSensor     float            null   default 0.0,
      CdomFluorescenceSensor float            null   default 0.0,
      CdomFluorescence       float            null   default 0.0,
      Oxygen                 float            null   default 0.0,
      OxygenSensor           float            null   default 0.0,
      Pressure               float            null   default 0.0,
      RecordRate             float            null   default 0.0,
      Salinity               float            null   default 0.0,
      SoundVelocity          float            null   default 0.0,
      Temperature            float            null   default 0.0,
      Transmisivity          float            null   default 0.0,
      TransmisivitySensor    float            null   default 0.0,
      Turbidity              float            null   default 0.0,
      TurbiditySensor        float            null   default 0.0,
      Pitch                  float            null   default 0.0,
      Roll                   float            null   default 0.0,
  	
      primary key (SipperFileId asc,  ScanLine asc),
      key          CTDDateTimeKey      (CTDDateTime   asc),
      
      foreign key (SipperFileId)  references  SipperFiles (SipperFileId)
  );




  CREATE TABLE FeatureData
  (
     ImageId                    int unsigned     not null  references Images(ImageId),
     SipperFileId               int unsigned     not null  references SipperFiles(SipperFileId),
     Size                       double           DEFAULT NULL,
     Moment1                    double DEFAULT NULL,
     Moment2                    double DEFAULT NULL,
     Moment3                    double DEFAULT NULL,
     Moment4                    double DEFAULT NULL,
     Moment5                    double DEFAULT NULL,
     Moment6                    double DEFAULT NULL,
     Moment7                    double DEFAULT NULL,
     EdgeSize                   double DEFAULT NULL,
     EdgeMoment1                double DEFAULT NULL,
     EdgeMoment2                double DEFAULT NULL,
     EdgeMoment3                double DEFAULT NULL,
     EdgeMoment4                double DEFAULT NULL,
     EdgeMoment5                double DEFAULT NULL,
     EdgeMoment6                double DEFAULT NULL,
     EdgeMoment7                double DEFAULT NULL,
     TransparancyConvexHull     double DEFAULT NULL,
     TransparancyPixelCount     double DEFAULT NULL,
     TransparancyOpen3          double DEFAULT NULL,
     TransparancyOpen5          double DEFAULT NULL,
     TransparancyOpen7          double DEFAULT NULL,
     TransparancyOpen9          double DEFAULT NULL,
     TransparancyClose3         double DEFAULT NULL,
     TransparancyClose5         double DEFAULT NULL,
     TransparancyClose7         double DEFAULT NULL,
     EigenRatio                 double DEFAULT NULL,
     EigenHead                  double DEFAULT NULL,
     ConvexArea                 double DEFAULT NULL,
     TransparancySize           double DEFAULT NULL,
     TransparancyWtd            double DEFAULT NULL,
     WeighedMoment0             double DEFAULT NULL,
     WeighedMoment1             double DEFAULT NULL,
     WeighedMoment2             double DEFAULT NULL,
     WeighedMoment3             double DEFAULT NULL,
     WeighedMoment4             double DEFAULT NULL,
     WeighedMoment5             double DEFAULT NULL,
     WeighedMoment6             double DEFAULT NULL,
     WeighedMoment7             double DEFAULT NULL,
     Fourier0                   double DEFAULT NULL,
     Fourier1                   double DEFAULT NULL,
     Fourier2                   double DEFAULT NULL,
     Fourier3                   double DEFAULT NULL,
     Fourier4                   double DEFAULT NULL,
     ContourFourierDescriptor0  double DEFAULT NULL,
     ContourFourierDescriptor1  double DEFAULT NULL,
     ContourFourierDescriptor2  double DEFAULT NULL,
     ContourFourierDescriptor3  double DEFAULT NULL,
     ContourFourierDescriptor4  double DEFAULT NULL,
     FourierDescRight1          double DEFAULT NULL,
     FourierDescLeft2           double DEFAULT NULL,
     FourierDescRight2          double DEFAULT NULL,
     FourierDescLeft3           double DEFAULT NULL,
     FourierDescRight3          double DEFAULT NULL,
     FourierDescLeft4           double DEFAULT NULL,
     FourierDescRight4          double DEFAULT NULL,
     FourierDescLeft5           double DEFAULT NULL,
     FourierDescRight5          double DEFAULT NULL,
     FourierDescLeft6           double DEFAULT NULL,
     FourierDescRight6          double DEFAULT NULL,
     FourierDescLeft7           double DEFAULT NULL,
     FourierDescRight7          double DEFAULT NULL,
     FourierDescLeft8           double DEFAULT NULL,
     FourierDescRight8          double DEFAULT NULL,
     IntensityHist1             double DEFAULT NULL,
     IntensityHist2             double DEFAULT NULL,
     IntensityHist3             double DEFAULT NULL,
     IntensityHist4             double DEFAULT NULL,
     IntensityHist5             double DEFAULT NULL,
     IntensityHist6             double DEFAULT NULL,
     IntensityHist7             double DEFAULT NULL,
     HeightVsWidth              double DEFAULT NULL,
     Length                     double DEFAULT NULL,
     Width                      double DEFAULT NULL,
     FilledArea                 double DEFAULT NULL,
     FlowRate1                  double DEFAULT NULL,
     FlowRate2                  double DEFAULT NULL,
     IntensityHistB0            double DEFAULT NULL,
     IntensityHistB1            double DEFAULT NULL,
     IntensityHistB2            double DEFAULT NULL,
     IntensityHistB3            double DEFAULT NULL,
     IntensityHistB4            double DEFAULT NULL,
     IntensityHistB5            double DEFAULT NULL,
     IntensityHistB6            double DEFAULT NULL,
     IntensityHistB7            double DEFAULT NULL,
     Depth                      double DEFAULT NULL,
     Salinity                   double DEFAULT NULL,
     Oxygen                     double DEFAULT NULL,
     Florences                  double DEFAULT NULL,

     PRIMARY KEY        (ImageId  asc),
     KEY SipperFileKey  (SipperFileId  asc, ImageId  asc)
  );

end
//

delimiter ;


call  CreateAllTables ();