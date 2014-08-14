/****************************************************************
 *  File Name:  CLASS.C
 *
 *  Purpose:  Calculate and print class level indices.
 *
 *  Passed Parameters:
 *	class:  code of the current class (patch type)
 *      index:  index into arrays of this class
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  3 December 1992
 *  
 *  ID:  $Id: class.c,v 2.1 1994/04/01 17:44:25 marks Exp $
 *****************************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "stats.h"


void class_statistics(class,index)
short	class;
short	index;
{
	FILE	*out1,*out2;
	double  sum_area_sq;
	double  sum_core_area;
	double  sum_core_sq;
	double  dvalue;
	double  sum_nndist,sum_nndist_sq;
	double	*x,*y;
	double	*ptrx,*ptry;
	double  slope,intercept;
	float	value;
	float	max_patch;
	float	stdev;
	float 	sum_shape,sum_aw_shape;
	float	sum_fract;
	float   sum_core_area_index;
	float	sum_contrast_edge;
	float 	sum_aw_contrast,sum_contrast;
	float   sum_aw_fract;
	float	sum_iji;
	float	land_area;
	float	cai;
	float	class_ratio,land_ratio;
	float	sum_prox;
	float   land_edge;
	int	j,num_coreareas;
	int	edgeval;
	short	i,nn;
	short	fill;
	char	label[MAX_LABEL_LENGTH+1];

	static int counter = 0;



/*
 *  Allocate space for two arrays that will hold log(area) and 
 *  log(perimeter) of each patch in this class.  These arrays will
 *  be passed to the regression routine for the calculation of the 
 *  double log fractal index.
 */
	x = (double *) calloc ((unsigned)num_patches,sizeof(double));
	if (x == NULL) {
	   printf ("\nERROR! class_statistics: Can not allocate space for x!");
	   exit(-1);
	}
	y = (double *) calloc ((unsigned)num_patches,sizeof(double));
	if (y == NULL) {
	   printf ("\nERROR! class_statistics: Can not allocate space for y!");
	   exit(-1);
	}
	ptrx = x;
	ptry = y;


/*
 *  Convert total number of cells in the landscape to hectares.
 */
	land_area = total_size * cellsize*cellsize / 10000.;

/*
 *  Initialize class level variables.
 */
	sum_area_sq = 0.0;
	sum_shape = 0.0;
	sum_aw_shape = 0.0;
	sum_fract = 0.0;
	sum_core_area = 0.0;
	sum_core_area_index = 0.0;
	sum_core_sq = 0.0;
	sum_contrast = 0.0;
	sum_contrast_edge = 0.0;
	sum_aw_contrast = 0.0;
	sum_aw_fract = 0.0;
	sum_nndist = 0.0;
	sum_nndist_sq = 0.0;
	sum_iji = 0.0;
	sum_prox = 0.0;
	max_patch = -9999.0;
	num_coreareas = 0;


	land_edge = (float)boundary + lshape_edge;

/*
 *  For every patch in class this class, sum up various parameters.
 */ 

  	for (j=0; j < num_patches; j++) {
	   class_ratio = area[j] / classarea;
	   land_ratio = area[j] / land_area;

	   if (area[j] > max_patch) max_patch = area[j];
	   sum_area_sq += area[j] * area[j];
	   sum_shape += shape[j];
	   sum_aw_shape += shape[j] * class_ratio;
	   sum_fract += fractal[j];
	   sum_aw_fract += fractal[j] * class_ratio;
	   sum_core_area += core_area[j];
	   sum_core_sq += core_area[j] * core_area[j];
 	   cai = core_area[j] / area[j];
	   sum_core_area_index += cai;
	   if (do_nndist) {
	       sum_nndist += near_neigh_dist[j];
	       sum_nndist_sq += near_neigh_dist[j] * near_neigh_dist[j];
	       if (near_neigh_dist[j] > 0.0) total_nn ++;
	   }
	   if (proximity) sum_prox += prox[j];

	   num_coreareas += core_area_patches[j];

	   total_aw_shape += shape[j] * land_ratio;
	   total_aw_fract += fractal[j] * land_ratio;
	
/*
 *  For the class and landscape level "double log fractal index",
 *  store the logs of area and perimeter.
 */
	   *x = log(perimeter[j]);
	   *y = log(area[j]);
	   xx[counter] = *x;
	   yy[counter] = *y;
	   x ++;
	   y ++;
	   counter ++;

	   if (contrast_indices) {
	      sum_contrast += edge_contrast[j] / perimeter[j];
              sum_aw_contrast += (edge_contrast[j] / perimeter[j]) *
                       (area[j] / classarea);
	      total_aw_contrast += (edge_contrast[j] / perimeter[j])
		       * (area[j] / land_area);
              sum_contrast_edge += edge_contrast[j];
	  }
	}

