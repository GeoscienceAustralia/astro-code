/*---------------------------------------------------------------------------
semidi.c, semidi()
History:
	18May02	Modified to handle transit time predictions. (BB)
	Earlier history went unrecorded.
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "astro.h"
#else
#include "..\\commonfnctns\\astro.h"
#endif

void semidi(double aa, double dd, double ss, double pp, double *tt,
	double *hh, int itwi, double pdeg, double pmin)
/* Although in the Delphi, aa, dd, ss, and pp are passed in by reference rather
than by value, none of them are altered by this function, so I am passing them
into the C code by value only.  The variables altered and "passed back" by this
function are tt and hh.  BB	*/

/* in Delphi:
PROCEDURE SEMDI(VAR AA   :DOUBLE;
                VAR DD   :DOUBLE;
                VAR SS   :DOUBLE;
                VAR PP   :DOUBLE;
                VAR TT   :DOUBLE;
                VAR HH   :DOUBLE;
                    ITWI :INTEGER;
                    PDEG :DOUBLE;
                    PMIN :DOUBLE);

     semidi uses alpha, delta, phi, in radians, stnon in frac. of a day.
     it returns tran and h in fracs. of a day
VAR
  RA :DOUBLE;
  x  :DOUBLE;
  z  :DOUBLE;

*/
{
	double ra, x, z;

	ra = aa/6.2831853;
	if (ra-ss > 0.5) ra = ra - 1.0;

	if (ra-ss < -0.5) ra = ra + 1.0;

	*tt = 0.5 + (ra-ss)*0.99727;

/* Make tt into a number > 0 and <= 1 */
	while (*tt <= 0.0) *tt = *tt + 1.0;

	while (*tt > 1.0) *tt = *tt - 1.0;

	switch(itwi) {
	case 2:  z = rdian(96.0, 0.0, 0.0); break;
	case 3:  z = rdian(102.0, 0.0, 0.0); break;
	case 4:  z = rdian(108.0, 0.0, 0.0); break;
	case ZENITHDISTPREDCTN:  z = rdian(pdeg, pmin, 0.0); break;
	default: z = rdian(90.0, 50.0, 0.0); break;	/* normal rise, set, transit */
	}
/* CASE itwi OF
	2 : z = Rdian(96.0,0.0,0.0);

	3 : z = Rdian(102.0,0.0,0.0);

	4 : z = Rdian(108.0,0.0,0.0);

	5 : z = Rdian(pdeg,pmin,0.0);

	ELSE z = Rdian(90.0,50.0,0.0);
	END;
*/

	x = (cos(z)-sin(pp) * sin(dd)) / (cos(pp) * cos(dd));

	if (fabs(x) > 1.0) {
		*hh = 999999.9;
	} else {
		*hh = fabs(x);
		*hh = atan((sqrt(1 - *hh * *hh)) / *hh);
		*hh = *hh / 6.2831853;

		if (x < 0.0) *hh = 0.5 - *hh;
	}  /*fabs(x) > 1*/

}
