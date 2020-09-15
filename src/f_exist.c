/*=======================================================================
f_exist.c
	Used to test whether a file exists.
Warning:
	On a DOS system this is foolproof, but under UNIX it can give a false
	return if the file is present but the user does not have READ permission.
	This fnctn should be rewritten to use a stat() call and look for a
	returned value of ENOENT (error-no entry).  For an example of using a
	stat() call, see the fnctn ctrlflmoddate() in the gattr2fc suite.
Returns:
	TRUE if the file can be opened for reading, or FALSE if it cannot.
History:
	02Feb99	Removed conditional compilation lines which allowed K&R
				prototyping for Sun4 machines; now full prototyping applies to
				all machines. (BB)
	30Sep92	Copied for use with Unix, and made self-contained. (BB)
	27Jul89	Converted from my Pascal version into C. (Brock Bryce)
=======================================================================*/
#include <stdio.h>
#ifdef UNIX
#include "broxbool.h"
#else
#include "..\\commonfnctns\\broxbool.h"
#endif

bool f_exist(char *fileName)
{
	FILE *fptr;
	fptr = fopen(fileName, "r");
	if (fptr==NULL) {
		return(false);
	} else {
		(void)fclose(fptr);
		return(true);
	}
}
