/*******************************************************
 *  File Name:  READIMG.C
 *
 *  Purpose:  The first time this routine is called it reads
 *	the input image from disk and stores it as orig_image,
 *	as well as image.  Any subsequent calls will just
 *	copy the image orig_image to image.
 *
 *  Passed Parameters:
 *	inmap:   image to be copied
 *	outmap:  copy of inmap
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  December 1992
 *
 *  ID:  $Id: readimg.c,v 2.5 1995/01/18 17:31:03 marks Exp marks $
 *
 *  Change history:
 *    Added an optimization so that image is stored and
 *    retreived from memory, rather than from disk.
 *    This (hopefully) will improve performance, especially
 *    on machines with slow disks (like PC's).
 *    Tom Moore, Sept. 28/93
 ********************************************************/	 
#include <stdio.h>
#include <stdlib.h>
#include "stats.h"

static void copyimage();

void read_image (alter_orig)
char	alter_orig;
{

  	static short *orig_image=(void *)0;


        if (!orig_image) {
	   orig_image = (short *) calloc ((unsigned)num_rows*num_cols,sizeof(short));
	   if (orig_image == NULL) {
	      printf ("\nERROR! Can not allocate space for image copy\n");
	      exit(-1);
	   }

	   switch (data_type) {
  	      case 1:
                read_svf (imagename,image,&min_class,&max_class);
		break;

	      case 2:
                read_ascii (imagename,image,&min_class,&max_class);
		break;

	      case 3:
                read_binary (imagename,image,data_type-2,&min_class,
		   &max_class);
		break;

	      case 4:
                read_binary (imagename,image,data_type-2,&min_class,
		   &max_class);
		break;

	      case 5:
	        read_erdas (imagename,image,&min_class,&max_class);
		break;

	      case 6:
                read_idrisi (imagename,image,&min_class,&max_class);
		break;
	   }

	   copyimage(image,orig_image);
	}

	else if (alter_orig)
	   copyimage(image,orig_image);

	else 
	  copyimage(orig_image,image);

}


static void copyimage(in,out)
short *in, *out;
{
  	int i;

  	for (i=0; i<num_rows*num_cols; i++)
    	   *out++ = *in++;
}
