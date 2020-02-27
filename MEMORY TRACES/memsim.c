#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>


void lru (char* fileName, int nframes, char* mode) {

    unsigned addr;
    char rw;
    FILE *fp;
    unsigned LRU[nframes];

    //open file. If open failed, exit.
    fp = fopen(fileName, "r");
    if(fp ==NULL){printf("ERROR Opening file\n");   exit(1);}

    while(fscanf(fileName,"%x %c",&addr,&rw) !=EOF)
    {

    }
}

struct trace   
{
    unsigned vpn;   //page number
    char d;        //Read/Write
    int dirt;          //dirty or not
};

void fifo (char* fileName, int nframes, char* mode) {

    unsigned addr;
    char rw;
    FILE *fp;
    struct trace FIFO[nframes];
    int k =0;   //keep track of oldest page 

    //open file. If open failed, exit.
    fp = fopen(fileName, "r");
    if(fp ==NULL){printf("ERROR Opening file\n");   exit(1);}

    //while not end of file, scan address and rw.
    while(fscanf(fileName,"%x %c",&addr,&rw) !=EOF)
    {
        struct trace temp;
        addr = addr >>12;
        temp.vpn = addr;
        temp.d = rw; 

        if(k==nframes){k=0;}    //if page is last, oldest page is at index 0

        if(mode == "debug") {printf("%x\t%c\n", addr, rw); }  //debug print 

        // for(int i =0; i < nframes; i++) //check address in frame
        // {
        //     if(FIFO[i].vpn == NULL)   //if frame has empty slot add to queue
        //     {
        //         FIFO[i] =temp;
        //     }
        //     else    //if no empty slot check if address is in
        //     {
        //         if(FIFO[i].vpn == addr) //if address exist in frame
        //         {

        //         }
        //         else    //else. if address not in frame
        //         {
                    
        //         }
        //     }            
        // }
        
        if(FIFO[k].vpn == NULL)
        {FIFO[k] = temp;}
        

    }

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