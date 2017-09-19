
all: fusuma-s

fusuma-s: main.c
	gcc main.c -o fusuma-s

rootup: fusuma-s
	sudo chown root:root fusuma-s
	sudo chmod u+s fusuma-s