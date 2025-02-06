#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>


//Helper function used to perform shift on letters
char vigenere_shift(char c, int shift) {
    if (isupper(c)) {
        return 'A' + (c - 'A' + shift + 26) % 26;
    } else if (islower(c)) {
        return 'a' + (c - 'a' + shift + 26) % 26;
    }
    return c;  
}

void file_process(const char* input_file,const char* output_file,const char* key, int mode){

    FILE* input = fopen(input_file, "r");

    if(input == NULL){
        fprintf(stderr,"Error, could not open input file.\n");
        exit(EXIT_FAILURE);
    }

    FILE* output = fopen(output_file, "w");

    if(output == NULL){
        fprintf(stderr,"Error, could not open output file.\n");
        exit(EXIT_FAILURE);
    }

    //Get key length and ensure the key consists of alphabetic characters only
    size_t key_length = strlen(key);

    if(key_length == 0){
        fprintf(stderr, "Key length must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    for(size_t i=0; i<key_length; i++){
        if(!isalpha(key[i])){
            printf("Key should consist of alphabetic characters only.\n");
            exit(EXIT_FAILURE);
        }
    }
   
    int ch;
    size_t index = 0; //key index

    //Process input file character by character
    while ((ch = fgetc(input)) != EOF) {
        if (isalpha(ch)) {
            // Calculate the shift value from the key letter and multiply by mode (1 for encryption, -1 for decryption)
            int key_shift = (tolower(key[index % key_length]) - 'a') * mode;
            char shifted = vigenere_shift((char)ch, key_shift);
            fputc(shifted, output);
            index++;
        } else {
            // Ignore all other characters
            fputc(ch, output);
        }
    }


    fclose(input);
    fclose(output);

}




int main(int argc, char* argv[]){

    char* input_file = NULL;
    char* output_file = NULL;
    char* key = NULL;

    int opt;
    int mode = 0; 

    while((opt = getopt(argc,argv,"i:o:k:edh")) != -1){
        switch(opt){

            case 'i':
                input_file = optarg;
                printf("Input file: %s\n",input_file);
                break;
            
            case 'o':
                output_file = optarg;
                printf("Output file: %s\n",output_file);
                break;
            
            case 'k':
                key = optarg;
                printf("Key: %s\n",key);
                break;
            
            case 'e':
                if(mode != 0){
                    fprintf(stderr, "Error: Please specify either -e or -d, not both.\n");
                    exit(EXIT_FAILURE);
                }
                mode = 1;
                break;
                
            case 'd':
                if(mode != 0){
                    fprintf(stderr, "Error: Please specify either -e or -d, not both.\n");
                    exit(EXIT_FAILURE);
                }
                mode = -1;
                break;
            
            case 'h':
                fprintf(stderr, "Usage: %s -i <input_file> -o <output_file> -k <key> (-e | -d)\n", argv[0]);
                fprintf(stderr, "  -e: encrypt the file\n");
                fprintf(stderr, "  -d: decrypt the file\n");
                exit(EXIT_SUCCESS);
                break;

            default:
                break;
        }
    }

    printf("Mode: %d\n",mode);

    if(input_file == NULL || output_file == NULL || key == NULL || mode == 0){
        fprintf(stderr, "Error, type ./vigenere -h for help\n");
        exit(EXIT_FAILURE);
    }

    file_process(input_file,output_file,key,mode); 

    return 0;
}