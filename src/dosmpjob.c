/*---------------------------------------------------------------------------
dosmpjob.c doSMPJob()
Purpose:
	This function processes the input data for one site, and one date & time.
	Called by sunmoonposn.c i.e. main().
Returns:
	0 for successful operation;
	1 for problems.
History:
    06Jul20 Modified to return JSON. (Bill Farmakis)
	18Jun02	Copied from doMoonJob() (by same author) & modified. (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for TWOPI definition etc	*/
#include "broxbool.h"
#else
#include "..\\commonfnctns\\astro.h"
#include "..\\commonfnctns\\broxbool.h"
#endif

int doSMPJob(struct siteType *pWhere, struct dateType *pWhen)
{
	bool		sign;
	char	  *tableHeading;
	double	lambda;
	double	lat[3], Long[3];	/* lat & long in degrees, minutes & seconds	*/
	double	phi;
	double	roundto = 0.1; /* to round decimal seconds to the nearest second */
	double	xs[4];
	int		ib;
	int		body = 0;	/* 1=sun, 2=moon	*/
	struct angleType azimuth;
	struct angleType elevation;
	struct angleType refraction;
	extern bool	debug;
	extern FILE *debugDest;

	if (debug) (void)fprintf(debugDest, "Entering fnctn doSMPJob()...\n");

/*----Check we have at least one body chosen:
*/
	if (!pWhere->eventCode[0] && !pWhere->eventCode[1]) {
		(void)fprintf(stdout, "Neither sun nor moon selected. Cannot continue!<BR>\n");
		return(1);
	}

/*----Load the lat and Long arrays:
*/
	lat[0] = (double)pWhere->latdeg;
	lat[1] = (double)pWhere->latmin;
	lat[2] = pWhere->latsec;
	Long[0] = (double)pWhere->longdeg;
	Long[1] = (double)pWhere->longmin;
	Long[2] = pWhere->longsec;

/*----Write out the site information:
*/
    fprintf(stdout,"{");
    fprintf(stdout, "\"response\": {");
    fprintf(stdout, "\"location\": \"%s\",", pWhere->location);
    fprintf(stdout, "\"latitude\": \"%3d%s%02d'%02.0f\",", pWhere->latdeg,  DEGREESYMBOL, pWhere->latmin,  pWhere->latsec);
    fprintf(stdout, "\"longitude\": \"%+4d%s%02d'%02.0f\",", pWhere->longdeg, DEGREESYMBOL, pWhere->longmin, pWhere->longsec);
    fprintf(stdout, "\"height\": \"%3.1fm\",", pWhere->height);

/*----For each body (sun and/or moon) selected:
*/
	for(ib=0; ib<2; ib++) {
		switch(ib) {
		case 0:
			if (!pWhere->eventCode[ib]) continue;
			tableHeading = "Position of the Sun";
			body = 1;
			break;
		case 1:
			if (!pWhere->eventCode[ib]) continue;
			tableHeading = "Position of the Moon";
			body = 2;
			break;
		}

        fprintf(stdout, "\"event_type\": \"%s\",", tableHeading);

        if (pWhere->tzdescr[0]) {
            fprintf(stdout, "\"time_zone_description\": \"%s\",", pWhere->tzdescr);
        }
        fprintf(stdout, "\"time_zone\": \"%+5.2f hours\",", pWhere->tzhours);

/*----Do some preparation:
*/
		cartesianCoords(&phi, &lambda, xs, pWhere->height, lat, Long);
		if (debug) (void)fprintf(debugDest, "phi=%f\nlambda=%f\n", phi, lambda);

/*----Calculate the position of the body:
*/
		calculatePosition(&(azimuth.radian), &(elevation.radian),
		 &(refraction.radian), xs, phi, lambda, pWhen, pWhere->tzhours, body);

/*----Convert the answers to degrees, minutes and seconds:
*/
		azimuth.decDeg = azimuth.radian*DEGREESPERRADIAN;
		elevation.decDeg = elevation.radian*DEGREESPERRADIAN;
		refraction.decDeg = refraction.radian*DEGREESPERRADIAN;

		(void)toDMS(azimuth.decDeg, &azimuth.deg, &azimuth.min,
		 &azimuth.sec, roundto, &sign);
		if (debug&&sign) (void)fprintf(debugDest, "azimuth answer from toDMS() was negative!!\n");
		if (debug) (void)fprintf(debugDest, "azimuth=%02d %02d' %02.0f\"\n", azimuth.deg, azimuth.min, azimuth.sec);

		(void)toDMS(elevation.decDeg, &elevation.deg, &elevation.min,
		 &elevation.sec, roundto, &sign);
		if (sign) elevation.deg = - elevation.deg;
		if (debug) (void)fprintf(debugDest, "elevation=%02d %02d' %02.0f\"\n", elevation.deg, elevation.min,
		 elevation.sec);

		(void)toDMS(refraction.decDeg, &refraction.deg, &refraction.min,
		 &refraction.sec, roundto, &sign);
		if (debug&&sign) (void)fprintf(debugDest, "refraction answer from toDMS() was negative!!\n");
		if (debug) (void)fprintf(debugDest, "refraction=%02d %02d' %02.0f\"\n", refraction.deg, refraction.min,
		 refraction.sec);

/*----Write out the results:
*/
        fprintf(stdout, "\"event_comment\": \"%s\",", "Altitude includes astronomical, refraction angle for a standard atmosphere.");
        fprintf(stdout, "\"date\": \"%02d/%02d/%04d\",",  pWhen->day, pWhen->month, pWhen->year);
        fprintf(stdout, "\"time\": \"%02d:%02d:%02d\",",  pWhen->hour, pWhen->minute, pWhen->second);
        fprintf(stdout, "\"refraction\": \"%02d%s%02d'%02.0f\",",  refraction.deg, DEGREESYMBOL, refraction.min, refraction.sec);
        fprintf(stdout, "\"azimuth\": \"%02d%s%02d'%02.0f\",", azimuth.deg, DEGREESYMBOL, azimuth.min, azimuth.sec);
        fprintf(stdout, "\"elevation\": \"%3d%s%02d'%02.0f\"", elevation.deg, DEGREESYMBOL, elevation.min, elevation.sec);
	}
 fprintf(stdout, "}}");
/*----Finish up:
*/
   return(0);
}
