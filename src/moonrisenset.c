/*---------------------------------------------------------------------------
moonrisenset.c Program
	Use a tab setting of 3 when editing these source files. (ts=3 sw=3 in vi)
Purpose:
	Reads a string of input fed in through stdin, in the form
	location=Canberra&latdeg=-35&latmin=12&longdeg=149... and so on.
	Calculates the times of moonrise and moonset for the location specifed by
	the user, in terms of the time zone specified by the user.
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
	The date can be either a single day or a complete year, the latter
	specified as either two or four digits. If no date is supplied the
	program assumes today's date.
Handling of Errors and Warnings:
	Since this program is now written for web-based use, any problems are
	reported to the output file which is later converted into an HTML page and
	shown to the user.
History:
    06Jul20 v1.3    Modified to remove HTML. (Bill Farmakis)
	22Jun02	v1.2	Rebuilt with a much more bombproof version of the
						separateVVpairs() fnctn.  Added degree symbols. (BB)
	17Jun02	v1.1	Added three #ifdef deBUG lines. Minor changes to
						presentation of output. (BB)
	09Jun02	v1.0	First C version of the moon-rise-and-set program written,
						based on Delphi code supplied by National Mapping Division.
	This program uses Fortran code originally written by Dr John Luck with some
	work done I believe by Mike Paterson and Geoff Luton.  After being ported
	to MS Fortran on a PC, the code was later ported to TurboPascal by Klaus
	Voigt, who then ported it to Borland Delphi, giving it a Windows GUI, and
	enabling it to be run interactively as well as its former batch mode.
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
FILE *debugDest;    // = stderr;	/* switch to stderr if desired	*/
char	progName[20];
char *version = "1.2";

/*---------------------------------------
						MAIN
---------------------------------------*/
int main( int argc, char *argv[] )
{
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
	debugDest = stderr;

#ifdef deBUG			/* You can turn on debugging in the makefile with	*/
	debug = true;		/* this, or simply alter the initialisation of the	*/
#endif					/* global variable above.									*/

    strcpy(buffer, argv[1]);
	if (debug) {
	    (void)fprintf(debugDest, "Input data read in was as follows:\n%s\n", buffer);
	}

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
	doMoonJob(&where, &when);
}
