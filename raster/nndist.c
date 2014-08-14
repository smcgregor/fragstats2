/*********************************************************************
 *  File Name:  NNDIST.C
 *
 *  Purpose:  Calculates the nearest neighbor distance between all
 *	patches in a class using the following algorithm to 
 *	calculate distance:
 *
 *	      distance = sqrt((x1-x2)(x1-x2) + (y1-y2)(y1-y2))
 *
 *  	Distance is only calculated for exterior patch points 
 * 	(i.e.  the distance for each exterior cell in patch 1
 *	to each exterior cell in patch 2 is calculated and 
 *      compared).  
 *
 *  Date:  8 May 1988
 *
 *  Programmer:  Barbara Marks
 *
 *  ID: $Id: nndist.c,v 2.4 1995/01/18 18:07:23 marks Exp marks $
 *
 *  Revisions:
 *	9-28-93	  Tom Moore - improved performance:
 *
 *	3-94      Barbara Marks - changing so that nearest neighbor
 *		  distance reported is from the edge of one cell to
 *		  the edge of its nearest neighbor.  Before, it was 
 *		  from the center of one edge cell to the center of 
 *		  its closest neighbor.
 *
 *	1-95      Barbara Marks -  missed 3 places in the 3-94 change;
 *		  see comments in program
 *********************************************************************/
#include <math.h>
#include <string.h>
#include "stats.h"

#include "patch.h"      /* TM 28/09/93 */
extern patchdef *patches;


