/*---------------------------------------------------------------------------
calculateposition.c, calculatePosition()
History:
	16Jun02	Translated from Delphi into C. (Brock Bryce)
	13Jan92	Last modification. (KPV)
	26May86	Translated from Fortran into TurboPascal. (Klaus P Voigt)
	unknown	First written and tested. (Dr John Luck)
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "astro.h"	/* for struct dateType definition, PI etc	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

void calculatePosition(
	double  *azimuth, 			/* important returned value	*/
	double  *elevation, 			/* important returned value	*/
	double  *refractionAngle,	/* value returned	*/
	double	xs[4],				/* passed in and not changed	*/
	double	phi,					/* seem to be input only	*/
	double	lambda,				/* seem to be input only	*/
	struct	dateType *pWhen,	/* input only	*/
	double	zone,					/* input only	*/
	int		body)					/* 1=sun, 2=moon	*/
{
	double	deps;
	double	dj;
	double	dpsi;
	double	gast;
	double	obl;
	double	r;
	double	x[4];
	int		i;

	dj = dmy2jday(pWhen->day, pWhen->month, pWhen->year) +
		(pWhen->hour + pWhen->minute/60.0 + pWhen->second/3600.0 - zone)/24.0;

/*---------------------------------------------------------------------------
	Coordinates of sun (or moon) in mean ecliptic & equinox of date
	are rotated through obliquity (to get to mean equator),
	then through nutation (to get to true equator & equinox),
	then through greenwich apparent sidereal time (to get gha & dec).
---------------------------------------------------------------------------*/
	if (body==SUN) sunCoords(dj, x);
	else          moonCoords(dj, x);

	obl = obliquity(dj);
	nutation(dj, &dpsi, &deps);
	rotate(3, -dpsi, x);
	rotate(1, -obl-deps, x);

	gast = gmst(dj) + dpsi*cos(obl);
	rotate(3, gast, x);

/*---------------------------------------------------------------------------
	Subtract site coordinates (which are now in consistent coordinate system
	apart from polar motion & ut1-utc which are neglected) to get topocentric
	coordinates of sun/moon.  Then rotate through longitude & latitude to get
	azimuth and elevation.
---------------------------------------------------------------------------*/
	for(i=1; i<4; i++) x[i] = x[i] - xs[i];

	rotate(3, lambda, x);
	rotate(2, 0.5*PI - phi, x);
	x[1] = -x[1];

	cartPolar(x, &r, azimuth, elevation);
	*refractionAngle = calcRefractionAngle(*elevation);
	*elevation = *elevation + *refractionAngle;

	return;
}
