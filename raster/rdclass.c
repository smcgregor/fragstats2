/*********************************************************************
 *  File Name:  RDCLASS.C
 *
 *  Purpose:  Reads in a file of descriptors for each patch type
 *	(class).  The file must be ascii and each record must
 *	contain the numeric class number, and a descriptive 
 *	name for the class. Commas or spaces can be used as 
 *	delimiters.  For example:
 *		1, woodland
 *		2, brush
 *		3, conifers
 *
 *	The parameter MAX_LABEL_LENGTH in the file stats.h 
 * 	controls the printed length of labels in the output
 *  	files.  Note that if MAX_LABEL_LENGTH exceeds 22, the
 *	columns will no longer be aligned in the .full file.
 *	
 *  Passed Parameters:
 *	filename:    name of ascii file containing descriptive
 *		     patch type (class) names.
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1993
 *
 *  ID: $Id: rdclass.c,v 2.1 1994/04/01 17:51:01 marks Exp $
 *********************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"

void read_classnames (filename)
char	*filename;
{
	FILE	*fp;
	char	line[105];
	char	*classname;
	short	classnum;
	int	i;

	printf ("\n ... reading classnames file: %s ... \n\n",filename);

	if ((fp = fopen(filename,"r")) == NULL) {
	   printf ("\nERROR! Can not open file: %s\n",filename);
	   printf ("\n  exiting .....");
	   exit(-1);
	}


	while (!feof(fp)) {
	   fgets(line,100,fp);
	   if (feof(fp)) break;
	   classnum = atoi(strtok(line," ,"));
	   classname = strtok('\0'," ,\n");
/*
 *  Only save the IDs for the classes that exist in the landscape.
 */
	   for (i=0; i < NUM_CLASSES; i++) 
	      if (classnum == patchtype[i]) 
	         set_char(name,i,classname);
	}

	fclose (fp);
} 
