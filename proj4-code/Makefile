.PHONY: all clean optimization timing zip

all: optimization timing

test: test-optimization test-timing

optimization:
	$(MAKE) -C optimization

timing:
	$(MAKE) -C timing

test-optimization:
	$(MAKE) -C optimization test

test-timing:
	$(MAKE) -C timing test

AN = proj4
SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')
HOST = $(shell echo $$HOSTNAME | grep 'kh1262-[0-9][0-9]')
CPU = $(shell lscpu | grep 'i7-7700')

zip: clean
	@if [[ "$(HOST)" == "" ]]; then echo "ERROR: It appears that you are not running this on a kh1262 machine. Please use a kh1262 machine to submit your code."; exit 2; fi
	@if [[ "$(CPU)" == "" ]]; then echo "ERROR: It appears that you are not running this on a machine with an i7-7700 CPU. Plese use a machine with the proper CPU to submit your code".; exit 2; fi
	$(MAKE) -C optimization test
	rm -f $(AN)-code.zip
	zip -r $(AN)-code.zip *
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

clean:
	$(MAKE) -C optimization clean
	$(MAKE) -C timing clean

clean-results:
	$(MAKE) -C optimization clean-results
