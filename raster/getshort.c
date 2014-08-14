/************************************************************
 *  File Name:  GETSHORT.C
 *
 *  Purpose:  Returns the value of the cell at the row and
 *	column passed.
 *
 *  Passed Parameters:
 *	arrayptr:     pointer to short integer array
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
 *  ID:  $Id: getshort.c,v 2.1 1994/04/01 17:47:01 marks Exp $
 **************************************************************/

short getshort (arrayptr,num_cols,row,col)
short	*arrayptr;
short	num_cols;
short	row,col;
{
	short	value;
	int 	position;
	
	position = row * num_cols + col;
	value = *(arrayptr + position);
	return (value);
}
