/***************************************************************
 *  Filename:  READBIN.C
 *
 *  Purpose:  Read an input 8 or 16 bit binary image file into
 *	the array pointed to by "image_ptr".
 *
 *  Passed Parameters:
 * 	filename:  name of input image data file
 *	image_ptr: pointer to image to fill with this data
 *      option:    type of binary file:  
 *	 	   1 - 8 bit    2 - 16 bit
 *
 *  Returned Parameters:
 *	min:   minimum data value (>= 0) in image
 *	max:   maximum data value in image
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  11 December 1992
 *
 *  ID: $Id: readbin.c,v 2.4 1994/10/05 16:07:05 marks Exp marks $
 ***************************************************************/
#include <stdio.h>
#include "stats.h"


void read_binary (filename,image_ptr,option,min,max)
char  *filename;
short *image_ptr;
short option;
short *min,*max;
{

	FILE	*fp;
	char	cval;
	short	*ptr;
	int	i,j;
	int	count_bck,count_intbck;



	count_bck = count_intbck = 0;
	*min = 9999;
	*max = -9999;
	total_size = 0;
	land_border = FALSE;
	MAX_CLASSES = -9999;
	ptr = image_ptr;

/*
 *  total_size is defined in the include file stats.h.  total_size is
 *  the number of cells inside the landscape (non-background cells).
 */
	total_size = 0;

/*
 *  Open input image file
 */
	if ((fp = fopen(filename,"rb")) == NULL) {
		printf ("\nERROR! Can not open file: %s\n",filename);
		exit(-1);
	}

/*
 *  Read 8 bit binary stream file
 */

	if (option == 1) {
	   for (i=0; i < num_rows*num_cols; i++) {
		cval = fgetc(fp);
		*ptr = (short) cval;

/* 
 *  If there are cell values < 0 and not background, then this
 *  image includes a landscape border.  Set a flag.
 */
	        if (*ptr < 0 && *ptr != background &&
                    *ptr != -background) land_border = TRUE;

/* 
 *  Find the minimum and maximum class values (only consider those
 *  classes inside the landscape).  
 */
	        if (*ptr != background && *ptr != -background) {
		    if (*ptr < *min && *ptr >= 0) *min = *ptr;
		    if (*ptr > *max) *max = *ptr;
		    if (*ptr >= 0) total_size ++;

/*
 *  Keep track of the maximum class found in the landscape, either
 *  positive or negative.  These is needed by the routine getsizes.c
 *  to allocate space for arrays.
 */
	           if (*ptr >= 0 && *ptr > MAX_CLASSES)
		      MAX_CLASSES = *ptr;
	           if (*ptr < 0 && -(*ptr) > MAX_CLASSES)
		      MAX_CLASSES = -(*ptr);
		}

/*
 *  The value of background cells was specified by the user.  Change
 *  interior background cells to -990 (positive background value) and
 *  set background cells exterior to the landscape of interest to -999
 *  (these should have a negative background value).
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
	}

/*
 *  Read 16 bit binary stream file
 */
	else {
	   fread (image_ptr,sizeof(short),num_rows*num_cols,fp);
	   for (i=0; i < num_rows; i++) {
	      for (j=0; j < num_cols; j++) {

	        if (*ptr < 0 && *ptr != background && *ptr != -background)
                   land_border = TRUE;
                if (*ptr != background && *ptr != -background) {
	           if (*ptr < *min && *ptr >= 0) *min = *ptr;
	           if (*ptr > *max) *max = *ptr;
		   if (*ptr >= 0) total_size ++;
			
	   	   if (*ptr >= 0 && *ptr > MAX_CLASSES)
		      MAX_CLASSES = *ptr;
		   if (*ptr < 0 && -(*ptr) > MAX_CLASSES)
		      MAX_CLASSES = -(*ptr);
		}

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
	   }
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
	if (count_intbck == 0 && count_bck == 0)
	   printf ("\n... landscape does not contain background");

}
 
