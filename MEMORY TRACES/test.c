#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

int TRACESIZE = 100001;

typedef struct {
	unsigned addr;
	char rw;
} Trace;

bool getEventTraces(FILE * file, Trace * Ref)
{
    if(file == 0)
        return false;

    unsigned int addr;
    char rw;
    int i;

    for(i = 0; fscanf(file, "%08x %c", &addr, &rw) != EOF; i++)
    {
        fgetc(file);
        Ref[i].addr = addr/4096; 	//Grabs the first bits - virtual page #
        Ref[i].rw = rw;			//Grabs the R/W char
    }
    fclose(file);
    return true;
}


int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    Trace traceFile[TRACESIZE];
    unsigned addr;
    char str[20];
    char rw;
    int i = 0;

	FILE * file;
	file = fopen(argv[1], "r");
    if(file == NULL) {
        fprintf(stderr, "ERROR: Can't open %s\n", fileName);
        exit(1); 
    } 

    if(!getEventTraces(fopen(argv[1], "r"), traceFile))
    {
        printf("File could not be opened.");
        return 1;
    }
    else {
        printf("File is successfully read.");
    }
    
    return 0;
}