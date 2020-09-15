/*---------------------------------------------------------------------------
daysinfebruary.c daysInFebruary()
Purpose:
	Given a year, return the number of days in February.
History:
	09Jun02	Rewritten in C. (Brock Bryce)
	16Dec91	Last modified. Code found at WINASTSB.PAS 1222-1244. (KPV)
	18Aug89	First written and tested. (Klaus P Voigt)
---------------------------------------------------------------------------*/
int daysInFebruary(int iyear)
{
	if ((iyear%100==0) && (iyear%400!=0)) {
		return(28);
	} else {
		if (iyear%4==0) {
			return(29);
		} else {
			return(28);
		}
	}
}
