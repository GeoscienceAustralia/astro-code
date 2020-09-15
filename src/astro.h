/*---------------------------------------------------------------------------
file astro.h
	Header file containing constants, general purpose type definitions, fnctn
	prototypes, etc. for the astro programs sunrisenset, moonrisenset.
History:
	03Jun02	Modified to be usable for moonrisenset as well as sunrisenset
				program. (BB)
	12Apr02	Derived from WINASTSB.PAS which seems to contain a stack of
				declarations and functions used by the other Astronomical prgrms
				originally written by John Luck (and Mike Paterson?), translated
				into MS Fortran, cared for by Geoff Luton, and converted to
				TurboPascal and then to Delphi by Klaus Voigt.  Finally rewritten
				in C.  I think the SB in the name WINASTSB.PAS is probably for
				"subroutines".  (Brock Bryce)
---------------------------------------------------------------------------*/
#ifndef ASTRO_H
#define ASTRO_H

/*----Other header files:
*/
#include <stdio.h>		/* for fprintf(), FILE *, fopen(), stdout etc.		*/
#include <string.h>		/* for strcmp(), strlen(), strtok() etc.				*/
#include <stdlib.h>		/* for getenv(), atoi(), exit()							*/
#include "broxbool.h"

/*----Symbolic Constants:
*/
#define AEGEOD					6378137.0				/* Earth radius in metres	*/
#define ARCSEC					4.84813681109535999E-6
#define ASTRO_UNIT			1.4959787066E+11  /*1 au, in metres (merit stds)*/
#define BUFFERLEN				512 /* primarily determined by input data length*/
#define DEGREESPERRADIAN	57.29577951308232
#define DEGREESYMBOL			"° "	/* Brings up a degree symbol in HTML;
 replace with a single blank if you need to write to screen or text file.	*/
#define DESCRLEN				40
#define EPHARRAYLEN			376		/* used only by the moonrisenset prgrm	*/
/* #define FLATINV			298.255	 old value used for geodetic position	*/
#define FLATINV				298.25722	/* new value used 4 geodetic pos'n	*/
#define FLNAMLEN				128
#define MAXPREDCTNTYPES		6
				/* a Variable-Value pair is something like "LOCATION=Sydney"	*/
#define MAXVALUELEN			100	/* Max length of values like "...=Sydney"	*/
#define MAXVALUEPAIRS		60	/* Max number of input Variable-Value pairs.	*/
#define MAXVBLELEN			40	/* Max length of vbles like "LOCATION=..."	*/
#define MOON					2			/* used only by the sunmoonposn prgrm	*/
#define NULC					'\0'
#define NUMBER_OF_MONTHS	12
#define PI						3.14159265358979323846
#define RADIANSPERDEGREE	0.01745329251992
#define SUN						1			/* used only by the sunmoonposn prgrm	*/
#define TRANSITPREDCTN		5			/* used only by the sunrisenset prgrm	*/
#define TSCRAD					7.272205216643E-05
#define TWOPI					6.28318530717958647692
#define ZENITHDISTPREDCTN	6			/* used only by the sunrisenset prgrm	*/

/*----Structure declarations:
*/
struct angleType {		/* I should have made more use of this structure	*/
	double	radian;		/* type had I not been so engrossed in simply		*/
	double	decDeg;		/* translating the Delphi, in which KPV used			*/
	int		deg;			/* a type called tAngle, which was basically an		*/
	int		min;			/* array like "double azimuth[3]" containing the 	*/
	double	sec;			/* DMS values, all as doubles.  (BB)					*/
};				/* This type is used to advantage in the sunmoonposn program.	*/

struct dateType {
	int	year;
	int	month;
	int	day;
	int	hour;	/* twenty-four hour clock	*/
	int	minute;
	int	second;
};

struct siteType {
	int		latdeg;
	int		latmin;
	double	latsec;
	double	latitude;
	int		longdeg;
	int		longmin;
	double	longsec;
	double	longitude;
	double	height;
	char		location[DESCRLEN+1];
	char		tzdescr[DESCRLEN+1];
	char		ctzhours[DESCRLEN+1];
	double	tzhours;
	int		eventCode[MAXPREDCTNTYPES];
	int		zenithdeg;
	int		zenithmin;
};

/*----Function prototypes:
*/
double	bessl(double f[6], double x);
double	calcRefractionAngle(double elevation);
void		calculatePosition(
			double  *azimuth, 			/* important returned value	*/
			double  *elevation, 			/* important returned value	*/
			double  *refractionAngle,	/* value returned	*/
			double	xs[4],				/* passed in and not changed	*/
			double	phi,					/* seem to be input only	*/
			double	lambda,				/* seem to be input only	*/
			struct	dateType *pWhen,	/* input only	*/
			double	zone,					/* input only	*/
			int		body);				/* 1=sun, 2=moon	*/
