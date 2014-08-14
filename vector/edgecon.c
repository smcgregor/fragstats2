/*****************************************************************
 *  File Name:  EDGECON.C
 *
 *  Purpose:   This program is called by the arc/info aml, 
 *	fragstats, to calculate the sum of the lengths of all
 *	line segments in a polygon times their contrast weight.
 *
 *  Command Line Arguments:
 *	weight_file:  file containing weights for each combin-
 *	              of class types.  The format is as follows:
 *
 *                        class#, class#, weight
 *            		  (int)   (int)   (float)
 * 
 *			  delimiter can be a comma or space(s)
 *
 *		      For example:
 *			  1,2,.91
 *			  1,3,.87
 *			  2,3,.5   etc.
 *
 *	in_file:      file containing the values of the classes on 
 *		      the right and left side of a line segment and 
 *		      its length.  The format is as follows:
 *
 *			  class#, class#, length
 *			  (int)   (int)   (float)
 *
 *		          delimiter can be a comma or space(s)
 * 
 *		      For example:  
 *			  2,10,132.4
 *			  10,4,77.3     etc.
 *
 *	out_file:     file containing the sum of weight*length of
 *		      all line segments in the input file.
 *
 *	bound_weight: weight to apply to line segments bordering
 *		      the landscape boundary or background class
 *		      edges (where no weight is available).  These
 *		      segments are flagged with class numbers of
 *		      -999.
 *		       
 *  Programmer:  Barbara Marks
 *
 *  Date:  December, 1991
 *
 *  ID:  $Id: edgecon.c,v 2.1 1994/03/30 19:48:20 marks Exp $
 *
 *  Revision History:
 *	3-14-94 BJM
 *	  Adding a new calling parameter:  boundary weight.  The
 *	  user can now specify a proportion (weight) to use for
 *	  background and boundary segments.  [Before these segments
 *	  were either counted (weight=1), or not (weight=0)].
 *
 *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp;
	int	class1,class2;
	int	v1,v2;
	int	i;
	float	v3;
	float	len;
	float	sum;
	float 	weight[100][100];
	float	bound_weight;
	char	string[80];
	char	filename[60];
	char	in_file[60];
	char	out_file[60];

  

        if (argc != 5) {
	   printf ("\nUsage: edge_contrast weight_file in_file out_file bound_weight");
	   exit(-1);
	}

	strcpy (filename,argv[1]);
	strcpy (in_file,argv[2]);
	strcpy (out_file,argv[3]);
	bound_weight = atof(argv[4]);

/*
 *  Set up the array of weights so that the weight of class
 *  1 vs 1 is 0, 2 vs 2 is 0, etc.
 */
	for (i=0; i < 100; i++)  weight[i][i] = 0.0;

/*
 *  The name of the file containing the weights is passed
 *  as argument 1.  Open the file and read it.
 */
	fp = fopen (filename,"r");
	while (!feof(fp)) {
	        fgets (string,80,fp);
                if (feof(fp)) break;
		v1 = atoi (strtok(string," ,"));
		v2 = atoi (strtok('\0'," ,"));
		v3 = atof (strtok('\0'," ,"));
                weight[v1][v2] = v3;
                weight[v2][v1] = v3;
        }
        fclose (fp);

/*
 *  Open the data file.  This file is created by the
 *  arc/info aml fragstats.aml.  It contains:  class# of the
 *  polygon to the left of the line segment, class# of the
 *  polygon to the right of the line segment, and the length
 *  of the line segment.  Read in data for all lines that
 *  make up the polygon(s) of interest, summing the weight * 
 *  the length.
 */
	if ((fp = fopen(in_file,"r")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",in_file);
	   exit(-1);
	}

	sum = 0.0;
	while (!feof(fp)) {
		fgets (string,80,fp);	
		if (feof(fp)) break;
		class1 = atoi (strtok(string," ,"));
		class2 = atoi (strtok('\0'," ,"));
		len = atof (strtok('\0'," ,"));

		if (class1 == -999 && class2 == -999) 
		   sum += (len * bound_weight);
                else
		   sum += (weight[class1][class2] * len);
	}
	fclose (fp);

/*
 *  Write the sum of the weights * length to the output file,
 *  edgecon.dat
 */

	if ((fp = fopen (out_file,"w")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",out_file);
	   exit(-1);
	}

	fprintf (fp,"%f\n",sum);
	fclose (fp);

	return (1);

}
