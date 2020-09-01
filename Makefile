.POSIX:

PREFIX = /usr/local
BINDIR = ${PREFIX}/bin
XCFLAGS = ${CPPFLAGS} ${CFLAGS} -std=c99 -D_GNU_SOURCE \
		  -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wstrict-prototypes

all: sls

sls: sls.o
	${CC} ${XCFLAGS} -o $@ $@.o ${LDFLAGS}

.c.o:
	${CC} ${XCFLAGS} -c -o $@ $<

install: sls
	mkdir -p  ${DESTDIR}${BINDIR}
	cp -f sls ${DESTDIR}${BINDIR}

uninstall:
	rm -f ${DESTDIR}${BINDIR}/sls

clean:
	rm -f -- *.o sls

.PHONY: all clean install uninstall
