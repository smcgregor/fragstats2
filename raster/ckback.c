/***************************************************************************
 *  File Name:  INTBACK.C
 *
 *  Purpose:  This routine is called to change the values of all 
 *	background patches interior to the landscape to -990.  This
 *	must be done to correctly calculate edge lengths.
 *  
 *  Passed Parameters:
 *	imageptr: 	pointer to the input image
 *
 *  Programmer:  Barbara Marks
 * 
 *  Date:  18 March 1994
 *
 *  ID:  $Id: ckback.c,v 1.3 1995/04/12 19:19:14 marks Exp marks $
 *****************************************************************************/
#include "stats.h"

void check_background (imageptr)
short	*imageptr;
{
	short   *temp;
	short 	i,j;
	short   x,y,xptr,yptr;
	short 	value;
	short	neighbor;
	int	n;
	int	k;
	int	numpts;
	char	exterior_background;
	char	interior_background;
	char	image_edge;
	char	positive,negative;

	
	printf ("\nVerifying that background patches are classified correctly ....\n");

	for (i=0; i < num_rows; i++) {
	   for (j=0; j < num_cols; j++) {

	      exterior_background = FALSE;
	      interior_background = FALSE;
	      positive = FALSE;
	      negative = FALSE;

/*
 *  Look for a background patch in the image.  The "read" routines set
 *  all exterior background cells to -999 and interior background cells
 *  to -990.  Verify that the -999 cells are exterior and the -990 cells
 *  are interior.  Change if not and report to the user.
 */
	      value = getshort (imageptr,num_cols,i,j);
	      if (value == -999 || value == -990) {

	         numpts = get_patch(j,i,value);

		 for (k=0; k < numpts; k++) {
		    x = stackx[k];
		    y = stacky[k];
		    image_edge = FALSE;

		    for (n=0; n < 4; n++) {
		       xptr = x + xpos[n];
		       if (xptr < 0 || xptr >= num_cols) image_edge = TRUE;
		       yptr = y + ypos[n];
	 	       if (yptr < 0 || yptr >= num_rows) image_edge = TRUE;

/*
 *  If an image edge is encountered, then this patch could not be interior
 *  to the landscape.
 */
		       if (image_edge) {
 		          exterior_background = TRUE;
			  break;
		       }

/*
 *  Go through all cells in the patch.  If an image edge is never reached, 
 *  and the patch is surrounded by patches with positive values, the patch 
 *  is interior.  If a background patch is bordered by both positive and
 *  negative patches, then the patch is on the boundary and there is a
 *  problem.  Its impossible to tell whether the patch in in the border
 *  or interior to the landscape.  Issue a warning about these patches and
 *  continue.
 */
		       neighbor = getshort (imageptr,num_cols,yptr,xptr);
		       if (neighbor == -777) continue;

		       if (!positive && neighbor >= 0) positive = TRUE;
		       if (!negative && neighbor < 0)  negative = TRUE;

		    }  /* end neighbors loop (n)  */

		    if (exterior_background) break;

		 }   /* end numpt points loop (k) */

/*
 *  Verify the patch has been classified correctly as interior or exterior.
 */
		 if (exterior_background) {
		    negative = TRUE;
		    positive = FALSE;
	         }
	         else {
	 	    if (positive && !negative) interior_background = TRUE;
		    if (negative && !positive) exterior_background = TRUE;
		 }


		 if (positive && negative)  {
		    printf ("\n\n   WARNING!  A background patch containing the cell row %d",i);
		    printf ("\n   col %d (size: %d) borders the landscape boundary.  It has been",
                       j,numpts);
		    if (value == -990) printf ("\n   classified as interior background.");
		    if (value == -999) printf ("\n   classified as exterior background.");
		    printf ("  If this classification is wrong ");
		    printf ("\n   these indices may be wrong:  total edge, edge density, landscape");
		    printf ("\n   shape, contrast weighted edge density, and total edge contrast index.");

		    if (background < 0) {
			printf ("\n   Fragstats expects a positive value for background and you");
		        printf ("\n   entered %d which would cause these problems.\n",background);
		    }

		 }

		 if (interior_background && value != -990 ) {
		    printf ("\n   WARNING! reclassifying exterior background patch to interior -");
	            printf ("  %d cells",numpts);
		    for (k=0; k < numpts; k++)
		       setshort (imageptr,num_cols,stacky[k],stackx[k],-890);
		 }
		 if (exterior_background && value != -999 ) {
		    printf ("\n   WARNING! reclassifying interior background patch to exterior -");
	            printf (" %d cells",numpts);
		    for (k=0; k < numpts; k++)
		       setshort (imageptr,num_cols,stacky[k],stackx[k],-899);
		 }

	      }  /* end if value == -999 || value == -990 loop  */
	   }     /* end num_cols loop  */
	}        /* end num rows loop  */

	printf ("\n");
/*
 *  get_patch sets all patch cells to -777; set these back to background (-999)
 *  before saving the image.
 */

	temp = imageptr;
	for (n=0; n < num_rows*num_cols; n++) {
	   if (*temp == -777) 
	      *temp = -999;
	   else if (*temp == -890) 
 	      *temp = -990;
	   else if (*temp == -899) 
	      *temp = -999;
	   temp ++;
	}
	

/*
 *  Call read_image with the image_edge set to 1; this will force the image
 *  "image" to be copied to "orig_image".  Anytime this program needs a 
 *  clean copy of the input image it gets "orig_image".  "orig_image"
 *  will now include these interior background patches.
 */

	read_image (1);
}
