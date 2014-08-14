/*****************************************************************
 *  File Name:  WRTPATCH.C
 *
 *  Purpose:  Writes to the output files the patch level statistics
 *	for all patches in the current class.
 *
 *  Passed Parameters:
 *	class:      class value of this patch
 *	npatches:   number of patches in this class
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  3 December 1992
 *
 *  ID:  $Id: wrtpatch.c,v 2.1 1994/04/01 17:56:51 marks Exp $
 *
 *  Revision History:
 *	3-14-94 BJM:
 *	   Changing index name:
 *		From:  Landscape Similarity 
 *		To:    Percent of Landscape
 ******************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"

void write_patch (class,index,npatches)
short   class;
short	index;
int 	npatches;
{

	FILE	*out1,*out2;
	char	nndist[15],nndist2[15];
	char  	prox1[15],prox2[15];
	char	edge1[15],edge2[15];
	char	label[MAX_LABEL_LENGTH+1];
	int	i,j;
	short	fill;
        float   land_area;
	float	land_sim;



/*
 *  Calculate landscape similarity
 */
        land_area = (float)total_size * cellsize*cellsize / 10000.;
	land_sim = classarea / land_area * 100.;

/*
 *  Open output files
 */
	if ((out1=fopen(file4,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",file4);
	   exit(-1);
	}

	if ((out2=fopen(file1,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",file1);
	   exit(-1);
	}


	fprintf (out1,"\nPATCH INDICES");

/*
 *  Write out patch level statistics for each patch in this class
 */
	for (j=0; j < npatches; j++) {

	   fprintf (out1,"\n\nPatch ID:                 %9d",patchid[j]);
	   if (descriptors) {
	      get_char(name,index,MAX_LABEL_LENGTH,label);
/*
 *  The .full file is formatted for labels with a length <= 22.  If
 *  they are any longer than this, no attempt is made to align columns.
 */
	      fprintf (out1,"   Patch Type: ");
	      fill = 22 - strlen(label);
	      if (fill > 0) 
	         for (i=0; i < fill; i++) fprintf (out1," ");
	      fprintf (out1,"%s",label);
	   }
	   else
	      fprintf (out1,"   Patch Type:              %9d",class);
	
	   fprintf (out1,"\nArea (ha):            %13.3f",area[j]);
	   fprintf (out1,"   Landscape Similarity (%%):%9.3f",land_sim);
	   fprintf (out1,"\nPerimeter (m):        %13.3f",
	      perimeter[j]);

	   if (contrast_indices) {
	      sprintf (edge1,"%9.3f",edge_contrast[j]/perimeter[j] * 100.);
	      sprintf (edge2,"%9.2f",edge_contrast[j]/perimeter[j] * 100.);
	   }
	   else {
	      sprintf (edge1,"NA");
	      sprintf (edge2,".");
	   }

	   fprintf (out1,"   Edge Contrast (%%):       %9s",edge1);
	   fprintf (out1,"\nShape Index:              %9.3f",
	      shape[j]);
	   fprintf (out1,"   Fractal Dimension:       %9.3f",
   	      fractal[j]);
	   fprintf (out1,"\nCore Area (ha):          %10.3f",
	      core_area[j]);
	   fprintf (out1,"   Num Core Areas:          %9d",
	      core_area_patches[j]);
	   fprintf (out1,"\nCore Area Index (%%):      %9.3f",
	      core_area_index[j]);

	   if (do_nndist) {
	      if (near_neigh_dist[j] == 0.0) {
		 strcpy (nndist,"NONE");
		 strcpy (nndist2,".");
	      }
	      else {
	 	 sprintf(nndist,"%12.3f",near_neigh_dist[j]);
		 sprintf(nndist2,"%12.2f",near_neigh_dist[j]);
	      }
	   }
	   else {
	      strcpy (nndist,"NA");
	      strcpy (nndist2,".");
	   }
	   fprintf (out1,"   Near Neigh Dist (m):  %12s",nndist);

	   if (proximity) {
	      sprintf (prox1,"%12.3f",prox[j]);
	      sprintf (prox2,"%12.2f",prox[j]);
	   }
	   else {
	      sprintf (prox1,"NA");
	      sprintf (prox2,".");
	   }
	   fprintf (out1,"\nProximity Index:       %12s",prox1);
		

/*
 *  Write statistics to "dump" file
 */
	   if (strlen(imagename) > 12) {
              for (i=0; i < 12; i++) fprintf(out2,"%c",imagename[i]);
              fprintf(out2," ");
           }
           else
              fprintf(out2,"%12s ",imagename);

	   fprintf (out2,"%6d ",patchid[j]);
	   if (descriptors) {
/*
 *  Force the label to fill MAX_LABEL_LENGTH spaces.
 */
	      fill = MAX_LABEL_LENGTH - strlen(label);
	      for (i=0; i < fill; i++) fprintf (out2," ");
	      fprintf (out2,"%s ",label);
	   }
	   else 
	      fprintf (out2,"%10d ",class);
		
	   fprintf (out2,"%10.2f %6.2f %10.1f %9s ",
	      area[j],land_sim,perimeter[j],edge2);
	   fprintf (out2,"%5.2f %5.2f %10.2f %3d %6.2f ",
	      shape[j],fractal[j],core_area[j],core_area_patches[j],
	      core_area_index[j]);

	   fprintf (out2,"%12s %12s",nndist2,prox2);

	   fprintf (out2,"\n");
	}

	fclose (out1);
	fclose (out2);
}
