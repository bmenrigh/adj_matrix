CC=gcc

CFLAGS=-std=c99 -Wall -Wextra -march=native -O3 -flto

# various debug / profiling options
#CFLAGS=-Wall -Wextray -O2 -g
#CFLAGS=-Wall -Wextra -march=native -O2 -pg
#CFLAGS=-Wall -Wextra -march=native -O0 -g

main: adjmatrix

adjmatrix: adjmatrix.o
	$(CC) $(CFLAGS) -o adjmatrix adjmatrix.o

adjmatrix.o: adjmatrix.c
	$(CC) $(CFLAGS) -c adjmatrix.c

clean:
	rm -f adjmatrix
	rm -f *.o
	rm -f *~
	rm -f \#*
