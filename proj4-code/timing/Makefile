CC = gcc
CFLAGS = -Wall -Werror -g

all: list_main list_funcs.o list_funcs_student.o

list_main: list_main.c list_funcs.o list_funcs_student.o
	$(CC) $(CFLAGS) -o $@ $^

list_funcs.o: linked_list.h list_funcs.c
	$(CC) $(CFLAGS) -c list_funcs.c

list_funcs_student.o: linked_list.h list_funcs_student.c
	$(CC) $(CFLAGS) -c list_funcs_student.c

test: list_main
	@echo "-----Running list_main testing program------"
	./list_main

clean:
	rm -f *.o *.gch list_main
