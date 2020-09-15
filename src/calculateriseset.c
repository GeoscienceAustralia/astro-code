/*---------------------------------------------------------------------------
calculateriseset.c calculateRiseSet()
	Function to calculate the moon rise and set times.
History:
	28May02	Translated from Delphi into C.  Sorry about the goto's and labels;
				they presumably date back to the original code.	(Brock Bryce)
	07Jan92	Modified. (KPV)
	13May86	Put into a TurboPascal procedure. (Klaus P Voigt)
	unknown	Original code written in Fortran. (John Luck)
---------------------------------------------------------------------------*/
#include <math.h>
#ifdef UNIX
#include "astro.h"	/* for TWOPI definition etc	*/
#include "broxbool.h"
#else
#include "..\\commonfnctns\\astro.h"
#include "..\\commonfnctns\\broxbool.h"
#endif

void calculateRiseSet(
	double	alpha[EPHARRAYLEN],		/* info from the moon ephemeris file	*/
	double	delta[EPHARRAYLEN],		/* info from the moon ephemeris file	*/
	double	gst[EPHARRAYLEN],			/* info from the moon ephemeris file	*/
	double	hp[EPHARRAYLEN],			/* info from the moon ephemeris file	*/
	double	alg,							/* input	*/
	double	sphi,							/* input	*/
	double	cphi,							/* input	*/
	double	zero,							/* input	*/
	double	zone,								/* input	*/
	int		mnDay[NUMBER_OF_MONTHS+1],		/* input	*/
	double	moon_rise[NUMBER_OF_MONTHS+1][34],	/* returned values	*/
	double	moon_set [NUMBER_OF_MONTHS+1][34])	/* returned values	*/
{

	double	alf;
	double	day;
	double	dec;
	double	delah;
	double	delaz;
	double	h;
	double	p;
	double	par;
	double	prt;
	double	q;
	double	qa[6];	/* cell 0 is not used	*/
	double	qb[6];	/* cell 0 is not used	*/
	double	qc[6];	/* cell 0 is not used	*/
	double	r;
	double	s;
	const double sdcon = 1.00273791E0;
	double	st;
	double	stl;
	double	tmz;		/*	zone over 24	*/
	double	tran;
	double	trnpr;
	double	z;
	int		i;
	int		ijtra;
	int		ikou;
	int		j;
	int		jkou;
	int		k;
	int		kkou;
	int		l;
	int		m;
	int		n;

	tmz = zone / 24.00;
	trnpr = 0.0;

	for(i=3; i<=373; i++) {
		ijtra = 0;

/*------Iterate for lmt of transit (tran):
*/
		for(j=1; j<=5; j++) {
			qa[j] = alpha[i+j-3];
			if (j!=1) {
				if (qa[j]<qa[j-1]) qa[j] = qa[j] + 1.0;
			}
			qb[j] = delta[i+j-3];
			qc[j] = hp[i+j-3];
		}  /* for(j...	*/

		day = i - 4;
		p = -tmz;
		tran = day;
		ikou = 0;

label100:
		alf = bessl(qa, p);
		alf = alf - (int)(alf);
		stl = gst[i] + alg + p*sdcon;
		stl = stl - (int)(stl);
		if (stl<0.0) stl = stl + 1.0;

/*----stl is lst at lmm, initially.
*/
		h = stl - alf;
		if ((ikou>0)&&(h>0.50)) h = h - 1.0;
		if ((ikou>0)&&(h<-0.50)) h = h + 1.0;
		tran = tran - h;
		p = p - h;
		if (fabs(h)>=0.00020) {
			ikou = ikou + 1;
			if (ikou<=5) goto label100;
		}  /* if	*/

/*----Iterate ra, dec, hp to rise & set times.
*/
		q = p - 0.250;
		r = tran - 0.250;
		st = stl - 0.250*sdcon;
		jkou = 0;

label150:
		alf = bessl(qa, q);
		jkou = jkou + 1;
		alf = alf - (int)(alf);
		dec = bessl(qb, q);
		par = bessl(qc, q);
		h = (st-alf)*TWOPI;
		z = sphi*sin(dec) + cphi*cos(dec)*cos(h);
		z = atan2(sqrt(1.00 - z*z), z);
		delaz = z - zero + 0.727554*par;
		if ((fabs(delaz)<=0.00030) || (jkou>5)) {
			q = p + 0.250;
			s = tran + 0.250;
			st = stl + 0.250*sdcon;
			kkou = 0;

label160:
			alf = bessl(qa, q);
			kkou = kkou + 1;
			alf = alf - (int)(alf);
			dec = bessl(qb, q);
			par = bessl(qc, q);
			h = (st-alf)*TWOPI;
			z = sphi*sin(dec) + cphi*cos(dec)*cos(h);
			z = atan2(sqrt(1.00-z*z), z);
			delaz = z - zero + 0.7275540*par;
			if ((fabs(delaz)<=0.00030) || (kkou>5)) {
				k = (int)r;
				if ((k>0)&&(k<=366)) {
					l = 0;
					for(n=1; n<=NUMBER_OF_MONTHS; n++) {
						l = l + mnDay[n];
						m = n;
						if (k<=l) goto label200;
					}  /* for(n...	*/
				}  /* if	*/
				goto label250;
			} else {
				delah = -sin(z)*delaz / (cphi*cos(dec)*sin(h)*TWOPI);
				q = q + delah/sdcon;
				s = s + delah/sdcon;
				st = st + delah;
				goto label160;
			}  /*	if	*/
		} else {
			delah = sin(z)*delaz / (cphi*cos(dec)*sin(h)*TWOPI);
			q = q - delah/sdcon;
			r = r - delah/sdcon;
			st = st - delah;
			goto label150;
		}  /*	if	*/

label200:
		n = m;
		l = k - l + mnDay[n];
		if ((l>0)&&(l<=31)&&(n>0)&&(n<=NUMBER_OF_MONTHS))
		 moon_rise[n][l] = r - k;

label250:
		k = (int)s;
		if ((k>0)&&(k<=366)) {
			l = 0;
			for(n=1; n<=NUMBER_OF_MONTHS; n++) {
				l = l + mnDay[n];
				m = n;
				if (k<=l) goto label300;
			}  /* for(n...	*/
		}  /*	if	*/
		goto label400;

label300:
		n = m;
		l = k - l + mnDay[n];
		if ((l>0)&&(l<=31)&&(n>0)&&(n<=NUMBER_OF_MONTHS)) {
			moon_set[n][l] = s - k;

			prt = tran;
			if ((i==3) || (ijtra==1) || ((tran-trnpr)<1.5)) {
				if (ijtra==0) trnpr = prt;
			} else {
				tran = tran - 1.033330;
				p = p - 1.033330;
				ikou = 1;
				ijtra = 1;
				goto label100;
			}  /*	if	*/
		}  /*	if	*/
label400:
		continue;	/* added 03Jun02, by BB	*/
	}  /*	for(i...	*/
}  /*Calculate_rise_set*/
