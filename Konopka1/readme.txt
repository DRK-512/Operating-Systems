/* Authors names : Darek Konopka & Tejas Bolar
   Assignment: 1
   Date of Submission: 10/20/2021
*/

To compile the code use following command:
    gcc 5ps.c parser.c -o 5ps

How it is used:
    ./5ps [options]

Options:
    -p <pid>		Display information about the given process id.
    -s			Display the processes current state.
    -t			Display the time the process has consumed.
    -v			Display the amount of virtual memory used by the process.
    -c			Display the program that spawned the process.

Notes:
    - All features described in WritingYourOwnPSWithOptions.pdf have been implimented
