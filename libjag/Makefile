OBJS+=	jag_get_known_hash.lo
OBJS+=	jag_get_known_name.lo
OBJS+=	jag_getu16.lo
OBJS+=	jag_getu24.lo
OBJS+=	jag_getu32.lo
OBJS+=	jag_getu8.lo
OBJS+=	jag_hash_entry_name.lo
OBJS+=	jag_map_read_dat.lo
OBJS+=	jag_map_read_hei.lo
OBJS+=	jag_map_read_jm.lo
OBJS+=	jag_map_read_loc.lo
OBJS+=	jag_read_entry.lo
OBJS+=	jag_read_header.lo
OBJS+=	jag_unpack_file.lo
OBJS+=	jag_unpack_stream.lo
OBJS+=	bzip.lo

LIBTOOL?=	libtool
PREFIX?=	/usr/local

CPPFLAGS+=	-I../include

LIBS+=		-lbz2

all: libjag.la

libjag.la: $(OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) -rpath $(PREFIX)/lib -version-info 1:0 $(LIBS) $(OBJS) -o $@

.SUFFIXES: .lo

.c.lo:
	$(LIBTOOL) --quiet --mode=compile --tag=CC $(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	rm -f *.lo *.la *.o
