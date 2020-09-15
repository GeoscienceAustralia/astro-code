/*---------------------------------------------------------------------------
initialisecalculation.c initialiseCalculation()
	Function for the moon rise and set package.
History:
	04Jun02	Translated from Delphi into C.  Function name changed from
				initialiseComputation() to avoid clashing with the module used
				for the sun rise and set package. (Brock Bryce)
	13May86	Put into a TurboPascal or Delphi function. (Klaus P Voigt)
	?			Original Fortan code. (John Luck)
---------------------------------------------------------------------------*/
#include <math.h>			/* for sin() and cos()	*/
#ifdef UNIX
#include "astro.h"	/* for NUMBER_OF_MONTHS, TWOPI	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

void initialiseCalculation(
	double	moon_rise[NUMBER_OF_MONTHS+1][34],
	double	moon_set[NUMBER_OF_MONTHS+1][34],
	double  *alg,
	double  *sphi,
	double  *cphi,
	double  *zero,
	int 		mnDay[NUMBER_OF_MONTHS+1],
	double  *lat,	/* the last 3 arguments are input only and need */
	double  *Long,	/* not have	been VAR ones in the Pascal. BB	*/
	int 		iyear)
/* In Delphi:
FUNCTION Initialise_computation(
				VAR rise     : triseSet;
				VAR moon_set : triseSet;
				VAR alg      : double;
				VAR sphi     : double;
				VAR cphi     : double;
				VAR mnDay    : tmonth;
				VAR lat      : tAngle;
				VAR long     : tAngle;
				VAR iyear    : int) : bool;
   initialise some variables
   initialise_computation(rise,moon_set,alg,sphi,cphi,mnDay,lat,long)
*/

{
	double	phi;
	int		i, j;
	extern bool	debug;
	extern FILE *debugDest;

/*----Set the days in each month:
*/
	/* mnDay[0] is not used.	*/
	mnDay[ 1] = 31;
	/* Calculate number of days in February:	*/
	mnDay[ 2] = daysInFebruary(iyear);
	mnDay[ 3] = 31;
	mnDay[ 4] = 30;
	mnDay[ 5] = 31;
	mnDay[ 6] = 30;
	mnDay[ 7] = 31;
	mnDay[ 8] = 31;
	mnDay[ 9] = 30;
	mnDay[10] = 31;
	mnDay[11] = 30;
	mnDay[12] = 31;

/*----Initialise the moon_rise and _set arrays:
*/
	for(i=1; i<=NUMBER_OF_MONTHS; i++) {
		for(j=1; j<=31; j++) {
			moon_rise[i][j] = 999.0;
			moon_set[i][j] = 999.0;
		}
	}

/*----Initialise the alg, cphi and sphi variables:
*/
	phi = rdian(lat[0], lat[1], lat[2]);
	if (debug) (void)fprintf(debugDest,
	 "phi (latitude in radians) set to %f\n", phi);
	*alg = rdian(Long[0], Long[1], Long[2])/TWOPI;
	if (debug) (void)fprintf(debugDest,
	 "alg (longitude in decimal days) set to %f\n", *alg);
	*sphi = sin(phi);
	*cphi = cos(phi);

	*zero = rdian(90.0, 34.0, 0.07960);

	return;
}
