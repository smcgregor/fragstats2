/****************************************************************
 *  File:  bndedge.c  
 *
 *  Purpose:  Returns the length of the landscape boundary.
 *  	This includes any interior background patch edges.
 *
 *  Returned Parmeters:
 *	length:  length of the landscape boundary
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  7 December 1992
 *
 *  ID:  $Id: bndedge.c,v 2.1 1994/04/01 17:43:45 marks Exp $
 *******************************************************************/
#include "stats.h"


int boundary_edge ()
{

	short   value,neighbor;
	short	xptr,yptr;
	short   i,j,k;
	char	flag;
	int	length;


	length = 0;

/*
 *  Step through all cells in the image
 */
	for (i=0; i < num_rows; i++) {
	   for (j=0; j < num_cols; j++) {

/*
 *  Get the value of this cell.  If it is >= 0 it is within the
 *  landscape so check it further.  A value of -999 is a background 
 *  cell;  -990 is a background patch interior to the landscape;
 *  other negative values are classes outside the landscape.  
 *
 *  3-94 BJM  Also check background patches interior to the 
 *  landscape.  If there is a landscape border, a background
 *  patch may border the landscape boundary -- need to include
 *  these in this length. 
 */
	      value = getshort (image,num_cols,i,j);
	      if (value >= 0 || value == -990) {
/*
 *  Check this cell's four neighbors.  If a neighbor is < 0,
 *  or this cell is on the image edge, this is a boundary cell. 
 */
	         for (k=0; k < 4; k++) {
		    flag = FALSE;
		    xptr = j + xpos[k];
		    if (xptr < 0 || xptr >= num_cols) flag = TRUE;
		    yptr = i + ypos[k];
		    if (yptr < 0 || yptr >= num_rows) flag = TRUE;

		    if (flag) {
		       length ++;
		       continue;
		    }

		    neighbor = getshort (image,num_cols,yptr,xptr);

		    if (value == -990 && neighbor != -990) 
		       length ++;
		    else if (value >= 0 && neighbor < 0 && neighbor != -990) 
		       length ++;
	 	 }
	      }
	   }
	}
	return (length);
}

