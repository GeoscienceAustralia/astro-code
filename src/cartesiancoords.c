/*---------------------------------------------------------------------------
cartesiancoords.c, cartesianCoords()
	Convert degrees, minutes, seconds to decimal degrees and compute the
	cartesian coordinates.
History:
	16Jun02	Translated from Delphi into C. (Brock Bryce)
	13Jan92	Last modification. (KPV)
	28May86	First written and tested. (Klaus P Voigt)
---------------------------------------------------------------------------*/
double rdian(double rd, double rm, double rs);
int geodetic(double phi, double lambda, double aHeight, double *xs);


void cartesianCoords(double *phi, double *lambda, double xs[4],
	double aHeight, double latitude[3], double longitude[3])
{
	*phi = rdian(latitude[0], latitude[1], latitude[2]);
	*lambda = rdian(longitude[0], longitude[1], longitude[2]);
	geodetic(*phi, *lambda, aHeight, xs);
	return;
}
