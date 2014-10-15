delimiter ;


/**********************************************************************************************************************/
drop procedure  if exists ImagesInsert;

delimiter //

CREATE PROCEDURE ImagesInsert (in  _ImageFileName         varChar(64),
                               in  _SipperFileName        varChar(48),
                               in  _byteOffset            bigint(13),
                               in  _topLeftRow            int(10),
                               in  _topLeftCol            int,
                               in  _height                int,            
                               in  _width                 int,           
                               in  _pixelCount            int,
                               in  _connectedPixelDist    tinyint unsigned,
                               in  _extractionLogEntryId  int unsigned,
                               in  _classLogEntryId       int unsigned,
                               in  _centroidRow           int(11),
                               in  _centroidCol           int,
                               in  _class1Name            varChar(64),
                               in  _class1Prob            float,           
                               in  _class2Name            varChar(64),
                               in  _class2Prob            float,
                               in  _validatedClassName    varChar(64),
                               in  _depth                 float,
                               in  _imageSize             float,
                               in  _sizeCoordinates       varChar(32),
                               in  _ThumbNail             blob
                              )

BEGIN
  declare   _sipperFileId      int  unsigned  default 0;
  declare   _class1Id          int  default 0;
  declare   _class2Id          int  default 0;
  declare   _classValidatedId  int  default 0;

  set  _sipperFileId     = (select sf.SipperFileId from SipperFiles sf  where  sf.SipperFileName = _SipperFileName);
  set  _class1Id         = (select c1.ClassId      from Classes     c1  where  c1.ClassName      = _class1Name);
  set  _class2Id         = (select c2.ClassId      from Classes     c2  where  c2.ClassName      = _class2Name);
  set  _classValidatedId = (select c2.ClassId      from Classes     c2  where  c2.ClassName      = _validatedClassName);
  
  if  (_class1Id is null)  then
    call ImageClassInsert(_class1Name, "AllClasses", "Added by 'ImagesInsert'");
    set  _class1Id = (select  c.ClassId  from  Classes c  where c.ClassName = _class1Name);
  end if;


  Insert into Images (ImageFileName, SipperFileId, 
                      ByteOffset, TopLeftRow, TopLeftCol, Height, Width, PixelCount, 
                      ConnectedPixelDist, ExtractionLogEntryId, ClassLogEntryId, 
                      CentroidRow, CentroidCol, 
                      Class1Id, Class1Prob, Class2Id, Class2Prob, ClassValidatedId, 
                      Depth, ImageSize, SizeCoordinates,
                      ThumbNail
                     )
         values (_imageFileName,
                 _sipperFileId,
                 _byteOffset,
                 _topLeftRow,
                 _topLeftCol,
                 _height,
                 _width,
                 _pixelCount,
                 _connectedPixelDist,
                 _extractionLogEntryId,
                 _classLogEntryId,
                 _centroidRow,
                 _centroidCol,
                 _class1Id,
                 _class1Prob,
                 _class2Id,
                 _class2Prob,
                 _classValidatedId,
                 _depth,
                 _imageSize,
                 _sizeCoordinates,
                 _ThumbNail
                );
end
//
delimiter ;








/**********************************************************************************************************************/
drop procedure  if exists ImagesUpdate;

delimiter //

create procedure  ImagesUpdate (in  _ImageId               int unsigned,
                                in  _ImageFileName         varChar(64),
                                in  _SipperFileName        varChar(48),
                                in  _byteOffset            bigint(14),
                                in  _topLeftRow            int(10),
                                in  _topLeftCol            int,
                                in  _height                int,            
                                in  _width                 int,           
                                in  _pixelCount            int,           
                                in  _connectedPixelDist    tinyint unsigned,
                                in  _extractionLogEntryId  int unsigned,
                                in  _classLogEntryId       int unsigned,
                                in  _centroidRow           int(11),
                                in  _centroidCol           int,
                                in  _class1Name            varChar(64),
                                in  _class1Prob            float,           
                                in  _class2Name            varChar(64),
                                in  _class2Prob            float,
                                in  _validatedClassName    varChar(64),
                                in  _depth                 float,
                                in  _imageSize             float,
                                in  _sizeCoordinates       varchar(32),
                                in  _ThumbNail             blob
                               )

begin
  declare   _sipperFileId      int  unsigned  default 0;
  declare   _class1Id          int  default 0;
  declare   _class2Id          int  default 0;
  declare   _classValidatedId  int  default 0;

  set  _sipperFileId     = (select sf.SipperFileId from SipperFiles sf  where  sf.SipperFileName = _SipperFileName);
  set  _class1Id         = (select c1.ClassId      from Classes     c1  where  c1.ClassName      = _class1Name);
  set  _class2Id         = (select c2.ClassId      from Classes     c2  where  c2.ClassName      = _class2Name);
  set  _classValidatedId = (select c2.ClassId      from Classes     c2  where  c2.ClassName      = _validatedClassName);

  if  (_class1Id is null)  then
    call ImageClassInsert(_class1Name, "AllClasses", "Added by 'ImagesUpdate'");
    set  _class1Id = (select  c.ClassId  from  Classes c  where c.ClassName = _class1Name);
  end if;

  update Images i  set  i.ImageFileName        = _ImageFileName,
                        i.SipperFileId         = _sipperFileId,
                        i.ByteOffset           = _byteOffset,
                        i.TopLeftRow           = _topLeftRow,
                        i.TopLeftCol           = _topLeftCol,
                        i.Height               = _height,
                        i.Width                = _width,
                        i.PixelCount           = _pixelCount,
                        i.ConnectedPixelDist   = _connectedPixelDist,
                        i.ExtractionLogEntryId = _extractionLogEntryId,
                        i.ClassLogEntryId      = _classLogEntryId,
                        i.CentroidRow          = _centroidRow,
                        i.CentroidCol          = _centroidCol,
                        i.Class1Id             = _class1Id,
                        i.Class1Prob           = _class1Prob,
                        i.Class2Id             = _class2Id,
                        i.Class2Prob           = _class2Prob,
                        i.ClassValidatedId     = _classValidatedId,
                        i.Depth                = _depth,
                        i.ImageSize            = _imageSize,
                        i.SizeCoordinates      = _sizeCoordinates,
                        i.ThumbNail            = _ThumbNail
         where  i.ImageId = _ImageId;
end
//
delimiter ;






/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesEraseSipperFile;

delimiter //
create procedure  ImagesEraseSipperFile (in  _SipperFileName   varChar(48))
begin
  declare  _sipperFileID     int unsigned  default 0;

  set _sipperFileId = (select sf.SipperFileId  from SipperFiles sf  where sf.SipperFileName = _sipperFileName);

  delete ImagesFullSize   FROM ImagesFullSize   INNER JOIN Images i
     where  ImagesFullSize.ImageId = i.ImageId  and  i.SipperFileid = _sipperFileID;

  delete ImageGroupEntries  from ImageGroupEntries   inner join Images i
     where  ImageGroupEntries.ImageId = i.ImageId  and  i.SipperFileid = _sipperFileID;

  delete from  InstrumentData  where sipperFileId = _sipperFileID;
  delete from  FeatureData     where sipperFileId = _sipperFileID;
  delete from  Images          where sipperFileId = _sipperFileID;
end
//
delimiter ;





/************************************************************************************************************************
 *  This routine is meant to deleat data related to a specific SIPPER file for a specified
 *  size range.
 */
drop procedure  if  exists ImagesEraseSipperFileSizeRange;
delimiter //
create procedure  ImagesEraseSipperFileSizeRange (in  _sipperFileName  varChar(48),
                                                  in  _minSize         int,
                                                  in  _maxSize         int
                                                 )
begin
  declare  _sipperFileID     int unsigned  default 0;

  set _sipperFileId = (select sf.SipperFileId  from SipperFiles sf  where sf.SipperFileName = _sipperFileName);

  delete ImagesFullSize   FROM ImagesFullSize   INNER JOIN Images i
     where  ImagesFullSize.ImageId = i.ImageId   and  
            i.SipperFileid =  _sipperFileID      and
            i.PixelCount   >= _minSize           and
            i.PixelCount   <= _maxSize;
  commit;

  delete ImageGroupEntries  from ImageGroupEntries   inner join Images i
     where  ImageGroupEntries.ImageId = i.ImageId  and  
            i.SipperFileid =  _sipperFileID        and
            i.PixelCount   >= _minSize             and
            i.PixelCount   <= _maxSize;
  commit;

  delete from  InstrumentData  where sipperFileId = _sipperFileID;
  commit;
 
  delete FeatureData   FROM FeatureData   INNER JOIN  Images i
     where  FeatureData.ImageId = i.ImageId   and  
            i.SipperFileId =  _sipperFileID      and
            i.PixelCount   >= _minSize           and
            i.PixelCount   <= _maxSize;
  commit;

  delete from  Images
    where Images.SipperFileId =  _sipperFileID      and
          Images.PixelCount   >= _minSize           and
          Images.PixelCount   <= _maxSize;
  commit;
end;  
//
delimiter ;






/************************************************************************************************************************
 *  This routine is meant to deleat data related to a specific SIPPER file for a specified
 *  size range.
 */
drop procedure  if  exists ImagesDeleteOneImage;
delimiter //
create procedure  ImagesDeleteOneImage (in  _imageFileName  varChar(64))
begin
  declare  _imageID     int unsigned  default 0;

  set _imageId = (select i.imageId  from images  i  where i.ImageFileName = _imageFileName);

  delete   FROM ImagesFullSize 
     where  ImagesFullSize.ImageId = _imageId;
  commit;

  delete from ImageGroupEntries
     where  ImageGroupEntries.ImageId = _imageId;
  commit;

  delete FROM FeatureData
     where  FeatureData.ImageId = _imageId;
  commit;

  delete from  Images
    where Images.ImageId = _imageID;
  commit;
  
  Insert into ImagesLogEntries (ImageId, ImageFileName, ValidatedClassName, DateTimeOccured, Action, DataBaseUserName)
         values (_imageId,
                 _imageFileName,
                 "",
                 @_DateNow,
                 "D",
                 User() 
                );     
end;  
//
delimiter ;





/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop  function  if exists ImagesBuildConditions;

delimiter //
create function ImagesBuildConditions
       (_imageGroupName    varchar(64),
        _sipperFileName    varchar(48),
        _imageClassName    varChar(64),
        _classKeyToUse     char,
        _probMin           float,
        _probMax           float,
        _sizeMin           int,
        _sizeMax           int,
        _depthMin          float,
        _depthMax          float
       )

  returns varchar(1024)
  deterministic

