import mysql.connector
from mysql.connector import errorcode
import datetime
import os


class  ClassIdResolver:

  class  MLClass:
    def __init__(self,_classId,_className,_parentId):
                 self.classId    = _classId
                 self.className  = _className
                 self.parentId   = _parentId
            
		
  classListByName = {}
  classListById   = {}
  renameIdLookUp  = {}


  def  LoadClassListFromDB(self):
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

    if  db is None:
      return

    self.classIdLookUp = {}

    c = db.cursor()

    sqlStr = "select c.ClassId, c.ClassName, c.ParentId  from Classes c"

    c.execute(sqlStr)

    for (classId, className, parentId) in c:
      mc = self.MLClass(classId, className, parentId)
      self.classListByName [className.upper ()] = mc
      self.classListById   [classId]            = mc
      self.renameIdLookUp  [classId]            = classId

    db.close()

    return


  def  ClassIdFromName(self, name):
    classId = -1;
    try:
      classId = self.classListByName[name.upper ()].classId;
    except  KeyError as err:
      classId = -1
    return classId
      
  



  def  UpdateFromRenameList(self, fileName):
    try:
      renameData = open(fileName)
    except  OSError  as  err:
      print("Could not open \"" + fileName + "\".");
      return  None

    for  l in renameData:
      fields=l.split()
      if  len(fields) > 1:
        deleteFlag=""
        oldName=fields[0]
        newName=fields[1]
        if  len(fields) > 2:
          deleteFlag = fields[2]

        print(oldName, newName, deleteFlag)

        if  (len(newName) > 0)  and  (deleteFlag != "delete"):
          srcIdx  = self.ClassIdFromName (oldName)
          destIdx = self.ClassIdFromName (newName)
          if  (srcIdx > 0)  and  (destIdx > 0):
            self.renameIdLookUp[srcIdx] = destIdx;
          print(srcIdx,destIdx)

    renameData.close()
    return



  def  ResolveClassId (self,classId):
    result = -1
    try:
      result = self.renameIdLookUp[classId]
    except  KeyError  as err:
      result = -1
    return result
    




  def  __init__(self):
    self.LoadClassListFromDB()
    self.UpdateFromRenameList("E:\\Users\\kkramer\\Dropbox\\Sipper\\FromAndrewToKurt\\ClassListChanges_2014-06-04.txt");
 
