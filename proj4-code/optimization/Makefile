CC = gcc
CFLAGS = -Wall -Werror -g

all: matrix_main matrix_funcs.o matrix_funcs_student.o

matrix_main: matrix_main.c matrix_funcs.o matrix_funcs_student.o
	$(CC) $(CFLAGS) -o $@ $^

matrix_funcs.o: matrix_funcs.c matrix.h
	$(CC) $(CFLAGS) -c matrix_funcs.c

matrix_funcs_student.o: matrix_funcs_student.c matrix.h
	$(CC) $(CFLAGS) -c matrix_funcs_student.c

test: matrix_main
	@echo "------Running speed test with nicely sized matrix------"
	@./matrix_main --even | tee evenResults.txt
	@echo "------Running speed test with oddly-sized matrix------"
	@./matrix_main --odd | tee nonEvenResults.txt
	@cat evenResults.txt nonEvenResults.txt | grep '^Ratio' > ratios.txt

clean:
	rm -f *.o *.gch matrix_main matrix_main_odd

clean-results:
	rm -f ratios.txt evenResults.txt nonEvenResults.txt
