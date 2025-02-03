#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

void print_usage(const char *progname) {
    fprintf(stderr, "Usage: %s -i <input_file> -o <output_file> -s <shift> (-e | -d)\n", progname);
    fprintf(stderr, "  -e: encrypt the file\n");
    fprintf(stderr, "  -d: decrypt the file\n");
}

/* Encrypts the input file using a Caesar cipher and writes the result to the output file. */
void encrypt_file(const char* input_file, const char* output_file, int shift) {
    FILE* input = fopen(input_file, "r");
    if (input == NULL) {
        fprintf(stderr, "Error: Unable to open input file '%s'\n", input_file);
        exit(EXIT_FAILURE);
    }

    FILE* output = fopen(output_file, "w");
    if (output == NULL) {
        fprintf(stderr, "Error: Unable to open output file '%s'\n", output_file);
        fclose(input);
        exit(EXIT_FAILURE);
    }

    /* Normalize shift*/
    shift %= 26;
    if (shift < 0) {
        shift += 26;
    }

    int ch;
    while ((ch = fgetc(input)) != EOF) {
        if (isalpha(ch)) {
            if (isupper(ch)) {
                int base = 'A';
                ch = base + ((ch - base + shift) % 26);
            } else {
                int base = 'a';
                ch = base + ((ch - base + shift) % 26);
            }
        }
        if (fputc(ch, output) == EOF) {
            fprintf(stderr, "Error: Problem writing to file '%s'\n", output_file);
            fclose(input);
            fclose(output);
            exit(EXIT_FAILURE);
        }
    }

    if (ferror(input)) {
        fprintf(stderr, "Error: Problem reading from file '%s'\n", input_file);
    }

    fclose(input);
    fclose(output);
}

void decrypt_file(const char* input_file, const char* output_file, int shift) {
    FILE* input = fopen(input_file, "r");
    if (input == NULL) {
        fprintf(stderr, "Error: Unable to open input file '%s'\n", input_file);
        exit(EXIT_FAILURE);
    }

    FILE* output = fopen(output_file, "w");
    if (output == NULL) {
        fprintf(stderr, "Error: Unable to open output file '%s'\n", output_file);
        fclose(input);
        exit(EXIT_FAILURE);
    }

    /* Normalize shift*/
    shift = shift % 26;
    if (shift < 0) {
        shift += 26;
    }

    int new_shift = (26 - shift) % 26;

    int ch;
    while ((ch = fgetc(input)) != EOF) {
        if (isalpha(ch)) {
            if (isupper(ch)) {
                int base = 'A';
                ch = base + ((ch - base + new_shift) % 26);
            } else {  // lowercase letter
                int base = 'a';
                ch = base + ((ch - base + new_shift) % 26);
            }
        }
        if (fputc(ch, output) == EOF) {
            fprintf(stderr, "Error: Problem writing to file '%s'\n", output_file);
            fclose(input);
            fclose(output);
            exit(EXIT_FAILURE);
        }
    }

    if (ferror(input)) {
        fprintf(stderr, "Error: Problem reading from file '%s'\n", input_file);
    }

    fclose(input);
    fclose(output);
}

int main(int argc, char* argv[]) {

    //Initialize input,output file names and shift value
    char* input_file = NULL;
    char* output_file = NULL;
    int shift = 0;


    int mode = 0; //0 is the default value, 1 for encryption, 2 for decryption

    int opt;
    /* Process command-line options */
    while ((opt = getopt(argc, argv, "i:o:s:ed")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 's':
                shift = atoi(optarg);
                break;
            case 'e':
                if (mode != 0) {
                    fprintf(stderr, "Error: Please specify either -e or -d, not both.\n");
                    print_usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                mode = 1;
                break;
            case 'd':
                if (mode != 0) {
                    fprintf(stderr, "Error: Please specify either -e or -d, not both.\n");
                    print_usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                mode = 2;
                break;
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    /* Validate required options */
    if (input_file == NULL || output_file == NULL || mode == 0) {
        fprintf(stderr, "Error: Missing required options.\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Call the appropriate function based on the mode */
    if (mode == 1) {
        encrypt_file(input_file, output_file, shift);
    } else if (mode == 2) {
        decrypt_file(input_file, output_file, shift);
    }

    return 0;
}
