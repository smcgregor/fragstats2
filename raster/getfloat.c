/************************************************************
 *  File Name:  GETFLOAT.C
 *
 *  Purpose:  Returns the value of the cell at the row and
 *	column passed.
 *
 *  Passed Parameters:
 *	arrayptr:     pointer to floating point array
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
 *  ID:  $Id: getfloat.c,v 2.1 1994/04/01 17:46:44 marks Exp $
 **************************************************************/

float getfloat (arrayptr,num_cols,row,col)
float	*arrayptr;
short	num_cols;
short	row,col;
{
	float   value;
	int 	position;
	
	position = row * num_cols + col;
	value = *(arrayptr + position);
	return (value);
}
