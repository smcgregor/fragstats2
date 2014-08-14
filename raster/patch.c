 /*************************************************************
 *  File Name:  PATCH.C
 *
 *  Purpose:  Determines the number of interior and exterior
 *	patch cells, and calculates various patch statistics
 *	(area, perimeter, core area, shape index, fractal
 *	dimension, core area index, and contrast weighted indices).
 *
 *  Passed Parameters:
 *	numpts:  	number of points in the patch
 *	class:	        patch type of the patch
 *	patch:	 	patch number (i.e, patch 3 of class 2)
 *
 *  Programmer:  Barbara Marks
 * 
 *  Date: 3 December 1992
 *
 *  ID:  $Id: patch.c,v 2.1 1994/04/01 17:49:36 marks Exp $
 *
 *  Modifications
 *	9-28-93  Tom Moore - changed the way exterior patch cells
 *		 are stored, and added bounding box code for each 
 *	 	 patch.
 *****************************************************************/ 
#include <math.h>
#include <stdio.h>
#include "stats.h"

#include "patch.h"
patchdef *patches = (void *) 0; /* TM 28/09/93 */



void patch_statistics (numpts,class,patch)
int	numpts;
short	class;
int	patch;
{
	short		x,y,xptr,yptr;
	short		value;
	short		newclass;
	short		zone,class1;
	short		nn;
	int		i,k;
	int		interior,exterior;
	int		perim;
	int		edgeval;
	float		extracon;
	float		sum;
	char		doit;
	unsigned char	exterior_flag;
	unsigned char	flag;

	static short    *px = (void *)0;
	static short    *py = (void *)0;
	short           *ppx, *ppy, *ppx2, *ppy2;

/*
 *   Initialize ...
 */
	perim = interior = exterior = 0;
	extracon = 0.0;

/*
 *   Allocate space for the patches structure (if it hasn't been allocated
 *   yet).  Initially, allocate space for the largest patch in the 
 *   landscape;  Later the size will be adjusted to the actual number
 *   of exterior cells in this patch.  TM 28/09/93
 */
	if (do_nndist) {
	   if (!patches) {
	      patches = (patchdef *) calloc((unsigned)MAX_PATCHES, sizeof(patchdef));
	      if (!patches) {
	        printf("\nERROR!  Cannot allocate space for patch structure\n");
	        exit(-1);
	      }
	   }
	   if (!px) {
	      px = (short *) calloc((unsigned)MAX_PATCH_SIZE,sizeof(short));
	      if (!px) {
	        printf("\nERROR!  Cannot allocate space for patch structure\n");
	        exit(-1);
	      }
	   }
	   if (!py) {
	      py = (short *) calloc((unsigned)MAX_PATCH_SIZE,sizeof(short));
	      if (!px) {
	        printf("\nERROR!  Cannot allocate space for patch structure\n");
	        exit(-1);
	      }
	   }
/*
 *  Initialize the bounding box for the patch
 */
	   ppx = px;
   	   ppy = py;
	   patches[patch].min_x = patches[patch].min_y =  999999;
	   patches[patch].max_x = patches[patch].max_y = -999999;
	}
	

/*
 *  Initialize array containing the patchtypes bordering this patch.
 */
	for (i=0; i < num_wt; i++) edgetype[i] = 0;

/*
 *  Decide if each pixel in the patch is interior or exterior.
 *  Determine perimeter by summing up the number of exterior
 *  edges. 
 *
 *  The locations of all the cells making up the current patch
 *  are in the array "stack".  This array was filled by the
 *  routine get_patch.
 */
	for (k=0; k < numpts; k++) {
	   x = stackx[k];
	   y = stacky[k];
	   exterior_flag = FALSE;

/*
 *  See if this pixel is "exterior" - check its four neighbors.
 *  If one neighbor is not within the patch then the pixel is 
 *  exterior. 
 */  	
	   for (i=0; i < 4; i++) {
	      flag = FALSE;
	      xptr = x + xpos[i];
	      if (xptr < 0 || xptr >= num_cols) flag = TRUE;
	      yptr = y + ypos[i];	
	      if (yptr < 0 || yptr >= num_rows) flag = TRUE;

/*
 *  This pixel is on the image edge.  This will only happen if there
 *  if no landscape border.  Set the exterior flag, increment the
 *  perimeter and edge counters, then go check the next neighbor.
 *
 *  "extracon" contains the proportion of the landscape boundary
 *  that is treated as edge.
 *
 */
	      if (flag) {
		perim ++;
		all_edge ++;
		true_edge += bound_wght;
		if (contrast_indices) extracon += bound_wght;
		exterior_flag = TRUE;
		continue;
	      }

/*
 *  Retrieve the value of this neighbor 
 */
	      value = getshort(image,num_cols,yptr,xptr); 

/*
 *  If the value of this neighbor is not equal to the current
 *  patch type, this is an exterior pixel.  Keep track of the type of
 *  edge cells bordering this patch.  Note that cells outside the
 *  area of interest (landscape) may be present and have values
 *  less than 1.  These are only used to determine edge types for
 *  patches bordering the area of interest (if they are present).
 *  Calculate edge lengths.
 *
 *  >= 0  landscape cells
 *  < 0 && > -777  landscape border cells
 *  -777  patch cells
 *  -800  interior patch cells; not an edge
 *  -888  exterior patch cells; not an edge
 *  -990  interior landscape background cells
 *  -999  background cells
 *
 *  all_edge:     includes all boundary and background edges in a class
 *  true edge:    includes true edge only; background/boundary edges are 
 * 		  adjusted by bound_wght
 *  iji_edge:     includes true edge boundary segments if a border exists;
 *		  if a border does not exist, all boundary segments are
 *		  excluded;  all background segments are excluded
 *  lshape_edge:  all class edge except boundary edges regardless of whether
 *		  there is a landscape border; interior background edges
 *		  are included.
 *
 */
	      if (value != class && value != -777) {

		 if (value != -800 && value != -888) {/* not int, ext */
		    perim ++;
                    exterior_flag = TRUE;
		    all_edge ++;     

	            if (value >= 0) {                 /* landscape cell*/
		       true_edge ++;   
		       lshape_edge ++; 
		       iji_edge ++;
		       edgetype[value-min_wt] ++;
		    }
		    if (value < 0) {
                       if (value == -999 || value == -990) { /* background cell*/
                          true_edge += bound_wght;
		          if (contrast_indices) extracon += bound_wght;
		       }
		       if (value > -990  && value != -class) { 
                          true_edge ++;               /* landscape border cell*/
                          iji_edge++;
		       }

		       if (value != -999 && value != -990) 
                          edgetype[(-value)-min_wt] ++;
	 	   }
		 }
	      }
	   }

/*
 *  Increment the number of exterior cells and set them to -888. 
 *  If the user requested nearest neighbor indices, save the
 *  coordinates of each exterior patch cell, and also keep
 *  track of the min,max X,Y coordinates of the patch (these form
 *  the bounding box).
 */
	   if (exterior_flag) { 
	      exterior++; 
	      setshort (image,num_cols,y,x,-888);
	      if (do_nndist) {          /* TM 28/09/93 */
	         *ppx++ = x; 
		 *ppy++ = y;
		 if (x < patches[patch].min_x) patches[patch].min_x = x;
		 if (y < patches[patch].min_y) patches[patch].min_y = y;
		 if (x > patches[patch].max_x) patches[patch].max_x = x;
		 if (y > patches[patch].max_y) patches[patch].max_y = y;
	      }
	   }

/*
 *   Set interior patch cells to -800 in the image map
 */      
           else 
	      setshort (image,num_cols,y,x,-800);   
/*
 *  If the user requested an ID image be created, assign each cell
 *  in this patch the passed ID.
 */
	   if (id_image == 1)
	      setshort(id,num_cols,y,x,patchid[patch]);
	}

/*
 *  Save the number of exterior patch cells in this patch
 */
	ext_npts[patch] = exterior;

/*
 * If doing nearest neighbor calculations, the program must store
 * the perimeter vertices of each patch.  Allocate space and store
 * the pointers in the patches structure.  Before creating more
 * space, make sure that any previous info left lying around is
 * cleaned up 
 * TM 30/09/93
 */
	if (do_nndist) { /* TM 28/09/93 */
	   if (patches[patch].px) {
	      cfree(patches[patch].px);
	      patches[patch].px = NULL;
	   }
	   if (patches[patch].py) {
	      cfree(patches[patch].py);
	      patches[patch].py = NULL;
	   }
	   patches[patch].px = (short*) calloc((unsigned)exterior,sizeof(short));
	   patches[patch].py = (short*) calloc((unsigned)exterior,sizeof(short));
	   if (!patches[patch].px || !patches[patch].py) {
	      printf("\nERROR Failed to allocate memory for patch boundary\n");
	      exit(-1);
	   }
	   ppx = px;
	   ppx2 = patches[patch].px;
	   ppy = py;
	   ppy2 = patches[patch].py;
/*
 *  transfer the exterior patch cell locations to the "patches" structure
 */
	   for (i=0;i < exterior;i++){
	      *ppx2++ = *ppx++;
	      *ppy2++ = *ppy++;
	   }
	}
	    
/*
 *  Determine number of core area patch cells (these are the interior
 *  patch cells >= to distance from edge specified by the user).
 *  Loop through all patch cells again assigning all neighbors
 *  of -2 cells (edge cells) to zone -3.  Next assign all neighbors 
 *  of -3 cells to zone -4.  Continue until no new zones are assigned.
 */
        doit = TRUE;
        class1 = -1;
        newclass = -2;
 
        while (doit) {
          class1 --;
          newclass --;
	  if (class1 <= -32768 || newclass <= -32768) {
 	     printf ("\n  patch.c -- fatal error!");
	     exit(-1);
	  }
          flag = FALSE;
 
            for (k=0; k < numpts; k++) {
               x = stackx[k];
               y = stacky[k];
               value = getshort(image,num_cols,y,x);
	       if (value == -888) {
		  value = -2;
	          setshort(image,num_cols,y,x,-2);
	       }
	       if (value == -800) value = -1;

               if (value != class1) continue;
 
               for (i=0; i < 4; i++) {
                  xptr = x + xpos[i];
	          if (xptr < 0 || xptr >= num_cols) continue;
	          yptr = y + ypos[i];	
	          if (yptr < 0 || yptr >= num_rows) continue;

                  value = getshort(image,num_cols,yptr,xptr);

                  if (value == -800) {
                     setshort (image,num_cols,yptr,xptr,newclass);
                     flag = TRUE;
                  }
               }
            }
            if (!flag) doit = FALSE;
        }
 

/*
 *  Reset "zone" cells to reflect buffer zone input by user.
 *  "min_dist" is the number of cells away from an edge a
 *  cell must be to be considered "core" area.  Add 2 to this
 *  number since edge cells are -2, cells one cell away from the
 *  edge are -3, etc.
 *
 *  set edge cells to -900 
 *  interior cells to -777
 *  core area cells to -800
 */

        zone = -(min_dist + 2);
        interior = 0;
        for (k=0; k < numpts; k++) {
           x = stackx[k];
           y = stacky[k];
	   value = getshort(image,num_cols,y,x);

           if (value <= zone) { 
	        interior ++;
		setshort (image,num_cols,y,x,-800);
           }
        }
 

/*
 *  The interior cells with values -800 are core area cells.  Now
 *  find how many core area patches there are within the current
 *  patch.  Odd shaped patches may have more than one core area.
 */
	if (interior > 0) 
	   core_area_patches[patch] = num_core_area_patches(numpts);
	else
	   core_area_patches[patch] = 0;

	
/*
 *  Calculate patch statistics
 */

	area[patch] = (float)numpts * cellsize*cellsize / 10000.;
	classarea += area[patch];

	perimeter[patch] = (float)perim * cellsize;
	
	core_area[patch] = (float)interior * cellsize*cellsize / 10000.;
	core_area_index[patch] = (float)interior / (float)numpts * 100.;

	shape[patch] = (.25 * (float)perim) / sqrt((double)numpts);
	
	if (numpts == 1 && perim == 4)
	   fractal[patch] = 1.0;
	else 
	   fractal[patch] = (2.0 * log(perimeter[patch]/4.0)) / 
     		log(area[patch]*10000.);


/*
 *  The edge array needs to be filled even if the user did not
 *  input a weight file for contrast indices.  The edge array is
 *  used for class level interspersion/juxtaposition.
 */
        sum = 0.0;
        for (nn=0; nn < num_wt; nn++) {
             if (edgetype[nn] > 0) {
		if (contrast_indices) 
                   sum += (float)edgetype[nn] * 
			  getfloat(contrast,num_wt,class-min_wt,nn);

	        if (nn+min_wt != class) {
		   edgeval = getint(edge,num_wt,(short)class-min_wt,nn);
		   edgeval += edgetype[nn];
		   setint(edge,num_wt,(short)class-min_wt,nn,edgeval);
	        }
	        else
		   setint(edge,num_wt,(short)class-min_wt,nn,0);
	     }
        }
	if (contrast_indices)
           edge_contrast[patch] = (sum + extracon) * cellsize;

}

