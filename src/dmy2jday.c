#if 0
/*   TEST HARNESS FOR dmy2jday()	*/
#include <stdio.h>
double dmy2jday(int day, int month, int year);

void main(void)
{
	int year = 1601;
	int month = 1;
	int day = 1;
	double jday;

	jday = dmy2jday(day, month, year);
	(void)fprintf(stdout,
	 "For %d/%d/%d, julian date is %.1lf\n", day, month, year, jday);

	/* Answers for dates in 1600 are wrong (1 too high) but as the comments
	say the algorithm is only good for 1800-2099 then that is not a problem.
	If 1/1/1600 is 2305447.5 and 1600 was a leap year then 1/1/1601 should be
	2305813.5.	*/
}
#endif
/*---------------------------------------------------------------------------
dmy2jday.c
Purpose:
	Given a date as a day, month and year, return the value of the Julian Day.
Warning:
	Valid for the range 1800 to 2099.
History:
	11May02	Rewritten in C at the request of National Mapping.  Shortened
				the name. (Brock Bryce)
	27Sep91	Rewritten in TurboPascal or Delphi. (Klaus P Voigt)
	22Apr88	First written and tested, probably in Fortran by John Luck.
---------------------------------------------------------------------------*/
int daysInFebruary(int iyear);	/* prototype	*/

double dmy2jday(int day, int month, int year)
/* FUNCTION Day_month_year_to_julian_day(day   : INTEGER;
                                      month : INTEGER;
                                      year  : INTEGER) : DOUBLE;
                                                   <880422.1134>
   Author :    K. P. Voigt     27/09/91

*$S "Day_month_year_to_julian_day"
 Routine to compute julian day, valid for a range from 1800 to 2099.
 Include
   USES
     Date_sub;

    julian_day = Day_month_year_to_julian_day(day,month,year);

    Where :
      day        : INTEGER   date for which julian day is to be computed.
      julian_day : DOUBLE;
      month      : INTEGER
      year       : INTEGER
*$E
CONST
  days_in_month : ARRAY [1 .. 12] OF BYTE = (0, 31, 28, 31, 30, 31
                                            , 30, 31, 31, 30, 31, 30);

VAR
  centuries_in_julian_year_period            : INTEGER;
  days_in_year                               : INTEGER;
  julian_days                                : LONGINT;
  julian_year                                : LONGINT;
  leap_year_centuries_in_julian_year_period  : INTEGER;
  leap_years_in_julian_year_period           : INTEGER;
  loop                                       : BYTE;

BEGIN
*/
{
	int centuries_in_julian_year_period;
	int days_in_year;
	int leap_year_centuries_in_julian_year_period;
	int leap_years_in_julian_year_period;
	int loop;
	long int julian_days;
	long int julian_year;
	int days_in_month[13] = {0, 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};
	/* yes I know it's a bit bizarre, but I'm only copying what's above - BB.*/

	/* The code I was given had 1900 in here.  I note that the web interface to
	this program won't allow two-digit years through.  For completeness I have
	left the code in anyway, and changed the 1900 to 2000. BB	*/
	if (year < 100) {
		year = year + 2000;
	}

/*----1600 has been chosen to make julian_year a multiple of 400 to allow
------proper leap year computation:
*/
	julian_year = year - 1600;

	days_in_month[3] = daysInFebruary(year);

	leap_years_in_julian_year_period          = (julian_year-1)/4;
	centuries_in_julian_year_period           = (julian_year-1)/100;
	leap_year_centuries_in_julian_year_period = (julian_year-1)/400;

	days_in_year = 0;
	for(loop=1; loop<=month; loop++) {
		days_in_year = days_in_year + days_in_month[loop];
	}

  julian_days = day + julian_year * 365 + leap_years_in_julian_year_period -
                 centuries_in_julian_year_period +
                 leap_year_centuries_in_julian_year_period;

/* 2305447.5 is the julian day number for the start of 1600. For details see
  "The Astronomical Almanac"*/
  /* Day_month_year_to_julian_day = 2305447.5 + julian_days + days_in_year;*/
	return(2305447.5 + julian_days + days_in_year);
}     /*Day_month_year_to_julian_day*/
