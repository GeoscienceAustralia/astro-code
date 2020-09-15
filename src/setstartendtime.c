/*---------------------------------------------------------------------------
setstartendtime.c, setStartEndTime()
Purpose:
	Given a date in the form of a dateType structure, set the start and end
	day and month values.  Basically if only a year value is set then the values
	are set to cover the entire year.  If a specific day is set the start and
	end values are the same.
History:
	01May02	Translated from the Delphi code (below) into C. (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for struct definitions	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

void setStartEndTime(struct dateType *pWhen, int *start_day, int *end_day,
	int *start_month, int *end_month)
{

/*----Get the start and end day and month values from the dateType structure:
*/
	*start_day = *end_day = pWhen->day;
	*start_month = *end_month = pWhen->month;

/*----An input like 26/00/2002 is treated as an error and is converted
------to 00/00/2002 (which means the user wants all of 2002).
*/
	if (*start_day>0 && *start_month<=0) *start_day = 0;

/*----An input like 00/03/2002 is treated as an error and is converted
------to 00/00/2002 (which means the user wants all of 2002).
*/
	if (*start_day<=0 && *start_month>0) *start_month = 0;

/*----Set the start and end days:
*/
	if (*start_day>0) {
		*end_day = *start_day;
		*end_month = *start_month;
	} else {
		*start_day = 1;
		*end_day = 33;
		*start_month = 1;
		*end_month = 12;
	}
}

/* Original Delphi code follows:
FUNCTION Set_start_end_time(VAR start_day      : INTEGER;
									 VAR finish_day     : INTEGER;
                            VAR day_selected   : INTEGER;
                            VAR start_month    : INTEGER;
                            VAR finish_month   : INTEGER;
                            VAR month_selected : INTEGER) : BOOLEAN;
{                                                   <880422.1134>
   Author :    K. P. Voigt     20/12/91

*$S "Set_start_end_time"

    Set_start_end_time(start_day,end_day,day_selected,start_month,finish_month
                      ,month_selected);

    Where :
      VAR start_day      : INTEGER
      VAR end_day        : INTEGER
          day_selected   : INTEGER
      VAR start_month    : INTEGER
      VAR finish_month   : INTEGER
          month_selected : INTEGER
*$E
}

BEGIN
  TRY
    IF (day_selected > 0) AND (month_selected < 1)
      THEN day_selected := 0;

    IF (day_selected < 1) AND (month_selected > 0)
      THEN month_selected := 0;

    IF day_selected < 1
      THEN
        BEGIN
          start_day := 1;
          finish_day := 33;
        END
      ELSE
        BEGIN
          start_day := day_selected;
          finish_day := day_selected;
        END; {if}

    IF month_selected < 1
      THEN
        BEGIN
          start_month := 1;
          finish_month := 12;
        END
      ELSE
        BEGIN
          start_month := month_selected;
          finish_month := month_selected;
        END; {if}
    Result := TRUE;
  EXCEPT
    Result := FALSE;
  END;  {TRY}
END;     {Set_start_end_time}
*/
