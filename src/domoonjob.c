/*---------------------------------------------------------------------------
domoonjob.c doMoonJob()
Purpose:
	This function processes the input data for one site and one date.
	Called by moonrisenset.c i.e. main().
Returns:
	0 for successful operation;
	1 for problems.
History:
    06Jul20 Modified to return JSON. (Bill Farmakis)
	22Jun02	Modified to use the HTML degree symbol. (BB)
	15Jun02	Modified to cope with no rise or set in a given day. (BB)
	09Jun02	Copied from doSunJob() (by same author) & modified. (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for TWOPI definition etc	*/
#include "broxbool.h"
#else
#include "..\\commonfnctns\\astro.h"
#include "..\\commonfnctns\\broxbool.h"
#endif

int doMoonJob(struct siteType *pWhere, struct dateType *pWhen)
{
	char	  *tableHeading;
	char		timeOday[5];

	double	alg;		/* longitude in decimal degrees?		*/
	double	alpha[EPHARRAYLEN];	/* in radians	*/
	double 	cphi;		/* probably cos() of the latitude	*/
	double	delta[EPHARRAYLEN];	/* in radians	*/
	double	gst[EPHARRAYLEN];	/* something in decimal days at 0000 U.T.	*/
	double	hp[EPHARRAYLEN];	/* horizontal parallax in radians	*/
	double	lat[3], Long[3];	/* lat & long in degrees, minutes & seconds	*/
	double	moon_rise[NUMBER_OF_MONTHS+1][34];
	double	moon_set[NUMBER_OF_MONTHS+1][34];
	double	sphi;		/* probably sin() of the latitude	*/
	double	zero;		/* this is not 0.0	*/

	int		mnDay[NUMBER_OF_MONTHS+1];	/* Days in January = mnDay[1] etc.	*/
	int		ret;
	int		start_day, end_day, start_month, end_month;

	extern bool	debug;
	extern FILE *debugDest;
	extern char progName[];

	if (debug) {
	    (void)fprintf(debugDest, "Entering fnctn doMoonJob()...\n");
	}

/*----Initialise the heading:
*/
	tableHeading = "TIMES OF MOONRISE AND MOONSET ";

/*----Load the lat and Long arrays:
*/
	lat[0] = (double)pWhere->latdeg;
	lat[1] = (double)pWhere->latmin;
	lat[2] = pWhere->latsec;
	Long[0] = (double)pWhere->longdeg;
	Long[1] = (double)pWhere->longmin;
	Long[2] = pWhere->longsec;

/*----Write out the site-related information:
*/
    fprintf(stdout,"{");
    fprintf(stdout, "\"response\": {");
    fprintf(stdout, "\"location\": \"%s\",", pWhere->location);
    fprintf(stdout, "\"latitude\": \"%3d%s%02d'%02.0f\",", pWhere->latdeg,  DEGREESYMBOL, pWhere->latmin,  pWhere->latsec);
    fprintf(stdout, "\"longitude\": \"%+4d%s%02d'%02.0f\",", pWhere->longdeg, DEGREESYMBOL, pWhere->longmin, pWhere->longsec);
    fprintf(stdout, "\"event_type\": \"%s\",", tableHeading);

/*----Read the moon ephemeris file:
*/
	if ((ret=readMoonEphile(pWhen->year, alpha, delta, gst, hp))>0) {
		if (debug) (void)fprintf(debugDest, "readMoonEphile() returned %d<BR>\n", ret);
		(void)fprintf(stdout, "%s: %s%d%s", progName, "Problem with the moon ephemeris file for ", pWhen->year, "<BR>\nUnable to continue processing.<BR>\n");
		return(1);
	}
	if (debug) (void)fprintf(debugDest, "%s%22.14E %22.14E %22.14E %22.14E\n","Values from ephemeris file for row 2 are\n",	alpha[2], delta[2], gst[2], hp[2]);

/*----Do some preparation:
*/
	initialiseCalculation(moon_rise, moon_set, &alg, &sphi, &cphi, &zero,
	 mnDay, lat, Long, pWhen->year);

	if (debug) (void)fprintf(debugDest, "sphi was set to %f\n", sphi);
	if (debug) (void)fprintf(debugDest, "moon_set[7][13] initialised to %5.1f\n", moon_set[7][13]);

	setStartEndTime(pWhen, &start_day, &end_day, &start_month, &end_month);

	if (debug) (void)fprintf(debugDest, "  start day=%d\n", start_day);
	if (debug) (void)fprintf(debugDest, "    end day=%d\n", end_day);
	if (debug) (void)fprintf(debugDest, "start month=%d\n", start_month);
	if (debug) (void)fprintf(debugDest, "  end month=%d\n", end_month);

/*----Write out the appropriate headings:
*/
//	(void)fprintf(stdout, "%s", tableHeading);
	/* (pWhen->day) is false if a whole year is being processed. */
    if (pWhen->day) {
        fprintf(stdout, "\"event_comment\": \"%s\",", "(for ideal horizon & meteorological conditions)");
    }

    if (!pWhen->day) {
        fprintf(stdout, "\"year\": \"%d\",", pWhen->year);
    }

    if (pWhere->tzdescr[0]) {
        fprintf(stdout, "\"time_zone_description\": \"%s\",", pWhere->tzdescr);
    }
    fprintf(stdout, "\"time_zone\": \"%+5.2f hours\",", pWhere->tzhours);

/*----Calculate the rise and set times:
*/
	calculateRiseSet(alpha, delta, gst, hp, alg, sphi, cphi, zero, pWhere->tzhours, mnDay, moon_rise, moon_set);

	if (debug) (void)fprintf(debugDest, "moon_rise[%d][%d]=%11.7f\n", start_month, start_day, moon_rise[start_month][start_day]);
	if (debug) (void)fprintf(debugDest, "moon_set [%d][%d]=%11.7f\n", start_month, start_day, moon_set[start_month][start_day]);

/*----Write out the results:
*/
	if (pWhen->day) {		/* if a single day	*/
		/* If there has been no rise or set in a particular day, the value
		stored in moon_rise[][] or moon_set[][] will be the value it was
		initialised to, which is 999.0.  Otherwise it will be between 0 & 1.	*/
		if (moon_rise[start_month][start_day]<500) {
			decday2time(moon_rise[start_month][start_day], timeOday);
		} else {
			(void)strcpy(timeOday, "    ");
		}
        fprintf(stdout, "\"date\": \"%02d/%02d/%4d\",", pWhen->day, pWhen->month, pWhen->year);
        fprintf(stdout, "\"rise\": \"%s\",", timeOday);

		if (moon_set[start_month][start_day]<500) {
			decday2time(moon_set[start_month][start_day], timeOday);
		} else {
			(void)strcpy(timeOday, "    ");
		}
		fprintf(stdout, "\"set\": \"%s\"", timeOday);
	} else {				/* a whole year		*/
		printYearMoon(stdout, moon_rise, moon_set, mnDay);
	}
    fprintf(stdout, "}}");
/*----Finish up:
*/
   return;
}
