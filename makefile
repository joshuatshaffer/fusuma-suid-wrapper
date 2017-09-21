
DESTDIR=/usr/local/bin

all: fusuma-s

fusuma-s: main.c
	gcc -std=gnu11 -pedantic -Wall -Wextra main.c -o fusuma-s

clean:
	rm -rf fusuma-s

install: fusuma-s
	cp fusuma-s fusuma-s-dup
	chown root:root fusuma-s-dup \
	&& chmod u+s fusuma-s-dup \
	&& mv fusuma-s-dup $(DESTDIR)/fusuma-s \
	|| rm fusuma-s-dup # Clean up this temp file if the above fails.

uninstall:
	rm $(DESTDIR)/fusuma-s
