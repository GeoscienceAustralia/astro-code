/*---------------------------------------------------------------------------
nutation.c, nutation()
	Calculates nutation parameters delta(psi) & delta(epsilon) from the 1980
	theory of j.m. wahr, at a given date.
	Abstracted from k. yokoyama's merit standard 'astrometry' tape.
Note Regarding Translation Into C:
	Since the original Fortran, TurboPascal, and then Delphi code all used
	arrays starting at 1, my practice during the translation into C was
	normally to make all arrays one cell longer and then completely ignore
	cell (or row and column) zero; thus subscripts used in the code itself
	could be left unchanged.  The array frq[][] below is an example of this;
	you can see the zeros I have added into the initialization statement.
	However with the 106*9 (in the original code) array ANUT, I have left
	the array size unchanged and modified all subscript references in the
	code by subtracting one from each. (BB)
Inputs:
	julianDay:	full julian date, incl. fractional part of day if wanted.
outputs:
	dpsi,deps:	nutation parameters, in radians.
Returns:
	nil.
History:
	18Jun02	Translated from Delphi in<=C. (Brock Bryce)
	Earlier	history unknown, but see also sunmoonposn.c's prologue.
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "astro.h"	/* for dMod() prototype, ARCSEC, TWOPI definitions	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

void nutation(double julianDay, double *dpsi, double *deps)
{
	double anut[106][9] = {
		/* A massive predefined 106*9 array of nutation data:	*/
