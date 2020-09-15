all: sunrisenset moonrisenset sunmoonposn placenames cocky
# makefile for the Astro programs.  Version: 20020622
# CFLAGS = -D_ANSI_C_SOURCE -DUNIX -Wall -DdeBUG
CFLAGS = -D_ANSI_C_SOURCE -DUNIX -Wall
BOOLHDR=broxbool.h
ASTROHDR=astro.h

SUNOBJEX=sunrisenset.c	\
	computeriseset.c	\
	dosunjob.c	\
	initialisecomputation.c	\
	printyearsun.c	\
	semidi.c

MOONOBJEX=moonrisenset.c	\
	bessl.c	\
	calculateriseset.c	\
	domoonjob.c	\
	initialisecalculation.c	\
	makemoonephile.c	\
	printyearmoon.c	\
	readmoonephile.c

POSNOBJEX=sunmoonposn.c	\
	calcrefractionangle.c	\
	calculateposition.c	\
	cartesiancoords.c	\
	cartpolar.c	\
	dosmpjob.c	\
	geodetic.c	\
	gmst.c	\
	mooncoords.c	\
	nutation.c	\
	obliquity.c	\
	rotate.c	\
	suncoords.c

GENOBJEX=daysinfebruary.c	\
	decday2time.c	\
	dequote.c	\
	dmod.c	\
	dmy2jday.c	\
	f_exist.c	\
	fixfunnies.c	\
	getcsvfield.c	\
	getdate.c	\
	hexchar2dec.c	\
	monthname.c	\
	parsecsvline.c	\
	parsedatetime.c	\
	populatestructs.c	\
	rdian.c	\
	separatevvpairs.c	\
	setstartendtime.c	\
	strncpy0.c	\
	todms.c	\
	trim.c	\
	writefile.c

PLACENAMES=placenames.c	\
	substring.c	\

COCKY=cocky.c	\
	substring.c	\

$(SUNOBJEX): $(BOOLHDR) $(ASTROHDR) makefile
$(MOONOBJEX): $(BOOLHDR) $(ASTROHDR) makefile
$(POSNOBJEX): $(BOOLHDR) $(ASTROHDR) makefile
$(GENOBJEX): $(BOOLHDR) $(ASTROHDR) makefile
$(PLACENAMES): makefile
$(COCKY): makefile

sunrisenset: $(SUNOBJEX) $(GENOBJEX) makefile
	gcc $(CFLAGS) -o $@ $(SUNOBJEX) $(GENOBJEX) -lm
	chmod 755 sunrisenset

moonrisenset: $(MOONOBJEX) $(GENOBJEX) makefile
	gcc $(CFLAGS) -o $@ $(MOONOBJEX) $(GENOBJEX) -lm
	chmod 755 moonrisenset

sunmoonposn: $(POSNOBJEX) $(GENOBJEX) makefile
	gcc $(CFLAGS) -o $@ $(POSNOBJEX) $(GENOBJEX) -lm
	chmod 755 sunmoonposn

placenames: $(PLACENAMES) makefile
	gcc $(CFLAGS) -o $@ $(PLACENAMES) -lm
	chmod 755 placenames

cocky: $(COCKY) makefile
	gcc $(CFLAGS) -o $@ $(COCKY) -lm
	chmod 755 cocky

clean:
	@touch fred.o
	rm -f *.o sunrisenset moonrisenset sunmoonposn placenames core

tidy:
	@touch fred.o
	rm -f *.o core
