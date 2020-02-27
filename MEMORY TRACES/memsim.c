#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

typedef struct{
    unsigned vpn;
    char d;
} Trace;

void lru (FILE* file, int nframes, char* mode) {
    
}

void fifo (FILE* file, int nframes, char* mode) {

}

void vms (FILE* file, int nframes, char* mode) {  

}

void rmd (FILE* file, int nframes, char* mode) {
    Trace page_table[nframes];
    unsigned addr;
    char d;
    int i = 0;

    for (i = 0; i < nframes; i++)
    {
        fscanf(file, "%x %c", &addr, &d);
        page_table[i].vpn = addr>>12;
        page_table[i].d = d;
        printf("%x\n", page_table[i].vpn);
    }
}

int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    int nframes = atoi(argv[2]);
    char *algo = argv[3];
    char *mode = argv[4];
    unsigned addr;
    char d;
    int i = 0;

	FILE *file;
	file = fopen(argv[1], "r");
    if(!file) {
        fprintf(stderr, "ERROR: Can't open %s\n", fileName);
        exit(1); 
    }

    if (strcmp(algo, "lru") == 0) 
    {
        printf("--- LRU: Last Recently Used --\n");
    }
    else if (strcmp(algo, "rmd") == 0) {
        printf("--- RMD: Randomly Selects to Replace --\n");
        rmd(file, nframes, mode);
    }
    else if (strcmp(algo, "fifo") == 0)
    {
        printf("--- FIFO: First in First Out --\n");
    }
    else if (strcmp(algo, "vms") == 0)
    {
        printf("--- VMS --\n");
        vms(file, nframes, mode);
    }
    else
    {
        printf("Invalid algorithm.\n");
    }
}