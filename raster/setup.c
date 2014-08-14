/***************************************************************
 *  File Name:  SETUP.C
 *
 *  Purpose:  Gets required inputs, allocates memory for arrays,
 *	reads input image(s), and opens output files.
 *
 *  Passed arguements:
 *	argc:  	number of command line arguements
 *	argv:   command line arguements
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  August 1993
 *
 *  ID: $Id: setup.c,v 2.2 1994/04/06 15:28:28 marks Exp marks $
 ***************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stats.h"


void setup(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp,*out;
	char	newfiles;
	char	answer[5];
	int	size;
	time_t	lt;
	struct  tm *time_ptr;



/*
 *  Get command line arguments.  If there are no arguments, call a
 *  routine that will prompt the user for all the required inputs. 
 *  If there are fewer arguements than required, issue the usuage 
 *  message.
 */
	if (argc == 1)
	   prompt();
	else {
	   if (argc < 6) {
    	      usage();
	      exit(-1);
	   }

	   strcpy (imagename,argv[1]);      /* input image name          */
    	   strcpy (out_file,argv[2]);       /* basename for output files */
	   cellsize = atof(argv[3]);        /* size of cell in meters    */
	   edge_dist = atof(argv[4]);       /* dist from edge - core area*/
	   data_type = atoi(argv[5]);       /* type of input image file: 
                                               svf, ascii, binary)       */
	   if (data_type < 1 || data_type > 6) {
	      printf ("\n\nERROR! Unsupported data type!\n\n");
	      exit(-1);
	   }

	   if (data_type >= 2 && data_type <= 4 && argc < 8) {
	      usage();                       /* #rows, #cols missing     */
	      exit(-1);
	   }
	   if (data_type >= 2 && data_type <= 4) {
	      num_rows = atoi(argv[6]);     /* number of rows in image   */
	      num_cols = atoi(argv[7]);     /* number of columns in image*/
	   }
	   background = 999;
	   if (argc >= 9) {
	      if (strcmp(argv[8],"$") != 0)
   	         background = atoi(argv[8]);/* value of background cells */
           }
	   do_patchrich = FALSE;
	   if (argc >= 10) {
	      if (strcmp(argv[9],"$") != 0) {
	         max_num_classes = atoi(argv[9]); /* max possible classes*/
	         do_patchrich = TRUE;
	      }
	   }	    
	   contrast_indices = FALSE;
           if (argc >= 11) {
	      if (strcmp(argv[10],"$") != 0) { 
	         strcpy(weight_file,argv[10]);  /* class weights filename*/
	         contrast_indices = TRUE;
		 if ((fp=fopen(weight_file,"r")) == NULL) {
		    printf ("\nERROR! Can not open file: %s\n",weight_file);
		    exit(-1);
		 }
		 fclose (fp);
	      }
 	   }
	   id_image = 2;
	   if (argc >= 12) {
	      if (strcmp(argv[11],"1") == 0)
		 id_image = 1;
	      else if (strcmp(argv[11],"$") == 0 || strcmp(argv[11],"2") == 0)
	         id_image = 2;
	      else {
	         strcpy(id_file,argv[11]);      /* name of ID image      */
		 id_image = 3;
		 if ((fp=fopen(id_file,"r")) == NULL) {
		    printf ("\nERROR! Can not open file: %s\n",id_file);
		    exit(-1);
		 }
		 fclose(fp);
	      }
	   }
	   descriptors = FALSE;
	   if (argc >= 13) {
	      if (strcmp(argv[12],"$") != 0) {
	         strcpy(desc_file,argv[12]);   /* class descriptors file */
	         descriptors = TRUE;
		 if ((fp=fopen(desc_file,"r")) == NULL) {
		    printf ("\nERROR! Can not open file: %s\n",desc_file);
		    exit(-1);
		 }
		 fclose(fp);
	      }
	   } 
   	   if (argc >= 14) {                   /* weight for boundary/back*/
	      bound_wght = atof(argv[13]);
	      if (bound_wght < 0.0 || bound_wght > 1.0) {
		 printf ("\nERROR! Invalid contrast weight: %f\n",bound_wght);
		 exit(-1);
	      }
	   }
	   use_diags = TRUE;
	   if (argc >= 15) {
	      strcpy (answer,argv[14]);     /* use diagonals for patches */
	      if (answer[0] == 'N' || answer[0] == 'n')
		 use_diags = FALSE;
	   }
	   proximity = FALSE;
	   if (argc >= 16) {
	      if (strcmp(argv[15],"$") != 0) {
		 search_radius = atof(argv[15]);/*proximity search radius*/
		 proximity = TRUE;
	      }
	   }
	   do_nndist = TRUE;
	   if (argc >= 17) {
	      strcpy (answer,argv[16]);     /* do nearest neighbor calcs */
	      if (answer[0] == 'N' || answer[0] == 'n')
		 do_nndist = FALSE;
	   }
	   patch_stats = TRUE;
	   if (argc >= 18) {
	      strcpy (answer,argv[17]);     /* write patch level stats    */
	      if (answer[0] == 'N' || answer[0] == 'n')
		 patch_stats = FALSE;
	   }
	   class_stats = TRUE;
	   if (argc == 19) {
	      strcpy (answer,argv[18]);     /* write class level stats    */
	      if (answer[0] == 'N' || answer[0] == 'n')
		 class_stats = FALSE;
	   }
	}
	
