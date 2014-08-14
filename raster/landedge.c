/***********************************************************************
 *  File:  LANDEDGE.C
 *
 *  Purpose:  Calculates several different edge lengths at the landscape
 *	level:
 *		true edge: does not include edges between patches of 
 *		           type i bordering the landscape boundary, and
 *			   patches of type i in the landscape border
 *		all edge:  all edge, regardless of whether it is true edge, 
 *		           or boundary
 *		iji_edge:  total edge needed for interspersion/juxta-
 *		           position index.  Excludes all background edge
 *			   segments, and boundary segments if there is
 *			   no landscape border.  If a border is included,
 *			   non-true edges are excluded.
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1993
 *
 *  ID:  $Id: landedge.c,v 2.1 1994/04/01 17:49:07 marks Exp $
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stats.h"


void landscape_edge_lengths ()
{

	int	edge1,edge2;
	short   value,neighbor,newval;
	short	xptr,yptr;
	short	i,j,k;
	char 	flag;


/*
 *  Get a  clean  copy of the image
 */
	read_image(0);

	all_edge = 0.0;
	true_edge = 0.0;
	iji_edge = 0.0;
	edcon = 0.0;

	for (i=0; i < num_wt; i++) {
	   for (j=0; j < num_wt; j++)
	      setint(edge,num_wt,i,j,0);
	}

/*
 *  Loop through all cells in the image
 */
	for (i=0; i < num_rows; i++) {
	   for (j=0; j < num_cols; j++) {

	      value = getshort (image,num_cols,i,j);

/*
 *  Only consider cells inside the landscape (values >= 0)
 */
	      if (value < 0 && value != -990) continue;

/*
 *  Check this cells four neighbors.  If the neighbor is a background
 *  cell (value of -990,-999), or a landscape border edge, or its not equal 
 *  to the value of this cell, it is an edge.
 */
	      for (k=0; k < 4; k++) {
	         flag = FALSE;
		 xptr = j + xpos[k];
		 if (xptr < 0 || xptr >= num_cols) flag = TRUE;
		 yptr = i + ypos[k];
		 if (yptr < 0 || yptr >= num_rows) flag = TRUE;

/*
 *  This cell is on the image edge - increment variables that
 *  include the landscape boundary.
 *
 */
		 if (flag) {
		    all_edge ++;
		    true_edge += bound_wght;
		    if (contrast_indices) edcon += bound_wght;
		    continue;
		 }

/*
 *  Get the value of this neighbor.
 *
 *  For landscapes with borders, we don't want to count as an edge
 *  the landscape boundary if the adjacent class is the same as the
 *  boundary class.  "true" contains this modified edge length. 
 */ 
		 neighbor = getshort (image,num_cols,yptr,xptr);
		 if (neighbor == -998) continue;

		 if (neighbor != value) {
		    all_edge++;
		    if (neighbor >= 0) {
		       if (value == -990)
			  true_edge += bound_wght;
		       else {
	   	          true_edge ++;
		          iji_edge ++;
		       }
		    }
		    else {
/*
 *  If this is a background/boundary cell, adjust the true edge 
 *  and boundary edge length by the proportion specified by the user.
 */

		       if (neighbor == -999 || neighbor == -990) 
                             true_edge += bound_wght;
		       
		       else if (neighbor > -990 && neighbor != -value) {
			  if (value == -990)
			     true_edge += bound_wght;
			  else {
		             true_edge ++;
			     iji_edge ++;
                          }
		       }
		    }
/*
 *  If requested, find the contrast weighted edge length
 */

		    if (contrast_indices) {
		       if (value == -990) 
			   edcon += bound_wght;
		       else if (neighbor < 0 && neighbor > -990) 
		           edcon += getfloat(contrast,num_wt,
			         value-min_wt,(-neighbor)-min_wt);
		       else if (neighbor >= 0)
			   edcon += getfloat(contrast,num_wt,
                                 value-min_wt,neighbor-min_wt);
		       else if (neighbor == -999 || neighbor == -990) 
		           edcon += bound_wght;
		    }

/*
 *  For contagion, increment the length between the patch type of
 *  of "value" and the patch type of "neighbor".
 */
		    if (value == -990 || neighbor == -999 || neighbor == -990) 
		       continue;

		    newval = (short)fabs((double)neighbor);
		    edge1 = getint(edge,num_wt,value-min_wt,newval-min_wt);
		    edge2 = getint(edge,num_wt,newval-min_wt,value-min_wt);

		    if (land_border && neighbor != -value) {
		        if (value < newval)
		            edge1 ++;
  			else
		            edge2 ++;
		    }
		    if (!land_border) {
		        if (value < newval)
		            edge1 ++;
  		        else
		            edge2 ++;
		    }
		    setint(edge,num_wt,value-min_wt,newval-min_wt,edge1);
		    setint(edge,num_wt,newval-min_wt,value-min_wt,edge2);
		 }
	      }
/*
 *  Set the current cell to -998, so that it isn't evaluated again.
 */
              setshort (image,num_cols,i,j,-998);
	   }
	}
}
