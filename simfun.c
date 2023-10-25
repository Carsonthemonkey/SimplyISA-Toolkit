#include <stdlib.h>
#include <stdio.h>
#include "simply.h"

#define MAX_INSTRUCTIONS 1024

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

    unsigned char buffer[MAX_INSTRUCTIONS];

    fread(buffer, sizeof(unsigned char), MAX_INSTRUCTIONS, file);
    for(int i = 0; i < 256; i++){
        printf("%i\n", buffer[i]);
        if(buffer[i] == HLT){
            printf("HLT Detected: End File.\n");
            break;
        }
    }
}