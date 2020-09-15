/*---------------------------------------------------------------------------
calcrefractionangle.c, calcRefractionAngle()
	This routine calculates the refraction angle for any celestial body.
	The formula has been taken from page b 61 of
		"the astronomical almanac 1986".
	This routine uses fixed values for temperature and pressure as defined
	for a standard atmosphere (see parameter statements); the formula used is
	suitable to be used with any temperature or pressure.
Inputs:
	elevation	The angle between the horizon and the celestial body.
History:
	17Jun02	Translated from Delphi into C. (Brock Bryce)
	27May86	Worked on. (Klaus P Voigt)
	Earlier	history unknown, but see also sunmoonposn.c's prologue.
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "astro.h"	/* for RADIANSPERDEGREE, DEGREESPERRADIAN	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

double calcRefractionAngle(double elevation)
{
	double			elevation_degrees;
	const double	fifteen_degrees		= 0.261799388;		/* in radians	*/
	const double	minus_50_minutes		= -0.0145444100;	/* in radians	*/
	const double	ninety_degrees			= 1.5707963270;	/* in radians	*/
	const double	pressure					= 1012.00;
	double			refraction_angle;
	const double	temperature				= 10.00;
	const double	thirty_four_minutes	= 0.566667;			/* in degrees	*/
	const double	zero_degrees			= 0.00;

	if ((elevation>=fifteen_degrees)&&(elevation<=ninety_degrees)) {
		refraction_angle =
		 (0.004520*pressure)/((273.00+temperature)*tan(elevation));
   } else {
		if ((elevation>=zero_degrees)&&(elevation<fifteen_degrees)) {
			/* Convert radians to degrees:	*/
			elevation_degrees = elevation*DEGREESPERRADIAN;
			refraction_angle =
			 (pressure*(0.15940+0.01960*elevation_degrees+0.00002*
			 elevation_degrees*elevation_degrees))/((273.00+temperature)*(1.0+
			 0.505*elevation_degrees+0.0845*elevation_degrees*elevation_degrees));
		} else {
			/* 50 minute applies to sun and moon (large bodies) only	*/
			if ((elevation>=minus_50_minutes)&&(elevation<zero_degrees))
			 refraction_angle = thirty_four_minutes;
			else refraction_angle = zero_degrees;
		}
	}

	/* Convert degrees to radians:	*/
	return(refraction_angle*RADIANSPERDEGREE);
}
