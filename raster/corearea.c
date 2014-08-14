/*************************************************************
 *  File Name:  COREAREA.C
 *
 *  Purpose:  Returns the number of core area patches in the
 *  	current patch (odd shaped patches may have more than
 *      one core area patch).  Core area cells have the value 
 *	-800.
 *
 *  Passed Parameters:
 *	numpts:    number of cells in the current patch (the
 *		   locations of the cells making up the
 *		   current patch are in the array stack).
 *
 *  Returned Parameters:
 *  	npatches:  number of core area patches
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  December 1992
 *
 *  ID:  $Id: corearea.c,v 2.1 1994/04/01 17:44:38 marks Exp $
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "stats.h"
	

int num_core_area_patches(numpts)
int    numpts;
{
	short		x,y,xptr,yptr;
	unsigned short 	npatches;
	short		num_neighs;
	short		*temp_x;
	short		*temp_y;
	int		i,j,k,end;
	int		pts;
	unsigned char	hit,newpts;


/*
 *  Initialize ...
 */
	temp_x = (short *) calloc ((unsigned)numpts,sizeof(short));
	if (temp_x == NULL) {
	   printf ("\nERROR! corearea: can not allocate space for temp_x");
	   exit(-1);
	}
	temp_y = (short *) calloc ((unsigned)numpts,sizeof(short));
	if (temp_y == NULL) {
	   printf ("\nERROR! corearea: can not allocate space for temp_y");
	   exit(-1);
	}
	npatches = 0;
	pts = 0;
	num_neighs = 4;
	if (use_diags) num_neighs = 8;

/*
 *  Loop over all the cells in the current patch, looking for
 *  core area cells (they have values of -800).
 */

	for (j=0; j < numpts; j++) {
 
	   if (getshort(image,num_cols,stacky[j],stackx[j]) == -800) {

/*
 *  Found a core area cell, so start growing the patch.  First,
 *  initalize the check array to TRUE.  A value of TRUE in this
 *  array indicates the cell needs to be checked for neighboring
 *  patch cells.
 */ 
	      for (i=0; i < numpts; i++) check[i] = TRUE;
/*
 *  The array "s" will contain core area cells in the current
 *  patch (the array "stack" contains the current patch cells).
 *  Add the location of the first core area patch cell to the
 *  "s" array, and increment the number of core area patches.
 */
	      x = stackx[j];
	      y = stacky[j];
	      temp_x[0] = x;
              temp_y[0] = y;
	      npatches ++;
/*
 *  "pts" is the number of pts in the current core area patch.
 *  "hit" and "newpts" indicate that a core area patch cell has
 *   been found (true).  They are used below.
 */
	      pts = 1;
	      hit = TRUE;
	      newpts = TRUE;
/* 
 *  Set the first patch cell to -888 in the "image" image so that it
 *  won't be counted again as another core area patch cell.
 */
	      setshort (image,num_cols,y,x,-888);

/*
 *  Start at x,y and look for other cells with values of -800.
 *  Keep track of the cells in the patch.  Loop until no new
 *  patch members are found. 
 */

	      while (newpts) {
	         newpts = FALSE; 
	         end = pts;
	         for (i=0; i < end; i++) {
/*
 *  If check[i] is false, all neighbors of this cell have been
 *  checked and no new patch cells were found.  Go to the next one.
 */
	            if (check[i]) {
/*
 *  Get a cell off the stack.
 */
	               x = temp_x[i];
	               y = temp_y[i];
		       hit = TRUE;

/*  
 *  "hit" will be true as long as core area patch cells were
 *  found in the neighborhood search below or if this is a new
 *  cell off the stack.
 */
		       while (hit) {
		          hit = FALSE;
/*
 *  Check the the neighbors of the cell at position x,y.  If a
 *  patch cell is found, set it to -888 in the "image" image so
 *  it won't be counted as a patch cell again, add the x,y location 
 *  to the "stack" of patch cells (array s), increment the number
 *  of cells in the current patch (pts), set the x,y pointer to the 
 *  new patch cell, indicate a new patch cell was found (hit, newpts 
 *  = true), and break out of the neighbor search loop.
 */
		          for (k=0; k < num_neighs; k++) {
			     xptr = x + xpos[k];
			     if (xptr < 0 || xptr >= num_cols) continue;
			     yptr = y + ypos[k];
			     if (yptr < 0 || yptr >= num_rows) continue;

			     if (getshort(image,num_cols,yptr,xptr) == -800) {
			       setshort(image,num_cols,yptr,xptr,-888);
			       temp_x[pts] = xptr;
			       temp_y[pts] = yptr;
			       pts++;
			       x = xptr;
			       y = yptr;
			       hit = TRUE;
			       newpts = TRUE;
		               break; 
			     }
  		          } 
/*
 *  All neighbors of the cell at position x,y have been evaluated 
 *  and no new core area patch cells were found, so don't evaluate
 *  this cell any more (set check[pts] to false).
 */
		          if (newpts && !hit) check[pts] = FALSE;
/*
 *  All neighbors for the current cell in the array "s" have been
 *  evaluated and no new core area patch cells were found, so don't
 *  evaluate this cell any more (set check[i] to false).
 */
		          if (!newpts && !hit) check[i] = FALSE;
		       }
	            }
	         }
	      }
	   }
	}
	cfree (temp_x);
	cfree (temp_y);

/*
 *  Return the number of core area patches in this patch.
 */
	return (npatches);
}