begin
  declare  _sipperFileId  int unsigned  default 0;
  declare  _classId       int unsigned  default 0;
  declare  _whereClauses   varchar(1024)  default "";

  set  @imageGroupId = -1;

  if   (!isnull(_imageGroupName)) and  (_imageGroupName != "") then
     set  @imageGroupId = (select ig.ImageGroupId  from  ImageGroup ig  where ig.ImagegroupName = _imageGroupName);
     if  (isnull(@imageGroupId))  then
        set @imageGroupId = -1;
     end if;
  end if;

  set  _whereClauses = "";

  if   (!isnull(_imageGroupName)) and  (_imageGroupName != "") then
    set _whereClauses = concat(_whereClauses, " and ige.ImageGroupId = ", @imageGroupId);
  end if;

  if  (!isnull(_sipperFileName)  and  (_sipperFileName != ""))  then
    set  _sipperFileId = (select Sf.SipperFileId from SipperFiles sf  where  sf.SipperFileName = _sipperFileName);
    set  _whereClauses = concat(_whereClauses, " and i.SipperFileId = ", _sipperFileId);
  end if;

 if  (!isnull(_imageClassName)  and  (_imageClassName != ""))  then
    set _classId = (select c.ClassId from Classes c  where c.ClassName = _imageClassName);

    if  (_classKeyToUse = "V")  then
      set _whereClauses = concat(_whereClauses, " and i.ClassValidatedId = ", _classId);
    else
      set _whereClauses = concat(_whereClauses, " and i.class1Id = ", _classId);
    end if;
  else
    if  (_classKeyToUse = "V")  then
      set _whereClauses = concat(_whereClauses, " and   (i.ClassValidatedId  is not null) and  ClassValidatedId > 0  ");
    end if;
  end if;

  if  (_probMin > 0.0)  then
    set _whereClauses = concat(_whereClauses, " and i.Class1Prob >= ", _probMin);
  end if;

  if  (_probMax < 1.0)  then
    set _whereClauses = concat(_whereClauses, " and i.Class1Prob <= ", _probMax);
  end if;

  if  (_sizeMin > 0.0)  then
    set _whereClauses = concat(_whereClauses, " and i.PixelCount >= ", _sizeMin);
  end if;

  if  (_sizeMax > 0.0)  then
    set _whereClauses = concat(_whereClauses, " and i.PixelCount <= ", _sizeMax);
  end if;

  if  (_depthMin > 0.0)  then
    set _whereClauses = concat(_whereClauses, " and i.Depth >= ", _depthMin);
  end if;

  if  (_depthMax > 0.0)  then
    set _whereClauses = concat(_whereClauses, " and i.Depth <= ", _depthMax);
  end if;

  if  (CHAR_LENGTH(_whereClauses) > 5)  then
    if  (substr(_whereClauses, 1, 5) = " and ") then
      set  _whereClauses = substr(_whereClauses, 6);
    end if;
  end if;


  return  _whereClauses;
end;
//
delimiter ;






/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesGetClassStatistics;

delimiter //

create procedure  ImagesGetClassStatistics (in  _imageGroupName    varChar(64),
                                            in  _sipperFileName    varChar(48),
                                            in  _className         varChar(64),
                                            in  _classKeyToUse     char,       /* 'P' = use Prediced Class,  'V' = use validated class */
                                            in  _probMin           float,      /* between 0 - 1.                                       */
                                            in  _probMax           float,      /* between 0 - 1, must be larger than _probMin.         */
                                            in  _sizeMin           int,        /* Minimum number of pixels.                   */
                                            in  _sizeMax           int,        /* Leave at '0' for all sizes.                 */
                                            in  _depthMin          float,      /* Minimum depth,  0 = surface                 */
                                            in  _depthMax          float       /* M<aximum depth, 0 = you dont care.          */
                                           )

begin
  declare  _whereClauses   varchar(1024);

  set @s = "select ";
  if  (_classKeyToUse = 'P')  then
    set @s = concat (@s, "i.Class1Id  as ClassId, ");
    set @s = concat (@s, "(select c1.ClassName  from  Classes c1  where c1.ClassId = i.Class1Id) as ClassName, ");
    set @s = concat (@s, "count(i.Class1Id) \n");
  else
    set @s = concat (@s, "i.ClassValidatedId  as  ClassId, ");
    set @s = concat (@s, "(select c1.ClassName  from  Classes c1  where c1.ClassId = i.ClassValidatedId) as ClassName, ");
    set @s = concat (@s, "count(i.ClassValidatedId) \n");
  end if;
    
  set @s = concat (@s, "from Images i \n");

  if   (!isnull(_imageGroupName)) and  (_imageGroupName != "") then
    set @s = concat(@s, "  join(ImageGroupEntries ige)  on(ige.ImageId = i.ImageId)", " \n");
  end if;

  set  _whereClauses = ImagesBuildConditions
       (_imageGroupName, _sipperFileName, _className, _classKeyToUse,
        _probMin,  _probMax,
        _sizeMin,  _sizeMax,
        _depthMin, _depthMax
       );


/* "ige.ImageGroupId = 120 and i.SipperFileId = 440" */

  if  (CHAR_LENGTH(_whereClauses) > 5)  then
    if  (substr(_whereClauses, 1, 5) = " and ") then
      set  _whereClauses = substr(_whereClauses, 6);
    end if;
    set  @s = concat(@s, "  where ", _whereClauses);
  end if;

  set @s = concat(@s, " group by ClassName");
  
  /* 
  insert into  ImageQueryLog(DateTime, SqlStr)
          values(now(), @s);
  */
  PREPARE stmt1 FROM @s;
  EXECUTE stmt1;
  DEALLOCATE PREPARE stmt1;

end
//

delimiter ;






/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesGetDepthStatistics;

delimiter //
create procedure  ImagesGetDepthStatistics (in  _imageGroupName    varChar(64),
                                            in  _sipperFileName    varChar(48),
                                            in  _depthIncrements   real,
                                            in  _className         varChar(64),
                                            in  _classKeyToUse     char(1),      /* 'P' = use Prediced Class,  'V' = use validated class */
                                            in  _probMin           float,        /* between 0 - 1.                                */
                                            in  _probMax           float,        /* between 0 - 1, must be larger than _probMin.  */
                                            in  _sizeMin           int,          /* Minimum number of pixels.                     */
                                            in  _sizeMax           int           /* Maximum depth, 0 = you dont care.             */
                                           )

begin
  declare  _whereClauses   varchar(1024);

  set  @incStr = concat("Floor(i.Depth/", _depthIncrements, ")");

  set  @s = concat ("select ", @incStr, " as Increment, count(", @incStr, ") as Count  from Images i");

  if   (!isnull(_imageGroupName)) and  (_imageGroupName != "") then
    set @s = concat(@s, "  join(ImageGroupEntries ige)  on(ige.ImageId = i.ImageId)");
  end if;

  set  _whereClauses = ImagesBuildConditions
       (_imageGroupName, _sipperFileName, _className, _classKeyToUse,
        _probMin,  _probMax,
        _sizeMin,  _sizeMax,
         0, 0
       );

  if  (CHAR_LENGTH(_whereClauses) > 5)  then
    if  (substr(_whereClauses, 1, 5) = " and ") then
      set  _whereClauses = substr(_whereClauses, 6);
    end if;
    set  @s = concat(@s, "  where ", _whereClauses);
  end if;

  set @s = concat(@s, " group by ", @incStr);

  /* select  @s; */
  PREPARE stmt1 FROM @s;
  EXECUTE stmt1;
  DEALLOCATE PREPARE stmt1;
end
//

delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop  function  if exists ImagesQuerySelectFieldNames;

delimiter //
create function ImagesQuerySelectFieldNames (_includeThumbNail  bool)
  returns varchar(1024)  deterministic

begin
  set @s = "i.ImageId, i.ImageFileName, ";
  set @s = concat(@s, "i.SipperFileId, (select sf.SipperFileName from SipperFiles sf where sf.SipperFileId = i.SipperFileId) as SipperFileName, ",  "\n",
                      "i.Class1Id,  (select c1.ClassName from Classes c1  where c1.ClassId = i.Class1Id) as Class1Name, i.Class1Prob, ",            "\n",
                      "i.Class2Id,  (select c2.ClassName from Classes c2  where c2.ClassId = i.Class2Id) as Class2Name, i.Class2Prob, ",            "\n",
                      "i.ClassValidatedId,  (select c3.ClassName from Classes c3  where c3.ClassId = i.ClassValidatedId) as ClassNameValidated, ",  "\n",
                      "i.TopLeftRow, i.TopLeftCol, i.PixelCount, i.ConnectedPixelDist, i.ExtractionLogEntryId, i.ClassLogEntryId, i.Height, ",      "\n",
                      "i.Width, i.ByteOffset, i.depth, i.ImageSize, i.SizeCoordinates",     "\n"
                 );
  if  (_includeThumbNail = "1")  then
    set @s = concat(@s, ", i.ThumbNail");
  end if;

  return  @s;
end;
//
delimiter ;





/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesLoadByImageId;

delimiter //
create procedure  ImagesLoadByImageId (in  _imageId  int unsigned )

begin
  select i.*,
        (select sf.SipperFileName from SipperFiles sf where sf.SipperFileId = i.SipperFileId) as SipperFileName,
        (select c1.ClassName from Classes c1  where c1.ClassId = i.Class1Id)                  as Class1Name,
        (select c2.ClassName from Classes c2  where c2.ClassId = i.Class2Id)                  as Class2Name,
        (select c3.ClassName from Classes c3  where c3.ClassId = i.ClassValidatedId)          as ClassNameValidated
   from  Images i  where  i.ImageId = _imageId;
end;
//
delimiter ;







/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesLoadByImageFileName;

delimiter //
create procedure  ImagesLoadByImageFileName (in  _imageFileName    varChar(64))

begin
  select i.*,
        (select sf.SipperFileName from SipperFiles sf where sf.SipperFileId = i.SipperFileId) as SipperFileName,
        (select c1.ClassName from Classes c1  where c1.ClassId = i.Class1Id)                  as Class1Name,
        (select c2.ClassName from Classes c2  where c2.ClassId = i.Class2Id)                  as Class2Name,
        (select c3.ClassName from Classes c3  where c3.ClassId = i.ClassValidatedId)          as ClassNameValidated
   from  Images i  where  i.ImageFileName = _imageFileName;
end;
//
delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesQueryByImageGroup;

delimiter //
create procedure  ImagesQueryByImageGroup (in  _imageGroupName    varChar(64),
                                           in  _includeThumbNail  bool
                                          )
begin
  set  @s = concat("select ", ImagesQuerySelectFieldNames(_includeThumbNail), "  ");
  set  @s = concat(@s, "from Images i  \n");
  set  @s = concat(@s, "join(ImageGroupEntries ige) on(ige.ImageId = i.ImageId)  \n");
  set  @s = concat(@s, "where ige.ImageGroupId = (select g.ImageGroupId from ImageGroup g  where g.ImageGroupName = ", Quote(_imageGroupName), ")");
  
  PREPARE stmt1 FROM @s;
  EXECUTE stmt1;
  DEALLOCATE PREPARE stmt1;
end;
//
delimiter ;








/**********************************************************************************************************************/
/**********************************************************************************************************************/

drop procedure  if exists ImagesQuery;

