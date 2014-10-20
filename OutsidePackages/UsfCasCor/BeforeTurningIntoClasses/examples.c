
/**************************************************************************/
/*        TEST examples                                                   */
/*                                                                        */
/* Two examples follow.  The first, zigzag, is presented as a complete    */
/* .net file.  Cut between the lines and create zigzag.net, then follow   */
/* the example session that follows.                                      */
/*                                                                        */
/* The two spirals benchmark is included as the second example.  You      */
/* must create the file two-spirals.c, compile and run it to produce      */
/* twospirals.net.  You may then use twospirals.net as an input to        */
/* cascor as shown for the zigzag example.                                */
/*                                                                        */
/* Both examples are included within a precompiler #ifdef block so that   */
/* this source file can be compiled without editing and so that I can     */
/* include comments within the example source code.  Please do not try    */
/* to compile this file with the symbol "YOU_WANT_TO_COMPILE_JUNK"        */
/* defined.  (Yes, someone has tried it.)                                 */
/**************************************************************************/
#ifdef YOU_WANT_TO_COMPILE_JUNK

------------------------- zigzag.net -----------------------------------/*
# Comment lines start with a '#'
#Zig Zag net work file with 5 zig-zags.
#
Ninputs 1  Noutputs 1
UnitType SIGMOID     OutputType SIGMOID
# used to say UseCache 1 now use TRUE or FALSE
UseCache TRUE
NTrainingPatterns 10  NTestPatterns 0
#Change some parameters
WeightRange 1.1
Training
# add some comments this is a zig
  1.0     0.5
 -1.0    -0.5
# here is a zag
  2.0    -0.5
 -2.0     0.5
# another zig
  3.0     0.5
 -3.0    -0.5
  4.0    -0.5
 -4.0     0.5
  5.0     0.5
 -5.0    -0.5
Testing
---------------------end of zigzag.net 1 ------------------------------
# Maybe I want to test to see if changing the sigmoid output range
# is important.
#
Ninputs 1  Noutputs 1
UnitType VARSIGMOID     OutputType VARSIGMOID
SigmoidMax1.0  SigmoidMin -1.0
# used to say UseCache 1 now use TRUE or FALSE
UseCache TRUE
NTrainingPatterns 10  NTestPatterns 0
#Change some parameters
WeightRange 1.1
Training
# instead of giving an explicit goal value I use the symbols 'max and'min
  1.0     max
 -1.0    min
# here is a zag
  2.0    min
 -2.0  p   max
# another zig
  3.0     max
 -3.0    min
  4.0    min
 -4.0     max
  5.0     max
 -5.0    min
Testing
----------------------- end of zigzag.net ------------------------------*/

Running the code with this example looks like this:
 
> % cascor
> Enter name of network: zigzag
> Number of epochs to train inputs: 50
> Number of epochs to train outputs: 50
> Maximum number of new units: 6
> Trials for this problem: 2
> Change some parameters? (y or n) n
>        ********* progress reports deleted *********
;;
;;   Hit Control-C to interrupt simulation 
;;
> ^CSimulation interrupted at epoch 215
> Type <parameter name> to inspect or change the current value 
>   of <parameter name>.
> Type 'go' to continue simulation.
> Type 'values' to inspect the current values of all parameters
> Enter Parameter: scorethreshold
> scorethreshold[0.3500]: 0.9
> Enter Parameter: go
>        ********* progress reports deleted *********
> Cascor.c Version:  1.14  Jul-18-90
> SigOff 0.10, WtRng 1.10, WtMul 1.00
> OMu 2.00, OEps 0.35, ODcy 0.00010, OPat 8, OChange 0.00
> IMu 2.00, IEps 1.00, IDcy 0.00000, IPat 8, IChange 0.03
> Utype: SIGMOID, Otype: SIGMOID, Pool 16
> Victory at 3 epochs, 2 units, 0 hidden, Error 2.6479.
> 
> 
> TRAINING LOOP STATS
> 
> Cascor.c Version:  1.14  Jul-18-90
> SigOff 0.10, WtRng 1.10, WtMul 1.00
> OMu 2.00, OEps 0.35, ODcy 0.00010, OPat 8, OChange 0.00
> IMu 2.00, IEps 1.00, IDcy 0.00000, IPat 8, IChange 0.03
> Utype: SIGMOID, Otype: SIGMOID, Pool 16
> 
>  Victories: 3, Defeats: 0, 
>    Training Epochs - Min: 3, Avg: 196,  Max: 306,
>    Hidden Units -    Min: 0, Avg:  2.7,  Max: 4,
> Do you want to run more trials? (y or n) n
> Do you want to test the last network? (y or n) n
> %
----------------------end of zigzag example -------------------------------

