/*************************************************************
 *  File Name:  READIDRI.C
 *
 *  Purpose:  Reads and writes IDRISI image files. Files may be
 *            ascii or binary; byte or integer.  Float
 *            valued files are not supported.  Convert them
 *            to integer first.
 *
 *  Routines in this file:
 *     read_idrisi(filename, imageptr, min, max)
 *       Read in an idrisi grid
 *
 *    read_idrisi_desc(filename,name_list)
 *       Read the class names from the documentation file
 *
 *    write_idrisi (filename,imageptr,packtype)
 *       Write out an integer grid
 *
 *  Passed Parameters:
 *	filename:    first part of file name containing the IDRISI data
 *	image_ptr:   pointer to the image to fill with this data
 *      name_list    array pointer to hold category names
 *      packtype     use 8 or 16 bit data values
 *
 *  Returned Parameters:
 *	min:	     minimum value (>= 0) in the input image
 *	max:         maximum value in the input image
 *
 *  Bugs:
 *      Possibly.  I have tried to keep the datafiles in Intel format so
 *      that IDRISI data files could be processed on workstations.  Since
 *      integers are different endian between Intel and Sparc this means
 *      that binary data has to be byte swapped.  This has only been tested
 *      on Sparc/Sunos and Intel/DOS machines. Compilation flags might be
 *      required for other platforms.
 *
 *  Programmer:  Tom Moore (tmoore@pnfi.forestry.ca)
 *
 *  Date:  20 December 1993
 *
 *  ID:  $Id: readidri.c,v 2.4 1994/10/05 16:07:28 marks Exp marks $
 **************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "stats.h"
#include "string.h"

static short get_image();
static int count;

/* IDRISI documentation file variables */
static char refsystem[20],refunits[20],poserr[20],resolut[20];
static float unitdist, min_x, max_x, min_y, max_y;

void read_idrisi (filename,image_ptr,min,max)
char	*filename;
short   *image_ptr;
short	*min,*max;
{
        int	i,j,f_type,d_type;
	int	count_bck,count_intbck;
	short	*ptr;
	FILE	*fp;
	char    *wholename,record[100],*token,*open_type;


	count = 0;
	count_bck = count_intbck = 0;

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
	  if (!strncmp(token,"data type",9)) {
	    token = strtok(NULL," \t\n\r");
	    if (!strcmp(token,"byte")) 
	      d_type = 1;
	    else if (!strcmp(token,"integer")) 
	      d_type = 2;
	    else {
	      printf("\nERROR bad input data type %s\n",token);
	      exit (-1);
	    }
	  }
	  /* file format ... */
	  else if (!strncmp(token,"file type",9)) {
	    token = strtok( NULL, " \t\n\r");
	    if (!strcmp(token,"ascii")) 
	      f_type = 1;
	    else if (!strcmp(token,"binary")) 
	      f_type = 2;
	    else if (!strcmp(token,"packed")) 
	      f_type = 3;
	    else {
	      printf("\nERROR bad input data type %s\n",token);
	      exit (-1);
	    }
	  }
	  else if (!strncmp(token,"ref. system",11)) {
	    token = strtok(NULL,"\n\r");
	    strncpy(refsystem,token,19);
	  }
	  else if (!strncmp(token,"ref. units",10)) {
	    token = strtok(NULL,"\n\r");
	    strncpy(refunits,token,19);
	  }
	  else if (!strncmp(token,"unit dist.",10))  {
	    token = strtok(NULL,"\n\r");
	    sscanf(token,"%f",&unitdist);
	  }
	  else if (!strncmp(token,"min. X",6))  {
	    token = strtok(NULL,"\n\r");
	    sscanf(token,"%f",&min_x);
	  }
	  else if (!strncmp(token,"max. X",6))  {
	    token = strtok(NULL,"\n\r");
	    sscanf(token,"%f",&max_x);
	  }
	  else if (!strncmp(token,"min. Y",6))  {
	    token = strtok(NULL,"\n\r");
	    sscanf(token,"%f",&min_y);
	  }
	  else if (!strncmp(token,"max. Y",6))  {
	    token = strtok(NULL,"\n\r");
	    sscanf(token,"%f",&max_y);
	  }
	  else if (!strncmp(token,"pos'n error",11)) {
	    token = strtok(NULL,"\n\r");
	    strncpy(poserr,token,19);
	  }
	  else if (!strncmp(token,"resolution",10)) {
	    token = strtok(NULL,"\n\r");
	    strncpy(resolut,token,19);
	  }
	}
      	    

	fclose(fp);

	*min = 9999;
	*max = -9999;
	MAX_CLASSES = -9999;
	land_border = FALSE;
	ptr = image_ptr;

	strcpy(wholename,filename);
	strcat(wholename,".img");
	if (f_type == 1) open_type = "r";
	else open_type = "rb";
	if ((fp = fopen(wholename,open_type)) == NULL) {
		printf("\nERROR reading file: %s\n",wholename);
		exit(-1);
	      }

