#if 0
#include <stdio.h>
#ifdef UNIX
#include "broxbool.h"
#else
#include "..\\commonfnctns\\broxbool.h"
#endif
int toDMS();

int main()
{                                  /* TEST HARNESS */
	double decdeg, decsec, roundto;
	int degrees, minutes, ret;
	bool sign;

	(void)printf("Round the answers to (in seconds)? ");
	(void)scanf("%lf", &roundto);
	/* for (decdeg = -370.52; decdeg<390.0; decdeg += 80.0) { */
	for (decdeg = 1.0; decdeg<1.01; decdeg += 0.0001) {
		ret = toDMS(decdeg, &degrees, &minutes, &decsec, roundto, &sign);
		(void)printf("%s%+9.4lf%s%3d %02d %06.3lf%s",
		"Decimal degrees ", decdeg, " = ", degrees, minutes, decsec,
		" DMS");
		if (sign) {
			(void)printf(" negative.  (%d)\n", ret);
		} else {
			(void)printf("            (%d)\n", ret);
		}
	}
	return(0);
}
#endif
/*========================================================================
toDMS.c
	Converts a "decimal degree" value into degrees, minutes and seconds.
	This process may seem trivial, but can be calculated inaccurately
	if a less appropriate algorithm is chosen.
	N.B. If a negative value is supplied, the "negative" flag is set to TRUE.
	The returned degrees, minutes and decimal seconds values will always be
	positive.
Parameters:
	decimal_degrees.  INPUT
	degrees.          OUTPUT
	minutes.          OUTPUT
	decimal_seconds.  OUTPUT
	roundto				INPUT	Note if roundto is set to 0, no rounding takes place.
	negative          OUTPUT
Returned values:
	If the input is > 360.0 in magnitude, a value of 2 is returned;
	if the input is > 180.0 but <= 360.0 in magnitude, 1 is returned;
	if the input is <= 180.0 in magnitude, a value of 0 is returned.
History:
	11Oct00	Modified the rounding code to use the floor() function instead
				of simply using (int).  The latter was overflowing and giving
				wrong answers when the user asked for more than about six
				decimal places of the seconds value (ridiculous anyway, but we
				can't have the program giving wrong answers). (BB)
	02Feb99	Removed conditional compilation lines which allowed K&R
				prototyping for Sun4 machines; now full prototyping applies to
				all machines. (BB)
	13Mar92	Fixed bug caused by using *minutes++ and *degrees++ when
				(*minutes)++ and (*degrees)++ were intended. (BB)
	09Mar92	Added the parameter 'roundto'.  Before using this, one could have a
				seconds value of 59.9 returned and, displaying it using %02.0
				(because you didn't want decimal places shown), get 60 seconds
				printed out.  Now if you know you want to display to the nearest 1
				you pass in the value 1.0 as the 'roundto' parameter. If you want
				to display to the nearest minute, pass in 60.0, not 0.0.  (BB)
	27Jun90  Converted into C. (BB)
	21Dec88  Written by Brock Bryce in Fortran.
=========================================================================*/
#include <math.h>
#ifdef UNIX
#include "broxbool.h"
#else
#include "..\\commonfnctns\\broxbool.h"
#endif

int toDMS(double decimal_degrees, int *degrees, int *minutes,
	double *decimal_seconds, double roundto, bool *negative)
{
	double decimal_minutes;

	*negative = (decimal_degrees<0.0);
	decimal_degrees = fabs(decimal_degrees);  /* We only change local copy.*/
	*degrees = (int)( decimal_degrees );
	decimal_minutes = decimal_degrees*60.0 - (double)(*degrees*60);
	*minutes = (int)( decimal_minutes );
	*decimal_seconds = decimal_minutes*60.0 - *minutes*60;

	if (roundto>0.0)
	 *decimal_seconds = roundto*floor(*decimal_seconds/roundto + 0.5);
	/* I could have used my roundnearest() function instead of the line above.	*/

	if (*decimal_seconds==60.0) {
		*decimal_seconds = 0.0;
		(*minutes)++;
		if (*minutes==60) {
			*minutes = 0;
			(*degrees)++;
		}
	}

	if (decimal_degrees>360.0) {
		return(2);
	} else {
		if (decimal_degrees>180.0) {
			return(1);
		} else {
			return(0);
		}
	}
}
