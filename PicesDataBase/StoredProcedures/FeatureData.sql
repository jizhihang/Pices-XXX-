delimiter  ;


drop procedure  if exists FeatureDataInsert;
delimiter //

CREATE PROCEDURE FeatureDataInsert(
    in  _ImageFileName                 varchar(64),
    in  _SipperFileName                varchar(48),
    in  _Size                          float,
    in  _Moment1                       float,
    in  _Moment2                       float,
    in  _Moment3                       float,
    in  _Moment4                       float,
    in  _Moment5                       float,
    in  _Moment6                       float,
    in  _Moment7                       float,
    in  _EdgeSize                      float,
    in  _EdgeMoment1                   float,
    in  _EdgeMoment2                   float,
    in  _EdgeMoment3                   float,
    in  _EdgeMoment4                   float,
    in  _EdgeMoment5                   float,
    in  _EdgeMoment6                   float,
    in  _EdgeMoment7                   float,
    in  _TransparancyConvexHull        float,
    in  _TransparancyPixelCount        float,
    in  _TransparancyOpen3             float,
    in  _TransparancyOpen5             float,
    in  _TransparancyOpen7             float,
    in  _TransparancyOpen9             float,
    in  _TransparancyClose3            float,
    in  _TransparancyClose5            float,
    in  _TransparancyClose7            float,
    in  _EigenRatio                    float,
    in  _EigenHead                     float,
    in  _ConvexArea                    float,
    in  _TransparancySize              float,
    in  _TransparancyWtd               float,
    in  _WeighedMoment0                float,
    in  _WeighedMoment1                float,
    in  _WeighedMoment2                float,
    in  _WeighedMoment3                float,
    in  _WeighedMoment4                float,
    in  _WeighedMoment5                float,
    in  _WeighedMoment6                float,
    in  _WeighedMoment7                float,
    in  _Fourier0                      float,
    in  _Fourier1                      float,
    in  _Fourier2                      float,
    in  _Fourier3                      float,
    in  _Fourier4                      float,
    in  _ContourFourierDescriptor0     float,
    in  _ContourFourierDescriptor1     float,
    in  _ContourFourierDescriptor2     float,
    in  _ContourFourierDescriptor3     float,
    in  _ContourFourierDescriptor4     float,
    in  _FourierDescRight1             float,
    in  _FourierDescLeft2              float,
    in  _FourierDescRight2             float,
    in  _FourierDescLeft3              float,
    in  _FourierDescRight3             float,
    in  _FourierDescLeft4              float,
    in  _FourierDescRight4             float,
    in  _FourierDescLeft5              float,
    in  _FourierDescRight5             float,
    in  _FourierDescLeft6              float,
    in  _FourierDescRight6             float,
    in  _FourierDescLeft7              float,
    in  _FourierDescRight7             float,
    in  _FourierDescLeft8              float,
    in  _FourierDescRight8             float,
    in  _IntensityHist1                float,
    in  _IntensityHist2                float,
    in  _IntensityHist3                float,
    in  _IntensityHist4                float,
    in  _IntensityHist5                float,
    in  _IntensityHist6                float,
    in  _IntensityHist7                float,
    in  _HeightVsWidth                 float,
    in  _Length                        float,
    in  _Width                         float,
    in  _FilledArea                    float,
    in  _FlowRate1                     float,
    in  _FlowRate2                     float,
    in  _IntensityHistB0               float,
    in  _IntensityHistB1               float,
    in  _IntensityHistB2               float,
    in  _IntensityHistB3               float,
    in  _IntensityHistB4               float,
    in  _IntensityHistB5               float,
    in  _IntensityHistB6               float,
    in  _IntensityHistB7               float,
    in  _Depth                         float,
    in  _Salinity                      float,
    in  _Oxygen                        float,
    in  _Florences                     float
 )
