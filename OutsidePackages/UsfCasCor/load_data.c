/*

  load.c

  Routines to load the names file, datafile and testfile.

*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "usfcascor.h"

void _load_training_data();
void _load_training_example();
void _load_test_data();
void _load_test_example();
void _load_example_weights();

int class_index();  /* Located in load_namesfile.c */
extern int normalization_method;

void load_data()
{

  _load_training_data();
  if ( ! resubstitution )
    _load_test_data();

  info("Read in %d training patterns, %d test patterns of dimension %d\n", 
       NTrainingPatterns, 
       NTestPatterns, 
       Ninputs
      );
  
  if  (normalize_features) 
  {
    if  (normalization_method == 0)
      rnormalize ();
    else                         
      lnormalize ();
  }

  if  (load_weights) 
    _load_example_weights();

  return;
}


/*****************************************************************
  private functions
******************************************************************/
void _load_training_data()
{
  FILE  *datafile, *indexfile;
  char realfname[LINELEN+1];
  int rec_num,i;
  /********/

  /* open training data file */
  sprintf (realfname, "%s.data", filestem);
  datafile=FOPEN(realfname,"r");

  /* If index files are being used, we read the index number from the
     .idata file and seek to the right place  in the data file */
  if ( use_index_files ) {
    sprintf (realfname, "%s.idata", filestem);
    indexfile=FOPEN(realfname,"r");
    
    i=0;
    while ( fscanf(indexfile, "%d", &rec_num) > 0 ) {
      fseek(datafile, rec_num*line_length,0);
      _load_training_example(datafile, i++);
    }
    fclose(indexfile);
  } else {
    
    /* Else read them in one line at a time. */
    for (i=0; i < NTrainingPatterns; i++) 
      _load_training_example(datafile,i);
    
  } 
  fclose(datafile);
}


/* Read the next NTrainingPattern number of lines into the TrainingInput and
 * TrainingOutput data structures.
 */
void _load_training_example (FILE *infile, int i)
{
  int j, k;
  char classlabel[LINELEN+1];
  /**************/

  /* Get features. If a feature_type is set to ignore, skip over it */
  for  (j = 0, k = 0;  j < Ninputs;  k++ )
  {
    /* Some features are to be ignored, do so here. */
    if  (feature_type[k] == IGNORE )
    {
      while (fgetc(infile) != ',');
    } 

    else if  (feature_type[k] == BINARY )
    {
      /* For binary features, we have a hex string that represents the values, so we have to read all of them in */
      int bit_num=0;
      int bn;
      unsigned int cb;
      unsigned int bitmask[8] = {128,64,32,16,8,4,2,1};
      fscanf (infile, "0x");
      while  (j < Ninputs  &&  feature_type[k] == BINARY ) 
      {
        bn = bit_num % 8;
        if  (bn == 0)
        {
          cb = 0;
	        fscanf(infile,"%02x", &cb);
        }
        TrainingInputs[i][j] = ((cb & bitmask[bn]) > 0);
        bit_num++;
        j++;
        k++;
      }
      while (fgetc(infile) != ',');
    } 
    else
    {
      fscanf (infile, "%f,", &TrainingInputs[i][j]);
      j++;
    }
  }
  
  fscanf (infile, "%s", classlabel);
  while  (fgetc(infile) != '\n');
  k = class_index (classlabel);

  for  (j = 0;  j < Noutputs;  j++)
  {
    if  (j == k) 
      TrainingOutputs[i][j] = SigmoidMax;
    else
      TrainingOutputs[i][j] = SigmoidMin; 
  }
}  /* _load_training_example */




/* Get the test data from a seperate file.  Open the file
 */
void  _load_test_data (void)
{
  FILE  *indexfile, *datafile;
  char realfname[LINELEN+1];
  int rec_num;
  int i;
  /********/

  if  (use_index_files)
  {
    /* open test data file */
    sprintf (realfname, "%s.data", filestem);
    datafile = FOPEN (realfname, "r");

    /* open test index file */
    sprintf (realfname, "%s.itest", filestem);
    indexfile = FOPEN (realfname, "r");

    i = 0;
    while  (fscanf (indexfile, "%d", &rec_num) > 0 )
    {
      fseek (datafile, rec_num*line_length,0);
      _load_test_example (datafile, i++);
    }

    fclose (indexfile);
    fclose (datafile);
  } 
  else 
  {
    /* open test data file */
    sprintf (realfname, "%s.test", filestem);
    datafile = FOPEN (realfname, "r");
    for (i = 0;  i < NTestPatterns;  i++) 
       _load_test_example (datafile, i);
    fclose (datafile);
  }

  return;
}  /* _load_test_data */



/* Read the next NTestPattern number of lines into the TestInput and
 * TestOutput data structures.
 */
void _load_test_example (FILE* infile, 
                         int   i
                        )
{
  int j,k;
  char classlabel[LINELEN+1];
  /**************/


  /* Get features. If a feature_type is set to ignore, skip over it */
  for  (j = 0, k = 0;  j < Ninputs;  k++ )
  {
    /* Some features are to be ignored, do so here. */
    if  (feature_type[k] == IGNORE )
    {
      while (fgetc(infile) != ',');
    }
    else if ( feature_type[k] == BINARY )
    {
      /* For binary features, we have a hex string that represents the values, so we have to read all of them in */
      int bit_num=0;
      int bn;
      unsigned int cb;
      unsigned int bitmask[8]={128,64,32,16,8,4,2,1};
      fscanf (infile, "0x");
      while  (j < Ninputs && feature_type[k] == BINARY)
      {
        bn = bit_num % 8;
        if ( bn == 0 )
        {
          cb = 0;
          fscanf (infile,"%02x", &cb);
        }

        TestInputs[i][j] = ((cb & bitmask[bn]) > 0);
        bit_num++;
        j++;
        k++;
      }
      while (fgetc(infile) != ',');
    }
    else 
    {
      fscanf (infile, "%f,", &TestInputs[i][j]);
      j++;
    }
  }

  fscanf (infile, "%s", classlabel);
  while  (fgetc (infile) != '\n');
  k = class_index (classlabel);
  for  (j = 0;  j < Noutputs;  j++)
  {
    if  (j == k) 
      TestOutputs[i][j] = SigmoidMax;
    else
      TestOutputs[i][j] = SigmoidMin;
  }

}  / _load_test_example */



/* Open filestem.weights and read in all of the floating point weights. */
void  _load_example_weights ()
{
  int i;
  char filenamebuf[1024];
  FILE *fp;

  example_weight = (float *)malloc (NTrainingPatterns * sizeof (float));
  sprintf(filenamebuf, "%s.weights", filestem);
  if  ((fp=fopen (filenamebuf, "r")) == NULL )
  {
    fprintf (stderr,"Cannot open weight file\n");
    exit (1);
  }
  for  (i = 0;  i < NTrainingPatterns;  i++) 
    fscanf (fp, "%f", &(example_weight[i]));
  fclose(fp);

  return;
}  /* _load_example_weights */
