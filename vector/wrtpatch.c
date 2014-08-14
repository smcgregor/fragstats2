/******************************************************************
 *  File Name:  WRTPATCH.C
 *
 *  Purpose:  Called by the arc/info AML, fragstats, to write out
 *	patch level indices.  The formatting capabilities of
 *	arc/info are very limited.
 *
 *  Command Line Arguments:
 *	in_file:    file containing a dump of all the patch level 
 *	            indices, 1 per record
 *      full_file:  name of .full file to write the patch level
 *		    indices to
 *	patch_file: name of the .patch file to write the patch
 *	            level indices to
 *      init_flag:  if 1, write header to .patch file
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1993
 *
 *  ID: $Id: wrtpatch.c,v 2.1 1994/03/30 19:48:59 marks Exp $
 ******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main (argc,argv)
int   argc;
char  *argv[];
{
	FILE 	*in,*out1,*out2;
	char	infile[50];
	char    outfile1[50];
	char    outfile2[50];
	char	string[15];
	char	landscape_id[15];
	char	type[15];
	char	edge_contrast[15],edge2[15];
	char	fract[15],fract2[15];
	int	ncore_patches;
	int	init;
	int	id;
	float   area,perimeter,core_area;
	float   shape,fractal,corearea_index;
	float   similar;


	if (argc != 5) {
	   printf ("\nUSAGE: wrtpatch in_file full_file patch_file init\n");
	   exit(-1);
	}

	strcpy (infile,argv[1]);   /* arc/info input file  */
	strcpy (outfile1,argv[2]); /* name for output .full file */
        strcpy (outfile2,argv[3]); /* name for output .patch file */
	init = atoi(argv[4]);      /* initialize; if 1 write header*/


	if ((in = fopen (infile,"r")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",infile);
	   exit(-1);
	}

	if ((out1 = fopen (outfile1,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",outfile2);
	   exit(-1);
	}

	if ((out2 = fopen (outfile2,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",outfile2);
	   exit(-1);
	}


/*
 *  Read input file
 */
	fscanf (in,"%s",landscape_id);
	fscanf (in,"%d",&id);
	fscanf (in,"%s",type);
	fscanf (in,"%f",&area);
	fscanf (in,"%f",&similar);
	fscanf (in,"%f",&perimeter);
	fscanf (in,"%s",string);
	if (strcmp(string,"NA") != 0) {
	   sprintf (edge_contrast,"%9.3f",atof(string));
	   sprintf (edge2,"%9.2f",atof(string));
	}
	else
	   strcpy (edge_contrast,"NA");
	fscanf (in,"%f",&shape);
	fscanf (in,"%f",&fractal);
	if (fractal > 0) {
	   sprintf (fract,"%9.3f",fractal);
	   sprintf (fract2,"%9.2f",fractal);
	}
	else
	   strcpy (fract,"NA");
	fscanf (in,"%f",&core_area);
        fscanf (in,"%d",&ncore_patches);
	fscanf (in,"%f",&corearea_index);

/*
 *  Write to .full file  (formatted file)
 */
	fprintf (out1,"\nPatch ID:                 %9d",id);
        fprintf (out1,"   Patch Type:             %10s",type);
        fprintf (out1,"\nArea (ha):            %13.3f",area);
        fprintf (out1,"   Percent of Landscape (%%):%9.3f",similar);
        fprintf (out1,"\nPerimeter (m):        %13.3f",perimeter);
        fprintf (out1,"   Edge Contrast (%%):       %9s",edge_contrast);
        fprintf (out1,"\nShape Index:              %9.3f",shape);
        fprintf (out1,"   Fractal Dimension:       %9s",fract);
        fprintf (out1,"\nCore Area (ha):          %10.3f",core_area);
        fprintf (out1,"   Num Core Areas:          %9d",ncore_patches);
        fprintf (out1,"\nCore Area Index (%%):      %9.3f",
              corearea_index);
	fprintf (out1,"\n");
            
/*
 *  Write to .patch file  (dump file)
 */
	if (init) {
	   fprintf (out2,"       LID ");
	   fprintf (out2,"   PID       TYPE       AREA  %%LAND");
	   fprintf (out2,"      PERIM   EDGECON  SHAPE ");
    	   fprintf (out2," FRACT       CORE NCORE  CAI");
	   fprintf (out2,"\n");
	}

	fprintf (out2,"%10s",landscape_id);
	fprintf (out2," %6d %10s %10.2f %6.2f %10.1f",
	   id,type,area,similar,perimeter);

	if (strcmp(edge_contrast,"NA") != 0) 
	   fprintf (out2," %9s",edge2);
	else
	   fprintf (out2,"         .");

	fprintf (out2," %6.2f ",shape);

	if (strcmp(fract,"NA") != 0)
	   fprintf (out2," %9s",fract2);
	else
	   fprintf (out2,"         .");

        fprintf (out2," %10.2f %3d %6.2f",
	   core_area,ncore_patches,corearea_index);

	fprintf (out2,"\n");


	fclose (in);
	fclose (out1);
	fclose (out2);

}