----------------------start of two spirals example ------------------------

----------------------start of two-spirals.c ------------------------------

/* generate the two spirals input set */
/* This code produces the input file necessary to run the two spirals       */
/* benchmark.  This benchmark was first proposed by Alexis Weiland of MITRE */
/* Corp.                                                                    */
#include <stdio.h>
#include <math.h>
#define M_PI	3.14159265358979323846

#define NUMPAIRS  96
#define TESTSHIFT 0.10	 /* fraction of the gap points are shifted for test */
main()
{
  float angle,radius,x,y;
  int i;
  FILE *foo;

  foo = fopen("twospirals.net","w");

  /* generate header stuff */
  fprintf(foo, "# Two Spirals benchmark with %d points on each spiral.\n#\n"
	  ,NUMPAIRS);
  fprintf(foo, "Ninputs %d  Noutputs %d\n", 2,1);
  fprintf(foo, "UnitType SIGMOID     OutputType SIGMOID\n");
  fprintf(foo, "UseCache TRUE\n");
  fprintf(foo, "NTrainingPatterns %d  NTestPatterns %d\n", 
	  (2 * NUMPAIRS), (2 * NUMPAIRS));
  fprintf(foo, 
	  "# set the Test parameter to TRUE so we can watch generalization\n");
  fprintf(foo, "Test TRUE\n");
  fprintf(foo, "Training\n");

  for(i=NUMPAIRS-1; i>=0; i--){
    angle = (float)i * M_PI / 16.0;
    radius = 6.5 * (float)(104 - i)/ 104.0;

    x = radius * sin(angle);
    y = radius * cos(angle);

    fprintf(foo, "%8.5f %8.5f     %3.1f\n",  x,  y,  0.5);
    fprintf(foo, "%8.5f %8.5f     %3.1f\n", -x, -y, -0.5);
  }
  fprintf(foo, "Testing\n");
  for(i=NUMPAIRS-1; i>=0; i--){
    angle = (float)(i+TESTSHIFT) * M_PI / 16.0;
    radius = 6.5 * (float)(104 - (i+TESTSHIFT))/ 104.0;

    x = radius * sin(angle);
    y = radius * cos(angle);

    fprintf(foo, "%8.5f %8.5f     %3.1f\n",  x,  y,  0.5);
    fprintf(foo, "%8.5f %8.5f     %3.1f\n", -x, -y, -0.5);
  }
 
  fclose(foo);
}

-------------------------end of two-spirals.c --------------------------------
-------------------------start of FTP instructions ------------------------
Tech reports describing these algorithms can also be obtained via FTP.
These are Postscript files, processed with the Unix compress/uncompress
program.

unix> ftp ftp.cs.cmu.edu (or 128.2.206.173)
Name: anonymous
Password: <your user id>
ftp> cd /afs/cs/project/connect/tr
ftp> binary
ftp> get filename.ps.Z
ftp> quit
unix> uncompress filename.ps.Z
unix> lpr filename.ps   (or however you print postscript files)

For "filename", sustitute the following:

qp-tr			Paper on Quickprop and other backprop speedups.
cascor-tr		Cascade-Correlation paper.
rcc-tr			Recurrent Cascade-Correlation paper.
precision		Hoehfeld-Fahlman paper on Cascade-Correlation with
			limited numerical precision.

-------------------------end of FTP instructions ------------------------

#endif				/* YOUWANTTOCOMPILEJUNK */

/* End of cascor1.c */
