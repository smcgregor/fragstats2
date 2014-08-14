/*************************************************************
 *  File Name:  PATCHSIZ.C
 *
 *  Purpose:  Returns the size of the patch at location 
 *	startcol, startrow.
 *
 *  Passed Parameters:
 *	startcol:	starting column of the patch
 *	startrow:	starting row of the patch
 *	class:		patch type of the patch
 *
 *  Returned Parameters: 
 *	numpts:	        number of points in the current patch
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January, 1993
 *
 *  ID:  $Id: patchsiz.c,v 2.2 1994/08/08 19:10:38 marks Exp marks $
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "stats.h"
	

int patch_size (startcol,startrow,class)
short	startcol;
short	startrow;
short   class;
{
	char		*new_ck;
	short		*new_x,*new_y;
	short		x,y,xptr,yptr;
	short		num_neighs;
	int		nn;
	int		i,j,k,end;
	int		numpts;
	int		size,new_size;
	unsigned char	hit,newpts;


/*
 *  Initially allocate space for a patch that has <= 8192 
 *  members.  More space will be allocated below if this is
 *  not enough.
 */
	size = 8192;

	check = (char *) calloc ((unsigned)size,sizeof(char));
	if (check == NULL) {
	   printf ("\nERROR! patchsize: can not allocate space for check!");
	   exit(-1);
	}
	stackx = (short *) calloc ((unsigned)size,sizeof(short));
	if (stackx == NULL) {
	   printf ("\nERROR! patchsize: can not allocate space for stackx!");
	   exit(-1);
	}
	stacky = (short *) calloc ((unsigned)size,sizeof(short));
	if (stacky == NULL) {
	   printf ("\nERROR! patchsize: can not allocate space for stacky!");
	   exit(-1);
	}

/*
 *  Initialize the array "check" to true.  A true value indicates
 *  that a cell needs to be checked for neighboring patch cells.
 */
	for (j=0; j < size; j++) check[j] = TRUE;


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
 *  in the patch in the array stack.  Loop until no new
 *  patch members are found.
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
 *  Check this cell's neighbors.  If a patch cell is found,
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
		        setshort(image,num_cols,yptr,xptr,-777);
		        stackx[numpts] = xptr;
		        stacky[numpts] = yptr;
		        check[numpts] = TRUE;
		        numpts++;
		        hit = TRUE;
		        newpts = TRUE;
		        x = xptr;
		        y = yptr;
		        if (numpts >= size) {
		          new_size = size * 2;
			  new_ck = (char *)calloc((unsigned)new_size,sizeof(char));
			  if (new_ck == NULL) {
			    printf ("\nERROR! patchsize: can not allocate space for new_ck");
			    exit(-1);
			  }
			  new_x = (short *)calloc((unsigned)new_size,sizeof(short));
			  if (new_x == NULL) {
			    printf ("\nERROR! patchsize: can not allocate space for new_x");
	 		    exit(-1);
			  }
			  new_y = (short *)calloc((unsigned)new_size,sizeof(short));
			  if (new_y == NULL) {
			    printf ("\nERROR! patchsize: can not allocate space for new_y");
	 		    exit(-1);
			  }
			  for (nn=0; nn < new_size; nn++) {
			    if (nn < size) {
				new_ck[nn] = check[nn];
				new_x[nn] = stackx[nn];
				new_y[nn] = stacky[nn];
			    }
			    else 
			        new_ck[nn] = TRUE;
			  }
			  cfree (check);
			  cfree (stackx);
			  cfree (stacky);
			  check = new_ck;
			  stackx = new_x;
			  stacky = new_y;
			  size = new_size;
                        }
		        break; 
		      }
  		   } 
/*
 *  All neighbors of the cell at position x,y have been evaluated
 *  and no new patch cells were found, so don't evaluate this cell
 *  anymore (set check[pts] to false).
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

	cfree (stackx);
	cfree (stacky);
	cfree (check);

	return (numpts);
}
