#if 0
/* TEST HARNESS FOR decday2time()	*/
#include <stdio.h>
void decday2time(double decDay, char *timeOday);
FILE *debugDest;    // = stderr;

void main(void)
{
	double decDay = 0.272869;	/* this gives 0633 hrs	*/
	char timeOday[5];

	decDay = 0.999306;		/* testing-gives 2359 which is correct	*/
	decDay = 0.0;		/* testing-gives 0000 which I think is OK	*/
	decDay = 1.0;		/* testing-gives 2400 which I don't like	*/
	decDay = 0.5;		/* testing-gives 1200 which is correct */
	decDay = 0.0416666667;		/* testing-gives 0100 which is good	*/
	decDay = 0.0413195;	/* gives 0100	*/
	decDay = 0.0413194;	/* gives 0059	*/
	decday2time(decDay, timeOday);
	(void)fprintf(stdout, "Time of day = %s\n", timeOday);
	debugDest = stderr;
}
#endif
/*---------------------------------------------------------------------------
decday2time.c, decday2time()
Purpose:
	Given a time of day expressed as a decimal day, e.g. 0.272869, convert
	this into a four-character time, e.g. 0633 expressed as HHMM.
Warning:
	This module does no error checking.  Input values less than 0 or greater
	than 1 are silently changed to acceptable values.
History:
	11May02	First written and tested. This module is basically functionally
				equivalent to the CONVT.PAS supplied to me by NMD. (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"
#else
#include "..\\commonfnctns\\astro.h"
#endif

/**
 * The variable timeOday is expected to be defined in the calling module as
 * char timeOday[5], allowing space for the terminating NUL character.
 *
 * @param decDay
 * @param timeOday
 */
void decday2time(double decDay, char *timeOday) {
	double hours, minutes;
	int ihours, iminutes;
	/* extern FILE *debugDest;	*/

	/* extern bool	debug;
	if (debug) (void)fprintf(debugDest, "Entering fnctn decday2time()...\n");
	if (debug) (void)fprintf(debugDest, "Input value was %11.9lf\n", decDay);	*/

	if (decDay < 0.0) {
		decDay = 1.0 + decDay;
	}
	if (decDay > 1.0) {
		decDay = decDay - 1.0;
	}

	/* Offset of 0.00833333333 (half a minute) prevents answers like 0560:	*/
	hours = decDay * 24.0 + 0.00833333333;
	ihours = (int) hours;
	minutes = (hours - (double) ihours) * 60.0;
	iminutes = (int) minutes;

	(void) sprintf(timeOday, "%02d", ihours);
	/* The following statement rounds its answer to the nearest minute	*/
	(void) sprintf(timeOday + 2, "%02d", iminutes);
}

/**
 * Determine whether a decimal day is on the next, previous (or same) day.
 *
 * @param decimalDay 0.0 to 1.0 covers 24 hours, values less than 0.0 are
 * in the previous day, values greater than 1.0 are in the next day
 * @return label for which day is represented by the decimal day
 */
char *decimalDayToRelativeLabel(double decimalDay) {
	if (decimalDay > 1.0) {
		return "NEXT";
	} else if (decimalDay < 0.0) {
		return "PREVIOUS";
	} else {
		return "SAME";
	}
}
