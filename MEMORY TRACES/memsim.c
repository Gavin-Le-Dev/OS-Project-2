#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>

int i, dr=0,cl=0;;
int frms=0,frmsPA=0, frmsPB=0;
int nEvent = 0, nRead = 0, nWrite = 0, nMiss = 0, nHit =0,timer=0;
int newPageLocation;
int foundLocation;
int olderPageLocation = 0;

struct Trace{
    unsigned vpn;
    bool isDirty;
    int access;
    bool secondchance;
};

/***************************************************************************************************************/
bool isPageExist (struct Trace* table, int nFrames, unsigned addr, bool isDebug){
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
        newPage.access = timer;
        bool full = isFull(page_table, nframes, isDebug);
        bool isExist = isPageExist(page_table, nframes, newPage.vpn, isDebug);

        
        if (d == 'w' || d == 'W'){ newPage.isDirty = true; }
        else { newPage.isDirty = false; }
        if (isDebug){
            printf ("\nNew Address is: %x | %c\n", addr, d);
            printf ("Page number is: %x\n", newPage.vpn);
        }

        if (full) {
            if (isDebug){printf("Table is full\n");}
            if (!isExist){
                int a = smallest(page_table, nframes);
                if (page_table[a].isDirty){
                    nWrite++;
                }
                page_table[a] = newPage;
                if (isDebug){
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Replace page %x to random location %d\n", newPage.vpn, olderPageLocation);
                }
                olderPageLocation++;
                nMiss++;
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                //hit. set timer
                page_table[foundLocation].access = timer;
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
                olderPageLocation++;
                nMiss++;
                nRead++;
            }
            else {
                if (newPage.isDirty){
                    page_table[foundLocation].isDirty = newPage.isDirty;
                }
                //hit. set timer
                page_table[foundLocation].access = timer;
                nHit++;
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }
        if (olderPageLocation == nframes){olderPageLocation = 0;}

        timer++;
    }

    if (isDebug)
	{
		printf("\n[*] Number of Hits: %d\n", nHit);
		printf("[*] Number of Misses: %d\n", nMiss);
		double hitrate = (double)nHit / (double)(nMiss + nHit);
		printf("[*] Hit Rate: %.7f\n", hitrate);
	}

    printf("\nTotal memory frames: %d\n", nframes);
    printf("Events in trace: %d\n", nEvent);
    printf("Total disk reads: %d\n", nRead);
    printf("Total disk writes: %d\n", nWrite);
}
/******************************************************************************************************************/


bool addExist(struct Trace* arr, int n, unsigned ad)
{
    for(int j=0; j<n; j++)
    {
        if(arr[j].vpn == ad)
        return true;
    }
    return false;
}
/******************************************************************************************************************/
/************************************************VMS***************************************************************/
/******************************************************************************************************************/
void VMSprocessA(struct Trace* VMS, struct Trace* PA, struct Trace* dirt, struct Trace* clean, struct Trace newPage, char d, int nframes, bool isDebug)
{
        
    
}

void VMSprocessB(struct Trace* VMS, struct Trace* PB, struct Trace* dirt, struct Trace* clean, struct Trace newPage, char d, int nframes, bool isDebug)
{
    bool isExist = isPageExist(PB, nframes/2, newPage.vpn, isDebug);
    bool Full = isFull(VMS, nframes, isDebug);
    bool BFull = isFull(PB, nframes/2, isDebug);
    if(isExist)//if new page exist in PA do nothing. Hit++
    {   
        if (isDebug){printf("Hit in PB\n");}
        nHit++;
    }
    else
    {
        //if PB is full
        if(BFull)
        {
            if (isDebug){printf("PB is full\n");}
            if(newPage.isDirty)
            {
                if (isDebug){printf("Dirty Added\n");}
                dirt[dr] = newPage;
                dr++;
            }
            else
            {
                if (isDebug){printf("Clean Added\n");}
                clean[cl] = newPage;
                cl++;
            }
            if (isDebug){printf("PB added %d\n", PB[frmsPB].vpn);}
            PB[frmsPB] = newPage;
        }
        else
        {
            if (isDebug){printf("PB added %d\n", PB[frmsPB].vpn);}
            PB[frmsPB] = newPage;
            frmsPB++;
        }

        if(isPageExist(VMS, nframes, newPage.vpn, isDebug) ==true)
        {   
            if (isDebug){printf("Hit in Table\n");}
            nHit++;
            dirt[foundLocation].vpn =-1;
            clean[foundLocation].vpn =-1;
        }
        else
        {   //miss in memory VMS
            nMiss++;
            if (isDebug){printf("Miss in Table\n");}
            //if VMS is full
            if(Full)
            {
                if (isDebug){printf("Table is full\n");}
                if (isDebug){printf("VMS added %d\n", VMS[frms].vpn);}
                VMS[frms] = newPage;
                nWrite++;
                frms++;
            }
            else
            {
                if (isDebug){printf("VMS added %d\n", VMS[frms].vpn);}
                VMS[frms] = newPage;
                frms++;
            }
            
        }
        
    }
    
    if(frms == nframes){frms =0;}
    if(frmsPB == nframes/2){frmsPB =0;}
}

void vms(FILE* file, int nframes, int debug)
{
    struct Trace VMS[nframes];
    unsigned addr;
    char rw;
    struct Trace PA[nframes/2];
    struct Trace PB[nframes/2];
    struct Trace dirt[nframes/2 +1];
    struct Trace clean[nframes/2 +1];

    for (i = 0; i < nframes; i++) {
        VMS[i].vpn = -1;
        PA[i].vpn = -1;
        PB[i].vpn = -1;
        dirt[i].vpn =-1;
        clean[i].vpn =-1;
        VMS[i].secondchance = false;
    }

	while(fscanf(file, "%x %c", &addr, &rw) > 0)
	{
        struct Trace newPage;
		newPage.vpn = addr >> 12;
		newPage.secondchance = false;
        
		unsigned first = addr >> 16;

		if (rw == 'w' || rw == 'W'){ newPage.isDirty = true; }
        else { newPage.isDirty = false; }
        if (debug){
            printf ("\nNew Address is: %x | %c\n", addr, rw);
            printf ("Page number is: %x\n", newPage.vpn);
        }

		if(first == 3){
			VMSprocessA(VMS, PA, dirt, clean,newPage, rw, nframes, debug);
        }
		else{
			VMSprocessB(VMS, PB,dirt, clean, newPage, rw, nframes, debug);
        }
		nEvent++;
	}
	if (debug)
	{
		printf("[*] Number of Hits: %d\n", nHit);
		printf("[*] Number of Misses: %d\n", nMiss);
		double hitrate = (double) nHit / ((double)(nHit+nMiss));
		printf("[*] Hit Rate: %.1f\n", hitrate);
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
        bool isExist = isPageExist(page_table, nframes, newPage.vpn, isDebug);

        
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
		printf("[*] Hit Rate: %.1f\n", hitrate);
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
        bool isExist = isPageExist(page_table, nframes, newPage.vpn, isDebug);

        
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
                olderPageLocation++;
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
        if (olderPageLocation == nframes){olderPageLocation = 0;}
    }

    if (isDebug)
	{
		printf("[*] Number of Hits: %d\n", nHit);
		printf("[*] Number of Misses: %d\n", nMiss);
		double hitrate = (double)nHit / ((double)(nHit+nMiss));
		printf("[*] Hit Rate: %.1f\n", hitrate);
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
        printf("VMS '%s' with %d frames in %s mode\n", fileName, nframes, mode);
        vms(file, nframes, isDebug);
    }
    else
    {
        printf("Invalid algorithm.\n");
    }
}