#include <stdlib.h>
#include <stdio.h>

void processBinaryFile(FILE *file){

    unsigned char buffer[10];
    FILE *ptr;
    ptr = fopen("binary.bin", "rb");
    fread(buffer, sizeof(buffer), 1, ptr);

}

int main(){
    unsigned char buffer[256];
    FILE *ptr;
    char fileName[10];

    printf("Enter File Name: ");
    scanf("%s", fileName);

    ptr = fopen(fileName, "rb");
    fread(buffer, sizeof(buffer), 1, ptr);
    for(int i = 0; i < 256; i++){
        if(buffer[i] == 'H'){
            printf("HLT\n");
            printf("HLT Detected: End File.\n");
            break;
        }
        printf("%c", buffer[i]);
    }
}