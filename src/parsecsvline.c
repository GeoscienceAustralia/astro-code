#if 0
/* TEST HARNESS FOR parseCSVLine()	*/
/* to build this use
cc -o testit parseCSVLine.c
after changing the 0 in line 1 to 1.	*/

#include <stdio.h>
int parseCSVLine(char *, int *, int *);
int debug = 3;

void main(void)
{
	FILE *fptr;
	int fieldStartP[25];
	int fieldLength[25];
	int i;
	char buffer[100];

	for(i=0; i<25; i++) {
		fieldStartP[i] = 0;
		fieldLength[i] = 0;
	}
	fptr = fopen("testfile.txt", "r");
	fgets(buffer, 99, fptr);
	parseCSVLine(buffer, fieldStartP, fieldLength);
	fclose(fptr);
}
#endif

/*---------------------------------------------------------------------------
parsecsvline.c, parseCSVLine()
Purpose:
	To look at a line in a file and locate the starting positions and lengths
		of the fields within it, assuming only that all fields are separated by
		commas. Literals (surrounded by single or double quotes) must not contain
		commas or else this function will split the literals at the commas.
Results:
	The fieldstartp array is an array of integers, set up by this fnctn, such
	that the int value in fieldstartp[0] is the starting position (i.e. first
	character) in the 'string' character array of the first field,
	fieldstartp[1] is the starting position (i.e. first character) in 'string'
	of the second field, etc..  The fieldlength array is an array of integers,
	set up by this fnctn, such that the int value in fieldlength[0] is the
	length of the first field in the 'string' character array,
	fieldlength[1]...of the second...etc..
	This fnctn is called by fnctn getCSVField().
Warnings:
	Trailing blanks and trailing double-quotes ARE included in a field's
	length by this fnctn; the fnctn getCSVField() removes all trailing
	whitespace characters and double-quotes from a field before sending the
	field up to its calling routine.
Returns:
	The number of fields found.
History:
	23Apr02	Written based on the function parseGiaLine() by the same author.
				(Brock Bryce)
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifdef UNIX
#include "astro.h"
#else
#include "..\\commonfnctns\\astro.h"
#endif

#define	A_BLANK		(string[i]==' ')
#define	A_DOUBLEQUOTE	(string[i]=='"')
#define	A_COMMA		(string[i]==',')
#define	A_TAB			(string[i]=='\t')
#define	DDQMARKER	(0x11)			/* double double-quote marker	*/
#define 	CMMAMARKER	(0x12)
#define	BLNKMARKER	(0x13)
#ifndef	NULC
#define	NULC			(0x00)
#endif
#define	TABMARKER	(0x14)

int parseCSVLine(char *string, int *fieldstartp, int *fieldlength)
{
	int	i;							/* Subscript to array 'string'.			*/
	int	nextptr = 0;			/* Subscript to arrays 'fieldstartp' and
											'fieldlength'.		*/
	bool	inafield = false;
	extern bool	debug;

	if (debug>2) (void)fprintf(stdout,
	"parseCSVLine: String as received was as follows:\n%s", string);

/*----Simply pick the
------position of the first character of each field, and the position one past
------the last position of each field.  The difference between these two is
------stored as the length (which may be zero):
*/
	inafield = false;
	for(i=0; string[i]!=NULC && string[i]!='\n'; i++) {
		switch(string[i]) {
			case ' ':
			case '\t':		/* These 4 lines mean that blanks and double-quotes	*/
			case '"':		/* are omitted from the beginnings of strings, but	*/
				break;		/* are left in if at the end of strings.	*/
			case '\r':
			case '\n':
				break;
			case ',':
				if (inafield) {
					fieldlength[nextptr] = i - fieldstartp[nextptr];
					nextptr++;
					inafield = false;
				} else {		/* a NULL field */
					fieldstartp[nextptr] = i;
					fieldlength[nextptr++] = 0;
				}
				break;
			default:
				if (!inafield) {
					fieldstartp[nextptr] = i;
					inafield = true;
				}
				break;
		}
	}
	if (!inafield) {		/* the last token was a comma	*/
		fieldstartp[nextptr] = i;
		fieldlength[nextptr] = 0;
	} else {
		fieldlength[nextptr] = i - fieldstartp[nextptr];
	}
	nextptr++;

/*----Return the number of fields found:
*/
	if (debug>2) {
		(void)fprintf(stdout,
		"Values of fieldstartp and fieldlength are as follows:\n");
		for(i=0; i<nextptr; i++) (void)fprintf(stdout, " %3d", i);
		(void)fprintf(stdout, "\n");
		for(i=0; i<nextptr; i++) (void)fprintf(stdout, " %3d", fieldstartp[i]);
		(void)fprintf(stdout, "\n");
		for(i=0; i<nextptr; i++) (void)fprintf(stdout, " %3d", fieldlength[i]);
		(void)fprintf(stdout, "\n");
	}
	return(nextptr);
}
