/*---------------------------------------------------------------------------
printyearmoon.c printYearMoon()
Purpose:
	Called by doMoonJob() to print a whole year of rise and set values.
History:
    27Jul20 Modified to return JSON. (Bill Farmakis)
	15Jun02	Modified to cope with no rise or set in a given day. (BB)
	14May02	First written and tested. (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for TWOPI definition etc	*/
#include "broxbool.h"
#else
#include "..\\commonfnctns\\astro.h"
#include "..\\commonfnctns\\broxbool.h"
#endif

void printYearMoon(FILE *ofp, double moon_rise[NUMBER_OF_MONTHS+1][34],
	double moon_set[NUMBER_OF_MONTHS+1][34], int *mday)
{
	char	cRise[5], cSet[5]; /* times expressed as 4 characters e.g. "0630"	*/
	int	half; /* half=0 means JAN thru JUN, half=1 means JUL thru DEC	*/
	int	i;
	int	iday; 		/* Day of the month.		*/
	int	imon = 1; 		/* Month of the year.	*/
	int	mStart = 1, mEnd = 12;	/* start and end months	*/
	extern bool	debug;
	extern FILE *debugDest;
    bool firstRun = true;

	if (debug) (void)fprintf(debugDest, "Entering fnctn printYearMoon()...\n");

    fprintf(stdout, "\"data\": [");
    for(imon=mStart; imon<=mEnd; imon++) {
        for(iday=1; iday<32; iday++) {
            if (iday<=mday[imon]) {
                if (moon_rise[imon][iday]<500.0) {
                    decday2time(moon_rise[imon][iday], cRise);
                } else {
                    (void)strcpy(cRise, "");
                }
                if (moon_set[imon][iday]<500.0) {
                    decday2time(moon_set[imon][iday], cSet);
                } else {
                    (void)strcpy(cSet, "");
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
                fprintf(stdout, "\"set\": \"%s\"", cSet);
                fprintf(stdout, "}");
            }
        }
    }

    fprintf(stdout, "]");

	return;
}
