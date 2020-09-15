/*---------------------------------------------------------------------------
sunmoonposn.c Program
	Use a tab setting of 3 when editing these source files. (ts=3 sw=3 in vi)
Purpose:
	Reads a string of input fed in through stdin, in the form
	location=Canberra&latdeg=-35&latmin=12&longdeg=149... and so on.
	Calculates the position of the sun or the moon for the date, time and
	location specifed by the user, where the time is interpreted according to
	the time zone specified in the input.
Command Line Parameters:
	There are no command line parameters any more, now that input is from a web
	page.
Input Data:
	The data are as follows:
	* latitude(DMS, i.e. 3 values),
	* longitude(DMS),
	* height above geoid in metres,
	* location description,
	* time zone description,
	* zone value in decimal hours.
	The date and time are specified in a string of the form
	DD/MM/YYYY,HH:MM:SS
Handling of Errors and Warnings:
	Since this program is now written for web-based use, any problems are
	reported to the output file which is later converted into an HTML page
	and shown to the user.
History:
    06Jul20 v1.2    Modified to remove HTML. (Bill Farmakis)
	22Jun02	v1.1	Rebuilt with a much more bombproof version of the
						separateVVpairs() fnctn. (BB)
	18Jun02	v1.0	Took a copy of moonrisenset program (by the same author) and
						changed it. (Brock Bryce)
	Earlier history when called WINSMPOS prgrm (includes all called functions):
		rev  4.04 19/01/00 KPV	(KPV is Klaus Voigt)
		rev. 4.01 14/10/96 KPV	Fix problem when printing from interactive
		rev. 4.00 10/01/96 KPV	Conversion to Delphi
		rev. 3.01 13/03/92 KPV	Uses BWCC, About
		rev. 3.00 13/01/92 KPV	Conversion to Turbo Pascal for Windows
		rev. 2.03 18/08/89 KPV	Conversion to MS FORTRAN on PC
		rev. 2.02 24/05/88 KPV	Don't abs elm and els when printing
		rev. 2.01 24/05/88 KPV	Check body type limits (1 .. 2)
		rev. 2.00 28/04/88 KPV	Changed batch input format
	Earlier still: written in Fortran. (Dr John Luck)
	-------------------------------------------------
	This program uses Fortran code originally written by Dr John Luck with
	possibly some work done by Mike Paterson and Geoff Luton.  After being
	ported to MS Fortran on a PC, the code was later ported to TurboPascal by
	Klaus Voigt, who then ported it to Borland Delphi, giving it a Windows GUI,
	and enabling it to be run interactively as well as its former batch mode.
	Still later again, after some period of its being run from a distance on the
	webserver machine for users external to National Mapping, it was decided to
	port it to C for web use only, with no Windows GUI.  (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"
#else
#include "..\\commonfnctns\\astro.h"
#endif

/*----Global Variables:
*/
bool	debug = false;
FILE *debugDest;	/* switch to stderr if desired	*/
char	progName[20];
char *version = "1.1";

/*---------------------------------------
						MAIN
---------------------------------------*/
int main( int argc, char *argv[] )
{
    debugDest = stdout;
	char	buffer[BUFFERLEN+1];
	char *env_var;
	char	fileName[100];
	char	pathvar[50];
	char	value[MAXVALUEPAIRS][MAXVALUELEN+1];
	char	vble[MAXVALUEPAIRS][MAXVBLELEN+1];
	int	inputlength;
	int	numberOfVbles = 0;
	int	ret;	/* value returned by certain functions	*/
	struct siteType where;
	struct dateType when;

#ifdef deBUG			/* You can turn on debugging in the makefile with	*/
	debug = true;		/* this, or simply alter the initialisation of the	*/
#endif					/* global variable above.									*/

    strcpy(buffer, argv[1]);
	if (debug) (void)fprintf(debugDest,
	 "Input data read in was as follows:\n%s\n", buffer);

/*----Convert any funny characters e.g. %44 into their appropriate real
------character:
*/
	ret = fixFunnies(buffer, BUFFERLEN+1);
	if (ret && debug) (void)fprintf(debugDest,
	 "fixFunnies() returned %d<BR>\n", ret);

/*----Parse the input data into vble=value pairs:
*/
	numberOfVbles = separateVVpairs(buffer, vble, value);

/*----Store the data into the two structures "where" and "when":
*/
	populateStructs(vble, value, numberOfVbles, &where, &when);

/*----Now process this site:
*/
	doSMPJob(&where, &when);
}
