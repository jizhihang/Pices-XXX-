delimiter ;


drop procedure  if exists LogEntriesProcessStart;

delimiter //

create procedure  LogEntriesProcessStart (in  _ProgCode          char(4),
                                          in  _ProgName          varchar(64),
                                          in  _DateTimeCompiled  datetime,
                                          in  _CmdLine           varchar(512),
                                          in  _CompName          varchar(32),
                                          in  _CompUserName      varchar(32),
                                          in  _SipperFileName    varchar(48)
                                         )

begin
  set  @_DateTimeStart    = Now();
  set  @_DateTimeStartUtc = Utc_TimeStamp();
  set  @_sipperFileId = -1;
  
  if   (!isnull(_SipperFileName)) and  (_SipperFileName != "") then
    set @_sipperFileId = (select sf.SipperFileId from SipperFiles sf  where sf.SipperFileName = _SipperFileName);
  end if;
  
  if  (isnull(@_sipperFileId))  then
    set @_sipperFileId = -1;
  end if;

  insert into LogEntries(ProgCode, ProgName, DateTimeCompiled, CmdLine, CompName, DataBaseUserName, CompUserName, DateTimeStart, DateTimeStartUtc, CpuTimeUsed, CompletionStatus, SipperFileId)
         values(_ProgCode, 
                _ProgName,
                _DateTimeCompiled,
                _CmdLine, 
                _CompName, 
                User(),
                _CompUserName,
                @_DateTimeStart,
                @_DateTimeStartUtc,
                0.0,
                "Started",
                @_sipperFileId
               );

  select  LAST_INSERT_ID()   as  LogEntryId,
          User()             as  DataBaseUserName,
          @_DateTimeStart    as  DateTimeStart,
          @_DateTimeStartUtc as  DateTimeStartUtc;

end
//
delimiter ;



/**********************************************************************************************************************/
drop procedure   if exists  LogEntriesDelete;

delimiter //

create procedure  LogEntriesDelete(in  _logEntryId    int unsigned)
begin
  delete from LogEntries  where  LogEntryId = _logEntryId;
end
//
delimiter ;



/**********************************************************************************************************************/
drop procedure   if exists  LogEntriesProcessEnd;

delimiter //

create procedure  LogEntriesProcessEnd (in  _logEntryId        int unsigned,
                                        in  _cpuTimeUsed       float,
                                        in  _completionStatus  varchar(128)
                                       )
begin
  set @_DateTimeEnd = Now();


  update  LogEntries le
    set le.DateTimeEnd      = @_DateTimeEnd,
        le.CpuTimeUsed      = _cpuTimeUsed,
        le.CompletionStatus = _completionStatus
     where  le.LogEntryId = _logEntryId;
     
  select  @_DateTimeEnd as DateTimeEnd;
end
//
delimiter ;




/**********************************************************************************************************************/
drop procedure   if exists  LogEntriesSelect;

delimiter //
create procedure  LogEntriesSelect(in  _logEntryId    int unsigned)
begin
  select le.*,
        (select sf.SipperFileName from SipperFiles sf where sf.SipperFileId = le.SipperFileId) as SipperFileName
        from LogEntries le  
        where LogEntryId = _logEntryId;
end
//



