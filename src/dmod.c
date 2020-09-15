#if 0
/* 							TEST HARNESS FOR dMod()	*/
#include <stdio.h>
double dMod(double numerator, double denomator);

void main(void)
{
	double answer, denom, num;

	num = 4.3;
	denom = 1.5;
	answer = dMod(num, denom);
	fprintf(stderr, "dMod(4.3, 1.5) came out as %4.2f\n", answer);
	/* it should come out to 1.30 in my opinion  -  BB	*/
	num = 4.7;
	answer = dMod(num, denom);
	fprintf(stderr, "dMod(4.7, 1.5) came out as %4.2f\n", answer);
	/* it should come out to 0.20 in my opinion  -  BB	*/
	return;
}
#endif
/*---------------------------------------------------------------------------
dmod.c, dMod()
History:
	17Jun02	Translated into C. (Brock Bryce)
---------------------------------------------------------------------------*/
double dMod(double numerator, double denominator)
{
	return(numerator - ((int)(numerator/denominator)*denominator));
}
/* based on the following original code in Delphi:
FUNCTION DMod(a : DOUBLE;
              b : DOUBLE) : DOUBLE;
{                                                   <880422.1134>
   Author :    K. P. Voigt     14/01/92

*$S "DMod"

  Compute the mod of two double precision numbers. Pascal allows only
  integers.

  The remainder is computed as follows:

    DMod(a,b) = a - (Trunc(a / b) * b)

    DMod

    Where :

*$E
}

BEGIN

  DMod := a - (Trunc(a / b) * b);

END;     {DMod}
*/