/*
 *  total_size is defined in the include file stats.h.  total_size
 *  is the number of cells inside the landscape (non-background and
 *  non-landscape border cells).
 */
	total_size = 0;

	
	for (i=0; i < num_rows; i++) {
  	   for (j=0; j < num_cols; j++) {
	        
	        *ptr = get_image(fp,f_type,d_type);

/*
 *  If there are class values < 0 and not background, then this
 *  image includes a landscape border.  Set a flag.
 */
	 	if (*ptr < 0 && *ptr != background && *ptr != -background)
		   land_border = TRUE;

/*
 *  Find the minimum and maximum class values (only consider those
 *  classes inside the landscape). 
 */
		if (*ptr != background && *ptr != -background) {
	   	   if (*ptr < *min && *ptr >= 0) *min = *ptr;
		   if (*ptr > *max) *max = *ptr;
		   if (*ptr >= 0) total_size ++;

/*
 *  If this image has a landscape border, for contagion the total
 *  number of classes is needed including those class types in the
 *  landscape border.
 */
	           if (*ptr >= 0 && *ptr > MAX_CLASSES)
		      MAX_CLASSES = *ptr;
	           if (*ptr < 0 && -(*ptr) > MAX_CLASSES)
                      MAX_CLASSES = -(*ptr);
		}

/*
 *  The value for background cells was input by the user.  Change all
 *  background cells interior to the landscape to -990.  Change those
 *  in the landscape border and outside the area of interest to -999.
 *  This makes life easier in other routines.
 */
	        if (*ptr == -background) {
		   count_bck ++;
		   *ptr = -999;
		}
	        if (*ptr == background) {
		   count_intbck ++;
		   *ptr = -990;
		}

		ptr++;
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
	if (count_intbck == 0 && count_bck == 0)
	   printf ("\n... landscape does not contain background");
}



static short 
get_image(imagein,f_type,d_type)
FILE *imagein;
int f_type, d_type;
{
    int error;
    static short i;
    union byte_int {
      short i;
      unsigned char b;
    };
    static union byte_int both;

    switch (f_type) {
    case 1: {			/* ASCII grid file */
	  error = fscanf(imagein,"%hd\n",&both.i);
	  break;
      }
	
      case 2:  {		/* Binary grid file */
	  error = fread(&both.i,d_type,1,imagein);
	  break;
      }
	
      case 3: {			/* Packed binary grid file */
	  while (count == 0) {
	      error = fread(&count,2,1,imagein);
	      if (error != 1) break;
	      error = fread(&both.i,d_type,1,imagein);
	      if (error != 1) break;
	  }
	  
	  count--;
	  break;
      }
    }

    if (d_type == 1) i = both.b;
    else
#ifdef __GO32__
      i = both.i;
#else
      swab(&both.i,&i,2);
#endif

    if (error != 1) {
      count = 0;
      return(-1);
    }
    else
      return(i);
}

void read_idrisi_desc(filename,name_list)
char *filename, *name_list;
{

  FILE *fp;
  char *wholename, *token, record[100];
  int i,j;

/*  
 *  Read in the values from the IDRISI doc file 
 */

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
	  /* file format */
	  if (!strncmp(token,"category",8)) {
	    sscanf(token,"category%d",&j);
	    token = strtok(NULL,"\n\r");
	    if (!token) token = "no_name";

	    /* strip leading blanks */
	    while (*token == ' ') token++; 

	    /* strip trailing blanks*/
	    for (i=strlen(token)-1;i>=0 && token[i] == ' ' ;i--) 
	      token[i] = '\0'; 

	    /* convert embedded ' ' to '_' */
	    for (i=0;i<strlen(token);i++)
	      if (token[i] == ' ') token[i] = '_'; 
	    
	    if (!strcmp(token,"")) token = "no_name";

/*
 *  Only save the IDs for the classes that exist in the landscape.
 */
	    for (i=0; i < NUM_CLASSES; i++) {
	      if (j == patchtype[i]) 
		set_char(name_list,i,token);
	    }
	    
	  }
	}	    
  
	fclose(fp);
	free(wholename);
}

