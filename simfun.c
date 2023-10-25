#include <stdlib.h>
#include <stdio.h>
#include "simply.h"

int main(int argc, char* argv[]){

    if (argc == 1)
    {
        fprintf(stderr, "No target assembly file specified\n");
        return 1;
    }

    char *fileName = argv[1];
    unsigned char buffer[256];

    
    FILE *file = fopen(fileName, "rb");
    if(file == NULL){
        fprintf(stderr, "Could not find specified input file\n");
        return 1;
    }
    fread(buffer, sizeof(buffer), 1, file);
    for(int i = 0; i < 256; i++){
        printf("%i\n", buffer[i]);
        if(buffer[i] == HLT){
            printf("HLT Detected: End File.\n");
            break;
        }
        printf("%c", buffer[i]);
    }
}