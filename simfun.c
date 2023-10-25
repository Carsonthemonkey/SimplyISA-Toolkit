#include <stdlib.h>
#include <stdio.h>
#include "simply.h"

#define MAX_INSTRUCTIONS 1024

int decode_operator(unsigned char instruction);

int main(int argc, char* argv[]){

    if (argc == 1)
    {
        fprintf(stderr, "No target assembly file specified\n");
        return 1;
    }

    char *fileName = argv[1];
    
    FILE *file = fopen(fileName, "rb");
    if(file == NULL){
        fprintf(stderr, "Could not find specified input file\n");
        return 1;
    }

    unsigned char program[MAX_INSTRUCTIONS];

    fread(program, sizeof(unsigned char), MAX_INSTRUCTIONS, file);
    for(int pc = 0; pc < 256; pc++){
        printf("%i\n", program[pc]);
        int operator = decode_operator(program[pc]);
        if(program[pc] == HLT){
            printf("HLT Detected: End File.\n");
            break;
        }
    }
}

/**
 * @brief Decodes the operator from the instruction passed to it by masking out the 5 least significant bits
 * 
 * @param instruction the instruction byte to decode
 * @return int 
 */
int decode_operator(unsigned char instruction){
    // 224 is a mask for 11100000
    return instruction & 224;
}

/**
 * @brief Displays the final program state
 * 
 */
void display_program_state(){
    //TODO: Implement
}