void		calculateRiseSet(
			double	alpha[376],		/* info from the moon ephemeris file	*/
			double	delta[376],		/* info from the moon ephemeris file	*/
			double	gst[376],		/* info from the moon ephemeris file	*/
			double	hp[376],			/* info from the moon ephemeris file	*/
			double	alg,				/* input	*/
			double	sphi,				/* input	*/
			double	cphi,				/* input	*/
			double	zero,				/* input	*/
			double	zone,				/* input	*/
			int		mnDay[NUMBER_OF_MONTHS+1],		/* input	*/
			double	moon_rise[NUMBER_OF_MONTHS+1][34],	/* returned values	*/
			double	moon_set [NUMBER_OF_MONTHS+1][34]);	/* returned values	*/
void		cartesianCoords(double *phi, double *lambda, double xs[4],
			double aHeight, double latitude[3], double longitude[3]);
void		cartPolar(double cartCoor[4], double *radiusVector, double *azimuth,
			double *elevation);
void 		computeRiseSet(
			int 		start_day,  	/* lower range of day	*/
			int 		finish_day,	/* upper range of loop	*/
			int 		start_month,	/* lower range of month	*/
			int 		finish_month,	/* upper range of loop	*/
			double 	pdeg,		/* zenith angle degrees	*/
			double 	pmin,		/* zenith angle minutes	*/
			int 		itwi,			/* prediction type	*/
			double	zone,		/* time zone in decimal hours		*/
			double	phi,			/* latitude as a decimal value	*/
			double	alg,			/* longitude as a decimal value	*/
			double	ddstr,		/* JulianDay for noon 31/12/1899	*/
			double	djmon[NUMBER_OF_MONTHS+2], /* JulianDays 1st of ea.month*/
			double	sun_rise[NUMBER_OF_MONTHS+1][34],	/* fraction of a day	*/
			double	sun_set[NUMBER_OF_MONTHS+1][34]);	/* fraction of a day	*/
int		daysInFebruary(int iyear);
void		decday2time(double decDay, char *timeOday);
int		dequote(char *str);
double	dMod(double numerator, double denominator);
double	dmy2jday(int day, int month, int year);
int		doMoonJob(struct siteType *pWhere, struct dateType *pWhen);
int		doSMPJob(struct siteType *pWhere, struct dateType *pWhen);
void		doSunJob(struct siteType *pWhere, struct dateType *pWhen);
bool		f_exist(char *fileName);
int		fixFunnies(char *buffer, int bufferlen);
int		getCSVField(char *buffer, int fieldNum, char *string);
void		getDate(char *buffer);
double	gmst(double julianDay);
int		hexChar2dec(char *instr);
void 		initialiseCalculation(
			double	moon_rise[NUMBER_OF_MONTHS+1][34],
			double	moon_set[NUMBER_OF_MONTHS+1][34],
			double  *alg,
			double  *sphi,
			double  *cphi,
			double  *zero,
			int 		mnDay[NUMBER_OF_MONTHS+1],
			double  *lat,
			double  *Long,
			int 		iyear);
void		initialiseComputation(
			double  *lat,
			double  *Long,
			double  *phi,
			double  *alg,
			double  *ddstr,
			int 		iyear,
			double  *djmon,
			int	  *mnDay);
int		makeMoonEphile(int iyear, char *fileName);
char	  *monthName(int n);
void		moonCoords(double julianDate, double ecliptic[4]);
void		nutation(double julianDate, double *deltaPsi, double *deltaEpsilon);
double	obliquity(double julianDate);
int		parseCSVLine(char *, int *, int *);
int		parseDateTime(char *buffer, struct dateType *pWhen);
int		populateStructs(char vble[MAXVALUEPAIRS][MAXVBLELEN+1],
			char value[MAXVALUEPAIRS][MAXVALUELEN+1], int noOfValues,
			struct siteType *pWhere, struct dateType *pWhen);
void		printYearMoon(FILE *ofp, double moon_rise[NUMBER_OF_MONTHS+1][34],
			double moon_set[NUMBER_OF_MONTHS+1][34], int *mday);
void		printYearSun(FILE *ofp, double sun_rise[NUMBER_OF_MONTHS+1][34],
			double sun_set[NUMBER_OF_MONTHS+1][34], int *mday, int itwi,
			double lat);
double	rdian(double deg, double min, double sec);
int		readMoonEphile(int iyear, double alpha[376],
			double delta[376], double gst[376], double hp[376]);
void		rotate(int iaxis, double angle, double vector[4]);
void		semidi(double aa, double dd, double ss, double pp, double *tt,
			double *hh, int itwi, double pdeg, double pmin);
int		separateVVpairs(char *buffer, char vble[MAXVALUEPAIRS][MAXVBLELEN+1],
			char value[MAXVALUEPAIRS][MAXVALUELEN+1]);
void		setStartEndTime(struct dateType *pWhen, int *start_day,
			int *end_day, int *start_month, int *end_month);
char	  *strncpy0(char *target, char *source, int length);
void		sunCoords(double julianDate, double ecliptic[4]);
int		toDMS(double decimaldegrees, int *degrees, int *minutes,
			double *decimalSeconds, double roundto, bool *negative);
int		trim(char *string);
int		writeFile(char *fileName);

#endif
