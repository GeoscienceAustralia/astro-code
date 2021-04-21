/*---------------------------------------------------------------------------
dosunjob.c, doSunJob()
Purpose:
	This function processes the input data for one site and one date.
	Called by sunrisenset.c i.e. main().
Returns:
	Nil.
History:
    06Jul20 Modified to return JSON. (Bill Farmakis)
	22Jun02	Modified to use the HTML degree symbol. (BB)
	24May02	Modified to work under new main() program that was written to work
				on the web site, rather than by reading an input file. (BB)
	18May02	Modified to handle transit time predictions. (BB)
	22Apr02	First written and tested. (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"
#include "broxbool.h"
#else
#include "..\\commonfnctns\\astro.h"
#include "..\\commonfnctns\\broxbool.h"
#endif

void doSunJob(struct siteType *pWhere, struct dateType *pWhen)
{
	bool		southernHemisphere;
	bool		summer;
	bool		sunUpOrDownAllDay = false;
	char		cRise[5], cSet[5]; /* times expressed as 4 characters e.g. "0630"	*/
	char	  *tableHeading[MAXPREDCTNTYPES];
	double	ddstr;	/* Julian date for 0.5 Jan 1900	*/
	double 	djmon[NUMBER_OF_MONTHS+2];	/* Julian dates of the 1st of each
				month plus the 1st Jan of the next year. Cell zero of this array
				is not used.	*/
	double	lat[3], Long[3];
	double	sun_rise[NUMBER_OF_MONTHS+1][34];
	double	sun_set[NUMBER_OF_MONTHS+1][34];
	int		i;
	int		itwi;	/* A value between 1 and MAXPREDCTNTYPES.	*/
	int		mday[NUMBER_OF_MONTHS+1];	/* No days in January is mday[1]	etc.	*/
	int		start_day, end_day, start_month, end_month;
	extern bool	debug;
//    bool	debug = true;
	extern FILE *debugDest;

	if (debug) {
	    (void)fprintf(debugDest, "Entering fnctn doSunJob()...\n");
	}

/*----Initialise the headings:
*/
	/* Take care these use no more than MAXPREDCTNTYPES pointers:	*/
	tableHeading[0] = "TIMES OF SUNRISE AND SUNSET ";
	tableHeading[1] = "TIMES OF CIVIL TWILIGHT ";
	tableHeading[2] = "TIMES OF NAUTICAL TWILIGHT ";
	tableHeading[3] = "TIMES OF ASTRONOMICAL TWILIGHT ";
	tableHeading[4] = "TIME OF SUN TRANSIT ";
	tableHeading[5] = "TIMES OF SUNRISE AND SUNSET FOR ZENITH DISTANCE ";

/*----Check the event codes in the input data:
*/
	if (debug) {
		for(i=0; i<MAXPREDCTNTYPES; i++) {
			(void)fprintf(stdout,
			 "pWhere->eventCode[%d]=%d\n", i, pWhere->eventCode[i]);
		}
	}

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

/*----Do some preparation:
*/
	initialiseComputation(lat, Long, &pWhere->latitude, &pWhere->longitude,
	 &ddstr, pWhen->year, djmon, mday);

	setStartEndTime(pWhen, &start_day, &end_day, &start_month, &end_month);
	if (debug) (void)fprintf(debugDest, "  start day=%d\n", start_day);
	if (debug) (void)fprintf(debugDest, "    end day=%d\n", end_day);
	if (debug) (void)fprintf(debugDest, "start month=%d\n", start_month);
	if (debug) (void)fprintf(debugDest, "  end month=%d\n", end_month);

/*----Decide which hemisphere we're in:
*/
	southernHemisphere = (pWhere->latitude<0.0);

/*----Process each prediction:
*/
    bool firstRun = true;
    fprintf(stdout, "\"events\": [");
	for(i=0; i<MAXPREDCTNTYPES; i++) {
		if (debug) (void)fprintf(debugDest,
		 "Looking at event code number %d...\n", i);

		if (pWhere->eventCode[i]) {
		    if (firstRun) {
		        firstRun = false;
		    } else {
		        fprintf(stdout, ",");
		    }

		    fprintf(stdout, "{");
			itwi = pWhere->eventCode[i];

/*----Write out the appropriate headings:
*/
            fprintf(stdout, "\"event_type\": \"%s\",", tableHeading[itwi-1]);
			if (itwi==ZENITHDISTPREDCTN) {
			    fprintf(stdout, "\"zenith_distance_prediction\": \"%02d%s%02d'\",", pWhere->zenithdeg, DEGREESYMBOL, pWhere->zenithmin);
			}
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

/*----Calculate the rise and set or transit time:
*/
			computeRiseSet(start_day, end_day, start_month, end_month,
			 (double)pWhere->zenithdeg, (double)pWhere->zenithmin, itwi,
			 pWhere->tzhours, pWhere->latitude, pWhere->longitude, ddstr,
			 djmon, sun_rise, sun_set);

/*----Write out the results:
*/
			if (pWhen->day) {		/* if a single day	*/
				/* decide whether this is summer or winter:	*/
				summer = (pWhen->month>=4)&&(pWhen->month<=9);	/* Apr thru Sep incl.*/
				if (southernHemisphere) summer = !summer;

				if (sun_rise[start_month][start_day]!=0.0) {
					decday2time(sun_rise[start_month][start_day], cRise);
				} else {
					sunUpOrDownAllDay = true;
					if (summer) strcpy(cRise, "  + ");
					else			strcpy(cRise, "  - ");
				}
				if (itwi!=TRANSITPREDCTN) {
                    fprintf(stdout, "\"date\": \"%02d/%02d/%4d\",", pWhen->day, pWhen->month, pWhen->year);
                    fprintf(stdout, "\"rise\": \"%s\",", cRise);
                    fprintf(stdout, "\"rise_day\": \"%s\",", decimalDayToRelativeLabel(sun_rise[start_month][start_day]));

					if (sun_set[start_month][start_day]!=0.0) {
						decday2time(sun_set[start_month][start_day], cSet);
					} else {
						sunUpOrDownAllDay = true;
						if (summer) strcpy(cSet, "  + ");
						else			strcpy(cSet, "  - ");
					}
                    fprintf(stdout, "\"set\": \"%s\",", cSet);
                    fprintf(stdout, "\"set_day\": \"%s\"", decimalDayToRelativeLabel(sun_set[start_month][start_day]));
				} else {
                    fprintf(stdout, "\"transit_time\": \"%02d/%02d/%4d\",", pWhen->day, pWhen->month, pWhen->year);
                    fprintf(stdout, "\"rise\": \"%s\"", cRise);
				}
				if (sunUpOrDownAllDay) {
                    if (itwi!=TRANSITPREDCTN) {
                        fprintf(stdout, ",\"sun_position\": \"%s\"",  "Sun above the horizon all day");
                    } else {
                        fprintf(stdout, ",\"sun_position\": \"%s\"",  "Sun below the horizon all day");
                    }
                    sunUpOrDownAllDay = false;
				}
			} else {				/* a whole year		*/
				printYearSun(stdout, sun_rise, sun_set, mday, itwi,
				 pWhere->latitude);
			}
			fprintf(stdout, "}");
		}
	}
	fprintf(stdout, "]");
    fprintf(stdout, "}}");
/*----Finish up:
*/
   return;
}
