import mysql.connector
from mysql.connector import errorcode
import datetime
import os


def  ToInt(s):
  i=0;
  try:
    i=int(s)
  except  ValueError:
    i=0
  return i




def  ProcessSipperFileStartStopPoints(fileName):
  try:
       db = mysql.connector.Connect(user='root',
                                    password="dasani30",
                                    host='sipper-db.marine.usf.edu',
                                    database='pices_new'
                                  )
  except  mysql.connector.Error as err:
    db = None
    if  err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
        print("Cound not conect to \"Sipper-d-marine.usf.edu\"")
    elif  err.errno == errorcode.ER_BAD_DB_ERROR:
        print("Database does not exists")
    else:
        print(err)

  if db is None:
    return

  c = db.cursor()
  
  startStopFile = open(fileName)

  for  l in startStopFile:
    l = l.strip ()
    fields=l.split('\t')
    if  len(fields) > 2:
      sipperFileName = fields[0]
      startPoint     = ToInt (fields[1])
      endPoint       = ToInt (fields[2])

      sqlStr = "update SipperFiles set ExtractionScanLineStart = " + str(startPoint) + ", " + \
                                       "ExtractionScanLineEnd = "   + str(endPoint) + " " +   \
                      "where  SipperFileName = " + sipperFileName
      print(sqlStr)
      try:
        c.execute(sqlStr)
        db.commit()
      except  mysql.connector.Error  as err:
        print(err)
   
  startStopFile.close ()
  db.close ()
  



def  main():
  fileName = "C:\\Temp\\MySQLDumps\\SipperFilesStartStopPoints.txt"
  ProcessSipperFileStartStopPoints (fileName)




main()

  
