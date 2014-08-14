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
	free (image);
	if (id_image != 2) free (id);

	free (name);
	free (stackx); 
	free (stacky);
	free (check);
	free (ext_npts);
	free (core_area_patches);
	free (patchid);
	free (area);
	free (perimeter);
	free (core_area);
	free (core_area_index);
	free (shape);
	free (fractal);
	free (class_area);
	free (xx);
	free (yy);
	free (edge);
	free (edgetype);
	free (patchtype);
	if (do_nndist) free (near_neigh_dist);
	if (proximity) free (prox);
	if (contrast_indices) {
	   free (edge_contrast);
	   free (contrast);
	}
}
