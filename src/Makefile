CC = gcc
CFLAGS = -I.
DEPS = csg.h config.h limits.h convert.h util.h
OBJ = csg.o config.o convert.o util.o
MAN_DIR = ../doc/
MAN_PAGE = csg.1
PREFIX = /usr/local
MANPREFIX = /usr/local/share/man

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

csg: $(OBJ)
	$(CC) -o csg $(OBJ) $(CFLAGS)
	mkdir -p $(HOME)/.csg
	cp ../config/ucsgrc $(HOME)/.csg/csgrc
	cp -r ../html $(HOME)/.csg/html
	cp -r ../css $(HOME)/.csg/css

.PHONY: clean

clean:
	rm -f $(OBJ) csg
	if [ -f "../test/index.html" ]; then rm ../test/*.html; fi


.PHONY: install

install: csg
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/csg
	chmod 755 ${DESTDIR}${PREFIX}/bin/csg
	mkdir -p ${PREFIX}/man/man1
	cp $(MAN_DIR)$(MAN_PAGE) $(DESTDIR)$(MANPREFIX)/man1/csg.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/csg.1
	mkdir -p /etc/csg
	cp ../config/scsgrc /etc/csg/csgrc
	cp -r ../html /etc/csg/
	cp -r ../css /etc/csg/

test: csg
	./csg ../test ../test

.PHONY: uninstall

uninstall-man:

uninstall: uninstall-man
	rm -f $(DESTDIR)$(PREFIX)/bin/csg \
		$(DESTDIR)$(MANPREFIX)/man1/csg.1
	rm -r /etc/csg
	rm -f ~/.csg
