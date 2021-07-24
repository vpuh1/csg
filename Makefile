CC=gcc
CFLAGS=-I.
DEPS = csg.h config.h
OBJ = csg.o config.o
MAN_DIR = ./doc/
MAN_PAGE = csg.1
PREFIX = /usr/local

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

csg: $(OBJ)
	$(CC) -o csg $(OBJ) $(CFLAGS)
	#gzip <$(MAN_DIR)$(MAN_PAGE) >$(MAN_DIR)$(MAN_PAGE).gz
	mkdir -p $(HOME)/.csg
	cp config/ucsgrc $(HOME)/.csg/csgrc
	cp -r html $(HOME)/.csg/
	cp -r css $(HOME)/.csg/

.PHONY: clean

clean:
	rm -f $(OBJ) csg
	if [ -d "./test" ]; then rm -r test; fi


.PHONY: install

install-man: 
	mkdir -p ${PREFIX}/man/man1
	cp $(MAN_DIR)$(MAN_PAGE) $(DESTDIR)$(PREFIX)/man/man1/$(MAN_PAGE)

install: csg install-man
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/csg
	mkdir -p /etc/csg
	cp config/scsgrc /etc/csg/csgrc
	cp -r html /etc/csg/
	cp -r css /etc/csg/

test: csg
	mkdir -p test
	./test.sh
	./csg test test

.PHONY: uninstall

uninstall-man:
	rm -f $(DESTDIR)$(PREFIX)/man/man1/${MAN_PAGE}

uninstall: uninstall-man
	rm -f $(DESTDIR)$(PREFIX)/bin/csg
	rm -r /etc/csg
	rm -f ~/.csg
