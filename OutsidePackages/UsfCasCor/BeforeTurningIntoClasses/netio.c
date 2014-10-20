/*
  save_network.c

  Routines to save a network to a file, that could be read in later
  in another run.
*/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "usfcascor.h"


#define BEGIN_PARAMETER_STRING "# Parameters\n"
#define END_PARAMETER_STRING   "# End Parameters\n"
#define BEGIN_CONNECTION_STRING "# Connections\n"
#define END_CONNECTION_STRING   "# End Connections\n"
#define BEGIN_INPUT_WEIGHTS_STRING "# Input Weights\n"
#define END_INPUT_WEIGHTS_STRING "# End Input Weights\n"
#define BEGIN_OUTPUT_WEIGHTS_STRING "# Output Weights\n"
#define END_OUTPUT_WEIGHTS_STRING "# End Output Weights\n"

/* 
   Save all of the parameters needed to a file, so that the entire 
   network state can be restored later.
*/
void save_network(int current_trial, int current_round)
{
  FILE *nfp;
  long tt;
  char fname[LINELEN];
  int i,j;
  /***************/

  /* The default filename is filestem-trial-rounds.net */
  if ( current_round == -1 )
    sprintf(fname,"%s-%d.network",filestem,current_trial);
  else
    sprintf(fname,"%s-%d-%d.network",filestem,current_trial,current_round);
  nfp=FOPEN(fname,"w");
  time(&tt);
  fprintf(nfp,"# Net file saved from Cascor Version %s\n", version);
  fprintf(nfp,"# Saved at %s#\n", ctime(&tt));


  /* Dump Architecture */
  fprintf(nfp,"# Architecture\n");
  fprintf(nfp, "number_of_units=%d\nnumber_of_outputs=%d\n",Nunits,Noutputs);



  /* Save parameters */
  fputs(BEGIN_PARAMETER_STRING,nfp);
  for (i=0; i<Nparameters; i++)
    fputs(parm_to_string(i),nfp);
  fputs(END_PARAMETER_STRING,nfp);


  /* Dump connections */
  fputs(BEGIN_CONNECTION_STRING,nfp);
  for(i=0; i<Nunits; i++)
    fprintf(nfp, "connections[%d]=%d\n",i, Nconnections[i]);
  fputs(END_CONNECTION_STRING,nfp);

  /* Dump Input Weights */
  fputs(BEGIN_INPUT_WEIGHTS_STRING,nfp);
  for(i=0; i<Nunits; i++)
    if(Nconnections[i]){
      for(j=0; j<Nconnections[i]; j++)
	fprintf(nfp, "input_weight[%d][%d]=%17.15g\n", i,j,Weights[i][j]);
    }
  fputs(END_INPUT_WEIGHTS_STRING,nfp);

  /* Dump Output Weights */
  fputs(BEGIN_OUTPUT_WEIGHTS_STRING,nfp);
  for(i=0; i<Noutputs; i++){
    for(j=0; j<Nunits; j++)
      fprintf(nfp, "output_weight[%d][%d]=%17.15g\n", i,j,OutputWeights[i][j]);
  }
  fputs(END_OUTPUT_WEIGHTS_STRING,nfp);

  /* All done */
  fclose(nfp);

  return;
}









/* Read a .wgt file and do limited consistency checking with the input
 * from the .net file.  Space must be allocated for the Weight vectors in 
 * this routine.  During normal training the space is allocated in 
 * INSTALL_NEW_UNIT.
 */
void load_network(char *netfile)
{
  int i,j, noutputs, nunits;
  int tmpi, tmpj;
  int tmp;
  float *w=NULL;			/* temporary weight array */
  char junk[LINELEN];
  FILE *nfp;
  /***************/

  info("Loading configuration from %s\n", netfile);

  /* open network configuration file */
  nfp=FOPEN(netfile,"r");

  /* First, we need to get both the particular parameters we need,
     as well as all of the general-purpose parameters */
  while ( fgets(junk,LINELEN,nfp) != NULL ) {
    if ( sscanf(junk,"number_of_units=%d", &nunits) > 0 ) continue;
    if ( sscanf(junk,"number_of_outputs=%d", &noutputs) > 0 ) continue;
    /* We should have a marker that denotes when parameters start
       and end. We stop when parameters end.
    */
    if ( !strcmp(END_PARAMETER_STRING, junk) ) break;
    if ( junk[0] == '#' ) continue;
    process_line(junk);
  }


  /* Sanity checks before reading in weight values */
  /* check to see if this matches the .net file inputs. */
  if(noutputs != Noutputs){
    warn("noutputs in net file does not match Noutputs.\n  Training will start from scratch.");
    return;
  }
  if(nunits > MaxUnits){
    die("nunits(%d) in .wgt file is greater than MaxUnits in .net\n  Please change MaxUnits and try again.", nunits);
  }
    
  /* Set globals from input. */
  Nunits = nunits;
  Noutputs = noutputs;
    

  /* Get Nconnections */
  while ( fgets(junk,LINELEN,nfp) != NULL ) 
    if ( !strcmp(BEGIN_CONNECTION_STRING,junk) ) break;
  for(i=0; i<Nunits; i++) {
    fscanf(nfp, "connections[%d]=%d\n",&tmp, &Nconnections[i]);
    if ( tmp != i ) die("Error parsing net file\n");
  }

  /* Get Input Weights */
  while (fgets(junk,LINELEN,nfp) != NULL )
    if ( !strcmp(BEGIN_INPUT_WEIGHTS_STRING,junk) ) break;
  for(i=0; i<Nunits; i++) {
    if(Nconnections[i]){
      w = (float *)CALLOC(Nconnections[i], sizeof(float));   
      for(j=0; j<Nconnections[i]; j++) {
	fscanf(nfp, "input_weight[%d][%d]=%f\n", &tmpi, &tmpj, &w[j]);
	if ( tmpi != i || tmpj != j ) die("Error parsing net file\n");
      }
      Weights[i] = w;
      Connections[i] = AllConnections;
    }
  }

  /* Get Output Weights */
  while (fgets(junk,LINELEN, nfp) != NULL )
    if ( !strcmp(BEGIN_OUTPUT_WEIGHTS_STRING,junk) ) break;

  for(i=0; i<Noutputs; i++) {
    for(j=0; j<Nunits; j++) {
      fscanf(nfp, "output_weight[%d][%d]=%f\n",&tmpi, &tmpj,
	     &OutputWeights[i][j]);
      if ( tmpi != i || tmpj != j ) die("Error parsing net file\n");
    }
  }

  fclose(nfp);			/* close the weight file. */
    
  /* If using cache, run an epoch to compute hidden units' values.  */
  if(UseCache) {

     /* Odd error check. If usecache was specified in file, but not on
        command line, then Cache was not allocated. We look for NULL
        value and allocate storage here. */
     if(ValuesCache == NULL ){
       ValuesCache = (float **)CALLOC(MaxCases, sizeof(float *));
       ErrorsCache = (float **)CALLOC(MaxCases, sizeof(float *));

       for(i=0; i<MaxCases; i++){
         ValuesCache[i] = (float *)CALLOC(MaxUnits, sizeof(float));
         ErrorsCache[i] = (float *)CALLOC(Noutputs, sizeof(float));
       }
     }


    for(i=0; i<NTrainingPatterns; i++){
      Values = ValuesCache[i];
      
      /* Unit values must be calculated in order because the activations */
      /* cascade down through the hidden layers */
      for(j= 1+Ninputs; j<Nunits; j++) 
	COMPUTE_UNIT_VALUE(j);
    }
  }

  return;
}