BEGIN
  insert into  FeatureData
  (
    ImageId,
    SipperFileId,
    Size, Moment1, Moment2, Moment3, Moment4, Moment5, Moment6, Moment7,
    EdgeSize, EdgeMoment1, EdgeMoment2, EdgeMoment3, EdgeMoment4, EdgeMoment5, EdgeMoment6, EdgeMoment7,
    TransparancyConvexHull, TransparancyPixelCount,
    TransparancyOpen3, TransparancyOpen5, TransparancyOpen7, TransparancyOpen9, TransparancyClose3, TransparancyClose5, TransparancyClose7,
    EigenRatio, EigenHead,
    ConvexArea,
    TransparancySize,TransparancyWtd,
    WeighedMoment0, WeighedMoment1, WeighedMoment2, WeighedMoment3, WeighedMoment4, WeighedMoment5, WeighedMoment6, WeighedMoment7,
    Fourier0, Fourier1, Fourier2, Fourier3, Fourier4,
    ContourFourierDescriptor0, ContourFourierDescriptor1, ContourFourierDescriptor2, ContourFourierDescriptor3, ContourFourierDescriptor4,
    FourierDescRight1, FourierDescLeft2,  FourierDescRight2, FourierDescLeft3,  FourierDescRight3, FourierDescLeft4, FourierDescRight4, FourierDescLeft5,
    FourierDescRight5, FourierDescLeft6,  FourierDescRight6, FourierDescLeft7,  FourierDescRight7, FourierDescLeft8, FourierDescRight8,
    IntensityHist1, IntensityHist2, IntensityHist3, IntensityHist4, IntensityHist5, IntensityHist6, IntensityHist7,
    HeightVsWidth, Length, Width,
    FilledArea,
    FlowRate1, FlowRate2,
    IntensityHistB0, IntensityHistB1, IntensityHistB2, IntensityHistB3, IntensityHistB4, IntensityHistB5, IntensityHistB6, IntensityHistB7,
    Depth, Salinity, Oxygen, Florences
    )
  values (
     (select i.ImageId from Images i where i.ImageFileName = _imageFileName),
     (select sf.SipperFileId  from SipperFiles sf where sf.SipperFileName = _sipperFileName),
    _Size, _Moment1, _Moment2, _Moment3, _Moment4, _Moment5, _Moment6, _Moment7,
    _EdgeSize, _EdgeMoment1, _EdgeMoment2, _EdgeMoment3, _EdgeMoment4, _EdgeMoment5, _EdgeMoment6, _EdgeMoment7,
    _TransparancyConvexHull, _TransparancyPixelCount,
    _TransparancyOpen3, _TransparancyOpen5, _TransparancyOpen7, _TransparancyOpen9, _TransparancyClose3, _TransparancyClose5, _TransparancyClose7,
    _EigenRatio, _EigenHead,
    _ConvexArea,
    _TransparancySize, _TransparancyWtd,
    _WeighedMoment0, _WeighedMoment1, _WeighedMoment2, _WeighedMoment3, _WeighedMoment4, _WeighedMoment5, _WeighedMoment6, _WeighedMoment7,
    _Fourier0, _Fourier1, _Fourier2, _Fourier3, _Fourier4,
    _ContourFourierDescriptor0, _ContourFourierDescriptor1, _ContourFourierDescriptor2, _ContourFourierDescriptor3, _ContourFourierDescriptor4,
    _FourierDescRight1, _FourierDescLeft2,  _FourierDescRight2, _FourierDescLeft3,  _FourierDescRight3, _FourierDescLeft4, _FourierDescRight4, _FourierDescLeft5,
    _FourierDescRight5, _FourierDescLeft6,  _FourierDescRight6, _FourierDescLeft7,  _FourierDescRight7, _FourierDescLeft8, _FourierDescRight8,
    _IntensityHist1, _IntensityHist2, _IntensityHist3, _IntensityHist4, _IntensityHist5, _IntensityHist6, _IntensityHist7,
    _HeightVsWidth, _Length, _Width,
    _FilledArea,
    _FlowRate1, _FlowRate2,
    _IntensityHistB0, _IntensityHistB1, _IntensityHistB2, _IntensityHistB3, _IntensityHistB4, _IntensityHistB5, _IntensityHistB6, _IntensityHistB7,
    _Depth, _Salinity, _Oxygen, _Florences
);
END
//
delimiter ;

 
 
 
 delimiter ;

/* use  pices_new; */

drop procedure  if exists FeatureDataLoadByImageFileName;

delimiter //

