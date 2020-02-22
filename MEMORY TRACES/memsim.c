#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <limits.h>

void lru (char* fileName, int nframes, char* mode) {

}

void fifo (char* fileName, int nframes, char* mode) {

}

void vms (char* fileName, int nframes, char* mode) {

}

void vms (char* fileName, int nframes, char* mode) {

}



int main(int argc, char *argv[]) {
    char *trace_file = argv[1];
    int nframes = atoi(argv[2]);
    char *algo = argv[3];
    char *mode = argv[4];

	FILE * file;
	file = fopen(argv[1], "r");

    if (strcmp(algo, "lru") == 0) 
    {
        printf("--- LRU: Last Recently Used --\n");
    }
    else if (strcmp(algo, "rmd") == 0) {
        printf("--- RMD: Randomly Selects to Replace --\n");
    }
    else if (strcmp(algo, "fifo") == 0)
    {
        printf("--- FIFO: First in First Out --\n");
    }
    else if (strcmp(algo, "vms") == 0)
    {
        printf("--- VMS --\n");
    }
    else
    {
        printf("Invalid algorithm.\n");
    }
}