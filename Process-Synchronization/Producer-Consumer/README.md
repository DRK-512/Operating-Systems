# Producer Consumer Problem
The producer-consumer problem in C is one of the most famous problems associated with operating systems.

In the producer-consumer problem in C, there is a producer which produces data and there is a consumer that consumes the data produced by the producer. Now, in the producer-consumer problem in C, there is a buffer. A buffer is a temporary storage area in the memory that stores data. The buffer in the Producer-Consumer Problem in C contains the produced items by the producer. The consumer consumes the products from the same buffer. In simpler terms, we can say that the buffer is a shared space between the producer and consumer.

Here are the roles of each thread: 
- Producer: Produces data to be put into the buffer
- Consumer: Consumes the data produced by the producer, and that is stored in the buffer

In our example we have 1 Producer and 3 Consumers
- The thing is, each consumer consumes a different amount of data
