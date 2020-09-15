/*---------------------------------------------------------------------------
makemoonephile.c makeMoonEphile()
Purpose:
	Given a year, create a "moon ephemeris file" for that year.
	There were almost no comments in the file supplied to BB for translation.
Returns:
	0 for successful operation; or
	1 for could not open the new file.
History:
	09Jun02	Translated into C. (Brock Bryce)
	1990s?	Converted into TurboPascal then Delhpi. (Klaus P Voigt)
	1970s?	First written in Fortran. (Dr John Luck)
---------------------------------------------------------------------------*/
#include <math.h>				/* for sin(), cos(), asin()...			*/
#ifdef UNIX
#include "astro.h"			/* for TWOPI definition						*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

int makeMoonEphile(int iyear, char *fileName)
{
	double	a1;
	double	a2;
	double	a3;
	double	a4;
	double	a5;
	double	a6;
	double	a7;
	double	a8;
	double	a9;
	double	a10;
	double	a11;
	double	a12;
	double	a13;
	double	a14;
	double	a15;
	double	a16;
	double	a17;
	double	a18;
	double	a19;
	double	a20;
	double	a21;
	double	a22;
	double	a23;
	double	a24;
	double	a25;
	double	a26;
	double	a27;
	double	a28;
	double	a29;
	double	a30;
	double	a31;
	double	a32;
	double	a33;
	double	a34;
	double	a35;
	double	a37;
	double	a38;
	double	a39;
	double	a40;
	double	a41;
	double	a42;
	double	a43;
	double	a44;
	double	a45;
	double	a46;
	double	a47;
	double	a48;
	double	a49;
	double	a50;
	double	a51;
	double	a52;
	double	a53;
	double	a54;
	double	along;	/* longitude in radians	*/
	double	alpha;
	double	b1;
	double	b2;
	double	b3;
	double	b4;
	double	b5;
	double	b6;
	double	b7;
	double	b8;
	double	b9;
	double	b10;
	double	b11;
	double	b12;
	double	b13;
	double	b14;
	double	b17;
	double	b20;
	double	b25;
	double	b28;
	double	b29;
	double	b38;
	double	b39;
	double	b44;
	double	b48;
	double	beta;		/* latitude in radians	*/
	double	cd;
	double	cf;
	double	cl;
	double	csalf;
	double	csbet;
	double	cseps;
	double	csdel;
	double	delta;
	double	dl;
	double	dj;		/* Julian day	*/
	double	djMon[NUMBER_OF_MONTHS+2];
	double	eps;
	double	ff;
	double	gst;		/* at 0000 UT in decimal of a day	*/
	double	hp;		/* horizontal parallax in radians	*/
	double	r;
	double	s;
	double	snalf;
	double	snbet;
	double	sndel;
	double	sneps;
	double	t;
	FILE	  *fptr;
	int		iday;
	int		imon;
				/* cell 0 of mnDay[] is never used.	*/
	int 		mnDay[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	extern bool debug;
	extern FILE *debugDest;

	if (debug) (void)fprintf(debugDest,
	 "Entering fnctn makeMoonEphile()...<BR>\n");
	mnDay[2] = daysInFebruary(iyear);

	djMon[1] = dmy2jday(1, 1, iyear);

	for(imon=2; imon<=13; imon++) djMon[imon] = djMon[imon-1] + mnDay[imon-1];

	/* dj starts at 27th December of the previous year but increments to
	the 28th before it is referenced. BB	*/
	dj = djMon[1] - 5.0;

/*----Create a new empty file with the passed-in file name:
*/
	if (debug) (void)fprintf(debugDest,
	 "makeMoonEphile() opening <BR>\n   %s for writing...<BR>\n", fileName);
	fptr = fopen(fileName, "w");
	if (fptr==NULL) return(1);

/*----For the period 28th Dec previous year thru to early Jan next year:
*/
	for(iday=1; iday<377; iday++) {
		dj = dj + 1.0;
		t = (dj - 2415020.0)/36525.0;
		gst = (23925.8360 + 8640184.5420*t + 0.09290*t*t)/86400.0;
		cl = 1065977.0 + 1717915857.0*t + 33.0*t*t;
		cl = cl*ARCSEC;
		dl = 1290513.0 + 129596579.0*t;
		dl = dl*ARCSEC;
		ff = 40503.0 + 1739527291.0*t - 12.0*t*t;
		cf = ff*ARCSEC;
		cd = 1262655.0 + 1602961611.0*t - 5.0*t*t;
		cd = cd*ARCSEC;
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
		a20 = a5*b13 - a13*b5;
		b20 = b5*b13 + a5*a13;
		a21 = a5*b11 + a11*b5;
		a22 = a5*b13 + a13*b5;
		a23 = a7*b13 - a13*b7;
		a24 = a9*b1 - a1*b9;
		a25 = a5*b1 - a1*b5;
		b25 = b1*b5 + a1*a5;
		a26 = a1*b11 - a11*b1;
		a27 = a1*b11 + a11*b1;
		a28 = a1*b13 + a13*b1;
		b28 = b1*b13 - a1*a13;
		a29 = a1*b13 - a13*b1;
		b29 = b1*b13 + a1*a13;
		a30 = a1*b17 - a17*b1;
		a31 = a3*b13 + a13*b3;
		a32 = a3*b13 - a13*b3;
		a33 = a3*b17 - a17*b3;
		a34 = a4*b13 - a13*b4;
		a35 = a1*b9 + a9*b1;
		a37 = a1*b9 - a9*b1;
		a38 = a1*b5 + a5*b1;
		b38 = b1*b5 - a1*a5;
		a39 = -a25;
		b39 = -b25;
		a40 = a3*b5 - a5*b3;
		a41 = a3*b5 + a5*b3;
		a42 = a25*b13 - a13*b25;
		a43 = a25*b13 + a13*b25;
		a44 = a38*b13 - a13*b38;
		b44 = b38*b13 + a38*a13;
		a45 = a38*b13 + a13*b38;
		a46 = a39*b13 - a13*b39;
		a47 = a39*b13 + a13*b39;

		s = ff - 113.0*a11 + 2373.0*a13 + 14.0*a17 + 193.0*a28 -
		 14.0*a27 + 22609.0*a1 - 4578.0*a29 - 39.0*a30 +
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
		 -526.0*a48 + 44.0*a49 + 21.0*a50 - 31.0*a51 - 25.0*a52
		 -23.0*a53 + 11.0*a54);

		r = 13.0*a17 + 2370.0*a13 + 192.0*a28 + 22640.0*a1 - 4586.0 *
		 a29 - 38.0*a30 - 24.0*a22 - 668.0*a5 - 165.0*a20 - 125.0 *
		 a11 + 14.0*a31 + 769.0*a3 - 212.0*a32 - 31.0*a33 - 110.0 *
		 a38 - 206.0*a44 + 15.0*a47 + 148.0*a39 + 28.0*a46 - 412.0 *
		 a9 - 55.0*a19 + 19.0*a26 + 18.0*a21 + 36.0*a4 - 13.0*a34
		 - 45.0*a35 + 40.0*a37;

		along = ARCSEC*(973563.0 + 1732564379.0*t - 4*t*t + r);

		hp = ARCSEC*(3422.70 + 28.2*b13 + 3.1*b28 + 186.5*b1 + 34.3*b29
		 +1.9*b20 + 10.2*b3 + 1.4*b44 + 1.2*b39);
		eps = 0.01745329250*(23.452294 - 0.0130125*t);
		csbet = cos(beta);
		cseps = cos(eps);
		snbet = sin(beta);
		sneps = sin(eps);
		delta = asin(csbet*sin(along)*sneps + snbet*cseps);
		csdel = cos(delta);
		sndel = sin(delta);
		csalf = csbet*cos(along)/csdel;
		snalf = (-snbet*sneps + csbet*cseps*sin(along))/csdel;
		alpha = asin(snalf);
		if (csalf < 0.0) alpha = 3.141592650 - alpha;
		else if (snalf < 0.0) alpha = TWOPI + alpha;

		alpha = alpha/TWOPI;
		alpha = alpha - (int)(alpha);
		if (alpha < 0.0) alpha = alpha + 1.0;

		(void)fprintf(fptr, "%10.2f %22.14E %22.14E %22.14E %22.14E %4d\n",
									dj, alpha, delta, gst, hp, iyear);

	}  /* for(iday...	*/

	(void)fclose(fptr);
	return(0);
}