void write_idrisi (filename,imageptr,packtype)
char	*filename;
short	*imageptr;
short	packtype;
{
	FILE	*fp;
	short	i,j;
	short   val1,val2;
	int	value;
	char *token, *wholename;

/* Write DOS format files on unix machines */
#ifdef __GO32__
#define EOL "\n"
#else
#define EOL "\r\n"
#endif	

	wholename = (char* ) malloc(strlen(filename)+5);
	if (!wholename) {
	  printf("\nERROR allocating space for filename %s\n",filename);
	  exit(-1);
	}

	strcpy(wholename,filename);
	token = strtok(wholename,".");
	if (!token || token[0] == 0) {
	  printf("\nERROR  Bad image name string\n");
	  exit(-1);
	}

	strcat(wholename,".doc");
	if ((fp = fopen(wholename,"w")) == NULL) {
	  printf("\nERROR creating file: %s\n",wholename);
	  exit(-1);
	}
	fprintf(fp,"file title  : Patch id%s",EOL);
	if (packtype == 1)
	  fprintf(fp,"data type   : byte%s",EOL);
	else
	  fprintf(fp,"data type   : integer%s",EOL);
	fprintf(fp,"file type   : binary%s",EOL);
	fprintf(fp,"columns     : %d%s",num_cols,EOL);
	fprintf(fp,"rows        : %d%s",num_rows,EOL);
	fprintf(fp,"ref. system :%s%s",refsystem,EOL);
	fprintf(fp,"ref. units  :%s%s",refunits,EOL);
	fprintf(fp,"unit dist.  : %f%s",unitdist,EOL);
	fprintf(fp,"min. X      : %f%s",min_x,EOL);
	fprintf(fp,"max. X      : %f%s",max_x,EOL);
	fprintf(fp,"min. Y      : %f%s",min_y,EOL);
	fprintf(fp,"max. Y      : %f%s",max_y,EOL);
	fprintf(fp,"pos'n error :%s%s",poserr,EOL);
	fprintf(fp,"resolution  :%s%s",resolut,EOL);
	fprintf(fp,"min. value  : 0%s",EOL);
	fprintf(fp,"max. value  : %d%s",MAX_PATCHES,EOL);
	fprintf(fp,"value units : unspecified%s",EOL);
	fprintf(fp,"value error : unknown%s",EOL);
	fprintf(fp,"flag value  : none%s",EOL);
	fprintf(fp,"flag def'n  : none%s",EOL);
	fprintf(fp,"legend cats : 0%s",EOL);
	fclose(fp);

	strcpy(wholename,filename);
	token = strtok(wholename,".");
	strcat(wholename,".img");
	if ((fp = fopen(wholename,"wb")) == NULL) {
	  printf("\nERROR creating file: %s\n",wholename);
	  exit(-1);
	}

/*
 *  Write 8 bit data
 */
        if (packtype == 1) {
           printf ("\nWriting ID image -- IDRISI binary byte file %s ....\n",
		   wholename);

           for (i=0; i < num_rows; i++) {
              for (j=0; j < num_cols; j++) {
		 value = (int) getshort (imageptr,num_cols,i,j);
	         fputc (value,fp);
              }
           }   
        }   
 
/*
 *  Write 16-bit data
 */
	if (packtype == 2) {
           printf ("\nWriting ID image -- IDRISI binary integer file %s....\n",
		   wholename);
	   for (i=0; i < num_rows; i++) {
	      for (j=0; j < num_cols; j++) {
		 val1 = (int) getshort (imageptr,num_cols,i,j);
#ifdef __GO32__
		 fwrite (&val1,sizeof(short),1,fp);
#else
		 swab(&val1,&val2,2);
		 fwrite (&val2,sizeof(short),1,fp);
#endif
	      }
	   }
	}

	fclose (fp);
	free(wholename);
}