create procedure  FeatureDataLoadByImageFileName (in _imageFileName   varchar(64))
begin
  select i.ImageId,
         i.ImageFileName,
         i.SipperFileId,
         i.ByteOffset,
         i.TopLeftRow,
         i.TopLeftCol,
         i.Height  as ThumbNailHeight,
         i.Width   as ThumbNailWidth,
         i.PixelCount,
         i.CentroidRow,
         i.CentroidCol,
         i.Depth,
         i.Class1Id,
         i.Class1Prob,
         i.Class2id,
         i.Class2Prob,
         i.ClassValidatedId,
         (select c1.ClassName from Classes c1  where  c1.ClassId = i.Class1Id)          as  Class1Name,
         (select c2.ClassName from Classes c2  where  c2.ClassId = i.Class2Id)          as  Class2Name,
         (select c3.ClassName from Classes c3  where  c3.ClassId = i.ClassValidatedId)  as  ClassNameValidated,
         fd.*

   from Images i
           join(FeatureData fd)  on (fd.ImageId = i.ImageId)
           where i.ImageFileName = _imageFileName;
END
//
delimiter ;





/* The following procedure has fields in same order as 'FeatureDataGetOneSipperFile'. */
/* The following procedure has fields in same order as 'FeatureDataGetOneSipperFile'. */
/* use  pices_new; */

drop procedure  if exists FeatureDataLoadByImageFileName2;

delimiter //

create procedure  FeatureDataLoadByImageFileName2 (in _imageFileName   varchar(64))
begin
  select  i.ImageId,
          i.ImageFileName,
          i.TopLeftRow,
          i.TopLeftCol,
          i.PixelCount,
          i.CentroidRow,
          i.CentroidCol,
          i.Class1Id,
          (select ClassName from Classes c1 where  c1.ClassId = i.Class1Id) as Class1Name,
          i.Class1Prob,
          i.Class2Prob,
          i.ClassValidatedId,
          (select ClassName from Classes c2 where  c2.ClassId = i.ClassValidatedId) as ClassNameValidated,
          i.Depth  as ImagesDepth,
          (select id.CTDDateTime  from  InstrumentData id  where  id.SipperFileId = i.SipperFileId  and  id.ScanLine > i.TopLeftRow limit 1)  as  CtdDateTime,
          fd.* 

   from Images i
           join(FeatureData fd)  on (fd.ImageId = i.ImageId)
           where i.ImageFileName = _imageFileName;
END
//
delimiter ;





/*****************************************************************************************************************/
/*****************************************************************************************************************/
/* use  pices_new;*/

drop procedure  if exists FeatureDataGetOneSipperFile;

delimiter //

create procedure  FeatureDataGetOneSipperFile (in _sipperFileName   varchar(48),
                                               in _classKeyToUse    char
                                              )
begin
  set  @sipperFileId = (select sf.SipperFileId  from SipperFiles sf  where  sf.SipperFileName = _sipperFileName);


  if  _classKeyToUse = "V"  then
    select  i.ImageId,
            i.ImageFileName,
            i.TopLeftRow,
            i.TopLeftCol,
            i.PixelCount,
            i.CentroidRow,
            i.CentroidCol,
            i.Class1Id,
            (select ClassName from Classes c1 where  c1.ClassId = i.Class1Id) as Class1Name,
            i.Class1Prob,
            i.Class2Prob,
            i.ClassValidatedId,
            (select ClassName from Classes c2 where  c2.ClassId = i.ClassValidatedId) as ClassNameValidated,
            i.Depth  as ImagesDepth,
            id.CTDDateTime  as  CtdDateTime,
            fd.* 
 
       from images i 
       left join(featuredata    fd)  on(fd.ImageId = i.ImageId)
			 left join(InstrumentData id)  on(id.SipperFileId=i.SipperFileid  and  id.ScanLine = (4096 * floor(i.TopLeftRow / 4096)))
       where  (i.SipperFileId =  @sipperFileId)  and  (i.ClassValidatedId is not null);
  else
    select  i.ImageId,
            i.ImageFileName,
            i.TopLeftRow,
            i.TopLeftCol,
            i.PixelCount,
            i.CentroidRow,
            i.CentroidCol,
            i.Class1Id,
            (select ClassName from Classes c1 where  c1.ClassId = i.Class1Id) as Class1Name,
            i.Class1Prob,
            i.Class2Prob,
            i.ClassValidatedId,
            (select ClassName from Classes c2 where  c2.ClassId = i.ClassValidatedId) as ClassNameValidated,
            i.Depth  as ImagesDepth,
            id.CTDDateTime  as  CtdDateTime,
            fd.* 
 
       from images i 
       left join(featuredata    fd)  on(fd.ImageId = i.ImageId)
			 left join(InstrumentData id)  on(id.SipperFileId=i.SipperFileid  and  id.ScanLine = (4096 * floor(i.TopLeftRow / 4096)))
       where  i.SipperFileId =  @sipperFileId;
   end if;
