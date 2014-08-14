/************************************************************************
 *  File Name:  WRTCLASS.C
 *
 *  Purpose:  This program is called by the arc/info AML, fragstats,
 *	to write out class level landscape indices.  Arc/info's
 *	formatting capabilities are very limited.
 *
 *  Command Line Arguements:
 *	in_file:      name of file containing a dump of all the class
 *	              level indices, 1 per record.
 *	
 *      full_file:    name of the .full file to write the indices to
 *	
 *	class_file:   name of the .class file to write the indices to
 *
 *      init_flag:    if 1, write header to class file
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  June 1993
 *
 *  ID: $Id: wrtclass.c,v 2.1 1994/03/30 19:49:08 marks Exp $
 ************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main (argc,argv)
int   argc;
char  *argv[];
{
	FILE 	*in,*out1,*out2;
	char	infile[50];
	char    outfile1[50],outfile2[50];
	char	cvalue[20],cvalue2[20];
	char	string[20];
	int	ivalue;
	int	j,init;
	float	value;


	if (argc != 5) {
	   printf ("\nUSAGE: wrtclass in_file full_file class_file init\n");
	   exit(-1);
	}

	strcpy (infile,argv[1]);    /* input arc/info file         */
	strcpy (outfile1,argv[2]);  /* name for output .full file  */
	strcpy (outfile2,argv[3]);  /* name for output .class file */
	init = atoi(argv[4]);       /* initialize flag; if 1 write header*/

/*
 *  Open the input "dump" file containing all the values for the indices
 */
	if ((in=fopen(infile,"r")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",infile);
	   exit(-1);
	}

/*
 *  Open the output .full file (formatted) for appending
 */
	if ((out1=fopen (outfile1,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",outfile1);
	   exit (-1);
	}

/*
 *  Open the output .class file (all indices written out to one record)
 */
	if ((out2=fopen(outfile2,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",outfile2);
	   exit(-1);
	}


/*
 *  Write header to output .class file if init = true
 */
	if (init) {
           fprintf (out2,"      LID          TYPE          CA          TA");
           fprintf (out2,"  %%LAND    LPI    NP       PD         MPS");
           fprintf (out2,"     PSSD     PSCV");
           fprintf (out2,"           TE        ED");
           fprintf (out2,"      CWED   TECI   MECI AWMECI");
	   fprintf (out2,"   LSI   MSI AWMSI   DLFD  MPFD AWMPFD");
	   fprintf (out2," C%%LAND         TCA   NCA      ");
           fprintf (out2,"CAD        MCA1       CASD1    CACV1       ");
           fprintf (out2," MCA2       CASD2    CACV2   TCAI   MCAI");
           fprintf (out2,"      IJI");
           fprintf (out2,"\n");
	}


/*
 *  Output class level indices
 */
	fprintf (out1,"\nCLASS INDICES\n");

/*
 *  image name
 */
	fscanf (in,"%s",cvalue);
	if (strlen(cvalue) > 12) {
	   for (j=0; j < 12; j++) fprintf(out2,"%c",cvalue[j]);
	   fprintf(out2," ");
	}
	else
	   fprintf(out2,"%12s ",cvalue);

/*
 *  patch type
 */
	fscanf (in,"%s",cvalue);
	fprintf (out1,"\nPatch Type:              %10s",cvalue);
	fprintf (out2,"%10s ",cvalue);

/*
 *  class area
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Class Area (ha):      %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  total area
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nTotal Area (ha):       %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  landscape similarity
 *  3-28-94 BJM  Changing name to Percent of Landscape
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Percent of Landscape (%%):%9.3f",value);
	fprintf (out2,"%6.2f ",value);
		
/*
 *  largest patch index
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nLargest Patch Index (%%):  %9.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  number of patches
 */
	fscanf (in,"%d",&ivalue);
  	fprintf (out1,"   Number Patches:          %9d",ivalue);
	fprintf (out2,"%5d ",ivalue);

/*
 *  patch density (#patches/ha)
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nPatch Density (#/100 ha): %9.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean patch size 
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Mean Patch Size (ha): %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  patch size standard deviation 
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nPatch Size SD (ha):    %12.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  patch size coefficient of variation
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Patch Size CV (%%):    %12.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  total class edge
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nTotal Edge (m):      %14.3f",value);
	fprintf (out2,"%12.1f ",value);

/*
 *  class edge density
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Edge Den (m/ha):      %12.3f",value);
	fprintf (out2,"%9.2f ",value);

/*
 *  contrast-weighted class edge density
 */
       	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   strcpy(cvalue,"NA");
	   strcpy(cvalue2,".");
	}
	else {
           sprintf (cvalue,"%9.3f",atof(string));
           sprintf (cvalue2,"%9.2f",atof(string));
	}
	fprintf (out1,"\nCon-Wght Edge Den (m/ha): %9s",cvalue);
	fprintf (out2,"%9s ",cvalue2);

/*
 *  total class edge contrast index
 */
       	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   strcpy(cvalue,"NA");
	   strcpy(cvalue2,".");
	}
	else {
           sprintf (cvalue,"%9.3f",atof(string));
	   sprintf (cvalue2,"%6.2f",atof(string));
	}
	fprintf (out1,"   Total Edge Contrast (%%): %9s",cvalue);
	fprintf (out2,"%6s ",cvalue2);

/*
 *  mean class edge contrast index
 */
       	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   strcpy(cvalue,"NA");
	   strcpy(cvalue2,".");
	}
	else {
           sprintf (cvalue,"%9.3f",atof(string));
           sprintf (cvalue2,"%6.2f",atof(string));
	}
	fprintf (out1,"\nMean Edge Contrast (%%):   %9s",cvalue);
	fprintf (out2,"%6s ",cvalue2);

/*
 *  area-weighted mean class edge contrast index
 */
       	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   strcpy(cvalue,"NA");
	   strcpy(cvalue2,".");
	}
	else {
           sprintf (cvalue,"%8.3f",atof(string));
           sprintf (cvalue2,"%6.2f",atof(string));
	}
	fprintf (out1,"   Area-Wt Mean Edge Con (%%):%8s",cvalue);
	fprintf (out2,"%6s ",cvalue2);

/*
 *  landscape shape index
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nLandscape Shape Index:    %9.3f",value);
	fprintf (out2,"%5.2f ",value);
	
/*
 *  mean shape index 
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Mean Shape Index:        %9.3f",value);
	fprintf (out2,"%5.2f ",value);

/*
 *  area-weighted mean shape index
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nArea-Weighted Mean Shape: %9.3f",value);
	fprintf (out2,"%5.2f ",value);

/*
 *  double log fractal index
 */
       	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   strcpy(cvalue,"NA");
	   strcpy(cvalue2,".");
	}
	else {
           sprintf (cvalue,"%9.3f",atof(string));
           sprintf (cvalue2,"%6.2f",atof(string));
	}
	fprintf (out1,"   Double Log Fractal Index:%9s",cvalue);
	fprintf (out2,"%6s ",cvalue2);

