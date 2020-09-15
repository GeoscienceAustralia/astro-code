/*---------------------------------------------------------------------------
cartpolar.c, cartPolar()
	Converts rectangular cartesian coordinates 'x' to polar.  Note cell 0 of
				x[4] is unused; this is a hangover from the original Fortran.
	r     :	radius vector
	theta :	major angle 0<theta<twopi (radians) - corresponds to
				longitude or right ascension or (-) hour angle or (-) azimuth
	phi   :	minor angle -pi/2 < phi < pi/2 (radians) - corresponds to
				latitude or declination or elevation
History:
	17Jun02	Translated into C. (Brock Bryce)
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "astro.h"	/* for dMod() prototype, TWOPI definition	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

void cartPolar(double x[4], double *r, double *theta, double *phi)
{
	*r = sqrt(x[1]*x[1] + x[2]*x[2] + x[3]*x[3]);
	*theta = atan2(x[2], x[1]);
	*theta = dMod(*theta, TWOPI);
	if (*theta<0.0) *theta = *theta + TWOPI;
	*phi = asin(x[3]/(*r));

	return;
}
