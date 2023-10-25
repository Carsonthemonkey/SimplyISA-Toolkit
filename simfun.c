#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simply.h"

#define MAX_INSTRUCTIONS 1024

int VERBOSE = 0;
unsigned char registers[4];

int decode_operator(unsigned char instruction);
int decode_immediate(unsigned char instruction);
int decode_register(unsigned char instruction, int arg_num);
void display_program_state(int program_counter);

//first arg is target
//second arg (optional) is verbose mode if input is '-v' (off by default) 
int main(int argc, char* argv[]){

    if (argc == 1)
    {
        fprintf(stderr, "No target assembly file specified\n");
        return 1;
    }

    char *fileName = argv[1];
    
    if(argc > 2){
        if(!strcmp(argv[2], "-v")) VERBOSE = 1;
    }

    FILE *file = fopen(fileName, "rb");
    if(file == NULL){
        fprintf(stderr, "Could not find specified input file\n");
        return 1;
    }

    unsigned char program[MAX_INSTRUCTIONS];

    int pc;
    fread(program, sizeof(unsigned char), MAX_INSTRUCTIONS, file);
    for(pc = 0; pc < MAX_INSTRUCTIONS; pc++){
        // printf("%i\n", program[pc]);
        int instruction = program[pc];
        int operator = decode_operator(instruction);
        if(instruction == HLT){
            break;
        }
        // BRA instruction
        else if (operator == BRA){
            //decode immediate and set PC to that location
            int immediate = decode_immediate(instruction);
            // offset pc by immediate
            if(VERBOSE) printf("info BRA: PC: %i --> %i\n", pc, pc + immediate + 1);
            pc += immediate;
        }
        else if (operator == BZ){
            // offset pc by immediate if X1 is 0
            if(registers[X1] == 0){
                int immediate = decode_immediate(instruction);
                pc += immediate;
            }
        }
        else if (operator == LD){
            int R = decode_register(instruction, 0);
            int S = decode_register(instruction, 1);
            registers[R] = program[registers[S]];
        }
        else if (operator == LDI){
            //decode immediate and store in X0
            int immediate = decode_immediate(instruction);
            if(VERBOSE) printf("info LDI: loading %i into X0\n", immediate);
            registers[X0] = (unsigned char)immediate;
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
 * @brief Decodes register arguments in an instruction
 * 
 * @param instruction the instruction from which to decode the register
 * @param arg_num the position of the argument to decode 0 decodes argument R (first arg) and 1 decodes argument S (second arg)
 * @return int 
 */
int decode_register(unsigned char instruction, int arg_num){
    if(!(arg_num == 0 || arg_num == 1)){
        fprintf(stderr, "arg_num out of ranged. Expected 0 or 1, got %i\n", arg_num);
        exit(1);
    }
    // shift right 2 if arg is 0, and shift 0 if arg is 1
    int rshift = !arg_num * 2;
    //7 is bitmask for 00000111
    return (instruction >> rshift) & 7;
}

/**
 * @brief Displays the final program state
 * 
 */
void display_program_state(int program_counter){
    printf("HLT encountered at PC = %i\n", program_counter);
    printf("PC 0 --> %i\n", program_counter);
    printf("X0: 0 --> %i\n", registers[X0]);
    printf("X1: 0 --> %i\n", registers[X1]);
    printf("X1: 2 --> %i\n", registers[X2]);
    printf("X1: 3 --> %i\n", registers[X3]);
    //TODO: add memory printouts
    //? How does it decide what memory addresses it should print?
}