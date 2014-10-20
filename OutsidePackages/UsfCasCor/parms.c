#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "usfcascor.h"


#define EOL '\0'


Boolean _boolean_convert();
int     _type_convert();

static char parm_string[LINELEN];
/* Save the settable parameters to a file for later use. */
char *parm_to_string(int k)
{

  switch(ParmTable[k].vartype){ 
  case INT: case INT_NO:
    sprintf(parm_string,"%s=%d\n", ParmTable[k].keyword,
	    *(int *)ParmTable[k].varptr);
    break;
  case FLOAT: case FLOAT_NO:
    sprintf(parm_string, "%s=%f\n", ParmTable[k].keyword, 
	    *(float *)ParmTable[k].varptr);
    break;
  case ENUM: case ENUM_NO:
    sprintf(parm_string, "%s=%s\n", ParmTable[k].keyword, 
	    type_to_string(*(int *)ParmTable[k].varptr));
    break;
  case BOOLE: case BOOLE_NO:
    sprintf(parm_string, "%s=%s\n", ParmTable[k].keyword, 
	    boolean_to_string(*(Boolean *)ParmTable[k].varptr));
    break;
  default:
    parm_string[0]=0;
    break;			/* skip anything else */
  }

  return parm_string;
}


char *type_strings[]={"SIGMOID","GAUSSIAN", "LINEAR","ASYMSIGMOID","VARSIGMOID","WIN","STAGNANT","TIMEOUT","LOSE","BITS","INDEX","Bad Type"};
/* Input of the type variables and return a string showing its value.  This
 * is only used as a output routine for the user's convenience. 
 */
char *type_to_string(int var)
{
  switch (var) {
  case SIGMOID:
    return(type_strings[0]);
  case GAUSSIAN:
    return(type_strings[1]);
  case LINEAR:
    return(type_strings[2]);
  case ASYMSIGMOID:
    return(type_strings[3]);
  case VARSIGMOID:
    return(type_strings[4]);
  case WIN:
    return(type_strings[5]);
  case STAGNANT:
    return(type_strings[6]);
  case TIMEOUT:
    return(type_strings[7]);
  case LOSE:
    return(type_strings[8]);
  case BITS:
    return(type_strings[9]);
  case INDEX:
    return(type_strings[10]);
  default: 
    return(type_strings[11]);

 }
}



static char *boolean_strings[2]={"False","True"};

char *boolean_to_string(Boolean var)
{
  switch (var) {
  case False:
    return(boolean_strings[0]);
  case True:
    return(boolean_strings[1]);
  default: 
    die("Bad boolean type\n");
 }
}





/* Convert '\0' terminated sting to all lower case characters.  This routine
 * is destructive.
 */
void strdncase(char *s)
{
  int i;
  /************/

  for(i=0; s[i] != EOL; i++)
    if(isupper(s[i]))
      s[i] = tolower(s[i]);	/* tolower only guaranteed on upper case */
    else
      s[i] = s[i];
}

/* Given a keyword string, return the index into the keyword table.
 * Assumes that the keys are in alphabetacal order.  Keyword comparison
 * is all lower case.  Return FAILURE when not found.
 */
int find_key(char *searchkey)
{
  int lower = 0;
  int upper = Nparameters - 1; 
  int m,dif;
  /************/

  strdncase(searchkey);		/* convert case for comparison */

  while(lower <= upper){
    m = (upper + lower) / 2;
    dif = strcmp(searchkey, ParmTable[m].keyword);
    if(dif<0)
      upper = m - 1;		/* look in lower half */
    else if(dif == 0)
      return(m);		/* found it */
    else if(dif > 0)
      lower = m + 1;		/* look in upper half */
  }
  
  /* search failed */
  return(FAILURE);
}

/* Parse a line of input into keyword value pairs and reset the given 
 * parameters to given values.  Comment lines start with the character
 * '#' and are ignored.  If a bad keyword is given a message is printed,
 * but processing continues.  The routine returns a value telling the 
 * calling routine whether to grap another line, read in  the training, 
 * or read in testing data.  The special keywords "Training", and 
 * "Testing" signal the changes in status.
 */
int process_line(char *line)
{
  int k = 0;			/* location in ParmTable */
  char *keytok;			/* token pointer */
  char *valtok;			/* token pointer */
  static char *seperators = "= \t\v\f\r\n,"; /* white space plus comma  */
  /*************/

  /* check for comment character */
  if(line[0] == '#' || line[0] == '\n')
    return(NEXTLINE);		/* skip comment and blank lines */
  else{

    keytok = strtok(line, seperators); /* get first token */
    while(keytok != NULL){
      k = find_key(keytok);

      if(k != FAILURE){
	/* get value token for this parameter */
	valtok = strtok(NULL, seperators);

	/* read value in correct format */
	switch(ParmTable[k].vartype){ 
	case INT: case INT_NO:
	  *(int *)ParmTable[k].varptr = atoi(valtok);
	  break;
	case FLOAT: case FLOAT_NO:
	  *(float *)ParmTable[k].varptr = (float)atof(valtok);
	  break;
	case ENUM: case ENUM_NO: 
	  *(int *)ParmTable[k].varptr = _type_convert(valtok);
	  break;
	case BOOLE: case BOOLE_NO:
	  *(Boolean *)ParmTable[k].varptr= _boolean_convert(valtok);
	  break;
	case INITFILE:
	  warn("initfile option no longer accepted.");
	  break;
	case GO: case VALUE: case SAVE:
	  warn("%s keyword only legal in interactive mode.", keytok);
	  break;
	default:
	  warn("%d: bad vartype for parameter %s.  No update performed.", 
		  ParmTable[k].vartype, keytok);
	  break;
	}
      }
      else			/* bad key */
	warn("%s: not in parameter table.  No update performed.", keytok);

      /* get next keyword token */
      keytok = strtok(NULL, seperators);	
    }				/* end while still keytok */
    return(NEXTLINE);
  }				/* end if comment */
}




/**********************************************************
 Functions needed only in this file. 
***********************************************************/

int _type_convert(char *input)
{
  strdncase(input);
  
  if(!strcmp(input,"true"))
    return(1);
  else if(!strcmp(input,"1"))	/* allow backward compatiple input */
    return(1);
  else if(!strcmp(input,"false"))
    return(0);
  else if(!strcmp(input,"0"))	/* allow backward compatiple input */
    return(0);
  else if(!strcmp(input,"sigmoid"))
    return(SIGMOID);
  else if(!strcmp(input,"gaussian"))
    return(GAUSSIAN);
  else if(!strcmp(input,"linear"))
    return(LINEAR);
  else if(!strcmp(input,"asymsigmoid"))
    return(ASYMSIGMOID);
  else if(!strcmp(input,"varsigmoid"))
    return(VARSIGMOID);
  else if(!strcmp(input,"bits"))
    return(BITS);
  else if(!strcmp(input,"index"))
    return(INDEX);
  else 
    die("Bad string sent to TYPE_CONVERT %s", input);
}

Boolean _boolean_convert(char *input)
{
  strdncase(input);
  if ( !strcmp(input,"true") || !strcmp(input,"1"))
    return True;

  if ( !strcmp(input,"false") || !strcmp(input,"0"))
    return False;

  die("Bad string sent to boolean_convert: %s", input);
}

