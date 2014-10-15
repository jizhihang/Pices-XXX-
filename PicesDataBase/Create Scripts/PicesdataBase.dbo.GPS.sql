drop table if exists GPS;

create table GPS
(
  CruiseName      char(10)      not null,
  GmtDateTime     datetime      not null,
  LocaleDateTime  datetime      not null,
  Latitude        double        not null default 0.0  comment 'Decimal Value, ex: 28.735 = 28 degrees, 44 mins, and 6 seconds',
  Longitude       double        not null default 0.0  comment 'Decimal Value, ex: 28.735 = 28 degrees, 44 mins, and 6 seconds',
  Heading         double        not null default 0.0  comment 'Compass Angle  (0 - 360), can be negative ex: (-10 = 350)',
  Speed           double        not null default 0.0  comment 'In KNOTS',
  
  Primary key     (CruiseName, GmtDateTime)
)
GO

/*
GRANT SELECT ON Table_Name TO PUBLIC

GO
*/

