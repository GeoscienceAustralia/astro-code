#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
/*
 * program:		gazmap_miki
 * compile:		gcc gazmap_miki.c -o gazmap_miki -lm
 * modifications:	29/09/2000 (JD) added decimal degrees >>> degrees minute seconds
 * modifications:	27/11/2000 (Mihajlo)
 * modifications:	06/07/2020 (Bill Farmakis) converted to return JSON
 * input: placename=CASEY&placetype=0&state=0
 */

#ifdef defined(_WIN32) || defined(WIN32)
  #define OS_Windows 0
#else
  #define OS_Windows 1
#endif

char* substring(char*, int, int);

/*
 * map program
 */
int main(int argc, char * argv[]) {
  FILE * qfie;
  FILE * infle; /* in file	*/

  int ii, j, n, m, kk;
  int matches;
  int done, going, dd, count;
  int hr;
  int lodeg, ldeg, lmin, lomin;
  long off, offf, top, bot, i, k;
  float la, lo, mn;
  char stin[10], ftin[125];
  char sin[180], ssin[180], sint[180];
  char ss[80];
  char ans[160];
  char ccode[35], state[10], code[35], lat[10], lon[10], fname[100];
  char inistring[100], pathvar[100];
  bool firstRun = true;
  char buff[PATH_MAX];
  char* cwd;
  int index;
  char location[180];

  if (OS_Windows) {
      cwd = getcwd(buff, PATH_MAX);
      for(int i = 0; i <= strlen(cwd); i++)
      {
          if(cwd[i] == '/')
          {
              index = i;
          }
      }
      cwd = substring(cwd, 0, index);
  } else {
      cwd = "/home/ec2-user/astronomical";
  }

  fprintf(stdout,"{");
  fprintf(stdout, "\"response\": {");

  strcpy(fname, cwd);
  strcat(fname, "/data/colcgi-ini.txt");
  infle = fopen(fname, "rb");
  if (!infle) {
    fprintf(stdout, "\"error\": \"Couldn't open file %s }\"", fname);
    exit(1);
  }
  fgets(inistring, 99, infle);
  fgets(inistring, 99, infle);
  sscanf(inistring, "%ld", & bot);
  fgets(inistring, 99, infle);
  fgets(inistring, 99, infle);
  fclose(infle);

  strcpy(ssin, argv[1]);

  strcpy(sint, & ssin[10]);
  count = 0;
  j = 0;
  i = 0;
  k = 0;
  dd = 0;
  while (sint[k++] != 0) {
    if (sint[i] == '%' && sint[i + 1] == '2' && sint[i + 2] == '7') {
      sin[j++] = '\'';
      i = i + 3;
      dd = 2;
    }
    sin[j++] = sint[i++];
  }
  sin[j] = 0;
  for (i = 1; i < strlen(sin); i++)
    if (sin[i] == '+') sin[i] = ' ';
  for (i = 1; i < strlen(sin); i++)
    if (sin[i] == '&') break;
  sin[i] = 0;
  strcpy(ftin, & ssin[i + 21 + dd]);
  ftin[1] = 0;
  strcpy(stin, & ssin[i + 29 + dd]);
  stin[3] = 0;

  if (strlen(sin) < 3) {
    fprintf(stdout, "\"warning\": \"Search requires a minimum of 3 characters.\"");
  } else {
    strcpy(fname, cwd);
    strcat(fname, "/data/gazind01.txt");
    infle = fopen(fname, "rb");
    if (!infle) {
      fprintf(stdout, "\"error\": \"Couldn't open file %s }\"", fname);
      exit(1);
    }
    strcpy(fname, cwd);
    strcat(fname, "/data/gaz2001.txt");
    qfie = fopen(fname, "rb");
    if (!infle) {
      fprintf(stdout, "\"error\": \"Couldn't open file %s }\"", fname);
      exit(1);
    }
    //if(qfie==NULL)          printf("bugger");
    for (j = 0; j < strlen(sin); j++) {
      sin[j] = toupper(sin[j]);
      if (sin[j] == '+')
        sin[j] = ' ';
    }

    fprintf(stdout, "\"placeNames\": [");
    matches = 0;
    i = bot;
    done = 0;
    k = bot / 2;
    top = 0;
    while (!done) {
      off = 50 * (k - 1);
      fseek(infle, off, 0);
      fgets(ans, 48, infle);
      ans[48] = 0;
      n = strncmp(sin, & ans[6], strlen(sin));
      if (n == 0) {
        while (n == 0 && off > 0) {
          off = off - 10 * 50;
          if (off < 0) off = 0;
          fseek(infle, off, 0);
          fgets(ans, 48, infle);
          ans[48] = 0;
          n = strncmp(sin, & ans[6], strlen(sin));
        }
        going = 0;
        count = 0;

        while (1) {
          fseek(infle, off, 0);
          fgets(ans, 48, infle);
          ans[48] = 0;
          n = strncmp(sin, & ans[6], strlen(sin));

          if (n == 0) {
            going = 1;
            strncpy(ss, ans, 6);
            ss[6] = 0;
            sscanf(ss, "%ld", & offf);
            offf = (offf - 1) * 233;
            if (fseek(qfie, offf, 0) != 0)
              printf("Ouch");

            fgets(ans, 232, qfie);
            ans[232] = 0;
            strncpy(code, & ans[102], 4);
            code[4] = 0;
            strncpy(ccode, & ans[230], 1);
            ccode[1] = 0;

            strncpy(state, ans, 3);
            state[3] = 0;

            if (stin[0] == '0' || strncmp("GA", state, 2) == 0 || strncmp(stin, state, 2) == 0) {
              if (ftin[0] == '0' || ftin[0] == ccode[0]) {
                switch (ccode[0]) {
                case 'A':
                  strcpy(ccode, "Airfield");
                  break;
                case 'B':
                  strcpy(ccode, "Bathymetric");
                  break;
                case 'C':
                  strcpy(ccode, "Bay or Gulf");
                  break;
                case 'D':
                  strcpy(ccode, "Bore, Tank or Waterpoint");
                  break;
                case 'E':
                  strcpy(ccode, "Building");
                  break;
                case 'F':
                  strcpy(ccode, "Cape or Peninsular");
                  break;
                case 'G':
                  strcpy(ccode, "Cave");
                  break;
                case 'H':
                  strcpy(ccode, "Coastal");
                  break;
                case 'I':
                  strcpy(ccode, "Dam or Lock");
                  break;
                case 'J':
                  strcpy(ccode, "Firing Range");
                  break;
                case 'K':
                  strcpy(ccode, "Forest or Plantation");
                  break;
                case 'L':
                  strcpy(ccode, "Hill or Mountain");
                  break;
                case 'M':
                  strcpy(ccode, "Island or Reef");
                  break;
                case 'N':
                  strcpy(ccode, "Mine or Fuel Site");
                  break;
                case 'O':
                  strcpy(ccode, "Misc Topography");
                  break;
                case 'P':
                  strcpy(ccode, "Park or Reserve");
                  break;
                case 'Q':
                  strcpy(ccode, "Port or Dock");
                  break;
                case 'R':
                  strcpy(ccode, "Town or Locality");
                  break;
                case 'S':
                  strcpy(ccode, "Track or Trail");
                  break;
                case 'T':
                  strcpy(ccode, "Trig Station");
                  break;
                case 'U':
                  strcpy(ccode, "Valley or Depression");
                  break;
                case 'V':
                  strcpy(ccode, "Water Body");
                  break;
                case 'W':
                  strcpy(ccode, "Water Course");
                  break;
                case 'X':
                  strcpy(ccode, "Lookout");
                  break;
                }
                ans[102] = 0;
                strncpy(lat, & ans[216], 9);
                lat[9] = 0;
                strncpy(lon, & ans[207], 9);
                lon[9] = 0;
                sscanf( & ans[216], "%f", & la);
                sscanf( & ans[207], "%f", & lo);
                ldeg = -la;
                lmin = -((la + ldeg) * 60) + .05;
                ldeg = la;
                lodeg = lo;
                lomin = ((lo - lodeg) * 60) + .05;

                // Round 60 minutes into degrees (latitude)
                if (lmin == 60) {
                    lmin = 0;
                    ldeg -= 1;
                }

                // Round 60 minutes into degrees (longitude)
                if (lomin == 60) {
                    lomin = 0;
                    lodeg += 1;
                }

                if (firstRun) {
                    firstRun = false;
                } else {
                    fprintf(stdout, ",");
                }

               strcpy(location, & ans[12]);
                int locationIndex = 0;
                for(int index = strlen(location) - 1; index >= 0; index--)
                {
                    if (location[index - 1] != 32) {
                             locationIndex = index;
                             break;
                    }
                }

                fprintf(stdout,"{");
                fprintf(stdout, "\"location\": \"%s\",", substring(location, 0, locationIndex));
                fprintf(stdout, "\"latitudeDegrees\": \"%d\",", ldeg);
                fprintf(stdout, "\"latitudeMinutes\": \"%d\",", lmin);
                fprintf(stdout, "\"longitudeDegrees\": \"%d\",", lodeg);
                fprintf(stdout, "\"longitudeMinutes\": \"%d\",", lomin);
                fprintf(stdout, "\"state\": \"%s\",", state);
                fprintf(stdout, "\"ccode\": \"%s\"", ccode);
                fprintf(stdout,"}");
                count++;
              }
            }
          } else if (going == 1) break;
          off = off + 50;
          if (off >= (i - 1) * 50) break;
        }
        break;
      } else if (n < 0) {
        bot = k;
        if (bot <= top + 1) {
          fprintf(stdout, "{\"warning\": {\"not_found\": \"%s\", \"closest\": \"%s\"}}", sin, ans);
          break;
        }
        k = ((top - bot) / 2) + bot;
      } else {
        top = k;
        if (bot <= top + 1) {
          fprintf(stdout, "{\"warning\": {\"not_found\": \"%s\", \"closest\": \"%s\"}}", sin, ans);
          break;
        }
        k = ((top - bot) / 2) + bot;
      }
    }
    fprintf(stdout,"]");
  }
  fprintf(stdout,"}}");
}

/* convert decimal degrees into degrees, minutes and seconds */
deg2dms(deg, hr, mn)
float deg;
int * hr;
float * mn; {
  float d;
  * mn = 60.0 * modff(deg, & d);
  if ( * mn < 0.0) * mn *= -1.0;
  * hr = d;
}
/* Local Variables: */
/* c-basic-offset:8 */
/* End: */