/* 
 *  Extra dependencies for IDRISI.  If the input file is IDRISI
 *  then descriptors are automatic.
 *  Tom Moore 10-93
 */
	if (data_type == 6) descriptors = TRUE;

/*
 *  If the user selected to calculate proximity indices and not 
 *  nearest neighbor indices, issue a message that the two go
 *  hand in hand -- you can't do one without the other.
 */
	if (proximity && !do_nndist) {
	   printf ("\n\nERROR!! The proximity indices require all");
	   printf ("\nthe nearest neighbor calculations.  You must"); 
	   printf ("\nselect to calculate nearest neighbor indices if");
	   printf ("\nyou want to calculate proximity indices!");
	   printf ("\nExiting ....\n");
	}
/*
 *  Print image file name to screen
 */
	printf ("\nProcessing image: %s",imagename);


/*
 *  If the input file is an arc/info SVF file, an ERDAS file, or
 *  an IDRISI file, read the number of rows and columns in the 
 *  image from the file header.  
 */
	if (data_type == 1 || data_type >= 5) 
	   read_header (imagename);


/*
 *  Convert the core area edge distance from meters to cells.
 *  "min_dist" is defined in stats.h.
 */
	min_dist = (int) (edge_dist / cellsize + .5);

/*
 *  Allocate space to hold the image file. 
 */
	size = num_rows * num_cols;

	image = (short *) calloc ((unsigned)size,sizeof(short));
	if (image == NULL) {
	   printf ("\nERROR! Can not allocate space for input");
	   printf ("\n  image!");
	   exit(-1);
	}

/*
 *  Allocate space for a patch ID image, if requested.
 */
	if (id_image != 2) {
	   id = (short *)calloc ((unsigned)size, sizeof(short));
	   if (id == NULL) {
	      	printf ("\nERROR! Can not allocate space - id map");
		exit(-1);
 	   }
	}

/*
 *  If a patch ID file was input, read it in.  
 */
	if (id_image == 3) {
	   if (data_type == 1)
		read_svf (id_file,id,&min_class,&max_class);
	   if (data_type == 2)
		read_ascii (id_file,id,&min_class,&max_class);
	   if (data_type == 3 || data_type == 4)
		read_binary (id_file,id,data_type-2,&min_class,&max_class);
	   if (data_type == 5)
		read_erdas (id_file,id,&min_class,&max_class);
	   if (data_type == 6)
		read_idrisi (id_file,id,&min_class,&max_class);
	}
	
	printf ("\nInterior Background Value: %d",background);
	printf ("\nExterior Background Value: %d",-background);

/*
 *  Read in image data
 */
	read_image (0,&bcode);


/*
 *  Read in weight file if one was specified
 */
	if (contrast_indices) read_weights(weight_file);

/*
 *  Find the number of patch types, and the maximum patch size in 
 *  the image.  These are needed to dynamically allocate memory for 
 *  several arrays.  (This requires a pass through the image and
 *  time to find all patches!)	
 */
	get_sizes ();

	allocate_memory();
	


/*
 *  If a file containing character descriptors for each class type was
 *  input, read it in.
 */
	if (descriptors)
	  if (data_type == 6)                  
             read_idrisi_desc(imagename, name); /* Tom Moore 10-93 */  
	  else 
	     read_classnames(desc_file);

/*
 *  7-19-94 BJM  If only negative or only postive background patches were
 *  found in the image, verify that these patches were classified correctly.
 *  If the user did not follow the convention of classifying internal 
 *  background patches as postive and external patches as negative, this
 *  routine tries to bail them out.  If both positive and negative backgroud
 *  patches were found, the user probably knows what he's doing, so don't
 *  bother checking.  (bcode is returned from the read routines:  0 is no
 *  background, 1 = positive or negative found, 2 = positive and negative 
 *  found)
 */
	if (bcode == 1) {
	   read_image (0);
	   check_background (image);
	}


