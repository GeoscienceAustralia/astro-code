/*---------------------------------------------------------------------------
parsedatetime.c, parseDateTime()
Purpose:
	This function receives as input a string like one of the following:
	2002
	25/05/2002
	25/05/2002,16:12:00
	It parses the input and puts the values into the dateType structure called
	"when", whose address is pWhen ("pointer to when").
Returns:
	0 if no errors detected;
	1 or more if errors found.
History:
	18Jun02	Wording of error message about time format was improved to suit
				input coming off web form rather than out of a flat file.  Now
				sets time values to -1 instead of 0 in such cases.  (BB)
	25May02	Written from a copy of readDateRec.c which I wrote to digest the
				same information supplied as a record in a file.  (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for BUFFERLEN, <stdio.h> etc.	*/
#include "broxbool.h"
#else
#include "..\\commonfnctns\\astro.h"
#include "..\\commonfnctns\\broxbool.h"
#endif

int parseDateTime(char *buffer, struct dateType *pWhen)
{
	bool	timeField;
	char	tempBuffer[BUFFERLEN+1];
	char	tempField[BUFFERLEN+1];
	int	fieldNo;
	int	i;		/* used to step through the date field.	*/
	extern char	progName[];
	extern bool	debug;
	extern FILE *debugDest;

	if (debug) (void)fprintf(debugDest, " Entering fnctn parseDateTime()...\n");
	if (debug) (void)fprintf(debugDest, " Input string was '%s'\n", buffer);

/*----How many fields are there? i.e. is there a time field?
*/
	timeField = (getCSVField(buffer, 2, tempBuffer)>0);
	if (debug && timeField) (void)fprintf(debugDest, " Time field found.\n");
	if (debug && !timeField) (void)fprintf(debugDest, " Time field not found.\n");

/*----Get the date field, put it into tempField, then change all the
------slashes into commas so we can use getCSVField():
*/
	(void)getCSVField(buffer, 1, tempField);
	for(i=0; tempField[i]; i++) {
		if (tempField[i]=='/') tempField[i] = ',';
	}
	if (debug) (void)fprintf(debugDest, " tempField now is '%s'\n", tempField);

/*----Parse the date field:
*/
	fieldNo = 1;
	while (getCSVField(tempField, fieldNo, tempBuffer)>=0) {
		switch(fieldNo) {
		case 1:
			pWhen->year = atoi(tempBuffer);
			break;
		case 2:
			pWhen->month = pWhen->year;
			pWhen->year = atoi(tempBuffer);
			break;
		case 3:
			pWhen->day = pWhen->month;
			pWhen->month = pWhen->year;
			pWhen->year = atoi(tempBuffer);
			break;
		}
		fieldNo++;
	}
	if (debug) fprintf(debugDest, " %02d/%02d/%04d\n",
	 pWhen->day, pWhen->month, pWhen->year);

/*----Get the time field if there is one:
*/
	if (timeField) {
		(void)getCSVField(buffer, 2, tempField);
		/* Change all the colons to commas so we can use getCSVField() again: */
		for(i=0; tempField[i]; i++) {
			if (tempField[i]==':') tempField[i] = ',';
		}
		for(fieldNo=1; fieldNo<=3; fieldNo++) {
			if (getCSVField(tempField, fieldNo, tempBuffer)<0) {
				(void)fprintf(stdout, "%s: %s", progName,
				 "Time portion of date field was not in HH:MM:SS format.\n");
				 pWhen->hour = pWhen->minute = pWhen->second = -1;
				return(1);
			}
			switch(fieldNo) {
			case 1:
				pWhen->hour = atoi(tempBuffer);
				break;
			case 2:
				pWhen->minute = atoi(tempBuffer);
				break;
			case 3:
				pWhen->second = atoi(tempBuffer);
				break;
			}
		}
		if (debug) fprintf(debugDest, " %02d:%02d:%02d\n",
		 pWhen->hour, pWhen->minute, pWhen->second);
	}

/*----Finish up:
*/
   return(0);
}
