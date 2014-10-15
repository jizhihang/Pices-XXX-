import mysql.connector
from mysql.connector import errorcode
import datetime
import os



def  ToFloat(s):
  f=0.0
  try:
    f=float(s)
  except  ValueError:
    f=0.0
  return f



def  ToInt(s):
  i=0;
  try:
    i=int(s)
  except  ValueError:
    i=0
  return i



def  DateFromStr(s):
  d = datetime.date(1,1,1)
  try:
    d=datetime.datetime.strptime(s.strip(),"%m/%d/%Y").date()
  except  ValueError:
    d=datetime.date(1900,1,1)
  return d



def  TimeFromStr(s):
  t=datetime.time(0,0,0)
  try:
    t=datetime.datetime.strptime(s.strip(),"%H:%M:%S").time()
  except  ValueError:
    t=datetime.time(0,0,0)
  return t



def  TimeFromHHMMSS(s):
  i=int(ToFloat(s))
  secs = i % 100
  i = int (i / 100)
  mins = i % 100
  i = int (i / 100)
  hours = i
  return datetime.time(hours,mins,secs)




def  ProcessMetDataFile(cruiseName, fileName):
  try:
       db = mysql.connector.Connect(user='root',
                                    password="dasani30",
                                    host='sipper-db.marine.usf.edu',
                                    database='pices_new'
                                  )
  except  mysql.connector.Error as err:
    if  err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
        print("Cound not conect to \"Sipper-d-marine.usf.edu\"")
    elif  err.errno == errorcode.ER_BAD_DB_ERROR:
        print("Database does not exists")
    else:
        print(err)

  c=db.cursor()

  gpsDataFile=open(fileName)

  for line in gpsDataFile:
     fields=line.split('\t')
     if  len(fields)>10:
         utcDate=DateFromStr(fields[0])
         time=TimeFromStr(fields[1])
         utcTime=TimeFromHHMMSS(fields[2])

         latDeg=ToInt(fields[3])
         latMin=ToFloat(fields[4])
         latDir=fields[5]
         latitude=latDeg + (latMin / 60.0)
         if  latDir == "S":
           latitude = 0.0 - latitude

         longDeg = ToInt   (fields[6])
         longMin = ToFloat (fields[7])
         longDir = fields[8]
         longitude = longDeg + (longMin/ 60.0)
         if  longDir == "W":
           longitude = 0.0 - longitude
           
         cog = 0.0
         sog = 0.0

         if  ((latitude!=0.0)  and (longitude!=0.0)):
           dateTimeStr=utcDate.strftime("%Y/%m/%d") + " " + utcTime.strftime("%H:%M:%S")

           sqlStr="call GpsDataInsert(" + "\"" + cruiseName  + "\"" + "," + \
                                          "\"" + dateTimeStr + "\"" + "," + \
                                          repr(latitude)            + "," + \
                                          repr(longitude)           + "," + \
                                          repr(cog)                 + "," + \
                                          repr(sog)                 + \
                                    ")"
           print(sqlStr)
           try:
             c.execute(sqlStr)
             db.commit()
           except  mysql.connector.Error  as err:
             print(err)
   
  gpsDataFile.close()
  db.close()




def  main():
  cruiseName="ETP2007"
  rootDir="E:\\Pices\\DataFiles\\GPSdata\\"

  fileNames=os.listdir(rootDir)
  for fileName in fileNames:
    if  fileName.endswith(".dat"):
      ProcessMetDataFile(cruiseName, rootDir+fileName)


main()

  
