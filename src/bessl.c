/*---------------------------------------------------------------------------
bessl.c 	bessl()
	Presumably implements the bessel mathematical function.
History:
	03Jun02	Translated from Delphi into C. All comments in the original have
				been retained :-) (Brock Bryce)
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "broxbool.h"
#else
#include "..\\commonfnctns\\broxbool.h"
#endif

double bessl(double f[6], double x)
/* values in f[] and x are not altered by this fnctn. BB	*/
{
	bool		done;
	double	answer;
	int		i, l;

	done = false;
	i = 0;
	while ((i<5) && !done) {
		i++;
		l = i - 3;
		if (fabs(x-l)<=0.000001) {
			answer = f[i];
			done = true;
		}  /*	fabs(x-l)<=0.000001	*/
	} /* while	*/

	if (!done) answer =
	 (x-2.0)*(x-1.0)*x*(x+1.0)*(x+2.0)*(f[5]/(x-2.0) -
	 4.0*f[4]/(x-1.0) + 6.0*f[3]/x - 4.0*f[2]/(x+1.0) + f[1]/(x+2.0))/24.0;

	return(answer);
}
