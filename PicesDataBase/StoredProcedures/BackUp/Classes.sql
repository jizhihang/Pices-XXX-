delimiter ;


drop procedure  if exists ClassesRetrieveAll;

delimiter //

create procedure ClassesRetrieveAll()
begin
  select   c.ClassId,
           c.ClassName,
           c.ParentId,
           (select c1.ClassName  from Classes c1  where c1.ClassId = c.ParentId)  as  ParentName,
           c.Description

    from Classes c
    order by c.ClassName;
end
//

delimiter ;






/**********************************************************************************************************************/
drop procedure    if exists ClassesRetrieveByName;

delimiter //

CREATE PROCEDURE ClassesRetrieveByName(in  _ClassName  varChar(64))
BEGIN
  select   c.ClassId,
           c.ClassName,
           c.ParentId,
           (select c1.ClassName  from Classes c1  where c1.ClassId = c.ParentId)  as  ParentName,
           c.Description
    from Classes c
    where c.ClassName = _ClassName;
END
//

delimiter ;






/**********************************************************************************************************************/
drop procedure    if exists ImageClassInsert;

delimiter //

CREATE PROCEDURE ImageClassInsert(in  _ClassName    varChar(64),
                                  in  _ParentName   varChar(64),
                                  in  _Description  varChar(255)
                                 )
BEGIN
  insert into Classes(ClassName, ParentId, Description)
         values(_ClassName,
                (select  c1.ClassId  from Classes c1  where  c1.ClassName = _ParentName),
                _Description
               );
END
//

delimiter ;






/**********************************************************************************************************************/
drop procedure   if exists  ImageClassDelete;

delimiter //

CREATE PROCEDURE ImageClassDelete(in  _className    varChar(64))
begin
  set @classId  = (select c.ClassId  from Classes c where  c.ClassName = _className);
  set @parentId = (select c.ParentId from Classes c where  c.ClassName = _className);

  delete from Classes  where  ClassId = @classId;

  /* select  @classId, @parentId; */

  update  Classes
       set ParentId = @parentId
       where  ParentId = @classId;
end
//

delimiter ;






/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure   if exists  ImageClassUpdate;

delimiter //

CREATE PROCEDURE  ImageClassUpdate(in  OldClassName    varChar(64),
                                   in  NewClassName    varChar(64),
                                   in  NewParentName   varChar(64),
                                   in  NewDescription  varChar(255)
                                  )
BEGIN
  set  @newParentId = (select c1.classId from Classes c1 where c1.ClassName = NewParentName);


  update  Classes c
    set  c.ClassName    = NewClassName,
         c.ParentId     = @newParentId,
         c.Description  = NewDescription
     where  c.ClassName = OldClassName;
END
//
delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure   if exists  ImageClassInsertReturn;

delimiter //

create procedure ImageClassInsertReturn (in  _name        varChar(64),
                                         in  _parentName  varChar(64)
                                        )
begin
  declare   _classId           int  default 0;
  declare   _parentId          int  default 0;

  set  _classId  = (select c1.ClassId from  Classes c1  where  c1.ClassName = _name);
  set  _parentId = (select c1.ClassId from  Classes c1  where  c1.ClassName = _parentName);

  if  (_classId is null)  then
    call ImageClassInsert (_name, _parentName, "Added by 'ImageClassGetClassId'");
  end if;


  select * from  Classes c3  where  c3.ClassName = _name;
end
//
delimiter ;







/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure   if exists  ImageClassMerge;

delimiter //

CREATE PROCEDURE ImageClassMerge (in  _srcClassName     varChar(64),
                                  in  _destClassName    varChar(64)
                                 )
begin
  set  @oldParentId = (select c1.ClassId from  Classes c1  where  c1.ClassName = _srcClassName);
  set  @newParentId = (select c1.ClassId from  Classes c1  where  c1.ClassName = _destClassName);


  update  Classes c
     set c.ParentId = @newParentId
     where  c.ParentId = @oldParentId;
     
  delete  from  Classes
     where  ClassId = @oldParentId;

end
//
delimiter ;
