/*---------------------------------------------------------------------------
separatevvpairs.c, separateVVpairs()
Purpose:
	Given an input string like loc=Spence&TimeZone=A.E.S.T.&lat=-35
	separate the string initially at the & signs into vble=value (VV) pairs,
	then separate the variables from the values into two separate arrays.
Returns:
	The number of vble/value pairs found, or a negative value for various
	error conditions:
	-1	if buffer pointer sent in was a NULL pointer.
	-2 if input string is too long for our stack tempBuffer variable.
History:
	22Jun02	Modified to protect against overflow of MAXVALUEPAIRS,
				MAXVBLELEN, and MAXVALUELEN. (BB)
	21May02	First written and tested.  (Brock Bryce)
---------------------------------------------------------------------------*/
#ifdef UNIX
#include "astro.h"	/* for BUFFERLEN, MAXVALUEPAIRS	*/
#else
#include "..\\commonfnctns\\astro.h"
#endif

int separateVVpairs(char *buffer, char vble[MAXVALUEPAIRS][MAXVBLELEN+1],
	char value[MAXVALUEPAIRS][MAXVALUELEN+1])
{
	int	i;
	int	NOV = 0;		/* number of variables found	*/
	char *leftPtr;		/* Points to LH end of VV pair.	*/
	char	pair[MAXVALUEPAIRS][MAXVALUELEN+MAXVBLELEN+2];
	char *strptr;		/* Points to the right of the = sign.	*/
	char	tempBuffer[BUFFERLEN+1];
	extern bool	debug;
	extern FILE *debugDest;
	extern char progName[];

	if (debug) (void)fprintf(debugDest, "Entering fnctn separateVVpairs()...\n");

/*----Handle the error cases that can occur:
*/
	if (buffer==NULL) return(-1);
	if (strlen(buffer)>BUFFERLEN) return(-2);
	if (debug) (void)fprintf(debugDest, "%s<BR>\n", buffer);

/*----Handle the zero length case:
*/
	if (strlen(buffer)==0) return(0);

/*----Put string into a temporary buffer and split it on the & characters,
------storing the resulting vble/value pairs into the "pair" array:
*/
	(void)strcpy(tempBuffer, buffer);
	leftPtr = strtok(tempBuffer, "&\n");

	while (leftPtr!=NULL) {
		if (NOV>=MAXVALUEPAIRS) {
			(void)fprintf(stdout, "%s: %s%d%s%s%s", progName,
			 "Input data contained more than ", MAXVALUEPAIRS,
			 " variable=value pairs.\n",
			 "   Either reduce the number of input data pairs or rebuild the \n",
			 "   program with a larger MAXVALUEPAIRS value.\n");
			leftPtr = NULL;	/* force an exit from the while loop	*/
		} else {
			if (strlen(leftPtr)>(MAXVALUELEN+MAXVBLELEN+1)) {
				(void)fprintf(stdout, "%s: %s%s%d%s%s%s", progName,
				 "Input data contained a variable=value pair whose combined \n",
				 "   length was more than ", MAXVALUELEN+MAXVBLELEN+1,
				 " characters.  This pair has been truncated.\n",
				 "   The program should be rebuilt with ",
				 "larger values\n   of MAXVALUELEN and/or MAXVBLELEN.\n");
			}
			strncpy0(pair[NOV++], leftPtr, MAXVALUELEN+MAXVBLELEN+1);
			leftPtr=strtok((char *)NULL, "&\n");
		}
	}
	if (debug) (void)fprintf(debugDest, "No of pairs found=%d\n", NOV);

/*----Now split each of the vble/value pairs at the = signs and store them:
*/
	for(i=0; i<NOV; i++) {
		vble[i][0] = value[i][0] = NULC;
		if (debug>2) fprintf(debugDest, "parsing \"%s\"<BR>", pair[i]);
		strptr = strtok(pair[i], "=");
		if (strlen(strptr)>MAXVBLELEN) {
			(void)fprintf(stdout, "%s: %s%s%d%s%s%s", progName,
			 "Input data contained a variable=value pair whose variable name\n",
			 "   exceeded ", MAXVBLELEN,
			 " characters. Either reduce the length of ",
			 "the variable\n   name, or rebuild the program with a larger ",
			 "MAXVBLELEN setting.\n");
		}
		strncpy0(vble[i], strptr, MAXVBLELEN);

		strptr = strtok((char *)NULL, "=");
		if (strptr!=NULL) {
			strncpy0(value[i], strptr, MAXVALUELEN);
			trim(value[i]);
		} else {
			strcpy(value[i], "");
		}
		if (debug>2) (void)fprintf(debugDest,
		 "Pair %d: %s equals '%s'\n<BR>", i, vble[i], value[i]);
	}
	return(NOV);
}
