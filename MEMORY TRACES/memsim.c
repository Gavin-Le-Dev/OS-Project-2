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

struct trace{
    unsigned vpn;
    bool isDirty;
    int access;
};

int existPage(struct trace* arr, int n, unsigned newAdd)
{
    int locate;

    for(int i=0; i<n; i++)
    {
        if(arr[i].vpn == newAdd)
        { locate =i;}
    }

    return locate;
}

/******************************************************************************************************************/
int smallest(struct trace* arr, int n) //find the smallest. For LRU to find smallest timer. i.e. least frequently 
{   
    int min, location;

    min = arr[0].access;
    for(int i=0; i<n; i++)
    {
        if(arr[i].access < min)
        {
            min = arr[i].access;
            location = i;
        }
    }

    return i;
}

void lru (FILE* file, int nframes, bool isDebug) 
{
    struct trace LRU[nframes];  //LRU table
    unsigned addr;
    char d;
    int timer = 0;

    for (i = 0; i < nframes; i++) //initialize frame to -1
        {LRU[i].vpn = -1;}

    while(fscanf(file, "%x %c", &addr, &d) != EOF)
    {

        struct trace newPage;
        newPage.vpn = addr>>12;     //new page address
        newPage.access = timer;     //new page frequency
        bool full = isFull(LRU, nframes, isDebug);
        bool isExist = rmdIsExist(LRU, nframes, newPage.vpn, isDebug);
        

        if (d == 'w' || d == 'W'){ newPage.isDirty = true; }
        else { newPage.isDirty = false; }

        if(isDebug){
            printf ("\nNew Address is: %x | %c\n", addr, d);
            printf ("Page number is: %x\n", newPage.vpn);
        }

         if (full) 
         {
            if (isDebug){printf("Table is full\n");}
            if (!isExist)   //if table is full and page does not exist in table
            {
                int LRUlocation = smallest(LRU, nframes); //replace with LRU page
                LRU[LRUlocation] = newPage;

                if (isDebug){
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Replace page %x to least frequently used location %d\n", newPage.vpn, LRUlocation);
                }
                nRead++;
            }
            else //if table is full and page exist on table
            {
                int foundlocation = existPage(LRU, nframes,  newPage.vpn);  //find the locatoin and update the timer to current
                LRU[foundlocation].access = timer;
                //maybe do sth with dirty

                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }
        else 
        {
            if (isDebug){printf("Table is not full\n");}
            if (!isExist) 
            { 
                int nextfreelocatoin;
                for(int a=0; a< nframes; a++)   //find the next empty spot
                {
                    if(LRU[a].vpn ==-1){nextfreelocatoin =a; break;}
                }
                LRU[nextfreelocatoin] = newPage;
                if (isDebug) {
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Add page %x to location %d", newPage.vpn, newPageLocation);
                }
                nRead++;
            }
            else 
            {
                int foundlocation = existPage(LRU, nframes,  newPage.vpn);  //find the locatoin and update the timer to current
                LRU[foundlocation].access = timer;
                if (isDebug){printf("Page %x is exist at location %d\n", newPage.vpn, foundLocation);}
            }
        }


        timer++;    //increment timer to the next time frame or next trace
    }


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


void vms (FILE* file, int nframes, bool isDebug) {  

    unsigned addr;
    char rw;
    FILE *fp;
    struct trace FIFO[nframes];
    int s, k =0;   //keep track of oldest page 
    
    //open file. If open failed, exit.
    fp = fopen(file, "r");
    if(fp ==NULL){printf("ERROR Opening file\n");   exit(1);}

    //while not end of file, scan address and rw.
    while(fscanf(file,"%x %c",&addr,&rw) !=EOF)
    {
        struct trace temp;
        addr = addr >>12;
        temp.vpn = addr;
        temp.d = rw; 
        s=0;

        if(k==nframes){k=0;}    //if page is last, oldest page is at index 0

        if(isDebug) {printf("%x\t%c\n", addr, rw); }  //debug print 

        //check if address exist in frame
        for(int i =0;i <nframes; i++)
        {
            if(FIFO[i].vpn == addr) //if address exist in frame
                {
                    //do something with that address in frame i
                    s++;
                    k--;   
                    }
        }
        k++;
            
        if((k <= nframes) && (s==0))
        {
        }
        else if(s==0)
        {
        }

        // //check if oldest slot is empty, insert page, and 
        // if(FIFO[k].vpn == NULL)
        // {FIFO[k] = temp; k++;}
        // else
        // {
    }

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
                if (page_table[foundLocation].isDirty){
                    if (!newPage.isDirty){
                        page_table[foundLocation] = newPage;
                        nWrite++;
                        nRead++;
                    }
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
                if (page_table[foundLocation].isDirty){
                    if (!newPage.isDirty){
                        page_table[foundLocation] = newPage;
                    }
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
                olderPageLocation++;
                if (isDebug){
                    printf("Page %x is not exist in table\n", newPage.vpn);
                    printf("Replace page %x to random location %d\n", newPage.vpn, olderPageLocation);
                }
                nRead++;
            }
            else {
                if (page_table[foundLocation].isDirty){
                    if (!newPage.isDirty){
                        page_table[foundLocation] = newPage;
                    }
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
                if (page_table[foundLocation].isDirty){
                    if (!newPage.isDirty){
                        page_table[foundLocation] = newPage;
                    }
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