import mysql.connector
from mysql.connector import errorcode
import  datetime


def LatitudeFromStr(s):
  fields=s.split(' ')
  lat=0
  if  len(fields)>=3:
      deg=float(fields[0])
      mins=float(fields[1])
      sign=1.0
      if  fields[2]=='S':
        sign=-1.0
      lat=sign*(deg+mins/60.0)
  return lat


def LongitudeFromStr(s):
  fields=s.split(' ')
  long=0
  if  len(fields)>=3:
      deg=float(fields[0])
      mins=float(fields[1])
      sign=1.0
      if  fields[2]=='W':
        sign=-1.0
      long=sign*(deg+mins/60.0)
  return long



def  DateTimeFromStr(s):
  fields=s.split(' ')
  if  len(fields) != 3:
     return datetime.datetime(1,1,1,0,0,0)
  dateParts=fields[0].split('/')
  if  len(dateParts)!=3:
     return datetime.datetime(1,1,1,0,0,0)
  month=int(dateParts[0])
  day=int(dateParts[1])
  year=int(dateParts[2])
  timeParts=fields[1].split(':')
  if  len(timeParts)!=3:
     return datetime.datetime(year,month,day,0,0,0)
  hours=int(timeParts[0])
  mins=int(timeParts[1])
  secs=int(timeParts[2])

  if  fields[2]=="AM"  and  hours==12:
    hours=0
  elif  fields[2]=="PM"  and  hours<12:
    hours = hours + 12     
  return  datetime.datetime(year,month,day, hours,mins,secs)



def  ToFloat(s):
  f=0.0
  try:
    f=float(s)
  except  ValueError:
    f=0.0
  return f




try:
     db = mysql.connector.Connect(user='root',
                                  password="dasani30",
                                  host='sipper-db.marine.usf.edu',
                                  database='pices_new')
except  mysql.connector.Error as err:
    if  err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
        print("Cound notconect to \"Sipper-d-marine.usf.edu\"")
    elif  err.errno == errorcode.ER_BAD_DB_ERROR:
        print("Database does not exists")
    else:
        print(err)

c=db.cursor()

sizeFile=open('C:\\Temp\\SizeRanges.txt', 'w')

growthFactor = 1.1
startValue   = 250
bucketSize   = 50

v = startValue


while  v < 200000:
  br = v
  nextV = v + bucketSize
  bucketSize = round(bucketSize * growthFactor)
  sizeFile.write("    sum((i.PixelCount >= " + repr(v)     + ")  and  " +
                         "(i.pixelCount < "  + repr(nextV) + ")) " +
                         " as  Size_" + repr(v) + "," +
                         "\n"
                )
  v = nextV

sizeFile.close ()


