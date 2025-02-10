#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <gmp.h>


#define SIZE 5

#define DEFAULT_MODE 0
#define MODE_ENCRYPT 1
#define MODE_DECRYPT -1


char table[SIZE][SIZE]; //5X5 playfair cipher key square


void generateKeyTable(char* key){
    
    size_t i;
    int k=0;

    int map[26] = {0}; //Used to check used letters
    size_t keyLength = strlen(key);
    char current;


    // Initialize table

    memset(table,0,sizeof(table));


    // Fill table with the key's letters
    for(i=0; i<keyLength; i++){
        current = toupper(key[i]);
        
        //Omit J
        if(current == 'J'){
            current = 'I';
        }

        if(!map[current - 'A'] && isalpha(current)){
            table[k/SIZE][k % SIZE] = current;
            map[current - 'A'] = 1;
            k++;
        }
    }


    // Fill the rest of the spots with the remaining letters
    for(i=0; i<26; i++){

        if(i + 'A' == 'J')
            continue;
        
        if(!map[i]){
            table[k/SIZE][k % SIZE] = i + 'A';
            k++;
        }
    }


}

void printKeyTable(){

    printf("\n\nTABLE\n\n");

    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            printf("%c",table[i][j]);
        }
        printf("\n");
    }
}


void findPosition(char letter, int* row, int* col){

    if(letter == 'J')
        letter = 'I';

    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            if(table[i][j] == letter){
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

void playfairCipher(char *text, int mode) {
    size_t len = strlen(text);
    // Pad if odd length:
    if (len % 2 != 0) {
        text[len] = 'X';
        text[len + 1] = '\0';
        len++;
    }
    for (size_t i = 0; i < len; i += 2) {
        int row1, col1, row2, col2;
        // Get positions of the two characters
        findPosition(toupper(text[i]), &row1, &col1);
        findPosition(toupper(text[i+1]), &row2, &col2);

        // Same row: shift columns
        if (row1 == row2) {
            col1 = (col1 + mode + SIZE) % SIZE;
            col2 = (col2 + mode + SIZE) % SIZE;
        }
        // Same column: shift rows
        else if (col1 ==col2){
            row1 = (row1 + mode + SIZE) % SIZE;
            row2 = (row2 + mode + SIZE) % SIZE;
        }
        // Rectangle: swap columns
        else {
            int temp = col1;
            col1 = col2;
            col2 = temp;
        }
        // Replace the digraph with the cipher letters.
        text[i]   = table[row1][col1];
        text[i+1] = table[row2][col2];
    }
}

void process_file(FILE *input, FILE *output, int mode) {
    char buffer[1024];
    size_t len;
    while ((len = fread(buffer, 1, sizeof(buffer), input)) > 0) {
        buffer[len] = '\0';
        playfairCipher(buffer, mode);
        fwrite(buffer, 1, len, output);
    }
}

int main(int argc, char *argv[]) {
    char *key = NULL;
    int mode = DEFAULT_MODE;
    char *input_text = NULL;
    FILE *input = stdin;
    FILE *output = stdout;

    int opt;
    while ((opt = getopt(argc, argv, "i:o:k:ed")) != -1) {
        switch (opt) {
            case 'i':
                // If the argument is '-' then use stdin; otherwise, treat as direct input text.
                if (strcmp(optarg, "-") == 0) {
                    input = stdin;
                } else {
                    input_text = optarg;
                }
                break;
            case 'o':
                // If the argument is '-' then use stdout; otherwise, open the given file for writing.
                if (strcmp(optarg, "-") != 0) {
                    output = fopen(optarg, "w");
                    if (!output) {
                        perror("Error opening output file");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 'k':
                key = optarg;
                break;
            case 'e':
                if (mode != DEFAULT_MODE) {
                    fprintf(stderr, "Error: Specify either -e or -d, not both.\n");
                    exit(EXIT_FAILURE);
                }
                mode = MODE_ENCRYPT;
                break;
            case 'd':
                if (mode != DEFAULT_MODE) {
                    fprintf(stderr, "Error: Specify either -e or -d, not both.\n");
                    exit(EXIT_FAILURE);
                }
                mode = MODE_DECRYPT;
                break;
        }
    }

    if (!key) {
        fprintf(stderr, "Error: Key is required (-k).\n");
        exit(EXIT_FAILURE);
    }

    generateKeyTable(key);
    printKeyTable();

    // If direct text input was provided via -i, make a modifiable copy.
    if (input_text) {
        size_t input_len = strlen(input_text);
        // Allocate enough space for a possible extra padding character and the terminator.
        char *modifiable_input = malloc(input_len + 2);
        if (!modifiable_input) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        strcpy(modifiable_input, input_text);
        playfairCipher(modifiable_input, mode);
        fprintf(output, "%s\n", modifiable_input);
        free(modifiable_input);
    } else {
        // Otherwise, process from the input FILE pointer (which might be stdin)
        process_file(input, output, mode);
    }

    if (input != stdin)
        fclose(input);
    if (output != stdout)
        fclose(output);

    return 0;
}