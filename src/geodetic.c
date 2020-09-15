/*---------------------------------------------------------------------------
geodetic.c, geodetic()
	Converts geodetic (ellipsoidal) coordinates to geocentric cartesian
	coordinates.
Inputs:
	rlat	geodetic latitude in radians
	rlong	longitude in radians
	h		ellipsoid height (ie. n+h) in metres
Output:
	x		geocentric rectangular coordinates referred to equator and
			Greenwich meridian, in metres.  Cell 0 of this array is unused.
History:
	16Jun02	Translated from Delphi into C. (Brock Bryce)
	Earlier	history unknown, but see also sunmoonposn.c's prologue.
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "astro.h"	/* for FLATINV	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

void geodetic(double rlat, double rlong, double h, double x[4])
{
	double	dcp;
	double	dsp;
	double	esq;
	double	f;
	double	n;

	f = 1.0/FLATINV;
	esq = 2.0*f - f*f;

	dsp = sin(rlat);
	dcp = cos(rlat);
	n = AEGEOD/sqrt(1.0 - esq*dsp*dsp);

	x[1] = (n+h)*dcp*cos(rlong);
	x[2] = (n+h)*dcp*sin(rlong);
	x[3] = (n*(1.0-esq)+h)*dsp;

	return;
}
