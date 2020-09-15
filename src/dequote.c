/*---------------------------------------------------------------------------
dequote.c, dequote()
Purpose:
	To change PAIRS of double-quotes in a string into single double-quotes.
	Single occurrences of double-quotes are removed altogether.
Returns:
	Length of dequoted string.
History:
	02Feb99	Removed conditional compilation lines which allowed K&R
				prototyping for Sun4 machines; now full prototyping applies to
				all machines. (BB)
	10Sep92	Written. (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"
#else
#include "..\\commonfnctns\\astro.h"
#endif

int dequote(char *string)
{
	int	i;
	int	dropped_chars = 0;

	for(i=0; true; i++) {
		if (string[i+dropped_chars]=='"') dropped_chars++;
		if (dropped_chars) string[i] = string[i+dropped_chars];
		if (string[i]==NULC) break;
	}
	return(i);
}