/*
 *  mean patch fractal index
 */
	fscanf (in,"%f",&value);
       	if (value > 0.) {
           sprintf (cvalue,"%9.3f",value);
           sprintf (cvalue2,"%5.2f",value);
	}
	else {
	   strcpy(cvalue,"NA");
	   strcpy(cvalue2,".");
	}
	fprintf (out1,"\nMean Patch Fractal:       %9s",cvalue);
	fprintf (out2,"%5s ",cvalue2);

/*
 *  area-weighted mean patch fractal index
 */
	fscanf (in,"%f",&value);
       	if (value > 0.) {
           sprintf (cvalue,"%7.3f",value);
           sprintf (cvalue2,"%5.2f",value);
	}
	else {
	   strcpy(cvalue,"NA");
	   strcpy(cvalue2,".");
	}
	fprintf (out1,"   Area-Weighted Mean Fractal:%7s",cvalue);
	fprintf (out2,"%5s ",cvalue2);

/*
 *   landscape core area similarity
 *   3-28-94 BJM  Changing name to Core % of Landscape
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nCore %% of Landscape (%%):  %9.3f",value);
	fprintf (out2,"%7.2f ",value);

/*
 *  total core area
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Total Core Area (ha): %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  number of core area patches
 */
	fscanf (in,"%d",&ivalue);
	fprintf (out1,"\nNumber Core Areas:        %9d",ivalue);
	fprintf (out2,"%5d ",ivalue);

/*
 *  core area density
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Core Area Den (#/100 ha):%9.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean core area 1
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nMean Core Area 1 (ha): %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area standard deviation 1
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Core Area SD 1 (ha):  %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area coefficient of variation 1
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nCore Area CV 1 (%%):    %12.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean core area 2
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Mean Core Area 2 (ha):%12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area standard deviation 2
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nCore Area SD  2 (ha):  %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area coefficient of variation 2
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Core Area CV 2 (%%):   %12.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  total core area index
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nTotal Core Area Index (%%): %8.3f",value);
	fprintf (out2,"%6.2f ",value);

/* 
 *  mean core area index
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"   Mean Core Area Index (%%):%9.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  interspersion/juxtaposition index
 */
       	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   strcpy(cvalue,"NA");
	   strcpy(cvalue2,".");
	}
	else {
           sprintf (cvalue,"%8.3f",atof(string));
           sprintf (cvalue2,"%8.2f",atof(string));
	}
	fprintf (out1,"\nIntersper/Juxtapos (%%):    %8s",cvalue);
	fprintf (out2,"%8s ",cvalue2);


	fprintf (out1,"\n");
	fprintf (out2,"\n");
	fclose (in);
	fclose (out1);
	fclose (out2);
}

