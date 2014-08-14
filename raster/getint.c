/************************************************************
 *  File Name:  GETINT.C
 *
 *  Purpose:  Returns the value of the cell at the row and
 *	column passed in the specified array.
 *
 *  Passed Parameters:
 *	arrayptr:     pointer to integer array
 *      num_cols:     number of columns in the array
 *	row:          row number
 *      col:          column number
 *
 *  Returned Parameters:
 *	value:	      value of the cell at row,col
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  8 May 1991
 *
 *  ID:  $Id: getint.c,v 2.1 1994/04/01 17:46:51 marks Exp $
 **************************************************************/

int getint (arrayptr,num_cols,row,col)
int	*arrayptr;
short	num_cols;
short	row,col;
{
	int     value;
	int 	position;
	
	position = row * num_cols + col;
	value = *(arrayptr + position);
	return (value);
}
