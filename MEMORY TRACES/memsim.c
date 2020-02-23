#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

int TRACESIZE = 100000;

typedef struct{
    char vpn;
    char d;
} Trace;

void lru (Trace* traceFile, int nframes, char* mode) {

}

void fifo (Trace* traceFile, int nframes, char* mode) {

}

void vms (Trace* traceFile, int nframes, char* mode) {   
    printf("PageNumber is %d", 20);
    int i;
    for(i = 0; i < TRACESIZE; i++)
    {
        printf(traceFile[i].vpn + " " + traceFile[i].d + '\n');
    }
}

void rmd (Trace* traceFile, int nframes, char* mode) {

}

void readTraceFile (FILE* file, Trace* traceFile){
    char address;
    char rw;
    int i;

    for(i = 0; fscanf(file, "%s %c", &address, &rw) != EOF; i++)
    {
        traceFile[i].vpn = address;
        traceFile[i].d = rw;
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    Trace traceFile[TRACESIZE];
    int nframes = atoi(argv[2]);
    char *algo = argv[3];
    char *mode = argv[4];

	FILE * file;
	file = fopen(argv[1], "r");
    if(!file) {
        fprintf(stderr, "ERROR: Can't open %s\n", fileName);
        exit(1); 
    }
    else

    if (strcmp(algo, "lru") == 0) 
    {
        printf("--- LRU: Last Recently Used --\n");
    }
    else if (strcmp(algo, "rmd") == 0) {
        printf("--- RMD: Randomly Selects to Replace --\n");
        lru(traceFile, nframes, mode);
    }
    else if (strcmp(algo, "fifo") == 0)
    {
        printf("--- FIFO: First in First Out --\n");
    }
    else if (strcmp(algo, "vms") == 0)
    {
        printf("--- VMS --\n");
        vms(traceFile, nframes, mode);
    }
    else
    {
        printf("Invalid algorithm.\n");
    }
}