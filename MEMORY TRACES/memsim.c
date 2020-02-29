#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>

int i;
int nEvent = 0, nRead = 0, nWrite = 0, nHit = 0, nMiss = 0;
int newPageLocation;
int foundLocation;
int olderPageLocation = 0;

struct Trace{
    unsigned vpn;
    bool isDirty;
};

void lru (FILE* file, int nframes, bool isDebug) {
    
}

void vms (FILE* file, int nframes, bool isDebug) {  

}

/***************************************************************************************************************/
bool rmdIsExist (struct Trace* table, int nFrames, unsigned addr, bool isDebug){
    for (i = 0; i < nFrames; i++){
        if (table[i].vpn == addr){
            foundLocation = i;
            return true;
        }
    }
    return false;
}

bool isFull (struct Trace* table, int nFrames, bool isDebug){
    for (i = 0; i < nFrames; i++)
    {
        if (table[i].vpn == -1){
            newPageLocation = i;
            return false;
        }
    }
    return true;
}

void rmd (FILE* file, int nframes, bool isDebug) {
    struct Trace page_table[nframes];
    unsigned addr;
    char d;
    
    for (i = 0; i < nframes; i++) {
        page_table[i].vpn = -1;
    }

    while (fscanf(file, "%x %c", &addr, &d) != EOF){
        int nRandonNumber;
        nEvent++;
        struct Trace newPage;
        newPage.vpn = addr>>12;
        bool full = isFull(page_table, nframes, isDebug);
        bool isExist = rmdIsExist(page_table, nframes, newPage.vpn, isDebug);

        
        if (d == 'w' || d == 'W'){ newPage.isDirty = true; }
        else { newPage.isDirty = false; }
        if (isDebug){
            printf ("\nNew Address is: %x | %c\n", addr, d);
            printf ("Page number is: %x\n", newPage.vpn);
        }

        if (full) {
            if (isDebug){printf("Table is full\n");}
            if (!isExist){
                nRandonNumber = rand()%nframes;
                if (page_table[nRandonNumber].isDirty){
                    nWrite++;
                }
                page_table[nRandonNumber] = newPage;
                if (isDebug){
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Replace page %x to random location %d\n", newPage.vpn, nRandonNumber);
                }
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }
        else {
            if (isDebug){printf("Table is not full\n");}
            if (!isExist) { 
                page_table[newPageLocation] = newPage;
                if (isDebug) {
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Add page %x to location %d", newPage.vpn, newPageLocation);
                }
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }
    }
    printf("\nTotal memory frames: %d\n", nframes);
    printf("Events in trace: %d\n", nEvent);
    printf("Total disk reads: %d\n", nRead);
    printf("Total disk writes: %d\n", nWrite);
}
/***************************************************************************************************************/

void fifo (FILE* file, int nframes, bool isDebug) {
    struct Trace page_table[nframes];
    unsigned addr;
    char d;
    
    for (i = 0; i < nframes; i++) {
        page_table[i].vpn = -1;
    }

    while (fscanf(file, "%x %c", &addr, &d) != EOF){
        nEvent++;
        struct Trace newPage;
        newPage.vpn = addr>>12;
        bool full = isFull(page_table, nframes, isDebug);
        bool isExist = rmdIsExist(page_table, nframes, newPage.vpn, isDebug);

        
        if (d == 'w' || d == 'W'){ newPage.isDirty = true; }
        else { newPage.isDirty = false; }
        if (isDebug){
            printf ("\nNew Address is: %x | %c\n", addr, d);
            printf ("Page number is: %x\n", newPage.vpn);
        }

        if (full) {
            if (isDebug){printf("Table is full\n");}
            if (!isExist){
                if (page_table[olderPageLocation].isDirty){
                    nWrite++;
                }
                page_table[olderPageLocation] = newPage;
                if (isDebug){
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Replace page %x to random location %d\n", newPage.vpn, olderPageLocation);
                }
                olderPageLocation++;
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }
        else {
            if (isDebug){printf("Table is not full\n");}
            if (!isExist) { 
                page_table[olderPageLocation] = newPage;
                if (isDebug) {
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Add page %x to location %d", newPage.vpn, newPageLocation);
                }
                olderPageLocation++;
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }
        if (olderPageLocation == nframes){olderPageLocation = 0;}
    }
    printf("\nTotal memory frames: %d\n", nframes);
    printf("Events in trace: %d\n", nEvent);
    printf("Total disk reads: %d\n", nRead);
    printf("Total disk writes: %d\n", nWrite);
}

/***************************************************************************************************************/

int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    int nframes = atoi(argv[2]);
    char *algo = argv[3];
    char *mode = argv[4];
    bool isDebug;

    if (strcmp(mode, "debug") == 0){
        isDebug = true;
    }
    else if (strcmp(mode, "quiet") == 0)
    {
        isDebug = false;
    }
    else
    {
        printf("Invalid Mode");
    }

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
        printf("RMD '%s' with %d frames in %s mode\n", fileName, nframes, mode);
        rmd(file, nframes, isDebug);
    }
    else if (strcmp(algo, "fifo") == 0)
    {
        printf("--- FIFO: First in First Out --\n");
        fifo(file, nframes, isDebug);
    }
    else if (strcmp(algo, "vms") == 0)
    {
        printf("VMS '%s' with %d frames in %s mode\n", fileName, nframes, mode);
        vms(file, nframes, isDebug);
    }
    else
    {
        printf("Invalid algorithm.\n");
    }
}