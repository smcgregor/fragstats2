/*******************************************************************
 *  Program Name:  READ_ERDAS_HEADER
 *
 *  Program Purpose:  Reads the number of rows and columns from
 *	the header of an arc/info SVF file or and ERDAS image file.
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  September 1993
 *
 *  ID: $Id: readhd.c,v 2.1 1994/04/01 17:51:59 marks Exp $
 *
 *  Modifications:
 *	10-93 Tom Moore
 *	   Added code for IDRISI file header
 *
 ********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "erdas.h"
#include "stats.h"

void read_header (filename)
char 	*filename;
{
	FILE	*fp;
	short	i;
	union union_type_int {
	   int b;
	   char a[4];
	};
	union union_type_int ivar;

	char 	*wholename, record[100], *token;
	double 	min_x, max_x, min_y, max_y, xsize, ysize;


/*
 *  Read header from IDRISI file  (added 10-93 by Tom Moore)
 */
	if (data_type == 6) { /* IDRISI format */
	  wholename = (char* ) malloc(strlen(filename)+5);
	  if (!wholename) {
	    printf("\nERROR allocating space for filename %s\n",filename);
	    exit(-1);
	  }
	  strcpy(wholename,filename);
	  strcat(wholename,".doc");
	  if ((fp = fopen(wholename,"r")) == NULL) {
	    printf("\nERROR reading file: %s\n",wholename);
	    exit(-1);
	  }
	  
	  /* read the input parameters from the IDRISI file */
	  while(fgets(record,100,fp)) {
	    token = strtok(record,":");
	    /* Number of columns */
	    if (!strncmp(token,"columns",7)) 
	      num_cols = atoi(strtok(NULL,"\n"));
	    /* rows ... */
	    else if (!strncmp(token,"rows",4)) 
	      num_rows = atoi(strtok(NULL,"\n"));
	    else if (!strncmp(token,"min. X",6)) 
	      sscanf(strtok(NULL,"\n"),"%lf",&min_x);
	    else if (!strncmp(token,"max. X",6)) 
	      sscanf(strtok(NULL,"\n"),"%lf",&max_x);
	    else if (!strncmp(token,"min. Y",6)) 
	      sscanf(strtok(NULL,"\n"),"%lf",&min_y);
	    else if (!strncmp(token,"max. Y",6)) 
	      sscanf(strtok(NULL,"\n"),"%lf",&max_y);
	  }
	  xsize = (max_x - min_x) / num_cols;
	  ysize = (max_y - min_y) / num_rows;
	  if (xsize == ysize) 
	    cellsize = xsize;
	  else {
	    printf("\nERROR  cells are not square\n");
	    exit(-1);
	  }
	  free(wholename);
	  return;
	}


/*
 *  Open image file
 */
        if ((fp=fopen(filename,"rb")) == NULL) {
           printf ("\nERROR opening image file: %s\n",filename);
           exit(-1);
        }


/*
 *  Read header from an SVF file
 */
	if (data_type == 1) {
           fread (&num_rows,sizeof(short),1,fp);
           fread (&num_cols,sizeof(short),1,fp);
           fclose (fp);
        }  
       
/*
 *  Read header from an ERDAS file.  ERDAS outputs "ints" in the
 *  reverse order from SUN, so a conversion must be made (i.e.,
 *  ERDAS byte order: 4,3,2,1 and SUN byte order: 1,2,3,4).
 */
	if (data_type == 5) {
	   fread (&imagehd,sizeof(struct image_header),1,fp);

	   for (i=0; i < 4; i++)
	      ivar.a[i] = imagehd.rrows >> (i * 8);
	   num_rows = ivar.b;
	
	   for (i=0; i < 4; i++)
	      ivar.a[i] = imagehd.rcols >> (i * 8);
	   num_cols = ivar.b;
	}

	fclose (fp);
	printf ("\nNumber of rows, cols: %d, %d",num_rows,num_cols);

}
