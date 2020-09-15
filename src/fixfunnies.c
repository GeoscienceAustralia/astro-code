#if 0
/* TEST HARNESS FOR fixFunnies()	*/
#include <stdio.h>
#include <string.h>
int fixFunnies(char *buffer, int bufferlen);	/* prototype	*/

void main(void)
{
	char	string[100];
	int	ret;

	(void)strcpy(string, "31%2F05%2F2002%2C18%3A30%3A00");
	ret = fixFunnies(string, 100);
	printf("%s\n", string);
	if (ret) printf("Returned value was %d\n", ret);
}
#endif
/*---------------------------------------------------------------------------
fixfunnies.c fixFunnies()
	Translates character groups like '%44' in the input string into the real
	character that they represent, in this example a comma.
Input:
	The second parameter, bufferlen, is assumed to be the full length of the
	character array passed in, including the final NULC.
Returns:
	0	for successful completion.
	1 or more for problems.
History:
	31May02	First written and tested. (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for BUFFERLEN, hexChar2dec()	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

int fixFunnies(char *buffer, int bufferlen)
{
	char	ASCIIrepresentation[3];
	char	tempBuffer[BUFFERLEN+1];
	int	i, j;

/*----Replace all plus signs with spaces:
*/
	for(i=0; buffer[i]; i++) {
		if (buffer[i]=='+') buffer[i] = ' ';
	}

/*----Handle all the %NN groups:
*/
	ASCIIrepresentation[2] = NULC;
	for(i=0, j=0; buffer[i]; i++, j++) {
		if (i>bufferlen) return(1);		/* tested	*/
		if (j>BUFFERLEN) return(2);		/* tested	*/
		switch(buffer[i]) {
		case '%':
			ASCIIrepresentation[0] = buffer[i+1];
			ASCIIrepresentation[1] = buffer[i+2];
			tempBuffer[j] = (char)hexChar2dec(ASCIIrepresentation);
			i = i + 2;
			if (i>bufferlen) return(3);	/* tested	*/
			break;
		default:
			tempBuffer[j] = buffer[i];
			break;
		}
	}
	tempBuffer[j] = NULC;

	(void)strncpy0(buffer, tempBuffer, bufferlen-1);

	return(0);
}
