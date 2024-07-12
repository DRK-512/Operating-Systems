#include "../include/functions.h"

int main(int argc, char *argv[]) {
	int shmid;
	unsigned char aes_key[AES_BLOCK_SIZE * 8];  // AES_KEY_SIZE = 256 bits = 32 bytes

	key_t key;
	char* stash=NULL;
	if(initVals(argc, argv, &key, aes_key)){
		printf("ERROR: Could Not Initialize"); 
		exit(EXIT_FAILURE); 
	}

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
		printf("Enter 'read' 'write' or 'exit': ");
		char input[10];
		scanf("%s", input);

		if (strcmp(input, "read") == 0) {
			if(readMem(ctx, stash, aes_key)) {
				exit(EXIT_FAILURE); 
			}
		} else if (strcmp(input, "write") == 0) {
			if(writeMem(ctx, stash, aes_key)) {
				exit(EXIT_FAILURE); 
			}
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