/*
 *  Calculate class level interspersion/juxtaposition.
 */
   	for (nn=0; nn < num_wt; nn++) {
	   edgeval = getint(edge,num_wt,(short)class-min_wt,nn);
	   if (edgeval > 0) {
	      value = (float)edgeval / iji_edge;
   	      sum_iji += value * log(value);
	   }
	}

/* 
 *  Sum up class level statistics for landscape level.
 */
	class_area[index] = classarea;
	total_area += classarea ;
	total_area_sq += sum_area_sq;
	total_shape += sum_shape;
	total_fract += sum_fract;
	total_prox += sum_prox;
	total_core_area += sum_core_area;
	total_core_patches += num_coreareas;
	total_core_index += sum_core_area_index;
	total_core_sq += sum_core_sq;
	total_contrast += sum_contrast;
	total_contrast_edge += sum_contrast_edge;
	total_nndist += sum_nndist;
	total_nndist_sq += sum_nndist_sq;
	if (total_maxarea < max_patch) total_maxarea = max_patch;

/*
 *  Add in the extra edge contrast (if any) if this class exists
 *  in the landscape border.
 */
	sum_contrast_edge += extra_contrast;

/*
 *  Return if the user does not want to write out class statistics.
 */
	if (!class_stats) {
	   cfree(ptrx);
	   cfree(ptry);
	   return;
	}

