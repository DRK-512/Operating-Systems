# Operating-Systems

- Implement an elavator controller for a building with F floors
- There should a thread for each rider and for each elavator
- The rider threads use functions built into the the Building rider interface to call elavator: 
	- CallUp
	- CallDown
- Riders use the elavator rider interface to ride the elavator: 
	- Enter
	- Exit
	- RequestFloor
- These methods behave in a way that is familiar in the real world. CallUp and CallDown request an elavator for travel in a particular direction, then wait for a called elavator to arrive for travel in the requested direction.
- Each elavator runs as a separate thread looping within the elavator class, calling internal elavator control methods (VisitFloor, OpenDoors, CloseDoors) in the obvious sequence to serve riders in an orderly fashion.

- Credit for this project can be found [here](https://courses.cs.duke.edu/spring14/compsci310/projects/lab3/lab3.pdf)

- Basically, there are some people waiting for a random elavator, and they want to go to another random floor. If the floor is above, they call a function called CallUp and same for down. In order to ride the elavator, a person must first request a floor, then enter the elavator, then exit the elavator. This sounds similar to that of the sleeping barbers problem  

- I need to come back to this readme when I finish, and write it out better