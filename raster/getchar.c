/************************************************************
 *  File Name:  GETCHAR.C
 *
 *  Purpose:  Returns the value of the character at the 
 *	passed position.
 *
 *  Passed Parameters:
 *	arrayptr:     pointer to character array of pointers
 *      row:          row number
 *	max_len:      maximum length for string;  if the
 *		      string is longer than this value, it
 *		      will be truncated.
 *	value:        returned; string at passed position
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  8 May 1991
 *
 *  ID:  $Id: getchar.c,v 2.1 1994/04/01 17:46:36 marks Exp $
 **************************************************************/
#include <string.h>

void get_char (arrayptr,row,max_len,value)
char	**arrayptr;
short	row;
short	max_len;
char	*value;
{
	int  length;


/*
 *  If there is no data at the passed position, return NULL
 */
	if ((!arrayptr) || (!arrayptr[row]))
	   sprintf (value,"NULL");

/*
 *  Get the string at the passed position.  If the string is
 *  longer than max_len, truncate it.
 */
	else {
	   length = strlen(arrayptr[row]);

	   if (length > max_len) {
	      strncpy (value,arrayptr[row],max_len);
	      value[max_len] = '\0';
	   }
	   else
	      strcpy (value,arrayptr[row]);
	}

}