delimiter //
create procedure  ImagesQuery (in  _imageGroupName         varChar(64),
                               in  _sipperFileName         varChar(48),
                               in  _className              varChar(64),
                               in  _classKeyToUse          char,          /* 'P' = use Prediced Class,  'V' = use validated class */
                               in  _probMin                float,         /* between 0 - 1.                                       */
                               in  _probMax                float,         /* between 0 - 1, must be larger than _probMin.         */
                               in  _sizeMin                int,           /* Minimum number of pixels.                            */
                               in  _sizeMax                int,           /* Maximum depth, 0 = you dont care.                    */
                               in  _depthMin               float,
                               in  _depthMax               float,
                               in  _restartImageId         int unsigned,  /* Loaded images will have to be greater than this image */
                               in  _limit                  int,
                               in  _includeThumbNail       bool
                              )
begin

 declare  _whereClauses   varchar(1024);
 declare  _s              varchar(1024);

  set _s = concat("select ", ImagesQuerySelectFieldNames(_includeThumbNail), "\n  from Images i  \n");

  if  (isnull(_imageGroupName)) or  (_imageGroupName = "")   then
    if   (!isnull(_className))  and  (_className != "")   then
      if  (_classKeyToUse = "P")  then
        set  _s = concat(_s, "   use index(ClassKey) ");
      else
        set  _s = concat(_s, "   use index(ValidatedKey) ");
      end if;
    elseif  (_depthMin > 0.0)  then
      set  _s = concat(_s, "   use index(DepthKey) ");
    end if;
  end if;

  if   (!isnull(_imageGroupName)) and  (_imageGroupName != "") then
    set _s = concat(_s, "  join(ImageGroupEntries ige)  on(ige.ImageId = i.ImageId)");
  end if;

  set  _whereClauses = ImagesBuildConditions
       (_imageGroupName, _sipperFileName, _className, _classKeyToUse,
        _probMin,  _probMax,
        _sizeMin,  _sizeMax,
        _depthMin, _depthMax
       );

  if  (_restartImageId > 0)  then
    if  ((!isnull(_whereClauses))  and  (_whereClauses != ""))  then
      set _whereClauses = concat(_whereClauses, " and ");
    end if;

    set  _whereClauses = concat(_whereClauses, "i.ImageId > ", _restartImageId);
  end if;


  if  ((!isnull(_whereClauses))  and  (_whereClauses != ""))  then
    set  _s = concat(_s, " where ", _whereClauses, "\n");
  end if;

  if  (_limit > 0)  then
    set _s = concat(_s, " limit ", _limit);
  end if;
  
  /*
  insert into  ImageQueryLog(DateTime, SqlStr)
          values(now(), _s);
  */

  set @s = _s;

  PREPARE stmt1 FROM @s;
  EXECUTE stmt1;
  DEALLOCATE PREPARE stmt1;
end;
//
delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesQueryByGrouop;

delimiter //
create procedure  ImagesQueryByGrouop
                              (in  _imageGroupName         varChar(64),
                               in  _cruiseName             varchar(10),
                               in  _stationName            varChar(10),
                               in  _deploymentNum          varChar(4),
                               in  _sipperFileName         varChar(48),
                               in  _className              varChar(64),
                               in  _classKeyToUse          char,
                               in  _probMin                float,
                               in  _probMax                float,
                               in  _sizeMin                int,
                               in  _sizeMax                int,
                               in  _depthMin               float,
                               in  _depthMax               float,
                               in  _restartImageId         int unsigned,  /* Loaded images will have to be greater than this image */
                               in  _limit                  int,
                               in  _includeThumbNail       bool
                              )
begin

  declare  _imageGroupId  int  unsigned   default 0;
  declare  _classId       int             default 0;

  set  _imageGroupId = (select  ig.ImageGroupId  from  ImageGroup ig  where  ig.ImageGroupName = _imageGroupName);


  set @s = concat("select  ", ImagesQuerySelectFieldNames(_includeThumbNail),     "\n",
                  "  from ImageGroupEntries ige ",                                "\n",
                  "  join(Images i)  on(i.ImageId = ige.ImageId)",                "\n",
                  "  where  ige.ImageGroupId = ", _imageGroupId, "  and  ige.ImageId > ", _restartImageId, "\n"
                 );

  set  @sfWhereClause = "";
  if  ((!isnull(_cruiseName))  and  (_cruiseName != ""))  then
     set  @sfWhereClause = concat(@sfWhereClause, "  and  sf.CruiseName = ", quote(_cruiseName));
  end if;


  if  ((!isnull(_stationName))  and  (_stationName != ""))  then
     set  @sfWhereClause = concat(@sfWhereClause, "  and  sf.StationName = ", quote(_stationName));
  end if;

  if  ((!isnull(_deploymentNum))  and  (_deploymentNum != ""))  then
     set  @sfWhereClause = concat(@sfWhereClause, "  and  sf.deploymentNum = ", quote(_deploymentNum));
  end if;


  if  ((!isnull(_sipperFileName))  and  (_sipperFileName != ""))  then
     set  @sfWhereClause = concat(@sfWhereClause, "  and  sf.SipperFileName = ", quote(_sipperFileName));
  end if;

  if  (CHAR_LENGTH(@sfWhereClause) > 7)  then
    set  @sfWhereClause = substr(@sfWhereClause, 8);
    set  @s = concat(@s, "  and  SipperFileId in (select sf.SipperFileId from SipperFiles sf  where  ", @sfWhereClause, ")\n");
  end if;

  set  @whereClauses = ImagesBuildConditions
       ("", "", _className, _classKeyToUse,
        _probMin,  _probMax,
        _sizeMin,  _sizeMax,
        _depthMin, _depthMax
       );

   if  ((!isnull(@whereClauses))  and  (@whereClauses != ""))  then
     set  @s = concat(@s, "  and  ", @whereClauses, "\n");
   end if;

  if  (_limit > 0)  then
    set @s = concat(@s, " limit ", _limit);
  end if;


  /* 
  insert into  ImageQueryLog(DateTime, SqlStr)
          values(now(), @s);
  */

  PREPARE stmt1 FROM @s;
  EXECUTE stmt1;
  DEALLOCATE PREPARE stmt1;
end;
//
delimiter ;














/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesQueryForScanLineRange;

delimiter //
create procedure  ImagesQueryForScanLineRange (in  _sipperFileName   varChar(48),
                                               in  _scanLineStart    int unsigned,
                                               in  _scanLineEnd      int unsigned
                                              )
begin
  declare  _sipperFileId  int unsigned  default  0;

  set _sipperFileId = (select  sf.SipperFileId  from  SipperFiles sf  where  sf.SipperFileName = _sipperFileName);

  set @s = concat("select ", ImagesQuerySelectFieldNames(true),
                             " from images i",
                             " where i.SipperFileId = ", _sipperFileId, " and ",
                             " i.TopLeftRow >= ", _scanLineStart,       " and ",
                             " i.TopLeftRow  < ", _scanLineEnd
                 );

  PREPARE stmt1 FROM @s;
  EXECUTE stmt1;
  DEALLOCATE PREPARE stmt1;

end;
//
delimiter ;






/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesImageFileNamesByScanLineRange;

delimiter //
create procedure  ImagesImageFileNamesByScanLineRange (in  _sipperFileName   varChar(48),
                                                       in  _scanLineStart    int unsigned,
                                                       in  _scanLineEnd      int unsigned
                                                      )
begin
  select i.ImageId, i.ImageFileName 
         from Images i  
         join(SipperFiles sf)  on(sf.SipperFileId = i.SipperFileId)
         where  sf.SipperFileName = _sipperFileName  and  
                i.TopLeftRow >= _scanLineStart       and  
                i.TopLeftRow <  _scanLineEnd;

end;
//
delimiter ;






/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesUpdatePredictions;

delimiter //
create procedure  ImagesUpdatePredictions (in  _imageFileName   varChar(64),
                                           in  _class1Name      varChar(64),
                                           in  _class1Prob      float,
                                           in  _class2Name      varChar(64),
                                           in  _class2Prob      float,
                                           in  _logEntryId      int unsigned
                                          )
begin
  declare  _class1Id  int unsigned  default 0;
  declare  _class2Id  int unsigned  default 0;
  
  set  _class1Id = (select  c.ClassId  from  Classes c  where c.ClassName = _class1Name);
  set  _class2Id = (select  c.ClassId  from  Classes c  where c.ClassName = _class2Name);
  
  if  (_class1Id is null)  then
    call ImageClassInsert(_class1Name, "AllClasses", "Added by 'ImagesUpdatePredictions'");
    set  _class1Id = (select  c.ClassId  from  Classes c  where c.ClassName = _class1Name);
  end if;
  
  
  update Images i  set  i.Class1Id        = _class1Id,
                        i.Class1Prob      = _class1Prob,
                        i.Class2Id        = _class2Id,
                        i.Class2Prob      = _class2Prob,
                        i.ClassLogEntryId = _logEntryId
         where  i.ImageFileName = _imageFileName;
end;
//
delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**
 *@param[in]  _predictionList  A tab delimited string that will contain the list of predictions.  Each
 *   image will be separated by line feed ('\n') charaters while each field for each uiage will be
 *   separated by 'tabs'('\t').  So if there are two images with predictions the string will look like
 *   "<imageId><\t><Class1Name><\t><Class1Prob><\t><Class2Name><\t><Class2Prob><\n><imageId><\t><Class1Name><\t><Class1Prob><\t><Class2Name><\t><Class2Prob>"
 *
 */
drop procedure  if exists ImagesUpdatePredictionsList;

delimiter //
create procedure  ImagesUpdatePredictionsList (in  _logEntryId       int unsigned,
                                               in  _predictionList   varChar(64000)
                                              )
begin
  declare  _idx             int default 0;
  declare  _idx2            int default 0;
  declare  _imageFileName   varchar(64);
  declare  _class1Id        int;
  declare  _class2id        int;
  declare  _class1Prob      float;
  declare  _class2Prob      float;
 
  declare  _nextLine        varchar(220);

  declare  _fieldNum        int  default 0;
  declare  _fieldIdx1       int  default 0;
  declare  _fieldIdx2       int  default 0;
  declare  _field           varchar(100);

  set  _idx = 1;

  while  (_idx < Char_Length(_predictionList))  do
    set _idx2 = Locate("\n", _predictionList, _idx);
    if  (_idx2 = 0)  then
      set _nextLine = substr(_predictionList, _idx);
      set _idx2 = Length(_predictionList);
    else
      set _nextLine = substr(_predictionList, _idx, _idx2 - _idx);
    end if;

    set _fieldNum  = 0;
    set _fieldIdx1 = 1;
    while  (_fieldIdx1 < Char_Length(_nextLine))  do
      set _fieldIdx2 = Locate("\t", _nextLine, _fieldIdx1);
      if  (_fieldIdx2 = 0)  then
        set _fieldIdx2 = Char_Length(_nextLine) + 1;
        set _field = substr(_nextLine, _fieldIdx1);
      else
        set _field = substr(_nextLine, _fieldIdx1, _fieldIdx2 - _fieldIdx1);
      end if;

      if  (_fieldNum = 0)  then
        set _imageFileName = _field;
      else if  (_fieldNum = 1)  then
        set _class1Id = _field;
      else if  (_fieldNum = 2)  then
        set _class1Prob = _field;
      else if  (_fieldNum = 3)  then
        set _class2Id = _field;
      else if  (_fieldNum = 4)  then
        set _class2Prob = _field;
      end if;
      end if;
      end if;
      end if;
      end if;

      set _fieldNum  = _fieldNum  + 1;
      set _fieldIdx1 = _fieldIdx2 + 1;
    end while;

    update Images i  set  i.Class1Id        = _class1Id,
                          i.Class1Prob      = _class1Prob,
                          i.Class2Id        = _class2Id,
                          i.Class2Prob      = _class2Prob,
                          i.ClassLogEntryId = _logEntryId
           where  i.ImageFileName = _imageFileName;

    set  _idx = _idx2 + 1;
  end while;
