/*---------------------------------------------------------------------------
printyearsun.c, printYearSun()
Purpose:
	Called by processSite() to print a whole year of rise and set values.
History:
    27Jul20 Modified to return JSON. (Bill Farmakis)
	11Jun02	Modified to print pluses or minuses if the sun does not rise or
				set all day. (BB)
	14May02	First written and tested. (Brock Bryce)
---------------------------------------------------------------------------*/
#include <stdio.h>
#ifdef UNIX
#include "astro.h"
#include "broxbool.h"
#else
#include "..\\commonfnctns\\astro.h"
#include "..\\commonfnctns\\broxbool.h"
#endif

void printYearSun(FILE *ofp, double sun_rise[NUMBER_OF_MONTHS+1][34],
	double sun_set[NUMBER_OF_MONTHS+1][34], int *mday, int itwi, double lat)
{
	bool	southernHemisphere;
	bool	summer;
	bool	sunUpOrDownAllDay = false;
	char	cRise[5], cSet[5]; /* times expressed as 4 characters e.g. "0630"	*/
	int	half; /* half=0 means JAN thru JUN, half=1 means JUL thru DEC	*/
	int	i;
	int	iday; 		/* Day of the month.		*/
	int	imon = 1; 		/* Month of the year.	*/
	int	mStart = 1, mEnd = 12;	/* start and end months	*/
	extern bool	debug;
	extern FILE *debugDest;
    bool firstRun = true;

	if (debug) (void)fprintf(debugDest, "Entering fnctn printYearSun()...\n");

/*----Decide which hemisphere we're in:
*/
	southernHemisphere = (lat<0.0);
    fprintf(stdout, "\"data\": [");
    for(imon=mStart; imon<=mEnd; imon++) {
        for(iday=1; iday<32; iday++) {

            /* decide whether this is summer or winter:	*/
            summer = (imon>=4)&&(imon<=9);	/* Apr thru Sep inclusive.	*/
            if (southernHemisphere) summer = !summer;
            if (iday<=mday[imon]) {
                if (sun_rise[imon][iday]==0.0) {
                    sunUpOrDownAllDay = true;
                    if (summer) strcpy(cRise, "  + ");
                    else			strcpy(cRise, "  - ");
                } else {
                    decday2time(sun_rise[imon][iday], cRise);
                }
                /* If a transit prediction, transit time is stored in the
                sun_rise[] array and the sun_set[] array is not used.	*/
                if (itwi!=TRANSITPREDCTN) {	/* set value of cSet	*/
                    if (sun_set[imon][iday]==0.0) {
                        sunUpOrDownAllDay = true;
                        if (summer) strcpy(cSet, "  + ");
                        else			strcpy(cSet, "  - ");
                    } else {
                        decday2time(sun_set[imon][iday], cSet);
                    }
                }

                if (firstRun) {
                    firstRun = false;
                } else {
                    fprintf(stdout, ",");
                }

                fprintf(stdout, "{");
                fprintf(stdout, "\"month\": \"%d\",", imon);
                fprintf(stdout, "\"day\": \"%d\",", iday);
                fprintf(stdout, "\"rise\": \"%s\",", cRise);
                if (itwi!=TRANSITPREDCTN) {
                    fprintf(stdout, "\"set\": \"%s\"", cSet);
                } else {
                    fprintf(stdout, "\"set\": \"\"");
                }
                fprintf(stdout, "}");
            }
        }
    }

    fprintf(stdout, "]");

    if (sunUpOrDownAllDay) {
        if (itwi!=TRANSITPREDCTN) {
            fprintf(stdout, ",\"event_comment\": \"%s\"", "Sun above the horizon all day");
        } else {
            fprintf(stdout, ",\"event_comment\": \"%s\"", "Sun below the horizon all day");
        }

        sunUpOrDownAllDay = false;
    }

	return;
}
