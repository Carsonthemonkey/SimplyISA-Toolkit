#include <stdlib.h>
#include <stdio.h>
#include "simply.h"

#define MAX_INSTRUCTIONS 1024

unsigned char registers[4];

int decode_operator(unsigned char instruction);
int decode_immediate(unsigned char instruction);
void display_program_state(int program_counter);

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

    int pc;
    fread(program, sizeof(unsigned char), MAX_INSTRUCTIONS, file);
    for(pc = 0; pc < MAX_INSTRUCTIONS; pc++){
        printf("%i\n", program[pc]);
        int operator = decode_operator(program[pc]);
        if(program[pc] == HLT){
            break;
        }
        // BRA instruction
        else if (operator == BRA){
            //decode immediate and set PC to that location
            int immediate = decode_immediate(program[pc]);
        }

    }

    if(pc == MAX_INSTRUCTIONS){
        fprintf(stderr, "Exceeded max instructions (%i)\n", MAX_INSTRUCTIONS);
        return 1;
    }

    display_program_state(pc);
    return 0;
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
 * @brief Decodes immediate constants by masking out the 3 most significant bits
 * 
 * @param instruction instruction to decode
 * @return int 
 */
int decode_immediate(unsigned char instruction){
    // check size of car to determine shift
    long shift = sizeof(char) - 5;
    // 31 is bitmask for 00011111
    // instruction must be cast to a char so that arithmetic shift works properly
    return (((char)instruction & 31) << shift) >> shift;
}

/**
 * @brief Displays the final program state
 * 
 */
void display_program_state(int program_counter){
    printf("HLT encountered at PC = %i\n", program_counter);
    printf("PC 0 --> %i\n", program_counter);
    printf("X0: 0 --> %i\n", registers[0]);
    printf("X1: 0 --> %i\n", registers[1]);
    printf("X1: 2 --> %i\n", registers[2]);
    printf("X1: 3 --> %i\n", registers[3]);
    //TODO: add memory printouts
    //? How does it decide what memory addresses it should print?
}