/*
 *  Open output files.  Check to see if they exist first.  If they
 *  don't, then header information will be added to the files.  If
 *  they do, just append data records.
 */
   	strcpy (file1,out_file);   /* patch stats in dump format */ 
   	strcat (file1,".patch");
        strcpy (file2,out_file);   /* class stats in dump format */
 	strcat (file2,".class");

   	strcpy (file3,out_file);   /* landscape stats in dump format*/

#ifdef __GO32__
	strcat (file3,".lnd");     /* for PC version only        */
#else
   	strcat (file3,".land");   
#endif

	strcpy (file4,out_file);   /* formatted file with all stats */
   	strcat (file4,".full");
 
	if (patch_stats) {
	   newfiles = FALSE;
	   if ((out = fopen(file1,"r")) == NULL) newfiles = TRUE;
	   else fclose (out);
	   if (newfiles) {
	      if ((out = fopen(file1,"a")) == NULL) {
   	         printf ("\nERROR opening file: %s\n",file1);
	         exit(-1);
	      }
	      head_patch(out);
	      fclose (out);
	   }
	}

	if (class_stats) {
	   newfiles = FALSE;
	   if ((out = fopen(file2,"r")) == NULL) newfiles = TRUE;
	   else fclose (out);
           if (newfiles) { 
	      if ((out = fopen(file2,"a")) == NULL) {
	         printf ("\nERROR opening file: %s\n",file2);
	         exit(-1);
	      }
	      head_class(out);
	      fclose (out);
	   }
	}

	newfiles = FALSE;
	if ((out = fopen(file3,"r")) == NULL) newfiles = TRUE;
	else fclose (out);
	if (newfiles) {
	   if ((out = fopen(file3,"a")) == NULL) {
	      printf ("\nERROR opening file: %s\n",file3);
	      exit(-1);
	   }
	   head_land(out);
	   fclose (out);
	}

	if ((out = fopen(file4,"a")) == NULL) {
	   printf ("\nERROR opening file: %s\n",file4);
	   exit(-1);
	}

/*
 *  Echo user inputs to output formatted (.full) file
 */
	lt = time(NULL);
	time_ptr = localtime(&lt);
	fprintf (out,"\n\nDate:  %s",asctime(time_ptr));
	fprintf (out,"Image Name: %s",imagename);
	fprintf (out,"\nBasename For Output Files: %s",out_file);
	fprintf (out,"\nRows: %d   Cols: %d   Cellsize: %.1f",
	   num_rows,num_cols,cellsize);
	fprintf (out,"   Data Type: %d",data_type);
	fprintf (out,"\nEdge Dist: %.1f   ",edge_dist);
	if (do_patchrich)
	   fprintf (out,"Max Patch Types Possible: %d   ",max_num_classes);
	else
	   fprintf (out,"Max Patch Type Possible: NA   ");
	fprintf (out,"   Background: %d",background);
	if (contrast_indices) 
	   fprintf (out,"\nWeight File: %s",weight_file);
	if (id_image == 1)
	   fprintf (out,"\nID Image Will Be Created");
	if (id_image == 2)
	   fprintf (out,"\nNo ID Image Will Be Output");
	if (id_image == 3)
	   fprintf (out,"\nID Image: %s",id_file);
	if (descriptors)
	   fprintf (out,"\nDescriptor File: %s",desc_file);
	if (land_border) 
	   fprintf (out,"\nImage Includes a Landscape Border");
  	else
	   fprintf (out,"\nImage Does Not Include a Landscape Border");
	fprintf (out,"\nProportion of Boundary/Background to Count as Edge: %.2f",
	      bound_wght);
	if (use_diags)
	   fprintf (out,"\nDiagonals Used");
	else
	   fprintf (out,"\nDiagonals Not Used");
	if (proximity)
	   fprintf (out,";   Proximity Dist (m): %.1f",search_radius);
	else
	   fprintf (out,";   Proximity Indices Not Calculated");
	if (do_nndist)
	   fprintf (out,"\nNearest Neighbor Calcs");
	else
	   fprintf (out,"\nNo Nearest Neighbor Calcs");
	if (patch_stats)
	   fprintf (out,"\nWrite Patch Indices;   ");
	else
	   fprintf (out,"\nDo not Write Patch Indices;   ");
	if (class_stats)
	   fprintf (out,"Write Class Indices");
	else
	   fprintf (out,"Do Not Write Class Indices");

	fprintf (out,"\n\n");

	fclose (out);
}
