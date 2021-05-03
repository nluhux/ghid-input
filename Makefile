COMPILER_CC ?= tcc
COMPILER_CFLAGS ?= -Os
PREFIX ?= /usr/local
DESTDIR ?=

all:
	${COMPILER_CC} ${COMPILER_CFLAGS} ghid-input-keyboard.c -o ghid-input-keyboard

install: all
	mkdir -pv ${DESTDIR}${PREFIX}/bin
	cp -f ghid-input-keyboard ${DESTDIR}${PREFIX}/bin
	chmod 0755 ${DESTDIR}${PREFIX}/bin/ghid-input-keyboard

clean:
	$(RM) -v ghid-input-keyboard

.PHONY: all clean install
