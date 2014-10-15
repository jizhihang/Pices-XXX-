import mysql.connector
from mysql.connector import errorcode
import datetime
import os
import glog




def  DateFromStr(s):
  d = datetime.date(1,1,1)
  try:
    d=datetime.datetime.strptime(s.strip(),"%Y/%m/%d").date()
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



def  ToFloat(s):
  f=0.0
  try:
    f=float(s)
  except  ValueError:
    f=0.0
  return f



def  ProcessMetDataFile(cruiseName, fileName):
  try:
       db = mysql.connector.Connect(user='root',
                                    password="dasani30",
                                    host='sipper-db.marine.usf.edu',
                                    database='pices_new'
                                  )
  except  mysql.connector.Error as err:
    if  err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
        print("Cound notconect to \"Sipper-d-marine.usf.edu\"")
    elif  err.errno == errorcode.ER_BAD_DB_ERROR:
        print("Database does not exists")
    else:
        print(err)

  c=db.cursor()

  gpsDataFile=open(fileName)

  for line in gpsDataFile:
     fields=line.split(',')
     if  len(fields)>10:
         umtDate=DateFromStr(fields[0])
         umtTime=TimeFromStr(fields[1])
         latitude=ToFloat(fields[6])
         longitude=ToFloat(fields[7])
         cog=ToFloat(fields[10])
         sog=ToFloat(fields[11])

         if  ((latitude!=0.0)  and (longitude!=0.0)):
           dateTimeStr=umtDate.strftime("%Y/%m/%d") + " " +umtTime.strftime("%H:%M:%S")

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
  cruiseName="WB0813"
  rootDir="F:\\Pices\\SipperFiles\\USF\\WB0813\\GpsData\\Daly Aug 2013 Met"

  fileNames=os.listdir(rootDir)
  for fileName in fileNames:
    if  fileName.endswith(".csv"):
      ProcessMetDataFile(cruiseName, rootDir+fileName)
  
