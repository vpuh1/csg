CC=gcc
CFLAGS=-I.
DEPS = csg.h config.h
OBJ = csg.o
MAN_DIR = ../doc
MAN_PAGE = csg.1
PREFIX = /usr/local
MAN_PREFIX = /usr/share/man

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

csg: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

doc:
	gzip $(MAN_DIR)/$(MAN_PAGE)

.PHONY: clean

clean:
	rm -f $(OBJ) csg
	gzip --decompress $(MAN_DIR)/$(MAN_PAGE)

.PHONY: install

install-man: 
	mkdir -p ${MAN_PREFIX}/man1
	cp $(MAN_DIR)/$(MAN_PAGE).gz $(DESTDIR)$(MAN_PREFIX)/man1/$(MAN_PAGE).gz

install: csg doc install-man
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/csg

.PHONY: uninstall

uninstall-man:
	rm -f $(DESTDIR)$(DOC_PREFIX)/man1

uninstall: uninstall-man
	rm -f $(DESTDIR)$(PREFIX)/bin/csg