/*	(J:=1,9),(I:=1,20) */
		{0.0,0.0,0.0,0.0,1.0,-171996.0E-4,-174.2E-4,92025.0E-4,8.9E-4},
		{0.0,0.0,2.0,-2.0,2.0,-13187.0E-4,-1.6E-4,5736.0E-4,-3.1E-4},
		{0.0,0.0,2.0,0.0,2.0,-2274.0E-4,-0.2E-4,977.0E-4,-0.5E-4},
		{0.0,0.0,0.0,0.0,2.0,2062.0E-4,0.2E-4,-895.0E-4,0.5E-4},
		{0.0,1.0,0.0,0.0,0.0,1426.0E-4,-3.4E-4,54.0E-4,-0.1E-4},
		{1.0,0.0,0.0,0.0,0.0,712.0E-4,0.1E-4,-7.0E-4,0.0E-4},
		{0.0,1.0,2.0,-2.0,2.0,-517.0E-4,1.2E-4,224.0E-4,-0.6E-4},
		{0.0,0.0,2.0,0.0,1.0,-386.0E-4,-0.4E-4,200.0E-4,0.0E-4},
		{1.0,0.0,2.0,0.0,2.0,-301.0E-4,0.0E-4,129.0E-4,-0.1E-4},
		{0.0,-1.0,2.0,-2.0,2.0,217.0E-4,-0.5E-4,-95.0E-4,0.3E-4},
		{1.0,0.0,0.0,-2.0,0.0,-158.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{0.0,0.0,2.0,-2.0,1.0,129.0E-4,0.1E-4,-70.0E-4,0.0E-4},
		{-1.0,0.0,2.0,0.0,2.0,123.0E-4,0.0E-4,-53.0E-4,0.0E-4},
		{1.0,0.0,0.0,0.0,1.0,63.0E-4,0.1E-4,-33.0E-4,0.0E-4},
		{0.0,0.0,0.0,2.0,0.0,63.0E-4,0.0E-4,-2.0E-4,0.0E-4},
		{-1.0,0.0,2.0,2.0,2.0,-59.0E-4,0.0E-4,26.0E-4,0.0E-4},
		{-1.0,0.0,0.0,0.0,1.0,-58.0E-4,-0.1E-4,32.0E-4,0.0E-4},
		{1.0,0.0,2.0,0.0,1.0,-51.0E-4,0.0E-4,27.0E-4,0.0E-4},
		{2.0,0.0,0.0,-2.0,0.0,48.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{-2.0,0.0,2.0,0.0,1.0,46.0E-4,0.0E-4,-24.0E-4,0.0E-4},
/* (J:=1,9),(I:=21,40) */
		{0.0,0.0,2.0,2.0,2.0,-38.0E-4,0.0E-4,16.0E-4,0.0E-4},
		{2.0,0.0,2.0,0.0,2.0,-31.0E-4,0.0E-4,13.0E-4,0.0E-4},
		{2.0,0.0,0.0,0.0,0.0,29.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{1.0,0.0,2.0,-2.0,2.0,29.0E-4,0.0E-4,-12.0E-4,0.0E-4},
		{0.0,0.0,2.0,0.0,0.0,26.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{0.0,0.0,2.0,-2.0,0.0,-22.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{-1.0,0.0,2.0,0.0,1.0,21.0E-4,0.0E-4,-10.0E-4,0.0E-4},
		{0.0,2.0,0.0,0.0,0.0,17.0E-4,-0.1E-4,0.0E-4,0.0E-4},
		{0.0,2.0,2.0,-2.0,2.0,-16.0E-4,0.1E-4,7.0E-4,0.0E-4},
		{-1.0,0.0,0.0,2.0,1.0,16.0E-4,0.0E-4,-8.0E-4,0.0E-4},
		{0.0,1.0,0.0,0.0,1.0,-15.0E-4,0.0E-4,9.0E-4,0.0E-4},
		{1.0,0.0,0.0,-2.0,1.0,-13.0E-4,0.0E-4,7.0E-4,0.0E-4},
		{0.0,-1.0,0.0,0.0,1.0,-12.0E-4,0.0E-4,6.0E-4,0.0E-4},
		{2.0,0.0,-2.0,0.0,0.0,11.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{-1.0,0.0,2.0,2.0,1.0,-10.0E-4,0.0E-4,5.0E-4,0.0E-4},
		{1.0,0.0,2.0,2.0,2.0,-8.0E-4,0.0E-4,3.0E-4,0.0E-4},
		{0.0,-1.0,2.0,0.0,2.0,-7.0E-4,0.0E-4,3.0E-4,0.0E-4},
		{0.0,0.0,2.0,2.0,1.0,-7.0E-4,0.0E-4,3.0E-4,0.0E-4},
		{1.0,1.0,0.0,-2.0,0.0,-7.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,1.0,2.0,0.0,2.0,7.0E-4,0.0E-4,-3.0E-4,0.0E-4},
/* (J:=1,9),(I:=41,60) */
		{-2.0,0.0,0.0,2.0,1.0,-6.0E-4,0.0E-4,3.0E-4,0.0E-4},
		{0.0,0.0,0.0,2.0,1.0,-6.0E-4,0.0E-4,3.0E-4,0.0E-4},
		{2.0,0.0,2.0,-2.0,2.0,6.0E-4,0.0E-4,-3.0E-4,0.0E-4},
		{1.0,0.0,0.0,2.0,0.0,6.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,0.0,2.0,-2.0,1.0,6.0E-4,0.0E-4,-3.0E-4,0.0E-4},
		{0.0,0.0,0.0,-2.0,1.0,-5.0E-4,0.0E-4,3.0E-4,0.0E-4},
		{0.0,-1.0,2.0,-2.0,1.0,-5.0E-4,0.0E-4,3.0E-4,0.0E-4},
		{2.0,0.0,2.0,0.0,1.0,-5.0E-4,0.0E-4,3.0E-4,0.0E-4},
		{1.0,-1.0,0.0,0.0,0.0,5.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,0.0,0.0,-1.0,0.0,-4.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,0.0,0.0,1.0,0.0,-4.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,1.0,0.0,-2.0,0.0,-4.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,0.0,-2.0,0.0,0.0,4.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{2.0,0.0,0.0,-2.0,1.0,4.0E-4,0.0E-4,-2.0E-4,0.0E-4},
		{0.0,1.0,2.0,-2.0,1.0,4.0E-4,0.0E-4,-2.0E-4,0.0E-4},
		{1.0,1.0,0.0,0.0,0.0,-3.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,-1.0,0.0,-1.0,0.0,-3.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{-1.0,-1.0,2.0,2.0,2.0,-3.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{0.0,-1.0,2.0,2.0,2.0,-3.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{1.0,-1.0,2.0,0.0,2.0,-3.0E-4,0.0E-4,1.0E-4,0.0E-4},
/* (J:=1,9),(I:=61,80) */
		{3.0,0.0,2.0,0.0,2.0,-3.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{-2.0,0.0,2.0,0.0,2.0,-3.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{1.0,0.0,2.0,0.0,0.0,3.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{-1.0,0.0,2.0,4.0,2.0,-2.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{1.0,0.0,0.0,0.0,2.0,-2.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{-1.0,0.0,2.0,-2.0,1.0,-2.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{0.0,-2.0,2.0,-2.0,1.0,-2.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{-2.0,0.0,0.0,0.0,1.0,-2.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{2.0,0.0,0.0,0.0,1.0,2.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{3.0,0.0,0.0,0.0,0.0,2.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,1.0,2.0,0.0,2.0,2.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{0.0,0.0,2.0,1.0,2.0,2.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{1.0,0.0,0.0,2.0,1.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,0.0,2.0,2.0,1.0,-1.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{1.0,1.0,0.0,-2.0,1.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,1.0,0.0,2.0,0.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,1.0,2.0,-2.0,0.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,1.0,-2.0,2.0,0.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,0.0,-2.0,2.0,0.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,0.0,-2.0,-2.0,0.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
/* (J:=1,9),(I:=81,100) */
		{1.0,0.0,2.0,-2.0,0.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,0.0,0.0,-4.0,0.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{2.0,0.0,0.0,-4.0,0.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,0.0,2.0,4.0,2.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,0.0,2.0,-1.0,2.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{-2.0,0.0,2.0,4.0,2.0,-1.0E-4,0.0E-4,1.0E-4,0.0E-4},
		{2.0,0.0,2.0,2.0,2.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,-1.0,2.0,0.0,1.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,0.0,-2.0,0.0,1.0,-1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,0.0,4.0,-2.0,2.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,1.0,0.0,0.0,2.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,1.0,2.0,-2.0,2.0,1.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{3.0,0.0,2.0,-2.0,2.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{-2.0,0.0,2.0,2.0,2.0,1.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{-1.0,0.0,0.0,0.0,2.0,1.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{0.0,0.0,-2.0,2.0,1.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,1.0,2.0,0.0,1.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{-1.0,0.0,4.0,0.0,2.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{2.0,1.0,0.0,-2.0,0.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{2.0,0.0,0.0,2.0,0.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
/* (J:=1,9),(I:=101,106) */
		{2.0,0.0,2.0,-2.0,1.0,1.0E-4,0.0E-4,-1.0E-4,0.0E-4},
		{2.0,0.0,-2.0,0.0,1.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{1.0,-1.0,0.0,-2.0,0.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{-1.0,0.0,0.0,1.0,1.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{-1.0,-1.0,0.0,2.0,1.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4},
		{0.0,1.0,0.0,1.0,0.0,1.0E-4,0.0E-4,0.0E-4,0.0E-4}
	};
	double	arg;
	double	argnut[6];	/* cell zero not used	*/
	double	frq[6][6] = {	/* zero row & column are not used	*/
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{0.0, 485866.733, 1325.0,   715922.633,  31.310,   0.064},
		{0.0, 1287099.804,   99.0,  1292581.224, - 0.577, - 0.012},
		{0.0, 335778.877, 1342.0,   295263.137, -13.257,   0.011},
		{0.0, 1072261.307, 1236.0,  1105601.328, - 6.891,   0.019},
		{0.0, 450160.280,  - 5.0, - 482890.539,   7.455,   0.008} };
	double	r;
	double	secarg;
	double	tj;
	double	tj2;
	double	tj3;
	int		i, j;

/*----Time variables and fundamental arguments:
*/
	tj = (julianDay-2451545.0)/36525.0;
	tj2 = tj*tj;
	tj3 = tj*tj2;
	*dpsi = 0.0;
	*deps = 0.0;
	for(j=1; j<=5; j++) {
		r = frq[j][2]*tj;
		r = dMod(r, 1.0)*TWOPI;

		secarg = frq[j][1] + frq[j][3]*tj + frq[j][4]*tj2 + frq[j][5]*tj3;
		secarg = dMod(secarg, 1296000.0);
		argnut[j] = secarg*ARCSEC + r;
	}

	for(i=1; i<=106; i++) {
		arg = 0.0;
		for(j=1; j<=5; j++) arg = arg + anut[i-1][j-1]*argnut[j];
		*dpsi = *dpsi + (anut[i-1][5] + anut[i-1][6]*tj)*sin(arg);
		*deps = *deps + (anut[i-1][7] + anut[i-1][8]*tj)*cos(arg);
	}

	*dpsi = *dpsi*ARCSEC;
	*deps = *deps*ARCSEC;

	return;
}
