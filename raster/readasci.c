/*************************************************************
 *  File Name:  READASCI.C
 *
 *  Purpose:  Reads an ascii image file. Data values should be
 *	separated by spaces or a comma.
 *
 *  Passed Parameters:
 *	filename:    name of the file containing the ascii data
 *	image_ptr:   pointer to the image to fill with this data
 *
 *  Returned Parameters:
 *	min:	     minimum value (>= 0) in the input image
 *	max:         maximum value in the input image
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  7 December 1992
 *
 *  ID:  $Id: readasci.c,v 2.4 1994/10/05 16:06:49 marks Exp marks $
 **************************************************************/
#include <stdio.h>
#include <string.h>
#include "stats.h"


void read_ascii (filename,image_ptr,min,max)
char	*filename;
short   *image_ptr;
short	*min,*max;
{
	int	i,j;
	int	val;
	int	count_bck,count_intbck;
	short	*ptr;
	FILE	*fp;
	

	if ((fp = fopen(filename,"r")) == NULL) {
		printf("\nERROR reading file: %s\n",filename);
		exit(-1);
	}

	count_bck = count_intbck = 0;
	*min = 9999;
	*max = -9999;
	MAX_CLASSES = -999;
	land_border = FALSE;
	ptr = image_ptr;

/*
 *  total_size is defined in the include file stats.h.  total_size
 *  is the number of cells inside the landscape (non-background and
 *  non-landscape border cells).
 */
	total_size = 0;

	
	for (i=0; i < num_rows; i++) {
  	   for (j=0; j < num_cols; j++) {
	        fscanf (fp,"%d",&val);

	        *ptr = (short) val;

/*
 *  If there are class values < 0 and not background, then this
 *  image includes a landscape border.  Set a flag.
 */
	 	if (*ptr < 0 && *ptr != background && *ptr != -background)
                   land_border = TRUE;

/*
 *  Find the minimum and maximum class values (only consider those
 *  classes inside the landscape).  
 */
	   	if (*ptr != background && *ptr != -background) {
	   	   if (*ptr < *min && *ptr >= 0) *min = *ptr;
		   if (*ptr > *max) *max = *ptr;
		   if (*ptr >= 0) total_size ++;

/*
 *  Keep track of the maximum class value encountered (either
 *  positive or negative).  This is needed by the routine getsizes.c
 *  to allocate space for arrays.
 */
	           if (*ptr >= 0 && *ptr > MAX_CLASSES)
		      MAX_CLASSES = *ptr;
	           if (*ptr < 0  && -(*ptr) > MAX_CLASSES)
                      MAX_CLASSES = -(*ptr);
	   	}

/*
 *  The value for background cells was input by the user.  Set background
 *  cells interior to the landscape to -990, and those in the landscape
 *  border or outside the area of interest to -999.  This makes life
 *  easier in other routines.
 */
		if (*ptr == -background) {
		   count_bck ++;
		   *ptr = -999;
		}
		if (*ptr == background) {
		   count_intbck ++;
		   *ptr = -990;
		}
	        
		ptr++;
	   }
	   fscanf (fp,"\n");
	}

	fclose (fp);
	MAX_CLASSES ++;


	bcode = 0;
        printf ("\n");
        if (count_bck > 0) {
           printf ("\n... %d cells of background exterior to the landscape found",
	      count_bck);
	   bcode ++;
	}
        if (count_intbck > 0) {
           printf ("\n... %d cells of background interior to the landscape found",
	      count_intbck);
	   bcode ++;
	}
	if (count_bck == 0 && count_intbck == 0)
	   printf ("\n... landscape does not contain background");

}
