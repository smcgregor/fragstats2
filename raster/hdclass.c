/*************************************************************
 *  File Name:  HDCLASS.C
 *
 *  Purpose:  Writes header record to "basename".class file.
 *	This file contains a dump of the class level indices.
 *
 *  Passed Parameters:
 *	fp:	pointer to class level dump file
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1993
 *
 *  ID:  $Id: hdclass.c,v 2.1 1994/04/01 17:47:45 marks Exp $
 **************************************************************/
#include <stdio.h>
#include "stats.h"

void head_class(fp)
FILE	*fp;
{
	short	i;

  	fprintf (fp,"         LID ");
	for (i=0; i < MAX_LABEL_LENGTH-4; i++) fprintf (fp," ");
	fprintf (fp,"TYPE          CA          TA");
	fprintf (fp,"  %%LAND    LPI    NP       PD         MPS");
	fprintf (fp,"     PSSD     PSCV");
	fprintf (fp,"           TE        ED");
	fprintf (fp,"      CWED   TECI   MECI AWMECI");
	fprintf (fp,"   LSI   MSI AWMSI  DLFD  MPFD AWMPFD");
	fprintf (fp," C%%LAND         TCA   NCA      ");
    	fprintf (fp,"CAD        MCA1       CASD1    CACV1        ");
        fprintf (fp,"MCA2       CASD2    CACV2   TCAI   MCAI");
	fprintf (fp,"         MNN      NNSD     NNCV       MPI     IJI");

	fprintf (fp,"\n");
}
