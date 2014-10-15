#ifndef  _IMAGENAME_
#define  _IMAGENAME_


class  ImageName
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
};




class  ImageNameList: HashTable<ImageName>
{
  ImageNameList (bool  _owner):
      HashTable<ImageName> (_owner, 5000, 33)
      {}
      
};



#endif
