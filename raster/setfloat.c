/*********************************************************
 *  File Name:  SETFLOAT.C
 *
 *  Purpose:  Sets the cell at the row and column passed
 *	in the specified array to 'value'.
 *
 *  Passed Parameters:
 *	arrayptr:   pointer to floating point array
 *      num_cols:   number of colums in the image
 *	row:        row number
 *      col:        column number
 *      value:      value to give to the cell at row,col
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  8 May 1991
 *
 *  ID:  $Id: setfloat.c,v 2.1 1994/04/01 17:50:25 marks Exp $
 *********************************************************/

void setfloat (arrayptr,num_cols,row,col,value)
float	*arrayptr;
short	num_cols;
short   row,col;
float	value;
{
	int 	position;

	position = row * num_cols + col;
	*(arrayptr + position) = value;
}
