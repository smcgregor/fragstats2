/*********************************************************
 *  File Name:  SETCHAR.C
 *
 *  Purpose:  Sets the value of the character at the 
 *	specified location.
 *
 *  Passed Parameters:
 *	arrayptr:   pointer to character array
 *	length:     length of each elememt in the array
 *      row:        row number
 *      value:      value of the character to set at the
 *                  specified location
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  8 May 1991
 *
 *  ID: $Id: setchar.c,v 2.1 1994/04/01 17:50:33 marks Exp $
 *********************************************************/
#include <stdlib.h>
#include <string.h>

void set_char (arrayptr,row,value)
char	**arrayptr;
short	row;
char	*value;
{
	int	i;
	int	length;
	char	*ptr;

/* 
 *  Free the previous label (if any) 
 */
	if (arrayptr[row]) cfree(arrayptr[row]);
	arrayptr[row] = NULL;

/* 
 *  Now make space for the new label 
 */
	length = strlen(value)+1;

	arrayptr[row] = (char *) calloc((unsigned)length,sizeof(char));
	if (!arrayptr[row]) {
	   printf("\nERROR: could not allocate space for descriptive label\n");
	   exit (-1);
	}

/* 
 *  Copy into place 
 */
	ptr = arrayptr[row];
	for (i=0; i < length-1; i++) {
	   *ptr = *value;
	   ptr ++;
	   value ++;
	}

	*ptr = '\0';
}
