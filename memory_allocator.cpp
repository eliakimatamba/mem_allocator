#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// PURPOSE:
// A program which maintains a heap which you will organize as an
// implicit free list. This program will allow a user to allocate memory, free memory,
// and see the current state of the heap. 


char* menu =  " COMMANDS: \n1. allocate (int)\n2. free (int)\n3. blocklist\n4. writeheap (int) (char) (int)\n5. printheap (int) (int)\n6. printheader (int)\n7. quit\n\0";


void allocate ( unsigned char * heapList, int numOfBytes , int *block_number){;

    // This function allows the user to allocate a block of memory from
    // your heap. This function should take one argument, the number of bytes
    // which the user wants in the payload of the allocated block. This function
    // should print out a unique block number which is associated with the block of
    // memory which has just been allocated. The block numbers should increment
    // each time a new block is allocated. So the first allocated block should be block
    // number 1, the second is block number 2, etc. Notice that only the allocated
    // blocks receive block numbers.

    int i = 0;
    while (i < 128 && ((heapList[i+1] & 1) || ((heapList[i+1] >> 1) <= numOfBytes))){
        i = i + (heapList[i+1] >> 1);
    }
                                                      
    int newsize = numOfBytes;
    int oldblocknum = heapList[i];                                                                      
    int oldsize = heapList[i+1] >> 1;
                                                                          
    heapList[i] = ++*block_number;                                                                      
    heapList[i+1] = newsize+2;                         
    heapList[i+1] = (heapList[i+1] << 1); // makes room for the allocated bit                                                                   
    heapList[i+1] = heapList[i+1] | 1;  // allocates the block                                                                   
                                                                          
    i = i + newsize + 2;                                                                                                                               
                                                                          
   if ((oldsize > newsize)) {//&& !(heapList[i+1] & 1)){
             heapList[i] = oldblocknum;
             heapList[i+1] = (oldsize - newsize)-2;
             heapList[i+1] = heapList[i+1] << 1;
   }

    printf("%d\n", *block_number);

}

void freeMem ( unsigned char * heapList, int inputBlockNum, int *block_number ){

    // This function allows the user to free a block of memory. This function
    // takes one argument, the block number associated with the previously
    // allocated block of memory.

    unsigned int i = 0;

    if (inputBlockNum > *block_number){
        printf("%s\n", "invalid block num" );
    }
    else{
        while ( i < 128 ){
            if (heapList[i] == inputBlockNum){
                // heapList[i+1] = heapList[i+1] << 1;
                // heapList[i+1] = heapList[i+1] & 0;
                heapList[i+1] = heapList[i+1] >> 1;
                heapList[i+1] = heapList[i+1] << 1;
                break;
            }
            i = i + (heapList[i+1] >> 1);
        }
    } 
};

void blocklist (unsigned char * heapList, int *block_number){

    // This command prints out information about all of the blocks in your
    // heap. It takes no arguments. The following information should be printed
    // about each block:
    // I. Size
    // II. Allocated (yes or no)
    // III. Start address
    // IV. End address
    // Addresses should be printed in hexadecimal. The blocks should be printed in the
    // order in which they are found in the heap.

    printf("%s\t%s\t%s\t\t%s\n", "Size", "Allocated", "Start", "End" );

    // ptr[1] >> 1; // prints size of block
    unsigned int i = 0;
    int count = 0;

    // checks if the index is smaller than heap size and doesn't exceed number of times blocks were created
    // the + 1 is for the initial header that was created ( ptr[0]=0 and ptr[1]=126)
    // if nothing is allocated blocklist should always return 126 and that it IS NOT allocated
    while ( i < 128 && count < *block_number + 1){ 
        // checks whether or not the block is allocated
        char allocatedOrNot[10];
        if ((heapList[i+1] & 1)){
            strcpy(allocatedOrNot, "yes");
        } else {
            strcpy(allocatedOrNot, "no");
        }

        printf("%d\t%s\t\t%p\t%p\n", (heapList[i+1] >> 1),  allocatedOrNot, &heapList[i], &heapList[i + (heapList[i+1] >> 1) - 1]);
        i = i + (heapList[i+1] >> 1);
        count++;

    }

};

