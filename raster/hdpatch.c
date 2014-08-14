/****************************************************************
 *  File Name:  HDPATCH.C
 *
 *  Purpose:  Writes header record to "basename".patch file.
 *      This file contains a dump of the patch characteristics.
 *
 *  Passed Parameters:
 *      fp:     pointer to patch level dump file
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  January 1993
 *
 *  ID:  $Id: hdpatch.c,v 2.1 1994/04/01 17:47:37 marks Exp $
 **************************************************************/
#include <stdio.h>
#include "stats.h"

void head_patch(fp)
FILE	*fp;
{
	short	i;

        fprintf (fp,"       LID      PID ");
	for (i=0; i < MAX_LABEL_LENGTH-4; i++) fprintf (fp," ");
        fprintf (fp,"TYPE");
	fprintf (fp,"       AREA   LSIM      PERIM   EDGECON");
        fprintf (fp," SHAPE FRACT       CORE NCORE  CAI");
	fprintf (fp,"         NEAR       PROXIM");
	
	fprintf (fp,"\n");
}
