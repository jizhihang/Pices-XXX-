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



def  LoadClassList(fileName):
  try:
    classData = open(fileName)
  except  OSError  as  err:
    print("Could not open \"" + fileName + "\".");
    return  None

  classDic = {}

  for  l in classData:
    fields=l.split('\t')
    if  len(fields) > 3:
      classId = ToInt (fields[0])
      classMame = fields[1]
      classDic[classId] = classMame

  classData.close()
  
  return  classDic



def  ImportValidationDataInDirectory(dirName):
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
  
  fullClassFileName = os.path.join(dirName, "ClassList") + ".txt"
  classDic = LoadClassList(fullClassFileName)
  if  classDic is None:
    return

  fullValdationFileName = os.path.join(dirName, "ValidatedImagesList") + ".txt"

  validationData = open(fullValdationFileName)

  for  l in validationData:
    fields=l.split('\t')
    if  len(fields) > 1:
      imageFileName=fields[0]
      classId=ToInt(fields[1])
      className=classDic[classId]

      sqlStr="call ImagesUpdateValidatedClassIfDiff(" + "\""  + imageFileName  + "\"" + "," + \
                                                        "\""  + className      + "\"" + "," + \
                                                        "1.0" + \
                                                   ")"
      print(sqlStr)
      try:
        c.execute(sqlStr)
        db.commit()
      except  mysql.connector.Error  as err:
        print(err)
   
  validationData.close ()
  


def  main():
  rootDir="E:\\Users\\kkramer\\Dropbox\\Sipper\\FromAndrewToKurt\\Validation\\2014-09-16\\"
  ImportValidationDataInDirectory(rootDir)


main()

  
