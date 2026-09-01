#include "functions.h"

// Purpose: parses the args and sorts them into the arguments struct
arguments parse_args(int argc, char *argv[]) {
        arguments args = {.address_path = NULL, .process_path = NULL, .help = 0};

        // NOTE: argv[0] is always the name of the executable
        for (int i = 1; i < argc; i++) {
                if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
                        args.help = 1;
                } else if (i + 1 < argc) {
                        if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--address")) {
                                args.address_path = argv[++i];
                        } else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--process")) {
                                args.process_path = argv[++i];
                        }
                }
        }

        return args;
}

// Purpose: Ensure the arguments from parse args are valid & user didnt call the help function
uint8_t check_args(arguments args) {
        // If user invokes help, I assume they dont want to run the program and just want to learn about it
        if (args.help) {
                printf("The purpose for this program can be found in the readme\n");
                printf("Please provide an address.txt and a process.txt file as such:\n");
                printf("%s -a ./input/addresses.txt -p ./input/process.txt\n", argv[0]);
                return 1;
        }
        // Check if files were even provided (though this should never invoke technically)
        if (args.address_path == NULL) {
                printf("ERROR: NO ADDRESS FILE PROVIDED\n");
                return 1;
        }

        if (args.process_path == NULL) {
                printf("ERROR: NO PROCESS FILE PROVIDED\n");
                return 1;
        }

        // Check if files are empty
        FILE *address_file = fopen(args.address_path, "r");
        if (address_file == NULL) {
                fclose(address_file);
                printf("ERROR: COULD NOT OPEN ADDRESS FILE\n");
                return 1;
        }

        FILE *process_file = fopen(args.process_path, "r");
        if (process_file == NULL) {
                fclose(address_file);
                fclose(process_file);
                printf("ERROR: COULD NOT OPEN PROCESS FILE\n");
                return 1;
        }
        fclose(address_file);
        fclose(process_file);
        return 0;
}

// This will do the virtual mem management
void main_loop(const char address_path, const char process_path) {
        // Open the files right away
        FILE *address_file = fopen(address_path, "r");
        FILE *process_file = fopen(process_path, "r");

        int    log_addr_int       = 0;
        int    page_num            = 0;
        int    offset              = 0;
        int    buffer              = 0;
        double page_faults         = 0;
        double addresses_requested = 0;

        // Utilize calloc because I want to start with the mem being 0
        char *process_buffer  = (char *)calloc(7, sizeof(char));
        char *logical_address = (char *)calloc(7, sizeof(char));

        // Initially, all entries in the page table will be invalid since there are no pages on the RAM.
        memset(page_table, -1, sizeof(page_table));

        // Iterate through the address file since:
        // The program will read the logical address from addresses.txt,
        // find the page number, bring in the page from process.txt,
        // and put it on RAM and then read the data at the address.
        while (fgets(logical_address, 7, address_file)) {
                // I have to convert the address since it reads in as a string
                log_addr_int = atoi(logical_address);

                // I need address total so I can divide by it in the end
                addresses_requested++;

                // page number is logical address, then we
                page_num      = log_addr_int / 16;
                offset        = log_addr_int % 16;

                // If the page number is avalible, then we can write to it
                if (page_table[page_num] == -1) {
                        // 
                        page_faults++;
                        buffer = 0;

                        while (buffer < page_num * 7 - 1) {
                                fgets(process_buffer, 7, processFile);
                                buffer++;
                        }

                        // FIX: all definintions should be at top
                        int   buffer_stop = buffer + 15;
                        pages pageValues;
                        pageValues.notUsedCycles = 0;
                        pageValues.pageNumber    = page_num;
                        char *value      = (char *)calloc(7, sizeof(char));
                        int   page_index = 0;

                        while (buffer <= buffer_stop) {
                                fgets(value, 7, processFile);
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
                                        ram[i]              = pageValues;
                                        is_ram_full         = 0;
                                        pageTable[page_num] = i;
                                        break;
                                }
                        }

                        if (is_ram_full == 1) {

                                int lruIndex = 0;
                                for (int i = 0; i < 8; i++) {
                                        if (ram[i].notUsedCycles
                                            > ram[lruIndex].notUsedCycles)
                                                lruIndex = i;
                                }

                                int invalidPage = ram[lruIndex].pageNumber;
                                pageTable[invalidPage] = -1;
                                ram[lruIndex]          = pageValues;
                                pageTable[page_num]    = lruIndex;
                        }

                        // get and display the physical adddress, and others
                        int physicalAddress
                            = page_num * pageTable[page_num] + offset;
                        char data = ram[pageTable[page_num]].values[offset];
                        printf("Logical Address: %d, Physical Address: %d, "
                               "Data: %c \n",
                               log_addr_int, physicalAddress, data);

                } else {

                        // if the page number existed, the we display it
                        int  ram_index       = pageTable[page_num];
                        int  physicalAddress = page_num * ram_index + offset;
                        char data            = ram[ram_index].values[offset];
                        printf("Logical Address: %d, Physical Address: %d, "
                               "Data: %c \n",
                               log_addr_int, physicalAddress, data);
                }

                // check RAM for page number
                for (int i = 0; i < 8; i++) {
                        if (ram[i].pageNumber != page_num)
                                ram[i].notUsedCycles++;
                }
        }

        // get the page fault rate once everything is complete
        double pageFaultRate = pageFaults / addresses_requested;
        printf("Page Fault Rate: %f\n", pageFaultRate);

        // free pointers
        free(logical_address);
        free(process_buffer);
        fclose(processFile);
        fclose(addressFile);
}
