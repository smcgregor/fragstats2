/**********************************************************************
 *  File Name:  WRTLAND.C
 *
 *  Purpose:  Called by the arc/info AML, fragstats, to write the
 *	landscape level indices to the .full and .land files.
 *	Arc/info's formatting capabilities are very limited.
 *
 *  Command Line Arguments:
 *	in_file:   file containing a dump of all the landscape level
 *	   	   indices, 1 per record
 *	full_file: name of .full file to write the landscape level
 *		   indices to
 * 	land_file: name of .land file to write the landscape level
 *	     	   indices to
 *	init_flag: if 1, write header to .land file
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1993
 *
 *  ID: $Id: wrtland.c,v 2.2 1994/04/19 14:32:51 marks Exp marks $
 ***********************************************************************/
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
	char	string[15];
	char	cvalue[20],cvalue2[20];
	int	i,init;
	int	ivalue;
	float   value;



	if (argc != 5) {
	   printf ("\nUSAGE: wrtland infile full_file1 land_file2 init\n");
	   exit(-1);
	}


	strcpy (infile,argv[1]);    /* input arc/info file        */
	strcpy (outfile1,argv[2]);  /* name for output .full file */
	strcpy (outfile2,argv[3]);  /* name for output .land file */
	init = atoi(argv[4]);       /* initialize flag; if 1 write header*/



/*
 *  Open input file containing the value of all the indices
 */
	if ((in=fopen(infile,"r")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",infile);
	   exit(-1);
	}

/*
 *  Open output .full file (formatted file)
 */
	if ((out1=fopen(outfile1,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",outfile1);
	   exit(-1);
	}

/*
 *  Open output .land file (all indices written to one record)
 */
	if ((out2 = fopen (outfile2,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",outfile2);
	   exit(-1);
	}

/*
 *  Write header to .land file if init = true
 */
	if (init) {
           fprintf (out2,"        LID           TA    LPI    NP       PD");
           fprintf (out2,"         MPS        PSSD       PSCV");
           fprintf (out2,"           TE");
           fprintf (out2,"        ED      CWED   TECI   MECI AWMECI");
           fprintf (out2,"    LSI    MSI  AWMSI  DLFD  MPFD AWMPFD");
           fprintf (out2,"        TCA   NCA      CAD");
	   fprintf (out2,"        MCA1       CASD1    CACV1");
           fprintf (out2,"        MCA2       CASD2    CACV2");
           fprintf (out2,"   TCAI   MCAI");
           fprintf (out2,"   SHDI   SIDI  MSIDI");
           fprintf (out2,"    PR      PRD    RPR");
	   fprintf (out2,"   SHEI   SIEI  MSIEI");
           fprintf (out2,"     IJI");
           fprintf (out2,"\n");
	}


/*
 *  Write landscape level indices
 */
	fprintf (out1,"\n\nLANDSCAPE INDICES");
	fscanf (in,"%s",cvalue);
	if (strlen(cvalue) > 12) {
	   for(i=0; i < 12; i++) fprintf (out2,"%c",cvalue[i]);
	   fprintf(out2," ");
	}
	else
	   fprintf(out2,"%12s ",cvalue);


/*
 *  total landscape area
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nTotal Area (ha):                  %13.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  largest patch index
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nLargest Patch Index(%%):              %10.3f",value);
	fprintf (out2,"%6.2f ",value);
	   
/*
 *  number of patches in the landscape
 */
	fscanf (in,"%d",&ivalue);
	fprintf (out1,"\nNumber of patches:                   %10d",ivalue);
	fprintf (out2,"%5d ",ivalue);

/*
 *  patch density (#patches/100 ha)
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nPatch Density (#/100 ha):            %10.3f",
 	   value);	
	fprintf (out2,"%8.2f ",value);

/*
 *  mean patch size 
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nMean Patch Size (ha):             %13.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  patch size standard deviation 
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nPatch Size Standard Dev (ha):      %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  patch size coefficient of variation
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nPatch Size Coeff of Variation (%%):   %10.3f",value);
	fprintf (out2,"%10.2f ",value);

/*
 *  total edge
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nTotal Edge (m):                 %15.3f",value);
	fprintf (out2,"%12.1f ",value);

/*
 *  edge density
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nEdge Density (m/ha):               %12.3f",value);
	fprintf (out2,"%9.2f ",value);

/*
 *  contrast-weighted edge density
 */
	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   sprintf (cvalue,"NA");
	   sprintf (cvalue2,".");
	}
	else {
	   sprintf (cvalue,"%10.3f",atof(string));
	   sprintf (cvalue2,"%9.2f",atof(string));
	}
        fprintf (out1,"\nContrast-Weight Edge Density (m/ha): %10s",cvalue); 
        fprintf (out2,"%9s ",cvalue2);

/*
 *  total edge contrast index
 */
	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   sprintf (cvalue,"NA");
	   sprintf (cvalue2,".");
	}
	else {
	   sprintf (cvalue,"%10.3f",atof(string));
	   sprintf (cvalue2,"%6.2f",atof(string));
	}
        fprintf (out1,"\nTotal Edge Contrast Index (%%):       %10s",cvalue);
        fprintf (out2,"%6s ",cvalue2);

/*
 *  mean edge contrast index
 */
	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   sprintf (cvalue,"NA");
	   sprintf (cvalue2,".");
	}
	else {
	   sprintf (cvalue,"%10.3f",atof(string));
	   sprintf (cvalue2,"%6.2f",atof(string));
	}
        fprintf (out1,"\nMean Edge Contrast Index (%%):        %10s",cvalue);
        fprintf (out2,"%6s ",cvalue2);

/*
 *  area-weighted mean edge contrast index
 */
	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   sprintf (cvalue,"NA");
	   sprintf (cvalue2,".");
	}
	else {
	   sprintf (cvalue,"%7.3f",atof(string));
	   sprintf (cvalue2,"%6.2f",atof(string));
	}
        fprintf (out1,"\nArea-Weighted Mean Edge Contrast (%%):   %7s",
	   cvalue);
        fprintf (out2,"%6s ",cvalue2);

/*
 *  landscape shape index
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nLandscape Shape Index:               %10.3f",value);
	fprintf (out2,"%6.2f ",value);
	
/*
 *  mean shape index 
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nMean Shape Index:                    %10.3f",value);
	fprintf (out2,"%6.2f ",value);
	
/*
 *  area-weighted mean shape index
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nArea-Weighted Mean Shape Index:      %10.3f",value);
	fprintf (out2,"%6.2f ",value);
	
/*
 *  double-log fractal index
 */
	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   sprintf (cvalue,"NA");
	   sprintf (cvalue2,".");
	}
	else {
	   sprintf (cvalue,"%10.3f",atof(string));
	   sprintf (cvalue2,"%5.2f",atof(string));
	}
	fprintf (out1,"\nDouble Log Fractal Dimension:        %10s",cvalue);
	fprintf (out2,"%5s ",cvalue2);