/*
 *  Open output files (.full and .class file)
 */
	if ((out1=fopen(file4,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",file4);
	   exit(-1);
	}

	if ((out2=fopen(file2,"a")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",file2);
	   exit(-1);
	}


/*
 *  OUTPUT CLASS LEVEL INDICES
 */
	fprintf (out1,"\n\nCLASS INDICES\n");

	if (strlen(imagename) > 12) {
	   for (j=0; j < 12; j++) fprintf(out2,"%c",imagename[j]);
	   fprintf(out2," ");
	}
	else
	   fprintf(out2,"%12s ",imagename);

	if (descriptors) {
	   fprintf (out1,"\nPatch Type:  ");
	   get_char(name,index,MAX_LABEL_LENGTH,label); 
	   fill = 22 - strlen(label);
	   for (i=0; i < fill; i++) fprintf (out1," ");
	   fprintf (out1,"%s",label);
	   fill = MAX_LABEL_LENGTH - strlen(label);
	   for (i=0; i < fill; i++) fprintf (out2," ");
	   fprintf (out2,"%s ",label);
  	}
	else {
	   fprintf (out1,"\nPatch Type:              %10d",
	      class);
	   fprintf (out2,"%10d ",class);
	}

/*
 *  class area
 */
	fprintf (out1,"   Class Area (ha):      %12.3f",classarea);
	fprintf (out2,"%11.2f ",classarea);

/*
 *  total area
 */
	fprintf (out1,"\nTotal Area (ha):       %12.3f",land_area);
	fprintf (out2,"%11.2f ",land_area);

/*
 *  landscape similarity
 *  3-14-94 BJM  Changing name to Percent of Landscape
 */
	value = classarea / land_area * 100.;
	fprintf (out1,"   Percent of Landscape (%%):%9.3f",value);
	fprintf (out2,"%6.2f ",value);
		
/*
 *  largest patch index
 */
	value = max_patch / land_area * 100.;
	fprintf (out1,"\nLargest Patch Index (%%):  %9.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  number of patches
 */
  	fprintf (out1,"   Number Patches:          %9d",num_patches);
	fprintf (out2,"%5d ",num_patches);

/*
 *  patch density (#patches/ha)
 */
	value = (float)num_patches / land_area * 100.;
	fprintf (out1,"\nPatch Density (#/100 ha): %9.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean patch size 
 */
	value = classarea / (float)num_patches;
	fprintf (out1,"   Mean Patch Size (ha): %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  patch size standard deviation 
 */
	dvalue = (sum_area_sq - ((classarea*classarea) /
     	   (float)num_patches)) / (float)(num_patches);
	if (dvalue > 0.0)
	   stdev = sqrt(dvalue);
	else
 	   stdev = 0.0;

	fprintf (out1,"\nPatch Size SD (ha):    %12.3f",stdev);
	fprintf (out2,"%8.2f ",stdev);

/*
 *  patch size coefficient of variation
 */
	if (stdev > 0.0)
	   value = stdev / (classarea / (float)num_patches) * 100.;
	else
	   value = 0.0;
		
	fprintf (out1,"   Patch Size CV (%%):       %9.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  total class edge
 */
	value = true_edge * cellsize;
	fprintf (out1,"\nTotal Edge (m):      %14.3f",value);
	fprintf (out2,"%12.1f ",value);

/*
 *  class edge density
 */
	value = (true_edge * cellsize) / land_area;
	fprintf (out1,"   Edge Den (m/ha):      %12.3f",value);
	fprintf (out2,"%9.2f ",value);

/*
 *  contrast-weighted class edge density
 */
	if (contrast_indices) {
	   value = sum_contrast_edge / land_area;
           fprintf (out1,"\nCon-Wght Edge Den (m/ha): %9.3f",value);
           fprintf (out2,"%9.2f ",value);
	}
	else {
           fprintf (out1,"\nCon-Wght Edge Den (m/ha):        NA");
           fprintf (out2,"        . ");
	}

/*
 *  total class edge contrast index
 */
	if (contrast_indices) {
	   value = sum_contrast_edge / (all_edge * cellsize) 
	   	   * 100;
	   fprintf (out1,"   Total Edge Contrast (%%): %9.3f",value);
	   fprintf (out2,"%6.2f ",value);
	}
	else {
	   fprintf (out1,"   Total Edge Contrast (%%):        NA");
	   fprintf (out2,"     . ");
	}

/*
 *  mean class edge contrast index
 */
	if (contrast_indices) {
	   value = sum_contrast / (float)num_patches * 100.;
           fprintf (out1,"\nMean Edge Contrast (%%):   %9.3f",value);
           fprintf (out2,"%6.2f ",value);
	}
	else {
           fprintf (out1,"\nMean Edge Contrast (%%):          NA");
           fprintf (out2,"     . ");
	}

/*
 *  area-weighted mean class edge contrast index
 */
	if (contrast_indices) {
	   value = sum_aw_contrast * 100.;
           fprintf (out1,"   Area-Wt Mean Edge Con(%%):%9.3f",value);
           fprintf (out2,"%6.2f ",value);
	}
	else {
           fprintf (out1,"   Area-Wt Mean Edge Con(%%):       NA");
           fprintf (out2,"     . ");
	}
        
/*
 *  landscape shape index
 */
	value = (.25 * land_edge) / sqrt((double)total_size);
	fprintf (out1,"\nLandscape Shape Index:    %9.3f",value);
	fprintf (out2,"%5.2f ",value);
	
/*
 *  mean shape index 
 */
	value = sum_shape / (float)num_patches;
	fprintf (out1,"   Mean Shape Index:        %9.3f",value);
	fprintf (out2,"%5.2f ",value);

/*
 *  area weighted mean shape index
 */
	fprintf (out1,"\nArea-Weighted Mean Shape: %9.3f",sum_aw_shape);
	fprintf (out2,"%5.2f ",sum_aw_shape);

/*
 *  double log fractal index
 */
	if (num_patches > 1) {
	   x = ptrx;
	   y = ptry;
	   regression(x,y,num_patches,&slope,&intercept);

	   if (slope != 0.0) {
	      value = 2.0 / slope;
	      fprintf (out1,"   Double Log Fractal:      %9.3f",value);
	      fprintf (out2,"%5.2f ",value);
	   }
	}
	if (num_patches == 1 || slope == 0.0) {
	   fprintf (out1,"   Double Log Fractal Index:       NA");
	   fprintf (out2,"    . ");
	}

/*
 *  mean patch fractal index
 */
	value = sum_fract / (float)num_patches;
	fprintf (out1,"\nMean Patch Fractal:       %9.3f",value);
	fprintf (out2,"%5.2f ",value);

/*
 *  area-weighted mean patch fractal index
 */
	fprintf (out1,"   Area-Weighted Mean Fractal:%7.3f",sum_aw_fract);
	fprintf (out2,"%5.2f ",sum_aw_fract);

/*
 *  landscape core area similarity
 *  3-14-94 BJM  Changing name to Core % of Landscape 
 */
	if (sum_core_area > 0.0)
	   value = sum_core_area / land_area * 100.0;
	else
	   value = 0.0;
	fprintf (out1,"\nCore %% of Landscape (%%):  %9.3f",value);
	fprintf (out2,"%7.2f ",value);

/*
 *  total core area
 */
	fprintf (out1,"   Total Core Area (ha): %12.3f",sum_core_area);
	fprintf (out2,"%11.2f ",sum_core_area);

/*
 *  number of core area patches
 */
	fprintf (out1,"\nNumber Core Areas:        %9d",num_coreareas);
	fprintf (out2,"%5d ",num_coreareas);

/*
 *  core area density
 */
	if (num_coreareas > 0) 
	   value = (float)num_coreareas / land_area * 100.;
	else
	   value = 0.0;
	fprintf (out1,"   Core Area Den (#/100 ha):%9.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean core area 1
 */
	if (sum_core_area > 0.0)
	   value = sum_core_area / (float)num_patches;
	else
	   value = 0.0;

	fprintf (out1,"\nMean Core Area 1 (ha): %12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area standard deviation 1
 */
	if (sum_core_area > 0.0) {
	   dvalue = (sum_core_sq -((sum_core_area*sum_core_area) /
	       (float)num_patches)) / (float)(num_patches);
	   if (dvalue > 0.0)
	       stdev = sqrt(dvalue);
	   else
	       stdev = 0.0;
	}
	else
	   stdev = 0.0;
	
	fprintf (out1,"   Core Area SD 1 (ha):  %12.3f",stdev);
	fprintf (out2,"%11.2f ",stdev);

/*
 *  core area coefficient of variation 1
 */
	if (sum_core_area > 0.0) 
	   value = stdev / (sum_core_area/(float)num_patches) * 100.;
	else
	   value = 0.0;

	fprintf (out1,"\nCore Area CV 1 (%%):       %9.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  mean core area 2
 */
	if (sum_core_area > 0.0)
	   value = sum_core_area / num_coreareas;
	else 
	   value = 0.0;

	fprintf (out1,"   Mean Core Area 2 (ha):%12.3f",value);
	fprintf (out2,"%11.2f ",value);

/*
 *  core area standard deviation 2
 */
	if (sum_core_area > 0.0) {
	   dvalue = (sum_core_sq -((sum_core_area*sum_core_area) /
	       (float)num_coreareas)) / (float)num_coreareas;
	   if (dvalue > 0.0)
	       stdev = sqrt(dvalue);
	   else
	       stdev = 0.0;
	}
	else
	   stdev = 0.0;
	
	fprintf (out1,"\nCore Area SD 2 (ha):   %12.3f",stdev);
	fprintf (out2,"%11.2f ",stdev);

/*
 *  core area coefficient of variation 2
 */
	if (sum_core_area > 0.0) 
	   value = stdev / (sum_core_area/(float)num_coreareas) * 100.;
	else
	   value = 0.0;

	fprintf (out1,"   Core Area CV 2 (%%):      %9.3f",value);
	fprintf (out2,"%8.2f ",value);

/*
 *  total core area index
 */
	if (sum_core_area > 0.0)
	   value = sum_core_area / classarea * 100.0;
	else
	   value = 0.0;
	fprintf (out1,"\nTotal Core Area Index (%%): %8.3f",value);
	fprintf (out2,"%6.2f ",value);

/* 
 *  mean core area index
 */
 	if (sum_core_area_index > 0.0)
	   value = sum_core_area_index / (float)num_patches * 100.;
	else
	   value = 0.0;

	fprintf (out1,"   Mean Core Area Index (%%):%9.3f",value);
	fprintf (out2,"%6.2f ",value);

/*
 *  mean nearest neighbor distance
 */
        if (sum_nndist == 0.0) {
           fprintf (out1,"\nMean NearNeigh Dist(m):        NONE");
	   fprintf (out2,"          . ");
	}
	else {
	   fprintf (out1,"\nMean NearNeigh Dist (m):%11.3f",
	      sum_nndist / (float)num_patches);
	   fprintf (out2,"%11.2f ",sum_nndist / (float)num_patches);
	}

/*
 *  nearest neighbor distance standard deviation
 */
	if (sum_nndist == 0.0) {
	   fprintf (out1,"   Near Neighor SD (m):            NA");
	   fprintf (out2,"        . ");
	}
	else {
	   dvalue = (sum_nndist_sq - ((sum_nndist*sum_nndist) /
	      (float)num_patches)) / (float)num_patches;
	   if (dvalue > 0.0)
	      stdev = sqrt(dvalue);
	   else
	      stdev = 0.0;

	   fprintf (out1,"   Nearest Neighbor SD (m):%10.3f",stdev);
	   fprintf (out2,"%9.2f ",stdev);
	}

/*
 *  nearest neighbor distance coefficient of variation
 */
	if (sum_nndist == 0.0) {
	   fprintf (out1,"\nNearest Neighbor CV (%%):         NA");
	   fprintf (out2,"       . ");
	}
	else {
	   if (stdev > 0.0)
	      value = stdev / (sum_nndist / (float)num_patches) * 100.;
	   else
	      value = 0.0;

	   fprintf (out1,"\nNearest Neighbor CV (%%):  %9.3f",value);
	   fprintf (out2,"%8.2f ",value);
	}

/*
 *  mean proximity index
 */
	if (proximity) {
	   value = sum_prox / (float)num_patches;
	   fprintf (out1,"   Mean Prox Index:   %15.3f",value);
	   fprintf (out2,"%9.2f ",value);
	}
	else {
	   fprintf (out1,"   Mean Prox Index:                NA");
	   fprintf (out2,"        . ");
	}

/*
 *  interspersion/juxtaposition
 */
        if (total_num_classes <= 2) {
           fprintf (out1,"\nIntersper/Juxtapos (%%):          NA");
           fprintf (out2,"      . ");
        }
        else {
           value = -sum_iji / log((float)(total_num_classes - 1))
                    * 100.;
           fprintf (out1,"\nIntersper/Juxtapos (%%):    %8.3f",value);
           fprintf (out2,"%7.2f ",value);
        }
      

	fprintf (out1,"\n");
	fprintf (out2,"\n");
	fclose (out1);
	fclose (out2);

	cfree (ptrx);
	cfree (ptry);
}

