#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){

    if (argc == 1)
    {
        fprintf(stderr, "No target assembly file specified\n");
        return 1;
    }
    char *fileName = argv[2];

    unsigned char buffer[256];

    
    FILE *file = fopen(fileName, "rb");
    fread(buffer, sizeof(buffer), 1, file);
    for(int i = 0; i < 256; i++){
        if(buffer[i] == 'H'){
            printf("HLT\n");
            printf("HLT Detected: End File.\n");
            break;
        }
        printf("%c", buffer[i]);
    }
}