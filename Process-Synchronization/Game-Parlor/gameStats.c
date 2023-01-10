#include "gameStats.h"

/* 
 * This file is used to store misc functions and variables 
 * We only don't store numRounds here because the main function needs it
 */

int available = maxDice;
char rounds[maxDice] = {0}; 
struct gameStruct games[maxDice]; 
sem_t mutex, setupMutex, returnMutex;

char getNumDice(char amount){
    switch(amount){
        case 0: 
        case 1: 
            return 4;
            break;  
        case 2: 
        case 3: 
            return 5; 
            break; 
        case 4: 
        case 5: 
            return 2; 
            break; 
        case 6: 
        case 7: 
            return 1; 
            break; 
    }
}

char* getName(char amount){
    switch(amount){
        case 0: 
        case 1: 
            return "Backgammon";
            break;  
        case 2: 
        case 3: 
            return "Risk"; 
            break; 
        case 4: 
        case 5: 
            return "Monopoly"; 
            break; 
        case 6: 
        case 7: 
            return "Pictionary"; 
            break;    
    }
}

char* getRoundName(char amount){
    switch(amount){
        case 0: 
            return "first";
            break;  
        case 1: 
            return "second"; 
            break; 
        case 2: 
            return "third"; 
            break; 
        case 3: 
            return "fourth"; 
            break; 
        case 4: 
            return "fifth"; 
            break; 
        case 5: 
            return "sixth"; 
            break; 
        case 6: 
            return "seventh"; 
            break; 
        case 7: 
            return "eighth"; 
            break; 
        case 8: 
            return "ninth"; 
            break; 
        case 9: 
            return "tenth"; 
            break; 
        case 10: 
            return "eleventh"; 
            break; 
        case 11: 
            return "twelfth"; 
            break; 
        case 12: 
            return "thirteenth"; 
            break; 

        default: 
            /* 
             * After 13, numbers follow a nice algorithm
             * I would be surprised if someone did more than 13 rounds anyways
             * NOTE: this is a char so its max is 255 
             */
            amount++; 
            char str[5];
            char* returnStr; 
            if((amount-1)%10==0) {
                //1st
                sprintf(str, "%d", amount);
                strcat(str, "st"); 
                returnStr = str; 
                return returnStr;  
            } else if((amount-2)%10==0) {
                //2nd
                sprintf(str, "%d", amount);
                strcat(str, "nd"); 
                returnStr = str; 
                return returnStr;  
            } else if((amount-3)%10==0) {
                //3rd
                sprintf(str, "%d", amount);
                strcat(str, "rd"); 
                returnStr = str; 
                return returnStr;  
            } else {
                //4th - 10th
                sprintf(str, "%d", amount);
                strcat(str, "th"); 
                returnStr = str; 
                return returnStr;  
            }
            break; 
    }
}
