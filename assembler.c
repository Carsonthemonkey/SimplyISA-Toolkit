#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "simply.h"

#define MAX_LABELS 100
#define TOKEN_DELIMITER " ,"


// arguments defined as:
// arg 1: target assembly file
// arg 2 (optional): outfile name

char outfile[100];

struct label
{
    int index;
    char *name;
};

struct label labels[MAX_LABELS];
int label_count = 0;
int line_number = 0;

int get_label_index(char *label_name);
int encode_operator(char *operator_name);
int encode_register(char *register_name);
int encode_constant_string(char *constant_string);
int str_to_int(const char *str, int *result);
int to_five_bit_twos_complement(int n);
void print_binary(unsigned int n);
int is_whitespace(char *string);
void trunc_token(char *token);
const int DEBUG = 0;

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "No target assembly file specified\n");
        return 1;
    }

    // name outfile
    if (argc == 2)
    {
        strcpy(outfile, "a.bin");
    }
    else
    {
        strcpy(outfile, argv[2]);
    }
    // open file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not find specified input file\n");
        return 1;
    }
    if (DEBUG)
        printf("Opened file\n");

    // read file
    char line[256];
    char *token;
    int pc = 0;

    // first pass to find and record labels
    while (fgets(line, 256, file))
    {
        token = strtok(line, " ");
        if (token == NULL)
            continue;
        ;
        if (token[strlen(token) - 1] == ':')
        {
            struct label current_label = {pc, strdup(token)}; // strdup or else token pointer is updated
            labels[label_count] = current_label;
            label_count++;
        }
        // if line is comment or empty line, do not include it in the program counter
        if (token[0] == '#' || token[0] == '\r' || token[0] == '\n')
            continue;
        pc++;
    }

    // second pass to write binary
    rewind(file);
    unsigned char bytes[500];
    pc = 0;

    while (fgets(line, 256, file))
    {
        line_number++;
        token = strtok(line, TOKEN_DELIMITER);
        trunc_token(token);
        if (token[0] == '#')
            continue; // go to next line if line is a comment
        // reading each token in the line
        int instruction = 0;
        for (; token != NULL && token[0] != '#'; token = strtok(NULL, TOKEN_DELIMITER))
        {
            trunc_token(token);
            if (is_whitespace(token))
                continue;
            if (token[strlen(token) - 1] == ':')
                continue; // ignore labels

            // encode operator
            instruction = encode_operator(token);
            if (instruction == -1)
            {
                // token is not an operator
                if (!str_to_int(token, &instruction))
                {
                    fprintf(stderr, "Line %i: Token \"%s\" is not a valid operator or constant\n", line_number, token);
                    return 1;
                }
            }
            // Everything except HLT expects arguments
            else if (instruction != HLT)
            {
                char *arg;
                // instruction is an operator
                arg = strtok(NULL, TOKEN_DELIMITER); // fetch next token
                trunc_token(arg);
                if (arg == NULL || is_whitespace(arg))
                {
                    fprintf(stderr, "Line %i: Operator %s Expects an argument\n",line_number, token);
                    return 1;
                }
                // if instruction takes 5 bit immediate
                if (instruction == BRA || instruction == BZ || instruction == LDI)
                {
                    int label_pc = get_label_index(arg);
                    int immediate;
                    if (label_pc == -1)
                    {
                        if (!str_to_int(arg, &immediate))
                        {
                            fprintf(stderr, "Line %i: Token \"%s\" is not a valid constant or label\n", line_number, arg);
                            return 1;
                        }
                    }
                    else
                    {
                        immediate = label_pc - (pc + 1);
                    }
                    if(immediate < -31 || immediate > 31){
                        fprintf(stderr, "Line %i: argument \"%s\"\n exceeds 5 bit immediate range (-31 to 31)\n", line_number, arg);
                    }
                    if (immediate < 1)
                        immediate = to_five_bit_twos_complement(immediate);
                    instruction += immediate;
                }
                else if (instruction == ADD || instruction == LD || instruction == ST)
                {
                    int R = encode_register(arg);
                    if (R == -1)
                    {
                        fprintf(stderr, "\"%s\" is not a valid register\n", arg);
                        return 1;
                    }
                    arg = strtok(NULL, TOKEN_DELIMITER);
                    trunc_token(arg);
                    if (arg == NULL || is_whitespace(arg))
                    {
                        fprintf(stderr, "Line %i: Operator %s Expects two arguments\n", line_number, token);
                        return 1;
                    }
                    int S;
                    S = encode_register(arg);
                    if (S == -1)
                    {
                        fprintf(stderr, "Line %i: \"%s\" is not a valid register\n", line_number, arg);
                        return 1;
                    }
                    instruction += R;
                    // Shift S into the correct position
                    instruction += S << 2;
                }
            }

            bytes[pc] = (unsigned char)instruction;
            // printf("instruction: "); print_binary(instruction);
            pc++;
            break;
        }
    }

    // create out file
    FILE *out = fopen(outfile, "wb");
    if (out == NULL)
    {
        fprintf(stderr, "Line %i: Failed to create outfile\n", line_number);
        return 1;
    }

    // write the binary to the outfile
    fwrite(bytes, sizeof(unsigned char), pc, out);
    fclose(file);
    fclose(out);
    return 0;
}

