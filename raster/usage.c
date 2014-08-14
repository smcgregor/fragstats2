/********************************************************************
 *  File Name:  USAGE.C
 *
 *  Purpose:  Prints to the screen the usage of the program 
 *	fragstats (i.e., the command line arguments and a 
 *	short description of each one).
 *
 *  Programmer:  Barbara Marks
 * 
 *  Date:  January 1993
 *
 *  ID: $Id: usage.c,v 2.4 1994/04/06 14:27:53 marks Exp marks $
 *******************************************************************/ 
void usage ()
{


	printf ("\n\nUSAGE: ");
	printf ("\n   fragstats in_image out_file cellsize edge_dist data_type [rows]");
	printf ("\n   [cols] [background] [max_classes] [wt_file] [id_image] [desc_file]");
	printf ("\n   [bound_wght] [diags] [prox_dist] [nndist] [patch_stats] [class_stats]");

	printf ("\n\nin_image:    name of input image file {char}");
	printf ("\nout_file:    basename for output files {char}");
	printf ("\ncellsize:    size of cells in meters in in_image {float}");
	printf ("\nedge_dist:   distance from edge in meters for core area {float}");
	printf ("\ndata_type:   type of input image file {int}:");
	printf ("\n             1 - SVF             2 - ascii  3 - 8 bit binary");
	printf ("\n             4 - 16 bit binary   5 - ERDAS  6 - IDRISI");
	printf ("\nrows:        optional; #rows in image; required if data_type 2,3,4 {int}");
	printf ("\ncols:        optional; #cols in image; required if data_type 2,3,4 {int}");
        printf ("\nbackground:  optional; value of background cells (positive) {int} {NONE]");
	printf ("\nmax_classes: optional; max possible classes in landscape {int} [NONE]");
        printf ("\nwt_file:     optional; name of file containing weights for each");
	printf ("\n             combination of patch type {char} [NONE]");
	printf ("\nid_image:    optional; method for assigning patch IDs:");
	printf ("\n             1 - create and output ID image");
	printf ("\n             2 - do not output an ID image [default]");
	printf ("\n               - name of ID image; data_type must be same as in_image");
	printf ("\ndesc_file:   optional; name of file containing character descriptors");
	printf ("\n             for each patch type {char} [NONE]"); 
	printf ("\nbound_wght:  optional; proportion of landscape boundary and background");
	printf ("\n             class edges to be considered edge {float} [0.0]");
	printf ("\ndiags:       optional; use diagonals in patch finding {y/n} [YES]");
	printf ("\nprox_dist:   optional; search radius in meters for proximity {float} [NONE]");
	printf ("\nnndist:      optional; calculate nearest neighbor {y/n} [YES] ");
	printf ("\npatch_stats: optional; print patch level stats {y/n} [YES]");
	printf ("\nclass_stats: optional; print class level stats {y/n} [YES]");
	printf ("\n\nNOTE:  use a $ in place of skipped optional parameters");

	printf ("\n\n");

}
