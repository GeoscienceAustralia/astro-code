/*---------------------------------------------------------------------------
getcsvfield.c, getCSVField()
Purpose:
	Given a line from a file (the string 'buffer') and a field number, this
	fnctn extracts the field assuming that all fields are comma separated,
	and puts it into a supplied character string 'string'.
Operation:
	Calls parseCSVLine(), to get array of 'pointers' (actually integers).
	Extracts the field as a string, using its pointer and the pointer for
	the next field.
	Trims off trailing junk.
	Replaces any doubled-up doublequotes with a single one.
Returns:
	-1 if not enough fields were present in the input buffer, or
	-2 if user wants a field beyond MAXFIELDS, or
	The length of the string.
History:
	23Apr02	Made by slightly altering a copy of the getField() function,
				written by the same author. (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"
#include "broxbool.h"
#else
#include "..\\commonfnctns\\astro.h"
#include "..\\commonfnctns\\broxbool.h"
#endif
#define MAXFIELDS 30

int getCSVField(char *buffer, int fieldnum, char *string)
{
	int	fieldstartp[MAXFIELDS];
	int	fieldlength[MAXFIELDS];
	int	fieldsfound = 0;
	int	i, j;
	extern bool	debug;

	if (debug>3) (void)fprintf(stdout, "\nEntering fnctn getCSVField()...\n");
	if (debug>3) (void)fprintf(stdout,
	"...looking for field number %d\n", fieldnum);

/*----Check that MAXFIELDS value is adequate:
*/
	if (fieldnum>MAXFIELDS) return(-2);

/*----Call parseCSVLine() to set up the 'fieldstartp' & 'fieldlength' arrays:
*/
	fieldsfound = parseCSVLine(buffer, fieldstartp, fieldlength);
	if (fieldsfound<fieldnum) return(-1);
	fieldnum--;		/* Alter the local copy of fieldnum; field 1 info is stored
							in cell number 0 of the arrays, 2 in 1 etc..		*/

/*----In case the returned string is NULL, initialize 'string':
*/
	string[0] = NULC;

/*----Extract the string using the startposition and length values:
*/
	for(i=0, j=fieldstartp[fieldnum]; i<fieldlength[fieldnum]; i++, j++) {
		string[i] = buffer[j];
	}
	string[i] = NULC;

/*----Trim off trailing junk:
*/
	(void)trim(string);

/*----If the last character is a double-quote, whip it off:
*/
	if (string[strlen(string)-1]=='"') string[strlen(string)-1] = NULC;

/*----Now change any PAIRS of double-quotes to single double-quotes,
------remove any lone double quotes, and return the length of the string:
*/
	if (debug>3) (void)fprintf(stdout,
	"getCSVField: Before dequoting, string was %s\n", string);
	i = dequote(string);
	if (debug>3) (void)fprintf(stdout,
	"getCSVField:  After dequoting, string was %s\n", string);
	if (debug>3) (void)fprintf(stdout,
	"getCSVField: String length of field was %d\n", i);
	return(i);
}