/*
 *  mean patch fractal
 */
	fscanf (in,"%f",&value);
	if (value > 0.) {
	   sprintf (cvalue,"%10.3f",value);
	   sprintf (cvalue2,"%5.2f",value);
   	}
	else {
	   sprintf (cvalue,"NA");
	   sprintf (cvalue2,".");
	}
	fprintf (out1,"\nMean Patch Fractal Dimension:        %10s",cvalue);
	fprintf (out2,"%5s ",cvalue2);

/*
 *  area-weighted mean patch fractal
 */
	fscanf (in,"%f",&value);
	if (value > 0.) {
	   sprintf (cvalue,"%10.3f",value);
	   sprintf (cvalue2,"%5.2f",value);
   	}
	else {
	   sprintf (cvalue,"NA");
	   sprintf (cvalue2,".");
	}
	fprintf (out1,"\nArea-Weighted Mean Fractal Dimension:%10s",cvalue);
	fprintf (out2,"%5s ",cvalue2);

/*
 *  total core area
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nTotal Core Area (ha):             %13.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  number of core area patches
 */
	fscanf (in,"%d",&ivalue);
	fprintf (out1,"\nNumber of Core Areas:                %10d",ivalue);
	fprintf (out2,"%5d ",ivalue);

/*
 *  core area density
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nCore Area Density (#/100 ha):      %12.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean core area 1
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nMean Core Area 1 (ha):            %13.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area standard deviation 1
 */ 
	fscanf (in,"%f",&value);
	fprintf (out1,"\nCore Area Standard Dev 1 (ha):    %13.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area coefficient of variation 1
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nCore Area Coeff of Variation 1 (%%):  %10.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean core area 2
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nMean Core Area 2 (ha):            %13.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area standard deviation 2
 */ 
	fscanf (in,"%f",&value);
	fprintf (out1,"\nCore Area Standard Dev 2 (ha):    %13.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area coefficient of variation 2
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nCore Area Coeff of Variation 2 (%%):  %10.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  core area index 
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nTotal Core Area Index (%%):           %10.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  mean core area index 
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nMean Core Area Index (%%):            %10.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  shannon diversity
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nShannon's Diversity Index:           %10.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  simpson diversity
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nSimpson's Diversity Index:           %10.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  modified simpson diversity
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nModified Simpson's Diversity Index:  %10.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  patch richness
 */
	fscanf (in,"%d",&ivalue);
	fprintf (out1,"\nPatch Richness:                      %10d",ivalue);
	fprintf (out2,"%5d ",ivalue);

/*
 *  patch richness density
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nPatch Richness Density (#/100 ha):   %10.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  relative patch richness
 */
	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   sprintf (cvalue,"NA");
	   sprintf (cvalue2,".");
	}
	else {
	   sprintf (cvalue,"%10.3f",atof(string));
	   sprintf (cvalue2,"%6.2f",atof(string));
	}
        fprintf (out1,"\nRelative Patch Richness (%%):         %10s",cvalue);
        fprintf (out2,"%6s ",cvalue2);

/*
 *  shannon evenness
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nShannon's Evenness Index:            %10.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  simpson evenness
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nSimpson's Evenness Index:            %10.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  modified simpson evenness
 */
	fscanf (in,"%f",&value);
	fprintf (out1,"\nModified Simpson's Evenness Index:   %10.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  interspersion/juxtaposition index
 */
	fscanf (in,"%s",string);
	if (strcmp(string,"NA") == 0) {
	   sprintf (cvalue,"NA");
	   sprintf (cvalue2,".");
	}
	else {
	   sprintf (cvalue,"%8.3f",atof(string));
	   sprintf (cvalue2,"%7.2f",atof(string));
	}
	fprintf (out1,"\nInterspersion/Juxtaposition Index (%%): %8s",cvalue);
	fprintf (out2,"%7s ",cvalue2);


 
	fprintf (out1,"\n");
	fprintf (out2,"\n");

	fclose (in);
	fclose (out1);
	fclose (out2);
}
