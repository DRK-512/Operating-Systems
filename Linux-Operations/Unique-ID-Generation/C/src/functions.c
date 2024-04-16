#include "../include/functions.h"

int initParams(int argc, char* argv[], int* idSize) {
	int opt; 
	// Parse command-line options
	const struct option long_options[] = {
		{"size", required_argument, NULL, 's'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
	while ((opt = getopt_long(argc, argv, "s:h", long_options, NULL)) != -1) {
		switch (opt) {
		case 's':
			*idSize = atoi(optarg);
			break;
		case 'h':
			printf("Usage: %s [-k|--key <key_file>] [-h|--help]\n", argv[0]);
			return 0;
		default:
			printf("%c Invalid option.\n", opt);
			return 1;
		}
	}
	return 0; 
}

int assignID(char** m_cryptID) {
	FILE *idFile = fopen("/etc/machine-id", "r");
	FILE *infoFile = fopen("/etc/machine-info", "r"); 
	char content1[100], content2[100];

	if (idFile==NULL || infoFile==NULL) {
		printf("ERROR: Could not find /etc/machine-info or /etc/machine-id"); 
		return 1; 
	}

	fgets(content1, sizeof(content1), idFile);
	fgets(content2, sizeof(content2), infoFile);
	if (content1[strlen(content1) - 1] == '\n') {
		content1[strlen(content1) - 1] = '\0'; // Remove the newline character
	}

	if (content2[strlen(content2) - 1] == '\n') {
		content2[strlen(content2) - 1] = '\0'; // Remove the newline character
	}
	fclose(idFile);
    	fclose(infoFile);

	*m_cryptID = malloc(strlen(content1) + strlen(content2) + 1); // +1 for the null terminator
	strcpy(*m_cryptID, content1);
	strcat(*m_cryptID, content2);
	return 0; 
} 

// My goal with this is to create a random number seed from the machine-ID
unsigned int createSeedFromID(char* input_ID) {
	unsigned int seed_gen=0; 
	int i=0, j=1; 
	while (1) {
		if (input_ID[i]>47 && input_ID[i]<58) {
			if (seed_gen==0) {
				seed_gen=input_ID[i] - '0';
			} else {
				seed_gen = seed_gen + ((input_ID[i] - '0') * (pow(10,j))); 
				j++; 
			if(j>9)
				break; 
			}
		}
		i++;
		if (i>31)
			break; 
	}
	return seed_gen; 
}

void uuid_form(char* output_uuid, char* input_string) {
	// Now I will put the machine-id into the form of a uuid
	int i, count=0;
	for (i=0; i<8; i++)
		output_uuid[i]=input_string[i]; 
		
	count+=8; 
	output_uuid[count]='-'; 
	for (i=count; i<count+4; i++)
		output_uuid[i+1]=input_string[i]; 
	
	count+=5; 
	output_uuid[count]='-'; 
	for (i=count; i<count+4; i++)
		output_uuid[i+1]=input_string[i-1]; 
	
	count+=5; 
	output_uuid[count]='-'; 
	for (i=count; i<count+4; i++)
		output_uuid[i+1]=input_string[i-2]; 

	count+=5; 
	output_uuid[count]='-';
	for (i=count; i<count+11; i++)
		output_uuid[i+1]=input_string[i-3]; 

}
