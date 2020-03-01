#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>

int i;
int frms=0,frmsPA=0, frmsPB=0;
int nEvent = 0, nRead = 0, nWrite = 0, nMiss = 0, nHit =0;
int newPageLocation;
int foundLocation;
int oldestPageLocation = 0;

struct Trace{
    unsigned vpn;
    bool isDirty;
    int access;
};

/***************************************************************************************************************/
bool isPageExist (struct Trace* table, int nFrames, unsigned addr){
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
/******************************************************************************************************************/

int smallest(struct Trace* arr, int n) //find the smallest. For LRU to find smallest timer. i.e. least frequently 
{   
    int min, location;

    min = arr[0].access;
    for(i=0; i<n; i++)
    {
        if(arr[i].access <= min)
        {
            min = arr[i].access;
            location = i;
        }
    }
    return location;
}

/******************************************************************************************************************/
/***************************************LRU************************************************************************/
/******************************************************************************************************************/

void lru (FILE* file, int nframes, bool isDebug) 
{
    struct Trace LRU[nframes];  //LRU table
    unsigned addr;
    char d;
    int timer = 0;
    int Read, Write;

    for (i = 0; i < nframes; i++) //initialize frame to -1
        {LRU[i].vpn = -1;}

    while(fscanf(file, "%x %c", &addr, &d) != EOF)
    {
        struct Trace newPage;
        newPage.vpn = addr>>12;     //new page address
        newPage.access = timer;     //new page frequency
        bool full = isFull(LRU, nframes, isDebug);
        bool isExist = isPageExist(LRU, nframes, newPage.vpn);
        

        if (d == 'w' || d == 'W'){ newPage.isDirty = true; }
        else { newPage.isDirty = false; }

        if(isDebug){
            printf ("\nNew Address is: %x | %c\n", addr, d);
            printf ("Page number is: %x\n", newPage.vpn);
        }

         if (full) 
         {
            if (isDebug){printf("Table is full\n");}
            if (!isExist)   //if table is full and page does not exist in table. Page fault, evict.
            {
                int LRUlocation = smallest(LRU, nframes); //replace with LRU page
                LRU[LRUlocation] = newPage;

                if (isDebug){
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Replace page %x to least frequently used location %d\n", newPage.vpn, LRUlocation);
                }
                nMiss++;
                Write++;
                Read++;
            }
            else //if table is full and page exist on table
            {
                LRU[foundLocation].access = timer;
                if (newPage.isDirty){
                    LRU[foundLocation].isDirty = newPage.isDirty;
                }
                nHit++;
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }
        else 
        {
            if (isDebug){printf("Table is not full\n");}
            if (!isExist) 
            { 
                LRU[newPageLocation] = newPage;
                if (isDebug) {
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Add page %x to location %d", newPage.vpn, newPageLocation);
                }
                nMiss++;
                Read++;
            }
            else 
            {
                LRU[foundLocation].access = timer;
                if (newPage.isDirty){
                    LRU[foundLocation].isDirty = newPage.isDirty;
                }
                nHit++;
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }

        timer++;    //increment timer to the next time frame or next trace
    }

    if (isDebug)
	{
		printf("\n[*] Number of Hits: %d\n", nHit);
		printf("[*] Number of Misses: %d\n", nMiss);
		double hitrate = (double)nHit / (double) nEvent;
		printf("[*] Hit Rate: %0.3f\n", hitrate);
	}

    printf("\nTotal memory frames: %d\n", nframes);
    printf("Events in trace: %d\n", timer);
    printf("Total disk reads: %d\n", Read);
    printf("Total disk writes: %d\n", Write);
}
/******************************************************************************************************************/


bool addExist(struct Trace* arr, int n, unsigned ad)
{
    int j;
    for(j=0; j<n; j++)
    {
        if(arr[j].vpn == ad)
        return true;
    }
    return false;
}
/******************************************************************************************************************/
/************************************************vmsA***************************************************************/
/******************************************************************************************************************/
void VMSprocessA(struct Trace* vmsA, struct Trace newPage, int nframes, bool isDebug)
{
    bool full = isFull(vmsA, nframes, isDebug);
    bool isExist = isPageExist(vmsA, nframes, newPage.vpn);
    if (full) {
        if (isDebug){printf("Table is full\n");}
        if (!isExist){
            if (vmsA[oldestPageLocation].isDirty){
                nWrite++;
            }
            vmsA[oldestPageLocation] = newPage;
            if (isDebug){
                printf("Page %x is not exist in table\n", newPage.vpn);
                printf("Replace page %x to random location %d\n", newPage.vpn, oldestPageLocation);
            }
            oldestPageLocation++;
            nMiss++;
            nRead++;
        }
        else {
            if (newPage.isDirty){
                vmsA[foundLocation].isDirty = newPage.isDirty;
            }
            nHit++;
            if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
        }
    }
    else {
        if (isDebug){printf("Table is not full\n");}
        if (!isExist) { 
            vmsA[newPageLocation] = newPage;
            if (isDebug) {
                printf("Page %x is not exist in table\n", newPage.vpn);
                printf("Add page %x to location %d", newPage.vpn, newPageLocation);
            }
            nMiss++;
            nRead++;
        }
        else {
            if (newPage.isDirty){
                vmsA[foundLocation].isDirty = newPage.isDirty;
            }
            nHit++;
            if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
        }
    }
    if (oldestPageLocation == nframes){oldestPageLocation = 0;}
}

void VMSprocessB(struct Trace* vmsB, struct Trace newPage, int nframes, bool isDebug)
{
    bool full = isFull(vmsB, nframes, isDebug);
    bool isExist = isPageExist(vmsB, nframes, newPage.vpn);
    if (full) {
        if (isDebug){printf("Table is full\n");}
        if (!isExist){
            if (vmsB[oldestPageLocation].isDirty){
                nWrite++;
            }
            vmsB[oldestPageLocation] = newPage;
            if (isDebug){
                printf("Page %x is not exist in table\n", newPage.vpn);
                printf("Replace page %x to random location %d\n", newPage.vpn, oldestPageLocation);
            }
            oldestPageLocation++;
            nMiss++;
            nRead++;
        }
        else {
            if (newPage.isDirty){
                vmsB[foundLocation].isDirty = newPage.isDirty;
            }
            nHit++;
            if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
        }
    }
    else {
        if (isDebug){printf("Table is not full\n");}
        if (!isExist) { 
            vmsB[newPageLocation] = newPage;
            if (isDebug) {
                printf("Page %x is not exist in table\n", newPage.vpn);
                printf("Add page %x to location %d", newPage.vpn, newPageLocation);
            }
            nMiss++;
            nRead++;
        }
        else {
            if (newPage.isDirty){
                vmsB[foundLocation].isDirty = newPage.isDirty;
            }
            nHit++;
            if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
        }
    }
    if (oldestPageLocation == nframes){oldestPageLocation = 0;}
}

void vms(FILE* file, int nframes, bool isDebug)
{
    struct Trace vmsA[nframes/2];
    struct Trace vmsB[nframes/2];
    
    unsigned addr;
    char rw;
    
    for (i = 0; i < nframes/2; i++) {
        vmsA[i].vpn = -1;
        vmsB[i].vpn = -1;
    }

	while(fscanf(file, "%x %c", &addr, &rw) != EOF)
	{
        struct Trace newPage;
		newPage.vpn = addr >> 12;
        bool full = isFull(vmsA, nframes, isDebug);
        bool isExist = isPageExist(vmsA, nframes, newPage.vpn);

        if (isDebug){
            printf ("\nNew Address is: %x | %c\n", addr, rw);
            printf ("Page number is: %x\n", newPage.vpn);
        }

        if (rw == 'w' || rw == 'W'){ newPage.isDirty = true; }
        else { newPage.isDirty = false; }

		unsigned current_process = newPage.vpn >> 16;

		if(current_process == 3){
			VMSprocessA(vmsA,newPage, nframes/2, isDebug);
        }
		else{
			VMSprocessB(vmsB, newPage, nframes/2, isDebug);
        }
		nEvent++;
	}
	if (isDebug)
	{
		printf("[*] Number of Hits: %d\n", nHit);
		printf("[*] Number of Misses: %d\n", nMiss);
		double hitrate = (double)nHit / ((double)(nHit+nMiss));
		printf("[*] Hit Rate: %0.3f\n", hitrate);
	}

	printf("\nTotal memory frames: %d\n", nframes);
    printf("Events in trace: %d\n", nEvent);
    printf("Total disk reads: %d\n", nRead);
    printf("Total disk writes: %d\n", nWrite);
}


/********************************************RANDOM****************************************************************/
/******************************************************************************************************************/
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
        bool isExist = isPageExist(page_table, nframes, newPage.vpn);

        
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
                nMiss++;
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                nHit++;
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
                nMiss++;
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                nHit++;
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }
    }

    if (isDebug)
	{
		printf("[*] Number of Hits: %d\n", nHit);
		printf("[*] Number of Misses: %d\n", nMiss);
		double hitrate = (double)nHit / ((double)(nHit+nMiss));
		printf("[*] Hit Rate: %0.3f\n", hitrate);
	}
    
    printf("\nTotal memory frames: %d\n", nframes);
    printf("Events in trace: %d\n", nEvent);
    printf("Total disk reads: %d\n", nRead);
    printf("Total disk writes: %d\n", nWrite);
}
/******************************************************************************************************************/
/***********************************************FIFO***************************************************************/
/******************************************************************************************************************/

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
        bool isExist = isPageExist(page_table, nframes, newPage.vpn);

        
        if (d == 'w' || d == 'W'){ newPage.isDirty = true; }
        else { newPage.isDirty = false; }
        if (isDebug){
            printf ("\nNew Address is: %x | %c\n", addr, d);
            printf ("Page number is: %x\n", newPage.vpn);
        }

        if (full) {
            if (isDebug){printf("Table is full\n");}
            if (!isExist){
                if (page_table[oldestPageLocation].isDirty){
                    nWrite++;
                }
                page_table[oldestPageLocation] = newPage;
                if (isDebug){
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Replace page %x to random location %d\n", newPage.vpn, oldestPageLocation);
                }
                oldestPageLocation++;
                nMiss++;
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                nHit++;
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
                nMiss++;
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                nHit++;
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }
        if (oldestPageLocation == nframes){oldestPageLocation = 0;}
    }

    if (isDebug)
	{
		printf("[*] Number of Hits: %d\n", nHit);
		printf("[*] Number of Misses: %d\n", nMiss);
		double hitrate = (double)nHit / ((double)(nHit+nMiss));
		printf("[*] Hit Rate: %0.3f\n", hitrate);
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
        lru(file, nframes, isDebug);
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
        printf("VMS: '%s' with %d frames in %s mode\n", fileName, nframes, mode);
        vms(file, nframes, isDebug);
    }
    else
    {
        printf("Invalid algorithm.\n");
    }
    return 0;
}