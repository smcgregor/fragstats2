/******************************************************************
 *  File Name:  RASCONT.C
 *
 *  Purpose:  Calculates contagion using an algorithm developed
 *	by Habin Lee.
 *
 *  Returned Parameters:
 *	contagion:	contagion as defined by Habin Lee	
 *	
 *  Programmer:  Barbara Marks
 *
 *  Date:  Feb 1993
 *
 *  ID:  $Id: rascont.c,v 2.1 1994/04/01 17:50:16 marks Exp $
 ******************************************************************/
#include <stdio.h>
#include <math.h>
#include "stats.h"


float raster_contagion ()
{
	int	*adj,*carea;
	int	edgeval;
	int	size;
	float	sum,contagion,prop1,prop2;
	short	i,j,n;
	short	value,neighbor;
	short	xptr,yptr;
	

/*
 *  Get a clean copy of the image.
 */
	read_image(0);

/*
 *  Allocate space for needed variables.
 */
	size = max_class - min_class + 1;

	adj = (int *) calloc ((unsigned)size,sizeof(int));
	if (adj == NULL) {
	   printf ("\nERROR! rascont: Can not allocate space for adj!");
	   exit(-1);
	}
	carea = (int *) calloc ((unsigned)size,sizeof(int));
	if (carea == NULL) {
	   printf ("\nERROR! rascont: Can not allocate space for carea!");
	   exit(-1);
	}

/*
 *  Initialize
 */
	for (i=0; i < size; i++) {
	   carea[i] = 0;
	   adj[i] = 0;
	   for(j=0; j < size; j++) 
	      setint(edge,num_wt,i,j,0);
	}


/*
 *  Step through all cells in the image 
 */

	for (i=0; i < num_rows; i++) {
	   for (j=0; j < num_cols; j++) {
	      value = getshort(image,num_cols,i,j);
	      if (value < 0) continue;

/*
 *  Get the area of each class.
 */
	      carea[value-min_class] ++;

/*
 *  Look at this values 4 neighbors ....
 */
	      for (n=0; n < 4; n++) {
                 xptr = j + xpos[n];
                 if (xptr < 0 || xptr >= num_cols) continue;
                 yptr = i + ypos[n];
                 if (yptr < 0 || yptr >= num_rows) continue;
 
		 neighbor = getshort(image,num_cols,yptr,xptr);

/*
 *  If the neighbor is within the landscape (value >=0), 
 *  increment the count of its patchtype, and increment the
 *  count of the number of this edge type combo.
 */
		 if (neighbor >= 0) {
		    adj[value-min_class] ++;
		    edgeval = getint(edge,num_wt,value-min_class,
			neighbor-min_class);
		    edgeval ++;
		    setint(edge,num_wt,value-min_class,neighbor-min_class,
			edgeval);
		 }
	      }
	   }
	}

/*
 *  Loop over all classes
 */
	sum = 0.0;
	for (i=0; i < size; i++) {
	   if (carea[i] == 0.0) continue;

/* 
 *  Get the proportion of the landscape in this class.
 */
	   prop1 = (float)carea[i] / (float)total_size;

/*
 *  Look for adjacencies with other classes.
 */
	   for (j=0; j < size; j++) {
	      edgeval = getint(edge,num_wt,i,j);
	      if (edgeval > 0) {
	         prop2 = (float)edgeval / (float)adj[i];
		 sum += (prop1 * prop2) * log(prop1 * prop2);
	      }
	   }
	}

/*
 *  Calculate contagion.
 */
	contagion = (sum / (2.0 * log((double)total_num_classes))) + 1.0;

	cfree (adj);
	cfree (carea);

	return (contagion);
}
