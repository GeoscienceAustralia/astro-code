/*---------------------------------------------------------------------------
mooncoords.c, moonCoords()
	Calculates the rectangular cartesian coordinates of the moon with respect
	to the centre of the earth in the mean ecliptic and equinox of date.  It is
	a sub-set of Brown's theory of the moon extracted from 'improved lunar
	ephemeris 1952-1959' (1954).  Only terms with coefficients greater than 5
	arcsec (1 arcsec in parallax) have been retained, so it is necessarily
	approximate.
Inputs:
	dj		Julian date.
Returns:
	nil
History:
	17Jun02	Translated from Delphi into C. (Brock Bryce)
	Earlier	history unknown, but see also sunmoonposn.c's prologue.
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "astro.h"	/* for dMod() prototype, ARCSEC, TWOPI, AEGEOD defns	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif
void moonCoords(double dj, double ecliptic[4])
{
	double a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
	double a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28;
	double a29, a30, a31, a32, a33, a34, a35, a37, a38, a39, a40, a41, a42;
	double a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54;
	double b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b17;
	double b20, b25, b28, b29, b38, b39, b44, b48, b19, b21, b22, b27, b30;
	double b31, b32, b33, b34, b37, b40, b41, b46, b47;

	double	along;
	double	beta;
	double	cd;
	double	cf;
	double	cl;
	double	dl;
	double	ff;
	double	hp;
	double	r;
	double	s;
	double	t;

/*---------------------------------------------------------------------------
	Time and fundamental arguments:
	t  : julian centuries since 1900.0
	cl : mean anomaly of moon
	dl : mean anomaly of sun
	ff : (argument of perigee + mean anomaly) of moon
	cd : mean elongation of moon from sun
---------------------------------------------------------------------------*/

	t = (dj - 2415020.0)/36525.0;
	cl = 1065977.0 + 1717915857.0*t + 33.0*t*t;
	cl = dMod(cl*ARCSEC, TWOPI);
	dl = 1290513.0 + 129596579.0*t;
	dl = dMod(dl*ARCSEC, TWOPI);
	ff = 40503.0 + 1739527291.0*t - 12.0*t*t;
	cf = dMod(ff*ARCSEC, TWOPI);
	cd = 1262655.0 + 1602961611.0*t - 5.0*t*t;
	cd = dMod(cd*ARCSEC, TWOPI);

