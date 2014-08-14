/*************************************************************
 *  File Name:  GETPATCH.C
 *
 *  Purpose:  Finds all cells that make up the patch at the
 * 	passed location.  Patch cells are replaced with the
 *	value -777 in the image.  The X,Y location of the cells
 *	making up the patch are stored in the array "stack" and
 *	used by other routines.
 *
 *  Passed Parameters:
 *	startcol:	starting column of the patch
 *	startrow:	starting row of the patch
 *	class:		patch type of the patch
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January, 1993
 *
 *  ID:  $Id: getpatch.c,v 2.1 1994/04/01 17:47:19 marks Exp $
 **************************************************************/
#include "stats.h"
	

int get_patch (startcol,startrow,class)
short	startcol;
short	startrow;
short   class;
{
	short		x,y,xptr,yptr;
	short		num_neighs;
	int		i,k,end,j;
	int		numpts;
	unsigned char	hit,newpts;



/*
 *  Initialize the array "check" to true.  A true value indicates
 *  that a cell needs to be checked for neighboring patch cells.
 */
	for (j=0; j < MAX_PATCH_SIZE; j++) check[j] = TRUE;


/*
 *  Based on the flag set at run-time, evaluate 4 or 8 neighbors.
 */
	num_neighs = 4;
	if (use_diags) num_neighs = 8;

/*
 *  The array "stack" will contain all the points in the patch
 *  beginning at the row and column passed.  Set the number of
 *  points in the patch to 1.  Set the value of the first patch
 *  cell to -777, so that it won't be counted again as a new
 *  member of the patch.
 */
	x = startcol;
	y = startrow;
	stackx[0] = x;
	stacky[0] = y;
	numpts = 1;
	hit = TRUE;
	newpts = TRUE;
	setshort (image,num_cols,y,x,-777);

/*
 *  Start at the row and column passed, and look for other
 *  cells of this patch type.  Keep track of the cells in
 *  the patch in the array stack.  Loop until no new patch
 *  members are found.
 */

	while (newpts) {
	   newpts = FALSE; 
	   end = numpts;
/*
 *  Loop over all the points currently on the "stack".
 */
	   for (i=0; i < end; i++) {
/*
 *  If check[i] is false, all neighbors of this cell have been
 *  checked and no new patch cells were found.  Go to the next
 *  cell in the "stack".
 */
	      if (check[i]) {
	        x = stackx[i];
	        y = stacky[i];
		hit = TRUE;

/*
 *  "hit" will be true as long as patch cells are found
 *  in the neighborhood search below, or if this is a new
 *  cell off the stack.
 */ 
		while (hit) {
		   hit = FALSE;
/*
 *  Check this cell's 4/8 neighbors.  If a patch cell is found,
 *  set the value of the neighbor to -777 so that this cell 
 *  won't be evaluated again, add the x,y location to the "stack"
 *  of patch cells, set the x,y pointer to the new patch cell,
 *  increment the number of points in the patch, indicate that
 *  a patch cell has been found (hit, newpts = TRUE), and
 *  break out of the loop.
 */ 
		   for (k=0; k < num_neighs; k++) {
			xptr = x + xpos[k];
			if (xptr < 0 || xptr >= num_cols) continue;
			yptr = y + ypos[k];
			if (yptr < 0 || yptr >= num_rows) continue;

			if (getshort(image,num_cols,yptr,xptr) == class) {
			   if (numpts >= MAX_PATCH_SIZE) {
			     printf ("\nERROR! There are more than %d points",MAX_PATCH_SIZE);
		             printf ("\n  in this patch.  Currently, the maximum");
		             printf ("\n  number of patch cells is %d!",MAX_PATCH_SIZE);
			     printf ("\n  numpts: %d",numpts);
		             printf ("\n  EXITING ....\n");
			     exit(-1);
                	   }
			   setshort(image,num_cols,yptr,xptr,-777);
			   stackx[numpts] = xptr;
			   stacky[numpts] = yptr;
			   check[numpts] = TRUE;
			   numpts++;
			   hit = TRUE;
			   newpts = TRUE;
			   x = xptr;
			   y = yptr;
		           break; 
			}
  		   } 
/*
 *  All neighbors of the cell at position x,y have been evaluated
 *  and no new patch cells were found, so don't evaluate this cell
 *  anymore (set check[numpts] to false).
 */
		   if (newpts && !hit) check[numpts] = FALSE;

/*
 *  All neighbors for the current cell in the array "stack" have been
 *  evaluated and no new patch cells were found, so don't evaluate
 *  this cell any more (set check[i] to false).
 */
		   if (!newpts && !hit) check[i] = FALSE;
		}
	      }
	   }
	}

	return (numpts);
}
