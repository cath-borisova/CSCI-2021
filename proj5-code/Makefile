CFLAGS = -Wall -Werror -g -Og -Wno-unused-result  # remove warning for system() calls
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')

PROGRAMS = \
	el_malloc.o \
	el_demo

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) *.o

help:
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make test                     # run all tests'
	@echo '  > make test testnum=5          # run problem 1 test #5 only'


############################################################
# 'make zip' to create p2-code.zip for submission
AN=p5
zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi


# EL MALLOC
el_malloc.o: el_malloc.c el_malloc.h
	$(CC) -c $<

el_demo: el_demo.c el_malloc.o
	$(CC) -o $@ $^

test_el_malloc: test_el_malloc.c el_malloc.o
	$(CC) -o $@ $^

# TESTING TARGETS
test: test-setup test_el_malloc
	./testy test_el_malloc.org $(testnum)

test-setup:
	@chmod u+rx testy

clean-tests:
	rm -rf test-results test_el_malloc
