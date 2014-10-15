#ifndef  _DELETEDUPLICATEIMAGES_
#define  _DELETEDUPLICATEIMAGES_

#include  "MLClass.h"


class  DeleteDuplicateImages: public  Application
{
public:
  DeleteDuplicateImages (int     argc,
                         char**  argv
                        );


  ~DeleteDuplicateImages ();

  virtual const 
  char*    ApplicationName ()  {return  "DeleteDuplicateImages";}


  void     DeleteImages ();


private:
  void    DisplayCmdLine ();

  virtual 
  bool     ProcessCmdLineParameter (char    parmSwitchCode, 
                                    KKStr  parmSwitch, 
                                    KKStr  parmValue
                                   );


  bool                 cancelFlag;
  MLClassConstList  mlClasses;
  ofstream*            r;
  KKStr                reportFileName;
  KKStr                rootDir;
  KKStr                duplicateImagesDir;
};


typedef  DeleteDuplicateImages*  DeleteDuplicateImagesPtr;


#endif
