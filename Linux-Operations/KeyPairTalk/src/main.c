#include <stdio.h>
#include <stdlib.h> // malloc
#include <unistd.h> // key_t
#include <string.h> // strlen | strstr
#include <sys/shm.h> // shmget | shmat
#include <openssl/aes.h> // AES_BLOCK_SIZE
#include <openssl/evp.h> // EVP_CIPHER_CTX
#include <getopt.h> // getopt | long_options | required_argumnents

int main(int argc, char *argv[]) {
	int shmid, opt;
	long fileSize;
	char *KEY_FILE = "key.bin";
	
	FILE *key_file;
	key_t key;
	char* stash=NULL;

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

	unsigned char aes_key[AES_BLOCK_SIZE * 8];  // AES_KEY_SIZE = 256 bits = 32 bytes
	fread(aes_key, 1, AES_BLOCK_SIZE * 8, key_file);

	fseek(key_file, 0, SEEK_END);
	fileSize = ftell(key_file);
	rewind(key_file);

	char *keyBuffer = (char *)malloc(fileSize * sizeof(char));
	if (keyBuffer == NULL) {
		fprintf(stderr, "Error allocating memory.\n");
		fclose(key_file);
		return 1;
	}

	// Put content of file into buffer
	fread(keyBuffer, sizeof(char), fileSize, key_file);

	// Close the file
	fclose(key_file);
	// grab the entire value of the key
	char *keyStart = strstr(keyBuffer, "key=");
	if (keyStart != NULL) {
		keyStart += strlen("key="); 
		char *keyEnd = strchr(keyStart, '\n'); 
		if (keyEnd != NULL) {
			strncpy(keyBuffer, keyStart, keyEnd - keyStart); // Copy the key value to the key buffer
			keyBuffer[keyEnd - keyStart] = '\0'; // every string must end with \0
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
	strncpy(substring, keyBuffer, 16);
	substring[16] = '\0';  

	key = strtol(substring, NULL, 16);

	// Create the shared memory segment that will be in kernel space, then attach the memory segment
	stash = (char *)malloc(1024 * sizeof(char));

	if ((shmid = shmget(key, sizeof(char), IPC_CREAT | 0666)) == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	stash = (char*)shmat(shmid, NULL, 0); 
	if (stash == (char*)-1) {
		perror("ERROR: shmat error");
		exit(EXIT_FAILURE);
	} 

	// Initialize OpenSSL
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	if (!ctx) {
		perror("EVP_CIPHER_CTX_new");
		exit(EXIT_FAILURE);
	}

    while (1) {
        // Read input from user
        printf("Enter 'read', 'write', or 'exit': ");
        char input[10];
        scanf("%s", input);

        // Perform read operation
        if (strcmp(input, "read") == 0) {
            // Decrypt data using key
            int len = strlen(stash);
            int plaintext_len = 0;
            unsigned char plaintext[sizeof(key)];

            if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, NULL)) {
                perror("EVP_DecryptInit_ex");
                exit(EXIT_FAILURE);
            }

            if (!EVP_DecryptUpdate(ctx, plaintext, &len, (unsigned char *)stash, len)) {
                perror("EVP_DecryptUpdate");
                exit(EXIT_FAILURE);
            }

            plaintext_len = len;

            if (!EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) { 
		printf("Nothing has been writen to this space in memory yet\n"); 
                continue; 
            }

            plaintext_len += len;
            plaintext[plaintext_len] = '\0'; // Null-terminate the string

            printf("Decrypted data: %s\n", plaintext);
        }
        // Perform write operation
        else if (strcmp(input, "write") == 0) {
            printf("Enter data to write to stash: ");
            scanf("%s", stash);

            // Encrypt data using key
            int len = strlen(stash);
            int ciphertext_len = 0;
            unsigned char ciphertext[sizeof(key)];

            if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, NULL)) {
                perror("EVP_EncryptInit_ex");
                exit(EXIT_FAILURE);
            }

            if (!EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char *)stash, len)) {
                perror("EVP_EncryptUpdate");
                exit(EXIT_FAILURE);
            }

            ciphertext_len = len;

            if (!EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
                perror("EVP_EncryptFinal_ex");
                exit(EXIT_FAILURE);
            }

            ciphertext_len += len;

            memcpy(stash, ciphertext, ciphertext_len);
        } else if (strcmp(input, "exit") == 0) {
            break;
        }
    }

    // Detach from shared memory
    if (shmdt(stash) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}
