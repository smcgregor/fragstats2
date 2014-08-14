/**********************************************************
 *  File Name:  REGRESS.C
 *
 *  Purpose:  Calculates the least squares equation for
 *	the data passed.
 *
 *  Passed Parameters:
 *	x:	   array of x values
 *	y: 	   array of y values
 *	num_pts:   number of points in the x and y arrays
 *
 *  Returned Parameters:
 *  	slope:     slope of the least squares line
 *	intercept: y-intercept of the least squares line
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1993
 *
 *  ID:  $Id: regress.c,v 1.2 1994/04/05 17:13:46 marks Exp marks $
 ***********************************************************/
#include <math.h>
#include "stats.h"

void regression (x,y,num_pts,slope,intercept) 
double  *x;
double	*y;
int	num_pts;
double  *slope;
double  *intercept;
{

	int	i;
	double	sum_x,sum_y;
	double 	sum_xy,sum_sq_x;
	double  val1,val2;
	double  delta = .900E-7;

	
	sum_x = sum_y = sum_xy = sum_sq_x = 0.0;

	for (i=0; i < num_pts; i++) {
	   sum_x += *x;
	   sum_y += *y;
	   sum_xy += (*x * *y);
	   sum_sq_x += (*x * *x);
	   x ++;
	   y ++;
	}

	if (num_pts == 1) {
	   *slope = 0.0;
	   *intercept = 0.0;
	}
	else {
	   val1 = fabs((sum_y * sum_sq_x) - (sum_x * sum_xy));
	   val2 = fabs(((float)num_pts * sum_sq_x) - (sum_x * sum_x));

	   if (fabs(val1 - val2) < delta)
	      *intercept = 0.0;
	   else
	      *intercept = val1 / val2;


	   val1 = fabs(((float)num_pts * sum_xy) - (sum_x * sum_y));
	   val2 = fabs(((float)num_pts * sum_sq_x) - (sum_x * sum_x));

           if (fabs(val1 - val2) < delta) 
	      *slope = 0.0;
	   else
	      *slope = val1 / val2;
	}

}
