/***************************************************************
 *  Filename:  WRITEBIN.C
 *
 *  Purpose:  Writes an 8 or 16 bit binary file depending on
 *	the "option" flag passed.
 *
 *  Passed Parameters:
 * 	filename:  name of output data file
 *	image_ptr: pointer to image to write
 *      option:    type of binary file:  
 *	 	   1 - 8 bit    2 - 16 bit
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  11 December 1992
 *
 *  ID:  $Id: writebin.c,v 2.1 1994/04/01 17:56:06 marks Exp $
 ***************************************************************/
#include <stdio.h>
#include "stats.h"


void write_binary(filename,image_ptr,option)
char  *filename;
short *image_ptr;
short option;
{
	FILE	*fp;
	short	i;


/*
 *  Open output image file
 */
	if ((fp = fopen(filename,"wb")) == NULL) {
		printf ("\nERROR! Can not open file: %s\n",filename);
		exit(-1);
	}

/*
 *  Write 8 or 16 bit binary stream file
 */
	if (option == 1) {
	   printf ("\nWriting 8 bit binary file ... \n");
	   for (i=0; i < num_rows*num_cols; i++) {
	      fputc ((unsigned char)*image_ptr,fp);
	      image_ptr ++;
	   }
	}

	else {
	   printf ("\nWriting 16 bit binary file ... \n");
	   fwrite (image_ptr,sizeof(short),num_rows*num_cols,fp);
	}

	fclose (fp);
}
 
