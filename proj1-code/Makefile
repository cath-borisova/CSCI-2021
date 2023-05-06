CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)

all: gradebook.o gradebook_main

gradebook.o: gradebook.h gradebook.c
	$(CC) -c gradebook.c

gradebook_main: gradebook.o gradebook_main.c
	$(CC) -o $@ $^

test: gradebook_main test_gradebook.org
	@chmod u+x testy
	./testy test_gradebook.org $(testnum)

clean:
	rm -f gradebook.o gradebook_main

clean-tests:
	rm -rf test-results
	rm -f MATH1572.txt MATH1573.bin csci1901.bin csci_2021.bin csci_2021.txt \
		phys1301.txt

SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')
AN = proj1
zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi
