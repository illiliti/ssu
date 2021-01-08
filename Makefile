.POSIX:

BINMOD = 4754
BINOWN = root
BINGRP = wheel

PREFIX = /usr/local
BINDIR = ${PREFIX}/bin
XCFLAGS = ${CPPFLAGS} ${CFLAGS} -std=c99 -D_GNU_SOURCE \
		  -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wstrict-prototypes

all: sls

sls: sls.o
	${CC} ${XCFLAGS} -o $@ $< ${LDFLAGS}

.c.o:
	${CC} ${XCFLAGS} -c -o $@ $<

install: sls
	mkdir -p     ${DESTDIR}${BINDIR}
	cp    -f sls ${DESTDIR}${BINDIR}
	chown ${BINOWN}:${BINGRP} ${DESTDIR}${BINDIR}/sls
	chmod ${BINMOD}           ${DESTDIR}${BINDIR}/sls

uninstall:
	rm -f ${DESTDIR}${BINDIR}/sls

clean:
	rm -f sls.o sls

.PHONY: all clean install uninstall
