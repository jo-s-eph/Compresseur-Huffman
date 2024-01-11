CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -fsanitize=address -g
CFILES= main
FILENAME= Projet_MAOUCHE

all: $(CFILES)

main: $(CFILES).c
	$(CC) $(CFLAGS) -o main.o main.c 


zip:
	mkdir $(FILENAME)
	cp *.c *.h Makefile readme.txt small.txt big.txt result.txt $(FILENAME)
	tar -zcvf $(FILENAME).tar.gz $(FILENAME)
	rm -rf $(FILENAME)

clean:
	rm -f *.o $(FILENAME).tar.gz