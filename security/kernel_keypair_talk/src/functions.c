#include "../include/functions.h"

int initVals(int argc, char *argv[], key_t* m_key, unsigned char* m_aeskey) {
	int opt;
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
			printf("%c Invalid option.\n", opt);
			return 1;
		}
	}

	// Grab the content & allocate mem for it
	key_file = fopen(KEY_FILE, "r");
	if (!key_file) {
		printf("Error opening key file");
		return 1; 
	}

	fread(m_aeskey, 1, AES_BLOCK_SIZE * 8, key_file);
	// The key will just be the ftok from the key.bin file
	*m_key = ftok(KEY_FILE, 'R');  

	return 0; 
}

int readMem(EVP_CIPHER_CTX * m_ctx, char* m_stash, unsigned char* m_aeskey) {

	int len = strlen(m_stash);
	int plaintext_len = 0;
	unsigned char plaintext[len + EVP_CIPHER_block_size(EVP_aes_256_cbc())]; // Adjust size for ciphertext

	// Init / Reinit enveloped data decryption context with the specified key
	if (!EVP_DecryptInit_ex(m_ctx, EVP_aes_256_cbc(), NULL, m_aeskey, NULL)) {
		perror("EVP_DecryptInit_ex");
		return 1;
	}

	// Now we decrypt to the output buffer
	if (!EVP_DecryptUpdate(m_ctx, plaintext, &len, (unsigned char *)m_stash, len)) {
		perror("EVP_DecryptUpdate");
		return 1;
	}

	plaintext_len = len;
	// If a write was not done first, we will be here
	if (!EVP_DecryptFinal_ex(m_ctx, plaintext + len, &len)) { 
		printf("Nothing has been writen to this space in memory yet\n"); 
		return 0; 
	}

	// This is the stuff being read, but it needs the null-terminator
	plaintext_len += len;
	plaintext[plaintext_len] = '\0'; 

	printf("Decrypted data: %s\n", plaintext);
	return 0;
}

int writeMem(EVP_CIPHER_CTX * m_ctx, char* m_stash, unsigned char* m_aeskey) {
	printf("Enter data to write to stash: ");
	scanf("%s", m_stash);

	// Encrypt data using key
	int len = strlen(m_stash);
	int ciphertext_len = 0;
	unsigned char ciphertext[len + EVP_CIPHER_block_size(EVP_aes_256_cbc())]; // Adjust size for ciphertext


	if (!EVP_EncryptInit_ex(m_ctx, EVP_aes_256_cbc(), NULL, m_aeskey, NULL)) {
		perror("EVP_EncryptInit_ex");
		return 1;
	}

	if (!EVP_EncryptUpdate(m_ctx, ciphertext, &len, (unsigned char *)m_stash, len)) {
		perror("EVP_EncryptUpdate");
		return 1;
	}

	ciphertext_len = len;

	if (!EVP_EncryptFinal_ex(m_ctx, ciphertext + len, &len)) {
		perror("EVP_EncryptFinal_ex");
		return 1; 
	}

	ciphertext_len += len;

	memcpy(m_stash, ciphertext, ciphertext_len);
	return 0;
}
