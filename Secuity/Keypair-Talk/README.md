# KeyPair Talk
- Create a way for two Linux processes to communicate via a key:value pair “stash" stored in kernel space. 
- Any process that knows the key should be able to read/write the value.

## How the Code Works
There is a password.txt file in this directory, and this is where I set the password that will be used to encrypt 
- Feel free to change the password.txt for your application; remember all of this is MIT, so anyone can use it
First, we need to generate a key from this password, and I save mine to a key.bin file
```bash
$ make generateKey
```
Now if you look inside the key.bin, it should look something like this: 
```bash
key=82F69EB3AE909B5F6FD30A8C822FE7524466765FA4A04EB8FD1250E2D7DBB61E
iv =A4636A7EBFB217224908967477F7DC87
```
Now that the password.txt and key.bin exist, we can build and run the application.

## Compiling the Code
I have been using make for everything, so this should not be new
```bash
$ make stash
$ ./stash
```
The stash file needs to point to an aes key, so if you have one, don't run the generate key command; just copy it here

## Running the Code
To demo this, I would open three terminals, one that sits here and two that sit in some other directory.
```bash
# Terminal 1
$ ./stash 
Enter 'read', 'write', or 'exit': write
Enter data to write to stash: some-String-without-spaces
```
```bash
# Terminal 2
$ mkdir bin
$ cd bin
$ cp ../stash .
$ ./stash -k ../key.bin
Enter 'read', 'write', or 'exit': read
Decrypted data: some-String-without-spaces
```
```bash
# Terminal 3
$ cd bin
$ echo "someDifferentPassword" > password.txt
$ openssl enc -aes-256-cbc -pass file:password.txt -P -pbkdf2 -md sha256 -nosalt > new-key.bin # can be seen in Makefile
$ ./stash -k ./new-key.bin
Enter 'read', 'write', or 'exit': read
Nothing has been writen to this space in memory yet
```

## Explanation
As you can see, we created a space in memory depending on the value of the key, and we wrote to it
From said space, we can read from it as well
The thing is, in order to read from said space, you need to have the correct key as you can see