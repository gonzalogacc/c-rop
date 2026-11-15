.PHONY: build test

build:
	gcc -Wall -o crop main.c

test:
	gcc -Wall -o tests test_main.c main.c.c && ./tests
