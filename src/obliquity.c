/*---------------------------------------------------------------------------
obliquity.c, obliquity()
	This routine calculates the obliquity of the ecliptic (epsilon) at a
	given date, w.r.t. the mean equator of date. (For true obl, add d(eps).)
	Abstracted from k. yokoyama's merit standard 'astrometry' tape.
Inputs:
	dj:	Full julian date, including fraction of day if wanted.
Returns:
	Obliquity of ecliptic in radians.
History:
	17Jun02	Translated from Delphi into C. (Brock Bryce)
	Earlier	history unknown; see prologue in sunmoonposn.c.
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for ARCSEC definition	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif
double obliquity(double dj)
{
	const double ec[4] = {84381.448, -46.8150, -0.00059, 0.001813};
	double	tj;
	double 	tj2;
	double	tj3;

/*----Time variables:
*/
	tj = (dj-2451545.0)/36525.0;
	tj2 = tj*tj;
	tj3 = tj*tj2;

/*----Angle between ecliptic and equator of date:
*/
	return((ec[0] + ec[1]*tj + ec[2]*tj2 + ec[3]*tj3)*ARCSEC);
}
