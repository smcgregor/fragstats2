/*******************************************************************
 *  File Name: READERD.C
 *
 *  Program Purpose:  Reads ERDAS data.
 *
 *  Passed Parameters:
 *	filename:   name of erdas file
 *	imageptr:   pointer to the image to fill with this data
 *
 *  Returned Parameters:
 *	min:  	    minimum class (>= 0) in the input image
 *      max:        maximum class in the input image 
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  March 1990
 *
 *  ID:  $Id: readerd.c,v 2.4 1994/10/05 16:07:17 marks Exp marks $
 *
 *  Modifications:
 *    Modified by T.Moore - January 7, 1994
 *  	Binary data are stored in Intel byte swapped format.  On
 * 	Sparcs (Sun worsktations) this means that byte swapping is 
 *  	required on input and output.  For the DOS version, no
 *  	swapping is required.  I have used the DJGPP automatically
 *  	defined compiler variable __GO32__ to detect if this
 *  	is being compiled on a DOS machine, and to disable the
 *  	byte swapping.  When compiled on all other platforms 
 *  	byte swapping will be enabled.  It is likely that
 *  	other architectures are the same as Intel, and will
 *  	also have to have byte swapping disabled, but I don't
 *  	know what they are.
 * 
 ********************************************************************/
#include <stdio.h>
#include "erdas.h"
#include "stats.h"


void read_erdas (filename,imageptr,min,max)
char	*filename;
short	*imageptr;
short	*min,*max;
{
	FILE	*fp;
	int	i,j;
	short	value;
	short	*ptr;
	short 	ibands;
	short   ipack;
	short   val;
	short	swapped_val;
	int	count,size;
	int	count_bck,count_intbck;


	count_bck = count_intbck = 0;


	if ((fp = fopen (filename,"rb")) == NULL) {
		printf ("\n\tERROR!! Can not open file: %s",filename);
		exit(-1);
	}

/*
 *  Read erdas image header
 */
	fread (&imagehd,sizeof(struct image_header),1,fp);

/*
 *  For some reason, ERDAS decided to swap the order of bytes.
 *  Unswap them to get number of bands in the input image.
 */

#ifdef __GO32__
	ibands = imagehd.nbands;
#else
	swab (&imagehd.nbands,&ibands,2);
#endif

	if (ibands != 1) {
	   printf ("\nERROR!  This image has more than 1 band!");
	   printf ("\n  This program can only read single band images!");
	   exit(-1);
	}

/*
 *  Again, swap the bytes to get the "pack type" (either 4, 8, or 16-bit)
 */

#ifdef __GO32__
	ipack = imagehd.pack_type;
#else
	swab (&imagehd.pack_type,&ipack,2);
#endif

	if (ipack < 0 || ipack > 3) {
	   printf ("\nERROR! Invalid pack type: %d",ipack);
	   exit(-1);
	}
	
	*min = 9999;
	*max = -9999;
	ptr = imageptr;
	land_border = FALSE;
	MAX_CLASSES = -9999;
	total_size = 0;
	size = num_rows * num_cols;
	
/*
 *  Read 8 bit data, no swapping required.  Find min, max data values.
 */
	if (ipack == 0) {
	   printf ("\nReading 8 bit ERDAS image ....\n");

	   for (i=0; i < size; i++) {
	      *ptr = (short) fgetc(fp);

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

	      ptr ++;
	   }
	}

/*
 *  Read 4-bit data packed into 1 byte. No swapping required.
 */
        if (ipack == 1) {
	   printf ("\nReading 4 bit ERDAS image ...\n");

	   count = 0;
	   for (i=0; i < size; i+=2) {
	      for (j=0; j < 2; j++) {
	         if (j == 0) {
	            value = (short) fgetc(fp);
                    *ptr = value & 15;
	         }
		 else 
		    *ptr = value >> 4;

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

	         ptr ++;
	         count ++;
	         if (count >= size) break;
	      }
	   }
	}

/*
 *  Read 16-bit data; swapping required.
 */
	if (ipack == 2) {
	   printf ("\nReading 16 bit ERDAS image ...\n");

	   fread (imageptr,sizeof(short),size,fp);

	   for (i=0; i < size; i++) {
	      swapped_val = *ptr;
#ifdef __GO32__
	      val = swapped_val;
#else
	      swab(&swapped_val,&val,2);
#endif
	      *ptr = val;

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
	      ptr ++;
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
