/**********************************************************************
 *  File Name:  WRITEIMG.C
 *
 *  Purpose:  Sets the output ID filename and calls the appropriate
 *	routine for writing it.
 *
 *  Passed Parameters:
 *	max_patch_id:    the largest patch ID assigned to a patch in
 *			 the landscape.
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  December 1992
 * 
 *  ID:  $Id: writeimg.c,v 2.3 1994/11/08 15:02:05 marks Exp marks $
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"
#include "erdas.h"


void write_image (max_patch_id)
short	max_patch_id;
{

	short  packtype;
	char   *ptr,*out;
	static char   outname[60] = " "; 



/*
 *  3-1-94 BJM  Per Eric Gustafson's comments, the output ID filename
 *	is being changed.  DOS does not support a filename like
 *	"test.dat.ID".  The new output ID filename will consist of
 *	all characters in the input file name up to the first dot
 * 	encountered, plus the extension ".ID".  For example, the ID 
 *      filename for an input file named test.dat would be test.ID.
 */

	ptr = imagename;
	out = outname;

	while (*ptr && *ptr != '.') 
	        *out++ = *ptr++;


/*
 *  11-8-94 BJM  Error discovered!!  Does not work if the input filename
 *  does not contain a ".".  The output file gets the same name as the 
 *  input -- the .ID does not get appended to the name.  So the input
 *  image was overwritten!  (Used to be "strcat(out,".ID"))
 */
        strcat(outname,".ID");
	printf ("\nOutput ID file is: %s\n",outname);


	switch (data_type) {
	   case 1:                      /*  SVF file   */
              write_svf(outname,id);
	      break;

	   case 2:                      /*  Ascii file */
              write_ascii (outname,id);
	      break;

	   case 3:                      /* 8-bit binary file */
              if (max_patch_id > 255) {
                 printf ("\nOutput ID file will be 16 bit -- patch IDs");
                 printf ("\nexceed 255 [limit of an 8 bit image].");
		 write_binary (outname,id,2);
              }
	      else 
                 write_binary (outname,id,1);

	      break;
          
	   case 4:                      /* 16 bit binary file */
	      write_binary (outname,id,2);
	      break;
 
           case 5:                      /* ERDAS file   */
/*
 *  3-10-95  ERROR!  Used to say packtype 0 was 4 bit image,
 *  packtype 1 was 8 bit.  An 8 bit image is packtype 0 and
 *  a 4-bit image is packtype 1!  Fixing!
 */
	      packtype = 1;             /* 4 bit image  */
	      if (max_patch_id > 15)
		 packtype = 0;          /* 8 bit image  */
	      if (max_patch_id > 255)
		 packtype = 2;          /* 16 bit image */
              
	      write_erdas (outname,id,packtype);
	      break;

	    case 6:                     /* IDRISI file  */
	      packtype = 1;             /* 8 bit image  */
	      if (max_patch_id > 255)
		 packtype = 2;          /* 16 bit image */

              write_idrisi(outname,id,2);
	      
	}

}