end;
//
delimiter ;







/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesUpdateValidatedClass;

delimiter //
create procedure  ImagesUpdateValidatedClass (in  _imageFileName   varChar(64),
                                              in  _className       varChar(64),
                                              in  _class1Prob      float
                                             )
begin
  declare  _imageId             int unsigned  default 0;
  declare  _validatedClassId    int unsigned  default 0;

  set  @_DateNow    = Now();

  set  _imageId          = (select i.ImageId   from  Images i   where  i.ImageFileName = _imageFileName);
  set  _validatedClassId = (select  c.ClassId  from  Classes c  where  c.ClassName     = _className);

  update Images i  set i.ClassValidatedId  = _validatedClassId,
                       i.Class1Id          = _validatedClassId,
                       i.Class1Prob        = _class1Prob
         where i.imageId = _imageId;
         
         
 Insert into ImagesLogEntries (ImageId, ImageFileName, ValidatedClassName, DateTimeOccured, Action, DataBaseUserName)
         values (_imageId,
                 _imageFileName,
                 _className,
                 @_DateNow,
                 "V",
                 User() 
                );         
         
end;
//
delimiter ;





/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesUpdateImageSize;

delimiter //
create procedure  ImagesUpdateImageSize (in  _imageFileName     varChar(64),
                                         in  _sizeCoordinates   varChar(32),
                                         in  _imageSize         float
                                        )
begin
  update  Images i  set i.SizeCoordinates  = _sizeCoordinates,
                        i.ImageSize        = _imageSize
         where i.ImageFileName = _imageFileName;
end;
//
delimiter ;





/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesRemoveValidation;

delimiter //
create procedure  ImagesRemoveValidation (in  _imageFileName   varChar(64))
begin
  declare  _imageId             int unsigned  default 0;
  declare  _validatedClassId    int unsigned  default 0;
  declare  _className           varchar(64)  default "";

  set  @_DateNow  = Now();
  set  _imageId   = (select i.ImageId           from  Images i where  i.ImageFileName = _imageFileName);

  update Images i  set i.ClassValidatedId  = NULL
         where i.ImageId       = _imageId;
         
  Insert into ImagesLogEntries (ImageId, ImageFileName, ValidatedClassName, DateTimeOccured, Action, DataBaseUserName)
         values (_imageId,
                 _imageFileName,
                 "",
                 @_DateNow,
                 "R",
                 User() 
                );           
end;
//
delimiter ;








/**********************************************************************************************************************/
drop procedure  if exists ImagesUpdateInstrumentDataFields ;

delimiter //
create procedure  ImagesUpdateInstrumentDataFields (in  _sipperFileName   varChar(48),
                                                    in  _scanLineStart    int unsigned,
                                                    in  _scanLineEnd      int unsigned,
                                                    in  _flowRate1        float,
                                                    in  _flowRate2        float,
                                                    in  _depth            float,
                                                    in  _salinity         float,
                                                    in  _oxygen           float,
                                                    in  _fluorescence     float
                                                   )
begin
  declare done           int default 0;
  declare ifn            varchar(64);
  declare _sipperFileId  int unsigned default 0;
  declare _imageId       int unsigned default 0;


  declare cur1 cursor for  select i.ImageId, i.ImageFileName
                                  from Images i
                                  join (SipperFiles sf)  on(sf.SipperFileId = i.SipperFileId)
                                  where  sf.SipperFileName = _sipperFileName  and
                                         i.TopLeftRow >= _scanLineStart       and
                                         i.TopLeftRow <  _scanLineEnd;

  declare continue HANDLER for not found set done = 1;


  set  _sipperFileId = (select sf.SipperFileId  from  SipperFiles sf  where  sf.SipperFileName = _sipperFileName);



  update   Images i  set  i.Depth = _depth
            where  (i.SipperFileId = _sipperFileId)
              and  (i.TopLeftRow >= _scanLineStart)   and  (i.TopLeftRow < _scanLineEnd);

  open cur1;

  repeat
    fetch  cur1 into _imageId, ifn;
    if not done then
      call FeatureDataUpdateInstrumentDataFields (ifn, _flowRate1, _flowRate2, _depth, _salinity, _oxygen, _fluorescence);
    end if;
  until done end repeat;

  close cur1;
end;
//
delimiter ;



/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesChangeClassName;

delimiter //
create procedure  ImagesChangeClassName (in  _sipperFileName   varChar(14),
                                         in  _oldClassName     varChar(64),
                                         in  _newClassName     varChar(64)
                                        )
begin
  /* Now that use ClassId there is nothing to do anymore. */
end;
//
delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure  if exists ImagesMergeClasses;

delimiter //
create procedure  ImagesMergeClasses (in  _sipperFileName   varChar(48),
                                      in  _srcClassName     varChar(64),
                                      in  _destClassName    varChar(64)
                                     )
begin
  declare  _SipperFileId   int unsigned  default 0;
  declare  _srcClassId     int unsigned  default 0;
  declare  _destClassId    int unsigned  default 0;

  set  _sipperFileId = (select  sf.SipperFileId  from SipperFiles sf  where  sf.SipperFileName = _sipperFileName);
  set  _srcClassId   = (select  c.ClassId  from Classes c  where c.ClassName = _srcClassName);
  set  _destClassId  = (select  c.ClassId  from Classes c  where c.ClassName = _destClassName);
  

  update  Images i
      set  i.Class1Id = _destClassid
      where  (i.Class1Id = _srcClassId)  and  (i.SipperFileid = _sipperFileId);

  set @class1Updated = Row_Count();


  update  Images i
      set  i.Class2ID = _destClassId
      where  (i.Class2Id = _srcClassId)  and  (i.SipperFileId = _sipperFileId);

  set @class2Updated = Row_Count();


  update  Images i
      set  i.ClassValidatedId = _destClassId
      where  (i.ClassValidatedId = _srcClassId)  and  (i.SipperFileId = _sipperFileId);

  set @classNameValidatedUpdated = Row_Count();


  select  _sipperFileName as SipperFileName, @class1Updated  as Class1Updated,   @class2Updated as Class2Updated,  @classNameValidatedUpdated as ClassNameValidatedUpdated;

end;
//
delimiter ;









/**********************************************************************************************************************/
drop procedure  if exists ImagesLocateClosestImage ;

delimiter //
create procedure  ImagesLocateClosestImage (in  _sipperFileName   varChar(48),
                                            in  _scanLine         int unsigned,
                                            in  _scanCol          int 
                                           )
begin
  declare  done                   int default 0;
  declare  _imageFileName         varchar(64);
  declare  _sipperFileId          int unsigned default 0;
  declare  _imageId               int unsigned default 0;
  declare  _topLeftRow            int unsigned default 0;
  declare  _topLeftCol            int unsigned default 0;
  declare  _closestImageId        int unsigned default 0;
  declare  _closestImageFileName  varchar(64);
  declare  _closestDistance       float default 0;
  declare  _imagesChecked         int unsigned  default 0;
  declare  _distance              float default 0;


  declare cur1 cursor for  select i.ImageId, i.ImageFileName, i.TopLeftRow, i.TopLeftCol
                                  from Images i
                                  join (SipperFiles sf)  on(sf.SipperFileId = i.SipperFileId)
                                  where  sf.SipperFileName = _sipperFileName    and
                                         i.TopLeftRow >= (_scanLine - 8)        and
                                         i.TopLeftRow <  (_scanLine + 8)        and
                                         i.TopLeftCol >= (_scanCol  - 5)        and
                                         i.TopLeftCol <= (_scanCol  + 5);

  declare continue HANDLER for not found set done = 1;


  set _closestDistance = 999999.99;
  set _imagesChecked   = 0;


  open cur1;

  repeat
    fetch  cur1 into _imageId, _imageFileName, _topLeftRow, _topLeftCol;
    if  not done then
      set _distance = sqrt((_topLeftRow - _scanLine) * (_topLeftRow - _scanLine) + (_topLeftCol - _scanCol) * (_topLeftCol - _scanCol));
      if  (_distance < _closestDistance)  then
        set _closestImageId = _imageId;
        set _closestDistance = _distance;
        set _closestImageFileName = _imageFileName;
      end if;
      set  _imagesChecked = _imagesChecked + 1;
    end if;
  until done end repeat;

  close cur1;

  if  (_imagesChecked > 0)  then
    select  _closestImageId, _closestImageFileName;
  end if;
  
end;
//
delimiter ;



/**********************************************************************************************************************/
drop procedure  if exists ImagesDepthSizeSummary;

delimiter //
create procedure  ImagesDepthSizeSummary (in  _className       varChar(64),
                                          in  _cruiseName      varChar(64),
                                          in  _stationName     varChar(10),
                                          in  _deploymentNum   varChar(4),
                                          in  _depthBinSize    int unsigned,
                                          in  _sizeBinSize     int unsigned
                                         )
begin

select   (i.depth      div _depthBinSize) * _depthBinSize  as DepthBucket,
         (i.PixelCount div _sizeBinSize)  * _sizeBinSize   as PixelBucket,
         count((i.depth div _depthBinSize) * 1000 + (i.PixelCount div _sizeBinSize))
    from images i
    join  (classes c)       on(c.ClassId       = i.Class1Id)
    join  (SipperFiles sf)  on(sf.SipperFileId = i.SipperFileId)
    where (c.ClassName      = _className)      and
          ((_cruiseName    = "")  or  (sf.CruiseName    = _cruiseName))     and
          ((_stationName   = "")  or  (sf.StationName   = _stationName))    and
          ((_deploymentNum = "")  or  (sf.DeploymentNum = _deploymentNum))
    group by (i.depth div _depthBinSize), (i.PixelCount div _sizeBinSize)
    order by (i.depth div _depthBinSize), (i.PixelCount div _sizeBinSize);
end;
//
delimiter ;






/**********************************************************************************************************************/
drop procedure  if exists ImagesStatsByUpAndDownCast;

delimiter //
create procedure  ImagesStatsByUpAndDownCast (in  _cruiseName      varChar(64),
                                              in  _stationName     varChar(10),
                                              in  _deploymentNum   varChar(4),
                                              in  _className       varChar(64),
                                              in  _depthBinSize    int unsigned
                                             )