end;
//
delimiter ;






/*****************************************************************************************************************/
/*****************************************************************************************************************/
/* use  pices_new; */

drop procedure  if exists FeatureDataGetOneSipperFileClassName;

delimiter //

create procedure  FeatureDataGetOneSipperFileClassName (in  _sipperFileName   varchar(48),
                                                        in  _class1Name       varchar(64),
                                                        in  _classKeyToUse    char
                                                       )
begin
  declare  _sipperFileId   int  unsigned  default 0;
  declare  _class1Id       int  unsigned  default 0;

  set  _sipperFileId = (select sf.SipperFileId  from SipperFiles sf  where  sf.SipperFileName = _sipperFileName);
  set  _class1Id     = (select c.ClassId  from Classes c  where c.ClassName = _class1Name);

  if  _classKeyToUse = "V"  then
    select  i.ImageId,
            i.ImageFileName,
            i.TopLeftRow,
            i.TopLeftCol,
            i.PixelCount,
            i.CentroidRow,
            i.CentroidCol,
            i.Class1Id,
            (select ClassName from Classes c1 where  c1.ClassId = i.Class1Id) as Class1Name,
            i.Class1Prob,
            i.Class2Prob,
            i.ClassValidatedId,
            (select ClassName from Classes c2 where  c2.ClassId = i.ClassValidatedId) as ClassNameValidated,
            i.Depth  as ImagesDepth,
            id.CTDDateTime  as  CtdDateTime,
            fd.*

       from images i 
       left join(featuredata    fd)  on(fd.ImageId = i.ImageId)
			 left join(InstrumentData id)  on(id.SipperFileId = i.SipperFileid  and  id.ScanLine = (4096 * floor(i.TopLeftRow / 4096)))
       where  (i.SipperFileId =  _sipperFileId)  and  (i.ClassValidatedId = _class1Id);
  
  else
    select  i.ImageId,
            i.ImageFileName,
            i.TopLeftRow,
            i.TopLeftCol,
            i.PixelCount,
            i.CentroidRow,
            i.CentroidCol,
            i.Class1Id,
            (select ClassName from Classes c1 where  c1.ClassId = i.Class1Id) as Class1Name,
            i.Class1Prob,
            i.Class2Prob,
            i.ClassValidatedId,
            (select ClassName from Classes c2 where  c2.ClassId = i.ClassValidatedId) as ClassNameValidated,
            i.Depth  as ImagesDepth,
            id.CTDDateTime  as  CtdDateTime,
            fd.*

       from images i 
       left join(featuredata    fd)  on(fd.ImageId = i.ImageId)
			 left join(InstrumentData id)  on(id.SipperFileId=i.SipperFileid  and  id.ScanLine = (4096 * floor(i.TopLeftRow / 4096)))
       where  (i.SipperFileId =  _sipperFileId)  and  (i.Class1Id = _class1Id);
  end if;
end;
//
delimiter ;








/*****************************************************************************************************************/
/*****************************************************************************************************************/
/* use  pices_new; */

drop procedure  if exists FeatureDataGetOneImageGroup;

delimiter //

create procedure  FeatureDataGetOneImageGroup (in  _imageGroupId   int,
                                               in  _classKeyToUse  char
                                              )
