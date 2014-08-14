/****************************************************************
 *  File Name:  LAND.C
 *
 *  Purpose:  Calculates and prints landscape level indices.
 *
 *  Passed Parameters:
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  4 December 1992
 *
 *  ID: $Id: land.c,v 2.4 1994/10/05 20:18:21 marks Exp marks $
 *****************************************************************/
#include <stdio.h>
#include <math.h>
#include "stats.h"


void landscape_statistics ()
{

	FILE	*out1,*out2;
	float	value,value2;
	float 	stdev;
	float	diversity;
	double  slope,intercept;
	double  sum,sumsq;
	double  dvalue;
	int	i,j;
	int	edgeval;


/*
 *  Set edge definitions 
 */

	true_edge = true_edge * cellsize;
	all_edge = all_edge * cellsize;	


/*
 *  Open output files
 */
	if ((out1=fopen(file4,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",file4);
	   exit(-1);
	}
	if ((out2=fopen(file3,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",file3);
	   exit(-1);
	}

/*
 *  LANDSCAPE INDICES
 */
	fprintf (out1,"\n\nLANDSCAPE INDICES");
	if (strlen(imagename) > 12) {
	   for(i=0; i < 12; i++) fprintf (out2,"%c",imagename[i]);
	   fprintf(out2," ");
	}
	else
	   fprintf(out2,"%12s ",imagename);


/*
 *  total landscape area
 */
	fprintf (out1,"\nTotal Area (ha):                  %13.3f",
	       	total_area);
	fprintf (out2,"%11.2f ",total_area);

/*
 *  largest patch index
 */
	value = total_maxarea / total_area * 100.;
	fprintf (out1,"\nLargest Patch Index(%%):              %10.3f",
	   value);
	fprintf (out2,"%6.2f ",value);

/*
 *  number of patches in the landscape
 */
	fprintf (out1,"\nNumber of patches:                   %10d", 
	   total_patches);
	fprintf (out2,"%5d ",total_patches);

/*
 *  patch density (#patches/100 ha)
 */
	value = (float)total_patches / (total_area / 100.);
	fprintf (out1,"\nPatch Density (#/100 ha):            %10.3f",
  	   value);		
	fprintf (out2,"%8.2f ",value);

/*
 *  mean patch size 
 */
	fprintf (out1,"\nMean Patch Size (ha):             %13.3f",
		total_area / (float)total_patches);
	fprintf (out2,"%11.2f ",total_area / (float)total_patches);

/*
 *  patch size standard deviation 
 */
	dvalue = (total_area_sq - ((total_area*total_area)
     	        /(float)total_patches)) / (float)(total_patches);
	if (dvalue > 0.0)
	   stdev = sqrt(dvalue);
	else
	   stdev = 0.0;

	fprintf (out1,"\nPatch Size Standard Dev (ha):      %12.3f",
	   stdev);
	fprintf (out2,"%11.2f ",stdev);

/*
 *  patch size coefficient of variation
 */
	if (stdev > 0.0) 
	   value = stdev / (total_area / (float)total_patches) * 100.;
	else
	   value = 0.0;

	fprintf (out1,"\nPatch Size Coeff of Variation (%%):   %10.3f",
	   value);
	fprintf (out2,"%10.2f ",value);
	   
/*
 *  total edge
 */
	fprintf (out1,"\nTotal Edge (m):                 %15.3f",
	   true_edge);
	fprintf (out2,"%12.1f ",true_edge);

/*
 *  edge density
 */
	value = true_edge / total_area;
	fprintf (out1,"\nEdge Density (m/ha):               %12.3f",
	   value);
	fprintf (out2,"%9.2f ",value);

/*
 *  contrast-weighted edge density
 */
	if (contrast_indices) {
	   value = (edcon * cellsize) / total_area;
           fprintf (out1,"\nContrast-Weight Edge Density (m/ha): %10.3f",    
              value);         
           fprintf (out2,"%9.2f ",value);
	}
	else {
           fprintf (out1,"\nContrast-Weight Edge Density (m/ha):         NA");
           fprintf (out2,"        . ");
	}

/*
 *  total edge contrast index
 */
	if (contrast_indices) {
           value = (edcon * cellsize) / all_edge * 100.;
           fprintf (out1,"\nTotal Edge Contrast Index (%%):       %10.3f",
	      value);
           fprintf (out2,"%6.2f ",value);
	}
	else {
           fprintf (out1,"\nTotal Edge Contrast Index (%%):               NA");
           fprintf (out2,"     . ");
        }

/*
 *  mean edge contrast index
 */
	if (contrast_indices) {
	   value = total_contrast / (float)total_patches * 100.;
           fprintf (out1,"\nMean Edge Contrast Index (%%):        %10.3f",
              value);
           fprintf (out2,"%6.2f ",value);
	}
	else {
           fprintf (out1,"\nMean Edge Contrast Index (%%):                NA");
           fprintf (out2,"     . ");
	}

/*
 *  area-weighted mean edge contrast index
 */
	if (contrast_indices) {
	   value = total_aw_contrast * 100.;
           fprintf (out1,"\nArea-Weighted Mean Edge Contrast (%%):  %8.3f",
	      value);
           fprintf (out2,"%6.2f ",value);
	}
	else {
           fprintf (out1,"\nArea-Weighted Mean Edge Contrast (%%):        NA");
           fprintf (out2,"     . ");
	}
 
/*
 *  landscape shape index
 */
	value =	(.25 * (all_edge/cellsize)) / sqrt((double)total_size);
	fprintf (out1,"\nLandscape Shape Index:               %10.3f",
	   value);
	fprintf (out2,"%6.2f ",value);
	
/*
 *  mean shape index 
 */
	value = total_shape / (float)total_patches;
	fprintf (out1,"\nMean Shape Index:                    %10.3f",
  	   value);		
	fprintf (out2,"%6.2f ",value);
	
/*
 *  area-weighted mean shape index
 */
	fprintf (out1,"\nArea-Weighted Mean Shape Index:      %10.3f",
		total_aw_shape);
	fprintf (out2,"%6.2f ",total_aw_shape);

/*
 *  double-log fractal index
 */
        if (total_patches > 1) {
	   regression(xx,yy,total_patches,&slope,&intercept);

	   if (slope != 0.0) {
	      fprintf (out1,"\nDouble Log Fractal Dimension:        %10.3f",
	 	 2.0 / slope);
	      fprintf (out2,"%5.2f ",2.0 / slope);
	   }
 	}
	if (total_patches == 1 || slope == 0.0) {
	   fprintf (out1,"\nDouble Log Fractal Dimension:                NA");
	   fprintf (out2,"     . ");
	}

/*
 *  mean patch fractal
 */
	value = total_fract / (float)total_patches;
	fprintf (out1,"\nMean Patch Fractal Dimension:        %10.3f",
	   value);
	fprintf (out2,"%5.2f ",value);

/*
 *  area-weighted mean patch fractal
 */
	fprintf (out1,"\nArea-Weighted Mean Fractal Dimension:%10.3f",
	   total_aw_fract);
	fprintf (out2,"%5.2f ",total_aw_fract);
	
/*
 *  total core area
 */
	fprintf (out1,"\nTotal Core Area (ha):             %13.3f",
	   total_core_area);
	fprintf (out2,"%11.2f ",total_core_area);

/*
 *  number of core area patches
 */
	fprintf (out1,"\nNumber of Core Areas:                %10d",
	   total_core_patches);
	fprintf (out2,"%5d ",total_core_patches);

/*
 *  core area density
 */
	if (total_core_patches > 0)
	   value = (float)total_core_patches / (total_area / 100.);
	else
	   value = 0.0;
	fprintf (out1,"\nCore Area Density (#/100 ha):      %12.3f",
	   value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean core area 1
 */
	if (total_core_area > 0.0) 
	   value = total_core_area / (float)total_patches;
	else
	   value = 0.0;
	fprintf (out1,"\nMean Core Area 1 (ha):            %13.3f",
	   value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area standard deviation 1
 */ 
	if (total_core_area > 0.0) {
	   dvalue = (total_core_sq-((total_core_area*total_core_area)
     	      /(float)total_patches)) / (float)(total_patches);
	   if (dvalue > 0.0) 
	      stdev = sqrt(dvalue);
	   else
	      stdev = 0.0;
	}
	else
	   stdev = 0.0;
	
	fprintf (out1,"\nCore Area Standard Dev 1 (ha):    %13.3f",
	   stdev);
	fprintf (out2,"%11.2f ",stdev);

/*
 *  core area coefficient of variation 1
 */
	if (total_core_area > 0.0)
	   value = stdev / (total_core_area / (float)total_patches) * 100.;
	else
	   value = 0.0;

	fprintf (out1,"\nCore Area Coeff of Variation 1 (%%):  %10.3f",
	   value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean core area 2
 */
	if (total_core_area > 0.0) 
	   value = total_core_area / (float)total_core_patches;
	else
	   value = 0.0;
	fprintf (out1,"\nMean Core Area 2 (ha):            %13.3f",
	   value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area standard deviation 2
 */ 
	if (total_core_area > 0.0) {
	   dvalue = (total_core_sq-((total_core_area*total_core_area)
     	      /(float)total_core_patches)) / (float)(total_core_patches);
	   if (dvalue > 0.0) 
	      stdev = sqrt(dvalue);
	   else
	      stdev = 0.0;
	}
	else
	   stdev = 0.0;
	
	fprintf (out1,"\nCore Area Standard Dev 2 (ha):    %13.3f",
	   stdev);
	fprintf (out2,"%11.2f ",stdev);

/*
 *  core area coefficient of variation 2
 */
	if (total_core_area > 0.0)
	   value = stdev / (total_core_area / (float)total_patches) * 100.;
	else
	   value = 0.0;

	fprintf (out1,"\nCore Area Coeff of Variation 2 (%%):  %10.3f",
	   value);
	fprintf (out2,"%8.2f ",value);

/*
 *  core area index 
 */
	if (total_core_area > 0.0)
	   value = total_core_area / total_area * 100.;
	else
	   value = 0.0;
	fprintf (out1,"\nTotal Core Area Index (%%):           %10.3f",
	   value);
	fprintf (out2,"%6.2f ",value);

/*
 *  mean core area index 
 */
	if (total_core_area > 0.0)
	   value = total_core_index / (float)total_patches * 100.;
	else
	   value = 0.0;
	fprintf (out1,"\nMean Core Area Index (%%):            %10.3f",
	   value);
	fprintf (out2,"%6.2f ",value);

/*
 *  mean nearest neighbor 
 */
	if (total_nndist == 0.0) {
	   fprintf (out1,"\nMean Nearest Neighbor (m):                 NONE");
	   fprintf (out2,"          . ");
	}
	else {
	   value = total_nndist / (float)total_nn;
  	   fprintf (out1,"\nMean Nearest Neighbor (m):        %13.3f",
 	      value);
	   fprintf (out2,"%11.1f ",value);
	}

/*
 *  nearest neighbor standard deviation
 */
	if (total_nndist == 0.0) {
	   fprintf (out1,"\nNearest Neighbor Standard Dev (m):           NA");
  	   fprintf (out2,"        . ");
	}
	else {
	   dvalue = (total_nndist_sq - ((total_nndist*total_nndist)
     	         /(float)total_nn)) / (float)(total_nn);
	   if (dvalue > 0.0) 
	      stdev = sqrt(dvalue);
	   else
	      stdev = 0.0;

	   fprintf (out1,"\nNearest Neighbor Standard Dev (m): %12.3f",
	      stdev);
	   fprintf (out2,"%9.2f ",stdev);
	}

/*
 *  nearest neighbor coefficient of variation
 */
	if (total_nndist == 0.0) {
	   fprintf (out1,"\nNearest Neigh Coeff of Variation (%%):        NA");
	   fprintf (out2,"       . ");
	}
	else {
	   if (stdev > 0.0) 
	      value = stdev / (total_nndist / (float)total_nn) * 100.;
	   else
	      value = 0.0;
	   fprintf (out1,"\nNearest Neigh Coeff of Variation (%%):%10.3f",
	      value);
   	   fprintf (out2,"%8.2f ",value);
	}

/*
 *  mean proximity index
 */
	if (proximity) {
	   value = total_prox / (float)total_patches;
	   fprintf (out1,"\nMean Proximity Index:             %13.3f",
	      value);
   	   fprintf (out2,"%12.2f ",value);
	}
	else {
	   fprintf (out1,"\nMean Proximity Index:                        NA");
	   fprintf (out2,"           . ");
	}

/*
 *  shannon diversity
 */
	sum = sumsq = value2 = 0.0;
	for (i=0; i < NUM_CLASSES; i++) {
	   	value = class_area[i] / total_area;
		sum += value * log((double)value);
		sumsq += value * value;
	}

	fprintf (out1,"\nShannon's Diversity Index:           %10.3f",-sum);
	fprintf (out2,"%6.2f ",-sum);

/*
 *  simpson diversity
 */
	diversity = 1.0 - sumsq;
	fprintf (out1,"\nSimpson's Diversity Index:           %10.3f",
		diversity);
	fprintf (out2,"%6.2f ",diversity);

/*
 *  modified simpson diversity
 */
	fprintf (out1,"\nModified Simpson's Diversity Index:  %10.3f",
	   -log(sumsq));
	fprintf (out2,"%6.2f ",-log(sumsq));

/*
 *  patch richness
 */
	fprintf (out1,"\nPatch Richness:                      %10d",
	 	NUM_CLASSES);
	fprintf (out2,"%4d ",NUM_CLASSES);

/*
 *  patch richness density
 */
	fprintf (out1,"\nPatch Richness Density (#/100 ha):   %10.3f",
		(float)NUM_CLASSES / (total_area/100.));
	fprintf (out2,"%8.2f ",(float)NUM_CLASSES / (total_area/100.));

/*
 *  relative patch richness
 */
	if (do_patchrich) {
           value = (float)NUM_CLASSES / (float)max_num_classes * 100.;
           fprintf (out1,"\nRelative Patch Richness (%%):         %10.3f",
	  	value);
           fprintf (out2,"%6.2f ",value);
	}
	else {
           fprintf (out1,"\nRelative Patch Richness (%%):                 NA");
           fprintf (out2,"     . ");
	}

/*
 *  shannon evenness
 */
	if (NUM_CLASSES == 1)
	   value = 0.0;
	else
	   value = -sum / log((double)NUM_CLASSES);

	fprintf (out1,"\nShannon's Evenness Index:            %10.3f",
	   value);
	fprintf (out2,"%6.2f ",value);

/*
 *  simpson evenness
 */
	if (NUM_CLASSES == 1)
	   value = 0.0;
	else
	   value = diversity / (1.0 - (1.0/(float)NUM_CLASSES));
	fprintf (out1,"\nSimpson's Evenness Index:            %10.3f",
	   value);
	fprintf (out2,"%6.2f ",value);

/*
 *  modified simpson evenness
 */
	if (NUM_CLASSES == 1)
	   value = 0.0;
	else
	   value = -log(sumsq) / -log(1.0 / (float)NUM_CLASSES);
	fprintf (out1,"\nModified Simpson's Evenness Index:   %10.3f",
	   value);
	fprintf (out2,"%6.2f ",value);

/*
 *  interspersion/juxtaposition index
 */
	sum = 0.0;
	for (i=0; i < num_wt; i++) {
	   for (j=0; j < num_wt; j++) {
	      edgeval = getint(edge,num_wt,i,j);
	      if (edgeval > 0) {
	         value = (float)edgeval / iji_edge;
	         sum += value * log(value);
	      }
	   }
	}


	if (total_num_classes <= 2) {
	   fprintf (out1,"\nInterspersion/Juxtaposition Index:           NA");
	   fprintf (out2,"      . ");
	}
	else {
	   value = log((double)((total_num_classes * (total_num_classes-1))
	      / 2.0));
  	   value2 = -sum / value * 100;
	   fprintf (out1,"\nInterspersion/Juxtaposition Index (%%):%9.3f",
	      value2);
	   fprintf (out2,"%7.2f ",value2);
	}

/*
 *  contagion index  -- based on Habin Lee's paper
 */
	if (total_num_classes > 1) {
	   value = raster_contagion();
	   fprintf (out1,"\nContagion (%%):                       %10.3f",
	      value*100.);
	   fprintf (out2,"%7.2f ",value*100.);
	}
	else {
	   fprintf (out1,"\nContagion (%%):                               NA");
	   fprintf (out2,"      . ");
	}

 
	fprintf (out1,"\n");
	fprintf (out2,"\n");

	fclose (out1);
	fclose (out2);
}
