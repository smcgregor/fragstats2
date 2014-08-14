/***********************************************************
 *  File Name:  FREEMEM.C
 *
 *  Purpose:  Free memory allocated for arrays
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  August 1993
 *
 *  ID:  $Id: freemem.c,v 1.2 1994/04/01 17:45:39 marks Exp $
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "stats.h"


void free_memory()
{
	cfree (image);
	if (id_image != 2) cfree (id);

	cfree (name);
	cfree (stackx); 
	cfree (stacky);
	cfree (check);
	cfree (ext_npts);
	cfree (core_area_patches);
	cfree (patchid);
	cfree (area);
	cfree (perimeter);
	cfree (core_area);
	cfree (core_area_index);
	cfree (shape);
	cfree (fractal);
	cfree (class_area);
	cfree (xx);
	cfree (yy);
	cfree (edge);
	cfree (edgetype);
	cfree (patchtype);
	if (do_nndist) cfree (near_neigh_dist);
	if (proximity) cfree (prox);
	if (contrast_indices) {
	   cfree (edge_contrast);
	   cfree (contrast);
	}
}
