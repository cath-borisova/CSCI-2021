AN = lab05
CC = gcc
CFLAGS = -Wall -Werror -g
all: count_hcp

count_hcp: count_hcp.c funcs.o
	$(CC) $(CFLAGS) -o $@ $^

funcs.o: funcs.h funcs.c
	$(CC) $(CFLAGS) -c funcs.c

test: test-setup all
	./testy test_$(AN).org

test-quiz: test-setup QUESTIONS.txt
	./testy test_$(AN).org 1

test-code: test-setup all
	./testy test_$(AN).org 2

test-setup:
	@chmod u+x ./testy ./test_quiz_filter

clean:
	rm -f funcs.o count_hcp

clean-tests:
	rm -rf test-results

SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')
zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi
