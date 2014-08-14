/*************************************************************
 *  File Name:  CKEDGE.C
 *
 *  Purpose:  This routine is only called if the input landscape
 *	contains a landscape border.  This routine checks to
 *	see if there are any patches in the landscape border of
 *	the passed class (patch) type.  If so, the length of 
 *	edge along the landscape boundary is added to the various
 *	class edge length variables.
 *
 *  Passed parameters:
 *	class:	 code of the current class (patch) type
 *
 *  Programmer:  Barbara Marks
 * 
 *  Date: February 1993
 *
 *  ID:  $Id: ckedge.c,v 2.1 1994/04/01 17:44:04 marks Exp $
 *****************************************************************/ 
#include <math.h>
#include "stats.h"


void check_edges(class)
short	class;
{
	float		sum;
	short		x,y,xptr,yptr;
	short		val,value;
	short 		ii,i,j;
	int		k;
	int		numpts;
	int		nedges;
	unsigned char	flag;
 

/*
 *  Get a clean copy of the landscape 
 */
	read_image(0);
	sum = 0.0;

/*
 *  Look for any edges of the current class type in the landscape 
 *  border.  These cells would have a negative class value.
 */

	for (ii=0; ii < num_rows; ii++) {
	   for (j=0; j < num_cols; j++) {
	      val = getshort(image,num_cols,ii,j);
	      if (val != -class) continue;

/*
 *  A class of this type appears in the landscape border.  Find all
 *  the cells in this patch and then get the length of edge bordering
 *  the "real" landscape.
 */
	      numpts = get_patch(j,ii,val);

	      for (k=0; k < numpts; k++) {
	         x = stackx[k];
	         y = stacky[k];

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

	            if (flag) continue;
/*
 *  Retrieve the value of this neighbor 
 */
		    value = getshort (image,num_cols,yptr,xptr);

/*
 *  If the value of this neighbor is not equal to the current
 *  patch type, and it is >= 0, this is a boundary edge.
 */
		    if (value != class && value >= 0) {
		       true_edge ++;
		       all_edge ++;
		       iji_edge ++;

		       nedges = getint(edge,num_wt,class-min_wt,
			   value-min_wt);
		       nedges ++;
		       setint(edge,num_wt,class-min_wt,value-min_wt,nedges);
		      
/*
 *  The edge contrast variable also needs to be updated.  Add in
 *  the contrast created by this new edge.
 */
		       if (contrast_indices)
		          sum += getfloat(contrast,num_wt,
			      class-min_wt,value-min_wt);
		    }

/*
 *  3-17-94 BJM  A background class may exist along the landscape boundary  
 *  (int the landscape border).  Count this as edge if the user requested so.
 */
		    if (value == -990 && bound_wght > 0) {
		       true_edge += bound_wght;
		       sum += bound_wght;
		    }
	            if (value == -990) all_edge ++;
	         }
	      }
	   }
	}

	if (contrast_indices)
           extra_contrast = sum * cellsize;

}