void near_neigh_distance ()
{
	int	size;
	int	i,j,k,n;
	short	x1,y1,x2,y2;
	short	*p1x,*p1y,*p2x,*p2y;
	float	dist,mindist;
	float	*distance;
	float	value,val2;
	float	sum;

	float   nearest_so_far;  /* TM 29/09/93 */
	int     above, below, left, right;


/*
 *  First check to see if any work needs to be done.  If
 *  there is only one patch in this class then release memory 
 *  and return.
 *  TM 30/09/93
 */

	if (num_patches == 1) {
	   if (patches[0].px) cfree(patches[0].px);
	   if (patches[0].py) cfree(patches[0].py);
	   return;
	}
/*
 *  Allocate space for the array that will hold the shortest distance
 *  between all patches in a class.
 */
	size = num_patches * num_patches;

	distance = (float *) calloc ((unsigned)size,sizeof(float));
	if (distance == NULL) {
	   printf ("\nERROR! Can not allocate space for distance array!");
	   exit(-1);
	}

/* 
 *  Initialize
 */
	for (i=0; i < num_patches; i++) {
	   near_neigh_dist[i] = 0.0;
	   for (j=0; j < num_patches; j++) 
	      setfloat(distance,num_patches,(short)i,(short)j,999999.0);
	}

/*
 *  Compare each patch to every other patch ....
 *  This is an (N*N) loop, but the search space is still (N*N)/2
 *  since the matrix is symmetrical.  The shortcuts added here 
 *  should speed things up even more.
 *  TM 01/10/93
 */

	for (i=0; i < num_patches; i++) {
/*
 *  Locate the data for patch i
 *  TM 28/09/93
 */
	   p1x = patches[i].px;
	   p1y = patches[i].py;
	   nearest_so_far = 999999.0;  

	   for (k=0; k < num_patches; k++) {

/*
 *  There are a few conditions for skipping this comparison.
 *  Obviously, if (i == k) then the distance is zero and no
 *  test is required.  Also check to see if the distance between
 *  cells has already been computed.  This can be tested by the
 *  value not equal to the initialized value (999999.0).  If it
 *  has already been computed, skip this one.
 *  TM 01/10/93
 */

	     if (k == i) {
	       setfloat(distance,num_patches,(short)i,(short)k,0.0);
	       setfloat(distance,num_patches,(short)k,(short)i,0.0);
	       continue;
	     }

	     if (getfloat(distance,num_patches,(short)i,(short)k)
		!=  999999.0) continue;

/*
 *  Locate the data for patch k
 *  TM 28/09/93
 */
	      p2x = patches[k].px;
	      p2y = patches[k].py;

/*
 *  Perform a few tests to see if we can advantageously skip
 *  the tedious distance calculation.  The main criteria is
 *  if the minimum distance between the bounding boxes is
 *  less than the nearest neighbour so far, or within the
 *  proximity search radius, then do the exhaustive search.  
 *  TM 28/09/93
 */
	      above = (patches[i].max_y < patches[k].min_y);
	      below = (patches[i].min_y > patches[k].max_y);
	      right = (patches[i].max_x < patches[k].min_x);
	      left =  (patches[i].min_x > patches[k].max_x);

/*
 *  Define a macro to make life easier and code more readable.
 */

/*
 *  3-1-94 BJM  Per Eric Gustafson's comments, the nearest neighbor
 *     distance will now be reported as the distance from the outer
 *     edge of an exterior cell to another.  Originally, the distance
 *     was calculated from the center of an exterior cell to another.
 */

#define DIST(x1,y1,x2,y2) sqrt((double) ((fabs((double)x1-x2)-1.)*(fabs((double)x1-x2)-1.))+((fabs((double)y1-y2)-1.)*(fabs((double)y1-y2)-1.)))

/*
 *  First figure out the minimum distance between the boxes 
 *  bounding patch i and k
 *
 *  1-18-95 BJM  - Corrected the following code.  The 4 lines
 *	where distance is calculated without calling the macro
 *	DIST were wrong - I forgot to subtract 1.0 as per the
 *	3/94 change.
 */
	      if (above) 
		if (right) 
		  dist = DIST(patches[i].max_x,patches[i].max_y,
			      patches[k].min_x,patches[k].min_y);
		else if (left)  
		  dist = DIST(patches[i].min_x,patches[i].max_y,
			      patches[k].max_x,patches[k].min_y);
		else
		  dist = patches[k].min_y - patches[i].max_y - 1.0;
	
	      else if (below) 
		if (right) 
		  dist = DIST(patches[i].max_x,patches[i].min_y,
			      patches[k].min_x,patches[k].max_y);
		else if (left)  
		  dist = DIST(patches[i].min_x,patches[i].min_y,
			      patches[k].max_x,patches[k].max_y);
		else
		  dist = patches[i].min_y - patches[k].max_y - 1.0;
	     
	      else 
		if (right) 
		  dist = patches[k].min_x - patches[i].max_x - 1.0;
		else if (left)  
		  dist = patches[i].min_x - patches[k].max_x - 1.0;
		else
		  dist = 0.0;


/*
 *  Now test to see if it is worth checking further into this pair.
 *
 *  1-18-95  BJM  Forgot to mutiply dist by cellsize before comparing
 *  it to the search radius!
 */
	      mindist = 999999.0;

	      if (dist < nearest_so_far || 
		  (proximity && ((dist*cellsize) <= search_radius))) {

/*
 *  Ok, use brute force to calculate the distance from each exterior 
 *  cell in patch i to every exterior cell in patch k.
 */

		  for (j=0; j < ext_npts[i]; j++) {
		     x1 = *p1x++;
		     y1 = *p1y++;
		     p2x = patches[k].px;          /* TM 29/09/93 */
		     p2y = patches[k].py;
		     for (n=0; n < ext_npts[k]; n++) {
		         x2 = *p2x++;
		         y2 = *p2y++;
			 dist = DIST (x1,y1,x2,y2);
		         if (dist < mindist) mindist = dist;
		     }
		  }
	      }
	      if (mindist < nearest_so_far) nearest_so_far = mindist;
/*
 *  Save the minimum distance from patch i to patch k (this is 
 *  also the minimum distance from patch k to patch i).
 */

	      setfloat(distance,num_patches,(short)i,(short)k,mindist);
	      setfloat(distance,num_patches,(short)k,(short)i,mindist);

	      p1x = patches[i].px;
	      p1y = patches[i].py;
	   }
/*
 *  For patch i, find the closest neighboring patch cell.
 */ 
	   mindist = 999999.0;
	   sum = 0.0;
  	   for (k=0; k < num_patches; k++) {
		if (k == i) continue;
		value = getfloat(distance,num_patches,(short)i,(short)k);
		if (value < mindist) mindist = value;

/*
 *  For the proximity index, find all patches within the buffer
 *  distance specified; sum up (area/near neighbor distance squared).
 */
		value *= cellsize;
                if (proximity && (value <= search_radius)) {
                   val2 = (area[k] * 10000.) / (value * value);
		   sum += val2;
		}
           }  
	   near_neigh_dist[i] = mindist * cellsize;
	   if (proximity) prox[i] = sum;
	}


/*
 *  Housekeeping.  Free up space for patch boundary cells before exiting.
 *  TM 01/10/93
 */
	for (i=0; i<num_patches; i++) {
	   cfree (patches[i].px); 
	   patches[i].px = NULL;
	   cfree (patches[i].py);
	   patches[i].py = NULL;
	}
	cfree (distance);
	distance = NULL;
}
