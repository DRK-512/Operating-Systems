#include "../include/functions.h"

// checkRAM()
// processing? 
// pageNumCheck()
// pageFaultRate

// this is a struct used to hold the page number, and other needed values
typedef struct charArray {
	int pageNumber;	
	char values[16];
	int notUsedCycles;
} pages;

// these need to be global 
int pageTable[32];
pages ram[8];
FILE *addressFile, *processFile; 

int initVars(int argc, char *argv[]) {
	int i; 

	for (i=1; i<argc; i++) {
		if ((!strcmp(argv[i], "-a") || !strcmp(argv[i], "--address")) && argv[i+1] != NULL) {
			addressFile = fopen(argv[i+1], "r");  
		}
		else if ((!strcmp(argv[i], "-p") || !strcmp(argv[i], "--process")) && argv[i+1] != NULL) {
			processFile = fopen(argv[i+1], "r");
		} 
		else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			printf("The purpose for this program can be found in the readme\n");
			printf("Please provide an address.txt and a process.txt file as such:\n");
			printf("./Virt-Mem-Manager -a ./input/addresses.txt -p ./input/process.txt");   
			return 1; 
		}
	}

	if ((addressFile == NULL) || (processFile == NULL)) { 
		if (addressFile == NULL) { 
			printf("ERROR: NO ADDRESS FILE PROVIDED\n"); 
		}
		if (processFile == NULL) { 
			printf("ERROR: NO PROCESS FILE PROVIDED\n"); 
		}
		printf("Please refer to -h | --help or the README\n"); 
		return 1;
	}

	// init all to -1
	memset(pageTable, -1, sizeof(pageTable));
	fclose(addressFile); 
	fclose(processFile); 
	
	return 0; 

}

void process(){
	int l_address_int = 0;
	int page_num = 0;
	int offset = 0;
	double pageFaults = 0;
	double addresses_requested = 0;
	// FIX: check if it needs to be dynamic, it seems everything is a static size
	char* process_buffer = (char*)calloc(7, sizeof(char)); 
	char* logical_address = (char*)calloc(7, sizeof(char));

	while (fgets(logical_address, 7, addressFile)) {

		// set varibles to use for the rest of the function
		addresses_requested++;
		l_address_int = atoi(logical_address);
		page_num = l_address_int / 16;
		offset = l_address_int % 16;

		// if the page number is avalible, then we can write to it
		if (pageTable[page_num] == -1) {

			pageFaults++;
			int buffer = 0;

			while(buffer < page_num * 16 - 1) {
				fgets(process_buffer,7,processFile);
				buffer++;
			}

			// FIX: all definintions should be at top
			int buffer_stop = buffer + 15;
			pages pageValues;
			pageValues.notUsedCycles = 0;
			pageValues.pageNumber = page_num;
			char* value = (char*)calloc(7, sizeof(char)); 
			int page_index = 0;

			while(buffer <= buffer_stop) {
				fgets(value,7,processFile);
				pageValues.values[page_index] = value[0];
				page_index++;
				buffer++;
			}
			// no longer using this
			free(value); 
			
			// check if our ram is full
			int is_ram_full = 1;
			for (int i = 0; i < 8; i++) {
				if (ram[i].values[0] == 0) {
					ram[i] = pageValues;
					is_ram_full = 0;
					pageTable[page_num] = i;
					break;
				}
			}
			
			if (is_ram_full == 1) {

				int lruIndex = 0;
				for (int i = 0; i < 8; i++) {
					if (ram[i].notUsedCycles > ram[lruIndex].notUsedCycles)
						lruIndex = i;	
				}

				int invalidPage = ram[lruIndex].pageNumber;
				pageTable[invalidPage] = -1;
				ram[lruIndex] = pageValues;
				pageTable[page_num] = lruIndex;
					
			}

			// get and display the physical adddress, and others 
			int physicalAddress = page_num*pageTable[page_num] + offset;
			char data = ram[pageTable[page_num]].values[offset];
			printf("Logical Address: %d, Physical Address: %d, Data: %c \n", l_address_int,physicalAddress,data);	  

		} else {

			// if the page number existed, the we display it 
			int ram_index = pageTable[page_num];
			int physicalAddress = page_num*ram_index + offset;
			char data = ram[ram_index].values[offset];
			printf("Logical Address: %d, Physical Address: %d, Data: %c \n", l_address_int,physicalAddress,data);
		}

		// check RAM for page number 
		for(int i = 0; i < 8; i++) {
			if (ram[i].pageNumber != page_num) 
				ram[i].notUsedCycles++;
			
		} 
	}
	

	// get the page fault rate once everything is complete
	double pageFaultRate = pageFaults/addresses_requested;
	printf("Page Fault Rate: %f\n", pageFaultRate);

	// free pointers
	free(logical_address); 
	free(process_buffer);
	fclose(processFile); 
	fclose(addressFile); 
}