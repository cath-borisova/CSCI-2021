CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)

puzzlebox: puzzlebox.c
	$(CC) -o $@ $^

test: puzzlebox input.txt
	./puzzlebox input.txt

clean:
	rm -f puzzlebox
