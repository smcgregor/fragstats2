/******************************************************************
 *  Filename:  RDWEIGHT.C
 *
 *  Purpose:  Reads in a file of weights for each combination of
 *	patch type.  The file must be ascii and each record must
 *	contain numeric values for the two patch types and their
 *	assigned weight.  Commas or spaces can be used as delimiters.
 *	For example:
 *		1, 2, .44
 *		1, 3, .5
 *		1, 4, .66
 *		  etc.
 *
 *  Passed Parameters:
 *	filename: 	name of file containing weights
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1993
 *
 *  ID:  $Id: rdweight.c,v 2.2 1994/04/07 13:48:49 marks Exp marks $
 ******************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stats.h"

void read_weights (filename)
char *filename;
{

	FILE	*fp;
	int	n1,n2;
	float	weight;
	char	string[90];

        if ((fp = fopen(filename,"r")) == NULL) {
              printf ("\nERROR opening file: %s\n",filename); 
	      exit(-1);
        }

	printf ("\n\n... reading weight file: %s ...\n",filename);

/*
 *  Read the file once to find the min, max classes in the weight file.
 */
	min_wt = 999;
	max_wt = -999;
        while (!feof(fp)) {
	  	fgets(string,80,fp);
		if (feof(fp)) break;
	        n1 = atoi(strtok(string," ,"));
	        n2 = atoi(strtok('\0'," ,"));
		if (n1 < min_wt) min_wt = n1;
		if (n2 < min_wt) min_wt = n2;
	 	if (n1 > max_wt) max_wt = n1;
	        if (n2 > max_wt) max_wt = n2;
	}
	fclose (fp);

	if (min_class < min_wt || max_class > max_wt) {
	   printf ("\nERROR! The weight file does not contain the min");
	   printf ("\nand/or max class value in the landscape!!");
	   exit(-1);
	}


	num_wt = max_wt - min_wt + 1;

	contrast = (float *) calloc ((unsigned)num_wt*num_wt,sizeof(float));
	if (contrast == NULL) {
	   printf ("\nERROR! read_weight: can not allocate space for contrast!");
	   exit(-1);
	}
	
/*
 *  Read in weights to contrast array.
 */

        if ((fp = fopen(filename,"r")) == NULL) {
              printf ("\nERROR opening file: %s\n",filename); 
	      exit(-1);
        }

        while (!feof(fp)) {
	  	fgets(string,80,fp);
		if (feof(fp)) break;
	        n1 = atoi(strtok(string," ,"));
	        n2 = atoi(strtok('\0'," ,"));
	        weight = atof(strtok('\0'," ,"));
	
		n1 = n1 - min_wt;
		n2 = n2 - min_wt;
	        setfloat(contrast,num_wt,(short)n1,(short)n2,weight);
		setfloat(contrast,num_wt,(short)n2,(short)n1,weight);
	}

	fclose (fp);
}