begin
  if  _classKeyToUse = "V"  then
    select  i.ImageId,
            i.ImageFileName,
            i.TopLeftRow,
            i.TopLeftCol,
            i.PixelCount,
            i.CentroidRow,
            i.CentroidCol,
            i.Class1Id,
            (select ClassName from Classes c1 where  c1.ClassId = i.Class1Id) as Class1Name,
            i.Class1Prob,
            i.Class2Prob,
            i.ClassValidatedId,
            (select ClassName from Classes c2 where  c2.ClassId = i.ClassValidatedId) as ClassNameValidated,
            i.Depth  as ImagesDepth,
            id.CTDDateTime  as  CtdDateTime,
            fd.* 

      from images i
          join (ImageGroupEntries  ig)  on(ig.ImageId = i.ImageId)
          left join(featuredata    fd)  on(fd.ImageId = i.ImageId)
          left join(InstrumentData id)  on(id.SipperFileId=i.SipperFileid  and  id.ScanLine = (4096 * floor(i.TopLeftRow / 4096)))
          where  (ig.ImageGroupId = _imageGroupId)  and  (i.ClassValidatedId is not null);
  else
    select  i.ImageId,
            i.ImageFileName,
            i.TopLeftRow,
            i.TopLeftCol,
            i.PixelCount,
            i.CentroidRow,
            i.CentroidCol,
            i.Class1Id,
            (select ClassName from Classes c1 where  c1.ClassId = i.Class1Id) as Class1Name,
            i.Class1Prob,
            i.Class2Prob,
            i.ClassValidatedId,
            (select ClassName from Classes c2 where  c2.ClassId = i.ClassValidatedId) as ClassNameValidated,
            i.Depth  as ImagesDepth,
            id.CTDDateTime  as  CtdDateTime,
            fd.* 

      from images i
          join (ImageGroupEntries ig)  on  (ig.ImageId = i.ImageId)
          left join(featuredata fd)    on  (fd.ImageId = i.ImageId)
          left join(InstrumentData id)  on(id.SipperFileId=i.SipperFileid  and  id.ScanLine = (4096 * floor(i.TopLeftRow / 4096)))
          where  ig.ImageGroupId = _imageGroupId;
  end if;
end
//
delimiter ;




/*****************************************************************************************************************/
/*****************************************************************************************************************/
/* use  pices_new; */

drop procedure  if exists FeatureDataGetOneImageGroupClassName;

delimiter //

create procedure  FeatureDataGetOneImageGroupClassName (in  _imageGroupId   int,
                                                        in  _class1Name     varchar(64),
                                                        in  _classKeyToUse  char
                                                       )
begin
  declare  _class1Id       int  unsigned  default 0;

  set  _class1Id  = (select c.ClassId  from Classes c  where c.ClassName = _class1Name);

  if  _classKeyToUse = 'V'  then
    select  i.ImageId,
            i.ImageFileName,
            i.TopLeftRow,
            i.TopLeftCol,
            i.PixelCount,
            i.CentroidRow,
            i.CentroidCol,
            i.Class1Id,
            (select ClassName from Classes c1 where  c1.ClassId = i.Class1Id) as Class1Name,
            i.Class1Prob,
            i.Class2Prob,
            i.ClassValidatedId,
            (select ClassName from Classes c2 where  c2.ClassId = i.ClassValidatedId) as ClassNameValidated,
            i.Depth  as ImagesDepth,
            id.CTDDateTime  as  CtdDateTime,
            fd.* 

      from images i
          join (ImageGroupEntries ig)  on  (ig.ImageId = i.ImageId)
          left join(featuredata fd)    on  (fd.ImageId = i.ImageId)
          left join(InstrumentData id)  on(id.SipperFileId=i.SipperFileid  and  id.ScanLine = (4096 * floor(i.TopLeftRow / 4096)))
          where  (ig.ImageGroupId = _imageGroupId)  and  (i.ClassValidatedId = _class1Id);
  else
    select  i.ImageId,
            i.ImageFileName,
            i.TopLeftRow,
            i.TopLeftCol,
            i.PixelCount,
            i.CentroidRow,
            i.CentroidCol,
            i.Class1Id,
            (select ClassName from Classes c1 where  c1.ClassId = i.Class1Id) as Class1Name,
            i.Class1Prob,
            i.Class2Prob,
            i.ClassValidatedId,
            (select ClassName from Classes c2 where  c2.ClassId = i.ClassValidatedId) as ClassNameValidated,
            i.Depth  as ImagesDepth,
            id.CTDDateTime  as  CtdDateTime,
            fd.* 

      from images i
          join (ImageGroupEntries ig)  on  (ig.ImageId = i.ImageId)
          left join(featuredata fd)    on  (fd.ImageId = i.ImageId)
          left join(InstrumentData id)  on(id.SipperFileId=i.SipperFileid  and  id.ScanLine = (4096 * floor(i.TopLeftRow / 4096)))
          where  (ig.ImageGroupId = _imageGroupId)  and (i.Class1Id = _class1Id);
  end if;
