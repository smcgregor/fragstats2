/************************************************************
 *  File Name:  HDLAND.C
 *
 *  Purpose:  Writes header record to "basename".land file.
 *      This file contains a dump of the landscape level
 *      indices.
 *
 *  Passed Parameters:
 *      fp:     pointer to landscape level dump file
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1993
 *
 *  ID:  $Id: hdland.c,v 2.1 1994/04/01 17:47:30 marks Exp $
 **************************************************************/
#include <stdio.h>

void head_land(fp)
FILE 	*fp;	
{
 	fprintf (fp,"        LID           TA    LPI    NP       PD");
	fprintf (fp,"         MPS        PSSD       PSCV");
	fprintf (fp,"           TE        ED      CWED");
	fprintf (fp,"   TECI   MECI AWMECI");
        fprintf (fp,"    LSI    MSI  AWMSI  DFLD  MPFD AWMPFD");
	fprintf (fp,"        TCA   NCA      CAD");
        fprintf (fp,"        MCA1       CASD1    CACV1 ");
	fprintf (fp,"       MCA2       CASD2    CACV2");
	fprintf (fp,"   TCAI   MCAI  ");
   	fprintf (fp,"       MNN      NNSD     NNCV          MPI ");
   	fprintf (fp,"  SHDI   SIDI  MSIDI");
	fprintf (fp,"   PR      PRD    RPR   ");
	fprintf (fp,"SHEI   SIEI  MSIEI");
        fprintf (fp,"     IJI  CONTAG  ");

	fprintf (fp,"\n");
	
}
