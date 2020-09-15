/*---------------------------------------------------------------------------
monthname.c, monthName()
Purpose:
	Returns the name of the n-th month.
History:
	15May02	Copied out of Kernighan and Richie 2nd Edn p113. (Brock Bryce)
---------------------------------------------------------------------------*/
char *monthName(int n)
{
	static char *name[] = {"BAD","JAN","FEB","MAR","APR","MAY","JUN","JUL",
	 "AUG","SEP","OCT","NOV","DEC"};

	return((n<1 || n>12) ? name[0] : name[n]);
}
