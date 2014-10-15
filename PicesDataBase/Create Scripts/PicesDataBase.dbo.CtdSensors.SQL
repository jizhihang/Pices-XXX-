drop table if exists CtdExternalSensors;


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

        