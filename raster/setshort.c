/*********************************************************
 *  File Name:  SETSHORT.C
 *
 *  Purpose:  Sets the cell at the row and column passed
 *	in the specified array to 'value'.
 *
 *  Passed Parameters:
 *	arrayptr:   pointer to short integer array
 *	num_cols:   number of cols in the image
 *	row:        row number
 *      col:        column number
 *      value:      value to give to the cell at row,col
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  8 May 1991
 *
 *  ID:  $Id: setshort.c,v 2.1 1994/04/01 17:50:49 marks Exp $
 *********************************************************/

void setshort (arrayptr,num_cols,row,col,value)
short	*arrayptr;
short	num_cols;
short	row,col,value;
{
	int 	position;

	position = row * num_cols + col;
	*(arrayptr + position) = value;
}