end
//
delimiter ;







/*****************************************************************************************************************/
/*****************************************************************************************************************/
/* use  pices_new; */

drop procedure  if exists FeatureDataUpdateInstrumentDataFields;

delimiter //

create procedure  FeatureDataUpdateInstrumentDataFields (in  _imageFileName    varchar(64),
                                                         in  _flowRate1        float,
                                                         in  _flowRate2        float,
                                                         in  _depth            real,
                                                         in  _salinity         real,
                                                         in  _oxygen           real,
                                                         in  _fluorescence     real
                                                        )
begin
  set @imageId = (select i.ImageId  from  Images i  where  i.ImageFileName = _imageFileName);
  
  update FeatureData fd
     set fd.FlowRate1     = _flowRate1,
         fd.FlowRate2     = _flowRate2,
         fd.Depth         = _depth,
         fd.Salinity      = _salinity,
         fd.Oxygen        = _oxygen,
         fd.Florences     = _fluorescence
     where  fd.ImageId = @imageId;
end
//
delimiter ;





/**********************************************************************************************************************/
/**********************************************************************************************************************/
/* use  pices_new; */

drop procedure  if exists FeatureDataUpdateInstrumentDataFieldsForSipperFile ;

delimiter //

create procedure  FeatureDataUpdateInstrumentDataFieldsForSipperFile (in  _sipperFileName   varChar(64))
begin
  declare done     int           default 0;
  declare sfid     int unsigned  default 0;
  declare ifid     int unsigned  default 0;
  declare sl       long          default 0;
  
  
  declare cur1 cursor for  select fd.ImageId, i.TopLeftRow as ScanLine
                                  from FeatureData fd
                                  join(Images i)        on(i.ImageId       = fd.ImageId)
                                  join(SipperFiles sf)  on(sf.SipperFileId = fd.SipperFileId)
                                  where    s.SipperFileName = _sipperFileName;

  declare continue HANDLER for not found set done = 1;

  set  sfid = (select sf.SipperFileId from SipperFiles sf  where  sf.SipperFileName = _sipperFileName);
  
  open cur1;

  repeat
    fetch  cur1 into ifid, sl;
    if not done then
       update   FeatureData fd  set
                fd.FlowRate1 = (select id.FlowRate1    from InstrumentData id  where id.SipperFileId = sfid  and  id.ScanLine >= sl  limit 1),
                fd.Depth     = (select id.Depth        from InstrumentData id  where id.SipperFileId = sfid  and  id.ScanLine >= sl  limit 1),
                fd.Salinity  = (select id.Salinity     from InstrumentData id  where id.SipperFileId = sfid  and  id.ScanLine >= sl  limit 1),
                fd.Oxygen    = (select id.Oxygen       from InstrumentData id  where id.SipperFileId = sfid  and  id.ScanLine >= sl  limit 1),
                fd.Florences = (select id.Fluorescence from InstrumentData id  where id.SipperFileId = sfid  and  id.ScanLine >= sl  limit 1)
       where  (fd.ImageId = ifid);
    end if;
  until done end repeat;

  close cur1;
end;
//
delimiter ;






/**********************************************************************************************************************/
/**********************************************************************************************************************/

/* use  pices_new; */

drop procedure  if exists FeatureDataGetScanLinesPerMeterProfile ;

delimiter //

create procedure  FeatureDataGetScanLinesPerMeterProfile (in  _sipperFileName   varChar(48))
begin
  select truncate(id.Depth, 0) as StartDepth, count(*) * 4096 as ScanLineCount
         from InstrumentData id
         join (SipperFiles sf)  on (sf.SipperFileId = id.SipperFileId)
         where  sf.SipperFileName = _sipperFileName
         group by (truncate(id.Depth, 0))
         order by (truncate(id.Depth, 0))
;
end;
//
delimiter ;

