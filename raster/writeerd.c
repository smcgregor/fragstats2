/*******************************************************************
 *  File Name: WRITEERD.C
 *
 *  Program Purpose:  Writes the ID image in ERDAS format.
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  March 1990
 *
 *  ID:  $Id: writeerd.c,v 2.1 1994/04/01 17:56:18 marks Exp $
 *
 *  Modifications:
 *    Modified by T.Moore - January 7, 1994
 *  	Binary data are stored in Intel byte swapped format.  On
 *  	Sparcs (Sun worsktations) this means that byte swapping is 
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
 *   3-10-95 Barbara Marks
 *	Bug in the way a 4 bit ERDAS file was output!
 *
 ********************************************************************/
#include <stdio.h>
#include "erdas.h"
#include "stats.h"


void write_erdas (filename,imageptr,packtype)
char	*filename;
short	*imageptr;
short	packtype;
{
	FILE	*fp;
	short   val1,val2,val3;
	short	*ptr;
	int     i;
	int	value;
	int	count;
	int	size;


	if ((fp = fopen (filename,"wb")) == NULL) {
		printf ("\n\tERROR!! Can not open file: %s",filename);
		exit(-1);
	}

/*
 *   Write the pack type to the image header
 */

#ifdef __GO32__
	imagehd.pack_type = packtype;
#else
	swab (&packtype,&imagehd.pack_type,2);
#endif

/*
 *  Write erdas image header
 */
	fwrite (&imagehd,sizeof(struct image_header),1,fp);

	size = num_rows * num_cols;
	ptr = imageptr;

/*
 *  Write 8 bit data, no swapping required.
 */
        if (packtype == 0) {
           printf ("\nWriting ID image -- 8 bit ERDAS file ....\n");

           for (i=0; i < size; i++) {
	      fputc ((int)*ptr,fp);
	      ptr ++;
           }   
        }   
 
/*
 *  Write 4-bit data, packing required.
 *
 *  3-10-95 BJM  The way this was originally coded never worked! The
 *  output image was the same size as an 8 bit image.  
 */
        if (packtype == 1) {
           printf ("\nWriting ID image -- 4 bit ERDAS file ...\n");

	   count = 0;
           for (i=0; i < size; i+=2) {
              val1 = *ptr++;
	      count ++;
	      if (count == size) 
	         val2 = 0;
	      else
                 val2 = *ptr++;
	      count++;

	      val3 = val2 << 4;
	      value = val3 | val1;

	      fputc (value,fp);
           }
        }  
 
/*
 *  Write 16-bit data; swapping required on Sparc.
 */

	if (packtype == 2) {

#ifndef __GO32__

	   for (i=0; i < size; i++) {
		 val1 = *ptr;
		 swab(&val1,&val2,2);
		 *ptr++ = val2;
	   }
#endif
	

	   fwrite (imageptr,sizeof(short),size,fp);
	}

	fclose (fp);
}
