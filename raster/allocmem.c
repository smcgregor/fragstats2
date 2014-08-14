/*****************************************************************
 *  File Name:  ALLOCMEM.C
 *
 *  Purpose:  Allocate memory for arrays.
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  August 1993
 *
 *  ID:  $Id: allocmem.c,v 2.1 1994/04/01 17:43:30 marks Exp $
 *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "stats.h"


void allocate_memory()
{
	int	size;

	if (descriptors) {
	   name = (char **) calloc ((unsigned)NUM_CLASSES,sizeof(char *));
	   if (name == NULL) {
	     printf ("\nERROR! Can not allocate space for name!");
	     exit(-1);
	   }
	}

	stackx = (short *) calloc ((unsigned)MAX_PATCH_SIZE,sizeof(short));
	if (stackx == NULL) {
	   printf ("\nERROR! Can not allocate space for stackx!");
	   exit(-1);
	}

	stacky = (short *) calloc ((unsigned)MAX_PATCH_SIZE,sizeof(short));
	if (stacky == NULL) {
	   printf ("\nERROR! Can not allocate space for stacky!");
	   exit(-1);
	}

	check = (char *) calloc ((unsigned)MAX_PATCH_SIZE,sizeof(char));
	if (check == NULL) {
	   printf ("\nERROR! Can not allocate space for check!");
	   exit(-1);
	}

	ext_npts = (int *) calloc ((unsigned)MAX_PATCHES,sizeof(int));
	if (ext_npts == NULL) {
	   printf ("\nERROR! Can not allocate space for ext_npts!");
	   exit(-1);
	}

	core_area_patches = (int *) calloc ((unsigned)MAX_PATCHES,sizeof(int));
	if (core_area_patches == NULL) {
	   printf ("\nERROR! Can not allocate space for core_area_patches!");
	   exit(-1);
	}

	patchid = (short *) calloc ((unsigned)MAX_PATCHES,sizeof(short));
	if (patchid == NULL) {
	   printf ("\nERROR! Can not allocate space for patchid!");
	   exit(-1);
	}

	area = (double *) calloc ((unsigned)MAX_PATCHES,sizeof(double));
	if (area == NULL) {
	   printf ("\nERROR! Can not allocate space for area!");
	   exit(-1);
	}

	perimeter = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	if (perimeter == NULL) {
	   printf ("\nERROR! Can not allocate space for perimeter!");
	   exit(-1);
	}

	core_area = (double *) calloc ((unsigned)MAX_PATCHES,sizeof(double));
	if (core_area == NULL) {
	   printf ("\nERROR! Can not allocate space for core_area!");
	   exit(-1);
	}

	core_area_index = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	if (core_area_index == NULL) {
	   printf ("\nERROR! Can not allocate space for core_area_index!");
	   exit(-1);
	}

	shape = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	if (shape == NULL) {
	   printf ("\nERROR! Can not allocate space for shape!");
	   exit(-1);
	}

	fractal = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	if (fractal == NULL) {
	   printf ("\nERROR! Can not allocate space for fractal!");
	   exit(-1);
	}

	class_area = (float *) calloc ((unsigned)NUM_CLASSES,sizeof(float));
	if (class_area == NULL) {
	   printf ("\nERROR! Can not allocate space for class_area!");
	   exit(-1);
	}

	xx = (double *) calloc ((unsigned)total_num_patches,sizeof(double));
	if (xx == NULL) {
	   printf ("\nERROR! Can not allocate space for xx!");
	   exit(-1);
	}

	yy = (double *) calloc ((unsigned)total_num_patches,sizeof(double));
	if (yy == NULL) {
	   printf ("\nERROR! Can not allocate space for yy!");
	   exit(-1);
	}
	
	size = num_wt * num_wt;
	edge = (int *) calloc ((unsigned)size,sizeof(int));
	if (edge == NULL) {
	   printf ("\nERROR! Can not allocate space for edge!");
	   exit(-1);
	}

	edgetype = (int *) calloc ((unsigned)num_wt,sizeof(int));
	if (edgetype == NULL) {
	   printf ("\nERROR! Can not allocate space for edgetype!");
	   exit(-1);
	}

	if (contrast_indices) {
	   edge_contrast = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	   if (edge_contrast == NULL) {
	      printf ("\nERROR! Can not allocate space for edge_contrast!");
	      exit(-1);
	   }
	}

	if (do_nndist) {
	   near_neigh_dist = (double *)calloc ((unsigned)MAX_PATCHES,sizeof(double));
	   if (near_neigh_dist == NULL) {
	      printf ("\nERROR! Can not allocate space for near_neigh_dist!");
	      exit(-1);
	   }
	}

	if (proximity) {
	   prox = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	   if (prox == NULL) {
	      printf ("\nERROR! Can not allocate space for prox!");
	      exit(-1);
	   }
	}
}
