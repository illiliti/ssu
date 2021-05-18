.POSIX:

BINMOD = 4754
BINOWN = root
BINGRP = wheel

PREFIX = /usr/local
BINDIR = ${PREFIX}/bin
MANDIR = ${PREFIX}/share/man
XCFLAGS = ${CPPFLAGS} ${CFLAGS} -std=c99 -D_GNU_SOURCE \
		  -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wstrict-prototypes

all: ssu

ssu: ssu.o
	${CC} ${XCFLAGS} -o $@ $< ${LDFLAGS}

.c.o:
	${CC} ${XCFLAGS} -c -o $@ $<

install: ssu
	mkdir -p     ${DESTDIR}${BINDIR}
	cp    -f ssu ${DESTDIR}${BINDIR}
	chown ${BINOWN}:${BINGRP} ${DESTDIR}${BINDIR}/ssu
	chmod ${BINMOD}           ${DESTDIR}${BINDIR}/ssu
	mkdir -p       ${DESTDIR}${MANDIR}/man1/
	cp    -f ssu.1 ${DESTDIR}${MANDIR}/man1/

uninstall:
	rm -f ${DESTDIR}${BINDIR}/ssu
	rm -f ${DESTDIR}${MANDIR}/man1/ssu.1

clean:
	rm -f ssu.o ssu

.PHONY: all clean install uninstall
