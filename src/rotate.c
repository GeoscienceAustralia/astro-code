/*---------------------------------------------------------------------------
rotate.c, rotate()
	This routine rotates coordinate system in which vector 'vectorX' is
	described through angle 'angle' (radians) about axis 'iaxis'.
Returns:
	nil
History:
	17Jun02	Translated from Delphi into C.  Previously was called PROCEDURE
				ROT.  Some of the variable names have been changed to improve
				readability.  (Brock Bryce)
	Earlier	history unknown, but see also sunmoonposn.c's prologue.
---------------------------------------------------------------------------*/
#include <math.h>

void rotate(int iaxis, double angle, double vectorX[4])
{
	double	c;
	double	r[4][4];
	double	s;
	double	vectorY[4];
	int		i;
	int		j;

	for(i=1; i<=3; i++) {
		vectorY[i] = vectorX[i];
		vectorX[i] = 0.0;
	}

	s = sin(angle);
	c = cos(angle);

/*----Set up rotation matrix:
*/

	for(i=1; i<=3; i++) {
		for(j=1; j<=3; j++) r[i][j] = 0.0;
		r[i][i] = c;
	}

	r[iaxis][iaxis] = 1.0;

	i = (iaxis%3) + 1;
	j = ((iaxis+1)%3) + 1;
	r[i][j] = s;
	r[j][i] = -s;

/*----now do the matrix-by-vector multiplication:
*/

	for(i=1; i<=3; i++) {
		for(j=1; j<=3; j++) vectorX[i] = vectorX[i] + r[i][j]*vectorY[j];
	}

}
