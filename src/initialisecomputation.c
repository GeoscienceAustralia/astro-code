#ifdef UNIX
#include "astro.h"
#else
#include "..\\commonfnctns\\astro.h"
#endif

void initialiseComputation(double *lat, double *Long, double *phi,
	double *alg, double *ddstr, int iyear, double *djmon, int *mday)
/* In Delphi:
PROCEDURE Initialise_computation(    lat   : tAngle;
                                     Long  : tAngle;
                                 VAR phi   : DOUBLE;
                                 VAR alg   : DOUBLE;
                                 VAR ddstr : DOUBLE;
                                     iyear : INTEGER;
                                 VAR djmon : tdjmon;
                                 VAR mday  : tMonth);
 this routine does some calculations which are identical for a point
*/
{
	int i;
	extern bool	debug;
	extern FILE *debugDest;

	if (debug) {
		for(i=0; i<3; i++) {
			(void)fprintf(debugDest, "lat[i]=%5.1f and Long[i]=%5.1f\n",
			 lat[i], Long[i]);
		}
	}

	*phi = rdian(lat[0],lat[1],lat[2]);
	*alg = rdian(Long[0],Long[1],Long[2]);

/*----Compute the julian day number for the first of each month:
*/
	for(i=1; i<=NUMBER_OF_MONTHS; i++)  djmon[i] = dmy2jday(1, i, iyear);

/*----For 1st Jan of following year:
*/
	djmon[13] = dmy2jday(1, 1, iyear + 1);

/*----compute number of days in each month:
*/
	for(i=1; i<=NUMBER_OF_MONTHS; i++) mday[i] = (int)(djmon[i+1] - djmon[i]);

/*----Set a constant: this is the julian day for 0.5 Jan 1900 (i.e. the julian
------day for 1 Jan 1900 is 2415020.5):
*/

	*ddstr = 2415020.0;

}
