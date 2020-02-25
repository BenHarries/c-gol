# the compiler: gcc for C program, define as g++ for C++
  CC = gcc
  RM = rm
  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  # the build target executable:

  all: gol.o libgol.so gameoflife
  gol.o: gol.c
	gcc -g -fPIC -Wall -Wextra -pedantic -c -std=c11 gol.c
  libgol.so:  gol.o
	gcc gol.o -shared -o libgol.so
  gameoflife: gameoflife.c
	gcc -g -fPIC -Wall -Wextra -pedantic -std=c11 -L . gameoflife.c -lgol -o gameoflife

  clean:
	rm gol.o libgol.so gameoflife