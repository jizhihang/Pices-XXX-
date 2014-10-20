/* utils.c
  
   This file is implemented as macros using gcc. If you are not fortunate
   enough to be able to use gcc, then these functions (mostly) implement
   the same functionality.

   Nothing in life is perfect. We now have some non-macros due to the 
   zlib facility.

*/


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _GNU_SOURCE
  #include "zlib.h"
#else
  #include <zlib.h>
#endif

extern int errno;

/* Open a file, checking also for a .gz extension */
gzFile GZOPEN(const char *path, const char *mode)
{
  char *p;
  gzFile f;

  /* Try to open as is */
  if ( (f=gzopen(path,mode)) != NULL ) return f;

  /* No luck, create a new string with a .gz extension (must be
     dynamic here). */
  p=malloc((strlen(path)+4)*sizeof(char));
  strcpy(p, path);
  strcat(p,".gz");

  /* Now try again */
  f=gzopen(p,mode);

  /* Whether it worked or not, free up space */
  free(p);
  if ( f == NULL ) {
    fprintf(stderr,"Error opening %s (mode %s): %s\n", path, mode,strerror(errno)); 
    exit(1);
  }

  return f;
}



#ifndef __GNUC__

int debug_level;
extern int errno;
void exit();

void info(char *msg, ...);
void warn(char *msg, ...);
void debug(int level, char *msg, ...);
void die(char *msg, ...);

void *MALLOC();
void *CALLOC();
void *REALLOC();
FILE *FOPEN();



void info(char *msg, ...)
{
   va_list ap;
  
   va_start(ap, msg);
   vfprintf(stderr, msg, ap);
   
   return;
}


void warn(char *msg, ...)
{
   va_list ap;
   va_start(ap,msg);

   vfprintf(stderr, msg, ap);
   return;

}

void debug(int level, char *msg, ...)
{
   va_list ap;
   va_start(ap,msg);
   if ( debug_level >= level ) 
      vfprintf(stderr,msg, ap);

   return;
}

void die(char *msg,...)
{
   va_list ap;
   va_start(ap,msg);
 
   vfprintf(stderr, msg, ap); 
   exit(1);
}


void *MALLOC(int size)
{
  register void *p; 

  if ( (p=(void *)malloc(size)) == 0 ) { 
    fprintf(stderr, "error allocating memory\n");
    exit(1); 
  }
  
  return p;
}

void *CALLOC(int n, int size)
{
  register void *p;

  if ( (p=(void *)calloc(n,size)) == 0 ) { 
    fprintf(stderr, "error allocating memory\n");
    exit(1);
  }
  return p; 
}

void *REALLOC(void *q, int n)
{
  register void *p; 
  if ( (p=(void *)realloc(q,n)) == 0 ) {
    fprintf(stderr, "error re-allocating memory\n");
    exit(1); 
  }
  return  p;  
}

FILE *FOPEN(char *filename, char *mode)
{
  FILE *fp; 

  if ( (fp=fopen(filename, mode)) == 0 ) {
    fprintf(stderr,"Error opening %s (mode %s): %s\n",
	    filename, mode, strerror(errno));
    exit(1);
  }

  return fp;
}


void chomp(char *q)
{
  register char *p=q; 
  register int plen=strlen(p); 
  if ( p != 0 && plen > 0 ) 
    p[plen-1]=0; 

  return;
}

#endif
