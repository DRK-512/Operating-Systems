CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

all: generateKey stash

generateKey: 
	openssl enc -aes-256-cbc -pass file:password.txt -P -pbkdf2 -md sha256 -nosalt > key.bin
stash:
	$(CC) $(CFLAGS) -o stash src/functions.c src/main.c -lcrypto

clean:
	rm -f stash key.bin
