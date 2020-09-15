/*---------------------------------------------------------------------------
gmst.c, gmst()
	This routine calculates the greenwich mean sidereal time at a given utc
	(or indeed at a given ut1 in which case gmst1 is returned).
	Uses procedure given in 'merit' standards.
Input:
	dj: absolutely full julian date in days (including fraction pertaining
		to the microsecond if desired)
Returns:
	gmst: greenwich mean sidereal time, in radians
History:
	18Jun02	Translated from Delphi into C. (Brock Bryce)
	Earlier	history unknown, but see also sunmoonposn.c's prologue.
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for TSCRAD, TWOPI definitions	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif
double gmst(double dj)
{
	double	fraction;
	/* cell zero of sc[] is not used	*/
	const double sc[5] =
	 {0.0, 24110.54841, 8640184.812866, 0.093104, -6.2E-06};
	double	temp;
	double	tj, tj2, tj3;

/*----Time variables:
*/
	tj = (dj - 2451545.0) / 36525.0;
	tj2 = tj*tj;
	tj3 = tj*tj2;

/*----Fraction of ut day:
*/
	fraction = 86400.0*dMod(dj-0.5, 1.0);

/*----Sidereal time:
*/
	temp = TSCRAD*(fraction + sc[1] + sc[2]*tj + sc[3]*tj2 + sc[4]*tj3);

	temp = dMod(temp, TWOPI);
	if (temp < 0.0) temp = temp + TWOPI;

	return(temp);
}