int get_label_index(char *label_name)
{
    for (int i = 0; i < label_count; i++)
    {
        if (!strncmp(labels[i].name, label_name, strlen(label_name)))
            return labels[i].index;
    }
    return -1;
}

int to_five_bit_twos_complement(int n)
{
    // 31 is bitmask for the 5 least significant bits
    return n & 31;
}

int encode_register(char *register_name)
{
    if (!strcmp(register_name, "X0"))
        return X0;
    else if (!strcmp(register_name, "X1"))
        return X1;
    else if (!strcmp(register_name, "X2"))
        return X2;
    else if (!strcmp(register_name, "X3"))
        return X3;
    else
        return -1;
}

int encode_constant_string(char *constant_string)
{
    if (!strcmp(constant_string, "0"))
        return 0;
    int n = atoi(constant_string);
    if (n == 0)
        return -1;
    return n;
}

// ChatGPT wrote this one
int str_to_int(const char *str, int *result)
{
    int sign = 1;
    int i = 0;
    int num = 0;

    // Check for NULL string or empty string
    if (str == NULL || *str == '\0')
    {
        return 0;
    }

    // Handle optional sign
    if (str[0] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[0] == '+')
    {
        i++;
    }

    // Convert string to integer
    for (; i < strlen(str); ++i)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            if (num > 214748364 || (num == 214748364 && str[i] > '7'))
            {
                return 0;
            }
            num = num * 10 + (str[i] - '0');
        }
        else
        {
            // Invalid character found
            return 0;
        }
    }

    *result = num * sign;
    return 1;
}

int encode_operator(char *operator_name)
{
    // This is case senstitive for now, not sure if this matters
    // BRA : 000 00000 = 0
    if (!strcmp(operator_name, "BRA"))
    {
        return BRA;
    }
    // BZ: 001 00000 = 32
    else if (!strcmp(operator_name, "BZ"))
        return BZ;
    // LD: 010 00000 = 64
    else if (!strcmp(operator_name, "LD"))
        return LD;
    // LDI: 011 00000 = 96
    else if (!strcmp(operator_name, "LDI"))
        return LDI;
    // ST: 100 00000 = 128
    else if (!strcmp(operator_name, "ST"))
        return ST;
    // ADD: 101 00000 = 160
    else if (!strcmp(operator_name, "ADD"))
        return ADD;
    // NEG: 110 00000 = 192
    else if (!strcmp(operator_name, "NEG"))
        return NEG;
    // HLT: 111 00000 = 224
    else if (!strcmp(operator_name, "HLT"))
        return HLT;
    else
        return -1;
}

void print_binary(unsigned int n)
{
    unsigned int i;
    for (i = 1 << 7; i > 0; i = i / 2)
    {
        if (n & i)
        {
            putchar('1');
        }
        else
        {
            putchar('0');
        }
    }
    putchar('\n');
}

void trunc_token(char *token)
{
    int token_len = strlen(token);
    while (token[token_len - 1] == '\n' || token[token_len - 1] == ' ' || token[token_len - 1] == '\r')
    {
        token[token_len - 1] = '\0';
        token_len--;
    }
}

int is_whitespace(char *string)
{
    // Returns 1 if the whole string is whitepace
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] != '\t' || string[i] != ' ' || string[i] != '\n')
            return 0;
    }
    return 1;
}
