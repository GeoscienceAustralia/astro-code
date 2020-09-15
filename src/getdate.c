#if 0
						/* TEST HARNESS FOR getDate()	*/
#include <stdio.h>
void getDate(char *buffer);

int main()
{
   char dateString[11];
   getDate(dateString);
   (void)printf("Today is %s\n",dateString);
	return(0);
}
#endif

/*---------------------------------------------------------------------------
getdate.c getDate()
	Returns the current date in the form DD/MM/YYYY. The buffer must be
	at least 11 bytes long.
Returns:
	Nil.
History:
	06Jun02	Written from a copy of timeStamp() by the same author. (Brock)
---------------------------------------------------------------------------*/
#include <time.h>
#include <stdio.h>
#include <string.h>

void getDate(char *buffer)
{
   int	i, imnth;	/* position in string mnths; month as an integer. */
	time_t	time_secs;	/* time in secs since 00:00:00 Jan 1st, 1970. */
   char time_str[26];	/* time expressed as Thu Sep 25 15:49:12 1989\n\0. */
   char mnths[37];
   (void)strcpy(mnths,"JanFebMarAprMayJunJulAugSepOctNovDec");

   (void)time(&time_secs);
   (void)strncpy(time_str, ctime(&time_secs), 25);
	time_str[25] = '\0';

   buffer[0] = time_str[8]!=' ' ? time_str[8] : '0';
	buffer[1] = time_str[9];
	buffer[2] = '/';
   for(i=0; i<34; i+=3) {	/* Find the month in the list of months. */
      if (strncmp(time_str+4, mnths+i, 3)==0) {
         imnth = (i+3)/3;
         break;
      }
   }
   (void)sprintf(buffer+3,"%02d",imnth);	/* Equivalent to an itoa function. */
	buffer[5] = '/';
   buffer[6] = time_str[20];
   buffer[7] = time_str[21];
   buffer[8] = time_str[22];
   buffer[9] = time_str[23];
   buffer[10] = '\0';

	return;
}
