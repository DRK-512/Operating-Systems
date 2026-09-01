#ifndef functions
#define functions

#include <stdio.h>
#include <stdlib.h>      // malloc | EXIT_FAILURE
#include <unistd.h>      // key_t
#include <string.h>      // strlen | strstr
#include <sys/shm.h>     // shmget | shmat
#include <openssl/aes.h> // AES_BLOCK_SIZE
#include <openssl/evp.h> // EVP_CIPHER_CTX
#include <getopt.h>      // getopt | long_options | required_argumnents

int initVals(int argc, char *argv[], key_t* m_key, unsigned char* m_aeskey); 
int readMem(EVP_CIPHER_CTX * m_ctx, char* m_stash, unsigned char* m_aeskey); 
int writeMem(EVP_CIPHER_CTX * m_ctx, char* m_stash, unsigned char* m_aeskey); 

#endif