# Define the compiler and the flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

default: all

all: caesar

caesar: caesar.c
	$(CC) $(CFLAGS) -o caesar caesar.c

clean:
	rm -f caesar
