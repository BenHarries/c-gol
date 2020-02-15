# the compiler: gcc for C program, define as g++ for C++
  CC = gcc
  RM = rm
  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  CFLAGS  = -Wall -Wextra -pedantic -std=c11

  # the build target executable:
  TARGET = gameoflife

  all: $(TARGET)

  $(TARGET):	$(TARGET).c
	gcc $(CFLAGS) gameoflife.c gol.c -o gameoflife     

  clean:
	rm gameoflife