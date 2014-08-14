/*************************************************************
 *  File Name:  WRITEASC.C
 *
 *  Purpose:  Writes an ascii image file. Each output record
 *	contains one row of image data.  Data values are 
 *	separated by spaces (columns are not aligned).
 *
 *  Passed Parameters:
 *	filename:    name of the file containing the ascii data
 *	image_ptr:   pointer to the image to fill with this data
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  7 December 1992
 *
 *  ID: $Id: writeasc.c,v 2.1 1994/04/01 17:55:55 marks Exp $
 **************************************************************/
#include <stdio.h>
#include <string.h>
#include "stats.h"


void write_ascii (filename,image_ptr)
char	*filename;
short   *image_ptr;
{
	short	i,j;
	FILE	*fp;
	

	if ((fp = fopen(filename,"w")) == NULL) {
		printf("\nERROR writing file: %s\n",filename);
		exit(-1);
	}

	for (i=0; i < num_rows; i++) {
  	   for (j=0; j < num_cols; j++) 
	        fprintf (fp,"%d ",getshort(image_ptr,num_cols,i,j));
	   fprintf (fp,"\n");
	}

	fclose (fp);
}