begin
  declare _classId      int       default 0;
  declare _midPoint     datetime;
  
  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else
    set _classId = -1;  
  end if;                                       

  set  _midPoint = InstrumentDataGetMidPoint(_cruiseName, _stationName, _deploymentNum);
  
  
  
  
  select (id.CTDDateTime >= _midPoint)                     as upCast,
          floor(i.depth / _depthBinSize)                   as bucketIdx,
         (floor(i.depth / _depthBinSize) * _depthBinSize)  as bucketDepth,
         Count(i.ImageId)                                  as ImageCount,
         sum(i.PixelCount)                                 as totalPixelCount
      from Images i
      join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
      join (InstrumentData id)  on  (id.SipperFileId = i.SipperFileId  and  id.ScanLine = (floor(i.TopLeftRow / 4096) * 4096))
      where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum  
             and  ((_classId < 0)  or  (i.Class1Id = _classId))
       group by (id.CTDDateTime >= _midPoint), floor(i.depth / _depthBinSize);

end;
//
delimiter ;



/*************************************************************************/

drop procedure  if exists ImagesDepthSizeDistribution;

delimiter //
create procedure  ImagesDepthSizeDistribution (in  _cruiseName      varChar(64),
                                               in  _stationName     varChar(10),
                                               in  _deploymentNum   varChar(4),
                                               in  _className       varChar(64),
                                               in  _depthBinSize    int unsigned
                                              )
begin
  declare _classId      int       default 0;

  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else
    set _classId = -1;  
  end if;                                       


  select  (_depthBinSize * floor(i.Depth / _depthBinSize))             as  Depth,
        count(*),
        sum(i.PixelCount <=   500.0)                                   as  "<500",
        sum((i.PixelCount >   500.0)  and  (i.PixelCount <=  1000.0))  as  " 501 - 1000",
        sum((i.PixelCount >  1000.0)  and  (i.PixelCount <=  5000.0))  as  "1001 - 5000",
        sum((i.PixelCount >  5000.0)  and  (i.PixelCount <= 10000.0))  as  "5001 - 10,000",
        sum((i.PixelCount >  10000.0))                                 as  ">10,000",
        "",
        sum(i.PixelCount <=   300.0)                                   as   "300",
        sum((i.PixelCount >   300.0)  and  (i.PixelCount <=   500.0))  as   "500",
        sum((i.PixelCount >   500.0)  and  (i.PixelCount <=  1000.0))  as  "1000",
        sum((i.PixelCount >  1000.0)  and  (i.PixelCount <=  1500.0))  as  "1500",
        sum((i.PixelCount >  1500.0)  and  (i.PixelCount <=  2000.0))  as  "2000",
        sum((i.PixelCount >  2000.0)  and  (i.PixelCount <=  2500.0))  as  "2500",
        sum((i.PixelCount >  2500.0)  and  (i.PixelCount <=  3000.0))  as  "3000",
        sum((i.PixelCount >  3000.0)  and  (i.PixelCount <=  3500.0))  as  "3500",
        sum((i.PixelCount >  3500.0)  and  (i.PixelCount <=  4000.0))  as  "4000",
        sum((i.PixelCount >  4000.0)  and  (i.PixelCount <=  4500.0))  as  "4500",
        sum((i.PixelCount >  4500.0)  and  (i.PixelCount <=  5000.0))  as  "5000",
        sum((i.PixelCount >  5000.0)  and  (i.PixelCount <=  5500.0))  as  "5500",
        sum((i.PixelCount >  5500.0)  and  (i.PixelCount <=  6000.0))  as  "6000",
        sum((i.PixelCount >  6000.0)  and  (i.PixelCount <=  6500.0))  as  "6500",
        sum((i.PixelCount >  6500.0)  and  (i.PixelCount <=  7000.0))  as  "7000",
        sum((i.PixelCount >  7000.0)  and  (i.PixelCount <=  7500.0))  as  "7500",
        sum((i.PixelCount >  7500.0)  and  (i.PixelCount <=  8000.0))  as  "8000",
        sum((i.PixelCount >  8000.0)  and  (i.PixelCount <=  8500.0))  as  "8500",
        sum((i.PixelCount >  8500.0)  and  (i.PixelCount <=  9000.0))  as  "9000",
        sum((i.PixelCount >  9000.0)  and  (i.PixelCount <=  9500.0))  as  "9500",
        sum((i.PixelCount >  9500.0)  and  (i.PixelCount <= 10000.0))  as "10000",
        sum((i.PixelCount > 10000.0)  and  (i.PixelCount <= 15000.0))  as "15000",
        sum((i.PixelCount > 15000.0)  and  (i.PixelCount <= 20000.0))  as "20000",
        sum((i.PixelCount > 20000.0)  and  (i.PixelCount <= 25000.0))  as "25000",
        sum((i.PixelCount > 25000.0)  and  (i.PixelCount <= 30000.0))  as "30000",
        sum((i.PixelCount > 30000.0)  and  (i.PixelCount <= 35000.0))  as "35000",
        sum((i.PixelCount > 35000.0)  and  (i.PixelCount <= 40000.0))  as "40000",
        sum((i.PixelCount > 40000.0)  and  (i.PixelCount <= 45000.0))  as "45000",
        sum((i.PixelCount > 45000.0)  and  (i.PixelCount <= 50000.0))  as "50000",
        sum((i.PixelCount > 50000.0))                                  as ">50,000"

    from Images i
    join(SipperFiles sf)  on(sf.SipperFileId = i.SipperFileId)
    where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum
        and  ((_classId < 0)  or   (Class1Id = _classId))
    group  by floor(i.Depth / _depthBinSize);

end;
//
delimiter ;








drop procedure  if exists  ImagesSizeDataByDepthSipper2;

/*  Andrew made this script to deel with Oil Droplets. */

DELIMITER $$

CREATE PROCEDURE `ImagesSizeDataByDepthSipper2`(in  _cruiseName       varChar(10),
                                                in  _stationName      varChar(10), 
                                                in  _deploymentNum    varchar(4), 
                                                in  _sipperFileName   varchar(64), 
                                                in  _className        varChar(64),
                                                in  _depthBinSize     int unsigned
                                               )
begin 
  declare _classId      int       default 0; 
  declare _maxDepth     float     default 0.0;
  declare _midPoint     datetime; 

  declare _deploymentDesc  varchar(60);

  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else 
    set  _classId = -1;
  end if; 

  select sf.CruiseName,
         sf.StationName, 
         sf.DeploymentNum, 
         _sipperFileName                                    as SpecifiedSipperFile, 
         floor(i.depth / _depthBinSize)                     as bucketIdx,
         (floor(i.depth / _depthBinSize) * _depthBinSize)   as bucketDepth,
         Count(i.ImageId)                                   as ImageCount, 
         sum(i.PixelCount)                                  as totalPixelCount,
         sum(i.PixelCount <=   154.0)                                  as  Less154,
         sum((i.PixelCount >   154.0)  and  (i.PixelCount <=  273.0))  as  Less273,
         sum((i.PixelCount >   273.0)  and  (i.PixelCount <=  426.0))  as  Less426,
         sum((i.PixelCount >   426.0)  and  (i.PixelCount <=  614.0))  as  Less614, 
         sum((i.PixelCount >   614.0)  and  (i.PixelCount <=  836.0))  as  Less836,
         sum((i.PixelCount >   836.0)  and  (i.PixelCount <= 1092.0))  as  Less1092,
         sum((i.PixelCount >  1092.0)  and  (i.PixelCount <= 1383.0))  as  Less1383, 
         sum((i.PixelCount >  1383.0)  and  (i.PixelCount <= 1707.0))  as  Less1707,
         sum((i.PixelCount >  1707.0)  and  (i.PixelCount <= 2066.0))  as  Less2066,
         sum((i.PixelCount >  2066.0)  and  (i.PixelCount <= 2549.0))  as  Less2549,
         sum((i.PixelCount >  2459.0)  and  (i.PixelCount <= 2886.0))  as  Less2886,
         sum((i.PixelCount >  2886.0)  and  (i.PixelCount <= 3347.0))  as  Less3347,
         sum((i.PixelCount >=  3347))                                   as  Greater2718
              from Images i
      join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
      join (classes c)          on  (c.ClassId       = i.Class1Id)
      where  (sf.CruiseName      = _cruiseName)   and
             (sf.StationName     = _stationName)  and
             ((_deploymentNum = "")  ||  (sf.DeploymentNum  = _deploymentNum))   and 
             ((_sipperFileName = "") ||  (sf.SipperFileName = _sipperFileName))  and
             (InStr (c.ClassName, _className) > 0)
      group by sf.CruiseName, sf.StationName, floor(i.depth / _depthBinSize);
end$$
delimiter ;




/**********************************************************************************************************************/



drop procedure  if exists  ImagesSizeDataByDepthSipper3;


DELIMITER $$

CREATE PROCEDURE `ImagesSizeDataByDepthSipper3`(in  _cruiseName       varChar(10),
                                                in  _stationName      varChar(10),
                                                in  _deploymentNum    varchar(4),
                                                in  _sipperFileName   varchar(64),
                                                in  _className        varChar(64),
                                                in  _depthBinSize     int unsigned
                                               )
begin
  declare _classId      int       default 0;
  declare _maxDepth     float     default 0.0;
  declare _midPoint     datetime;

  declare _deploymentDesc  varchar(60);

  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else
    set  _classId = -1;
  end if;


  select sf.CruiseName,
         sf.StationName,
         sf.DeploymentNum,
         _sipperFileName                                    as SpecifiedSipperFile,
         floor(i.Depth / _depthBinSize)                     as bucketIdx,
         (floor(i.Depth / _depthBinSize) * _depthBinSize)   as bucketDepth,
         Count(i.ImageId)                                   as ImageCount,
         sum(i.PixelCount)                                  as totalPixelCount,
         sum(i.PixelCount <=   302.0)                                  as  Less302,
         sum((i.PixelCount >   302.0)  and  (i.PixelCount <=  412.0))  as  Less412,
         sum((i.PixelCount >   412.0)  and  (i.PixelCount <=  537.0))  as  Less537,
         sum((i.PixelCount >   537.0)  and  (i.PixelCount <=  680.0))  as  Less680,
         sum((i.PixelCount >   680.0)  and  (i.PixelCount <=  839.0))  as  Less839,
         sum((i.PixelCount >   839.0)  and  (i.PixelCount <= 1015.0))  as  Less1015,
         sum((i.PixelCount >  1015.0)  and  (i.PixelCount <= 1208.0))  as  Less1208,
         
         sum((i.PixelCount >  1208.0)  and  (i.PixelCount <= 1418.0))  as  Less1418,         
         sum((i.PixelCount >  1418.0)  and  (i.PixelCount <= 1645.0))  as  Less1645,         
         sum((i.PixelCount >  1645.0)  and  (i.PixelCount <= 1888.0))  as  Less1888,         
         sum((i.PixelCount >  1888.0)  and  (i.PixelCount <= 2148.0))  as  Less2148,         
         sum((i.PixelCount >  2148.0)  and  (i.PixelCount <= 2425.0))  as  Less2425,         
         sum((i.PixelCount >  2425.0)  and  (i.PixelCount <= 2718.0))  as  Less2718,         
         sum((i.PixelCount >  2718.0)  and  (i.PixelCount <= 3029.0))  as  Less3029,         

         sum((i.PixelCount >  3029.0)  and  (i.PixelCount <= 3356.0))  as  Less3356,         
         sum((i.PixelCount >  3356.0)  and  (i.PixelCount <= 3700.0))  as  Less3700,         
         sum((i.PixelCount >  3700.0)  and  (i.PixelCount <= 4061.0))  as  Less4061,         
         sum((i.PixelCount >  4061.0)  and  (i.PixelCount <= 4438.0))  as  Less4438,         
         sum((i.PixelCount >  4438.0)  and  (i.PixelCount <= 4832.0))  as  Less4832,         
         sum((i.PixelCount >  4832.0)  and  (i.PixelCount <= 5243.0))  as  Less5243,         
         sum((i.PixelCount >  5243))                                   as  Greater5243
      from Images i
      join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
      join (classes c)          on  (c.ClassId       = i.Class1Id)
      where  (sf.CruiseName      = _cruiseName)   and  
             (sf.StationName     = _stationName)  and  
             ((_deploymentNum = "")  ||  (sf.DeploymentNum  = _deploymentNum))   and
             ((_sipperFileName = "") ||  (sf.SipperFileName = _sipperFileName))  and
             (InStr (c.ClassName, _className) > 0)
      group by sf.CruiseName, sf.StationName, floor(i.depth / _depthBinSize);
