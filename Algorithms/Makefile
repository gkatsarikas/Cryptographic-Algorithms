CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

TARGETS = caesar vigenere

all: $(TARGETS)

caesar: caesar.c
	$(CC) $(CFLAGS) -o caesar caesar.c

vigenere: vigenere.c
	$(CC) $(CFLAGS) -o vigenere vigenere.c

clean:
	rm -f $(TARGETS)

.PHONY: all clean
