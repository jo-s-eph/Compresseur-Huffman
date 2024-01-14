CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -fsanitize=address -g
CFILES= main
FILENAME= Huffman_Joseph

all: $(CFILES)

main: $(CFILES).c
	$(CC) $(CFLAGS) -o main.o main.c 


zip:
	mkdir $(FILENAME)
	cp *.c *.h *.txt Makefile $(FILENAME)
	tar -zcvf $(FILENAME).tar.gz $(FILENAME)
	rm -rf $(FILENAME)

clean:
	rm -f *.o $(FILENAME).tar.gz