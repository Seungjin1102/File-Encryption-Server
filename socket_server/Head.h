#pragma comment(lib, "ws2_32")

#include <winsock2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "openssl/sha.h"

typedef struct pair {
    unsigned char label[10];
    unsigned char value[17];
}Pair;

typedef struct word {
    unsigned char a[16];
}Word;

struct Flags {
    int a : 8;
    unsigned int b : 1;//Beta°ª 0 or 1
};

typedef struct index {
    int i;
    int j;
}Index;



void ErrorHandling(char* message);
Word* TsetRetrieve(Pair (*)[128], unsigned char* );
