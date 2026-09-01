# TPM File Encryption
Create a secure file encryption and decryption system that leverages the TPM to securely store and manage encryption keys. 
Here’s a broad outline of the project:
- Initialize the TPM 
- Derive and store a persistent encryption key in the TPM.
- Write a function to encrypt files using AES.
	- The AES key should be encrypted with the TPM-stored key before being stored on disk.
- Write a function to decrypt files.
	- Retrieve and decrypt the AES key using the TPM-stored key.
- Create a command-Line interface
- Provide options for specifying files and managing keys.

The key I decided to utilize is the storage key of my device because it is the key used to provide integrity and confidentiality protection for descendant keys that are stored off of the TPM
Basically I will create a storage key then use said key to encrypt and decrypt files
