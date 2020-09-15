#if 0
/* TEST HARNESS FOR hexChar2dec()	*/
#include <stdio.h>
#include <string.h>
int hexChar2dec(char *instr);	/* prototype	*/

void main(void)
{
	char	string[3];
	int	ret;

	(void)strcpy(string, "A1");
	ret = hexChar2dec(string);
	printf("Input hex char was %s\n", string);
	printf("Returned value was %d\n", ret);
}
#endif
/*---------------------------------------------------------------------------
hexchar2dec.c, hexChar2dec()
	Assists the fixFunnies() function by taking a two character string like
	"3A" and, treating it as the hexadecimal number '\x3A', works out its
	value and returns it as a decimal number.
Returns:
	A number between 0 and 255 normally.
	88 if problems encountered; this represents the character 'X'.
	It is assumed that there is no point in crying "error" to the calling
	routine.
History:
	03Jun02	First written and tested.  There's probably a better way of doing
				this algorithm but I can't see it at the moment. (Brock Bryce)
---------------------------------------------------------------------------*/
#include <ctype.h>			/* for isdigit()			*/
#ifdef UNIX
#include "astro.h"	/* for NULC	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

int hexChar2dec(char *instr)
{
	char	temp[2];
	int	number;
	int	val;

	if (strlen(instr)!=2) return(88);

	temp[1] = NULC;
	switch(instr[0]) {
	case 'A': case 'a': val = 10; break;
	case 'B': case 'b': val = 11; break;
	case 'C': case 'c': val = 12; break;
	case 'D': case 'd': val = 13; break;
	case 'E': case 'e': val = 14; break;
	case 'F': case 'f': val = 15; break;
	default: if (!isdigit((int)instr[0])) return(88);
		temp[0] = instr[0]; val = atoi(temp); break;
	}
	number = 16*val;
	switch(instr[1]) {
	case 'A': case 'a': val = 10; break;
	case 'B': case 'b': val = 11; break;
	case 'C': case 'c': val = 12; break;
	case 'D': case 'd': val = 13; break;
	case 'E': case 'e': val = 14; break;
	case 'F': case 'f': val = 15; break;
	default: if (!isdigit((int)instr[1])) return(88);
		temp[0] = instr[1]; val = atoi(temp); break;
	}
	number += val;
	return(number);
}

