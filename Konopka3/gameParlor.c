

#include "gameParlor.h"

int repeatFix = 1; // to fix repeated front desks

// arrays used for taking/returning dice
int returnDice[9] = {AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE}; 
int requestDice[9] = {AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE}; 

// counts how many games each group has played
int gameNumCounter[9] = {0,0,0,0,0,0,0,0,0}; 
int groupsLeft = 8; 

int sleepToggle=0;


/*  Function Name: frontDesk
    Input to the method: frontDeskID = which game group this is 
    Output(Return value): none
    Brief description of the task: this is the thread created for each group of games                         
*/ 
void frontDesk(){

    int diceAvalible = 8; 
   
    // when you run out of games, the while loop stops
    while(checkGame()){

        // return dice back to front desk when a game ends
        for(int i=0; i<8; i++){
            if(returnDice[i]!=AVAILABLE){
                diceAvalible = diceAvalible+returnDice[i]; 
                returnDice[i] = AVAILABLE; 
            }
        }
        // sometimes the semaphores can finish while in that for loop, so this is a safety
        if(diceAvalible>8){diceAvalible==8;}

        // display the number of dice we have avalible
        if(repeatFix){
            printf("Front Desk: I have %d dice available\n", diceAvalible);
            repeatFix=0; 
        }  

        if(sleepToggle){
            sleep(1); 
        }
        // if we have dice to work with, we will 
        if(diceAvalible>0){ 
            // loop through each group to see if we can play some games
            for(int i=0; i<8; i++){
                if(requestDice[i]!=AVAILABLE){
                    if(diceAvalible>=requestDice[i]){
                        switch(requestDice[i]){
                            case 4:
                                sem_post(&Backgammon);
                                diceAvalible=diceAvalible-4; 
                                requestDice[i] = AVAILABLE; 
                                break; 
                            
                            case 5:
                                sem_post(&Risk);
                                diceAvalible=diceAvalible-5; 
                                requestDice[i] = AVAILABLE; 
                                break; 
                            
                            case 2:
                                
                                sem_post(&Monopoly);
                                diceAvalible=diceAvalible-2; 
                                requestDice[i] = AVAILABLE; 
                                break; 
                            
                            case 1:
                                
                                sem_post(&Pictionary);
                                diceAvalible=diceAvalible-1;  
                                requestDice[i] = AVAILABLE; 
                                break;
                        }
                    }
                } 
            }
        }
    }
    pthread_exit(gameParlor); // this code returns to the corresponding pthread_join issued in main()
}

/*  Function Name: gameParlor
    Input to the method: gameGroupID = which game group this is 
    Output(Return value): none
    Brief description of the task: this is the thread/thread handler created for each group of games                    
*/ 
void* gameParlor(void* gameID) {
    
    // convert input to int 
    int* groupID = gameID;
    int gameGroupID = *groupID;
    // if ID = -1, run front desk 
    if(gameGroupID==-1){
        frontDesk(); 
    } else {
    // Here we will request the dice 
    for(int games=0; games<5; games++){
        int retVal = gameGroupID+1; 
        switch(gameGroupID){
            case 0:
            case 1:
                requestDice[gameGroupID]=4;
                repeatFix=1; 
                // this is backgammon
                printf("Group %d: Requesting 4 dice for Backgammon.\n", retVal);
                
                
                sem_trywait(&Backgammon);
                printMesssage(1, gameGroupID);

                // now we wait because we are playing the game
                if(sleepToggle){
                    sleep(1); 
                }
                printMesssage(0, gameGroupID);
                // since we are done with the game, we can incement the number of games
                gameNumCounter[gameGroupID]++; 
                // now we put dice back into queue
                returnDice[gameGroupID]=4; 
                break; 

            case 2:
            case 3:
                requestDice[gameGroupID]=5; 
                repeatFix=1; 
                // this is risk
                printf("Group %d: Requesting 5 dice for Risk.\n", retVal);
                

                sem_trywait(&Risk);
                printMesssage(1, gameGroupID);

                // now we wait because we are playing the game
                if(sleepToggle){
                    sleep(1); 
                } 
                printMesssage(0, gameGroupID);
                // since we are done with the game, we can incement num games
                gameNumCounter[gameGroupID]++; 
                // now we put dice back into queue
                returnDice[gameGroupID]=5; 
                break; 

            case 4:
            case 5:
                requestDice[gameGroupID]=2; 
                repeatFix=1; 
                // this is monopoly
                printf("Group %d: Requesting 2 dice for Monopoly.\n", retVal);
                
                sem_trywait(&Monopoly);
                printMesssage(1, gameGroupID);

                // now we wait because we are playing the game
                if(sleepToggle){
                    sleep(1); 
                }
                printMesssage(0, gameGroupID);
                // since we are done with the game, we can incement num games
                gameNumCounter[gameGroupID]++; 
                 // now we put dice back into queue
                returnDice[gameGroupID]=2; 
                break; 

            case 6:
            case 7:
                requestDice[gameGroupID]=1; 
                repeatFix=1; 
                // this is pictionary 
                printf("Group %d: Requesting 1 die for Pictionary.\n", retVal);
               
                sem_trywait(&Pictionary);
                printMesssage(1, gameGroupID);

                // now we wait because we are playing the game
                if(sleepToggle){
                    sleep(1); 
                }
                printMesssage(0, gameGroupID);
                // since we are done with the game, we can incement num games
                gameNumCounter[gameGroupID]++; 
                // now we put dice back into queue
                returnDice[gameGroupID]=1; 
                break;
            }
        }
    }
}

