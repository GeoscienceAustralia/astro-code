#if 0
/* TEST HARNESS FOR rdian()	*/
#include <stdio.h>
#include <math.h>
double rdian(double rd, double rm, double rs);
void main(void)
{
	/* These values should give exactly one radian.	*/
	double rd = 57.0;
	double rm = 17.0;
	double rs = 44.8062;
	double rads;

	rads = rdian(rd, rm, rs);
	(void)fprintf(stdout, "No radians = %lf\n", rads);
}
#endif
/*---------------------------------------------------------------------------
rdian.c, rdian()
Purpose:
	Given an angle in degrees, minutes and seconds, return the radian
	equivalent value.
History:
	28Apr02	First written and tested. (Brock Bryce)
---------------------------------------------------------------------------*/
#include <math.h>

double rdian(double rd, double rm, double rs)
{
	double radian;

	radian = (fabs(rd) + (fabs(rm) + fabs(rs)/60.0)/60.0)/57.2957795;
	if (rd<0.0 || rm<0.0 || rs<0.0) radian = -radian;
	return(radian);
}
