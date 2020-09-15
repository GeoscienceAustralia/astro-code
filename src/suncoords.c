/*---------------------------------------------------------------------------
suncoords.c, sunCoords()
	This routine calculates rectangular cartesian coordinates of the sun
	relative to mean ecliptic & equinox of date at any given julian date.
Input:
	dj:	full julian date (including fraction of day, if wanted).
Output:
	ecliptic:	rectangular ecliptic coordinates of sun w.r.t. earth-moon
					barycentre, in metres.
Note:
	These results are really only approximate - they assume Kepler elements
	as defined by 'old' IAU system as in 1975 explanatory supplement, and
	that the semi-diameter of the orbit is exactly 1 astronomical unit.
	Further, we tacitly suppose that the earth-moon barycentre is at the
	centre of the earth.
Returns:
	nil
History:
	17Jun02	Translated from Delphi into C. (Brock Bryce)
	Earlier	history unknown, but see also sunmoonposn.c's prologue.
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "astro.h"	/* for ASTRO_UNIT definition	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

void sunCoords(double dj, double ecliptic[4])
	/* cell zero of ecliptic[] is never used	*/
{
	double	anom;
	double	d;
	double	eccen;
	double	peri;
	double	r;
	double	sunlng;
	double	t;
	double	v;

/*----Time parameters:
*/

	d = dj - 2415020.0;
	t = d/36525.0;

/*----Kepler elements & longitude at date:
*/
	anom = 358.47583 + 0.985600267*d - 0.15E-03*t*t - 0.3E-05*t*t*t;
	anom = dMod(anom, 360.0)*RADIANSPERDEGREE;
	eccen = 0.01675104 - 0.418E-04*t - 0.126E-06*t*t;
	peri = 281.22083 + 0.470684E-04*d + 0.453E-03*t*t + 0.3E-05*t*t*t;
	peri = dMod(peri, 360.0)*RADIANSPERDEGREE;

	v = anom + (2.0*eccen - 0.25*eccen*eccen*eccen)*sin(anom)
	+ 1.25*eccen*eccen*sin(2.0*anom)
	+ 1.08333333333*eccen*eccen*eccen*sin(3.0*anom);
	sunlng = peri + v;
	sunlng = dMod(sunlng, TWOPI);

/*----Assume celestial latitude (beta) of the sun is exactly zero.
------Convert from ecliptic lat/long to ecliptic x,y,z:
*/
	r = ASTRO_UNIT*(1.0-eccen*eccen)/(1.0+eccen*cos(v));
	ecliptic[1] = r*cos(sunlng);
	ecliptic[2] = r*sin(sunlng);
	ecliptic[3] = 0.0;

	return;
}
