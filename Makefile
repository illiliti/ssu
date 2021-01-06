.POSIX:

PREFIX = /usr/local
BINDIR = ${PREFIX}/bin
XCFLAGS = ${CPPFLAGS} ${CFLAGS} -std=c99 -D_GNU_SOURCE \
		  -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wstrict-prototypes

all: sls

sls: sls.o
	${CC} -o $@ $< ${LDFLAGS}

config.h:
	cp config.def.h config.h

sls.o: sls.c config.h
	${CC} ${XCFLAGS} -c -o $@ sls.c

install: sls
	mkdir -p  ${DESTDIR}${BINDIR}
	cp -f sls ${DESTDIR}${BINDIR}
	chmod 4755 ${DESTDIR}${BINDIR}/sls

uninstall:
	rm -f ${DESTDIR}${BINDIR}/sls

clean:
	rm -f sls.o sls

.PHONY: all clean install uninstall
