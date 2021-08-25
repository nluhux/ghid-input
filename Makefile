PREFIX ?= /usr
DESTDIR ?=

all:
	${CC} ${CFLAGS} ghid-input-keyboard-visible-char.c -o ghid-input-keyboard-visible-char

install:
	mkdir -pv ${DESTDIR}${PREFIX}/bin
	cp -f ghid-input-keyboard-visible-char ${DESTDIR}${PREFIX}/bin
	chmod 0755 ${DESTDIR}${PREFIX}/bin/ghid-input-keyboard-visible-char

clean:
	$(RM) -v ghid-input-keyboard-visible-char

.PHONY: all clean install