/*---------------------------------------------------------------------------
	longitude (along), latitude (beta) and horizontal parallax (hp)
---------------------------------------------------------------------------*/

	a1 = sin(cl);
	b1 = cos(cl);
	a2 = -a1;
	b2 = -b1;
	a3 = 2.0*a1*b1;
	b3 = 2.0*b1*b1 - 1.0;
	a4 = a3*b1 + a1*b3;
	b4 = b3*b1 - a3*a1;
	a5 = sin(dl);
	b5 = cos(dl);
	a6 = -a5;
	b6 = -b5;
	a7 = 2.0*a5*b5;
	b7 = 2.0*b5*b5 - 1.0;
	a8 = sin(cf);
	b8 = cos(cf);
	a9 = 2.0*a8*b8;
	b9 = 2.0*b8*b8 - 1.0;
	a10 = -a8;
	b10 = -b8;
	a11 = sin(cd);
	b11 = cos(cd);
	a12 = -a11;
	b12 = -b11;
	a13 = 2.0*a11*b11;
	b13 = 2.0*b11*b11 - 1.0;
	a14 = -a13;
	b14 = -b13;
	a15 = a11*b13 + a13*b11;
	a16 = -a15;
	a17 = 2.0*a13*b13;
	b17 = 2.0*b13*b13 - 1.0;
	a18 = -a17;
	a19 = a9*b13 - a13*b9;
	b19 = b9*b13 + a13*a9;
	a20 = a5*b13 - a13*b5;
	b20 = b5*b13 + a5*a13;
	a21 = a5*b11 + a11*b5;
	b21 = b5*b11 - a11*a5;
	a22 = a5*b13 + a13*b5;
	b22 = b5*b13 - a13*a5;
	a23 = a7*b13 - a13*b7;
	a24 = a9*b1 - a1*b9;
	a25 = a5*b1 - a1*b5;
	b25 = b1*b5 + a1*a5;
	a26 = a1*b11 - a11*b1;
	a27 = a1*b11 + a11*b1;
	b27 = b1*b11 - a11*a1;
	a28 = a1*b13 + a13*b1;
	b28 = b1*b13 - a1*a13;
	a29 = a1*b13 - a13*b1;
	b29 = b1*b13 + a1*a13;
	a30 = a1*b17 - a17*b1;
	b30 = b1*b17 + a17*a1;
	a31 = a3*b13 + a13*b3;
	b31 = b3*b13 - a13*b3;
	a32 = a3*b13 - a13*b3;
	b32 = b3*b13 + a13*a3;
	a33 = a3*b17 - a17*b3;
	b33 = b3*b17 + a17*a3;
	a34 = a4*b13 - a13*b4;
	b34 = b4*b13 + a13*a4;
	a35 = a1*b9 + a9*b1;
	a37 = a1*b9 - a9*b1;
	b37 = b1*b9 + a9*a1;
	a38 = a1*b5 + a5*b1;
	b38 = b1*b5 - a1*a5;
	a39 = a1*b5 - a5*b1;
	b39 = b1*b5 + a1*a5;
	a40 = a3*b5 - a5*b3;
	b40 = b3*b5 + a5*a3;
	a41 = a3*b5 + a5*b3;
	b41 = b3*b5 - a5*a3;
	a42 = a25*b13 - a13*b25;
	a43 = a25*b13 + a13*b25;
	a44 = a38*b13 - a13*b38;
	b44 = b38*b13 + a38*a13;
	a45 = a38*b13 + a13*b38;
	a46 = a39*b13 - a13*b39;
	b46 = b39*b13 + a13*a39;
	a47 = a39*b13 + a13*b39;
	b47 = b39*b13 - a13*a39;

	s = ff - 113.0*a11 + 2373.0*a13 + 14.0*a17 + 193.0*a28 -
	 14.0 *a27 + 22609.0*a1 - 4578.0*a29 - 39.0*a30 +
	 15.0*a31 + 768.0*a3 - 153.0*a32 - 34.0*a33 + 51.0*a4 -
	 16.0*a34 - 25.0*a22 + 18.0*a21 - 127.0*a5 - 165.0*a20 -
	 16.0*a23 - 12*a45 - 115.0*a38 - 182.0*a44 - 24.0*a43 -
	 139.0*a25 - 32.0*a42 - 11.0*a41 + 12.0*a40 - 52.0*a19 -
	 85.0*a24;

	a48 = a8*b13 - a13*b8;
	b48 = b8*b13 + a8*a13;
	a49 = a8*b29 + a29*b8;
	a50 = a8*b1 - a1*b8;
	a51 = a48*b1 - a1*b48;
	a52 = a8*b3 - a3*b8;
	a53 = a5*b48 + a48*b5;
	a54 = a48*b5 - a5*b48;

	beta = ARCSEC*(18518.0*sin(ARCSEC*s) - 6.0*sin(3.0*ARCSEC*s)
	 - 526.0*a48 + 44.0*a49 + 21.0*a50 - 31.0*a51 - 25.0*a52
	 - 23.0*a53 + 11.0*a54);

	r = 13.0*a17 + 2370.0*a13 + 192.0*a28 + 22640.0*a1 - 4586.0*a29
	 - 38.0*a30 - 24.0*a22 - 668.0*a5 - 165.0*a20 - 125.0*a11
	 + 14.0*a31 + 769.0*a3 - 212.0*a32 - 31.0*a33 - 110.0*a38
	 - 206.0*a44 + 15.0*a47 + 148.0*a39 + 28.0*a46 - 412.0*a9
	 - 55.0*a19 + 19.0*a26 + 18.0*a21 + 36.0*a4 - 13.0*a34 - 45.0
	 *a35 + 40.0*a37;

	along = ARCSEC*(973563.0 + 1732564379.0*t - 4*t*t + r);
	along = dMod(along, TWOPI);

	hp = ARCSEC*(3422.70 + 0.26*b17 + 28.23*b13 + 3.09*b28 + 186.54
	 *b1 + 34.31*b29 + 0.60*b30 - 0.30*b22 - 0.40*b5 + 1.92
	 *b20 - 0.98*b11 + 0.28*b31 + 10.17*b3 - 0.30*b32 + 0.37
	 *b33 - 0.95*b38 + 1.44*b44 + 0.23*b47 + 1.15*b39 - 0.23
	 *b46 - 0.11*b19 - 0.11*b27 + 0.15*b21 + 0.62*b4 - 0.12
	 *b34 - 0.10*b41 + 0.13*b40 - 0.71*b37);

/*---------------------------------------------------------------------------
	Convert from angular coordinates to ecliptic cartesians:
---------------------------------------------------------------------------*/

	r = AEGEOD / sin(hp);
	ecliptic[1] = r*cos(beta)*cos(along);
	ecliptic[2] = r*cos(beta)*sin(along);
	ecliptic[3] = r*sin(beta);

	return;
}
