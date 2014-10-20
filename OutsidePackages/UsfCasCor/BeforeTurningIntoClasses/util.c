#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "usfcascor.h"

extern float WeightRange;

/*
 * Return a float between -range and +range.
 */
float random_weight(void)
{
  return ( (float) (WeightRange * (rand()%1000 / 500.0)) - WeightRange);
}


char *trim(char *p)
{
  char *q,*r;

  /* Trim leading spaces */
  for (q=p; *q != 0 && isspace(*q); q++);
  /* Strip off trailing spaces */
  for (r=q+strlen(q)-1; r >= q && isspace(*r); r--) *r=0;
  /* And check for the last character a . */
  if ( r >= q && *r=='.') *r=0;
  return q;
}


/* Additional functions */
int class_index(char *label) {
  int i;
  int endoflabel;
  
  if ( label == NULL || (endoflabel=strlen(label)-1) < 0) 
    die("Class label not correct");

  if ( label[endoflabel] == '.') label[endoflabel]=0;

  for (i=0; i < number_of_classes;i++) {
    if (strcmp(label, class_label[i]) == 0 )
      return i;
  }

  die("Class label %s not found\n", label);

  return -1;
}
