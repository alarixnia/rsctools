OBJS+=	jag_bzip2_decompress.lo
OBJS+=	jag_find_entry.lo
OBJS+=	jag_get_known_hash.lo
OBJS+=	jag_get_known_name.lo
OBJS+=	jag_getu16.lo
OBJS+=	jag_getu24.lo
OBJS+=	jag_getu32.lo
OBJS+=	jag_getu8.lo
OBJS+=	jag_gets8.lo
OBJS+=	jag_hash_entry_name.lo
OBJS+=	jag_map_read_dat.lo
OBJS+=	jag_map_read_hei.lo
OBJS+=	jag_map_read_jm.lo
OBJS+=	jag_map_read_loc.lo
OBJS+=	jag_map_write_jm.lo
OBJS+=	jag_putu16.lo
OBJS+=	jag_putu24.lo
OBJS+=	jag_putu32.lo
OBJS+=	jag_putu8.lo
OBJS+=	jag_read_entry.lo
OBJS+=	jag_read_header.lo
OBJS+=	jag_unpack_entry.lo
OBJS+=	jag_unpack_file.lo
OBJS+=	jag_unpack_stream.lo

LIBTOOL?=	libtool
PREFIX?=	/usr/local

CFLAGS+=	-Wall
CFLAGS+=	-g -Og

CPPFLAGS+=	-I../include

LIBS+=		-lbz2

all: libjag.la

libjag.la: $(OBJS)
	$(LIBTOOL) --quiet --mode=link --tag=CC $(CC) $(LDFLAGS) -rpath $(PREFIX)/lib -version-info 1:0 $(LIBS) $(OBJS) -o $@

.SUFFIXES: .lo

.c.lo:
	$(LIBTOOL) --quiet --mode=compile --tag=CC $(CC) $(CFLAGS) $(CPPFLAGS) -c $<

install:
	mkdir -p $(DESTDIR)$(PREFIX)/include/libjag
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	$(LIBTOOL) --quiet --tag=CC --mode=install \
		cp -p libjag.la $(DESTDIR)$(PREFIX)/lib
	cp -p ../include/*.h $(DESTDIR)$(PREFIX)/include/libjag

clean:
	rm -f *.lo *.la *.o
