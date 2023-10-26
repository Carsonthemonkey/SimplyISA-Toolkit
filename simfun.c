#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simply.h"

#define MAX_INSTRUCTIONS 255

int VERBOSE = 0;

//registers are signed, as they can hold negative values
char registers[4];
unsigned char program[MAX_INSTRUCTIONS];


unsigned char decode_operator(unsigned char instruction);
unsigned char decode_register(unsigned char instruction, int arg_num);
char decode_immediate(unsigned char instruction);
char get_value_at_pointer(unsigned char reg);
void save_value_at_ptr(unsigned char reg, char value);
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

    int pc;
    fread(program, sizeof(unsigned char), MAX_INSTRUCTIONS, file);
    for(pc = 0; pc < MAX_INSTRUCTIONS; pc++){
        // printf("%i\n", program[pc]);
        int instruction = program[pc];
        int operator = decode_operator(instruction);
        if(operator == HLT){
            if(VERBOSE) printf("info HLT: Halting\n");
            break;
        }
        // BRA instruction
        else if (operator == BRA){
            //decode immediate and set PC to that location
            int immediate = decode_immediate(instruction);
            if(VERBOSE) printf("info BRA: PC: %i --> %i\n", pc, pc + immediate + 1);
            // offset pc by immediate
            pc += immediate;
        }
        else if (operator == BZ){
            // offset pc by immediate if X1 is 0
            if(registers[X1] == 0){
                int immediate = decode_immediate(instruction);
                if(VERBOSE) printf("info BZ: PC: %i --> %i\n", pc, pc + immediate + 1);
                pc += immediate;
            }
            else if (VERBOSE){
                printf("info BZ: X1 is %i, continuing to next PC\n", registers[X1]);
            }
        }
        else if (operator == LD){
            int R = decode_register(instruction, 0);
            int S = decode_register(instruction, 1);
            if(VERBOSE) printf("info LD: loading value at mem[%i] (%i) into register X%i\n", registers[S], get_value_at_pointer(S), R);
            // registers[R] = program[registers[S]];
            registers[R] = get_value_at_pointer(S);
        }
        else if (operator == LDI){
            //decode immediate and store in X0
            int immediate = decode_immediate(instruction);
            if(VERBOSE) printf("info LDI: loading %i into X0\n", immediate);
            registers[X0] = (unsigned char)immediate;
        }
        else if (operator == ST){
            int R = decode_register(instruction, 0);
            int S = decode_register(instruction, 1);
            save_value_at_ptr(S, registers[R]);
            if(VERBOSE) printf("info ST: Saving value in X%i (%i) to mem[%i]\n", R, registers[R], registers[S]);
        }
        else if (operator == ADD){
            int R = decode_register(instruction, 0);
            int S = decode_register(instruction, 1);
            if(VERBOSE) printf("info ADD: loading %i + %i into register X%i\n", registers[R], registers[S], R);
            registers[R] += registers[S];
        }
        else if (operator == NEG){
            // 1 because neg stores it in rightmost position
            int R = decode_register(instruction, 1);
            if(VERBOSE) printf("info NEG: negating value in X%i (%i)\n", R, registers[R]);
            registers[R] *= -1;
        }
    }


    if(pc == MAX_INSTRUCTIONS){
        fprintf(stderr, "Exceeded max instructions (%i)\n", MAX_INSTRUCTIONS);
        return 1;
    }
    
    if(VERBOSE) printf("\n");

    display_program_state(pc);
    return 0;
}

/**
 * @brief Decodes the operator from the instruction passed to it by masking out the 5 least significant bits
 * 
 * @param instruction the instruction byte to decode
 * @return int 
 */
unsigned char decode_operator(unsigned char instruction){
    // 224 is a mask for 11100000
    return instruction & 224;
}

/**
 * @brief Decodes immediate constants by masking out the 3 most significant bits
 * 
 * @param instruction instruction to decode
 * @return int 
 */
char decode_immediate(unsigned char instruction){
    // check size of char to determine shift
    long shift = sizeof(char) - 5;
    // 31 is bitmask for 00011111
    // instruction must be cast to a signed char so that arithmetic shift works properly
    return (((char)instruction & 31) << shift) >> shift;
}

/**
 * @brief Decodes register arguments in an instruction
 * 
 * @param instruction the instruction from which to decode the register
 * @param arg_num the position of the argument to decode 0 decodes argument R (first arg) and 1 decodes argument S (second arg)
 * @return int 
 */
unsigned char decode_register(unsigned char instruction, int arg_num){
    if(!(arg_num == 0 || arg_num == 1)){
        fprintf(stderr, "arg_num out of ranged. Expected 0 or 1, got %i\n", arg_num);
        exit(1);
    }
    // shift right 0 if arg is 0, and shift 2 if arg is 1
    int rshift = arg_num * 2;   
    // 3 is bitmask for 00000011
    return (instruction >> rshift) & 3;
}

/**
 * @brief Retrieve the value in memory pointed to by the value in register reg
 * 
 * @param reg The register index of the memory value
 * @return char The value in memory that register reg points to
 */
char get_value_at_pointer(unsigned char reg){
    // get the value in the register reg and cast it to an unsigned int so it can be used as an array subscript
    unsigned char ptr = (unsigned char)registers[reg];
    // use the ptr to get the value in the program memory. Then cast that as a signed char so that it can be interpreted as negative
    return (char)program[ptr];
}

/**
 * @brief Stores value at the program memory address pointed to by the value in register reg 
 * 
 * @param reg The register index of the memory pointer
 * @param value The value to be saved to memory
 */
void save_value_at_ptr(unsigned char reg, char value){
    // get the value in the register reg and cast it to an unsigned int so it can be used as an array subscript
    unsigned char ptr = (unsigned char)registers[reg];
    program[ptr] = value;
}

/**
 * @brief Displays the final program state
 * 
 */
void display_program_state(int program_counter){
    printf("---------- Program State ----------\n");
    printf("HLT encountered at PC = %i\n", program_counter);
    printf("PC 0 --> %i\n", program_counter);
    printf("X0: 0 --> %i\n", registers[X0]);
    printf("X1: 0 --> %i\n", registers[X1]);
    printf("X2: 0 --> %i\n", registers[X2]);
    printf("X3: 0 --> %i\n", registers[X3]);
    printf("mem[2] 0 --> %i\n", program[2]);
}