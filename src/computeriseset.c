/*---------------------------------------------------------------------------
computeriseset.c, computeRiseSet()
Purpose:
	Calculates the sun rise and sun set times expressed as fractions of a day
	(i.e. 0.25 is 6am) for a range of dates.  The prediction type determines
	whether it is a real sunrise time or that of one of the three twilights, or
	the user may request the times for a given zenith angle expressed as degrees
	down from the vertical, i.e. 90 would give the standard sunrise or set time.
History:
	13Jun02	Now correctly handles transit predictions even when the sun does
				not cross the horizon all day. (BB)
	18May02	Modified to handle transit time predictions. (BB)
	22Apr02	Translated from Delphi to C at the request of National Mapping.
				All comments in the original have been retained.  My understanding
				is that this code was originally written by John Luck in Fortran
				and translated into TurboPascal and later Delphi by Klaus Voigt.
				(Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"
#else
#include "..\\commonfnctns\\astro.h"
#endif
#include <math.h>

void computeRiseSet(int start_day,  /* lower range of day	*/
				int finish_day,	/* upper range of loop	*/
				int start_month,	/* lower range of month	*/
				int finish_month,	/* upper range of loop	*/
				double pdeg,		/* zenith angle degrees	*/
				double pmin,		/* zenith angle minutes	*/
				int itwi,			/* prediction type	*/
				double zone,		/* time zone in decimal hours		*/
				double phi,			/* latitude as a decimal value	*/
				double alg,			/* longitude as a decimal value	*/
				double ddstr,		/* JulianDay for noon 31/12/1899	*/
				double djmon[NUMBER_OF_MONTHS+2], /* JulianDays 1st of ea.month*/
				double sun_rise[NUMBER_OF_MONTHS+1][34],	/* fraction of a day	*/
				double sun_set[NUMBER_OF_MONTHS+1][34])	/* fraction of a day	*/

/**     this routine computes the rise and set times in fraction of	*/
/**     a day, values returned are in rise and set.	*/
{
	bool		southernHemisphere;
	bool		summer;
	double	aa;
	double	alpha;
	double	anom;
	double	c;
	double	d;
	double	dam;
	double	delta;
	double	dum;
	double	e;
	double	eccen;
	double	gst;
	double	h;
	double	obl;
	double	peri;
	double	stnon;
	double	sunlg;
	double	t;
	double	tran;
	double	v;
	int		i;  /* loop control	*/
	int		j;  /* loop control	*/
	extern bool	debug;
	extern FILE *debugDest;

	if (debug) (void)fprintf(debugDest,
	 "\nEntering fnctn computeRiseSet()...\n");
	if (debug) (void)fprintf(debugDest, "itwi=%d\n", itwi);
	if (debug) (void)fprintf(debugDest,
	 "start_day=%d, end_day=%d\n", start_day, finish_day);
	if (debug) (void)fprintf(debugDest,
	 "start_month=%d, end_month=%d\n", start_month, finish_month);
	if (debug) (void)fprintf(debugDest, "pdeg=%f, pmin=%f\n", pdeg, pmin);
	if (debug) (void)fprintf(debugDest,
	 "zone=%f, phi=%f, alg=%f\n", zone, phi, alg);

/**  for each month	*/
	for(i=start_month; i<finish_month+1; i++) {
/**   for each day of a month	*/
		for(j=start_day; j<finish_day+1; j++) {
			/* if (debug) (void)printf("day=%d and month=%d\n", j, i);	*/

/**      d is the nos. of days from 1900 jan. 0.5 to local noon today	*/
			d = (djmon[i] - ddstr) + (j - 1.0) + 0.5 - zone / 24.0;
			/* if (debug) (void)fprintf(debugDest, "d=%4.2lf\n", d);	*/
			t = d / 36525.0;

/**      calculate elements	*/

			dum = 358.47583 + 0.985600267*d - 0.15E-3*t*t - 0.3E-5*t*t*t;
			dam = dum - ((int)dum/360)*360;
			anom = rdian(dam, 0.0, 0.0);
			eccen = 0.01675104 - 0.418E-4*t - 0.126E-6*t*t;
			dum = 281.22083 + 0.470684E-4*d + 0.453E-3*t*t + 0.3E-5*t*t*t;
			dam = dum - ((int)dum/360)*360;
			peri = rdian(dam, 0.0, 0.0);
			dum = 23.452294 - 0.0130125*t - 0.164E-5*t*t;
			dam = dum - ((int)dum/360)*360;
			obl = rdian(dam, 0.0, 0.0);
			t = (d + zone / 24.0 - 0.5) / 36525.0;
			gst = 23925.836 + 8640184.542 * t + 0.0929 * t * t;
			aa = (int)gst / 86400.0;
			aa = (int)aa * 86400.0;
			stnon = (gst - aa) / 86400.0;
			stnon = stnon + alg / 6.2831853;
			stnon = stnon + (0.5 - zone / 24.0) * 1.00273791;

/**      stnon is local siderial time at local noon (standard time), in	*/
/**      fractions of a day	*/

/**      do until stnon lt 1.0	*/
			while(stnon >= 1.0) stnon = stnon - 1.0;

			e = eccen;
			c = anom;
			v = c + (2.0 * e - 0.25 * e * e * e) * sin(c);
			v = v + 1.25 * e * e * sin(2.0 * c);
			v = v + (13.0 / 12.0) * e * e * e * sin(3.0 * c);
			sunlg = v + peri;

/**      do until sunlg lt 6.2831853	*/
			while(sunlg >= 6.2831853) sunlg = sunlg - 6.2831853;

/**      do until sunlg ge 0	*/
			while(sunlg < 0.0) sunlg = sunlg + 6.2831853;

			alpha = atan(cos(obl) * sin(sunlg) / cos(sunlg));
			if ((alpha >= 0.0) && (sunlg >= 3.1415927)) alpha = alpha + 3.1415927;

			if ((alpha < 0.0) && (sunlg < 3.1415927)) alpha = alpha + 3.1415927;

			if ((alpha < 0.0) && (sunlg >= 3.1415927)) alpha = alpha + 6.2831853;

			delta = (sin(obl) * sin(sunlg));
			delta = (delta / sqrt(1.0 - delta * delta));
			delta = atan(delta);
			semidi(alpha, delta, stnon, phi, &tran, &h, itwi, pdeg, pmin);

/**      semidi returns local transit time (tran) and semi-diurnal arc	*/
/**      (h) both in fractions of a day	*/

			if (itwi!=TRANSITPREDCTN) {
				sun_rise[i][j] = tran - h;
				sun_set[i][j]  = tran + h;
			} else {
				sun_rise[i][j] = tran;
			}

			/* if (debug) (void)fprintf(debugDest,
			 "Values of tran and h were %f and %f\n", tran, h);	*/

			/* semidi() sets h to 999999.9 at times	*/
			if (h>=99999.0) {
				sun_rise[i][j] = 0.0;
				sun_set[i][j] = 0.0;
				southernHemisphere = (phi<0.0);
				summer = (i>=4 && i<=9);	/* i is the month	*/
				if (southernHemisphere) summer = !summer;
				if (itwi==TRANSITPREDCTN && summer) sun_rise[i][j] = tran;
			}	/* if	sun does not cross the horizon at all	*/
		}  /* for each day of the month	*/
	}	/* for each month	*/
	return;
}
