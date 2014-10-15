delimiter ;


/**********************************************************************************************************************/
drop procedure  if exists  CruisesDelete;

delimiter //
create procedure CruisesDelete (_cruiseName  char(10))
begin
  delete  from  Cruises
    where  (CruiseName = _cruiseName);
end
//
delimiter ; 

