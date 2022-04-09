1. Group 3: Michael Lembck, Dongze Li, Meilin Liu
2. In this group project, we solved a shared stair problem in a department store imitating multi-thread conditions using semaphores. Special attention was paid to increase program efficiency and deadlocks.

The question we were asked to solve was:

A department store consists of two floors that are connected by narrow stairs that cannot fit more than one person on any given stair. The stairs can become deadlocked if both a first-floor customer is trying to go up to the second floor, while a second-floor customer is trying to go down to the first floor. Customers in such situation refuse to backup leading to deadlock. 
The store manager is hiring you to design a C program using POSIX synchronization, that 
1.	prevents deadlock
2.	prevents starvation (the situation in which first-floor customers prevent second-floor customers from using the stairs, or vice versa)
3.	and allows more than one customer to use the stairs in the same direction in an “efficient” way that you determine.

•	Represent first-floor customers and second-floor customers as separate threads. Once a customer is on the stairs, the associated thread will sleep for a random period of time, representing the time to go over all the 13 stairs between the two floors.
•	Design your program so that you can create several threads representing first-floor customers and second-floor customers, and test it using different possible scenarios.
•	Find the Turnaroud time and Response time for each customer


We had a basic version with 1 mutex, and an improved version with pthread_condition, and tried to implement a buffered queue to batch process customers 


