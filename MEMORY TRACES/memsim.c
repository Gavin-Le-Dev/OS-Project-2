#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

typedef struct{
    unsigned vpn;
    char d;
} Trace;

struct trace   
{
    unsigned vpn;   //page number
    char d;        //Read/Write
    int dirt;          //dirty or not
};


void lru (FILE* file, int nframes, char* mode) {
    
}


bool addExist(Trace* arr, int n, unsigned ad)
{
    for(int j=0; j<n; j++)
    {
        if(arr[j].vpn == ad)
        return true;
    }

    return false;
}


void fifo (FILE* file, int nframes, char* mode) {

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

        if(mode == "debug") {printf("%x\t%c\n", addr, rw); }  //debug print 

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