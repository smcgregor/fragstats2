/********************************************************************
 *  File Name:  REGRESS.C
 *
 *  Purpose:  Returns the coefficients of the least square line 
 *	approximating the set of points passed.  (Returns a0, a1)
 *
 *		Y = a0 + a1X
 *
 *  Command Line Arguments:
 *	in_file:  file of X,Y pairs, 1 pair per record, separated
 *		  by a comma or a space(s)
 *
 *      out_file: file containing the coefficients a0, a1, for the
 *	          least square line
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1992
 *
 *  ID: $Id: regress.c,v 2.1 1994/03/30 19:48:48 marks Exp $
 *********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void main(argc,argv)
int   argc;
char  *argv[];
{

	FILE	*fp;
        char	string[90];
	int	num;
	double  x,y;
	double	sum_x,sum_y;
	double  sum_xy,sum_sq_x;
	double  a0,a1;
	double  val1,val2;
	double  delta = .900E-7;

	
	if (argc != 3) {
	   printf ("\nUsage: in_file out_file\n");
	   exit(-1);
	}


/*
 *  Open input file
 */
	if ((fp = fopen(argv[1],"r")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",argv[1]);
	   exit(-1);
	}


	num = 0;
	sum_x = sum_y = sum_xy = sum_sq_x = 0.0;

	while (!feof(fp)) {
	   fgets (string,80,fp);
	   if (feof(fp)) break;
	   x = atof (strtok(string," ,"));
	   y = atof (strtok('\0'," ,"));
	   sum_x += x;
	   sum_y += y;
	   sum_xy += (x * y);
	   sum_sq_x += (x * x);
	   num ++;
	}

	fclose (fp);

	if (num == 1) {
	   a0 = 0.0;
	   a1 = 0.0;
	}
	else {
           val1 = fabs((sum_y * sum_sq_x) - (sum_x * sum_xy));             
           val2 = fabs(((float)num * sum_sq_x) - (sum_x * sum_x));

           if (fabs(val1 - val2) < delta)
              a0 = 0.0;
           else
              a0 = val1 / val2;
               

           val1 = fabs(((float)num * sum_xy) - (sum_x * sum_y));
           val2 = fabs(((float)num * sum_sq_x) - (sum_x * sum_x));
 
           if (fabs(val1 - val2) < delta)
              a1 = 0.0;
           else
              a1 = val1 / val2;
        }      

/*
 *  Open the output file and write a0, a1 to it.
 */
	         
	if ((fp = fopen(argv[2],"w")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",argv[2]);
	   exit(-1);
	}
	fprintf (fp,"%f\n",a0);
        fprintf (fp,"%f\n",a1);
	fclose (fp);
}
