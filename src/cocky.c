#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// Bill Farmakis 06/07/2020
// Input: 'originLatitude=-35.167&originLongitude=149.083&destinationLatitude=-33.889&destinationLongitude=151.184'
// Formerly known as drawdistance2.c

char* substring(char*, int, int);

int main(int argc,char *argv[])
{
	double p1long, p1lat, p2long, p2lat, p1longrad, p1latrad, p2longrad, p2latrad,pi,length,difflong,degrees,tmp;

    char *token = strtok(argv[1], "&");
    char buffer[100], *pointer;
    int i = 0;

    while (token != NULL)
    {
        strcpy(buffer, token);
        // Find the index of the equals sign e.g. originLatitude=-35.167
        for(i = 0; i <= strlen(buffer); i++)
        {
            if(buffer[i] == '=')
            {
                break;
            }
        }

       if (strstr(token, "originLatitude") != NULL) {
            p1lat = atof(substring(buffer, i + 2, strlen(buffer)));
       } else if (strstr(token, "originLongitude") != NULL) {
            p1long = atof(substring(buffer, i + 2, strlen(buffer)));
        } else if (strstr(token, "destinationLatitude") != NULL) {
            p2lat = atof(substring(buffer, i + 2, strlen(buffer)));
        } else if (strstr(token, "destinationLongitude") != NULL) {
            p2long = atof(substring(buffer, i + 2, strlen(buffer)));
        }

        token = strtok(NULL, "&");
    }

	// Convert decimal degrees to radians
	pi=3.141592654;

	p1longrad=(p1long/180)*pi;
	p1latrad=(p1lat/180)*pi;
	p2longrad=(p2long/180)*pi;
	p2latrad=(p2lat/180)*pi;

	// Calculate the distance between them
	difflong=(p2longrad-p1longrad);
	tmp=acos(sin(p1latrad)*sin(p2latrad)+cos(p1latrad)*cos(p2latrad)*cos(difflong));
	degrees=(tmp/pi)*180;
	length=degrees*1.852*60;

    // Build the JSON response
    fprintf(stdout,"{");
    fprintf(stdout, "\"response\": {");
    fprintf(stdout, "\"origin_longitude\": \"%.1f km\",", p1long);
    fprintf(stdout, "\"origin_latitude\": \"%.1f km\",", p1lat);
    fprintf(stdout, "\"destination_longitude\": \"%.1f km\",", p2long);
    fprintf(stdout, "\"destination_latitude\": \"%.1f km\",", p2lat);

    if (length < 300) {
      fprintf(stdout, "\"distance\": \"%.1f km\",", length);
      fprintf(stdout, "\"accuracy_description\": \"%s\"", "This distance calculator computes the great circle distance between points. It is based on spherical trigonometry, and assumes that one minute of arc is one nautical mile, and one nautical mile is 1.852 km. Indicatively, Cocky Calculations are accurate to: a few 10s of metres over short (approx. 10 km) lines, a few 100 metres over a few 100 km, and a few kilometres over 1000s of kilometres.");
    }
    else {
      fprintf(stdout, "\"distance\": \"%.0f km\",", length);
      fprintf(stdout, "\"accuracy_description\": \"%s\"", "This distance calculator computes the great circle distance between points. It is based on spherical trigonometry, and assumes that one minute of arc is one nautical mile, and one nautical mile is 1.852 km. Indicatively, Cocky Calculations are accurate to: a few 10s of metres over short (approx. 10 km) lines, a few 100 metres over a few 100 km, and a few kilometres over 1000s of kilometres.");
    }

    fprintf(stdout,"}");
    fprintf(stdout,"}");
}
