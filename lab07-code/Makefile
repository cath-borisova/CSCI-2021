# -Og debug optimization to prevent loops from collapsing
CFLAGS = -Wall -g -Og
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
AN     = lab07
CWD    = $(shell pwd | sed 's/.*\///g')

all: superscalar_main

superscalar_main : superscalar_main.o superscalar_funcs.o
	$(CC) -o $@ $^

superscalar_main.o : superscalar_main.c
	$(CC) -c $<

superscalar_funcs.o : superscalar_funcs.c
	$(CC) -c $<

test-setup:
	@chmod u+rx testy

test: test-setup QUESTIONS.txt
	./testy test_$(AN).org 1

test-quiz: test-setup QUESTIONS.txt
	./testy test_$(AN).org 1

clean:
	rm -f superscalar_main.o superscalar_funcs.o superscalar_main

clean-tests:
	rm -rf test-results/

zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi
