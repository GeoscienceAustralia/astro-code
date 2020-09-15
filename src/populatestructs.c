/*---------------------------------------------------------------------------
populatestructs.c, populateStructs()
Purpose:
	Given two arrays, one containing variable names, and the other containing
	(matching) values, extract the values and store then into the siteType and
	dateType structures "where" and "when", whose addresses are pWhere and
	pWhen ("pointer to when").
Returns:
	Zero always.
History:
	18Jun02	Now initialises hour to 12 rather than 0. (BB)
	21May02	First written and tested. There must be a better way of writing
				this function but I can't see what it is.	(Brock Bryce)
---------------------------------------------------------------------------*/
#include <ctype.h>	/* for toupper()				*/
#ifdef UNIX
#include "astro.h"	/* for MAXVALUEPAIRS, structs etc.	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

int populateStructs(char vble[MAXVALUEPAIRS][MAXVBLELEN+1],
						  char value[MAXVALUEPAIRS][MAXVALUELEN+1], int noOfValues,
						  struct siteType *pWhere, struct dateType *pWhen)
{
	char	cdate[MAXVALUELEN+1];
	int	i, j;
	extern bool	debug;
	extern FILE *debugDest;

	if (debug) (void)fprintf(debugDest, "Entering fnctn populateStructs()...\n");
	if (debug) (void)fprintf(debugDest, "noOfValues=%d\n", noOfValues);

/*----Initialise the values in the two structures in case the incoming data
------is short some values:
*/
	pWhen->year = pWhen->month = pWhen->day = 0;
	pWhen->hour = 12;
	pWhen->minute = pWhen->second = 0;
	pWhere->latdeg = pWhere->latmin = 0;
	pWhere->latsec = pWhere->latitude = 0.0;
	pWhere->longdeg = pWhere->longmin = 0;
	pWhere->longsec = pWhere->longitude = 0.0;
	pWhere->height = 0.0;
	(void)strcpy(pWhere->location, "");
	(void)strcpy(pWhere->tzdescr, "");
	(void)strcpy(pWhere->ctzhours, "");
	pWhere->tzhours = 0.0;
	for(i=0; i<MAXPREDCTNTYPES; i++) pWhere->eventCode[i] = 0;
	pWhere->zenithdeg = pWhere->zenithmin = 0;

/*----Use the input values to populate the structures:
*/
	for(i=0; i<noOfValues; i++) {
		if (debug) (void)fprintf(debugDest,
		 "Vble=%s; value=%s\n", vble[i], value[i]);
		for(j=0; vble[i][j]; j++) vble[i][j] = toupper(vble[i][j]);
		if (strcmp(vble[i], "LATDEG")==0) {
			pWhere->latdeg=atoi(value[i]); continue;}
		if (strcmp(vble[i], "LATMIN")==0) {
			pWhere->latmin=atoi(value[i]); continue;}
		if (strcmp(vble[i], "LATSEC")==0) {
			pWhere->latsec=atof(value[i]); continue;}
		if (strcmp(vble[i], "LONGDEG")==0) {
			pWhere->longdeg=atoi(value[i]); continue;}
		if (strcmp(vble[i], "LONGMIN")==0) {
			pWhere->longmin=atoi(value[i]); continue;}
		if (strcmp(vble[i], "LONGSEC")==0) {
			pWhere->longsec=atof(value[i]); continue;}
		if (strcmp(vble[i], "HEIGHT")==0) {
			pWhere->height=atof(value[i]); continue;}
		if (strcmp(vble[i], "LOCATION")==0) {
			(void)strcpy(pWhere->location, value[i]); continue;}
		if (strcmp(vble[i], "TIMEZONENAME")==0) {
			(void)strcpy(pWhere->tzdescr, value[i]); continue;}
		if (strcmp(vble[i], "TIMEZONE")==0) {
			pWhere->tzhours=atof(value[i]); continue;}
		if (strcmp(vble[i], "EVENT")==0) {
			j = atoi(value[i]);
			if (debug) (void)fprintf(debugDest,
			 "Setting pWhere->eventCode[%d] to %d...\n", j-1, j);
			pWhere->eventCode[j-1] = j; continue; }
		if (strcmp(vble[i], "ZENITHDEG")==0) {
			pWhere->zenithdeg=atoi(value[i]); continue;}
		if (strcmp(vble[i], "ZENITHMIN")==0) {
			pWhere->zenithmin=atoi(value[i]); continue;}
		if (strcmp(vble[i], "DATE")==0) {
			if (value[i][0])	{	/* if it's been set in the web page	*/
				(void)strcpy(cdate, value[i]);
			} else {
				getDate(cdate);	/* otherwise, get today's date	*/
			}
			(void)parseDateTime(cdate, pWhen);
			continue;}
		if (debug) (void)fprintf(debugDest,
		 "Variable unrecognised or of no interest: '%s'\n", vble[i]);
	}
	return(0);
}