end$$

delimiter ;









/**********************************************************************************************************************/

drop procedure  if exists  ImagesSizeDataByDepthSipper6;

DELIMITER $$

CREATE PROCEDURE `ImagesSizeDataByDepthSipper6`(in  _cruiseName       varChar(10),
                                                in  _stationName      varChar(10), 
                                                in  _deploymentNum    varchar(4), 
                                                in  _depthBinSize     int unsigned
                                               )
begin 
  declare _classId      int       default 0; 

  drop  temporary table if exists  TempSizeDistributionTable;

  create temporary table TempSizeDistributionTable
  (
    ImageId           int unsigned,
    PixelCount        int    default 0,
    FullSize          int    default 0,
    Depth             float  default 0.0
  );

  insert into TempSizeDistributionTable
      select i.ImageId,
             i.PixelCount,
             (select fd.FilledArea from FeatureData fd where fd.ImageId = i.ImageId),
             i.Depth

          from Images i
          join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
          where  (sf.CruiseName      = _cruiseName)    and  
                 (sf.StationName     = _stationName)   and
                 (sf.DeploymentNum   = _deploymentNum) and
                 (i.Class1Id not in (select  c.ClassId  from Classes c  where c.ClassName like "Noise%"));

  select _cruiseName                                               as CruiseName,
         _stationName                                              as StationName,
         _deploymentNum                                            as DeploymentNum,
         floor(t.Depth / _depthBinSize)                            as bucketIdx,
         (floor(t.Depth / _depthBinSize) * _depthBinSize)          as bucketDepth,
         count(t.ImageId)                                          as ImageCount,
         sum(t.FullSize <=   250.0)                               as Less_250_0_497,
         sum((t.FullSize >   250)    and  (t.FullSize <= 253))    as Less_253_0_497,
         sum((t.FullSize >   253)    and  (t.FullSize <= 306))    as Less_306_0_5,
         sum((t.FullSize >   306)    and  (t.FullSize <= 364))    as Less_364_0_55,
         sum((t.FullSize >   364)    and  (t.FullSize <= 427))    as Less_427_0_6,
         sum((t.FullSize >   427)    and  (t.FullSize <= 496))    as Less_496_0_65,
         sum((t.FullSize >   496)    and  (t.FullSize <= 569))    as Less_569_0_7,
         sum((t.FullSize >   569)    and  (t.FullSize <= 647))    as Less_647_0_75,
         sum((t.FullSize >   647)    and  (t.FullSize <= 731))    as Less_731_0_8,
         sum((t.FullSize >   731)    and  (t.FullSize <= 819))    as Less_819_0_85,
         sum((t.FullSize >   819)    and  (t.FullSize <= 913))    as Less_913_0_9,
         sum((t.FullSize >   913)    and  (t.FullSize <= 1011))   as Less_1011_0_95,
         sum((t.FullSize >   1011)   and  (t.FullSize <= 1115))   as Less_1115_1,
         sum((t.FullSize >   1115)   and  (t.FullSize <= 1224))   as Less_1224_1_05,
         sum((t.FullSize >   1224)   and  (t.FullSize <= 1337))   as Less_1337_1_1,
         sum((t.FullSize >   1337)   and  (t.FullSize <= 1456))   as Less_1456_1_15,
         sum((t.FullSize >   1456)   and  (t.FullSize <= 1580))   as Less_1580_1_2,
         sum((t.FullSize >   1580)   and  (t.FullSize <= 1709))   as Less_1709_1_25,
         sum((t.FullSize >   1709)   and  (t.FullSize <= 1843))   as Less_1843_1_3,
         sum((t.FullSize >   1843)   and  (t.FullSize <= 1982))   as Less_1982_1_35,
         sum((t.FullSize >   1982)   and  (t.FullSize <= 2126))   as Less_2126_1_4,
         sum((t.FullSize >   2126)   and  (t.FullSize <= 2276))   as Less_2276_1_45,
         sum((t.FullSize >   2276)   and  (t.FullSize <= 2430))   as Less_2430_1_5,
         sum((t.FullSize >   2430)   and  (t.FullSize <= 2589))   as Less_2589_1_55,
         sum((t.FullSize >   2589)   and  (t.FullSize <= 2753))   as Less_2753_1_6,
         sum((t.FullSize >   2753)   and  (t.FullSize <= 2923))   as Less_2923_1_65,
         sum((t.FullSize >   2923)   and  (t.FullSize <= 3097))   as Less_3097_1_7,
         sum((t.FullSize >   3097)   and  (t.FullSize <= 3277))   as Less_3277_1_75,
         sum((t.FullSize >   3277)   and  (t.FullSize <= 3461))   as Less_3461_1_8,
         sum((t.FullSize >   3461)   and  (t.FullSize <= 3651))   as Less_3651_1_85,
         sum((t.FullSize >   3651)   and  (t.FullSize <= 3846))   as Less_3846_1_9,
         sum((t.FullSize >   3846)   and  (t.FullSize <= 4045))   as Less_4045_1_95,
         sum((t.FullSize >   4045))                               as Greater_4045_2
         
        
     from  TempSizeDistributionTable t
     group by floor(t.Depth / _depthBinSize);        
        
end$$
delimiter ;




/**********************************************************************************************************************/

drop procedure  if exists  ImagesSizeDataByDepthSipper8;

DELIMITER $$

CREATE PROCEDURE `ImagesSizeDataByDepthSipper8`(in  _cruiseName       varChar(10),
                                                in  _stationName      varChar(10), 
                                                in  _deploymentNum    varchar(4), 
                                                in  _depthBinSize     int unsigned
                                               )
begin 
  
  declare _midPoint     datetime;
  set _midPoint = InstrumentDataGetMidPoint(_cruiseName, _stationName, _deploymentNum);

  select sf.CruiseName                                                as CruiseName,
         sf.StationName                                               as StationName,
         sf.deploymentNum                                             as DeploymentNum,
		     (id.CTDDateTime <= _midPoint)                                as DownCast,
         floor(i.Depth / _depthBinSize)                               as bucketIdx,
         (floor(i.Depth / _depthBinSize) * _depthBinSize)             as bucketDepth,
         count(i.ImageId)                                             as ImageCount,
         sum(i.PixelCount)                                            as TotalPixelCount,
         sum(i.PixelCount <   250.0)                                  as Less_250,
         sum((i.PixelCount >=  250)   and  (i.PixelCount <= 259))     as Size__250,
         sum((i.PixelCount >=  300)   and  (i.PixelCount <= 399))     as Size__300,
         sum((i.PixelCount >=  400)   and  (i.PixelCount <= 499))     as Size__400,
         sum((i.PixelCount >=  500)   and  (i.PixelCount <= 599))     as Size__500,
         sum((i.PixelCount >=  600)   and  (i.PixelCount <= 699))     as Size__600,
         sum((i.PixelCount >=  700)   and  (i.PixelCount <= 799))     as Size__700,
         sum((i.PixelCount >=  800)   and  (i.PixelCount <= 899))     as Size__800,
         sum((i.PixelCount >=  900)   and  (i.PixelCount <= 999))     as Size__900,
         sum((i.PixelCount >= 1000)   and  (i.PixelCount <= 1099))    as Size_1000,
         sum((i.PixelCount >= 1100)   and  (i.PixelCount <= 1199))    as Size_1100,
         sum((i.PixelCount >= 1200)   and  (i.PixelCount <= 1299))    as Size_1200,
         sum((i.PixelCount >= 1300)   and  (i.PixelCount <= 1399))    as Size_1300,
         sum((i.PixelCount >= 1400)   and  (i.PixelCount <= 1499))    as Size_1400,
         sum((i.PixelCount >= 1500)   and  (i.PixelCount <= 1599))    as Size_1500,
         sum((i.PixelCount >= 1600)   and  (i.PixelCount <= 1699))    as Size_1600,
         sum((i.PixelCount >= 1700)   and  (i.PixelCount <= 1799))    as Size_1700,
         sum((i.PixelCount >= 1800)   and  (i.PixelCount <= 1899))    as Size_1800,
         sum((i.PixelCount >= 1900)   and  (i.PixelCount <= 1999))    as Size_1900,
         sum((i.PixelCount >= 2000)   and  (i.PixelCount <= 2099))    as Size_2000,
         sum((i.PixelCount >= 2100)   and  (i.PixelCount <= 2199))    as Size_2100,
         sum((i.PixelCount >= 2200)   and  (i.PixelCount <= 2299))    as Size_2200,
         sum((i.PixelCount >= 2300)   and  (i.PixelCount <= 2399))    as Size_2300,
         sum((i.PixelCount >= 2400)   and  (i.PixelCount <= 2499))    as Size_2400,
         sum((i.PixelCount >= 2500)   and  (i.PixelCount <= 2599))    as Size_2500,
         sum((i.PixelCount >= 2600)   and  (i.PixelCount <= 2699))    as Size_2600,
         sum((i.PixelCount >= 2700)   and  (i.PixelCount <= 2799))    as Size_2700,
         sum((i.PixelCount >= 2800)   and  (i.PixelCount <= 2899))    as Size_2800,
         sum((i.PixelCount >= 2900)   and  (i.PixelCount <= 2999))    as Size_2900,
         sum((i.PixelCount >= 3000)   and  (i.PixelCount <= 3099))    as Size_3000,
         sum((i.PixelCount >= 3100)   and  (i.PixelCount <= 3199))    as Size_3100,
         sum((i.PixelCount >= 3200)   and  (i.PixelCount <= 3299))    as Size_3200,
         sum((i.PixelCount >= 3300)   and  (i.PixelCount <= 3399))    as Size_3300,
         sum((i.PixelCount >= 3400)   and  (i.PixelCount <= 3499))    as Size_3400,
         sum((i.PixelCount >= 3500)   and  (i.PixelCount <= 3599))    as Size_3400,
         sum((i.PixelCount >= 3600)   and  (i.PixelCount <= 3699))    as Size_3400,
         sum((i.PixelCount >= 3700)   and  (i.PixelCount <= 3799))    as Size_3400,
         sum((i.PixelCount >= 3800)   and  (i.PixelCount <= 3899))    as Size_3400,
         sum((i.PixelCount >= 3900)   and  (i.PixelCount <= 3999))    as Size_3400,
         sum((i.PixelCount >= 4000)   and  (i.PixelCount <= 4099))    as Size_4000,
         sum((i.PixelCount >= 4100)   and  (i.PixelCount <= 4199))    as Size_4100,
         sum((i.PixelCount >= 4200)   and  (i.PixelCount <= 4299))    as Size_4200,
         sum((i.PixelCount >= 4300)   and  (i.PixelCount <= 4399))    as Size_4300,
         sum((i.PixelCount >= 4400)   and  (i.PixelCount <= 4499))    as Size_4400,
         sum((i.PixelCount >= 4500)   and  (i.PixelCount <= 4599))    as Size_4500,
         sum((i.PixelCount >= 4600)   and  (i.PixelCount <= 4699))    as Size_4600,
         sum((i.PixelCount >= 4700)   and  (i.PixelCount <= 4799))    as Size_4700,
         sum((i.PixelCount >= 4800)   and  (i.PixelCount <= 4899))    as Size_4800,
         sum((i.PixelCount >= 4900)   and  (i.PixelCount <= 4999))    as Size_4900,
         sum((i.PixelCount >= 5000)   and  (i.PixelCount <= 5999))    as Size_5000,
         sum((i.PixelCount >= 6000)   and  (i.PixelCount <= 6999))    as Size_6000,
         sum((i.PixelCount >= 7000)   and  (i.PixelCount <= 7999))    as Size_7000,
         sum((i.PixelCount >= 8000)   and  (i.PixelCount <= 8999))    as Size_8000,
         sum((i.PixelCount >= 9000)   and  (i.PixelCount <= 9999))    as Size_9000,
         sum((i.PixelCount >= 10000))                                 as Size_10000
         
         from  Images i 
         join (SipperFiles sf)     on(sf.SipperFileId = i.SipperFileId)
         join (InstrumentData id)  on((id.SipperFileId = i.SipperFileId)  and  (id.ScanLine = Floor(TopLeftRow/4096) * 4096) )
         where  (sf.CruiseName      = _cruiseName)     and  
                (sf.StationName     = _stationName)    and
                (sf.DeploymentNum   = _deploymentNum)  and
                (id.CTDDateTime     <= _midPoint)      and
                (i.Class1Id in (select  c.ClassId  from Classes c  where c.ClassName like "%snow%"))
         group by DownCast, floor(i.Depth / _depthBinSize);        
        
