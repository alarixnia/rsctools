UNJAG_OBJS+=		unjag.o

CRACK_OBJS+=		jag-crack.o

KNOWN_HASHES_OBJS+=	jag-known-hashes.o

CFLAGS+=	-Og -g
CFLAGS+=	-Wall
CFLAGS+=	-fsanitize=undefined

CPPFLAGS+=	-I../include
LDFLAGS+=	-L../libjag
LDFLAGS+=	-fsanitize=undefined
LIBS+=		-ljag -lbz2

LIBTOOL?=	libtool

all: jag-crack jag-known-hashes unjag

.c.o:
	$(LIBTOOL) --quiet --mode=compile --tag=CC $(CC) $(CFLAGS) $(CPPFLAGS) -c $<

unjag: $(UNJAG_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(UNJAG_OBJS)

jag-crack: $(CRACK_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(CRACK_OBJS)

jag-known-hashes: $(KNOWN_HASHES_OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) $(LIBS) -o $@ $(KNOWN_HASHES_OBJS)

clean:
	rm -f *.o *.lo
	rm -f unjag jag-known-hashes
