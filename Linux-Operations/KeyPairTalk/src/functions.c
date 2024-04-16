#include "../include/functions.h"

int initVals(int argc, char *argv[], key_t* KEY, unsigned char* AES_KEY) {
	int opt;
	long fileSize;
	char *KEY_FILE = "key.bin";
	
	FILE *key_file;

	// Parse command-line options
	const struct option long_options[] = {
		{"key", required_argument, NULL, 'k'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
	while ((opt = getopt_long(argc, argv, "k:h", long_options, NULL)) != -1) {
		switch (opt) {
		case 'k':
			KEY_FILE = strdup(optarg);
			break;
		case 'h':
			printf("Usage: %s [-k|--key <key_file>] [-h|--help]\n", argv[0]);
			return 0;
		default:
			fprintf(stderr, "Invalid option.\n");
			return 1;
		}
	}

	// Grab the content & allocate mem for it
	key_file = fopen(KEY_FILE, "r");
	if (!key_file) {
		perror("Error opening key file");
		exit(EXIT_FAILURE);
	}

	
	fread(AES_KEY, 1, AES_BLOCK_SIZE * 8, key_file);

	fseek(key_file, 0, SEEK_END);
	fileSize = ftell(key_file);
	rewind(key_file);

	//char *keyBuffer = (char *)malloc(fileSize * sizeof(char));
	if (AES_KEY == NULL) {
		fprintf(stderr, "Error allocating memory.\n");
		fclose(key_file);
		return 1;
	}

	// Put content of file into buffer
	fread(AES_KEY, sizeof(char), fileSize, key_file);

	// Close the file
	fclose(key_file);
	// grab the entire value of the key
	char *keyStart = strstr((char*)AES_KEY, "key=");
	if (keyStart != NULL) {
		keyStart += strlen("key="); 
		char *keyEnd = strchr(keyStart, '\n'); 
		if (keyEnd != NULL) {
			strncpy((char*)AES_KEY, keyStart, keyEnd - keyStart); // Copy the key value to the key buffer
			AES_KEY[keyEnd - keyStart] = '\0'; // every string must end with \0
		} else {
			printf("Invalid key format: no newline found.\n");
		}
	} else {
		printf("Key not found in the buffer.\n");
		exit(EXIT_FAILURE); 
	}

	// Instead of using the src/stash.c file for kernel mem space
	// I use the key.bin file since
	char substring[17];  // I am converting the keyBuffer to a long

	// Copy the first 16 characters (8 bytes) into the substring
	strncpy(substring, (char*)AES_KEY, 16);
	substring[16] = '\0';  

	*KEY = strtol(substring, NULL, 16);
	return EXIT_SUCCESS; 
}
