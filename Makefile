.POSIX:

PREFIX = /usr/local
BINDIR = ${PREFIX}/bin
XCFLAGS = ${CPPFLAGS} ${CFLAGS} -std=c99 -D_POSIX_C_SOURCE=200809L \
		  -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wstrict-prototypes

all: sls

sls:
	${CC} ${XCFLAGS} -o sls sls.c ${LDFLAGS}

install: sls
	mkdir -p  ${DESTDIR}${BINDIR}
	cp -f sls ${DESTDIR}${BINDIR}

uninstall:
	rm -f ${DESTDIR}${BINDIR}/sls

clean:
	rm -f sls

.PHONY: all clean install uninstall
