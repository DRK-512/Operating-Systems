# System Info Unique ID
- This is the simple, and easy approach to the problem
- Essentially you collect a large amount of system information, then generate a hash from it and use that as the ID
- Personally I would also want to use a math function of my own so it is not a common hash
- I can do this in Bash which is the easy way out, or do it in C and learn how to gather this information using C
## System Information Used for Bash script
- The system information I used were: 
	- KERNEL_INFO
	- CPU_INFO
	- RAM_COUNT
	- HOSTNAME
	- SYSTEM_UUID
	- BIOS_INFO
- With the system information fetched in bash, I then hashed it with sha256
## In C things were a bit different
- I first wanted to create a hash based on the key fetched from the endorsement key on the TPM since every TPM should have a different endorsement key
- Then I thought of doing it based off MAC address, but every linux laptop uses something different for interfaces (eth0 or enp0s1) so that was not an option either 
- I did find that every linux machine already has its own machine-id file, so I grabbed that, ran an sha256 hash on it and got ?r?, which is not good enough to be an ID
- So I decided to dive into uuid generation based off name
- in bash the command I wanted to replicate looked like this: 
```bash
uuidgen -N hand -n @x500 --sha1
```
- This created a name based UUID hashed with sha1