end$$
delimiter ;









drop procedure  if   exists  ImagesAllDeployments;

delimiter //
create procedure  ImagesAllDeployments ()
begin
  declare done             int default 0;
  declare _cruiseName      varchar(48);
  declare _stationName     varchar(48);
  declare _deploymentNum   varchar(48);

  declare cur1 cursor for  select CruiseName, StationName, DeploymentNum  
                                  from deployments 
                                  order by CruiseName, StationName, DeploymentNum;

  declare continue HANDLER for not found set done = 1;

  open cur1;

  repeat
    fetch  cur1 into _cruiseName,_stationName,_deploymentNum;
    if not done then
       call  ImagesSizeDataByDepthSipper8(_cruiseName, _stationName, _deploymentNum,1);
       commit;
    end if;
  until done end repeat;

  close cur1; 

end;
//
delimiter ;

























/**********************************************************************************************************************/

drop procedure  if exists  ImagesValidatedDepthSipper;

DELIMITER $$

CREATE PROCEDURE ImagesValidatedDepthSipper(in  _cruiseName       varChar(10),
                                            in  _className        varChar(64),
                                            in  _depthBinSize     int unsigned
                                           )
begin
  declare _classId      int       default 0;
  declare _maxDepth     float     default 0.0;
  declare _midPoint     datetime;

  declare _deploymentDesc  varchar(60);

  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else
    set  _classId = -1;
  end if;


  select sf.CruiseName,
         sf.StationName,
         sf.DeploymentNum,
         _className,
         floor(i.Depth / _depthBinSize)                     as bucketIdx,
         (floor(i.Depth / _depthBinSize) * _depthBinSize)   as bucketDepth,
         Count(i.ImageId)                                   as ImageCount,
         sum(i.PixelCount)                                  as totalPixelCount,
         sum(i.PixelCount <     300.0)                                  as  Less300,
         sum((i.PixelCount >=   300.0)  and  (i.PixelCount <   500.0))  as  Less500,
         sum((i.PixelCount >=   500.0)  and  (i.PixelCount <  1000.0))  as  Less1000,
         sum((i.PixelCount >=  1000.0)  and  (i.PixelCount <  2000.0))  as  Less2000,
         sum((i.PixelCount >=  2000.0)  and  (i.PixelCount <  5000.0))  as  Less5000,
         sum((i.PixelCount >=  5000.0)  and  (i.PixelCount < 10000.0))  as  Less10000,
         sum((i.PixelCount >= 10000.0)  and  (i.PixelCount < 20000.0))  as  Less20000,
         sum((i.PixelCount >= 20000.0))                                 as  Greater20000
      from Images i
      join (SipperFiles sf)     on  (sf.SipperFileId  = i.SipperFileId)
      join (classes c)          on  (c.ClassId        = i.ClassValidatedId)
      where  (sf.CruiseName      = _cruiseName)   and  
             (InStr (c.ClassName, _className) > 0)
      group by sf.CruiseName, sf.StationName, sf.DeploymentNum, floor(i.depth / _depthBinSize);
end$$

delimiter ;













drop procedure  if exists  ImagesSizeDataByDepthSipper4;

-- --------------------------------------------------------------------------------
-- Routine DDL
-- --------------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `ImagesSizeDataByDepthSipper4`(in  _cruiseName       varChar(10),
                                                in  _stationName      varChar(10)
                                               )
begin

  select sf.CruiseName,
         sf.StationName,
         Count(i.ImageId)                                   as ImageCount,
         sum(i.PixelCount <    100.0)                                  as  Less_100,
         sum((i.PixelCount >=   100.0)  and  (i.PixelCount <  150.0))  as  R_100_149,
         sum((i.PixelCount >=   150.0)  and  (i.PixelCount <  200.0))  as  R_150_199,
         sum((i.PixelCount >=   200.0)  and  (i.PixelCount <  250.0))  as  R_200_249,
         sum((i.PixelCount >=   250.0)  and  (i.PixelCount <  300.0))  as  R_250_299,
         sum((i.PixelCount >=   300.0)  and  (i.PixelCount <  350.0))  as  R_300_349,
         sum((i.PixelCount >=   350.0)  and  (i.PixelCount <  400.0))  as  R_350_399,
         
         sum((i.PixelCount >=   400.0)  and  (i.PixelCount <  450.0))  as  R_400_449,
         sum((i.PixelCount >=   450.0)  and  (i.PixelCount <  500.0))  as  R_450_499, 
         sum((i.PixelCount >=   500.0)  and  (i.PixelCount <  600.0))  as  R_500_599,
         sum((i.PixelCount >=   600.0)  and  (i.PixelCount <  700.0))  as  R_600_699,
         sum((i.PixelCount >=   700.0)  and  (i.PixelCount <  800.0))  as  R_700_799,
         sum((i.PixelCount >=   800.0)  and  (i.PixelCount <  900.0))  as  R_800_899,
         sum((i.PixelCount >=   900.0)  and  (i.PixelCount < 1000.0))  as  R_900_999,

         sum((i.PixelCount >=  1000.0)  and  (i.PixelCount < 1100.0))  as  R_1000_1099,
         sum((i.PixelCount >=  1100.0)  and  (i.PixelCount < 1200.0))  as  R_1100_1199,
         sum((i.PixelCount >=  1200.0)  and  (i.PixelCount < 1300.0))  as  R_1200_1299,
         sum((i.PixelCount >=  1300.0)  and  (i.PixelCount < 1400.0))  as  R_1300_1399,
         sum((i.PixelCount >=  1400.0)  and  (i.PixelCount < 1500.0))  as  R_1400_1499,
         sum((i.PixelCount >=  1500.0)  and  (i.PixelCount < 1600.0))  as  R_1500_1599,
         sum((i.PixelCount >=  1600.0)  and  (i.PixelCount < 1700.0))  as  R_1600_1599,
         sum((i.PixelCount >=  1700.0)  and  (i.PixelCount < 1800.0))  as  R_1700_1599,
         sum((i.PixelCount >=  1800.0)  and  (i.PixelCount < 1900.0))  as  R_1800_1899,
         sum((i.PixelCount >=  1900.0)  and  (i.PixelCount < 2000.0))  as  R_1900_1999,

         sum((i.PixelCount >=  2000.0)  and  (i.PixelCount < 2100.0))  as  R_2000_2099,
         sum((i.PixelCount >=  2100.0)  and  (i.PixelCount < 2200.0))  as  R_2100_2199,
         sum((i.PixelCount >=  2200.0)  and  (i.PixelCount < 2300.0))  as  R_2200_2299,
         sum((i.PixelCount >=  2300.0)  and  (i.PixelCount < 2400.0))  as  R_2300_2399,
         sum((i.PixelCount >=  2400.0)  and  (i.PixelCount < 2500.0))  as  R_2400_2499,
         sum((i.PixelCount >=  2500.0)  and  (i.PixelCount < 2600.0))  as  R_2500_2599,
         sum((i.PixelCount >=  2600.0)  and  (i.PixelCount < 2700.0))  as  R_2600_2699,
         sum((i.PixelCount >=  2700.0)  and  (i.PixelCount < 2800.0))  as  R_2700_2799,
         sum((i.PixelCount >=  2800.0)  and  (i.PixelCount < 2900.0))  as  R_2800_2899,
         sum((i.PixelCount >=  2900.0)  and  (i.PixelCount < 3000.0))  as  R_2900_2999,

         sum((i.PixelCount >=  3000.0)  and  (i.PixelCount < 3500.0))  as  R_3000_3499,
         sum((i.PixelCount >=  3500.0)  and  (i.PixelCount < 4000.0))  as  R_3500_3999,
         sum((i.PixelCount >=  4000.0)  and  (i.PixelCount < 4500.0))  as  R_4000_4499,
         sum((i.PixelCount >=  4500.0)  and  (i.PixelCount < 5000.0))  as  R_4500_4999,
         sum((i.PixelCount >=  5000.0))                                as  Greater_5000
      from Images i
      join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
      where  (sf.CruiseName      = _cruiseName)   and  
             (sf.StationName     = _stationName) 
      group by sf.CruiseName, sf.StationName, floor(i.depth / _depthBinSize);
end$$
delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop  function  if exists ImagesTimeBucket;

