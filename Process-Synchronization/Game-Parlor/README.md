# The Game Parlor Problem
## Project Description
- Eight groups of customers come in to a game parlor to play games
- Each group comes into the parlor wanting to play their favorite game where each game involves a different number of dice
- Each group has a representative that goes to the front desk to obtain one or more dice depending on which game they are playing
- The dice are used by the corresponding group while they are playing their game, and then returned to the front desk
- The front desk is in charge of lending out the dice and collecting them after each game is finished
- The gaming parlor owns only 8 dice, which are available at the front desk before the first group comes in 
- No group knows how many dice there are and they are not allowed to change the total number of dice at the front desk. 
- Only the front desk can assign dice and reclaim dice and thus change the number of dice.
- Each group is assigned to play one of the following games: 
	- Backgammon (4 dice)
	- Risk (5 dice)
	- Monopoly (2 dice)
	- Pictionary (1 die)

Here are the list of games played by groups 1 to 8
Note that each game is played by two groups.

| Group Number | Game Played |
| :------:     | :-----:     |
| 1 		   | Backgammon  |
| 2 		   | Backgammon  |
| 3 		   |    Risk     |
| 4 		   |    Risk     |
| 5 		   |   Monopoly  |
| 6 		   |   Monopoly  |
| 7 		   | Pictionary  |
| 8 		   | Pictionary  |

- The number of rounds every group is allowed to play must be adjustable 
- After each game, the group must return its dice and ask for them again

### Example 
For example, group 7 plays Pictionary will play it 5 times. Pictionary needs 1 die. Therefore group 7 will ask for 1 die and play one game of Pictionary for anywhere between 0 to 10 seconds. They then will return the die, and ask for it again. This request-return occurs 5 times by group 7 and then group 7 leaves the parlor. 

Each representative will request the corresponding number of dice, and, after using the dice for
the specified period of time, will return the number of dice that they borrowed.

This project idea came from [here](http://web.cecs.pdx.edu/~harry/Blitz/version-1-0/OSProject/p3/proj3.pdf)
