CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

TARGETS = caesar vigenere playfair

all: $(TARGETS)

caesar: caesar.c
	$(CC) $(CFLAGS) -o caesar caesar.c

vigenere: vigenere.c
	$(CC) $(CFLAGS) -o vigenere vigenere.c

playfair: playfair.c
	$(CC) $(CFLAGS) -o playfair playfair.c

clean:
	rm -f $(TARGETS)

.PHONY: all clean