delimiter //
create function ImagesTimeBucket  (_dateTime datetime,
                                   _slotLen  int  unsigned
                                  )

  returns int unsigned
  deterministic

begin
   
  declare  _numSecs  int unsigned  default 0;
  
  set  _numSecs = Hour(_dateTime) * 3600 + Minute(_dateTime) * 60 + Second(_dateTime);

  return  _numSecs div _slotLen;
end;
//
delimiter ;





/**********************************************************************************************************************/

drop procedure  if exists  ImagesClassificationByTimeInterval;

delimiter //
create procedure  ImagesClassificationByTimeInterval (in  _cruiseName       varChar(10),
                                                      in  _stationName      varChar(10),
                                                      in  _deploymentNum    varchar(4),
                                                      in  _timeSlotLen      int unsigned
                                                     )
begin


  drop  temporary table if exists  InstrumentStatsByTimeSlot;

  create temporary table InstrumentStatsByTimeSlot
  (
     DateBucket       date,
     TimeBucket       int unsigned,

     Latitude         double not null,
     Longitude        double not null,
     
     ImageCount       int unsigned not null,
     ImageAvgSize     float  not null,

     DepthMin         float  not null,
     DepthMax         float  not null,
     DepthAvg         float  not null,
     
     TemperatureMin   float  not null,
     TemperatureMax   float  not null,
     TemperatureAvg   float  not null,

     DensityAvg             float  not null,
     FluorescenceSensorAvg  float  not null,
     FluorescenceAvg        float  not null, 
     OxygenSensorAvg        float  not null,
     OxygenAvg              float  not null,
     RecordRateAvg          float  not null,
     SalinityAvg            float  not null,
     TransmisivitySensorAvg float  not null,
     TransmisivityAvg       float  not null,
     TurbiditySensorAvg     float  not null, 
     TurbidityAvg           float  not null, 
     
     FlowRateMin      float  not null,
     FlowRateMax      float  not null,
     FlowRateAvg      float  not null,

     key TheKey (DateBucket asc, TimeBucket asc)
   );


  insert into InstrumentStatsByTimeSlot
      select Date(id.CTDDateTime),
             TIME_TO_SEC(Time(id.CTDDateTime)) div _timeSlotLen,

             avg(id.Latitde),
             avg(id.Longitude),

             count(*),
             (sum(i.PixelCount) / count(*)),
             
             min(id.Depth),
             max(id.Depth),
             avg(id.Depth),

             min(id.Temperature),
             max(id.Temperature),
             avg(id.Temperature),
             avg(Density),
             avg(FluorescenceSensor),
             avg(Fluorescence),
             avg(OxygenSensor),
             avg(Oxygen),
             avg(RecordRate),
             avg(Salinity),
             avg(TransmisivitySensor),
             avg(Transmisivity),
             avg(TurbiditySensor),
             avg(Turbidity),
             min(id.FlowRate1),
             max(id.FlowRate1),
             avg(id.FlowRate1)

          from Images i
          join (SipperFiles sf)     on  (sf.SipperFileId = i.SipperFileId)
          join (InstrumentData id)  on  (id.SipperFileId = i.SipperFileId  and  id.ScanLine = (floor(i.TopLeftRow / 4096) * 4096))
          where  (sf.CruiseName      = _cruiseName)    and  
                 (sf.StationName     = _stationName)   and
                 (sf.DeploymentNum   = _deploymentNum)
          group by sf.CruiseName, sf.StationName, sf.StationName, sf.DeploymentNum, 
                   date(id.CTDDateTime), (TIME_TO_SEC(Time(id.CTDDateTime)) div _timeSlotLen);



  select  t.DateBucket                              as SlotDate,
          t.TimeBucket                              as TimeBucket,
          SEC_TO_TIME(t.TimeBucket * _timeSlotLen)  as SlotTime,
          i.Class1Id,
          c.ClassName                               as ClassName,
          count(i.Class1Id),
          sum(i.PixelCount)  as totalPixelCount,
          (sum(i.PixelCount) / count(i.Class1Id))  as  AvgSize,

          t.Latitude           as  Latitude,
          t.Longitude          as  Longitude,
          
          t.ImageCount         as  NumParticlesForTimeSlot,
          t.ImageAvgSize       as  AverageSizeForTimeSlot,
          
          t.DepthMin           as  MinDepth,
          t.DepthMax           as  MaxDepth,
          t.DepthAvg           as  AvgDepth,

          t.TemperatureMin     as  MinTemperature,
          t.TemperatureMax     as  MaxTemperature,
          t.TemperatureAvg     as  AvgTemperature,
          
          t.FlowRateMin        as  MinFlowRate,
          t.FlowRateMax        as  MaxFlowRate,
          t.FlowRateAvg        as  AvgFlowRate,

          t.DensityAvg             as  DensityAvg,
          t.FluorescenceSensorAvg  as  FluorescenceSensorAvg,
          t.FluorescenceAvg        as  FluorescenceAvg,
          t.OxygenSensorAvg        as  OxygenSensorAvg,
          t.OxygenAvg              as  OxygenAvg,
          t.RecordRateAvg          as  RecordRateAvg,
          t.SalinityAvg            as  SalinityAvg,
          t.TransmisivitySensorAvg as  TransmisivitySensorAvg,
          t.TransmisivityAvg       as  TransmisivityAvg,
          t.TurbiditySensorAvg     as  TurbiditySensorAvg,
          t.TurbidityAvg           as  TurbidityAvg
          
      from  Images i
      join (SipperFiles sf)                on  (sf.SipperFileId = i.SipperFileId)
      join (InstrumentData id)             on  (id.SipperFileId = i.SipperFileId  and  id.ScanLine = (floor(i.TopLeftRow / 4096) * 4096))
      join (InstrumentStatsByTimeSlot  t)  on  ((t.DateBucket = Date(id.CTDDateTime) and  (t.TimeBucket = (TIME_TO_SEC(Time(id.CTDDateTime)) div _timeSlotLen))))
      join (Classes c)                     on  (c.ClassId = i.Class1Id)
      where  sf.CruiseName = _cruiseName  and  sf.StationName = _stationName  and  sf.DeploymentNum = _deploymentNum  
      group by t.DateBucket, t.TimeBucket, i.Class1Id
      order by t.DateBucket, t.TimeBucket, c.ClassName;
end;
//

delimiter ;








/**********************************************************************************/
/*                        `ImagesRetrieveValidatedClasses`                        */
/*   This will retrieve a list of images that have been validated(Labeled) by a   */
/*   user with the class name they labeled it as.                                 */
/**********************************************************************************/
delimiter $$
drop procedure if exists `ImagesRetrieveValidatedClasses` $$
create DEFINER=`root`@`localhost` procedure `ImagesRetrieveValidatedClasses`(in  _cruiseName  varchar(10))
begin
  declare done             int default 0;
  declare _sipperFileName  varchar(48);
  declare _sipperFileId    int default 0;

  declare cur1 cursor for  select sf.SipperFileId, sf.SipperFileName
                                  from SipperFiles sf
                                  where  sf.CruiseName = _cruiseName
                                  order by sf.SipperFileName;

  declare continue HANDLER for not found set done = 1;


  drop  temporary table if exists  ValidatedImages;

  create TEMPORARY table ValidatedImages
   (
     ImageFileName     varchar(64)  not null,
     ValidatedClassId  int           not null
   );


  open cur1;

  repeat
    fetch  cur1 into _sipperFileId, _sipperFileName;
    if not done then
      insert into ValidatedImages
         select i.ImageFileName, i.ClassValidatedId
                 from  Images i
                 where i.SipperFileId = _sipperFileId  and  i.ClassValidatedId > 0;
    end if;
  until done end repeat;
  close cur1;

  select  vi.ImageFileName,
          vi.ValidatedClassId,
          (select c.ClassName  from Classes c  where  c.ClassId = vi.ValidatedClassId)
     from  ValidatedImages vi;

end $$

delimiter ;






/**********************************************************************************/
/*                        `ImagesResetNullClasses`                                */
/*   Will scan entire Images table for instances where "Class1Id" is set to NULL  */
/* and set to Class1Id = 1.                                                       */
/**********************************************************************************/
delimiter $$
drop procedure if exists `ImagesResetNullClasses` $$
create procedure `ImagesResetNullClasses`()
begin
  declare done             int default 0;
  declare _sipperFileName  varchar(48);
  declare _sipperFileId    int default 0;

  declare cur1 cursor for  select sf.SipperFileId, sf.SipperFileName  
                                  from SipperFiles  sf
                                  order by sf.SipperFileName;

  declare continue HANDLER for not found set done = 1;

  open cur1;

  repeat
    fetch  cur1 into _sipperFileId, _sipperFileName;
    if not done then
      update  Images  set Class1Id = 1  where (Class1Id is null)  and  SipperFileId =  _sipperFileId;
    end if;
  until done end repeat;
  close cur1;
end $$

delimiter ;








/**********************************************************************************************************************/
drop procedure  if exists ImagesRetrieveCtdTimestamp;

delimiter //
create procedure  ImagesRetrieveCtdTimestamp (in  _cruiseName      varChar(64),
                                              in  _stationName     varChar(10),
                                              in  _deploymentNum   varChar(4),
                                              in  _className       varChar(64)
                                             )
begin
  declare _classId      int       default -1;
  
  if  (!isnull(_className)  and  (_className != ""))  then
    set  _classId = (select ClassId  from  Classes  where  ClassName = _className);
  else
    set _classId = -1;  
  end if;                                       

  select i.ImageId,
         i.ImageFileName,
         i.SipperFileId,
         i.TopLeftRow,
         i.PixelCount,
         i.Depth,
         i.Class1Id,
         (select c.ClassName from Classes c where c.ClassId = i.Class1Id),
         CTDDateTime
      from images i
      join(SipperFiles sf)    on(sf.SipperFileId = i.SipperFileId)
      join(InstrumentData id) on(id.SipperFileId = i.SipperFileId)
      where
        sf.CruiseName    = _cruiseName        and  
        sf.StationName   = _stationName       and  
        sf.DeploymentNum = _deploymentNum     and 
        id.ScanLine      = (floor(i.TopLeftRow / 4096) * 4096)  and
        ((_classId < 0)  or  (i.Class1Id = _classId));

end;
//
delimiter ;





/**********************************************************************************************************************/
drop procedure  if exists ImagesRetrieveValidatedClass;

delimiter //
create procedure  ImagesRetrieveValidatedClass (in  _sipperFileName  varChar(48))
begin
  declare _sipperFileId      int       default -1;
  
  set  _sipperFileId = (select SipperFileId  from  SipperFiles  where  SipperFileName = _sipperFileName);

  if  (_sipperFileId >= 0)  then
    select i.ImageId,
           i.ImageFileName,
           i.ClassValidatedId,
           (select c.ClassName  from Classes c  where c.ClassId = i.ClassValidatedId) as ClassNameValidated,
           i.SizeCoordinates
        from images i
        where  (i.SipperFileId = _sipperFileId)  and  ((i.ClassValidatedId >= 0)  or  (i.SizeCoordinates <> ""));
  end if;
end;
//
delimiter ;

