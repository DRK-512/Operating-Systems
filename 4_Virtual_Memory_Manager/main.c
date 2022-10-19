#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// this is a struct used to hold the page number, and other needed values
typedef struct charArray
{
	int pageNumber;	
	char values[16];
	int notUsedCycles;
}pages;

// these need to be global 
int pageTable[32];
pages ram[8];

/*
   Function Name: main
   Input to the method: There are none
   Output(Return value): There are none
   Brief description of the task: This will run LRU for the process and address files
*/
int main(int argc, char *argv[]){
	// define needed varibles
	char logical_address[4] = {0};
	int l_address_int = 0;
	int page_num = 0;
	int offset = 0;
	double pageFaults = 0;
	double addresses_requested = 0;
	char process_buffer[5] = {0};
	

	// page table starts with all invalids
	for(int i = 0; i < 32; i++){
		pageTable[i] = -1;
	}

    // Get the input file 
	FILE *in_file  = fopen(argv[1], "r"); // read only 
	

	// Check if file exists 
  	if (in_file == NULL) {   
        // The file does not exist
		printf("Error! Could not open file\n"); 
  		exit(-1); // must include stdlib.h in order to use this
    } else {
    	while(fgets(logical_address,7,in_file))
		{
			// set varibles correctly to use for the rest of the function
			addresses_requested++;
			l_address_int = atoi(logical_address);
			page_num = l_address_int / 16;
			offset = l_address_int % 16;

			// if the page number is avalible, then we can write to it
			if(pageTable[page_num] == -1){
				pageFaults++;
				int buffer = 0;
				FILE *process_file = fopen(argv[2],"r");

				// check if second file exists
				if (process_file == NULL){
					printf("Error! Could not open file\n"); 
  					exit(-1); // must include stdlib.h 
				}
				else{
					while(buffer < page_num * 16 - 1){
						fgets(process_buffer,7,process_file);
						buffer++;
					}
                    int buffer_stop = buffer + 15;
					pages pageValues;
                    pageValues.notUsedCycles = 0;
                    pageValues.pageNumber = page_num;
					char value[3] = {0};
					int page_index = 0;
					while(buffer <= buffer_stop){
						fgets(value,7,process_file);
                        pageValues.values[page_index] = value[0];
						page_index++;
						buffer++;
					}

					// close the process file since we no longer need it 
					fclose(process_file);
					int is_ram_full = 1;
					for(int i = 0; i < 8; i++){
						if(ram[i].values[0] == 0){
							ram[i] = pageValues;
							is_ram_full = 0;
							pageTable[page_num] = i;
                            break;
                        }
                    }

                    if (is_ram_full == 1){
                        int lruIndex = 0;
                        for(int i = 0; i < 8; i++){
                            if(ram[i].notUsedCycles > ram[lruIndex].notUsedCycles){
                                lruIndex = i;
                            }
                        }
                        int invalidPage = ram[lruIndex].pageNumber;
                        pageTable[invalidPage] = -1;
                        ram[lruIndex] = pageValues;
                        pageTable[page_num] = lruIndex;
						
                    }            
                } 
                // get and display the physical adddress, and others 
				int physicalAddress = page_num*pageTable[page_num] + offset;
            	char data = ram[pageTable[page_num]].values[offset];
            	printf("Logical Address: %d, Physical Address: %d, Data: %c \n", l_address_int,physicalAddress,data);	  
            }
            else{
            	// if the page number existed, the we display it 
                int ram_index = pageTable[page_num];
                int physicalAddress = page_num*ram_index + offset;
                char data = ram[ram_index].values[offset];
                printf("Logical Address: %d, Physical Address: %d, Data: %c \n", l_address_int,physicalAddress,data);


            }
            // check RAM for page number 
            for(int i = 0; i < 8; i++){
                if(ram[i].pageNumber != page_num){
                    ram[i].notUsedCycles++;
                }
            }
            
        }
    }
    // get the page fault rate once everything is complete
	double pageFaultRate = pageFaults/addresses_requested;
    printf("Page Fault Rate: %f\n", pageFaultRate);
    return 0;
} 
            