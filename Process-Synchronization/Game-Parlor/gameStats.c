#include "gameStats.h"

/* 
    This file is used to store misc functions and variables 
*/

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

char* getRound(char amount){
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
    }
}
