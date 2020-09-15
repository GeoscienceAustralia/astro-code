/*---------------------------------------------------------------------------
readmoonephile.c readMoonEphile()
	Reads the moon ephemeris file for the year in question; if it can't find
	the ephemeris file for that year, it calls makeMoonEphile() to create it.
	Data from the eph file is stored into the arrays alpha, delta, gst and
	ph, for each of the 375 days from the minus 3rd of January (I make this the
	28th of December of the previous year) through to the 36th of December.
Note 1:
	Cell 0 of each of the arrays alpha, delta etc., is never accessed.  This is
	a hangover from the code originally being written in Fortran, and then in
	TurboPascal in which one could define arrays starting at cell 1.
Note 2:
	Although the moon ephemeris files are 376 lines long, only the first 375
	lines ever seem to be read.  I don't know why this is so. BB
Returns:
	0	for successful completion;
	1	if year passed in is invalid;
	2	if cannot find and cannot create new ephemeris file;
	3	if file found but cannot be opened;
	4	if file records are too long for our buffer length;
	5	if some problem reading the file;
	6	if file is incomplete;
	7  file is corrupt; or
	8	if sscanf() call failed to return six values.
History:
	08Jun02	Rewritten in C and tested. (Brock Bryce)
	07Jan92	Turned into a TurboPascal or Delphi procedure. (Klaus P Voigt)
	1970s?	Originally written in Fortran. (Dr John Luck)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for BUFFERLEN, NULC definitions	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

int readMoonEphile(int iyear, double *alpha, double *delta, double *gst,
	double *hp)
{
	char		buffer[BUFFERLEN+1];
	char	  *env_var;
	char		fileName[FLNAMLEN+1];
	char		theRest[9];
	double	unused = 3.0;
	FILE	  *fptr;
	int		charsin;
	int		i;
	int		jyear = 0;
	int		ret;
	extern bool	debug;
	extern FILE *debugDest;

	if (debug) (void)fprintf(debugDest, "Entering fnctn readMoonEphile()...\n");

/*----Check year is valid:
*/
	/* Here I have simply translated the Delphi code. Personally I think any
	year less than 1600 is probably invalid. BB	*/
	if (iyear<1000) return(1);

/*----Construct the moon ephemeris file's full name:
*/
	char *tmp_dir = getenv("TMP_DIR");
	if (tmp_dir) {
		(void)strcpy(fileName, tmp_dir);
	} else {
		(void)strcpy(fileName, "");	/* any better suggestions?	*/
	}
#ifdef UNIX
	(void)strcat(fileName, "moon");
#else
	(void)strcat(fileName, "\\moon");
#endif
	(void)sprintf(theRest, "%4d.eph", iyear);
	(void)strcat(fileName, theRest);
	if (debug) (void)fprintf(debugDest,
	 "Ephemeris file's name: %s\n", fileName);

/*----Does that year's moon ephemeris file already exist?  If not create it:
*/
	if (!f_exist(fileName)) {
		if (makeMoonEphile(iyear, fileName)) return(2);
	}

/*----Open the file:
*/
	fptr = fopen(fileName, "r");

/*----Read the data out of the file, into the arrays:
*/
	for(i=1; i<EPHARRAYLEN; i++) {
		if (fgets(buffer, BUFFERLEN, fptr)!=NULL) {
			charsin = strlen(buffer);
			if (charsin==BUFFERLEN-1 && buffer[charsin-1]!='\n') return(4);
			ret = sscanf(buffer, "%lf %le %le %le %le %d",
			 &unused, &alpha[i], &delta[i], &gst[i], &hp[i], &jyear);
			if (ret!=6) return(8);
		} else {
			if (ferror(fptr)) return(5);	/* problem reading file	*/
			return(6);							/* premature EOF			*/
		}
		if (jyear!=iyear) return(7);
	}

/*----Close the file:
*/
	(void)fclose(fptr);
	return(0);
}
