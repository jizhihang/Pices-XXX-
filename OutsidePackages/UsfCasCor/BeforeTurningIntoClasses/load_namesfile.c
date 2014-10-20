/*
  load_namesfile

  Process the names file to get the appropriate information needed
  for creating the neural network.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "usfcascor.h"


#define N 1024
#define ALLOC_INCR 100


void process_namesfile (char *filestem);
void get_example_counts ();



/* The guts of the transformation to C4.5-like format. 
   We have to read in both the data and test files, to get the number of
   patterns from each.
   We also have to open the names file and figure out:
    1) Class names (assign indices to them)
    2) number of features
*/



void  load_namesfile (char *filestem)
{
  /* First, ensure the necessary variables are reset */
  NTrainingPatterns = -1;
  NTestPatterns     = -1;
  line_length       = -1;

  /* It is possible to get everything out of the names file. */
  process_namesfile (filestem);

  /* If not everything was included in the names file, we have 
     to get extra information */
  get_example_counts (filestem);

  return;
}






void process_namesfile (char *filestem) 
{
  char filename[LINELEN+1];
  FILE *fp;
  char s[N], *p, *q, *r;
  Boolean seen_classes=False;
  int feature_count=0;
  int i;

  Ninputs=0;

  sprintf  (filename,"%s.names", filestem);
  fp = FOPEN (filename,"r");

  /* First useful line will be the class information */
  while ((p=fgets (s, N, fp)) != NULL) 
  {
    /* All of this strips leading/trailing spaces */
    p = trim (s);
    if  (strlen (p) <= 0 ) 
      continue;

    /* Check for various meta-data in the form of |@ comments */
    if  (!strncmp (p, "|@LINELENGTH=",13)) 
    {
      line_length=atoi(p+13);
      continue;
    } 

    else if  (!strncmp (p, "|@TRAINEXAMPLES=", 16))
    {
      NTrainingPatterns=atoi (p+16);
      continue;
    } 
    
    else if  (!strncmp(p, "|@TESTEXAMPLES=", 15)) 
    {
      NTestPatterns=atoi (p + 15);
      continue;
    }
    
    else if  (!strncmp (p, "|@DESCRIPTION=",14)) 
    {
      info ("-- %s\n", p + 14);
      continue;
    }

    /* Ignore any other comments */
    if  (*p == '|') 
       continue;

    /* Now we have a fairly clean line, figure out what to do with it */
    if  (!seen_classes) 
    {   
      /* This is the classes line                                          */
      /* First, go through the string and find the number of commas. This  */
	  /* is the number of classes-1                                        */
      number_of_classes = 1;
      for  (q = p; *q != 0; q++) 
         if  (*q == ',') 
            number_of_classes++;

      class_label = (char **)CALLOC (number_of_classes, sizeof (char *));

      for (i = 0; i < number_of_classes; i++) 
      {
        if  ((q = index (p, ','))  !=  NULL) 
        {
          *q=0;
          q++;
        }

        class_label[i] = strdup (trim (p));
        p = q;
      }

      seen_classes=True;
    } 
    
    else 
    {
      /* Else this is a feature description line of the type  */
	  /* feature name: type.                                  */

      if  ((q = index (p, ':')) != NULL)
      {
        /* Make sure feature_type has enough examples */
        if  (feature_count % ALLOC_INCR == 0)
           feature_type = REALLOC (feature_type, sizeof (int) * (feature_count + ALLOC_INCR));

        /* Find a clean description of the feature */
        q++;
        r = trim (q);

        /* If it's ignore, note that */
        if  (!strncasecmp("IGNORE", r, 6)) 
        {
          feature_type[feature_count]=IGNORE;
        } 

        else if  (!strncasecmp ("BINARY", r, 6)) 
        {
	      /* In the case of binary, allocate that many floats */
          int n,k;
          n = atoi (r + 6);
          feature_type = REALLOC (feature_type, sizeof (int) * (feature_count + n));
          for (k = 0; k < n; k++) 
          {
            feature_type[feature_count] = BINARY;
            Ninputs++;
            feature_count++;
          }

	      feature_count--; // Kludge for below
        } 

        else 
        {
          feature_type[feature_count] = REAL;
          Ninputs++;
        }

        feature_count++;
      }
    }
  }

  fclose (fp);

  /* Setup nn variables */
  Noutputs = number_of_classes;

  return;
}




void  get_example_counts ()
{
  char filename[LINELEN];
  FILE *fp;
  int num_lines,c;


  if  (NTrainingPatterns == -1) 
  {
    /* Easy stuff first, get the number of lines from data and test,
       they correspond to the number of patterns */
    if  (use_index_files) 
      sprintf (filename, "%s.idata", filestem);
    else 
      sprintf (filename, "%s.data",  filestem);
  
    fp = FOPEN (filename, "r");
    
    num_lines = 0;
    while ((c = fgetc(fp)) != EOF ) 
    {
      if  (c == '\n' && (c = fgetc (fp)) != '\n') 
        num_lines++;
    }

    fclose (fp);
    NTrainingPatterns = num_lines;
  }


  /* This needs to handle resubstitution better */
  if  (NTestPatterns == -1) 
  {
    if  (use_index_files) 
      sprintf (filename, "%s.itest", filestem);
    else 
      sprintf (filename, "%s.test",  filestem);

    fp = FOPEN (filename,"r");
    
    num_lines = 0;
    while  ((c = fgetc (fp)) != EOF) 
    {
      if (c == '\n' && (c = fgetc(fp)) != '\n') 
	    num_lines++;
    }

    fclose (fp);
    NTestPatterns = num_lines;
  }


  /* Here we peek to get the length of a line */
  if  (use_index_files  &&  line_length == -1) 
  {
    sprintf (filename,"%s.data", filestem);
    fp = FOPEN (filename,"r");
    
    for  (line_length=1; (c=fgetc(fp)) && c != '\n'; line_length++);

    fclose(fp);
  }

  return;
}

