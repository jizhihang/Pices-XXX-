

#include  <stdlib.h>
#include  <stdio.h>
#include  "Str.h"



void  StripCarrageReturns (FILE* inFile, 
                           FILE* outFile)
{
  char  buff[10240];
  int   l;

  while  (fgets (buff, sizeof (buff), inFile))
  {
    l = strlen (buff);
    while  ((l > 0)   &&  (buff[l - 2] == '\r'))
    {
      buff[l - 2] = buff[l - 1];
      buff[l - 1] = 0;
      l--;
    }

    fputs (buff, outFile);
  }
}


bool  FileExists (String&  fileName)
{
  FILE*  f;

  bool  exists;

  f = fopen (fileName.Str (), "r");
  
  exists = (f != NULL);
  if  (exists)
    fclose (f);

  return  exists;
}



void  RenameFiles (String&  inFileName, 
                   String&  outFileName)
{
  String  cmd;
  String  newInFileName;
  int     returnCd;
  int     x;

  x = 0;

  do  {
    newInFileName = inFileName;
    newInFileName << ".old";
    if  (x > 0)
      newInFileName << x;
    x++;
  }  while  (FileExists (newInFileName));
   
  cmd = "mv ";
  cmd << inFileName << " " << newInFileName;
  returnCd = system (cmd.Str ());
  if  (returnCd != 0)
  {
    cerr << endl;
    cerr << "StripCntrlM  *** ERROR[" << returnCd << "] *** - Renaming '"
         << inFileName    << "'  to  '"
         << newInFileName << "'."
         << endl;
    exit (1);
  }


  cmd = "mv ";
  cmd << outFileName << " " << inFileName;
  returnCd = system (cmd.Str ());
  if  (returnCd != 0)
  {
    cerr << endl;
    cerr << "StripCntrlM  *** ERROR[" << returnCd << "] *** - Renaming '"
         << outFileName    << "'  to  '"
         << inFileName << "'."
         << endl;
    exit (1);
  }



}




void  main (int argc,  char**  argv)
{
  String  inFileName;
  FILE*   inFile;
  FILE*   outFile;
  String  outFileName;

  if  (argc > 1)
  {
    inFileName = strdup (argv[1]);;
    inFile = fopen (inFileName.Str (), "r");
    if  (!inFile)
    {
      cerr << endl
           << "StripCntrlM - *** ERROR ***  File[" << inFileName 
           << "] not found." 
           << endl;
      exit (1);
    }

    outFileName = inFileName;
    outFileName << ".temp";

    outFile = fopen (outFileName.Str (), "w");

    if  (!outFile)
    {
      cerr << endl
           << "StripCntrlM - *** ERROR ***  Could not create Output File[" 
           << outFileName  << "]." 
           << endl;
      exit (1);
    }
  }
  else
  {
    inFile = stdin;
    outFile = stdout;
  }    

  StripCarrageReturns (inFile, outFile);  

  if (!inFileName.Empty ())
  {
    fclose (inFile);
    fclose (outFile);

    RenameFiles (inFileName, outFileName);
  }


}