/*---------------------------------------------------------------------------
strncpy0.c, strncpy0()
Purpose:
	Similar to strncpy() but sticks in a final NULC to make your program all
	that more bulletproof.  That is a zero on the end of the name.
Usage:
	If your target string has been declared as follows:
		char	target[TARGETLEN+1];
	then you use:
		(void)strncpy0(target, source, TARGETLEN);
	If your target string was declared without the '+1' then you use:
		(void)strncpy0(target, source, TARGETLEN-1);
	Basically using this fnctn is identical to typing:
		(void)strncpy(target, source, TARGETLEN);
		target[TARGETLEN] = NULC;
Warning:
	Your selection of the third parameter is critical; if you don't know the
	amount of memory allotted for the target string, it is safest not to use
	this fnctn.  For example if you say strncpy0(target, source, 200), and target
	is only 60 characters long, you will clobber another variable with a NULC
	140 characters further on!
Returns:
	Pointer to the target string (emulating strncpy()).
History:
	30Apr02	Renamed from strNcpy() to strncpy0() because away from the Unix
				environment, strNcpy looks too much like the standard strncpy that
				comes with string.h. (BB)
	02Feb99	Removed conditional compilation lines which allowed K&R
				prototyping for Sun4 machines; now full prototyping applies to
				all machines. (BB)
	30Sep92	Written. (Brock Bryce)
---------------------------------------------------------------------------*/
#include <string.h>
#define NULC '\0'

char *strncpy0(char *target, char *source, int length)
{
	char	*ret;

	ret = strncpy(target, source, length);
	target[length] = NULC;
	return(ret);
}
