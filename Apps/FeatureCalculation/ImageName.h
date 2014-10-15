#ifndef  _IMAGENAME_
#define  _IMAGENAME_

#include  "HashTable.h"

Class  ImageName
{
public:
  ImageName (String  _name,
             String  _dir):
         name (_name),
         dir (_dir)
         {}
          
  const 
  String&  Key ()  {return  name;}
  
  const
  String&  Name () {return  name;}

  const
  String&  Dir ()  {return  dir;}



private:
  String  name;
  String  dir;
}




Class  ImageNameList: HashTable<TrainingImage>
{
  ImageNameList (bool  _owner):
      HashTable (_owner, 5000, 33)
      {}
      
}



#endif
