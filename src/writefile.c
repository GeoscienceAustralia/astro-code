#if 0
/* TEST HARNESS FOR writeFile()	*/
#include <stdio.h>
#include <string.h>
int writeFile(char *fileName);	/* prototype	*/

void main(void)
{
	char	fileName[30];
	int	ret;

	(void)strcpy(fileName, "astro.inp");
	ret = writeFile(fileName);
	(void)fprintf(stderr, "Returned value was %d.\n", ret);
	return;
}
#endif

/*---------------------------------------------------------------------------
writefile.c, writeFile()
Purpose:
	Opens a file, writes its contents out to stdout, and closes the file.
Returns:
	0	if no problems.
	1	if can't open file.
	2	if can't read file.
	3	if problems closing the file.
History:
	31May02	First written and tested.  (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for BUFFERLEN, <stdio.h> etc.	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

int writeFile(char *fileName)
{
	char	buffer[BUFFERLEN+1];

	FILE *ifp;	/* input-file pointer	*/
	if ((ifp=fopen(fileName, "r"))==NULL) return(1);
	while(fgets(buffer, BUFFERLEN, ifp)!=NULL) {
		if (ferror(ifp)) return(2);
		(void)fputs(buffer, stdout);
	}
	if (fclose(ifp)!=0) return(3);
	return(0);
}