void writeheap (unsigned char * heapList, int *block_number, int inputBlockNum, char charToWrite, int numOfCopies ){
    
    // This function writes characters into a block in the heap. The
    // function takes three arguments: the block number to write to, the character to
    // write into the block, and the number of copies of the character to write into the
    // block. The specified character will be written into n successive locations of
    // the block, where n is the third argument. This function should not overwrite
    // the header of the block which it is writing to, only the payload. The function
    // should not write outside of the payload of the block that it is writing to. If the
    // number of copies passed as an argument to writeheap is larger than the
    // payload size then nothing should be written to the block and the message
    // “Write too big” should be printed to the screen.

    if (inputBlockNum > *block_number) {
        printf("Error: block %d does not exist", inputBlockNum);
    }
    int i = 0;
    while ( i < 128) {
        printf("%d\n", i );
        printf("%s %d\n", "heap", heapList[i] );
        printf("%s %d\n", "inputBlockNum", inputBlockNum);
        if ( heapList[i] == inputBlockNum ) {
            if ( (heapList[i+1] >> 1) - 2 < numOfCopies ) {
                printf("Error: write too big\n");
                return;
            }
            if ( !(heapList[i+1] & 1) ) {
                printf("Error: Can't write to unallocated block\n");
                return;
            }
            int j;
            for ( j = 0; j <= numOfCopies-1; j++ ) {
                heapList[i+2+j] = charToWrite;
            }
            return;
        }
        i = i + (heapList[i+1] >> 1);
        printf("%s\n", "incremented");
        break;
    }
    int j = 0;
    for (j=0; j < 20; j++) {
        printf("%c ", heapList[j]);
    }
          
};

void printheap ( unsigned char * heapList, int *block_number, int inputBlockNum, int numOfBytes ){
    
    // This prints out the contents of a portion of the heap. This function
    // takes two arguments: the number of the block to be printed, and the number
    // of bytes to print after the start of the block. This function should not print the
    // header of the chosen block, only the payload. If the number of bytes to print
    // is larger than the size of the block, this function should print the bytes anyway,
    // even though they might extend into a neighboring block.

    if (inputBlockNum > *block_number) {
        printf("Error: block %d does not exist\n", inputBlockNum);
    }
    else {
        int i = 0;
        while ( i < 128 ) {
            if ( heapList[i] == inputBlockNum ) {
                if ( !(heapList[i+1] & 1) ) {
                    printf("Error: block %d is unallocated\n", inputBlockNum);
                }
                int current_size = (heapList[i+1] >> 1) -2;
                int j;
                for ( j = 1; j <= numOfBytes; j++) {
                    if ( j == current_size+1 ) {
                        j = j + 2;
                        numOfBytes = numOfBytes + 2;
                    }
                    printf("%c", heapList[i+1+j]);
                }
                printf("\n");
                return;
            }
            i = i + (heapList[i+1] >> 1);
        }
    }  
}

void printheader ( unsigned char * heapList, int inputBlockNum, int *block_number  ){
    
    // This prints the header of a block in your heap. This function
    // takes one argument, the number of the block whose header is to be printed.
    // The header should be two bytes long and it should be printed in hexadecimal.
    // For example, the following command might be used to print the header of block 2:
    // >printheader 2
    // 020b

    unsigned int i = 0;

    if (inputBlockNum > *block_number){
        printf("%s\n", "invalid block num" );
    }
    else{
        while ( i < 128 ){
            if (heapList[i] == inputBlockNum){
                //printf("0x%x\n", heapList[i] );
                printf("%02hhx%02hhx\n", heapList[i], heapList[i+1]);
                //printf("%04x\n", heapList[i]+heapList[i+1]);
                break;
            }
            i = i + (heapList[i+1] >> 1);
        }
    }     
}


int main(){

        // no more than 256 blocks will ever be allocated. 
        // a block is never larger than 128 bytes, including header.
        int sizeOfBlock = 128;
        int block_number = 0;


        unsigned char *ptr = (unsigned char*) malloc(sizeOfBlock * sizeof(char));
        
        // creates the header block and makes space for the allocated bit
        ptr[0] = 0;
        ptr[1] = 126;
        ptr[1] = ptr[1] << 1;

        // gets command as well as the command's possible arguments
        char input[50], command[20], arg2;
        int arg1, arg3;

        // keep asking for commands as long as user doesn't type quit
        do{
				system("clear");
				printf("%s",menu);
                printf("> ");
                fgets (input, 100, stdin);
                sscanf(input, "%s %d %s %d", command, &arg1, &arg2, &arg3);

                if (!strncmp(command,"allocate",8)){
                    allocate(ptr, arg1, &block_number);
                }
                else if (!strcmp(command, "free")){
                    freeMem(ptr, arg1, &block_number);
                }
                else if (!strcmp(command, "blocklist")){
                    blocklist(ptr, &block_number);
                }
                else if (!strcmp(command, "writeheap")){
                    writeheap(ptr, &block_number, arg1, arg2, arg3);
                }
                else if (!strcmp(command, "printheap")){
                    printheap(ptr, &block_number, arg1, atoi(&arg2));
                }
                else if (!strcmp(command, "printheader")){
                    printheader(ptr, arg1, &block_number);
                }
                else if (!strcmp(command, "quit")){
                    break;
                }
                else {
                    printf("%s\n", "Invalid Command, try again");
                }
        }while (strcmp(command,"quit") != 0);

        free(ptr);
        return 0 ;

}
