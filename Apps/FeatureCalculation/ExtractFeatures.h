#ifndef  _EXTRACTFEATURES_
#define  _EXTRACTFEATURES_


#include "Application.h"

#include "FileDesc.h"
#include "ImageFeatures.h"
#include "MLClass.h"


class  ExtractFeatures: public  Application
{
public:
  typedef  KKU::uint  uint;

  ExtractFeatures (int     argc,
                   char**  argv
                  );
  
  ~ExtractFeatures ();


  void  ClassifyImage (ImageFeaturesPtr  image);

  void  Extract ();


  virtual const 
  char*          ApplicationName ()  {return  "TrainingProcess";}

  void           DisplayCommandLineParameters ();


  virtual 
  bool           ProcessCmdLineParameter (char    parmSwitchCode, 
                                          KKStr  parmSwitch, 
                                          KKStr  parmValue
                                         );


private:
  void  AddInstrumentData (ImageFeaturesListPtr  data);

  void  ClassifyImages ();

  void  MoveImagesToDestinationDirectoryByClassName ();

  // One time only function to separate out images that originated from SIPPER 
  // file "SMP751001037_22".  This was the one SIPPER file that andrew extracted images
  // at a lower threshol;d than 250.
  void  MoveOver_SMP751001037_22_ImagesIntoSeparateDir (ImageFeaturesListPtr  examples);

  void  ReportDuplicates ();


  bool                      cancelFlag;
  Classifier2Ptr            classifier;
  KKStr                     configFileName;
  KKStr                     destDirectory;
  KKStr                     featureFileName;
  FileDescPtr               fileDesc;
  KKStrListPtr              fileNameList;
  MLClassConstPtr           mlClass;
  MLClassConstListPtr       mlClasses;
  ImageFeaturesListPtr      images;
  ostream*                  report;
  ofstream*                 reportFile;
  KKStr                     reportFileName;
  bool                      retrieveFeaturesFromFile;
  KKStr                     rootFeatureFileName;
  KKStr                     sourceRootDirPath;
  KKStr                     startDirectory;
  KKStr                     statusMessage;
  bool                      successful;
  TrainingProcess2Ptr       trainer;
  TrainingConfiguration2Ptr trainingConfiguration;
  bool                      useDirectoryNameForClassName;
};





#endif