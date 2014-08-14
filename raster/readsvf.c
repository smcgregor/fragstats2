/**************************************************************
 *  Function Name:  READSVF
 *
 *  Purpose:  Reads Arc/Info SVF file and puts the image data
 *  	into the array pointed to by "image_ptr".
 *
 *  Passed Parameters:
 *	filename:    name of SVF file
 *	image_ptr:   pointer to image to fill with this data
 *    
 *  Returned Parameters:
 *	min: 	     returned; minimum class value in this dataset
 *	max:         returned; maximum class value in this dataset
 *
 *  Programmer:  Barbara Marks
 *  
 *  Date:  26 October 1990
 *
 *  ID:  $Id: readsvf.c,v 2.6 1994/10/05 16:07:42 marks Exp marks $
 ***************************************************************/
#include <stdio.h>
#include "stats.h"

void read_svf(filename,image_ptr,min,max)
char	*filename;
short	*image_ptr;
short   *min,*max;
{
	FILE	*fp;	
	int	i,j,k;
	int	count_intbck,count_bck;
	short	npairs,value;
	short	*ptr;
	char	rowflag;
	unsigned char   repeat;
	

	
	if ((fp = fopen(filename,"rb")) == NULL) {
		printf ("\nERROR reading file: %s\n",filename);
		exit(-1);
	}

	ptr = image_ptr;

/*
 *  Read the Arc/Info SVF file (compressed format):
 *  	Record 1 contains the number of rows and columns in the image.
 *	Records 2-n contain the image image in the following format.
 *		rowstart flag (-1) - indicates the start of an image
 *			row [char]
 *		npairs  - the number of pairs of (repeat,value) for 
 *		 	this row [short]
 *		repeat value - number of times the next value is to be
 *			repeated [char]
 *		image value [short]
 */
	fread (&num_rows,sizeof(short),1,fp);
	fread (&num_cols,sizeof(short),1,fp);


/*
 *  total_size is the number of cells inside the landscape (this
 *  does not include interior background or landscape border cells).
 */
        *min = 9999;
	*max = -9999;
        total_size = 0;
	land_border = FALSE;
	MAX_CLASSES = -9999;
	count_bck = count_intbck = 0;

	for (i=0; i < num_rows; i++) {	
		fread(&rowflag,sizeof(char),1,fp);
		if (rowflag != -1 ) {
		   printf("\nROWFLAG not -1:  %d",rowflag);
		   printf ("\nFile [%s] probably not an SVF file\n",filename);
		   exit(-1);
		}
		fread (&npairs,sizeof(short),1,fp);
		for (j=0; j < npairs; j++) {
		   fread (&repeat,sizeof(char),1,fp);
		   fread (&value,sizeof(short),1,fp);

/*
 *  If there are negative cell values that are not background,
 *  then this image includes a landscape border.  Set a flag.
 */
	 	   if (value < 0 && value != background && value != -background)
                      land_border = TRUE;

/*
 *  Keep track of the minimum and maximum class value inside the
 *  landscape.  
 */
		   if (value != background && value != -background) {
		      if (value < *min && value >= 0) *min = value;
		      if (value > *max) *max = value;

/*
 *  If the image includes a landscape border, for contagion we
 *  need to know the number of different classes in the landscape
 *  including those in the landsape border.
 */
	              if (value >= 0 && value > MAX_CLASSES)
		         MAX_CLASSES = value;
		      if (value < 0 && -value > MAX_CLASSES)
			 MAX_CLASSES = -value;
		   }

/*
 *  The value for background cells was input by the user.  Set those 
 *  background cells interior to the landscape to -990.  Set those 
 *  background cells in the landscape border, or outside the area of 
 *  interest to -999.  This makes life easier in several other routines.
 */
		   if (value == -background) {
		      count_bck += repeat;
		      value = -999;
		   }

		   if (value == background) {
		      count_intbck += repeat;
		      value = -990;
 		   }
		   if (value >= 0) total_size += repeat;


		   for (k=0; k < repeat; k++) 
			*ptr++ = value;

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
	if (count_bck == 0 && count_intbck == 0) 
	   printf ("\n... landscape does not contain background");

}
