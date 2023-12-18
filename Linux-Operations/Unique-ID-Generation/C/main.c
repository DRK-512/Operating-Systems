#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <math.h>

#define IDSize 32

void uuid_form(char* output_uuid, char* input_string)
{
    // Now I will put the machine-id into the form of a uuid
    int i, count=0;
    for(i=0; i<8; i++){
        output_uuid[i]=input_string[i]; 
    }
    count+=8; 
    output_uuid[count]='-'; 
    for(i=count; i<count+4; i++){
        output_uuid[i+1]=input_string[i]; 
    }
    count+=5; 
    output_uuid[count]='-'; 
    for(i=count; i<count+4; i++){
        output_uuid[i+1]=input_string[i-1]; 
    }
    count+=5; 
    output_uuid[count]='-'; 
    for(i=count; i<count+4; i++){
        output_uuid[i+1]=input_string[i-2]; 
    }
    count+=5; 
    output_uuid[count]='-';
    for(i=count; i<count+11; i++){
        output_uuid[i+1]=input_string[i-3]; 
    }
}

// My goal with this is to create a random number seed from the machine-ID
unsigned int createSeedFromID(char* input_ID)
{
    unsigned int seed_gen=0; 
    int i=0, j=1; 
    while(1){
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
        if(i>31)
            break; 
    }
}


int main() 
{
    FILE *idFile = fopen("/etc/machine-id", "r"); 
    if(idFile==NULL){
        printf("The /etc/machine-id file is required for this project"); 
        return -1; 
    }
    
    char* idToCrypt = (char *)calloc(37, sizeof(char)); 
  
    fgets(idToCrypt, 32, idFile);  
    fclose(idFile);

    printf("My machine ID: %s\n", idToCrypt);

    srand(createSeedFromID(idToCrypt)); 
    printf("Randomly Generated Clear-Text ID: ");
    int randomNum=0;
    int i=0;  
    while(i<IDSize) {
        randomNum = rand();
        randomNum = 33 + randomNum % (122 - 48 + 1);
        if((randomNum>47 && randomNum<58) || 
            (randomNum>64 && randomNum<91) ||
            (randomNum>96 && randomNum<123)) {
             
            if(i%4==0 && i>1){
                printf("-");
            }
            printf("%c", randomNum); 
            i++;
        }
    }
    free(idToCrypt); 
    return 0;
}

