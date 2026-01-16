UNJAG_OBJS+=		unjag.o

CRACK_OBJS+=		jag-crack.o

HEI2JM_OBJS+=		hei2jm.o utility.o

JM2TXT_OBJS+=		jm2txt.o utility.o

MAPDAT2TXT_OBJS+=	mapdat2txt.o utility.o

JM2TGA_OBJS+=		jm2tga.o utility.o

KNOWN_HASHES_OBJS+=	jag-known-hashes.o

CFLAGS+=	-std=c99
CFLAGS+=	-Wall
CFLAGS+=	-O2 -g

CPPFLAGS+=	-I../include
LDFLAGS+=	-L../libjag
LIBS+=		-ljag -lbz2

LIBTOOL?=	libtool

PROGS=		jag-crack jag-known-hashes unjag hei2jm mapdat2txt
PROGS+=		jm2txt jm2tga

all: $(PROGS)

.c.o:
	$(LIBTOOL) --quiet --mode=compile --tag=CC $(CC) $(CFLAGS) $(CPPFLAGS) -c $<

jm2txt: $(JM2TXT_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(JM2TXT_OBJS)

mapdat2txt: $(MAPDAT2TXT_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(MAPDAT2TXT_OBJS)

hei2jm: $(HEI2JM_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(HEI2JM_OBJS)

unjag: $(UNJAG_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(UNJAG_OBJS)

jag-crack: $(CRACK_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(CRACK_OBJS)

jag-known-hashes: $(KNOWN_HASHES_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(KNOWN_HASHES_OBJS)

jm2tga: $(JM2TGA_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(JM2TGA_OBJS)

clean:
	rm -f *.o *.lo
	rm -f $(PROGS)
