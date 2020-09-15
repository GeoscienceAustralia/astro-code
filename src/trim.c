/*---------------------------------------------------------------------------
trim.c, trim()
Purpose:
	Trims trailing blanks, tabs, carriage-returns and/or linefeeds
	from a string.
Returns:
	The length of the remaining string, or
	-1 if a NULL pointer was passed to this function.
History:
	06Sep99	Now removes carriage-returns also. (BB)
	02Feb99	Removed conditional compilation lines which allowed K&R
				prototyping for Sun4 machines; now full prototyping applies to
				all machines. (BB)
	16Aug90	Written. (Brock Bryce)
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

int trim(char *string)
{
	int i;
	int len;

	if (string==NULL) return(-1); /*We don't have a string, we have a null ptr.*/

	len = strlen(string);
	for(i=len-1; i>=0; i--) {
		switch(string[i]) {
		case ' ':
		case '\n':
		case '\t':
		case '\r':
			break;
		default:
			string[i+1] = '\0';
			return(i+1);
		}
	}
	string[0] = '\0';
	return(0);
}