/*  Function Name: checkGame
    Input to the method: none
    Output(Return value): none
    Brief description of the task: this will check if we can still play games                      
*/ 
int checkGame(){
    for(int i=0; i<8;i++){
        if(gameNumCounter[i]<5){
            return 1; 
        } 
    }
    printf("\nAll the groups have played their games!\n"); 
    exit(0); // since everything has completed we can exit the program
    return 0; 
}


/*  Function Name: printMesssage
    Input to the method: num = which time we are going through
    state = whether we are playing or done
    gameGroupID = which game group this is 
    Output(Return value): none
    Brief description of the task: this will display some messages needed.
    The reason we have it is to keep the above code clean                        
*/ 
void printMesssage(int state, int gameGroup){
    int gameGroupID = gameGroup+1; 
    // state 1 = playing game, state 0 = done playing 
    if(state){
        switch(gameGroupID-1){
            case 0:
            case 1:

                switch(gameNumCounter[gameGroupID-1]){
                    case 0:
                        printf("Group %d: We are playing our first game of Backgammon\n", gameGroupID);
                        break; 
                    case 1:
                        printf("Group %d: We are playing our second game of Backgammon\n", gameGroupID);
                        break; 
                    case 2:
                        printf("Group %d: We are playing our third game of Backgammon\n", gameGroupID);
                        break; 
                    case 3:
                        printf("Group %d: We are playing our forth game of Backgammon\n", gameGroupID);
                        break; 
                    case 4: 
                        printf("Group %d: We are playing our fifth game of Backgammon\n", gameGroupID);
                        break; 
                }
                break; 
            case 2:
            case 3:
                switch(gameNumCounter[gameGroupID-1]){
                    case 0:
                        printf("Group %d: We are playing our first game of Risk\n", gameGroupID);
                        break; 
                    case 1:
                        printf("Group %d: We are playing our second game of Risk\n", gameGroupID);
                        break; 
                    case 2:
                        printf("Group %d: We are playing our third game of Risk\n", gameGroupID);
                        break; 
                    case 3:
                        printf("Group %d: We are playing our forth game of Risk\n", gameGroupID);
                        break; 
                    case 4: 
                        printf("Group %d: We are playing our fifth game of Risk\n", gameGroupID);
                        break; 
                    
                }    
                break; 
            case 4:
            case 5:
                switch(gameNumCounter[gameGroupID-1]){
                    case 0:
                        printf("Group %d: We are playing our first game of Monopoly\n", gameGroupID);
                        break; 
                    case 1:
                        printf("Group %d: We are playing our second game of Monopoly\n", gameGroupID);
                        break; 
                    case 2:
                        printf("Group %d: We are playing our third game of Monopoly\n", gameGroupID);
                        break; 
                    case 3:
                        printf("Group %d: We are playing our forth game of Monopoly\n", gameGroupID);
                        break; 
                    case 4: 
                        printf("Group %d: We are playing our fifth game of Monopoly\n", gameGroupID);
                        break; 

                }
                break; 
            case 6:
            case 7:
                switch(gameNumCounter[gameGroupID-1]){
                    case 0:
                        printf("Group %d: We are playing our first game of Pictionary\n", gameGroupID);
                        break; 
                    case 1:
                        printf("Group %d: We are playing our second game of Pictionary\n", gameGroupID);
                        break; 
                    case 2:
                        printf("Group %d: We are playing our third game of Pictionary\n", gameGroupID);
                        break; 
                    case 3:
                        printf("Group %d: We are playing our forth game of Pictionary\n", gameGroupID);
                        break; 
                    case 4: 
                        printf("Group %d: We are playing our fifth game of Pictionary\n", gameGroupID);
                        break; 
                }
                break;
        }
    } else {
        switch(gameGroupID-1){
            case 0:
            case 1:
                switch(gameNumCounter[gameGroupID-1]){
                    case 0:
                        printf("Group %d: We are done playing our first game of Backgammon\nReturning 4 dice.\n", gameGroupID);
                        break; 
                    case 1:
                        printf("Group %d: We are done playing our second game of Backgammon\nReturning 4 dice.\n", gameGroupID);
                        break; 
                    case 2:
                        printf("Group %d: We are done playing our third game of Backgammon\nReturning 4 dice.\n", gameGroupID);
                        break; 
                    case 3:
                        printf("Group %d: We are done playing our forth game of Backgammon\nReturning 4 dice.\n", gameGroupID);
                        break; 
                    case 4: 
                        printf("Group %d: We are done playing our fifth game of Backgammon\nReturning 4 dice.\n", gameGroupID);

                }
                break; 
            case 2:
            case 3:

                switch(gameNumCounter[gameGroupID-1]){
                    case 0:
                        printf("Group %d: We are done playing our first game of Risk\nReturning 5 dice.\n", gameGroupID);
                        break; 
                    case 1:
                        printf("Group %d: We are done playing our second game of Risk\nReturning 5 dice.\n", gameGroupID);
                        break; 
                    case 2:
                        printf("Group %d: We are done playing our third game of Risk\nReturning 5 dice.\n", gameGroupID);
                        break; 
                    case 3:
                        printf("Group %d: We are done playing our forth game of Risk\nReturning 5 dice.\n", gameGroupID);
                        break; 
                    case 4: 
                        printf("Group %d: We are done playing our fifth game of Risk\nReturning 5 dice.\n", gameGroupID);
                    
               }
                break; 
            case 4:
            case 5:
                switch(gameNumCounter[gameGroupID-1]){
                    case 0:
                        printf("Group %d: We are done playing our first game of Monopoly\nReturning 2 dice.\n", gameGroupID);
                        break; 
                    case 1:
                        printf("Group %d: We are done playing our second game of Monopoly\nReturning 2 dice.\n", gameGroupID);
                        break; 
                    case 2:
                        printf("Group %d: We are done playing our third game of Monopoly\nReturning 2 dice.\n", gameGroupID);
                        break; 
                    case 3:
                        printf("Group %d: We are done playing our forth game of Monopoly\nReturning 2 dice.\n", gameGroupID);
                        break; 
                    case 4: 
                        printf("Group %d: We are done playing our fifth game of Monopoly\nReturning 2 dice.\n", gameGroupID);
                }
                break; 
            case 6:
            case 7:
                switch(gameNumCounter[gameGroupID-1]){
                    case 0:
                        printf("Group %d: We are done playing our first game of Pictionary\nReturning 1 die.\n", gameGroupID);
                        break; 
                    case 1:
                        printf("Group %d: We are done playing our second game of Pictionary\nReturning 1 die.\n", gameGroupID);
                        break; 
                    case 2:
                        printf("Group %d: We are done playing our third game of Pictionary\nReturning 1 die.\n", gameGroupID);
                        break; 
                    case 3:
                        printf("Group %d: We are done playing our forth game of Pictionary\nReturning 1 die.\n", gameGroupID);
                        break; 
                    case 4: 
                        printf("Group %d: We are done playing our fifth game of Pictionary\nReturning 1 die.\n", gameGroupID);
                }
                break; 
        }
    }
}
